#include "servo.h"
#include "adc.h"

int main ()
{		
	ADC_Init();
	ServoInit(100);
	
	while(1)
	{	

		if (DONE == Check_ADC_Status())
		{
			ServoGoTo(uiScaleValue(uiADC_GetValue(), 0, 1023, 0, 48));
		}
	}
}
