#define RECIEVER_SIZE 255
#define TERMINATOR '\r'

enum eRecieverStatus {EMPTY, READY, OVERFLOW};

void UART_InitWithInt(unsigned int uiBaudRate);
void Reciever_PutCharacterToBuffer(char cCharacter);
enum eRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char * ucDestination);
