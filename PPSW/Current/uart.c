#include <LPC210X.H>
#include "uart.h"
#include "stringi.h"

/************ UART ************/
// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

/************ Interrupts **********/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020

////////////// Zmienne globalne ////////////
char cOdebranyZnak;

struct RecieverBuffer sReciverBuffer;
struct TransmiterBuffer sTransmiterBuffer;

///////////////////////////////////////////

void Reciever_PutCharacterToBuffer(char cCharacter)
{	
	if(RECIEVER_SIZE == sReciverBuffer.ucCharCtr)
	{
		sReciverBuffer.eStatus = OVERFLOW;
	}
	else if(TERMINATOR == cCharacter)
	{
		sReciverBuffer.cData[sReciverBuffer.ucCharCtr] = '\0';
		sReciverBuffer.eStatus = READY;
	}
	else
	{
		sReciverBuffer.cData[sReciverBuffer.ucCharCtr] = cCharacter;
		sReciverBuffer.ucCharCtr++;
	}
}

char Transmiter_GetCharacterFromBuffer(void)
{
	char cCurrentCharacter = 0;
	
	if(0 == sTransmiterBuffer.fLastCharacter)
	{
		cCurrentCharacter = sTransmiterBuffer.cData[sTransmiterBuffer.ucCharCtr];
		if ('\0' == cCurrentCharacter)
		{
			cCurrentCharacter = TERMINATOR;
			sTransmiterBuffer.fLastCharacter = 1;
		}
		sTransmiterBuffer.ucCharCtr++;		
	}
	else
	{
		cCurrentCharacter = '\0'; 
	}
	return cCurrentCharacter;
}

__irq void UART0_Interrupt (void) 
{
	// jesli przerwanie z odbiornika (Rx)

	unsigned int uiCopyOfU0IIR=U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii
	char cCurrentCharacter;

	if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
	{
		cOdebranyZnak = U0RBR;
		Reciever_PutCharacterToBuffer(cOdebranyZnak);
	} 

	if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
	{
		if(BUSY == sTransmiterBuffer.eStatus)
		{
			cCurrentCharacter = Transmiter_GetCharacterFromBuffer();
			if('\0' != cCurrentCharacter)
			{
			 U0THR = cCurrentCharacter;
			}
			else
			{
				sTransmiterBuffer.eStatus = FREE;
			}
		}
	}
	VICVectAddr = 0; // Acknowledge Interrupt
}

////////////////////////////////////////////
void UART_InitWithInt(unsigned int uiBaudRate)
{
   // UART0
   PINSEL0 = PINSEL0 | 0x05;                                     // ustawic pina na odbiornik uart0
   U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // dlugosc slowa, DLAB = 1
   U0DLL   = (((15000000)/16)/uiBaudRate);                      // predkosc transmisji
   U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
   U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE | mTHRE_INTERRUPT_ENABLE;               // ??? co tu robinmy

   // INT
   VICVectAddr2  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
   VICVectCntl2  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
   VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);  							// Enable UART 0 Interrupt Channel
	
	sReciverBuffer.eStatus = EMPTY;
	sReciverBuffer.ucCharCtr = 0;
}

enum eRecieverStatus eReciever_GetStatus(void)
{
	return sReciverBuffer.eStatus;
}

void Reciever_GetStringCopy(char *ucDestination)
{
	CopyString(sReciverBuffer.cData, ucDestination);
	sReciverBuffer.eStatus = EMPTY;
	sReciverBuffer.ucCharCtr = 0;
}

void Transmiter_SendString(char pcString[])
{
	CopyString(pcString ,sTransmiterBuffer.cData);
	
	sTransmiterBuffer.eStatus = BUSY;
	sTransmiterBuffer.ucCharCtr = 0;
	sTransmiterBuffer.fLastCharacter = 0;
	U0THR = Transmiter_GetCharacterFromBuffer();
}

enum eTransmiterStatus Transmiter_GetStatus(void)
{
	return sTransmiterBuffer.eStatus;
}
