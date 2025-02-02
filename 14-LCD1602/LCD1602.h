#ifndef __LCD1602_H__
#define __LCD1602_H__

#define BYTE unsigned char

void LCD1602_init();
void LCD1602_showChar(BYTE row, BYTE col, BYTE show);
void LCD1602_showString(BYTE row, BYTE col, char *show);

void LCD1602_showNum(BYTE row, BYTE col, unsigned int show, BYTE len);
void LCD1602_showSignedNum(BYTE row, BYTE col, signed int show, BYTE len);
void LCD1602_showHexNum(BYTE row, BYTE col, unsigned int show, BYTE len);
void LCD1602_showBinNum(BYTE row, BYTE col, unsigned int show, BYTE len);

#endif