#include <LPC21xx.H> 

#define MILISECOND 11996
#define LED0_bm 0x00010000
#define LED1_bm 0x00020000
#define LED2_bm 0x00040000
#define LED3_bm 0x00080000
#define S0_bm 0x10
#define S1_bm 0x40
#define S2_bm 0x20
#define S3_bm 0x80

enum KeyboardState {RELASED, BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3};

enum eStepDirection {LEFT, RIGHT};

void Delay(int iTime)
{
	unsigned int uiCycle;
	unsigned int uiLoopCycles;
	uiLoopCycles = MILISECOND * iTime;
	for (uiCycle = 0; uiCycle < uiLoopCycles; uiCycle++) {}
}

void LedInit()
{
	IO1DIR = IO1DIR | LED0_bm | LED1_bm | LED2_bm | LED3_bm;
	IO1CLR = LED0_bm | LED1_bm | LED2_bm | LED3_bm;
	IO1SET = LED0_bm;
}

void KeyboardInit()
{
	IO0DIR = IO0DIR & (~S0_bm) & (~S1_bm) & (~S2_bm) & (~S3_bm);
}

void LedOn(unsigned char ucLedIndeks)
{
	IO1CLR = LED0_bm | LED1_bm | LED2_bm | LED3_bm;
	
	switch (ucLedIndeks)
	{
		case 0:
			IO1SET = LED0_bm;
			break;
		case 1:
			IO1SET = LED1_bm;
			break;
		case 2:
			IO1SET = LED2_bm;
			break;
		case 3:
			IO1SET = LED3_bm;
			break;
	}
}

enum KeyboardState eKeyboardRead()
{
	if ((~IO0PIN) & (S0_bm))
	{
		return BUTTON_0;
	}
	else if((~IO0PIN) & (S1_bm))
	{
		return BUTTON_1;
	}
	else if((~IO0PIN) & (S2_bm))
	{
		return BUTTON_2;
	}
	else if ((~IO0PIN) & (S3_bm))
	{
		return BUTTON_3;
	}
	else
	{
		return RELASED;
	}
}

void LedStep (enum eStepDirection eDirection)
{
	static unsigned int uiLedPosition = 0;
	
	if (LEFT == eDirection)
	{
		uiLedPosition = (uiLedPosition + 1) % 4;
		LedOn(uiLedPosition);
	}
	else if (RIGHT == eDirection)
	{
		uiLedPosition = (uiLedPosition - 1) % 4;
		LedOn(uiLedPosition);
	}
}

void  LedStepLeft() 
{
	LedStep(LEFT);
}

void LedStepRight()
{
	LedStep(RIGHT);
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
