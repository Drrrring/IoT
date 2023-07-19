#include "fire.h"

void FIRE_init(gpio_pin_enum pin){
	gpio_init(pin, GPI, GPIO_HIGH, GPI_FLOATING_IN);
	system_delay_ms(5);
}


bool FIRE_read(gpio_pin_enum pin){	
	return gpio_get_level(pin);
}

