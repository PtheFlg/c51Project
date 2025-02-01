#include <REGX52.H>
#include "SingleKey.h"
#include "Nixie.h"
#include "Delay.h"
#include "Timer.h"
#include "AT24C02.h"


#define BYTE unsigned char

BYTE minutes = 0;
BYTE seconds = 0;
BYTE miniSec = 0;

BYTE keyNum = 0;
BYTE lastKeyNum = 0;

BYTE runFlag = 0;

void main() {

    timer0init();

    while(1) {
        // keyNum = getSingleKey();
        // if(keyNum) {
        //     lastKeyNum = keyNum;
        // }

        // Nixie_showNum(7, lastKeyNum);
        // Nixie_showNum(6, lastKeyNum);
        // Nixie_showNum(5, lastKeyNum);
        // // Nixie_setBuf(7, 1);
        // // Nixie_setBuf(6, 2);
        // // Nixie_setBuf(5, 3);

        keyNum = getSingleKey();

        if(keyNum == 1) {
            runFlag = !runFlag;
        }

        if(keyNum == 2) {
            minutes = 0;
            seconds = 0;
            miniSec = 0;
        }

        if(keyNum == 3) {
            AT24C02_wirteByte(0, miniSec);
            delay(5);
            AT24C02_wirteByte(1, seconds);
            delay(5);
            AT24C02_wirteByte(2, minutes);
            delay(5);
        }

        if(keyNum == 4) {
            miniSec = AT24C02_readByte(0);
            seconds = AT24C02_readByte(1);
            minutes = AT24C02_readByte(2);
        }

        Nixie_setBuf(7, minutes / 10);
        Nixie_setBuf(6, minutes % 10);

        Nixie_setBuf(5, 17);

        Nixie_setBuf(4, seconds / 10);
        Nixie_setBuf(3, seconds % 10);

        Nixie_setBuf(2, 17);

        Nixie_setBuf(1, miniSec / 10);
        Nixie_setBuf(0, miniSec % 10);
    }
}

void secLoop() {
    if(!runFlag)
        return;
    miniSec ++;
    if(miniSec == 100) {
        miniSec = 0;
        seconds++;

        if(seconds == 60) {
            seconds = 0;
            minutes ++;

            if(minutes == 60) {
                minutes = 0;
            }
        }
    }
}


void timer0intp() interrupt 1 {
    static unsigned int timerCounter1 = 0, timerCounter2 = 0, timerCounter3 = 0;

    // TL0 = 0x66;		//设置定时初值
    TL0 = 0x18;
	TH0 = 0xFC;		//设置定时初值

    if(timerCounter1++ == 20) {
        timerCounter1 = 0;
        singleKeyLoop();
    }

    if(timerCounter2++ == 1) {
        timerCounter2 = 0;
        Nixie_loop();
    }

    // 每隔 10ms 执行一下这个 secLoop 函数，
    timerCounter3++;
    if(timerCounter3 == 10) {
        secLoop();
        timerCounter3 = 0;
    }

}

