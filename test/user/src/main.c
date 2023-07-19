#include "dht11.h"
#include "fire.h"
#include "gps.h"
#include "light.h"
#include "mic.h"
#include "mpu6050.h"
#include "pid.h"
#include "smoke.h"
#include "wifi_control.h"
#include "zf_common_headfile.h"

#define DHT11_PIN (B0)
#define FIRE_PIN (B1)
#define LIGHT_PIN (ADC1_CH4_A4)
#define MIC_PIN (ADC1_CH5_A5)
#define SMOKE_PIN (ADC1_CH6_A6)

static float buffer[200];
int main(void) {
  clock_init(SYSTEM_CLOCK_120M); // 初始化芯片时钟 工作频率为 120MHz
  debug_init();                  // 初始化默认 Debug UART

  location_fifo_init(); // 初始化 location fifo

  // 用于保存数据的变量
  uint8 dht11_buf[5];
  uint8 value_fire = 200;
  uint8 value_light = 200;
  uint8 value_mic = 200;
  uint8 value_smoke = 200;
  char message[100];
  location a;
  a.longitude = 100.0f;
  a.latitude = 20.0f;

  char str1[100];
  char str2[50];
  char str3[50];
  char str4[50];
  char str5[50];



  // 初始化芯片引脚
   FIRE_init(FIRE_PIN);
   LIGHT_init_analog(LIGHT_PIN);
   MIC_init(MIC_PIN);
   SMOKE_init(SMOKE_PIN);
 gps_init();
  printf("init\r\n");

 // wifiConnect("SAO", "12346789", "192.168.43.1", "8888", "8888");
  // wifiConnect("iPhone", "zy000000", "172.20.10.4", "8888", "8888");
//   wifiConnect("SAO", "12346789", "192.168.43.120", "8888", "8888");
 //  wifiConnect("SAO", "12346789", "47.115.223.230", "8888", "8888");


  while (1) {
    DHT11_receive(DHT11_PIN, dht11_buf);
    value_fire = FIRE_read(FIRE_PIN);
     value_light = LIGHT_read_analog(LIGHT_PIN);
    value_mic = MIC_read(MIC_PIN);
    value_smoke = SMOKE_read(SMOKE_PIN);
       GPS_read();

     a.latitude = GPS_get_latitude();
     a.longitude = GPS_get_longitude();

     sprintf(str1, "humidity: %d.%d, temperature: %d.%d \r\n", dht11_buf[0],
             dht11_buf[1], dht11_buf[2], dht11_buf[3]);
     sprintf(str2, "fire: %d \t", value_fire);
     sprintf(str3, "light: %d \r\n", value_light);
     sprintf(str4, "mic: %d \t", value_mic);
     sprintf(str5, "smoke: %d \r\n", value_smoke);

     sprintf(message, "%03d%02d%03d%02d%03d%03d%03d%03d%9.6f%8.6f",
     dht11_buf[0],
             dht11_buf[1], dht11_buf[2], dht11_buf[3], value_mic, value_smoke,
             value_light, value_fire, a.longitude, a.latitude);

    // printf("%s", str1);
    // printf("%s", str2);
    // printf("%s", str3);
    // printf("%s", str4);
    // printf("%s", str5);
     printf("%s\r\n", message);
    // printf("latitude: %f, longitude: %f\r\n", a.latitude, a.longitude);
    //	system_delay_ms(50);
    
 //   wifiExchangeMessage((uint8 *)message);
     pid_control(114.351875, 30.540173);
  }
}
