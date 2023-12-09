#ifndef _ultrasonic_h_
#define _ultrasonic_h_
#include "zf_common_headfile.h"

// 引脚定义
#define ULTRASONIC_TRIG_PIN (B0)    // 触发引脚
#define ULTRASONIC_ECHO_PIN (B1)    // 接收引脚

#define ULTRASONIC_SPEED_MM_PER_US 0.35              // 30摄氏度时的声速，声速V=331.5+0.6*温度；
#define ULTRASONIC_OVERTIME_DISTANCE_MM 1750         // 超声波测距超时 距离默认为1750mm
volatile static uint16 ultrasonic_distance_mm;                // 超声波测出的距离 单位 mm
// static bool overtime = 0;                            // 超声波接收的信号是否超时
volatile static bool up = 0;                                  // 用在EXTI1_IRQHandler中断中，标识该边沿是上升沿还是下降沿
volatile static bool barrier = 0;                             // 标识前方是否有障碍物

void ultrasonic_init();
void ultrasonic_range();
void ultrasonic_receive_callback();
void ultrasonic_test();

void ultrasonic_block();
void ultrasonic_receive();
bool ultrasonic_barrier();

#endif