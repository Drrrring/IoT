#ifndef _smoke_h
#define _smoke_h
#include "zf_common_headfile.h"

void SMOKE_init(adc_channel_enum pin);
uint8 SMOKE_read(adc_channel_enum pin);

#endif
