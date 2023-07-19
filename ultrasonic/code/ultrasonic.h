#ifndef _ultrasonic_h_
#define _ultrasonic_h_
#include "zf_common_headfile.h"

// 引脚定义
#define ULTRASONIC_TRIG_PIN (B0)    // 触发引脚
#define ULTRASONIC_ECHO_PIN (B1)    // 接收引脚

#define ULTRASONIC_SPEED_MM_PER_US 0.35              // 30摄氏度时的声速，声速V=331.5+0.6*温度；
#define ULTRASONIC_OVERTIME_DISTANCE_MM 1750         // 超声波测距超时 距离默认为1750mm
static uint16 ultrasonic_distance_mm;                // 超声波测出的距离 单位 mm
static bool overtime = 0;                            // 超声波接收的信号是否超时

void ultrasonic_init();
void ultrasonic_range();
void ultrasonic_receive_callback();

#endif