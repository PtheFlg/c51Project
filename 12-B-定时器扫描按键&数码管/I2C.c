#include <REGX52.H>

#define BYTE unsigned char

sbit I2C_SCL = P2^1;
sbit I2C_SDA = P2^0;

/**
 * SCL 高电平情况下 SDA 变成了低电平，视为 开始
 */
void I2C_start() {
    // 先把 SCL 和 SDA 拉高
    I2C_SCL = 1;
    I2C_SDA = 1;

    // 在 SCL 高电平下 SCL 变为低电平 视为开始传输
    I2C_SDA = 0;

    // 将 SCL 置低
    I2C_SCL = 0;
}

/**
 * SCL 高电平情况下，SDA 跳变回了高电平，视为结束
 */
void I2C_stop() {
    // 将 SCL 和 SDA 置0（SCL 可能用不着，因为 stop 前 SCL 一定是0，每次发送或者接收后都会将 SCL 归 0 ）
    I2C_SCL = 0;
    I2C_SDA = 0;

    // 先将 SCL 拉高，然后在 SCL 高时，拉高 SDA，视为结束
    I2C_SCL = 1;
    I2C_SDA = 1;
}

/**
 * I2C 写字节：
 * SCL 低电平期间，主机先将数据写到 SDA 上，然后将 SCL 拉高，从机检测到 SCL 高，会自动读取 SDA 上的值，
 * 然后主机再拉回 SCL 到低，重复八次
 * 
 * 从高到低位依次写，结束时 SCL = 0， SDA 未知
 */
void I2C_writeByte(BYTE b) {
    BYTE i = 0;
    for(; i < 8; i++) {
        // 0x80 >> i，是从高位到低位
        I2C_SDA = b & (0x80 >> i);
        I2C_SCL = 1;

        // SCL 变化频率最高是 1MHz，也就是一次 1us，咱的 51 单片机最快也就这速度，
        // 所以 SCL 变为 1 后不需要等待从机读取
        I2C_SCL = 0;
    }
}

/**
 * I2C 读字节：
 * SCL 低电平期间，主机先释放 SDA，也就是将 SDA 拉高，从机会自动将要写入的数据的高位，写到 SDA 上
 * 主机拉高 SCL，读取 SDA，然后再拉低 SCL，从机就知道主机读取完成，就会将下一位数据写道 SDA ，
 * 重复八次，读完一个字节
 * 
 * 从高到低依次读，结束时 SCL = 0，SDA 未知 
 */
BYTE I2C_readByte() {
    BYTE rec = 0x00;
    BYTE i;

    I2C_SDA = 1;
    for(i = 0; i < 8; i++) {
        I2C_SCL = 1;
        if(I2C_SDA) {
            rec = rec | (0x80 >> i);
        }
        I2C_SCL = 0;
    }

    return rec;
}

/**
 * I2C 发送应答：
 * 主机接受一个字节后，向从机发送一个应答来表示我收到了，或者不响应表示我收够了or没收到
 * 
 * 和发送字节同理，就是发送字节的 1/8。
 * 在 SCL 低电平期间，先将应答写到 SDA 上，然后拉高 SCL 通知从机读，然后撤回 SCL 到0
 * 
 * 1 表示不应答，0 表示应答
 * 
 * 发送应答指的不是 发送数据需要接收从机的应答，而真的是我要发送这个应答
 */
void I2C_writeAck(BYTE ack) {
    I2C_SDA = ack;
    I2C_SCL = 1;
    I2C_SCL = 0;
}

/**
 * I2C 接收应答：
 * 主机发送一个字节后，接收到来自从机的应答，1 = 不应答，0 = 应答
 * 
 * 同理，SCL 低电平期间，先释放 SDA（SDA拉高），从机就会将应答数据写到 SDA 上，
 * 随后主机拉高 SCL 读取数据，再撤回 SCL
 */
BYTE I2C_readAck() {
    BYTE ack = 0;
    I2C_SDA = 1;
    I2C_SCL = 1;
    ack = I2C_SDA;
    I2C_SCL = 0;

    return ack;
}