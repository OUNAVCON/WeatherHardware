/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include <stdbool.h>
#include <stdio.h>

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "tasks.h"
#include "../hardware/serial.h"
#include "../algorithms/weather.h"

/* The software timer period. */
#define SW_TIMER_PERIOD_MS (60000 / portTICK_PERIOD_MS)
/* The callback function. */
static void SwTimerCallback(TimerHandle_t xTimer);

/* Stores the handle of the task that will be notified when the
timer has been called. */
static TaskHandle_t xTaskToNotify = NULL;

/*!
 * @brief Task weather task
 */
void weather_task(void *pvParameters) {
	WEATHER weather;
	extern QueueHandle_t weatherMessageQueue;
	AWeatherMessage pxRxedMessage;
	TimerHandle_t SwTimerHandle = NULL;
	uint32_t ulNotificationValue;
	const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
	char* weatherJSON;
	uint8_t receive_buffer;
	uint8_t number_of_bytes = 1;
	size_t currentByte;


	init_serial();
	getDefaultWeather(&weather);

	/* At this point xTaskToNotify should be NULL as no transmission
	    is in progress.  A mutex can be used to guard access to the
	    peripheral if necessary. */
	    configASSERT( xTaskToNotify == NULL );

	    /* Store the handle of the calling task. */
	    xTaskToNotify = xTaskGetCurrentTaskHandle();


	/* Create the software timer. */
	SwTimerHandle = xTimerCreate("sendSerial", /* Text name. */
	SW_TIMER_PERIOD_MS, /* Timer period. */
	pdTRUE, /* Enable auto reload. */
	0, /* ID is not used. */
	SwTimerCallback); /* The callback function. */
	/* Start timer. */
	xTimerStart(SwTimerHandle, 0);

	for (;;) {
		if (weatherMessageQueue != 0) {
			// Receive a message on the created queue.  Don't block, on process if a
			// message is not immediately available.
			if (xQueueReceive(weatherMessageQueue, &(pxRxedMessage), 0)) {

				switch (pxRxedMessage.messageType) {
				case TEMPERATURE1:
					weather.temperature1.current = pxRxedMessage.weather_data.current;
					break;
				case HUMIDITY:
					weather.humidity.current = pxRxedMessage.weather_data.current;
					break;
				case TEMPERATURE2:
					weather.temperature2.current = pxRxedMessage.weather_data.current;
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
			}
		}

		//receive_serial(&receive_buffer, &number_of_bytes, &currentByte);
		//send_serial(&receive_buffer);

	    /* Wait to be notified that the transmission is complete.  Note the first
	    parameter is pdTRUE, which has the effect of clearing the task's notification
	    value back to 0, making the notification value act like a binary (rather than
	    a counting) semaphore.  */
	    ulNotificationValue = ulTaskNotifyTake( pdTRUE, 0);

	    if( ulNotificationValue == 1 )
	    {
	    	weatherJSON = createJSONFromWeather(&weather);
			send_serial((uint8_t*)weatherJSON);
	    }
	    else
	    {
	        /* The call to ulTaskNotifyTake() timed out. */
	    }
	}
		vTaskDelay(xDelay);
}


/*!
 * @brief Software timer callback.
 */
static void SwTimerCallback(TimerHandle_t xTimer) {
    /* At this point xTaskToNotify should not be NULL as a transmission was
    in progress. */
    configASSERT( xTaskToNotify != NULL );

    /* Notify the task that the transmission is complete. */
    xTaskNotifyGive(xTaskToNotify);
}
