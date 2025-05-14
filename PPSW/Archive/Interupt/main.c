#include "led.h"
#include "keyboard.h"
#include "timer_interrupts.h"

void Automat()
{
	enum eLedState {LED_STOP, LED_LEFT, LED_RIGHT};
	static enum eLedState eLedState = LED_STOP;
	enum KeyboardState eKeyboardState;

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
	}

}

int main ()
{
	LedInit();
	KeyboardInit();
	
	Timer0Interrupts_Init(100000, &Automat);
	while(1){}
}
