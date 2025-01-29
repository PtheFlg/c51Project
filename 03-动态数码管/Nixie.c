#include <REGX52.H>
#define BYTE unsigned char

/**
 * 这个数码管模块和 LCD 1602 模块冲突，插上 LCD 数码管就不亮了
 */

void nixie_delay2ms()		//@12.000MHz
{
	BYTE i, j;
    BYTE z = 2;

	do{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} 	while (--i);
	}while( --z);
}

BYTE NUMBERS[] = {
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

void selectLED(BYTE i) {
    P2 = P2 & 0xE3;  // 1110 0011
    P2 = P2 | (i << 2);  // 0001 
}

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
void Nixie_showNum(BYTE i, _data) {
    selectLED(i);
    P0 = NUMBERS[_data];
    nixie_delay2ms();
    P0 = 0x00;
}