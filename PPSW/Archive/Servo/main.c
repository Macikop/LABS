#include "keyboard.h"
#include "servo.h"

int main ()
{
	enum KeyboardState eKeyboard;
	
	KeyboardInit();
	ServoInit(50);
	
	ServoGoTo(24);
	ServoGoTo(12);
	
	while(1)
	{
		eKeyboard = eKeyboardRead();
		switch(eKeyboard)
		{
			case BUTTON_0:
				ServoCallib();
				break;
			case BUTTON_1:
				ServoGoTo(12);
				break;
			case BUTTON_2:
				ServoGoTo(24);
				break;
			case BUTTON_3:
				ServoGoTo(36);
				break;
			default:
				break;
		}
	}
}
