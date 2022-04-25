/*!
 * @file  OATDemo.ino
 * @brief  The progress bar displays Demo
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
#include "DFRobot_OLED12864.h" // alias for `#include "DFRobot_OLED12864Wire.h"`


// Initialize the OLED display using Wire library
DFRobot_OLED12864  display(0x3c);

int counter = 1;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();

}

void drawProgressBarDemo()
{
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
}

void loop()
{
	// clear the display
	display.clear();
	// draw the current demo method
	drawProgressBarDemo();

	// write the buffer to the display
	display.display();
	counter++;
	delay(10);
}
