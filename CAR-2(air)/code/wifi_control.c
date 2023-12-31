#include "wifi_control.h"

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WiFi 模块连接热点或WiFi
// 参数说明     wifi_ssid        需要连接的wifi或热点名称
// 参数说明     wifi_password    验证密码
// 参数说明		target_ip		 服务器的ip地址
// 参数说明		target_port		 服务器的端口
// 参数说明		localport		 WiFi模块的本地端口
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void wifiConnect(char *wifi_ssid, char *wifi_password, char *target_ip, char *target_port, char *localport)
{
	// 打印外设初始化语句
	printf("\r\n Wi-Fi UART init.");

	// 将 wifi uart 的中断级别设置为最高
	interrupt_set_priority(UART6_IRQn, 0);

	// 当前使用的是 WIFI 串口模块的 station 模式 可以用于连接路由器 WIFI
	// 发送数据方式可以使用命令发送(默认) 也可以使用透传发送 可以在 wifi_uart_init 函数内修改 <当模块作为服务器时 只能使用命令方式发送数据>
	while (wifi_uart_init(wifi_ssid, wifi_password, WIFI_UART_STATION))
	{
		system_delay_ms(500); // 初始化失败 等待 500ms
	}

	// zf_device_wifi_uart.h 文件内的宏定义可以更改模块连接(建立) WIFI 之后，是否自动连接 TCP 服务器、创建 UDP 连接、创建 TCP 服务器等操作
	if (2 != WIFI_UART_AUTO_CONNECT) // 如果没有开启自动连接 就需要手动连接目标 IP
	{
		if (wifi_uart_connect_udp_client( // 向指定目标 IP 的端口建立 UDP 连接(即电脑)
				target_ip,				  // 这里使用与自动连接时一样的目标 IP 实际使用时也可以直接填写目标 IP 字符串
				target_port,			  // 这里使用与自动连接时一样的目标端口 实际使用时也可以直接填写目标端口字符串
				localport,				  // 这里使用与自动连接时一样的本地端口 也就是自己通信使用的端口号  实际使用时也可以直接填写其他的本地端口字符串
				WIFI_UART_COMMAND))		  // 采用命令传输模式 可以改成透传模式 实际上差别并不是很大
		{
			// 如果一直建立失败 考虑一下是不是没有接硬件复位  开发板的C8串口
			printf("\r\n Connect UDP Client error, try again.");
			system_delay_ms(500); // 建立连接失败 等待 500ms
		}
	}

	// 用于第一次连接确定WiFi模块的ip地址，之后可以注释掉
	printf("\r\n module ip     :%s", wifi_uart_information.wifi_uart_local_ip); // 打印模块 IP 地址
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     WiFi模块传输与接受数据
// 参数说明     transmit_message   接收传输数据的地址
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void wifiExchangeMessage(uint8 *transmit_message)
{
	// 获取TRANSMIT_MESSAGE的大小size
	size_t size = strlen((const char *)transmit_message) + 1; // +1 包含空终止符
	// 向服务端传输的数据
	uint8 wifi_uart_test_buffer[size];
	memcpy(wifi_uart_test_buffer, transmit_message, size);

	// 接收数据
	uint8 wifi_uart_get_data_buffer[256];
	uint16 data_length; // 数据长度

	// 主机发送数据：{#008P2500T1000!,#000P2000T1000!,#009P2500T1000!}
	data_length = wifi_uart_read_buffer(wifi_uart_get_data_buffer, sizeof(wifi_uart_get_data_buffer));

	if (data_length) // 如果接收到数据 则进行数据类型判断
	{
		char *message = strstr((char *)wifi_uart_get_data_buffer, "+IPD");
		if (message) // 判断数据格式是否是通过网络发送过来的数据
		{
			printf("\r\n Get data: <%s>.", wifi_uart_get_data_buffer);
			// 串口解析
			wifi_uart_parse(message + 4, strlen(message) - 4);
		}
	}

	// 发送数据至服务器
	data_length = wifi_uart_send_buffer(wifi_uart_test_buffer, sizeof(wifi_uart_test_buffer));
	if (!data_length)
	{
		printf("\r\n send success."); // 发送成功
	}
	else
	{
		printf("\r\n %d bytes data send failed.", data_length);
	}

	system_delay_ms(1000);
	// 此处编写需要循环执行的代码
}

// 解析wifi串口接受到的消息（指令，坐标）
void wifi_uart_parse(char *wifi_uart_get_data, uint16 length)
{
	char *current = wifi_uart_get_data;
	while (current - wifi_uart_get_data < length)
	{
		// 指令以#开头
		char *pcommand = strstr(current, "#");

		// 找到指令
		if (pcommand != NULL)
		{
			// 示例：#000P1500T1000!
			char command[16];
			memcpy(command, pcommand, 15);
			command[15] = '\0';
			// 串口打印指令
			printf("%s\r\n", command);

			// 更新current
			current = pcommand + 15;
			continue;
		}

		// 坐标以&开头 &后是维度 ~后是经度 以^结束
		char *platitude = strstr(current, "&");
		// 找到坐标
		if (platitude != NULL)
		{
			current = platitude;
			// 示例：&+40.123456~+116.123456^
			// +40.123456  表示北纬 40.123456°
			// +116.123456 表示东经 116.123456°

			char *plongitude = strstr(platitude, "~");
			char *pend = strstr(platitude, "^");
			char latitude[13];
			int llength = plongitude - platitude - 1;

			memcpy(latitude, platitude + 1, llength);
			latitude[llength] = '\0';
			char longitude[13];
			int glength = pend - plongitude - 1;

			memcpy(longitude, plongitude + 1, glength);
			longitude[glength] = '\0';

			// 将经纬度加入fifo中
			location a = {func_str_to_float(latitude), func_str_to_float(longitude)};
			location_fifo_write(&a);
			// 若数据合法，更新p
			if (pend - platitude + 1 <= 26)
			{
				current += pend - platitude;
			}
			continue;
		}
		break;
	}
}
