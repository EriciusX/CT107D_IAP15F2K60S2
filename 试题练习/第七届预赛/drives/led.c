#include "led.h"
/*
* @brief LED��ʾ 
* @Param uchar led8 8λled���������(�͵�ƽ��Ч)  
*/
void led_disp(uchar led8)
{
	P0 = ~led8;
	P2 = P2 & 0x1F | 0x80;
	P2 &= 0x1F;
}
