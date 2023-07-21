#include "ultrasonic.h"
#include "zf_common_headfile.h"

uint8 pit_state = 0;

int main(void)
{
    clock_init(SYSTEM_CLOCK_120M); // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                  // 初始化默认 Debug UART

    ultrasonic_init();

    // ultrasonic_test();

    //    pit_ms_init(TIM6_PIT, 1000);
    //	interrupt_set_priority(TIM6_IRQn, 0);

    while (1)
    {
			  ultrasonic_block();
        ultrasonic_run_test();
        
        //system_delay_ms(1000);
    }
}
