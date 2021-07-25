/*
 * MCP4728 I2C DAC Driver
 *
 * @file
 *   mcp4728.c
 *
 * @date
 *   2021-07-24
 *
 * @par
 *   Copyright 2021 Frequencer Team
 *
 * @brief
 *   Simple voltage output control for MCP4728 DAC.
 */

#include <definitions.h>
#include "hang_here.h"

#include "mcp4728.h"


static double voltages[DAC_OUTPUTS];


void
dac_init (void)
{
	const uint8_t bytes = 9;
	uint8_t i2c_out[bytes];
	unsigned int i;

	// Figure 5-9: Sequential Write Command: Write DAC and EEPROM Sequentially.
	// Set all outputs to zero.
	i2c_out[0] = 0b01010000;
	i2c_out[1] = 0b10010000;
	i2c_out[2] = 0b00000000;
	i2c_out[3] = 0b10010000;
	i2c_out[4] = 0b00000000;
	i2c_out[5] = 0b10010000;
	i2c_out[6] = 0b00000000;
	i2c_out[7] = 0b10010000;
	i2c_out[8] = 0b00000000;

	while (I2C5_IsBusy())
	{
	}

	if (!I2C5_Write(DAC_I2C_ADDR, i2c_out, bytes))
	{
		HANG_HERE();
	}

	while (I2C5_IsBusy())
	{
	}

	for (i = 0; i < DAC_OUTPUTS; i++)
	{
		voltages[i] = 0.0;
	}
}


static uint16_t
volts_to_counts (double volts)
{
	if (volts < 0.0)
	{
		return 0;
	}

	uint16_t out = volts * DAC_CODE_SCALE / DAC_VREF;

	if (out > DAC_CODE_MAX)
	{
		return DAC_CODE_MAX;
	}

	return out;
}

void
dac_set (dac_out_t output, double voltage)
{
	const uint8_t bytes = 3;
	uint8_t i2c_out[bytes];
	uint16_t set = volts_to_counts(voltage);

	if ((output < 0) || (output >= DAC_OUTPUTS))
	{
		HANG_HERE();
	}

	// Figure 5-8: Multi-Write Command: Write Multiple DAC Input Registers.
	i2c_out[0] = 0b01000000 | (output << 1);
	i2c_out[1] = 0b10010000 | (set >> 8 & 0x0F);
	i2c_out[2] = set & 0xFF;

	while (I2C5_IsBusy())
	{
	}

	if (!I2C5_Write(DAC_I2C_ADDR, i2c_out, bytes))
	{
		HANG_HERE();
	}

	while (I2C5_IsBusy())
	{
	}

	voltages[output] = voltage;
}

double
dac_get (dac_out_t output)
{
	if ((output < 0) || (output >= DAC_OUTPUTS))
	{
		HANG_HERE();
	}

	return voltages[output];
}
