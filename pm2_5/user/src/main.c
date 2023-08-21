
#include "zf_common_headfile.h"
#include "pm2_5.h"

int main (void)
{
    clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                                                               // 初始化默认 Debug UART

		PM25_init();
		double pm25_value = 200;
    while(1)
    {
			double voltage = PM25_read() * 3.3 /4095;	//Air_ad/4095*3300;	电压放大1000倍 0-3300
			pm25_value = 0.18 * voltage;
			pm25_value = PM25_read();
			printf("pm25: %lf \r\n", voltage);
			system_delay_ms(500);
    }
}

