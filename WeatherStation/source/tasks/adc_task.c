#include "tasks.h"
#include "stdio.h"
/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

//TODO: Remove this once the digitalOutput.c has been created.
#include "board.h"
#include "../hardware/adc.h"
#include "../algorithms/weather.h"



/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
void lightSensor_task(void *pvParameters) {

	const TickType_t xDelay = 5000 / portTICK_PERIOD_MS;
	WEATHER_ELEMENT_T ambientLight;
	AWeatherMessage pxRxedMessage;
	extern QueueHandle_t weatherMessageQueue;

	pxRxedMessage.weather_data = ambientLight;


	init_lightSensor();

	for (;;) {
		//TODO: ADC should take successive measurements,
		// aveages them, then use dma to transefer them to
		// where this task just reads the variables location.
		pxRxedMessage.weather_data.current = getLightSensorValue();
		//strncpy(&pxRxedMessage.ucData, &data, 20);
		//pxRxedMessage->ucData = data;
		pxRxedMessage.messageType = LIGHTSENSOR;

		    // Send a pointer to a struct AMessage object.  Don't block if the
		    // queue is already full.
		    xQueueSend( weatherMessageQueue, ( void * ) &pxRxedMessage, ( TickType_t ) 5 );

		vTaskDelay(xDelay);
	}
}
