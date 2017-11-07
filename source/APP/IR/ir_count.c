
#include "SPI_CPLD.H"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "ir_relay.h"
#include "ir_count.h"
#include "ir_test.h"
#include "test_com.h"



static int8_t ir_judge_err(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data);

/**
  * @brief  �����IR�Ĳ���ֵ������������
  * @param  [in] ir_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void ir_count_dis_value(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
	uint16_t temp_cur = 0;
	uint16_t temp_vol = 0;
	float temp_res = 0;
    
    temp_vol = test_data->sample_vol_ad;
    temp_cur = test_data->sample_cur_ad;
    
    test_data->cur_ad_vol = temp_cur * 3.3 / 4096;/* ���������ad�����˵ĵ�ѹֵ */
    ir_test_flag.cur_res_area = get_cur_area(test_data);
    
    
    
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
        if(test_data->vol_value <= ir_par->testing_voltage)
        {
            if(test_data->vol_value + 10 > ir_par->testing_voltage)
            {
                test_data->vol_value = ir_par->testing_voltage;
            }
        }
        else
        {
            if(ir_par->testing_voltage + 3 > test_data->vol_value)
            {
                test_data->vol_value = ir_par->testing_voltage;
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
    
    /* �������Ϊ0ʱ����������1 */
    if(test_data->cur_value <= 0)
    {
        test_data->cur_value = 1;
        test_data->res_value = 1000*1000;
        return;
    }
    
    /* ������ʱ�����Ͳ��������ֵ */
    if(ir_test_flag.dly > 5)
    {
        return;
    }
    
    temp_res = (float)test_data->vol_value * test_data->res_adc_k / test_data->cur_value;
    
    test_data->res_value = temp_res;
    
    if(test_data->res_value < 0)
    {
        test_data->res_value = 0;
    }
    
    /* �쳣�ж� */
    if(ir_test_flag.judge_err_en == ENABLE)
    {
        test_data->fail_num = ir_judge_err(ir_par, test_data);
        
        if(test_data->fail_num != ST_ERR_NONE)
        {
            ir_test_flag.judge_err_en == DISABLE;
            close_test_timer();/* �ض�ʱ�� */
            irq_stop_relay_motion();///<�رո�ѹ
            test_data->test_over = 1;
        }
    }
}

/**
  * @brief  IR��ѹ�쳣�ж�
  * @param  [in] ir_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
uint8_t judge_ir_vol_exception(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
	/* ��ѹ���� �׶� ����׶� */
	if(test_data->gradation == STAGE_RISE || test_data->gradation == STAGE_CHANGE)
    {
        if(test_data->vol_value > (ir_par->testing_voltage * 11 / 10 + 10))
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
        if(test_data->dis_time >= 5 || test_data->dis_time == ir_par->testing_time)
        {
            /* ��ѹ����1V */
            if(test_data->vol_value < 1)
            {
                return ST_ERR_SHORT;
            }
        }
        
        /* ����ʱ�䵽 */
        if(ir_par->testing_time > 0 && test_data->dis_time + 1 >= ir_par->testing_time)
        {
            return ST_ERR_NONE;
        }
        
        if((test_data->vol_value < ir_par->testing_voltage / 2
            || test_data->vol_value > (ir_par->testing_voltage * 11 / 10 + 10)))
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


/**
  * @brief  IR�쳣�ж�
  * @param  [in] ir_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval �쳣��
  */
int8_t ir_judge_err(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    int8_t err = ST_ERR_NONE;
	
	/* ��ѹ���� �׶� ����׶� */
    if(test_data->gradation == STAGE_RISE || test_data->gradation == STAGE_CHANGE)
	{
        err = judge_ir_vol_exception(ir_par, test_data);
	}
	/* ���Խ׶� */
	else if(test_data->gradation == STAGE_TEST && test_data->dis_time > 2)
	{
        uint16_t delay_t = ir_par->delay_time;
        
        /* ��ѹ�쳣�ж� */
        err = judge_ir_vol_exception(ir_par, test_data);
        
        if(ST_ERR_NONE != err)
        {
            return err;
        }
        
        if(ir_par->delay_time > ir_par->testing_time && ir_par->testing_time != 0)
        {
            delay_t = ir_par->testing_time;
        }
        
        /* ��ʱʱ�䵽 */
        if(test_data->dis_time >= delay_t)
        {
            return err;
        }
        
        if(test_data->dis_time >= 3)
        {
            return err;
        }
        
        /* ����ʱ�䵽ǰ0.1s */
        if(ir_par->testing_time && test_data->dis_time + 1 >= ir_par->testing_time)
        {
            if(ST_ERR_NONE == err)
            {
                /* �������ޱ��� */
                if(test_data->res_value > ir_par->upper_limit && ir_par->upper_limit > 0)
                {
                    test_data->test_over_fail = ST_ERR_H;
                }
                else
                {
                    test_data->test_over_fail = ST_ERR_NONE;
                }
            }
        }
        
        /* ������ʱû����ֱ���˳� �����ж� ����ʱ�䵽���� */
        if(ir_test_flag.dly > 0 && test_data->dis_time != ir_par->testing_time)
        {
            return err;
        }
        
        if(ST_ERR_NONE == err)
        {
            /* �ж϶�·���� */
            if(test_data->res_value <= 0.5)
            {
                err = ST_ERR_SHORT;
            }
        }
        
        if(ST_ERR_NONE == err)
        {
            /* �������ޱ��� */
            if(test_data->res_value < ir_par->lower_limit && test_data->vol_value > 0)
            {
                if(++test_data->low_err_count > 0)
                {
                    err = ST_ERR_L;
                }
            }
            else
            {
                test_data->low_err_count = 0;
            }
        }
    }
	
	return err;
}

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
