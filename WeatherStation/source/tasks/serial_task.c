/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
//#include "fsl_device_registers.h"
//#include "fsl_debug_console.h"
//#include "board.h"

#include "clock_config.h"
#include "fsl_ftm.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_gpio.h"

#include "tasks.h"
#include "../algorithms/weather.h"

#include "../hardware/DHT11.h"

AWeatherMessage pxRxedMessage;
extern QueueHandle_t weatherMessageQueue;

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
void dht_task(void *pvParameters) {

	const TickType_t xDelay = 10000 / portTICK_PERIOD_MS;

	init_DHT11();

	for (;;) {

		read_DHT11();

		while(!isReadComplete()){
			vTaskDelay(xDelay);
		}

		getDHT11TimeValues();

		convertBitsToBytes();

		if (isChecksumValid()) {
			//read was valid, send data.
			pxRxedMessage.weather_data.current = readDHT11_Temperature();
			pxRxedMessage.messageType = TEMPERATURE2;
			xQueueSend(weatherMessageQueue, (void * ) &pxRxedMessage,
					(TickType_t ) 1);
			vTaskDelay(5); //wait a few ticks to allow the queue to read this pointer.
			pxRxedMessage.weather_data.current = readDHT11_Humidity();
			pxRxedMessage.messageType = HUMIDITY;
			xQueueSend(weatherMessageQueue, (void * ) &pxRxedMessage,
					(TickType_t ) 1);
		}

		vTaskDelay(xDelay);
	}

}

