#include "ultrasonic.h"

// 超声波模块初始化
void ultrasonic_init()
{
    gpio_init(ULTRASONIC_TRIG_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL); // 触发引脚 推挽输出
    gpio_init(ULTRASONIC_ECHO_PIN, GPI, GPIO_LOW, GPI_PULL_DOWN); // 接收引脚 下拉输入（无输入时引脚为低电平）
    exti_init(ULTRASONIC_ECHO_PIN, EXTI_TRIGGER_BOTH);           // 接收引脚 双边沿时触发中断 在 EXTI1_IRQHandler 中完成中断处理
    pit_ms_init(TIM6_PIT, 2000);                                  // 初始化 TIM6_PIT 为周期中断 1000ms 周期 在 TIM6_IRQHandler 中完成中断处理
    timer_init(TIM_3, TIMER_US);                                  // 定时器使用 TIM_3 使用微秒级计数

    interrupt_set_priority(TIM6_IRQn, 3); 
	interrupt_set_priority(TIM3_IRQn, 2); 
    interrupt_set_priority(EXTI1_IRQn, 1);
}

// 超声波模块开始发送测距信号
void ultrasonic_range()
{
    timer_stop(TIM_3);
    timer_clear(TIM_3); // 清空计时器
    gpio_high(ULTRASONIC_TRIG_PIN);
    system_delay_us(11);
    gpio_low(ULTRASONIC_TRIG_PIN); // 发出测距信号
    timer_start(TIM_3);            // 开始计时
}

// 超声波模块接收到信号 外部中断回调函数
// 这个函数需要放在对应的外部中断服务函数里
void ultrasonic_receive_callback()
{
    // uint16 time = timer_get(TIM_3); // 获取时间
    // ultrasonic_distance_mm = (uint16)((float)time * ULTRASONIC_SPEED_MM_PER_US / 2);
    // timer_stop(TIM_3);
    // timer_clear(TIM_3);
    // overtime = 0;
    // printf("distance: %d\r\n", ultrasonic_distance_mm);

    // 当前电位为低，表示现在是上升沿，刚开始收到高电平
    if (!up) {
        timer_clear(TIM_3);
        timer_start(TIM_3);         // 开始计时
        up = !up;
    }
    else {
        timer_stop(TIM_3);         // 停止计时
        int time = timer_get(TIM_3);
        ultrasonic_distance_mm = (float)time * ULTRASONIC_SPEED_MM_PER_US / 2;
        printf("time: %d, distance_mm: %d", time, ultrasonic_distance_mm);
        up = !up;
    }
}

// 测试超声波模块用，没用使用中断，只用了计时器
void ultrasonic_test()
{
    gpio_init(ULTRASONIC_TRIG_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL); // 触发引脚 推挽输出
    gpio_init(ULTRASONIC_ECHO_PIN, GPI, GPIO_LOW, GPI_PULL_DOWN); // 接收引脚 下拉输入（无输入时引脚为低电平）
    timer_init(TIM_4, TIMER_US);

    // 发送信号
    gpio_high(ULTRASONIC_TRIG_PIN);
    system_delay_us(11);
    gpio_low(ULTRASONIC_TRIG_PIN); // 发出测距信号

    // 等待至接收引脚开始高电平
    while (!gpio_get_level(ULTRASONIC_ECHO_PIN))
    {
    }
    timer_start(TIM_4);
    // 低电平退出循环
    while (gpio_get_level(ULTRASONIC_ECHO_PIN))
    {
    }
    timer_stop(TIM_4);

    int test_time = timer_get(TIM_4);
    int test_distance_mm = (float)test_time * ULTRASONIC_SPEED_MM_PER_US / 2;
    printf("time: %d, distance: %d\r\n", test_time, test_distance_mm);
}