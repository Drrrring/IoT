#include "ultrasonic.h"

// 超声波模块初始化
void ultrasonic_init()
{
    gpio_init(ULTRASONIC_TRIG_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL); // 触发引脚 推挽输出
    gpio_init(ULTRASONIC_ECHO_PIN, GPI, GPIO_LOW, GPI_PULL_DOWN); // 接收引脚 下拉输入（无输入时引脚为低电平）
    exti_init(ULTRASONIC_ECHO_PIN, EXTI_TRIGGER_BOTH);            // 接收引脚 双边沿时触发中断 在 EXTI1_IRQHandler 中完成中断处理
    // pit_ms_init(TIM6_PIT, 2000);                                   // 初始化 TIM6_PIT 为周期中断 1000ms 周期 在 TIM6_IRQHandler 中完成中断处理
    timer_init(TIM_3, TIMER_US); // 定时器使用 TIM_3 使用微秒级计数

    // pit_ms_init(TIM5_PIT, 2000); // 检查 Barrier 是否为1，是的话阻塞

    // interrupt_set_priority(TIM6_IRQn, 3);
    interrupt_set_priority(TIM3_IRQn, 2);
    interrupt_set_priority(EXTI1_IRQn, 1);
    // interrupt_set_priority(TIM5_IRQn, 4);
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

    // 当前电位为低，表示现在是上升沿，刚开始收到高电平
    if (!up)
    {
        timer_clear(TIM_3);
        timer_start(TIM_3); // 开始计时
        up = 1;
    }
    else
    {
        timer_stop(TIM_3); // 停止计时
        up = 0;

        // int time = timer_get(TIM_3);
        // ultrasonic_distance_mm = (float)time * ULTRASONIC_SPEED_MM_PER_US / 2;
        // printf("time: %d, distance_mm: %d\r\n", time, ultrasonic_distance_mm);

        // // 监测到障碍物距离小于300mm，阻塞
        // if (ultrasonic_distance_mm < 300)
        // {
        //     barrier = 1;
        // }
        // else
        // {
        //     barrier = 0;
        // }
    }
}

// 处理测量结果，内部调用
void ultrasonic_receive()
{
    if (!up) // 已完成上次读取
    {
        int time = timer_get(TIM_3);
        ultrasonic_distance_mm = (float)time * ULTRASONIC_SPEED_MM_PER_US / 2;
        printf("time: %d, distance_mm: %d\r\n", time, ultrasonic_distance_mm);
        // 监测到障碍物距离小于300mm，阻塞
        if (ultrasonic_distance_mm < 300)
        {
            barrier = 1;
        }
        else
        {
            barrier = 0;
        }
    }
    else
    {
        printf("read not finished\r\n");
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

void ultrasonic_block()
{
    // todo 发送信号告知后端已阻塞
    if (barrier)
    {
        printf("blocked\r\n");
    }

    // 当前面有障碍物时就什么都不做
    while (barrier)
    {
    }

    // TODO 发送信号告知后端已解除阻塞
    printf("not blocked\r\n");
}

bool ultrasonic_barrier()
{
    ultrasonic_range();
    system_delay_ms(1);
    ultrasonic_receive();
    return barrier;
}