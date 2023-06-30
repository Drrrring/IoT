// 陀螺仪跟GPS用的是库函数
#include "dht11.h"
#include "fire.h"
#include "light.h"
#include "mic.h"
#include "smoke.h"
#include "zf_common_headfile.h"

// 接线方式：
// 温湿度计		3.3V/5V 	S - B0   中间 - VCC
// 火焰       3.3V/5V   DO - B1
// 光照       3.3V/5V   AO - A4
// 麦克风     5V        S - A5
// 烟雾       5V        AO - A6
// GPS        3.3V/5V   TX RX (PPS可不管)
// MPU6050    3.3V/5V   SCL - B13  SDA - B15  (ADO和INT可不管)

#define DHT11_PIN (B0)
#define FIRE_PIN (B1)
#define LIGHT_PIN (ADC1_CH4_A4)
#define MIC_PIN (ADC1_CH5_A5)
#define SMOKE_PIN (ADC1_CH6_A6)

int main(void) {
  clock_init(SYSTEM_CLOCK_120M); // 初始化芯片时钟 工作频率为 120MHz
  debug_init();                  // 初始化默认 Debug UART
	
	

  // 用于保存数据的变量
  uint8 dht11_buf[5];
  uint8 value_fire = 200;
  uint8 value_light = 200;
  uint8 value_mic = 200;
  uint8 value_smoke = 200;
	
	DHT11_receive(DHT11_PIN, dht11_buf);

  // 初始化芯片引脚
  FIRE_init(FIRE_PIN);
//	printf("fire init success\r\n");
	LIGHT_init_analog(LIGHT_PIN);
//	printf("light init success\r\n");
	MIC_init(MIC_PIN);
//	printf("mic init success\r\n");
	SMOKE_init(SMOKE_PIN);
//	printf("smoke init success\r\n");
	mpu6050_init();
//	printf("mpu6050 init success\r\n");
	gps_init();
//	printf("gps init success\r\n");
	
	pit_ms_init(TIM6_PIT, 5);
	
	

  while (1) {
  //  DHT11_receive(DHT11_PIN, dht11_buf);
    value_fire = FIRE_read(FIRE_PIN);
    value_light = LIGHT_read_analog(LIGHT_PIN);
    value_mic = MIC_read(MIC_PIN);
    value_smoke = SMOKE_read(SMOKE_PIN);
		
		printf("temperature: %d.%d, humidity: %d.%d \r\n", dht11_buf[0], dht11_buf[1], dht11_buf[2], dht11_buf[3]);
		printf("fire: %d \t", value_fire);
		printf("light: %d \r\n", value_light);
		printf("mic: %d \t", value_mic);
		printf("smoke: %d \r\n", value_smoke);
		
		//gps数据接收与解析都是通过串口中断调用gps_uart_callback函数进行实现的
        //数据解析完毕之后gps_tau1201_flag标志位会置1
        if(gps_tau1201_flag)
        {
            gps_tau1201_flag = 0;

            if(!gps_data_parse())          //开始解析数据
            {
                printf("now time:  ");                                        // 输出年月日时分秒
                printf("year-%d, month-%d, day-%d\r\n", gps_tau1201.time.year, gps_tau1201.time.month, gps_tau1201.time.day);           // 输出年月日时分秒
                printf("hour-%d, minute-%d, second-%d\r\n", gps_tau1201.time.hour, gps_tau1201.time.minute, gps_tau1201.time.second);   // 输出年月日时分秒
                printf("gps_state       = %d\r\n" , gps_tau1201.state);         //输出当前定位有效模式 1：定位有效  0：定位无效
                printf("latitude        = %lf\r\n", gps_tau1201.latitude);      //输出纬度信息
                printf("longitude       = %lf\r\n", gps_tau1201.longitude);     //输出经度信息
                printf("speed           = %lf\r\n", gps_tau1201.speed);         //输出速度信息
                printf("direction       = %lf\r\n", gps_tau1201.direction);     //输出方向信息
                printf("satellite_used  = %d\r\n" , gps_tau1201.satellite_used);//输出当前用于定位的卫星数量
                printf("height          = %lf\r\n", gps_tau1201.height);        //输出当前GPS天线所处高度
            }
        }
				
				printf("MPU6050 acc data:  x=%5d, y=%5d, z=%5d\r\n", mpu6050_acc_x, mpu6050_acc_y, mpu6050_acc_z);
		printf("MPU6050 gyro data: x=%5d, y=%5d, z=%5d\r\n", mpu6050_gyro_x, mpu6050_gyro_y, mpu6050_gyro_z);
		system_delay_ms(1000);
  }
}

void mpu6050_pit_handler() {
  mpu6050_get_acc();  // 获取 MPU6050 的加速度测量数值
  mpu6050_get_gyro(); // 获取 MPU6050 的角速度测量数值
}