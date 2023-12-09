#include "pm2_5.h"

void PM25_init()
{
	adc_init(PM25_OUTPUT_PIN, ADC_12BIT);
	system_delay_ms(5);
	gpio_init(PM25_PUL_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL); // 触发引脚 推挽输出
	system_delay_ms(5);
}

double PM25_read()
{
	double pm_ad = 0;
	for (uint8 kk = 0; kk < 20; kk++) // 20次去平均值
	{
		gpio_low(PM25_PUL_PIN); // 输出低
		system_delay_us(320);
		gpio_high(PM25_PUL_PIN); // 输出高
		system_delay_us(280);
		double ad1 = adc_convert(PM25_OUTPUT_PIN);
		system_delay_ms(9);
		system_delay_us(400);
		pm_ad = pm_ad + ad1;
	}
	pm_ad = pm_ad / 20; // PM2.5 取平均值
	return (double)adc_convert(PM25_OUTPUT_PIN) / 4095.0 * 3.3;
}
