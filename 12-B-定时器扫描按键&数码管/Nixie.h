#ifndef __NIXIE_H__
#define __NIXIE_H__

#define BYTE unsigned char

extern void Nixie_showNum(unsigned char i, _data);

void Nixie_setBuf(BYTE i, BYTE _data);

void Nixie_loop();

#endif