#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"

#define BYTE unsigned char


void delay(unsigned int xms)		//@12.000MHz
{
	BYTE i, j;

	do{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} 	while (--i);
	}while( --xms);
}

void main() {

    BYTE readRec = 0x00;

    LCD_Init();
    DS1302_init();
    // DS1302_writeByte(0x8E, 0x00);


    // DS1302_writeByte(0x80, 0x03);

    // LCD_ShowString(1, 1, "  -  -  ");
    // LCD_ShowString(2, 1, "  :  :  ");

    DS1302_setTime();

    while(1) {

        // LCD_ShowNum(1, 1, ds_time[0], 2);

        DS1302_readTime();

        // readRec = DS1302_readByte(0x80);
        // readRec = readRec / 16 * 10 + readRec % 16;

        LCD_ShowNum(1, 1, readRec, 2);
        LCD_ShowNum(1, 4, ds_time[1], 2);
        LCD_ShowNum(1, 7, ds_time[2], 2);

        LCD_ShowNum(2, 1, ds_time[3], 2);
        LCD_ShowNum(2, 4, ds_time[4], 2);
        LCD_ShowNum(2, 7, ds_time[5], 2);

        // readRec = DS1302_readByte(0x82);
        // LCD_ShowBinNum(1, 1, readRec, 8);

        // // readRec = readRec / 16 * 10 + readRec % 16;
        // readRec = (readRec >> 4) * 10 + readRec % 16;
        // LCD_ShowNum(1, 10, readRec, 2);

        

        // readRec = DS1302_readByte(0x8C);
        // LCD_ShowNum(1, 1, readRec, 2);

    }
}