#include "zf_common_headfile.h"

#ifndef __WIFI_CONTROL_H__
#define __WIFI_CONTROL_H__

void wifiConnect(char *wifi_ssid, char *wifi_password, char *target_ip, char *target_port, char *localport);
void wifiExchangeMessage(uint8 *transmit_message);

#endif