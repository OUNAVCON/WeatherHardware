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

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_uart_freertos.h"
#include "fsl_uart.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "tasks.h"
#include "../hardware/serial.h"
#include "../algorithms/weather.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
//#define DEMO_UART UART2
//#define DEMO_UART_CLKSRC SYS_CLK
//#define DEMO_UART_RX_TX_IRQn UART2_RX_TX_IRQn
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
const char *to_send = "Hello, welcome to my world!\r\n";
uint8_t background_buffer[32];
uint8_t recv_buffer[4];

uart_rtos_handle_t handle;
struct _uart_handle t_handle;

struct rtos_uart_config uart_config = { .baudrate = 115200, .parity =
		kUART_ParityDisabled, .stopbits = kUART_OneStopBit, .buffer =
		background_buffer, .buffer_size = sizeof(background_buffer), };

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Task weather task
 */
void weather_task(void *pvParameters) {
	WEATHER weather;
	extern QueueHandle_t weatherMessageQueue;
	AWeatherMessage pxRxedMessage;

	const TickType_t xDelay = 500 / portTICK_PERIOD_MS;

	char result[20];


	//TODO: config move to serial.c
	NVIC_SetPriority(DEMO_UART_RX_TX_IRQn, 5);
	uart_config.srcclk = CLOCK_GetFreq(DEMO_UART_CLKSRC);
	uart_config.base = DEMO_UART;

	init_serial();
	getDefaultWeather(&weather);

	if (0 > UART_RTOS_Init(&handle, &t_handle, &uart_config)) {
		//PRINTF("Error during UART initialization.\r\n");
		vTaskSuspend(NULL);
	}

	for (;;) {
		if (weatherMessageQueue != 0) {
			// Receive a message on the created queue.  Block for 10 ticks if a
			// message is not immediately available.
			if (xQueueReceive(weatherMessageQueue, &(pxRxedMessage),
					(TickType_t ) 10)) {

				switch (pxRxedMessage.messageType) {
				case TEMPERATURE:
					weather.temperature.current = pxRxedMessage.weather_data.current;
					//TODO: calculate max and min.
					break;
				case HUMIDITY:
					weather.humidity.current = pxRxedMessage.weather_data.current;
					break;
				case PRESSURE:
					weather.pressure.current = pxRxedMessage.weather_data.current;
					break;
				case LIGHTSENSOR:
					weather.ambientLight.current = pxRxedMessage.weather_data.current;
					break;
				case WINDSPEED:
					weather.windSpeed.current = pxRxedMessage.weather_data.current;
					break;
				case RAINFALL:
					weather.rainFall.current = pxRxedMessage.weather_data.current;
					break;
				}

				// pcRxedMessage now points to the struct AMessage variable posted
				// by vATask.
				/*if (0
						> UART_RTOS_Send(&handle, (uint8_t *) result,
								strlen(result))) {
					vTaskSuspend(NULL);
				}*/
			}
		}

		vTaskDelay(xDelay);
	}

	/*    do
	 {
	 error = UART_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer), &n);
	 if (n > 0)
	 {
	 //send back the received data
	 UART_RTOS_Send(&handle, (uint8_t *)recv_buffer, n);
	 }
	 } while (kStatus_Success == error);*/

	//UART_RTOS_Deinit(&handle);
	//vTaskSuspend(NULL);
}

