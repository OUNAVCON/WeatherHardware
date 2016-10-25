#ifndef tasks_h
#define tasks_h

void heartbeat_task(void *pvParameters);

void i2c_task(void *pvParameters);

void lightSensor_task(void *pvParameters);

void weather_task(void *pvParameters);

void capture_task(void *pvParameters);

void dht_task(void *pvParameters);

#endif
