#include "mic.h"

void MIC_init(adc_channel_enum pin){
	adc_init(pin, ADC_12BIT);
	system_delay_ms(5);
}

uint8 MIC_read(adc_channel_enum pin){
	return adc_convert(pin);
}
