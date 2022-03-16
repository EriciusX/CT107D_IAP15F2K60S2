#include "iic.h"
#include "pcf8591.h"



#ifdef pcf8591_adc
uchar pcf8591_Adc(void)
{
	uchar temp;
	
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();

	IIC_SendByte(0x43);
	IIC_WaitAck();

	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();

	temp = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	return temp;
}
#endif
#ifdef pcf8591_dac
void pcf8591_Dac(uchar dat)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();

	IIC_SendByte(0x43);
	IIC_WaitAck();

	IIC_SendByte(dat);
	IIC_SendAck();
	IIC_Stop();	
}
#endif