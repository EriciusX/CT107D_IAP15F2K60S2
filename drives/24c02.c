#include "24c02.h"
#include "iic.h"


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
	operate_delay(20);
		
}
#endif