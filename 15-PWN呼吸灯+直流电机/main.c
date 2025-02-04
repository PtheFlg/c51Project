#include <REGX52.H>
#include "Timer.h"
#include "SingleKey.h"
#include "Nixie.h"

#define BYTE unsigned char

sbit LED0 = P2^0;
sbit MOTOR = P1^0;

BYTE COUNTER;
BYTE COMPARE;
// void delay(BYTE t) {
//     while(t--);
// }

BYTE key;
BYTE SPEED;

void main() {

    timer0init();
    COMPARE = 70;

    /**
     * 呼吸灯
     */
    // BYTE i, j;

    // while(1) {

    //     for(i = 0; i < 100; i++) {
    //         for(j = 0; j < 20; j++) {
    //             LED0 = 0;
    //             delay(i);

    //             LED0 = 1;
    //             delay(100 - i);
    //         }
    //     }

    //     for(i = 100; i > 0; i--) {
    //         for(j = 0; j < 20; j++) {
    //             LED0 = 0;
    //             delay(i);

    //             LED0 = 1;
    //             delay(100 - i);
    //         }
    //     }

    // }

    while(1) {
        key = singleKey();

        if(key == 1) {
            SPEED++;
            SPEED %= 4;
        }

        if(SPEED == 0) COMPARE = 0;
        else if(SPEED == 1) COMPARE = 30;
        else if(SPEED == 2) COMPARE = 70;
        else if(SPEED == 3) COMPARE = 100; 

        Nixie_showNum(7, SPEED);
        
    }
		
	
}

/**
 * 100us 进入一次中断
 */
void timer0routine() interrupt 1 {
    TH0 = 0xFF;
    TL0 = 0xA4;

    COUNTER++;
    COUNTER = COUNTER % 100;

    // 这种写法不行，又不知道为啥
    // COUNTER = (COUNTER++) % 100;
    if(COUNTER > COMPARE) {
        MOTOR = 0;
    } else {
        MOTOR = 1;
    }
}