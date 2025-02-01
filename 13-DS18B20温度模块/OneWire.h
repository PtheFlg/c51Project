#ifndef __ONE_WIRE_H__
#define __ONE_WIRE_H__

unsigned char OneWire_init();
void OneWire_writeByte(unsigned char b);
unsigned char OneWire_readByte();

#endif