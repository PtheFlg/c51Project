#ifndef __AT24C02_H__
#define __AT24C02_H__

#define BYTE unsigned char

void AT24C02_wirteByte(BYTE addr, BYTE b);

BYTE AT24C02_readByte(BYTE addr);

#endif