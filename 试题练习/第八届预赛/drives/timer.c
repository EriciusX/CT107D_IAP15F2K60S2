#include "timer.h"
/*
* @brief ��ʱ��0��ʼ�� 
* @Param None
*/
void Timer0Init(void)	//1����@12.000MHz	
{
	AUXR |= 0x80;	
	TMOD &= 0xF0;	
	TL0 = 0x20;	
	TH0 = 0xD1;	
	TF0 = 0;	
	TR0 = 1;	
}


/*
* @brief ��ʱ��1��ʼ�� 
* @Param None 
*/
void Timer1Init(void)		//1����@12.000MHz
{
	AUXR |= 0x40;	
	TMOD &= 0x0F;	
	TL1 = 0x20;	
	TH1 = 0xD1;	
	TF1 = 0;		
	TR1 = 1;	
}






