#include "type.h"
#include "config.h"
#include "led.h"
#include "keys.h"
#include "seg.h"
#include "timer.h" 
#include "onewire.h"
#include "stdio.h"

sbit pwm_out = P3^4;
uchar temp_flag; 
uchar temp_val;
uchar pwm;	 
uchar mode_state = 3, pwm_state = 7;
uchar seg_buf[9], seg_code[8], seg_pos;
uchar key_dly, key_old;
uchar sec, delay;
uint seg_dly; 
uchar led8 = 0x04;
long int ms;

void key_prog(void);
void seg_prog(void);

void main(void)
{
	Timer0Init();
	Timer1Init();
	cls_led();
	cls_buzz();

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
	if(sec)
	{
		if(++pwm == 10) pwm = 0;
		if(!pwm)
		{
			pwm_out = 1;
			led8 |= 0x80; 
		}
		else if(pwm == pwm_state)
		{
			pwm_out = 0;
			led8 &= 0x7F;
		}
	}
	else led8 &= 0x7F;
	led_disp(led8);
}

void timer1(void) interrupt 3
{
	ms++;
	if(++key_dly == 10) key_dly = 0;
	if(++seg_dly == 500) seg_dly = 0;
	if(!(ms % 1000))
	{
		if(delay) delay--;
		else if(sec) sec--;
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
		case 4: if(++mode_state == 4) mode_state = 1;
				switch(mode_state)
				{
					case 1: led8 = 0x01; pwm_state = 2;
							break;
					case 2:	led8 = 0x02; pwm_state = 5;
							break;
					case 3:	led8 = 0x04; pwm_state = 7;
							break;
				}break;				
		case 5: sec += 60;
				if(sec >= 180) sec = 0;
				else if(sec >= 120) sec = 120;
				else if(sec >= 60) sec = 60;
				delay = 2; break;
		case 9:	sec = 0; break;			
		case 8: temp_flag ^= 1;
	}
}

void seg_prog(void)
{
	if(seg_dly) return;
	seg_dly = 1;

	if(!temp_flag)
		sprintf(seg_buf, "-%1u- %04u", (uint)mode_state, (uint)sec);
	else
	{
		temp_val = rd_temperature();
		sprintf(seg_buf, "-4- %2u.%1uC", (uint)temp_val/10, (uint)temp_val%10);
	}
	seg_tran(seg_buf, seg_code);
}

