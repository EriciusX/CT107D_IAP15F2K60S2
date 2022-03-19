#include "seg.h"
#include "config.h"
/*
* @brief 显示转换 
* @Param uchar *seg_buf 缓存数组, uchar *seg_code 显示内容数组  
*/
void seg_tran(uchar *seg_buf, uchar *seg_code)
{
	uchar i, j = 0, temp;
	for(i = 0; i < 8; i++, j++)
	{
		switch(seg_buf[j])
		{
			case '0': temp = 0xC0; break;
			case '1': temp = 0xF9; break;
			case '2': temp = 0xA4; break;
			case '3': temp = 0xB0; break;
			case '4': temp = 0x99; break;
			case '5': temp = 0x92; break;
			case '6': temp = 0x82; break;
			case '7': temp = 0xF8; break;
			case '8': temp = 0x80; break;
			case '9': temp = 0x90; break;

			case 'C': temp = 0xC6; break;
			case ' ': temp = 0xFF; break;
			case '-': temp = 0xBF; break;
			case '.': temp = 0x7F; break;

		}

		if(seg_buf[j + 1] == '.')
		{
			temp = temp & 0x7F;
			j++;
		}
		seg_code[i] = temp;
	}
}
/*
* @brief 数码管显示 
* @Param uchar *seg_code 显示内容数组, uchar seg_pos 数码管段位标志 
*/
void seg_disp(uchar *seg_code, uchar seg_pos)
{
	P0 = 0xFF;
	Select_HC573(7);
	P2 &= 0x1F;
	P0 = 1 << seg_pos;
	Select_HC573(6);
	P2 &= 0x1F;
	P0 = seg_code[seg_pos];
	Select_HC573(7);
	P2 &= 0x1F;
}