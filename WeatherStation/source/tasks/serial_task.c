
/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
//#include "fsl_device_registers.h"
//#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_uart_freertos.h"
#include "fsl_uart.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "tasks.h"
#include "../hardware/serial.h"
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
 * @brief Task responsible for printing of "Hello world." message.
 */
void serial_task(void *pvParameters) {
  // int error;
  //  size_t n;
  const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
  char result[20];
  //NVIC_SetPriority(DEMO_UART_RX_TX_IRQn, 5);
  //uart_config.srcclk = CLOCK_GetFreq(DEMO_UART_CLKSRC);
  //uart_config.base = DEMO_UART;

  init_serial();

  if (0 > UART_RTOS_Init(&handle, &t_handle, &uart_config)) {
    //PRINTF("Error during UART initialization.\r\n");
    vTaskSuspend(NULL);
  }

  for (;;) {


        strcpy(result, "hello");
        strcat(result, "\r\n");
        // pcRxedMessage now points to the struct AMessage variable posted
        // by vATask.
        if (0
            > UART_RTOS_Send(&handle, (uint8_t *) result,
                strlen(result))) {
          //PRINTF("Error during UART send.\r\n");
          vTaskSuspend(NULL);
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

