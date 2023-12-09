#include "air.h"

uint8 air_uart_get_data[128]; // 串口接收数据缓冲区
uint8 air_fifo_get_data[128]; // fifo 输出读出缓冲区

uint8 air_get_data = 0;         // 接收数据变量
uint32 air_fifo_data_count = 0; // fifo 数据个数

fifo_struct air_uart_data_fifo;

uint8 air_res = 0; // 保存空气质量数据

void AIR_init()
{
    fifo_init(&air_uart_data_fifo, FIFO_DATA_8BIT, air_uart_get_data, 64); // 初始化 fifo 挂载缓冲区

    uart_init(UART_INDEX_AIR, UART_BAUDRATE_AIR, UART_TX_PIN_AIR, UART_RX_PIN_AIR); // 初始化编码器模块与引脚 正交解码编码器模式
    uart_rx_interrupt(UART_INDEX_AIR, ZF_ENABLE);                       // 开启 UART_INDEX 的接收中断
    interrupt_set_priority(UART_PRIORITY_AIR, 0);                       // 设置对应 UART_INDEX 的中断优先级为 0
}

void AIR_read()
{
    air_fifo_data_count = fifo_used(&air_uart_data_fifo); // 查看 fifo 是否有数据
    if (0 != air_fifo_data_count)                     // 读取到数据了
    {
        fifo_read_buffer(&air_uart_data_fifo, air_fifo_get_data, &air_fifo_data_count, FIFO_READ_AND_CLEAN); // 将 fifo 中数据读出并清空 fifo 挂载的缓冲

        for (int i = 0; i < air_fifo_data_count - 1; ++i)
        {
            if (air_fifo_get_data[i] == 0x42 && air_fifo_get_data[i+1] == 0x4d && i+7 < air_fifo_data_count)
            {
                air_res = air_fifo_get_data[i+7] + 256 * air_fifo_get_data[i+6];
            }
        }
    }
    system_delay_ms(10);
}