#include "LCD1602.h"
#include "XPT2046.h"
#include "Delay.h"
#include <REGX52.H>

unsigned int adValue = 0;

void main() {

    LCD_Init();
    LCD_ShowString(1, 1, "ADJ NTC RJ");

    while(1) {
        adValue = XPT2046_readAD(XPT2046_XP_8);
        LCD_ShowNum(2, 1, adValue, 3);

        adValue = XPT2046_readAD(XPT2046_YP_8);
        LCD_ShowNum(2, 5, adValue, 3);

        adValue = XPT2046_readAD(XPT2046_VBAT_8);
        LCD_ShowNum(2, 9, adValue, 3);
        delay(10);
    }
}