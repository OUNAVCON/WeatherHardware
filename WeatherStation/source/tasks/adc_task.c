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



/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
void lightSensor_task(void *pvParameters) {

	const TickType_t xDelay = 5000 / portTICK_PERIOD_MS;
	struct ASerialMessage pxRxedMessage;
	extern QueueHandle_t serialMessageQueue;
	int lightValue = 0;

	char data[20] = "";

	init_lightSensor();

	for (;;) {
		lightValue = getLightSensorValue();
		sprintf(pxRxedMessage.ucData,"%d",lightValue);
		//strncpy(&pxRxedMessage.ucData, &data, 20);
		//pxRxedMessage->ucData = data;
		pxRxedMessage.messageType = LIGHTSENSOR;

		    // Send a pointer to a struct AMessage object.  Don't block if the
		    // queue is already full.
		    xQueueSend( serialMessageQueue, ( void * ) &pxRxedMessage, ( TickType_t ) 5 );

			// ... Rest of task code.

		vTaskDelay(xDelay);
	}
}
