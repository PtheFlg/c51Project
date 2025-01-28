#include <REGX52.H>
#include <INTRINS.H>

#define BYTE unsigned char

sbit RCK = P3^5;
sbit DS1302_SCK = P3^6;
sbit SER = P3^4;

void delay(unsigned int xms)		//@12.000MHz
{
	BYTE i, j;

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
 * 74HC595 串行转并行，串行传入一个 BYTE，并行输出8位
 * 
 * 看原理图：
 * 
 * SER: 串行传输线，通过这个线传入 1/0
 * SERCLK: 移位时钟，当 SER 传入 1/0 时，让 SERCLK = 1 （电平上升沿检测）
 *  就会让芯片内部的位全部下移一位，顺带将 SER 上的电平(0/1)移到芯片内部
 * RCLK: 寄存器时钟，上面的步骤重复8次，每次都是，SER 传入 -> SERCLK 置高电平 -> 串行数据移位
 *  最终填满 8 位，给 RCLK 一个高电平，就会将这 8 位输出到 8 个输出线上，同时清空 8 位
 */
void s2c(BYTE con) {
    // con 的二进制
    BYTE i = 8;
    BYTE bin[8] = {0};

    while(i--) {
        bin[i] = con % 2;
        con /= 2;
    }

    for(i = 0; i < 8; i++) {
        P3_4 = bin[i];
        P3_6 = 1;
        _nop_();
        P3_6 = 0;
    }

    P3_5 = 1;
    _nop_();
    P3_5 = 0;
}

/**
 * 74HC595 的另一种写法：用 & 和 | 运算来算出每一位
 * 
 * 检测某位是 1/0：&，如：
 * 
 * 检测 b 的 第8位 是 1/0，则 b & 0x80
 * b 如果是 1010 0011，则 1010 0011 & 1000 0000
 * 按位计算得到： 1000 0000，说明第八位是 1
 * 
 * 如果 b 是 0111 0110，则 0111 0110 & 1000 0000 
 * 得 0000 0000，说明第八位是 0
 * 
 * 只需要检测最后结果是不是 0 即可，非 0 即 1，
 * 比如最终 b & 0x80 得到 0x80，然后 SCK = 0x80，SCK 只是一位，则
 * 只要后面的不是 0，那就按 1 算
 * 
 * 要检测每一位，那就让 0x80 右移即可
 */
void _74HC595writeByte(BYTE b) {
    BYTE i = 0;
    SER = 0; DS1302_SCK = 0; RCK = 0;
    for(; i < 8; i++) {
        SER = b & (0x80 >> i);
        DS1302_SCK = 1;
        DS1302_SCK = 0;
    }

    RCK = 1;
    RCK = 0;
}

/**
 * 行选：
 * 选哪一行，就在前面放 row - 1 个 0，再放个1，后面再补 0
 */
void rowSelect(BYTE row) {
    BYTE i = row - 1;
    BYTE j = 8 - row;
    SER = 0;
    while(i--) {
        DS1302_SCK = 1;
        DS1302_SCK = 0;
    }

    SER = 1;
    DS1302_SCK = 1;
    DS1302_SCK = 0;
    SER = 0;

    while(j--) {
        DS1302_SCK = 1;
        DS1302_SCK = 0;
    }

    RCK = 1;
    RCK = 0;
    
}


/**
 * LED 点阵屏显示一行数据
 * 
 * row：哪一行
 * _data：显示内容
 */
void matrixLedsShowRow(BYTE row, _data) {
    rowSelect(row);
    P0 = ~(_data);
    delay(1);
    P0 = 0xFF;
}


void main() {

    while(1) {
        // rowSelect(3);
        // _74HC595writeByte(0xE0);
        // P0 = 0x7F;

        matrixLedsShowRow(1, 0x80);
        matrixLedsShowRow(2, 0x40);
        matrixLedsShowRow(3, 0x20);
        matrixLedsShowRow(4, 0x10);
    }
}