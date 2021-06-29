/*******************************************************************************
  SPI PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spi2_master.c

  Summary:
    SPI2 Master Source File

  Description:
    This file has implementation of all the interfaces provided for particular
    SPI peripheral.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018-2019 Microchip Technology Inc. and its subsidiaries.
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

#include "plib_spi2_master.h"

// *****************************************************************************
// *****************************************************************************
// Section: SPI2 Implementation
// *****************************************************************************
// *****************************************************************************


#define SPI2_CON_MSTEN                      (1 << _SPI2CON_MSTEN_POSITION)
#define SPI2_CON_CKP                        (0 << _SPI2CON_CKP_POSITION)
#define SPI2_CON_CKE                        (1 << _SPI2CON_CKE_POSITION)
#define SPI2_CON_MODE_32_MODE_16            (0 << _SPI2CON_MODE16_POSITION)
#define SPI2_CON_ENHBUF                     (1 << _SPI2CON_ENHBUF_POSITION)
#define SPI2_CON_MCLKSEL                    (0 << _SPI2CON_MCLKSEL_POSITION)
#define SPI2_CON_MSSEN                      (1 << _SPI2CON_MSSEN_POSITION)
#define SPI2_CON_SMP                        (0 << _SPI2CON_SMP_POSITION)

void SPI2_Initialize ( void )
{
    uint32_t rdata = 0U;

    /* Disable SPI2 Interrupts */
    IEC4CLR = 0x4000;
    IEC4CLR = 0x8000;
    IEC4CLR = 0x10000;

    /* STOP and Reset the SPI */
    SPI2CON = 0;

    /* Clear the Receiver buffer */
    rdata = SPI2BUF;
    rdata = rdata;

    /* Clear SPI2 Interrupt flags */
    IFS4CLR = 0x4000;
    IFS4CLR = 0x8000;
    IFS4CLR = 0x10000;

    /* BAUD Rate register Setup */
    SPI2BRG = 23;

    /* CLear the Overflow */
    SPI2STATCLR = _SPI2STAT_SPIROV_MASK;

    /*
    MSTEN = 1
    CKP = 0
    CKE = 1
    MODE<32,16> = 0
    ENHBUF = 1
    MSSEN = 1
    MCLKSEL = 0
    */
    SPI2CONSET = (SPI2_CON_MSSEN | SPI2_CON_MCLKSEL | SPI2_CON_ENHBUF | SPI2_CON_MODE_32_MODE_16 | SPI2_CON_CKE | SPI2_CON_CKP | SPI2_CON_MSTEN | SPI2_CON_SMP);

    /* Enable transmit interrupt when transmit buffer is completely empty (STXISEL = '01') */
    /* Enable receive interrupt when the receive buffer is not empty (SRXISEL = '01') */
    SPI2CONSET = 0x00000005;


    /* Enable SPI2 */
    SPI2CONSET = _SPI2CON_ON_MASK;
}

bool SPI2_TransferSetup (SPI_TRANSFER_SETUP* setup, uint32_t spiSourceClock )
{
    return false;
}

bool SPI2_Write(void* pTransmitData, size_t txSize)
{
    return(SPI2_WriteRead(pTransmitData, txSize, NULL, 0));
}

bool SPI2_Read(void* pReceiveData, size_t rxSize)
{
    return(SPI2_WriteRead(NULL, 0, pReceiveData, rxSize));
}

bool SPI2_WriteRead(void* pTransmitData, size_t txSize, void* pReceiveData, size_t rxSize)
{
    size_t txCount = 0;
    size_t rxCount = 0;
    uint32_t rxData;

    // Verify the request.
    if (((txSize <= 0) || (NULL == pTransmitData)) && ((rxSize <= 0) || (NULL == pReceiveData)))
    {
		return false;
	}

	if (pTransmitData == NULL)
	{
		txSize = 0;
	}
	if (pReceiveData == NULL)
	{
		rxSize = 0;
	}

	// Make sure the transmit buffer is empty.
	while(!SPI2STATbits.SPITBE)
	{
	}

	// Clear the receive overflow error, if any.
	SPI2STATCLR = _SPI2STAT_SPIROV_MASK;

	// Make sure the receive buffer is empty.
	while(!SPI2STATbits.SPIRBE)
	{
		rxData = SPI2BUF;
	}

	// Fail when word size is not 8 bits.
	if (SPI2CONbits.MODE16 || SPI2CONbits.MODE32)
	{
		return false;
	}

	// Loop until length of read and write are satisfied.
	while ((txCount < txSize) || (rxCount < rxSize))
	{
		// Stuff TX byte in FIFO when it's not full.
		if (!SPI2STATbits.SPITBF)
		{
			if (txCount < txSize)
			{
				SPI2BUF = ((uint8_t*)pTransmitData)[txCount];
				txCount++;
			}
			else if (txCount < rxSize)
			{
				SPI2BUF = 0x00;
				txCount++;
			}
		}

		// Read RX byte from FIFO when it's not empty.
		if (!SPI2STATbits.SPIRBE)
		{
			rxData = SPI2BUF;

			if (rxCount < rxSize)
			{
				((uint8_t*)pReceiveData)[rxCount] = rxData;
				rxCount++;
			}
		}
	}

	// Make sure no data is pending in the shift register.
	while (!SPI2STATbits.SRMT)
	{
	}

    return true;
}
