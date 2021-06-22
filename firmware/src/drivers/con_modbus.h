/*
 * Modbus ASCII Driver
 *
 * @file
 *   con_modbus.h
 *
 * @date
 *   2021-06-21
 *
 * @par
 *   Copyright 2021 Frequencer Team
 *
 * @brief
 *   Modbus ASCII packet parser and register manager, backed by Harmony console.
 */

#ifndef CON_MODBUS_H
#define	CON_MODBUS_H


#define CMB_MULTI_MAX (123U)

#define CMB_FRAME_MAX (252U)


#ifdef	__cplusplus
extern "C" {
#endif


void cmb_init (void);
void cmb_task (void);


#ifdef	__cplusplus
}
#endif

#endif  /* CON_MODBUS_H */
