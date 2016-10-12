/*
 * weather.c
 *
 *  Created on: Oct 11, 2016
 *      Author: isaac
 */
#include "weather.h"
#include <string.h>
#include <stdio.h>

void getDefaultWeather(WEATHER* weather) {
	WEATHER_ELEMENT_T temp, rh, press, ws, rf, am;

	weather->temperature = temp;
	weather->humidity = rh;
	weather->pressure = press;
	weather->windSpeed = ws;
	weather->rainFall = rf;
	weather->ambientLight = am;

}

void createJSONFromWeather(WEATHER* weather, char* result) {
	char temp[10];

	strcpy(result, "{weather:{");
	strcat(result, "{temperature:");
	sprintf(temp, "3.2%f", weather->temperature.current);
	strcat(result, &temp[0]);
	strcat(result, "},");
	strcat(result, "{humidity:");
	sprintf(temp, "3.2%f", weather->humidity.current);
	strcat(result, &temp[0]);
	strcat(result, "},");
	strcat(result, "{pressure:");
	sprintf(temp, "3.2%f", weather->pressure.current);
	strcat(result, &temp[0]);
	strcat(result, "},");
	strcat(result, "{lightSensor:");
	sprintf(temp, "8.%f", weather->ambientLight.current);
	strcat(result, &temp[0]);
	strcat(result, "},");
	strcat(result, "{windSpeed:");
	sprintf(temp, "3.2%f", weather->windSpeed.current);
	strcat(result, &temp[0]);
	strcat(result, "},");
	strcat(result, "{rainFall:");
	sprintf(temp, "3.2%f", weather->rainFall.current);
	strcat(result, &temp[0]);
	strcat(result, "}}}");

}
