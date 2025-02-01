#include <REGX52.H>
#define BYTE unsigned char
#include <INTRINS.H>

sbit OW_DQ = P3^7;

/**
 * 单总线初始化，单总线这个破玩意每次发送都得初始化一下，
 * 主机拉低电平询问是否有从机，从机紧接着也拉低电平回应主机
 * 
 * 注意这个响应，从机会响应低电平，所以如果有从机会响应 0，没有才响应 1
 */
BYTE OneWire_init() {
    BYTE i;
    BYTE ack;

    // 主机拉低电平至少 480 us，取 500
    OW_DQ = 1;
    OW_DQ = 0;
    i = 227;    while (--i);    // 这里大致延迟 500 us
    OW_DQ = 1;  // 主机释放总线

    // 等待15 - 60 us，然后从机将会拉低总线来回应主机，持续 60-240us
    // 这里取 70us，然后就直接读总线，看总线是否是低电平
    i = 29; while (--i);  
    ack = OW_DQ;

    // 再等待 500us，等待从机释放总线
    i = 227;    while (--i);

    return ack;

}

/**
 * 主机写一个 bit：
 * 
 * 总的来说就是从机在总线变成低电平 30us 后采样，如果还是低电平说明发0
 * 如果在采样之前主机释放了总线，总线弹回了 1，说明发的是 1
 * 
 * 
 */
void OneWire_writeBit(BYTE bi) {

    BYTE i;

    // 拉低总线，15us后将 bi 写到总线上
    // 如果 bi 是 1，那就拉高了电平，如果是 0，则电平保持不动
    // 15us 后从机就会采样
    OW_DQ = 0;
    i = 6;  while (--i);    // 延迟 15us，但是生成的时候给 19us，因为函数调用占 4us
    OW_DQ = bi;

    // 设置完以后等待 45us
    // 因为手册说，发送一个bit的时间片应该大于 60us
    i = 27; while (--i);

    // 释放总线
    OW_DQ = 1;
} 

/**
 * 主机读一个 bit：
 * 
 * 总结起来也是：主机先把总线拉低，然后等待 1-15us 就释放总线，然后拉低总线后 15us，读取总线
 * 读到低电平，就是 0，读到高电平，就是 1
 */
BYTE OneWire_readBit() {
    BYTE rec;
    BYTE i;

    // 先拉低总线，等待 5us 就释放总线
    OW_DQ = 0;
    _nop_();    i = 1;  while (--i);
    OW_DQ = 1;

    // 等待 10us 读取总线，然后再等 45us 凑够一个 60us 周期
    _nop_();    i = 3;  while (--i);    // 等待 10us
    rec = OW_DQ;
    i = 20; while (--i);    // 等待 45us

    // 这里就不用释放总线了，从机发完会释放的
    // 直接 return 即可。
    return rec;
}

/**
 * 写入一个字节：
 * 
 * 将一个字节从低位到高位依次写入
 */
void OneWire_writeByte(BYTE b) {
    BYTE i;
    for(i = 0; i < 8; i++) {
        OneWire_writeBit(b & (0x01 << i));
    }
}

/**
 * 读出一个字节：
 * 
 * 同理从低位到高位
 */
BYTE OneWire_readByte() {
    BYTE i;
    BYTE rec = 0x00;
    for(i = 0; i < 8; i++) {
        if(OneWire_readBit()) {
            rec = rec | (0x01 << i);
        }
    }

    return rec;
}
