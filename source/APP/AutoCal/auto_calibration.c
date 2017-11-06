/*
 * Copyright(c) 2016,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�auto_calibration.c
 * ժ  Ҫ  ���Զ�У׼ϵͳ
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#define AUTO_CALIBRATE_GLOBALS

#include "stm32f10x.h"
#include "GUI_Draw.h"
#include "auto_calibration.h"
#include "calibration.h"
#include "serve_test.h"
#include "cs99xx_type.h"
#include "cs99xx_mem_api.h"
#include "DAC_VREF.h"
#include "app.h"
#include "keyboard.h"
#include "auto_cal_server.h"
#include <string.h>
#include "auto_cal_comm.h"
#include "MC14094.h"
#include "dc_module.h"
#include "cs99xx_collect.h"


/* У׼�Ĵ��� */
uint32_t *cur_rang;
uint32_t *ac_cur_rang;
uint32_t *dc_cur_rang;
uint32_t res_rang;
static void auto_cal_vol_fun(void);
static void auto_cal_cur_fun(void);
static void auto_cal_gr_fun(void);
static void auto_cal_ir_fun(void);

/*
 * ��������init_cal_cur_order
 * ����  ����ʼ������У׼����
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void init_cal_cur_order(uint8_t mode, CAL_ORDER* order, uint16_t *index)
{
    uint16_t temp = *index;
    uint8_t temp_gear = 0;
    void (*fun)(void) = auto_cal_cur_fun;
    
    switch(mode)
    {
        case ACW:
        {
            temp_gear = type_spe.acw_gear;
            
            if(temp_gear & _AC_200uA)
            {
                order[temp].num = ACW_CUR_G_200uA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_AC_A;
                order[temp].range = cur_rang[AC_200uA];
                order[temp++].data = AC_200uA;
            }
            if(temp_gear & _AC_2mA)
            {
                order[temp].num = ACW_CUR_G_2mA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_AC_A;
                order[temp].range = cur_rang[AC_2mA];
                order[temp++].data = AC_2mA;
            }
            if(temp_gear & _AC_10mA)
            {
                order[temp].num = ACW_CUR_G_10mA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_AC_A;
                order[temp].range = cur_rang[AC_10mA];
                order[temp++].data = AC_10mA;
            }
            if(temp_gear & _AC_20mA)
            {
                order[temp].num = ACW_CUR_G_20mA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_AC_A;
                order[temp].range = cur_rang[AC_20mA];
                order[temp++].data = AC_20mA;
            }
            if(temp_gear & _AC_50mA)
            {
                order[temp].num = ACW_CUR_G_50mA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_AC_A;
                order[temp].range = cur_rang[AC_50mA];
                order[temp++].data = AC_50mA;
            }
            if(temp_gear & _AC_100mA)
            {
                order[temp].num = ACW_CUR_G_100mA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_AC_A;
                order[temp].range = cur_rang[AC_100mA];
                order[temp++].data = AC_100mA;
            }
            if(temp_gear & _AC_200mA)
            {
                order[temp].num = ACW_CUR_G_200mA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_AC_A;
                order[temp].range = cur_rang[AC_200mA];
                order[temp++].data = AC_200mA;
            }
            
            break;
        }
        case DCW:
        {
            temp_gear = type_spe.dcw_gear;
            
            if(temp_gear & _DC_2uA)
            {
                order[temp].num = DCW_CUR_G_2uA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_DC_A;
                order[temp].range = cur_rang[DC_2uA];
                order[temp++].data = DC_2uA;
            }
            if(temp_gear & _DC_20uA)
            {
                order[temp].num = DCW_CUR_G_20uA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_DC_A;
                order[temp].range = cur_rang[DC_20uA];
                order[temp++].data = DC_20uA;
            }
            if(temp_gear & _DC_200uA)
            {
                order[temp].num = DCW_CUR_G_200uA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_DC_A;
                order[temp].range = cur_rang[DC_200uA];
                order[temp++].data = DC_200uA;
            }
            if(temp_gear & _DC_2mA)
            {
                order[temp].num = DCW_CUR_G_2mA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_DC_A;
                order[temp].range = cur_rang[DC_2mA];
                order[temp++].data = DC_2mA;
            }
            if(temp_gear & _DC_10mA)
            {
                order[temp].num = DCW_CUR_G_10mA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_DC_A;
                order[temp].range = cur_rang[DC_10mA];
                order[temp++].data = DC_10mA;
            }
            if(temp_gear & _DC_20mA)
            {
                order[temp].num = DCW_CUR_G_20mA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_DC_A;
                order[temp].range = cur_rang[DC_20mA];
                order[temp++].data = DC_20mA;
            }
            if(temp_gear & _DC_50mA)
            {
                order[temp].num = DCW_CUR_G_50mA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_DC_A;
                order[temp].range = cur_rang[DC_50mA];
                order[temp++].data = DC_50mA;
            }
            if(temp_gear & _DC_100mA)
            {
                order[temp].num = DCW_CUR_G_100mA;
                order[temp].fun = fun;
                order[temp].mode = mode;
                order[temp].type = AUTOCAL_DC_A;
                order[temp].range = cur_rang[DC_100mA];
                order[temp++].data = DC_100mA;
            }
            break;
        }
    }
    
    *index = temp;
}

uint8_t get_gr_total_cal_points(void)
{
	uint8_t n = 0;
	
	if(DC_GR_EN)
	{
		n = get_dc_module_using_num();
		n *= 3;
		
		return n;
	}
	else
	{
		return 3;
	}
}

/*
 * ��������init_cal_vol_order
 * ����  ����ʼ����ѹУ׼����
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void init_cal_vol_order(uint8_t mode, CAL_ORDER* order, uint16_t *index)
{
    uint16_t temp = *index;
    int32_t i = 0;
    uint8_t n = 0;
    uint16_t start_point = 0;
    void (*fun)(void) = NULL;
    uint8_t type = 0;
    uint32_t range[6];
    
    if(mode == GR)
    {
		n = get_gr_total_cal_points();
        start_point = GR_CUR_SEG_11;
        fun = auto_cal_gr_fun;
        type = AUTOCAL_GR_A;
    }
    else
    {
        fun = auto_cal_vol_fun;
        
        if(type_spe.vol_is_segment == VOL_SEGMENT_YES)
        {
            n = 6;
            if(mode == ACW)
            {
                start_point = ACW_VOL_SEG_11;
                type = AUTOCAL_AC_V;
				get_acw_vol_cal_point_range(range);
            }
            else if(mode == DCW)
            {
                start_point = DCW_VOL_SEG_11;
                type = AUTOCAL_DC_V;
				get_dcw_vol_cal_point_range(range);
            }
            else if(mode == IR)
            {
                start_point = IR_VOL_SEG_11;
                type = AUTOCAL_DC_V;
				get_ir_vol_cal_point_range(range);
            }
        }
        else
        {
            n = 3;
            if(mode == ACW)
            {
                start_point = ACW_VOL_SEG_21;
                type = AUTOCAL_AC_V;
				get_acw_vol_cal_point_range(range);
            }
            else if(mode == DCW)
            {
                start_point = DCW_VOL_SEG_21;
                type = AUTOCAL_DC_V;
				get_dcw_vol_cal_point_range(range);
            }
            else if(mode == IR)
            {
                start_point = IR_VOL_SEG_21;
                type = AUTOCAL_DC_V;
				get_ir_vol_cal_point_range(range);
            }
        }
    }
    
	for(i = 0; i < n; i++, temp++)
    {
        order[temp].num = start_point + i;
        order[temp].fun = fun;
        order[temp].data = i % 3;
        order[temp].mode = mode;
        order[temp].type = type;
        order[temp].range = range[i] * 10;//����ѹ��λתΪ0.1V
    }
    
    *index = temp;
}


/*
 * ��������init_cal_order
 * ����  ����ʼ������У׼����
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void init_cal_res_order(uint8_t mode, CAL_ORDER* order, uint16_t *index)
{
    uint16_t temp = *index;
    int32_t i = 0;
    uint16_t start_point = IR_RES_G_11;
    uint8_t type = AUTOCAL_RES;
    uint8_t meal = get_ir_res_meal();
    uint8_t n = geat_ir_res_cal_points_num();
    
    for(i = 0; i < n; i++, temp++)
    {
        order[temp].num = start_point + i;
        order[temp].fun = auto_cal_ir_fun;
        order[temp].data = i;
        order[temp].mode = mode;
        order[temp].type = type;
        order[temp].range = ir_cur_cal.point_da[meal][i] * 1000;/* ���赥λk�� */
    }
    
    *index = temp;
}
/*
 * ��������init_cal_order
 * ����  ����ʼ��У׼����
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void init_cal_order_table(uint8_t mode, CAL_ORDER* order, uint16_t *index)
{
    /* ��ѹ */
    init_cal_vol_order(mode, order, index);
    
    if(mode == ACW || mode == DCW)
    {
        /* ���� */
        init_cal_cur_order(mode, order, index);
    }
    else if(mode == IR)
    {
        /* ���� */
        init_cal_res_order(mode, order, index);
    }
}

/*
 * ��������init_cal_order
 * ����  ����ʼ��У׼����
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void init_cal_order(void)
{
    uint16_t index = 0;
    
    if(type_spe.mode_en & __ACW)
    {
		select_cur_range_pool(ACW);
        init_cal_order_table(ACW, cal_order, &index);
    }
    
    if(type_spe.mode_en & __DCW)
    {
		select_cur_range_pool(DCW);
        init_cal_order_table(DCW, cal_order, &index);
    }
    
    if(type_spe.mode_en & __IR)
    {
        init_cal_order_table(IR, cal_order, &index);
    }
    
    if(type_spe.mode_en & __GR)
    {
        init_cal_order_table(GR, cal_order, &index);
    }
    
    auto_cal.cal_total_points = index;
}
/*
 * ��������count_cur_ratio
 * ����  ������ӵص���У׼ϵ��
 * ����  ��p �ɼ�������ָ�룬������ϵ���ṩ����
 * ���  ����
 * ����  ����
 */
static void count_cur_ratio(void* p)
{
    AUTO_CAL_CUR *cal = p;
    
    if(cal->ad_i_k == NULL || p == NULL)
    {
        return;
    }
    
    if(cal->base.ad > 0)
    {
        *cal->ad_i_k = cal->base.measure / cal->base.ad;
    }
}
/*
 * ��������count_gr_ratio
 * ����  ������ӵص���У׼ϵ��
 * ����  ��p �ɼ�������ָ�룬������ϵ���ṩ����
 * ���  ����
 * ����  ����
 */
static void count_gr_ratio(void* p)
{
    AUTO_CAL_GR_CUR *cal = p;
    int32_t i = 0;
    uint8_t n = 3;
    float sumx=0.0,sumy=0.0,sumz=0.0,sumxx=0.0,sumzz=0.0,sumxy=0.0,sumxz=0.0;
    
    if(cal->da_k == NULL || cal->da_b == NULL || cal->ad_v_k == NULL || cal->ad_v_b == NULL
        || cal->ad_i_k == NULL || cal->ad_i_b == NULL)
    {
        return;
    }
    
    /* ������������Ƿ�Ϊ0 ����оͲ�������ֱ��ֹͣ */
    for(i = 0; i < 3; i++)
    {
        if(cal->base[i].measure == 0)
        {
            return;
        }
    }
    
    // ����ϵ����������㷨
    for(i = 0; i < n; i++)
    {
        sumx  += cal->base[i].measure;
        sumy  += cal->base[i].da;
        sumz  += cal->base[i].ad_vol;
        sumxx += cal->base[i].measure * cal->base[i].measure;
        sumzz += cal->base[i].ad_vol * cal->base[i].ad_vol;
        sumxy += cal->base[i].measure * cal->base[i].da;
        sumxz += cal->base[i].measure * cal->base[i].ad_vol;
    }
    
    if(n * sumxx - sumx * sumx)
    {
        *cal->da_k = ((float)sumxy * n - sumx * sumy) / (n * sumxx - sumx * sumx);
        *cal->da_b = ((float)sumy - (*cal->da_k) * sumx) / n;
    }
    
    if(n * sumzz - sumz * sumz)
    {
        *cal->ad_v_k = ((float)sumxz * n - sumx * sumz) / (n * sumzz - sumz * sumz);
        *cal->ad_v_b = ((float)sumx - (*cal->ad_v_k) * sumz) / n;
    }
    
    sumx = sumz = sumxx = sumzz = sumxz = 0.0;
    for(i = 0; i < n; i++)
    {
        sumx 	+= (cal->base[i].measure / cal->base[i].dis);//����=��ѹ/����
        sumz	+= cal->base[i].ad_cur;
        sumxx	+= (cal->base[i].measure / cal->base[i].dis * cal->base[i].measure / cal->base[i].dis);
        
        sumzz	+= (cal->base[i].ad_cur * cal->base[i].ad_cur);
        sumxz	+= (cal->base[i].measure / cal->base[i].dis * cal->base[i].ad_cur);
    }
    
    if(n * sumzz - sumz * sumz)
    {
        *cal->ad_i_k = ((float)sumxz * n - sumx * sumz) / (n * sumzz - sumz * sumz);
        *cal->ad_i_b = ((float)sumx - (*cal->ad_i_k) * sumz) / n;
    }
}

/*
 * ��������count_vol_ratio
 * ����  �������ѹУ׼ϵ��
 * ����  ��p �ɼ�������ָ�룬������ϵ���ṩ����
 * ���  ����
 * ����  ����
 */
static void count_vol_ratio(void* p)
{
    AUTO_CAL_VOL *cal = p;
    int32_t i = 0;
    uint8_t n = 3;
    float sumx=0.0,sumy=0.0,sumz=0.0,sumxx=0.0,sumzz=0.0,sumxy=0.0,sumxz=0.0;
    
    if(cal->da_k == NULL || cal->da_b == NULL || cal->ad_k == NULL || cal->ad_b == NULL)
    {
        return;
    }
    
    /* ������������Ƿ�Ϊ0 ����оͲ�������ֱ��ֹͣ */
    for(i = 0; i < 3; i++)
    {
        if(cal->base[i].measure == 0)
        {
            return;
        }
    }
    
    for(i = 0; i < n; i++)
    {
        sumx  += cal->base[i].measure;
        sumy  += cal->base[i].da;
        sumz  += cal->base[i].ad;
        sumxx += cal->base[i].measure * cal->base[i].measure;
        sumzz += cal->base[i].ad * cal->base[i].ad;
        sumxy += cal->base[i].measure * cal->base[i].da;
        sumxz += cal->base[i].measure * cal->base[i].ad;
    }
    
    if(n * sumxx - sumx * sumx)
    {
        *cal->da_k = ((float)sumxy * n - sumx * sumy) / (n * sumxx - sumx * sumx);
        *cal->da_b = ((float)sumy - (*cal->da_k) * sumx) / n;
    }
    
    
    if(n * sumzz - sumz * sumz)
    {
        *cal->ad_k = ((float)sumxz * n - sumx * sumz) / (n * sumzz - sumz * sumz);
        *cal->ad_b = ((float)sumx - (*cal->ad_k) * sumz) / n;
    }
    
}

/*
 * ��������auto_cal_save_ratio
 * ����  ���Զ�У׼�������ݵĹ��ýӿ�
 * ����  ��mode Ҫ�����ģʽ
 * ���  ����
 * ����  ����
 */
static void auto_cal_save_ratio(const uint8_t mode)
{
    save_ratio(mode);
}

static void auto_cal_save_dc_gr_ratio(const uint8_t mode)
{
	send_dc_module_cmd(DC_MODULE_CMD_SET_CAL_DATA, 0);/* ��ȡ�������� */
}
/*
 * ��������init_acw_vol_cal_info
 * ����  ����ʼ����ʼ��DCW��ѹ��Ϣ
 * ����  ��n У׼�������һ��3����
 * ���  ��info ���س�ʼ���������
 * ����  ����
 */
static void init_acw_vol_cal_info(AUTO_CAL_VOL *info, uint8_t n)
{
    get_vol_cal_point_info(ACW, &info->base[n], n);
    
    info->da_k = &ratio_acw.dac_k[vol_segment];
    info->da_b = &ratio_acw.dac_b[vol_segment];
    info->ad_k = &ratio_acw.adc_v_k[vol_segment];
    info->ad_b = &ratio_acw.adc_v_b[vol_segment];
    info->count_ratio = count_vol_ratio;
    info->save = auto_cal_save_ratio;
}
/*
 * ��������init_dcw_vol_cal_info
 * ����  ����ʼ����ʼ��DCW��ѹ��Ϣ
 * ����  ��n У׼�������һ��3����
 * ���  ��info ���س�ʼ���������
 * ����  ����
 */
static void init_dcw_vol_cal_info(AUTO_CAL_VOL *info, uint8_t n)
{
    get_vol_cal_point_info(DCW, &info->base[n], n);
    
    info->da_k = &ratio_dcw.dac_k[vol_segment];
    info->da_b = &ratio_dcw.dac_b[vol_segment];
    info->ad_k = &ratio_dcw.adc_v_k[vol_segment];
    info->ad_b = &ratio_dcw.adc_v_b[vol_segment];
    info->count_ratio = count_vol_ratio;
    info->save = auto_cal_save_ratio;
}
/*
 * ��������init_ir_vol_cal_info
 * ����  ����ʼ����ʼ����Ե�����ѹ��Ϣ
 * ����  ��n У׼�������һ��3����
 * ���  ��info ���س�ʼ���������
 * ����  ����
 */
static void init_ir_vol_cal_info(AUTO_CAL_VOL *info, uint8_t n)
{
    get_vol_cal_point_info(IR, &info->base[n], n);
    
    info->da_k = &ratio_ir.dac_k[vol_segment];
    info->da_b = &ratio_ir.dac_b[vol_segment];
    info->ad_k = &ratio_ir.adc_v_k[vol_segment];
    info->ad_b = &ratio_ir.adc_v_b[vol_segment];
    info->count_ratio = count_vol_ratio;
    info->save = auto_cal_save_ratio;
}
/*
 * ��������init_ir_res_cal_info
 * ����  ����ʼ��������Ϣ
 * ����  ��n У׼�������һ��3����
 * ���  ��info ���س�ʼ���������
 * ����  ����
 */
static void init_ir_res_cal_info(AUTO_CAL_IR_RES *info, uint8_t n)
{
    get_res_cal_point_info(IR, &info->base, n);
    
//    info->k = &ratio_ir.res_k[info->base.gear][0];
    
    info->count_ratio = count_vol_ratio;
    info->save = auto_cal_save_ratio;
}
/*
 * ��������init_gr_cal_info
 * ����  ����ʼ��������Ϣ
 * ����  ��times У׼�������һ��3����
 * ���  ��info ���س�ʼ���������
 * ����  ����
 */
static void init_gr_cal_info(AUTO_CAL_GR_CUR *info, uint8_t n)
{
    int32_t i = n;
    uint8_t gear = 0;
    
    get_vol_cal_point_info(GR, &info->base[i], n);
    gear = get_gr_gear();
    
    info->da_k = &ratio_gr.dac_k[gear];
    info->da_b = &ratio_gr.dac_b[gear];
    
    info->ad_v_k = &ratio_gr.adc_v_k[gear];
    info->ad_v_b = &ratio_gr.adc_v_b[gear];
    
    info->ad_i_k = &ratio_gr.adc_i_k[gear];
    info->ad_i_b = &ratio_gr.adc_i_b[gear];
    
    info->count_ratio = count_gr_ratio;
    info->save = auto_cal_save_ratio;
}
static void init_dc_gr_cal_info(AUTO_CAL_GR_CUR *info, uint8_t n)
{
    int32_t i = n;
	uint8_t index = 0;
    
    get_vol_cal_point_info(GR, &info->base[i], n);
    
	if(-1 == find_dc_gr_map_ratio_n(get_cur_cal_dc_module_addr(), &index))
	{
		return;
	}
	
    info->da_k = &ratio_dc_gr[0][index].dac_k;
    info->da_b = &ratio_dc_gr[0][index].dac_b;
	
	info->ad_i_k = &ratio_dc_gr[0][index].adc_i_k;
	info->ad_i_b = &ratio_dc_gr[0][index].adc_i_b;
	
    info->ad_v_k = &ratio_dc_gr[0][index].adc_v_k;
    info->ad_v_b = &ratio_dc_gr[0][index].adc_v_b;
    
    info->count_ratio = count_gr_ratio;
    info->save = auto_cal_save_dc_gr_ratio;
}
/*
 * ��������init_cur_cal_info
 * ����  ����ʼ��������Ϣ
 * ����  ��mode ģʽ
 *         gear ��λ
 *         vol  �����ѹֵ ���ڼ������DA
 * ���  ��info ���س�ʼ���������
 *         
 * ����  ����
 */
static void init_cur_cal_info(uint8_t mode, AUTO_CAL_CUR *info, uint8_t gear, uint16_t vol)
{
    if(info == NULL)
    {
        return;
    }
    
    if(mode == ACW)
    {
        info->ad_i_k = &ratio_acw.adc_cur_k[gear];
        info->da_k = ratio_acw.dac_k[vol_segment];
        info->da_b = ratio_acw.dac_b[vol_segment];
    }
    else if(mode == DCW)
    {
        info->ad_i_k = &ratio_dcw.adc_cur_k[gear];
        info->da_k = ratio_dcw.dac_k[vol_segment];
        info->da_b = ratio_dcw.dac_b[vol_segment];
    }
    else
    {
        info->ad_i_k = NULL;
        info->da_k = NULL;
        info->da_b = NULL;
        info->count_ratio = NULL;
        info->save = NULL;
        return;
    }
    
    info->output = vol;
    
    get_cur_cal_point_info(mode, info, gear);
    
    info->count_ratio = count_cur_ratio;
    info->save = auto_cal_save_ratio;
}
/*
 * ��������test_led_flash
 * ����  ������ʱ���Ե���˸
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void test_led_flash(void)
{
    LED_TEST = !LED_TEST;
}
/*
 * ��������auto_cal_ir_fun
 * ����  ���Զ�У׼��Ե������ں���
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void auto_cal_ir_fun(void)
{
	uint16_t ir_vol = 0;
	uint16_t temp_cur = 0;
	uint16_t temp_vol = 0;
	uint16_t cal_time = 0;
	float std_res = 0.0;/* ��׼�ĵ���ֵ */
	int i = 0;
	int j = 0;
    uint8_t temp_delay = 0;
    uint8_t temp = 0;
    uint8_t target_gear = 0;/* Ŀ�굵λ */
    uint8_t vol_gear_num;/* ��ѹ��λ���� */
    uint8_t vol_base;/* ��ѹ��ʼ�� */
    uint8_t mode =  cal_order[g_cur_cal_opt_num].mode;
    uint8_t n = cal_order[g_cur_cal_opt_num].data;
    
    
    /* ��ȡ��־�����ֵ */
    auto_measure = 0;/* ���赥λk�� */
    init_ir_res_cal_info(&cal_info_un.ir_res, n);
    
    clear_ui_ele();
    transform_auto_cal_num_to_string(g_auto_ele_info->ele_str[UI_ELE_NUM], g_cur_cal_opt_num);
    strcpy((void*)g_auto_ele_info->ele_str[UI_ELE_MODE], (void*)mode_pool[0][mode]);
    strcpy((void*)g_auto_ele_info->ele_str[UI_ELE_OPT], (void*)opt_pool[RES_OPT]);
    strcpy((void*)g_auto_ele_info->ele_str[UI_ELE_POINT], (void*)cal_info_un.cur.base.name);
    
    draw_auto_cal_ui_ele(&GUI_FontHZ_SimSun_12, g_auto_ele_info->auto_cal_ui_ele);
    
    auto_cal.state = AUTOCAL_WAIT_DATA;/* ����״̬Ϊ���ڵȴ�У׼���� */
    
    while(auto_measure == 0)
    {
        OSTimeDlyHMSM(0,0,0,10);
        
		/* ���¸�λ��ʱ��Ҫ���� */
		if(0 == STOP_PIN)
		{
			stop_cal(mode);
			return;
		}
        
        if(auto_cal.state == AUTOCAL_WAIT_OUTPUT)
        {
            stop_cal(mode);
            return;
        }
    }
    
    auto_cal.state = AUTOCAL_OUTPUTING;/* ����״̬Ϊ���ڵȴ�У׼���� */
    std_res = auto_measure / 1000;//cal_info_un.ir_res.base.dis;
    cal_time = 100;//delay_cal[gear];
	vol_segment = 1;
    //cal_info_un.cur.base.measure
    target_gear = cal_info_un.ir_res.base.gear;//ir_cur_cal.gear[cur_cal_ir_res_meal][gear - 1];
    
    cur_gear = target_gear;
    transform_auto_cal_res_to_string(g_auto_ele_info->ele_str[UI_ELE_MEASURE], std_res, mode, cur_gear);
    draw_auto_cal_ui_ele(&GUI_FontHZ_SimSun_12, g_auto_ele_info->auto_cal_ui_ele);
    
    switch(type_spe.ir_vol_range)
    {
        case IR_1kV:
            vol_gear_num = 10;
            vol_base = 0;/* 100v��ʼ */
            break;
        case IR_1_5kV:
            vol_gear_num = 15;
            vol_base = 0;/* 100v��ʼ */
            break;
        case IR_2_5kV:
            vol_gear_num = 17;
            vol_base = 4;/* 500v��ʼ */
            break;
        case IR_5kV:
            vol_gear_num = 22;
            vol_base = 4;/* 500v��ʼ */
            break;
    }
    
	startup_cal(IR, cur_gear);
    
	while(1)
	{
		/* ���¸�λ��ʱ��Ҫ���� */
		if(0 == STOP_PIN)
		{
			stop_cal(IR);
			return;
		}
        
		ir_vol = 0;
        
        MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, 0);/* DA��ѹ��ѡ�� */
        MC14094_Updata();
        
		for(i = vol_base; i < vol_gear_num; i++)
		{
            if(i < 15)
            {
                ir_vol = 100 + i * 100;/* ÿ�ε���100v */
            }
            else
            {
                ir_vol = 1500 + (i - 14) * 500;/* ÿ�ε���500v */
            }
            
            temp = 5;
            
            if(ir_vol > 500)
            {
                vol_segment = 1;
            }
            else
            {
                vol_segment = 0;
            }
            
            MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !vol_segment);
            MC14094_Updata();
            
            DAC_Vol[0] = ir_vol * ratio_ir.dac_k[vol_segment] + ratio_ir.dac_b[vol_segment];/* �����ѹ ʹ��dcw��У׼ϵ�� */
            g_ir_dly += 1000;
            
			for(j = 0; j < cal_time; j++)
			{
                test_led_flash();
                
				temp_vol = sample_vol * ratio_ir.adc_v_k[vol_segment] + ratio_ir.adc_v_b[vol_segment];
				temp_cur = sample_cur;
                
                if(cur_gear > 4)
                {
                    OSTimeDlyHMSM(0,0,0,90);
                }
                else
                {
                    OSTimeDlyHMSM(0,0,0,9);
                }
                
                if(++temp_delay > temp)
                {
                    temp_delay = 0;
        transform_auto_cal_da_ad_to_string(g_auto_ele_info->ele_str[UI_ELE_DA], DAC_Vol[0]);
        
        transform_auto_cal_da_ad_to_string(g_auto_ele_info->ele_str[UI_ELE_AD_VOL], sample_vol);
        transform_auto_cal_da_ad_to_string(g_auto_ele_info->ele_str[UI_ELE_AD_CUR], sample_cur);
        transform_auto_cal_vol_to_string(g_auto_ele_info->ele_str[UI_ELE_VOL], vol_ave);
        transform_auto_cal_da_ad_to_string(g_auto_ele_info->ele_str[UI_ELE_GEAR], cur_gear);
        draw_auto_cal_ui_ele(&GUI_FontHZ_SimSun_12, g_auto_ele_info->auto_cal_ui_ele);
                }
                
				if(STOP_PIN == 0)
				{
					stop_cal(IR);
					return;
				}
                
                if(auto_cal.state == AUTOCAL_WAIT_OUTPUT)
                {
                    stop_cal(mode);
                    return;
                }
			}
			
//            ratio_ir.res_k[n][i] = (float)std_res * temp_cur / (float)temp_vol;
			
// 			ratio_ir.res_k[cur_gear-1][i] = (float)std_res * temp_cur / (float)temp_vol;
		}
        
        auto_cal.state = AUTOCAL_WAIT_STOP;
        
		stop_cal(IR);
		save_ratio(IR);
		
        while(auto_cal.state == AUTOCAL_WAIT_STOP)
        {
            OSTimeDlyHMSM(0,0,0,50);
            
            if(STOP_PIN == 0)
            {
                stop_cal(IR);
                return;
            }
            
            if(auto_cal.state == AUTOCAL_WAIT_OUTPUT)
            {
                stop_cal(mode);
                return;
            }
        }
        
		return;
	}
}

/*
 * ��������auto_cal_gr_fun
 * ����  ���Զ�У׼�ӵص�����ں���
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void auto_cal_gr_fun(void)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
    uint8_t times = cal_order[g_cur_cal_opt_num].data;
    uint8_t mode =  cal_order[g_cur_cal_opt_num].mode;
    uint8_t gear = get_gr_gear();
	uint16_t temp_cur_ad, temp_vol_ad;
	
	if(DC_GR_EN)
	{
		set_cur_cal_dc_module_addr(get_dc_gr_module_id(times/3));//ѡ��ǰģ���ͨ�ŵ�ַ
		init_dc_gr_cal_info(&cal_info_un.gr, times);
	}
	else
	{
		/* ��ʼ��GR��У׼��Ϣ */
		init_gr_cal_info(&cal_info_un.gr, times);
	}
    
    /* ���� */
    clear_ui_ele();
    transform_auto_cal_num_to_string(g_auto_ele_info->ele_str[UI_ELE_NUM], g_cur_cal_opt_num);
    strcpy((void*)g_auto_ele_info->ele_str[UI_ELE_MODE], (void*)mode_pool[0][mode]);
    strcpy((void*)g_auto_ele_info->ele_str[UI_ELE_OPT], (void*)opt_pool[CUR_OPT]);
    strcpy((void*)g_auto_ele_info->ele_str[UI_ELE_POINT], (void*)cal_info_un.gr.base[times].name);
    
    transform_auto_cal_da_ad_to_string(g_auto_ele_info->ele_str[UI_ELE_DA], cal_info_un.gr.base[times].da);
    
    auto_measure = 0;
	startup_cal(GR, cur_gear);
    
	if(DC_GR_EN)
	{
		set_cur_cal_dc_module_addr(get_dc_gr_module_id(times/3));//ѡ��ǰģ���ͨ�ŵ�ַ
		app_flag.dc_gr_module_status = DC_GR_RUNING;
		cur_vol = cal_info_un.gr.base[times].da;
        send_dc_module_cmd(DC_MODULE_CMD_DC, cur_vol);
	}
	/* ���� */
	else
	{
        DAC_Vol[0] = cal_info_un.gr.base[times].da;
	}
    
	auto_cal.state = AUTOCAL_WAIT_DATA;/* ����״̬Ϊ���ڵȴ�У׼���� */
    
	while(1)
	{
		if(DC_GR_EN)
		{
			if(test_flag.uart_next == 0)
			{
				test_flag.uart_next = 1;
				
				send_dc_module_cmd(DC_MODULE_CMD_FETCH, 0);/* ��ȡ�������� */
			}
			
			if(0 == get_dc_gr_ad(get_cur_cal_dc_module_addr(), &temp_cur_ad, &temp_vol_ad))
			{
				test_flag.uart_next = 0;
				sample_cur = temp_cur_ad;/* �ɼ��ĵ���ֵ */
				sample_vol = temp_vol_ad;/* �ɼ���ѹֵ */
			}
			if(ERR_OPEN == get_dc_gr_open_err())
			{
				stop_cal(mode);
				return;
			}
		}
		
        test_led_flash();
//         draw_frame_data(frame_buf);
        
        cal_info_un.gr.base[times].ad_vol = sample_vol;/* �ɼ���ѹֵ */
        cal_info_un.gr.base[times].ad_cur = sample_cur;/* �ɼ��ĵ���ֵ */
        
        dis_gr_measure(auto_measure / 10);/* ����ֵ�ĵ�λ0.1mv */
        dis_cal_ele(sample_vol, sample_cur, 0);
        
        OSTimeDlyHMSM(0,0,0,50);
        
		/* ���¸�λ��ʱ��Ҫ���� */
		if(0 == STOP_PIN)
		{
			stop_cal(mode);
			return;
		}
        
        if(auto_cal.state == AUTOCAL_WAIT_OUTPUT)
        {
            stop_cal(mode);
            return;
        }
        
        if(auto_cal.state == AUTOCAL_SAVEDATA)
        {
            cal_info_un.gr.base[times].measure = auto_measure / 10;/* ����ֵ */
            cal_info_un.gr.count_ratio(&cal_info_un);
            cal_info_un.gr.save(mode);
            auto_cal.state = AUTOCAL_WAIT_STOP;
            LED_FAIL++;// = LED_ON;
        }
        
		pkey_value = OSQAccept(KeyboardQSem, &err);
        
		if(NULL == pkey_value)
        {
            continue;
        }
        
		switch(*pkey_value)
		{
			case KEY_F4:
                stop_cal(mode);
                return;
            case KEY_EXIT:
                stop_cal(mode);
                return;
        }
    }
}


/*
 * ��������auto_cal_cur_fun
 * ����  ���Զ�У׼��ѹ��У׼������ں���
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void auto_cal_cur_fun(void)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	
    uint8_t mode =  cal_order[g_cur_cal_opt_num].mode;
    uint8_t gear = cal_order[g_cur_cal_opt_num].data;
	
	float *unit_k;
    //��λת��ͨ�ŵĵ�λ��0.01uA
    float ac_unit_k[]={
        1.0,
        10.0,  //2uA		Э���е�1��ʾ 0.01uA 2uA����ʾ��ʽ��	x.xxx==>1 ��ʾ 0.001uA Э���е�1Ӧ��ʾΪ0.010uA ��� x 10
        1.0,   //20uA		Э���е�1��ʾ 0.01uA 20uA����ʾ��ʽ��	xx.xx==>1 ��ʾ 0.01uA  Э���е�1Ӧ��ʾΪ0.010uA ��� x 1
        0.1,    //200uA		Э���е�1��ʾ 0.01uA 200uA����ʾ��ʽ��	xxx.x==>1 ��ʾ 0.1uA   Э���е�10Ӧ��ʾΪ000.1uA ��� / 10
        0.01,   //2mA
        0.001,   //10mA
        0.001,   //20mA
        0.001,   //50mA
        0.0001,  //100mA
        0.0001,  //200mA
    };
    //��λת��ͨ�ŵĵ�λ��0.001uA
    float dc_unit_k[]={
        1.0,
        1.0,    //2uA	 Э���е�1��ʾ 0.001uA 2uA����ʾ��ʽ��	x.xxx 		1 ��ʾ 0.001uA Э���е�1Ӧ��ʾΪ0.001uA ��� x 1.0
        0.1,   //20uA
        0.01,    //200uA
        0.001,    //2mA	 Э���е�1��ʾ 0.001uA 2mA����ʾ��ʽ��	x.xxx ==> 1 ��ʾ 0.001mA Э���е�1000Ӧ��ʾΪ0.001mA ��� /1000
        0.0001,   //10mA Э���е�1��ʾ 0.001uA 2mA����ʾ��ʽ��	xx.xx ==> 1 ��ʾ 00.01mA Э���е�10000Ӧ��ʾΪ00.01mA ��� /10000
        0.0001,   //20mA
        0.0001,   //50mA
        0.00001,  //100mA
        0.00001,  //200mA
    };
    //������λ�����ѹ ��λV
    uint16_t output[]=
    {
        0,
        500,    //2uA
        500,    //20uA
        100,    //200uA
        500,    //2mA
        500,    //10mA
        500,    //20mA
        500,    //50mA
        500,    //100mA
        500,    //200mA
    };
    
	switch(mode)
	{
		case ACW:
			unit_k = ac_unit_k;
			break;
		case DCW:
			unit_k = dc_unit_k;
			break;
		default:
			return;
	}
	
    auto_measure = 0;
    cur_gear = gear;//�������������ʹ�� count_dis_value
    /* ���� */
    clear_ui_ele();
    cur_vol = output[gear];/* �����ѹΪ500V */
    confirm_vol_segment_s();
    load_ratio(mode);//����У׼ϵ�����ڼ����ѹ
    
    /* ��ʼ��������У׼��Ϣ */
    init_cur_cal_info(mode, &cal_info_un.cur, gear, cur_vol);
    
	startup_cal(mode, gear);
    
    DAC_Vol[0] = cal_info_un.cur.base.da;
    
    clear_ui_ele();
    transform_auto_cal_num_to_string(g_auto_ele_info->ele_str[UI_ELE_NUM], g_cur_cal_opt_num);
    strcpy((void*)g_auto_ele_info->ele_str[UI_ELE_MODE], (void*)mode_pool[0][mode]);
    strcpy((void*)g_auto_ele_info->ele_str[UI_ELE_OPT], (void*)opt_pool[CUR_OPT]);
    strcpy((void*)g_auto_ele_info->ele_str[UI_ELE_POINT], (void*)cal_info_un.cur.base.name);
    
    transform_auto_cal_da_ad_to_string(g_auto_ele_info->ele_str[UI_ELE_DA], cal_info_un.cur.base.da);
    
    auto_cal.state = AUTOCAL_WAIT_DATA;/* ����״̬Ϊ���ڵȴ�У׼���� */
    
	while(1)
	{
        test_led_flash();
        
        cal_info_un.cur.base.ad = sample_cur;
        
        dis_cal_ele(sample_vol, sample_cur, vol_ave);
        
        cur_ave = sample_cur * (*cal_info_un.cur.ad_i_k);
        transform_auto_cal_cur_to_string(g_auto_ele_info->ele_str[UI_ELE_CUR], cur_ave, mode, gear);
        
        transform_auto_cal_cur_to_string(g_auto_ele_info->ele_str[UI_ELE_MEASURE],
                                auto_measure * unit_k[gear], mode, gear);
        
        draw_auto_cal_ui_ele(&GUI_FontHZ_SimSun_12, g_auto_ele_info->auto_cal_ui_ele);
        
        OSTimeDlyHMSM(0,0,0,50);
        
		pkey_value = OSQPend(KeyboardQSem,10,&err);
        
		/* ���¸�λ��ʱ��Ҫ���� */
		if(0 == STOP_PIN)
		{
			stop_cal(mode);
			return;
		}
        
        if(auto_cal.state == AUTOCAL_WAIT_OUTPUT)
        {
            stop_cal(mode);
            return;
        }
        
        if(auto_cal.state == AUTOCAL_SAVEDATA)
        {
            cal_info_un.cur.base.measure = auto_measure * unit_k[gear];/* ����ֵ */
            cal_info_un.cur.count_ratio(&cal_info_un);
            cal_info_un.cur.save(mode);
            auto_cal.state = AUTOCAL_WAIT_STOP;
        }
        
		switch(*pkey_value)
		{
			case KEY_UP:
				break;
			case KEY_DOWN:
				break;
			case KEY_SHIFT:
				break;
			case KEY_F4:
                stop_cal(mode);
				return;
			case KEY_EXIT:
                stop_cal(mode);
				return;
			default:
				break;
		}
	}
}

/*
 * ��������auto_cal_vol_fun
 * ����  ���Զ�У׼��ѹ��ں���
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void auto_cal_vol_fun(void)
{
	uint8_t err = 0;
	uint16_t* pkey_value = NULL;
    int16_t num = cal_order[g_cur_cal_opt_num].num;
    uint8_t times = cal_order[g_cur_cal_opt_num].data;
    uint8_t mode =  cal_order[g_cur_cal_opt_num].mode;
    uint8_t gear = 0;
    
    /* ȷ����ѹ�� */
    if((num >= ACW_VOL_SEG_11 && num <= ACW_VOL_SEG_13)
        || (num >= DCW_VOL_SEG_11 && num <= DCW_VOL_SEG_13)
        || (num >= IR_VOL_SEG_11 && num <= IR_VOL_SEG_13))
    {
        vol_segment = 0;
    }
    else if((num >= ACW_VOL_SEG_21 && num <= ACW_VOL_SEG_23)
        || (num >= DCW_VOL_SEG_21 && num <= DCW_VOL_SEG_23)
        || (num >= IR_VOL_SEG_21 && num <= IR_VOL_SEG_23))
    {
        vol_segment = 1;
    }
    /* �޷�ȷ����ѹ����ֹУ׼ */
    else
    {
        return;
    }
    
    if(mode == ACW)
    {
        gear = AC_20mA;
        cur_set_meal  = get_cal_acw_vol_meal();
        init_acw_vol_cal_info(&cal_info_un.vol, times);
    }
    else if(mode == DCW)
    {
        gear = DC_20mA;
        cur_set_meal  = get_cal_dcw_vol_meal();
        init_dcw_vol_cal_info(&cal_info_un.vol, times);
    }
    else if(mode == IR)
    {
        cur_set_meal  = get_cal_ir_meal();
        init_ir_vol_cal_info(&cal_info_un.vol, times);
    }
    /* �޷�ȷ��ģʽ��ֹУ׼ */
    else
    {
        return;
    }
    
    auto_measure = 0;
	cur_vol = 5100;
    vol_ave = 0;
	startup_cal(mode, gear);
    
    DAC_Vol[0] = cal_info_un.vol.base[times].da;
    
    clear_ui_ele();
    transform_auto_cal_num_to_string(g_auto_ele_info->ele_str[UI_ELE_NUM], g_cur_cal_opt_num);
    strcpy((void*)g_auto_ele_info->ele_str[UI_ELE_MODE], (void*)mode_pool[0][mode]);
    strcpy((void*)g_auto_ele_info->ele_str[UI_ELE_OPT], (void*)opt_pool[VOL_OPT]);
    strcpy((void*)g_auto_ele_info->ele_str[UI_ELE_POINT], (void*)cal_info_un.vol.base[times].name);
    
    transform_auto_cal_da_ad_to_string(g_auto_ele_info->ele_str[UI_ELE_DA], cal_info_un.vol.base[times].da);
    
    auto_cal.state = AUTOCAL_WAIT_DATA;/* ����״̬Ϊ���ڵȴ�У׼���� */
    
    while(1)
    {
        test_led_flash();
        
        cal_info_un.vol.base[times].ad = sample_vol;
        
        transform_auto_cal_da_ad_to_string(g_auto_ele_info->ele_str[UI_ELE_AD_VOL], sample_vol);
        transform_auto_cal_da_ad_to_string(g_auto_ele_info->ele_str[UI_ELE_AD_CUR], sample_cur);
        
        transform_auto_cal_vol_to_string(g_auto_ele_info->ele_str[UI_ELE_MEASURE], auto_measure / 10.0);
        
        draw_auto_cal_ui_ele(&GUI_FontHZ_SimSun_12, g_auto_ele_info->auto_cal_ui_ele);
        
        OSTimeDlyHMSM(0,0,0,50);
        
		/* ���¸�λ��ʱ��Ҫ���� */
		if(0 == STOP_PIN)
		{
			stop_cal(mode);
			return;
		}
        
        if(auto_cal.state == AUTOCAL_WAIT_OUTPUT)
        {
            stop_cal(mode);
            return;
        }
        
        if(auto_cal.state == AUTOCAL_SAVEDATA)
        {
            cal_info_un.vol.base[times].measure = auto_measure / 10.0;/* ����ֵ ��λV */
            cal_info_un.vol.count_ratio(&cal_info_un);
            cal_info_un.vol.save(mode);
            auto_cal.state = AUTOCAL_WAIT_STOP;
        }
        
		pkey_value = OSQAccept(KeyboardQSem, &err);
        
		if(NULL == pkey_value)
        {
            continue;
        }
        
		switch(*pkey_value)
		{
			case KEY_F4:
                stop_cal(mode);
                return;
            case KEY_EXIT:
                stop_cal(mode);
                return;
        }
    }
}
void select_cur_range_pool(uint8_t mode)
{
	switch(mode)
	{
		case ACW:
			cur_rang = ac_cur_rang;
			break;
		case DCW:
			cur_rang = dc_cur_rang;
			break;
	}
}
/*
 * ��������auto_calibration
 * ����  ���Զ�У׼��ں���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void auto_calibration(void)
{
    uint8_t flag = 0;
	uint8_t err = 0;
	uint16_t* pkey_value = NULL;
    AUTO_ELE_INFO auto_cal_ui_ele;
    CAL_ORDER cal_order_pool[ORDER_SIZE];
    uint32_t ac_cur_rang_pool[]=
    {
        0,
        2   * 100,           //2uA
        20  * 100,           //20uA
        200 * 100,           //200uA
        2   * 1000 * 100,    //2mA
        10  * 1000 * 100,    //10mA
        20  * 1000 * 100,    //20mA
        50  * 1000 * 100,    //50mA
        100 * 1000 * 100,    //100mA
        200 * 1000 * 100,    //200mA
    };
    uint32_t dc_cur_rang_pool[]=
    {
        0,
        2   * 1000,           //2uA
        20  * 1000,           //20uA
        200 * 1000,           //200uA
        2   * 1000 * 1000,    //2mA
        10  * 1000 * 1000,    //10mA
        20  * 1000 * 1000,    //20mA
        50  * 1000 * 1000,    //50mA
        100 * 1000 * 1000,    //100mA
        200 * 1000 * 1000,    //200mA
    };
    
    memset(cal_order_pool, 0, sizeof(cal_order_pool));
    
    ac_cur_rang = ac_cur_rang_pool;//������λ��������
    dc_cur_rang = dc_cur_rang_pool;//������λ��������
    cal_order = cal_order_pool;//У׼˳���
    g_auto_ele_info = &auto_cal_ui_ele;/* �����ݸ���ȫ��ָ��Ϳ����������ط���������ֲ������� */
    init_ui_ele_info(g_auto_ele_info->auto_cal_ui_ele, g_auto_ele_info->ele_str);
    draw_auto_calibration_gui();
    clear_ui_ele();
    init_cal_order();
    dis_cal_point_base_info();
    
    while(1)
    {
		OSSemPend(ScheduleSem, 10, &err);
        
//         draw_frame_data(frame_buf);
        
        if(flag == 0)
        {
            dis_cal_point_base_info();
        }
        
        if(err == OS_ERR_NONE)
        {
            if(NULL != cal_order[g_cur_cal_opt_num].fun)
            {
                dis_cal_point_base_info();
                cal_order[g_cur_cal_opt_num].fun();
                auto_cal.state = AUTOCAL_CONNECTED;
                if(g_cur_cal_opt_num + 1 == auto_cal.cal_total_points)
                {
                    auto_cal_pass();
                    BUZZER = BUZZER_ON;
                    OSTimeDlyHMSM(0,0,0,500);
                    BUZZER = BUZZER_OFF;
                    flag = 1;
                }
//                 clear_ui_ele();
            }
        }
        
        if(auto_cal.state == AUTOCAL_OUTPUTING)
        {
            OSSemPost(ScheduleSem);
            if(flag == 1)
            {
                flag = 0;
                draw_auto_calibration_gui();
            }
        }
        
		if(g_exit)
		{
			return;
		}
		
		pkey_value = OSQAccept(KeyboardQSem, &err);
        
		if(NULL == pkey_value)
        {
            continue;
        }
        
		switch(*pkey_value)
		{
			case KEY_F4:
                return;
            case KEY_ENTER:
                OSSemPost(ScheduleSem);
                if(flag == 1)
                {
                    flag = 0;
                    draw_auto_calibration_gui();
                }
                break;
        }
    }
}



/******************* (C) COPYRIGHT 2016 ��ʢ���� *****END OF FILE*******************/
