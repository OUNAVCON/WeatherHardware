#include "i2c.h"
/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
//#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_I2C_MASTER_BASEADDR I2C0
#define EXAMPLE_I2C_SLAVE_BASEADDR I2C1
#define I2C_MASTER_CLK_SRC I2C0_CLK_SRC
#define I2C_SLAVE_CLK_SRC I2C1_CLK_SRC
#define I2C_MASTER_IRQ I2C0_IRQn
#define I2C_SLAVE_IRQ I2C1_IRQn
#define I2C_MASTER_IRQHandler I2C0_IRQHandler
#define I2C_SLAVE_IRQHandler I2C1_IRQHandler
#define I2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define I2C_BAUDRATE 100000U

#define I2C_DATA_LENGTH 32U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile uint8_t g_slave_buff[I2C_DATA_LENGTH];
volatile uint8_t g_master_buff[I2C_DATA_LENGTH];
volatile uint8_t g_masterTxIndex = 0;
volatile uint8_t g_masterRxIndex = 0xFFU;
volatile uint8_t g_slaveTxIndex = 0xFFU;
volatile uint8_t g_slaveRxIndex = 0;
volatile bool g_masterReadBegin = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief only initialize the i2c.
 */
void I2C_init(void)
{
	/* Debug uart port mux config */
	/* Enable uart port clock */
	CLOCK_EnableClock(kCLOCK_PortB);
	/* Affects PORTE_PCR0 register */
	PORT_SetPinMux(PORTB, 2U, kPORT_MuxAlt2); //SCL
	/* Affects PORTE_PCR1 register */
	PORT_SetPinMux(PORTB, 3U, kPORT_MuxAlt2); //SDA

    EnableIRQ(I2C_MASTER_IRQ);

    //NVIC_SetPriority(I2C_MASTER_IRQ, 1);
}

void I2C_Read(char addressOfSlave, /**< [in] Address of the slave( with offset?). */
		char numOfBytesToSend, /**< [in] Number of bytes to send to slave to start read. (1 based) */
		char* bytesToSend, /**< [in] pointer to the array to send. */
		char numBytesToRead, /**< [in] the number of bytes to read. */
		char*bytesToRead /**< [out] pointer to the array of bytes read from the slave. */
		){

}
