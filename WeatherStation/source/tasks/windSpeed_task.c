#include "tasks.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "fsl_ftm.h"

#include "pin_mux.h"
#include "clock_config.h"
//TODO: Remove this once the digitalOutput.c has been created.
//#include "board.h"
#include "../hardware/capture.h"


/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
void windSpeed_task(void *pvParameters) {
	capture_init();
    ftm_config_t ftmInfo;
    status_t status;
	uint32_t currentCapture;
	uint32_t statusCapture;
	const TickType_t xDelay = 500 / portTICK_PERIOD_MS;


	FTM_GetDefaultConfig(&ftmInfo);
    /* Initialize FTM module */
	ftmInfo.bdmMode = kFTM_BdmMode_3;
	ftmInfo.prescale = kFTM_Prescale_Divide_128;
	ftmInfo.faultMode = kFTM_Fault_Disable;
	ftmInfo.useGlobalTimeBase = true;
    status = FTM_Init(BOARD_FTM_BASEADDR, &ftmInfo);
    if(status != kStatus_Success){
    	FTM_Deinit(BOARD_FTM_BASEADDR);
    	for(;;){}
    }

	FTM_SetupInputCapture(BOARD_FTM_BASEADDR, BOARD_FIRST_FTM_CHANNEL, kFTM_FallingEdge,0);
	FTM_StartTimer(BOARD_FTM_BASEADDR, kFTM_SystemClock);
		for(;;){

			//status = FTM_GetStatusFlags(BOARD_FTM_BASEADDR);
			FTM_ClearStatusFlags(BOARD_FTM_BASEADDR, 0xFFFF);
			statusCapture = BOARD_FTM_BASEADDR->CONTROLS[BOARD_FIRST_FTM_CHANNEL].CnSC;
			currentCapture = BOARD_FTM_BASEADDR->CONTROLS[BOARD_FIRST_FTM_CHANNEL].CnV;

			vTaskDelay(xDelay);
			}
}
