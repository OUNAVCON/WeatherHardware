#include "DHT11.h"
#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

#include "clock_config.h"
#include "fsl_ftm.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_gpio.h"

#define DHT11_FTM_BASEADDR FTM1
#define DHT11_FTM_CHANNEL 0U

#define DHT11_IRQ FTM1_IRQn
#define DHT11_IRQ_HANDLER FTM1_IRQHandler

/* Get source clock for FTM driver */
#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)

const TickType_t xSample_Delay = 15000 / portTICK_PERIOD_MS;

ftm_config_t ftmInfo;
ftm_dual_edge_capture_param_t ftmParam;

uint16_t deltas[42];
uint8_t idx = 0;
uint8_t myCount = 41;
uint8_t results[5];
float temperature = 0.0;
float humidity = 0.0;

bool readCompleteStatus = false;

void DHT11_IRQ_HANDLER() {
	uint32_t interruptFlags = FTM_GetStatusFlags(DHT11_FTM_BASEADDR);
	uint32_t value1, value2;

	if (interruptFlags & kFTM_Chnl1Flag)
		value1 = DHT11_FTM_BASEADDR->CONTROLS[0].CnV;
	value2 = DHT11_FTM_BASEADDR->CONTROLS[1].CnV;

	value2 = (value2 < value1) ? value2 += 0xFFFF : value2;

	deltas[idx] = value2 - value1;
	FTM_ClearStatusFlags(DHT11_FTM_BASEADDR,
			kFTM_Chnl1Flag | kFTM_ChnlTriggerFlag | kFTM_TimeOverflowFlag);
	idx++;
	//FTM_StopTimer(DHT11_FTM_BASEADDR);
	//FTM_StartTimer(DHT11_FTM_BASEADDR, kFTM_SystemClock);
	//DHT11_FTM_BASEADDR->CNT = 0;
	if (idx > myCount) {
		FTM_StopTimer(DHT11_FTM_BASEADDR);
		FTM_ClearStatusFlags(DHT11_FTM_BASEADDR,
				kFTM_Chnl0Flag | kFTM_Chnl1Flag | kFTM_ChnlTriggerFlag
						| kFTM_TimeOverflowFlag);
		idx = 0;
		readCompleteStatus = true;
	}
}

void getDHT11TimeValues(void){
}

void convertBitsToBytes(){
	uint8_t i = 0;
	uint8_t offset = 2;
	uint8_t bitIndex = 7;
	uint8_t byteIndex = 0,
				temp = 0; //offset is the second element - the largest value is the start pulse.
		for (i = 0; i < 40; i++) { //40 bits 0-39
			//1800 is an average the (949, 2824)
			uint16_t bitPoint = deltas[i + offset];
			if (bitPoint > 1800) {
				temp |= 1 << bitIndex;
			}
			bitIndex--;
			if ((i + 1) % 8 == 0 && i > 0) {
				results[byteIndex] = temp;
				temp = 0;
				byteIndex++;
				bitIndex = 7;
			}
		}

 humidity = (float) results[0];
 temperature = (float)results[2] + ((float)results[3])/256.0;
}

static void delay(uint32_t clockCycles) {
	volatile uint32_t i = 0U;
	for (i = 0U; i < clockCycles; ++i) {
		__asm("NOP");
		/* delay */
	}
}


bool isChecksumValid() {
	uint8_t checksum = 0;
	uint8_t i = 0;
	for(i=0; i<4; i++){
		checksum += results[i];
	}
	return checksum == results[4] ? true : false;
}

void init_DHT11(void)
{
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_Ftm1);

	ftmParam.currChanEdgeMode = kFTM_RisingEdge;
	ftmParam.nextChanEdgeMode = kFTM_FallingEdge;
	ftmParam.mode = kFTM_Continuous;

	FTM_GetDefaultConfig(&ftmInfo);
	ftmInfo.prescale = kFTM_Prescale_Divide_1; //puts sys ftm clock at 80Mhz/64
	FTM_Init(DHT11_FTM_BASEADDR, &ftmInfo);
	DHT11_FTM_BASEADDR->CNTIN = 0x0000;
	DHT11_FTM_BASEADDR->MOD = 0xFFFF;
	EnableIRQ(DHT11_IRQ);
	FTM_EnableInterrupts(DHT11_FTM_BASEADDR, kFTM_Chnl1InterruptEnable);
}

//Initiates a capture sequence.
void read_DHT11(void){
	taskENTER_CRITICAL();
	PORT_SetPinMux(PORTB, DHT11_FTM_CHANNEL, kPORT_MuxAsGpio); //SCL
	gpio_pin_config_t DHT11_pin = { kGPIO_DigitalOutput, 0 };
	GPIO_PinInit(GPIOB, DHT11_FTM_CHANNEL, &DHT11_pin);
	GPIO_ClearPinsOutput(GPIOB, 1u << DHT11_FTM_CHANNEL);
	delay(160000); //Drive pin low for 20ms, 160000 based on clock speed of 80MHz
	GPIO_SetPinsOutput(GPIOB, 1u << DHT11_FTM_CHANNEL);
	/* Affects PORTE_PCR1 register */
	PORT_SetPinMux(PORTB, DHT11_FTM_CHANNEL, kPORT_MuxAlt3); //FTM1_CH0
	FTM_ClearStatusFlags(DHT11_FTM_BASEADDR,
			kFTM_Chnl0Flag | kFTM_Chnl1Flag | kFTM_ChnlTriggerFlag
					| kFTM_TimeOverflowFlag);
	FTM_SetupDualEdgeCapture(DHT11_FTM_BASEADDR, kFTM_Chnl_0, &ftmParam, 0);
	//DHT11_FTM_BASEADDR->CONTROLS[1].CnSC |= 0x01;
	FTM_StartTimer(DHT11_FTM_BASEADDR, kFTM_SystemClock);
	taskEXIT_CRITICAL();
	readCompleteStatus = false;
}

/*
 * read the data back. Though it is stored locally.
 */
float readDHT11_Temperature(void){
	return temperature;
}

float readDHT11_Humidity(void){
	return humidity;
}

bool isReadComplete(void){
	return readCompleteStatus;
}



