#include "gps.h"

void GPS_read()
{
    // gps数据接收与解析都是通过串口中断调用gps_uart_callback函数进行实现的
    // 数据解析完毕之后gps_tau1201_flag标志位会置1
    if (gps_tau1201_flag)
    {
        gps_tau1201_flag = 0;

        if (!gps_data_parse()) // 开始解析数据
        {
            // printf("now time:  ");                                                                                                // 输出年月日时分秒
            // printf("year-%d, month-%d, day-%d\r\n", gps_tau1201.time.year, gps_tau1201.time.month, gps_tau1201.time.day);         // 输出年月日时分秒
            // printf("hour-%d, minute-%d, second-%d\r\n", gps_tau1201.time.hour, gps_tau1201.time.minute, gps_tau1201.time.second); // 输出年月日时分秒
            // printf("gps_state       = %d\r\n", gps_tau1201.state);                                                                // 输出当前定位有效模式 1：定位有效  0：定位无效
            // printf("latitude        = %lf\r\n", gps_tau1201.latitude);                                                            // 输出纬度信息
            // printf("longitude       = %lf\r\n", gps_tau1201.longitude);                                                           // 输出经度信息
            // printf("speed           = %lf\r\n", gps_tau1201.speed);                                                               // 输出速度信息
            // printf("direction       = %lf\r\n", gps_tau1201.direction);                                                           // 输出方向信息
            // printf("satellite_used  = %d\r\n", gps_tau1201.satellite_used);                                                       // 输出当前用于定位的卫星数量
            // printf("height          = %lf\r\n", gps_tau1201.height);                                                              // 输出当前GPS天线所处高度
            printf("GPS read success\r\n");
        } else {
            printf("GPS read fail\r\n");
        }
    }
}

uint8 GPS_get_year(){
    return gps_tau1201.time.year;
}

uint8 GPS_get_month(){
    return gps_tau1201.time.month;
}

uint8 GPS_get_day(){
    return gps_tau1201.time.day;
}

uint8 GPS_get_hour(){
    return gps_tau1201.time.hour;
}

uint8 GPS_get_minute(){
    return gps_tau1201.time.minute;
}

uint8 GPS_get_second(){
    return gps_tau1201.time.second;
}

uint8 GPS_get_state(){
    return gps_tau1201.state;
}

double GPS_get_latitude(){
    return gps_tau1201.latitude;
}

double GPS_get_longitude(){
    return gps_tau1201.longitude;
}

double GPS_get_speed(){
    return gps_tau1201.speed;
}

double GPS_get_direction(){
    return gps_tau1201.direction;
}

uint8 GPS_get_satellite_used(){
    return gps_tau1201.satellite_used;
}

double GPS_get_height(){
    return gps_tau1201.height;
}
