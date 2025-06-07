enum ADC_Status {DONE, OVERRUN, CONVERTING};

struct ADC
{
	unsigned int uiADCValue;
	enum ADC_Status eADCstatus;
};

void ADC_Init(void);
enum ADC_Status Check_ADC_Status(void);
unsigned int uiADC_GetValue(void);

unsigned int uiScaleValue(unsigned int uiValue, unsigned int uiMinIn, unsigned int uiMaxIn, unsigned int uiMinOut, unsigned int uiMaxOut);
