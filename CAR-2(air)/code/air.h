#ifndef _air_h
#define _air_h
#include "zf_common_headfile.h"

#define UART_INDEX_AIR              (UART_3   )                           // 默认 UART_1
#define UART_BAUDRATE_AIR           (9600 )                           // 默认 115200
#define UART_TX_PIN_AIR             (UART3_TX_D8  )                           // 默认 UART1_TX_A9
#define UART_RX_PIN_AIR             (UART3_RX_D9  )                           // 默认 UART1_RX_A10
#define UART_PRIORITY_AIR           (UART3_IRQn)                                    // 对应串口中断的中断编号 在 mm32f3277gx.h 头文件中查看 IRQn_Type 枚举体



void AIR_init();
void AIR_read();
#endif