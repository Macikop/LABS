#include <LPC21xx.H>
#include "timer.h"

#define TIMER_ENABLE (1<<0)
#define TIMER_RESET (1<<1) 

#define INTERUPT_MR0 (1<<0)
#define RESET_MR0 (1<<1)

#define INTERUPT_FLAG_MR0 (1<<0)

void InitTimer0()
{
	T0TCR = T0TCR | TIMER_ENABLE;
}

void WaitOnTimer0(unsigned int uiTime) // uiTime in us
{	
	T0TCR = T0TCR | TIMER_RESET;
	T0TCR = T0TCR & (~TIMER_RESET);
	
	while(T0TC <= uiTime * 15) {}
}

void InitTimer0Match0(unsigned int uiDelayTime)
{
	T0MR0 = uiDelayTime * 15;
	T0MCR = INTERUPT_MR0 | RESET_MR0;
	
	T0TCR = T0TCR | TIMER_RESET;
	T0TCR = T0TCR & (~TIMER_RESET);
	
	T0TCR = T0TCR | TIMER_ENABLE;
}

void WaitOnTimer0Match0()
{
	while(0 == (T0IR & INTERUPT_FLAG_MR0)) {}
	T0IR = INTERUPT_FLAG_MR0;
}
