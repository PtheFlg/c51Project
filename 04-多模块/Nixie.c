#include <REGX52.H>
#include "Delay.h"


#define uchar unsigned char


uchar NUMBERS[] = {
    0x3F,
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D,
    0x7D,
    0x07,
    0x7F,
    0x6F,

    0x77,
    0x7C,
    0x39,
    0x5E,
    0x79,
    0x71
};

void selectLeds(uchar i) {
    uchar s = 0xE3 + ( (i - 1) << 2 );

    P2 = s;
}

void nixie(uchar i, uchar number) {
    selectLeds(i);
    P0 = NUMBERS[number];

    delay(2);
    P0 = 0x00;
}