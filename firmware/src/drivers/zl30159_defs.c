/*
 * ZL30159 Register Definitions
 * 
 * @file
 *   zl30159_defs.c
 * 
 * @date
 *   2021-06-17
 * 
 * @par
 *   Copyright 2021 Frequencer Team
 * 
 * @brief
 *   Register definitions for the ZL30159.
*/

#include "zl30159_defs.h"


const zl_register_t zl_reg_id_reg = {
	.address = 0x00,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READONLY,
	.default_ = 0x09,
};

const zl_register_t zl_reg_ref_fail_isr_status = {
	.address = 0x02,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_STICKYR,
	.default_ = 0x00,
};

const zl_register_t zl_reg_dpll_isr_status = {
	.address = 0x03,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_STICKYR,
	.default_ = 0x00,
};

const zl_register_t zl_reg_ref_fail_isr_mask = {
	.address = 0x04,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_dpll_isr_mask = {
	.address = 0x05,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_ref_mon_fail = {
	.address = 0x07,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_STICKYR,
	.default_ = 0x00,
};

const zl_register_t zl_reg_ref_mon_fail_mask = {
	.address = 0x09,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x66,
};

const zl_register_t zl_reg_ref_config = {
	.address = 0x0A,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_gst_disqualif_time = {
	.address = 0x0B,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0xAA,
};

const zl_register_t zl_reg_gst_qualif_time = {
	.address = 0x0C,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x55,
};

const zl_register_t zl_reg_sticky_r_lock = {
	.address = 0x0D,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_ref_base_freq = {
	.address = 0x10,
	.size = ZL_RSIZE_16,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x61A8,
};

const zl_register_t zl_reg_ref_freq_multiple = {
	.address = 0x12,
	.size = ZL_RSIZE_16,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x03E8,
};

const zl_register_t zl_reg_ref_ratio_m_n = {
	.address = 0x14,
	.size = ZL_RSIZE_32,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00010001,
};

const zl_register_t zl_reg_dpll_ctrl = {
	.address = 0x30,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x0C,
};

const zl_register_t zl_reg_dpll_mode_refsel = {
	.address = 0x33,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x03,
};

const zl_register_t zl_reg_dpll_ref_fail_mask = {
	.address = 0x34,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x87,
};

const zl_register_t zl_reg_dpll_hold_lock_fail = {
	.address = 0x44,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_STICKYR,
	.default_ = 0x00,
};

const zl_register_t zl_reg_phasememlimit_ref = {
	.address = 0x47,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x0A,
};

const zl_register_t zl_reg_scm_cfm_limit_ref = {
	.address = 0x4B,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x55,
};

const zl_register_t zl_reg_dpll_config = {
	.address = 0x4F,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x31,
};

const zl_register_t zl_reg_synth_base_freq = {
	.address = 0x50,
	.size = ZL_RSIZE_16,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x61A8,
};

const zl_register_t zl_reg_synth_freq_multiple = {
	.address = 0x52,
	.size = ZL_RSIZE_16,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x0EA6,
};

const zl_register_t zl_reg_synth_ratio_m_n = {
	.address = 0x54,
	.size = ZL_RSIZE_32,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00010001,
};

const zl_register_t zl_reg_output_synthesizer_en = {
	.address = 0x71,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x01,
};

const zl_register_t zl_reg_dpll_lock_selection = {
	.address = 0x72,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0xAA,
};

const zl_register_t zl_reg_central_freq_offset = {
	.address = 0x73,
	.size = ZL_RSIZE_32,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x046AAAAB,
};

const zl_register_t zl_reg_synth_filter_sel = {
	.address = 0x77,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_synth_filter_phase_shift = {
	.address = 0x78,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_page_register = {
	.address = 0x7F,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_synth_post_div_a = {
	.address = 0x86,
	.size = ZL_RSIZE_24,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00003C,
};

const zl_register_t zl_reg_synth_post_div_b = {
	.address = 0x89,
	.size = ZL_RSIZE_24,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00000C,
};

const zl_register_t zl_reg_hp_cmos_en = {
	.address = 0xB1,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_synth_stop_clk = {
	.address = 0xB8,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_sync_fail_flag_status = {
	.address = 0xB9,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_STICKYR,
	.default_ = 0x00,
};

const zl_register_t zl_reg_clear_sync_fail_flag = {
	.address = 0xBA,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_phase_shift_s_postdiv_a = {
	.address = 0xBF,
	.size = ZL_RSIZE_16,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_phase_shift_s_postdiv_b = {
	.address = 0xC1,
	.size = ZL_RSIZE_16,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_xo_or_crystal_sel = {
	.address = 0xC3,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_chip_revision = {
	.address = 0xC6,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x03,
};

const zl_register_t zl_reg_gpio_function_pin0 = {
	.address = 0xE0,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_gpio_function_pin1 = {
	.address = 0xE1,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_gpio_function_pin2 = {
	.address = 0xE2,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x70,
};

const zl_register_t zl_reg_gpio_function_pin3 = {
	.address = 0xE3,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_gpio_function_pin4 = {
	.address = 0xE4,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_gpio_function_pin5 = {
	.address = 0xE5,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_gpio_function_pin6 = {
	.address = 0xE6,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x72,
};

const zl_register_t zl_reg_dpll_ctrl2 = {
	.address = 0xEC,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_dpll_holdpull = {
	.address = 0xED,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x07,
};

const zl_register_t zl_reg_pfm_mask_ho = {
	.address = 0xF4,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0xF0,
};

const zl_register_t zl_reg_pfm_mask_ref_fail = {
	.address = 0xF5,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x00,
};

const zl_register_t zl_reg_pfm_range_ref = {
	.address = 0xF7,
	.size = ZL_RSIZE_8,
	.type = ZL_RTYPE_READWRITE,
	.default_ = 0x33,
};

const zl_register_t * const zl_all_regs[] = {
	ZL_REG_ID_REG,
	ZL_REG_REF_FAIL_ISR_STATUS,
	ZL_REG_DPLL_ISR_STATUS,
	ZL_REG_REF_FAIL_ISR_MASK,
	ZL_REG_DPLL_ISR_MASK,
	ZL_REG_REF_MON_FAIL,
	ZL_REG_REF_MON_FAIL_MASK,
	ZL_REG_REF_CONFIG,
	ZL_REG_GST_DISQUALIF_TIME,
	ZL_REG_GST_QUALIF_TIME,
	ZL_REG_STICKY_R_LOCK,
	ZL_REG_REF_BASE_FREQ,
	ZL_REG_REF_FREQ_MULTIPLE,
	ZL_REG_REF_RATIO_M_N,
	ZL_REG_DPLL_CTRL,
	ZL_REG_DPLL_MODE_REFSEL,
	ZL_REG_DPLL_REF_FAIL_MASK,
	ZL_REG_DPLL_HOLD_LOCK_FAIL,
	ZL_REG_PHASEMEMLIMIT_REF,
	ZL_REG_SCM_CFM_LIMIT_REF,
	ZL_REG_DPLL_CONFIG,
	ZL_REG_SYNTH_BASE_FREQ,
	ZL_REG_SYNTH_FREQ_MULTIPLE,
	ZL_REG_SYNTH_RATIO_M_N,
	ZL_REG_OUTPUT_SYNTHESIZER_EN,
	ZL_REG_DPLL_LOCK_SELECTION,
	ZL_REG_CENTRAL_FREQ_OFFSET,
	ZL_REG_SYNTH_FILTER_SEL,
	ZL_REG_SYNTH_FILTER_PHASE_SHIFT,
	ZL_REG_PAGE_REGISTER,
	ZL_REG_SYNTH_POST_DIV_A,
	ZL_REG_SYNTH_POST_DIV_B,
	ZL_REG_HP_CMOS_EN,
	ZL_REG_SYNTH_STOP_CLK,
	ZL_REG_SYNC_FAIL_FLAG_STATUS,
	ZL_REG_CLEAR_SYNC_FAIL_FLAG,
	ZL_REG_PHASE_SHIFT_S_POSTDIV_A,
	ZL_REG_PHASE_SHIFT_S_POSTDIV_B,
	ZL_REG_XO_OR_CRYSTAL_SEL,
	ZL_REG_CHIP_REVISION,
	ZL_REG_GPIO_FUNCTION_PIN0,
	ZL_REG_GPIO_FUNCTION_PIN1,
	ZL_REG_GPIO_FUNCTION_PIN2,
	ZL_REG_GPIO_FUNCTION_PIN3,
	ZL_REG_GPIO_FUNCTION_PIN4,
	ZL_REG_GPIO_FUNCTION_PIN5,
	ZL_REG_GPIO_FUNCTION_PIN6,
	ZL_REG_DPLL_CTRL2,
	ZL_REG_DPLL_HOLDPULL,
	ZL_REG_PFM_MASK_HO,
	ZL_REG_PFM_MASK_REF_FAIL,
	ZL_REG_PFM_RANGE_REF
};

const unsigned int zl_all_regs_count = sizeof(zl_all_regs) / sizeof(zl_register_t *);
