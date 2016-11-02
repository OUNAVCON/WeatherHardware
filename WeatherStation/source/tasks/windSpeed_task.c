/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


#include "pin_mux.h"
#include "clock_config.h"
#include "../hardware/capture.h"
#include "../hardware/adc.h"
#include "../algorithms/weather.h"

#include "tasks.h"

/* The software timer period. */
#define SW_TIMER_PERIOD_MS (1000 / portTICK_PERIOD_MS)
/* The callback function. */
static void SwTimerCallback(TimerHandle_t xTimer);

static float rainFallCountsPerSecond = 0.0;
static float windSpeedCountsPerSecond = 0.0;

/*
 * The capture_Init() sets up two gpio pins as inputs
 * and enables the port change to fire an interrupt.
 * The ISR increments a counter.
 */
void capture_task(void *pvParameters) {
	const TickType_t xDelay = 10000 / portTICK_PERIOD_MS;
	TimerHandle_t SwTimerHandle = NULL;
	AWeatherMessage pxRxedMessage;
	extern QueueHandle_t weatherMessageQueue;

	capture_init();

	/* Create the software timer. */
	SwTimerHandle = xTimerCreate("pulseRates", /* Text name. */
	SW_TIMER_PERIOD_MS, /* Timer period. */
	pdTRUE, /* Enable auto reload. */
	0, /* ID is not used. */
	SwTimerCallback); /* The callback function. */
	/* Start timer. */
	xTimerStart(SwTimerHandle, 0);

	for (;;) {

		pxRxedMessage.weather_data.current = windSpeedCountsPerSecond;
		pxRxedMessage.messageType = WINDSPEED;
		// Send a pointer to a struct AMessage object.  Block if the
		// queue is already full.
		xQueueSend(weatherMessageQueue, (void * ) &pxRxedMessage,
				(TickType_t ) 5);

		pxRxedMessage.weather_data.current = rainFallCountsPerSecond;
		pxRxedMessage.messageType = RAINFALL;
		xQueueSend(weatherMessageQueue, (void * ) &pxRxedMessage,
				(TickType_t ) 5);

		vTaskDelay(xDelay);
	}
}

/*!
 * @brief Software timer callback.
 */
static void SwTimerCallback(TimerHandle_t xTimer) {
	windSpeedCountsPerSecond = (float) readWindSpeedCounter();
	clearWindSpeedCounter();

	rainFallCountsPerSecond = (float) readRainFallCounter();
	clearRainFallCounter();
}
