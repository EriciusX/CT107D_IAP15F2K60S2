/*
  程序说明: DS1302驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台 8051，12MHz
  日    期: 2011-8-9
*/

#include <intrins.h>
#include "ds1302.h"

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;   // DS1302复位												

void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK=0;
		SDA=temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}

void set_rtc(unsigned char* pucrtc)
{
	uchar temp;	
	Write_Ds1302_Byte(0x8E, 0); //允许写操作

	temp = ((pucrtc[0] / 10) << 4) + pucrtc[0] % 10;	//时
	Write_Ds1302_Byte(0x84, temp);
	temp = ((pucrtc[1] / 10) << 4) + pucrtc[1] % 10;	//分
	Write_Ds1302_Byte(0x82, temp);
	temp = ((pucrtc[2] / 10) << 4) + pucrtc[2] % 10;	//秒
	Write_Ds1302_Byte(0x80, temp);
	Write_Ds1302_Byte(0x8E, 0x80); //禁止写操作
}

void read_rtc(unsigned char* pucrtc)
{
	uchar temp;
	temp = Read_Ds1302_Byte(0x85);
	pucrtc[0] = (temp >> 4) * 10 + (temp & 0x0f);
	temp = Read_Ds1302_Byte(0x83);
	pucrtc[1] = (temp >> 4) * 10 + (temp & 0x0f);
	temp = Read_Ds1302_Byte(0x81);
	pucrtc[2] = (temp >> 4) * 10 + (temp & 0x0f);
}








