/*
 * Modbus Function Driver
 *
 * @file
 *   modbus_pdu.c
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

#include <stdbool.h>
#include <stdint.h>

#include "../drivers/hang_here.h"

#include "modbus_defs.h"
#include "modbus_pdu.h"


/// Functions to decompose function data into usable form.

bool
pdu_parse_read_regs (modbus_pdu_t* pdu, pdu_reg_data_t* reg_data)
{
	uint16_t address, count;

	if (pdu->length != 5)
	{
		mb_debug(
			"Ignoring Read Registers request with length %d, should be 5",
			pdu->length
		);

		return false;
	}

	address = (uint16_t)(pdu->data[2]);
	address |= ((uint16_t)(pdu->data[1]) << 8);
	count = (uint16_t)(pdu->data[4]);
	count |= ((uint16_t)(pdu->data[3]) << 8);

	reg_data->address = address;
	reg_data->count = count;

	return true;
}

bool
pdu_parse_write_regs (modbus_pdu_t* pdu, pdu_reg_data_t* reg_data)
{
	unsigned int i;
	uint8_t byte_count;
	uint16_t address, count;

	if (pdu->length < 6)
	{
		mb_debug(
			"Ignoring Write Registers request with length %d, should be at least 6",
			pdu->length
		);

		return false;
	}

	address = (uint16_t)(pdu->data[2]);
	address |= ((uint16_t)(pdu->data[1]) << 8);
	count = (uint16_t)(pdu->data[4]);
	count |= ((uint16_t)(pdu->data[3]) << 8);
	byte_count = pdu->data[5];

	if (byte_count != (count * 2))
	{
		mb_debug(
			(
				"Ignoring Write Registers request with count mismatch, reports "
				"%d registers but %d data bytes."
			),
			count,
			byte_count
		);

		return false;
	}

	if (byte_count != (pdu->length - 6))
	{
		mb_debug(
			(
				"Ignoring Write Registers request with length mismatch, reports "
				"%d data bytes but only %d received."
			),
			byte_count,
			(pdu->length - 6)
		);

		return false;
	}

	reg_data->address = address;
	reg_data->count = count;

	for (i = 0; i < count; i++)
	{
		reg_data->data[i] = (uint16_t)(pdu->data[7 + (i * 2)]);
		reg_data->data[i] |= ((uint16_t)(pdu->data[6 + (i * 2)]) << 8);
	}

	return true;
}


/// Functions to compose a reply. They do not send the reply.

void
pdu_reply_none (modbus_pdu_t* pdu)
{
	// Simply configures state such that no reply is sent.

	pdu->length = 0;
}

void
pdu_reply_exception (modbus_pdu_t* pdu, modbus_exception_t exception)
{
	// Must only be called when a valid frame is in the data buffer.

	pdu->data[0] += MODBUS_EXCEPTION_OFFSET;
	pdu->data[1] = exception;
	pdu->length = 2;
}

void
pdu_reply_read_regs (modbus_pdu_t* pdu, pdu_reg_data_t* reg_data)
{
	unsigned int i;
	uint8_t byte_count;

	if (reg_data->count > MODBUS_REGS_MULTI_MAX)
	{
		HANG_HERE();
	}

	byte_count = (uint8_t)(reg_data->count * 2);
	pdu->data[1] = byte_count;

	for (i = 0; i < reg_data->count; i++)
	{
		pdu->data[2 + i] = (uint8_t)(reg_data->data[i] >> 8);
		pdu->data[3 + i] = (uint8_t)(reg_data->data[i] & 0xFF);
	}

	pdu->data[0] = MB_FN_READ_REGS;
	pdu->length = 2 + byte_count;
}

void
pdu_reply_write_regs (modbus_pdu_t* pdu, pdu_reg_data_t* reg_data)
{
	pdu->data[1] = (uint8_t)(reg_data->address >> 8);
	pdu->data[2] = (uint8_t)(reg_data->address & 0xFF);
	pdu->data[3] = (uint8_t)(reg_data->count >> 8);
	pdu->data[4] = (uint8_t)(reg_data->count & 0xFF);

	pdu->data[0] = MB_FN_WRITE_REGS;
	pdu->length = 5;
}
