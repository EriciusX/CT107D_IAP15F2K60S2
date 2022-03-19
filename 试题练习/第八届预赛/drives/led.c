#include "led.h"
/*
* @brief LED显示 
* @Param uchar led8 8位led灯亮灭变量(低电平有效)  
*/
void led_disp(uchar led8)
{
	P0 = ~led8;
	P2 = P2 & 0x1F | 0x80;
	P2 &= 0x1F;
}
