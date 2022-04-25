/*!
 * @file  OLEDDisplay.h
 * @brief  screen display
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @copyright  Copyright (c) 2016 by Daniel Eichhorn
 * @copyright  Copyright (c) 2016 by Fabrice Weinberg
 * @licence  The MIT License (MIT)
 * @author  [wxzed](xiao.wu@dfrobot.com)
 * @maintainer  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-04-25
 * @url  https://github.com/DFRobot/DFRobot_OLED12864
 * @note  Credits for parts of this code go to Mike Rankin. Thank you so much for sharing!
 */
#ifndef OLEDDISPLAY_h
#define OLEDDISPLAY_h

#include <Arduino.h>
#include "OLEDDisplayFonts.h"

// #define DEBUG_OLEDDISPLAY(...) Serial.printf( __VA_ARGS__ )

#ifndef DEBUG_OLEDDISPLAY
#define DEBUG_OLEDDISPLAY(...)
#endif

//!< Use DOUBLE BUFFERING by default
#ifndef OLEDDISPLAY_REDUCE_MEMORY
#define OLEDDISPLAY_DOUBLE_BUFFER
#endif


//!< Display settings
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_BUFFER_SIZE 1024

//!< Header Values
#define JUMPTABLE_BYTES 4

#define JUMPTABLE_LSB   1
#define JUMPTABLE_SIZE  2
#define JUMPTABLE_WIDTH 3
#define JUMPTABLE_START 4

#define WIDTH_POS 0
#define HEIGHT_POS 1
#define FIRST_CHAR_POS 2
#define CHAR_NUM_POS 3


//!< Display commands
#define CHARGEPUMP 0x8D
#define COLUMNADDR 0x21
#define COMSCANDEC 0xC8
#define COMSCANINC 0xC0
#define DISPLAYALLON 0xA5
#define DISPLAYALLON_RESUME 0xA4
#define DISPLAYOFF 0xAE
#define DISPLAYON 0xAF
#define EXTERNALVCC 0x1
#define INVERTDISPLAY 0xA7
#define MEMORYMODE 0x20
#define NORMALDISPLAY 0xA6
#define PAGEADDR 0x22
#define SEGREMAP 0xA0
#define SETCOMPINS 0xDA
#define SETCONTRAST 0x81
#define SETDISPLAYCLOCKDIV 0xD5
#define SETDISPLAYOFFSET 0xD3
#define SETHIGHCOLUMN 0x10
#define SETLOWCOLUMN 0x00
#define SETMULTIPLEX 0xA8
#define SETPRECHARGE 0xD9
#define SETSEGMENTREMAP 0xA1
#define SETSTARTLINE 0x40
#define SETVCOMDETECT 0xDB
#define SWITCHCAPVCC 0x2

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

/**
 * @enum OLEDDISPLAY_COLOR
 * @brief OLED Screen Colors
 */
enum OLEDDISPLAY_COLOR
{
  BLACK = 0,
  WHITE = 1,
  INVERSE = 2
};

/**
 * @enum OLEDDISPLAY_TEXT_ALIGNMENT
 * @brief Oled display text alignment
 */
enum OLEDDISPLAY_TEXT_ALIGNMENT
{
  TEXT_ALIGN_LEFT = 0,
  TEXT_ALIGN_RIGHT = 1,
  TEXT_ALIGN_CENTER = 2,
  TEXT_ALIGN_CENTER_BOTH = 3
};


class OLEDDisplay: public Print 
{
public:
    /**
     * @fn init
     * @brief OLED init
     * @return true or false
     */
    bool init();

    /**
     * @fn end
     * @brief free the memory used by the display
     * @return None
     */
    void end();

    /**
     * @fn resetDisplay
     * @brief Cycle through the initialization
     * @return None
     */
    void resetDisplay(void);

/******************* Drawing functions ******************/

    /**
     * @fn setColor
     * @brief Sets the color of all pixel operations
     * @param color - WHITE, BLACK, INVERSE
     * @return None
     */
    void setColor(OLEDDISPLAY_COLOR color);

    /**
     * @fn setPixel
     * @brief Draw a pixel at given position
     * @param x - x coordinate
     * @param y - y coordinate
     * @return None
     */
    void setPixel(int16_t x, int16_t y);

    /**
     * @fn clearPixel
     * @brief Clear a pixel at given position
     * @param x - x coordinate
     * @param y - y coordinate
     * @return None
     */
    void clearPixel(int16_t x, int16_t y);

    /**
     * @fn drawLine
     * @brief Draw a line from position 0 to position 1
     * @param x0 - position 0 x coordinate
     * @param y0 - position 0 y coordinate
     * @param x1 - position 1 x coordinate
     * @param y1 - position 1 y coordinate
     * @return None
     */
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

    /**
     * @fn drawRect
     * @brief Draw the border of a rectangle at the given location
     * @param x - Starting point x coordinate
     * @param y - Starting point y coordinate
     * @param width - rectangle width
     * @param height - rectangle height
     * @return None
     */
    void drawRect(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * @fn fillRect
     * @brief Fill the rectangle
     * @param x - Starting point x coordinate
     * @param y - Starting point y coordinate
     * @param width - rectangle width
     * @param height - rectangle height
     * @return None
     */
    void fillRect(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * @fn drawCircle
     * @brief Draw the border of a circle
     * @param x - Center point x coordinate
     * @param y - Center point y coordinate
     * @param radius - radius of a circle
     * @return None
     */
    void drawCircle(int16_t x, int16_t y, int16_t radius);

    /**
     * @fn drawCircleQuads
     * @brief Draw all Quadrants specified in the quads bit mask
     * @param x0 - Starting point x coordinate
     * @param y0 - Starting point y coordinate
     * @param radius - radius
     * @param quads - quads
     * @return None
     */
    void drawCircleQuads(int16_t x0, int16_t y0, int16_t radius, uint8_t quads);

    /**
     * @fn fillCircle
     * @brief Fill circle
     * @param x - Center point x coordinate
     * @param y - Center point y coordinate
     * @param radius - radius of a circle
     * @return None
     */
    void fillCircle(int16_t x, int16_t y, int16_t radius);

    /**
     * @fn drawHorizontalLine
     * @brief Draw a line horizontally
     * @param x - Center point x coordinate
     * @param y - Center point y coordinate
     * @param length - length
     * @return None
     */
    void drawHorizontalLine(int16_t x, int16_t y, int16_t length);

    /**
     * @fn drawVerticalLine
     * @brief Draw a lin vertically
     * @param x - Center point x coordinate
     * @param y - Center point y coordinate
     * @param length - length
     * @return None
     */
    void drawVerticalLine(int16_t x, int16_t y, int16_t length);

    /**
     * @fn drawProgressBar
     * @brief Draws a rounded progress bar with the outer dimensions given by width and height. Progress is 
     * @n a unsigned byte value between 0 and 100
     * @param x - Starting point x coordinate
     * @param y - Starting point y coordinate
     * @param width - width
     * @param height - height
     * @param progress - progress
     * @return None
     */
    void drawProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t progress);

    /**
     * @fn drawFastImage
     * @brief Draw a bitmap in the internal image format
     * @param x - Starting point x coordinate
     * @param y - Starting point y coordinate
     * @param width - width
     * @param height - height
     * @param image - image
     * @return None
     */
    void drawFastImage(int16_t x, int16_t y, int16_t width, int16_t height, const char *image);

    /**
     * @fn drawXbm
     * @brief Draw a XBM
     * @param x - Starting point x coordinate
     * @param y - Starting point y coordinate
     * @param width - width
     * @param height - height
     * @param xbm - xbm
     * @return None
     */
    void drawXbm(int16_t x, int16_t y, int16_t width, int16_t height, const char *xbm);

/******************* Text functions ******************/

    /**
     * @fn drawString
     * @brief Draws a string at the given location
     * @param x - Starting point x coordinate
     * @param y - Starting point y coordinate
     * @param text - text
     * @return None
     */
    // 
    void drawString(int16_t x, int16_t y, String text);

    /**
     * @fn drawStringMaxWidth
     * @brief Draws a String with a maximum width at the given location.
     * @n If the given String is wider than the specified width
     * @n The text will be wrapped to the next line at a space or dash
     * @param x - Starting point x coordinate
     * @param y - Starting point y coordinate
     * @param maxLineWidth - max Line Width
     * @param text - text
     * @return None
     */
    void drawStringMaxWidth(int16_t x, int16_t y, uint16_t maxLineWidth, String text);

    /**
     * @fn getStringWidth
     * @brief get String Width
     * @param text - text
     * @param length - length
     * @return Returns the width of the const char* with the current font settings
     * @note Convencience method for the const char version
     */
    uint16_t getStringWidth(const char* text, uint16_t length);
    uint16_t getStringWidth(String text);

    /**
     * @fn setTextAlignment
     * @brief Specifies relative to which anchor point the text is rendered.
     * @param textAlignment - Available constants:
     * @n TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT, TEXT_ALIGN_CENTER_BOTH
     * @return None
     */
    void setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT textAlignment);

    /**
     * @fn setFont
     * @brief Sets the current font. 
     * @param fontData - Available default fonts
     * @n ArialMT_Plain_10, ArialMT_Plain_16, ArialMT_Plain_24
     * @return None
     */
    void setFont(const char *fontData);

/******************* Display functions ******************/

    /**
     * @fn displayOn
     * @brief Turn the display on
     * @return None
     */
    void displayOn(void);

    /**
     * @fn displayOff
     * @brief Turn the display offs
     * @return None
     */
    void displayOff(void);

    /**
     * @fn invertDisplay
     * @brief Inverted display mode
     * @return None
     */
    void invertDisplay(void);

    /**
     * @fn normalDisplay
     * @brief Normal display mode
     * @return None
     */
    void normalDisplay(void);

    /**
     * @fn setContrast
     * @brief Set display contrast
     * @param contrast - contrast
     * @return None
     */
    void setContrast(char contrast);

    /**
     * @fn flipScreenVertically
     * @brief Turn the display upside down
     * @return None
     */
    void flipScreenVertically(void);

    /**
     * @fn display
     * @brief Write the buffer to the display memory
     * @return None
     */
    virtual void display(void) = 0;

    /**
     * @fn clear
     * @brief Clear the local pixel buffer
     * @return None
     */
    void clear(void);

    /**
     * @fn setLogBuffer
     * @brief Log buffer implementation
     * @n This will define the lines and characters you can
     * @n print to the screen. When you exeed the buffer size (lines * chars)
     * @n the output may be truncated due to the size constraint.
     * @param lines - lines
     * @param chars - chars
     * @return None
     */
    bool setLogBuffer(uint16_t lines, uint16_t chars);

    /**
     * @fn drawLogBuffer
     * @brief Draw the log buffer at position (x, y)
     * @param x - Starting point x coordinate
     * @param y - Starting point y coordinate
     * @return None
     */
    void drawLogBuffer(uint16_t x, uint16_t y);

    /**
     * @fn drawLogBuffer
     * @brief Implementent needed function to be compatible with Print class
     * @param c - char
     * @param s - string
     * @return 1
     */
    size_t write(uint8_t c);
    size_t write(const char* s);

    uint8_t cs;
    uint8_t            *buffer;

    #ifdef OLEDDISPLAY_DOUBLE_BUFFER
    uint8_t            *buffer_back;
    #endif

  protected:

    OLEDDISPLAY_TEXT_ALIGNMENT   textAlignment = TEXT_ALIGN_LEFT;
    OLEDDISPLAY_COLOR            color         = WHITE;

    const char          *fontData              = ArialMT_Plain_10;

    /* State values for logBuffer */
    uint16_t   logBufferSize                   = 0;
    uint16_t   logBufferFilled                 = 0;
    uint16_t   logBufferLine                   = 0;
    uint16_t   logBufferMaxLines               = 0;
    char      *logBuffer                       = NULL;

    /**
     * @fn sendCommand
     * @brief Send a command to the display (low level function)
     * @param com - Command
     * @return None
     */
    virtual void sendCommand(uint8_t com) {};

    /**
     * @fn connect
     * @brief Connect to the display
     * @return None
     */
    virtual bool connect() {};

    /**
     * @fn sendInitCommands
     * @brief Send all the init commands
     * @return None
     */
    void sendInitCommands();

    /**
     * @fn utf8ascii
     * @brief converts utf8 characters to extended ascii
     * @param s - string
     * @param ascii - byte
     * @return None
     */
    static char* utf8ascii(String s);
    static byte utf8ascii(byte ascii);

    /**
     * @fn drawInternal
     * @brief draw Internal
     * @return None
     */
    void inline drawInternal(int16_t xMove, int16_t yMove, int16_t width, int16_t height, const char *data, uint16_t offset, uint16_t bytesInData) __attribute__((always_inline));

    /**
     * @fn drawStringInternal
     * @brief draw String Internal
     * @return None
     */
    void drawStringInternal(int16_t xMove, int16_t yMove, char* text, uint16_t textLength, uint16_t textWidth);

};

#endif
