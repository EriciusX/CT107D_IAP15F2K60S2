#include "onewire.h"

//�������ڲ���ʱ����
void Delay_OneWire(unsigned int t)  
{
	t = t * 10;
	while(t--);
}

//������д����
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//�����߶�����
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//DS18B20��ʼ��
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  	Delay_OneWire(10); 
    initflag = DQ;     
  	Delay_OneWire(5);
  
  	return initflag;
}

uint rd_temp(void)
{
	uint low, high;

	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0x44);


	Delay_OneWire(10);
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0xBE);

	low = Read_DS18B20();
	high = Read_DS18B20();
	
	return (high << 8) + low; 	
}

