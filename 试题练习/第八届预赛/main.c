#include "type.h"
#include "config.h"
#include "led.h"
#include "keys.h"
#include "seg.h"
#include "timer.h" 
#include "stdio.h"
#include "ds1302.h"
#include "onewire.h"

uchar led8 = 0x00;
uchar seg_code[8], seg_buf[9], seg_pos;
uchar state;
uchar key_old, key_down_old;
uint key_dly, seg_dly, led_dly;
uchar rtc[3] = {23, 59, 50};
uchar ding[3] = {0, 0, 0};
long int ms, ms0, count;
uchar sec;
uchar temp_val;

void key_prog(void);
void seg_prog(void);

void main(void)
{
	Timer0Init();
	Timer1Init();
	cls_led();
	cls_buzz();
	set_rtc(rtc);

	ET0 = 1;
	ET1 = 1;
	EA = 1;

	while(1)
	{
		key_prog();
		seg_prog();
	}
}

void timer0(void) interrupt 1
{
	ms0++;
	if(led_dly) led_dly--;
	if(!(ms0%200))
	{
		count++;
		if(led_dly)
		{
			if(!(count % 2)) led8 = 0x01;
			else led8 = 0;	
		}
		else
		{
			led8 = 0;
		}
	}
}

void timer1(void) interrupt 3
{
	ms++;
	if(++key_dly == 10) key_dly = 0;
	if(++seg_dly == 500) seg_dly = 0;
	if(!(ms % 1000))
	{
		sec++;			 	
	}
	seg_disp(seg_code, seg_pos);
	if(++seg_pos == 8) seg_pos = 0;
}

void key_prog(void)
{
	uchar key_down, key_val, key_up;
	if(key_dly) return;
	key_dly = 1;

	key_val = key_read();
	key_down = key_val & (key_old ^ key_val);
	key_up = !key_val & (key_old ^ key_val);

	if(key_down != key_down_old) led_dly = 0;

	key_old = key_val;
	key_down_old = key_down;
	switch(key_down)
	{
		case 4: switch(state)
				{
					case 0: if(++state == 2) state = 0; break;
					case 1: if(++state == 2) state = 0; break;
					case 0x11: if(ding[0]) ding[0]--;
							   else ding[0] = 23; break;
					case 0x12: if(ding[1]) ding[1]--;
							   else ding[1] = 59; break;
					case 0x13: if(ding[2]) ding[2]--;
							   else ding[2] = 59; break;
					case 0x21: if(rtc[0]) rtc[0]--;
							   else rtc[0] = 23; break;
					case 0x22: if(rtc[1]) rtc[1]--;
							   else rtc[1] = 59; break;
					case 0x23: if(rtc[2]) rtc[2]--;
							   else rtc[2] = 59;
				}break;
		case 5:	switch(state)
				{
					case 0x11: if(++ding[0] == 24) ding[0] = 0; break; 
					case 0x12: if(++ding[1] == 60) ding[1] = 0; break;
					case 0x13: if(++ding[2] == 60) ding[2] = 0; break;
					case 0x21: if(++rtc[0] == 24) rtc[0] = 0; break;
					case 0x22: if(++rtc[1] == 60) rtc[1] = 0; break;
					case 0x23: if(++rtc[2] == 60) rtc[2] = 0;
				}break;
		case 6:	if((state & 0xf0) != 0x10) state = 0x10;
				if(++state == 0x14) state = 0;
				break;
		case 7:	if((state & 0xf0) != 0x20) state = 0x20;
				if(++state == 0x24) state = 0; 
	}
	
	if(key_up == 4) state = 0;
	if(ding[0]==rtc[0]&&ding[1]==rtc[1]&&ding[2]==rtc[2])
		led_dly = 5000;
		
	led_disp(led8);
}

void seg_prog(void)
{
	if(seg_dly) return;
	seg_dly = 1;

	switch(state & 0xf0)
	{
		case 0:	if(state == 1)
				{
					temp_val = rd_temperature();
					sprintf(seg_buf, "     %02uC", (uint)(temp_val/10));
				} 
				else
				{
					read_rtc(rtc);
					sprintf(seg_buf, "%02u-%02u-%02u", (uint)rtc[0],\
					(uint)rtc[1], (uint)rtc[2]);	
				}break; 
		case 0x10: read_rtc(rtc);
			  	   sprintf(seg_buf, "%02u-%02u-%02u", (uint)rtc[0],\
			       (uint)rtc[1], (uint)rtc[2]);
				   if(sec & 1)
					   switch(state)
					   {
						   case 0x11: seg_buf[0] = 0xff;
						              seg_buf[1] = 0xff;
								      break;
						   case 0x12: seg_buf[3] = 0xff;
						              seg_buf[4] = 0xff;
								      break;
						   case 0x13: seg_buf[6] = 0xff;
						              seg_buf[7] = 0xff;
					   }break;
		case 0x20: sprintf(seg_buf, "%02u-%02u-%02u", (uint)ding[0],\
				   (uint)ding[1], (uint)ding[2]);
				   if(sec & 1)
					   switch(state)
					   {
					       case 0x21: seg_buf[0] = 0xff;
							          seg_buf[1] = 0xff;
									  break;
						   case 0x22: seg_buf[3] = 0xff;
							          seg_buf[4] = 0xff;
									  break;
						   case 0x23: seg_buf[6] = 0xff;
							          seg_buf[7] = 0xff;
					   }	
	}
	seg_tran(seg_buf, seg_code);
}


