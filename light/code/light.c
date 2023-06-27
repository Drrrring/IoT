#include "light.h"

void LIGHT_init_digital(gpio_pin_enum pin){
	gpio_init(pin, GPI, GPIO_HIGH, GPI_FLOATING_IN);
	system_delay_ms(5);
}

void LIGHT_init_analog(adc_channel_enum pin){
	adc_init(pin, ADC_12BIT);
	system_delay_ms(5);
}

bool LIGHT_read_digital(gpio_pin_enum pin){	
	return gpio_get_level(pin);
}

uint8 LIGHT_read_analog(adc_channel_enum pin){
	return adc_convert(pin);
}