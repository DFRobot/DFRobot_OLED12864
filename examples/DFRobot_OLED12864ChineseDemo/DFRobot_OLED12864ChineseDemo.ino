#include "DFRobot_OLED12864.h"

const uint8_t I2C_addr = 0x3c;
const uint8_t pin_SPI_cs = D5;

DFRobot_OLED12864 OLED(I2C_addr, pin_SPI_cs);


void setup(void)
{
  Serial.begin(115200);
  OLED.init();
  OLED.flipScreenVertically();
}


void loop(void)
{
  const char str_fireBeetle[] = "fireBeetle萤火虫系列";
  
  OLED.disStr(0, 0, (char *)str_fireBeetle);
  OLED.disStr(0, 32, "中文标点：，。");
	OLED.disStr(0, 48, "英文标点:,.");
  OLED.display();
  delay(1000);
}    
