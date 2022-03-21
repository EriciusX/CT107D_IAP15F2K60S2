#include "type.h"
#include "config.h"
#include "led.h"
#include "keys.h"
#include "seg.h"
#include "timer.h" 
#include "stdio.h"
#include "sonic.h"
#include "24c02.h"
#include "pcf8591.h"

uchar dist_rom[8];
uint Vout;
uchar dist, dist_old, page_flag, old_flag, blind;
uchar led8 = 0x00;
uchar seg_code[8], seg_buf[9], seg_pos;
uchar state = 0;
uchar key_old;
uint key_dly, seg_dly, led_dly;
long int ms, dist_flag;

void key_prog(void);
void seg_prog(void);

void main(void)
{
	Timer0Init();
	Timer1Init();
	cls_led();
	cls_buzz();

	eeprom_read(seg_buf, 8, 1);
	if(seg_buf[0] <= 30) blind = seg_buf[0];
	else blind = 20;
	eeprom_read(dist_rom, 0, 8);

	ET1 = 1;
	EA = 1;

	while(1)
	{
		key_prog();
		seg_prog();
	}
}


void timer1(void) interrupt 3
{
	ms++;
	if(++key_dly == 10) key_dly = 0;
	if(led_dly)
	{
		if(!(led_dly % 1000)) led8 += 1;	
		else led8 &= 0xfe;
		led_dly--;
		led_disp(led8);
	}
	seg_disp(seg_code, seg_pos);
	if(++seg_pos == 8) seg_pos = 0;
}

void key_prog(void)
{
	uchar key_down, key_val, i;
	if(key_dly) return;
	key_dly = 1;

	key_val = key_read();
	key_down = key_val & (key_old ^ key_val);

	key_old = key_val;
	switch(key_down)
	{
		case 4: dist = wave_recv();
				if(dist >= blind)
				{
					dist = dist - blind;
					Vout = dist * 1.02;
					if(Vout > 255) Vout = 255; 
				} 
				else
				{
					dist = 0;
					Vout = 0;
				}

				if(dist_flag >= 4)
				{
					for(i = 4; i < 8; i++) dist_rom[i] -= 1;
					dist_rom[(dist_flag % 4) + 4] = 4;	
				} 
				else dist_rom[dist_flag + 4] = dist_flag + 1;
				dist_rom[dist_flag % 4] = dist;
				page_flag = dist_flag % 4;
				eeprom_write(dist_rom, 0, 8);
				pcf8591_Dac(Vout);
				led_dly = 3000;
				dist_flag++;
				break;
		case 5:	if((state & 0xf0) != 0x10)	
				{
					state = 0x10;
					led8 |= 0x80;
				}
				if(++state == 0x12) 
				{
					state = 0;
					led8 &= 0x7f;
				}
				break;
		case 6:	if((state & 0xf0) != 0x20)
				{ 
					led8 |= 0x40;
					state = 0x20;
				}
				if(++state == 0x22) 
				{
					led8 &= 0xbf;
					state = 0;
				} 
				break;
		case 7:	switch(state)
				{ 
					case 0x21:
					if(blind < 30) blind += 10;
					else blind = 0;
					seg_buf[0] = blind;
					eeprom_write(seg_buf, 8, 1);
					break;
					case 0x11:
					if(++page_flag == 4) page_flag = 0;
				}
				 
	}
	led_disp(led8);	
}

void seg_prog(void)
{
	switch(state & 0xf0)
	{
		case 0:	if(dist_flag > 1)
				sprintf(seg_buf, "C %03u%03u", (uint)dist_rom[(dist_flag - 1) % 4],\
				 (uint)dist_rom[(dist_flag - 2) % 4]);
				else if(dist_flag == 1) sprintf(seg_buf, "C %03u000", (uint)dist_rom[0]);
				else sprintf(seg_buf, "C 000000");
				break;
		case 0x10: sprintf(seg_buf, "%1u    %03u", (uint)dist_rom[page_flag + 4],\
					(uint)dist_rom[page_flag]);
				   break;
		case 0x20: sprintf(seg_buf, "F     %2u", (uint)blind);
	}
	seg_tran(seg_buf, seg_code);
}


