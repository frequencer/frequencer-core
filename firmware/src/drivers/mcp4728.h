/*
 * MCP4728 I2C DAC Driver
 *
 * @file
 *   mcp4728.h
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

#ifndef MCP4728_H
#define MCP4728_H


#include <stdint.h>


#define DAC_I2C_ADDR (0b01100000)

#define DAC_CODE_MAX (4095U)
#define DAC_VREF (4.096)
#define DAC_CODE_SCALE (4096.0)


#ifdef  __cplusplus
extern "C" {
#endif


typedef enum
{
	DAC_VO1 = 0,
	DAC_VO2 = 1,
	DAC_FVO = 2,
	DAC_AOD = 3,
	DAC_OUTPUTS = 4,
}
dac_out_t;


void dac_init (void);

void dac_set (dac_out_t output, double voltage);
double dac_get (dac_out_t output);


#ifdef  __cplusplus
}
#endif

#endif /* MCP4728_H */
