#include <LPC21xx.H>
#include "led.h"
#include "timer_interrupts.h"
#include "servo.h"

#define DEFECTOR_bm (1<<10)

enum eServoState {CALLIB, IDLE, IN_PROGRESS, OFFSET};

unsigned char ucOffset = 12;

struct Servo
{
	enum eServoState eState; 
	unsigned int uiCurrentPosition; 
	unsigned int uiDesiredPosition;
}; 

struct Servo sServo;

void DetectorInit()
{
	IO0DIR = IO0DIR & (~DEFECTOR_bm);
}

enum DetectorState {INACTIVE, ACTIVE};

enum DetectorState eReadDetector()
{
	if ((IO0PIN & DEFECTOR_bm) == 0)
	{
		return ACTIVE;
	}
	else
	{
		return INACTIVE;
	}
}

void ServoCallib(void)
{
	sServo.eState = CALLIB;
}

void ServoGoTo(unsigned int uiPosition)
{
	sServo.uiDesiredPosition = uiPosition;
}

void Automat()
{
	switch(sServo.eState)
	{
		case CALLIB:
			if(eReadDetector()==INACTIVE)
			{ 
				LedStepRight();
				sServo.eState = CALLIB;
			}
			else
			{
				sServo.uiDesiredPosition = 0;
				sServo.uiCurrentPosition = 0;
				sServo.eState = OFFSET;
			}
			break;
		case OFFSET:
			sServo.uiDesiredPosition = sServo.uiDesiredPosition + ucOffset;
		
			break;
		case IDLE:
			if (sServo.uiCurrentPosition != sServo.uiDesiredPosition)
			{
				sServo.eState = IN_PROGRESS;
			}
			else
			{
				sServo.eState = IDLE;
			}
			break;
		case IN_PROGRESS:
			if(sServo.uiCurrentPosition == sServo.uiDesiredPosition)
			{
				sServo.eState = IDLE;
			}
			else
			{
				if(sServo.uiCurrentPosition > sServo.uiDesiredPosition)
				{
					LedStepLeft();
					sServo.uiCurrentPosition--;
				}
				else if (sServo.uiCurrentPosition < sServo.uiDesiredPosition)
				{
					LedStepRight();
					sServo.uiCurrentPosition++;
				}
				sServo.eState = IN_PROGRESS;
			}
			break;
	}
}

void ServoInit(unsigned int uiServoFrequency) // f in Hz
{
	LedInit();
	DetectorInit();
	
	Timer0Interrupts_Init(1000000 / uiServoFrequency, &Automat);
	sServo.eState = CALLIB;
}

