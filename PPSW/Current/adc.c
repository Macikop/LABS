#include <LPC21xx.H>
#include "adc.h"

// --- ADC Control Register (ADCR) ---
#define mBURST         0x00010000   // Burst mode enable (bit 16)
#define mPDN           0x00200000   // Power-down mode control (bit 21)
#define mCLKDIV        0x00000400   // Clock division: PCLK / (CLKDIV+1), here CLKDIV=4 (bits 15:8)

// --- ADC Global Data Register (ADDR) ---
#define mANALOG_READ   0xFFC0   		// Mask for 10-bit ADC result (bits 15:6)
#define mDONE          0x80000000   // DONE flag (bit 31)
#define mOVERUN        0x40000000   // OVERRUN flag (bit 30)
#define mCHN           0x07000000   // Channel selection mask for START field (bits 26:24)

// --- VIC (Vector Interrupt Controller) ---
#define VIC_ADC_CHANNEL_NR  0x12    // ADC interrupt channel number (18)
#define mIRQ_SLOT_ENABLE    0x00000020 // Used to enable a VIC slot (bit 5)
#define mAIN0              0x00400000  // Interrupt enable bit for ADC in VICIntEnable (bit 22)

struct ADC sADC;

__irq void ADC_Interrupt()
{
	unsigned long data = ADDR;
	unsigned long done;
	
	done = data & mDONE;
	
	if(done == mDONE)
	{
		sADC.eADCstatus = DONE;
		sADC.uiADCValue = ((data & mANALOG_READ) >> 6);
	}
	else
	{
		sADC.eADCstatus = CONVERTING;
	}
	
	VICVectAddr = 0;
}

void ADC_Init()
{
	PINSEL1 = PINSEL1 | mAIN0;
	
	ADCR = ADCR | mBURST | mCLKDIV | mPDN;
	
	VICVectAddr3  = (unsigned long) ADC_Interrupt;            // set interrupt service routine address
	VICVectCntl3  = mIRQ_SLOT_ENABLE | VIC_ADC_CHANNEL_NR;    // use it for ADCInterrupt
	VICIntEnable |= (0x1 << VIC_ADC_CHANNEL_NR);  						// Enable ADC Interrupt Channe
}

enum ADC_Status Check_ADC_Status()
{
	return sADC.eADCstatus;
}

unsigned int uiADC_GetValue()
{
	sADC.eADCstatus = CONVERTING;
	return sADC.uiADCValue;
}

unsigned int uiScaleValue(unsigned int uiValue, unsigned int uiMinIn, unsigned int uiMaxIn, unsigned int uiMinOut, unsigned int uiMaxOut)
{
	return ((((uiValue - uiMinIn) * (uiMaxOut - uiMinOut)) / (uiMaxIn - uiMinIn)) + uiMinOut);
}
