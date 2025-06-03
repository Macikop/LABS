#include "uart.h"
#include "servo.h"
#include "dekodowanie.h"

char pcRecivedString[RECIEVER_SIZE];

int main ()
{	
	UART_InitWithInt(9600);
	ServoInit(50);
		
	while(1)
	{
		if (READY == eReciever_GetStatus())
		{
			Reciever_GetStringCopy(pcRecivedString);
			DecodeMsg(pcRecivedString);
			
			if((0 != ucTokenNr) && (KEYWORD == asToken[0].eType))
			{
				switch (asToken[0].uValue.eKeyword)
				{
					case CAL:
						ServoCallib();
						break;
					case GT:
						if(NUMBER == asToken[1].eType)
						{
							ServoGoTo(asToken[1].uValue.uiNumber);
						}
						break;
					default:
						break;
				}
			}			
		}
	}
}
