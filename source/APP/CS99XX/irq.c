/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�irq.c
 * ժ  Ҫ  ���ⲿ�жϷ������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
/********************* ����ͷ�ļ� ************************/
#include    "DAC_VREF.h"
#include    "cs99xx_vref.h"
#include    "app.h"
#include 	"serve_test.h"
#include	"cs99xx_relay_motion.h"
#include 	"mb_server.h"
#include	"cs99xx_type.h"
#include    "test_com.h"
#include    "cs99xx_it.h"


#define GFI_DELAY_COUNT_MAX      10  /* GFI �ۻ����� */
#define GFI_CLEAR_COUNT_CYCLE    100 /* GFI �������ֵ���� ��λms */


/*******************************************************************************
*				�жϷ������ ���Ӻ��� ���жϳ�������������Խ��Խ��             *
*******************************************************************************/

/*
 * ��������gfi_cycle_clear_count
 * ����  ��GFI����������жϼ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gfi_cycle_clear_count(void)
{
    static uint32_t c = 0;
    
    /* 80������ 90���� ��Ϊ100 */
    if(++c > GFI_CLEAR_COUNT_CYCLE)
    {
        test_flag.gfi_delay_count = 0;
        c = 0;
    }
}
/*
 * ��������stop_irq
 * ����  ����λ�жϷ������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void stop_irq(void)
{
	irq_stop_relay_motion();/* �رյ��ӿ��� ��ѹ */
    
	STOP = 1;	/* ��λ���Ա�־ */
    FAIL = 0;/* ���¸�λ������ղ���ʧ�ܱ�� */
    TERMINATE = 1;/* ��ֹ���Ա����λ */
	cur_status = ST_STOP;
//    CYLINDER_CTRL(1000);
    
    clear_keyboard();/* ��ռ��� */
    STOP_INT(DISABLE);/* �ر��ж� */
    BUZZER = BUZZER_OFF;/* �رշ��� */
    LED_FAIL = LED_OFF;/* �ر�LED_FAIL�� */
}
/*
 * ��������short_irq
 * ����  ����·�жϷ������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void short_irq(void)
{
	ERR_NUM = ERR_SHORT;
    
    disable_sample_task();/* �رղ������� *//* ֹͣˢ����Ļ���� ���ֳ���ʱ��״̬ */
    
    /* ��ѹ����ʱ��·����������ʾ����ֵ */
    if(cur_mode == ACW || cur_mode == DCW)
    {
        cur_ave = cur_high;
    }
    
    record_exception_scene();/* ��¼�쳣�ֳ� */
	irq_stop_relay_motion();
	SHORT_INT(DISABLE);
}
/*
 * ��������arc_irq
 * ����  ��arc�жϷ������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void arc_irq(void)
{
	ERR_NUM = ERR_ARC;
    
    disable_sample_task();/* �رղ������� */
    record_exception_scene();/* ��¼�쳣�ֳ� */
	irq_stop_relay_motion();
	ARC_INT(DISABLE);
}
/*
 * ��������gfi_irq
 * ����  ��gfi�жϷ������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gfi_irq(void)
{
    if(++test_flag.gfi_delay_count < GFI_DELAY_COUNT_MAX)
    {
        return;
    }
    
	ERR_NUM = ERR_GFI;
    disable_sample_task();/* �رղ������� */
    record_exception_scene();/* ��¼�쳣�ֳ� */
	irq_stop_relay_motion();
	GFI_INT(DISABLE);
}

/*
 * ��������offset_res
 * ����  ��offset����ƫ�Ʋ��Խ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void offset_res_f(const int8_t res)
{
	cur_offset_result = res;
	switch(cur_mode)
	{
		case ACW:
			g_cur_step->one_step.acw.offset_result = res;
            g_cur_step->one_step.acw.offset_cur = cur_ave + ROUND;
            g_cur_step->one_step.acw.offset_real = real_ave + ROUND;
			break;
		case DCW:
			g_cur_step->one_step.dcw.offset_result = res;
            g_cur_step->one_step.dcw.offset_cur = cur_ave + ROUND;
			break;
		case GR:
			g_cur_step->one_step.gr.offset_result = res;
            g_cur_step->one_step.gr.offset_res = res_ave + ROUND;/* ����ʱ�ĵ�������� */
			break;
        case BBD:
			g_cur_step->one_step.bbd.offset_result = res;
            if(cap_ave > 0)
            {
                g_cur_step->one_step.bbd.offset_cap = cap_ave + ROUND;/* ����ʱ�ĵ�������� �����*/
            }
            break;
	}
}

void serve_test_over(void)
{
    HIGH = 0;
    close_test_timer();
    irq_stop_relay_motion();//�رո�ѹ 2017.5.24
    
    /* ���fail_mode��ΪFAIL_MODE_FPDFC �����ǵ�һ�� ���������ͽ����������� */
    if(sys_par.fail_mode == FAIL_MODE_FPDFC && cur_step == 1 && !CUR_FAIL)
    {
        CUR_OVER = 1;
        OVER = 1;/* ���Բ�ȫ����� */
    }
    /* ������һ�� */
    else
    {
        if(steps_con)
        {
            CUR_OVER = 1;
            CONT = 1;
            
            if(cur_step == g_cur_file->total)
            {
                if(FAIL)
                {
                    OVER = 1;/* ���Բ�ȫ����� */
                    CONT = 0;
                }
            }
            return;
        }
        /* ֹͣ���� */
        else
        {
            CUR_OVER = 1;
            
            /* ��ǰ�������һ�� */
            if(cur_step == g_cur_file->total)
            {
                OVER = 1;/* ���Բ�ȫ����� */
            }
        }
    }
}

void sever_offset_test_over(void)
{
    /* ����ģʽ */
    if(app_flag.offset_mode == OFFSET_CON)
    {
        if(g_cur_step->one_step.com.step < g_cur_file->total)
        {
            close_test_timer();/* �ض�ʱ�� */
            stop_test();/* ֹͣ���� */
            g_test_time = 0;/* ������1�׶� */
            test_flag.gradation = STAGE_READY;/* �������Ҫ */
            OFFSET_NEXT = 1;/* �����һ���� */
            offset_res_f(OFFSET_PASS);/* ��ǵ�ǰ�����Ժϸ� */
            return;
        }
    }
    
    stop_test();/* ֹͣ���� */
    offset_res_f(OFFSET_PASS);
    PASS = 1;
}

void report_absorptance_res_value(void)
{
	if(g_dis_time < 150)
	{
		ir_15s_res = 0;
		ir_60s_res = 0;
		ir_absorptance = 0;
		return;
	}
	
	if(g_dis_time == 150)
	{
		ir_15s_res = res_ave;/* IR���� 15sʱ�ĵ���ֵ */
	}
	else if(g_dis_time == 600)
	{
		ir_60s_res = res_ave;/* IR���� 60sʱ�ĵ���ֵ */
	}
	
	if(g_dis_time >= 600)
	{
		if(ir_15s_res > 0 && ir_60s_res > 0)
		{
			ir_absorptance = ir_60s_res / ir_15s_res;
		}
	}
}

/*
 * ��������test_irq
 * ����  ������ʱ�ӿ��ƣ��������������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void test_irq(void)
{
	/* ������ʱ��Ϊ0ʱ ����ʱ������� */
	if(0 == tes_t)
	{
		/* �����ǰ�������ڽ��� �����ڵڶ��׶εĻ���һֱ���� */
		if(TESTING)
		{
			test_flag.gradation = STAGE_TEST;
			g_dis_time = g_test_time % 10000;
            if(g_dis_time == 0)
            {
                g_test_time = 0;
            }
			return;
		}
		
		if(g_test_time <= zeo_t)
		{
			g_dis_time = g_test_time - 0;
			test_flag.gradation = STAGE_READY;
			test_flag.dis_status = TEST_WAIT_INDEX;
			test_flag.vol_change_flag = 1;
			return;
		}
		/* ʹ��������һ�׶ε�ѹ���� */
		else if(g_test_time <= one_t)/* ������ʱ����Ϊ0ʱ���Ͳ��ܽ���if,���Ҫ+1 */
		{
            count_rise_vol_step_value();/* �����ѹ��������ֵ */
			g_dis_time = g_test_time - zeo_t;
			test_flag.gradation = STAGE_RISE;
			test_flag.dis_status = TEST_RISE_INDEX;
			test_flag.vol_change_flag = 1;
            
            if(g_dis_time == 1)
            {
                count_vol_ch_step();
            }
			return;
		}
		
		report_absorptance_res_value();
		
        if(g_test_time == one_t + 1)
        {
            set_da_value();
        }
        
		if(CUR_FAIL)
		{
			if(g_test_time < for_t)
			{
				/* ���������ر� */
				if(!steps_con)
				{
                    serve_test_over();
                    return;
				}
				g_dis_time = g_test_time - thr_t + 1;
				test_flag.gradation = STAGE_INTER;
				test_flag.dis_status = TEST_INTER_INDEX;
				return;
			}
			else
			{
                serve_test_over();
                return;
			}
		}
		
		test_flag.vol_change_flag = 0;
		test_flag.gradation = STAGE_TEST;
		test_flag.dis_status = TEST_TEST_INDEX;
		g_dis_time = g_test_time - one_t;
		
		if(g_dis_time >= 9999)
		{
            g_test_time = 0;
			TESTING = 1;/* ������ʱ����� */
		}
		return;
	}
	
	/* ���Ծ��� */
	if(g_test_time <= zeo_t)
	{
		g_dis_time = g_test_time;
		test_flag.gradation = STAGE_READY;
		test_flag.dis_status = TEST_WAIT_INDEX;
        test_flag.vol_change_flag = 1;
	}
	/* ʹ��������һ�׶ε�ѹ���� */
	else if(g_test_time <= one_t)
	{
		HIGH = 1;
		g_dis_time = g_test_time - zeo_t;
		test_flag.gradation = STAGE_RISE;
		test_flag.dis_status = TEST_RISE_INDEX;
		test_flag.vol_change_flag = 1;
        
        if(g_dis_time == 1)
        {
            count_vol_ch_step();
        }
	}
	/* ����ڶ��׶� ���� */
	else if(g_test_time <= two_t)
	{
		HIGH = 1;
		g_dis_time = g_test_time - one_t;
		test_flag.gradation = STAGE_TEST;
		test_flag.dis_status = TEST_TEST_INDEX;
		test_flag.vol_change_flag = 0;
        
		report_absorptance_res_value();
		
        if(g_test_time == one_t + 1)
        {
            set_da_value();
        }
	}
	/* �����׶� ��ѹ�½� */
	else if(g_test_time <= thr_t)
	{
		HIGH = 1;
		test_flag.vol_change_flag = 1;
		test_flag.dis_status = TEST_FALL_INDEX;
		g_dis_time = g_test_time - two_t;
		test_flag.gradation = STAGE_FALL;
        
        if(g_dis_time == 1)
        {
            count_vol_ch_step();
        }
	}
	/* ���Ľ׶� ����ȴ� */
	else if(g_test_time <= for_t)
	{
		HIGH = 0;
		
		if(sys_par.fail_mode == FAIL_MODE_FPDFC && cur_step == 1)
		{
			if(!CUR_FAIL)
			{
				serve_test_over();
				return;
			}
		}
		/* ���������ر� */
		if(!steps_con || (FAIL && cur_step == g_cur_file->total))
		{
            serve_test_over();
            return;
		}
        
		g_dis_time = g_test_time - thr_t;
		test_flag.gradation = STAGE_INTER;
		test_flag.dis_status = TEST_INTER_INDEX;
	}
	/* ��ǰ�����Խ��� */
	else
	{
		/* �������� */
		if(!OFFSET_BBD)
		{
            serve_test_over();
        }
        else
        {
            sever_offset_test_over();
        }
	}
}

/*
 * ��������test_big_cap_irq
 * ����  ������ʱ�ӿ��ƣ��������������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void test_big_cap_irq(void)
{
    static uint16_t dis_time_bak;
    
	/* ������ʱ��Ϊ0ʱ ����ʱ������� */
	if(0 == tes_t)
	{
        /* ���ڷŵ� */
        if(TERMINATE)
        {
            test_flag.gradation = STAGE_DIS_CHA;
            g_dis_time = g_test_time - for_t;
            test_flag.dis_status = TEST_DISCHARGE_INDEX;
            test_flag.vol_change_flag = 1;
            
            if(g_dis_time == 1)
            {
                amp_relay_ctrl_off();
                on_sample_task();/* ������������ */
            }
            return;
        }
        
		/* �����ǰ�������ڽ��� �����ڵڶ��׶εĻ���һֱ���� */
		if(TESTING)
		{
			test_flag.gradation = STAGE_TEST;
			g_dis_time = g_test_time % 10000;
            if(g_dis_time == 0)
            {
                g_test_time = 0;
            }
			return;
		}
		
		if(g_test_time <= zeo_t)
		{
			g_dis_time = g_test_time - 0;
			test_flag.gradation = STAGE_READY;
			test_flag.dis_status = TEST_WAIT_INDEX;
			return;
		}
		/* ʹ��������һ�׶����ڳ�� */
		else if(g_test_time <= one_t)/* ������ʱ����Ϊ0ʱ���Ͳ��ܽ���if,���Ҫ+1 */
		{
			g_dis_time = g_test_time - zeo_t;
			test_flag.gradation = STAGE_CHARGE;
			test_flag.dis_status = TEST_RISE_INDEX;
			test_flag.vol_change_flag = 1;
            
            if(g_dis_time == 0)
            {
                count_vol_ch_step();
            }
			return;
		}
		
        if(g_test_time == one_t + 1)
        {
            set_da_value();
        }
        
		if(CUR_FAIL)
		{
			if(g_test_time < for_t)
			{
				/* ���������ر� */
				if(!steps_con)
				{
					goto l;/* �������� */
				}
				g_dis_time = g_test_time - thr_t + 1;
				test_flag.gradation = STAGE_INTER;
				test_flag.dis_status = TEST_INTER_INDEX;
				return;
			}
			else
			{
				goto l;/* �������� */
			}
		}
		
		test_flag.vol_change_flag = 0;
		test_flag.gradation = STAGE_TEST;
		test_flag.dis_status = TEST_TEST_INDEX;
		g_dis_time = g_test_time - one_t;
		
		if(g_dis_time >= 9999)
		{
            g_test_time = 0;
			TESTING = 1;/* ������ʱ����� */
		}
        
		return;
	}
	
	/* ���Ծ��� */
	if(g_test_time <= zeo_t)
	{
		g_dis_time = g_test_time;
		test_flag.gradation = STAGE_READY;
		test_flag.dis_status = TEST_WAIT_INDEX;
	}
	/* ʹ��������һ�׶����ڳ�� */
	else if(g_test_time <= one_t)
	{
		HIGH = 1;
		g_dis_time = g_test_time - zeo_t;
		test_flag.gradation = STAGE_CHARGE;
		test_flag.dis_status = TEST_CHARGE_INDEX;
		test_flag.vol_change_flag = 1;
        
        if(g_dis_time == 1)
        {
            count_vol_ch_step();
        }
	}
    /* ����ڶ��׶� ��ѹ */
	else if(g_test_time <= two_t)
	{
		HIGH = 1;
		g_dis_time = g_test_time - one_t;
		test_flag.gradation = STAGE_STAB;
		test_flag.dis_status = TEST_STAB_INDEX;
		test_flag.vol_change_flag = 1;
        
        if(g_test_time == one_t + 1)
        {
            set_da_value();
        }
	}
	/* ��������׶� ���� */
	else if(g_test_time <= thr_t)
	{
		HIGH = 1;
		g_dis_time = g_test_time - two_t;
		test_flag.gradation = STAGE_TEST;
		test_flag.dis_status = TEST_TEST_INDEX;
        
        if(g_dis_time > 10)
        {
            test_flag.vol_change_flag = 0;
        }
		
        if(g_test_time == two_t + 1)
        {
            set_da_value();
        }
	}
	/* ���Ľ׶� ��ѹ�½� */
	else if(g_test_time <= for_t)
	{
		HIGH = 1;
		test_flag.vol_change_flag = 1;
		g_dis_time = g_test_time - thr_t;
		test_flag.dis_status = TEST_FALL_INDEX;
		test_flag.gradation = STAGE_FALL;
        
        if(g_dis_time == 0)
        {
            count_vol_ch_step();
        }
	}
    /* ����׶� ���ڷŵ� */
    else if(0 == test_flag.dis_charge_end)
    {
        test_flag.gradation = STAGE_DIS_CHA;
        g_dis_time = g_test_time - for_t;
        dis_time_bak = g_dis_time;
        test_flag.dis_status = TEST_DISCHARGE_INDEX;
        test_flag.vol_change_flag = 1;
        
        if(g_dis_time == 1)
        {
            amp_relay_ctrl_off();
            
            on_sample_task();
        }
    }
	/* �����׶� ����ȴ� */
	else if(g_test_time <= fiv_t + dis_time_bak)
	{
		HIGH = 0;
        
        if(TERMINATE)
        {
            goto l;/* �������� */
        }
        
		g_dis_time = g_test_time - thr_t - dis_time_bak;
		test_flag.gradation = STAGE_INTER;
		test_flag.dis_status = TEST_INTER_INDEX;
        
		/* ���������ر� */
		if(!steps_con)
		{
			goto l;/* �������� */
		}
	}
	/* ��ǰ�����Խ��� */
	else
	{
		HIGH = 0;
        close_test_timer();
        
		/* ������һ�� */
		if(steps_con)
		{
            CUR_OVER = 1;
			CONT = 1;
			return;
		}
        /* ֹͣ���� */
		else
		{
	l:
            close_test_timer();/* �ض�ʱ�� */
			irq_stop_relay_motion();
            test_flag.dis_status = TEST_DISCHARGE_OVER_INDEX;
            
            CUR_OVER = 1;
            
			/* ��ǰ�������һ�� */
			if(g_cur_step == list_99xx.tail)
			{
				OVER = 1;/* ���Բ�ȫ����� */
			}
		}
	}
}

/*
 * �������� test_g_irq
 * ����  ��Gģʽ����ʱ�ӿ��ƣ��������������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void test_g_irq(void)
{
	/* ������ʱ��Ϊ0ʱ ����ʱ������� */
	if(0 == tes_t)
	{
		/* �����ǰ�������ڽ��� �����ڵڶ��׶εĻ���һֱ���� */
		if(TESTING)
		{
			test_flag.gradation = STAGE_TEST;
			g_dis_time = g_test_time % 10000;
            if(g_dis_time == 0)
            {
                g_test_time = 0;
            }
			test_flag.vol_change_flag = 0;
			return;
		}
		
		if(g_test_time <= zeo_t)
		{
			g_dis_time = g_test_time - 0;
			test_flag.gradation = STAGE_READY;
			test_flag.dis_status = TEST_WAIT_INDEX;
            test_flag.vol_change_flag = 1;
			return;
		}
		/* ʹ��������һ�׶ε�ѹ���� */
		else if(g_test_time <= one_t)
		{
			g_dis_time = g_test_time - zeo_t;
			test_flag.gradation = STAGE_RISE;
			test_flag.dis_status = TEST_RISE_INDEX;
			test_flag.vol_change_flag = 1;
            
            if(g_dis_time == 1)
            {
                count_vol_ch_step();
            }
			return;
		}
		
        if(g_test_time == one_t + 1)
        {
            set_da_value();
        }
        
		test_flag.gradation = STAGE_TEST;
		test_flag.dis_status = TEST_TEST_INDEX;
		test_flag.vol_change_flag = 0;
		g_dis_time = g_test_time - one_t;
		
		if(g_dis_time >= 9999)
		{
            g_test_time = 0;
			TESTING = 1;/* ������ʱ������ */
		}
		return;
	}
	
	/* �����0�׶� ����׼�� */
	if(g_test_time <= zeo_t)
	{
		g_dis_time = g_test_time;
		test_flag.gradation = STAGE_READY;
        test_flag.dis_status = TEST_WAIT_INDEX;
		test_flag.vol_change_flag = 1;
	}
	/* �����1�׶� ��ѹ���� */
	else if(g_test_time <= one_t)
	{
		g_dis_time = g_test_time - zeo_t;
		test_flag.gradation = STAGE_RISE;
		test_flag.dis_status = TEST_RISE_INDEX;
		test_flag.vol_change_flag = 1;
        
        if(g_dis_time == 1)
        {
            count_vol_ch_step();
        }
	}
	/* �����2�׶� ��ѹ���� */
	else if(g_test_time <= two_t)
	{
		g_dis_time = g_test_time - one_t;
		test_flag.gradation = STAGE_CHANGE;
		test_flag.dis_status = TEST_CHANGE_INDEX;
		test_flag.vol_change_flag = 1;
	}
	/* �����3�׶� ���� */
	else if(g_test_time <= thr_t)
	{
		g_dis_time = g_test_time - two_t;
		test_flag.gradation = STAGE_TEST;
		test_flag.dis_status = TEST_TEST_INDEX;
		
        if(g_dis_time > 10)
        {
            test_flag.vol_change_flag = 0;
        }
        
        if(g_test_time == one_t + 1)
        {
            set_da_value();
        }
	}
	/* �����4�׶� ��ѹ�½�*/
	else if(g_test_time <= for_t)
	{
		test_flag.vol_change_flag = 0;
		g_dis_time = g_test_time - thr_t;
		test_flag.dis_status = TEST_FALL_INDEX;
		test_flag.gradation = STAGE_FALL;
		test_flag.vol_change_flag = 1;
        
        if(g_dis_time == 1)
        {
            count_vol_ch_step();
        }
	}
	else
	{
		/* �������� */
		if(!OFFSET_BBD)
		{
            close_test_timer();
            
            if(g_cur_step->next != NULL)
            {
                CONT = 1;
                return;
            }
            
            irq_stop_relay_motion();
            OVER = 1;
        }
		/* ƫ�Ʋ��� */
        else
        {
            if(app_flag.offset_mode == OFFSET_CON)
            {
                if(g_cur_step->next != NULL)
                {
                    close_test_timer();/* �ض�ʱ�� */
                    stop_test();/* ֹͣ���� */
                    test_pass();/* ������ */
                    offset_res_f(OFFSET_PASS);
                    OFFSET_NEXT = 1;/* �����һ���� */
                    return;
                }
            }
            
            stop_test();/* ֹͣ���� */
            test_pass();
            offset_res_f(OFFSET_PASS);
        }
        
		PASS = 1;/* �����ٽ��в���ʱ�ͽ�pass��־д1 */
	}
}
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
