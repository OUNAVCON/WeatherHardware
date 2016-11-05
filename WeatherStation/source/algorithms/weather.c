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

	strcpy(result, "{\"wx\":{\"t1\":\"");
	sprintf(temp, "%d", (int16_t)(weather->temperature1.current*10));
	strcat(result, temp);
	strcat(result, "\",\"h\":\"");
	sprintf(temp, "%d", (int16_t)weather->humidity.current);
	strcat(result, temp);
	strcat(result, "\",\"t2\":\"");
	sprintf(temp, "%d", (int16_t)(weather->temperature2.current*10));
	strcat(result, temp);
	strcat(result, "\",\"p\":\"");
	sprintf(temp, "%d",(int16_t)( weather->pressure.current*1000));
	strcat(result, temp);
	strcat(result, "\",\"al\":\"");
	sprintf(temp, "%d", (int16_t)weather->ambientLight.current);
	strcat(result, temp);
	strcat(result, "\",\"ws\":\"");
	sprintf(temp, "%d", (int16_t)weather->windSpeed.current);
	strcat(result, temp);
	strcat(result, "\",\"rf\":\"");
	sprintf(temp, "%d", (int16_t)weather->rainFall.current);
	strcat(result, temp);
	strcat(result, "\"}}\r\n");
	return &result[0];
}
