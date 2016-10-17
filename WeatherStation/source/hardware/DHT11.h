#ifndef DHT11_H
#define DHT11_H

#include <stdbool.h>

typedef enum {DHT11_READ_COMPLETE, DHT11_READ_IN_PROGRESS, DHT11_READ_FAILED} DHT11_STATUS ;

typedef struct {
	float temperature;
	float humidity;
}DHT11_SENSOR_DATA;

void init_DHT11(void);

//Initiates a capture sequence.
void startDHT11_Read(void);

/*
 * read the data back. Though it is stored locally.
 */
void readDHT11_Data(DHT11_SENSOR_DATA* data);


//check to see if the read has completed and the data can be read.
bool isReadComplete(void);


#endif
