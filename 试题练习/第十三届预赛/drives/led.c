#include "stc15f2k60s2.h"
#include "led.h"
#include "type.h"
#include "config.h"

void led_disp(uchar led8)
{
	P0 = ~led8;
	SelectHc573(4);
	P2 &= 0x1F;
}

