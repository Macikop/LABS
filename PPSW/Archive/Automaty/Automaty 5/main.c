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
	enum eLedState eLedState = LED_RIGHT;
	enum KeyboardState eKeyboardState;

	LedInit();
	KeyboardInit();
	
	while(1)
	{	
		eKeyboardState = eKeyboardRead();
		switch(eLedState)
		{
			case LED_STOP:
				if (BUTTON_1 == eKeyboardState)
				{
					eLedState = LED_RIGHT;
				}
				else
				{
					eLedState = LED_STOP;
				}
				break;
			case LED_RIGHT:
				if (BUTTON_0 == eKeyboardState)
				{
					eLedState = LED_STOP;
				}
				else
				{
					LedStepRight();
					eLedState = LED_RIGHT;
				}
				break;
		}
		Delay(100);
	}
}
