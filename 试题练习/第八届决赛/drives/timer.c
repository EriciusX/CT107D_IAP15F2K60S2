#include "timer.h"
/*
* @brief 定时器0初始化 
* @Param None
*/
void Timer0Init(void)	//12us@12.000MHz	
{
	AUXR &= 0x7F;	
	TMOD &= 0xF0;	
	TL0 = 0xF4;	
	TH0 = 0xFF;	
	TF0 = 0;	
	TR0 = 0;	
}


/*
* @brief 定时器1初始化 
* @Param None 
*/
void Timer1Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x40;	
	TMOD &= 0x0F;	
	TL1 = 0x20;	
	TH1 = 0xD1;	
	TF1 = 0;		
	TR1 = 1;	
}






