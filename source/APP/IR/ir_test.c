
#define _IR_TEST_GLOBALS
#include "cs99xx_relay_motion.h"
#include "cs99xx_vref.h"
#include "DAC_VREF.h"
#include "test_com.h"
#include "ir_count.h"
#include "ir_test.h"
#include "ir_relay.h"

static uint16_t ir_zeo_t;///< �ک��׶ε��ۼ�ʱ��
static uint16_t ir_one_t;///< ��һ�׶ε��ۼ�ʱ��
static uint16_t ir_two_t;///< �ڶ��׶ε��ۼ�ʱ��
static uint16_t ir_thr_t;///< �����׶ε��ۼ�ʱ��
static uint16_t ir_for_t;///< ���Ľ׶ε��ۼ�ʱ��

/**
  * @brief  ����IR����ʱ����Ϣ
  * @param  [in] ir_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
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
  * @brief  ����IR��ѹ�����ĵ�ѹ����ֵ
  * @param  [in] ir_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
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
  * @brief  ����IR��ѹ�½��ĵ�ѹ����ֵ
  * @param  [in] ir_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void ir_count_fall_vol_step_value(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
}

/**
  * @brief  ����IR�ڲ��Թ����е�ѹ�仯�Ĳ���ֵ�Ϳ�ʼֵ��Ŀ��ֵ
  * @param  [in] ir_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void ir_count_vol_ch_step(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    test_data->vol_rise_step_t = 0;
    
    switch(test_data->gradation)
    {
        case STAGE_CHARGE://���
        case STAGE_RISE://����
            ir_count_rise_vol_step_value(ir_par, test_data);
            break;
        case STAGE_FALL://�½�
            ir_count_fall_vol_step_value(ir_par, test_data);
            break;
    }
}

/**
  * @brief  IR��������DA�����ֵ
  * @param  [in] ir_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void ir_set_da_value(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    test_data->output_da =  (u16)(ir_par->testing_voltage * test_data->out_da_k + test_data->out_da_b);
    set_output_da_vref(test_data->output_da);
}

/**
  * @brief  IR����״̬��״̬���㺯��
  * @param  [in] ir_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void ir_test_irq(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
	/* ������ʱ��Ϊ0ʱ ����ʱ������� ��ǰû�з���ʧ�ܼ��� */
	if(0 == ir_par->testing_time && (!ir_test_flag.fail_cont))
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
		
		if(test_data->test_time <= ir_zeo_t)
		{
			test_data->dis_time = test_data->test_time - 0;
			test_data->gradation = STAGE_READY;
			ir_test_flag.vol_change_flag = 1;
			return;
		}
		/* ʹ��������һ�׶ε�ѹ���� */
		else if(test_data->test_time <= ir_one_t)/* ������ʱ����Ϊ0ʱ���Ͳ��ܽ���if,���Ҫ+1 */
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
				/* ���������ر� */
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
			ir_test_flag.forever_testing = 1;/* ������ʱ����� */
		}
		return;
	}
	
	/* ���Ծ��� */
	if(test_data->test_time <= ir_zeo_t)
	{
		test_data->dis_time = test_data->test_time;
		test_data->gradation = STAGE_READY;
        ir_test_flag.vol_change_flag = 1;
	}
	/* ʹ��������һ�׶ε�ѹ���� */
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
	/* ����ڶ��׶� ���� */
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
	/* �����׶� ��ѹ�½� */
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
	/* ���Ľ׶� ����ȴ� */
	else if(test_data->test_time <= ir_for_t)
	{
        /* �ظ�ѹ */
        if(ir_test_flag.testing)
        {
            ir_test_flag.testing = 0;
            ir_exit_test_relay_motion(ir_par, test_data);
        }
        
		/* ���������ر� */
		if(!ir_par->steps_cont)
		{
            ir_exit_test_relay_motion(ir_par, test_data);
            test_data->test_over = 1;
            return;
		}
        
		test_data->dis_time = test_data->test_time - ir_thr_t;
		test_data->gradation = STAGE_INTER;
	}
	/* ��ǰ�����Խ��� */
	else
	{
        /* �ظ�ѹ */
        if(ir_test_flag.testing)
        {
            ir_test_flag.testing = 0;
            ir_exit_test_relay_motion(ir_par, test_data);
        }
        
        /* ���������� */
		if(ir_par->steps_cont)
		{
            test_data->cont = 1;
            return;
        }
        
        /* �������� */
        test_data->test_over = 1;
	}
}

/**
  * @brief  IR����ǰ��׼��
  * @param  [in] ir_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void ir_test_ready(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    test_vref(ir_par->upper_limit);	/* �������׼ */
    
	updata_port(&ir_par->port);/* ���¶˿�״̬ */
    ir_into_test_relay_ready(ir_par, test_data);/* �̵������� */
    amp_relay_ctrl_on();/* ���ż̵��� */
    cs_delay_ms(10);
    
    ir_relay_ready();//�����ӿ���
    open_sine(4000);/* �����Ҳ� */
    
    if(ir_par->rise_time == 0)
    {
        ir_set_da_value(ir_par, test_data);//�ͻ�׼
    }
    
    open_test_timer();/* ����ʱ�� */
    
    test_data->test_over = 0;
    test_data->dis_time = 0;
    test_data->test_status = ST_WAIT;
    test_data->fail_num = ST_ERR_NONE;//Ĭ�ϳ�ʼ��Ϊ�ϸ�
}

/**
  * @brief  ����IR����״̬��
  * @param  [in] ir_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void ir_test_details(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
	switch(test_data->gradation)
	{
		case STAGE_READY:/* �ک��׶� */
        {
            if(test_data->ready_ok == 0)
            {
                test_data->ready_ok = 1;//���
                ir_test_flag.testing = 1;//���ڲ��Ա��
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
            ir_test_flag.fail_cont = 1;//����ʧ�ܼ�������������1
            
            /* ���ʱ�䲻Ϊ0�ʹӼ��ʱ�俪ʼ */
            if(ir_par->interval_time > 0)
            {
                test_data->ready_ok = 1;
                test_data->test_time = ir_thr_t + 1;/* ������1�׶� */
                test_data->gradation = STAGE_INTER;/* �������ȴ� */
                exit_sw();//���ж�
                open_test_timer();/* ����ʱ�� */
            }
            /* ���ʱ��Ϊ0�ͼ�����һ������ */
            else
            {
                test_data->cont = 1;
            }
            break;
        }
		case STAGE_RISE:/* ��һ�׶� ��ѹ���� */
            test_data->test_status = ST_VOL_RISE;
			break;
		case STAGE_TEST:/* �ڶ��׶� ���ڲ��� */
            test_data->test_status = ST_TESTING;
			break;
		case STAGE_FALL:/* �����׶� ��ѹ�½� */
            test_data->test_status = ST_VOL_FALL;
			break;
		case STAGE_INTER:/* ���Ľ׶� ����ȴ� */
            test_data->test_status = ST_INTER_WAIT;
			break;
	}
}
/**
  * @brief  ����IR����
  * @param  [in] step ��ǰ������������
  * @param  [in] next_step ��һ������������ ��� Ϊ��˵����ǰ���Ե������һ��
  * @param  [in] test_data ��������
  * @retval ��
  */
void run_ir_test(NODE_STEP *step, NODE_STEP *next_step, TEST_DATA_STRUCT *test_data)
{
    IR_STRUCT *ir_par = &step->one_step.ir;
    
    /* ׼��������ɺ��ڼ������״̬ */
    if(test_data->ready_ok)
    {
        ir_test_irq(ir_par, test_data);
    }
    
    /* ����״̬�� */
    ir_test_details(ir_par, test_data);
    
    /* �Զ����� */
    ir_auto_find_gear();
    
    /* ���ֱ������ٽ��в��� ���Խ������ٽ��в��� */
    if((test_data->fail_num) == ST_ERR_NONE && (!test_data->test_over))
    {
        /* ����׶β����в��� */
        if(test_data->gradation != STAGE_INTER)
        {
            ir_count_dis_value(ir_par, test_data);
        }
    }
    
    /* ���Գ����쳣��������Ծ������ */
    if(test_data->fail_num != ST_ERR_NONE)
    {
        test_data->ready_ok = 0;
    }
}

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
