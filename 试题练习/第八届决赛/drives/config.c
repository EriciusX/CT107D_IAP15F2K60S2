#include "type.h"
#include "config.h"
/*
* @brief ��ʱn���� 
* @Param uint n ��ʱʱ����� 
*/
void delay_ms(uint n) 
{
    unsigned int i;
    while(n--)
        for(i = 0; i < 628; i ++);
}
/*
* @brief ��ʱn�� 
* @Param uint n ��ʱʱ����� 
*/
void delay_s(uint n) //��ʱn��
{
    unsigned int i=0,j=0;
    for(i=0;i<n;i++)
        for(j=0;j<21738;j++);
}
/*
* @brief ѡ�������� 
* @Param uint channel ������ͨ�� 
*/
void Select_HC573(uint channel)  
{
	switch(channel)
	{
		case 4: P2 = P2 & 0x1F | 0x80; break;
		case 5:	P2 = P2 & 0x1F | 0xA0; break;
		case 6: P2 = P2 & 0x1F | 0xC0; break;
		case 7: P2 = P2 & 0x1F | 0xE0; break;
	}
}
/*
* @brief ��ʼ��LED������ 
* @Param None 
*/
void cls_led(void)
{
	P0 = 0xFF;
	Select_HC573(4);
	P2 &= 0x1F;
}
/*
* @brief ��ʼ���������ͼ̵������� 
* @Param None
*/
void cls_buzz(void)
{
	P0 = 0;
	Select_HC573(5);
	P2 &= 0x1F;
}