/*
 * ZL30159 Driver
 *
 * @file
 *   zl30159.c
 *
 * @date
 *   2021-06-17
 *
 * @par
 *   Copyright 2021 Frequencer Team
 *
 * @brief
 *   Provides a driver to the ZL30159 PLL chip in SPI mode.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "definitions.h"
#include "hang_here.h"

#include "zl30159.h"


static bool cur_bank_upper;


static void bank_select (bool upper);


void
zl_init (void)
{
	PLL_RST_Clear();
	CORETIMER_DelayMs(2);
	PLL_RST_Set();

	cur_bank_upper = true;
	bank_select(false);

	zl_set_sticky_r_lock(false);
}

zl_value_t
zl_read_reg (const zl_register_t* reg)
{
	return zl_read_reg_sticky(reg, true);
}

zl_value_t
zl_read_reg_sticky (const zl_register_t* reg, bool now)
{
	uint8_t spi_out[5] = { 0 };
	uint8_t spi_in[5] = { 0 };
	uint8_t bytes = reg->size + 1;
	uint8_t i;
	zl_value_t value;

	if (bytes > 5)
	{
		HANG_HERE();
	}

	if ((ZL_RTYPE_STICKYR == reg->type) && now)
	{
		zl_value_t clear = { .i32 = 0x00 };

		zl_write_reg(reg, clear);

		CORETIMER_DelayMs(5);
	}

	bank_select(reg->address >= ZL_BANK_BOUNDARY);
	spi_out[0] = (reg->address & 0x7F) | 0x80;

	if (!SPI2_WriteRead((void*)spi_out, bytes, (void*)spi_in, bytes))
	{
		HANG_HERE();
	}

	for (i = 0; i < reg->size; i++)
	{
		value.raw[(reg->size - 1) - i] = spi_in[i + 1];
	}

	for (i = reg->size; i < 4; i++)
	{
		value.raw[i] = 0;
	}

	return value;
}

void
zl_write_reg (const zl_register_t* reg, zl_value_t value)
{
	uint8_t spi_out[5] = { 0 };
	uint8_t bytes = reg->size + 1;
	uint8_t i;

	if (bytes > 5)
	{
		HANG_HERE();
	}

	if (ZL_RTYPE_READONLY == reg->type)
	{
		HANG_HERE();
	}

	bank_select(reg->address >= ZL_BANK_BOUNDARY);
	spi_out[0] = reg->address & 0x7F;

	for (i = 0; i < reg->size; i++)
	{
		spi_out[i + 1] = value.raw[(reg->size - 1) - i];
	}

	if (!SPI2_Write((void*)spi_out, bytes))
	{
		HANG_HERE();
	}
}


void
zl_set_sticky_r_lock (bool sticky)
{
	uint8_t spi_out[2];

	spi_out[0] = zl_reg_sticky_r_lock.address & 0x7F;
	spi_out[1] = sticky ? 1 : 0;

	if (!SPI2_Write((void*)spi_out, 2))
	{
		HANG_HERE();
	}
}


const zl_register_t*
zl_find_reg (uint8_t address)
{
	unsigned int i;

	for (i = 0; i < zl_all_regs_count; i++)
	{
		if (zl_all_regs[i]->address == address)
		{
			return zl_all_regs[i];
		}
	}

	return NULL;
}


static void
bank_select (bool upper)
{
	uint8_t spi_out[2];

	if (upper == cur_bank_upper)
	{
		return;
	}

	spi_out[0] = zl_reg_page_register.address & 0x7F;
	spi_out[1] = upper ? 1 : 0;

	if (!SPI2_Write((void*)spi_out, 2))
	{
		HANG_HERE();
	}

	cur_bank_upper = upper;
}
