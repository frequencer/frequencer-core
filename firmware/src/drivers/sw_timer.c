/*
 * Software Timer Library
 *
 * @file
 *   sw_timer.c
 *
 * @date
 *   2021-07-05
 *
 * @par
 *   Copyright 2021 Frequencer Team
 *
 * @brief
 *   Provides simple configurable timers backed by the CPU core timer.
 *   All values are always in milliseconds.
 */

#include <stdbool.h>
#include <stdint.h>

#include "sw_timer.h"


uint32_t
sw_timer_elapsed (sw_timer_t* timer)
{
	// On the PIC32MZ this is atomic
	uint32_t local_ticks = sw_timer_time();

	if (timer->pause > 0)
	{
		local_ticks = timer->pause;
	}

	if (local_ticks >= timer->start)
	{
		return local_ticks - timer->start;
	}
	else
	{
		return SWT_ROLLOVER_MS - timer->start - local_ticks;
	}
}

bool
sw_timer_expired (sw_timer_t* timer)
{
	return sw_timer_elapsed(timer) > timer->length;
}

void
sw_timer_reset (sw_timer_t* timer)
{
	timer->start = sw_timer_time();
	timer->pause = 0;
}

void
sw_timer_pause (sw_timer_t* timer)
{
	// On the PIC32MZ this is atomic
	uint32_t local_ticks = sw_timer_time();

	if (0 == local_ticks)
	{
		// Small chance of being off by 1ms... it's fine
		local_ticks++;
	}

	timer->pause = local_ticks;
}
