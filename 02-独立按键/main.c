#include <REGX52.H>

void delay(int xms)		//@12.000MHz
{
	unsigned char i, j;

	do{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} 	while (--i);
	}while( --xms);
}

/**
 * 检测按键是否一直被按着
 */
char buttonDown(unsigned char _index) {
    if(_index == 1) return P3 == 0xFD;
    if(_index == 2) return P3 == 0xFE;
    if(_index == 3) return P3 == 0xFB; // 1111 1011
    if(_index == 4) return P3 == 0xF7; // 1111 0111

    return 0;
}

/**
 * 检测按键按下1次
 * 
 * 这个复杂一点，按键按下以后，电压会有波动，5-10ms 后，电压趋于平稳 0V
 * 然后松开到稳定 5V 又需要 5-10ms
 * 
 * 检测按键被按下一次一般来说检测的是按键弹起，也就是针脚先变为 0V，后变为 5V，检测后变
 * 
 * 思路：
 * 当寄存器为 0 说明按键被按下，然后开始根据 寄存器 == 0 循环，直到寄存器又变为 1
 */

char _buttonDown(unsigned char _index) {
    unsigned char _regs[4] = {0xFD, 0xFE, 0xFB, 0xF7};

    if(P3 == _regs[_index]) {
        delay(10);

        while(P3 == _regs[_index]);

        delay(10);

        return 1;
    }

    return 0;
}

/**
 * LED 右移一位
 * (最左边的 LED 的引脚是 P20，P2 寄存器的最低位，所以要实现LED右移，就得实现 P2 寄存器 0 的左移)
 * 1111 1110 -> 1111 1101 -> 1111 1011 每次都是 0 左移，只需要这个数左移一位，末尾 + 1 即可。
 */
void ledRight() {

    if(P2 == 0x7F) 
        P2 = 0xFE;
    else    
        P2 = (P2 << 1) + 1;
}

/**
 * LED 左移一位（P2 寄存器 0 的右移）
 * 1111 1110 -> 0111 1111 -> 1011 1111 -> 1101 1111
 * P2 的数整体右移，左边会补0，加上 1000 0000 (0x80)即可
 */
void ledLeft() {
    if(P2 == 0xFE) 
        P2 = 0x7F;
    else
        P2 = (P2 >> 1) + 0x80;
}


void main() {

    /**
     * 独立按键: 一边接 GND，一边接 P30 31 32 33
     * 当按键按下，接通引脚和 GND，引脚就会变成 0V，对应寄存器值就变为 0
     * 
     * 根据参考手册寄存器说明，P30 31 32 33 属于寄存器 P3 的低四位
     * 所以目标就是检测低四位的 01 情况
     */

    // while(1) {

    //     // 按键 K1 接 P31 接口，若接通 p3 变为 1111 1101 = 0xFD
    //     if (P3 == 0xFD) {
    //         P2 = 0xFE;
    //     } else {
    //         P2 = 0xFF;
    //     }
    // }

    while(1) {
        if(_buttonDown(0)) {
            P2 = 0xFE;
        } else if(_buttonDown(1)) {
            ledLeft();
            // delay(100);
        } else if(_buttonDown(2)) {
            ledRight();
            // delay(100);
        } else if(_buttonDown(3)) {
            P2 = 0xFF;
        }
    }

    
}