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
 *   This only implements a subset of functions, relating to reading/writing the
 *   holding registers.
 */

#ifndef CON_MODBUS_H
#define CON_MODBUS_H


#define CMB_MULTI_MAX (123U)

#define CMB_FRAME_MAX (252U)

#define CMB_ADDRESS (0U)

#define CMB_EXCEPTION_OFFSET (0x80U)


#ifdef  __cplusplus
extern "C" {
#endif


void cmb_init (void);
void cmb_task (void);


enum ascii_framing
{
	CMB_ASF_START = 0x3A,
	CMB_ASF_END_CR = 0x0D,
	CMB_ASF_END_LF = 0x0A,
};

typedef enum
{
	CMB_MF_NONE = 0x00,
	CMB_MF_READ_COILS = 0x01,
	CMB_MF_READ_INPUTS = 0x02,
	CMB_MF_READ_REGS = 0x03,
	CMB_MF_READ_IREGS = 0x04,
	CMB_MF_WRITE_COIL = 0x05,
	CMB_MF_WRITE_REG = 0x06,
	CMB_MF_READ_ESTAT = 0x07,
	CMB_MF_DIAGNOSTICS = 0x08,
	CMB_MF_GET_CE_CTR = 0x0B,
	CMB_MF_GET_CE_LOG = 0x0C,
	CMB_MF_WRITE_COILS = 0x0F,
	CMB_MF_WRITE_REGS = 0x10,
	CMB_MF_REPORT_SERVER_ID = 0x11,
	CMB_MF_READ_FILE = 0x14,
	CMB_MF_WRITE_FILE = 0x15,
	CMB_MF_MASK_WRITE_REG = 0x16,
	CMB_MF_RW_MULTI_REG = 0x17,
	CMB_MF_READ_FIFO = 0x18,
	CMB_MF_EIT = 0x2B,
}
cmb_modbus_function_t;

typedef enum
{
	CMB_EX_NONE = 0x00,
	CMB_EX_ILLEGAL_FN = 0x01,
	CMB_EX_ILLEGAL_ADDR = 0x02,
	CMB_EX_ILLEGAL_VALUE = 0x03,
	CMB_EX_DEVICE_FAILURE = 0x04,
	CMB_EX_ACKNOWLEDGE = 0x05,
	CMB_EX_DEVICE_BUSY = 0x06,
	CMB_EX_PARITY_ERROR = 0x08,
	CMB_EX_GATEWAY_PATH = 0x0A,
	CMB_EX_GATEWAY_TARGET = 0x0B,
}
cmb_modbus_exception_t;


#ifdef  __cplusplus
}
#endif

#endif /* CON_MODBUS_H */
