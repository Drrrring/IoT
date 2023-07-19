#include "mpu6050.h"

void mpu6050_pit_handler() {
  mpu6050_get_acc();  // 获取 MPU6050 的加速度测量数值
  mpu6050_get_gyro(); // 获取 MPU6050 的角速度测量数值
}