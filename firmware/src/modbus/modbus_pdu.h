/*
 * Modbus Function Driver
 *
 * @file
 *   modbus_pdu.h
 *
 * @date
 *   2021-06-23
 *
 * @par
 *   Copyright 2021 Frequencer Team
 *
 * @brief
 *   Modbus per-function request decode and reply encode methods.
 */

#include <stdint.h>


#ifndef MODBUS_PDU_H
#define MODBUS_PDU_H


#define MODBUS_REGS_MULTI_MAX (123U)


#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	uint16_t address;
	uint16_t count;
	uint16_t data[MODBUS_REGS_MULTI_MAX];
}
pdu_reg_data_t;


bool pdu_parse_read_regs (modbus_pdu_t* pdu, pdu_reg_data_t* reg_data);
bool pdu_parse_write_regs (modbus_pdu_t* pdu, pdu_reg_data_t* reg_data);

void pdu_reply_none (modbus_pdu_t* pdu);
void pdu_reply_exception (modbus_pdu_t* pdu, modbus_exception_t exception);
void pdu_reply_read_regs (modbus_pdu_t* pdu, pdu_reg_data_t* reg_data);
void pdu_reply_write_regs (modbus_pdu_t* pdu, pdu_reg_data_t* reg_data);


#ifdef __cplusplus
}
#endif

#endif /* MODBUS_PDU_H */
