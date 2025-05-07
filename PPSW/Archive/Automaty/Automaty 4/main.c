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
	enum eLedState {LED_STOP, LED_RIGHT};
	enum eLedState eLedState = LED_STOP;

	unsigned char ucStepCounter = 0;

	LedInit();
	KeyboardInit();
	
	while(1)
	{	
		switch(eLedState)
		{
			case LED_STOP:
				if(BUTTON_0 == eKeyboardRead())
				{
					eLedState = LED_RIGHT;
				}
				else
				{
					eLedState = LED_STOP;
				}
				break;
			case LED_RIGHT:
				if(3 == ucStepCounter)
				{
					eLedState = LED_STOP;
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
