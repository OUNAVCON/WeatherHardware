#ifndef tasks_h
#define tasks_h

/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
#define weather_task_PRIORITY 2
#define i2c_task_PRIORITY 3
#define capture_task_PRIORITY 3
#define dht_task_PRIORITY 3
#define lightSensor_task_PRIORITY 4

/* Task priorities. */
#define hello_task_STACK_SIZE configMINIMAL_STACK_SIZE
#define i2c_task_STACK_SIZE configMINIMAL_STACK_SIZE + 60
#define weather_task_STACK_SIZE configMINIMAL_STACK_SIZE + 256
#define capture_task_STACK_SIZE configMINIMAL_STACK_SIZE
#define dht_task_STACK_SIZE configMINIMAL_STACK_SIZE + 60
#define lightSensor_task_STACK_SIZE configMINIMAL_STACK_SIZE

void heartbeat_task(void *pvParameters);

void i2c_task(void *pvParameters);

void lightSensor_task(void *pvParameters);

void weather_task(void *pvParameters);

void capture_task(void *pvParameters);

void dht_task(void *pvParameters);

#endif
