#include "tasks.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


//TODO: Remove this once the digitalOutput.c has been created.
#include "board.h"


/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
void hello_task(void *pvParameters) {
	/*PRINTF("Hello world.\r\n");*/
	float x = 5;
	float y = 5;
	float theta = 0.0;
	const TickType_t xDelay = 500 / portTICK_PERIOD_MS;

		for(;;){
			//LED_BLUE_TOGGLE();
			//PRINTF("Hello world. %d\r\n", i);
			LED_BLUE_TOGGLE();
			theta = atan(y/x);

			vTaskDelay(xDelay);
			}
}
