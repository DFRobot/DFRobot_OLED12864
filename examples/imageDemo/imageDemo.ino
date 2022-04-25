/*!
 * @file  imageDemo.ino
 * @brief  Picture display demo  
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence  The MIT License (MIT)
 * @author  [wxzed](xiao.wu@dfrobot.com)
 * @maintainer  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-04-21
 * @url  https://github.com/DFRobot/DFRobot_OLED12864
 */
// Include the correct display library
// For a connection via I2C using Wire include
#include "DFRobot_OLED12864.h" 

// Include custom images
#include "images.h"


// Initialize the OLED display using Wire library
DFRobot_OLED12864  display(0x3c);

void setup()
{
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

void drawImageDemo()
{
  display.drawXbm(0, 0, Picture_width, Picture_height, Picture_bits);
}

void loop()
{
}
