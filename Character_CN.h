#ifndef _CHARACTER_CN_H
#define _CHARACTER_CN_H

#include "SPI.h"
#include "Wire.h"
#include "Arduino.h"

#define CHARACTER_TYPE_UNICODE 0
#define CHARACTER_TYPE_UTF8    1

#define ASC0808D2HZ_ADDR ( 0x66c0  )
#define ASC0812M2ZF_ADDR ( 0x66d40 )
#define GBEX0816ZF_ADDR   0x27BFAA  //8*16英文字模
#define ZF1112B2ZF_ADDR  ( 0x3cf80 )
#define HZ1112B2HZ_ADDR  ( 0x3cf80+376*24 )
#define CUTS1516ZF_ADDR  0x00
#define JFLS1516HZ_ADDR  0x21E72C  //16*16中文字模

#define ASCII0507ZF_ADDR        245696
#define ARIAL_16B0_ADDR         246464
#define ARIAL_12B0_ADDR         422720
#define SPAC1616_ADDR           425264
#define GB2312TOUNICODE_ADDR    (12032)
#define UNICODETOGB2312_ADDR    (0x267B06)  //267B06  425328

#define   CHARACTER_TYPE_8    0
#define   CHARACTER_TYPE_12   1
#define   CHARACTER_TYPE_16   2  //16位字符高度

class Character_CN {
	public:
		uint8_t cs;
		uint8_t character_type = CHARACTER_TYPE_UTF8;
		

		uint32_t GB2312_addr(char *ch, uint8_t type);
    void readByte(uint32_t addr, uint8_t len, char *ch);
		char unicodeToGB2312(char *unicode, char *GB2312);
		void byteOverturn(char *ch);
};

#endif

