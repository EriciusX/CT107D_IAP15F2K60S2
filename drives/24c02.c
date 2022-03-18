/*--------------------------------
需要去Target Options里的C51选项中，
在Preprocessor Symbols下的Define中，
输入#ifdef后的定义，才可定义对应函数
---------------------------------*/
#include "24c02.h"
#include "iic.h"
#include "config.h"
/*
* @brief 读取ROM 
* @Param uchar *pucbuf 存放数组，uchar addr ROM起始地址，uchar num 长度 
*/
#ifdef eeprom_at24c02
void eeprom_read(uchar *pucbuf, uchar addr, uchar num)
{
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();

	IIC_SendByte(addr);
	IIC_WaitAck();

	IIC_Start();
	IIC_SendByte(0xA1);
	IIC_WaitAck();

	while(num--)
	{
		*pucbuf++ = IIC_RecByte();
		if(num) IIC_SendAck(0);
		else IIC_SendAck(1);
	}
	IIC_Stop();
}
/*
* @brief 写入ROM 
* @Param uchar *pucbuf 存放数组，uchar addr ROM起始地址，uchar num 长度
*/
void eeprom_write(uchar *pucbuf, uchar addr, uchar num)
{
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();

	IIC_SendByte(addr);
	IIC_WaitAck();

	while(num--)
	{
		IIC_SendByte(*pucbuf++);
		IIC_WaitAck();
		IIC_Delay(200);
	}
	IIC_Stop();
	delay_ms(20);	
}
#endif