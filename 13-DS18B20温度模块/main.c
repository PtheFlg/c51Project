#include <REGX52.H>
#include "LCD1602.h"
#include "DS18B20.h"
#include "OneWire.h"
#include "Timer.h"
#include "SingleKey.h"


#define BYTE unsigned char

BYTE oneWireAck = 0x00;

float temperature = 0.0;

float tempShow;

BYTE key = 0;

char tlh = 25;
char tll = 20;

void main() {

    LCD_Init();

    timer0init();

    // oneWireAck = OneWire_init();

    LCD_ShowString(1, 1, "TM:");
    LCD_ShowString(2, 1, "H:");
    LCD_ShowString(2, 9, "L:");

    while(1) {
        /**
         * 读取并显示温度部分：
         */
        DS18B20_convertTemperature();

        temperature = DS18B20_readTemperature();

        if(temperature < 0) {
            LCD_ShowChar(1, 4, '-');
            tempShow = -temperature;
        } else {
            LCD_ShowChar(1, 4, '+');
            tempShow = temperature;
        }

        // 先显示整数部分，直接传入，他就会显示整数部分丢弃小数
        LCD_ShowNum(1, 5, tempShow, 3);
        // 显示小数点
        LCD_ShowChar(1, 8, '.');
        // 显示小数，先让这个数 x 10000，相当于把 4 位小数进位了，然后转化成无符号数，取后面 4 位
        // LCD_ShowNum(1, 9, (unsigned long)(temperature * 10000) % 10000, 4);
        LCD_ShowNum(1, 9, (unsigned long)(tempShow * 100) % 100, 2);

        // ============================================================================
        /**
         * 显示温度上下限：
         */
        LCD_ShowSignedNum(2, 3, tlh, 3);
        LCD_ShowSignedNum(2, 11, tll, 3);

        // ============================================================================

        /**
         * 独立按键控制温度阈值
         */
        key = getSingleKey();
        // key = singleKey();
        if(key == 1) {
            if(tlh < 125)   tlh++;
        }
        if(key == 2) {
            if(tlh > tll)   tlh--;
        }
        if(key == 3) {
            if(tll < tlh)   tll++;
        }
        if(key == 4) {
            if(tll > -55)   tll--;
        }

        if(temperature > tlh) {
            LCD_ShowString(1, 13, "OV:H");
        }

        if(temperature < tll) {
            LCD_ShowString(1, 13, "OV:L");
        }

        if(temperature < tlh && temperature > tll) {
            LCD_ShowString(1, 13, "    ");
        }
        
    }
}

/**
 * 如果用中断来扫描独立按键的话，LCD 就会老闪
 * 原因就是单总线的通讯时序都是十几微妙的，如果在通讯时进入了中断，就可能导致和芯片通讯失败，
 * 
 * 可以在芯片的通讯函数中，进行关中断操作。
 */
void timer_routine() interrupt 1 {
    static BYTE singleKeyCounter = 0;
    TH0 = 0xFC;
    TL0 = 0x18;

    if(++singleKeyCounter == 20) {
        singleKeyCounter = 0;
        singleKeyLoop();
    }
}
