
#include "test_com.h"
#include "Timer_Config.h"
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

/*
 * ��������close_test_timer
 * ����  ���رղ��Զ�ʱ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void close_test_timer(void)
{
    bsp_tim2_close();/* �ض�ʱ�� */
}
/*
 * ��������open_test_timer
 * ����  ���򿪲��Զ�ʱ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void open_test_timer(void)
{
    bsp_tim2_open();/* ����ʱ�� */
}

