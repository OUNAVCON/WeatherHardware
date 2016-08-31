#include "digitalOutput.h"
#include "board.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "clock_config.h"


void init_heartbeat_LED(){
	/* Define the init structure for the output LED pin*/
	gpio_pin_config_t led_config = { kGPIO_DigitalOutput, 0, };

	/* Init output LED GPIO. */
	GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, &led_config);
}

void toggle_heartbeat_LED(){
	GPIO_TogglePinsOutput(BOARD_LED_RED_GPIO, 1u << BOARD_LED_RED_GPIO_PIN);
}
