/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
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

/**
 * This is template for main module created by New Kinetis SDK 2.x Project Wizard. Enjoy!
 **/

#include <string.h>

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
/*#include "fsl_debug_console.h"*/

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "tasks/tasks.h"
#include "algorithms/weather.h"

QueueHandle_t weatherMessageQueue;


/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
#define weather_task_PRIORITY 2
#define capture_task_PRIORITY 3
#define dht_task_PRIORITY 3
#define lightSensor_task_PRIORITY 4
/*!
 * @brief Application entry point.
 */
int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();

  /* Create Queues */
  weatherMessageQueue = xQueueCreate( 5, sizeof( AWeatherMessage ) );

  if( weatherMessageQueue == NULL )
  {
      /* Queue was not created and must not be used. */
  }

  /* Create RTOS task */
  // use heartbeat with debug and maybe for future use.
  // xTaskCreate(heartbeat_task, "HB", configMINIMAL_STACK_SIZE, NULL, hello_task_PRIORITY, NULL);
  xTaskCreate(i2c_task, "px", configMINIMAL_STACK_SIZE + 60, NULL, hello_task_PRIORITY, NULL);
  xTaskCreate(lightSensor_task, "AmbL", configMINIMAL_STACK_SIZE + 60, NULL, hello_task_PRIORITY, NULL);
  xTaskCreate(weather_task, "WX", configMINIMAL_STACK_SIZE + 60, NULL, weather_task_PRIORITY, NULL);
  xTaskCreate(capture_task, "WS_RF", configMINIMAL_STACK_SIZE, NULL, capture_task_PRIORITY-1, NULL);
  xTaskCreate(dht_task, "RH", configMINIMAL_STACK_SIZE + 60, NULL, dht_task_PRIORITY-1, NULL);

  vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}



