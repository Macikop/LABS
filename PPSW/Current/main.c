#include <LPC21xx.H>

#include "led.h"
#include "keyboard.h"

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
	KeyboardInit();
	
	while(1)
	{	
		switch(eKeyboardRead())
		{
			case BUTTON_1:
				LedStepRight();
				Delay(200);
				break;
			case BUTTON_2:
				LedStepLeft();
				Delay(200);
				break;
			default:
				break;
		}
	}
}
