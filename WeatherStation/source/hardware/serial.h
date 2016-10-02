
#ifndef SERIAL_H
#include "fsl_uart_freertos.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_UART UART1
#define DEMO_UART_CLKSRC SYS_CLK
#define DEMO_UART_RX_TX_IRQn UART1_RX_TX_IRQn


void init_serial(void);

void send_serial(char a);




extern struct rtos_uart_config uart_config;
extern struct _uart_handle t_handle;
extern uart_rtos_handle_t handle;
extern uint8_t recv_buffer[4];
#endif
