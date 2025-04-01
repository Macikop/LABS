#include <LPC21xx.H>

#define MAX_TOKEN_NR 3 							//maksymalna dopuszczalna ilosc tokenów
#define MAX_KEYWORD_STRING_LTH 10 	// maksymalna dlugosc komendy
#define MAX_KEYWORD_NR 3

enum CompResult { DIFFERENT, EQUAL };
enum Result { OK, ERROR };
enum KeywordCode { LD, ST, RST };
enum TokenType { KEYWORD, NUMBER, STRING};

unsigned char ucTokenNr;		//liczba tokenów w zdekodowanym komunikacie

union TokenValue
{
	enum KeywordCode eKeyword; 	// jezeli KEYWORD
	unsigned int uiNumber; 			// jezeli NUMBER
	char * pcString; 						// jezeli STRING
};

struct Token
{
	enum TokenType eType; 			// KEYWORD, NUMBER, STRING
	union TokenValue uValue;		// enum, unsigned int, char*
};

struct Token asToken[MAX_TOKEN_NR];							// wypelniana przez DecodeMsg na podstawie cUartRxBuffer i asCommandList

struct Keyword
{
	enum KeywordCode eCode;
	char cString[MAX_KEYWORD_STRING_LTH + 1];
};

struct Keyword asKeywordList[MAX_KEYWORD_NR]=		// uzywana przez bStringToCommand
{
	{RST,"reset"},
	{LD, "load" },
	{ST, "store"}
};

void CopyString(char pcSource[], char pcDestination[])
{	
	unsigned int uiCharacterCounter;
	
	for(uiCharacterCounter = 0; '\0' != pcSource[uiCharacterCounter]; uiCharacterCounter++)
	{
		pcDestination[uiCharacterCounter] = pcSource[uiCharacterCounter];
	}
	pcDestination[uiCharacterCounter] = '\0';
}

enum CompResult eCompareString(char pcStr1[], char pcStr2[])
{
	unsigned int uiCharacterCounter;
	
	for(uiCharacterCounter = 0; '\0' != pcStr1[uiCharacterCounter] || '\0' != pcStr2[uiCharacterCounter]; uiCharacterCounter++)
	{
		if (pcStr1[uiCharacterCounter] != pcStr2[uiCharacterCounter])
		{
			return DIFFERENT;
		}
	}
	return EQUAL;
}

void AppendString(char pcSourceStr[], char pcDestinationStr[])
{
	unsigned int uiPointerPosition;
	
	for(uiPointerPosition = 0; '\0' != pcDestinationStr[uiPointerPosition]; uiPointerPosition++) {}
	CopyString(pcSourceStr, pcDestinationStr+uiPointerPosition);
}

void ReplaceCharactersInString(char pcString[], char cOldChar, char cNewChar)
{
	unsigned int uiCharacterCounter;
	
	for(uiCharacterCounter = 0; '\0' != pcString[uiCharacterCounter]; uiCharacterCounter++)
	{
		if(pcString[uiCharacterCounter] == cOldChar)
		{
			pcString[uiCharacterCounter] = cNewChar;
		}
	}
}

void UIntToHexStr (unsigned int uiValue, char pcStr[])
{
	int iHexChar;
	
	pcStr[10] = '\0';
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
	
	if(('0' == pcStr[0]) && ('x' == pcStr[1]) && ('\0' != pcStr[2]))
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

unsigned char ucFindTokensInString(char *pcString)
{
	unsigned char ucCharacterCounter;
	unsigned char ucTokenCounter = 0;
	unsigned char ucDelimiterPosition = 0;
	
	for(ucCharacterCounter = 1 ;; ucCharacterCounter++)
	{
		switch(pcString[ucCharacterCounter - 1])
		{
			case '\0':
				if (1 < (ucCharacterCounter - ucDelimiterPosition))
				{
					asToken[ucTokenCounter].uValue.pcString = pcString + ucDelimiterPosition;
				}
				return ucTokenCounter;
			case 0x20:
				if (1 < (ucCharacterCounter - ucDelimiterPosition))
				{
					asToken[ucTokenCounter].uValue.pcString = pcString + ucDelimiterPosition;
					ucTokenCounter++;
				}
				ucDelimiterPosition = ucCharacterCounter;
				break;
			default:
			{
				if (MAX_TOKEN_NR <= ucTokenCounter)
				{
					return ucTokenCounter;
				}
				break;
			}
		}
	}
}

enum Result eSringToKeyword (char pcStr[], enum KeywordCode *peKeywordCode)
{
	unsigned char ucKeyordIterator = 0;

	for(ucKeyordIterator = 0; MAX_KEYWORD_NR > ucKeyordIterator; ucKeyordIterator++)
	{
		if(EQUAL == eCompareString(pcStr, asKeywordList[ucKeyordIterator].cString))
		{
			*peKeywordCode = asKeywordList[ucKeyordIterator].eCode;
			return OK;
		}
	}
	return ERROR;
}

void DecodeTokens()
{
	unsigned char ucTokenNr;
	enum KeywordCode eDecodedCode;
	unsigned int uiDecodedNumber;
	struct Token *spCurrentToken;
	
	for(ucTokenNr = 0; MAX_TOKEN_NR > ucTokenNr; ucTokenNr++)
	{
		spCurrentToken = &asToken[ucTokenNr];
		if(OK == eSringToKeyword(spCurrentToken->uValue.pcString, &eDecodedCode))
		{
			spCurrentToken->eType = KEYWORD;
			spCurrentToken->uValue.eKeyword = eDecodedCode;
		}
		else if (OK == eHexStringToUInt(spCurrentToken->uValue.pcString, &uiDecodedNumber))
		{
			spCurrentToken->eType = NUMBER;
			spCurrentToken->uValue.uiNumber = uiDecodedNumber;
		}
		else
		{
			spCurrentToken->eType = STRING;
		}
	}
}

void DecodeMsg(char *pcString)
{
	ucFindTokensInString(pcString);
	ReplaceCharactersInString(pcString, ' ', '\0');
	DecodeTokens();
}

char acArreyOne[] = "Hej ale ja oo ro te";
char acArreyTwo[4];
enum CompResult eResoults;
char pcHex[32];
enum Result eDoesHex;
unsigned int iHex;

int main()
{
	//CopyString(acArreyOne, acArreyTwo);
	/*
	eResoults = eCompareString(acArreyOne, acArreyTwo);
	if(eResoults == EQUAL)
	{
		AppendString(acArreyTwo, acArreyOne);
	}
	*/
	/*ReplaceCharactersInString(acArreyOne, 'e', 'w');
	UIntToHexStr(0xFFFF, pcHex);
	eDoesHex = eHexStringToUInt("0xAABCD", &iHex);
	AppendUIntToString(iHex ,acArreyOne);
	*/
	
	//ucFindTokensInString(acArreyOne);
	
	DecodeMsg("store 0x001c spryciarz");
	
	return 0;
}
