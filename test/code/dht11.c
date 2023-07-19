#include "dht11.h"
#include "zf_common_headfile.h"

// 初始化芯片
void DHT11_start(gpio_pin_enum pin) {

  system_delay_ms(1500); // 上电后要等待1S以越过不稳定状态

  gpio_init(pin, GPO, GPIO_HIGH,
            GPO_PUSH_PULL); // 初始化 pin 输出 高电平 推挽输出模式
  system_delay_ms(20);
  gpio_set_level(pin, GPIO_LOW);  // 拉低电压，主机发送开始信号
  system_delay_ms(20);            // 延时18ms以上
  gpio_set_level(pin, GPIO_HIGH); // 拉高电压，等待设备信号
  system_delay_us(20);            // 延时20-40us
  gpio_set_dir(pin, GPI, GPI_FLOATING_IN); // 将引脚设为输入引脚
  system_delay_us(30);
}

uint8 DHT11_rec_byte(gpio_pin_enum pin) {
  uint8 i, dat = 0;
  for (i = 0; i < 8; i++) // 从高到低依次接收8位数据
  {
    // uint8 level = gpio_get_level(pin);
    while (!gpio_get_level(pin))
      ;                  // 等待50us低电平过去
    system_delay_us(45); // 延时45us，如果还为高则数据为1，否则为0
    dat <<= 1; // 移位使正确接收8位数据，数据为0时直接移位
    if (gpio_get_level(pin) == 1) // 数据为1时，使dat加1来接收数据1
      dat += 1;
    while (gpio_get_level(pin))
      ; // 等待数据线拉低
  }
  return dat;
}

void DHT11_receive(gpio_pin_enum pin, uint8 *rec_dat) // 接收40位的数据
{

  uint8 R_H, R_L, T_H, T_L, RH, RL, TH, TL, revise;
  DHT11_start(pin);
  // uint8 level = gpio_get_level(pin);
  if (gpio_get_level(pin) == 0) {
    while (gpio_get_level(pin) == 0)
      ;                           // 等待拉高
    system_delay_us(80);          // 拉高后延时80us
    R_H = DHT11_rec_byte(pin);    // 接收湿度高八位
    R_L = DHT11_rec_byte(pin);    // 接收湿度低八位
    T_H = DHT11_rec_byte(pin);    // 接收温度高八位
    T_L = DHT11_rec_byte(pin);    // 接收温度低八位
    revise = DHT11_rec_byte(pin); // 接收校正位

    system_delay_us(25); // 结束

    if ((R_H + R_L + T_H + T_L) == revise) // 校正
    {
      RH = R_H;
      RL = R_L;
      TH = T_H;
      TL = T_L;
    } else {
      printf("Bits errors");
      RH = R_H;
      RL = R_L;
      TH = T_H;
      TL = T_L;
    }
    /*数据处理，方便显示*/
    // rec_dat[0]='0'+(RH/10);
    // rec_dat[1]='0'+(RH%10);
    // rec_dat[2]='R';
    // rec_dat[3]='H';
    // rec_dat[4]=' ';
    // rec_dat[5]=' ';
    // rec_dat[6]='0'+(TH/10);
    // rec_dat[7]='0'+(TH%10);
    // rec_dat[8]='C';

    rec_dat[0] = RH;
    rec_dat[1] = RL;
    rec_dat[2] = TH;
    rec_dat[3] = TL;
  }
}
