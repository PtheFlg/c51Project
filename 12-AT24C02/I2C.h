#ifndef __I2C_H__
#define __I2C_H__

#define BYTE unsigned char

void I2C_start();

void I2C_stop();

void I2C_writeByte(BYTE b);

BYTE I2C_readByte();

void I2C_writeAck(BYTE ack);

BYTE I2C_readAck();

#endif