
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


void init_com_test_env(TEST_DATA_STRUCT *test_data)
{
    g_test_data.gradation = STAGE_READY;//�л������Ծ���״̬
    g_test_data.test_time = 0;//����ʱ����0
    g_test_data.ready_ok = 0;//���Ծ������
    g_test_data.fail_num = ST_ERR_NONE;//��ʼ��
}

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
