
#define _GR_TEST_GLOBALS
#include "cs99xx_relay_motion.h"
#include "cs99xx_vref.h"
#include "DAC_VREF.h"
#include "test_com.h"
#include "gr_count.h"
#include "gr_test.h"
#include "gr_relay.h"

static uint16_t gr_zeo_t;///< �ک��׶ε��ۼ�ʱ��
static uint16_t gr_one_t;///< ��һ�׶ε��ۼ�ʱ��
static uint16_t gr_two_t;///< �ڶ��׶ε��ۼ�ʱ��
static uint16_t gr_thr_t;///< �����׶ε��ۼ�ʱ��
static uint16_t gr_for_t;///< ���Ľ׶ε��ۼ�ʱ��

/**
  * @brief  ����GR����ʱ����Ϣ
  * @param  [in] gr_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void load_gr_data(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data)
{
    /* ƫ�Ʋ����������ʱ */
    gr_zeo_t = 0;
    gr_one_t = 0 + gr_zeo_t;
    gr_two_t = gr_one_t + gr_par->testing_time;
    gr_thr_t = gr_two_t + 0;
    gr_for_t = gr_thr_t + gr_par->interval_time;
}

/**
  * @brief  GR��������DA�����ֵ
  * @param  [in] gr_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void gr_set_da_value(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data)
{
    test_data->output_da = (uint16_t)(gr_par->testing_cur * test_data->out_da_k + test_data->out_da_b);
    set_output_da_vref(test_data->output_da);
}

/**
  * @brief  GR����״̬��״̬���㺯��
  * @param  [in] gr_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void gr_test_irq(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data)
{
	/* ������ʱ��Ϊ0ʱ ����ʱ������� ��ǰû�з���ʧ�ܼ��� */
	if(0 == gr_par->testing_time && (!gr_test_flag.fail_cont))
	{
		/* �����ǰ�������ڽ��� �����ڵڶ��׶εĻ���һֱ���� */
		if(gr_test_flag.forever_testing)
		{
			test_data->dis_time = test_data->test_time % 10000;
            
            if(test_data->dis_time == 0)
            {
                test_data->test_time = 0;
            }
			return;
		}
		
		if(test_data->test_time <= gr_zeo_t)
		{
			test_data->dis_time = test_data->test_time - 0;
			test_data->gradation = STAGE_READY;
			gr_test_flag.vol_change_flag = 1;
			return;
		}
		/* ʹ��������һ�׶ε�ѹ���� */
		else if(test_data->test_time <= gr_one_t)/* ������ʱ����Ϊ0ʱ���Ͳ��ܽ���if,���Ҫ+1 */
		{
			test_data->dis_time = test_data->test_time - gr_zeo_t;
			test_data->gradation = STAGE_RISE;
			gr_test_flag.vol_change_flag = 1;
			return;
		}
		
        if(test_data->test_time == gr_one_t + 1)
        {
            gr_set_da_value(gr_par, test_data);
        }
        
		if(test_data->fail_num != ST_ERR_NONE)
		{
			if(test_data->test_time < gr_for_t)
			{
				/* ���������ر� */
				if(!gr_par->steps_cont)
				{
                    gr_exit_test_relay_motion(gr_par, test_data);
                    test_data->test_over = 1;
                    return;
				}
				test_data->dis_time = test_data->test_time - gr_thr_t + 1;
				test_data->gradation = STAGE_INTER;
				return;
			}
			else
			{
                gr_exit_test_relay_motion(gr_par, test_data);
                test_data->test_over = 1;
                return;
			}
		}
		
		gr_test_flag.vol_change_flag = 0;
		test_data->gradation = STAGE_TEST;
		test_data->dis_time = test_data->test_time - gr_one_t;
		
		if(test_data->dis_time >= 9999)
		{
            test_data->test_time = 0;
			gr_test_flag.forever_testing = 1;/* ������ʱ����� */
		}
		return;
	}
	
	/* ���Ծ��� */
	if(test_data->test_time <= gr_zeo_t)
	{
		test_data->dis_time = test_data->test_time;
		test_data->gradation = STAGE_READY;
        gr_test_flag.vol_change_flag = 1;
	}
	/* ʹ��������һ�׶ε�ѹ���� */
	else if(test_data->test_time <= gr_one_t)
	{
		test_data->dis_time = test_data->test_time - gr_zeo_t;
		test_data->gradation = STAGE_RISE;
		gr_test_flag.vol_change_flag = 1;
	}
	/* ����ڶ��׶� ���� */
	else if(test_data->test_time <= gr_two_t)
	{
		test_data->dis_time = test_data->test_time - gr_one_t;
		test_data->gradation = STAGE_TEST;
		gr_test_flag.vol_change_flag = 0;
        
        if(test_data->test_time == gr_one_t + 1)
        {
            gr_set_da_value(gr_par, test_data);
        }
	}
	/* �����׶� ��ѹ�½� */
	else if(test_data->test_time <= gr_thr_t)
	{
		gr_test_flag.vol_change_flag = 1;
		test_data->dis_time = test_data->test_time - gr_two_t;
		test_data->gradation = STAGE_FALL;
	}
	/* ���Ľ׶� ����ȴ� */
	else if(test_data->test_time <= gr_for_t)
	{
        /* �ظ�ѹ */
        if(gr_test_flag.testing)
        {
            gr_test_flag.testing = 0;
            gr_exit_test_relay_motion(gr_par, test_data);
        }
        
		/* ���������ر� */
		if(!gr_par->steps_cont)
		{
            gr_exit_test_relay_motion(gr_par, test_data);
            test_data->test_over = 1;
            return;
		}
        
		test_data->dis_time = test_data->test_time - gr_thr_t;
		test_data->gradation = STAGE_INTER;
	}
	/* ��ǰ�����Խ��� */
	else
	{
        /* �ظ�ѹ */
        if(gr_test_flag.testing)
        {
            gr_test_flag.testing = 0;
            gr_exit_test_relay_motion(gr_par, test_data);
        }
        
        /* ���������� */
		if(gr_par->steps_cont)
		{
            test_data->cont = 1;
            return;
        }
        
        /* �������� */
        test_data->test_over = 1;
	}
}

/**
  * @brief  GR����ǰ��׼��
  * @param  [in] gr_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void gr_test_ready(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data)
{
    test_vref(gr_par->upper_limit);	/* �������׼ */
    
    gr_into_test_relay_ready(gr_par, test_data);/* �̵������� */
    amp_relay_ctrl_on();/* ���ż̵��� */
    cs_delay_ms(10);
    
    gr_relay_ready();//�����ӿ���
    open_sine(gr_par->output_freq);/* �����Ҳ� */
    gr_set_da_value(gr_par, test_data);//�ͻ�׼
    open_test_timer();/* ����ʱ�� */
    
    test_data->test_over = 0;
    test_data->dis_time = 0;
    test_data->test_status = ST_WAIT;
    test_data->fail_num = ST_ERR_NONE;//Ĭ�ϳ�ʼ��Ϊ�ϸ�
}

/**
  * @brief  ����GR����״̬��
  * @param  [in] gr_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void gr_test_details(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data)
{
	switch(test_data->gradation)
	{
		case STAGE_READY:/* �ک��׶� */
        {
            if(test_data->ready_ok == 0)
            {
                test_data->ready_ok = 1;//���
                gr_test_flag.testing = 1;//���ڲ��Ա��
                gr_test_flag.forever_testing = 0;
                gr_test_flag.fail_cont = 0;
                gr_test_flag.judge_err_en = ENABLE;
                load_gr_data(gr_par, test_data);
                gr_test_ready(gr_par, test_data);
            }
            break;
        }
        case STAGE_FAIL_CONT:/* ʧ�ܶ��� */
        {
            gr_test_flag.fail_cont = 1;//����ʧ�ܼ�������������1
            
            /* ���ʱ�䲻Ϊ0�ʹӼ��ʱ�俪ʼ */
            if(gr_par->interval_time > 0)
            {
                test_data->ready_ok = 1;
                test_data->test_time = gr_thr_t + 1;/* ������1�׶� */
                test_data->gradation = STAGE_INTER;/* �������ȴ� */
                exit_sw();//���ж�
                open_test_timer();/* ����ʱ�� */
            }
            /* ���ʱ��Ϊ0�ͼ�����һ������ */
            else
            {
                test_data->cont = 1;
            }
            break;
        }
		case STAGE_RISE:/* ��һ�׶� ��ѹ���� */
            test_data->test_status = ST_VOL_RISE;
			break;
		case STAGE_TEST:/* �ڶ��׶� ���ڲ��� */
            test_data->test_status = ST_TESTING;
			break;
		case STAGE_FALL:/* �����׶� ��ѹ�½� */
            test_data->test_status = ST_VOL_FALL;
			break;
		case STAGE_INTER:/* ���Ľ׶� ����ȴ� */
            test_data->test_status = ST_INTER_WAIT;
			break;
	}
}
/**
  * @brief  ����GR����
  * @param  [in] step ��ǰ������������
  * @param  [in] next_step ��һ������������ ��� Ϊ��˵����ǰ���Ե������һ��
  * @param  [in] test_data ��������
  * @retval ��
  */
void run_gr_test(NODE_STEP *step, NODE_STEP *next_step, TEST_DATA_STRUCT *test_data)
{
    GR_STRUCT *gr_par = &step->one_step.gr;
    
    /* ׼��������ɺ��ڼ������״̬ */
    if(test_data->ready_ok)
    {
        gr_test_irq(gr_par, test_data);
    }
    
    /* ����״̬�� */
    gr_test_details(gr_par, test_data);
    
    /* ���ֱ������ٽ��в��� ���Խ������ٽ��в��� */
    if((test_data->fail_num) == ST_ERR_NONE && (!test_data->test_over))
    {
        /* ����׶β����в��� */
        if(test_data->gradation != STAGE_INTER)
        {
            gr_count_dis_value(gr_par, test_data);
        }
    }
    
    /* ���Գ����쳣��������Ծ������ */
    if(test_data->fail_num != ST_ERR_NONE)
    {
        test_data->ready_ok = 0;
    }
}

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
