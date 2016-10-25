#ifndef DHT11_H
#define DHT11_H

#include <stdbool.h>
#include "FreeRTOS.h"

void init_DHT11(void);

//Initiates a capture sequence.
void DHT11_Read(void);

/*
 * read the data back. Though it is stored locally.
 */
void read_DHT11(void);


//check to see if the read has completed and the data can be read.
bool isReadComplete(void);

float readDHT11_Humidity(void);

float readDHT11_Temperature(void);

void convertBitsToBytes();

bool isChecksumValid();

void getDHT11TimeValues(void);

#endif
