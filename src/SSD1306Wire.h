/*!
 * @file  SSD1306Wire.h
 * @brief  SSD1306 Wire
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
#ifndef SSD1306Wire_h
#define SSD1306Wire_h

#include "OLEDDisplay.h"
#include "Character_CN.h"
#include <Wire.h>

class SSD1306Wire : public OLEDDisplay, public Character_CN {

private:
        uint8_t    _address;

public:
    SSD1306Wire(uint8_t _address, uint8_t cs = 0) {
        this->_address = _address;
        Character_CN::cs = cs;
        pinMode(cs, OUTPUT);
        digitalWrite(cs, 1);
        SPI.begin();
    }

    bool connect() {
        Wire.begin();
        /*
         * Let's use ~700khz if ESP8266 is in 160Mhz mode
         * this will be limited to ~400khz if the ESP8266 in 80Mhz mode.
         */
        Wire.setClock(700000);
        return true;
    }

    void display(void) {
        #ifdef OLEDDISPLAY_DOUBLE_BUFFER 
            uint8_t minBoundY = ~0;
            uint8_t maxBoundY = 0;

            uint8_t minBoundX = ~0;
            uint8_t maxBoundX = 0;
            uint8_t x, y;

            /*
             * Calculate the Y bounding box of changes
             * and copy buffer[pos] to buffer_back[pos];
             */
            Wire.begin();
            for (y = 0; y < (DISPLAY_HEIGHT / 8); y++) {
                for (x = 0; x < DISPLAY_WIDTH; x++) {
                 uint16_t pos = x + y * DISPLAY_WIDTH;
                 if (buffer[pos] != buffer_back[pos]) {
                     minBoundY = _min(minBoundY, y);
                     maxBoundY = _max(maxBoundY, y);
                     minBoundX = _min(minBoundX, x);
                     maxBoundX = _max(maxBoundX, x);
                 }
                 buffer_back[pos] = buffer[pos];
             }
             yield();
            }

            /*
             * If the minBoundY wasn't updated
             * we can savely assume that buffer_back[pos] == buffer[pos]
             * holdes true for all values of pos
             */
            if (minBoundY == ~0) return;

            sendCommand(COLUMNADDR);
            sendCommand(minBoundX);
            sendCommand(maxBoundX);

            sendCommand(PAGEADDR);
            sendCommand(minBoundY);
            sendCommand(maxBoundY);

            byte k = 0;
            for (y = minBoundY; y <= maxBoundY; y++) {
                for (x = minBoundX; x <= maxBoundX; x++) {
                    if (k == 0) {
                        Wire.beginTransmission(_address);
                        Wire.write(0x40);
                    }
                    Wire.write(buffer[x + y * DISPLAY_WIDTH]);
                    k++;
                    if (k == 16)  {
                        Wire.endTransmission();
                        k = 0;
                    }
                }
                yield();
            }

            if (k != 0) {
                Wire.endTransmission();
            }
        #else

            sendCommand(COLUMNADDR);
            sendCommand(0x0);
            sendCommand(0x7);

            sendCommand(PAGEADDR);
            sendCommand(0x0);
            sendCommand(0x7f);

            for (uint16_t i=0; i < DISPLAY_BUFFER_SIZE; i++) {
                Wire.beginTransmission(this->_address);
                Wire.write(0x40);
                for (uint8_t x = 0; x < 16; x++) {
                    Wire.write(buffer[i]);
                    i++;
                }
                i--;
                Wire.endTransmission();
            }
        #endif
    }
    
    void disStr(uint8_t x, uint8_t y, char *ch) 
    {    
        char unicode_hz[2] = {0};
        char gb2312_hz [2] = {0};
        char temp_char[2] = {0};
        int  temp = 0;
        if(Character_CN::character_type == CHARACTER_TYPE_UTF8) {
            while(*ch) {
                if(x >= (128-15)) {
                    y += 16;
                    x  = 0;
                }
                if(y >= (64-15)) {
                    return;
                }
                if((*ch & 0xf0) == 0xe0) {
                    temp = (*ch & 0x0f) << 12 | (*(ch + 1) & 0x3f) << 6 | (*(ch + 2) & 0x3f);
                    unicode_hz[0] = temp >> 8;
                    unicode_hz[1] = temp;
                    Serial.print(" ");
                    Serial.print(unicode_hz[0], HEX);
                    Serial.print(unicode_hz[1], HEX);
                    Character_CN::unicodeToGB2312(unicode_hz, gb2312_hz);
                    this->disCharacter(x, y, gb2312_hz);
                    x  += 16;
                    ch += 3;
                } else if((*ch & 0x80) == 0x80) {
                    gb2312_hz[0] = *ch;
                    gb2312_hz[1] = *(ch + 1);
                    this->disCharacter(x, y, gb2312_hz);
                    x += 16;
                    ch += 2;
                } else {
                    temp_char[0] = *ch;
                    this->disChar(x, y, temp_char);
                    x += 8;
                    ch ++;
                }
            }
        } else if(Character_CN::character_type == CHARACTER_TYPE_UNICODE) {
            Serial.println(strlen(ch));
        }
    }

private:
    inline void sendCommand(uint8_t command) __attribute__((always_inline))
    {
        Wire.beginTransmission(_address);
        Wire.write(0x80);
        Wire.write(command);
        Wire.endTransmission();
    }

    void disChar(uint8_t x, uint8_t y, char *ch)
    {    
        char a_dot[16];
        uint32_t a_dot_address;
        uint8_t i = 0;
        a_dot_address = Character_CN::GB2312_addr(ch, CHARACTER_TYPE_16);
        Character_CN::readByte(a_dot_address, 16, a_dot);
        for(;i < 16;i ++) {
            Character_CN::byteOverturn(&a_dot[i]);
        }
        a_dot[15] = 0;
        a_dot[14] = 0;
        OLEDDisplay::drawXbm(x, y, 8, 16, a_dot);
    }
    
    void disCharacter(uint8_t x, uint8_t y, char *ch)
    {    
        char a_dot[32];
        uint32_t a_dot_address;
        int i,j;
        a_dot_address = Character_CN::GB2312_addr(ch, CHARACTER_TYPE_16);
        Character_CN::readByte(a_dot_address, 32, a_dot);
        for(i = 0;i < 32;i ++) {
            Character_CN::byteOverturn(&a_dot[i]);
        }
        OLEDDisplay::drawXbm(x, y, 16, 16, a_dot);
    }
};

#endif
