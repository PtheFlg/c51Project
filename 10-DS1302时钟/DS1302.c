#include <REGX52.H>

#define BYTE unsigned char

#define DSTIME_YEAR     0x8C
#define DSTIME_MONTH    0x88
#define DSTIME_DATE     0x86
#define DSTIME_HOUR     0x84
#define DSTIME_MIN      0x82
#define DSTIME_SEC      0x80
#define DSTIME_DAY      0x8A

#define DSTIME_WP       0x8E

sbit DS1302_CE = P3^5;
sbit DS1302_IO = P3^4;
sbit DS1302_SCK = P3^6;

unsigned char ds_time[] = {
    25, 01, 27, 22, 34, 10, 1
};

// struct DSTime {
//     BYTE year;
//     BYTE month;
//     BYTE date;
//     BYTE hour;
//     BYTE min;
//     BYTE sec;
//     BYTE day;
// };
// DSTime ds_time = {25, 1, 27, 22, 58, 10, 1};
// ds_time = {25, 1, 27, 22, 58, 10, 1};

void DS1302_init() {
    DS1302_SCK = 0;
    DS1302_CE = 0;
}

BYTE DS1302_readByte(BYTE cmd) {
    // BYTE cmd[] = {
    //     1, 0, 0, 0, 0, 0, 0, 1
    // };
    BYTE i;
    BYTE _rec = 0x00;
	

    // 上面给出了 写不同寄存器的 cmd，但这里是读寄存器的命令最后一位肯定是1
    // 所以给 写命令最后一位置 1 即可变成读命令
    cmd |= 0x01;

    DS1302_CE = 1;
    
    for(i = 0; i < 8; i++) {

        // 这里用这种办法可行，因为是给 IO 赋值，付完 cmd 就没用了，
        // 多移一次没关系
        // DS1302_IO = cmd & 0x01;
        // cmd = cmd >> 1;
        DS1302_IO = cmd & (0x01 << i);
        DS1302_SCK = 0;
        DS1302_SCK = 1;
    }

    // rec[0] = IO;
    // SCK = 1;
    // for(i = 1; i < 8; i++) {
    //     SCK = 0;
    //     rec[i] = IO;
    //     SCK = 1;
    // }

    // _rec = _rec + (DS1302_IO ? 0x80 : 0x00);
    // _rec = _rec >> 1;
    // DS1302_SCK = 1;

    // for(i = 1; i < 8; i++) {
    //     DS1302_SCK = 0;
    //     _rec = _rec + (DS1302_IO ? 0x80 : 0x00);
    //     _rec = _rec >> 1;
    //     DS1302_SCK = 1;
    // }

    for(i = 0; i < 8; i++) {
        DS1302_SCK = 1;
        DS1302_SCK = 0;

        // 这种写法会导致多一次移位
        // _rec 只需要右移 7 次，把左边第一位一道最右边，7 次
        // 但是这种写法会导致最后一次，将左边第一位加上以后，又移了一次，导致 bug
        // _rec = _rec + (DS1302_IO ? 0x80 : 0x00);
        // _rec = _rec >> 1;

        // 只有这种写法可行，不知道为啥
        if(DS1302_IO) {
            _rec = _rec | (0x01 << i);
            //_rec = _rec + 0x80;
        }

        //_rec = _rec >> 1;
    }

    // 这一步比较关键：
    DS1302_IO = 0;

    DS1302_CE = 0;

    return _rec;
}

void DS1302_writeByte(BYTE cmd, _data) {
    BYTE i;

    DS1302_CE = 1;
    
    for(i = 0; i < 8; i++) {
        // DS1302_IO = cmd & 0x01;
        // cmd = cmd >> 1;
        DS1302_IO = cmd & (0x01 << i);
        DS1302_SCK = 1;
        DS1302_SCK = 0;
    }

    for(i = 0; i < 8; i++) {
        // DS1302_IO = _data & 0x01;
        // _data = _data >> 1;
        DS1302_IO = _data & (0x01 << i);
        DS1302_SCK = 1;
        DS1302_SCK = 0;
    }

    DS1302_CE = 0;
}

void DS1302_setTime() {
    DS1302_writeByte(DSTIME_WP, 0x00);
    DS1302_writeByte(DSTIME_YEAR,   ds_time[0]   / 10 * 16 + ds_time[0]   % 10);
    DS1302_writeByte(DSTIME_MONTH,  ds_time[1]   / 10 * 16 + ds_time[1]   % 10);
    DS1302_writeByte(DSTIME_DATE,   ds_time[2]   / 10 * 16 + ds_time[2]   % 10);
    DS1302_writeByte(DSTIME_HOUR,   ds_time[3]   / 10 * 16 + ds_time[3]   % 10);
    DS1302_writeByte(DSTIME_MIN,    ds_time[4]   / 10 * 16 + ds_time[4]   % 10);
    DS1302_writeByte(DSTIME_SEC,    ds_time[5]   / 10 * 16 + ds_time[5]   % 10);
    DS1302_writeByte(DSTIME_DAY,    ds_time[6]   / 10 * 16 + ds_time[6]   % 10);
    DS1302_writeByte(DSTIME_WP, 0x80);
}

void DS1302_readTime() {
    BYTE temp;
    temp = DS1302_readByte(DSTIME_YEAR);

    // 这里 / 16 为啥不能用 >> 4?

    // 移位操作一定要加括号，移位操作的优先级要比 + - x / 要低，所以 temp >> 4 * 10 就变成了 temp >> 40 !!!
    ds_time[0] = ( temp >> 4 ) * 10 + temp % 16;

    temp = DS1302_readByte(DSTIME_MONTH);
    ds_time[1] = ( temp >> 4 ) * 10 + temp % 16;

    temp = DS1302_readByte(DSTIME_DATE);
    ds_time[2] = ( temp >> 4 ) * 10 + temp % 16;

    temp = DS1302_readByte(DSTIME_HOUR);
    ds_time[3] = ( temp >> 4 ) * 10 + temp % 16;

    temp = DS1302_readByte(DSTIME_MIN);
    ds_time[4] = ( temp >> 4 ) * 10 + temp % 16;

    temp = DS1302_readByte(DSTIME_SEC);
    ds_time[5] = ( temp >> 4 ) * 10 + temp % 16;

    temp = DS1302_readByte(DSTIME_DAY);
    ds_time[6] = ( temp >> 4 ) * 10 + temp % 16;
}
