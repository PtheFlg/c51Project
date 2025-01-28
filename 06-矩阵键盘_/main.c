#include <REGX52.H>
#include "LCD1602.h"
#include "MatrixKey.h"

#define uchar unsigned char

unsigned int password = 0;
unsigned int corPas = 1234;

uchar passLen = 0;

uchar isLogin = 0;

uchar changeMode = 0;

void delay(unsigned int xms)		//@12.000MHz
{
	uchar i, j;

	do{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} 	while (--i);
	}while( --xms);
}

void showPassword(){
    LCD_ShowNum(2, 1, password, 4);
}

void addPassword(uchar i) {
    if(passLen++ < 4)
        password = password * 10 + i;
    showPassword();
}

void main () {

    uchar keyNum = 0;

    LCD_Init();
    LCD_ShowString(1, 1, "PASSWORD: ");
    showPassword();

    while(1) {

        keyNum = getKeyNum();

        if(keyNum == 0) continue;

        switch(keyNum) {
            case 1:
            case 2:
            case 3:
                addPassword(keyNum);
                break;
            case 4:
                password = password / 10;
                passLen --;
                showPassword();
                break;
            case 5:
            case 6:
            case 7:
                addPassword(keyNum - 1);
                break;
            case 8:

                // 登陆情况下 按下 8，打开修改密码模式，密码情况重置（密码 密码长度）
                // 修改模式 变 1
                if(isLogin) {
                    password = 0;
                    passLen = 0;
                    changeMode = 1;
                    LCD_ShowString(1, 11, "CHANGE");
                    showPassword();
                } 
                break;
            case 9:
            case 10:
            case 11:
                addPassword(keyNum - 2);
                break;
            case 12: break;
            case 13: break;
            case 14:
                addPassword(0);
                break;
            case 15: break;
            case 16:

                // 不是改密码模式，对比密码和正确密码，并修改登录标志
                if(changeMode == 0) {
                    if(password == corPas) {
                        LCD_ShowString(1, 11, "    OK");
                        isLogin = 1;
                    } else {
                        LCD_ShowString(1, 11, "   ERR");
                        password = 0;
                        passLen = 0;
                        isLogin = 0;
                        showPassword();
                    }
                } 
                // 是改密码模式，如果密码长度 == 4，覆写 corPas 正确密码
                // 并修改 登陆标志，修改模式，初始化密码情况（密码，密码长度）
                else {
                    if(passLen == 4) {
                        corPas = password;
                        password = 0;
                        changeMode = 0;
                        isLogin = 0;
                        passLen = 0;
                        LCD_ShowString(1, 11, "   COK");
                        showPassword();
                    }
                }
                
        }

        // LCD_ShowString(1, 1, "key:");
        // LCD_ShowNum(1, 5, keyNum, 2);

    }
}