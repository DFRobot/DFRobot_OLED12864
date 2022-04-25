/*!
 * @file  testAll.ino
 * @brief  All functions are combined for testing
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence  The MIT License (MIT)
 * @author  [wxzed](xiao.wu@dfrobot.com)
 * @maintainer  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-04-21
 * @url  https://github.com/DFRobot/DFRobot_OLED12864
 */
#include <DFRobot_BME280.h>
#include "DFRobot_OLED12864.h"

#ifdef          __ets__       //esp8266
#define ADC_BIT 1024
#elif defined   ESP_PLATFORM  //esp32
#define ADC_BIT 4096
#else
#define ADC_BIT 1024          //other MCU, revise according to actual situation
#endif

#define ADC_SECTION 5
#define SEA_LEVEL_PRESSURE 1013.25f

#ifdef __AVR__
const uint8_t pin_SPI_cs = 5, keyA = 3, keyB = 8;
#elif ((defined __ets__) || (defined ESP_PLATFORM))
const uint8_t pin_SPI_cs = D5, keyA = D3, keyB = D8;
#endif

const uint8_t  I2C_addr = 0x3c;
const char     str_num[] = "0123456789";

bool           metric = false;
const uint8_t  pin_analogKey = A0;

DFRobot_BME280    bme;
DFRobot_OLED12864 OLED(I2C_addr, pin_SPI_cs);

enum enum_key_analog {
	key_analog_no,
	key_analog_right,
	key_analog_center,
	key_analog_up,
	key_analog_left,
	key_analog_down,
} key_analog;

int8_t a = 0, b = 0, c = 0;
char str_temp[] = "T:00.00  C";
char str_hum[]  = "H:00.00% RH";
char str_pres[] = "P:00000  pa";
char str_alt[]  = "A:00000  m";
char str_x[] = "X:+00";
char str_y[] = "Y:+00";
char str_z[] = "Z:+00";


enum_key_analog read_key_analog(void)
{
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


void numToStr(char *ch, float dat)
{
  int var = dat * 100;
  ch[2] = str_num[var / 1000];
  ch[3] = str_num[var / 100 % 10];
  ch[5] = str_num[var / 10 % 10];
  ch[6] = str_num[var % 10];
}


void numToStr1(char *ch, int8_t dat)
{
  if(dat < 0) {
    ch[2] = '-';
  } else {
    ch[2] = '+';
  }
  dat = abs(dat);
  ch[3] = str_num[dat / 10];
  ch[4] = str_num[dat % 10];
}


void numToStr2(char *ch, float dat)
{
  uint16_t var = (uint16_t) dat;
  ch[2] = str_num[var / 10000];
  ch[3] = str_num[var / 1000 % 10];
  ch[4] = str_num[var / 100 % 10];
  ch[5] = str_num[var / 10 % 10];
  ch[6] = str_num[var % 10];
}


void AccelerometerInit(void)
{
  byte Version[3] = {0};
  int8_t x_data = 0, y_data = 0, z_data = 0;
  Wire.beginTransmission(0x0B); // address of the accelerometer 
  // reset the accelerometer 
  Wire.write(0x04); // Y data
  Wire.endTransmission(); 
  Wire.requestFrom(0x0B,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
  	Version[0] = Wire.read(); // receive a byte as characte
  }  
  x_data = (int8_t)Version[0] >> 2;
  
  Wire.beginTransmission(0x0B); // address of the accelerometer 
  // reset the accelerometer 
  Wire.write(0x06); // Y data
  Wire.endTransmission(); 
  Wire.requestFrom(0x0B,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
  	Version[1] = Wire.read(); // receive a byte as characte
  }  
  y_data = (int8_t)Version[1] >> 2;
  
  Wire.beginTransmission(0x0B); // address of the accelerometer 
  // reset the accelerometer 
  Wire.write(0x08); // Y data
  Wire.endTransmission(); 
  Wire.requestFrom(0x0B,1);    // request 6 bytes from slave device #2
  while(Wire.available()) 
  { 
  	Version[2] = Wire.read(); // receive a byte as characte
  }
  z_data = (int8_t)Version[2] >> 2; 
  a = x_data;
  b = y_data;
  c = z_data;
  Serial.print("acc_X = ");
  Serial.print(a);
  Serial.print("  ");
  Serial.print("acc_Y = ");
  Serial.print(b);
  Serial.print("  ");
  Serial.print("acc_Z = ");
  Serial.println(c);
}


void setup(void)
{
  delay(500);
  pinMode(keyA, INPUT);
  pinMode(keyB, INPUT);
  Serial.begin(115200);
  bme.begin(0x76);
  Serial.println();
  OLED.init();
  OLED.flipScreenVertically();
  Wire.beginTransmission(0x0B); // address of the accelerometer 
  Wire.write(0x20); 
  Wire.write(0x05);
}


void loop(void)
{
  OLED.clear();
  if(digitalRead(keyA) == 0) {
    delay(10);
    if(digitalRead(keyA) == 0) {
      Serial.println("KEY A PRESSED");
      while(digitalRead(keyA) == 0) {
        OLED.clear();
        float temp, hum, pre, alt;
        temp = bme.temperatureValue();
				pre = bme.pressureValue();
				hum = bme.humidityValue();
				alt = bme.altitudeValue(SEA_LEVEL_PRESSURE);
        numToStr(str_temp, temp);
        numToStr(str_hum, hum);
        numToStr2(str_pres, pre);
        numToStr2(str_alt, alt);
        Serial.print(str_temp);
        Serial.print("  ");
        Serial.print(str_hum);
        Serial.print("  ");
        Serial.print(str_pres);
        Serial.print("  ");
        Serial.println(str_alt);
        OLED.clear();
        OLED.disStr(0, 0, str_temp);
        OLED.disStr(0, 16, str_hum);
        OLED.disStr(0, 32, str_pres);
        OLED.disStr(0, 48, str_alt);
        OLED.display();
        delay(100);
      }
    }
  } else if(digitalRead(keyB) == 0) {
    delay(10);
    if(digitalRead(keyB) == 0) {
      Serial.println("KEY B PRESSED");
      while(digitalRead(keyB) == 0) {
        OLED.clear();
        AccelerometerInit();
        numToStr1(str_x, a);
        numToStr1(str_y, b);
        numToStr1(str_z, c);
        OLED.clear();
        OLED.disStr(0, 32, str_x);
        OLED.disStr(64, 32, str_y);
        OLED.disStr(0, 48, str_z);
        OLED.display();
        delay(100);
      }
    }
  } else {
    OLED.disStr(0, 0, "没有按键被按下");
  }
  key_analog = read_key_analog();
  switch(key_analog) {
    case key_analog_no:     OLED.disStr(0, 16, "模拟按键未被按下");break;
    case key_analog_up:     OLED.disStr(0, 16, "模拟按键向上");break;
    case key_analog_down:   OLED.disStr(0, 16, "模拟按键向下");break;
    case key_analog_right:  OLED.disStr(0, 16, "模拟按键向右");break;
    case key_analog_left:   OLED.disStr(0, 16, "模拟按键向左");break;
    case key_analog_center: OLED.disStr(0, 16, "模拟按键中间按下");break;
  }
  OLED.display();
}