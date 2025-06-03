#define RECIEVER_SIZE 25
#define TRANSMITER_SIZE 25
#define TERMINATOR '\r'

enum eRecieverStatus {EMPTY, READY, OVERFLOW};

struct RecieverBuffer
{ 
char cData[RECIEVER_SIZE];
unsigned char ucCharCtr;
enum eRecieverStatus eStatus;
};

enum eTransmiterStatus {FREE, BUSY};

struct TransmiterBuffer
{
	char cData[TRANSMITER_SIZE];
	enum eTransmiterStatus eStatus;
	unsigned char fLastCharacter;
	unsigned char ucCharCtr;
};

void UART_InitWithInt(unsigned int uiBaudRate);

//void Reciever_PutCharacterToBuffer(char cCharacter);
enum eRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char * ucDestination);

//char Transmiter_GetCharacterFromBuffer(void);
enum eTransmiterStatus Transmiter_GetStatus(void);
void Transmiter_SendString(char pcString[]);

