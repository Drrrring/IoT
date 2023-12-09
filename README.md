# IoT
The repo is for IoT competition, including codes for device end (base on MM32F5277 mother board).

## 目录说明
CAR和CAR-2文件夹为完整的工程代码，其中两辆车烧录的代码不同（因为当时传感器不够，两辆车搭载的传感器不同，小车id号不同）  
其他文件夹为各传感器的测试代码

## 引脚说明
 | 传感器           | VCC     | 引脚1              | 引脚2              | 引脚3            |
 | ---------------- | ------- | ------------------ | ------------------ | ---------------- |
 | 温湿度计         | 3.3V/5V | S - D10            | 中间 - VCC         |
 | 火焰（已移除）   | 3.3V/5V | DO - E4            |                    |
 | 光照（已移除）   | 3.3V/5V | AO - A4            |                    |
 | 麦克风（已移除） | 5V      | S - A5             |                    |
 | 烟雾（MQ2）      | 5V      | AO - A6            |
 | GPS              | 3.3V/5V | TX - C11(核心板RX) | RX - C10(核心板TX) | (PPS可不管)      |
 | MPU6050          | 3.3V/5V | SCL - B13          | SDA - B15          | (ADO和INT可不管) |
 | WiFi             | 3.3V/5V | TX - C7(核心板RX)  | RX - C6(核心板TX)  | RST - C8         |
 | 超声波（未搭载）  | 5V      | TRIG - B0          | ECHO - B1          |
 | 噪声             | 5V      | RX - D8            | TX - D9            |
 | MQ7              | 5V      | AO - A7            |
 | MQ135            | 5V      | AO - A4            |
 | PM2.5（已换新传感器）| 5V      | AO - A5            | PUL - B0           |
 | 空气（新）          |5V       | RX - D8            |TX - D9|

 ## CAR文件夹的目录说明
 - code中为各模块的代码。其中pid.c中为控制小车行进到指定目的地的代码，其余为传感器控制代码。
 - mdk中为keil工程项目文件，双击.uvprojx文件即可用keil打开该工程
 - user中为主函数和中断的相关函数