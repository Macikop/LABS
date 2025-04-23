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
	enum eLedState {LED_RIGHT, LED_LEFT};
	enum eLedState eLedState = LED_RIGHT;

	unsigned char ucStepCounter = 0;

	LedInit();
	
	while(1)
	{	
		switch (eLedState)
		{
			case LED_RIGHT:
				LedStepLeft();
				ucStepCounter++;
				if (ucStepCounter == 3)
				{
					eLedState = LED_LEFT;
				}
				break;
			
			case LED_LEFT:
				LedStepRight();
				ucStepCounter--;
				if (ucStepCounter == 0)
				{
					eLedState = LED_RIGHT;
				}
				break;
			
		}
		Delay(500);
	}
}
