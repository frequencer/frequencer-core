/*
 * Modbus Driver
 *
 * @file
 *   modbus.c
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

#include <stdbool.h>
#include <stdint.h>

#include "../drivers/hang_here.h"

#include "modbus.h"
#include "modbus_con_ascii.h"
#include "modbus_defs.h"
#include "modbus_pdu.h"


static enum
{
	CMD_NONE = 0,
	CMD_READ_REGS,
	CMD_WRITE_REGS,
}
command;

static mb_handled_regs_t write_handlers[MODBUS_MAX_REG_HANDLERS];
static mb_handled_regs_t read_handlers[MODBUS_MAX_REG_HANDLERS];

static modbus_pdu_t pdu;
static mb_reg_data_t reg_data = { 0 };


static void parse_pdu (void);
static void run_command (void);
static mb_reg_handler_t get_handler (mb_reg_data_t* reg_data, mb_handled_regs_t* list);
static inline bool is_address_handled (uint16_t address, mb_handled_regs_t* range);


void
modbus_init (void)
{
	unsigned int i;

	command = CMD_NONE;

	pdu.data = NULL;
	pdu.length = 0;
	reg_data.address = 0;
	reg_data.count = 0;

	mca_init();

	for (i = 0; i < MODBUS_MAX_REG_HANDLERS; i++)
	{
		write_handlers[i].handler = NULL;
		read_handlers[i].handler = NULL;
	}
}

void
modbus_task (void)
{
	mca_task();

	pdu = mca_parse_adu();

	if ((pdu.length > 0) && (NULL != pdu.data))
	{
		parse_pdu();
		run_command();
		mca_send_reply(&pdu);
	}

	mca_done();
}


void
modbus_add_reg_handler (
	uint16_t address,
	uint16_t count,
	mb_reg_action_t action,
	mb_reg_handler_t handler
)
{
	unsigned int unused_idx;
	mb_handled_regs_t* list;

	// Decide what list we're adding to.
	switch (action)
	{
		case MB_RA_WRITE:
		{
			list = write_handlers;

			break;
		}

		case MB_RA_READ:
		{
			list = read_handlers;

			break;
		}

		default:
		{
			HANG_HERE();
		}
	}

	// Find unused entry in the list.
	for (unused_idx = 0; unused_idx < MODBUS_MAX_REG_HANDLERS; unused_idx++)
	{
		if (is_address_handled(address, &(list[unused_idx])))
		{
			// Starting address already registered.
			HANG_HERE();
		}

		// We don't check the middle address... don't do anything stupid.

		if (is_address_handled(address + count - 1, &(list[unused_idx])))
		{
			// Ending address already registered.
			HANG_HERE();
		}

		if (NULL == list[unused_idx].handler)
		{
			// Found unused entry.
			break;
		}
	}

	if (unused_idx < MODBUS_MAX_REG_HANDLERS)
	{
		// Store handler data in unused list entry.
		list[unused_idx].address = address;
		list[unused_idx].count = count;
		list[unused_idx].handler = handler;
	}
	else
	{
		// No unused entries.
		HANG_HERE();
	}
}


static void
parse_pdu (void)
{
	// Determine function based on first PDU byte.
	// Try to decode the PDU according to the function format - if successful,
	// set the command code so run_command will do something. Otherwise, reply
	// with an exception.
	switch (pdu.data[0])
	{
		case MB_FN_READ_REGS:
		{
			if (pdu_parse_read_regs(&pdu, &reg_data))
			{
				command = CMD_READ_REGS;
			}
			else
			{
				pdu_reply_exception(&pdu, MB_EX_DEVICE_FAILURE);
				command = CMD_NONE;
			}

			break;
		}

		case MB_FN_WRITE_REGS:
		{
			if (pdu_parse_write_regs(&pdu, &reg_data))
			{
				command = CMD_WRITE_REGS;
			}
			else
			{
				pdu_reply_exception(&pdu, MB_EX_DEVICE_FAILURE);
				command = CMD_NONE;
			}

			break;
		}

		default:
		{
			mb_debug("Ignoring request with unimplemented function.");
			pdu_reply_exception(&pdu, MB_EX_ILLEGAL_FN);
			command = CMD_NONE;

			break;
		}
	}
}

static void
run_command (void)
{
	switch (command)
	{
		case CMD_NONE:
		{
			// Do nothing!

			break;
		}

		case CMD_READ_REGS:
		{
			mb_reg_handler_t handler = get_handler(&reg_data, read_handlers);

			if (NULL != handler)
			{
				if (handler(&reg_data))
				{
					pdu_reply_read_regs(&pdu, &reg_data);
				}
				else
				{
					pdu_reply_exception(&pdu, MB_EX_ILLEGAL_VALUE);
				}
			}
			else
			{
				pdu_reply_exception(&pdu, MB_EX_ILLEGAL_ADDR);
			}

			break;
		}

		case CMD_WRITE_REGS:
		{
			mb_reg_handler_t handler = get_handler(&reg_data, write_handlers);

			if (NULL != handler)
			{
				if (handler(&reg_data))
				{
					pdu_reply_write_regs(&pdu, &reg_data);
				}
				else
				{
					pdu_reply_exception(&pdu, MB_EX_ILLEGAL_VALUE);
				}
			}
			else
			{
				pdu_reply_exception(&pdu, MB_EX_ILLEGAL_ADDR);
			}

			break;
		}

		default:
		{
			HANG_HERE();
		}
	}

	command = CMD_NONE;
}

static mb_reg_handler_t
get_handler (mb_reg_data_t* reg_data, mb_handled_regs_t* list)
{
	unsigned int i;
	uint16_t end_address = reg_data->address + reg_data->count - 1;

	if (reg_data->address > end_address)
	{
		mb_debug("Ignoring zero-count request.");

		return NULL;
	}

	if (reg_data->count > MODBUS_REGS_MULTI_MAX)
	{
		mb_debug("Ignoring too big request.");

		return NULL;
	}

	for (i = 0; i < MODBUS_MAX_REG_HANDLERS; i++)
	{
		if (NULL == list[i].handler)
		{
			// No handler at this index.
			continue;
		}

		if (!is_address_handled(reg_data->address, &(list[i])))
		{
			// First address not in range of this handler.
			continue;
		}

		if (!is_address_handled(end_address, &(list[i])))
		{
			// Request range continues beyond handler range.
			mb_debug("Request spanning handlers is unsupported, ignoring.");

			return NULL;
		}

		// It's good!
		return list[i].handler;
	}

	// No handler found.
	mb_debug("No handler found for address %d.", reg_data->address);

	return NULL;
}

static inline bool
is_address_handled (uint16_t address, mb_handled_regs_t* range)
{
	return (address >= range->address) && (address < (range->address + range->count));
}
