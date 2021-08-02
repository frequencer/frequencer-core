/*
 * Frequency Counter
 *
 * @file
 *   counter.c
 *
 * @date
 *   2021-07-05
 *
 * @par
 *   Copyright 2021 Frequencer Team
 *
 * @brief
 *   Automatic wide-range frequency counter for the signal on T1CK.
 */

#include <definitions.h>
#include <math.h>
#include <stdio.h>
#include "hang_here.h"
#include "sw_timer.h"

#include "counter.h"

#define FH_LEN (5000U)


typedef enum
{
	CS_INIT = 0,
	CS_WAIT_START,
	CS_WAIT_END,
	CS_CALC,
	CS_TIMED_OUT,
}
state_t;


static sw_timer_t timeout, dbg_timer;
static uint32_t ct_start, ct_stop, timeout_tmr1;
static double frequency;
static double freq_history[FH_LEN];
static unsigned int fh_index, n_avg, n_cur;
static state_t state;


void
counter_init (void)
{
	timeout = SW_TIMER(100);
	dbg_timer = SW_TIMER(1000);
	sw_timer_reset(&dbg_timer);

	ct_start = 0;
	ct_stop = 0;
	frequency = 0.0;
	state = CS_INIT;
	fh_index = 0;
	n_avg = 1;
	n_cur = 0;

	PMD4bits.T1MD = 0;
	T1CONbits.ON = 0;
	_nop();

	T1CONbits.SIDL = 0;
	T1CONbits.TWDIS = 1;
	T1CONbits.TGATE = 0;
	T1CONbits.TCKPS = 0b00;
	T1CONbits.TSYNC = 0;
	T1CONbits.TCS = 1;
	TMR1 = 0;

	IPC1bits.T1IP = 7;
	IPC1bits.T1IS = 0;
	IFS0bits.T1IF = 0;
	IEC0bits.T1IE = 0;

	T1CONbits.ON = 1;
}

static unsigned int
timer1_prescale (void)
{
	switch (T1CONbits.TCKPS)
	{
		case 0b00:
		{
			return 1;
		}

		/*
		 * case 0b01:
		 * {
		 *  return 8;
		 * }
		 *
		 * case 0b10:
		 * {
		 *  return 64;
		 * }
		 *
		 * case 0b11:
		 * {
		 *  return 256;
		 * }
		 */

		default:
		{
			HANG_HERE();
		}
	}
}

static inline float
timeout_progress (void)
{
	// 0.0 to 1.0 (0-100%)
	return (float)(sw_timer_elapsed(&timeout)) / (float)(timeout.length);
}

static void
debug_report (bool now)
{
	// Print periodic message detailing current settings and frequency measured.
	if (sw_timer_expired(&dbg_timer) || now)
	{
		int dec_digits = 1;

		if (frequency > 0.0)
		{
			dec_digits = 8 - (int)floor(log10(frequency));

			if (dec_digits < 0)
			{
				dec_digits = 0;
			}

			if (dec_digits > 8)
			{
				dec_digits = 8;
			}
		}

		printf(
			"Counter: %10.*f Hz, PR1 = %5d, TO = %4dms, N = %2d\n",
			dec_digits,
			frequency,
			PR1,
			timeout.length,
			n_avg
		);
		sw_timer_reset(&dbg_timer);
	}
}

static void
update_frequency (void)
{
	// Use current timestamps and timer settings to calculate input signal
	// frequency and update module data.

	double edges = (double)(PR1 * timer1_prescale());
	double time = ns_time_delta(ct_start, ct_stop) / 1000000000.0;

	// There seem to be large inaccuracies in the math if this
	// operation is performed when declaring/assigning the variable.
	edges += 1.0;
	frequency = (edges * (double)n_avg) / time;

	freq_history[fh_index] = frequency;
	fh_index++;

	if (fh_index >= FH_LEN)
	{
		fh_index = 0;
	}

	debug_report(false);
}

static void
handle_interval_too_short (void)
{
	// Adjust timer settings to account for timer events happening too often.
	if (timeout.length > TIMEOUT_MIN)
	{
		// First stage - reduce timeout.
		uint32_t timeout_new = timeout.length / 2;

		if (timeout_new < TIMEOUT_MIN)
		{
			timeout_new = TIMEOUT_MIN;
		}

		timeout.length = timeout_new;
	}
	else if (PR1 < PR1_MAX)
	{
		// Second stage - increase timer overflow setpoint by timeout ratio.
		uint32_t pr1_new = PR1 * (1.0 / timeout_progress());

		if (pr1_new > PR1_MAX)
		{
			pr1_new = PR1_MAX;
		}

		PR1 = pr1_new;
	}
	else
	{
		// Third stage - double averaging count.
		n_avg *= 2;
	}

	// There's no signal too fast!
	// TODO: Monitor die temperature.
}

static void
handle_interval_too_long (void)
{
	// Adjust timer settings to account for timer events being too infrequent.
	if (n_avg > 1)
	{
		// First stage - reduce averaging count by half.
		n_avg /= 2;
	}
	else if (PR1 > PR1_MIN)
	{
		// Second stage - reduce timer overflow setpoint by timer fill ratio.
		// Reduced further by 5% for hysteresis.
		float tmr_progress = (float)timeout_tmr1 / (float)PR1;
		uint32_t pr1_new = (uint32_t)floor((float)PR1 * tmr_progress * 0.95);

		if (pr1_new < PR1_MIN)
		{
			pr1_new = PR1_MIN;
		}

		if (pr1_new > PR1_MAX)
		{
			pr1_new = PR1_MAX;
		}

		if (pr1_new != PR1)
		{
			PR1 = pr1_new;
			TMR1 = 0;
		}
	}
	else if (timeout.length < TIMEOUT_MAX)
	{
		// Third stage - extend timeout.
		uint32_t timeout_new = timeout.length * 3;

		if (timeout_new > TIMEOUT_MAX)
		{
			timeout_new = TIMEOUT_MAX;
		}

		timeout.length = timeout_new;
	}
	else
	{
		// Signal not present or less than 1Hz - can't measure.
		frequency = 0.0;
		printf("Counter: No signal detected.\n");
	}
}

void
counter_task (void)
{
	switch (state)
	{
		case CS_CALC:
		{
			sw_timer_pause(&timeout);

			if (timeout_progress() > 0.45)
			{
				// Timestamp of first and nth timer overflow event established.
				// Calculate frequency from time delta and edge count.
				update_frequency();
			}
			else
			{
				// Time between overflow events is too short. The calculation
				// will be inaccurate. Adjust timer settings to compensate.
				handle_interval_too_short();
				printf("Counter: Interval too short (%.0f%%).\n", timeout_progress() * 100.0);
				debug_report(true);
			}

			// Fallthrough to CS_INIT to start next measurement.
			state = CS_INIT;
		}

		case CS_INIT:
		{
			// Initiate counter measurement. This enables the timer interrupt to
			// capture the time of the first overflow event.
			sw_timer_reset(&timeout);
			state = CS_WAIT_START;
			IFS0bits.T1IF = 0;
			IEC0bits.T1IE = 1;

			break;
		}

		case CS_WAIT_START:
		case CS_WAIT_END:
		{
			if (!sw_timer_expired(&timeout))
			{
				// Wait for interrupt to advance state. After first overflow
				// event is captured, start time is established. After nth
				// overflow event is captured, end time and delta are
				// established and frequency can be calculated. The next state
				// is set by the interrupt.
				break;
			}
			else
			{
				// An overflow event did not occur within the timeout period.
				// Save TMR1 value to prevent race condition during PR1 adjust.
				// Fallthrough to CS_TIMED_OUT to adjust settings.
				timeout_tmr1 = TMR1;
				state = CS_TIMED_OUT;
			}
		}

		case CS_TIMED_OUT:
		{
			// Timed out before the input signal caused a timer overflow. Adjust
			// settings to compensate for slower input signal.
			printf("Counter: Interval too long (%d/%d, %d/%d).\n", TMR1, PR1, n_cur, n_avg);
			handle_interval_too_long();
			state = CS_INIT;
			debug_report(true);

			break;
		}

		default:
		{
			HANG_HERE();
		}
	}

	// printf("Counter: State %d, progress %.0f%%.\n", state, 100.0 * timeout_progress());
}


double
counter_freq_hz (void)
{
	return frequency;
}

static uint32_t ticks_local;

void
__ISR (_TIMER_1_VECTOR, ipl7SRS) counter_timer1_isr(void)
{
	__builtin_disable_interrupts();
	LED3_Set();
	ticks_local = __builtin_mfc0(_CP0_COUNT, _CP0_COUNT_SELECT);
	LED3_Clear();
	__builtin_enable_interrupts();

	IFS0bits.T1IF = 0;

	switch (state)
	{
		case CS_WAIT_START:
		{
			ct_start = ticks_local;
			n_cur = 0;
			sw_timer_reset(&timeout);
			state = CS_WAIT_END;

			break;
		}

		case CS_WAIT_END:
		{
			n_cur++;

			if (n_cur >= n_avg)
			{
				n_cur = 0;
				ct_stop = ticks_local;
				IEC0bits.T1IE = 0;
				state = CS_CALC;
			}

			break;
		}

		default:
		{
			IEC0bits.T1IE = 0;
		}
	}

	LED3_Set();
	LED3_Clear();
}
