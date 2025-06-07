#include "uart.h"
#include "timer_interrupts.h"
#include "konwersje.h"
#include "stringi.h"
#include "dekodowanie.h"

char pcRecivedString[RECIEVER_SIZE];

struct Watch { 
	unsigned char ucMinutes, ucSecconds; 
	unsigned char fSeccondsValueChanged, fMinutesValueChanged;
};

struct Watch sWatch;

void WatchUpdate()
{
	sWatch.ucSecconds++;
	sWatch.fSeccondsValueChanged = 1;
	
	if(60 == sWatch.ucSecconds)
	{
		sWatch.ucSecconds = 0;
		sWatch.ucMinutes++;
		sWatch.fMinutesValueChanged = 1;
	}
}

unsigned char fCalcValueChanged = 0;
unsigned int uiRecivedNumber = 0;

unsigned char fMessageReady = 0;


int main ()
{	
	sWatch.ucMinutes = 0;
	sWatch.ucSecconds = 0;
	sWatch.fSeccondsValueChanged = 0;
	sWatch.fMinutesValueChanged = 0;
	
	Timer0Interrupts_Init(1000000, WatchUpdate);
	UART_InitWithInt(115200);
	
	while(1)
	{
		char pcMessage[TRANSMITER_SIZE] = "";
		char pcSec[25] = "sec ";
		char pcMin[25] = "min ";
		char pcCalc[25] = "calc ";
		
		if (READY == eReciever_GetStatus())
		{
			Reciever_GetStringCopy(pcRecivedString);
			DecodeMsg(pcRecivedString);
			
			if((0 != ucTokenNr) && (KEYWORD == asToken[0].eType))
			{
				switch(asToken[0].uValue.eKeyword)
				{
					case CALC:
						if(NUMBER == asToken[1].eType)
						{
							uiRecivedNumber = asToken[1].uValue.uiNumber;
							fCalcValueChanged = 1;
						}
						break;
					case ID:
							CopyString("AGH MTM SERVO", pcMessage);
							fMessageReady = 1;
						break;
					default:
						break;
				}
			}
			else
			{
				CopyString("unknown command", pcMessage);
				fMessageReady = 1;
			}
		}
		
		if(FREE == Transmiter_GetStatus())
		{
			if(1 == fMessageReady)
			{
				AppendString("\n", pcMessage);
				Transmiter_SendString(pcMessage);
				fMessageReady = 0;
			}
			else if(1 == fCalcValueChanged)
			{
				AppendUIntToString(2 * uiRecivedNumber , pcCalc);
				AppendString("\n", pcCalc);
				Transmiter_SendString(pcCalc);
				fCalcValueChanged = 0;
			}
			else if(1 == sWatch.fMinutesValueChanged)
			{				
				AppendUIntToString(sWatch.ucMinutes, pcMin);
				sWatch.fMinutesValueChanged = 0;
				
				AppendString("\n", pcMin);
				
				Transmiter_SendString(pcMin);
			}
			else if(1 == sWatch.fSeccondsValueChanged)
			{
				AppendUIntToString(sWatch.ucSecconds, pcSec);
				sWatch.fSeccondsValueChanged = 0;
				AppendString("\n", pcSec);
				Transmiter_SendString(pcSec);
			}
		}
	}
}
// id, unnown command