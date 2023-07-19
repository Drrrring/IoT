#include "zf_common_headfile.h"
// 008 左轮 大于1500向前
// 009 右轮 小于1500向前

int main (void)
{
    clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                                                               // 初始化默认 Debug UART

	printf("get:#000P1300T1000!#009P2000T1000!");
//		printf("{#008P1000T2000!#009P1000T2000!}");
//		printf("#000P1300T1000!#008P1300T2000!#009P1300T2000!");

    system_delay_ms(2000);

    while(1)
    {
        
    }
}

