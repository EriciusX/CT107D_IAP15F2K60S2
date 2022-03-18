/*--------------------------------
需要去Target Options里的C51选项中，
在Preprocessor Symbols下的Define中，
输入#ifdef后的定义，才可定义对应函数
---------------------------------*/


#include "iic.h"
#include "pcf8591.h"


/*
* @brief AC转换 
* @Param None
*/
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
/*
* @brief DC转换 
* @Param uchar dat 
*/
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