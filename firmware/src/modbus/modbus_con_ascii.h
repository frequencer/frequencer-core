/*
 * Modbus LL Driver - ASCII/Harmony Console
 *
 * @file
 *   modbus_con_ascii.h
 *
 * @date
 *   2021-06-23
 *
 * @par
 *   Copyright 2021 Frequencer Team
 *
 * @brief
 *   Modbus ASCII frame/ADU interface. IO via Harmony Console.
 */

#ifndef MODBUS_CON_ASCII_H
#define MODBUS_CON_ASCII_H


#include "modbus_defs.h"


#define MCA_FRAME_MAX (252U)


#ifdef  __cplusplus
extern "C" {
#endif


enum ascii_framing
{
	CMB_ASF_START = 0x3A,
	CMB_ASF_END_CR = 0x0D,
	CMB_ASF_END_LF = 0x0A,
};


typedef modbus_pdu_t mca_adu_t;


void mca_init (void);
void mca_task (void);

modbus_pdu_t mca_parse_adu (void);
void mca_send_reply (modbus_pdu_t* pdu);

void mb_debug (const char* fmt, ...);


#ifdef  __cplusplus
}
#endif

#endif /* MODBUS_CON_ASCII_H */
