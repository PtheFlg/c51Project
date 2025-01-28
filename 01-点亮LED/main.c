#include <REGX52.H>
#include <INTRINS.H>

void Delay500ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 4;
	j = 205;
	k = 187;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void delay(int xms)		//@12.000MHz
{
	unsigned char i, j;

	do{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} 	while (--i);
	}while( --xms);
}


void main()
{

	/**
	 * 1. 接通单个 LED
	 * 
	 * 单片机中间核心有 40 个 IO引脚，每八个引脚为一组
	 * 每组又被包含在一个寄存器内
	 * 
	 * 51单片机通过寄存器来控制引脚，通过给寄存器里面填入 1，则引脚给高电压 5V
	 * 寄存器填入 0 ，引脚给 0V，相当于负极 0V，正极 VCC 5V，就能接通
	 * 
	 * 控制 LED 的引脚是 P20 - P27，这八个引脚包括在 P2 寄存器内，所以给 P2 寄存器赋值 1111 1110 就可以接通一个 LED
	 */
	// P2 = 0xFE; // 1111 1110
	
	// while(1){
		
	// }

	/**
	 * 
	 */

	// while(1){

	// 	P2 = 0xFE;
	// 	Delay500ms();

	// 	P2 = 0xFF;
	// 	Delay500ms();
	// }
	
	
	/**
	 * 
	 */

	unsigned char i = 0;
	while(1){
		P2 = 0xFE;
		
		for(; i < 8; i++){
			//Delay500ms();
			
			delay(200);
			P2 = (P2 << 1) + 1;
		}
		
		i = 0;
		delay(200);

	}

}
	