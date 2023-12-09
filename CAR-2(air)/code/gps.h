#ifndef _gps_h
#define _gps_h
#include "zf_common_headfile.h"

void GPS_read();

uint8 GPS_get_year();

uint8 GPS_get_month();

uint8 GPS_get_day();

uint8 GPS_get_hour();

uint8 GPS_get_minute();

uint8 GPS_get_second();

uint8 GPS_get_state();

double GPS_get_latitude();

double GPS_get_longitude();

double GPS_get_speed();

double GPS_get_direction();

uint8 GPS_get_satellite_used();

double GPS_get_height();

#endif