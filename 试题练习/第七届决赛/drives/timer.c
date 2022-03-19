#include "timer.h"
/*
* @brief ��ʱ��0��ʼ�� 
* @Param None
*/
void Timer0Init(void)
{
	AUXR &= 0x7f;	
	TMOD |= 5;		
	TL0 = 0;	
	TH0 = 0;	
	TF0 = 0;	
	TR0 = 1;	
}


/*
* @brief ��ʱ��1��ʼ�� 
* @Param None 
*/
void Timer1Init(void)		//1??@11.0592MHz
{
	AUXR |= 0x40;		//?????1T??
	TMOD &= 0x0F;		//???????
	TL1 = 0xCD;		//??????
	TH1 = 0xD4;		//??????
	TF1 = 0;		//??TF1??
	TR1 = 1;		//???1????
}




