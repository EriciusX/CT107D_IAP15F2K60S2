#include "type.h"
#include "led.h"
#include "stdio.h"
#include "onewire.h"
#include "config.h"
#include "timer.h"
#include "keys.h"
#include "seg.h"
#include "ds1302.h"

uchar rtc[3] = {23, 55, 30};
uchar led8 = 0x02;
uchar mode = 0;
uchar state = 0;
uchar key_val;
uint key_dly, seg_dly; 
uint led_dly;
uchar seg_code[8] = {0xC1, 0xC1}, seg_pos = 1, seg_buf[9];
float temperature, high, low;
uchar temp_limit = 23;
uchar relay_flag;
long int ms, ms1;

void key_prog(void);
void seg_prog(void);

void main(void)
{	
	cls_all();
		
	Timer0Init();
	Timer1Init();
	EA = 1;

	while(1)
	{
		key_prog();
		seg_prog();
	}
}

void timer0() interrupt 1
{
	ms1++;
	if(!(ms1%500))
	{	
		read_rtc(rtc);
		temperature = ((rd_temp()&0xFF0) >> 4) + ((rd_temp()&0x0F)*0.625);
	}	
}

void timer1() interrupt 3
{
	ms++;
	if(++key_dly == 150) key_dly = 0;
	if(++seg_dly == 10) seg_dly = 0;

	if(led_dly) led_dly--;
 	else led8 &= 0xFE;

   	if(!(ms%50))
   	{	
		if(led_dly)
		{
			if(led8 & 0x04) led8 &= 0xFB;
			else led8 |= 0x04;
		}
	}
	seg_disp(seg_code, seg_pos);
	if(++seg_pos == 8) seg_pos = 0;	
}

void key_prog(void)
{
	uchar key_old, key_up, key_down;

	if(key_dly) return;
	key_dly = 1;

	key_val = key_read();
	key_down = key_val ^ (key_old & key_val);
	key_up = ~key_val ^ (key_old & key_val);

	key_old = key_val;	

	switch(key_down)
	{
		case 0:
			if((state & 0xF0) == 0x10)
				state = 0x10;
			break;
		case 12: 
			if((state += 0x10) == 0x30) state = 0x00; 
			break;
		case 13: 
			if(++mode == 2) mode = 0;
			if(mode == 0) led8 |= 0x02;
			else led8 &= 0xFD;
			break;
		case 16: 
			if(state == 0x20)
				if(++temp_limit == 100) temp_limit = 99;
			break;
		case 17:
			switch(state & 0xF0)
			{
				case 0x10:
					state = 0x11;
					break;
				case 0x20: 
					if(--temp_limit < 10) temp_limit = 10;
			}		
	}
	if(key_up == 17)
		if((state & 0xF0) == 0x10) state = 0x10;

	if(mode == 0)
	{
		if(temperature > temp_limit) 
		{
			relay_open();
			led_dly = 5000;
			led8 |= 0x01;
		}
		else 
		{
			cls_buzz();
			led8 &= 0xFB;
		}
	}
	else 
	{
		if(rtc[1]==0&&rtc[2]==0)
		{
			led_dly = 5000;
			led8 |= 0x01;
			relay_open();
		}
		else
		{
			cls_buzz();
			led8 &= 0xFB;		
		}		
	}
	led_disp(led8);
}

void seg_prog(void)
{
	if(!seg_dly) return;
	seg_dly = 1;
	switch(state & 0xF0)
	{
		case 0x00:
			sprintf(seg_buf, "U1   %3.1f", (float)temperature);
			break;
		case 0x10:
			if(state == 0x10) sprintf(seg_buf, "U2 %02u-%02u", (uint)rtc[0], (uint)rtc[1]);
			else if(state == 0x11) sprintf(seg_buf, "U2 %02u-%02u", (uint)rtc[1], (uint)rtc[2]);
			break;
		case 0x20:
			sprintf(seg_buf, "U3    %02u", (uint)temp_limit);
			break;
	}
	seg_tran(seg_buf, seg_code);
}

