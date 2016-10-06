#include "capture.h"

//#include "fsl_debug_console.h"
//#include "board.h"
#include "fsl_ftm.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "clock_config.h"

void capture_init(void){

	/* Debug uart port mux config */
	/* Enable uart port clock */
	CLOCK_EnableClock(kCLOCK_PortB);
	/* Affects PORTE_PCR0 register */
	PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt3); //FTM1_CH1

    //EnableIRQ(I2C_MASTER_IRQ);

    //NVIC_SetPriority(I2C_MASTER_IRQ, 1);

}
