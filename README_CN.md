# DFRobot_OLED12864
* [English Version](./README.md)

这是基于在Arduino/ESP8266平台上运行的128x64像素OLED显示屏的SSD1306的驱动程序。可与 I2C 或 SPI 版本的显示器一起使用。

Ui 库用于提供一组名为 Frames 和 Overlays 的基本 Ui 元素。Frame 用于提供信息，默认行为是在定义的时间内显示 Frame，然后移动到下一个时间。该库还提供了一个 Indicator 将相应更新的内容。另一方面 Overlay ，是始终显示在同一位置的信息片段（例如时钟）。

![产品实物图](./resources/images/OLED12864.png)

> Please have a look at our [upgrade guide](UPGRADE.md)

## 产品链接 (https://www.dfrobot.com.cn/goods-1675.html)
    SKU: DFR0507

## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性)
* [历史](#历史)
* [创作者](#创作者)


## 概述

* 在给定坐标处绘制像素
* 从给定坐标绘制从给定坐标到给定坐标的线条
* 绘制或填充具有给定尺寸的矩形
* 在给定坐标处绘制文本：
* 定义对齐方式：左、右和居中
* 设置要使用的字体（请参阅下面的字体部分）
* 将文本的宽度限制为像素数。在达到此宽度之前，如果可能，渲染器会将文本换行为新行
* 在自动横向滚动轮播中显示内容
* 定义过渡周期
* 定义一帧的显示时间
* 在回调方法中绘制不同的帧
* 每帧将自动显示一个指示器。活动帧将从非活动状态显示一次

## 库安装

这里有2种安装方法：

1. 使用此库前，请首先下载库文件，将其粘贴到\Arduino\libraries目录中，然后打开examples文件夹并在该文件夹中运行演示。
2. 直接在Arduino软件库管理中搜索下载 DFRobot_OLED12864 库。


## 方法

## I2C with Wire.h

```C++
#include <Wire.h>  
#include "DFRobot_OLED12864.h"

DFRobot_OLED12864  display(ADDRESS, PIN_CHARACTER_CS);

```

## Display Control

```C++
/*
 * @brief OLED init
 */
void init(void);

/*
 * @brief free the memory used by the display
 */
void end(void);

/*
 * @brief cycle through the initialization
 */
void resetDisplay(void);

/*
 * @brief connect again to the display through I2C
 */
void reconnect(void);

/*
 * @brief turn the display on
 */
void displayOn(void);

/*
 * @brief turn the display off
 */
void displayOff(void);

/*
 * @brief clear the local pixel buffer
 */
void clear(void);

/*
 * @brief write the buffer to the display memory
 */
void display(void);

/*
 * @brief inverted display mode
 */
void invertDisplay(void);

/*
 * @brief normal display mode
 */
void normalDisplay(void);

/*
 * @brief set display contrast
 *
 * @param contrast from 0 to 255
 */
void setContrast(char contrast);

/*
 * @brief turn the display upside down
 */
void flipScreenVertically(void);

/*
 * @brief display string
 *
 * @param x from 0 to 127
 * @param y from 0 to 63
 * @param *ch The string you want to show (use character IC)
 */
void disStr(uint8_t x, uint8_t y, char *ch);
```

## Pixel drawing

```C++

/* Drawing functions */

/*
 * @brief sets the color of all pixel operations
 *
 * @param color to display
 *     BLACK        display main color to black
 *     WHITE        display main color to white
 *     INVERSE      display to inverse
 */
void setColor(OLEDDISPLAY_COLOR color);

/*
 * @brief draw a pixel at given position
 *
 * @param x from 0 to 127
 * @param y from 0 to 63
 */
void setPixel(int16_t x, int16_t y);

/*
 * @brief draw a line from position (x0, y0) to position (x1, y1)
 *
 * @param x0 from 0 to 127
 * @param y0 from 0 to 63
 * @param x1 from 0 to 127
 * @param y1 from 0 to 63
 */
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/*
 * @brief draw the border of a rectangle at the given location
 *
 * @param x from 0 to 127
 * @param y from 0 to 63
 * @param width from 0 to 127
 * @param height from 0 to 63
 */
void drawRect(int16_t x, int16_t y, int16_t width, int16_t height);

/*
 * @brief fill the rectangle at the given location
 *
 * @param x from 0 to 127
 * @param y from 0 to 63
 * @param width from 0 to 127
 * @param height from 0 to 63
 */
void fillRect(int16_t x, int16_t y, int16_t width, int16_t height);

/*
 * @brief draw the border of a circle
 *
 * @param x from 0 to 127
 * @param y from 0 to 63
 * @param radius from 0 to 64
 */
void drawCircle(int16_t x, int16_t y, int16_t radius);

/*
 * @brief fill circle
 *
 * @param x from 0 to 127
 * @param y from 0 to 63
 * @param radius from 0 to 64
 */
void fillCircle(int16_t x, int16_t y, int16_t radius);

/*
 * @brief draw a line horizontally at the given location
 *
 * @param x from 0 to 127
 * @param y from 0 to 63
 * @param length from 0 to 127
 */
void drawHorizontalLine(int16_t x, int16_t y, int16_t length);

/*
 * @brief draw a lin vertically at the given location
 *
 * @param x from 0 to 127
 * @param y from 0 to 63
 * @param length from 0 to 63
 */
void drawVerticalLine(int16_t x, int16_t y, int16_t length);

/*
 * @brief draw a lin vertically at the given location
 *
 * @param x from 0 to 127
 * @param y from 0 to 63
 * @param width from 0 to 127
 * @param height from 0 to 63
 * @param progress from 0 to 100
 */
void drawProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t progress);

/*
 * @brief draw a bitmap in the internal image format
 *
 * @param x from 0 to 127
 * @param y from 0 to 63
 * @param width from 0 to 127
 * @param height from 0 to 63
 * @param *image image data
 */
void drawFastImage(int16_t x, int16_t y, int16_t width, int16_t height, const char *image);

/*
 * @brief draw a XBM
 *
 * @param x from 0 to 127
 * @param y from 0 to 63
 * @param width from 0 to 127
 * @param height from 0 to 63
 * @param *xbm xbm data
 */
void drawXbm(int16_t x, int16_t y, int16_t width, int16_t height, const char* xbm);
```

## Text operations

``` C++

/*
 * @brief draw string at the given location
 *
 * @param x from 0 to 127
 * @param y from 0 to 63
 * @param text you want to show (don't use character IC)
 */
void drawString(int16_t x, int16_t y, String text);

/*
 * @brief Draws a String with a maximum width at the given location.
 *        If the given String is wider than the specified width
 *        The text will be wrapped to the next line at a space or dash
 *
 * @param x from 0 to 127
 * @param y from 0 to 63
 * @param maxLineWidth max line width
 * @param text you want to show (don't use character IC)
 */
void drawStringMaxWidth(int16_t x, int16_t y, int16_t maxLineWidth, String text);

/*
 * @brief return string length on OLED(uint pixel)
 *
 * @param test text
 * @param length text length
 */
uint16_t getStringWidth(const char* text, uint16_t length);

/*
 * @brief return string length on OLED(uint pixel)
 *
 * @param test text
 */
uint16_t getStringWidth(String text);

/*
 * @brief specifies relative to which anchor point
 *
 * @param text alignment
 *      TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT, TEXT_ALIGN_CENTER_BOTH
 */
void setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT textAlignment);

/*
 * @brief Sets the current font. Available default fonts
 *
 * @param fontData font data
 *      ArialMT_Plain_10, ArialMT_Plain_16, ArialMT_Plain_24
 */
void setFont(const char* fontData);

```

## Ui Library (OLEDDisplayUi)

```C++
/*
 * @brief initialise the display
 */
void init(void);

/*
 * @brief configure the internal used target FPS
 *
 * @param fps from 0 to 255
 */
void setTargetFPS(uint8_t fps);

/*
 * @brief enable automatic transition to next frame after the some time can be configured with
 *     `setTimePerFrame` and `setTimePerTransition`.
 */
void enableAutoTransition(void);

/*
 * @brief disable automatic transition to next frame.
 */
void disableAutoTransition(void);

/*
 * @brief set the direction if the automatic transitioning
 */
void setAutoTransitionForwards(void);
void setAutoTransitionBackwards(void);

/*
 * @brief set the approx. time a frame is displayed
 *
 * @param time time (unit ms)
 */
void setTimePerFrame(uint16_t time);

/*
 * @brief set the approx. time a transition will take
 *
 * @param time time (unit ms)
 */
void setTimePerTransition(uint16_t time);

/*
 * @brief draw the indicator.
 *    This is the default state for all frames if
 *    the indicator was hidden on the previous frame
 *    it will be slided in.
 */
void enableIndicator(void);

/*
 * @brief don't draw the indicator.
 *    This will slide out the indicator
 *    when transitioning to the next frame.
 */
void disableIndicator(void);

/*
 * @brief enable drawing of all indicators.
 */
void enableAllIndicators(void);

/*
 * @brief disable drawing of all indicators.
 */
void disableAllIndicators(void);

/*
 * @brief set the position of the indicator bar.
 *
 * @param pos indicator pos
 *     BOOTM, TOP, RIGHT, LEFT
 */
void setIndicatorPosition(IndicatorPosition pos);

/*
 * @brief set the direction of the indicator bar. Defining the order of frames ASCENDING / DESCENDING
 *
 * @param dir indicator dirction
 *     LEFT_RIGHT, RIGHT_LEFT
 */
void setIndicatorDirection(IndicatorDirection dir);

/*
 * @brief set the symbol to indicate an active frame in the indicator bar.
 *
 * @param symbol active symbol
 *     ANIMATION_activeSymbol
 */
void setActiveSymbol(const char* symbol);

/*
 * @brief set the symbol to indicate an inactive frame in the indicator bar.
 *
 * @param symbol inactive symbol
 *     ANIMATION_inactiveSymbol
 */
void setInactiveSymbol(const char* symbol);

/*
 * @brief configure what animation is used to transition from one frame to another
 *
 * @param dir frame animation dirction
 *     SLIDE_UP, SLIDE_DOWN, SLIDE_LEFT, SLIDE_RIGHT
 */
void setFrameAnimation(AnimationDirection dir);

/*
 * @brief add frame drawing functions
 *
 * @param frameFunctions your frame functions
 * @param frameCount your frames count
 */
void setFrames(FrameCallback* frameFunctions, uint8_t frameCount);

/*
 * @brief add overlays drawing functions that are draw independent of the Frames
 *
 * @param overlayFunctions your overlay function
 * @param overlayCount your overlays count
 */
void setOverlays(OverlayCallback* overlayFunctions, uint8_t overlayCount);

/*
 * @brief set the function that will draw each step
 *     in the loading animation
 *
 * @param loadingDrawFunction load your function
 */
void setLoadingDrawFunction(LoadingDrawFunction loadingDrawFunction);

/*
 * @brief run the loading process
 *
 * @param *stages your stages
 * @param stagesCount your stages count
 */
void runLoadingProcess(LoadingStage* stages, uint8_t stagesCount);

/*
 * @brief manuell Controll
 */
void nextFrame(void);
void previousFrame(void);

/*
 * @brief switch without transition to frame `frame`.
 *
 * @param frame select next frame
 */
void switchToFrame(uint8_t frame);

/*
 * @brief transition to frame `frame`, when the `frame` number is bigger than the current
 *    frame the forward animation will be used, otherwise the backwards animation is used.
 *
 * @param frame the frame you want to show
 */
void transitionToFrame(uint8_t frame);

/*
 * @brief return display info
 */
OLEDDisplayUiState* getUiState(void);

/*
 * @brief This needs to be called in the main loop
 *     the returned value is the remaining time (in ms)
 *     you have to draw after drawing to keep the frame budget.
 */
int8_t update(void);

```


## 兼容性

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | :----:
FireBeetle-ESP8266 |      √       |              |             |
FireBeetle-ESP32   |      √       |              |             |
FireBeetle-BLE4.1  |              |       √      |             |


## 历史

- 2022/04/02 - 1.0.0 版本


## 创作者

Written by wxzed(xiao.wu@dfrobot.com), 2022. (Welcome to our [website](https://www.dfrobot.com/))

