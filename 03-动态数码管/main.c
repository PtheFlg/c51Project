#include <REGX52.H>
#include "Nixie.h"

#define uchar unsigned char
#define _DELAY 5

/**
 * 选中某一个数目管
 * LED1(Y0) : 111 000 11
 * LED2(Y1) : 111 001 11
 * LED3(Y2) : 111 010 11
 * 
 * 想选通 Y几，就给 111 000 11 加上 (几 << 2)
 * 比如要选通 Y2 给 LED3 一个低电平，就是 111 000 11 + 000 010 00
 * 也就是 111 000 11 加上 10 再左移两位
 */

// void selectLeds(uchar i) {
//     uchar s = 0xE3 + ( (i - 1) << 2 );

//     P2 = s;
// }

/**
 * 选中某个数码管并显示 number
 * 
 * 2. 动态数码管显示
 * 
 * 如果直接调用函数，就会发现多个数码管之间有残影，不清楚，原因：
 * 
 * 这里我们先段选（选择数码管），再显示，所以在段选之后短暂的一个空挡，P0 还没有改过来，
 * 这个新的数码管会显示上一个数码管的数字，这就导致了串字
 * 
 * 解决办法就是：数码管稳定的显示 2ms 后，将 P0 归 0，不让他显示了
 * 这样下一个数码管就不会显示上一个数字
 */
// void nixie(uchar i, uchar number) {
//     selectLeds(i);
//     P0 = NUMBERS[number];

//     delay(2);
//     P0 = 0x00;
// }

void main() {
    /**
     * 1. 第八个数码管显示 2
     * 
     * 第八个数码管的负极是 LED1，然后 LED 1-8 使用三八译码器译码
     * 译码器输入端是 P22 23 24 高电平，输出端是 LED 1-8 的低电平
     * 比如 输入 001，则输出 LED2 低电平 0V
     * 输入 011，输出 LED4 低电平 0V
     */
    while (1) {
        // 想要片选 LED1，则输入的 P22 23 24 就得是第一个状态 ：000
        // P2 = 0xE3; // 1110 0011

        // selectLeds(8);

        // 显示 2，则数码管的 a b g e d 点亮，也就是对应的引脚给高电平，而 f c dp 给低电平
        // (c - P02) (f - P05) (dp - P07) (g -P06) (b - P01) (e - P04)
        // P0 = NUMBERS[2]; // 0101 1011  0x5B = 2

        // delay(_DELAY);

        // selectLeds(7);
        // P0 = NUMBERS[5]; // 0110 1101  0x6D = 5
        // delay(_DELAY);

        // selectLeds(6);
        // P0 = NUMBERS[0]; // 0011 1111  0x3F = 0
        // delay(_DELAY);

        // nixie(8, 2);
        // // delay(5);

        // nixie(7, 5);
        // // delay(5);

        // nixie(6, 0);
        // // delay(5);

        Nixie_showNum(7, 1);
        Nixie_showNum(6, 2);
        Nixie_showNum(5, 3);
    }
}