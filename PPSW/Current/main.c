#include <LPC21xx.H> 

#define MILISECOND 11996
#define LED0_bm 0x00010000
#define LED1_bm 0x00020000
#define LED2_bm 0x00040000
#define LED3_bm 0x00080000

//7499998 ~ 1s bez int iLoopCycles
//11996 ~ 1ms z int iLoopCycles



void Delay(int iTime)
{
	unsigned int uiCycle;
	unsigned int uiLoopCycles;
	uiLoopCycles = MILISECOND * iTime;
	for (uiCycle = 0; uiCycle < uiLoopCycles; uiCycle++)
	{
	}
}

int main()
{
	IO1DIR = IO1DIR | LED0_bm | LED1_bm | LED2_bm | LED3_bm;
	
	/*
	IO1SET = LED3_bm;
	IO1CLR = LED3_bm;
	*/
	while(1)
	{
		/*
		IO1SET = LED3_bm;
		Delay(50);
	
		IO1CLR = LED3_bm;
		Delay(50);
		*/
		IO1SET = LED0_bm;
		IO1SET = LED1_bm;
		IO1SET = LED2_bm;
		IO1SET = LED3_bm;

	}
}
