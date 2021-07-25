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
}
state_t;


static sw_timer_t timeout;
static uint32_t ct_start, ct_stop;
static double frequency;
static double freq_history[FH_LEN];
static unsigned int fh_index;
static state_t state;


void
counter_init (void)
{
	timeout = SW_TIMER(100);
	ct_start = 0;
	ct_stop = 0;
	frequency = 0;
	state = CS_INIT;
	fh_index = 0;

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

		case 0b01:
		{
			return 8;
		}

		case 0b10:
		{
			return 64;
		}

		case 0b11:
		{
			return 256;
		}

		default:
		{
			HANG_HERE();
		}
	}
}

void
counter_task (void)
{
	switch (state)
	{
		case CS_CALC:
		{
			double edges = (double)(PR1 * timer1_prescale());
			double time = ns_time_delta(ct_start, ct_stop) / 1000000000.0;

			// There seem to be large inaccuracies in the math if this
			// operation is performed when declaring/assigning the variable.
			edges += 1.0;
			frequency = edges / time;

			freq_history[fh_index] = frequency;
			fh_index++;

			if (fh_index >= FH_LEN)
			{
				fh_index = 0;
			}

			// Fallthrough to CS_INIT.
			state = CS_INIT;
		}

		case CS_INIT:
		{
			IFS0bits.T1IF = 0;
			IEC0bits.T1IE = 1;
			state = CS_WAIT_START;

			break;
		}

		case CS_WAIT_START:
		case CS_WAIT_END:
		{
			// Wait for interrupt to advance state.

			// TODO: Monitor timeout.

			break;
		}

		default:
		{
			HANG_HERE();
		}
	}
}


double
counter_freq_hz (void)
{
	return frequency;
}

void
__ISR (_TIMER_1_VECTOR, ipl7SRS) counter_timer1_isr(void)
{
	LED3_Toggle();
	uint32_t ticks_local = sw_timer_ticks();


	IFS0bits.T1IF = 0;
	LED3_Toggle();

	switch (state)
	{
		case CS_WAIT_START:
		{
			ct_start = ticks_local;
			state = CS_WAIT_END;

			break;
		}

		case CS_WAIT_END:
		{
			ct_stop = ticks_local;
			state = CS_CALC;

			IEC0bits.T1IE = 0;

			break;
		}

		default:
		{
			// Do nothing.
		}
	}

	LED3_Toggle();
}
