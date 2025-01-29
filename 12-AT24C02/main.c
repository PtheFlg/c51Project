#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "SingleKey.h"
#include "AT24C02.h"

#define BYTE unsigned char

void main() {

    BYTE recByte;

    LCD_Init();

    
    LCD_ShowString(1, 1, "Hello;");

    // AT24C02_wirteByte(1, 0xAA);
    // delay(5);
    // AT24C02_wirteByte(2, 0x77);
    // delay(5);

    recByte = AT24C02_readByte(1);
    LCD_ShowHexNum(1, 10, recByte, 2);
    while(1) {
    }
}