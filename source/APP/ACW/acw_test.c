
#define _ACW_TEST_GLOBALS
#include "acw_test.h"
#include "acw_relay.h"
#include "serve_test.h"
#include "calibration.h"
#include "SPI_CPLD.H"
#include "SysTemConfig.h"

static uint16_t acw_zeo_t;///< �ک��׶ε��ۼ�ʱ��
static uint16_t acw_one_t;///< ��һ�׶ε��ۼ�ʱ��
static uint16_t acw_two_t;///< �ڶ��׶ε��ۼ�ʱ��
static uint16_t acw_thr_t;///< �����׶ε��ۼ�ʱ��
static uint16_t acw_for_t;///< ���Ľ׶ε��ۼ�ʱ��
static uint16_t acw_fiv_t;///< ����׶ε��ۼ�ʱ��
//static uint16_t acw_tes_t;///< ��ǰ������ʱ��
//static uint16_t acw_ris_t;///< ����ʱ��
//static uint16_t acw_sta_t;///< ��ѹʱ��
//static uint16_t acw_fal_t;///< �½�ʱ��
//static uint16_t acw_int_t;///< ���ʱ��
//static uint16_t acw_cha_t;///< ����ʱ��
//static uint8_t acw_cur_gear;///<ACW������λ
//static NODE_STEP *acw_test_par;///<ACW���Բ���
//uint8_t acw_step;///<ACW������

//float acw_vol_ave = 0;///<ACW���Ե�ѹ
//float acw_cur_ave = 0;///<ACW���Ե���

void acw_start_test(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    acw_into_test_relay_ready(acw_par, test_data);
    
}

void load_acw_data(TEST_FILE *test_file, ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    /* ƫ�Ʋ����������ʱ */
    acw_zeo_t = 0;
    
    if(test_file->work_mode == N_MODE)
    {
        acw_one_t = acw_par->rise_time + acw_zeo_t;
        acw_two_t = acw_par->rise_time + acw_par->testing_time + acw_zeo_t;
        acw_thr_t = acw_par->rise_time + acw_par->testing_time + acw_par->fall_time + acw_zeo_t;
        acw_for_t = acw_par->rise_time + acw_par->testing_time + acw_par->fall_time + acw_par->interval_time + acw_zeo_t;
        
    }
    else if(test_file->work_mode == G_MODE)
    {
        if(cur_step == 1)
        {
            acw_one_t = acw_par->rise_time + acw_zeo_t;
        }
        else
        {
            acw_one_t = 0 + acw_zeo_t;
        }
        
        acw_two_t = acw_one_t + acw_par->interval_time;/* ����ʱ�� */
        acw_thr_t = acw_two_t + acw_par->testing_time;
        
        if(cur_step == test_file->total)
        {
            acw_for_t = acw_thr_t + acw_par->fall_time;
        }
        else
        {
            acw_for_t = acw_thr_t + 0;
        }
    }
}
/*
 * ��������test_irq
 * ����  ������ʱ�ӿ��ƣ��������������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void acw_test_irq(TEST_FILE *test_file, ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    uint16_t l_dis_time = 0;
    uint16_t test_status;
    
	/* ������ʱ��Ϊ0ʱ ����ʱ������� */
	if(0 == acw_par->testing_time)
	{
		/* �����ǰ�������ڽ��� �����ڵڶ��׶εĻ���һֱ���� */
		if(acw_test_flag.testing)
		{
			test_status = STAGE_TEST;
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
			acw_test_flag.gradation = STAGE_READY;
			acw_test_flag.dis_status = TEST_WAIT_INDEX;
			acw_test_flag.vol_change_flag = 1;
			return;
		}
		/* ʹ��������һ�׶ε�ѹ���� */
		else if(g_test_time <= acw_one_t)/* ������ʱ����Ϊ0ʱ���Ͳ��ܽ���if,���Ҫ+1 */
		{
            count_rise_vol_step_value();/* �����ѹ��������ֵ */
			test_data->dis_time = test_data->test_time - acw_zeo_t;
			acw_test_flag.gradation = STAGE_RISE;
			acw_test_flag.dis_status = TEST_RISE_INDEX;
			acw_test_flag.vol_change_flag = 1;
            
            if(test_data->dis_time == 1)
            {
                count_vol_ch_step();
            }
			return;
		}
		
        if(test_data->test_time == acw_one_t + 1)
        {
            open_hv();
        }
        
		if(test_data->fail_num != ERR_NONE)
		{
			if(test_data->test_time < acw_for_t)
			{
				/* ���������ر� */
				if(!acw_par->steps_cont)
				{
                    acw_test_flag.test_over = 1;
                    return;
				}
				test_data->dis_time = test_data->test_time - acw_thr_t + 1;
				acw_test_flag.gradation = STAGE_INTER;
				acw_test_flag.dis_status = TEST_INTER_INDEX;
				return;
			}
			else
			{
                acw_test_flag.test_over = 1;
                return;
			}
		}
		
		acw_test_flag.vol_change_flag = 0;
		acw_test_flag.gradation = STAGE_TEST;
		acw_test_flag.dis_status = TEST_TEST_INDEX;
		test_data->dis_time = test_data->test_time - acw_one_t;
		
		if(test_data->dis_time >= 9999)
		{
            test_data->test_time = 0;
			acw_test_flag.testing = 1;/* ������ʱ����� */
		}
		return;
	}
	
	/* ���Ծ��� */
	if(test_data->test_time <= acw_zeo_t)
	{
		test_data->dis_time = test_data->test_time;
		acw_test_flag.gradation = STAGE_READY;
		acw_test_flag.dis_status = TEST_WAIT_INDEX;
        acw_test_flag.vol_change_flag = 1;
	}
	/* ʹ��������һ�׶ε�ѹ���� */
	else if(test_data->test_time <= acw_one_t)
	{
		test_data->dis_time = test_data->test_time - acw_zeo_t;
		acw_test_flag.gradation = STAGE_RISE;
		acw_test_flag.dis_status = TEST_RISE_INDEX;
		acw_test_flag.vol_change_flag = 1;
        
        if(test_data->dis_time == 1)
        {
            count_vol_ch_step();
        }
	}
	/* ����ڶ��׶� ���� */
	else if(test_data->test_time <= acw_two_t)
	{
		test_data->dis_time = test_data->test_time - acw_one_t;
		acw_test_flag.gradation = STAGE_TEST;
		acw_test_flag.dis_status = TEST_TEST_INDEX;
		acw_test_flag.vol_change_flag = 0;
        
        if(test_data->test_time == acw_one_t + 1)
        {
            open_hv();
        }
	}
	/* �����׶� ��ѹ�½� */
	else if(test_data->test_time <= acw_thr_t)
	{
		acw_test_flag.vol_change_flag = 1;
		acw_test_flag.dis_status = TEST_FALL_INDEX;
		test_data->dis_time = test_data->test_time - acw_two_t;
		acw_test_flag.gradation = STAGE_FALL;
        
        if(test_data->dis_time == 1)
        {
            count_vol_ch_step();
        }
	}
	/* ���Ľ׶� ����ȴ� */
	else if(test_data->test_time <= acw_for_t)
	{
		if(sys_par.fail_mode == FAIL_MODE_FPDFC && acw_par->step == 1)
		{
			if(!CUR_FAIL)
			{
                acw_test_flag.test_over = 1;
				return;
			}
		}
        
		/* ���������ر� */
		if(!acw_par->steps_cont)
		{
            acw_test_flag.test_over = 1;
            return;
		}
        
		test_data->dis_time = test_data->test_time - acw_thr_t;
		acw_test_flag.gradation = STAGE_INTER;
		acw_test_flag.dis_status = TEST_INTER_INDEX;
	}
	/* ��ǰ�����Խ��� */
	else
	{
        acw_test_flag.test_over = 1;
	}
}

void acw_test_details(TEST_FILE *test_file, ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
	switch(acw_test_flag.gradation)
	{
		case STAGE_READY:/* �ک��׶� */
        {
            acw_test_flag.testing = 1;
            test_data->test_status = ST_WAIT;
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
            count_fall_vol_step_value();/* �����ѹ�½�����ֵ */
			break;
		case STAGE_INTER:/* ���Ľ׶� ����ȴ� */
        {
            test_data->test_status = ST_INTER_WAIT;
			
			if(acw_test_flag.testing == 1)
			{
				acw_test_flag.testing = 0;
                
                /* �ظ�ѹ */
                irq_stop_relay_motion();
				acw_exit_test_relay_motion(acw_par, test_data);
				
                /* ����½�ʱ�䲻Ϊ0����0���Ե�ѹ�͵��� */
				if(acw_par->fall_time > 0)
				{
                    test_data->vol_value = 0;
                    test_data->cur_value = 0;
                    test_data->real_value = 0;
				}
			}
			break;
        }
	}
}

void run_acw_test(TEST_FILE *test_file, ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    acw_test_details(test_file, acw_par, test_data);
    count_acw_dis_value(acw_par, test_data);
}

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
