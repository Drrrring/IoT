

#include "zf_common_headfile.h"
#include "smoke.h"

// VCC -- 5V 
// 当测量浓度大于设定浓度时，单片机IO口输出低电平
#define SMOKE_PIN (ADC1_CH6_A6)
// **************************** 代码区域 ****************************
int main (void)
{
    clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                                                               // 初始化默认 Debug UART

		SMOKE_init(SMOKE_PIN);
		uint8 smoke_value = 200;
    while(1)
    {
			smoke_value = SMOKE_read(SMOKE_PIN);
			printf("smoke: %d \r\n", smoke_value);
			system_delay_ms(1000);
    }
}
// **************************** 代码区域 ****************************
