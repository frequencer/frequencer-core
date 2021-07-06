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

uint32_t
ns_timer_elapsed (ns_timer_t* timer)
{
	// On the PIC32MZ this is atomic
	uint32_t local_ticks = ns_timer_time();

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
		return UINT32_MAX - timer->_start - local_ticks;
	}
}
