#ifndef WEATHER_H
#define WEATHER_H

typedef struct{
	float current;
}WEATHER_ELEMENT_T;


typedef struct{
	WEATHER_ELEMENT_T temperature;
	WEATHER_ELEMENT_T humidity;
	WEATHER_ELEMENT_T pressure;
	WEATHER_ELEMENT_T windSpeed;
	WEATHER_ELEMENT_T rainFall;
	WEATHER_ELEMENT_T ambientLight;
}WEATHER;


typedef enum {TEMPERATURE, HUMIDITY, PRESSURE, LIGHTSENSOR, WINDSPEED, RAINFALL} weather;

typedef struct
{
	weather messageType;
	WEATHER_ELEMENT_T weather_data;
}AWeatherMessage;


void getDefaultWeather(WEATHER* weather);

char* createJSONFromWeather(WEATHER* weather);

#endif
