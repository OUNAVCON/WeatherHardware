#include "tasks.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "../hardware/digitalOutput.h"
#include "tasks/tasks.h"


/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
void heartbeat_task(void *pvParameters) {
	/*PRINTF("Hello world.\r\n");*/

	const TickType_t xDelay = 500 / portTICK_PERIOD_MS;

	init_heartbeat_LED();
		for(;;){
			toggle_heartbeat_LED();
			vTaskDelay(xDelay);
			}
}
