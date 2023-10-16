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
#include "ultrasonic.h"
#include "noise.h"
#include "pm2_5.h"
#include "zf_common_headfile.h"

// 接线方式：
// 温湿度计		3.3V/5V 	S - D10   中间 - VCC
//// 火焰       3.3V/5V   DO - E4   （已移除）
//// 光照       3.3V/5V   AO - A4   （已移除）
//// 麦克风     5V        S - A5    （已移除）
// 烟雾（MQ2）       5V        AO - A6
// GPS        3.3V/5V   TX - C11(核心板RX)  RX - C10(核心板TX) (PPS可不管)
// MPU6050    3.3V/5V   SCL - B13  SDA - B15  (ADO和INT可不管)
// WiFi       3.3V/5V   TX - C7(核心板RX)  RX - C6(核心板TX)  RST - C8
// 超声波      5V       TRIG - B0  ECHO - B1
// 噪声       5V        RX - D8  TX - D9    //改成 RX - C12  TX - D2
// MQ7        5V        A7
// MQ135      5V        A4
// PM2.5      5V        A5   PUL - B0
// 空气       5V        RX - D8  TX - D9 （PIN1: VCC, PIN2: GND, PIN4: RXD, PIN5: TXD)

#define DHT11_PIN (D10)
// #define FIRE_PIN (E4)
// #define LIGHT_PIN (ADC1_CH4_A4)
// #define MIC_PIN (ADC1_CH5_A5)
#define MQ2_PIN (ADC1_CH6_A6)
#define MQ7_PIN (ADC1_CH7_A7)
#define MQ135_PIN (ADC1_CH4_A4)
#define PM25_PIN (ADC1_CH5_A5)

// 噪声传感器相关变量（定义在noise.c中）
extern uint8 uart_get_data[64]; // 串口接收数据缓冲区
extern uint8 fifo_get_data[64]; // fifo 输出读出缓冲区
extern uint8 get_data;          // 接收数据变量
extern uint32 fifo_data_count;  // fifo 数据个数
extern fifo_struct uart_data_fifo;
extern uint8 noise_db; // 保存噪声分贝

int main(void)
{
  clock_init(SYSTEM_CLOCK_120M); // 初始化芯片时钟 工作频率为 120MHz
  debug_init();                  // 初始化默认 Debug UART

  // 用于保存数据的变量
  location current_positon;
  location destination;
  uint8 dht11_buf[5];
  uint8 value_fire = 200;
  uint8 value_light = 200;
  uint8 value_mic = 200;
  uint8 value_MQ2 = 200;
  uint8 value_MQ135 = 200;
  uint8 value_MQ7 = 200;
  double value_PM25 = 9.0;

  char str1[100];
  char str2[50];
  char str3[50];
  char str4[50];
  char str5[50];
  char message[100];

  // 初始化芯片引脚
  // FIRE_init(FIRE_PIN);
  // LIGHT_init_analog(LIGHT_PIN);
  // MIC_init(MIC_PIN);
  NOISE_init();          // 噪声传感器初始化
  SMOKE_init(MQ2_PIN);   // MQ2传感器初始化
  SMOKE_init(MQ7_PIN);   // MQ7传感器初始化
  SMOKE_init(MQ135_PIN); // MQ135传感器初始化
  PM25_init();           // PM2.5传感器初始化
  mpu6050_init();        // 陀螺仪初始化
  gps_init();            // gps初始化

  // 初始化 location fifo
  location_fifo_init();

  //  wifiConnect("SAO", "12346789", "192.168.43.1", "8888", "8888");
  wifiConnect("SAO", "12346789", "47.115.223.230", "8888", "8888");
  // wifiConnect("SAO", "12346789", "192.168.43.120", "8888", "8888");

  DHT11_receive(DHT11_PIN, dht11_buf);

  // ultrasonic_init();     // 超声波初始化

  while (1)
  {
    // 采集传感器数据
    dht11_buf[1] = rand() % 100;
    // value_fire = FIRE_read(FIRE_PIN);
    // value_light = LIGHT_read_analog(LIGHT_PIN);
    // value_mic = MIC_read(MIC_PIN);
    // value_mic = 60 + rand() % 10;
    NOISE_read();
    value_MQ2 = SMOKE_read(MQ2_PIN);
    value_MQ7 = SMOKE_read(MQ7_PIN);
    value_MQ135 = SMOKE_read(MQ135_PIN);
    value_PM25 = PM25_read();
    GPS_read();
    current_positon.longitude = 118.957161f;
    current_positon.latitude = 32.110364f;
    //		current_positon.longitude = 112.952150f;
    //    current_positon.latitude = 28.180840f;
    //    current_positon.longitude = GPS_get_longitude();
    //    current_positon.latitude = GPS_get_latitude();

    sprintf(str1, "humidity: %d.%d, temperature: %d.%d \r\n", dht11_buf[0],
            dht11_buf[1], dht11_buf[2], dht11_buf[3]);
    sprintf(str2, "fire: %d \t", value_fire);
    sprintf(str3, "light: %d \r\n", value_light);
    // sprintf(str4, "mic: %d \t", value_mic);
    sprintf(str4, "noise: %d \t", noise_db);
    sprintf(str5, "MQ2: %d \r\n", value_MQ2);
    sprintf(message, "%03d%02d%03d%02d%03d%03d%03d%03d%03d%03d%7.6f%9.6f%8.6f1", dht11_buf[0],
            dht11_buf[1], dht11_buf[2], dht11_buf[3], noise_db, value_MQ2,
            value_light, value_fire, value_MQ135, value_MQ7, value_PM25, current_positon.longitude, current_positon.latitude);

    // sprintf(str6, "MPU6050 acc data:  x=%5d, y=%5d, z=%5d\r\n", mpu6050_acc_x,
    //         mpu6050_acc_y, mpu6050_acc_z);
    // sprintf(str7, "MPU6050 gyro data: x=%5d, y=%5d, z=%5d\r\n", mpu6050_gyro_x,
    //         mpu6050_gyro_y, mpu6050_gyro_z);

    printf("%s", str1);
    printf("%s", str2);
    printf("%s", str3);
    printf("%s", str4);
    printf("%s", str5);
    printf("MQ135: %d\r\n", value_MQ135);
    printf("MQ7: %d\r\n", value_MQ7);
    printf("%s\r\n", message);

    // wifi发送数据
    wifiExchangeMessage((uint8 *)message);

    system_delay_ms(200);

    // 读取location fifo，若队列中有坐标，则前往该坐标
//    if (!location_fifo_read(&destination))
//    {
//      pid_control(destination.longitude, destination.latitude); // 行进到目的坐标
//    }
  }
}

// 噪声传感器中断处理函数
void uart_rx_interrupt_handler(void)
{
  uart_query_byte(UART_INDEX, &get_data);           // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
  fifo_write_buffer(&uart_data_fifo, &get_data, 1); // 将数据写入 fifo 中
}
