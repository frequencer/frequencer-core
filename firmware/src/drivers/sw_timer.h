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
 *   All values are always in milliseconds.
 */

#ifndef SW_TIMER_H
#define SW_TIMER_H


#include <cp0defs.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include "../config/default/peripheral/coretimer/plib_coretimer.h"


#define SW_TIMER(LEN) ((sw_timer_t){ 0, LEN, 0 })

#define SWT_COUNTS_1MS (CORE_TIMER_FREQUENCY / 1000)
#define SWT_ROLLOVER_MS (UINT32_MAX / SWT_COUNTS_1MS)

#define __delay_ms CORETIMER_DelayMs
#define __delay_us CORETIMER_DelayUs


#ifdef __cplusplus
extern "C"
{
#endif


typedef struct
{
	uint32_t start;  // do not set manually
	uint32_t length;  // period in ms, 0 is convention for use as capture
	uint32_t pause;  // 0 == not paused
}
sw_timer_t;


extern uint32_t g_ticks;


// No initialization needed!
uint32_t sw_timer_elapsed (sw_timer_t* timer);
bool sw_timer_expired (sw_timer_t* timer);
void sw_timer_reset (sw_timer_t* timer);
void sw_timer_pause (sw_timer_t* timer);


static inline uint32_t
sw_timer_time (void)
{
	return __builtin_mfc0(_CP0_COUNT, _CP0_COUNT_SELECT) / SWT_COUNTS_1MS;
}


#ifdef __cplusplus
}
#endif

#endif /* SW_TIMER_H */
