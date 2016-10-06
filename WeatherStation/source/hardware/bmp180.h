/*
 * bmp180.h
 *
 *  Created on: Oct 4, 2016
 *      Author: isaac
 */
#include "FreeRTOS.h"
#ifndef SOURCE_HARDWARE_BMP180_H_
#define SOURCE_HARDWARE_BMP180_H_

typedef enum BMP180_ADDRESS{id=0xD0, out_xlsb=0xF8, out_lsb=0xF7, out_msb=0xF6, ctrl_meas=0xF5, startMeasure=0xF4, calib0_21=0xAA }BMP180_ADDRESS;
typedef enum BMP180_CONTROL{START_TEMP_CONVERSION=0x2E, START_PRESS_CONVERSION=0x34}BMP180_CONTROL;
typedef enum BMP180_CONTROL_OSS{Val0=0}BMP180_CONTROL_OSS;

typedef struct {
	int16_t AC1; //0xAA,0xAB
	int16_t AC2; //0xAC,0xAD
	int16_t AC3; //0xAE,0xAF
	uint16_t AC4; //0xB0,0xB1
	uint16_t AC5; //0xB2,0xB3
	uint16_t AC6; //0xB4,0xB5
	int16_t B1; //0xB6,0xB7
	int16_t B2; //0xB8,0xB9
	int16_t MB; //0xBA,0xBB
	int16_t MC; //0xBC,0xBD
	int16_t MD; //0xBE,0xBF
	int32_t uncompensatedTemperature;
	int32_t uncompensatedPressure;
	int32_t trueTemperature;
	int32_t truePressure;

} BMP180_Parameters;

/**
 * Internal private functions
 */
uint16_t createUnsignedInt(uint8_t* data, int startByteAddress);
int16_t createSignedInt(uint8_t* data, int startByteAddress);

/**
 * After retrieving the array of coefficients convert them to coefficients.
 */
void convertCalibrationToCoefficients(BMP180_Parameters* parameters, uint8_t* data);

/**
 * Convert uncompensated temperatures to true temperature
 * @pre the coefficients and the uncompensated temperature values are populated.
 */
void calculateTrueTemperature(BMP180_Parameters* parameters);

/**
 * Convert uncompensated pressure to true pressure
 * @pre the coefficients and the uncompensated pressure values are populated.
 */
void calculateTruePressure(BMP180_Parameters* parameters);


#endif /* SOURCE_HARDWARE_BMP180_H_ */
