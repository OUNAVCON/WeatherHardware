#include "bmp180.h"


/**
 * After retrieving the array of coefficients convert them to coefficients.
 */
void convertCalibrationToCoefficients(BMP180_Parameters* parameters, uint8_t* data){
 parameters->AC1 = createSignedInt(data, 0);
 parameters->AC2 = createSignedInt(data, 2);
 parameters->AC3 = createSignedInt(data, 4);
 parameters->AC4 = createUnsignedInt(data, 6);
 parameters->AC5 = createUnsignedInt(data, 8);
 parameters->AC6 = createUnsignedInt(data, 10);
 parameters->B1 = createSignedInt(data, 12);
 parameters->B2 = createSignedInt(data, 14);
 parameters->MB = createSignedInt(data, 16);
 parameters->MC = createSignedInt(data, 18);
 parameters->MD = createSignedInt(data, 20);
}

/**
 * Convert uncompensated temperatures to true temperature
 * @pre the coefficients and the uncompensated temperature values are populated.
 */
void calculateTrueTemperature(BMP180_Parameters* parameters){

}

/**
 * Convert uncompensated pressure to true pressure
 * @pre the coefficients and the uncompensated pressure values are populated.
 */
void calculateTruePressure(BMP180_Parameters* parameters){

}

/**
 * Internal private functions
 */
uint16_t createUnsignedInt(uint8_t* data, int startByteAddress){
	uint8_t upperValue = *(data+startByteAddress);
	uint8_t lowerValue = *(data+startByteAddress+1);
	uint16_t returnResult = upperValue;
	returnResult = returnResult <<8;
	returnResult += lowerValue;
return (returnResult);
}

int16_t createSignedInt(uint8_t* data, int startByteAddress){
	uint8_t upperValue = *(data+startByteAddress);
	uint8_t lowerValue = *(data+startByteAddress+1);
	uint16_t returnResult = upperValue;
	returnResult = returnResult <<8;
	returnResult += lowerValue;
return ((int16_t)returnResult);
}
