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

    LCD_ShowString(1, 1, "  -  -  ");
    LCD_ShowString(2, 1, "  :  :  ");

    DS1302_setTime();

    while(1) {

        DS1302_readTime();

        LCD_ShowNum(1, 1, ds_time.year, 2);
        LCD_ShowNum(1, 4, ds_time.month, 2);
        LCD_ShowNum(1, 7, ds_time.date, 2);

        LCD_ShowNum(2, 1, ds_time.hour, 2);
        LCD_ShowNum(2, 4, ds_time.min, 2);
        LCD_ShowNum(2, 7, ds_time.sec, 2);

    }
}