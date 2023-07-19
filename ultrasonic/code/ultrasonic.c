#include "ultrasonic.h"

// 超声波模块初始化
void ultrasonic_init() {
    gpio_init(ULTRASONIC_TRIG_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);  // 触发引脚 推挽输出
    exti_init(ULTRASONIC_ECHO_PIN, EXTI_TRIGGER_FALLING);          // 接收引脚 有下降沿时触发中断 在 EXTI0_IRQHandler 中完成中断处理
    pit_ms_init(TIM6_PIT, 1000);                                   // 初始化 TIM6_PIT 为周期中断 1000ms 周期 在 TIM6_IRQHandler 中完成中断处理
    timer_init(TIM_3, TIMER_US);                                   // 定时器使用 TIM_3 使用微秒级计数


}

// 超声波模块开始发送测距信号
void ultrasonic_range() {
    // 上一次发送的测距信号已超时
    if (overtime) {
        ultrasonic_distance_mm = ULTRASONIC_OVERTIME_DISTANCE_MM;
    }
    timer_stop(TIM_3);
    timer_clear(TIM_3);                                            // 清空计时器
    gpio_high(ULTRASONIC_TRIG_PIN);
    system_delay_us(11);
    gpio_low(ULTRASONIC_TRIG_PIN);                                 // 发出测距信号
    timer_start(TIM_3);                                            // 开始计时
}

// 超声波模块接收到信号 外部中断回调函数
// 这个函数需要放在对应的外部中断服务函数里
void ultrasonic_receive_callback() {
    uint16 time = timer_get(TIM_3);                                // 获取时间
    ultrasonic_distance_mm = (uint16)((float)time * ULTRASONIC_SPEED_MM_PER_US / 2);
    timer_stop(TIM_3);
    timer_clear(TIM_3);
    overtime = 0;
}