#include <LPC21xx.H>
#include "timer.h"

#define TIMER_ENABLE_bm (1<<0)
#define TIMER_RESET_bm (1<<1) 

#define INTERUPT_MR0_bm (1<<0)
#define RESET_MR0_bm (1<<1)

#define INTERUPT_FLAG_MR0_bm (1<<0)

void InitTimer0()
{
	T0TCR = T0TCR | TIMER_ENABLE_bm;
}

void WaitOnTimer0(unsigned int uiTime) // uiTime in us
{	
	T0TCR = T0TCR | TIMER_RESET_bm;
	T0TCR = T0TCR & (~TIMER_RESET_bm);
	
	while(T0TC <= uiTime * 15) {}
}

void InitTimer0Match0(unsigned int uiDelayTime) // uiDelayTime in us
{
	T0MR0 = uiDelayTime * 15;
	T0MCR = INTERUPT_MR0_bm | RESET_MR0_bm;
	
	T0TCR = T0TCR | TIMER_RESET_bm;
	T0TCR = T0TCR & (~TIMER_RESET_bm);
	
	T0TCR = T0TCR | TIMER_ENABLE_bm;
}

void WaitOnTimer0Match0()
{
	while(0 == (T0IR & INTERUPT_FLAG_MR0_bm)) {}
	T0IR = INTERUPT_FLAG_MR0_bm;
}
