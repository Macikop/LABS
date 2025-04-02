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
	unsigned char ucCharacterCounter;
	
	for(ucCharacterCounter = 0; '\0' != pcSource[ucCharacterCounter]; ucCharacterCounter++)
	{
		pcDestination[ucCharacterCounter] = pcSource[ucCharacterCounter];
	}
	pcDestination[ucCharacterCounter] = '\0';
}

enum CompResult eCompareString(char pcStr1[], char pcStr2[])
{
	unsigned char ucCharacterCounter;
	
	for(ucCharacterCounter = 0; ('\0' != pcStr1[ucCharacterCounter]) || ('\0' != pcStr2[ucCharacterCounter]); ucCharacterCounter++)
	{
		if (pcStr1[ucCharacterCounter] != pcStr2[ucCharacterCounter])
		{
			return DIFFERENT;
		}
	}
	return EQUAL;
}

void AppendString(char pcSourceStr[], char pcDestinationStr[])
{
	unsigned char ucPointerPosition;
	
	for(ucPointerPosition = 0; '\0' != pcDestinationStr[ucPointerPosition]; ucPointerPosition++) {}
	CopyString(pcSourceStr, pcDestinationStr + ucPointerPosition);
}

void ReplaceCharactersInString(char pcString[], char cOldChar, char cNewChar)
{
	unsigned char ucCharacterCounter;
	
	for(ucCharacterCounter = 0; '\0' != pcString[ucCharacterCounter]; ucCharacterCounter++)
	{
		if(pcString[ucCharacterCounter] == cOldChar)
		{
			pcString[ucCharacterCounter] = cNewChar;
		}
	}
}

void UIntToHexStr (unsigned int uiValue, char pcStr[])
{
	unsigned char ucHexCounter;
	
	pcStr[10] = '\0';
	pcStr[1] = 'x';
	pcStr[0] = '0';
	for(ucHexCounter = 4; 0 < ucHexCounter ; ucHexCounter--)
	{
		pcStr[6 - ucHexCounter] = ((uiValue & (0xF << ((ucHexCounter - 1) * 4))) >> ((ucHexCounter - 1) * 4));
		if(9 >= pcStr[6 - ucHexCounter])
		{
			 pcStr[6 - ucHexCounter] = pcStr[6 - ucHexCounter] + 48;
		}
		else
		{
			pcStr[6 - ucHexCounter] = pcStr[6 - ucHexCounter] + 55;
		}
	}
}

enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue)
{
	unsigned char ucCharacterCounter;
	
	if(('0' == pcStr[0]) && ('x' == pcStr[1]) && ('\0' != pcStr[2]))
	{
		for(ucCharacterCounter = 2; pcStr[ucCharacterCounter]; ucCharacterCounter++)
		{
			if(('A' <= pcStr[ucCharacterCounter]) && ('F' >= pcStr[ucCharacterCounter]))
			{
				*puiValue = (*puiValue * 16) + (pcStr[ucCharacterCounter] - 55);
			}
			else if(('0' <= pcStr[ucCharacterCounter]) && ('9' >= pcStr[ucCharacterCounter]))
			{
				*puiValue = (*puiValue * 16) + (pcStr[ucCharacterCounter] - 48);
			}
			else
				return ERROR;
		}
		if(ucCharacterCounter > 6)
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
	unsigned char ucCharacterCounter;
	
	for(ucCharacterCounter = 0; '\0' != pcDestinationStr[ucCharacterCounter]; ucCharacterCounter++) {}
	UIntToHexStr(uiValue, pcDestinationStr+ucCharacterCounter);
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

char acArreyOne[50] = "Hej ale ja oo ro te";
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
	UIntToHexStr(0xFFFF, pcHex);*/
	eDoesHex = eHexStringToUInt("0xAABCD", &iHex);
	AppendUIntToString(iHex ,acArreyOne);
	
	
	//ucFindTokensInString(acArreyOne);
	
	//DecodeMsg("store 0x001c spryciarz");
	
	return 0;
}
