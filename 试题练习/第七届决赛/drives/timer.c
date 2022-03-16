#include "timer.h"
/*
* @brief 定时器0初始化 
* @Param None
*/
void Timer0Init(void)		//100us@12.000MHz
{
	AUXR &= 0x7f;	
	TMOD |= 5;		
	TL0 = 0;	
	TH0 = 0;	
	TF0 = 0;	
	TR0 = 1;	
}


/*
* @brief 定时器1初始化 
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




