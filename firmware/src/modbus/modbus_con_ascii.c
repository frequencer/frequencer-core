/*
 * Modbus LL Driver - ASCII/Harmony Console
 *
 * @file
 *   modbus_con_ascii.c
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

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../drivers/hang_here.h"
#include "definitions.h"

#include "modbus_con_ascii.h"
#include "modbus_defs.h"


#define PDU_EMPTY ((modbus_pdu_t){ .data = NULL, .length = 0 })


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


uint8_t adu_data[MCA_FRAME_MAX] = { 0 };
mca_adu_t adu;

static unsigned int dbg_seq;
static unsigned int dbg_char;

static void ascii_frame_sm (char in);
static bool decode_hex (char in, uint8_t* out);
static bool con_write_safe (const char* fmt, ...);

static bool validate_lrc (mca_adu_t* adu);
static uint8_t calculate_lrc (mca_adu_t* adu);
static void update_lrc (mca_adu_t* adu);


void
mca_init (void)
{
	ascii_state = AS_IDLE;

	adu.data = adu_data;
	adu.length = 0;

	dbg_seq = 0;
	dbg_char = 0;
}


void
mca_task (void)
{
	char in;
	ssize_t count, free;

	free = SYS_CONSOLE_ReadFreeBufferCountGet(sysObj.sysConsole0);

	if (free == 0)
	{
		if (AS_READY == ascii_state)
		{
			mb_debug("RX buffer overflow, ignoring.");
		}
		else
		{
			mb_debug("RX buffer overflow, resetting.");
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


modbus_pdu_t
mca_parse_adu (void)
{
	if (AS_READY != ascii_state)
	{
		// Frame not fully received.
		return PDU_EMPTY;
	}

	if (adu.length < 3)
	{
		mb_debug("Ignoring too-short frame.");

		return PDU_EMPTY;
	}

	if (MODBUS_ADDRESS != adu.data[0])
	{
		mb_debug("Ignoring frame with wrong address %d.", adu.data[0]);

		return PDU_EMPTY;
	}

	if (!validate_lrc(&adu))
	{
		mb_debug("Ignoring frame with wrong LRC.");

		return PDU_EMPTY;
	}

	return (modbus_pdu_t){
			   .data = &(adu.data[1]),
			   .length = adu.length - 2,
	};
}

void
mca_send_reply (modbus_pdu_t* pdu)
{
	unsigned int i = 0;

	if (NULL == pdu)
	{
		adu.length = 0;

		return;
	}

	if (pdu->length <= 0)
	{
		adu.length = 0;

		return;
	}

	adu.length = pdu->length + 2;

	if (adu.length > MCA_FRAME_MAX)
	{
		mb_debug("Overlength reply, won't send.");
		adu.length = 0;

		return;
	}

	adu.data[0] = MODBUS_ADDRESS;
	update_lrc(&adu);

	if (!con_write_safe(":"))
	{
		adu.length = 0;

		return;
	}

	while (i < adu.length)
	{
		if (!con_write_safe("%02X", adu.data[i]))
		{
			adu.length = 0;

			return;
		}

		i++;
	}

	con_write_safe("\r\n");

	adu.length = 0;
}

// Call this function when you are done with the result of mca_parse_adu.
// Call it even if mca_parse_adu gave an invalid result.
void
mca_done (void)
{
	if (AS_READY == ascii_state)
	{
		ascii_state = AS_IDLE;
	}
}


void
mb_debug (const char* fmt, ...)
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
			"MCA: Function warning. Frame %d, function %d.\r\n",
			dbg_seq,
			adu.data[1]
		);
	}
	else
	{
		printf(
			"MCA: Parse warning. Frame %d, char %d, state %d.\r\n",
			dbg_seq,
			dbg_char,
			ascii_state
		);
	}

	if (adu.length > 0)
	{
		printf("MCA: Frame data: ");

		for (i = 0; (i < adu.length) && (i < MCA_FRAME_MAX); i++)
		{
			printf("%02X", adu.data[i]);
		}

		printf("\r\n");
	}

	printf("MCA: Warning: %s\r\n\n", buf);
}


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
				mb_debug("Unexpected SOF.");
			}

			// Reset.
			ascii_state = AS_IDLE;
		}
		else
		{
			// Can't handle it now as other mechanisms may be using the data.
			mb_debug("Ignoring SOF.");
		}
	}

	// Note that ``ascii_state`` is state BEFORE processing...
	// i.e. it corresponds to the last received character, not ``in``.
	switch (ascii_state)
	{
		case AS_IDLE:
		{
			adu.length = 0;
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

			if (adu.length > MCA_FRAME_MAX)
			{
				mb_debug("Overlength frame, resetting.");
				ascii_state = AS_IDLE;

				break;
			}

			if (CMB_ASF_END_CR == in)
			{
				if (AS_LSNIB != ascii_state)
				{
					// If last char wasn't LSnib then we only got half a byte.
					mb_debug("Misaligned EOF.");
				}

				ascii_state = AS_END_CR;

				break;
			}

			if (!decode_hex(in, &nib))
			{
				mb_debug("Invalid hex char %c, resetting.", in);
				ascii_state = AS_IDLE;

				break;
			}

			if (AS_MSNIB == ascii_state)
			{
				// Current char is LSnib.
				adu.data[adu.length] |= (nib & 0x0F);
				adu.length++;
				ascii_state = AS_LSNIB;
			}
			else
			{
				// Current char is MSnib.
				adu.data[adu.length] = (nib << 4);
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
				mb_debug("Incomplete EOF, resetting.");
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
		mb_debug("TX overflow, discarding reply.");

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
validate_lrc (mca_adu_t* adu)
{
	return true;

	return calculate_lrc(adu) == adu->data[adu->length - 1];
}

static uint8_t
calculate_lrc (mca_adu_t* adu)
{
	return 0x00;
}

static void
update_lrc (mca_adu_t* adu)
{
	adu->data[adu->length - 1] = calculate_lrc(adu);
}
