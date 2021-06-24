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


#include "modbus_defs.h"


#define MODBUS_MAX_REG_HANDLERS (3U)


#ifdef  __cplusplus
extern "C" {
#endif


typedef enum
{
	MB_RA_WRITE,
	MB_RA_READ,
}
mb_reg_action_t;


typedef struct
{
	uint16_t address;
	uint16_t count;
	uint16_t data[MODBUS_REGS_MULTI_MAX];
}
mb_reg_data_t;

typedef bool (* mb_reg_handler_t)(mb_reg_data_t*);

typedef struct
{
	uint16_t address;
	uint16_t count;
	mb_reg_handler_t handler;
}
mb_handled_regs_t;


void modbus_init (void);
void modbus_task (void);

void modbus_add_reg_handler (
	uint16_t address,
	uint16_t count,
	mb_reg_action_t action,
	mb_reg_handler_t handler
);


#ifdef  __cplusplus
}
#endif

#endif /* CON_MODBUS_H */
