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

#include "pin_mux.h"
#include "clock_config.h"

#include "tasks.h"

#include "../hardware/i2c.h"
#include "../hardware/bmp180.h"
#include "../algorithms/weather.h"

/*
 * TODO: read BMP
 * 1. get calibration data from BMP180
 * 2. periodically get updated data
 * 3. use an enum to toggle between pressure and temperature reads.
 */
void i2c_task(void *pvParameters) {

	const TickType_t xDelay = 5000 / portTICK_PERIOD_MS;
	uint8_t sendBuffer[10];
	uint8_t receiveBuffer[30];
	BMP180_Parameters parameters;
	parameters.oss = 0;
	I2C_init();
	float temp = 0.0;

	//start of eeprom memory address
	sendBuffer[0] = 0xAA;
	//Get calibration Data for BMP180
	I2C_Read(0x77U, 1, &sendBuffer[0], 22, &receiveBuffer[0]);
	convertCalibrationToCoefficients(&parameters, &receiveBuffer[0]);

	for (;;) {
		//initiate a temperature conversion
		sendBuffer[0] = 0xF4;
		sendBuffer[1] = 0x2E;
		I2C_Read(0x77U, 2, &sendBuffer[0], 0, NULL);
		//wait 4.5 ms
		vTaskDelay(1);
		//read adc conversion value.
		sendBuffer[0] = 0xF6;
		I2C_Read(0x77U, 1, &sendBuffer[0], 2, &receiveBuffer[0]);
		calculateUncompensatedTemperature(&parameters, &receiveBuffer[0]);
		temp = calculateTrueTemperature(&parameters);

		//testBMPCode();
		vTaskDelay(xDelay);

		//initiate a pressure conversion

		sendBuffer[0] = 0xF4;
		sendBuffer[1] = 0x34 + (parameters.oss << 6);
		I2C_Read(0x77U, 2, &sendBuffer[0], 0, NULL);
		//wait 4.5 ms
		vTaskDelay(1);
		//read adc conversion value.
		sendBuffer[0] = 0xF6;
		I2C_Read(0x77U, 1, &sendBuffer[0], 3, &receiveBuffer[0]);
		calculateUncompensatedPressure(&parameters, &receiveBuffer[0]);
		temp = calculateTruePressure(&parameters);
		vTaskDelay(xDelay);
	}
}
