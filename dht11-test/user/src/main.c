
#include "dht11.h"
#include "zf_common_headfile.h"

#define pin (B0)

// **************************** 代码区域 ****************************
int main (void)
{
    clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                                                               // 初始化默认 Debug UART

		uint8 buf[10];
	  uint8* pbuf = buf;

    while(1)
    {

        // 此处编写需要循环执行的代码
        DHT11_receive(pin, buf);
			printf("Humidity: %d.%d  Temperature:%d.%d \r\n", buf[0], buf[1], buf[2], buf[3]);
        // 此处编写需要循环执行的代码
    }
}

