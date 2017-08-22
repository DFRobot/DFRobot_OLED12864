/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 by Daniel Eichhorn
 *
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

// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "DFRobot_OLED12864.h" 

// Include custom images
#include "images.h"


// Initialize the OLED display using Wire library
DFRobot_OLED12864  display(0x3c, 21, 22);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
 // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();// flip vertical
 display.clear();
 drawImageDemo();
 display.display();

}
void drawImageDemo() {
    display.drawXbm(0, 0, Picture_width, Picture_height, Picture_bits);
}

void loop() {
  // clear the display
 
}
