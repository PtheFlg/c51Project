#ifndef __IR_H__
#define __IR_H__

#define BYTE unsigned char

void IR_init();

BYTE IR_getCommand();
BYTE IR_getAddress();
BYTE IR_getDataFlag();
BYTE IR_getRepeat();


#endif