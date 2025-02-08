#include "INT0.h"
#include "Timer0.h"

#include <REGX52.H>

#define BYTE unsigned char
#define uint unsigned int

BYTE IR_State = 0;
BYTE IR_Repeat = 0;

uint IR_Time = 0;
BYTE IR_Data[4];
BYTE IR_pData = 0;

BYTE IR_DataFlag = 0;   // 是否收到了数据
BYTE IR_Address = 0;    // 发送器地址码
BYTE IR_Command = 0;    // 发送器指令字


void IR_init() {
    Timer0_init();
    int0init();
}

BYTE IR_getAddress() {
    return IR_Address;
}

BYTE IR_getCommand() {
    return IR_Command;
}

BYTE IR_getDataFlag() {
    if(IR_DataFlag == 1) {
        IR_DataFlag = 0;
        return 1;
    }
    return 0;
}

BYTE IR_getRepeat() {
    if(IR_Repeat == 1) {
        IR_Repeat = 0;
        return 1;
    }
    return 0;
}

void INT0_routine() interrupt 0 {

    /**
     * IR 为空闲态，说明还没开始传输数据，这时检测到下降沿
     * 说明准备要发送数据了，就开始启动定时器0，然后将 IR 转为 1 阶段
     */
    if(IR_State == 0) {
        // P2 = 0;
        Timer0_setCounter(0);
        Timer0_run(1);
        IR_State = 1;
    }
    /**
     * IR 为 start 或者 repeat 状态，也就是这个状态刚从空闲态 0 过来
     * start 或者 repeat 肯定还没发，所以不需要解码，只需要检测是不是 start / repeat
     */
    else if(IR_State == 1) {
        IR_Time = Timer0_getCounter();
        Timer0_setCounter(0);
        // P2 = 0;

        // 两个下降沿间隔 13500us 也就是 13.5ms，说明是 UNS 协议的 start 状态
        // 后面要开始正式传数据了，将状态转为 2 （解码），这里给 +- 1000us 的误差
        // 500us 不行
        if(IR_Time > 13500 - 1000 && IR_Time < 13500 + 1000) {
            // P2 = 0;
            IR_State = 2;
        } 
        // 如果间隔 11250us = 11.25ms，说明刚刚发送的是 repeat，这一个数据帧已经发送完了
        // 后面就会重复整个过程：空闲-start-数据-repeat 这个过程
        // 所以重置状态为 0，关闭计时器
        else if(IR_Time > 11250 - 1000 && IR_Time < 11250 + 1000) {
            IR_Repeat = 1;
            IR_State = 0;
            Timer0_run(0);
        } 
        // 如果都不是，可能是发错了，出问题了，那就把状态重新设为 1
        else {
            IR_State = 1;
        }
    }
    /**
     * 解码状态，start / repeat 状态后，正式开始发送数据，开始解码
     */
    else if(IR_State == 2) {
        IR_Time = Timer0_getCounter();
        Timer0_setCounter(0);

        // 两个下降沿间隔 1120us，说明发的是 1
        if(IR_Time > 1120-500 && IR_Time < 1120+500) {

            /**
             * 总共会发送 0-31 位数据，怎么把他们有序的收集起来？
             * 
             * 1. 定义4个 BYTE放到一个数组，作为接收的四个字节
             * 2. 定义一个指针 IR_pData，这个数会从 0 加到 31
             * 3. 把这 31 个索引看作四组，对应数组里的四个 BYTE，所以第几个 BYTE 就是 IR_pData / 8
             * 4. BYTE 里的某一位，就是 IR_pData % 8。
             * 
             * 这里收到 0，那就把对应 BYTE 里的对应位置 0，用 & ~(0x01 << 第几位) 的方法
             * 后面置 1 同理
             */
            IR_Data[IR_pData / 8] = IR_Data[IR_pData / 8] & ~(0x01 << (IR_pData % 8));
            IR_pData++;
        } 
        // 间隔为
        else if(IR_Time > 2250 - 500 && IR_Time < 2250 + 500) {
            IR_Data[IR_pData / 8] = IR_Data[IR_pData / 8] | (0x01 << (IR_pData % 8));
            IR_pData++;
        }
        // 出问题了，直接放弃所有的数据，重新开始接收
        else {
            IR_State = 1;
            IR_pData = 0;
        }

        /**
         * 总共收了 32 位，说明收完了
         */
        if(IR_pData  >= 32) {
            IR_pData = 0;
            if((IR_Data[0] == ~IR_Data[1]) && (IR_Data[2] == ~IR_Data[3])) {
                IR_DataFlag = 1;    // 收到了数据
                IR_Address = IR_Data[0];
                IR_Command = IR_Data[2];
            }

            Timer0_run(0);
            IR_State = 0;
        }
    }
}
