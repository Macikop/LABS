#define RECIEVER_SIZE 12
#define TERMINATOR '\r'

enum eRecieverStatus {EMPTY, READY, OVERFLOW};

struct RecieverBuffer
{ 
	char cData[RECIEVER_SIZE];
	unsigned char ucCharCtr;
	enum eRecieverStatus eStatus;
};

void UART_InitWithInt(unsigned int uiBaudRate);

//void Reciever_PutCharacterToBuffer(char cCharacter);
enum eRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char * ucDestination);
