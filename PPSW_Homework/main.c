#define NULL '\0'

enum CompResult { DIFFERENT, EQUAL };

enum Result { OK, ERROR };

void CopyString(char pcSource[], char pcDestination[])
{	
	int iCharacterCounter;
	
	for(iCharacterCounter = 0; NULL != pcSource[iCharacterCounter]; iCharacterCounter++)
	{
		pcDestination[iCharacterCounter] = pcSource[iCharacterCounter];
	}
	pcDestination[iCharacterCounter] = NULL;
	//quick but potential buffer overflow
}

enum CompResult eCompareString(char pcStr1[], char pcStr2[])
{
	int iCharacterCounter;
	
	for(iCharacterCounter = 0; NULL != pcStr1[iCharacterCounter] || NULL != pcStr2[iCharacterCounter]; iCharacterCounter++)
	{
		if (pcStr1[iCharacterCounter] != pcStr2[iCharacterCounter])
		{
			return DIFFERENT;
		}
	}
	return EQUAL;
}

void AppendString(char pcSourceStr[],char pcDestinationStr[])
{
	int iPointerPosition;
	
	for(iPointerPosition = 0; NULL != pcDestinationStr[iPointerPosition]; iPointerPosition++) {}
	CopyString(pcSourceStr, pcDestinationStr+iPointerPosition);
}

void ReplaceCharactersInString(char pcString[],char cOldChar,char cNewChar)
{
	int iCharacterCounter;
	
	for(iCharacterCounter = 0; NULL != pcString[iCharacterCounter]; iCharacterCounter++)
	{
		if(pcString[iCharacterCounter] == cOldChar)
		{
			pcString[iCharacterCounter] = cNewChar;
		}
	}
}

void UIntToHexStr (unsigned int uiValue, char pcStr[])
{
	int iHexChar;
	
	pcStr[10] = NULL;
	pcStr[1] = 'x';
	pcStr[0] = '0';
	for(iHexChar = 4; 0 < iHexChar ; iHexChar--)
	{
		pcStr[6 - iHexChar] = ((uiValue & (0xF << ((iHexChar - 1) * 4))) >> ((iHexChar - 1) * 4));
		if(9 >= pcStr[6 - iHexChar])
		{
			 pcStr[6 - iHexChar] = pcStr[6 - iHexChar] + 48;
		}
		else
		{
			pcStr[6 - iHexChar] = pcStr[6 - iHexChar] + 55;
		}
	}
}

enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue)
{
	int iCounter;
	
	if(('0' == pcStr[0]) && ('x' == pcStr[1]) && (NULL != pcStr[2]))
	{
		for(iCounter = 2; pcStr[iCounter]; iCounter++)
		{
			if(('A' <= pcStr[iCounter]) && ('F' >= pcStr[iCounter]))
			{
				*puiValue = (*puiValue * 16) + (pcStr[iCounter] - 55);
			}
			else if(('0' <= pcStr[iCounter]) && ('9' >= pcStr[iCounter]))
			{
				*puiValue = (*puiValue * 16) + (pcStr[iCounter] - 48);
			}
			else
				return ERROR;
		}
		if(iCounter > 6)
		{
			return ERROR;
		}
		else
		{
			return OK;
		}
	}
	else
	{
		return ERROR;
	}
}

void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[])
{
	char cHexStr[7]; 
	UIntToHexStr(uiValue, cHexStr);
	AppendString(cHexStr, pcDestinationStr);
}

int main()
{
	
	char acArreyOne[50] = "Hej";
	char acArreyTwo[4];
	enum CompResult eResoults;
	char pcHex[32];
	enum Result eDoesHex;
	unsigned int iHex;
	

	CopyString(acArreyOne, acArreyTwo);
	eResoults = eCompareString(acArreyOne, acArreyTwo);
	AppendString(acArreyTwo, acArreyOne);
	ReplaceCharactersInString(acArreyOne, 'e', 'w');
	UIntToHexStr(0xFFFF, pcHex);
	eDoesHex = eHexStringToUInt("0xAABCD", &iHex);
	AppendUIntToString(iHex ,acArreyOne);
	
	
	return 0;
}
