#include <REGX52.H>

#define uchar unsigned char

void delay(unsigned int xms)		//@12.000MHz
{
	uchar i, j;

	do{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} 	while (--i);
	}while( --xms);
}

void uartInit() {
    // 串口设置：SCON 串口控制寄存器
    SM0 = 0;    //
    SM1 = 1;    // SM0/1 = 01：串口工作方式1：8位 UART 波特率可变
    REN = 1;    // 允许接收 receive enable ，前面这三位 SM0 SM1 SM2 REN，正好就是 0x50
    TI = 0;     // 发送中断
    RI = 0;     // 接收中断

    PCON |= 0x80;		//使能波特率倍速位SMOD
    
    // 中断设置：IE 中断允许寄存器
    EA = 1;     // 开启全部中断
    ES = 1;     // 开启串口中断
    ET1 = 0;    // 禁止 Timer1 中断

    // Timer设置：TCON 寄存器 和 TMOD 寄存器
    TR1 = 1;    // Timer1 Ready

    TMOD = TMOD & 0x0F;     // TMOD 前4b 置0
    TMOD = TMOD | 0x20;     // TMOD 前4b 置 0010

    // ！！！切记切记，我这个晋中开发板的频率是 11.0592，不是 12MHz
    // 如果设置 12MHz，会导致 Bug，电脑给 任何数，D8 LED 都会亮
    // 初始化计数器 11.0592MHz
    TL1 = 0xF4;		//设定定时初值
    TH1 = 0xF4;		//设定定时器重装值
}

void writeByte(uchar _byte);

void uartRoutine() interrupt 4 {
    if(RI = 1) {
        RI = 0;
        P2 = ~SBUF;
        writeByte(SBUF);
    }
}

void writeByte(unsigned char _byte) {
    SBUF = _byte;

    while(TI == 0);

    TI = 0;
}

void main() {

    unsigned char i = 0;

    uartInit();

    while(1) {
        // writeByte(i++);
        // delay(1000);
    }
    
}