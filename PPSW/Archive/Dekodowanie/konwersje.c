#include "konwersje.h"

void UIntToHexStr(unsigned int uiValue, char pcStr[])
{
	unsigned char ucNibbleCounter;
	unsigned char ucCurrentNibble;
	
	pcStr[0] = '0';
	pcStr[1] = 'x';
	pcStr[6] = '\0';
	
	for(ucNibbleCounter = 0; 4 > ucNibbleCounter ; ucNibbleCounter++)
	{
		ucCurrentNibble = ((uiValue >> ((ucNibbleCounter) * 4)) & 0xF);
		
		if(10 > ucCurrentNibble)
		{
			 pcStr[5 - ucNibbleCounter] = ucCurrentNibble + '0';
		}
		else
		{
			pcStr[5 - ucNibbleCounter] = ucCurrentNibble - 10 + 'A';
		}
	}
}

enum Result eHexStringToUInt(char pcStr[], unsigned int *puiValue)
{
	unsigned char ucCharacterCounter;
	char cCurrentCharacter;
	
	*puiValue = 0;
	
	if(('0' == pcStr[0]) && ('x' == pcStr[1]) && ('\0' != pcStr[2]))
	{
		for(ucCharacterCounter = 2; '\0' != pcStr[ucCharacterCounter]; ucCharacterCounter++)
		{
			cCurrentCharacter =  pcStr[ucCharacterCounter];
			if(6 == ucCharacterCounter)
			{
				return ERROR;
			}
			else if(('A' <= cCurrentCharacter) && ('F' >= cCurrentCharacter))
			{
				*puiValue = (*puiValue << 4) + (cCurrentCharacter - 'A' + 10);
			}
			else if(('0' <= cCurrentCharacter) && ('9' >= cCurrentCharacter))
			{
				*puiValue = (*puiValue << 4) + (cCurrentCharacter - '0');
			}
			else
			{
				return ERROR;
			}
		}
		return OK;
	}
	else
	{
		return ERROR;
	}
}

void AppendUIntToString(unsigned int uiValue, char pcDestinationStr[])
{
	unsigned char ucCharacterCounter;
	
	for(ucCharacterCounter = 0; '\0' != pcDestinationStr[ucCharacterCounter]; ucCharacterCounter++) {}
	UIntToHexStr(uiValue, pcDestinationStr + ucCharacterCounter);
}
