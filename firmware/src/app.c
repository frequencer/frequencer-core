/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
	Microchip Technology Inc.

  File Name:
	app.c

  Summary:
	This file contains the source code for the MPLAB Harmony application.

  Description:
	Wow, it's the main file
 *******************************************************************************/

#include "definitions.h"
#include "drivers/hang_here.h"

#include "app.h"


/// Definitions

static app_states_t state;
static app_states_t next_state;
static volatile app_states_t isr_state;

static SYS_TIME_HANDLE led_timer_handle = SYS_TIME_HANDLE_INVALID;
static SYS_CONSOLE_HANDLE console_handle = SYS_CONSOLE_HANDLE_INVALID;

static unsigned int message_counter;


void sw1_callback (GPIO_PIN pin, uintptr_t context);
void led_timer_callback (uintptr_t context);


/// Main Functions

// Initialize application. Call once before calling app_task.
void app_init (void)
{
	LED2_Set();

	state = APPS_INIT;
	next_state = APPS_NONE;
	isr_state = APPS_NONE;

	message_counter = 0;

	bool result = GPIO_PinInterruptCallbackRegister(
		SW1_PIN,
		sw1_callback,
		NULL
	);
	SW1_InterruptEnable();

	if (false == result)
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
}

// Main app task. Call as often as possible.
void app_task (void)
{
	switch (state)
	{
		case APPS_INIT:
		{
			bool appInitialized = true;

			if (appInitialized)
			{
				LED2_Clear();
				next_state = APPS_IDLE;
			}
			else
			{
				next_state = APPS_INIT;
			}

			break;
		}

		case APPS_IDLE:
		{
			next_state = APPS_IDLE;

			break;
		}

		case APPS_SEND_MESSAGE:
		{
			SYS_CONSOLE_Print(console_handle, "Hello World %d\r\n", message_counter);

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
		isr_state = APPS_SEND_MESSAGE;
	}
}

// LED timer callback. Toggles LED1.
void
led_timer_callback (uintptr_t context)
{
	LED1_Toggle();
}
