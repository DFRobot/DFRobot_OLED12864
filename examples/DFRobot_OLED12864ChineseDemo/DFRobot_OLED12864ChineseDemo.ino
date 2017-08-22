#include "DFRobot_OLED12864.h"

const uint8_t I2C_addr = 0x3c;
const uint8_t pin_SPI_cs = 25;

DFRobot_OLED12864 OLED(I2C_addr, pin_SPI_cs);


void setup(void) {

  Serial.begin(115200);
  OLED.init();
  OLED.flipScreenVertically();
}


void loop(void) {

  OLED.disStr(0, 0, "成都极趣科技成都极趣科技");
  OLED.display();
  Serial.println("show");
  delay(60000);
}

