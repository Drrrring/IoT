#include "myfifo.h"

void my_fifo_init(my_fifo_struct *fifo, location *buffer_addr, uint32 num){
    fifo->buffer = buffer_addr;
    fifo->head = 0;       // 指向第一个非空元素
    fifo->end = 0;        // 指向第一个空元素
    fifo->num = num;      // 最多可容纳的元素个数
}

int my_fifo_write(my_fifo_struct *fifo, location *dat){
    // 队列已满
    if (my_fifo_full(fifo)) {
        printf("my_fifo full. write error.\r\n");
        return -1;
    }
    fifo->buffer[fifo->end] = *dat;
    fifo->end = (fifo->end + 1) % fifo->num;
    return 0;
}

int my_fifo_read(my_fifo_struct *fifo, location *dat) {
    if (my_fifo_empty(fifo)) {
        printf("my_fifo empty. read error.\r\n");
        return -1;
    }
    *dat = fifo->buffer[fifo->head];
    fifo->head = (fifo->head + 1) % fifo->num;
    return 0;
}

bool my_fifo_full(my_fifo_struct *fifo){
    return (fifo->end + 1) % fifo->num == fifo->head;
}

bool my_fifo_empty(my_fifo_struct *fifo) {
    return fifo->end == fifo->head;
}