
#include "fire.h"
#include "zf_common_headfile.h"


#define FIRE_PIN (B1)          // 传感器DO 接 板子B1
// VCC: 3.3V/5V
// 有火焰时输出低电平

int main(void) {
  clock_init(SYSTEM_CLOCK_120M); // 初始化芯片时钟 工作频率为 120MHz
  debug_init();                  // 初始化默认 Debug UART
  
  FIRE_init(FIRE_PIN);
  uint8 fire_value = 200;
  while (1) {
    
    fire_value = FIRE_read(FIRE_PIN);
    printf("fire: %d \r\n", fire_value);
    system_delay_ms(1000);
    
  }
}
