#include <stdio.h>
#include <string.h>

#include "stringi.h"
#include "konwersje.h"
#include "dekodowanie.h"

void TestOf_CopyString()
{
    char pcString1[] = "Test CopyString";
    char pcString2[30];
    
    printf("\nCopyString\n");
    printf("Test 1 - ");
    
    CopyString(pcString1, pcString2);
    //sprawdzenie czy kopjuje poprawnie
    if (0 == strcmp(pcString1, pcString2)) printf("OK\n"); else printf("Error\n");
}

void TestOf_eCompareString()
{
    char pcString1[] = "Test eCompareString 1";
    char pcStringTooShort[] = "Test eCompare";
    char pcStringTooLong[] = "Test eCompareString 12345";
    char pcStringWrongLetter[] = "Test uCompareString 1";
    char pcStringEmpty[] = "";
    
    printf("\neCompareString\n");
    
    printf("Test 1 - ");
    //sprawdzenie czy poprawnie porównuje dwa takie same stringi
    if (EQUAL == eCompareString(pcString1, pcString1)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 2 - ");
    //porównanie stringu z dłuższym stringiem
    if (DIFFERENT == eCompareString(pcString1, pcStringTooLong)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 3 - ");
    //porównanie stringu z krótszym stringiem
    if (DIFFERENT == eCompareString(pcString1, pcStringTooShort)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 4 - ");
    //porównanie różnych stringów
    if (DIFFERENT == eCompareString(pcString1, pcStringWrongLetter)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 5 - ");
    //porównanie dwóch pustych stringów
    if (EQUAL == eCompareString(pcStringEmpty, pcStringEmpty)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 6 - ");
    //porównanie stringu z pustym stringiem
    if (DIFFERENT == eCompareString(pcString1, pcStringEmpty)) printf("OK\n"); else printf("Error\n");
    
}

void TestOf_AppendString()
{
    char pcString[] = "Test AppendString";
    char pcFirst_Part[30] = "Test ";
    char pcSecond_Part[] = "AppendString";
    
    printf("\nAppendString\n");
    
    printf("Test 1 - ");
    
    AppendString(pcSecond_Part, pcFirst_Part);
    //czy łączy łańcuchy znakowe
    if(EQUAL == eCompareString(pcFirst_Part, pcString)) printf("OK\n"); else printf("Error\n");
}

void TestOf_ReplaceCharactersInString()
{
    char pcString[] = "Test ReplaceCharactersInString";
    
    printf("\nReplaceCharactersInString\n");
    
    printf("Test 1 - ");
    //sprawdzenie czy podmienia znaki w stringu
    ReplaceCharactersInString(pcString, 'e', 'x');
    if (0 == strcmp(pcString, "Txst RxplacxCharactxrsInString")) printf("OK\n"); else printf("Error\n");

}

void TestOf_UIntToHexStr()
{
    char pcStringHex[7];
    
    printf("\nUIntToHexStr\n");
    
    printf("Test 1 - ");
    //czy zamienia liczbę na hex string, końce przedziałów A,F i 0,9
    UIntToHexStr(0xAF09, pcStringHex);
    if (0 == strcmp(pcStringHex, "0xAF09")) printf("OK\n"); else printf("Error\n");

}

void TestOf_eHexStringToUInt()
{
    unsigned int uiNumber = 0;
    
    printf("\neHexStringToUInt\n");
    
    printf("Test 1 - ");
    //czy poprawnie zamienia hex string na liczbe
    if ((OK == eHexStringToUInt("0xAF09", &uiNumber)) && (0xAF09 == uiNumber)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 2 - ");
    //czy poprawnie działą na pusty string
    if (ERROR == eHexStringToUInt("", &uiNumber)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 3 - ");
    //czy poprawnie działa tylko prefix
    if (ERROR == eHexStringToUInt("0x", &uiNumber)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 4 - ");
    //czy poprawnie działa na za długi string
    if (ERROR == eHexStringToUInt("0x15ABC", &uiNumber)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 5 - ");
    //czy poprawnie działa na poprawny krótki string
    if ((OK == eHexStringToUInt("0x7", &uiNumber)) && (0x7 == uiNumber)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 6 - ");
    //czy poprawnie reaguje na string bez prefixu
    if (ERROR == eHexStringToUInt("AF09", &uiNumber)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 7 - ");
    //czy poprawnie działa na string z nienpoprawnymi znakami
    if (ERROR == eHexStringToUInt("0x1GH3", &uiNumber)) printf("OK\n"); else printf("Error\n");
}

void TestOf_AppendUIntToString()
{
    char pcString[32] = "Test AppendUIntToString ";
    char pcEmptyString[32] = "Test \0 Ola ma jeza";
    
    printf("\nAppendUIntToString\n");
    
    printf("Test 1 - ");
    //Czy poprawnie dokleja na koniec stringa
    AppendUIntToString(0xAF09, pcString);
    
    if(0 == strcmp(pcString, "Test AppendUIntToString 0xAF09")) printf("OK\n"); else printf("Error\n");
    
    printf("Test 2 - ");
    //czy poprawnie dokleja stringa na koniec ostatniego
    AppendUIntToString(0xAF09, pcEmptyString);
    
    if(0 == strcmp(pcEmptyString, "Test 0xAF09")) printf("OK\n"); else printf("Error\n");
    
}

void TestOf_ucFindTokensInString()
{
    char pcValidString[] = "reset 0x01AB kot";
    char pcOnlyDelimiterString[] = "          ";
    char pcDelimiterFirstString[] = " load 0x51AF pies";
    char pcTwoDelimitersString[] = "store  0x31CB  zebra";
    
    printf("\nucFindTokensInString\n");
    
    printf("Test 1 - ");
    //czy działa dla poprawnego stringu
    if ((3 == ucFindTokensInString(pcValidString)) && (pcValidString == asToken[0].uValue.pcString) && ((pcValidString + 6) == asToken[1].uValue.pcString) && ((pcValidString + 13) == asToken[2].uValue.pcString)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 2 - ");
    //czy działa dla pustego stringa
    if (0 == ucFindTokensInString(pcOnlyDelimiterString)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 3 - ");
    //czy działa jeżeli na string zaczyna się od delimitera
    if ((3 == ucFindTokensInString(pcDelimiterFirstString)) && ((pcDelimiterFirstString + 1) == asToken[0].uValue.pcString) && ((pcDelimiterFirstString + 6) == asToken[1].uValue.pcString) && ((pcDelimiterFirstString + 13) == asToken[2].uValue.pcString)) printf("OK\n"); else printf("Error\n");

    printf("Test 4 - ");
    //czy działa dla stringa gdzie odzielone są więcej niż jednym delimiterem
    if ((3 == ucFindTokensInString(pcTwoDelimitersString)) && (pcTwoDelimitersString == asToken[0].uValue.pcString) && ((pcTwoDelimitersString + 7) == asToken[1].uValue.pcString) && ((pcTwoDelimitersString + 15) == asToken[2].uValue.pcString)) printf("OK\n"); else printf("Error\n");
    
}

void TestOf_eStringToKeyword()
{
    char pcString1[] = "reset";
    char pcString2[] = "load";
    char pcString3[] = "store";
    char pcInvalidString[] = "reeeesssseeettt";
    enum KeywordCode eTest;
    
    printf("\neStringToKeyword\n");
    
    printf("Test 1 - ");
    //czy rozpoznaje reset
    if ((OK == eStringToKeyword(pcString1, &eTest)) && (eTest == RST)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 2 - ");
    //czy rozpoznaje load
    if ((OK == eStringToKeyword(pcString2, &eTest)) && (eTest == LD)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 3 - ");
    //czy rozpoznaje store
    if ((OK == eStringToKeyword(pcString3, &eTest)) && (eTest == ST)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 4 - ");
    //czy poprawnie reaguje na niepoprawy string
    if(ERROR == eStringToKeyword(pcInvalidString, &eTest)) printf("OK\n"); else printf("Error\n");
}

void TestOf_DecodeTokens()
{
    char pcString[] = "load test 0xFC35";
    ucTokenNr = ucFindTokensInString(pcString);
    ReplaceCharactersInString(pcString, ' ', '\0');
    DecodeTokens();
    
    printf("\nDecodeTokens\n");
    
    printf("Test 1 - ");
    //czy poprawnie dekoduje keyword
    if ((asToken[0].eType == KEYWORD) && (asToken[0].uValue.eKeyword == LD)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 2 - ");
    //czy poprawnie dekoduje string
    if ((asToken[1].eType == STRING) && (asToken[1].uValue.pcString == (pcString + 5))) printf("OK\n"); else printf("Error\n");
    
    printf("Test 3 - ");
    //czy poprawnie dekoduje liczbe
    if ((asToken[2].eType == NUMBER) && (asToken[2].uValue.uiNumber == 0xFC35)) printf("OK\n"); else printf("Error\n");
}

void TestOf_DecodeMsg()
{
    
    char pcValidString[] = "reset 0x01AB kot";
    char pcOnlyDelimiterString[] = "          ";
    char pcDelimiterFirstString[] = " load 0x51AF pies";
    char pcTwoDelimitersString[] = "store  0x31CB  zebra";
    
    printf("\nDecodeMsg\n");
    
    printf("Test 1 - ");
    //czy poprawnie dekoduje poprawny string
    DecodeMsg(pcValidString);
    if ((KEYWORD == asToken[0].eType) && (RST == asToken[0].uValue.eKeyword) && (NUMBER == asToken[1].eType) && (0x01AB == asToken[1].uValue.uiNumber) && (STRING == asToken[2].eType) && ((pcValidString + 13) == asToken[2].uValue.pcString)) printf("OK\n"); else printf("Error\n");
    
    printf("Test 2 - ");
    //czy poprawnie reaguje na pusty string
	DecodeMsg(pcOnlyDelimiterString);
	if (0 == ucTokenNr) printf("OK\n"); else printf("Error\n");
	
	printf("Test 3 - ");
	//czy poprawnie dekoduje string zaczynający się od delimitera
	DecodeMsg(pcDelimiterFirstString);
	if ((KEYWORD == asToken[0].eType) && (LD == asToken[0].uValue.eKeyword) && (NUMBER == asToken[1].eType) && (0x51AF == asToken[1].uValue.uiNumber) && (STRING == asToken[2].eType) && ((pcDelimiterFirstString + 13) == asToken[2].uValue.pcString)) printf("OK\n"); else printf("Error\n");
	
	printf("Test 4 - ");
	//czy poprawnie dekoduje na string gdzie tokeny są oddzielone wiecej niż jednym delimiterem
	DecodeMsg(pcTwoDelimitersString);
	if ((KEYWORD == asToken[0].eType) && (ST == asToken[0].uValue.eKeyword) && (NUMBER == asToken[1].eType) && (0x31CB == asToken[1].uValue.uiNumber) && (STRING == asToken[2].eType) && ((pcTwoDelimitersString + 15) == asToken[2].uValue.pcString)) printf("OK\n"); else printf("Error\n");

}

int main()
{
    printf("\n\nTEST FUNKCJI OPERACJI NA STRINGACH\n\n");
    TestOf_CopyString();
    TestOf_eCompareString();
    TestOf_AppendString();
    TestOf_ReplaceCharactersInString();
    
    printf("\n\nTEST FUNKCJI KONWERSJI STRINGÓW\n\n");
    TestOf_UIntToHexStr();
    TestOf_eHexStringToUInt();
    TestOf_AppendUIntToString();
    
    printf("\n\nTEST FUNKCJI DEKODOWANIA KOMUNIKATÓW\n\n");
    TestOf_ucFindTokensInString();
    TestOf_eStringToKeyword();
    TestOf_DecodeTokens();
    TestOf_DecodeMsg();
    
    return 0;
}