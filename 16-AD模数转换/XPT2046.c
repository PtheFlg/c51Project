#include <REGX52.H>
#include "Delay.h"

#define BYTE unsigned char

/**
 * XPT 2046 是一个触摸屏适配器，内置了 AD 模块
 * 看原理图的话，里面有 X+，X-，Y+，Y-，就是触摸屏的位置的电阻，可能
 * 但是这里他就把 XPT2046 当作 AD 用了
 * 
 * X-，Y- 直接接了 GND 不用，X+，Y+ 连了 AIN0，AIN1。
 * AIN0 连了一个可变的电阻，可以通过调节电阻阻值改变 XPT2046 的 X+ 口的电压，
 * 然后 XPT2046 根据这个模拟值输出数字值。
 * 
 * 遵循 SPI 总线规范
 */

sbit XPT2046_DIN = P3^4;
sbit XPT2046_DOUT = P3^7;
sbit XPT2046_CS = P3^5;
sbit XPT2046_DCLK = P3^6;

unsigned int XPT2046_readAD(BYTE cmd) {
    BYTE i;
    unsigned int rec = 0;

    XPT2046_DCLK = 0;
    XPT2046_CS = 0;

    for(i = 0; i < 8; i++) {
        XPT2046_DIN = cmd & (0x80 >> i);
        XPT2046_DCLK = 1;
        XPT2046_DCLK = 0;
    }

    for(i = 0; i < 16; i++) {
        XPT2046_DCLK = 1;
        delay(1);
        XPT2046_DCLK = 0;
        if(XPT2046_DOUT) {

            // 这里搞错了，rec 这里是 16bit，所以要置高位需要 | 0x8000
            rec |= (0x8000 >> i);
        }
    }

    XPT2046_CS = 1;

    if(cmd & 0x08) {
        return rec >> 8;
    } else 

        return rec >> 4;
}