/*
 * Modbus Driver
 *
 * @file
 *   modbus.h
 *
 * @date
 *   2021-06-21
 *
 * @par
 *   Copyright 2021 Frequencer Team
 *
 * @brief
 *   Modbus ASCII driver, backed by Harmony console.
 *   This only implements a subset of functions, relating to reading/writing the
 *   holding registers.
 */

#ifndef CON_MODBUS_H
#define CON_MODBUS_H


#ifdef  __cplusplus
extern "C" {
#endif


void modbus_init (void);
void modbus_task (void);


#ifdef  __cplusplus
}
#endif

#endif /* CON_MODBUS_H */
