#include "capture.h"

//#include "fsl_debug_console.h"
//#include "board.h"
#include "fsl_common.h"
#include "fsl_ftm.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "clock_config.h"


/*!
 * @brief Interrupt service function of pulse inputs.
 *
 */

static int windSpeedCounter = 0;
static int rainFallCounter = 0;

void CAPTURE_IRQ_HANDLER(){
	uint32_t interruptFlags = PORT_GetPinsInterruptFlags(CAPTURE_GPIO_PORT);
	if(interruptFlags && 1<< WINDSPEED_GPIO_PIN){
		/* Clear external interrupt flag. */
		PORT_ClearPinsInterruptFlags(CAPTURE_GPIO_PORT, 1 << WINDSPEED_GPIO_PIN);
    	/* Change state of button. */
    	windSpeedCounter++;
	} else if(interruptFlags && 1<< RAINFALL_GPIO_PIN){
		/* Clear external interrupt flag. */
		PORT_ClearPinsInterruptFlags(CAPTURE_GPIO_PORT, 1 << RAINFALL_GPIO_PIN);
    	/* Change state of button. */
    	rainFallCounter++;
	}
}


void capture_init(void){

	/* Enable windspeed and rainfall input port clock */
	CLOCK_EnableClock(kCLOCK_PortB);

	port_pin_config_t capturePinConfig;
	//set pull down since interrupt is on rising edge.
	capturePinConfig.pullSelect = kPORT_PullDown;
	capturePinConfig.mux = kPORT_MuxAsGpio;
	//wind speed
	PORT_SetPinConfig(CAPTURE_GPIO_PORT, WINDSPEED_GPIO_PIN, &capturePinConfig);
	//rain fall
	PORT_SetPinConfig(CAPTURE_GPIO_PORT, RAINFALL_GPIO_PIN, &capturePinConfig);

	/* Affects PTB16 register */
	gpio_pin_config_t capture_config = { kGPIO_DigitalInput, 0 };

	/* windspeed pin init */
	GPIO_PinInit(CAPTURE_GPIO, WINDSPEED_GPIO_PIN, &capture_config);
	/* rainfall pin init */
	GPIO_PinInit(CAPTURE_GPIO, RAINFALL_GPIO_PIN, &capture_config);

	EnableIRQ(CAPTURE_IRQ);
	/* set windspeed interrupt pin pulse is a 50% duty-cycle pulse so edge to edge is a period. */
	PORT_SetPinInterruptConfig(CAPTURE_GPIO_PORT, WINDSPEED_GPIO_PIN, kPORT_InterruptRisingEdge);
	/* set rain fall interrupt pin a change in state indicates that a measure has been made.
	 * So a transition in either direction needs to be counted.
	 */
	PORT_SetPinInterruptConfig(CAPTURE_GPIO_PORT, RAINFALL_GPIO_PIN, kPORT_InterruptEitherEdge);

}

int readWindSpeedCounter(void){
	return windSpeedCounter;
}

int readRainFallCounter(void){
	return windSpeedCounter;
}

void clearWindSpeedCounter(void){
	windSpeedCounter = 0;
}

void clearRainFallCounter(void){
	rainFallCounter = 0;
}


