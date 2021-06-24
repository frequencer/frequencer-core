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
#include "drivers/hang_here.h"
#include "drivers/zl30159.h"
#include "modbus/modbus.h"

#include "app.h"


#define MB_PLL_BASE (0x100U)
#define MB_PLL_RAW_BASE (0x200U)


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
bool modbus_read_pll_raw_callback (mb_reg_data_t* reg_data);
bool modbus_write_pll_raw_callback (mb_reg_data_t* reg_data);


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

	// PLL can be accessed via Read Holding Registers (0x03) and Write Multiple
	// Registers (0x10) Modbus functions in ASCII frame format over USB serial.
	// Note that we go by ADDRESSES always, not register numbers. If you have
	// to use a "number" add one to the address.
	// Also note that ONLY 123 registers can be read/written at once. This is
	// the Modbus spec... we can probably crank that as high as we want though
	// without much issue (adjust MODBUS_REGS_MULTI_MAX).
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
	// The raw reading/writing really screws with the PLL. Seems the burst mode
	// can't handle passing over an unimplemented address..?
	modbus_add_reg_handler(
		MB_PLL_RAW_BASE,  // Or add this constant to do direct SPI transactions.
		0x80,
		MB_RA_READ,
		modbus_read_pll_raw_callback
	);
	modbus_add_reg_handler(
		MB_PLL_RAW_BASE,
		0x80,
		MB_RA_WRITE,
		modbus_write_pll_raw_callback
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

// Read from PLL over SPI directly, bypassing PIC's driver.
// One modbus address = one byte.
bool
modbus_read_pll_raw_callback (mb_reg_data_t* reg_data)
{
	uint8_t spi_out[1] = { 0 };
	uint8_t spi_in[MODBUS_REGS_MULTI_MAX + 1] = { 0 };
	uint8_t bytes = reg_data->count + 1;
	uint8_t i;

	spi_out[0] = ((reg_data->address - MB_PLL_RAW_BASE) & 0x7F) | 0x80;

	if (!SPI2_WriteRead((void*)spi_out, 1U, (void*)spi_in, bytes))
	{
		HANG_HERE();
	}

	for (i = 0; i < reg_data->count; i++)
	{
		reg_data->data[i] = spi_in[i + 1];
	}

	return true;
}

// Write to PLL over SPI directly, bypassing PIC's driver.
// One modbus address = one byte. Modbus register values above 255 will be
// rejected.
bool
modbus_write_pll_raw_callback (mb_reg_data_t* reg_data)
{
	uint8_t spi_out[MODBUS_REGS_MULTI_MAX + 1] = { 0 };
	uint8_t bytes = reg_data->count + 1;
	uint8_t i;

	spi_out[0] = ((reg_data->address - MB_PLL_RAW_BASE) & 0x7F);

	for (i = 0; i < reg_data->count; i++)
	{
		if (reg_data->data[i] > 0xFF)
		{
			return false;
		}

		spi_out[i + 1] = reg_data->data[i];
	}

	if (!SPI2_WriteRead((void*)spi_out, bytes, NULL, 0U))
	{
		HANG_HERE();
	}

	return true;
}
