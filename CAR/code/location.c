#include "location.h"



// // 清除location队列中的数据
// fifo_state_enum location_fifo_clear              (fifo_struct *fifo) {
//     return fifo_clear(fifo);
// }
// uint32          location_fifo_used               (fifo_struct *fifo){
//     zf_assert(fifo != NULL);
//     return ((fifo->max - fifo->size) / 2);
// }

// // 从location队列中写入一个单精度浮点数（经度或纬度）
// // fifo_write_buffer()函数第三个参数length的单位为字节
// fifo_state_enum location_fifo_write_float       (float* dat) {
//     return fifo_write_buffer(&location_float_fifo, dat, 4);
// }

// // 从location队列中读出一个单精度浮点数（经度或纬度）
// fifo_state_enum location_fifo_read_float       (float* dat) {
	
//     return fifo_read_buffer(&location_float_fifo, dat, &temp, FIFO_READ_ONLY);
// }

static my_fifo_struct location_fifo;
static location location_float_buffer[LOCATION_FLOAT_BUFFER_SIZE];

// 初始化location队列
void location_fifo_init() {
    my_fifo_init(&location_fifo, location_float_buffer, LOCATION_FLOAT_BUFFER_SIZE - 1);
}

//以下位暴露出去的方法

// 向location fifo中写入数据
// 返回值： 0正常 -1队列满了 -2dat为空
int location_fifo_write(location* dat) {
    if (dat != NULL) {
        return my_fifo_write(&location_fifo, dat);
    }
    return -2;
}

int location_fifo_read(location* dat) {
    return my_fifo_read(&location_fifo, dat);
}