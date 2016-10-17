
#ifndef SERIAL_H

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SERIAL_UART UART2
#define SERIAL_UART_CLKSRC BUS_CLK
#define SERIAL_UART_RX_TX_IRQn UART2_RX_TX_IRQn


void init_serial(void);

void send_serial(uint8_t* send_buffer);
void receive_serial(uint8_t* receive_buffer, uint8_t number_of_bytes);



#endif
