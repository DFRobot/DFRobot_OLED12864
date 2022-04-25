/*!
 * @file  Character_CN.cpp
 * @brief  Implementation of Chinese coding features of the file
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence  The MIT License (MIT)
 * @author  [wxzed](xiao.wu@dfrobot.com)
 * @maintainer  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-04-25
 * @url  https://github.com/DFRobot/DFRobot_OLED12864
 */
#include "Character_CN.h"

void Character_CN::byteOverturn(char *ch)
{
  char temp = *ch;
  *ch = 0;
  uint8_t i;
  for(i = 0;i < 8;i ++) {
    if(temp & (0x80 >> i)) {
      *ch |= 0x01 << i;
    }
  }
}

uint32_t Character_CN::GB2312_addr(char *ch, uint8_t type)
{
  uint32_t temp = 0;
  if((ch[0] & 0x80) != 0x80) {
    if( ch[0] >= ' ' )
    temp = ch[0] - ' ';
    if( type == CHARACTER_TYPE_8 )        temp = temp*8  + ASC0808D2HZ_ADDR;      //7*8 ascii code
    else if( type == CHARACTER_TYPE_12 )  temp = temp*12 + ASC0812M2ZF_ADDR;      //6*12 ascii code
    else if( type == CHARACTER_TYPE_16 )  temp = temp*16 + GBEX0816ZF_ADDR;     //8*16 ascii code
  } else {
    if(ch[0] >=0xA4 && ch[0] <= 0Xa8 && ch[1] >=0xA1)
      temp = JFLS1516HZ_ADDR;
    else if(ch[0] >=0xA1 && ch[0] <= 0Xa9 && ch[1] >=0xA1)
      temp =( (ch[0] - 0xA1) * 94 + (ch[1] - 0xA1))*32+ ADDR_CCP;
    else if(ch[0] >=0xB0 && ch[0] <= 0xF7 && ch[1] >=0xA1)
      temp = ((ch[0] - 0xB0) * 94 + (ch[1] - 0xA1)+ 846)*32+ JFLS1516HZ_ADDR;
  }
  return temp;
}

void Character_CN::readByte(uint32_t addr, uint8_t len, char *ch)
{  
  digitalWrite(this->cs, 0);
  SPI.transfer(0x0b);
  SPI.transfer(addr >> 16);
  SPI.transfer(addr >> 8);
  SPI.transfer(addr);
  SPI.transfer(0x00);
  while(len --) {
    *ch = SPI.transfer(0x00);
    ch ++;
  }
  digitalWrite(this->cs, 1);
}

char Character_CN::unicodeToGB2312(char *unicode, char *GB2312)
{  
  uint32_t addr;
  uint8_t result=0;
  uint32_t h;
  uint32_t code;
  code = *unicode++;
  code = (code<<8) + *unicode;
  if(code < 0x4e00 || code > 0x9FBF) {
    for(int i = 0 ; i < sizeof(table_punctuation_CN) / 4 ; i ++) {
      if(table_punctuation_CN[i][0] == code) {
        GB2312[0] = table_punctuation_CN[i][1] >> 8;
        GB2312[1] = table_punctuation_CN[i][1];
        return 0;
      }
    }
  }
  else if(code<0xa0) result=1;
  else if(code<=0xf7) h=code-160;
  else if(code<0x2c7) result=1;
  else if(code<=0x2c9) h=code-160-463;
  else if(code<0x2010) result=1;
  else if(code<=0x2312) h=code-160-463-7494;
  else if(code<0x2460) result=1;
  else if(code<=0x2642) h=code-160-463-7494-333;
  else if(code<0x3000) result=1;
  else if(code<=0x3017) h=code-160-463-7494-333-2493;
  else if(code<0x3220) result=1;
  else if(code<=0x3229) h=code-160-463-7494-333-2493-520;
  else if(code<0x4e00) result=1;
  else if(code<=0x9b54) h=code-160-463-7494-333-2493-520-7126;
  else if(code<0x9c7c) result=1;
  else if(code<=0x9ce2) h=code-160-463-7494-333-2493-520-7126-295;
  else if(code<0x9e1f) result=1;
  else if(code<=0x9fa0) h=code-160-463-7494-333-2493-520-7126-295-316;
  else if(code<0xe76c) result=1;
  else if(code<=0xe774) h=code-160-463-7494-333-2493-520-7126-295-316-18379;
  else if(code<0xff00) result=1;
  else if(code<=0xff5f) h=code-160-463-7494-333-2493-520-7126-295-316-18379-6027;
  else if(code<0xffe0) result=1;
  else if(code<=0xffe5) h=code-160-463-7494-333-2493-520-7126-295-316-18379-6027-128;
  else result=1;
  if(result==0){
    addr = UNICODETOGB2312_ADDR + (h<<1);
//    GB2312[0] = this->readByte(addr);
//    GB2312[1] = this->readByte(addr+1);
    this->readByte(addr, 2, GB2312);
  } else {
    GB2312[0] = 0xa1;
    GB2312[1] = 0xa1;
  }
  return 0;
}
