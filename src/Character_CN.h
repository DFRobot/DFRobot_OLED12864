/*!
 * @file  Character_CN.h
 * @brief  Implementation of Chinese coding features of the file
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence  The MIT License (MIT)
 * @author  [wxzed](xiao.wu@dfrobot.com)
 * @maintainer  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-04-25
 * @url  https://github.com/DFRobot/DFRobot_OLED12864
 */
#ifndef _CHARACTER_CN_H
#define _CHARACTER_CN_H

#include "SPI.h"
#include "Wire.h"
#include "Arduino.h"
#include "table_punctuation_CN.h"


#define CHARACTER_TYPE_UNICODE 0
#define CHARACTER_TYPE_UTF8    1

#define ADDR_CCP         ((uint32_t)0x22242c)   //!< character chinese punctuation

#define ASC0808D2HZ_ADDR ( 0x66c0  )
#define ASC0812M2ZF_ADDR ( 0x66d40 )
#define GBEX0816ZF_ADDR   0x27BFAA   //!< 8*16 ASCII character
#define ZF1112B2ZF_ADDR  ( 0x3cf80 )
#define HZ1112B2HZ_ADDR  ( 0x3cf80+376*24 )
#define CUTS1516ZF_ADDR  0x00
#define JFLS1516HZ_ADDR  0x21E72C   //!< 16*16 chinese character

#define ASCII0507ZF_ADDR        245696
#define ARIAL_16B0_ADDR         246464
#define ARIAL_12B0_ADDR         422720
#define SPAC1616_ADDR           425264
#define GB2312TOUNICODE_ADDR    (12032)
#define UNICODETOGB2312_ADDR    (0x267B06)   //!< 267B06  425328

#define   CHARACTER_TYPE_8    0
#define   CHARACTER_TYPE_12   1
#define   CHARACTER_TYPE_16   2   //!< 16 bits character height

class Character_CN {

public:
  /**
   * @fn GB2312_addr
   * @brief GB2312 ascii code 
   * @param ch - 字符
   * @param type - 编码类型
   * @return 转换后的值
   */
  uint32_t  GB2312_addr(char *ch, uint8_t type);

  /**
   * @fn readByte
   * @brief 读字节
   * @param addr - 字符对应的地址
   * @param len - 长度
   * @param ch - 字符
   * @return None
   */
  void  readByte(uint32_t addr, uint8_t len, char *ch);

  /**
   * @fn unicodeToGB2312
   * @brief unicode to GB2312
   * @param unicode - unicode编码的字符
   * @param GB2312 - GB2312编码的字符
   * @return 0
   */
  char  unicodeToGB2312(char *unicode, char *GB2312);

  /**
   * @fn byteOverturn
   * @brief 字节翻转
   * @param ch - 需翻转的字节
   * @return None
   */
  void  byteOverturn(char *ch);

  uint8_t  cs;
  uint8_t  character_type = CHARACTER_TYPE_UTF8;
};

#endif

