/*
 * Software Timer Library
 *
 * @file
 *   sw_timer.h
 *
 * @date
 *   2021-07-05
 *
 * @par
 *   Copyright 2021 Frequencer Team
 *
 * @brief
 *   Provides simple configurable timers backed by the CPU core timer.
 *   sw_timer_t values are in milliseconds, and ns_timer_t values are in
 *   nanoseconds. Values of internal members _
 */

#ifndef SW_TIMER_H
#define SW_TIMER_H


#include <cp0defs.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include "../config/default/peripheral/coretimer/plib_coretimer.h"


#define SW_TIMER(LEN) ((sw_timer_t){ 0, LEN, 0 })

#define SWT_COUNTS_MS (CORE_TIMER_FREQUENCY / 1000)
#define SWT_COUNTS_NS (CORE_TIMER_FREQUENCY / 1000)
#define SWT_ROLLOVER_MS (UINT32_MAX / SWT_COUNTS_MS)

#define __delay_ms CORETIMER_DelayMs
#define __delay_us CORETIMER_DelayUs


#ifdef __cplusplus
extern "C"
{
#endif


typedef struct
{
	uint32_t _start;  // time timer was reset at - do not set manually
	uint32_t _pause;  // time timer was paused at - do not set manually
	uint32_t length;  // period in ms, 0 is convention for use as capture
	bool running;  // Delta is between start and now if true, else between start and pause
}
sw_timer_t;

typedef sw_timer_t ns_timer_t;


// No initialization needed!
uint32_t sw_timer_elapsed (sw_timer_t* timer);
uint32_t ns_timer_elapsed (ns_timer_t* timer);


static inline uint32_t
sw_timer_time (void)
{
	return __builtin_mfc0(_CP0_COUNT, _CP0_COUNT_SELECT) / SWT_COUNTS_MS;
}

static inline uint32_t
ns_timer_time (void)
{
	return __builtin_mfc0(_CP0_COUNT, _CP0_COUNT_SELECT) / SWT_COUNTS_NS;
}


static inline bool
sw_timer_expired (sw_timer_t* timer)
{
	return sw_timer_elapsed(timer) > timer->length;
}

static inline void
sw_timer_reset (sw_timer_t* timer)
{
	timer->_start = sw_timer_time();
	timer->running = true;
}

static inline void
sw_timer_pause (sw_timer_t* timer)
{
	timer->_pause = sw_timer_time();
	timer->running = false;
}


static inline bool
ns_timer_expired (ns_timer_t* timer)
{
	return ns_timer_elapsed(timer) > timer->length;
}

static inline void
ns_timer_reset (ns_timer_t* timer)
{
	timer->_start = ns_timer_time();
	timer->running = true;
}

static inline void
ns_timer_pause (ns_timer_t* timer)
{
	timer->_pause = ns_timer_time();
	timer->running = false;
}


#ifdef __cplusplus
}
#endif

#endif /* SW_TIMER_H */
