#include <LPC21xx.H>


//7499998 ~ 1s bez int iterations
//11996 ~ 1ms z int iterations

void Delay(int time)
{
	int i;
	int iterations;
	iterations = 11996 * time;
	for (i = 0; i < iterations; i++)
	{
	}
}

int main()
{
	IO1DIR = 0x00010000;
	
	IO1SET = 0x00010000;
	
	IO1SET = 0x00000000;
	IO1CLR = 0x00010000;
	
	while(1)
	{
		IO1CLR = 0x00000000;
		IO1SET = 0x00010000;
		Delay(1000);
	
		IO1SET = 0x00000000;
		IO1CLR = 0x00010000;
		Delay(1000);
	}
}
