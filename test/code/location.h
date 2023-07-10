#ifndef _location_h
#define _location_h
#include "zf_common_headfile.h"

typedef struct 
{
    float latitude;    // 纬度
    float gratitude;   // 经度
} location;

#define LOCATION_FLOAT_BUFFER_SIZE (200)

static fifo_struct location_float_fifo;
static float location_float_buffer[LOCATION_FLOAT_BUFFER_SIZE];
static unsigned int temp = 1;

void location_fifo_init();
fifo_state_enum location_fifo_clear              (fifo_struct *fifo);
uint32          location_fifo_used               (fifo_struct *fifo);


fifo_state_enum location_fifo_write_float       (fifo_struct *fifo, float* dat);
fifo_state_enum location_fifo_read_float       (fifo_struct *fifo, float* dat);

fifo_state_enum location_fifo_read_tail_buffer   (fifo_struct *fifo, void *dat, uint32 *length, fifo_operation_enum flag);
#endif