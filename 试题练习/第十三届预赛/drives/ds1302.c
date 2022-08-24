#include "ds1302.h"
#include "type.h"  									

//写字节
void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK = 0;
		SDA = temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

//向DS1302寄存器写入数据
void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

//从DS1302寄存器读出数据
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

void set_rtc(unsigned char *rtc)
{
	uchar temp;
	Write_Ds1302_Byte(0x8E, 0);
	
	temp = ((rtc[0] / 10) << 4) + (rtc[0] % 10);
	Write_Ds1302_Byte(0x84, temp);
	temp = ((rtc[1] / 10) << 4) + (rtc[1] % 10);
	Write_Ds1302_Byte(0x82, temp);
	temp = ((rtc[2] / 10) << 4) + (rtc[2] % 10);
	Write_Ds1302_Byte(0x80, temp);
	
	Write_Ds1302_Byte(0x8E, 0x80);	
}

void read_rtc(unsigned char *rtc)
{
	uchar temp;
	temp = Read_Ds1302_Byte(0x85);
	rtc[0] = ((temp >> 4) * 10) + temp & 0x0F;
	temp = Read_Ds1302_Byte(0x83);
	rtc[1] = ((temp >> 4) * 10) + temp & 0x0F;
	temp = Read_Ds1302_Byte(0x81);
	rtc[2] = ((temp >> 4) * 10) + temp & 0x0F; 
}

