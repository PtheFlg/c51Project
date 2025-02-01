#include <REGX52.H>
#include "OneWire.h"
#define BYTE unsigned char

#define DS18B20_SKIP_ROM    0xCC
#define DS18B20_CONVERT_T   0x44
#define DS18B20_READ_SCRAT  0xBE


/**
 * 向 DS18B20 芯片发送 跳过 ROM 转换温度 命令
 */
void DS18B20_convertTemperature() {
    EA = 0;
    OneWire_init();
    OneWire_writeByte(DS18B20_SKIP_ROM);
    OneWire_writeByte(DS18B20_CONVERT_T);
    EA = 1;
}

/**
 * 读取温度：发送跳过 ROM 读取寄存器命令
 * 
 * 寄存器中温度的两个字节使用补码存的，s s s s s 6 5 4, 3 2 1 0 -1 -2 -3 -4（2的几次方）
 * c 语言对于负数就会用补码存储，所以拿到一个补码机器数，只需要给他强制类型转换一下，转成 int
 * 他就可以识别了。
 * 
 * 但是原数里卖弄有4位小数，int 直接给这 4 位解释成整数了，所有需要 >> 4，但这样又会让小数部分丢失
 * 所以就 / 16.0
 */
float DS18B20_readTemperature() {

    BYTE tlsb = 0x00;
    BYTE tmsb = 0x00;

    int t;

    EA = 0;
    OneWire_init();
    OneWire_writeByte(DS18B20_SKIP_ROM);
    OneWire_writeByte(DS18B20_READ_SCRAT);

    tlsb = OneWire_readByte();  // 低 8 bit
    tmsb = OneWire_readByte();  // 高 8 bit
    EA = 1;

    t = (tmsb << 8) | tlsb;

    return t / 16.0;
}
