/*******************************************************************************
*  MPLAB Harmony Application Source File
*
*  Company:
*   Microchip Technology Inc.
*
*  File Name:
*   app.c
*
*  Summary:
*   This file contains the source code for the MPLAB Harmony application.
*
*  Description:
*   Wow, it's the main file
*******************************************************************************/

#include "definitions.h"
#include "drivers/hang_here.h"
#include "drivers/zl30159.h"

#include "app.h"


/// Definitions

static app_states_t state;
static app_states_t next_state;
static volatile app_states_t isr_state;

static SYS_TIME_HANDLE led_timer_handle = SYS_TIME_HANDLE_INVALID;
static SYS_CONSOLE_HANDLE console_handle = SYS_CONSOLE_HANDLE_INVALID;

static unsigned int message_counter;


void sw1_callback (GPIO_PIN pin, uintptr_t context);
void sw2_callback (GPIO_PIN pin, uintptr_t context);
void led_timer_callback (uintptr_t context);


/// Main Functions

// Initialize application. Call once before calling app_task.
void
app_init (void)
{
	LED2_Set();

	state = APPS_INIT;
	next_state = APPS_NONE;
	isr_state = APPS_NONE;

	message_counter = 0;

	bool gpio_result = GPIO_PinInterruptCallbackRegister(
		SW1_PIN,
		sw1_callback,
		NULL
	);

	SW1_InterruptEnable();

	gpio_result &= GPIO_PinInterruptCallbackRegister(
		SW2_PIN,
		sw2_callback,
		NULL
	);
	SW2_InterruptEnable();

	if (false == gpio_result)
	{
		HANG_HERE();
	}

	led_timer_handle = SYS_TIME_TimerCreate(
		0,
		SYS_TIME_MSToCount(500),
		led_timer_callback,
		NULL,
		SYS_TIME_PERIODIC
	);

	if (SYS_TIME_HANDLE_INVALID == led_timer_handle)
	{
		HANG_HERE();
	}

	if (SYS_TIME_SUCCESS != SYS_TIME_TimerStart(led_timer_handle))
	{
		HANG_HERE();
	}

	console_handle = SYS_CONSOLE_HandleGet(SYS_CONSOLE_INDEX_0);

	if (SYS_CONSOLE_HANDLE_INVALID == console_handle)
	{
		HANG_HERE();
	}

	zl_init();
}

// Main app task. Call as often as possible.
void
app_task (void)
{
	switch (state)
	{
		case APPS_INIT:
		{
			SYS_STATUS console_status = SYS_CONSOLE_Status(sysObj.sysConsole0);

			if (SYS_STATUS_READY == console_status)
			{
				CPRINT("Press S2 to get started!\r\n")
				next_state = APPS_WAIT_USER_READY;
			}
			else if (SYS_STATUS_ERROR == console_status)
			{
				HANG_HERE();
			}
			else
			{
				next_state = APPS_INIT;
			}

			break;
		}

		case APPS_WAIT_USER_READY:
		{
			// Exit via SW2 ISR.
			next_state = APPS_WAIT_USER_READY;

			break;
		}

		case APPS_QUERY_PLL:
		{
			zl_value_t id_reg = zl_read_reg(ZL_REG_ID_REG);

			CPRINTF("PLL ID: 0x%02X (should be 0x89)\r\n", id_reg.u8);

			if (id_reg.id_reg.ready)
			{
				next_state = APPS_IDLE;
			}
			else
			{
				CPRINT("PLL not ready. Press S2 to retry.\r\n");
				next_state = APPS_WAIT_USER_READY;
			}

			break;
		}

		case APPS_IDLE:
		{
			// Exit via SW1 ISR.
			next_state = APPS_IDLE;

			break;
		}

		case APPS_SEND_MESSAGE:
		{
			CPRINTF("Hello World %d\r\n", message_counter);

			message_counter++;
			next_state = APPS_IDLE;

			break;
		}

		default:
		{
			HANG_HERE();
			break;
		}
	}

	if (APPS_NONE != isr_state)
	{
		state = isr_state;
		isr_state = APPS_NONE;
	}
	else
	{
		state = next_state;
		next_state = APPS_NONE;
	}
}


/// Callbacks

// SW1 pushbutton callback. Triggers hello world message.
void
sw1_callback (GPIO_PIN pin, uintptr_t context)
{
	if (SW1_PIN != pin)
	{
		HANG_HERE();
	}

	if (!SW1_Get())
	{
		if (APPS_IDLE == state)
		{
			isr_state = APPS_SEND_MESSAGE;
		}
	}
}

// SW2 pushbutton callback. Starts main app execution.
void
sw2_callback (GPIO_PIN pin, uintptr_t context)
{
	if (SW2_PIN != pin)
	{
		HANG_HERE();
	}

	if (!SW2_Get())
	{
		if (APPS_WAIT_USER_READY == state)
		{
			LED2_Clear();
			isr_state = APP_START_STATE;
		}
	}
}

// LED timer callback. Toggles LED1.
void
led_timer_callback (uintptr_t context)
{
	LED1_Toggle();
}
