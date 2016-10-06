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

/*  Standard C Included Files */
#include <string.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/*  SDK Included Files */
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "fsl_i2c_freertos.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "tasks.h"

#include "../hardware/i2c.h"
#include "../hardware/bmp180.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_I2C_MASTER_BASE I2C0_BASE
//#define EXAMPLE_I2C_SLAVE_BASE I2C1_BASE

#define EXAMPLE_I2C_MASTER ((I2C_Type *)EXAMPLE_I2C_MASTER_BASE)
//#define EXAMPLE_I2C_SLAVE ((I2C_Type *)EXAMPLE_I2C_SLAVE_BASE)

#if (EXAMPLE_I2C_MASTER_BASE == I2C0_BASE)
#define I2C_MASTER_CLK_SRC (I2C0_CLK_SRC)
#elif(EXAMPLE_I2C_MASTER_BASE == I2C1_BASE)
#define I2C_MASTER_CLK_SRC (I2C1_CLK_SRC)
#elif(EXAMPLE_I2C_MASTER_BASE == I2C2_BASE)
#define I2C_MASTER_CLK_SRC (I2C2_CLK_SRC)
#else
#error Should define the I2C_MASTER_CLK_SRC!
#endif

#define I2C_SLAVE_ADDR_7BIT (0xEEU)>>1
#define I2C_BAUDRATE (100000) /* 100K */
#define I2C_DATA_TX_LENGTH (1)  /* MAX is 256 */
#define I2C_DATA_RX_LENGTH (22)  /* MAX is 256 */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_send_buff[I2C_DATA_TX_LENGTH];
uint8_t g_receive_buff[I2C_DATA_RX_LENGTH];

i2c_master_handle_t g_m_handle;
i2c_slave_handle_t g_s_handle;

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
#define slave_task_PRIORITY (configMAX_PRIORITIES - 1)
#define master_task_PRIORITY (configMAX_PRIORITIES - 2)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/




void i2c_task(void *pvParameters)
{
	SemaphoreHandle_t transerSemaphore;
	const TickType_t xDelay = 10000 / portTICK_PERIOD_MS;
    i2c_rtos_handle_t master_rtos_handle;
    i2c_master_config_t masterConfig;
    i2c_master_transfer_t masterTX;
    i2c_master_transfer_t masterRX;

    uint32_t sourceClock;
    status_t status;
	struct ASerialMessage pxRxedMessage;
	extern QueueHandle_t serialMessageQueue;

	BMP180_Parameters parameters;
	I2C_init();



#if (EXAMPLE_I2C_MASTER_BASE == LPI2C0_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(LPI2C0_IRQn, 6);
    else
        NVIC_SetPriority(LPI2C0_IRQn, 3);
#elif(EXAMPLE_I2C_MASTER_BASE == LPI2C1_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(LPI2C1_IRQn, 6);
    else
        NVIC_SetPriority(LPI2C1_IRQn, 3);
#elif(EXAMPLE_I2C_MASTER_BASE == LPI2C2_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(LPI2C2_IRQn, 6);
    else
        NVIC_SetPriority(LPI2C2_IRQn, 3);
#elif(EXAMPLE_I2C_MASTER_BASE == I2C0_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(I2C0_IRQn, 6);
    else
        NVIC_SetPriority(I2C0_IRQn, 3);
#elif(EXAMPLE_I2C_MASTER_BASE == I2C1_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(I2C1_IRQn, 6);
    else
        NVIC_SetPriority(I2C1_IRQn, 3);
#elif(EXAMPLE_I2C_MASTER_BASE == I2C2_BASE)
    if (__CORTEX_M >= 0x03)
        NVIC_SetPriority(I2C2_IRQn, 6);
    else
        NVIC_SetPriority(I2C2_IRQn, 3);
#endif

    /* Set up i2c master to send data to slave */

     masterConfig.baudRate_Bps = 100000U;
     masterConfig.enableHighDrive = false;
     masterConfig.enableStopHold = false;
     masterConfig.glitchFilterWidth = 0U;
     masterConfig.enableMaster = true;

    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = I2C_BAUDRATE;
    sourceClock = CLOCK_GetFreq(I2C_MASTER_CLK_SRC);

    status = I2C_RTOS_Init(&master_rtos_handle, EXAMPLE_I2C_MASTER, &masterConfig, sourceClock);
    if (status != kStatus_Success)
    {
        //PRINTF("I2C master: error during init, %d", status);
    }

    g_send_buff[0] = 0xAA;
    memset(&masterTX, 0, sizeof(masterTX));
    masterTX.slaveAddress = I2C_SLAVE_ADDR_7BIT;
    masterTX.direction = kI2C_Write;
    masterTX.subaddress = 0xAA;
    masterTX.subaddressSize = 0;
    masterTX.data = g_send_buff;
    masterTX.dataSize = 1;
    masterTX.flags = kI2C_TransferDefaultFlag;

    memset(&masterRX, 0, sizeof(masterRX));
    masterRX.slaveAddress = I2C_SLAVE_ADDR_7BIT;
    masterRX.direction = kI2C_Read;
    masterRX.subaddress = (0xAA);
    masterRX.subaddressSize = 0;
    masterRX.data = g_receive_buff;
    masterRX.dataSize = I2C_DATA_RX_LENGTH;
    masterRX.flags = kI2C_TransferDefaultFlag;




    for(;;){


    	    /*!< kStatus_I2C_Busy - I2C is busy with current transfer. */
    	    /*!< kStatus_I2C_Idle - Bus is Idle. */
    	    /*!< kStatus_I2C_Nak - NAK received during transfer. */
    	    /*!< kStatus_I2C_ArbitrationLost - Arbitration lost during transfer. */
    	    /*!< kStatus_I2C_Timeout - Wait event timeout. */
    while(master_rtos_handle.async_status != kStatus_Success){
    	vTaskDelay(1); //wait one tick
    };

    do{
    	status = I2C_RTOS_Transfer(&master_rtos_handle, &masterTX);
    }
   	while(status == kStatus_I2C_Busy);

    //wait until the transer has completed.
    while(master_rtos_handle.async_status != kStatus_Success){
    	vTaskDelay(1); //wait one tick
    };


    for (uint32_t i = 0; i < I2C_DATA_RX_LENGTH; i++)
    {
        g_receive_buff[i] = 0;
    }

    do{
    	status = I2C_RTOS_Transfer(&master_rtos_handle, &masterRX);}
    while(status == kStatus_I2C_Busy);

    while(master_rtos_handle.async_status != kStatus_Success){
    	vTaskDelay(1); //wait one tick
    };

    if (status == kStatus_Success)
    {
    	convertCalibrationToCoefficients(&parameters, masterRX.data);
        //pxRxedMessage.messageType = TEMPERATURE;
        //strcpy(pxRxedMessage.ucData, (char*)masterRX.data);
        //xQueueSend( serialMessageQueue, ( void * ) &pxRxedMessage, ( TickType_t ) 5 );
    }


    vTaskDelay(xDelay);
}
}
