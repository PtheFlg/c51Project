#include <REGX52.H>
#include "SingleKey.h"
#include "Nixie.h"
#include "Delay.h"
#include "Timer.h"


#define BYTE unsigned char

BYTE keyNum = 0;
BYTE lastKeyNum = 0;

void main() {

    timer0init();

    while(1) {
        keyNum = getSingleKey();
        if(keyNum) {
            lastKeyNum = keyNum;
        }

        Nixie_showNum(7, lastKeyNum);
        Nixie_showNum(6, lastKeyNum);
        Nixie_showNum(5, lastKeyNum);
        // Nixie_setBuf(7, 1);
        // Nixie_setBuf(6, 2);
        // Nixie_setBuf(5, 3);
    }
}


void timer0intp() interrupt 1 {
    static unsigned int timerCounter1 = 0, timerCounter2 = 0;

    TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值

    if(timerCounter1++ == 20) {
        timerCounter1 = 0;
        singleKeyLoop();
    }

    if(timerCounter2++ == 2) {
        timerCounter2 = 0;
        Nixie_loop();
    }

}

