/*
 * Modbus ASCII Driver
 *
 * @file
 *   con_modbus.c
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

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "definitions.h"
#include "hang_here.h"

#include "con_modbus.h"


static enum
{
	CMD_NONE = 0,
	CMD_READ_REGS,
	CMD_WRITE_REGS,
}
command;

static enum
{
	AS_IDLE = 0,
	AS_START = 10,
	AS_MSNIB = 20,
	AS_LSNIB = 30,
	AS_END_CR = 40,
	AS_READY = 50,
}
ascii_state;

static struct
{
	uint16_t address;
	uint16_t count;
	uint16_t data[CMB_MULTI_MAX];
}
reg_data = { 0 };


static uint8_t frame_data[CMB_FRAME_MAX] = { 0 };
static uint8_t* const pdu_data = frame_data + 1;
static unsigned int frame_len;
static unsigned int pdu_len;

static unsigned int dbg_seq;
static unsigned int dbg_char;


static void parse_ascii (void);
static void parse_adu (void);
static void run_command (void);
static void send_reply (void);

static void ascii_frame_sm (char in);
static bool decode_hex (char in, uint8_t* out);
static void parser_debug (const char* fmt, ...);
static bool con_write_safe (const char* fmt, ...);
static bool validate_lrc (void);
static uint8_t calculate_lrc (void);

static void parse_pdu_read_regs (void);
static void parse_pdu_write_regs (void);

static void reply_none (void);
static void reply_exception (cmb_modbus_exception_t exception);
static void reply_read_regs (void);
static void reply_write_regs (void);


/// Public functions.

void
cmb_init (void)
{
	command = CMD_NONE;
	ascii_state = AS_IDLE;

	reg_data.address = 0;
	reg_data.count = 0;

	frame_len = 0;
	pdu_len = 0;

	dbg_seq = 0;
	dbg_char = 0;
}

void
cmb_task (void)
{
	parse_ascii();

	if (AS_READY == ascii_state)
	{
		parse_adu();
		run_command();
		send_reply();

		ascii_state = AS_IDLE;
	}
}


/// High-level internal functions.

static void
parse_ascii (void)
{
	char in;
	ssize_t count, free;

	free = SYS_CONSOLE_ReadFreeBufferCountGet(sysObj.sysConsole0);

	if (free == 0)
	{
		if (AS_READY == ascii_state)
		{
			parser_debug("RX buffer overflow, ignoring.");
		}
		else
		{
			parser_debug("RX buffer overflow, resetting.");
			ascii_state = AS_IDLE;
		}
	}
	else if (free < 0)
	{
		HANG_HERE();
	}

	while (true)
	{
		count = SYS_CONSOLE_Read(sysObj.sysConsole0, &in, 1);

		if (count < 0)
		{
			HANG_HERE();
		}

		if (count < 1)
		{
			break;
		}

		ascii_frame_sm(in);
	}
}

static void
parse_adu (void)
{
	if (frame_len < 3)
	{
		parser_debug("Ignoring too-short frame.");

		command = CMD_NONE;
		reply_none();

		return;
	}

	if (CMB_ADDRESS != frame_data[0])
	{
		parser_debug("Ignoring frame with wrong address %d.", frame_data[0]);

		command = CMD_NONE;
		reply_none();

		return;
	}

	if (!validate_lrc())
	{
		parser_debug("Ignoring frame with wrong LRC.");

		command = CMD_NONE;
		reply_none();

		return;
	}

	pdu_len = frame_len - 2;

	switch (pdu_data[0])  // Function byte.
	{
		case CMB_MF_READ_REGS:
		{
			// Set command before decoding PDU because decode error might change it.
			command = CMD_READ_REGS;
			parse_pdu_read_regs();

			break;
		}

		case CMB_MF_WRITE_REGS:
		{
			command = CMD_WRITE_REGS;
			parse_pdu_write_regs();

			break;
		}

		default:
		{
			parser_debug("Ignoring request with unimplemented function.");
			reply_exception(CMB_EX_ILLEGAL_FN);
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
			parser_debug("Read Regs NYI");

			break;
		}

		case CMD_WRITE_REGS:
		{
			parser_debug("Write Regs NYI");

			break;
		}

		default:
		{
			HANG_HERE();
		}
	}
}

static void
send_reply (void)
{
	unsigned int i = 0;

	// Create reply.
	switch (pdu_data[0])  // Function byte.
	{
		case CMB_MF_READ_REGS:
		{
			if (CMD_READ_REGS == command)
			{
				reply_read_regs();
			}

			break;
		}

		case CMB_MF_WRITE_REGS:
		{
			if (CMD_WRITE_REGS == command)
			{
				reply_write_regs();
			}

			break;
		}

		default:
		{
			// No reply (except maybe exception).

			break;
		}
	}

	command = CMD_NONE;

	// Send reply.
	if (pdu_len > 0)
	{
		frame_len = pdu_len + 2;
		pdu_len = 0;

		if (frame_len > CMB_FRAME_MAX)
		{
			parser_debug("Overlength reply, won't send.");
			frame_len = 0;

			return;
		}

		frame_data[frame_len - 1] = calculate_lrc();

		if (!con_write_safe(":"))
		{
			frame_len = 0;

			return;
		}

		while (i < frame_len)
		{
			if (!con_write_safe("%02X", frame_data[i]))
			{
				frame_len = 0;

				return;
			}

			i++;
		}

		con_write_safe("\r\n");
	}

	frame_len = 0;
}


/// General internal functions.

static void
ascii_frame_sm (char in)
{
	dbg_char++;

	if (CMB_ASF_START == in)
	{
		if (AS_READY != ascii_state)
		{
			if (AS_IDLE != ascii_state)
			{
				parser_debug("Unexpected SOF.");
			}

			// Reset.
			ascii_state = AS_IDLE;
		}
		else
		{
			// Can't handle it now as other mechanisms may be using the data.
			parser_debug("Ignoring SOF.");
		}
	}

	// Note that ``ascii_state`` is state BEFORE processing...
	// i.e. it corresponds to the last received character, not ``in``.
	switch (ascii_state)
	{
		case AS_IDLE:
		{
			frame_len = 0;
			dbg_char = 0;

			if (CMB_ASF_START == in)
			{
				ascii_state = AS_START;
				dbg_seq++;
			}

			break;
		}

		case AS_START:
		case AS_MSNIB:
		case AS_LSNIB:
		{
			uint8_t nib;

			if (frame_len > CMB_FRAME_MAX)
			{
				parser_debug("Overlength frame, resetting.");
				ascii_state = AS_IDLE;

				break;
			}

			if (CMB_ASF_END_CR == in)
			{
				if (AS_LSNIB != ascii_state)
				{
					// If last char wasn't LSnib then we only got half a byte.
					parser_debug("Misaligned EOF.");
				}

				ascii_state = AS_END_CR;

				break;
			}

			if (!decode_hex(in, &nib))
			{
				parser_debug("Invalid hex char %c, resetting.", in);
				ascii_state = AS_IDLE;

				break;
			}

			if (AS_MSNIB == ascii_state)
			{
				// Current char is LSnib.
				frame_data[frame_len] |= (nib & 0x0F);
				frame_len++;
				ascii_state = AS_LSNIB;
			}
			else
			{
				// Current char is MSnib.
				frame_data[frame_len] = (nib << 4);
				ascii_state = AS_MSNIB;
			}

			break;
		}

		case AS_END_CR:
		{
			if (CMB_ASF_END_LF == in)
			{
				ascii_state = AS_READY;
			}
			else
			{
				parser_debug("Incomplete EOF, resetting.");
				ascii_state = AS_IDLE;
			}

			break;
		}

		case AS_READY:
		{
			// Wait.

			break;
		}

		default:
		{
			HANG_HERE();
		}
	}
}

static bool
decode_hex (char in, uint8_t* out)
{
	if ((in >= '0') && (in <= '9'))
	{
		*out = in - '0';

		return true;
	}
	else if ((in >= 'a') && (in <= 'f'))
	{
		*out = (in - 'a') + 10;

		return true;
	}
	else if ((in >= 'A') && (in <= 'F'))
	{
		*out = (in - 'A') + 10;

		return true;
	}
	else
	{
		return false;
	}
}

static void
parser_debug (const char* fmt, ...)
{
	char buf[100];
	unsigned int i;

	va_list args;

	va_start(args, fmt);
	vsnprintf(buf, 100, fmt, args);
	va_end(args);

	if (AS_READY == ascii_state)
	{
		printf(
			"CMB: Function warning. Frame %d, function %d.\r\n",
			dbg_seq,
			frame_data[1]
		);
	}
	else
	{
		printf(
			"CMB: Parse warning. Frame %d, char %d, state %d.\r\n",
			dbg_seq,
			dbg_char,
			ascii_state
		);
	}

	if (frame_len > 0)
	{
		printf("CMB: Frame data: ");

		for (i = 0; (i < frame_len) && (i < CMB_FRAME_MAX); i++)
		{
			printf("%02X", frame_data[i]);
		}

		printf("\r\n");
	}

	printf("CMB: Warning: %s\r\n\n", buf);
}

static bool
con_write_safe (const char* fmt, ...)
{
	// Note: Don't write more than 100 bytes at a time...

	char buf[100];
	ssize_t free;
	int written;

	free = SYS_CONSOLE_WriteFreeBufferCountGet(sysObj.sysConsole0);

	if (free < 0)
	{
		HANG_HERE();
	}
	else if (free < 2)
	{
		parser_debug("TX overflow, discarding reply.");

		return false;
	}

	va_list args;

	va_start(args, fmt);
	written = vsnprintf(buf, 100, fmt, args);
	va_end(args);

	if ((written < 0) || (written > 100))
	{
		HANG_HERE();
	}

	SYS_CONSOLE_Message(sysObj.sysConsole0, buf);

	return true;
}

static bool
validate_lrc (void)
{
	return true;
}

static uint8_t
calculate_lrc (void)
{
	return 0x00;
}


/// Functions to parse a PDU already identified by function code.

static void
parse_pdu_read_regs (void)
{
	uint16_t address, count;

	address = (uint16_t)(pdu_data[2]);
	address |= ((uint16_t)(pdu_data[1]) << 8);
	count = (uint16_t)(pdu_data[4]);
	count |= ((uint16_t)(pdu_data[3]) << 8);

	reg_data.address = address;
	reg_data.count = count;
}

static void
parse_pdu_write_regs (void)
{
	unsigned int i;
	uint8_t byte_count;
	uint16_t address, count;

	address = (uint16_t)(pdu_data[2]);
	address |= ((uint16_t)(pdu_data[1]) << 8);
	count = (uint16_t)(pdu_data[4]);
	count |= ((uint16_t)(pdu_data[3]) << 8);
	byte_count = pdu_data[5];

	if (byte_count != (count * 2))
	{
		parser_debug("Ignoring write registers request with count mismatch.");
		command = CMD_NONE;
	}

	reg_data.address = address;
	reg_data.count = count;

	for (i = 0; i < reg_data.count; i++)
	{
		reg_data.data[i] = (uint16_t)(pdu_data[7 + (i * 2)]);
		reg_data.data[i] |= ((uint16_t)(pdu_data[6 + (i * 2)]) << 8);
	}
}


/// Functions to compose a reply. They do not send the reply.

static void
reply_none (void)
{
	// Simply configures state such that no reply is sent.

	pdu_len = 0;
}

static void
reply_exception (cmb_modbus_exception_t exception)
{
	// Must only be called when a valid frame is in the data buffer.

	pdu_data[0] += CMB_EXCEPTION_OFFSET;
	pdu_data[1] = exception;
	pdu_len = 2;
}

static void
reply_read_regs (void)
{
	unsigned int i;
	uint8_t byte_count;

	if (reg_data.count > CMB_MULTI_MAX)
	{
		HANG_HERE();
	}

	byte_count = (uint8_t)(reg_data.count * 2);
	pdu_data[1] = byte_count;

	for (i = 0; i < reg_data.count; i++)
	{
		pdu_data[2 + i] = (uint8_t)(reg_data.data[i] >> 8);
		pdu_data[3 + i] = (uint8_t)(reg_data.data[i] & 0xFF);
	}

	pdu_len = 2 + byte_count;
}

static void
reply_write_regs (void)
{
	pdu_data[1] = (uint8_t)(reg_data.address >> 8);
	pdu_data[2] = (uint8_t)(reg_data.address & 0xFF);
	pdu_data[3] = (uint8_t)(reg_data.count >> 8);
	pdu_data[4] = (uint8_t)(reg_data.count & 0xFF);

	pdu_len = 5;
}
