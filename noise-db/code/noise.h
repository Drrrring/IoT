#ifndef _noise_h
#define _noise_h
#include "zf_common_headfile.h"

#define UART_INDEX              (UART_3   )                           // 默认 UART_1
#define UART_BAUDRATE           (115200 )                           // 默认 115200
#define UART_TX_PIN             (UART3_TX_D8  )                           // 默认 UART1_TX_A9
#define UART_RX_PIN             (UART3_RX_D9  )                           // 默认 UART1_RX_A10
#define UART_PRIORITY           (UART3_IRQn)                                    // 对应串口中断的中断编号 在 mm32f3277gx.h 头文件中查看 IRQn_Type 枚举体



void NOISE_init();
void NOISE_read();
#endif