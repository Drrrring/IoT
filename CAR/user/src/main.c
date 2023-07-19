// zf_device_wifi_uart.c 335 if(wifi_uart_get_version())
// zf_device_wifi_uart.c 711 "AT+CIPSTARTEX=\"UDP\",\"");
// zf_device_wifi_uart.c 618 "AT+CIPSTARTEX=\"TCP\",\""

// 陀螺仪跟GPS用的是库函数
#include "dht11.h"
#include "fire.h"
#include "gps.h"
#include "light.h"
#include "mic.h"
#include "smoke.h"
#include "wifi_control.h"
#include "pid.h"
#include "zf_common_headfile.h"

// 接线方式：
// 温湿度计		3.3V/5V 	S - B0   中间 - VCC
// 火焰       3.3V/5V   DO - B1
// 光照       3.3V/5V   AO - A4
// 麦克风     5V        S - A5
// 烟雾       5V        AO - A6
// GPS        3.3V/5V   TX - C11(核心板RX)  RX - C10(核心板TX) (PPS可不管)
// MPU6050    3.3V/5V   SCL - B13  SDA - B15  (ADO和INT可不管)
// WiFi       3.3V/5V   RST - C8  TX - C7(核心板RX)  RX - C6(核心板TX)

#define DHT11_PIN (B0)
#define FIRE_PIN (B1)
#define LIGHT_PIN (ADC1_CH4_A4)
#define MIC_PIN (ADC1_CH5_A5)
#define SMOKE_PIN (ADC1_CH6_A6)

int main(void)
{
  clock_init(SYSTEM_CLOCK_120M); // 初始化芯片时钟 工作频率为 120MHz
  debug_init();                  // 初始化默认 Debug UART
  location_fifo_init();          // 初始化 location fifo

  // 用于保存数据的变量
  location current_positon;
  location destination;
  uint8 dht11_buf[5];
  uint8 value_fire = 200;
  uint8 value_light = 200;
  uint8 value_mic = 200;
  uint8 value_smoke = 200;
  char str1[100];
  char str2[50];
  char str3[50];
  char str4[50];
  char str5[50];
  // char str6[100];
  // char str7[100];
  char message[100];

  // 初始化芯片引脚
  FIRE_init(FIRE_PIN);
  LIGHT_init_analog(LIGHT_PIN);
  MIC_init(MIC_PIN);
  SMOKE_init(SMOKE_PIN);
  // mpu6050_init();
   gps_init();

  // 初始化 location fifo
  location_fifo_init();

//  wifiConnect("SAO", "12346789", "192.168.43.1", "8888", "8888");
wifiConnect("SAO", "12346789", "47.115.223.230	", "8888", "8888");
  // pit_ms_init(TIM6_PIT, 5);

  while (1)
  {
    // 采集传感器数据
    DHT11_receive(DHT11_PIN, dht11_buf);
    value_fire = FIRE_read(FIRE_PIN);
    value_light = LIGHT_read_analog(LIGHT_PIN);
    value_mic = MIC_read(MIC_PIN);
    value_smoke = SMOKE_read(SMOKE_PIN);
     GPS_read();
    current_positon.longitude = 100.0f;
    current_positon.latitude = 20.0f;
		current_positon.longitude = GPS_get_longitude();
    current_positon.latitude = GPS_get_latitude();

    sprintf(str1, "temperature: %d.%d, humidity: %d.%d \r\n", dht11_buf[0],
            dht11_buf[1], dht11_buf[2], dht11_buf[3]);
    sprintf(str2, "fire: %d \t", value_fire);
    sprintf(str3, "light: %d \r\n", value_light);
    sprintf(str4, "mic: %d \t", value_mic);
    sprintf(str5, "smoke: %d \r\n", value_smoke);
    sprintf(message, "%03d%02d%03d%02d%03d%03d%03d%03d%9.6f%8.6f", dht11_buf[0],
            dht11_buf[1], dht11_buf[2], dht11_buf[3], value_mic, value_smoke,
            value_light, value_fire, current_positon.longitude, current_positon.latitude);

    // sprintf(str6, "MPU6050 acc data:  x=%5d, y=%5d, z=%5d\r\n", mpu6050_acc_x,
    //         mpu6050_acc_y, mpu6050_acc_z);
    // sprintf(str7, "MPU6050 gyro data: x=%5d, y=%5d, z=%5d\r\n", mpu6050_gyro_x,
    //         mpu6050_gyro_y, mpu6050_gyro_z);

    printf("%s", str1);
    printf("%s", str2);
    printf("%s", str3);
    printf("%s", str4);
    printf("%s", str5);
    printf("%s", message);
    // printf("%s", str6);
    // printf("%s", str7);

    wifiExchangeMessage((uint8 *)message);
    // wifiExchangeMessage((uint8 *)str1);
    // wifiExchangeMessage((uint8 *)str2);
    // wifiExchangeMessage((uint8 *)str3);
    // wifiExchangeMessage((uint8 *)str4);
    // wifiExchangeMessage((uint8 *)str5);
    // wifiExchangeMessage((uint8 *)str6);
    // wifiExchangeMessage((uint8 *)str7);

    system_delay_ms(500);

    // 读取location fifo，若队列中有坐标，则前往该坐标
    if (!location_fifo_read(&destination))
    {
      // 行进到目的坐标
      //pid_control(destination.longitude, destination.latitude);
    }
  }
}
