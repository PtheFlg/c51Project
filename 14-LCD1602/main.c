#include <REGX52.H>
#include "LCD1602.h"

#define BYTE unsigned char

void main() {

    LCD1602_init();

    LCD1602_showChar(1, 1, 'A');
    // LCD1602_showChar(1, 2, 'H');

    LCD1602_showString(1, 3, "Hello?");

    LCD1602_showNum(1, 10, 47, 3);

    LCD1602_showSignedNum(2, 1, 123, 3);

    LCD1602_showHexNum(2, 5, 243, 2);

    LCD1602_showBinNum(2, 8, 0xAA, 8);

    while(1) {

    }

}