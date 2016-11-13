#include "tasks.h"
#include "stdio.h"
/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "../hardware/adc.h"
#include "../algorithms/weather.h"

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
void lightSensor_task(void *pvParameters) {

	const TickType_t xDelay = 10000 / portTICK_PERIOD_MS;
	WEATHER_ELEMENT_T ambientLight;
	AWeatherMessage pxRxedMessage;
	extern QueueHandle_t weatherMessageQueue;
	int read = 0;
	pxRxedMessage.weather_data = ambientLight;

	init_lightSensor();

	for (;;) {
		//TODO: ADC should take successive measurements,
		// averages them, then use dma to transfer them to
		// where this task just reads the variables location.
		read = getLightSensorValue();
		pxRxedMessage.weather_data.current = (float)read;
		pxRxedMessage.messageType = LIGHTSENSOR;

		// Send a pointer to a struct AMessage object.  Don't block if the
		// queue is already full.
		xQueueSend(weatherMessageQueue, (void * ) &pxRxedMessage,
				(TickType_t ) 5);

		vTaskDelay(xDelay);
	}
}
