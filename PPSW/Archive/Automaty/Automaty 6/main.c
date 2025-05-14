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
	enum eLedState {LED_STOP, LED_LEFT, LED_RIGHT, LED_WIPER};
	enum eLedState eLedState = LED_STOP;
	enum KeyboardState eKeyboardState;

	unsigned char ucStepCounter = 0;

	LedInit();
	KeyboardInit();
	
	while(1)
	{	
		eKeyboardState = eKeyboardRead();
		switch(eLedState)
		{
			case LED_STOP:
				if (BUTTON_0 == eKeyboardState)
				{
					eLedState = LED_LEFT;
				}
				else if (BUTTON_2 == eKeyboardState)
				{
					eLedState = LED_RIGHT;
				}
				else
				{
					eLedState = LED_STOP;
				}
				break;
			case LED_LEFT:
				if (BUTTON_1 == eKeyboardState)
				{
					eLedState = LED_STOP;
				}
				else if(BUTTON_3 == eKeyboardState)
				{
					ucStepCounter = 0;
					eLedState = LED_WIPER;
				}
				else
				{
					eLedState = LED_LEFT;
					LedStepLeft();
				}
				break;
			case LED_RIGHT:
				if (BUTTON_1 == eKeyboardState)
				{
					eLedState = LED_STOP;
				}
				else
				{
					eLedState = LED_RIGHT;
					LedStepRight();
				}
				break;
			case LED_WIPER:
				if (5 == ucStepCounter)
				{
					eLedState = LED_RIGHT;
				}
				else 
				{
					if (1 == (ucStepCounter % 2))
					{
						LedStepRight();
					}
					else
					{
						LedStepLeft();
					}
					ucStepCounter++;
					eLedState = LED_WIPER;
				}
				break;
		}
		Delay(100);
	}
}
