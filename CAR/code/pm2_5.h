#ifndef _pm25_h
#define _pm25_h
#include "zf_common_headfile.h"

#define PM25_PUL_PIN (B0)
#define PM25_OUTPUT_PIN (ADC1_CH5_A5)

void PM25_init();
double PM25_read();

#endif
