
#include "SPI_CPLD.H"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "gr_relay.h"
#include "gr_count.h"
#include "gr_test.h"
#include "test_com.h"



static int8_t gr_judge_err(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data);

/**
  * @brief  计算出GR的测试值，电流，电阻
  * @param  [in] ir_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void gr_count_dis_value(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data)
{
	uint16_t temp_cur = 0;
	uint16_t temp_vol = 0;
    
    temp_vol = test_data->sample_vol_ad;
    temp_cur = test_data->sample_cur_ad;
    
    if(temp_vol == 0)
    {
        test_data->cur_value = 0;
    }
    else
    {
        test_data->cur_value = temp_vol * test_data->cur_adc_k + test_data->cur_adc_b;
    }
    
    /* 测试阶段 */
    if(test_data->gradation == STAGE_TEST)
    {
        //对电压小于设定值10个字的电压进行修正 2017.6.28
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
    
    /* 引入偏移值 */
    if(test_data->res_value >= gr_par->offset_res)
    {
        test_data->res_value -= gr_par->offset_res;
    }
    else
    {
        test_data->res_value = 0;
    }
    
    /* 异常判断 */
    if(gr_test_flag.judge_err_en == ENABLE)
    {
        test_data->fail_num = gr_judge_err(gr_par, test_data);
        
        if(test_data->fail_num != ST_ERR_NONE)
        {
            gr_test_flag.judge_err_en == DISABLE;
            close_test_timer();/* 关定时器 */
            irq_stop_relay_motion();///<关闭高压
            test_data->test_over = 1;
        }
    }
}


/**
  * @brief  GR异常判断
  * @param  [in] ir_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 异常码
  */
int8_t gr_judge_err(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data)
{
    int8_t err = ST_ERR_NONE;
	
	/* 测试阶段 */
    if(test_data->gradation == STAGE_TEST && test_data->dis_time > 2)
	{
        /* 开路报警 电流小于设定值的3% 时认为开路 */
        if(test_data->cur_value * 100 <= gr_par->testing_cur * 0.03)
        {
            if(++test_data->cur_err_count > 1)
            {
                test_data->res_value = 5100;
                test_data->cur_value = 0;
                err = ST_ERR_OPEN;
            }
        }
        else
        {
            test_data->cur_err_count = 0;
        }
        
        /* 0.5s后判断电阻下限报警 */
        if(test_data->dis_time > 4)
        {
            if(test_data->res_value < gr_par->lower_limit)
            {
                err = ST_ERR_L;
            }
        }
        
        /* 上限报警 */
        if(ST_ERR_NONE == err)
        {
            if(test_data->res_value > gr_par->upper_limit)
            {
                err = ST_ERR_H;
            }
        }
        
        /* 测试时间到报警判断 */
        if(ST_ERR_NONE == err)
        {
            if(gr_par->testing_time && test_data->dis_time + 1 >= gr_par->testing_time)
            {
                /* 电流下限报警 */
                if(test_data->res_value < gr_par->lower_limit)
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
