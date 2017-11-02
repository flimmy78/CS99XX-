
#include "SPI_CPLD.H"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "acw_relay.h"
#include "acw_count.h"
#include "acw_test.h"
#include "test_com.h"



static int8_t acw_judge_err(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data);

void acw_count_dis_value(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
	double cos_val = 0.0;
	double sin_val = 0.0;
	uint16_t temp_cur = 0;
	uint16_t temp_vol = 0;
    
    /* ������λ�ǵ�����ֵ������ֵ */
    if(acw_par->ac_real_cur > 0)
    {
        cpld_count_angle(&sin_val, &cos_val);
    }
	
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
        if(test_data->vol_value <= acw_par->testing_voltage)
        {
            if(test_data->vol_value + 10 > acw_par->testing_voltage)
            {
                test_data->vol_value = acw_par->testing_voltage;
            }
        }
        else
        {
            if(acw_par->testing_voltage + 3 > test_data->vol_value)
            {
                test_data->vol_value = acw_par->testing_voltage;
            }
        }
    }
    
    if(test_data->vol_value < 0)
    {
        test_data->vol_value = 0;
    }
    
    if(test_data->cur_value < 0)
    {
        test_data->cur_value = 0;
    }
    
    test_data->cur_value = temp_cur * test_data->cur_adc_k;
    test_data->real_value = test_data->cur_value * cos_val;
    
    /* ����ƫ��ֵ */
    if(test_data->cur_value >= acw_par->offset_cur)
    {
        test_data->cur_value -= acw_par->offset_cur;
    }
    else
    {
        test_data->cur_value = 0;
    }
    
    if(test_data->real_value >= acw_par->offset_real)
    {
        test_data->real_value -= acw_par->offset_real;
    }
    else
    {
        test_data->real_value = 0;
    }
    
    /* �쳣�ж� */
    if(acw_test_flag.judge_err_en == ENABLE)
    {
        test_data->fail_num = acw_judge_err(acw_par, test_data);
        
        if(test_data->fail_num != ST_ERR_NONE)
        {
            acw_test_flag.judge_err_en == DISABLE;
            irq_stop_relay_motion();///<�رո�ѹ
            test_data->test_over = 1;
        }
    }
}

uint8_t judge_acw_vol_exception(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
	/* ��ѹ���� �׶� ����׶� */
	if(test_data->gradation == STAGE_RISE || test_data->gradation == STAGE_CHANGE)
    {
        if(test_data->vol_value > (acw_par->testing_voltage * 11 / 10 + 10))
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
        if(test_data->dis_time >= 5 || test_data->dis_time == acw_par->testing_time)
        {
            /* ��ѹ����1V */
            if(test_data->vol_value < 1)
            {
                return ST_ERR_SHORT;
            }
        }
        
        /* ����ʱ�䵽 */
        if(acw_par->testing_time > 0 && test_data->dis_time + 1 >= acw_par->testing_time)
        {
            return ST_ERR_NONE;
        }
        
        if((test_data->vol_value < acw_par->testing_voltage / 2
            || test_data->vol_value > (acw_par->testing_voltage * 11 / 10 + 10)))
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

int8_t acw_judge_err(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    int8_t err = ST_ERR_NONE;
	
    /* �ж���ʵ���� */
    if(test_data->real_value > acw_par->ac_real_cur && acw_par->ac_real_cur > 0)
    {
        err = ST_ERR_REAL;
    }
    /* ������ֵ��������ֵʱ�ͽ�����ֵǿ�и�Ϊ����ֵ���������ޱ��� */
    else if(test_data->cur_value > acw_par->upper_limit)
    {
        err = ST_ERR_H;
    }
	/* ��ѹ���� �׶� ����׶� */
    else if(test_data->gradation == STAGE_RISE || test_data->gradation == STAGE_CHANGE)
	{
        err = judge_acw_vol_exception(acw_par, test_data);
	}
	/* ���Խ׶� */
	else if(test_data->gradation == STAGE_TEST)
	{
        /* ��ѹ�쳣�ж� */
        err = judge_acw_vol_exception(acw_par, test_data);
        
        if(ST_ERR_NONE == err)
        {
            /* �������ޱ��� */
            if(test_data->cur_value < acw_par->lower_limit)
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
            if(acw_par->testing_time && test_data->dis_time + 1 >= acw_par->testing_time)
            {
                /* �������ޱ��� */
                if(test_data->cur_value < acw_par->lower_limit)
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
