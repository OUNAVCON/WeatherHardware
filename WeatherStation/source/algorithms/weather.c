/*
 * weather.c
 *
 *  Created on: Oct 11, 2016
 *      Author: isaac
 */
#include "weather.h"
#include <string.h>
#include <stdio.h>
//#include <strings.h>
#include <stdlib.h>
#include "FreeRTOS.h"

void getDefaultWeather(WEATHER* weather) {
	WEATHER_ELEMENT_T temp, rh, press, ws, rf, am;

	weather->temperature = temp;
	weather->humidity = rh;
	weather->pressure = press;
	weather->windSpeed = ws;
	weather->rainFall = rf;
	weather->ambientLight = am;

}

char result[128];

char* createJSONFromWeather(WEATHER* weather) {
	char temp[16];

	strcpy(result, "{weather:{temperature:");
	sprintf(temp, "%d", (int16_t)(weather->temperature.current*10));
	strcat(result, temp);
	strcat(result, ",humidity:");
	sprintf(temp, "%d", (int16_t)weather->humidity.current);
	strcat(result, temp);
	strcat(result, ",pressure:");
	sprintf(temp, "%d",(int16_t)( weather->pressure.current*100));
	strcat(result, temp);
	strcat(result, ",ambientLight:");
	sprintf(temp, "%d", (int16_t)weather->ambientLight.current);
	strcat(result, temp);
	strcat(result, ",windSpeed:");
	sprintf(temp, "%d", (int16_t)weather->windSpeed.current);
	strcat(result, temp);
	strcat(result, ",rainFall:");
	sprintf(temp, "%f", weather->rainFall.current);
	strcat(result, temp);
	strcat(result, "}}\r\n");

	return &result[0];
}
