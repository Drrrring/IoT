#ifndef _mic_h
#define _mic_h
#include "zf_common_headfile.h"

void MIC_init(adc_channel_enum pin);
uint8 MIC_read(adc_channel_enum pin);

#endif
