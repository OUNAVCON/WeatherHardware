

//#include "board.h"
//#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "clock_config.h"
#include "fsl_i2c.h"
#include "fsl_i2c_freertos.h"
#include "i2c.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


#define I2C_BAUDRATE (100000) /* 100K */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
i2c_rtos_handle_t master_rtos_handle;
i2c_master_config_t masterConfig;
i2c_master_transfer_t masterTX;
i2c_master_transfer_t masterRX;

i2c_master_handle_t g_m_handle;



/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief only initialize the i2c.
 */
void I2C_init(void)
{
	uint32_t sourceClock;
	status_t status;
	/* Debug uart port mux config */
	/* Enable uart port clock */
	CLOCK_EnableClock(kCLOCK_PortB);
	/* Affects PORTE_PCR0 register */
	PORT_SetPinMux(PORTB, 2U, kPORT_MuxAlt2); //SCL
	/* Affects PORTE_PCR1 register */
	PORT_SetPinMux(PORTB, 3U, kPORT_MuxAlt2); //SDA

    EnableIRQ(I2C_MASTER_IRQ);

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

	status = I2C_RTOS_Init(&master_rtos_handle, EXAMPLE_I2C_MASTER,
			&masterConfig, sourceClock);
	if (status != kStatus_Success) {
		//TODO: Should return status here.
	}

}

void I2C_Read(uint8_t addressOfSlave, /**< [in] Address of the slave( with offset?). */
		char numOfBytesToSend, /**< [in] Number of bytes to send to slave to start read. (1 based) */
		uint8_t* bytesToSend, /**< [in] pointer to the array to send. */
		char numBytesToRead, /**< [in] the number of bytes to read. */
		uint8_t* bytesToRead /**< [out] pointer to the array of bytes read from the slave. */
		){


	status_t status;

	memset(&masterTX, 0, sizeof(masterTX));
	masterTX.slaveAddress = addressOfSlave;
	masterTX.direction = kI2C_Write;
	masterTX.subaddress = 0;
	masterTX.subaddressSize = 0;
	masterTX.data = bytesToSend;
	masterTX.dataSize = numOfBytesToSend;
	masterTX.flags = kI2C_TransferDefaultFlag;

	memset(&masterRX, 0, sizeof(masterRX));
	masterRX.slaveAddress = addressOfSlave;
	masterRX.direction = kI2C_Read;
	masterRX.subaddress = 0;
	masterRX.subaddressSize = 0;
	masterRX.data = bytesToRead;
	masterRX.dataSize = numBytesToRead;
	masterRX.flags = kI2C_TransferDefaultFlag;

	/*!< kStatus_I2C_Busy - I2C is busy with current transfer. */
	/*!< kStatus_I2C_Idle - Bus is Idle. */
	/*!< kStatus_I2C_Nak - NAK received during transfer. */
	/*!< kStatus_I2C_ArbitrationLost - Arbitration lost during transfer. */
	/*!< kStatus_I2C_Timeout - Wait event timeout. */
	while (master_rtos_handle.async_status != kStatus_Success) {
		vTaskDelay(1); //wait one tick
	};

	do {
		status = I2C_RTOS_Transfer(&master_rtos_handle, &masterTX);
	} while (status == kStatus_I2C_Busy);

	//wait until the transer has completed.
	while (master_rtos_handle.async_status != kStatus_Success) {
		vTaskDelay(1); //wait one tick
	};

	//if only a tx was requested then return after the tx.
	if(numBytesToRead == 0){
		return;
	}

	for (uint32_t i = 0; i < numBytesToRead; i++) {
		bytesToRead[i] = 0;
	}

	do {
		status = I2C_RTOS_Transfer(&master_rtos_handle, &masterRX);
	} while (status == kStatus_I2C_Busy);

	while (master_rtos_handle.async_status != kStatus_Success) {
		vTaskDelay(1); //wait one tick
	};

}
