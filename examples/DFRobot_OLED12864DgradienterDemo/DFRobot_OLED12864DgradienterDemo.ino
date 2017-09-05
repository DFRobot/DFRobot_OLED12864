/**
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <Wire.h>  
#include "DFRobot_OLED12864.h" 
#include "images.h"

byte Version[3];
int8_t x_data;
int8_t y_data;
int8_t z_data;

int8_t a;
int8_t b;
int8_t c;


int8_t minX = 0;
int8_t maxX = 0;
int8_t minY = 0;
int8_t maxY = 0;
int8_t minZ = 0;
int8_t maxZ = 0;
uint8_t k;
uint8_t o;

DFRobot_OLED12864  display(0x3c);

void setup()
{
	Serial.begin(115200);
	// Initialising the UI will init the display too.
	display.init();
	display.flipScreenVertically();// flip vertical
	Wire.begin(); 
	Wire.beginTransmission(0x0B); // address of the accelerometer 
	// low pass filter, range settings 
	Wire.write(0x20); 
	Wire.write(0x05); 
	Wire.endTransmission();
}

void AccelerometerInit() 
{ 
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
	while(Wire.available())    // slave may send less than requested
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

void drawImageDemo(void)
{
  display.drawXbm(k, o, Picture_width, Picture_height, Picture_bits);
}

void loop(void)
{ 
  if(a > 0) {
    k = 64 - (a * 4);
  } else {
    k = 64 + (-(a * 4));
  }
  if(b > 0) {
    o = 32 + (b * 2);
  } else {
    o = 32 - (-(b * 2));
  }
  if(k > 111) {
    k = 111;
  }
  if(o > 47) {
    o = 47;
  }
  
  AccelerometerInit();
  display.clear();
  drawImageDemo();
  display.display();
  delay(40);
}
