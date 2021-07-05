/*******************************************************************************
*  MPLAB Harmony Application Source File
*
*  Company:
*   Microchip Technology Inc.
*
*  File Name:
*   app.c
*
*  Summary:
*   This file contains the source code for the MPLAB Harmony application.
*
*  Description:
*   Wow, it's the main file
*******************************************************************************/

#include "definitions.h"
#include "drivers/counter.h"
#include "drivers/hang_here.h"
#include "drivers/zl30159.h"
#include "modbus/modbus.h"

#include "app.h"


#define MB_PLL_BASE (0x100U)
#define MB_PLL_GPIO_BASE (0x200U)
#define MB_DAC_RAW_BASE (0x300U)
#define MB_DAC_MAX_WRITE MODBUS_REGS_MULTI_MAX

#define DAC_I2C_ADDR (0b01100000)


/// Definitions

static app_states_t state;
static app_states_t next_state;
static volatile app_states_t isr_state;

static SYS_TIME_HANDLE led_timer_handle = SYS_TIME_HANDLE_INVALID;
static SYS_CONSOLE_HANDLE console_handle = SYS_CONSOLE_HANDLE_INVALID;

static unsigned int message_counter;


void sw1_callback (GPIO_PIN pin, uintptr_t context);
void sw2_callback (GPIO_PIN pin, uintptr_t context);
void led_timer_callback (uintptr_t context);
bool modbus_read_pll_callback (mb_reg_data_t* reg_data);
bool modbus_write_pll_callback (mb_reg_data_t* reg_data);
bool modbus_read_pll_gpio_callback (mb_reg_data_t* reg_data);
bool modbus_write_pll_gpio_callback (mb_reg_data_t* reg_data);
bool modbus_read_dac_raw_callback (mb_reg_data_t* reg_data);
bool modbus_write_dac_raw_callback (mb_reg_data_t* reg_data);


/// Main Functions

// Initialize application. Call once before calling app_task.
void
app_init (void)
{
	LED2_Set();

	state = APPS_INIT;
	next_state = APPS_NONE;
	isr_state = APPS_NONE;

	message_counter = 0;

	bool gpio_result = GPIO_PinInterruptCallbackRegister(
		SW1_PIN,
		sw1_callback,
		NULL
	);

	SW1_InterruptEnable();

	gpio_result &= GPIO_PinInterruptCallbackRegister(
		SW2_PIN,
		sw2_callback,
		NULL
	);
	SW2_InterruptEnable();

	if (false == gpio_result)
	{
		HANG_HERE();
	}

	led_timer_handle = SYS_TIME_TimerCreate(
		0,
		SYS_TIME_MSToCount(500),
		led_timer_callback,
		NULL,
		SYS_TIME_PERIODIC
	);

	if (SYS_TIME_HANDLE_INVALID == led_timer_handle)
	{
		HANG_HERE();
	}

	if (SYS_TIME_SUCCESS != SYS_TIME_TimerStart(led_timer_handle))
	{
		HANG_HERE();
	}

	console_handle = SYS_CONSOLE_HandleGet(SYS_CONSOLE_INDEX_0);

	if (SYS_CONSOLE_HANDLE_INVALID == console_handle)
	{
		HANG_HERE();
	}

	modbus_init();
	zl_init();
	counter_init();

	// PLL can be accessed via Read Holding Registers (0x03) and Write Multiple
	// Registers (0x10) Modbus functions in ASCII frame format over USB serial.
	// Note that we go by ADDRESSES always, not register numbers. If you have
	// to use a "number" add one to the address.
	// Also note that ONLY 123 registers can be read/written at once. This is
	// the Modbus spec.
	// Oh, and the responses aren't going to have the LRC set because I didn't
	// implement it, so ignore that.
	modbus_add_reg_handler(
		MB_PLL_BASE,  // The first register address in modbus.
		0x100,  // The number of contiguous registers starting there.
		MB_RA_READ,
		modbus_read_pll_callback
	);
	modbus_add_reg_handler(
		MB_PLL_BASE,  // You'll need to add this constant to the PLL address.
		0x100,
		MB_RA_WRITE,
		modbus_write_pll_callback
	);
	// This register lets you read/write the GPIO and reset line of the PLL.
	modbus_add_reg_handler(
		MB_PLL_GPIO_BASE,
		0x01,
		MB_RA_READ,
		modbus_read_pll_gpio_callback
	);
	modbus_add_reg_handler(
		MB_PLL_GPIO_BASE,
		0x01,
		MB_RA_WRITE,
		modbus_write_pll_gpio_callback
	);
	// These registers can read/write registers on the DAC. Higher level driver
	// is not yet implemented, so no protection against bad address/data.
	modbus_add_reg_handler(
		MB_DAC_RAW_BASE,
		0x18,
		MB_RA_READ,
		modbus_read_dac_raw_callback
	);
	modbus_add_reg_handler(
		MB_DAC_RAW_BASE,
		MB_DAC_MAX_WRITE,
		MB_RA_WRITE,
		modbus_write_dac_raw_callback
	);
}

// Main app task. Call as often as possible.
void
app_task (void)
{
	switch (state)
	{
		case APPS_INIT:
		{
			SYS_STATUS console_status = SYS_CONSOLE_Status(sysObj.sysConsole0);

			if (SYS_STATUS_READY == console_status)
			{
				printf("Press S2 to get started!\r\n");
				next_state = APPS_WAIT_USER_READY;
			}
			else if (SYS_STATUS_ERROR == console_status)
			{
				HANG_HERE();
			}
			else
			{
				next_state = APPS_INIT;
			}

			break;
		}

		case APPS_WAIT_USER_READY:
		{
			// Exit via SW2 ISR.
			next_state = APPS_WAIT_USER_READY;

			break;
		}

		case APPS_QUERY_PLL:
		{
			zl_value_t id_reg = zl_read_reg(ZL_REG_ID_REG);

			printf("PLL ID: 0x%02X (should be 0x89)\r\n", id_reg.u8);

			if (id_reg.id_reg.ready)
			{
				next_state = APPS_IDLE;
			}
			else
			{
				printf("PLL not ready. Press S2 to retry.\r\n");
				next_state = APPS_WAIT_USER_READY;
			}

			break;
		}

		case APPS_IDLE:
		{
			// Exit via SW1 ISR.
			next_state = APPS_IDLE;

			break;
		}

		case APPS_INT_SEND_MESSAGE:
		{
			printf("Hello World %d\r\n", message_counter);

			message_counter++;

			break;
		}

		case APPS_INT_TOGGLE_LED:
		{
			LED1_Toggle();

			break;
		}

		default:
		{
			HANG_HERE();

			break;
		}
	}

	if (APPS_NONE != isr_state)
	{
		state = isr_state;
		isr_state = APPS_NONE;
	}
	else
	{
		state = next_state;
		next_state = APPS_NONE;
	}

	if (APPS_INIT != state)
	{
		// General tasks for after init.
		modbus_task();
		counter_task();
	}
}


/// Callbacks

// SW1 pushbutton callback. Triggers hello world message.
void
sw1_callback (GPIO_PIN pin, uintptr_t context)
{
	if (SW1_PIN != pin)
	{
		HANG_HERE();
	}

	if (!SW1_Get())
	{
		isr_state = APPS_INT_SEND_MESSAGE;
	}
}

// SW2 pushbutton callback. Starts main app execution.
void
sw2_callback (GPIO_PIN pin, uintptr_t context)
{
	if (SW2_PIN != pin)
	{
		HANG_HERE();
	}

	if (!SW2_Get())
	{
		if (APPS_WAIT_USER_READY == state)
		{
			LED2_Clear();
			isr_state = APP_START_STATE;
		}
	}
}

// LED timer callback. Toggles LED1.
void
led_timer_callback (uintptr_t context)
{
	isr_state = APPS_INT_TOGGLE_LED;
}

// Read from PLL with automatic bank switching and any other conversion
// features that are part of the PIC's driver. This will avoid reading addresses
// that do not correspond to a known register, and also avoid reading registers
// that can't fully fit in the requested number of bytes.
// One modbus address = one byte.
bool
modbus_read_pll_callback (mb_reg_data_t* reg_data)
{
	uint16_t cur_addr = reg_data->address - MB_PLL_BASE;
	uint16_t end_addr = cur_addr + reg_data->count;  // exclusive
	unsigned int i, offset = 0;

	printf("Reading PLL registers 0x%02X - 0x%02X.\r\n", cur_addr, end_addr);

	while (cur_addr < end_addr)
	{
		const zl_register_t* pll_reg = zl_find_reg(cur_addr);
		zl_value_t value;

		if (NULL == pll_reg)
		{
			// No register at this address!
			reg_data->data[offset] = 0;
			cur_addr++;
			offset++;
			continue;
		}

		if ((end_addr - cur_addr) < pll_reg->size)
		{
			// Requested size less than register size!
			reg_data->data[offset] = 0;
			cur_addr++;
			offset++;
			continue;
		}

		value = zl_read_reg(pll_reg);

		// Flip endian back when sending, so byte order is same as on device.
		for (i = 0; i < pll_reg->size; i++)
		{
			reg_data->data[offset + i] = value.raw[(pll_reg->size - 1) - i];
		}

		cur_addr += pll_reg->size;
		offset += pll_reg->size;
	}

	return true;
}

// Write to PLL with automatic bank switching and any other conversion
// features that are part of the PIC's driver. This will avoid writing addresses
// that do not correspond to a known register, and also avoid writing registers
// that don't have the full number of bytes provided. It may also reject data
// that fails certain range checks.
// One modbus address = one byte. Modbus register values above 255 will be
// rejected.
bool
modbus_write_pll_callback (mb_reg_data_t* reg_data)
{
	uint16_t cur_addr = reg_data->address - MB_PLL_BASE;
	uint16_t end_addr = cur_addr + reg_data->count;  // exclusive
	unsigned int i, offset = 0;

	printf("Writing PLL registers 0x%02X - 0x%02X.\r\n", cur_addr, end_addr - 1);

	// Check no data more than 8 bits
	for (i = 0; i < reg_data->count; i++)
	{
		if (reg_data->data[i] > 0xFF)
		{
			return false;
		}
	}

	while (cur_addr < end_addr)
	{
		const zl_register_t* pll_reg = zl_find_reg(cur_addr);
		zl_value_t value;

		if (NULL == pll_reg)
		{
			// No register at this address!
			cur_addr++;
			offset++;
			continue;
		}

		if ((end_addr - cur_addr) < pll_reg->size)
		{
			// Requested size less than register size!
			return false;
		}

		// Flip endian when writing, because driver will flip it too.
		for (i = 0; i < pll_reg->size; i++)
		{
			value.raw[(pll_reg->size - 1) - i] = reg_data->data[offset + i];
		}

		if (!zl_write_reg(pll_reg, value))
		{
			return false;
		}

		cur_addr += pll_reg->size;
		offset += pll_reg->size;
	}

	return true;
}

// Write PLL GPIO input, and direction.
// Implements a single address. Upper byte is direction (0 = out), lower byte
// is GPIO input value (1 = high) regardless of direction. Input value should
// match what latch is set to unless there's a short circuit or something.
// Bit 15/7 is PLL_RST.
// Bit 14/6 is PLL_GPO6.
// Bit 10/2 is PLL_GPO2.
// Other bits direction/value is undefined.
bool
modbus_read_pll_gpio_callback (mb_reg_data_t* reg_data)
{
	app_pll_gpio_t out = { .word = 0 };

	out.pll.rst_dir = 0;
	out.pll.gpo6_dir = PLL_GPO6_Direction();
	out.pll.gpo2_dir = PLL_GPO2_Direction();
	out.pll.rst_val = PLL_RST_Get();
	out.pll.gpo6_val = PLL_GPO6_Get();
	out.pll.gpo2_val = PLL_GPO2_Get();

	reg_data->data[0] = out.word;

	return true;
}

// Write PLL GPIO latch and direction.
// Implements a single address. Upper byte is direction (0 = out), lower byte
// is output latch (1 = high). Direction is processed before latch. Latch is
// ignored for bits with direction = 0.
// Bit 15/7 is PLL_RST. Reset direction (bit 15) cannot be set to input/1.
// Bit 14/6 is PLL_GPO6.
// Bit 10/2 is PLL_GPO2.
// If reset latch is set low, and reset direction is set out in same write,
// then PLL reset sequence will happen. Otherwise reset pin will actually be
// high (not-reset state). No need to set reset high again manually.
// Other bits direction/latch is ignored.
bool
modbus_write_pll_gpio_callback (mb_reg_data_t* reg_data)
{
	app_pll_gpio_t in;

	in.word = reg_data->data[0];

	if (!in.pll.rst_dir && !in.pll.rst_val)
	{
		zl_init();
	}

	if (in.pll.gpo6_dir)
	{
		PLL_GPO6_InputEnable();
	}
	else
	{
		PLL_GPO6_OutputEnable();
		GPIO_PinWrite(PLL_GPO6_PIN, in.pll.gpo6_val);
	}

	if (in.pll.gpo2_dir)
	{
		PLL_GPO2_InputEnable();
	}
	else
	{
		PLL_GPO2_OutputEnable();
		GPIO_PinWrite(PLL_GPO2_PIN, in.pll.gpo2_val);
	}

	return true;
}

// Read from DAC over I2C directly.
// One modbus address = one byte.
// All DAC bytes are read every time due to the interface design.
// See MCP4728 DS Figure 5-15 for data organization.
bool
modbus_read_dac_raw_callback (mb_reg_data_t* reg_data)
{
	uint8_t i2c_in[24] = { 0 };
	uint8_t bytes = 24;
	uint8_t i;

	while (I2C5_IsBusy())
	{
	}

	if (!I2C5_Read(DAC_I2C_ADDR, i2c_in, bytes))
	{
		HANG_HERE();
	}

	while (I2C5_IsBusy())
	{
	}

	for (i = 0; i < reg_data->count; i++)
	{
		reg_data->data[i] = i2c_in[i + (reg_data->address - MB_DAC_RAW_BASE)];
	}

	return true;
}

// Write to DAC over I2C directly.
// One modbus address = one byte. Modbus register values above 255 will be
// rejected.
// Start address must be equal to MB_DAC_RAW_BASE. Data will be written exactly
// as sent, for as many bytes as specified in the multi-register write, up to
// MB_DAC_MAX_WRITE. Device address is added automatically, other than that,
// write type and structure is defined by the user.
// Example: Write { 0x005E, 0x0093, 0x00E8 } sets channel D to 1.0V.
bool
modbus_write_dac_raw_callback (mb_reg_data_t* reg_data)
{
	uint8_t i2c_out[MB_DAC_MAX_WRITE] = { 0 };
	uint8_t bytes = reg_data->count;
	uint8_t i;

	if (reg_data->address != MB_DAC_RAW_BASE)
	{
		return false;
	}

	for (i = 0; i < reg_data->count; i++)
	{
		if (reg_data->data[i] > 0xFF)
		{
			return false;
		}

		i2c_out[i] = reg_data->data[i];
	}

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

	return true;
}
