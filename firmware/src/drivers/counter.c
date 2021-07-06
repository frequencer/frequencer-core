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
#include "sw_timer.h"

#include "counter.h"


static sw_timer_t timeout, count;
static double frequency;


void
counter_init (void)
{
	timeout = SW_TIMER(100);
	count = SW_TIMER(0);
	frequency = 0;
}

void
counter_task (void)
{

}


double
counter_freq_hz (void)
{
	return frequency;
}
