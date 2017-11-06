
#define _DCW_TEST_GLOBALS
#include "cs99xx_relay_motion.h"
#include "cs99xx_vref.h"
#include "DAC_VREF.h"
#include "test_com.h"
#include "dcw_count.h"
#include "dcw_test.h"
#include "dcw_relay.h"

static uint16_t dcw_zeo_t;///< �ک��׶ε��ۼ�ʱ��
static uint16_t dcw_one_t;///< ��һ�׶ε��ۼ�ʱ��
static uint16_t dcw_two_t;///< �ڶ��׶ε��ۼ�ʱ��
static uint16_t dcw_thr_t;///< �����׶ε��ۼ�ʱ��
static uint16_t dcw_for_t;///< ���Ľ׶ε��ۼ�ʱ��

/**
  * @brief  ����DCW����ʱ����Ϣ
  * @param  [in] dcw_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void load_dcw_data(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
    dcw_zeo_t = 0;
    
    dcw_one_t = dcw_par->rise_time + dcw_zeo_t;
    dcw_two_t = dcw_par->rise_time + dcw_par->testing_time + dcw_zeo_t;
    dcw_thr_t = dcw_par->rise_time + dcw_par->testing_time + dcw_par->fall_time + dcw_zeo_t;
    dcw_for_t = dcw_par->rise_time + dcw_par->testing_time + dcw_par->fall_time + dcw_par->interval_time + dcw_zeo_t;
}

/**
  * @brief  ����DCW��ѹ�����ĵ�ѹ����ֵ
  * @param  [in] dcw_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void dcw_count_rise_vol_step_value(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
	test_data->vol_ch_base = 0;
    test_data->vol_ch_target = dcw_par->testing_voltage;
    
	if(dcw_par->rise_time != 0)
	{
		test_data->vol_ch_step =
            (float)(test_data->vol_ch_target - test_data->vol_ch_base) / dcw_par->rise_time;
	}
}

/**
  * @brief  ����DCW��ѹ�½��ĵ�ѹ����ֵ
  * @param  [in] dcw_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void dcw_count_fall_vol_step_value(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
	test_data->vol_ch_base = dcw_par->testing_voltage;
    test_data->vol_ch_target = 0;
    
	if(dcw_par->fall_time != 0)
	{
		test_data->vol_ch_step =
            (float)(test_data->vol_ch_target - test_data->vol_ch_base) / dcw_par->fall_time;
	}
}

/**
  * @brief  ����DCW�ڲ��Թ����е�ѹ�仯�Ĳ���ֵ�Ϳ�ʼֵ��Ŀ��ֵ
  * @param  [in] dcw_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void dcw_count_vol_ch_step(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
    test_data->vol_rise_step_t = 0;
    
    switch(test_data->gradation)
    {
        case STAGE_CHARGE://���
        case STAGE_RISE://����
            dcw_count_rise_vol_step_value(dcw_par, test_data);
            break;
        case STAGE_FALL://�½�
            dcw_count_fall_vol_step_value(dcw_par, test_data);
            break;
    }
}

/**
  * @brief  �����Թ������½�ʱ�䵽��Ҫ��Ϊ����0��������
  * @param  [in] dcw_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void dcw_clear_test_data_fall_time_timeout(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
    /* ����½�ʱ�䲻Ϊ0����0���Ե�ѹ�͵��� */
    if(dcw_par->fall_time > 0)
    {
        test_data->vol_value = 0;
        test_data->cur_value = 0;
        test_data->real_value = 0;
    }
}

/**
  * @brief  DCW��������DA�����ֵ
  * @param  [in] gr_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void dcw_set_da_value(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
    test_data->output_da = (uint16_t)(dcw_par->testing_voltage * test_data->out_da_k + test_data->out_da_b);
    set_output_da_vref(test_data->output_da);
}

/**
  * @brief  DCW����״̬��״̬���㺯��
  * @param  [in] gr_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void dcw_test_irq(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
	/* ������ʱ��Ϊ0ʱ ����ʱ������� ��ǰû�з���ʧ�ܼ��� */
	if(0 == dcw_par->testing_time && (!dcw_test_flag.fail_cont))
	{
		/* �����ǰ�������ڽ��� �����ڵڶ��׶εĻ���һֱ���� */
		if(dcw_test_flag.forever_testing)
		{
			test_data->dis_time = test_data->test_time % 10000;
            
            if(test_data->dis_time == 0)
            {
                test_data->test_time = 0;
            }
			return;
		}
		
		if(test_data->test_time <= dcw_zeo_t)
		{
			test_data->dis_time = test_data->test_time - 0;
			test_data->gradation = STAGE_READY;
			dcw_test_flag.vol_change_flag = 1;
			return;
		}
		/* ʹ��������һ�׶ε�ѹ���� */
		else if(test_data->test_time <= dcw_one_t)/* ������ʱ����Ϊ0ʱ���Ͳ��ܽ���if,���Ҫ+1 */
		{
			test_data->dis_time = test_data->test_time - dcw_zeo_t;
			test_data->gradation = STAGE_RISE;
			dcw_test_flag.vol_change_flag = 1;
            
            if(test_data->dis_time == 1)
            {
                dcw_count_vol_ch_step(dcw_par, test_data);
            }
			return;
		}
		
        if(test_data->test_time == dcw_one_t + 1)
        {
            dcw_set_da_value(dcw_par, test_data);
        }
        
		if(test_data->fail_num != ST_ERR_NONE)
		{
			if(test_data->test_time < dcw_for_t)
			{
				/* ���������ر� */
				if(!dcw_par->steps_cont)
				{
                    dcw_exit_test_relay_motion(dcw_par, test_data);
                    test_data->test_over = 1;
                    return;
				}
				test_data->dis_time = test_data->test_time - dcw_thr_t + 1;
				test_data->gradation = STAGE_INTER;
				return;
			}
			else
			{
                dcw_exit_test_relay_motion(dcw_par, test_data);
                test_data->test_over = 1;
                return;
			}
		}
		
		dcw_test_flag.vol_change_flag = 0;
		test_data->gradation = STAGE_TEST;
		test_data->dis_time = test_data->test_time - dcw_one_t;
		
		if(test_data->dis_time >= 9999)
		{
            test_data->test_time = 0;
			dcw_test_flag.forever_testing = 1;/* ������ʱ����� */
		}
		return;
	}
	
	/* ���Ծ��� */
	if(test_data->test_time <= dcw_zeo_t)
	{
		test_data->dis_time = test_data->test_time;
		test_data->gradation = STAGE_READY;
        dcw_test_flag.vol_change_flag = 1;
	}
	/* ʹ��������һ�׶ε�ѹ���� */
	else if(test_data->test_time <= dcw_one_t)
	{
		test_data->dis_time = test_data->test_time - dcw_zeo_t;
		test_data->gradation = STAGE_RISE;
		dcw_test_flag.vol_change_flag = 1;
        
        if(test_data->dis_time == 1)
        {
            dcw_count_vol_ch_step(dcw_par, test_data);
        }
	}
	/* ����ڶ��׶� ���� */
	else if(test_data->test_time <= dcw_two_t)
	{
		test_data->dis_time = test_data->test_time - dcw_one_t;
		test_data->gradation = STAGE_TEST;
		dcw_test_flag.vol_change_flag = 0;
        
        if(test_data->test_time == dcw_one_t + 1)
        {
            dcw_set_da_value(dcw_par, test_data);
        }
	}
	/* �����׶� ��ѹ�½� */
	else if(test_data->test_time <= dcw_thr_t)
	{
		dcw_test_flag.vol_change_flag = 1;
		test_data->dis_time = test_data->test_time - dcw_two_t;
		test_data->gradation = STAGE_FALL;
        
        if(test_data->dis_time == 1)
        {
            dcw_count_vol_ch_step(dcw_par, test_data);
        }
	}
	/* ���Ľ׶� ����ȴ� */
	else if(test_data->test_time <= dcw_for_t)
	{
        dcw_clear_test_data_fall_time_timeout(dcw_par, test_data);
        
        /* �ظ�ѹ */
        if(dcw_test_flag.testing)
        {
            dcw_test_flag.testing = 0;
            dcw_exit_test_relay_motion(dcw_par, test_data);
        }
        
		/* ���������ر� */
		if(!dcw_par->steps_cont)
		{
            dcw_exit_test_relay_motion(dcw_par, test_data);
            test_data->test_over = 1;
            return;
		}
        
		test_data->dis_time = test_data->test_time - dcw_thr_t;
		test_data->gradation = STAGE_INTER;
	}
	/* ��ǰ�����Խ��� */
	else
	{
        dcw_clear_test_data_fall_time_timeout(dcw_par, test_data);
        
        /* �ظ�ѹ */
        if(dcw_test_flag.testing)
        {
            dcw_test_flag.testing = 0;
            dcw_exit_test_relay_motion(dcw_par, test_data);
        }
        
        /* ���������� */
		if(dcw_par->steps_cont)
		{
            test_data->cont = 1;
            return;
        }
        
        /* �������� */
        test_data->test_over = 1;
	}
}

/**
  * @brief  DCW����ǰ��׼��
  * @param  [in] gr_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void dcw_test_ready(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
    test_vref(dcw_par->upper_limit);	/* �������׼ */
    
	updata_port(&dcw_par->port);/* ���¶˿�״̬ */
    dcw_into_test_relay_ready(dcw_par, test_data);/* �̵������� */
    amp_relay_ctrl_on();/* ���ż̵��� */
    cs_delay_ms(10);
    
    dcw_relay_ready();//�����ӿ���
    
    open_sine(4000);/* �����Ҳ� */
    
    if(dcw_par->rise_time == 0)
    {
        dcw_set_da_value(dcw_par, test_data);//�ͻ�׼
    }
    
    open_test_timer();/* ����ʱ�� */
    
    test_data->test_over = 0;
    test_data->dis_time = 0;
    test_data->test_status = ST_WAIT;
    test_data->fail_num = ST_ERR_NONE;//Ĭ�ϳ�ʼ��Ϊ�ϸ�
}

/**
  * @brief  ����DCW����״̬��
  * @param  [in] gr_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void dcw_test_details(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
	switch(test_data->gradation)
	{
		case STAGE_READY:/* �ک��׶� */
        {
            if(test_data->ready_ok == 0)
            {
                test_data->ready_ok = 1;//���
                dcw_test_flag.testing = 1;//���ڲ��Ա��
                dcw_test_flag.forever_testing = 0;
                dcw_test_flag.judge_err_en = ENABLE;
                dcw_test_flag.fail_cont = 0;
                dcw_test_flag.err_charge = 1;//��籨�������1 ֻ�е������ڳ�����ʱ���ܽ������
                load_dcw_data(dcw_par, test_data);
                dcw_test_ready(dcw_par, test_data);
            }
            break;
        }
        case STAGE_FAIL_CONT:
        {
            dcw_test_flag.fail_cont = 1;//����ʧ�ܼ�������������1
            
            /* ���ʱ�䲻Ϊ0�ʹӼ��ʱ�俪ʼ */
            if(dcw_par->interval_time > 0)
            {
                test_data->ready_ok = 1;
                test_data->test_time = dcw_thr_t + 1;/* ������1�׶� */
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
  * @brief  ����DCW����
  * @param  [in] step ��ǰ������������
  * @param  [in] next_step ��һ������������ ��� Ϊ��˵����ǰ���Ե������һ��
  * @param  [in] test_data ��������
  * @retval ��
  */
void run_dcw_test(NODE_STEP *step, NODE_STEP *next_step, TEST_DATA_STRUCT *test_data)
{
    DCW_STRUCT *dcw_par = &step->one_step.dcw;
    
    /* ׼��������ɺ��ڼ������״̬ */
    if(test_data->ready_ok)
    {
        dcw_test_irq(dcw_par, test_data);
    }
    
    /* ����״̬�� */
    dcw_test_details(dcw_par, test_data);
    
    /* ���ֱ������ٽ��в��� ���Խ������ٽ��в��� */
    if((test_data->fail_num) == ST_ERR_NONE && (!test_data->test_over))
    {
        /* ����׶β����в��� */
        if(test_data->gradation != STAGE_INTER)
        {
            dcw_count_dis_value(dcw_par, test_data);
        }
    }
    
    /* ���Գ����쳣��������Ծ������ */
    if(test_data->fail_num != ST_ERR_NONE)
    {
        test_data->ready_ok = 0;
    }
}

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
