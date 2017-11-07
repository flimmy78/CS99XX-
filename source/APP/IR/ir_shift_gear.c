/*
 * Copyright(c) 2016,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�ir_shift_gear.c
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V1.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#include "stm32f10x.h"
#include "ir_shift_gear.h"
#include "serve_test.h"
#include "cs99xx_type.h"
#include "calibration.h"
#include "string.h"
#include "ir_relay.h"
#include "ir_test.h"

void shift_gear_dly(void)
{
    if(ir_test_flag.dly > 0)
    {
        ir_test_flag.dly--;
    }
}

void init_set_ir_gear(void)
{
//    ir_set_gear(cur_gear);
}

uint8_t get_cur_area(TEST_DATA_STRUCT *test_data)
{
	uint32_t i = 0;
	uint32_t j = 0;
	float ad_v_1;
	float ad_v_2;
	float ad_v;
	float ad_v_b[CAL_POINTS];
	uint8_t index_b[CAL_POINTS];
	uint8_t flag = 0;
	
	memset(ad_v_b, 0, sizeof(ad_v_b));
	
	for(i = 0, j = 0; i < CAL_POINTS; i++)
	{
		if(test_data->cur_gear == ratio_ir.gear_b[i][test_data->vol_gear])
		{
			index_b[j] = i;
			ad_v_b[j] = ratio_ir.cur_ad_v[i][test_data->vol_gear];
			j++;
		}
	}
	
	if(j <= 1)
	{
		return index_b[0];
	}
	
	if(test_data->cur_ad_vol < ad_v_b[j - 1])
	{
		return index_b[j - 1];
	}
	
	if(test_data->cur_ad_vol > ad_v_b[0])
	{
		return index_b[0];
	}
	
	for(i = 0; i < j - 1; i++)
	{
		ad_v_1 = ad_v_b[i];//��
		ad_v_2 = ad_v_b[i + 1];//С
		
		if(test_data->cur_ad_vol <= ad_v_1 && test_data->cur_ad_vol >= ad_v_2)
		{
			flag = 1;
			ad_v = (ad_v_1 + ad_v_2) / 2;
			break;
		}
	}
	
	if(flag == 0)
	{
		return index_b[j/2];
	}
	
	/* �������ֵ�ھ�ֵ�����Ļ��Ͳ��ڸı䵱ǰ���򱣳��ȶ� */
	if(ad_v > test_data->cur_ad_vol)
	{
		if(ad_v - test_data->cur_ad_vol < (3.3 * 3/4096))
		{
			return index_b[i + 1];
		}
	}
	else
	{
		if(test_data->cur_ad_vol - ad_v < (3.3 * 3/4096))
		{
			return index_b[i + 1];
		}
	}
	
	if(test_data->cur_ad_vol < ad_v)
	{
		return index_b[i + 1];
	}
	
	return index_b[i];
}

static void set_res_k(const uint8_t gear, TEST_DATA_STRUCT *test_data)
{
	int32_t i = 0;
	
	if(test_data->cur_gear == ratio_ir.gear_b[gear][test_data->vol_gear])
	{
		test_data->res_adc_k = ratio_ir.res_3_k[gear][test_data->vol_gear];
	}
	else
	{
		/* ������ */
		for(i = 1; i <= cur_vol_gear_max - cur_vol_gear; i++)
		{
			if(test_data->cur_gear == ratio_ir.gear_b[gear][test_data->vol_gear + i])
			{
				test_data->res_adc_k = ratio_ir.res_3_k[gear][test_data->vol_gear + i];
				return;
			}
		}
		/* ������ */
		for(i = 1; i <= cur_vol_gear; i++)
		{
			if(test_data->cur_gear == ratio_ir.gear_b[gear][test_data->vol_gear - i])
			{
				test_data->res_adc_k = ratio_ir.res_3_k[gear][test_data->vol_gear - i];
				return;
			}
		}
	}
}

void ir_auto_find_gear(TEST_DATA_STRUCT *test_data)
{
    uint8_t LEAD_T  = 20; /* ��ǰʱ�� */
	
    /* ��ʱʱ�仹��LEAD_T����˳� */
    if(ir_test_flag.dly > LEAD_T)
    {
        return;
    }
	
	if(test_data->cur_ad_vol < 0.3)
	{
		if(test_data->cur_gear < test_data->gear_max)
		{
			test_data->cur_gear++;
			ir_set_gear(test_data->cur_gear);
			ir_test_flag.dly = 200;
		}
	}
	else if(test_data->cur_ad_vol > 3.1)
	{
		if(test_data->cur_gear > test_data->gear_min)
		{
			test_data->cur_gear--;
			ir_set_gear(test_data->cur_gear);
			ir_test_flag.dly = 200;
		}
	}
	
	set_res_k(ir_test_flag.cur_res_area, test_data);
}

/******************* (C) COPYRIGHT 2015 ��ʢ���� *****END OF FILE****/
