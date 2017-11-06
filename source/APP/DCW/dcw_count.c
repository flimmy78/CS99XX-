
#include "SPI_CPLD.H"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "dcw_relay.h"
#include "dcw_count.h"
#include "dcw_test.h"
#include "test_com.h"



static int8_t dcw_judge_err(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data);

void dcw_count_dis_value(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
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
        if(test_data->vol_value <= dcw_par->testing_voltage)
        {
            if(test_data->vol_value + 10 > dcw_par->testing_voltage)
            {
                test_data->vol_value = dcw_par->testing_voltage;
            }
        }
        else
        {
            if(dcw_par->testing_voltage + 3 > test_data->vol_value)
            {
                test_data->vol_value = dcw_par->testing_voltage;
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
    
    /* ����ƫ��ֵ */
    if(test_data->cur_value >= dcw_par->offset_cur)
    {
        test_data->cur_value -= dcw_par->offset_cur;
    }
    else
    {
        test_data->cur_value = 0;
    }
    
    /* �쳣�ж� */
    if(dcw_test_flag.judge_err_en == ENABLE)
    {
        test_data->fail_num = dcw_judge_err(dcw_par, test_data);
        
        if(test_data->fail_num != ST_ERR_NONE)
        {
            dcw_test_flag.judge_err_en == DISABLE;
            close_test_timer();/* �ض�ʱ�� */
            irq_stop_relay_motion();///<�رո�ѹ
            test_data->test_over = 1;
        }
    }
}

uint8_t judge_dcw_vol_exception(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
	/* ��ѹ���� �׶� ����׶� */
	if(test_data->gradation == STAGE_RISE || test_data->gradation == STAGE_CHANGE)
    {
        if(test_data->vol_value > (dcw_par->testing_voltage * 11 / 10 + 10))
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
        if(test_data->dis_time >= 5 || test_data->dis_time == dcw_par->testing_time)
        {
            /* ��ѹ����1V */
            if(test_data->vol_value < 1)
            {
                return ST_ERR_SHORT;
            }
        }
        
        /* ����ʱ�䵽 */
        if(dcw_par->testing_time > 0 && test_data->dis_time + 1 >= dcw_par->testing_time)
        {
            return ST_ERR_NONE;
        }
        
        if((test_data->vol_value < dcw_par->testing_voltage / 2
            || test_data->vol_value > (dcw_par->testing_voltage * 11 / 10 + 10)))
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

int8_t dcw_judge_err(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
    int8_t err = ST_ERR_NONE;
	
    
    /* �жϳ����� */
    if(test_data->cur_value > dcw_par->charge_cur || dcw_par->charge_cur == 0)
    {
       dcw_test_flag.err_charge = 0;/* ������ */
    }
    
    /* ������ֵ��������ֵʱ�ͽ�����ֵǿ�и�Ϊ����ֵ���������ޱ��� */
    if(test_data->cur_value > dcw_par->upper_limit)
    {
        err = ST_ERR_H;
    }
	/* ��ѹ���� �׶� ����׶� */
    else if(test_data->gradation == STAGE_RISE || test_data->gradation == STAGE_CHANGE)
	{
        err = judge_dcw_vol_exception(dcw_par, test_data);
	}
	/* ���Խ׶� */
	else if(test_data->gradation == STAGE_TEST && test_data->dis_time > 2)
	{
        /* ��ѹ�쳣�ж� */
        err = judge_dcw_vol_exception(dcw_par, test_data);
        
        if(ST_ERR_NONE == err)
        {
            /* �������ޱ��� */
            if(test_data->cur_value < dcw_par->lower_limit)
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
            if(dcw_par->testing_time && test_data->dis_time + 1 >= dcw_par->testing_time)
            {
                /* �������ޱ��� */
                if(test_data->cur_value < dcw_par->lower_limit)
                {
                    test_data->test_over_fail = ST_ERR_L;
                }
                /* ��籨�� */
                else if(dcw_test_flag.err_charge)
                {
                    test_data->test_over_fail = ST_ERR_CHAR;
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
