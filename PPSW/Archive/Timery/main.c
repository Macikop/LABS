#include "led.h"
#include "timer.h"

#define MILISECOND 11996

void Delay(int iTime)
{
	unsigned int uiCycle;
	unsigned int uiLoopCycles;
	uiLoopCycles = MILISECOND * iTime;
	for (uiCycle = 0; uiCycle < uiLoopCycles; uiCycle++) {}
}

int main()
{
	LedInit();
	//InitTimer0();
	InitTimer0Match0(250000);
	
	while(1)
	{	
		WaitOnTimer0Match0();
		LedStepRight();
	}
	
	/*
	while(1)
	{	
		WaitOnTimer0(250000);
		LedStepRight();
	}
	*/
}
