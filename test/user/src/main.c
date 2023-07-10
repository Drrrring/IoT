/*********************************************************************************************************************
 * MM32F527X-E9P Opensourec Library 即（MM32F527X-E9P 开源库）是一个基于官方 SDK
 *接口的第三方开源库 Copyright (c) 2022 SEEKFREE 逐飞科技
 *
 * 本文件是 MM32F527X-E9P 开源库的一部分
 *
 * MM32F527X-E9P 开源库 是免费软件
 * 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即
 *GNU通用公共许可证）的条款 即 GPL 的第3版（即
 *GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
 *
 * 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
 * 甚至没有隐含的适销性或适合特定用途的保证
 * 更多细节请参见 GPL
 *
 * 您应该在收到本开源库的同时收到一份 GPL 的副本
 * 如果没有，请参阅<https://www.gnu.org/licenses/>
 *
 * 额外注明：
 * 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
 * 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt
 *文件中 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
 * 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
 *
 * 文件名称          main
 * 公司名称          成都逐飞科技有限公司
 * 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
 * 开发环境          MDK 5.37
 * 适用平台          MM32F527X_E9P
 * 店铺链接          https://seekfree.taobao.com/
 *
 * 修改记录
 * 日期              作者                备注
 * 2022-08-10        Teternal            first version
 ********************************************************************************************************************/

#include "location.h"
#include "zf_common_headfile.h"
// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// 本例程是开源库移植用空工程

// **************************** 代码区域 ****************************

static float buffer[200];
int main(void) {
  clock_init(SYSTEM_CLOCK_120M); // 初始化芯片时钟 工作频率为 120MHz
  debug_init();                  // 初始化默认 Debug UART

  // 此处编写用户代码 例如外设初始化代码等

  // 此处编写用户代码 例如外设初始化代码等
  location_fifo_init();
	fifo_struct* pstruct = &location_float_fifo;
  
	float* p = location_float_fifo.buffer;
  float a = 1.1f;
  float b = 2.2f;
  location_fifo_write_float(&location_float_fifo, &a);
  location_fifo_write_float(&location_float_fifo, &b);
	printf("%f\r\n", location_float_buffer[0]);
	printf("%f\r\n", location_float_buffer[1]);

	float res1 = 0;
	float res2 = 0;
  float *temp1 = &res1;
  float *temp2 = &res2;
  location_fifo_read_float(&location_float_fifo, temp1);
  location_fifo_read_float(&location_float_fifo, temp1);
	printf("%f\r\n", location_float_buffer[0]);
	printf("%f\r\n", location_float_buffer[1]);
	
  printf("temp1: %f \r\n", *temp1);
  printf("temp2: %f \r\n", *temp2);

//	float* p = buffer;
//	buffer[0] = 1.1;
//	float a = 1.2;
//	float temp = a + buffer[0];
//	buffer[2] = temp;
//	buffer[11] = 1;
//	printf("%f\r\n", buffer[0]);
  while (1) {
  }
}
// **************************** 代码区域 ****************************
