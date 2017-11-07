
#include "test_com.h"
#include "Timer_Config.h"

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
    g_test_data.vol_err_count = 0;
    g_test_data.cur_err_count = 0;
    g_test_data.low_err_count = 0;
}

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
