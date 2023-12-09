#ifndef _dht11_h
#define _dht11_h
#include "zf_common_headfile.h"

//初始化芯片
void DHT11_start(gpio_pin_enum pin);
uint8 DHT11_rec_byte(gpio_pin_enum pin);
void DHT11_receive(gpio_pin_enum pin, uint8* rec_dat);


#endif
