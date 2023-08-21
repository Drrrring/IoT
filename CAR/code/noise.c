#include "noise.h"

uint8 uart_get_data[64]; // 串口接收数据缓冲区
uint8 fifo_get_data[64]; // fifo 输出读出缓冲区

uint8 get_data = 0;         // 接收数据变量
uint32 fifo_data_count = 0; // fifo 数据个数

fifo_struct uart_data_fifo;

uint8 noise_db = 0;    // 保存噪声分贝

void NOISE_init()
{
    fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);              // 初始化 fifo 挂载缓冲区

    uart_init(UART_INDEX, UART_BAUDRATE, UART_TX_PIN, UART_RX_PIN);             // 初始化编码器模块与引脚 正交解码编码器模式
    uart_rx_interrupt(UART_INDEX, ZF_ENABLE);                                   // 开启 UART_INDEX 的接收中断
    interrupt_set_priority(UART_PRIORITY, 0);                                   // 设置对应 UART_INDEX 的中断优先级为 0
}

void NOISE_read()
{
    fifo_data_count = fifo_used(&uart_data_fifo); // 查看 fifo 是否有数据
    if (0 != fifo_data_count)                     // 读取到数据了
    {
        fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN); // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
        for(int i = 0; i < fifo_data_count / 6; ++i)  // 6字节为一个数据包
        {
            if (fifo_get_data[i] == 0xBB && fifo_get_data[i+1] == 0xAA)
            {
                noise_db = fifo_get_data[i+3];
            }
        }
    }
    system_delay_ms(10);
}