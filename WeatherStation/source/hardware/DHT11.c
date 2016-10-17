#include "DHT11.h"
#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

static bool readCompleteFlag = false;
DHT11_STATUS status = DHT11_READ_COMPLETE;
static DHT11_SENSOR_DATA dht11_Data;
static uint8_t data[5];

static void convertDataToDHTObject(uint8_t* raw){
	dht11_Data.humidity = (float)raw[0];
	dht11_Data.temperature = (float)((int8_t)raw[2]) + (float)(raw[3]/0xFFFF);
}
static bool isChecksumValid(uint8_t* raw) {
	uint8_t result = 0;
	for(int i=0; i=5; i<5){
		result+=raw[i];
	}
	return result==0 ? true : false;
}

void init_DHT11(void)
{
	dht11_Data.temperature = 0.0;
	dht11_Data.humidity = 0.0;
}

//Initiates a capture sequence.
void startDHT11_Read(void){
	status = DHT11_READ_IN_PROGRESS;
	taskENTER_CRITICAL();
	//read the DHT11 here.
	taskEXIT_CRITICAL();
	if(isChecksumValid(&data[0])){
		status = DHT11_READ_COMPLETE;
		convertDataToDHTObject(&data[0]);
	} else {
		status = DHT11_READ_FAILED;
	}



}

/*
 * read the data back. Though it is stored locally.
 */
void readDHT11_Data(DHT11_SENSOR_DATA* data){
	data->temperature = dht11_Data.temperature;
	data->humidity = dht11_Data.humidity;
}


//check to see if the read has completed and the data can be read.
bool isReadComplete(void){
	return readCompleteFlag;
}


