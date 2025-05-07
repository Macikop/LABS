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

void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[])
{
	unsigned char ucCharacterCounter;
	
	for(ucCharacterCounter = 0; '\0' != pcDestinationStr[ucCharacterCounter]; ucCharacterCounter++) {}
	UIntToHexStr(uiValue, pcDestinationStr + ucCharacterCounter);
}

enum eTokenFinderState {TOKEN, DELIMITER};

unsigned char ucFindTokensInString(char *pcString)
{
	enum eTokenFinderState eFinderState = DELIMITER;
	
	unsigned char ucCharacterCounter;
	unsigned char ucTokenCounter = 0;
	
	char cCurrentCharacter;
	
	for(ucCharacterCounter = 0 ;; ucCharacterCounter++)
	{
		cCurrentCharacter = pcString[ucCharacterCounter];
		switch(eFinderState)
		{
			case TOKEN:
				if (MAX_TOKEN_NR == ucTokenCounter)
				{
					return ucTokenCounter;
				}
				else if ('\0' == cCurrentCharacter)
				{
					return ucTokenCounter;
				}
				else if (' ' != cCurrentCharacter)
				{
					eFinderState = TOKEN;
				}
				else
				{
					eFinderState = DELIMITER;
				}
				break;
			case DELIMITER:
				if ('\0' == cCurrentCharacter)
				{
					return ucTokenCounter;
				}
				else if (' ' == cCurrentCharacter)
				{
					eFinderState = DELIMITER;
				}
				else
				{
					eFinderState = TOKEN;
					asToken[ucTokenCounter].uValue.pcString = pcString + ucCharacterCounter;
					ucTokenCounter++;
				}
				break;
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
	unsigned char ucTokenIndex;
	
	enum KeywordCode eDecodedKeyword;
	unsigned int uiDecodedNumber;
	struct Token *spCurrentToken;
	
	for(ucTokenIndex = 0; ucTokenNr > ucTokenIndex; ucTokenIndex++)
	{
		spCurrentToken = &asToken[ucTokenIndex];
		if(OK == eSringToKeyword(spCurrentToken->uValue.pcString, &eDecodedKeyword))
		{
			spCurrentToken->eType = KEYWORD;
			spCurrentToken->uValue.eKeyword = eDecodedKeyword;
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
	ucTokenNr = ucFindTokensInString(pcString);
	ReplaceCharactersInString(pcString, ' ', '\0');
	DecodeTokens();
}

char acArreyOne[50] = "Hej\0absaaaaaaaa";
char acArreyTwo[4];
enum CompResult eResoults;
char pcHex[32];
enum Result eHexResoult;
unsigned int uiHex;

unsigned char TokenNumber = 0;

char cValidString[] = "reset 0x01AB kot";
char cOnlyDelimiterString[] = "          ";
char cDelimiterFirstString[] = " load 0x51AF pies";
char cTwoDelimitersString[] = " store  0x31CB  zebra";


int main()
{
	DecodeMsg(cValidString);
	DecodeMsg(cOnlyDelimiterString);
	DecodeMsg(cDelimiterFirstString);
	DecodeMsg(cTwoDelimitersString);
	
	return 0;
}
