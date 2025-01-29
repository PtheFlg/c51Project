#include "I2C.h"
#include <REGX52.H>
#define BYTE unsigned char

// AT24C02 写地址，最后一位 0，如果要用读地址，加 1
#define SLAVE_ADDR 0xA0

void AT24C02_wirteByte(BYTE addr, BYTE b) {
    BYTE ack;
    I2C_start();

    I2C_writeByte(SLAVE_ADDR);
    ack = I2C_readAck();

    // if(!ack) {
    //     P2 = 0x00;
    // }
    // while(ack);

    I2C_writeByte(addr);
    ack = I2C_readAck();

    I2C_writeByte(b);
    ack = I2C_readAck();

    I2C_stop();
}

BYTE AT24C02_readByte(BYTE addr) {
    BYTE ack;
    BYTE readByte;

    I2C_start();

    I2C_writeByte(SLAVE_ADDR);
    ack = I2C_readAck();

    I2C_writeByte(addr);
    ack = I2C_readAck();

    I2C_start();

    I2C_writeByte(SLAVE_ADDR + 1);
    ack = I2C_readAck();

    readByte = I2C_readByte();
    I2C_writeAck(1);

    I2C_stop();

    return readByte;
}