
#include "acw_test_g.h"
#include "acw_relay.h"
#include "acw_count.h"
#include "acw_test.h"
#include "cs99xx_relay_motion.h"
#include "cs99xx_vref.h"
#include "test_com.h"
#include "DAC_VREF.h"

static uint16_t acw_g_zeo_t;///< 第〇阶段的累计时间
static uint16_t acw_g_one_t;///< 第一阶段的累计时间
static uint16_t acw_g_two_t;///< 第二阶段的累计时间
static uint16_t acw_g_thr_t;///< 第三阶段的累计时间
static uint16_t acw_g_for_t;///< 第四阶段的累计时间

void load_acw_data_g(ACW_STRUCT *acw_par, ACW_STRUCT *next_acw_par, TEST_DATA_STRUCT *test_data)
{
    /* 偏移测试无输出延时 */
    acw_g_zeo_t = 0;
    
    if(acw_par->step == 1)
    {
        acw_g_one_t = acw_par->rise_time + acw_g_zeo_t;
    }
    else
    {
        acw_g_one_t = 0 + acw_g_zeo_t;
    }
    
    acw_g_two_t = acw_g_one_t + acw_par->interval_time;/* 缓变时间 */
    acw_g_thr_t = acw_g_two_t + acw_par->testing_time;
    
    /* 下一步为空那当前步就是最后一步 */
    if(next_acw_par == NULL)
    {
        acw_g_for_t = acw_g_thr_t + acw_par->fall_time;
    }
    else
    {
        acw_g_for_t = acw_g_thr_t + 0;
    }
}

/*
 * 函数名：acw_count_vol_step_value_g
 * 描述  ：计算下一步的缓变电压步进值
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void acw_count_vol_step_value_g(ACW_STRUCT *acw_par, ACW_STRUCT *next_acw_par, TEST_DATA_STRUCT *test_data)
{
	int16_t vol_1 = 0;/* 初始电压值 */
	int16_t vol_2 = 0;/* 目标电压值 */
	uint16_t temp_inter_t = 0;/* 缓变时间 */
	
    /* 不是最后一步 */
	if(next_acw_par != NULL)
	{
        vol_1        = acw_par->testing_voltage;
        vol_2        = next_acw_par->testing_voltage;
        temp_inter_t = next_acw_par->interval_time;
	}
    /* 是最后一步 */
	else
	{
        vol_1        = acw_par->testing_voltage;
        temp_inter_t = next_acw_par->fall_time;
		vol_2 = 0;
	}
    
	test_data->vol_ch_base = vol_1;
    test_data->vol_ch_target = vol_2;
    
	if(temp_inter_t != 0)
	{
		test_data->vol_ch_step = (float)(vol_2 - vol_1) / temp_inter_t;
	}
    
    test_data->vol_rise_step_t = 0;
}

void acw_g_mode_next_step(ACW_STRUCT *acw_par, ACW_STRUCT *next_acw_par, TEST_DATA_STRUCT *test_data)
{
    close_test_timer();// 关定时器
    acw_count_vol_step_value_g(acw_par, next_acw_par, test_data);//计算缓变数据
}

/*
 * 函数名：test_irq
 * 描述  ：测试时钟控制，被调度任务调用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void acw_test_irq_g(ACW_STRUCT *acw_par, ACW_STRUCT *next_acw_par, TEST_DATA_STRUCT *test_data)
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
		
		if(test_data->test_time <= acw_g_zeo_t)
		{
			test_data->dis_time = test_data->test_time - 0;
			test_data->gradation = STAGE_READY;
			acw_test_flag.vol_change_flag = 1;
			return;
		}
		/* 使程序进入第一阶段电压上升 */
		else if(test_data->test_time <= acw_g_one_t)/* 当上升时间设为0时，就不能进入if,因此要+1 */
		{
			test_data->dis_time = test_data->test_time - acw_g_zeo_t;
			test_data->gradation = STAGE_RISE;
			acw_test_flag.vol_change_flag = 1;
            
            if(test_data->dis_time == 1)
            {
                acw_count_vol_ch_step(acw_par, test_data);
            }
			return;
		}
		
        if(test_data->test_time == acw_g_one_t + 1)
        {
            acw_set_da_value(acw_par, test_data);
        }
        
		if(test_data->fail_num != ST_ERR_NONE)
		{
            acw_exit_test_relay_motion(acw_par, test_data);
            test_data->test_over = 1;
            return;
		}
		
		acw_test_flag.vol_change_flag = 0;
		test_data->gradation = STAGE_TEST;
		test_data->dis_time = test_data->test_time - acw_g_one_t;
		
		if(test_data->dis_time >= 9999)
		{
            test_data->test_time = 0;
			acw_test_flag.forever_testing = 1;/* 在启动时被清空 */
		}
		return;
	}
	
	/* 测试就绪 */
	if(test_data->test_time <= acw_g_zeo_t)
	{
		test_data->dis_time = test_data->test_time;
		test_data->gradation = STAGE_READY;
        acw_test_flag.vol_change_flag = 1;
	}
	/* 使程序进入第一阶段电压上升 */
	else if(test_data->test_time <= acw_g_one_t)
	{
		test_data->dis_time = test_data->test_time - acw_g_zeo_t;
		test_data->gradation = STAGE_RISE;
		acw_test_flag.vol_change_flag = 1;
        
        if(test_data->dis_time == 1)
        {
            acw_count_vol_ch_step(acw_par, test_data);
        }
	}
	/* 进入第二阶段 电压缓变 */
	else if(test_data->test_time <= acw_g_two_t)
	{
		test_data->dis_time = test_data->test_time - acw_g_one_t;
		test_data->gradation = STAGE_CHANGE;
		acw_test_flag.vol_change_flag = 1;
	}
	/* 第三阶段 测试 */
	else if(test_data->test_time <= acw_g_thr_t)
	{
		test_data->dis_time = test_data->test_time - acw_g_two_t;
		test_data->gradation = STAGE_TEST;
		acw_test_flag.vol_change_flag = 0;
        
        if(test_data->test_time == acw_g_one_t + 1)
        {
            acw_set_da_value(acw_par, test_data);
        }
	}
	/* 第四阶段 电压下降 */
	else if(test_data->test_time <= acw_g_for_t)
	{
		acw_test_flag.vol_change_flag = 1;
		test_data->dis_time = test_data->test_time - acw_g_thr_t;
		test_data->gradation = STAGE_FALL;
        
        if(test_data->dis_time == 1)
        {
            acw_count_vol_ch_step(acw_par, test_data);
        }
	}
	/* 当前步测试结束 */
	else
	{
        /* 不是最后一步就继续测试 */
        if(next_acw_par != NULL)
        {
            test_data->cont = 1;
            acw_g_mode_next_step(acw_par, next_acw_par, test_data);
            test_data->gradation = STAGE_RE_READY;//切换到再次就绪状态
            test_data->ready_ok = 0;//清
            return;
        }
        
        acw_clear_test_data_fall_time_timeout(acw_par, test_data);
        acw_exit_test_relay_motion(acw_par, test_data);
        test_data->test_over = 1;
	}
}

void acw_test_ready_g(ACW_STRUCT *acw_par, ACW_STRUCT *next_acw_par, TEST_DATA_STRUCT *test_data)
{
    test_vref(acw_par->upper_limit);	/* 输出各基准 */
    
	updata_port(&acw_par->port);/* 更新端口状态 */
    acw_into_test_relay_ready(acw_par, test_data);/* 继电器动作 */
    amp_relay_ctrl_on();/* 功放继电器 */
    cs_delay_ms(10);
    
    acw_relay_ready();//开电子开关
    
    if(acw_par->rise_time == 0)
    {
        acw_set_da_value(acw_par, test_data);//送基准
    }
    
    open_sine(acw_par->output_freq);/* 开正弦波 */
    
    test_data->test_over = 0;
    test_data->dis_time = 0;
    test_data->test_status = ST_WAIT;
    test_data->fail_num = ST_ERR_NONE;//默认初始化为合格
    
    open_test_timer();/* 开定时器 */
}

void acw_test_details_g(ACW_STRUCT *acw_par, ACW_STRUCT *next_acw_par, TEST_DATA_STRUCT *test_data)
{
	switch(test_data->gradation)
	{
		case STAGE_READY:/* 第〇阶段 */
        {
            if(test_data->ready_ok == 0)
            {
                test_data->ready_ok = 1;//就绪标记
                acw_test_flag.err_real = 0;/* acw 真实电流报警  */
                acw_test_flag.testing = 1;
                acw_test_flag.forever_testing = 0;
                load_acw_data_g(acw_par, next_acw_par, test_data);
                acw_test_ready_g(acw_par, next_acw_par, test_data);
            }
            break;
        }
        case STAGE_RE_READY:
        {
            if(test_data->ready_ok == 0)
            {
                test_data->ready_ok = 1;//就绪标记
                load_acw_data_g(acw_par, next_acw_par, test_data);
                shift_acw_cur_gear(acw_par->gear_i);
                test_vref(acw_par->upper_limit);/* 输出各基准 */
                cs_delay_ms(50);
                
                test_data->test_time = acw_g_zeo_t;/* 跳过第1阶段 */
                test_data->gradation = STAGE_CHANGE;/* 进入电压缓变阶段 这个很重要 */
                open_test_timer();/* 开定时器 */
            }
            break;
        }
		case STAGE_RISE:/* 第一阶段 电压上升 */
            test_data->test_status = ST_VOL_RISE;
			break;
		case STAGE_CHANGE:/* 第二阶段 电压缓变 */
			test_data->test_status = ST_CHANGE;
			break;
		case STAGE_TEST:/* 第三阶段 正在测试 */
            test_data->test_status = ST_TESTING;
			break;
		case STAGE_FALL:/* 第四阶段 电压下降 */
            test_data->test_status = ST_VOL_FALL;
			break;
	}
}

void run_acw_test_g(NODE_STEP *step, NODE_STEP *next_step, TEST_DATA_STRUCT *test_data)
{
    ACW_STRUCT *acw_par = &step->one_step.acw;
    ACW_STRUCT *next_acw_par = &next_step->one_step.acw;
    
    if(test_data->ready_ok)
    {
        acw_test_irq_g(acw_par, next_acw_par, test_data);
    }
    
    if(!test_data->cont)
    {
        acw_test_details_g(acw_par, next_acw_par, test_data);
    }
    
    if((test_data->fail_num == ST_ERR_NONE) && (!test_data->test_over))
    {
        acw_count_dis_value(acw_par, test_data);
    }
}


/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
