
#include "test_tools.h"


/*
 * ��������soft_delay_us
 * ����  �����������ʱ10us ����
 * ����  ��dly_us ����
 * ���  ����
 * ����  ����
 */
void cs_delay_us(int time)
{
	int i = 0;
	
	while(time--)
	{
		i = 10; //�Լ�����
		
		while(i--);
	}
}
void cs_delay_ms(int time)
{
	int i = 0;
	
	while(time--)
	{
		i = 12000;  //�Լ�����
		while(i--);
	}
}


/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
