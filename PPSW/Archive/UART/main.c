#include "uart.h"
#include "led.h"
#include "keyboard.h"
#include "servo.h"

extern char cOdebranyZnak;

int main ()
{	
	unsigned char ucSteppCounter = 0;
	UART_InitWithInt(9600);
	KeyboardInit();
	ServoInit(50);
	
	while(1) 
	{		
		switch (cOdebranyZnak)
		{
			case '1':
				ucSteppCounter = (ucSteppCounter + 1) % 4;
				ServoGoTo(12 * ucSteppCounter);
				break;
			case '2':
				ucSteppCounter = 2;
				ServoGoTo(24);
				break;
			case '3':
				ucSteppCounter = 3;
				ServoGoTo(36);
				break;
			case 'c':
				ucSteppCounter = 0;
				ServoCallib();
				break;
		}
		
		if (cOdebranyZnak != '\0')
		{
			cOdebranyZnak = '\0';
		}
		
		switch(eKeyboardRead())
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
