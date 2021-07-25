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

	if (!(timer->running))
	{
		local_ticks = timer->_pause;
	}

	if (local_ticks >= timer->_start)
	{
		return local_ticks - timer->_start;
	}
	else
	{
		return SWT_ROLLOVER_MS - timer->_start - local_ticks;
	}
}

double
ns_timer_elapsed (ns_timer_t* timer)
{
	// On the PIC32MZ this is atomic
	uint32_t local_ticks = sw_timer_ticks();

	if (!(timer->running))
	{
		local_ticks = timer->_pause;
	}

	return ns_time_delta(timer->_start, local_ticks);
}

double
ns_time_delta (uint32_t ticks_start, uint32_t ticks_end)
{
	uint32_t delta_ticks;

	if (ticks_end >= ticks_start)
	{
		delta_ticks = ticks_end - ticks_start;
	}
	else
	{
		delta_ticks = UINT32_MAX - ticks_start - ticks_end;
	}

	return delta_ticks / SWT_COUNTS_NS;
}
