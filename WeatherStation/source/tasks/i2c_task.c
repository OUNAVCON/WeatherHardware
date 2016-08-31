#include "tasks.h"

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
void i2c_task(void *pvParameters){


}
