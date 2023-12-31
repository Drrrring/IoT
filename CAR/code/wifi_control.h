#include "zf_common_headfile.h"

#ifndef __WIFI_CONTROL_H__
#define __WIFI_CONTROL_H__
#include "location.h"

extern my_fifo_struct location_fifo;
extern location location_float_buffer[LOCATION_FLOAT_BUFFER_SIZE];

void wifiConnect(char *wifi_ssid, char *wifi_password, char *target_ip, char *target_port, char *localport);
void wifiExchangeMessage(uint8 *transmit_message);
void wifi_uart_parse(char *wifi_uart_get_data, uint16 length);

#endif