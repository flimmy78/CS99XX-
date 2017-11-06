
#include "SPI_CPLD.H"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "gr_relay.h"
#include "gr_count.h"
#include "gr_test.h"
#include "test_com.h"



static int8_t gr_judge_err(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data);

void gr_count_dis_value(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data)
{
	uint16_t temp_cur = 0;
	uint16_t temp_vol = 0;
    
    temp_vol = test_data->sample_vol_ad;
    temp_cur = test_data->sample_cur_ad;
    
    if(temp_vol == 0)
    {
        test_data->vol_value = 0;
    }
    else
    {
        test_data->vol_value = temp_vol * test_data->vol_adc_k + test_data->vol_adc_b;
    }
    
    /* ���Խ׶� */
    if(test_data->gradation == STAGE_TEST)
    {
        //�Ե�ѹС���趨ֵ10���ֵĵ�ѹ�������� 2017.6.28
        if(test_data->vol_value <= gr_par->testing_cur)
        {
            if(test_data->vol_value + 10 > gr_par->testing_cur)
            {
                test_data->vol_value = gr_par->testing_cur;
            }
        }
        else
        {
            if(gr_par->testing_cur + 3 > test_data->vol_value)
            {
                test_data->vol_value = gr_par->testing_cur;
            }
        }
    }
    
    if(test_data->cur_value < 0)
    {
        test_data->cur_value = 0;
    }
    
    if(test_data->res_value < 0)
    {
        test_data->cur_value = 0;
    }
    
    test_data->res_value = temp_cur * test_data->cur_adc_k;
    
    /* ����ƫ��ֵ */
    if(test_data->res_value >= gr_par->offset_res)
    {
        test_data->res_value -= gr_par->offset_res;
    }
    else
    {
        test_data->res_value = 0;
    }
    
    /* �쳣�ж� */
    if(gr_test_flag.judge_err_en == ENABLE)
    {
        test_data->fail_num = gr_judge_err(gr_par, test_data);
        
        if(test_data->fail_num != ST_ERR_NONE)
        {
            gr_test_flag.judge_err_en == DISABLE;
            close_test_timer();/* �ض�ʱ�� */
            irq_stop_relay_motion();///<�رո�ѹ
            test_data->test_over = 1;
        }
    }
}

uint8_t judge_gr_vol_exception(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data)
{
	/* ��ѹ���� �׶� ����׶� */
	if(test_data->gradation == STAGE_RISE || test_data->gradation == STAGE_CHANGE)
    {
        if(test_data->vol_value > (gr_par->testing_cur * 11 / 10 + 10))
        {
            if(++test_data->vol_err_count > 2)
            {
                return ST_ERR_VOL_ABNORMAL;
            }
        }
        else
        {
            test_data->vol_err_count = 0;
            return ST_ERR_NONE;/* ����ʱ�䵽���� */
        }
    }
    /* ���Խ׶� */
    else if(test_data->gradation == STAGE_TEST)
    {
        if(test_data->dis_time >= 5 || test_data->dis_time == gr_par->testing_time)
        {
            /* ��ѹ����1V */
            if(test_data->vol_value < 1)
            {
                return ST_ERR_SHORT;
            }
        }
        
        /* ����ʱ�䵽 */
        if(gr_par->testing_time > 0 && test_data->dis_time + 1 >= gr_par->testing_time)
        {
            return ST_ERR_NONE;
        }
        
        if((test_data->vol_value < gr_par->testing_cur / 2
            || test_data->vol_value > (gr_par->testing_cur * 11 / 10 + 10)))
        {
            return ST_ERR_VOL_ABNORMAL;
        }
        else
        {
            return ST_ERR_NONE;/* ����ʱ�䵽���� */
        }
    }
    
    return ST_ERR_NONE;
}

int8_t gr_judge_err(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data)
{
    int8_t err = ST_ERR_NONE;
	
//    /* �ж���ʵ���� */
//    if(test_data->real_value > gr_par->ac_real_cur && gr_par->ac_real_cur > 0)
//    {
//        err = ST_ERR_REAL;
//    }
//    /* ������ֵ��������ֵʱ�ͽ�����ֵǿ�и�Ϊ����ֵ���������ޱ��� */
//    else 
        if(test_data->cur_value > gr_par->upper_limit)
    {
        err = ST_ERR_H;
    }
	/* ��ѹ���� �׶� ����׶� */
    else if(test_data->gradation == STAGE_RISE || test_data->gradation == STAGE_CHANGE)
	{
        err = judge_gr_vol_exception(gr_par, test_data);
	}
	/* ���Խ׶� */
	else if(test_data->gradation == STAGE_TEST && test_data->dis_time > 2)
	{
        /* ��ѹ�쳣�ж� */
        err = judge_gr_vol_exception(gr_par, test_data);
        
        if(ST_ERR_NONE == err)
        {
            /* �������ޱ��� */
            if(test_data->cur_value < gr_par->lower_limit)
            {
                /* ���ޱ����ж����� */
                if(++test_data->low_err_count >= 2)
                {
                    err = ST_ERR_L;
                }
            }
            else
            {
                test_data->low_err_count = 0;
            }
        }
        
        if(ST_ERR_NONE == err)
        {
            /* ����ʱ�䵽 */
            if(gr_par->testing_time && test_data->dis_time + 1 >= gr_par->testing_time)
            {
                /* �������ޱ��� */
                if(test_data->cur_value < gr_par->lower_limit)
                {
                    test_data->test_over_fail = ST_ERR_L;
                }
                else
                {
                    test_data->test_over_fail = ST_ERR_NONE;
                }
            }
        }
	}
	
	return err;
}

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
