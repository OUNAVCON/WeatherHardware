#ifndef CAPTURE_H
/*******************************************************************************
 * Definitions
 ******************************************************************************/


#define CAPTURE_GPIO GPIOB
#define CAPTURE_GPIO_PORT PORTB

#define WINDSPEED_GPIO_PIN 16U
#define RAINFALL_GPIO_PIN 18U

#define CAPTURE_IRQ PORTB_IRQn
#define CAPTURE_IRQ_HANDLER PORTB_IRQHandler

void capture_init(void);

int readWindSpeedCounter(void);
void clearWindSpeedCounter(void);

int readRainFallCounter(void);
void clearRainFallCounter(void);
#endif
