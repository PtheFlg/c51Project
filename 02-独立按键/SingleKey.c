#include <REGX52.H>

#define BYTE unsigned char

void SingleKey_delay(int xms)		//@12.000MHz
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

// sbit P3^0 = P3^1;
// sbit P3^1 = P3^0;
// sbit P3^2 = P3^2;
// sbit P3^3 = P3^3;

BYTE singleKey() {
    BYTE i = 0;
    if(P3_1 == 0) {
        SingleKey_delay(20);   while(P3_1 == 0);  SingleKey_delay(20); i = 1;
    }
    if(P3_0 == 0) {
        SingleKey_delay(20);   while(P3_0 == 0);  SingleKey_delay(20); i = 2;
    }
    if(P3_2 == 0) {
        SingleKey_delay(20);   while(P3_2 == 0);  SingleKey_delay(20); i = 3;
    }
    if(P3_3 == 0) {
        SingleKey_delay(20);   while(P3_3 == 0);  SingleKey_delay(20); i = 4;
    }

    return i;
}