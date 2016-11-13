
/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
//#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_uart_freertos.h"
#include "fsl_uart.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#include "serial.h"

uint8_t background_buffer[200];
uint8_t recv_buffer[16];

uart_rtos_handle_t handle;
struct _uart_handle t_handle;

struct rtos_uart_config uart_config = { .baudrate = 115200, .parity =
		kUART_ParityDisabled, .stopbits = kUART_OneStopBit, .buffer =
		background_buffer, .buffer_size = sizeof(background_buffer), };

volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */
/*******************************************************************************
 * Code
 ******************************************************************************/

void init_serial(void) {
	/* Initialize UART1 pins below */
	/* Ungate the port clock */
	CLOCK_EnableClock(kCLOCK_PortD);

	/* Affects PORTE_PCR2 register */
	PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt3);
	/* Affects PORTE_PCR3 register */
	PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt3);

	NVIC_SetPriority(SERIAL_UART_RX_TX_IRQn, 5);
	uart_config.srcclk = CLOCK_GetFreq(SERIAL_UART_CLKSRC);
	uart_config.base = SERIAL_UART;


	if (0 > UART_RTOS_Init(&handle, &t_handle, &uart_config)) {
		//PRINTF("Error during UART initialization.\r\n");
		vTaskSuspend(NULL);
	}
}

void send_serial(uint8_t* send_buffer) {
	UART_RTOS_Send(&handle, send_buffer, strlen(send_buffer));
	//UART_RTOS_Deinit(&handle);
}

void receive_serial(uint8_t* receive_buffer, uint8_t* number_of_bytes, size_t* currentByte) {
	UART_RTOS_Receive(&handle, &background_buffer, sizeof(background_buffer), currentByte);
}
