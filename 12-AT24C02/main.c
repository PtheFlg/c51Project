#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include "SingleKey.h"
#include "AT24C02.h"

#define BYTE unsigned char

unsigned int num = 0;
BYTE keyNum = 0;

void main() {

    BYTE recByte;

    LCD_Init();

    
    // LCD_ShowString(1, 1, "Hello;");

    // AT24C02_wirteByte(1, 0xAA);
    // delay(5);
    // AT24C02_wirteByte(2, 0x77);
    // delay(5);

    // recByte = AT24C02_readByte(1);
    // LCD_ShowHexNum(1, 10, recByte, 2);
    while(1) {

        keyNum = singleKey();

        LCD_ShowNum(1, 1, num, 5);

        if(keyNum == 1) {
            num++;
        }
        if(keyNum == 2) {
            num--;
        }

        if(keyNum == 3) {
            AT24C02_wirteByte(0, num % 256);
            delay(5);

            AT24C02_wirteByte(1, num >> 8);
            delay(5);

            LCD_ShowString(2, 1, "Store OK");
            delay(1000);
            LCD_ShowString(2, 1, "        ");
        }

        if(keyNum == 4) {
            num = AT24C02_readByte(0);
            num |= AT24C02_readByte(1) << 8;

            LCD_ShowString(2, 1, "Read OK");
            delay(1000);
            LCD_ShowString(2, 1, "       ");
        }
        
    }
}