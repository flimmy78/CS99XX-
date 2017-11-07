
#define _DC_GR_TEST_GLOBALS
#include "cs99xx_relay_motion.h"
#include "cs99xx_vref.h"
#include "DAC_VREF.h"
#include "test_com.h"
#include "dc_gr_count.h"
#include "dc_gr_test.h"
#include "dc_gr_relay.h"

static uint16_t dc_gr_zeo_t;///< 第〇阶段的累计时间
static uint16_t dc_gr_one_t;///< 第一阶段的累计时间
static uint16_t dc_gr_two_t;///< 第二阶段的累计时间
static uint16_t dc_gr_thr_t;///< 第三阶段的累计时间
static uint16_t dc_gr_for_t;///< 第四阶段的累计时间

/**
  * @brief  加载GR测试时间信息
  * @param  [in] dc_gr_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void load_dc_gr_data(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data)
{
    /* 偏移测试无输出延时 */
    dc_gr_zeo_t = 0;
    dc_gr_one_t = 0 + dc_gr_zeo_t;
    dc_gr_two_t = dc_gr_one_t + dc_gr_par->testing_time;
    dc_gr_thr_t = dc_gr_two_t + 0;
    dc_gr_for_t = dc_gr_thr_t + dc_gr_par->interval_time;
}

/**
  * @brief  GR测试设置DA的输出值
  * @param  [in] dc_gr_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void dc_gr_set_da_value(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data)
{
    test_data->output_da = (uint16_t)(dc_gr_par->testing_cur * test_data->out_da_k + test_data->out_da_b);
    set_output_da_vref(test_data->output_da);
}

/**
  * @brief  GR测试状态机状态计算函数
  * @param  [in] dc_gr_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void dc_gr_test_irq(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data)
{
	/* 当测试时间为0时 测试时间无穷大 当前没有发生失败继续 */
	if(0 == dc_gr_par->testing_time && (!dc_gr_test_flag.fail_cont))
	{
		/* 如果当前测试正在进行 即处于第二阶段的话就一直保持 */
		if(dc_gr_test_flag.forever_testing)
		{
			test_data->dis_time = test_data->test_time % 10000;
            
            if(test_data->dis_time == 0)
            {
                test_data->test_time = 0;
            }
			return;
		}
		
		if(test_data->test_time <= dc_gr_zeo_t)
		{
			test_data->dis_time = test_data->test_time - 0;
			test_data->gradation = STAGE_READY;
			dc_gr_test_flag.vol_change_flag = 1;
			return;
		}
		/* 使程序进入第一阶段电压上升 */
		else if(test_data->test_time <= dc_gr_one_t)/* 当上升时间设为0时，就不能进入if,因此要+1 */
		{
			test_data->dis_time = test_data->test_time - dc_gr_zeo_t;
			test_data->gradation = STAGE_RISE;
			dc_gr_test_flag.vol_change_flag = 1;
			return;
		}
		
        if(test_data->test_time == dc_gr_one_t + 1)
        {
            dc_gr_set_da_value(dc_gr_par, test_data);
        }
        
		if(test_data->fail_num != ST_ERR_NONE)
		{
			if(test_data->test_time < dc_gr_for_t)
			{
				/* 步间连续关闭 */
				if(!dc_gr_par->steps_cont)
				{
                    dc_gr_exit_test_relay_motion(dc_gr_par, test_data);
                    test_data->test_over = 1;
                    return;
				}
				test_data->dis_time = test_data->test_time - dc_gr_thr_t + 1;
				test_data->gradation = STAGE_INTER;
				return;
			}
			else
			{
                dc_gr_exit_test_relay_motion(dc_gr_par, test_data);
                test_data->test_over = 1;
                return;
			}
		}
		
		dc_gr_test_flag.vol_change_flag = 0;
		test_data->gradation = STAGE_TEST;
		test_data->dis_time = test_data->test_time - dc_gr_one_t;
		
		if(test_data->dis_time >= 9999)
		{
            test_data->test_time = 0;
			dc_gr_test_flag.forever_testing = 1;/* 在启动时被清空 */
		}
		return;
	}
	
	/* 测试就绪 */
	if(test_data->test_time <= dc_gr_zeo_t)
	{
		test_data->dis_time = test_data->test_time;
		test_data->gradation = STAGE_READY;
        dc_gr_test_flag.vol_change_flag = 1;
	}
	/* 使程序进入第一阶段电压上升 */
	else if(test_data->test_time <= dc_gr_one_t)
	{
		test_data->dis_time = test_data->test_time - dc_gr_zeo_t;
		test_data->gradation = STAGE_RISE;
		dc_gr_test_flag.vol_change_flag = 1;
	}
	/* 进入第二阶段 测试 */
	else if(test_data->test_time <= dc_gr_two_t)
	{
		test_data->dis_time = test_data->test_time - dc_gr_one_t;
		test_data->gradation = STAGE_TEST;
		dc_gr_test_flag.vol_change_flag = 0;
        
        if(test_data->test_time == dc_gr_one_t + 1)
        {
            dc_gr_set_da_value(dc_gr_par, test_data);
        }
	}
	/* 第三阶段 电压下降 */
	else if(test_data->test_time <= dc_gr_thr_t)
	{
		dc_gr_test_flag.vol_change_flag = 1;
		test_data->dis_time = test_data->test_time - dc_gr_two_t;
		test_data->gradation = STAGE_FALL;
	}
	/* 第四阶段 间隔等待 */
	else if(test_data->test_time <= dc_gr_for_t)
	{
        /* 关高压 */
        if(dc_gr_test_flag.testing)
        {
            dc_gr_test_flag.testing = 0;
            dc_gr_exit_test_relay_motion(dc_gr_par, test_data);
        }
        
		/* 步间连续关闭 */
		if(!dc_gr_par->steps_cont)
		{
            dc_gr_exit_test_relay_motion(dc_gr_par, test_data);
            test_data->test_over = 1;
            return;
		}
        
		test_data->dis_time = test_data->test_time - dc_gr_thr_t;
		test_data->gradation = STAGE_INTER;
	}
	/* 当前步测试结束 */
	else
	{
        /* 关高压 */
        if(dc_gr_test_flag.testing)
        {
            dc_gr_test_flag.testing = 0;
            dc_gr_exit_test_relay_motion(dc_gr_par, test_data);
        }
        
        /* 步间连续打开 */
		if(dc_gr_par->steps_cont)
		{
            test_data->cont = 1;
            return;
        }
        
        /* 结束测试 */
        test_data->test_over = 1;
	}
}

/**
  * @brief  GR测试前的准备
  * @param  [in] dc_gr_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void dc_gr_test_ready(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data)
{
    test_vref(dc_gr_par->upper_limit);	/* 输出各基准 */
    
    dc_gr_into_test_relay_ready(dc_gr_par, test_data);/* 继电器动作 */
    amp_relay_ctrl_on();/* 功放继电器 */
    cs_delay_ms(10);
    
    dc_gr_relay_ready();//开电子开关
    open_sine(dc_gr_par->output_freq);/* 开正弦波 */
    dc_gr_set_da_value(dc_gr_par, test_data);//送基准
    open_test_timer();/* 开定时器 */
    
    test_data->test_over = 0;
    test_data->dis_time = 0;
    test_data->test_status = ST_WAIT;
    test_data->fail_num = ST_ERR_NONE;//默认初始化为合格
}

/**
  * @brief  运行GR测试状态机
  * @param  [in] dc_gr_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void dc_gr_test_details(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data)
{
	switch(test_data->gradation)
	{
		case STAGE_READY:/* 第〇阶段 */
        {
            if(test_data->ready_ok == 0)
            {
                test_data->ready_ok = 1;//标记
                dc_gr_test_flag.testing = 1;//正在测试标记
                dc_gr_test_flag.forever_testing = 0;
                dc_gr_test_flag.fail_cont = 0;
                dc_gr_test_flag.judge_err_en = ENABLE;
                load_dc_gr_data(dc_gr_par, test_data);
                dc_gr_test_ready(dc_gr_par, test_data);
            }
            break;
        }
        case STAGE_FAIL_CONT:/* 失败断续 */
        {
            dc_gr_test_flag.fail_cont = 1;//发生失败继续把这个标记置1
            
            /* 间隔时间不为0就从间隔时间开始 */
            if(dc_gr_par->interval_time > 0)
            {
                test_data->ready_ok = 1;
                test_data->test_time = dc_gr_thr_t + 1;/* 跳过第1阶段 */
                test_data->gradation = STAGE_INTER;/* 进入间隔等待 */
                exit_sw();//开中断
                open_test_timer();/* 开定时器 */
            }
            /* 间隔时间为0就继续下一步测试 */
            else
            {
                test_data->cont = 1;
            }
            break;
        }
		case STAGE_RISE:/* 第一阶段 电压上升 */
            test_data->test_status = ST_VOL_RISE;
			break;
		case STAGE_TEST:/* 第二阶段 正在测试 */
            test_data->test_status = ST_TESTING;
			break;
		case STAGE_FALL:/* 第三阶段 电压下降 */
            test_data->test_status = ST_VOL_FALL;
			break;
		case STAGE_INTER:/* 第四阶段 间隔等待 */
            test_data->test_status = ST_INTER_WAIT;
			break;
	}
}
/**
  * @brief  运行GR测试
  * @param  [in] step 当前步的设置数据
  * @param  [in] next_step 下一步的设置数据 如果 为空说明当前测试的是最后一步
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void run_dc_gr_test(NODE_STEP *step, NODE_STEP *next_step, TEST_DATA_STRUCT *test_data)
{
    GR_STRUCT *dc_gr_par = &step->one_step.gr;
    
    /* 准备就绪完成后在计算测试状态 */
    if(test_data->ready_ok)
    {
        dc_gr_test_irq(dc_gr_par, test_data);
    }
    
    /* 测试状态机 */
    dc_gr_test_details(dc_gr_par, test_data);
    
    /* 出现报警不再进行采样 测试结束不再进行采样 */
    if((test_data->fail_num) == ST_ERR_NONE && (!test_data->test_over))
    {
        /* 间隔阶段不进行采样 */
        if(test_data->gradation != STAGE_INTER)
        {
            dc_gr_count_dis_value(dc_gr_par, test_data);
        }
    }
    
    /* 测试出现异常后清除测试就绪标记 */
    if(test_data->fail_num != ST_ERR_NONE)
    {
        test_data->ready_ok = 0;
    }
}

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
