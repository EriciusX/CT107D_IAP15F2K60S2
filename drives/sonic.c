#include "sonic.h"
#include "timer.h"

/*
* @brief 超声波接收 
* @Param None
* @retval 返回距离值
*/
uchar wave_recv(void)
{
	uchar dist, num = 10;
	TX = 0;
	TL0 = 0xF4;
	TH0 = 0xFF;
	TR0 = 1;

	while(num--)
	{
		while(!TF0);
		TX ^= 1;
		TF0 = 0;
	}
	TR0 = 0;
	TL0 = 0;
	TH0 = 0;
	TR0 = 1;
	while(RX && !TF0);
	TR0 = 0;
	if(TF0)
	{
		TF0 = 0;
		dist = 255;
	}
	else
		dist = ((TH0 << 8) + TL0) * 0.017;
	return dist;
}
