# IoT
The repo is for IoT competition, including codes for device end (base on MM32F5277 mother board).

## 目录说明
CAR文件夹为完整的工程代码
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
 | 超声波           | 5V      | TRIG - B0          | ECHO - B1          |
 | 噪声             | 5V      | RX - D8            | TX - D9            |
 | MQ7              | 5V      | AO - A7            |
 | MQ135            | 5V      | AO - A4            |
 | PM2.5            | 5V      | AO - A5            | PUL - B0           |