
#include "zf_common_headfile.h"
#include "noise.h"

extern uint8 uart_get_data[64]; // 串口接收数据缓冲区
extern uint8 fifo_get_data[64]; // fifo 输出读出缓冲区

extern uint8 get_data;         // 接收数据变量
extern uint32 fifo_data_count; // fifo 数据个数

extern fifo_struct uart_data_fifo;

extern uint8 noise_db;    // 保存噪声分贝

int main(void)
{
    clock_init(SYSTEM_CLOCK_120M); // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                  // 初始化默认 debug uart

    NOISE_init();

    while (1)
    {
        NOISE_read();
        printf("noise: %d\r\n", noise_db);
        system_delay_ms(400);
        
    }
}

void uart_rx_interrupt_handler(void)
{
    uart_query_byte(UART_INDEX, &get_data);                                     // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
    fifo_write_buffer(&uart_data_fifo, &get_data, 1);                           // 将数据写入 fifo 中
}
