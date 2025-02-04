#include <REGX52.H>

#define TIMER_INIT 64535

void timer0init(){
    TMOD &= 0xF0;
    TMOD |= 0x01;

    // TCON : TF: TimerFlag  TR: TimerReady  IE: InterruptEnable  IT:不用
    // TF1  TR1  TF0  TR0  IE1  IT1  IE0  IT0
    //  0    0    0    1    0    0    0    0
    // TCON = 0x10; 

    // TMOD:  
    // Gate  C^T  M1  M0  Gate  C^T  M1  M0
    //  0     0    0   0    0    0    0   1

    // TimerReady0 = 1
    TR0 = 1;

    // TimerFlag0 = 0
    TF0 = 0;

    EA = 1;
    ET0 = 1;

    PT0 = 0;

    // TH0 = TIMER_INIT >> 8;
    // TL0 = TIMER_INIT % 256;

    TL0 = 0xA4;		//设置定时初值
    TH0 = 0xFF;		//设置定时初值


}

/*
void timer0intp() interrupt 1 {
    static unsigned int timerCounter = 0;

    TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值

    if(timerCounter++ == 1000) {
        // ...

        timerCounter = 0;
    }
}
*/