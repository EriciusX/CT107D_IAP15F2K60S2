#include "stc15f2k60s2.h"
#include "config.h"
#include "type.h"

void SelectHc573(uchar num)
{
	switch(num)
	{
		case 4: P2 = P2 & 0x1F | 0x80; break;
		case 5:	P2 = P2 & 0x1F | 0xA0; break;
		case 6:	P2 = P2 & 0x1F | 0xC0; break;
		case 7:	P2 = P2 & 0x1F | 0xE0; break;
	}
}

void cls_led(void)
{
	P0 = 0xFF;
	SelectHc573(4);
	P2 &= 0x1F;
}

void cls_buzz(void)
{
	P0 = 0;
	SelectHc573(5);
	P2 &= 0x1F;
}

void cls_all(void)
{
	cls_buzz();
	cls_led();
}

void relay_open(void)
{
	P0 = 0x10;
	SelectHc573(5);
	P2 &= 0x1F;
}



