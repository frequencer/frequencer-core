/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for PMD5 pin ***/
#define PMD5_Get()               ((PORTE >> 5) & 0x1)
#define PMD5_PIN                  GPIO_PIN_RE5

/*** Macros for PMD6 pin ***/
#define PMD6_Get()               ((PORTE >> 6) & 0x1)
#define PMD6_PIN                  GPIO_PIN_RE6

/*** Macros for PMD7 pin ***/
#define PMD7_Get()               ((PORTE >> 7) & 0x1)
#define PMD7_PIN                  GPIO_PIN_RE7

/*** Macros for PLL_SCK pin ***/
#define PLL_SCK_Get()               ((PORTG >> 6) & 0x1)
#define PLL_SCK_PIN                  GPIO_PIN_RG6

/*** Macros for PLL_MISO pin ***/
#define PLL_MISO_Get()               ((PORTG >> 7) & 0x1)
#define PLL_MISO_PIN                  GPIO_PIN_RG7

/*** Macros for PLL_MOSI pin ***/
#define PLL_MOSI_Get()               ((PORTG >> 8) & 0x1)
#define PLL_MOSI_PIN                  GPIO_PIN_RG8

/*** Macros for PLL_CS pin ***/
#define PLL_CS_Get()               ((PORTG >> 9) & 0x1)
#define PLL_CS_PIN                  GPIO_PIN_RG9

/*** Macros for LED3 pin ***/
#define LED3_Set()               (LATBSET = (1<<5))
#define LED3_Clear()             (LATBCLR = (1<<5))
#define LED3_Toggle()            (LATBINV= (1<<5))
#define LED3_OutputEnable()      (TRISBCLR = (1<<5))
#define LED3_InputEnable()       (TRISBSET = (1<<5))
#define LED3_Get()               ((PORTB >> 5) & 0x1)
#define LED3_PIN                  GPIO_PIN_RB5

/*** Macros for LED2 pin ***/
#define LED2_Set()               (LATBSET = (1<<4))
#define LED2_Clear()             (LATBCLR = (1<<4))
#define LED2_Toggle()            (LATBINV= (1<<4))
#define LED2_OutputEnable()      (TRISBCLR = (1<<4))
#define LED2_InputEnable()       (TRISBSET = (1<<4))
#define LED2_Get()               ((PORTB >> 4) & 0x1)
#define LED2_PIN                  GPIO_PIN_RB4

/*** Macros for LED1 pin ***/
#define LED1_Set()               (LATBSET = (1<<3))
#define LED1_Clear()             (LATBCLR = (1<<3))
#define LED1_Toggle()            (LATBINV= (1<<3))
#define LED1_OutputEnable()      (TRISBCLR = (1<<3))
#define LED1_InputEnable()       (TRISBSET = (1<<3))
#define LED1_Get()               ((PORTB >> 3) & 0x1)
#define LED1_PIN                  GPIO_PIN_RB3

/*** Macros for REF_ANA pin ***/
#define REF_ANA_Get()               ((PORTB >> 2) & 0x1)
#define REF_ANA_PIN                  GPIO_PIN_RB2

/*** Macros for FE_FILTB pin ***/
#define FE_FILTB_Set()               (LATBSET = (1<<6))
#define FE_FILTB_Clear()             (LATBCLR = (1<<6))
#define FE_FILTB_Toggle()            (LATBINV= (1<<6))
#define FE_FILTB_OutputEnable()      (TRISBCLR = (1<<6))
#define FE_FILTB_InputEnable()       (TRISBSET = (1<<6))
#define FE_FILTB_Get()               ((PORTB >> 6) & 0x1)
#define FE_FILTB_PIN                  GPIO_PIN_RB6

/*** Macros for FE_FILTA pin ***/
#define FE_FILTA_Set()               (LATBSET = (1<<7))
#define FE_FILTA_Clear()             (LATBCLR = (1<<7))
#define FE_FILTA_Toggle()            (LATBINV= (1<<7))
#define FE_FILTA_OutputEnable()      (TRISBCLR = (1<<7))
#define FE_FILTA_InputEnable()       (TRISBSET = (1<<7))
#define FE_FILTA_Get()               ((PORTB >> 7) & 0x1)
#define FE_FILTA_PIN                  GPIO_PIN_RB7

/*** Macros for FE_DC pin ***/
#define FE_DC_Set()               (LATBSET = (1<<8))
#define FE_DC_Clear()             (LATBCLR = (1<<8))
#define FE_DC_Toggle()            (LATBINV= (1<<8))
#define FE_DC_OutputEnable()      (TRISBCLR = (1<<8))
#define FE_DC_InputEnable()       (TRISBSET = (1<<8))
#define FE_DC_Get()               ((PORTB >> 8) & 0x1)
#define FE_DC_PIN                  GPIO_PIN_RB8

/*** Macros for SW1 pin ***/
#define SW1_Set()               (LATBSET = (1<<11))
#define SW1_Clear()             (LATBCLR = (1<<11))
#define SW1_Toggle()            (LATBINV= (1<<11))
#define SW1_OutputEnable()      (TRISBCLR = (1<<11))
#define SW1_InputEnable()       (TRISBSET = (1<<11))
#define SW1_Get()               ((PORTB >> 11) & 0x1)
#define SW1_PIN                  GPIO_PIN_RB11
#define SW1_InterruptEnable()   (CNENBSET = (1<<11))
#define SW1_InterruptDisable()  (CNENBCLR = (1<<11))

/*** Macros for SW2 pin ***/
#define SW2_Set()               (LATBSET = (1<<12))
#define SW2_Clear()             (LATBCLR = (1<<12))
#define SW2_Toggle()            (LATBINV= (1<<12))
#define SW2_OutputEnable()      (TRISBCLR = (1<<12))
#define SW2_InputEnable()       (TRISBSET = (1<<12))
#define SW2_Get()               ((PORTB >> 12) & 0x1)
#define SW2_PIN                  GPIO_PIN_RB12
#define SW2_InterruptEnable()   (CNENBSET = (1<<12))
#define SW2_InterruptDisable()  (CNENBCLR = (1<<12))

/*** Macros for SW3 pin ***/
#define SW3_Set()               (LATBSET = (1<<13))
#define SW3_Clear()             (LATBCLR = (1<<13))
#define SW3_Toggle()            (LATBINV= (1<<13))
#define SW3_OutputEnable()      (TRISBCLR = (1<<13))
#define SW3_InputEnable()       (TRISBSET = (1<<13))
#define SW3_Get()               ((PORTB >> 13) & 0x1)
#define SW3_PIN                  GPIO_PIN_RB13
#define SW3_InterruptEnable()   (CNENBSET = (1<<13))
#define SW3_InterruptDisable()  (CNENBCLR = (1<<13))

/*** Macros for PMA1 pin ***/
#define PMA1_Get()               ((PORTB >> 14) & 0x1)
#define PMA1_PIN                  GPIO_PIN_RB14

/*** Macros for PMA0 pin ***/
#define PMA0_Get()               ((PORTB >> 15) & 0x1)
#define PMA0_PIN                  GPIO_PIN_RB15

/*** Macros for X20_PIC pin ***/
#define X20_PIC_Get()               ((PORTC >> 12) & 0x1)
#define X20_PIC_PIN                  GPIO_PIN_RC12

/*** Macros for USBID pin ***/
#define USBID_Get()               ((PORTF >> 3) & 0x1)
#define USBID_PIN                  GPIO_PIN_RF3

/*** Macros for DAC_SDA pin ***/
#define DAC_SDA_Get()               ((PORTF >> 4) & 0x1)
#define DAC_SDA_PIN                  GPIO_PIN_RF4

/*** Macros for DAC_SCL pin ***/
#define DAC_SCL_Get()               ((PORTF >> 5) & 0x1)
#define DAC_SCL_PIN                  GPIO_PIN_RF5

/*** Macros for PMCS2 pin ***/
#define PMCS2_Get()               ((PORTD >> 9) & 0x1)
#define PMCS2_PIN                  GPIO_PIN_RD9

/*** Macros for PMCS1 pin ***/
#define PMCS1_Get()               ((PORTD >> 11) & 0x1)
#define PMCS1_PIN                  GPIO_PIN_RD11

/*** Macros for DBG_RX pin ***/
#define DBG_RX_Get()               ((PORTD >> 0) & 0x1)
#define DBG_RX_PIN                  GPIO_PIN_RD0

/*** Macros for PLL_RST pin ***/
#define PLL_RST_Set()               (LATCSET = (1<<13))
#define PLL_RST_Clear()             (LATCCLR = (1<<13))
#define PLL_RST_Toggle()            (LATCINV= (1<<13))
#define PLL_RST_OutputEnable()      (TRISCCLR = (1<<13))
#define PLL_RST_InputEnable()       (TRISCSET = (1<<13))
#define PLL_RST_Get()               ((PORTC >> 13) & 0x1)
#define PLL_RST_PIN                  GPIO_PIN_RC13

/*** Macros for REF_PIC pin ***/
#define REF_PIC_Get()               ((PORTC >> 14) & 0x1)
#define REF_PIC_PIN                  GPIO_PIN_RC14

/*** Macros for DBG_TX pin ***/
#define DBG_TX_Get()               ((PORTD >> 1) & 0x1)
#define DBG_TX_PIN                  GPIO_PIN_RD1

/*** Macros for PMENB pin ***/
#define PMENB_Get()               ((PORTD >> 4) & 0x1)
#define PMENB_PIN                  GPIO_PIN_RD4

/*** Macros for PMRD pin ***/
#define PMRD_Get()               ((PORTD >> 5) & 0x1)
#define PMRD_PIN                  GPIO_PIN_RD5

/*** Macros for PLL_GPO6 pin ***/
#define PLL_GPO6_Set()               (LATFSET = (1<<0))
#define PLL_GPO6_Clear()             (LATFCLR = (1<<0))
#define PLL_GPO6_Toggle()            (LATFINV= (1<<0))
#define PLL_GPO6_OutputEnable()      (TRISFCLR = (1<<0))
#define PLL_GPO6_InputEnable()       (TRISFSET = (1<<0))
#define PLL_GPO6_Get()               ((PORTF >> 0) & 0x1)
#define PLL_GPO6_PIN                  GPIO_PIN_RF0

/*** Macros for PLL_GPO2 pin ***/
#define PLL_GPO2_Set()               (LATFSET = (1<<1))
#define PLL_GPO2_Clear()             (LATFCLR = (1<<1))
#define PLL_GPO2_Toggle()            (LATFINV= (1<<1))
#define PLL_GPO2_OutputEnable()      (TRISFCLR = (1<<1))
#define PLL_GPO2_InputEnable()       (TRISFSET = (1<<1))
#define PLL_GPO2_Get()               ((PORTF >> 1) & 0x1)
#define PLL_GPO2_PIN                  GPIO_PIN_RF1

/*** Macros for PMD0 pin ***/
#define PMD0_Get()               ((PORTE >> 0) & 0x1)
#define PMD0_PIN                  GPIO_PIN_RE0

/*** Macros for PMD1 pin ***/
#define PMD1_Get()               ((PORTE >> 1) & 0x1)
#define PMD1_PIN                  GPIO_PIN_RE1

/*** Macros for PMD2 pin ***/
#define PMD2_Get()               ((PORTE >> 2) & 0x1)
#define PMD2_PIN                  GPIO_PIN_RE2

/*** Macros for PMD3 pin ***/
#define PMD3_Get()               ((PORTE >> 3) & 0x1)
#define PMD3_PIN                  GPIO_PIN_RE3

/*** Macros for PMD4 pin ***/
#define PMD4_Get()               ((PORTE >> 4) & 0x1)
#define PMD4_PIN                  GPIO_PIN_RE4


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/

typedef enum
{
    GPIO_PORT_B = 0,
    GPIO_PORT_C = 1,
    GPIO_PORT_D = 2,
    GPIO_PORT_E = 3,
    GPIO_PORT_F = 4,
    GPIO_PORT_G = 5,
} GPIO_PORT;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/

typedef enum
{
    GPIO_PIN_RB0 = 0,
    GPIO_PIN_RB1 = 1,
    GPIO_PIN_RB2 = 2,
    GPIO_PIN_RB3 = 3,
    GPIO_PIN_RB4 = 4,
    GPIO_PIN_RB5 = 5,
    GPIO_PIN_RB6 = 6,
    GPIO_PIN_RB7 = 7,
    GPIO_PIN_RB8 = 8,
    GPIO_PIN_RB9 = 9,
    GPIO_PIN_RB10 = 10,
    GPIO_PIN_RB11 = 11,
    GPIO_PIN_RB12 = 12,
    GPIO_PIN_RB13 = 13,
    GPIO_PIN_RB14 = 14,
    GPIO_PIN_RB15 = 15,
    GPIO_PIN_RC12 = 28,
    GPIO_PIN_RC13 = 29,
    GPIO_PIN_RC14 = 30,
    GPIO_PIN_RC15 = 31,
    GPIO_PIN_RD0 = 32,
    GPIO_PIN_RD1 = 33,
    GPIO_PIN_RD2 = 34,
    GPIO_PIN_RD3 = 35,
    GPIO_PIN_RD4 = 36,
    GPIO_PIN_RD5 = 37,
    GPIO_PIN_RD9 = 41,
    GPIO_PIN_RD10 = 42,
    GPIO_PIN_RD11 = 43,
    GPIO_PIN_RE0 = 48,
    GPIO_PIN_RE1 = 49,
    GPIO_PIN_RE2 = 50,
    GPIO_PIN_RE3 = 51,
    GPIO_PIN_RE4 = 52,
    GPIO_PIN_RE5 = 53,
    GPIO_PIN_RE6 = 54,
    GPIO_PIN_RE7 = 55,
    GPIO_PIN_RF0 = 64,
    GPIO_PIN_RF1 = 65,
    GPIO_PIN_RF3 = 67,
    GPIO_PIN_RF4 = 68,
    GPIO_PIN_RF5 = 69,
    GPIO_PIN_RG6 = 86,
    GPIO_PIN_RG7 = 87,
    GPIO_PIN_RG8 = 88,
    GPIO_PIN_RG9 = 89,

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
    GPIO_PIN_NONE = -1

} GPIO_PIN;

typedef  void (*GPIO_PIN_CALLBACK) ( GPIO_PIN pin, uintptr_t context);

void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortInterruptEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortInterruptDisable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: Local Data types and Prototypes
// *****************************************************************************
// *****************************************************************************

typedef struct {

    /* target pin */
    GPIO_PIN                 pin;

    /* Callback for event on target pin*/
    GPIO_PIN_CALLBACK        callback;

    /* Callback Context */
    uintptr_t               context;

} GPIO_PIN_CALLBACK_OBJ;

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
    GPIO_PortWrite((GPIO_PORT)(pin>>4), (uint32_t)(0x1) << (pin & 0xF), (uint32_t)(value) << (pin & 0xF));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return (bool)(((GPIO_PortRead((GPIO_PORT)(pin>>4))) >> (pin & 0xF)) & 0x1);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (bool)((GPIO_PortLatchRead((GPIO_PORT)(pin>>4)) >> (pin & 0xF)) & 0x1);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInterruptEnable(GPIO_PIN pin)
{
    GPIO_PortInterruptEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInterruptDisable(GPIO_PIN pin)
{
    GPIO_PortInterruptDisable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

bool GPIO_PinInterruptCallbackRegister(
    GPIO_PIN pin,
    const   GPIO_PIN_CALLBACK callBack,
    uintptr_t context
);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H
