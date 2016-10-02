#ifndef tasks_h
/**
 * Heartbeat task to just blink an led 1 a second. This should be converted into a status task.
 * Where the color and blinks mean something.
 */
void heartbeat_task(void *pvParameters);

typedef enum {TEMPERATURE, HUMIDITY, PRESSURE, LIGHTSENSOR} weather;

typedef struct ASerialMessage
{
	weather messageType;
    char ucData[ 20 ];
}ASerialMessage;




/**
 * Task does the following.
 *
 * 1. Checks input queue to see if a read/write is requested.
 * 2. once the read/write is complete the data is put in the return queue specified by in the input queue object.
 *
 *
 * Input Queue object
 * 	   char address to read.
 * 	   char number of bytes to write
 * 	   char* to array two write.
 * 	   char number of bytes to read
 * 	   char* array to put read bytes in
 * 	   queue* the return queue to put the results in.
 */
void i2c_task(void *pvParameters);

void lightSensor_task(void *pvParameters);

void serial_task(void *pvParameters);

#endif
