/*
 * ZL30159 Driver
 *
 * @file
 *   zl30159.h
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

#ifndef ZL30159_H
#define ZL30159_H


#include "zl30159_defs.h"


#define ZL_BANK_BOUNDARY (0x80U)


#ifdef  __cplusplus
extern "C" {
#endif


void zl_init (void);

zl_value_t zl_read_reg (const zl_register_t* reg);
zl_value_t zl_read_reg_sticky (const zl_register_t* reg, bool now);
bool zl_write_reg (const zl_register_t* reg, zl_value_t value);

void zl_set_sticky_r_lock (bool sticky);

const zl_register_t* zl_find_reg (uint8_t address);


#ifdef  __cplusplus
}
#endif

#endif /* ZL30159_H */
