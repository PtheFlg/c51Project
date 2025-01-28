#ifndef __DS1302_H__
#define __DS1302_H__

// extern unsigned char ds_time[];

#define BYTE unsigned char
typedef struct {
    BYTE year;
    BYTE month;
    BYTE date;
    BYTE hour;
    BYTE min;
    BYTE sec;
    BYTE day;
}DSTime;

extern DSTime ds_time;

void DS1302_init();
unsigned char DS1302_readByte(unsigned char command);
void DS1302_writeByte(unsigned char command, _data);

void DS1302_setTime();
void DS1302_readTime();

#endif