#include <REGX52.H>
#include "Timer.h"
#include "LCD1602.h"

#define uchar unsigned char

unsigned char ledDir = 0;

unsigned char second = 0;
unsigned char minuate = 0;

void delay(unsigned int xms)		//@12.000MHz
{
	uchar i, j;

	do{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} 	while (--i);
	}while( --xms);
}

void ledRight(){
    if(P2 == 0x7F) {
        P2 = 0xFE;
    } else {
        P2 = (P2 << 1) + 1;
    }
}

void ledLeft() {
    if(P2 == 0xFE) {
        P2 = 0x7F;
    } else {
        P2 = (P2 >> 1) + 0x80;
    }   
}



void timer0intp() interrupt 1 {
    static unsigned int timerCounter = 0;

    TL0 = 0x18;		//设置定时初值
    TH0 = 0xFC;		//设置定时初值

    if(timerCounter++ == 250) {
        // if(ledDir) {
        //     ledRight();
        // } else {
        //     ledLeft();
        // }

        timerCounter = 0;

        second++;
    }
}

void main() {

    // P2 = 0xFE;

    timer0init();

    LCD_Init();
    LCD_ShowString(1, 1, "Time:");

    while(1) {
        if(second == 60) {
            minuate++;
            second = 0;
        }

        if(minuate == 60) {
            minuate = 0;
        }

        LCD_ShowNum(2, 1, minuate, 2);
        LCD_ShowChar(2, 3, ':');
        LCD_ShowNum(2, 4, second, 2);

        // if(!P3_0) {
        //     delay(5);
        //     while(!P3_0);
        //     delay(5);

        //     ledDir = 0;
        // }

        // if(!P3_2) {
        //     delay(5);
        //     while(!P3_2);
        //     delay(5);

        //     ledDir = 1;
        // }
    }
}