#include "type.h"
#include "config.h"
#include "led.h"
#include "keys.h"
#include "seg.h"
#include "timer.h" 
#include "stdio.h"
#include "ds1302.h"
#include "pcf8591.h"
#include "24c02.h"

uint vol_val, fre_val, vol_low, vol_high;
uchar seg_buf[9], seg_code[8], seg_pos;
uchar state, event[4], sec;
uchar rtc[3] = {23, 59, 55};
uchar key_old;
uint seg_dly, key_dly; 
uchar led8 = 0x04;
long int ms;

void key_prog(void);
void seg_prog(void);

void main(void)
{
	Timer1Init();
	Timer0Init();
	cls_led();
	cls_buzz();
	set_rtc(rtc);
	ET1 = 1;
	ET0 = 1;
	EA = 1;

	eeprom_read(seg_buf, 0, 4);
	vol_low = (seg_buf[1]<<8) + seg_buf[0];
	vol_high = (seg_buf[3]<<8) + seg_buf[2];
	if(vol_low > 5000) vol_low = 1000;
	if(vol_high > 5000) vol_high = 2000;

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
	if(++seg_dly == 500) seg_dly = 0;
	if(!(ms % 1000))
	{
		sec++;
		fre_val = (TH0 << 8) + TL0;
		TH0 = 0;
		TL0 = 0;			 	
	}
	seg_disp(seg_code, seg_pos);
	if(++seg_pos == 8) seg_pos = 0;
}

void key_prog(void)
{
	uchar key_val, key_down;
	if(key_dly) return;
	key_dly = 1;

	key_val = key_read();
	key_down = key_val & (key_old ^ key_val);
	key_old = key_val;

	switch(key_down)
	{
		case 4: switch(state&0xf0)
				{
					case 0: if(++state == 2) state = 0; break;
					case 0x10: if(++state == 0x13) 
							   {
							   state = 0x10;
							   seg_buf[0] = vol_low & 0xff;
							   seg_buf[1] = vol_low >> 8;
							   seg_buf[2] = vol_high & 0xff;
							   seg_buf[3] = vol_high >> 8;
							   eeprom_write(seg_buf, 0, 4); 
							   }break;
					case 0x20: if(++state == 0x24) 
							   {
							       state = 0x20;
								   set_rtc(rtc);
							   }break;	
					case 0x30: if(++state == 0x32) state = 0x30; 
				}break;
		case 5:	state = 0x00;   //频率
				break;
		case 6:	state = 0x10; 	//电压
				break;
		case 7:	state = 0x20;	//时间
				break;
		case 9:	state = 0x30;	//事件
				if(state == 0x30)
				{
					eeprom_read(event, 4, 4);
					if(event[0] > 2) event[0] = 0;
					if(event[1] > 24) event[1] = 0;
					if(event[2] > 60) event[2] = 0;
					if(event[3] > 60) event[3] = 0;
				}
				break;
		case 11: switch(state)
				 {	
					case 0x11: if(vol_high <= 4500) vol_high += 500; break;
					case 0x12:	if((vol_high - vol_low) > 500) vol_low += 500; break;
				 	case 0x21: if(++rtc[0] >= 24) rtc[0] = 0; break;
					case 0x22: if(++rtc[1] >= 60) rtc[1] = 0; break;
					case 0x23: if(++rtc[2] >= 60) rtc[2] = 0; 

				 }break;	
		case 10: switch(state)
				 {						
				    case 0x11: if((vol_high - vol_low) > 500) vol_high -= 500; break;
					case 0x12: if(vol_low > 0) vol_low -= 500; break;
				 	case 0x21: if(rtc[0]) rtc[0]--;
							   else rtc[0] = 23; break;
					case 0x22: if(rtc[1]) rtc[1]--;
							   else rtc[1] = 59; break;
					case 0x23: if(rtc[2]) rtc[2]--;
							   else rtc[2] = 59; 
				 }											   
	}
	led_disp(state+0x11);
}

void seg_prog(void)
{
	if(seg_dly) return;
	seg_dly = 1;

	switch(state & 0xf0)
	{
		case 0: if(state == 0) sprintf(seg_buf, "-2-0%04u", (uint)fre_val);
				else sprintf(seg_buf, "-2-0%04u", (uint)(fre_val / 1000000)); 
				break;
		case 0x10: vol_val = pcf8591_Adc() * 19.6;
				   if((vol_val < vol_low)||(vol_val > vol_high))
				   {
				       if(vol_val < vol_low) seg_buf[0] = 0;
					   else seg_buf[0] = 1;
					   seg_buf[1] = rtc[0];
					   seg_buf[2] = rtc[1];
					   seg_buf[3] = rtc[2];
					   eeprom_write(seg_buf, 4, 4);
				   }
				   if(state == 0x10)
				       sprintf(seg_buf, "-1- %04u", vol_val);
				   else
				   {
				       sprintf(seg_buf, "%04u%04u", vol_high, vol_low);
					   if(state == 0x12)
					   {
					      seg_buf[0] = 0xFF;
						  seg_buf[1] = 0xFF;
						  seg_buf[2] = 0xFF;
						  seg_buf[3] = 0xFF;
					   }
					   else if(state == 0x11)
					   {
					   	   seg_buf[4] = 0xFF;
						   seg_buf[5] = 0xFF;
						   seg_buf[6] = 0xFF;
						   seg_buf[7] = 0xFF;
					   }
				   }break;
		case 0x20: if(!state) read_rtc(rtc);
				   sprintf(seg_buf, "%02u-%02u-%02u", (uint)rtc[0],\
				   (uint)rtc[1], (uint)rtc[2]);
				   if(sec & 1)
				       switch(state)
					   {
					       case 0x21:
						   seg_buf[0] = 0xff;
						   seg_buf[1] = 0xff;
						   break;
						   case 0x22:
						   seg_buf[3] = 0xff;
						   seg_buf[4] = 0xff;
						   break;
						   case 0x23:
						   seg_buf[6] = 0xff;
						   seg_buf[7] = 0xff;						   
					   }break;
		case 0x30: switch(state)
		           {
				       case 0x30: sprintf(seg_buf, "      %02u", (uint)event[0]);
					              break;	 
					   case 0x31: sprintf(seg_buf, "%02u-%02u-%02u", (uint)event[1],\
					                     (uint)event[2], (uint)event[3]);
				   }
	}
	seg_tran(seg_buf, seg_code);
}