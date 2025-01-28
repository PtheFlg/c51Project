#include <REGX52.H>

#define uchar unsigned char

unsigned char getKeyNum()
{
    uchar p1Row = 0x80; // 1000 0000
    uchar row = 0;

    uchar col = 0;

    uchar i = 4;

    while(i--) {

        P1 = ~p1Row;

        if (P1_3 == 0) {
            col = 1;
            while (P1_3 == 0);
        }
        if (P1_2 == 0) {
            col = 2;
            while (P1_2 == 0);
        }
        if (P1_1 == 0) {
            col = 3;
            while (P1_1 == 0);
        }
        if (P1_0 == 0){
            col = 4;
            while (P1_0 == 0);
        }

        if(col) {
            return row * 4 + col;
        }

        p1Row = p1Row >> 1;
        row = (row + 1) % 4;
    }

    

    return 0;
}