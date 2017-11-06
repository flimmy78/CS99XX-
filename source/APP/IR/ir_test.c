
#define _IR_TEST_GLOBALS
#include "cs99xx_relay_motion.h"
#include "cs99xx_vref.h"
#include "DAC_VREF.h"
#include "test_com.h"
#include "ir_count.h"
#include "ir_test.h"
#include "ir_relay.h"

static uint16_t ir_zeo_t;///< 第〇阶段的累计时间
static uint16_t ir_one_t;///< 第一阶段的累计时间
static uint16_t ir_two_t;///< 第二阶段的累计时间
static uint16_t ir_thr_t;///< 第三阶段的累计时间
static uint16_t ir_for_t;///< 第四阶段的累计时间

/**
  * @brief  加载IR测试时间信息
  * @param  [in] ir_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void load_ir_data(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    ir_zeo_t = 0;
    ir_one_t = ir_par->rise_time + ir_zeo_t;
    ir_two_t = ir_one_t + ir_par->testing_time;
    ir_thr_t = ir_two_t + 0;
    ir_for_t = ir_thr_t + ir_par->interval_time;
}

/**
  * @brief  计算IR电压上升的电压步进值
  * @param  [in] ir_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void ir_count_rise_vol_step_value(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
	test_data->vol_ch_base = 0;
    test_data->vol_ch_target = ir_par->testing_voltage;
    
	if(ir_par->rise_time != 0)
	{
		test_data->vol_ch_step =
            (float)(test_data->vol_ch_target - test_data->vol_ch_base) / ir_par->rise_time;
	}
}
/**
  * @brief  计算IR电压下降的电压步进值
  * @param  [in] ir_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void ir_count_fall_vol_step_value(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
}

/**
  * @brief  计算IR在测试过程中电压变化的步进值和开始值与目标值
  * @param  [in] ir_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void ir_count_vol_ch_step(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    test_data->vol_rise_step_t = 0;
    
    switch(test_data->gradation)
    {
        case STAGE_CHARGE://充电
        case STAGE_RISE://上升
            ir_count_rise_vol_step_value(ir_par, test_data);
            break;
        case STAGE_FALL://下降
            ir_count_fall_vol_step_value(ir_par, test_data);
            break;
    }
}

/**
  * @brief  IR测试设置DA的输出值
  * @param  [in] ir_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void ir_set_da_value(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    test_data->output_da =  (u16)(ir_par->testing_voltage * test_data->out_da_k + test_data->out_da_b);
    set_output_da_vref(test_data->output_da);
}

/**
  * @brief  IR测试状态机状态计算函数
  * @param  [in] ir_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void ir_test_irq(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
	/* 当测试时间为0时 测试时间无穷大 当前没有发生失败继续 */
	if(0 == ir_par->testing_time && (!ir_test_flag.fail_cont))
	{
		/* 如果当前测试正在进行 即处于第二阶段的话就一直保持 */
		if(ir_test_flag.forever_testing)
		{
			test_data->dis_time = test_data->test_time % 10000;
            
            if(test_data->dis_time == 0)
            {
                test_data->test_time = 0;
            }
			return;
		}
		
		if(test_data->test_time <= ir_zeo_t)
		{
			test_data->dis_time = test_data->test_time - 0;
			test_data->gradation = STAGE_READY;
			ir_test_flag.vol_change_flag = 1;
			return;
		}
		/* 使程序进入第一阶段电压上升 */
		else if(test_data->test_time <= ir_one_t)/* 当上升时间设为0时，就不能进入if,因此要+1 */
		{
			test_data->dis_time = test_data->test_time - ir_zeo_t;
			test_data->gradation = STAGE_RISE;
			ir_test_flag.vol_change_flag = 1;
            
            if(test_data->dis_time == 1)
            {
                ir_count_vol_ch_step(ir_par, test_data);
            }
			return;
		}
		
        if(test_data->test_time == ir_one_t + 1)
        {
            ir_set_da_value(ir_par, test_data);
        }
        
		if(test_data->fail_num != ST_ERR_NONE)
		{
			if(test_data->test_time < ir_for_t)
			{
				/* 步间连续关闭 */
				if(!ir_par->steps_cont)
				{
                    ir_exit_test_relay_motion(ir_par, test_data);
                    test_data->test_over = 1;
                    return;
				}
				test_data->dis_time = test_data->test_time - ir_thr_t + 1;
				test_data->gradation = STAGE_INTER;
				return;
			}
			else
			{
                ir_exit_test_relay_motion(ir_par, test_data);
                test_data->test_over = 1;
                return;
			}
		}
		
		ir_test_flag.vol_change_flag = 0;
		test_data->gradation = STAGE_TEST;
		test_data->dis_time = test_data->test_time - ir_one_t;
		
		if(test_data->dis_time >= 9999)
		{
            test_data->test_time = 0;
			ir_test_flag.forever_testing = 1;/* 在启动时被清空 */
		}
		return;
	}
	
	/* 测试就绪 */
	if(test_data->test_time <= ir_zeo_t)
	{
		test_data->dis_time = test_data->test_time;
		test_data->gradation = STAGE_READY;
        ir_test_flag.vol_change_flag = 1;
	}
	/* 使程序进入第一阶段电压上升 */
	else if(test_data->test_time <= ir_one_t)
	{
		test_data->dis_time = test_data->test_time - ir_zeo_t;
		test_data->gradation = STAGE_RISE;
		ir_test_flag.vol_change_flag = 1;
        
        if(test_data->dis_time == 1)
        {
            ir_count_vol_ch_step(ir_par, test_data);
        }
	}
	/* 进入第二阶段 测试 */
	else if(test_data->test_time <= ir_two_t)
	{
		test_data->dis_time = test_data->test_time - ir_one_t;
		test_data->gradation = STAGE_TEST;
		ir_test_flag.vol_change_flag = 0;
        
        if(test_data->test_time == ir_one_t + 1)
        {
            ir_set_da_value(ir_par, test_data);
        }
	}
	/* 第三阶段 电压下降 */
	else if(test_data->test_time <= ir_thr_t)
	{
		ir_test_flag.vol_change_flag = 1;
		test_data->dis_time = test_data->test_time - ir_two_t;
		test_data->gradation = STAGE_FALL;
        
        if(test_data->dis_time == 1)
        {
            ir_count_vol_ch_step(ir_par, test_data);
        }
	}
	/* 第四阶段 间隔等待 */
	else if(test_data->test_time <= ir_for_t)
	{
        /* 关高压 */
        if(ir_test_flag.testing)
        {
            ir_test_flag.testing = 0;
            ir_exit_test_relay_motion(ir_par, test_data);
        }
        
		/* 步间连续关闭 */
		if(!ir_par->steps_cont)
		{
            ir_exit_test_relay_motion(ir_par, test_data);
            test_data->test_over = 1;
            return;
		}
        
		test_data->dis_time = test_data->test_time - ir_thr_t;
		test_data->gradation = STAGE_INTER;
	}
	/* 当前步测试结束 */
	else
	{
        /* 关高压 */
        if(ir_test_flag.testing)
        {
            ir_test_flag.testing = 0;
            ir_exit_test_relay_motion(ir_par, test_data);
        }
        
        /* 步间连续打开 */
		if(ir_par->steps_cont)
		{
            test_data->cont = 1;
            return;
        }
        
        /* 结束测试 */
        test_data->test_over = 1;
	}
}

/**
  * @brief  IR测试前的准备
  * @param  [in] ir_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void ir_test_ready(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    test_vref(ir_par->upper_limit);	/* 输出各基准 */
    
	updata_port(&ir_par->port);/* 更新端口状态 */
    ir_into_test_relay_ready(ir_par, test_data);/* 继电器动作 */
    amp_relay_ctrl_on();/* 功放继电器 */
    cs_delay_ms(10);
    
    ir_relay_ready();//开电子开关
    open_sine(4000);/* 开正弦波 */
    
    if(ir_par->rise_time == 0)
    {
        ir_set_da_value(ir_par, test_data);//送基准
    }
    
    open_test_timer();/* 开定时器 */
    
    test_data->test_over = 0;
    test_data->dis_time = 0;
    test_data->test_status = ST_WAIT;
    test_data->fail_num = ST_ERR_NONE;//默认初始化为合格
}

/**
  * @brief  运行IR测试状态机
  * @param  [in] ir_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void ir_test_details(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
	switch(test_data->gradation)
	{
		case STAGE_READY:/* 第〇阶段 */
        {
            if(test_data->ready_ok == 0)
            {
                test_data->ready_ok = 1;//标记
                ir_test_flag.testing = 1;//正在测试标记
                ir_test_flag.forever_testing = 0;
                ir_test_flag.fail_cont = 0;
                ir_test_flag.judge_err_en = ENABLE;
                load_ir_data(ir_par, test_data);
                ir_test_ready(ir_par, test_data);
            }
            break;
        }
        case STAGE_FAIL_CONT:
        {
            ir_test_flag.fail_cont = 1;//发生失败继续把这个标记置1
            
            /* 间隔时间不为0就从间隔时间开始 */
            if(ir_par->interval_time > 0)
            {
                test_data->ready_ok = 1;
                test_data->test_time = ir_thr_t + 1;/* 跳过第1阶段 */
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
  * @brief  运行IR测试
  * @param  [in] step 当前步的设置数据
  * @param  [in] next_step 下一步的设置数据 如果 为空说明当前测试的是最后一步
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void run_ir_test(NODE_STEP *step, NODE_STEP *next_step, TEST_DATA_STRUCT *test_data)
{
    IR_STRUCT *ir_par = &step->one_step.ir;
    
    /* 准备就绪完成后在计算测试状态 */
    if(test_data->ready_ok)
    {
        ir_test_irq(ir_par, test_data);
    }
    
    /* 测试状态机 */
    ir_test_details(ir_par, test_data);
    
    /* 自动换档 */
    ir_auto_find_gear();
    
    /* 出现报警不再进行采样 测试结束不再进行采样 */
    if((test_data->fail_num) == ST_ERR_NONE && (!test_data->test_over))
    {
        /* 间隔阶段不进行采样 */
        if(test_data->gradation != STAGE_INTER)
        {
            ir_count_dis_value(ir_par, test_data);
        }
    }
    
    /* 测试出现异常后清除测试就绪标记 */
    if(test_data->fail_num != ST_ERR_NONE)
    {
        test_data->ready_ok = 0;
    }
}

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
