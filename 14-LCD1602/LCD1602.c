#include <REGX52.H>
#include <INTRINS.H>

#define LCD1602_Dn  P0

#define BYTE unsigned char

#define LCD1602_CLEAR       0x01        // 清屏指令
#define LCD1602_FUNCTION_SET    0x38        // 功能设置：8 位数据结构，两行显示，5x7点阵
#define LCD1602_DISPLAY_SET     0x0C        // 显示开，光标不显示，闪烁关
#define LCD1602_INPUT_SET       0x06        // 读写操作后，光标 AC 自动加一，画面不动

sbit LCD1602_RS = P2^6;         // 0 指令 1 数据
sbit LCD1602_RW = P2^5;         // 0 写   1 读
sbit LCD1602_EN = P2^7;         // 

void LCD1602_delay1ms(){		//@11.0592MHz

	unsigned char i, j;

	_nop_();
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}

/**
 * LCD 写命令：让 RS = 1 表示发命令，RW = 0 表示写
 * 然后把 cmd 整体写到 8个并行线上
 * 然后 EN = 1，等待1ms让 LCD 反应一下，读取这个命令
 * 然后 EN = 0，然后 LCD 执行，同样等待 1ms
 */
void LCD1602_writeCommand(BYTE cmd) {
    LCD1602_RS = 0;
    LCD1602_RW = 0;

    LCD1602_Dn = cmd;

    LCD1602_EN = 1;
    LCD1602_delay1ms();
    LCD1602_EN = 0;
    LCD1602_delay1ms();
}

/**
 * 和前面差不多，一个道理，唯一的区别就是 RS = 1，变成数据模式
 */
void LCD1602_writeByte(BYTE b) {
    LCD1602_RS = 1;
    LCD1602_RW = 0;

    LCD1602_Dn = b;

    LCD1602_EN = 1;
    LCD1602_delay1ms();
    LCD1602_EN = 0;
    LCD1602_delay1ms();
}

/**
 * LCD 初始化总共就 4 步：
 * 1. 设成 8 位，2 行，5x7 显示模式
 * 2. 开启显示器，关闭光标显示，关闭光标闪烁
 * 3. 读取/写入 光标自动 + 1，后移
 * 4. 清屏
 */
void LCD1602_init() {
    LCD1602_writeCommand(LCD1602_FUNCTION_SET);
    LCD1602_writeCommand(LCD1602_DISPLAY_SET);
    LCD1602_writeCommand(LCD1602_INPUT_SET);
    LCD1602_writeCommand(LCD1602_CLEAR);
}

/**
 * 这个 LCD 的光标就是读取写入的位置，每次写都是往光标位置写，
 * 然后前面设置了光标自增，那写完一个字符，光标就会自动往后挪，
 * 然后就可以连续的发送字节，实现顺序的往后写
 */
void LCD1602_setCursor(BYTE row, BYTE col) {
    BYTE cmd;

    if(row == 1) {
        cmd = 0x80 | ( col - 1 );
    } else if(row ==2) {
        cmd = 0x80 | (col - 1) + 0x40;
    }

    LCD1602_writeCommand(cmd);
}

/**
 * 显示字符：
 * 
 * 先根据 行列 设置光标
 * 然后传入要写入的字符 的 ascii
 */
void LCD1602_showChar(BYTE row, BYTE col, BYTE show) {
    LCD1602_setCursor(row, col);
    LCD1602_writeByte(show);
}

/**
 * 写入字符串，这里字符串就是一个 char 数组，用 * 指针来接收
 * 
 * 同理，先设置光标，然后遍历字符串，字符串的末尾是一个 \0
 * 所以检测到 \0 停止读取字符串
 */
void LCD1602_showString(BYTE row, BYTE col, char *st) {
    BYTE i;

    LCD1602_setCursor(row, col);

    for(i = 0; st[i] != '\0'; i++) {
        LCD1602_writeByte(st[i]);
    }

    // LCD1602_writeByte('H');
    // LCD1602_writeByte('e');
    // LCD1602_writeByte('l');
}

int LCD_pow(int x, int y) {
    BYTE i;
    int result = 1;
    for(i = 0; i < y; i++) {
        result = result * x;
    }

    return result;
}

/**
 * 显示数字：
 * 
 * LCD 从左往右显示，所以要先显示最高位，最后显示最低位，所以如何取高位？
 * 
 * 一个数，要取 i 位，那就 n / 10^(i-1) % 10。
 * 其实也好理解，就是把要取的位数挪到个位，然后 % 10 取个位。
 * 1234 的 1 在千位，怎么挪到个位？这个数右移 3 位（十进制情况下的右移），那就是 1234 / 10 ^ 3
 * 
 * 这个方法在待会同理，传入 1234，要把它变成 16 进制，输出每一位，一样，就把他看作 16进制
 * 取哪位，就把哪位挪到个位（最低位），在 16 进制下，右移3 位，就是 1234 / 16 ^ 3，
 * 然后取个位数，% 16
 * 
 * 最后要显示这个数字的时候，显示的是这个数字对应的字符的 ascii，需要给数值 + '0'，也就是 0x30
 */
void LCD1602_showNum(BYTE row, BYTE col, unsigned int show, BYTE len) {
    LCD1602_setCursor(row, col);

    while(--len) {
        LCD1602_writeByte(0x30 + show / LCD_pow(10, len) % 10);
    }

    LCD1602_writeByte(0x30 + show % 10);
}

void LCD1602_showSignedNum(BYTE row, BYTE col, signed int show, BYTE len) {
    LCD1602_setCursor(row, col);
    if(show < 0){
        LCD1602_writeByte('-');
        show = -show;
    }
    else
        LCD1602_writeByte('+');

    while(--len) {
        LCD1602_writeByte(0x30 + show / LCD_pow(10, len) % 10);
    }

    LCD1602_writeByte(0x30 + show % 10);
}

void LCD1602_showHexNum(BYTE row, BYTE col, unsigned int show, BYTE len) {
    BYTE i;

    LCD1602_setCursor(row, col);
    while(len--) {
        i = show / LCD_pow(16, len) % 16;
        if(i >= 10) {
            LCD1602_writeByte('A' + (i - 10));
        } else {
            LCD1602_writeByte('0' + i);
        }
    }
}

void LCD1602_showBinNum(BYTE row, BYTE col, unsigned int show, BYTE len) {
    BYTE i;

    LCD1602_setCursor(row, col);

    while(len--) {
        i = show / LCD_pow(2, len) % 2;
        if(i) {
            LCD1602_writeByte('1');
        } else {
            LCD1602_writeByte('0');
        }
    }
}
