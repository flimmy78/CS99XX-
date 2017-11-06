
#include "ir_test_g.h"
#include "ir_relay.h"
#include "ir_count.h"
#include "ir_test.h"
#include "cs99xx_relay_motion.h"
#include "cs99xx_vref.h"
#include "test_com.h"
#include "DAC_VREF.h"

static uint16_t ir_g_zeo_t;///< �ک��׶ε��ۼ�ʱ��
static uint16_t ir_g_one_t;///< ��һ�׶ε��ۼ�ʱ��
static uint16_t ir_g_two_t;///< �ڶ��׶ε��ۼ�ʱ��
static uint16_t ir_g_thr_t;///< �����׶ε��ۼ�ʱ��
static uint16_t ir_g_for_t;///< ���Ľ׶ε��ۼ�ʱ��

void load_ir_data_g(IR_STRUCT *ir_par, IR_STRUCT *next_ir_par, TEST_DATA_STRUCT *test_data)
{
    /* ƫ�Ʋ����������ʱ */
//    ir_g_zeo_t = 0;
//    
//    if(ir_par->step == 1)
//    {
//        ir_g_one_t = ir_par->rise_time + ir_g_zeo_t;
//    }
//    else
//    {
//        ir_g_one_t = 0 + ir_g_zeo_t;
//    }
//    
//    ir_g_two_t = ir_g_one_t + ir_par->interval_time;/* ����ʱ�� */
//    ir_g_thr_t = ir_g_two_t + ir_par->testing_time;
//    
//    /* ��һ��Ϊ���ǵ�ǰ���������һ�� */
//    if(next_ir_par == NULL)
//    {
//        ir_g_for_t = ir_g_thr_t + ir_par->fall_time;
//    }
//    else
//    {
//        ir_g_for_t = ir_g_thr_t + 0;
//    }
}

/*
 * ��������ir_count_vol_step_value_g
 * ����  ��������һ���Ļ����ѹ����ֵ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void ir_count_vol_step_value_g(IR_STRUCT *ir_par, IR_STRUCT *next_ir_par, TEST_DATA_STRUCT *test_data)
{
//	int16_t vol_1 = 0;/* ��ʼ��ѹֵ */
//	int16_t vol_2 = 0;/* Ŀ���ѹֵ */
//	uint16_t temp_inter_t = 0;/* ����ʱ�� */
//	
//    /* �������һ�� */
//	if(next_ir_par != NULL)
//	{
//        vol_1        = ir_par->testing_voltage;
//        vol_2        = next_ir_par->testing_voltage;
//        temp_inter_t = next_ir_par->interval_time;
//	}
//    /* �����һ�� */
//	else
//	{
//        vol_1        = ir_par->testing_voltage;
//        temp_inter_t = next_ir_par->fall_time;
//		vol_2 = 0;
//	}
//    
//	test_data->vol_ch_base = vol_1;
//    test_data->vol_ch_target = vol_2;
//    
//	if(temp_inter_t != 0)
//	{
//		test_data->vol_ch_step = (float)(vol_2 - vol_1) / temp_inter_t;
//	}
//    
//    test_data->vol_rise_step_t = 0;
}

void ir_g_mode_next_step(IR_STRUCT *ir_par, IR_STRUCT *next_ir_par, TEST_DATA_STRUCT *test_data)
{
    close_test_timer();// �ض�ʱ��
    ir_count_vol_step_value_g(ir_par, next_ir_par, test_data);//���㻺������
}

/*
 * ��������test_irq
 * ����  ������ʱ�ӿ��ƣ��������������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void ir_test_irq_g(IR_STRUCT *ir_par, IR_STRUCT *next_ir_par, TEST_DATA_STRUCT *test_data)
{
	/* ������ʱ��Ϊ0ʱ ����ʱ������� */
	if(0 == ir_par->testing_time)
	{
		/* �����ǰ�������ڽ��� �����ڵڶ��׶εĻ���һֱ���� */
		if(ir_test_flag.forever_testing)
		{
			test_data->dis_time = test_data->test_time % 10000;
            
            if(test_data->dis_time == 0)
            {
                test_data->test_time = 0;
            }
			return;
		}
		
		if(test_data->test_time <= ir_g_zeo_t)
		{
			test_data->dis_time = test_data->test_time - 0;
			test_data->gradation = STAGE_READY;
			ir_test_flag.vol_change_flag = 1;
			return;
		}
		/* ʹ��������һ�׶ε�ѹ���� */
		else if(test_data->test_time <= ir_g_one_t)/* ������ʱ����Ϊ0ʱ���Ͳ��ܽ���if,���Ҫ+1 */
		{
			test_data->dis_time = test_data->test_time - ir_g_zeo_t;
			test_data->gradation = STAGE_RISE;
			ir_test_flag.vol_change_flag = 1;
            
            if(test_data->dis_time == 1)
            {
                ir_count_vol_ch_step(ir_par, test_data);
            }
			return;
		}
		
        if(test_data->test_time == ir_g_one_t + 1)
        {
            ir_set_da_value(ir_par, test_data);
        }
        
		if(test_data->fail_num != ST_ERR_NONE)
		{
            ir_exit_test_relay_motion(ir_par, test_data);
            test_data->test_over = 1;
            return;
		}
		
		ir_test_flag.vol_change_flag = 0;
		test_data->gradation = STAGE_TEST;
		test_data->dis_time = test_data->test_time - ir_g_one_t;
		
		if(test_data->dis_time >= 9999)
		{
            test_data->test_time = 0;
			ir_test_flag.forever_testing = 1;/* ������ʱ����� */
		}
		return;
	}
	
	/* ���Ծ��� */
	if(test_data->test_time <= ir_g_zeo_t)
	{
		test_data->dis_time = test_data->test_time;
		test_data->gradation = STAGE_READY;
        ir_test_flag.vol_change_flag = 1;
	}
	/* ʹ��������һ�׶ε�ѹ���� */
	else if(test_data->test_time <= ir_g_one_t)
	{
		test_data->dis_time = test_data->test_time - ir_g_zeo_t;
		test_data->gradation = STAGE_RISE;
		ir_test_flag.vol_change_flag = 1;
        
        if(test_data->dis_time == 1)
        {
            ir_count_vol_ch_step(ir_par, test_data);
        }
	}
	/* ����ڶ��׶� ��ѹ���� */
	else if(test_data->test_time <= ir_g_two_t)
	{
		test_data->dis_time = test_data->test_time - ir_g_one_t;
		test_data->gradation = STAGE_CHANGE;
		ir_test_flag.vol_change_flag = 1;
	}
	/* �����׶� ���� */
	else if(test_data->test_time <= ir_g_thr_t)
	{
		test_data->dis_time = test_data->test_time - ir_g_two_t;
		test_data->gradation = STAGE_TEST;
		ir_test_flag.vol_change_flag = 0;
        
        if(test_data->test_time == ir_g_one_t + 1)
        {
            ir_set_da_value(ir_par, test_data);
        }
	}
	/* ���Ľ׶� ��ѹ�½� */
	else if(test_data->test_time <= ir_g_for_t)
	{
		ir_test_flag.vol_change_flag = 1;
		test_data->dis_time = test_data->test_time - ir_g_thr_t;
		test_data->gradation = STAGE_FALL;
        
        if(test_data->dis_time == 1)
        {
            ir_count_vol_ch_step(ir_par, test_data);
        }
	}
	/* ��ǰ�����Խ��� */
	else
	{
        /* �������һ���ͼ������� */
        if(next_ir_par != NULL)
        {
            test_data->cont = 1;
            ir_g_mode_next_step(ir_par, next_ir_par, test_data);
            test_data->gradation = STAGE_RE_READY;//�л����ٴξ���״̬
            test_data->ready_ok = 0;//��
            return;
        }
        
        ir_clear_test_data_fall_time_timeout(ir_par, test_data);
        ir_exit_test_relay_motion(ir_par, test_data);
        test_data->test_over = 1;
	}
}

void ir_test_ready_g(IR_STRUCT *ir_par, IR_STRUCT *next_ir_par, TEST_DATA_STRUCT *test_data)
{
    test_vref(ir_par->upper_limit);	/* �������׼ */
    
	updata_port(&ir_par->port);/* ���¶˿�״̬ */
    ir_into_test_relay_ready(ir_par, test_data);/* �̵������� */
    amp_relay_ctrl_on();/* ���ż̵��� */
    cs_delay_ms(10);
    
    ir_relay_ready();//�����ӿ���
    
    if(ir_par->rise_time == 0)
    {
        ir_set_da_value(ir_par, test_data);//�ͻ�׼
    }
    
//    open_sine(ir_par->output_freq);/* �����Ҳ� */
    
	test_data->err_real = 0;/* acw ��ʵ��������  */
    
    ir_test_flag.testing = 1;
    test_data->test_over = 0;
    test_data->dis_time = 0;
    test_data->test_status = ST_WAIT;
    test_data->fail_num = ST_ERR_NONE;//Ĭ�ϳ�ʼ��Ϊ�ϸ�
    
    open_test_timer();/* ����ʱ�� */
}

void ir_test_details_g(IR_STRUCT *ir_par, IR_STRUCT *next_ir_par, TEST_DATA_STRUCT *test_data)
{
	switch(test_data->gradation)
	{
		case STAGE_READY:/* �ک��׶� */
        {
            if(test_data->ready_ok == 0)
            {
                test_data->ready_ok = 1;//�������
                load_ir_data_g(ir_par, next_ir_par, test_data);
                ir_test_ready_g(ir_par, next_ir_par, test_data);
            }
            break;
        }
        case STAGE_RE_READY:
        {
            if(test_data->ready_ok == 0)
            {
                test_data->ready_ok = 1;//�������
                load_ir_data_g(ir_par, next_ir_par, test_data);
//                shift_ir_cur_gear(ir_par->gear_i);
                test_vref(ir_par->upper_limit);/* �������׼ */
                cs_delay_ms(50);
                
                test_data->test_time = ir_g_zeo_t;/* ������1�׶� */
                test_data->gradation = STAGE_CHANGE;/* �����ѹ����׶� �������Ҫ */
                open_test_timer();/* ����ʱ�� */
            }
            break;
        }
		case STAGE_RISE:/* ��һ�׶� ��ѹ���� */
            test_data->test_status = ST_VOL_RISE;
			break;
		case STAGE_CHANGE:/* �ڶ��׶� ��ѹ���� */
			test_data->test_status = ST_CHANGE;
			break;
		case STAGE_TEST:/* �����׶� ���ڲ��� */
            test_data->test_status = ST_TESTING;
			break;
		case STAGE_FALL:/* ���Ľ׶� ��ѹ�½� */
            test_data->test_status = ST_VOL_FALL;
			break;
	}
}

void run_ir_test_g(NODE_STEP *step, NODE_STEP *next_step, TEST_DATA_STRUCT *test_data)
{
    IR_STRUCT *ir_par = &step->one_step.ir;
    IR_STRUCT *next_ir_par = &next_step->one_step.ir;
    
    if(test_data->ready_ok)
    {
        ir_test_irq_g(ir_par, next_ir_par, test_data);
    }
    
    if(!test_data->cont)
    {
        ir_test_details_g(ir_par, next_ir_par, test_data);
    }
    
    if((test_data->fail_num == ST_ERR_NONE) && (!test_data->test_over))
    {
        ir_count_dis_value(ir_par, test_data);
    }
}


/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
