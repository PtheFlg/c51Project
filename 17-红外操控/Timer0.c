#include <REGX52.H>

#define uint unsigned int

/**
 * 这个 Timer0 当作计数器来用
 * 
 * 调用 Timer0_setCounter(0), Timer0_run(1) 启动计时器，
 * 然后在一段时间后，调用 Timer0_getCounter() 得到16位计时器
 * 也就是得到了这段时间过了多少微妙
 * 
 * 用于检测外部中断 INT0 两个下降沿中间的时间，来判断红外线发送的是 1 还是 0，start 还是 repeat 还是 end
 */

void Timer0_init() {
    TMOD = TMOD & 0xF0;
    TMOD = TMOD | 0x01;

    // 不需要中断
    TR0 = 0;

    TF0 = 0;
}

void Timer0_setCounter(uint counter) {
    TH0 = counter >> 8;
    TL0 = counter % 0x0100;
}

void Timer0_run(unsigned char run) {
    TR0 = run;
}

uint Timer0_getCounter() {
    return (TH0 << 8) | TL0;
}