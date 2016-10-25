#include "weather.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"

char result[128];

void getDefaultWeather(WEATHER* weather) {
	WEATHER_ELEMENT_T temp1, temp2, rh, press, ws, rf, am;

	weather->temperature1 = temp1;
	weather->humidity = rh;
	weather->temperature2 = temp2;
	weather->pressure = press;
	weather->windSpeed = ws;
	weather->rainFall = rf;
	weather->ambientLight = am;
}

char* createJSONFromWeather(WEATHER* weather) {
	char temp[16];

	strcpy(result, "{weather:{temperature1:");
	sprintf(temp, "%d", (int16_t)(weather->temperature1.current*10));
	strcat(result, temp);
	strcat(result, ",humidity:");
	sprintf(temp, "%d", (int16_t)weather->humidity.current);
	strcat(result, temp);
	strcat(result, ",temperature2:");
	sprintf(temp, "%d", (int16_t)(weather->temperature2.current*10));
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
	sprintf(temp, "%d", (int16_t)weather->rainFall.current);
	strcat(result, temp);
	strcat(result, "}}\r\n");
	return &result[0];
}
