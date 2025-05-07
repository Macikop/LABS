#include "led.h"

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
	enum eLedState {LED_LEFT, LED_RIGHT};
	enum eLedState eLedState = LED_LEFT;

	unsigned char ucStepCounter = 0;

	LedInit();
	
	while(1)
	{	
		switch(eLedState)
		{
			case LED_LEFT:
				if(3 == ucStepCounter)
				{
					eLedState = LED_RIGHT;
					ucStepCounter = 0;
				}
				else
				{
					LedStepLeft();
					ucStepCounter++;
					eLedState = LED_LEFT;
				}
				break;
			case LED_RIGHT:
				if(3 == ucStepCounter)
				{
					eLedState = LED_LEFT;
					ucStepCounter = 0;
				}
				else
				{
					LedStepRight();	
					ucStepCounter++;
					eLedState = LED_RIGHT;
				}
				break;
		}
		Delay(100);
	}
}
