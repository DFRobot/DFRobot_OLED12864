#include "DFRobot_OLED12864.h"

#define ADC_BIT     4096
#define ADC_SECTION 5

const uint8_t pin_character_cs = 25;
const uint8_t I2C_OLED_addr = 0x3c;
const uint8_t keyA = D7;
const uint8_t keyB = D8;
const uint8_t pin_analogKey = A0;

DFRobot_OLED12864 OLED(I2C_OLED_addr, pin_character_cs);

const char drawNull[256] = {0};

enum {
key_analog_no,
key_analog_right,
key_analog_center,
key_analog_up,
key_analog_left,
key_analog_down,
} key_analog_status;


uint8_t read_key_analog(void) {

  int adValue = analogRead(pin_analogKey);
  if(adValue > ADC_BIT * (ADC_SECTION * 2 - 1) / (ADC_SECTION * 2)) {
    return key_analog_no;
  }
  else if(adValue > ADC_BIT * (ADC_SECTION * 2 - 3) / (ADC_SECTION * 2)) {
    return key_analog_right;
  }
  else if(adValue > ADC_BIT * (ADC_SECTION * 2 - 5) / (ADC_SECTION * 2)) {
    return key_analog_center;
  }
  else if(adValue > ADC_BIT * (ADC_SECTION * 2 - 7) / (ADC_SECTION * 2)) {
    return key_analog_up;
  }
  else if(adValue > ADC_BIT * (ADC_SECTION * 2 - 9) / (ADC_SECTION * 2)) {
    return key_analog_left;
  }
  else {
    return key_analog_down;
  }
}


void setup(void) {

  pinMode(keyA, INPUT);
  pinMode(keyB, INPUT);
  Serial.begin(115200);
  OLED.init();
  OLED.flipScreenVertically();
  OLED.disStr(0, 0, "没有按键按下");
  
}

void loop(void) {

  OLED.clear();
  if(digitalRead(keyA) == 0) {
    delay(10);
    if(digitalRead(keyA) == 0) {
      Serial.println("KEY A PRESSED");
      OLED.drawXbm(0, 0, 128, 16, drawNull);
      OLED.disStr(0, 0, "按键A被按下");
    }
  } else if(digitalRead(keyB) == 0) {
    delay(10);
    if(digitalRead(keyB) == 0) {
      Serial.println("KEY B PRESSED");
      OLED.drawXbm(0, 0, 128, 16, drawNull);
      OLED.disStr(0, 0, "按键B被按下");
    }
  } else {
    OLED.drawXbm(0, 0, 128, 16, drawNull);
    OLED.disStr(0, 0, "没有按键被按下");
  }
  uint8_t keyAnalog = 0;
  keyAnalog = read_key_analog();
  OLED.drawXbm(0, 16, 128, 16, drawNull);
  switch(keyAnalog) {
    case key_analog_no:     OLED.disStr(0, 16, "模拟按键未被按下");break;
    case key_analog_up:     OLED.disStr(0, 16, "模拟按键向上");break;
    case key_analog_down:   OLED.disStr(0, 16, "模拟按键向下");break;
    case key_analog_right:  OLED.disStr(0, 16, "模拟按键向右");break;
    case key_analog_left:   OLED.disStr(0, 16, "模拟按键向左");break;
    case key_analog_center: OLED.disStr(0, 16, "模拟按键中间按下");break;
  }
  OLED.display();
  delay(50);
}
