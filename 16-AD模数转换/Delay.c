#define BYTE unsigned char

void delay(unsigned int xms)		//@12.000MHz
{
	BYTE i, j;
	do{
		i = 2;
		j = 239;
		do{
			while (--j);
		}while (--i);
	}while( --xms);
}