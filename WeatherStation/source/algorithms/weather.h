#ifndef WEATHER_H
#define WEATHER_H

typedef struct{
	float current;
}WEATHER_ELEMENT_T;


typedef struct{
	WEATHER_ELEMENT_T temperature1;
	WEATHER_ELEMENT_T humidity;
	WEATHER_ELEMENT_T temperature2;
	WEATHER_ELEMENT_T pressure;
	WEATHER_ELEMENT_T windSpeed;
	WEATHER_ELEMENT_T rainFall;
	WEATHER_ELEMENT_T ambientLight;
}WEATHER;


typedef enum {TEMPERATURE1, HUMIDITY, TEMPERATURE2, PRESSURE, LIGHTSENSOR, WINDSPEED, RAINFALL} weather;

typedef struct
{
	weather messageType;
	WEATHER_ELEMENT_T weather_data;
}AWeatherMessage;


void getDefaultWeather(WEATHER* weather);

char* createJSONFromWeather(WEATHER* weather);

#endif
