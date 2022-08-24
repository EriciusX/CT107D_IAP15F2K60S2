#include "timer.h"
#include "stc15f2k60s2.h"

void Timer0Init(void)		
{
	AUXR |= 0x80;	
	TMOD &= 0xF0;	
	TL0 = 0x20;	
	TH0 = 0xD1;	
	TF0 = 0;	
	TR0 = 1;
	ET0 = 1;	
}

void Timer1Init(void)		
{
	AUXR |= 0x40;	
	TMOD &= 0x0F;	
	TL1 = 0x20;	
	TH1 = 0xD1;		
	TF1 = 0;	
	TR1 = 1;
	ET1 = 1;	
}
