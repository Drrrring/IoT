#ifndef _light_h
#define _light_h
#include "zf_common_headfile.h"

void LIGHT_init_digital(gpio_pin_enum pin);
void LIGHT_init_analog(adc_channel_enum pin);
bool LIGHT_read_digital(gpio_pin_enum pin);
uint8 LIGHT_read_analog(adc_channel_enum pin);

#endif
