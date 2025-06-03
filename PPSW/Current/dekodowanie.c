#include "dekodowanie.h"
#include "stringi.h"
#include "konwersje.h"

#define MAX_KEYWORD_NR 3

struct Keyword asKeywordList[MAX_KEYWORD_NR] = 
{
    {GT, "goto"}, 
    {CAL, "callib"},
		{CALC, "calc"}
};
    
struct Token asToken[MAX_TOKEN_NR];
unsigned char ucTokenNr = 0;

enum eTokenFinderState { TOKEN, DELIMITER };

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

enum Result eStringToKeyword(char pcStr[], enum KeywordCode *peKeywordCode)
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
		if(OK == eStringToKeyword(spCurrentToken->uValue.pcString, &eDecodedKeyword))
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
