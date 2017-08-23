#include <Wire.h>
#include <BME280I2C.h>
#include "DFRobot_OLED12864.h"

#define ADC_BIT     4096
#define ADC_SECTION 5

const uint8_t I2C_addr = 0x3c;
const uint8_t pin_SPI_cs = 25;
const char str_num[] = "0123456789";
bool metric = false;
const uint8_t keyA = D7;
const uint8_t keyB = D8;
const uint8_t pin_analogKey = A0;

BME280I2C bme;
DFRobot_OLED12864 OLED(I2C_addr, pin_SPI_cs);

enum {
key_analog_no,
key_analog_right,
key_analog_center,
key_analog_up,
key_analog_left,
key_analog_down,
} key_analog_status;

int8_t a = 0, b = 0, c = 0;
char str_temp[] = "T:00.00  C";  //温度
char str_hum[]  = "H:00.00% RH";  //湿度
char str_pres[] = "P:00.00  ATM";  //压力
char str_alt[]  = "A:0000   FT";  //海拔
char str_dew[]  = "D:00.00  F";  //露点
char str_x[] = "X:+00";
char str_y[] = "Y:+00";
char str_z[] = "Z:+00";


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


void numToStr(char *ch, float dat) {

  int temp = 0;
  if(dat < 500) {
    temp = dat * 100;
    ch[2] = str_num[temp / 1000];
    ch[3] = str_num[temp / 100 % 10];
    ch[5] = str_num[temp / 10 % 10];
    ch[6] = str_num[temp % 10];
  } else {
    temp = dat;
    ch[2] = str_num[temp / 1000];
    ch[3] = str_num[temp / 100 % 10];
    ch[4] = str_num[temp / 10 % 10];
    ch[5] = str_num[temp % 10];
  }
}


void numToStr1(char *ch, int8_t dat) {

  if(dat < 0) {
    ch[2] = '-';
  } else {
    ch[2] = '+';
  }
  dat = abs(dat);
  ch[3] = str_num[dat / 10];
  ch[4] = str_num[dat % 10];
}


void AccelerometerInit(void) { 

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
  while(Wire.available()) { 
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


void setup(void) {

  pinMode(keyA, INPUT);
  pinMode(keyB, INPUT);
  Serial.begin(115200);
  bme.begin();
  Serial.println();
  OLED.init();
  OLED.flipScreenVertically();
  Wire.beginTransmission(0x0B); // address of the accelerometer 
  Wire.write(0x20); 
  Wire.write(0x05);
}


void loop(void) {

  OLED.clear();
  if(digitalRead(keyA) == 0) {
    delay(10);
    if(digitalRead(keyA) == 0) {
      Serial.println("KEY A PRESSED");
      while(digitalRead(keyA) == 0) {
        OLED.clear();
        float temp(NAN), hum(NAN), pres(NAN);
        uint8_t pressureUnit(3);
        float altitude = bme.alt(metric);
        bme.read(pres, temp, hum, metric, pressureUnit);
        temp = (temp - 32) / 1.8;
        numToStr(str_temp, temp);
        numToStr(str_hum, hum);
        numToStr(str_pres, pres);
        numToStr(str_alt, altitude);
        Serial.print(str_temp);
        Serial.print("  ");
        Serial.print(str_hum);
        Serial.print("  ");
        Serial.print(str_pres);
        Serial.print("  ");
        Serial.print(str_alt);
        Serial.print("  ");
        Serial.println(str_dew);
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
        float dewPoint = bme.dew(metric);
        AccelerometerInit();
        numToStr1(str_x, a);
        numToStr1(str_y, b);
        numToStr1(str_z, c);
        numToStr(str_dew, dewPoint);
        OLED.clear();
        OLED.disStr(0, 0, str_dew);
        OLED.disStr(0, 16, "成都极趣科技");
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
  uint8_t keyAnalog = 0;
  keyAnalog = read_key_analog();
  switch(keyAnalog) {
    case key_analog_no:     OLED.disStr(0, 16, "模拟按键未被按下");break;
    case key_analog_up:     OLED.disStr(0, 16, "模拟按键向上");break;
    case key_analog_down:   OLED.disStr(0, 16, "模拟按键向下");break;
    case key_analog_right:  OLED.disStr(0, 16, "模拟按键向右");break;
    case key_analog_left:   OLED.disStr(0, 16, "模拟按键向左");break;
    case key_analog_center: OLED.disStr(0, 16, "模拟按键中间按下");break;
  }
  OLED.display();
}

