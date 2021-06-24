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
#include "definitions.h"

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


static modbus_pdu_t pdu;
static pdu_reg_data_t reg_data = { 0 };


static void parse_pdu (void);
static void run_command (void);
static void make_reply (void);


void
modbus_init (void)
{
	command = CMD_NONE;

	pdu.data = NULL;
	pdu.length = 0;
	reg_data.address = 0;
	reg_data.count = 0;

	mca_init();
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
		make_reply();
		mca_send_reply(&pdu);
	}
}


static void
parse_pdu (void)
{
	switch (pdu.data[0])  // Function byte.
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
			mb_debug("Read Regs NYI");

			break;
		}

		case CMD_WRITE_REGS:
		{
			mb_debug("Write Regs NYI");

			break;
		}

		default:
		{
			HANG_HERE();
		}
	}
}

static void
make_reply (void)
{
	// Create reply.
	switch (command)
	{
		case CMD_READ_REGS:
		{
			pdu_reply_read_regs(&pdu, &reg_data);

			break;
		}

		case CMD_WRITE_REGS:
		{
			pdu_reply_write_regs(&pdu, &reg_data);

			break;
		}

		default:
		{
			// No reply (except maybe exception).

			break;
		}
	}

	command = CMD_NONE;
}
