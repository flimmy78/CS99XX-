
#include "SPI_CPLD.H"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "ir_relay.h"
#include "ir_count.h"
#include "ir_test.h"
#include "test_com.h"



static int8_t ir_judge_err(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data);

/**
  * @brief  计算出IR的测试值，电流，电阻
  * @param  [in] ir_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void ir_count_dis_value(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
	uint16_t temp_cur = 0;
	uint16_t temp_vol = 0;
	float temp_res = 0;
    
    temp_vol = test_data->sample_vol_ad;
    temp_cur = test_data->sample_cur_ad;
    
    test_data->cur_ad_vol = temp_cur * 3.3 / 4096;/* 计算出电流ad采样端的电压值 */
    ir_test_flag.cur_res_area = get_cur_area(test_data);
    
    
    
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
    
    /* 如果电流为0时就让他等于1 */
    if(test_data->cur_value <= 0)
    {
        test_data->cur_value = 1;
        test_data->res_value = 1000*1000;
        return;
    }
    
    /* 换档延时不到就不计算电阻值 */
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
    
    /* 异常判断 */
    if(ir_test_flag.judge_err_en == ENABLE)
    {
        test_data->fail_num = ir_judge_err(ir_par, test_data);
        
        if(test_data->fail_num != ST_ERR_NONE)
        {
            ir_test_flag.judge_err_en == DISABLE;
            close_test_timer();/* 关定时器 */
            irq_stop_relay_motion();///<关闭高压
            test_data->test_over = 1;
        }
    }
}

/**
  * @brief  IR电压异常判断
  * @param  [in] ir_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
uint8_t judge_ir_vol_exception(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
	/* 电压上升 阶段 缓变阶段 */
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
            return ST_ERR_NONE;/* 测试时间到报警 */
        }
    }
    /* 测试阶段 */
    else if(test_data->gradation == STAGE_TEST)
    {
        if(test_data->dis_time >= 5 || test_data->dis_time == ir_par->testing_time)
        {
            /* 电压低于1V */
            if(test_data->vol_value < 1)
            {
                return ST_ERR_SHORT;
            }
        }
        
        /* 测试时间到 */
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
            return ST_ERR_NONE;/* 测试时间到报警 */
        }
    }
    
    return ST_ERR_NONE;
}


/**
  * @brief  IR异常判断
  * @param  [in] ir_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 异常码
  */
int8_t ir_judge_err(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    int8_t err = ST_ERR_NONE;
	
	/* 电压上升 阶段 缓变阶段 */
    if(test_data->gradation == STAGE_RISE || test_data->gradation == STAGE_CHANGE)
	{
        err = judge_ir_vol_exception(ir_par, test_data);
	}
	/* 测试阶段 */
	else if(test_data->gradation == STAGE_TEST && test_data->dis_time > 2)
	{
        uint16_t delay_t = ir_par->delay_time;
        
        /* 电压异常判断 */
        err = judge_ir_vol_exception(ir_par, test_data);
        
        if(ST_ERR_NONE != err)
        {
            return err;
        }
        
        if(ir_par->delay_time > ir_par->testing_time && ir_par->testing_time != 0)
        {
            delay_t = ir_par->testing_time;
        }
        
        /* 延时时间到 */
        if(test_data->dis_time >= delay_t)
        {
            return err;
        }
        
        if(test_data->dis_time >= 3)
        {
            return err;
        }
        
        /* 测试时间到前0.1s */
        if(ir_par->testing_time && test_data->dis_time + 1 >= ir_par->testing_time)
        {
            if(ST_ERR_NONE == err)
            {
                /* 电阻上限报警 */
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
        
        /* 换档延时没到就直接退出 不做判断 测试时间到除外 */
        if(ir_test_flag.dly > 0 && test_data->dis_time != ir_par->testing_time)
        {
            return err;
        }
        
        if(ST_ERR_NONE == err)
        {
            /* 判断短路报警 */
            if(test_data->res_value <= 0.5)
            {
                err = ST_ERR_SHORT;
            }
        }
        
        if(ST_ERR_NONE == err)
        {
            /* 电阻下限报警 */
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

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
