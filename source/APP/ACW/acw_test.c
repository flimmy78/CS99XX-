
#define _ACW_TEST_GLOBALS
#include "acw_test.h"
#include "acw_relay.h"
#include "serve_test.h"
#include "calibration.h"
#include "SPI_CPLD.H"
#include "SysTemConfig.h"

static uint16_t acw_zeo_t;///< 第〇阶段的累计时间
static uint16_t acw_one_t;///< 第一阶段的累计时间
static uint16_t acw_two_t;///< 第二阶段的累计时间
static uint16_t acw_thr_t;///< 第三阶段的累计时间
static uint16_t acw_for_t;///< 第四阶段的累计时间
static uint16_t acw_fiv_t;///< 第五阶段的累计时间
//static uint16_t acw_tes_t;///< 当前步测试时间
//static uint16_t acw_ris_t;///< 上升时间
//static uint16_t acw_sta_t;///< 稳压时间
//static uint16_t acw_fal_t;///< 下降时间
//static uint16_t acw_int_t;///< 间隔时间
//static uint16_t acw_cha_t;///< 缓变时间
//static uint8_t acw_cur_gear;///<ACW电流档位
//static NODE_STEP *acw_test_par;///<ACW测试参数
//uint8_t acw_step;///<ACW步骤编号

//float acw_vol_ave = 0;///<ACW测试电压
//float acw_cur_ave = 0;///<ACW测试电流

void acw_start_test(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    acw_into_test_relay_ready(acw_par, test_data);
    
}

void load_acw_data(TEST_FILE *test_file, ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    /* 偏移测试无输出延时 */
    acw_zeo_t = 0;
    
    if(test_file->work_mode == N_MODE)
    {
        acw_one_t = acw_par->rise_time + acw_zeo_t;
        acw_two_t = acw_par->rise_time + acw_par->testing_time + acw_zeo_t;
        acw_thr_t = acw_par->rise_time + acw_par->testing_time + acw_par->fall_time + acw_zeo_t;
        acw_for_t = acw_par->rise_time + acw_par->testing_time + acw_par->fall_time + acw_par->interval_time + acw_zeo_t;
        
    }
    else if(test_file->work_mode == G_MODE)
    {
        if(cur_step == 1)
        {
            acw_one_t = acw_par->rise_time + acw_zeo_t;
        }
        else
        {
            acw_one_t = 0 + acw_zeo_t;
        }
        
        acw_two_t = acw_one_t + acw_par->interval_time;/* 缓变时间 */
        acw_thr_t = acw_two_t + acw_par->testing_time;
        
        if(cur_step == test_file->total)
        {
            acw_for_t = acw_thr_t + acw_par->fall_time;
        }
        else
        {
            acw_for_t = acw_thr_t + 0;
        }
    }
}
/*
 * 函数名：test_irq
 * 描述  ：测试时钟控制，被调度任务调用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void acw_test_irq(TEST_FILE *test_file, ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    uint16_t l_dis_time = 0;
    uint16_t test_status;
    
	/* 当测试时间为0时 测试时间无穷大 */
	if(0 == acw_par->testing_time)
	{
		/* 如果当前测试正在进行 即处于第二阶段的话就一直保持 */
		if(acw_test_flag.testing)
		{
			test_status = STAGE_TEST;
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
			acw_test_flag.gradation = STAGE_READY;
			acw_test_flag.dis_status = TEST_WAIT_INDEX;
			acw_test_flag.vol_change_flag = 1;
			return;
		}
		/* 使程序进入第一阶段电压上升 */
		else if(g_test_time <= acw_one_t)/* 当上升时间设为0时，就不能进入if,因此要+1 */
		{
            count_rise_vol_step_value();/* 计算电压上升步进值 */
			test_data->dis_time = test_data->test_time - acw_zeo_t;
			acw_test_flag.gradation = STAGE_RISE;
			acw_test_flag.dis_status = TEST_RISE_INDEX;
			acw_test_flag.vol_change_flag = 1;
            
            if(test_data->dis_time == 1)
            {
                count_vol_ch_step();
            }
			return;
		}
		
        if(test_data->test_time == acw_one_t + 1)
        {
            open_hv();
        }
        
		if(test_data->fail_num != ERR_NONE)
		{
			if(test_data->test_time < acw_for_t)
			{
				/* 步间连续关闭 */
				if(!acw_par->steps_cont)
				{
                    acw_test_flag.test_over = 1;
                    return;
				}
				test_data->dis_time = test_data->test_time - acw_thr_t + 1;
				acw_test_flag.gradation = STAGE_INTER;
				acw_test_flag.dis_status = TEST_INTER_INDEX;
				return;
			}
			else
			{
                acw_test_flag.test_over = 1;
                return;
			}
		}
		
		acw_test_flag.vol_change_flag = 0;
		acw_test_flag.gradation = STAGE_TEST;
		acw_test_flag.dis_status = TEST_TEST_INDEX;
		test_data->dis_time = test_data->test_time - acw_one_t;
		
		if(test_data->dis_time >= 9999)
		{
            test_data->test_time = 0;
			acw_test_flag.testing = 1;/* 在启动时被清空 */
		}
		return;
	}
	
	/* 测试就绪 */
	if(test_data->test_time <= acw_zeo_t)
	{
		test_data->dis_time = test_data->test_time;
		acw_test_flag.gradation = STAGE_READY;
		acw_test_flag.dis_status = TEST_WAIT_INDEX;
        acw_test_flag.vol_change_flag = 1;
	}
	/* 使程序进入第一阶段电压上升 */
	else if(test_data->test_time <= acw_one_t)
	{
		test_data->dis_time = test_data->test_time - acw_zeo_t;
		acw_test_flag.gradation = STAGE_RISE;
		acw_test_flag.dis_status = TEST_RISE_INDEX;
		acw_test_flag.vol_change_flag = 1;
        
        if(test_data->dis_time == 1)
        {
            count_vol_ch_step();
        }
	}
	/* 进入第二阶段 测试 */
	else if(test_data->test_time <= acw_two_t)
	{
		test_data->dis_time = test_data->test_time - acw_one_t;
		acw_test_flag.gradation = STAGE_TEST;
		acw_test_flag.dis_status = TEST_TEST_INDEX;
		acw_test_flag.vol_change_flag = 0;
        
        if(test_data->test_time == acw_one_t + 1)
        {
            open_hv();
        }
	}
	/* 第三阶段 电压下降 */
	else if(test_data->test_time <= acw_thr_t)
	{
		acw_test_flag.vol_change_flag = 1;
		acw_test_flag.dis_status = TEST_FALL_INDEX;
		test_data->dis_time = test_data->test_time - acw_two_t;
		acw_test_flag.gradation = STAGE_FALL;
        
        if(test_data->dis_time == 1)
        {
            count_vol_ch_step();
        }
	}
	/* 第四阶段 间隔等待 */
	else if(test_data->test_time <= acw_for_t)
	{
		if(sys_par.fail_mode == FAIL_MODE_FPDFC && acw_par->step == 1)
		{
			if(!CUR_FAIL)
			{
                acw_test_flag.test_over = 1;
				return;
			}
		}
        
		/* 步间连续关闭 */
		if(!acw_par->steps_cont)
		{
            acw_test_flag.test_over = 1;
            return;
		}
        
		test_data->dis_time = test_data->test_time - acw_thr_t;
		acw_test_flag.gradation = STAGE_INTER;
		acw_test_flag.dis_status = TEST_INTER_INDEX;
	}
	/* 当前步测试结束 */
	else
	{
        acw_test_flag.test_over = 1;
	}
}

void acw_test_details(TEST_FILE *test_file, ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
	switch(acw_test_flag.gradation)
	{
		case STAGE_READY:/* 第〇阶段 */
        {
            acw_test_flag.testing = 1;
            test_data->test_status = ST_WAIT;
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
            count_fall_vol_step_value();/* 计算电压下降步进值 */
			break;
		case STAGE_INTER:/* 第四阶段 间隔等待 */
        {
            test_data->test_status = ST_INTER_WAIT;
			
			if(acw_test_flag.testing == 1)
			{
				acw_test_flag.testing = 0;
                
                /* 关高压 */
                irq_stop_relay_motion();
				acw_exit_test_relay_motion(acw_par, test_data);
				
                /* 如果下降时间不为0就清0测试电压和电流 */
				if(acw_par->fall_time > 0)
				{
                    test_data->vol_value = 0;
                    test_data->cur_value = 0;
                    test_data->real_value = 0;
				}
			}
			break;
        }
	}
}

void run_acw_test(TEST_FILE *test_file, ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    acw_test_details(test_file, acw_par, test_data);
    count_acw_dis_value(acw_par, test_data);
}

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
