#include <REGX52.H>

void int0init() {

    EA = 1;

    // 外部中断0 允许
    EX0 = 1;
    IE0 = 0;

    // 下降沿触发外部中断
    IT0 = 1;

    // 中断优先级
    PX0 = 1;
}