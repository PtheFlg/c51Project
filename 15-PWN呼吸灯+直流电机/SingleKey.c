#include <REGX52.H>
#include "Delay.h"
#define BYTE unsigned char

sbit K0=P3^1;
sbit K1=P3^0;
sbit K2=P3^2;
sbit K3=P3^3;

BYTE keyBuf = 0;

BYTE getSingleKey() {
	BYTE t = keyBuf;
	keyBuf = 0;
	return t;
}

BYTE singleKey() {
	BYTE i = 0;
	if(!K0) {
		delay(10);	while(!K0);	delay(10);  i = 1;
	}
	if(!K1) {
		delay(10);	while(!K1);	delay(10);  i = 2;
	}
	if(!K2) {
		delay(10);	while(!K2);	delay(10);  i = 3;
	}
	if(!K3) {
		delay(10);	while(!K3);	delay(10);  i = 4;
	}
	
	return i;
}

BYTE singleKeyScan() {
	BYTE i = 0;
	if(!K0)	i = 1;
	if(!K1)	i = 2;
	if(!K2)	i = 3;
	if(!K3)	i = 4;

	return i;
}

void singleKeyLoop() {
	static BYTE nowState = 0, lastState = 0;
	lastState = nowState;

	nowState = singleKeyScan();

	if(lastState && !nowState) {
		keyBuf = lastState;
	}
}
