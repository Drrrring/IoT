#ifndef C_TEST_MYFIFO_H
#define C_TEST_MYFIFO_H

#include "zf_common_headfile.h"
#include <stdio.h>

// 保存位置信息的结构体
typedef struct {
    float latitude;    // 纬度
    float gratitude;   // 经度
} location;

// 自己实现的fifo队列
typedef struct {
    location *buffer;                                                // 缓存指针
    uint32 head;                                                   // 缓存头指针 总是指向空的缓存
    uint32 end;                                                    // 缓存尾指针 总是指向非空缓存（缓存全空除外）
    uint32 num;                                                    // 最多可容纳的元素个数
} my_fifo_struct;

void my_fifo_init(my_fifo_struct *fifo, location *buffer_addr, uint32 num);

int my_fifo_write(my_fifo_struct *fifo, location *dat);

int my_fifo_read(my_fifo_struct *fifo, location *dat);

bool my_fifo_full(my_fifo_struct *fifo);

bool my_fifo_empty(my_fifo_struct *fifo);

#endif //C_TEST_MYFIFO_H
