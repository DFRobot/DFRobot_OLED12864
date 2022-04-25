/*!
 * @file  keyDemo.ino
 * @brief  Key function demo
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence  The MIT License (MIT)
 * @author  [wxzed](xiao.wu@dfrobot.com)
 * @maintainer  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-04-21
 * @url  https://github.com/DFRobot/DFRobot_OLED12864
 */
#include "DFRobot_OLED12864.h"

#ifdef 					__ets__       //esp8266
#define ADC_BIT 1024
#elif 	defined ESP_PLATFORM  //esp32
#define ADC_BIT	4096
#else
#define ADC_BIT 1024          //other MCU, revise according to actual situation
#endif

#define ADC_SECTION 5

#ifdef __AVR__
const uint8_t pin_character_cs = 5, keyA = 3, keyB = 8;
#elif ((defined __ets__) || (defined ESP_PLATFORM))
const uint8_t pin_character_cs = D5, keyA = D3, keyB = D8;
#endif

const uint8_t I2C_OLED_addr = 0x3c;
const uint8_t pin_analogKey = A0;

DFRobot_OLED12864 OLED(I2C_OLED_addr, pin_character_cs);

const char drawNull[256] = {0};

enum enum_key_analog {
	key_analog_no,
	key_analog_right,
	key_analog_center,
	key_analog_up,
	key_analog_left,
	key_analog_down,
} eKey_analog;


enum_key_analog read_key_analog(void)
{
  int adValue = analogRead(pin_analogKey);
  if(adValue > ADC_BIT * (ADC_SECTION * 2 - 1) / (ADC_SECTION * 2)) {
    return key_analog_no;
  } else if(adValue > ADC_BIT * (ADC_SECTION * 2 - 3) / (ADC_SECTION * 2)) {
    return key_analog_right;
  } else if(adValue > ADC_BIT * (ADC_SECTION * 2 - 5) / (ADC_SECTION * 2)) {
    return key_analog_center;
  } else if(adValue > ADC_BIT * (ADC_SECTION * 2 - 7) / (ADC_SECTION * 2)) {
    return key_analog_up;
  } else if(adValue > ADC_BIT * (ADC_SECTION * 2 - 9) / (ADC_SECTION * 2)) {
    return key_analog_left;
  } else {
    return key_analog_down;
  }
}


void setup(void)
{
  pinMode(keyA, INPUT);
  pinMode(keyB, INPUT);
  Serial.begin(115200);
  OLED.init();
  OLED.flipScreenVertically();
  OLED.disStr(0, 0, "没有按键按下");
}

void loop(void)
{
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
  eKey_analog = read_key_analog();
  OLED.drawXbm(0, 16, 128, 16, drawNull);
  switch(eKey_analog) {
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
