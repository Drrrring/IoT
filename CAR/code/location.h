#ifndef _location_h
#define _location_h

#include "myfifo.h"
// typedef struct 
// {
//     float latitude;    // 纬度
//     float gratitude;   // 经度
// } location;

#define LOCATION_FLOAT_BUFFER_SIZE (101)

void location_fifo_init();
int location_fifo_write(location* dat);
int location_fifo_read(location* dat);

#endif