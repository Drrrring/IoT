#ifndef __PID_H
#define __PID_H

#define M_PI 3.14159265358979323846
// PID控制器参数
#define Kp 0.43
#define Ki 0.0
#define Kd 0.0

// 当前车辆的坐标和朝向角度
static double currentX = 0.0;
static double currentY = 0.0;
static double currentAngle = 0.0;

// 上一时间点的坐标值
static double lastX = 0.0;
static double lastY = 0.0;

// 采样时间
static double sampling_time = 0.1;  // 假设采样时间为0.1秒

// 控制第一次获取方向角
static int flag = 1;

// PID控制
void pid_control(double inputDestinationX, double inputDestinationY);

#endif
