
#define _ACW_TEST_GLOBALS
#include "acw_test.h"
#include "acw_relay.h"
#include "cs99xx_relay_motion.h"
#include "cs99xx_vref.h"
#include "test_com.h"
#include "acw_count.h"
#include "DAC_VREF.h"

static uint16_t acw_zeo_t;///< 第〇阶段的累计时间
static uint16_t acw_one_t;///< 第一阶段的累计时间
static uint16_t acw_two_t;///< 第二阶段的累计时间
static uint16_t acw_thr_t;///< 第三阶段的累计时间
static uint16_t acw_for_t;///< 第四阶段的累计时间

void load_acw_data(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    /* 偏移测试无输出延时 */
    acw_zeo_t = 0;
    
    acw_one_t = acw_par->rise_time + acw_zeo_t;
    acw_two_t = acw_par->rise_time + acw_par->testing_time + acw_zeo_t;
    acw_thr_t = acw_par->rise_time + acw_par->testing_time + acw_par->fall_time + acw_zeo_t;
    acw_for_t = acw_par->rise_time + acw_par->testing_time + acw_par->fall_time + acw_par->interval_time + acw_zeo_t;
}

/*
 * 函数名：count_rise_vol_step_value
 * 描述  ：计算当前步电压上升的电压步进值
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
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
 * 函数名：count_vol_step_value
 * 描述  ：计算当前步电压下降的电压步进值
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
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
        case STAGE_CHARGE://充电
        case STAGE_RISE://上升
            acw_count_rise_vol_step_value(acw_par, test_data);
            break;
        case STAGE_FALL://下降
            acw_count_fall_vol_step_value(acw_par, test_data);
            break;
    }
}

void clear_test_data_fall_time_timeout(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    /* 如果下降时间不为0就清0测试电压和电流 */
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
 * 函数名：test_irq
 * 描述  ：测试时钟控制，被调度任务调用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void acw_test_irq(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
	/* 当测试时间为0时 测试时间无穷大 */
	if(0 == acw_par->testing_time)
	{
		/* 如果当前测试正在进行 即处于第二阶段的话就一直保持 */
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
		/* 使程序进入第一阶段电压上升 */
		else if(test_data->test_time <= acw_one_t)/* 当上升时间设为0时，就不能进入if,因此要+1 */
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
				/* 步间连续关闭 */
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
			acw_test_flag.forever_testing = 1;/* 在启动时被清空 */
		}
		return;
	}
	
	/* 测试就绪 */
	if(test_data->test_time <= acw_zeo_t)
	{
		test_data->dis_time = test_data->test_time;
		test_data->gradation = STAGE_READY;
        acw_test_flag.vol_change_flag = 1;
	}
	/* 使程序进入第一阶段电压上升 */
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
	/* 进入第二阶段 测试 */
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
	/* 第三阶段 电压下降 */
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
	/* 第四阶段 间隔等待 */
	else if(test_data->test_time <= acw_for_t)
	{
        clear_test_data_fall_time_timeout(acw_par, test_data);
        
        /* 关高压 */
        if(acw_test_flag.testing == 1)
        {
            acw_test_flag.testing = 0;
            acw_exit_test_relay_motion(acw_par, test_data);
        }
        
		/* 步间连续关闭 */
		if(!acw_par->steps_cont)
		{
            acw_exit_test_relay_motion(acw_par, test_data);
            test_data->test_over = 1;
            return;
		}
        
		test_data->dis_time = test_data->test_time - acw_thr_t;
		test_data->gradation = STAGE_INTER;
	}
	/* 当前步测试结束 */
	else
	{
        clear_test_data_fall_time_timeout(acw_par, test_data);
        
        /* 关高压 */
        if(acw_test_flag.testing == 1)
        {
            acw_test_flag.testing = 0;
            acw_exit_test_relay_motion(acw_par, test_data);
        }
        
        /* 步间连续打开 */
		if(acw_par->steps_cont)
		{
            test_data->cont = 1;
            return;
        }
        
        /* 结束测试 */
        test_data->test_over = 1;
	}
}

void acw_test_ready(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    test_vref(acw_par->upper_limit);	/* 输出各基准 */
    
	updata_port(&acw_par->port);/* 更新端口状态 */
    acw_into_test_relay_ready(acw_par, test_data);/* 继电器动作 */
    amp_relay_ctrl_on();/* 功放继电器 */
    cs_delay_ms(10);
    
    acw_relay_ready();//开电子开关
    
    open_sine(acw_par->output_freq);/* 开正弦波 */
    
    if(acw_par->rise_time == 0)
    {
        acw_set_da_value(acw_par, test_data);//送基准
    }
    
    open_test_timer();/* 开定时器 */
    
	test_data->err_real = 0;/* acw 真实电流报警  */
    acw_test_flag.forever_testing = 0;
    test_data->test_over = 0;
    test_data->dis_time = 0;
    test_data->test_status = ST_WAIT;
    test_data->fail_num = ST_ERR_NONE;//默认初始化为合格
}

void acw_test_details(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
	switch(test_data->gradation)
	{
		case STAGE_READY:/* 第〇阶段 */
        {
            if(test_data->ready_ok == 0)
            {
                test_data->ready_ok = 1;//标记
                acw_test_flag.testing = 1;//正在测试标记
                acw_test_flag.judge_err_en = ENABLE;
                load_acw_data(acw_par, test_data);
                acw_test_ready(acw_par, test_data);
            }
            break;
        }
        case STAGE_FAIL_CONT:
        {
            /* 间隔时间不为0就从间隔时间开始 */
            if(acw_par->interval_time > 0)
            {
                test_data->ready_ok == 1;
                test_data->test_time = acw_thr_t + 1;/* 跳过第1阶段 */
                test_data->gradation = STAGE_INTER;/* 进入间隔等待 */
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

void run_acw_test(NODE_STEP *step, NODE_STEP *next_step, TEST_DATA_STRUCT *test_data)
{
    ACW_STRUCT *acw_par = &step->one_step.acw;
    
    /* 准备就绪完成后在计算测试阶段 */
    if(test_data->ready_ok == 1)
    {
        acw_test_irq(acw_par, test_data);
    }
    
    acw_test_details(acw_par, test_data);
    
    /* 出现报警不再进行采样 测试结束不再进行采样 */
    if(test_data->fail_num == ST_ERR_NONE && test_data->test_over == 0)
    {
        /* 间隔阶段不进行采样 */
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

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
