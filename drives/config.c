#include "type.h"
#include "config.h"
/*
* @brief 延时n毫秒 
* @Param uint n 延时时间变量 
*/
void delay_ms(uint n) 
{
    unsigned int i;
    while(n--)
        for(i = 0; i < 628; i ++);
}
/*
* @brief 延时n秒 
* @Param uint n 延时时间变量 
*/
void delay_s(uint n) //延时n秒
{
    unsigned int i=0,j=0;
    for(i=0;i<n;i++)
        for(j=0;j<21738;j++);
}
/*
* @brief 选择锁存器 
* @Param uint channel 锁存器通道 
*/
void Select_HC573(uint channel)  
{
	switch(channel)
	{
		case 4: P2 = P2 & 0x1F | 0x80; break;
		case 5:	P2 = P2 & 0x1F | 0xA0; break;
		case 6: P2 = P2 & 0x1F | 0xC0; break;
		case 7: P2 = P2 & 0x1F | 0xE0; break;
	}
}
/*
* @brief 初始化LED灯外设 
* @Param None 
*/
void cls_led(void)
{
	P0 = 0xFF;
	Select_HC573(4);
	P2 &= 0x1F;
}
/*
* @brief 初始化蜂鸣器和继电器外设 
* @Param None
*/
void cls_buzz(void)
{
	P0 = 0;
	Select_HC573(5);
	P2 &= 0x1F;
}