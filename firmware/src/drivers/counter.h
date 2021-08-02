/*
 * Frequency Counter
 *
 * @file
 *   counter.h
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

#ifndef COUNTER_H
#define COUNTER_H


#define PR1_MIN (1U)
#define PR1_MAX (65535U)

#define TIMEOUT_MIN (100U)  // ms
#define TIMEOUT_MAX (4000U)  // ms


#ifdef __cplusplus
extern "C" {
#endif


void counter_init (void);
void counter_task (void);

double counter_freq_hz (void);  // return of zero is no frequency available


#ifdef __cplusplus
}
#endif

#endif /* COUNTER_H */
