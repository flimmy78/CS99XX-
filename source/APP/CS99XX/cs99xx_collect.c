/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_collect.c
 * ժ  Ҫ  ��Ϊͨ�ų����ṩ������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#include    <stdio.h>
#include    "cs99xx_collect.h"
#include	"SPI_CPLD.h"
#include    "cs99xx_result.h"
#include	"calibration.h"
#include    "stm32f10x.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include    "cs99xx_vref.h"
#include 	"DAC_VREF.h"
#include    "ADC_DMA.h"
#include 	"STM32_ADC.H"
#include    "dc_module.h"
#include	"string.h"
#include    "test_com.h"

/*
 * ��������smoothing_filtering
 * ����  ��dacƽ���˲�
 * ����  ��sf_n ����vol cur, 0 vol�� 1 cur
 * ���  ����
 * ����  ���˲��Ľ��
 */

uint32_t smoothing_filtering(const u8 sf_n,const u32 val)
{
	int i = 0, j = 0;
	
	for(i = SF_DEPTH-1; i > 0; i--)
	{
		f_buf.sf_buf_u[sf_n][i] = f_buf.sf_buf_u[sf_n][i-1];
	}
    
	f_buf.sf_buf_u[sf_n][0] = val;
	
	for(i = 0; i < SF_DEPTH-1; i++)
	{
		for(j = i+1; j < SF_DEPTH; j++)
		{
			if(f_buf.sf_buf_u[sf_n][i] == f_buf.sf_buf_u[sf_n][j])
			{
				return f_buf.sf_buf_u[sf_n][i];
			}
		}
	}
	
	return val;
}

void set_high_err(void)
{
    test_flag.high_err = 1;
}

void reset_high_err(void)
{
    test_flag.high_err = 0;
}

uint8_t get_high_err(void)
{
    return test_flag.high_err;
}

#define DEEP_SLOW		120
#define DEEP_FAST		5
#define DEEP_MID		80
#define SLITHER_DEEP_	DEEP_SLOW
#define SLITHER_DEEP	 ((sys_par.ir_speed_sw == IR_FILTERING_DEEP_SLOW)? DEEP_SLOW : \
					(sys_par.ir_speed_sw == IR_FILTERING_DEEP_FAST)? DEEP_FAST : DEEP_MID)

typedef struct{
    uint32_t index;
    uint64_t sum;
    uint32_t c;
    uint16_t buf[SLITHER_DEEP_];/* �˲����� */
}SLITHER_AVE_INFO;

SLITHER_AVE_INFO slither_ave_info;

void clear_slither_data(void)
{
    memset(&slither_ave_info, 0, sizeof slither_ave_info);
}

uint16_t slither_f(uint16_t value)
{
    uint16_t res = 0;
	uint16_t deep = SLITHER_DEEP;
    
    slither_ave_info.index = (slither_ave_info.index + 1) % deep;
    
    slither_ave_info.sum -= slither_ave_info.buf[slither_ave_info.index];
    
    slither_ave_info.buf[slither_ave_info.index] = value;
    
    slither_ave_info.sum += slither_ave_info.buf[slither_ave_info.index];
    
    if(slither_ave_info.c < deep)
    {
        slither_ave_info.c++;
    }
    else
    {
        slither_ave_info.c = deep;
    }
    
    res = slither_ave_info.sum / slither_ave_info.c;
    
    return res;
}

/*
 * ��������count_dis_value
 * ����  ��������ʾֵ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void count_dis_value(void)
{
	double cos_val = 0.0;
	double sin_val = 0.0;
	uint16_t temp_cur = 0;
	uint16_t temp_vol = 0;
	float temp_res = 0.0;
    
    /* ������λ�ǵ�����ֵ������ֵ */
    if((cur_mode == ACW && cur_real_cur) || cur_mode == BBD)
    {
        cpld_count_angle(&sin_val, &cos_val);
    }
	
	if(cur_mode != GR)
	{
		temp_vol = sample_vol;
		temp_cur = sample_cur;
        cur_ad_vol = sample_cur * 3.3 / 4096;/* ���������ad�����˵ĵ�ѹֵ */
		
		if(temp_vol == 0)
		{
			vol_ave = 0;
		}
		else
		{
			vol_ave = temp_vol * cur_adc_k + cur_adc_b;
		}
		
        /* ���Խ׶� */
        if(test_flag.gradation == STAGE_TEST)
        {
            //�Ե�ѹС���趨ֵ10���ֵĵ�ѹ�������� 2017.6.28
            if(vol_ave <= cur_vol)
            {
                if(vol_ave + 10 > cur_vol)
                {
                    vol_ave = cur_vol;
                }
            }
            else
            {
                if(cur_vol + 3 > vol_ave)
                {
                    vol_ave = cur_vol;
                }
            }
        }
        
		if(cur_mode == IR)
		{
//			temp_cur = slither_f(temp_cur);
		}
		
		if(temp_cur == 0)
		{
			cur_ave = 0;
		}
		else
		{
			cur_ave = temp_cur;
		}
	}
	/* GR���� */
	else
	{
		/* ֱ�� GR */
		if(DC_GR_EN)
		{
            float t_cur = 0;
            float t_vol = 0;
            
            if(test_flag.allow_dc_gr_fetch == 0)
            {
                return;
            }
            
			if(test_flag.uart_next == 0)
			{
				test_flag.uart_next = 1;
                
                send_dc_module_cmd(DC_MODULE_CMD_FETCH, 0);/* ��ȡ�������� */
//                 send_dc_module_cmd(DC_MODULE_CMD_FETCH_SHORT, 0);/* ��ȡ�������� */
			}
            
            if(0 == get_dc_gr_data(&t_cur, &t_vol))
//             if(0 == get_dc_gr_short_data(&t_cur, &t_vol))
            {
                test_flag.uart_next = 0;
                cur_ave = t_cur;
                vol_ave = t_vol;
            }
		}
		/* ���� GR */
		else
		{
			temp_cur = sample_cur;/* �ɼ��ĵ���ֵ */
			temp_vol = sample_vol;/* �ɼ���ѹֵ */
            
            if(temp_cur == 0)
            {
                cur_ave = 0;
            }
            else
            {
                cur_ave = temp_cur * ratio_gr.adc_i_k[cur_gear] + ratio_gr.adc_i_b[cur_gear] + ROUND / 100;
            }
            
            if(temp_vol == 0)
            {
                vol_ave = 0;
            }
            else
            {
                vol_ave = temp_vol * ratio_gr.adc_v_k[cur_gear] + ratio_gr.adc_v_b[cur_gear] + ROUND / 100;
            }
		}
        
        //�Ե�ѹС���趨ֵ10���ֵĵ�ѹ�������� 2017.6.28
        if((cur_ave * 100) <= cur_vol)
        {
            if((cur_ave * 100) + 30 > cur_vol)
            {
                cur_ave = cur_vol / 100.0;
            }
        }
        else
        {
            if(cur_vol + 3 > (cur_ave * 100))
            {
                cur_ave = cur_vol / 100.0;
            }
        }
	}
    
    if(cur_ave < 0)
    {
        cur_ave = 0;
    }
    
    if(vol_ave < 0)
    {
        vol_ave = 0;
    }
	
	switch(cur_mode)
	{
		case ACW:
		{
			cur_ave = temp_cur * ratio_acw.adc_cur_k[cur_gear];
			real_ave = cur_ave * cos_val;
			
            /* �Ե�����ʽ��ʾ��ʵ���� */
            if(type_spe.real_cur_format == CAP_FORMAT)
            {
                if(vol_ave <= 0)
                {
                    cap_ave = 0;
                }
                else
                {
                    /* nA/(v*2*PI*Hz) = nF */
                    switch(cur_gear)
                    {
                        case AC_200uA:
                            cap_ave = cur_ave * sin_val / 10000 * 1000 * 1000 / (vol_ave * 2 * PI * cur_frequency);
                            break;
                        case AC_2mA:
                            cap_ave = cur_ave * sin_val / 1000 * 1000 * 1000 / (vol_ave * 2 * PI * cur_frequency);
                            break;
                        case AC_10mA:
                        case AC_20mA:
                        case AC_50mA:
                            cap_ave = cur_ave * sin_val / 100 * 1000 * 1000 / (vol_ave * 2 * PI * cur_frequency);
                            break;
                        case AC_100mA:
                            cap_ave = cur_ave * sin_val / 10 * 1000 * 1000 / (vol_ave * 2 * PI * cur_frequency);
                            break;
                    }
                }
            }
			
			if(!OFFSET_BBD)
			{
				/* ����ƫ��ֵ */
				if(cur_ave >= cur_offset)
				{
					cur_ave -= cur_offset;
				}
				else
				{
					cur_ave = 0;
				}
				
				if(real_ave >= cur_real_offset)
				{
					real_ave -= cur_real_offset;
				}
				else
				{
					real_ave = 0;
				}
			}
			
			/* �ж���ʵ���� */
			if(real_ave > cur_real_cur && cur_real_cur)
			{
				err_real = 1;
			}
			break;
		}
		case DCW:
		{
			cur_ave = temp_cur * ratio_dcw.adc_cur_k[cur_gear];
			
			if(!OFFSET_BBD)
			{
				/* ����ƫ��ֵ */
				if(cur_ave >= cur_offset)
				{
					cur_ave -= cur_offset;
				}
				else
				{
					cur_ave = 0;
				}
			}
			
			/* �жϳ����� */
			if(cur_ave > cur_charge_cur)
			{
				err_charge = 0;/* ������ */
			}
			break;
		}
		case IR:
		{
			/* �������Ϊ0ʱ����������1 */
			if(cur_ave <= 0)
			{
				cur_ave = 1;
				res_ave = 1000*1000;
				res_ave_comm = ~(uint16_t)0;
				break;
			}
			
            if(g_ir_dly > 5)
            {
                return;
            }
            
			temp_res = (float)vol_ave * cur_res_k / cur_ave;
			
            res_ave = temp_res;
			
			if(res_ave < 0)
			{
				res_ave = 0;
			}
			else
			{
				uint32_t temp = 0;
				temp = res_ave * 1000;
				res_ave = (float)temp / 1000.0; /* ����3λС�� */
			}
			break;
		}
		case GR:/* mOhm = mV/mA*1000 */
		{
			/* ������ʾ����λС���㣬����10�ͽ���ת��ΪmA */
// 			if(cur_ave*10 > cur_vol * 0.08)
            if(cur_ave)
			{
				res_ave = vol_ave * 10 / cur_ave;
			}
			
			if(!OFFSET_BBD)
			{
				/* ����ƫ��ֵ */
				if(res_ave >= cur_offset)
				{
					res_ave -= cur_offset;
				}
				else
				{
					res_ave = 0;
				}
			}
			
			cur_ave_comm = cur_ave * 100;
			res_ave_comm = res_ave;
			break;
		}
		case BBD:
		{
			cur_ave = temp_cur * cur_adc_cur_k;
			
            if(vol_ave <= 0)
            {
                cap_ave = 0;
            }
            else
            {
                /* nA/(v*2*PI*Hz) = nF */
                switch(cur_gear)
                {
                    case AC_200uA:
                        cap_ave = cur_ave * sin_val / 10000 * 1000 * 1000 / (vol_ave * 2 * PI * cur_frequency);
                        break;
                    case AC_2mA:
                        cap_ave = cur_ave * sin_val / 1000 * 1000 * 1000 / (vol_ave * 2 * PI * cur_frequency);
                        break;
                    case AC_20mA:
                        cap_ave = cur_ave * sin_val / 100  * 1000 * 1000 / (vol_ave * 2 * PI * cur_frequency);
                        break;
                }
            }
			
// 			real_ave = cur_ave*cos_val;
// 			T_DELAY();
// 			GUI_GotoXY(X+11*CW, Y);
// 			GUI_DispFloatFix ((float)temp_cap, 5, 3);
			
			if(!OFFSET_BBD)
			{
				/* ����ƫ��ֵ */
				if(cap_ave >= g_cur_step->one_step.bbd.offset_cap)
				{
// 					cap_ave -= g_cur_step->one_step.bbd.offset_cap;
				}
				else
				{
// 					cap_ave = 0;
				}
			}
			
// 			g_cur_step->one_step.bbd.cap_value = cap_ave;
			break;
		}
		case CC:
// 			cur_ave = temp_cur * 1;
			break;
	}
    
    
    if(cur_mode == ACW || cur_mode == DCW)
    {
        /* ����ݲ���ֻ�ڲ��Խ׶����ж����ޱ���*/
        if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW)
        {
            if(test_flag.gradation == STAGE_TEST)
            {
                /* ������ֵ��������ֵʱ�ͽ�����ֵǿ�и�Ϊ����ֵ���������ޱ��� */
                if(cur_ave > cur_high)
                {
                    cur_ave = cur_high;
                    
                    set_high_err();
                }
                else
                {
                    reset_high_err();
                }
            }
        }
        /* �������� */
        else
        {
            /* ������ֵ��������ֵʱ�ͽ�����ֵǿ�и�Ϊ����ֵ���������ޱ��� */
            if(cur_ave > cur_high && app_flag.calibration == 0)
            {
				uint8_t buf[14]  ={1,1,1,1,2,2,3,3,4,4,1,1,2,2};
				uint8_t index[18]={0,1,2,16,17,3,9,10,4,14,15,5,6,7,8,11,12,13};
				static uint8_t c;
				
				sample_cur = g_ad_dog + buf[index[c++%18] % 14];
				
				cur_ave = sample_cur * cur_adc_cur_k;
                
				irq_stop_relay_motion();
				record_exception_scene();/* ��¼�쳣�ֳ� */
                set_high_err();
				test_flag.judge_err_en == DISABLE;
				ERR_NUM = ERR_HIGH;
            }
            else
            {
                reset_high_err();
            }
        }
    }
    
    /* �쳣�ж� */
    if(OFFSET_BBD != 2)
    if(test_flag.judge_err_en == ENABLE)
    {
        if(ERR_NUM == ERR_NONE)
        {
            ERR_NUM = judge_err();
        }
    }
}

/*
 * ��������judge_vol_exception
 * ����  ���жϵ�ѹ�쳣
 * ����  ����
 * ���  ����
 * ����  �������� cur_status
 */
uint8_t judge_vol_exception(void)
{
	/* ��ѹ���� �׶� ����׶� */
	if(test_flag.gradation == STAGE_RISE || test_flag.gradation == STAGE_CHANGE)
    {
        if(vol_ave > (cur_vol * 11 / 10 + 10))
        {
            if(++test_flag.vol_err_count > 2)
            {
                record_exception_scene();/* ��¼�쳣�ֳ� */
                irq_stop_relay_motion();
                return ERR_VOL;
            }
        }
        else
        {
            test_flag.vol_err_count = 0;
            return ERR_NONE;/* ����ʱ�䵽���� */
        }
    }
    else if(test_flag.gradation == STAGE_TEST)
    {
        if(g_dis_time >= 5 || g_dis_time == tes_t)
        {
            /* ��ѹ����1V */
            if(vol_ave < 1)
            {
                record_exception_scene();/* ��¼�쳣�ֳ� */
                irq_stop_relay_motion();
                return ERR_SHORT;
            }
        }
        
        /* ����ʱ�䵽 */
        if(tes_t && g_dis_time + 1 >= tes_t)
        {
            return ERR_NONE;
        }
        
        if((vol_ave < cur_vol / 2 || vol_ave > (cur_vol * 11 / 10 + 10)))
        {
//            if(++test_flag.vol_err_count > 2)
            {
                record_exception_scene();/* ��¼�쳣�ֳ� */
                irq_stop_relay_motion();
                return ERR_VOL;
            }
        }
        else
        {
            test_flag.vol_err_count = 0;
            return ERR_NONE;/* ����ʱ�䵽���� */
        }
    }
    
    return ERR_NONE;
}
/*
 * ��������set_dc_gr_open_err
 * ����  �����ÿ�·����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void set_dc_gr_open_err(void)
{
    test_flag.dc_gr_open_err = ERR_OPEN;
}

/*
 * ��������get_dc_gr_open_err
 * ����  ����ȡֱ��ģ�鿪·����
 * ����  ����
 * ���  ����
 * ����  ��������
 */
uint8_t get_dc_gr_open_err(void)
{
    return test_flag.dc_gr_open_err;
}
/*
 * ��������set_dc_gr_amp_err
 * ����  ������ֱ��ģ�鹦�ű���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void set_dc_gr_amp_err(void)
{
    test_flag.dc_gr_amp_err = ERR_AMP;
}

/*
 * ��������get_dc_gr_open_err
 * ����  ����ȡֱ��ģ�鹦�ű���
 * ����  ����
 * ���  ����
 * ����  ��������
 */
uint8_t get_dc_gr_amp_err(void)
{
    return test_flag.dc_gr_amp_err;
}
/*
 * ��������judge_err
 * ����  �������ж�
 * ����  ����
 * ���  ����
 * ����  ����
 */
int8_t judge_err(void)
{
	#define JUDGE_SHORT() {if(SHORT_PIN == RESET){if(SHORT_PIN == RESET){if(SHORT_PIN == RESET){err = ERR_SHORT;}}}}
	int8_t err = 0;
	
    /* �ŵ�ʱ�������쳣�ж� */
    if(cur_mode == DCW)
    {
        if(test_flag.gradation == STAGE_DIS_CHA)
        {
            return err;
        }
    }
    
    if(cur_mode == CC)
    {
        /* �������ޱ��� */
        if(cur_ave < cur_low)
        {
            record_exception_scene();/* ��¼�쳣�ֳ� */
            ERR_NUM_OVER = ERR_OPEN;
        }
        else
        {
            ERR_NUM_OVER = ERR_NONE;
        }
        return err;
    }
    
    /* ��ѹ�׶� */
    if(test_flag.gradation == STAGE_STAB)
    {
        switch(cur_mode)
        {
            case DCW:
            {
				if(0 == cur_delay_t)
				{
					/* �������ޱ��� */
                    if(get_high_err())
					{
                        record_exception_scene();/* ��¼�쳣�ֳ� */
						err = ERR_HIGH;
						result_dcw();
					}
				}
                break;
            }
        }
    }
    /* ����ݲ��Գ��׶� */
	else if(test_flag.gradation == STAGE_CHARGE)
    {
        if(cur_ave > get_cur_upper(DCW, cur_gear))
        {
            record_exception_scene();/* ��¼�쳣�ֳ� */
            err = ERR_GEAR;
            result_dcw();
        }
    }
	/* ��ѹ���� �׶� ����׶� */
	else if(test_flag.gradation == STAGE_RISE || test_flag.gradation == STAGE_CHANGE)
	{
		switch(cur_mode)
		{
			case ACW:
            {
                err = judge_vol_exception();
                
                if(ERR_NONE != err)
                {
                    return err;
                }
				/* �������ޱ��� */
// 				if(cur_ave > cur_high)
                if(get_high_err())
				{
                    record_exception_scene();/* ��¼�쳣�ֳ� */
					err = ERR_HIGH;
					result_acw();
				}
				break;
            }
			case DCW:
            {
                /* ��ʱʱ��Ϊ0ʱ�ж����ޱ��� */
                if(0 == cur_delay_t)
				{
                    err = judge_vol_exception();
                    
                    if(ERR_NONE != err)
                    {
                        return err;
                    }
                    
					/* �������ޱ��� */
                    if(get_high_err())
					{
                        record_exception_scene();/* ��¼�쳣�ֳ� */
						err = ERR_HIGH;
						result_dcw();
					}
				}
                /* ��ʱʱ��>0ʱ�ڵ�ѹ�����׶�Ҫ�жϵ�λ�������ֲ����ж����ޱ��� */
				else
				{
                    err = judge_vol_exception();
                    
                    if(ERR_NONE != err)
                    {
                        return err;
                    }
                    
                    if(get_high_err())
                    {
                        reset_high_err();
                    }
                    
                    if(ERR_NONE == err)
                    {
                        /* ��λ���� ���Ĵ�����Ϊ���ֲ���ʱʱ���޷��������ޱ��� */
                        if(cur_ave > get_cur_upper(DCW, cur_gear))
                        {
                            record_exception_scene();/* ��¼�쳣�ֳ� */
                            err = ERR_GEAR;
                            result_dcw();
                        }
                    }
				}
				break;
            }
			case IR:
            {
                err = judge_vol_exception();
                
                if(ERR_NONE != err)
                {
                    return err;
                }
                
				/* ��·��ƽ�ж����� */
				if(!type_spe.ir_short_int_en)
				{
					JUDGE_SHORT();
				}
				break;
            }
		}
	}
	/* ���Խ׶� */
	else if(test_flag.gradation == STAGE_TEST && (g_test_time>2 || cur_mode==BBD || cur_mode==CC))
	{
		static int low_times = 0;/* ��¼���ޱ������� */
		
		switch(cur_mode)
		{
			case ACW:
			{
                /* ��ѹ�쳣�ж� */
                err = judge_vol_exception();
                
                if(ERR_NONE != err)
                {
                    return err;
                }
				
                if(ERR_NONE == err)
                {
                    /* �������ޱ��� */
                    if(get_high_err())
                    {
                        record_exception_scene();/* ��¼�쳣�ֳ� */
                        err = ERR_HIGH;
                        result_acw();
                    }
                }
				
                if(ERR_NONE == err)
                {
                    /* ��ʵ�������� */
                    if(err_real)
                    {
                        record_exception_scene();/* ��¼�쳣�ֳ� */
                        err = ERR_REAL;
                    }
                }
				
                if(ERR_NONE == err)
                {
                    /* �������ޱ��� */
                    if(cur_ave < cur_low)
                    {
                        /* ���ޱ����ж����� */
                        if(++low_times >= 2)
                        {
                            record_exception_scene();/* ��¼�쳣�ֳ� */
                            err = ERR_LOW;
                            result_acw();
                            
                            low_times = 0;
                        }
                    }
                }
				
                if(ERR_NONE == err)
                {
                    /* ����ʱ�䵽 */
                    if(tes_t && g_dis_time + 1 >= tes_t)
                    {
                        /* �������ޱ��� */
                        if(cur_ave < cur_low)
                        {
                            /* ���ޱ����ж����� */
                            if(++low_times >= 2)
                            {
                                record_exception_scene();/* ��¼�쳣�ֳ� */
                                ERR_NUM_OVER = ERR_LOW;
                                result_acw();
                                
                                low_times = 0;
                            }
                        }
                    }
                }
				break;
			}
			case DCW:
			{
                /* ��ѹ�쳣�ж� */
                err = judge_vol_exception();
                
                /* �����ʱʱ�� ������ڲ���ʱ��Ӧ������ */
				if(cur_delay_t > tes_t && tes_t != 0)
				{
					cur_delay_t = tes_t;
				}
				
				/* ��ʱ���� */
				if(g_dis_time+1 >= cur_delay_t)
				{
                    if(ERR_NONE == err)
                    {
                        /* �������ޱ��� */
                        if(get_high_err())
                        {
                            record_exception_scene();/* ��¼�쳣�ֳ� */
                            err = ERR_HIGH;
                            result_dcw();
                        }
                    }
					
                    if(ERR_NONE == err)
                    {
                        /* �������ޱ��� */
                        if(cur_ave < cur_low)
                        {
                            record_exception_scene();/* ��¼�쳣�ֳ� */
                            err = ERR_LOW;
                            result_dcw();
                        }
                    }
                    
                    if(ERR_NONE == err)
                    {
                        /* ��籨�� ����ʱ�䵽��ű� */
                        if(tes_t && g_dis_time + 2 >= tes_t)
                        {
                            /* ��籨�� */
                            if(err_charge && cur_charge_cur)
                            {
                                record_exception_scene();/* ��¼�쳣�ֳ� */
                                ERR_NUM_OVER = ERR_CHARGE;
                            }
                        }
                    }
				}
				else
				{
                    if(get_high_err())
                    {
                        reset_high_err();
                    }
                    
                    if(ERR_NONE == err)
                    {
                        /* ��λ���� ���Ĵ�����Ϊ���ֲ���ʱʱ���޷��������ޱ��� */
                        if(cur_ave > get_cur_upper(DCW, cur_gear))
                        {
                            record_exception_scene();/* ��¼�쳣�ֳ� */
                            err = ERR_GEAR;
                            result_dcw();
                        }
                    }
				}
				break;
			}
			case IR:
			{
                /* ��ѹ�쳣�ж� */
                err = judge_vol_exception();
                if(ERR_NONE != err)
                {
                    return err;
                }
                
				if(cur_delay_t > tes_t && tes_t != 0)
				{
					cur_delay_t = tes_t;
				}
				
				/* ��ʱʱ�䵽 */
				if(g_dis_time >= cur_delay_t)
				{
					if(g_dis_time >= 3)
					{
                        /* ����ʱ�䵽ǰ0.1s */
                        if(tes_t && g_dis_time + 1 >= tes_t)
                        {
                            if(ERR_NONE == err)
                            {
                                /* �������ޱ��� */
                                if(res_ave > cur_high && cur_high)
                                {
                                    record_exception_scene();/* ��¼�쳣�ֳ� */
                                    ERR_NUM_OVER = ERR_HIGH;
                                    err_res_bak = res_ave;
                                    result_ir();
                                }
                                else
                                {
                                    ERR_NUM_OVER = 0;
                                }
                            }
                        }
						
						/* ������ʱû����ֱ���˳� �����ж� ����ʱ�䵽���� */
						if(g_ir_dly && g_dis_time != tes_t)
						{
							break;
						}
						
                        if(ERR_NONE == err)
                        {
                            /* �ж϶�·���� */
                            if(res_ave <= 0.5)
                            {
                                record_exception_scene();/* ��¼�쳣�ֳ� */
                                err = ERR_SHORT;
                                result_ir();
                                return err;
                            }
                        }
						
                        if(ERR_NONE == err)
                        {
                            /* �������ޱ��� */
                            if(res_ave < cur_low && vol_ave > 0)
                            {
                                if(++test_flag.ir_err_times > 0)
                                {
                                    record_exception_scene();/* ��¼�쳣�ֳ� */
                                    err = ERR_LOW;
                                    result_ir();
                                }
                            }
                            else
                            {
                                test_flag.ir_err_times = 0;
                            }
                        }
					}
				}
				break;
			}
			case GR:
			{
				if(g_test_time > 2)
				{
                    if(err == ERR_NONE)
                    {
						/* ֱ�� */
                        if(DC_GR_EN)
                        {
//                             err = get_dc_gr_open_err();//test_flag.dc_gr_open_err;
//                             if(err == ERR_OPEN)
//                             {
//                                 res_ave = 5100;
//                                 cur_ave = 0;
//                                 result_gr();
//                                 record_exception_scene();/* ��¼�쳣�ֳ� */
//                             }
                            /* ��ֱ��ģ�����������һֱ��С����δ�յ�ģ��ı�������ʱ�������жϱ��� 
//                             else*/
							if(cur_ave*100 <= cur_vol*0.03)
                            {
                                err = ERR_OPEN;
                                res_ave = 5100;
                                cur_ave = 0;
                                result_gr();
                                record_exception_scene();/* ��¼�쳣�ֳ� */
                            }
                            
                            if(err == ERR_NONE)
                            {
                                err = get_dc_gr_amp_err();
                                if(err == ERR_AMP)
                                {
                                    result_gr();
                                    record_exception_scene();/* ��¼�쳣�ֳ� */
                                }
                            }
                        }
                        /* ���� */
						else
                        {
                            /* ��·���� ����С���趨ֵ��3% ʱ��Ϊ��· */
                            if(cur_ave*100 <= cur_vol*0.03)
                            {
                                if(++test_flag.vol_err_count > 1)
                                {
                                    err = ERR_OPEN;
                                    res_ave = 5100;
                                    cur_ave = 0;
                                    result_gr();
                                    record_exception_scene();/* ��¼�쳣�ֳ� */
                                }
                            }
                            else
                            {
                                test_flag.vol_err_count = 0;
                            }
                        }
                    }
					
                    if(err == ERR_NONE)
                    {
						/* ѹ�������� */
                        if(GR_VOL_DROP_EN)
                        {
                            /* �������ޱ��� */
                            if(res_ave	> cur_high && cur_high)
                            {
                                err = ERR_HIGH;
                                result_gr();
                                record_exception_scene();/* ��¼�쳣�ֳ� */
                            }
                        }
						/* ֱ���ӵ� */
                        else
                        {
                            /* �������ޱ��� */
                            if(res_ave > cur_high && cur_high)
                            {
								/* �ڵ�������������²Ż��ж����ޱ��� */
								if(++test_flag.hight_err_count > 1 && cur_ave > 1.0)
								{
									err = ERR_HIGH;
									result_gr();
									record_exception_scene();/* ��¼�쳣�ֳ� */
								}
                            }
							else
							{
								test_flag.hight_err_count = 0;
							}
                        }
                    }
					
                    if(err == ERR_NONE)
                    {
                        /* ����ʱ�䵽 */
//                         if(tes_t && g_dis_time+1 >= tes_t)
                        /* 0.5s���жϵ������ޱ��� */
                        if(g_dis_time > 4)
                        {
                            if(GR_VOL_DROP_EN == 0)
                            {
                                /* �������ޱ��� */
                                if(res_ave < cur_low)
                                {
                                    err = ERR_LOW;
                                    result_gr();
                                    record_exception_scene();/* ��¼�쳣�ֳ� */
                                }
                            }
                            else
                            {
                                /* �������ޱ��� */
                                if(res_ave < cur_low)
                                {
                                    err = ERR_LOW;
                                    result_gr();
                                    record_exception_scene();/* ��¼�쳣�ֳ� */
                                }
                            }
                        }
                    }
					
					if(err == ERR_NONE)
					{
						/* ƫ�Ʋ��� */
						if(OFFSET_BBD)
						{
							/* ѹ��ģʽ */
							if(GR_VOL_DROP_EN)
							{
								g_cur_step->one_step.gr.offset_res = vol_ave;/* ƫ�Ƶ�ѹ */
							}
							/* GRģʽ */
							else
							{
								g_cur_step->one_step.gr.offset_res = res_ave;
							}
							
							g_cur_step->one_step.gr.offset_result = 1;	/* 1 fail 2pass */
						}
					}
				}
				break;
			}
			case BBD:
			{
				float temp;
                
				if(cur_ave > 2000)
				{
                    record_exception_scene();/* ��¼�쳣�ֳ� */
					err = ERR_SHORT;
					cap_ave = 99.99;
					sprintf((char*)loop_buf, "----");
					result_bbd();
					break;
				}
				
				if(g_test_time < 1)
				{
					break;
				}
				
				if(OFFSET_BBD == 0)
				{
					temp = cur_cap * cur_open_ratio / 100.0;
                    
					if(temp > cap_ave)
					{
						if(++test_flag.err_times > 1)
						{
                            record_exception_scene();/* ��¼�쳣�ֳ� */
							err = ERR_OPEN;
							result_bbd();
						}
					}
					else if(cur_cap * cur_short_ratio / 100.0 < cap_ave)
					{
                        record_exception_scene();/* ��¼�쳣�ֳ� */
						err = ERR_SHORT;
						result_bbd();
					}
				}
				break;
			}
		}//end switch()
	}//end if(test_flag.gradation == STAGE_TEST)
	
	/* ���ű��� */
	if(AMP_PIN == RESET)
	{
        /* 3s����й��ű����ж� */
// 		if(test_flag.err_amp_timer > 2)
        {
            test_flag.err_amp_cont++;
            if(test_flag.err_amp_cont > 20)
            {
                disable_sample_task();/* �رղ�������  ֹͣˢ����Ļ���� ���ֳ���ʱ��״̬ */
                test_flag.err_amp_cont = 0;
                
                /* PWM���Ų�Ҫ�����ű�������Ҫ����·���� */
                if(type_spe.amp_type == PWM_AMP)
                {
                    err = ERR_SHORT;
                }
                /* ���Թ����������� */
                else
                {
                    err = ERR_AMP;
                }
            }
        }
	}
    else
    {
        test_flag.err_amp_cont = 0;
    }
    
	if(ERR_NUM != ERR_NONE)
	{
		err = ERR_NUM;
	}
	
	return err;
}


void adc_sample(void)
{
	#define SM_DEEP 5
    
#if DMA_FOR_ADC_EN>0
	read_adc_from_dma();
	/* Ŀ����Ϊ�˼ӿ���ʾ���� */
	if(g_test_time < 3)
	{
		if(cur_mode == GR)
		{
			sample_vol = dma_buf[1];
			sample_cur = dma_buf[0];
			smoothing_filtering(0, sample_vol);	/* �˲� */
			smoothing_filtering(1, sample_cur);
		}
		else
		{
			sample_vol = dma_buf[0];
			sample_cur = dma_buf[1];
			smoothing_filtering(0, sample_vol);	/* �˲� */
			smoothing_filtering(1, sample_cur);
		}
	}
	else
	{
		if(cur_mode == GR)
		{
			sample_vol = smoothing_filtering(0, adc_value[1]);	/* �˲� */
			sample_cur = smoothing_filtering(1, adc_value[0]);
		}
		else
		{
			sample_vol = smoothing_filtering(1, adc_value[0]);
			sample_cur = smoothing_filtering(0, adc_value[1]);	/* �˲� */
		}
	}
#else
	/* Ŀ����Ϊ�˼ӿ���ʾ���� */
    if(app_flag.calibration == 0 && g_test_time < 3)
	{
		if(cur_mode == GR)
		{
			sample_vol = Get_Adc(13);
			sample_cur = Get_Adc(12);
			smoothing_filtering(0, sample_vol);
			smoothing_filtering(1, sample_cur);
		}
		else
		{
			sample_vol = Get_Adc(12);
			sample_cur = Get_Adc(13);
			smoothing_filtering(0, sample_vol);
			smoothing_filtering(1, sample_cur);
		}
	}
	else
	{
		if(cur_mode == GR)
		{
			sample_vol = smoothing_filtering(0, Get_Adc_Average(13,5));	/* �˲� */
			sample_cur = smoothing_filtering(1, Get_Adc_Average(12,5));
		}
		else
		{
			sample_vol = smoothing_filtering(0, Get_Adc_Average(12,5));
			sample_cur = smoothing_filtering(1, Get_Adc_Average(13,5));	/* �˲� */
		}
	}
#endif
    
    if(test_flag.gradation == STAGE_TEST)
    {
        if(tes_t && g_dis_time < tes_t + 2)
        {
            if(cur_mode == DCW)
            {
                if(sample_cur > cur_charge_cur)
                {
                    err_charge = 0;
                }
            }
        }
    }
	
    g_test_data.sample_vol_ad = sample_vol;
    g_test_data.sample_cur_ad = sample_cur;
    
    if(app_flag.calibration == 1)
    {
        count_dis_value();
    }
}

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
