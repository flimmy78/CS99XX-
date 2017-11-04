
#define _ACW_TEST_GLOBALS
#include "acw_test.h"
#include "acw_relay.h"
#include "cs99xx_relay_motion.h"
#include "cs99xx_vref.h"
#include "test_com.h"
#include "acw_count.h"
#include "DAC_VREF.h"

static uint16_t acw_zeo_t;///< �ک��׶ε��ۼ�ʱ��
static uint16_t acw_one_t;///< ��һ�׶ε��ۼ�ʱ��
static uint16_t acw_two_t;///< �ڶ��׶ε��ۼ�ʱ��
static uint16_t acw_thr_t;///< �����׶ε��ۼ�ʱ��
static uint16_t acw_for_t;///< ���Ľ׶ε��ۼ�ʱ��

void load_acw_data(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    /* ƫ�Ʋ����������ʱ */
    acw_zeo_t = 0;
    
    acw_one_t = acw_par->rise_time + acw_zeo_t;
    acw_two_t = acw_par->rise_time + acw_par->testing_time + acw_zeo_t;
    acw_thr_t = acw_par->rise_time + acw_par->testing_time + acw_par->fall_time + acw_zeo_t;
    acw_for_t = acw_par->rise_time + acw_par->testing_time + acw_par->fall_time + acw_par->interval_time + acw_zeo_t;
}

/*
 * ��������count_rise_vol_step_value
 * ����  �����㵱ǰ����ѹ�����ĵ�ѹ����ֵ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void acw_count_rise_vol_step_value(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
	test_data->vol_ch_base = 0;
    test_data->vol_ch_target = acw_par->testing_voltage;
    
	if(acw_par->rise_time != 0)
	{
		test_data->vol_ch_step =
            (float)(test_data->vol_ch_target - test_data->vol_ch_base) / acw_par->rise_time;
	}
}
/*
 * ��������count_vol_step_value
 * ����  �����㵱ǰ����ѹ�½��ĵ�ѹ����ֵ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void acw_count_fall_vol_step_value(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
	test_data->vol_ch_base = acw_par->testing_voltage;
    test_data->vol_ch_target = 0;
    
	if(acw_par->fall_time != 0)
	{
		test_data->vol_ch_step =
            (float)(test_data->vol_ch_target - test_data->vol_ch_base) / acw_par->fall_time;
	}
}

void acw_count_vol_ch_step(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    test_data->vol_rise_step_t = 0;
    
    switch(test_data->gradation)
    {
        case STAGE_CHARGE://���
        case STAGE_RISE://����
            acw_count_rise_vol_step_value(acw_par, test_data);
            break;
        case STAGE_FALL://�½�
            acw_count_fall_vol_step_value(acw_par, test_data);
            break;
    }
}

void clear_test_data_fall_time_timeout(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    /* ����½�ʱ�䲻Ϊ0����0���Ե�ѹ�͵��� */
    if(acw_par->fall_time > 0)
    {
        test_data->vol_value = 0;
        test_data->cur_value = 0;
        test_data->real_value = 0;
    }
}

void acw_set_da_value(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    test_data->output_da =  (u16)(acw_par->testing_voltage * test_data->out_da_k + test_data->out_da_b);
    set_output_da_vref(test_data->output_da);
}
/*
 * ��������test_irq
 * ����  ������ʱ�ӿ��ƣ��������������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void acw_test_irq(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
	/* ������ʱ��Ϊ0ʱ ����ʱ������� */
	if(0 == acw_par->testing_time)
	{
		/* �����ǰ�������ڽ��� �����ڵڶ��׶εĻ���һֱ���� */
		if(acw_test_flag.forever_testing)
		{
			test_data->dis_time = test_data->test_time % 10000;
            
            if(test_data->dis_time == 0)
            {
                test_data->test_time = 0;
            }
			return;
		}
		
		if(test_data->test_time <= acw_zeo_t)
		{
			test_data->dis_time = test_data->test_time - 0;
			test_data->gradation = STAGE_READY;
			acw_test_flag.vol_change_flag = 1;
			return;
		}
		/* ʹ��������һ�׶ε�ѹ���� */
		else if(test_data->test_time <= acw_one_t)/* ������ʱ����Ϊ0ʱ���Ͳ��ܽ���if,���Ҫ+1 */
		{
			test_data->dis_time = test_data->test_time - acw_zeo_t;
			test_data->gradation = STAGE_RISE;
			acw_test_flag.vol_change_flag = 1;
            
            if(test_data->dis_time == 1)
            {
                acw_count_vol_ch_step(acw_par, test_data);
            }
			return;
		}
		
        if(test_data->test_time == acw_one_t + 1)
        {
            acw_set_da_value(acw_par, test_data);
        }
        
		if(test_data->fail_num != ST_ERR_NONE)
		{
			if(test_data->test_time < acw_for_t)
			{
				/* ���������ر� */
				if(!acw_par->steps_cont)
				{
                    acw_exit_test_relay_motion(acw_par, test_data);
                    test_data->test_over = 1;
                    return;
				}
				test_data->dis_time = test_data->test_time - acw_thr_t + 1;
				test_data->gradation = STAGE_INTER;
				return;
			}
			else
			{
                acw_exit_test_relay_motion(acw_par, test_data);
                test_data->test_over = 1;
                return;
			}
		}
		
		acw_test_flag.vol_change_flag = 0;
		test_data->gradation = STAGE_TEST;
		test_data->dis_time = test_data->test_time - acw_one_t;
		
		if(test_data->dis_time >= 9999)
		{
            test_data->test_time = 0;
			acw_test_flag.forever_testing = 1;/* ������ʱ����� */
		}
		return;
	}
	
	/* ���Ծ��� */
	if(test_data->test_time <= acw_zeo_t)
	{
		test_data->dis_time = test_data->test_time;
		test_data->gradation = STAGE_READY;
        acw_test_flag.vol_change_flag = 1;
	}
	/* ʹ��������һ�׶ε�ѹ���� */
	else if(test_data->test_time <= acw_one_t)
	{
		test_data->dis_time = test_data->test_time - acw_zeo_t;
		test_data->gradation = STAGE_RISE;
		acw_test_flag.vol_change_flag = 1;
        
        if(test_data->dis_time == 1)
        {
            acw_count_vol_ch_step(acw_par, test_data);
        }
	}
	/* ����ڶ��׶� ���� */
	else if(test_data->test_time <= acw_two_t)
	{
		test_data->dis_time = test_data->test_time - acw_one_t;
		test_data->gradation = STAGE_TEST;
		acw_test_flag.vol_change_flag = 0;
        
        if(test_data->test_time == acw_one_t + 1)
        {
            acw_set_da_value(acw_par, test_data);
        }
	}
	/* �����׶� ��ѹ�½� */
	else if(test_data->test_time <= acw_thr_t)
	{
		acw_test_flag.vol_change_flag = 1;
		test_data->dis_time = test_data->test_time - acw_two_t;
		test_data->gradation = STAGE_FALL;
        
        if(test_data->dis_time == 1)
        {
            acw_count_vol_ch_step(acw_par, test_data);
        }
	}
	/* ���Ľ׶� ����ȴ� */
	else if(test_data->test_time <= acw_for_t)
	{
        clear_test_data_fall_time_timeout(acw_par, test_data);
        
        /* �ظ�ѹ */
        if(acw_test_flag.testing == 1)
        {
            acw_test_flag.testing = 0;
            acw_exit_test_relay_motion(acw_par, test_data);
        }
        
		/* ���������ر� */
		if(!acw_par->steps_cont)
		{
            acw_exit_test_relay_motion(acw_par, test_data);
            test_data->test_over = 1;
            return;
		}
        
		test_data->dis_time = test_data->test_time - acw_thr_t;
		test_data->gradation = STAGE_INTER;
	}
	/* ��ǰ�����Խ��� */
	else
	{
        clear_test_data_fall_time_timeout(acw_par, test_data);
        
        /* �ظ�ѹ */
        if(acw_test_flag.testing == 1)
        {
            acw_test_flag.testing = 0;
            acw_exit_test_relay_motion(acw_par, test_data);
        }
        
        /* ���������� */
		if(acw_par->steps_cont)
		{
            test_data->cont = 1;
            return;
        }
        
        /* �������� */
        test_data->test_over = 1;
	}
}

void acw_test_ready(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    test_vref(acw_par->upper_limit);	/* �������׼ */
    
	updata_port(&acw_par->port);/* ���¶˿�״̬ */
    acw_into_test_relay_ready(acw_par, test_data);/* �̵������� */
    amp_relay_ctrl_on();/* ���ż̵��� */
    cs_delay_ms(10);
    
    acw_relay_ready();//�����ӿ���
    
    open_sine(acw_par->output_freq);/* �����Ҳ� */
    
    if(acw_par->rise_time == 0)
    {
        acw_set_da_value(acw_par, test_data);//�ͻ�׼
    }
    
    open_test_timer();/* ����ʱ�� */
    
	test_data->err_real = 0;/* acw ��ʵ��������  */
    acw_test_flag.forever_testing = 0;
    test_data->test_over = 0;
    test_data->dis_time = 0;
    test_data->test_status = ST_WAIT;
    test_data->fail_num = ST_ERR_NONE;//Ĭ�ϳ�ʼ��Ϊ�ϸ�
}

void acw_test_details(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
	switch(test_data->gradation)
	{
		case STAGE_READY:/* �ک��׶� */
        {
            if(test_data->ready_ok == 0)
            {
                test_data->ready_ok = 1;//���
                acw_test_flag.testing = 1;//���ڲ��Ա��
                acw_test_flag.judge_err_en = ENABLE;
                load_acw_data(acw_par, test_data);
                acw_test_ready(acw_par, test_data);
            }
            break;
        }
        case STAGE_FAIL_CONT:
        {
            /* ���ʱ�䲻Ϊ0�ʹӼ��ʱ�俪ʼ */
            if(acw_par->interval_time > 0)
            {
                test_data->ready_ok == 1;
                test_data->test_time = acw_thr_t + 1;/* ������1�׶� */
                test_data->gradation = STAGE_INTER;/* �������ȴ� */
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

void run_acw_test(NODE_STEP *step, NODE_STEP *next_step, TEST_DATA_STRUCT *test_data)
{
    ACW_STRUCT *acw_par = &step->one_step.acw;
    
    /* ׼��������ɺ��ڼ�����Խ׶� */
    if(test_data->ready_ok == 1)
    {
        acw_test_irq(acw_par, test_data);
    }
    
    acw_test_details(acw_par, test_data);
    
    /* ���ֱ������ٽ��в��� ���Խ������ٽ��в��� */
    if(test_data->fail_num == ST_ERR_NONE && test_data->test_over == 0)
    {
        /* ����׶β����в��� */
        if(test_data->gradation != STAGE_INTER)
        {
            acw_count_dis_value(acw_par, test_data);
        }
    }
    
    if(test_data->fail_num != ST_ERR_NONE)
    {
        test_data->ready_ok = 0;
    }
}

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
