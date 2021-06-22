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

enum ascii_framing
{
	ASF_START = 0x3A,
	ASF_END_CR = 0x0D,
	ASF_END_LF = 0x0A,
};


static struct
{
	uint16_t address;
	uint16_t count;
	uint16_t data[CMB_MULTI_MAX];
}
reg_data = { 0 };


static uint8_t frame_data[CMB_FRAME_MAX] = { 0 };
static unsigned int frame_len = 0;

static unsigned int dbg_seq = 0;
static unsigned int dbg_char = 0;


static void parse_ascii (void);
static void parse_adu (void);
static void run_command (void);
static void send_reply (void);

static void ascii_frame_sm (char in);
static bool decode_hex (char in, uint8_t * out);
static void parser_debug (const char *fmt, ...);


void
cmb_init (void)
{
	command = CMD_NONE;
	ascii_state = AS_IDLE;

	reg_data.address = 0;
	reg_data.count = 0;

	frame_len = 0;

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
	parser_debug("cool");
}

static void
run_command (void)
{
	
}

static void
send_reply (void)
{
	
}


static void
ascii_frame_sm (char in)
{
	dbg_char++;

	if (ASF_START == in)
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

			if (ASF_START == in)
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
				frame_len = CMB_FRAME_MAX;
				parser_debug("Overlength frame, resetting.");
				ascii_state = AS_IDLE;

				break;
			}

			if (ASF_END_CR == in)
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
			if (ASF_END_LF == in)
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
decode_hex (char in, uint8_t * out)
{
	if (in >= '0' && in <= '9')
	{
		*out = in - '0';

		return true;
	}
	else if (in >= 'a' && in <= 'f')
	{
		*out = (in - 'a') + 10;

		return true;
	}
	else if (in >= 'A' && in <= 'F')
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
parser_debug (const char *fmt, ...)
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

		for (i = 0; i < frame_len; i++)
		{
			printf("%02X", frame_data[i]);
		}

		printf("\r\n");
	}

	printf("CMB: Warning: %s\r\n\n", buf);
}