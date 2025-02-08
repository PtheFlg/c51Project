
#ifndef __TIMER0_H__
#define __TIMER0_H__

void Timer0_init();

void Timer0_setCounter(unsigned int counter);

void Timer0_run(unsigned char run);

unsigned int Timer0_getCounter();


#endif