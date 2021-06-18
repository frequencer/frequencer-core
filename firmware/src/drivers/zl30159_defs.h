/*
 * ZL30159 Register Definitions
 * 
 * @file
 *   zl30159_defs.h
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

#ifndef ZL30159_DEFS_H
#define	ZL30159_DEFS_H


#include <stdbool.h>
#include <stdint.h>


// Convenience macros for pointers to the register definitions.

#define ZL_REG_ID_REG (&zl_reg_id_reg)
#define ZL_REG_REF_FAIL_ISR_STATUS (&zl_reg_ref_fail_isr_status)
#define ZL_REG_DPLL_ISR_STATUS (&zl_reg_dpll_isr_status)
#define ZL_REG_REF_FAIL_ISR_MASK (&zl_reg_ref_fail_isr_mask)
#define ZL_REG_DPLL_ISR_MASK (&zl_reg_dpll_isr_mask)
#define ZL_REG_REF_MON_FAIL (&zl_reg_ref_mon_fail)
#define ZL_REG_REF_MON_FAIL_MASK (&zl_reg_ref_mon_fail_mask)
#define ZL_REG_REF_CONFIG (&zl_reg_ref_config)
#define ZL_REG_GST_DISQUALIF_TIME (&zl_reg_gst_disqualif_time)
#define ZL_REG_GST_QUALIF_TIME (&zl_reg_gst_qualif_time)
#define ZL_REG_STICKY_R_LOCK (&zl_reg_sticky_r_lock)
#define ZL_REG_REF_BASE_FREQ (&zl_reg_ref_base_freq)
#define ZL_REG_REF_FREQ_MULTIPLE (&zl_reg_ref_freq_multiple)
#define ZL_REG_REF_RATIO_M_N (&zl_reg_ref_ratio_m_n)
#define ZL_REG_DPLL_CTRL (&zl_reg_dpll_ctrl)
#define ZL_REG_DPLL_MODE_REFSEL (&zl_reg_dpll_mode_refsel)
#define ZL_REG_DPLL_REF_FAIL_MASK (&zl_reg_dpll_ref_fail_mask)
#define ZL_REG_DPLL_HOLD_LOCK_FAIL (&zl_reg_dpll_hold_lock_fail)
#define ZL_REG_PHASEMEMLIMIT_REF (&zl_reg_phasememlimit_ref)
#define ZL_REG_SCM_CFM_LIMIT_REF (&zl_reg_scm_cfm_limit_ref)
#define ZL_REG_DPLL_CONFIG (&zl_reg_dpll_config)
#define ZL_REG_SYNTH_BASE_FREQ (&zl_reg_synth_base_freq)
#define ZL_REG_SYNTH_FREQ_MULTIPLE (&zl_reg_synth_freq_multiple)
#define ZL_REG_SYNTH_RATIO_M_N (&zl_reg_synth_ratio_m_n)
#define ZL_REG_OUTPUT_SYNTHESIZER_EN (&zl_reg_output_synthesizer_en)
#define ZL_REG_DPLL_LOCK_SELECTION (&zl_reg_dpll_lock_selection)
#define ZL_REG_CENTRAL_FREQ_OFFSET (&zl_reg_central_freq_offset)
#define ZL_REG_SYNTH_FILTER_SEL (&zl_reg_synth_filter_sel)
#define ZL_REG_SYNTH_FILTER_PHASE_SHIFT (&zl_reg_synth_filter_phase_shift)
#define ZL_REG_PAGE_REGISTER (&zl_reg_page_register)
#define ZL_REG_SYNTH_POST_DIV_A (&zl_reg_synth_post_div_a)
#define ZL_REG_SYNTH_POST_DIV_B (&zl_reg_synth_post_div_b)
#define ZL_REG_HP_CMOS_EN (&zl_reg_hp_cmos_en)
#define ZL_REG_SYNTH_STOP_CLK (&zl_reg_synth_stop_clk)
#define ZL_REG_SYNC_FAIL_FLAG_STATUS (&zl_reg_sync_fail_flag_status)
#define ZL_REG_CLEAR_SYNC_FAIL_FLAG (&zl_reg_clear_sync_fail_flag)
#define ZL_REG_PHASE_SHIFT_S_POSTDIV_A (&zl_reg_phase_shift_s_postdiv_a)
#define ZL_REG_PHASE_SHIFT_S_POSTDIV_B (&zl_reg_phase_shift_s_postdiv_b)
#define ZL_REG_XO_OR_CRYSTAL_SEL (&zl_reg_xo_or_crystal_sel)
#define ZL_REG_CHIP_REVISION (&zl_reg_chip_revision)
#define ZL_REG_GPIO_FUNCTION_PIN0 (&zl_reg_gpio_function_pin0)
#define ZL_REG_GPIO_FUNCTION_PIN1 (&zl_reg_gpio_function_pin1)
#define ZL_REG_GPIO_FUNCTION_PIN2 (&zl_reg_gpio_function_pin2)
#define ZL_REG_GPIO_FUNCTION_PIN3 (&zl_reg_gpio_function_pin3)
#define ZL_REG_GPIO_FUNCTION_PIN4 (&zl_reg_gpio_function_pin4)
#define ZL_REG_GPIO_FUNCTION_PIN5 (&zl_reg_gpio_function_pin5)
#define ZL_REG_GPIO_FUNCTION_PIN6 (&zl_reg_gpio_function_pin6)
#define ZL_REG_DPLL_CTRL2 (&zl_reg_dpll_ctrl2)
#define ZL_REG_DPLL_HOLDPULL (&zl_reg_dpll_holdpull)
#define ZL_REG_PFM_MASK_HO (&zl_reg_pfm_mask_ho)
#define ZL_REG_PFM_MASK_REF_FAIL (&zl_reg_pfm_mask_ref_fail)
#define ZL_REG_PFM_RANGE_REF (&zl_reg_pfm_range_ref)


// Valid configuration/status values for the control registers.

// Any registers which have a boolean value will not have defines here.
// Refer to the datasheet for polarity.

#define ZL_DEF_ID_CHIP_ID_VALUE (0x9U)
#define ZL_DEF_ID_CHIP_REVISION_REV_E (0x3U)
#define ZL_DEF_ID_CHIP_REVISION_SEE_C6 (0x0U)

#define ZL_DEF_GST_DISQUAL_TIMER_MINIMUM (0x0U)
#define ZL_DEF_GST_DISQUAL_TIMER_10MS (0x1U)
#define ZL_DEF_GST_DISQUAL_TIMER_50MS (0x2U)
#define ZL_DEF_GST_DISQUAL_TIMER_2500MS (0x3U)

#define ZL_DEF_GST_QUAL_TIMER_2X (0x0U)
#define ZL_DEF_GST_QUAL_TIMER_4X (0x1U)
#define ZL_DEF_GST_QUAL_TIMER_6X (0x2U)
#define ZL_DEF_GST_QUAL_TIMER_8X (0x3U)

#define ZL_DEF_REF_BASE_FREQ_1KHZ (0x03E8U)
#define ZL_DEF_REF_BASE_FREQ_2KHZ (0x07D0U)
#define ZL_DEF_REF_BASE_FREQ_5KHZ (0x1388U)
#define ZL_DEF_REF_BASE_FREQ_6_25KHZ (0x186AU)
#define ZL_DEF_REF_BASE_FREQ_8KHZ (0x1F40U)
#define ZL_DEF_REF_BASE_FREQ_10KHZ (0x2710U)
#define ZL_DEF_REF_BASE_FREQ_12_5KHZ (0x30D4U)
#define ZL_DEF_REF_BASE_FREQ_25KHZ (0x61A8U)
#define ZL_DEF_REF_BASE_FREQ_40KHZ (0x9C40U)

#define ZL_DEF_DPLL_PHASE_SLOPE_LIMIT_61000NSPS (0x0U)
#define ZL_DEF_DPLL_PHASE_SLOPE_LIMIT_7500NSPS (0x1U)
#define ZL_DEF_DPLL_PHASE_SLOPE_LIMIT_885NSPS (0x2U)
#define ZL_DEF_DPLL_PHASE_SLOPE_LIMIT_UNLIMITED (0x3U)

#define ZL_DEF_DPLL_LOOP_BANDWIDTH_14HZ (0x0U)
#define ZL_DEF_DPLL_LOOP_BANDWIDTH_28HZ (0x1U)
#define ZL_DEF_DPLL_LOOP_BANDWIDTH_56HZ (0x2U)
#define ZL_DEF_DPLL_LOOP_BANDWIDTH_112HZ (0x3U)
#define ZL_DEF_DPLL_LOOP_BANDWIDTH_224HZ (0x4U)
#define ZL_DEF_DPLL_LOOP_BANDWIDTH_448HZ (0x5U)
#define ZL_DEF_DPLL_LOOP_BANDWIDTH_896HZ (0x6U)
#define ZL_DEF_DPLL_LOOP_BANDWIDTH_30MHZ (0x7U)

#define ZL_DEF_DPLL_MODE_FREERUN (0x0U)
#define ZL_DEF_DPLL_MODE_HOLDOVER (0x1U)
#define ZL_DEF_DPLL_MODE_AUTOMATIC (0x3U)

#define ZL_DEF_PHASEMEM_LIMIT_10US (0x00U)
#define ZL_DEF_PHASEMEM_LIMIT_100US (0x20U)
#define ZL_DEF_PHASEMEM_LIMIT_1MS (0x40U)
#define ZL_DEF_PHASEMEM_LIMIT_930MS (0x9FU)

#define ZL_DEF_MON_REF_LIMIT_0_1_PCT (0x0U)
#define ZL_DEF_MON_REF_LIMIT_0_5_PCT (0x1U)
#define ZL_DEF_MON_REF_LIMIT_1_PCT (0x2U)
#define ZL_DEF_MON_REF_LIMIT_2_PCT (0x3U)
#define ZL_DEF_MON_REF_LIMIT_5_PCT (0x4U)
#define ZL_DEF_MON_REF_LIMIT_10_PCT (0x5U)
#define ZL_DEF_MON_REF_LIMIT_20_PCT (0x6U)
#define ZL_DEF_MON_REF_LIMIT_50_PCT (0x7U)

#define ZL_DEF_SYNTH_BASE_FREQ_5KHZ (0x1388U)
#define ZL_DEF_SYNTH_BASE_FREQ_6_25KHZ (0x186AU)
#define ZL_DEF_SYNTH_BASE_FREQ_8KHZ (0x1F40U)
#define ZL_DEF_SYNTH_BASE_FREQ_10KHZ (0x2710U)
#define ZL_DEF_SYNTH_BASE_FREQ_12_5KHZ (0x30D4U)
#define ZL_DEF_SYNTH_BASE_FREQ_25KHZ (0x61A8U)
#define ZL_DEF_SYNTH_BASE_FREQ_40KHZ (0x9C40U)

#define ZL_DEF_STOP_CLOCK_CONTINUOUS (0x0U)
#define ZL_DEF_STOP_CLOCK_FALLING (0x2U)
#define ZL_DEF_STOP_CLOCK_RISING (0x3U)

#define ZL_DEF_QUAD_SHIFT_0 (0x0U)
#define ZL_DEF_QUAD_SHIFT_N45 (0x1U)
#define ZL_DEF_QUAD_SHIFT_N90 (0x2U)
#define ZL_DEF_QUAD_SHIFT_N135 (0x3U)
#define ZL_DEF_QUAD_SHIFT_180 (0x4U)
#define ZL_DEF_QUAD_SHIFT_P135 (0x5U)
#define ZL_DEF_QUAD_SHIFT_P90 (0x6U)
#define ZL_DEF_QUAD_SHIFT_P45 (0x7U)

#define ZL_DEF_CHIP_REVISION_SEE_00 (0x00U)
#define ZL_DEF_CHIP_REVISION_REV_F (0x01U)
#define ZL_DEF_CHIP_REVISION_REF_H (0x03U)

#define ZL_DEF_GPIO_FUNCTION_ (0x0U)

#define ZL_DEF_GPIO_CON_SEL (0x0U)
#define ZL_DEF_GPIO_STAT_SEL (0x1U)

#define ZL_DEF_DPLL_HOLDOVER_STORAGE_10MS (0x00U)
#define ZL_DEF_DPLL_HOLDOVER_STORAGE_100MS (0x08U)
#define ZL_DEF_DPLL_HOLDOVER_STORAGE_1S (0x10U)
#define ZL_DEF_DPLL_HOLDOVER_STORAGE_42_2S (0x1DU)

#define ZL_DEF_DPLL_INPUT_EDGE_RISING (0x0U)
#define ZL_DEF_DPLL_INPUT_EDGE_FALLING (0x1U)
#define ZL_DEF_DPLL_INPUT_PULSE_LOW (0x2U)
#define ZL_DEF_DPLL_INPUT_PULSE_HIGH (0x3U)

#define ZL_DEF_DPLL_HOLD_IN_RANGE_12PPM (0x0U)
#define ZL_DEF_DPLL_HOLD_IN_RANGE_52PPM (0x1U)
#define ZL_DEF_DPLL_HOLD_IN_RANGE_83PPM (0x2U)
#define ZL_DEF_DPLL_HOLD_IN_RANGE_130PPM (0x3U)
#define ZL_DEF_DPLL_HOLD_IN_RANGE_400PPM (0x4U)
#define ZL_DEF_DPLL_HOLD_IN_RANGE_UNLIMITED (0x7U)

#define ZL_DEF_PFM_ACCEPT_9_2PPM_REJECT_12PPM (0x0U)
#define ZL_DEF_PFM_ACCEPT_40PPM_REJECT_52PPM (0x1U)
#define ZL_DEF_PFM_ACCEPT_64PPM_REJECT_83PPM (0x2U)
#define ZL_DEF_PFM_ACCEPT_100PPM_REJECT_130PPM (0x3U)
#define ZL_DEF_PFM_ACCEPT_13_8PPM_REJECT_18PPM (0x4U)
#define ZL_DEF_PFM_ACCEPT_24_6PPM_REJECT_32PPM (0x5U)
#define ZL_DEF_PFM_ACCEPT_33_6PPM_REJECT_47_5PPM (0x6U)
#define ZL_DEF_PFM_ACCEPT_52PPM_REJECT_67_5PPM (0x7U)


#ifdef	__cplusplus
extern "C" {
#endif


// Register types.

typedef enum
{
	ZL_RSIZE_NONE = 0,
	ZL_RSIZE_8 = 1,
	ZL_RSIZE_16 = 2,
	ZL_RSIZE_24 = 3,
	ZL_RSIZE_32 = 4,
}
zl_rsize_t;

typedef enum
{
	ZL_RTYPE_READONLY,
	ZL_RTYPE_READWRITE,
	ZL_RTYPE_STICKYR,
}
zl_rtype_t;


typedef struct
{
	uint8_t address;
	zl_rsize_t size;
	zl_rtype_t type;
	uint32_t default_;
}
zl_register_t;


// Register access definitions.

extern const zl_register_t zl_reg_id_reg;
extern const zl_register_t zl_reg_ref_fail_isr_status;
extern const zl_register_t zl_reg_dpll_isr_status;
extern const zl_register_t zl_reg_ref_fail_isr_mask;
extern const zl_register_t zl_reg_dpll_isr_mask;
extern const zl_register_t zl_reg_ref_mon_fail;
extern const zl_register_t zl_reg_ref_mon_fail_mask;
extern const zl_register_t zl_reg_ref_config;
extern const zl_register_t zl_reg_gst_disqualif_time;
extern const zl_register_t zl_reg_gst_qualif_time;
extern const zl_register_t zl_reg_sticky_r_lock;
extern const zl_register_t zl_reg_ref_base_freq;
extern const zl_register_t zl_reg_ref_freq_multiple;
extern const zl_register_t zl_reg_ref_ratio_m_n;
extern const zl_register_t zl_reg_dpll_ctrl;
extern const zl_register_t zl_reg_dpll_mode_refsel;
extern const zl_register_t zl_reg_dpll_ref_fail_mask;
extern const zl_register_t zl_reg_dpll_hold_lock_fail;
extern const zl_register_t zl_reg_phasememlimit_ref;
extern const zl_register_t zl_reg_scm_cfm_limit_ref;
extern const zl_register_t zl_reg_dpll_config;
extern const zl_register_t zl_reg_synth_base_freq;
extern const zl_register_t zl_reg_synth_freq_multiple;
extern const zl_register_t zl_reg_synth_ratio_m_n;
extern const zl_register_t zl_reg_output_synthesizer_en;
extern const zl_register_t zl_reg_dpll_lock_selection;
extern const zl_register_t zl_reg_central_freq_offset;
extern const zl_register_t zl_reg_synth_filter_sel;
extern const zl_register_t zl_reg_synth_filter_phase_shift;
extern const zl_register_t zl_reg_page_register;
extern const zl_register_t zl_reg_synth_post_div_a;
extern const zl_register_t zl_reg_synth_post_div_b;
extern const zl_register_t zl_reg_hp_cmos_en;
extern const zl_register_t zl_reg_synth_stop_clk;
extern const zl_register_t zl_reg_sync_fail_flag_status;
extern const zl_register_t zl_reg_clear_sync_fail_flag;
extern const zl_register_t zl_reg_phase_shift_s_postdiv_a;
extern const zl_register_t zl_reg_phase_shift_s_postdiv_b;
extern const zl_register_t zl_reg_xo_or_crystal_sel;
extern const zl_register_t zl_reg_chip_revision;
extern const zl_register_t zl_reg_gpio_function_pin0;
extern const zl_register_t zl_reg_gpio_function_pin1;
extern const zl_register_t zl_reg_gpio_function_pin2;
extern const zl_register_t zl_reg_gpio_function_pin3;
extern const zl_register_t zl_reg_gpio_function_pin4;
extern const zl_register_t zl_reg_gpio_function_pin5;
extern const zl_register_t zl_reg_gpio_function_pin6;
extern const zl_register_t zl_reg_dpll_ctrl2;
extern const zl_register_t zl_reg_dpll_holdpull;
extern const zl_register_t zl_reg_pfm_mask_ho;
extern const zl_register_t zl_reg_pfm_mask_ref_fail;
extern const zl_register_t zl_reg_pfm_range_ref;

extern const zl_register_t * const zl_all_regs[];
extern const unsigned int zl_all_regs_count;


// Register data definitions.

// Registers that contain a single value which starts at the LSbit do not have
// a definition here, regardless of width, and should simply be treated as the
// most appropriate integer type.
// Some registers share a single datatype from this section when the data
// structure is the same.

typedef struct
{
	unsigned int chip_id : 5;
	unsigned int chip_revision : 2;
	unsigned int ready: 1;
}
zl_rt_id_reg_t;

typedef struct
{
	unsigned int holdover : 1;
	unsigned int lock : 1;
}
zl_rt_dpll_hold_lock_t;

typedef struct
{
	unsigned int los : 1;
	unsigned int scm : 1;
	unsigned int cfm : 1;
	unsigned int gst : 1;
}
zl_rt_ref_mon_fail_t;

typedef struct
{
	unsigned int pre_divider_enable : 1;
	unsigned int _ : 3;
	unsigned int diff_input_enable: 1;
}
zl_rt_ref_config_t;

typedef struct
{
	unsigned int denom_n : 16;
	unsigned int numer_m : 16;
}
zl_rt_ratio_m_n_t;

typedef struct
{
	unsigned int _ : 2;
	unsigned int phase_slope_limit : 2;
	unsigned int tie_clear_enable : 1;
	unsigned int loop_bandwidth : 3;
}
zl_rt_dpll_ctrl_t;

typedef struct
{
	unsigned int cfm_limit : 3;
	unsigned int _ : 1;
	unsigned int scm_limit : 3;
}
zl_rt_scm_cfm_limit_ref_t;

typedef struct
{
	unsigned int dpll_config : 1;
	unsigned int _ : 3;
	unsigned int phase_acquisition_enable : 1;
}
zl_rt_dpll_config_t;

typedef union
{
	struct
	{
		unsigned int pulse_rate : 16;
		unsigned int source : 2;
		unsigned int pulse_polarity : 1;
		unsigned int pulse_type : 1;
		unsigned int set_to_ff : 4;
	}
	frame;

	struct
	{
		unsigned int div : 16;
		unsigned int source : 2;
		unsigned int _ : 2;
		unsigned int set_to_ff : 4;
	}
	lfclk;

	struct
	{
		unsigned int div : 24;
	}
	div;
}
zl_rt_synth_post_div_t;

typedef struct
{
	unsigned int hpout0 : 1;
	unsigned int hpout1 : 1;
}
zl_rt_hp_cmos_en_t;

typedef struct
{
	unsigned int post_div_a : 2;
	unsigned int post_div_b : 2;
}
zl_rt_synth_stop_clock_t;

typedef struct
{
	signed int phase_shift : 13;
	unsigned int quad_shift : 3;
}
zl_rt_phase_shift_t;

typedef struct
{
	unsigned int table_address : 7;
	unsigned int con_or_stat_sel: 1;
}
zl_rt_gpio_function_t;

typedef struct
{
	unsigned int holdover_storage_delay : 5;
	unsigned int _ : 1;
	unsigned int input_edge_sensitivity: 2;
}
zl_rt_dpll_ctrl2_t;

typedef struct
{
	unsigned int _ : 4;
	unsigned int mask : 1;
}
zl_rt_pfm_mask_ho_t;


// Unified value type.

typedef union
{
	zl_rt_id_reg_t id_reg;
	zl_rt_dpll_hold_lock_t dpll_hold_lock;
	zl_rt_ref_mon_fail_t ref_mon_fail;
	zl_rt_ref_config_t ref_config;
	zl_rt_ratio_m_n_t ratio_m_n;
	zl_rt_dpll_ctrl_t dpll_ctrl;
	zl_rt_scm_cfm_limit_ref_t scm_cfm_limit_ref;
	zl_rt_dpll_config_t dpll_config;
	zl_rt_synth_post_div_t synth_post_div;
	zl_rt_hp_cmos_en_t hp_cmos_en;
	zl_rt_synth_stop_clock_t synth_stop_clock;
	zl_rt_phase_shift_t phase_shift;
	zl_rt_gpio_function_t gpio_function;
	zl_rt_dpll_ctrl2_t dpll_ctrl2;
	zl_rt_pfm_mask_ho_t pfm_mask_ho;
	uint8_t u8;
	uint16_t u16;
	int32_t i32;
	bool bool_;
	uint8_t raw[4];
}
zl_value_t;


#ifdef	__cplusplus
}
#endif

#endif	/* ZL30159_DEFS_H */
