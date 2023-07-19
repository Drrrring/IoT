#include <zf_common_headfile.h>
#include <stdio.h>
#include <math.h>
#include <pid.h>
#include <gps.h>
#include <wifi_control.h>
// 小车008在左 009在右
// PWM大于1500时 008往前 009往后

// 目标坐标B 需要修改为GPS经纬度
double targetX = 0.0;
double targetY = 0.0;

// PID控制器的误差累积和上一次误差
double integral = 0.0;
double previous_error = 0.0;

// 计算两点之间的距离 单位为m
double calculateDistance(double x1, double y1, double x2, double y2) {
    // 将单位换算为m
	double dx = (x2 - x1) * 111194.926644 * cos(30.538);
    double dy = (y2 - y1) * 111194.926644558737;
    return sqrt(dx*dx + dy*dy);
}

// 计算朝向角度误差
double calculateAngleError(double targetAngle, double currentAngle) {
    double error = targetAngle - currentAngle;
    // 对误差进行归一化到[-180, 180]范围内
    if (error > 180.0) {
        error -= 360.0;
    } else if (error < -180.0) {
        error += 360.0;
    }
    return error;
}

// PID控制器
double pidController(double distance, double angleError, double dt) {
	// 比例项P 用于根据当前误差的大小产生一个控制量
    double proportional = Kp * distance;
	// 计算积分项，将当前角度误差 angleError 乘以积分系数 Ki
    integral += Ki * angleError * dt;
	// 计算微分项，计算当前角度误差 angleError 与上一次角度误差 previous_error 的差，并乘以微分系数 Kd
    double derivative = Kd * (angleError - previous_error) / dt;
	// 输出为三项相加
    double output = proportional + integral + derivative;
	// 更新角度误差
    previous_error = angleError;
    return output;
}

// 控制舵机转动的时间
//double controlServo(double angleError, double distance, double samplingTime) {
//    // 计算PID控制器的误差项
//    double proportional = Kp * angleError;
//    integral += Ki * angleError * samplingTime;
//    double derivative = Kd * (angleError - previous_error) / samplingTime;

//    // 计算控制量
//    double control = proportional + integral + derivative;

//    // 转换控制量为转动时间
//    double T = 1000.0 * distance / control;  // 假设控制量与转动时间成线性关系

//    // 更新角度偏差和积分项
////    previous_error = angleError;

//    return T;
//}

// 根据控制量调整车辆的速度和转向角度
void adjustVehicle(double control, double angleError) {
    int pwm = (int)(control + 1900);  // 将控制量转换为PWM值（左、前）
	int pwm9 = (int)(1100 - control);  // 将控制量转换为PWM值（右）

	// 根据角度偏差调整左右后轮的PWM值
    int leftWheelPwm = pwm - angleError * 10;  // 根据角度偏差增加或减小左后轮的速度
    int rightWheelPwm = pwm9 - angleError * 10;  // 根据角度偏差增加或减小右后轮的速度
	
	// 脱离舵机的停滞区间
	if (leftWheelPwm < 1900)
		leftWheelPwm = 1900;
	else if(leftWheelPwm > 2500)
		leftWheelPwm = 2500;
	if (rightWheelPwm > 1100)
		rightWheelPwm = 1100;
	else if(rightWheelPwm < 500)
		rightWheelPwm = 500;
	
	// 根据角度偏差调整转向舵机的PWM值
    int steeringPwm = 1500 - angleError * 2;  // 根据角度偏差增加或减小转向舵机的PWM值
	// 脱离舵机的停滞区间
	if (steeringPwm < 800)
		steeringPwm = 800;
	else if(steeringPwm > 2200)
		steeringPwm = 2200;
	
    // 控制左后轮
    char leftWheelCmd[20];
    sprintf(leftWheelCmd, "#008P%04dT5000!", leftWheelPwm);  // 生成控制左后轮的命令字符串

    // 控制右后轮
    char rightWheelCmd[20];
    sprintf(rightWheelCmd, "#009P%04dT5000!", rightWheelPwm);  // 生成控制右后轮的命令字符串
	
    // 控制转向
    char steeringCmd[20];
    sprintf(steeringCmd, "#000P%04dT0000!", steeringPwm);  // 生成控制转向的命令字符串
    printf("%s", steeringCmd);
	system_delay_ms(500);
	
	// 打印输出 驱动舵机运动
	printf("%s%s%s", steeringCmd, leftWheelCmd, rightWheelCmd);
	system_delay_ms(2000);
	
	// 回正转向舵机000
	printf("#000P1500T0000!");
	system_delay_ms(5000);
	
	wifiExchangeMessage((uint8*)steeringCmd);
	wifiExchangeMessage((uint8*)leftWheelCmd);
	wifiExchangeMessage((uint8*)rightWheelCmd);

}

// 经度为X，纬度为Y
void pid_control(double inputDestinationX, double inputDestinationY) {
	
	while(flag)
	{
		GPS_read();
		// 获取当前坐标 并赋值为上一次的坐标
		// 经线为X，纬线为Y
		lastX = GPS_get_longitude();
		lastY = GPS_get_latitude();
		printf("{#008P2500T7000!,#009P0500T7000!}"); // 前进一段距离
		system_delay_ms(11000);

		GPS_read();		
		// 获取当前坐标位置
		currentX = GPS_get_longitude();
		currentY = GPS_get_latitude();
		
		// 获取当前的方向
		currentAngle = atan2(currentY - lastY, currentX - lastX) * 180.0 / M_PI;
		
		while(currentAngle == 0)
		{
			uint8 str11[] = "error";
			wifiExchangeMessage(str11);
			system_delay_ms(2000);
			// 测试代码
			// wifi传输数据
			uint8 str1[200];
			uint8 str2[] = "--------";
			sprintf((char*)str1, "currentAngle:%lf(du), currentX:%lf, currentY:%lf, lastX:%lf, lastY:%lf", 
				currentAngle, currentX, currentY, lastX, lastY);
			wifiExchangeMessage(str1);
			wifiExchangeMessage(str2);
			// 测试代码
		}
		
		// 测试代码
		// wifi传输数据
		uint8 str1[200];
		uint8 str2[] = "--------";
		sprintf((char*)str1, "currentAngle:%lf(du), currentX:%f, currentY:%f, lastX:%f, lastY:%f", 
			currentAngle, currentX, currentY, lastX, lastY);
		wifiExchangeMessage(str1);
		wifiExchangeMessage(str2);
		// 测试代码
		
		flag = 0;
	}
	
	// 传参输入目标坐标
	targetX = inputDestinationX;
	targetY = inputDestinationY;

    // 车辆移动过程，循环执行直到车辆到达目标附近1m左右位置
    while (calculateDistance(currentX, currentY, targetX, targetY) > 1) {
        // 计算车辆当前位置与目标位置之间的距离和朝向角度误差
        double distance = calculateDistance(currentX, currentY, targetX, targetY);
        double targetAngle = atan2(targetY - currentY, targetX - currentX) * 180.0 / M_PI;
        double angleError = calculateAngleError(targetAngle, currentAngle);

        // 使用PID控制器计算控制量
        double control = pidController(distance, angleError, sampling_time);

        // 根据控制量调整车辆的速度和转向角度
		adjustVehicle(control, angleError);
		
		system_delay_ms(4000);

		// 刷新上一次的坐标
		lastX = currentX;
		lastY = currentY;
		
        // 模拟车辆移动过程，更新车辆的GPS坐标
		GPS_read();
        currentX = GPS_get_longitude();
		currentY = GPS_get_latitude();
		if (atan2(currentY - lastY, currentX - lastX) != 0)
		{
			currentAngle = atan2(currentY - lastY, currentX - lastX) * 180.0 / M_PI;
		}
		
		// 测试代码
		// wifi传输数据
		uint8 str3[200];
		uint8 str4[200];
		uint8 str5[] = "--------";
		sprintf((char*)str3, "currentAngle:%lf(du), currentX:%lf, currentY:%lf, lastX:%lf, lastY:%lf", 
			currentAngle, currentX, currentY, lastX, lastY);
		sprintf((char*)str4, "distance:%lf, targetAngle:%f(du), angleError:%f(du), control:%f", 
			distance, targetAngle, angleError, control);
		wifiExchangeMessage(str3);
		wifiExchangeMessage(str5);
		wifiExchangeMessage(str4);		
		wifiExchangeMessage(str5);
		// 测试代码
		
        // 输出当前车辆的位置和朝向角度
//        printf("Current Position: (%lf, %lf)\n", currentX, currentY);
    }
	
	// 到达目的地
	printf("#000P1500T1000!");
	system_delay_ms(1500);
//    printf("Reached the target position!\n");
	// 测试代码
	uint8 str6[] = "Reached the target position!";
	wifiExchangeMessage(str6);
	// 测试代码
}
