#include <REGX52.H>
#include "INT0.h"
#include "Timer0.h"
#include "Delay.h"
#include "LCD1602.h"
#include "IR.h"

#define BYTE unsigned char
#define uint unsigned int

BYTE ADDRESS;
BYTE COMMAND;


void main() {
    LCD_Init();
    IR_init();    

    while(1) {
        if(IR_getDataFlag()) {
            ADDRESS = IR_getAddress();
            COMMAND = IR_getCommand();

            LCD_ShowNum(2, 1, ADDRESS, 2);
            LCD_ShowNum(2, 4, COMMAND, 2);
        }   
    }

}

