#ifndef __DS1302_H__
#define __DS1302_H__

// extern unsigned char ds_time[];


extern unsigned char ds_time[];

void DS1302_init();
unsigned char DS1302_readByte(unsigned char command);
void DS1302_writeByte(unsigned char command, _data);

void DS1302_setTime();
void DS1302_readTime();

#endif