#include "timer.h"
/*
* @brief 定时器0初始化 
* @Param None
*/
void Timer0Init(void)		//100us@12.000MHz
{
	AUXR |= 0x80;	
	TMOD &= 0xF0;		
	TL0 = 0x50;	
	TH0 = 0xFB;	
	TF0 = 0;	
	TR0 = 1;	
}


/*
* @brief 定时器1初始化 
* @Param None 
*/
void Timer1Init(void)		//1??@12.000MHz
{
	AUXR |= 0x40;		//?????1T??
	TMOD &= 0x0F;		//???????
	TL1 = 0x20;		//??????
	TH1 = 0xD1;		//??????
	TF1 = 0;		//??TF1??
	TR1 = 1;		//???1????
}



