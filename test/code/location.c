#include "location.h"

// 初始化location队列
void location_fifo_init() {
    fifo_init(&location_float_fifo, FIFO_DATA_32BIT, location_float_buffer, LOCATION_FLOAT_BUFFER_SIZE);
}

// 清除location队列中的数据
fifo_state_enum location_fifo_clear              (fifo_struct *fifo) {
    return fifo_clear(fifo);
}
uint32          location_fifo_used               (fifo_struct *fifo){
    zf_assert(fifo != NULL);
    return ((fifo->max - fifo->size) / 2);
}

// 从location队列中写入一个单精度浮点数（经度或纬度）
fifo_state_enum location_fifo_write_float       (fifo_struct *fifo, float* dat) {
    return fifo_write_buffer(fifo, dat, 1);
}

// 从location队列中读出一个单精度浮点数（经度或纬度）
fifo_state_enum location_fifo_read_float       (fifo_struct *fifo, float* dat) {
	
    return fifo_read_buffer(fifo, dat, &temp, FIFO_READ_ONLY);
}