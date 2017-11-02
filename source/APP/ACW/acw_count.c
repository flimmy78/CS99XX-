
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
    
    /* 计算相位角的正弦值和余弦值 */
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
    
    /* 测试阶段 */
    if(test_data->gradation == STAGE_TEST)
    {
        //对电压小于设定值10个字的电压进行修正 2017.6.28
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
    
    /* 引入偏移值 */
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
    
    /* 异常判断 */
    if(acw_test_flag.judge_err_en == ENABLE)
    {
        test_data->fail_num = acw_judge_err(acw_par, test_data);
        
        if(test_data->fail_num != ST_ERR_NONE)
        {
            acw_test_flag.judge_err_en == DISABLE;
            irq_stop_relay_motion();///<关闭高压
            test_data->test_over = 1;
        }
    }
}

uint8_t judge_acw_vol_exception(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
	/* 电压上升 阶段 缓变阶段 */
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
            return ST_ERR_NONE;/* 测试时间到报警 */
        }
    }
    /* 测试阶段 */
    else if(test_data->gradation == STAGE_TEST)
    {
        if(test_data->dis_time >= 5 || test_data->dis_time == acw_par->testing_time)
        {
            /* 电压低于1V */
            if(test_data->vol_value < 1)
            {
                return ST_ERR_SHORT;
            }
        }
        
        /* 测试时间到 */
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
            return ST_ERR_NONE;/* 测试时间到报警 */
        }
    }
    
    return ST_ERR_NONE;
}

int8_t acw_judge_err(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    int8_t err = ST_ERR_NONE;
	
    /* 判断真实电流 */
    if(test_data->real_value > acw_par->ac_real_cur && acw_par->ac_real_cur > 0)
    {
        err = ST_ERR_REAL;
    }
    /* 当电流值大于上限值时就将电流值强行改为上限值并设置上限报警 */
    else if(test_data->cur_value > acw_par->upper_limit)
    {
        err = ST_ERR_H;
    }
	/* 电压上升 阶段 缓变阶段 */
    else if(test_data->gradation == STAGE_RISE || test_data->gradation == STAGE_CHANGE)
	{
        err = judge_acw_vol_exception(acw_par, test_data);
	}
	/* 测试阶段 */
	else if(test_data->gradation == STAGE_TEST)
	{
        /* 电压异常判断 */
        err = judge_acw_vol_exception(acw_par, test_data);
        
        if(ST_ERR_NONE == err)
        {
            /* 电流下限报警 */
            if(test_data->cur_value < acw_par->lower_limit)
            {
                /* 下限报警判断三次 */
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
            /* 测试时间到 */
            if(acw_par->testing_time && test_data->dis_time + 1 >= acw_par->testing_time)
            {
                /* 电流下限报警 */
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

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
