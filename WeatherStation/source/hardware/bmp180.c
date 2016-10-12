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

void calculateUncompensatedTemperature(BMP180_Parameters* parameters, uint8_t* data){
	uint16_t ut = createUnsignedInt(data, 0);
	parameters->uncompensatedTemperature = ut;
}

/**
 * Convert uncompensated temperatures to true temperature
 * @pre the coefficients and the uncompensated temperature values are populated.
 */
float calculateTrueTemperature(BMP180_Parameters* parameters){
	int16_t x1, x2, b5;

	x1 = (parameters->uncompensatedTemperature - parameters->AC6) * parameters->AC5 / (1<<15);
	x2 = (parameters->MC * 1<<11)/(x1 + parameters->MD);
	b5 = x1+x2;
	return ((float)((b5 + 8)/(1<<4)))*0.1;
}

/**
 * Convert uncompensated pressure to true pressure
 * @pre the coefficients and the uncompensated pressure values are populated.
 */
float calculateTruePressure(BMP180_Parameters* parameters){

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


void testBMPCode(void){
	BMP180_Parameters parameters;
	parameters.AC1 = 408;
	parameters.AC2 = -72;
	parameters.AC3 = -14383;
	parameters.AC4 = 32741;
	parameters.AC5 = 32757;
	parameters.AC6 = 23153;
	parameters.B1 = 6190;
	parameters.B2 = 4;
	parameters.MB = -32768;
	parameters.MC = -8711;
	parameters.MD = 2868;

	parameters.uncompensatedTemperature = 27898;
	parameters.oss = 0;
	parameters.uncompensatedPressure = 23843;

	const float Temp = 15.0;
	const float Press = 69964;
	float calculatedTemp;
	float calulatedPressure;

	calculatedTemp = calculateTrueTemperature(&parameters);

}
