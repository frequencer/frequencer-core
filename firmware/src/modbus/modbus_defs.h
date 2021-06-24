/*
 * Modbus Protocol Definitions
 *
 * @file
 *   modbus_defs.h
 *
 * @date
 *   2021-06-23
 *
 * @par
 *   Copyright 2021 Frequencer Team
 *
 * @brief
 *   General Modbus definitions not relating to a specific frame format.
 */

#ifndef MODBUS_DEFS_H
#define MODBUS_DEFS_H


#define MODBUS_ADDRESS (0U)

#define MODBUS_EXCEPTION_OFFSET (0x80U)


#ifdef  __cplusplus
extern "C" {
#endif


typedef enum
{
	MB_FN_NONE = 0x00,
	MB_FN_READ_COILS = 0x01,
	MB_FN_READ_INPUTS = 0x02,
	MB_FN_READ_REGS = 0x03,
	MB_FN_READ_IREGS = 0x04,
	MB_FN_WRITE_COIL = 0x05,
	MB_FN_WRITE_REG = 0x06,
	MB_FN_READ_ESTAT = 0x07,
	MB_FN_DIAGNOSTICS = 0x08,
	MB_FN_GET_CE_CTR = 0x0B,
	MB_FN_GET_CE_LOG = 0x0C,
	MB_FN_WRITE_COILS = 0x0F,
	MB_FN_WRITE_REGS = 0x10,
	MB_FN_REPORT_SERVER_ID = 0x11,
	MB_FN_READ_FILE = 0x14,
	MB_FN_WRITE_FILE = 0x15,
	MB_FN_MASK_WRITE_REG = 0x16,
	MB_FN_RW_MULTI_REG = 0x17,
	MB_FN_READ_FIFO = 0x18,
	MB_FN_EIT = 0x2B,
}
modbus_function_t;

typedef enum
{
	MB_EX_NONE = 0x00,
	MB_EX_ILLEGAL_FN = 0x01,
	MB_EX_ILLEGAL_ADDR = 0x02,
	MB_EX_ILLEGAL_VALUE = 0x03,
	MB_EX_DEVICE_FAILURE = 0x04,
	MB_EX_ACKNOWLEDGE = 0x05,
	MB_EX_DEVICE_BUSY = 0x06,
	MB_EX_PARITY_ERROR = 0x08,
	MB_EX_GATEWAY_PATH = 0x0A,
	MB_EX_GATEWAY_TARGET = 0x0B,
}
modbus_exception_t;


typedef struct
{
	uint8_t* data;
	unsigned int length;
}
modbus_pdu_t;


extern void mb_debug (const char* fmt, ...);


#ifdef  __cplusplus
}
#endif

#endif /* MODBUS_DEFS_H */
