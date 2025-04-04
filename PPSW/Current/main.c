#include <LPC21xx.H> 

#define MILISECOND 11996
#define LED0_bm 0x00010000
#define LED1_bm 0x00020000
#define LED2_bm 0x00040000
#define LED3_bm 0x00080000
#define S1_bm 0x10
#define S2_bm 0x20
#define S3_bm 0x40

enum KeyboardState {RELASED, BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3};

//7499998 ~ 1s bez int iLoopCycles
//11996 ~ 1ms z int iLoopCycles
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
	IO1SET = LED0_bm;
}

void KeyboardInit()
{
	IO0DIR = IO1DIR | S1_bm | S2_bm | S3_bm;
}

void LedOn(unsigned char ucLedIndeks)
{
	IO1CLR = 0xF0000;
	IO1SET = (1 << (ucLedIndeks + 16));
}

enum KeyboardState eKeyboardRead()
{
	switch (IO0PIN & S1_bm)
	{
		case 0x10:
		{
			return BUTTON_0;
		}
		case 0x20:
		{
			return BUTTON_1;
		}
		case 0x40:
		{
			return BUTTON_2;
		}
		case 0x80:
		{
			return BUTTON_3;
		}
		default:
		{
			return RELASED;
		}
	}
}

int main()
{
	LedInit();
	KeyboardInit();
	
	while(1)
	{	
		unsigned char ucLedPosition;
		LedOn(eKeyboardRead());
		/*
		IO1SET = LED0_bm;
		Delay(250);
		IO1CLR = LED0_bm;
		IO1SET = LED1_bm;
		Delay(250);
		IO1CLR = LED1_bm;
		IO1SET = LED2_bm;
		Delay(250);
		IO1CLR = LED2_bm;
		IO1SET = LED3_bm;
		Delay(250);
		IO1CLR = LED3_bm;
		*/
		/*
		for (ucLedPosition = 0; 4 > ucLedPosition; ucLedPosition++)
		{
			LedOn(ucLedPosition);
			Delay(250);
		}*/
	}
}
