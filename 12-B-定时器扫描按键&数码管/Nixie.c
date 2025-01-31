#include <REGX52.H>
#define BYTE unsigned char

/**
 * 这个数码管模块和 LCD 1602 模块冲突，插上 LCD 数码管就不亮了
 */

/**
 * 除了常规的 1 - F(15) 以外，多加了一个 0x00，代表什么都不显示
 */
BYTE NUMBERS[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,

    0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,

    0x00
};

/**
 * 8个数码管都要显示几
 */
BYTE Nixie_buf[] = {
    16, 16, 16, 16, 16, 16, 16, 16
};

/**
 * 设置数码管缓冲
 */
void Nixie_setBuf(BYTE i, BYTE _data) {
    Nixie_buf[i] = _data;
}

void selectLED(BYTE i) {
    P2 = P2 & 0xE3;  // 1110 0011
    P2 = P2 | (i << 2);  // 0001 
}


void Nixie_showNum(BYTE i, _data) {
    P0 = 0x00;
    selectLED(i);
    P0 = NUMBERS[_data];
}

/**
 * 数码管的外部中断函数，由外部计时器的中断函数调用
 */
void Nixie_loop() {
    static BYTE i = 0;
    Nixie_showNum(i, Nixie_buf[i]);
    i++;
    if(i == 8)  i = 0;
}