/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�serve_test.c
 * ժ  Ҫ  �����Է����ӳ����ṩ���Գ�������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#define _SERVE_TEST_GLOBALS
#include    <string.h>
#include    <math.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"DAC_VREF.h"
#include	"SPI_CPLD.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_relay_motion.h"
#include	"cs99xx_type.h"
#include	"calibration.h"
#include 	"mb_server.h"
#include    "cs99xx_vref.h"
#include 	"MC14094.h"
#include	"RTC_Config.h"
#include    "stdio.h"
#include    "cs99xx_result.h"
#include 	"SysTemConfig.h"
#include    "test_com.h"
#include    "on_chip_flash_api.h"
#include    "cs99xx_err_log.h"
#include	"Timer_Config.h"
#include    "cs99xx_it.h"
#include    "cs99xx_buzzer.h"
#include    "cs99xx_plc.h"
#include    "irq.h"
#include    "dc_module.h"
#include    "cs99xx_collect.h"

/*
 * ��������record_exception_scene
 * ����  ����¼�쳣�ֳ�
 * ����  ����
 * ���  ����
 * ����  ����
 */
void record_exception_scene(void)
{
    err_vol_bak = g_test_data.vol_value;
    err_cur_bak = g_test_data.cur_value;
    err_res_bak = g_test_data.res_value;
    
    err_real_bak = g_test_data.real_value;
    err_cap_bak  = g_test_data.cap_value;
}
/*
 * ��������record_exception_scene
 * ����  ���ָ��쳣�ֳ�
 * ����  ����
 * ���  ����
 * ����  ����
 */
void recover_exception_scene(void)
{
    g_test_data.vol_value = err_vol_bak;
    g_test_data.cur_value = err_cur_bak;
    g_test_data.res_value = err_res_bak;
    g_test_data.real_value = err_real_bak;
    g_test_data.cap_value = err_cap_bak;
}

/*
 * ��������set_da_value
 * ����  �����ø�ѹ�����׼
 * ����  ����
 * ���  ����
 * ����  ����
 */
void set_da_value(void)
{
    DAC_Vol[0] =  (u16)(cur_vol * cur_dac_k + cur_dac_b);
}

/*
 * ��������close_hv
 * ����  �������ѹ�����׼
 * ����  ����
 * ���  ����
 * ����  ����
 */
void close_hv(void)
{
    disable_sample_task();/* �رղ������� */
    
    if(cur_mode != BBD)
    {
        DAC_Vol[DAC_OUTPUT_VREF] = 0;
    }
    else
    {
        DAC_Vol[DAC_OUTPUT_VREF] = 70;
    }
}

void cur_cylinder_ctrl_stop(void)
{
//    CYLINDER_CTRL(2000);
}

/*
 * �������� dis_exception_info
 * ����  ����ʾ�쳣��Ϣ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void dis_exception_log(void)
{
	uint8_t X0	= 0;
	uint8_t Y0	= 0;
	uint8_t X1	= (240-1);
	uint8_t Y1	= (64-1);
    
    char data_buf[15] = {0};
    char time_buf[15] = {0};
    char temp_buf[2][31];
    int32_t i = 0;
    uint8_t size = 0;
    uint16_t *pkey_value = NULL;
	INT8U err = 0;
    
    EXCEP_INFO_STR *p_e = (EXCEP_INFO_STR *)&on_chip_flash_page_buf[1];
    
    for(i = 0; i < 1024; i++)
    {
        on_chip_flash_page_buf[i] = ((uint16_t*)EXCEP_INFO_ADDR)[i];
    }
    
    i = 0;
l:
    size = strlen((p_e + i)->excep_buf);
    transform_data_to_str((p_e + i)->excep_time, data_buf);
    transform_time_to_str((p_e + i)->excep_time, time_buf);
    
    memset(temp_buf, 0 , sizeof temp_buf);
    strncpy(temp_buf[0], (p_e + i)->excep_buf, 30);
    
    if(size > 30)
    {
        strncpy(temp_buf[1] , (p_e + i)->excep_buf+30, 10);
    }
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
    
    GUI_DispDecAt(i+1, X0 + 3, Y0 + 3, 2);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"����:", 0+X0+35, Y0+4);
		GUI_DispStringAt((const char*)data_buf, 0+X0+35+6*5, Y0+4);
		GUI_DispStringAt((const char*)"ʱ��:", 0+X0+35, Y0+4+14);
		GUI_DispStringAt((const char*)time_buf, 0+X0+35+6*5, Y0+4+14);
		GUI_DispStringAt((const char*)temp_buf[0], 0+X0+4, Y0+4+14+18);//sys_flag.excep_buf
		GUI_DispStringAt((const char*)temp_buf[1], 0+X0+4, Y0+4+14+18+14);
	}
	else
	{
		GUI_DispStringAt((const char*)"DATE:"__DATE__, 0+X0+35, Y0+4);
		GUI_DispStringAt((const char*)data_buf, 0+X0+35+6*5, Y0+4);
		GUI_DispStringAt((const char*)"Time:"__TIME__, 0+X0+35, Y0+4+14);
		GUI_DispStringAt((const char*)time_buf, 0+X0+35+6*5, Y0+4+14);
		GUI_DispStringAt((const char*)temp_buf[0], 0+X0+4, Y0+4+14+18);
		GUI_DispStringAt((const char*)temp_buf[1], 0+X0+4, Y0+4+14+18+14);
	}
    
    GUI_DrawRect(X0, Y0, X1, Y1);
    
	LCD_REFRESH();
    
    while(1)
    {
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
        
		switch(*pkey_value)
		{
			case KEY_F1:
                if(--i < 0)
                {
                    i = MAX_EXCE_INFO_NUM - 1;
                }
                goto l;
			case KEY_F2:
                if(++i > MAX_EXCE_INFO_NUM - 1)
                {
                    i = 0;
                }
                goto l;
            case KEY_0 & KEY_1://�����¼
                memset(on_chip_flash_page_buf, 0 ,sizeof(on_chip_flash_page_buf));
                clear_excep_info();
                goto l;
            case KEY_BACK:
            case KEY_EXIT:
                goto l_end;
        }
    }
    
l_end:
	GUI_ClearRect (X0, Y0, X1, Y1);
}

/*
 * ��������set_offset_mode
 * ����  ������ƫ��ģʽ
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void set_offset_mode(void)
{
	uint8_t X0	= 40;
	uint8_t X1	= 170;
	uint8_t Y0	= 5;
	uint8_t Y1	= 57;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t n = 1;//g_offset_mode;//Ĭ�ϵ���ģʽ
	
    GUI_DrawRect(X0, Y0, X1, Y1);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine         (X0, Y0+17, X1, Y0+17);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//���÷�ת�ı�
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"ƫ�Ʋ���ģʽ", X0+30, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Offset mode", X0+40, Y0+3);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//���������ı�
    
    draw_short_cut_bm(__RB);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"����ģʽ", X0+15, Y0+30);
		GUI_DispStringAt((const char*)"����ģʽ", X0+15+60, Y0+30);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Single", X0+15, Y0+30);
		GUI_DispStringAt((const char*)"Series", X0+15+60, Y0+30);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y0+30+3);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+60, Y0+30+3);
	
	while(1)
	{
		switch(n)
		{
			case OFFSET_SIG:/* ����ģʽ */
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y0+30+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+60, Y0+30+3);
				cursor_pos(X0+5+1, Y0+30);
				break;
			case OFFSET_CON:/* ����ģʽ */
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,X0+5, Y0+30+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, X0+5+60, Y0+30+3);
				cursor_pos(X0+5+60+1, Y0+30);
				break;
			default:
				break;
		}
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		
		switch(*pkey_value)
		{
			case KEY_SHIFT:
                if(n == OFFSET_SIG)
                {
                    n = OFFSET_CON;
                }
                else
                {
                    n = OFFSET_SIG;
                }
				break;
			case KEY_ENTER:
				app_flag.offset_mode = n;
				GUI_ClearRect (40, 5, 170, 57);
				return;
			case KEY_BACK:
				GUI_ClearRect (40, 5, 170, 57);
				g_exit = 1;/* ����ͬʱ��һ���ص����Խ��� */
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			default:
				break;
		}
	}
}

/*
 * ��������test_unit_dis
 * ����  �����Խ��浥λ��ʾ�����ڵ�λ��ʾʹ�õ��������������岻һ�����Ҫ������ʾ
 * ����  ��mode ����ģʽ
 *         gear ��λ
 *         pNewFont  ����
 * ���  ����
 * ����  ����
 */
void test_unit_dis(uint8_t mode, uint8_t gear, const GUI_FONT * pNewFont)
{
	uint8_t O	= 13;
	uint8_t CW	= 9;	/* ���ַ���� */
    uint8_t unit_e_1_n = 0;
    uint8_t unit_e_2_n = 0;
    uint8_t unit_e_3_n = 0;
	
    if(pNewFont == &GUI_FontHZ_SimSun_12)
    {
        CW = 6;
        O = 12;
    }
    
    if(mode == CC)
    {
        unit_e_1_n  = NULL_U_NULL;
    }
	else
    if(mode != GR)
	{
        unit_e_1_n  = VOL_U_kV;
	}
	else
	{
        unit_e_1_n  = type_spe.gr_cur_unit;
	}
    
    /* ��ʵ������λ */
	if(mode == ACW)
	{
        /* �Ե�����ʽ��ʾ��ʵ���� */
        if(type_spe.real_cur_format == CAP_FORMAT)
        {
            unit_e_3_n  = CAP_U_uF;
        }
        else
        {
            if(cur_real_cur != 0)
            {
                unit_e_3_n  = ac_gear[gear].unit;
            }
            /* ��ʵ�����ر�ʱҪ�����λ */
            else
            {
                unit_e_3_n  = NULL_U_NULL;
            }
        }
	}
    else if(mode == BBD && OFFSET_BBD == 0)
    {
        unit_e_3_n  = CAP_U_nF;
    }
	/* �����ʵ������λ */
	else
	{
		unit_e_3_n  = NULL_U_NULL;
	}
	
	switch(mode)
	{
		case ACW:
            unit_e_2_n  = ac_gear[gear].unit;
			break;
		case DCW:
            unit_e_2_n  = dc_gear[gear].unit;
			break;
		case IR:
            if(OFFSET_BBD > 0)
            {
                unit_e_2_n  = NULL_U_NULL;
            }
            else
            {
                if(ERR_NUM == ERR_SHORT)
                {
                    unit_e_2_n  = RES_U_MOHM;
                }
                else
                {
                    unit_e_2_n  = ir_gear[gear].unit;
                }
            }
			break;
		case GR:
			if(GR_VOL_DROP_EN)
			{
				if(type_spe.gr_dis_opt & GR_DIS_VOL)
				{
					if(type_spe.gr_vol_drop_mode>0)
					{
						/* ��ǰ��λ��12v�� */
						if(cur_gear == DC_GR_12V
							|| cur_gear == DC_GR_12V_100mA
							|| cur_gear == DC_GR_12V_1000mA)
						{
                            unit_e_2_n  = VOL_U_V;
                        }
						else
						{
                            unit_e_2_n  = VOL_U_mV;
						}
					}
				}
				
				if(type_spe.gr_dis_opt & GR_DIS_RES)
				{
					/* ��λ */
					if(type_spe.gr_res_unit == RES_U_OHM)
					{
                        unit_e_2_n  = RES_U_OHM;
                    }
				}
			}
			else
			{
                unit_e_2_n  = RES_U_mOHM;
            }
			break;
		case BBD:
            unit_e_2_n  = CAP_U_nF;
            break;
		case CC:
            unit_e_2_n  = ac_gear[gear].unit;
            break;
	}
    
	GUI_ClearRect(test_ui_ele_set[TEST_ELE_1].u_x,
                  test_ui_ele_set[TEST_ELE_1].y,
                  test_ui_ele_set[TEST_ELE_1].u_x + CW * 2,
                  test_ui_ele_set[TEST_ELE_1].y + O);
    
	GUI_ClearRect(test_ui_ele_set[TEST_ELE_2].u_x,
                  test_ui_ele_set[TEST_ELE_2].y,
                  test_ui_ele_set[TEST_ELE_2].u_x + CW * 2,
                  test_ui_ele_set[TEST_ELE_2].y + O);
    
	GUI_ClearRect(test_ui_ele_set[TEST_ELE_3].u_x,
                  test_ui_ele_set[TEST_ELE_3].y,
                  test_ui_ele_set[TEST_ELE_3].u_x + CW * 2,
                  test_ui_ele_set[TEST_ELE_3].y + O);
    
    GUI_SetFont(pNewFont);
    
    GUI_DispStringAt((const char*)unit_pool[unit_e_1_n],
        test_ui_ele_set[TEST_ELE_1].u_x, test_ui_ele_set[TEST_ELE_1].y);
    
    GUI_DispStringAt((const char*)unit_pool[unit_e_2_n],
                    test_ui_ele_set[TEST_ELE_2].u_x, test_ui_ele_set[TEST_ELE_2].y);
    
    GUI_DispStringAt((const char*)unit_pool[unit_e_3_n],
            test_ui_ele_set[TEST_ELE_3].u_x, test_ui_ele_set[TEST_ELE_3].y);
}


/*
 * ��������set_offset_mode
 * ����  ������ƫ��ģʽ
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void serve_test_more(void)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	
	GUI_ClearRect (200+1, 0+0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 31-1);
	GUI_ClearRect (200+1, 31+1, 240-1, 47-1);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"ƫ��", 208, 2);
		GUI_DispStringAt((const char*)"��ֵ", 208, 18);
		GUI_DispStringAt((const char*)"���", 208, 34);
	}
	else
	{
		GUI_DispStringAt((const char*)"Offset", 204, 2);
		GUI_DispStringAt((const char*)"Get cs", 204, 18);
		GUI_DispStringAt((const char*)"Result", 204, 34);
	}
	LCD_REFRESH();
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		switch(*pkey_value)
		{
			case KEY_F1:/* ƫ�� */
				if(type_spe.offset_fun_en == OFFSET_ON)
				{
					set_offset_mode();
					if(g_return || g_exit)
					{
						break;
					}
					sys_offset(1);
					if(g_exit)
					{
						return;
					}
					g_return = 1;/* ͬʱ��1���ص����Խ��� */
					g_exit = 1;
				}
				break;
			case KEY_F2:/* BBD��� */
				if(type_spe.mode_en & __BBD)
				{
					sys_bbd(0);
					g_return = 1;/* ͬʱ��1���ص����Խ��� */
					g_exit = 1;
				}
				break;
			case KEY_F3: /* �����ʾ */
				sys_result(0);
				g_exit = 1;	/* Ϊ�˱�ʾΪ���Ӳ˵��а����˷��ؼ������Ǳ����˵� */
				break;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
        
		if(g_exit || g_return)
		{
			return;
		}
	}
}
/*
 * ��������serve_test_shortcut
 * ����  ������ƫ��ģʽ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void serve_test_shortcut(void)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t backup = 0;
    
	backup = g_cur_step->one_step.com.step;
l:	GUI_ClearRect (200+1, 0+0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 31-1);
	GUI_ClearRect (200+1, 31+1, 240-1, 47-1);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"�ڴ�", 208, 2);
		GUI_DispStringAt((const char*)"����", 208, 18);
		GUI_DispStringAt((const char*)"����", 208, 34);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Memory", 202, 2);
		GUI_DispStringAt((const char*)"Set", 212, 18);
		GUI_DispStringAt((const char*)"More", 208, 34);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	LCD_REFRESH();
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		switch(*pkey_value)
		{
			case KEY_TEST_MEM:
				sys_memory(1);/* �����ڴ������� 1��ʾ�Ӳ��Խ������� */
				Lcd24064_Clr();
				break;
			case KEY_TEST_SET:
				set_subtask(1);/* �������ý��� 1��ʾ�ǴӲ��Խ�����õ� */
				Lcd24064_Clr();
				break;
			case KEY_TEST_MORE:
				serve_test_more();
				if(g_return && g_exit)
				{/* ��g_return ��g_exitͬʱΪ1ʱ��ʾ����Ͳ�˵���ɲ����󷵻أ�Ӧ�ü������Ϸ��� ֮ǰ �轫g_exit���� */
					/* Ҳ�������жϷ���ֵ�ķ������ж� */
					Lcd24064_Clr();
					g_exit = 0;
					break;
				}
				if(g_return)
				{
					g_return = 0;
					goto l;
				}
				break;
			case KEY_BACK:
				g_return = 1;
				break;
			case KEY_EXIT:
				g_exit = 1;
				break;
		}
        
		if(g_exit || g_return)
		{
			if(g_cur_step->one_step.com.step != backup)
			{/* �����ǰ�����ı�����ô�ڷ���ʱӦ�ָ� */
				NODE_STEP *p;
				
				p = position_step(backup);/* ����Ҫ��p��ֵ�����ж� ������������ */
				if(p != NULL)
				{
					g_cur_step = p;
				}
			}
			return;
		}
	}
}




/*
 * ��������serve_count
 * ����  ��ϵͳ�¼�����
 * ����  ��n �¼�����
 * ���  ����
 * ����  ����
 */
void serve_count(const int8_t n)
{
	switch(n)
	{
		case KEY_START_COUNT:/* ��START������ */
			sys_flag.key_start_times++;
			break;
		case KEY_STOP_COUNT:/* ��STOP������ */
			sys_flag.key_stop_times++;
			break;
		case AMP_COUNT:/* ��AMP���� */
			sys_flag.amp_times++;
			break;
		case BOOT_COUNT:/* ��BOOT���� */
			sys_flag.boot_times++;
			break;
	}
    //ע�͵����湦�ܣ����Ƿ񻹻���� �����ɳɲ���1��෢�����ݶ�ʧ
//     save_sys_flag();
}



/*
 * ��������count_gear_vol
 * ����  ������IR�ĵ�ѹ��λ
 * ����  ��cur û����
 * ���  ����
 * ����  ����
 */
void count_gear_vol(uint32_t cur)
{
    switch(type_spe.ir_vol_range)
    {
        case IR_1kV:
        case IR_1_5kV:
        {
            if(cur_vol < 150)
            {
                cur_vol_gear = 0;
            }
            else if(cur_vol < 250)
            {
                cur_vol_gear = 1;
            }
            else if(cur_vol < 350)
            {
                cur_vol_gear = 2;
            }
            else if(cur_vol < 450)
            {
                cur_vol_gear = 3;
            }
            else if(cur_vol < 550)
            {
                cur_vol_gear = 4;
            }
            else if(cur_vol < 650)
            {
                cur_vol_gear = 5;
            }
            else if(cur_vol < 750)
            {
                cur_vol_gear = 6;
            }
            else if(cur_vol < 850)
            {
                cur_vol_gear = 7;
            }
            else if(cur_vol < 950)
            {
                cur_vol_gear = 8;
            }
            else if(cur_vol < 1050)
            {
                cur_vol_gear = 9;
            }
            else if(cur_vol < 1150)
            {
                cur_vol_gear = 10;
            }
            else if(cur_vol < 1250)
            {
                cur_vol_gear = 11;
            }
            else if(cur_vol < 1350)
            {
                cur_vol_gear = 12;
            }
            else if(cur_vol < 1450)
            {
                cur_vol_gear = 13;
            }
            else
            {
                cur_vol_gear = 14;
            }
            
            break;
        }
        case IR_2_5kV:
        {
            if(cur_auto == AUTO_SHIFT_OFF)
            {
                gear_up_limit = 0;
            }
            
            if(cur_vol < 1500)
            {
				if(cur_vol < 100)
				{
					cur_vol_gear = 0;
				}
				else
				{
					cur_vol_gear = ((cur_vol - 100) / 100 + 0) % 15;
				}
            }
            else
            {
                cur_vol_gear = 14 + (cur_vol - 1500) / 500;
            }
            break;
        }
        case IR_5kV:
        {
            if(cur_auto == AUTO_SHIFT_OFF)
            {
                gear_up_limit = 0;
            }
            
            if(type_spe.amp_type == PWM_AMP)
            {
				if(cur_vol < 500)
				{
					cur_vol_gear = 0;
				}
				else
				{
					cur_vol_gear = (cur_vol - 500) / 500;
				}
            }
            else
            {
                if(cur_vol < 1500)
                {
					if(cur_vol < 100)
					{
						cur_vol_gear = 0;
					}
					else
					{
						cur_vol_gear = (cur_vol - 100) / 100;
					}
                }
                else
                {
                    cur_vol_gear = 14 + (cur_vol - 1500) / 500;
                }
            }
            
            if(cur_vol_gear < 2)
            {
                cur_vol_gear = 2;
            }
            break;
        }
        case IR_10kV:
        {
            if(cur_auto == AUTO_SHIFT_OFF)
            {
                gear_up_limit = 0;
            }
            
            if(cur_vol < 5250)
            {
                cur_vol_gear = 0;
            }
            else if(cur_vol < 5750)
            {
                cur_vol_gear = 1;
            }
            else if(cur_vol < 6250)
            {
                cur_vol_gear = 2;
            }
            else if(cur_vol < 6750)
            {
                cur_vol_gear = 3;
            }
            else if(cur_vol < 7250)
            {
                cur_vol_gear = 4;
            }
            else if(cur_vol < 7750)
            {
                cur_vol_gear = 5;
            }
            else if(cur_vol < 8250)
            {
                cur_vol_gear = 6;
            }
            else if(cur_vol < 8750)
            {
                cur_vol_gear = 7;
            }
            else if(cur_vol < 9250)
            {
                cur_vol_gear = 8;
            }
            else
            {
                cur_vol_gear = 9;
            }
            
            break;
        }
    }
    
    if(cur_auto == AUTO_SHIFT_OFF)// && cur_ir_dis_gear == 1)
    {
        switch(cur_gear)
        {
            case IR_10MOHM:
                gear_up_limit = 10;//10 * 10;
                break;
            case IR_100MOHM:
                gear_up_limit = 100;//10 * 100;
                break;
            case IR_1GOHM:
                gear_up_limit = 1 * 1000; // 999M
                break;
            case IR_10GOHM:
                gear_up_limit = 10 * 1000;
                break;
            case IR_100GOHM:
                gear_up_limit = 1 * 1000 * 100;
                break;
        }
        
        if(gear_up_limit > IR_RES_H)
        {
            gear_up_limit = IR_RES_H;
        }
    }
}

/*
 * ��������transform_gr_cur_string
 * ����  ����GR����ת��Ϊ�ַ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void transform_gr_cur_string(void)
{
	switch(type_spe.gr_cur_unit)
	{
		case CUR_U_A:
            if(cur_ave < 100.00)
			{
                mysprintf((uint8_t*)output_buf, NULL, 2 + 150, cur_ave * 100);
            }
            else
            {
                mysprintf((uint8_t*)output_buf, NULL, 1 + 150, cur_ave * 10);
            }
			break;
		case CUR_U_mA:
            if(cur_ave < 1000)
			{
				mysprintf((uint8_t*)output_buf, NULL, 1 + 150, cur_ave*10);
			}
			else
			{
				mysprintf((uint8_t*)output_buf, NULL, 2 + 150, cur_ave*100);
			}
			break;
	}
}

/*
 * ��������transform_test_vol_string
 * ����  ����ACW DCW IR CC BBD ...��ѹת��Ϊ�ַ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void transform_test_vol_string(void)
{
	switch(cur_mode)
	{
		case ACW:
		{
			if(type_spe.acw_vol_range == ACW_20kV
				|| type_spe.acw_vol_range == ACW_30kV)
			{
				mysprintf((uint8_t*)output_buf, NULL, 2 + 150, vol_ave / 10.0);
			}
			else
			{
				if(vol_ave < 10000)
				{
					mysprintf((uint8_t*)output_buf, NULL, 3 + 150, vol_ave);
				}
				else
				{
					mysprintf((uint8_t*)output_buf, NULL, 2 + 150, vol_ave/10.0);
				}
			}
			break;
		}
		case DCW:
		{
			if(type_spe.dcw_vol_range == DCW_20kV)
			{
				mysprintf((uint8_t*)output_buf, NULL, 2 + 150, vol_ave / 10.0);
			}
			else
			{
				if(vol_ave < 10000)
				{
					mysprintf((uint8_t*)output_buf, NULL, 3 + 150, vol_ave);
				}
				else
				{
					mysprintf((uint8_t*)output_buf, NULL, 2 + 150, vol_ave/10.0);
				}
			}
			break;
		}
        case CC:
		case IR:
		case BBD:
		{
            if(vol_ave < 10000)
            {
                mysprintf((uint8_t*)output_buf, NULL, 3 + 150, vol_ave);
            }
            else
            {
                mysprintf((uint8_t*)output_buf, NULL, 2 + 150, vol_ave/10.0);
            }
			break;
		}
		case GR:
		{
			transform_gr_cur_string();
			break;
		}
	}
}

void transform_test_vol_to_str(TEST_DATA_STRUCT *test_data)
{
	switch(cur_mode)
	{
		case ACW:
		{
			if(type_spe.acw_vol_range == ACW_20kV
				|| type_spe.acw_vol_range == ACW_30kV)
			{
				mysprintf((uint8_t*)output_buf, NULL, 2 + 150, test_data->vol_value / 10.0);
			}
			else
			{
				if(vol_ave < 10000)
				{
					mysprintf((uint8_t*)output_buf, NULL, 3 + 150, test_data->vol_value);
				}
				else
				{
					mysprintf((uint8_t*)output_buf, NULL, 2 + 150, test_data->vol_value/10.0);
				}
			}
			break;
		}
		case DCW:
		{
			if(type_spe.dcw_vol_range == DCW_20kV)
			{
				mysprintf((uint8_t*)output_buf, NULL, 2 + 150, test_data->vol_value / 10.0);
			}
			else
			{
				if(vol_ave < 10000)
				{
					mysprintf((uint8_t*)output_buf, NULL, 3 + 150, test_data->vol_value);
				}
				else
				{
					mysprintf((uint8_t*)output_buf, NULL, 2 + 150, test_data->vol_value/10.0);
				}
			}
			break;
		}
        case CC:
		case IR:
		case BBD:
		{
            if(vol_ave < 10000)
            {
                mysprintf((uint8_t*)output_buf, NULL, 3 + 150, test_data->vol_value);
            }
            else
            {
                mysprintf((uint8_t*)output_buf, NULL, 2 + 150, test_data->vol_value/10.0);
            }
			break;
		}
		case GR:
		{
			break;
		}
	}
}

/*
 * ��������transform_test_time_string
 * ����  ��������ʱ��ת��Ϊ�ַ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void transform_test_time_string(uint16_t time)
{
    mysprintf((uint8_t*)time_buf, NULL, 1 + 150, time);
}
/*
 * ��������transform_test_time_string
 * ����  �������Ե��� ���� ���� ת��Ϊ�ַ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void transform_test_loop_string(void)
{
    if(OFFSET_BBD == 0)
    {
        real_buf[0] = 0;
    }
    else
    {
        strcpy((char*)real_buf, "-----");
    }
    
    switch(cur_mode)
    {
        case ACW:
        {
            mysprintf((uint8_t*)loop_buf, NULL, ac_gear[cur_gear].decs + 150, cur_ave + ROUND);
            if(cur_real_cur > 0)
            {
                mysprintf((uint8_t*)real_buf, NULL, ac_gear[cur_gear].decs + 150, real_ave + ROUND);
            }
            break;
        }
        case DCW:
        {
            mysprintf((uint8_t*)loop_buf, NULL, dc_gear[cur_gear].decs + 150, cur_ave + ROUND);
            break;
        }
        case IR:
        {
            if(OFFSET_BBD == 0)
            {
                transform_test_ir_res_loop();
            }
            else
            {
                strcpy((char*)loop_buf, "-----");
            }
            break;
        }
        case GR:
        {
            transform_test_gr_vol();
            break;
        }
        case BBD:
        {
            /* nA/(v*2*PI*Hz) = nF */
// 			cap_ave = cur_ave*1000/(vol_ave*2*PI*50);
            
            uint8_t temp = 0;
            uint8_t temp1 = 0;
            
            temp = COUNT_FLOAT_DECS(cap_ave);
            temp1 = COUNT_FLOAT_DECS(cur_cap);
            
			if(ERR_NUM != ERR_NONE)
			{
				sprintf((char*)loop_buf, "----");
			}
			else
			{
				mysprintf((uint8_t*)loop_buf, NULL, temp + 150 , cap_ave * pow(10, temp)  + ROUND);
			}
			
            if(OFFSET_BBD == 0)
            {
                mysprintf((uint8_t*)real_buf, NULL, temp1 + 150, cur_cap * pow(10, temp1) + ROUND);
            }
            break;
        }
        case CC:
        {
            mysprintf((uint8_t*)loop_buf, NULL, ac_gear[cur_gear].decs + 150, cur_ave + ROUND);
            break;
        }
    }
}
void transform_test_loop_to_str(TEST_DATA_STRUCT *test_data)
{
    if(OFFSET_BBD == 0)
    {
        real_buf[0] = 0;
    }
    else
    {
        strcpy((char*)real_buf, "-----");
    }
    
    switch(cur_mode)
    {
        case ACW:
        {
            mysprintf((uint8_t*)loop_buf, NULL, ac_gear[cur_gear].decs + 150, test_data->cur_value + ROUND);
            if(cur_real_cur > 0)
            {
                mysprintf((uint8_t*)real_buf, NULL, ac_gear[cur_gear].decs + 150, test_data->real_value + ROUND);
            }
            break;
        }
        case DCW:
        {
            mysprintf((uint8_t*)loop_buf, NULL, dc_gear[cur_gear].decs + 150, test_data->cur_value + ROUND);
            break;
        }
        case IR:
        {
            if(OFFSET_BBD == 0)
            {
                transform_test_ir_res_loop();
            }
            else
            {
                strcpy((char*)loop_buf, "-----");
            }
            break;
        }
        case GR:
        {
            transform_test_gr_vol();
            break;
        }
        case BBD:
        {
            /* nA/(v*2*PI*Hz) = nF */
// 			cap_ave = cur_ave*1000/(vol_ave*2*PI*50);
            
            uint8_t temp = 0;
            uint8_t temp1 = 0;
            
            temp = COUNT_FLOAT_DECS(test_data->cap_value);
            temp1 = COUNT_FLOAT_DECS(cur_cap);
            
			if(ERR_NUM != ERR_NONE)
			{
				sprintf((char*)loop_buf, "----");
			}
			else
			{
				mysprintf((uint8_t*)loop_buf, NULL, temp + 150 , test_data->cap_value * pow(10, temp)  + ROUND);
			}
			
            if(OFFSET_BBD == 0)
            {
                mysprintf((uint8_t*)real_buf, NULL, temp1 + 150, test_data->cap_value * pow(10, temp1) + ROUND);
            }
            break;
        }
        case CC:
        {
            mysprintf((uint8_t*)loop_buf, NULL, ac_gear[cur_gear].decs + 150, test_data->cur_value + ROUND);
            break;
        }
    }
}

void confirm_contact_segment_s(void)
{
	if(cur_vol <= type_spe.contact_segment_point)
	{
		contact_segment = 0;
	}
	else
	{
		contact_segment = 1;
	}
}
/*
 *name: confirm_vol_segment
 *fun : ȷ����ѹ��
 * 
 */
void confirm_vol_segment(void)
{
	//˫�����ѹ����׼���ֶ� ����ѡ����ݴ���ֽ��ѹ����
	if(type_spe.transformer_point == DOUBLE_POINT)
	{
		confirm_contact_segment_s();
		vol_segment = 1;
	}
	/* �������ѹ�� */
	else
	{
		if(g_cur_file->work_mode == G_MODE)
		{
			vol_segment = 1;
		}
		else
		{
			confirm_vol_segment_s();
		}
	}
}
/*
 * ��������load_cur_frequency
 * ����  �����㵱ǰ�����Ƶ��
 * ����  ��frequency Ƶ������ֵ
 * ���  ����
 * ����  �����ض�Ӧ��Ƶ��ֵ 50hz--400hz �� 40.0hz - 400.0hz
 */
uint16_t load_cur_frequency(uint16_t frequency)
{
    uint16_t freq_buf[10] = {50,50,60,100,150,200,250,300,350,400};
    uint16_t temp = 0;
    
    if(cur_mode == GR)
    {
        temp = freq_buf[frequency % 10];/* �����Ҳ� */
    }
    else
    {
        if(type_spe.hz_type == HZ_TYPE_GRADE)
        {
            temp = freq_buf[frequency % 10];/* �����Ҳ� */
        }
        else
        {
            temp = frequency/10.0;
        }
    }
    
    return temp;
}
uint8_t get_gr_gear(void)
{
    uint8_t gear = 0;
    
    if(GR_VOL_DROP_EN == DISABLE)
    {
        if(DC_GR_EN == ENABLE)
        {
            gear = 1;
        }
        else
        {
            gear = 0;
        }
    }
    else
    {
        if(type_spe.gr_cur_divide_gear_en == ENABLE)
        {
            if(cur_vol > 1000)
            {
                gear = DC_GR_12V_1000mA;
            }
            else
            {
                gear = DC_GR_12V_100mA;
            }
        }
        else
        {
            gear = DC_GR_12V;
        }
    }
    
    return gear;
}

void load_ratio(uint8_t mode)
{
    switch(mode)
    {
        case ACW:
            cur_dac_k = ratio_acw.dac_k[vol_segment];
            cur_dac_b = ratio_acw.dac_b[vol_segment];
            cur_adc_k = ratio_acw.adc_v_k[vol_segment];
            cur_adc_b = ratio_acw.adc_v_b[vol_segment];
            cur_arc_k = ratio_acw.arc_k;
			cur_adc_cur_k = ratio_acw.adc_cur_k[cur_gear];
            
            SAFE_FLOAT_VOLU_CAL(cur_dac_k, cur_dac_k);
            SAFE_FLOAT_VOLU_CAL(cur_dac_b, cur_dac_b);
            SAFE_FLOAT_VOLU_CAL(cur_adc_k, cur_adc_k);
            SAFE_FLOAT_VOLU_CAL(cur_adc_b, cur_adc_b);
            SAFE_FLOAT_VOLU_CAL(cur_arc_k, cur_arc_k);
            break;
        case DCW:
            cur_dac_k = ratio_dcw.dac_k[vol_segment];
            cur_dac_b = ratio_dcw.dac_b[vol_segment];
            cur_adc_k = ratio_dcw.adc_v_k[vol_segment];
            cur_adc_b = ratio_dcw.adc_v_b[vol_segment];
            cur_arc_k = ratio_dcw.arc_k;
			cur_adc_cur_k = ratio_dcw.adc_cur_k[cur_gear];
            
            SAFE_FLOAT_VOLU_CAL(cur_dac_k, cur_dac_k);
            SAFE_FLOAT_VOLU_CAL(cur_dac_b, cur_dac_b);
            SAFE_FLOAT_VOLU_CAL(cur_adc_k, cur_adc_k);
            SAFE_FLOAT_VOLU_CAL(cur_adc_b, cur_adc_b);
            SAFE_FLOAT_VOLU_CAL(cur_arc_k, cur_arc_k);
            break;
        case IR:
            cur_dac_k = ratio_ir.dac_k[vol_segment];
            cur_dac_b = ratio_ir.dac_b[vol_segment];
            cur_adc_k = ratio_ir.adc_v_k[vol_segment];
            cur_adc_b = ratio_ir.adc_v_b[vol_segment];
//            cur_res_k = ratio_ir.res_k[cur_gear-1][cur_vol_gear];
            
            SAFE_FLOAT_VOLU_CAL(cur_dac_k, cur_dac_k);
            SAFE_FLOAT_VOLU_CAL(cur_dac_b, cur_dac_b);
            SAFE_FLOAT_VOLU_CAL(cur_adc_k, cur_adc_k);
            SAFE_FLOAT_VOLU_CAL(cur_adc_b, cur_adc_b);
            SAFE_FLOAT_VOLU_CAL(cur_res_k, cur_res_k);
            break;
        case GR:
            cur_dac_k = ratio_gr.dac_k[cur_gear];
            cur_dac_b = ratio_gr.dac_b[cur_gear];
            cur_adc_k = ratio_gr.adc_i_k[cur_gear];
            cur_adc_b = ratio_gr.adc_i_b[cur_gear];
            
            SAFE_FLOAT_VOLU_CAL(cur_dac_k, cur_dac_k);
            SAFE_FLOAT_VOLU_CAL(cur_dac_b, cur_dac_b);
            SAFE_FLOAT_VOLU_CAL(cur_adc_k, cur_adc_k);
            SAFE_FLOAT_VOLU_CAL(cur_adc_b, cur_adc_b);
            break;
        case BBD:
            /* bbd��ϵ��ʹ��acw�� */
            cur_dac_k = ratio_acw.dac_k[vol_segment];
            cur_dac_b = ratio_acw.dac_b[vol_segment];
            cur_adc_k = ratio_acw.adc_v_k[vol_segment];
            cur_adc_b = ratio_acw.adc_v_b[vol_segment];
            
            SAFE_FLOAT_VOLU_CAL(cur_dac_k, cur_dac_k);
            SAFE_FLOAT_VOLU_CAL(cur_dac_b, cur_dac_b);
            SAFE_FLOAT_VOLU_CAL(cur_adc_k, cur_adc_k);
            SAFE_FLOAT_VOLU_CAL(cur_adc_b, cur_adc_b);
            break;
        case CC:
            /* ��Ϊ�ĵ���500v */
            cur_dac_k = 3.5;
            cur_dac_b = 42;
            cur_adc_k = 1.8;
            cur_adc_b = 0;
            
            if(cur_intensity == CC_CUR_HIGH)
            {
                cur_dac_k = 3.5;
                cur_dac_b = 42;
            }
            else if(cur_intensity == CC_CUR_MID)
            {
                cur_dac_k = 3;
                cur_dac_b = 42;
            }
            else if(cur_intensity == CC_CUR_LOW)
            {
                cur_dac_k = 2;
                cur_dac_b = 42;
            }
            else
            {
                cur_dac_k = 2;
                cur_dac_b = 42;
            }
        break;
    }
}

#include "dc_module_bridge.h"
void set_dc_gr_module_info(void)
{
    int32_t i = 0;
    uint16_t t_m_output[4] = {0,0,0,0};/* ÿ����Ա����ʾ����ģ����������ֵ */
    uint8_t n = 0;/* ѡ��ģ��Ĺ�����Ŀ */
    uint16_t temp_c = 0;
    
    if(cur_mode == GR)
    {
        if(DC_GR_EN)
        {
            /* �������õ��������������Ҫ����ģ�������� */
            if(cur_vol > ONE_DCGR_MAX_CUR)
            {
                n = sys_par.dc_module_using_info.count;
//                 if(cur_vol % ONE_DCGR_MAX_CUR != 0)
//                 {
//                     n = cur_vol / ONE_DCGR_MAX_CUR + 1;
//                 }
//                 else
//                 {
//                     n = cur_vol / ONE_DCGR_MAX_CUR;
//                 }
            }
            else
            {
                n = 1;
            }
            
            select_dc_gr_n_moduls(n);
            
            temp_c = cur_vol / n;
            
            /* ����ÿ��ģ����������ֵ */
            for(i = 0; i < n - 1; i++)
            {
                t_m_output[i] = temp_c;
            }
            
            t_m_output[i] = cur_vol - (temp_c * (n - 1));
            
            for(i = 0; i < n; i++)
            {
                set_dc_gr_module_output_da(sys_par.dc_module_using_info.addr[i], t_m_output[i]);
            }
            
            /* ���õ�ѹ��λ�����Է��� */
            set_dc_gr_vol_gear(cur_gear);
            set_dc_gr_test_method(cur_method);
            
            /* ������ֵ���͸�ģ�� */
            send_dc_module_cmd(DC_MODULE_CMD_SET_DATA, 0);
        }
    }
}
/*
 * ��������load_data
 * ����  �����ز�������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void load_data(void)
{
	UN_STRUCT *pun = &g_cur_step->one_step;
	
	cur_step = pun->com.step;/* ���µ�ǰ���� */
	cur_mode = pun->com.mode;/* ���µ�ǰ��ģʽ */
    
    /* ƫ�Ʋ����������ʱ */
    zeo_t = 0;
    
    dly_t = sys_par.output_delay;/* �����ʱ */
    
    result_r_queue[result_r_queue_h++] = cur_step;
	
    if(result_r_queue_h >= RES_QUEUE_MAX)
    {
        result_r_queue_h = 0;
    }
    
	switch(cur_mode)
	{
		case ACW:
        {
			tes_t = pun->acw.testing_time;
            ris_t = pun->acw.rise_time;
            fal_t = pun->acw.fall_time;
            int_t = pun->acw.interval_time;
            
			steps_con = pun->acw.steps_cont;
			steps_pass = pun->acw.steps_pass;
			
			cur_vol = pun->acw.testing_voltage;
			cur_gear = pun->acw.gear_i;
			cur_high = pun->acw.upper_limit;
			cur_low = pun->acw.lower_limit;
			cur_frequency_gear = pun->acw.output_freq;
			cur_real_cur = pun->acw.ac_real_cur; /* ��ǰ��ʵ���� */
			cur_arc_gear = pun->acw.arc_sur;/* �绡��� */
			cur_port = pun->acw.port;
            
            cur_frequency = load_cur_frequency(cur_frequency_gear);
            
			cur_offset_result = pun->acw.offset_result;
			/* ���ƫ�Ʋ��Գɹ� �� �ֶ�ƫ�ƴ� */
			if(pun->acw.offset_result == OFFSET_PASS
				|| g_custom_sys_par.offset_set_en)
			{
				cur_offset = pun->acw.offset_cur;/* ���������ƫ��ֵ */
				cur_real_offset = pun->acw.offset_real;/* ��ʵ����ƫ�� */
			}
			/* ���ƫ�Ʋ���δ����ʧ�� */
			else
			{
				cur_offset = 0;
				cur_real_offset = 0;
			}
            
			if(type_spe.acw_vol_range == ACW_20kV
				|| type_spe.acw_vol_range == ACW_30kV)
			{
				cur_vol *= 10;
			}
			
            vol_ave = cur_vol;
            /* ƫ�Ʋ��� */
            if(OFFSET_BBD > 0)
            {
                cur_ave = cur_offset;
                real_ave = cur_real_offset;
            }
            /* �������� */
            else
            {
                cur_ave = cur_high;
                real_ave = cur_real_cur;
            }
            
            g_dis_time = tes_t;
			cur_gear_comm = ac_gear[cur_gear].comm;
            
            confirm_vol_segment();/* ȷ����ѹ�� */
            g_test_data.vol_segment = vol_segment;
            g_test_data.vol_adc_k = ratio_acw.adc_v_k[vol_segment];
            g_test_data.vol_adc_b = ratio_acw.adc_v_b[vol_segment];
            g_test_data.cur_adc_k = ratio_acw.adc_cur_k[cur_gear];
            g_test_data.out_da_k = ratio_acw.dac_k[vol_segment];
            g_test_data.out_da_b = ratio_acw.dac_b[vol_segment];
			
			if(cur_adc_cur_k != 0)
			{
				g_ad_dog = (cur_high + 1) / cur_adc_cur_k;
			}
			break;
		}
		case DCW:
		{
			if(g_cur_file->work_mode == N_MODE)
			{
                if(type_spe.dcw_big_cap_en == DISABLE)
                {
                    one_t = pun->dcw.rise_time + zeo_t;
                    two_t = one_t + pun->dcw.testing_time;
                    thr_t = two_t + pun->dcw.fall_time;
                    for_t = thr_t + pun->dcw.interval_time;
                }
                /* ����ݲ��� */
                else
                {
                    one_t = pun->dcw.rise_time + zeo_t;
                    two_t = one_t + pun->dcw.stab_time;
                    thr_t = two_t + pun->dcw.testing_time;
                    for_t = thr_t + pun->dcw.fall_time;
                    fiv_t = for_t + pun->dcw.interval_time;
                }
                
				int_t = pun->dcw.interval_time;
                cha_t = 0;
			}
			else if(g_cur_file->work_mode == G_MODE)
			{
                if(cur_step == 1)
                {
                    one_t = pun->dcw.rise_time + zeo_t;
                }
                else
                {
                    one_t = 0 + zeo_t;
                }
				two_t = one_t + pun->dcw.interval_time;/* ����ʱ�� */
				thr_t = two_t + pun->dcw.testing_time;/* ���� */
                
                if(cur_step == g_cur_file->total)
                {
                    for_t = thr_t + pun->dcw.fall_time;/* �½�ʱ�� */
                }
                else
                {
                    for_t = thr_t + 0;
                }
                
                cha_t = pun->dcw.interval_time;
                int_t = cha_t;
			}
            
			tes_t = pun->dcw.testing_time;
            ris_t = pun->dcw.rise_time;
            fal_t = pun->dcw.fall_time;
            sta_t = pun->dcw.stab_time;
			
			steps_con = pun->dcw.steps_cont;
			steps_pass = pun->dcw.steps_pass;
			
			cur_vol  = pun->dcw.testing_voltage;
			cur_gear = pun->dcw.gear_i;
			cur_high = pun->dcw.upper_limit;
			cur_low  = pun->dcw.lower_limit;
			cur_charge_cur = pun->dcw.charge_cur;	/* ��ǰ������ */
			cur_delay_t    = pun->dcw.delay_time;
			cur_arc_gear   = pun->dcw.arc_sur;/* �绡��� */
			cur_port = pun->dcw.port;
            cur_output_impedan = pun->dcw.output_impedance;/* ����迹 */
			cur_frequency = 400;/* ѡ��400���� */
            
			cur_offset_result = pun->dcw.offset_result;
			
			/* ���ƫ�Ʋ��Գɹ� */
			if(pun->dcw.offset_result == OFFSET_PASS
				|| g_custom_sys_par.offset_set_en)
			{
				cur_offset = pun->dcw.offset_cur;/* ���������ƫ��ֵ */
			}
			/* ���ƫ�Ʋ���δ����ʧ�� */
			else
			{
				cur_offset = 0;
			}
			
			if(type_spe.dcw_vol_range == DCW_20kV)
			{
				cur_vol *= 10;
			}
			
			vol_ave = cur_vol;
            /* ƫ�Ʋ��� */
            if(OFFSET_BBD > 0)
            {
                cur_ave = cur_offset;
            }
            /* �������� */
            else
            {
                cur_ave = cur_high;
            }
            g_dis_time = tes_t;
            
			cur_gear_comm = dc_gear[cur_gear].comm;
            
            cur_real_gear_comm = cur_gear;
			
            confirm_vol_segment();/* ȷ����ѹ�� */
			
			if(cur_adc_cur_k != 0)
			{
				g_ad_dog = (cur_high + 1) / cur_adc_cur_k;
//				ADC_WatchdogConfig(g_ad_dog);
			}
			break;
		}
		case IR:
		{
			one_t = pun->ir.rise_time + zeo_t;
			two_t = one_t + pun->ir.testing_time;
			thr_t = two_t + 0;
			for_t = thr_t + pun->ir.interval_time;
            
			tes_t = pun->ir.testing_time;
            ris_t = pun->ir.rise_time;
            fal_t = 0;
            int_t = pun->ir.interval_time;
            cha_t = 0;
			
			steps_con = pun->ir.steps_cont;
			steps_pass = pun->ir.steps_pass;
			cur_frequency_gear = 9;/* ѡ��400���� */
			cur_frequency = 400;/* ѡ��400���� */
			cur_vol = pun->ir.testing_voltage;
			cur_auto = pun->ir.auto_shift_gears;
			cur_high = pun->ir.upper_limit;
			cur_low = pun->ir.lower_limit;
			cur_delay_t = pun->ir.delay_time;
			cur_port = pun->ir.port;
			cur_ir_rang_h = IR_RES_H;
            
            vol_ave = cur_vol;
            res_ave = cur_low;
            res_ave_comm = cur_low * 1000;
            g_dis_time = tes_t;
            cur_offset_result = 3;
            
            if(g_cur_type->num == CS9922BX_DZ)/* ����һ��С���� */
            {
                cur_high /= 10.0;
                cur_low /= 10.0;
                cur_ir_rang_h = IR_RES_H / 10.0;
            }
            
			/* ���������ĵ�λ */
			if(cur_low < 10)
			{
				cur_gear = 1;
			}
			else if(cur_low < 100)
			{
				cur_gear = 2;
			}
			else if(cur_low < 1000)
			{
				cur_gear = 3;
			}
			else if(cur_low < 10*1000)
			{
				cur_gear = 4;
			}
			else
			{
				cur_gear = 5;
			}
            
            /* 10kV�ĵ�λ��3��ʼ */
            if(type_spe.ir_vol_range == IR_10kV)
            {
                cur_gear_min = 3;
            }
            else
            {
                cur_gear_min = 1;
            }
            
            if(cur_gear < cur_gear_min)
            {
                cur_gear = cur_gear_min;
            }
            
            if(type_spe.ir_res_h <= 10*1000.0)
            {
                cur_gear_max = 4;
            }
            else
            {
                cur_gear_max = 5;
            }
            
            if(cur_gear > cur_gear_max)
            {
                cur_gear = cur_gear_max;
            }
            
            /* ����ѹС��200Vʱ���赵λ���ֵΪ4 */
            if(cur_vol < 200)
            {
                if(cur_gear_max == 5)
                {
                    cur_gear_max = 4;
                }
            }
            
			cur_gear_comm = cur_gear;
			cur_ir_dis_gear = cur_gear;/* ir��ʾ��λ ֻ���ڷ��Զ������õ� */
            cur_ad_vol = 0;
            g_ir_dly = 500;/* ����������Ҫ������ʱ */
			
            g_test_data.cur_gear = cur_gear;
            g_test_data.gear_max = cur_gear_max;
            g_test_data.gear_min = cur_gear_min;
            
			cur_ir_dis_gear = cur_gear;/* ir��ʾ��λ ֻ���ڷ��Զ������õ� */
            cur_ir_dis_gear_0 = cur_ir_dis_gear - 1;
            cur_ir_dis_gear_1 = cur_ir_dis_gear + 1;
            
            count_gear_vol(cur_vol);/* ȷ����ѹ�ĵ�λ */
            
            confirm_vol_segment();/* ȷ����ѹ�� */
			g_ir_gear_hold = 100;//sys_par.ir_gear_hold;
			
			if(tes_t >= 600)
			{
				app_flag.absorptance = 1;
			}
			else
			{
				app_flag.absorptance = 0;
			}
			
			ir_15s_res = 0;
			ir_60s_res = 0;
			ir_absorptance = 0;
			break;
		}
		case GR:
		{
			one_t = 0 + zeo_t;
			two_t = one_t + pun->gr.testing_time;
			thr_t = two_t + 0;
			for_t = thr_t + pun->gr.interval_time;
            
			tes_t = pun->gr.testing_time;
            ris_t = 0;
            fal_t = 0;
            int_t = pun->gr.interval_time;
            cha_t = 0;
			
			steps_con = pun->gr.steps_cont;
			steps_pass = pun->gr.steps_pass;
			
			cur_vol = pun->gr.testing_cur;
			cur_gear = pun->gr.voltage_gear;
			cur_high = pun->gr.upper_limit;
			cur_low = pun->gr.lower_limit;
			cur_frequency_gear = pun->gr.output_freq;
			cur_method = pun->gr.test_method;
            cur_frequency = load_cur_frequency(cur_frequency_gear);
			
			cur_offset_result = pun->gr.offset_result;
			/* ���ƫ�Ʋ��Գɹ� */
			if(pun->gr.offset_result == OFFSET_PASS
				|| g_custom_sys_par.offset_set_en)
			{
				cur_offset = pun->gr.offset_res;/* ���������ƫ��ֵ */
			}
			/* ���ƫ�Ʋ���δ����ʧ�� */
			else
			{
				cur_offset = 0;
			}
			
            cur_ave = cur_vol/100.0;
            /* ƫ�Ʋ��� */
            if(OFFSET_BBD > 0)
            {
                res_ave = cur_offset;
            }
            /* �������� */
            else
            {
                res_ave = cur_high;
            }
			
            res_ave_comm = cur_high;
            g_dis_time = tes_t;
            
			cur_gear = get_gr_gear();
            set_dc_gr_module_info();
			break;
		}
		case BBD:
		{
            //BBD�ڲ���GETCʱʹ��3s
            if(OFFSET_BBD)
            {
                one_t = 0;
                two_t = 30;
                thr_t = two_t;
                for_t = thr_t;
            }
            /* BBD��������ʱʹ��1s */
            else
            {
                one_t = 0;
                two_t = 10;
                thr_t = two_t;
                for_t = thr_t;
            }
            
			tes_t = two_t - one_t;
            ris_t = 0;
            fal_t = 0;
            int_t = 0;
            cha_t = 0;
			
			steps_con = pun->bbd.steps_cont;
			steps_pass = pun->bbd.steps_pass;
			
			cur_vol = pun->ir.testing_voltage;
			cur_cap = g_cur_step->one_step.bbd.cap_value;
            cur_getc_result = g_cur_step->one_step.bbd.get_cs_ok;
			cur_gear = g_cur_step->one_step.bbd.gear;
            cur_open_ratio = g_cur_step->one_step.bbd.open_ratio;
            cur_short_ratio = g_cur_step->one_step.bbd.short_ratio;
			cur_port = pun->bbd.port;
// 			cur_frequency = 400;/* 400Hz */
			cur_frequency_gear = g_cur_step->one_step.bbd.output_freq;/* 400Hz */
			
            cur_frequency = 100;/* 100Hz *///load_cur_frequency(cur_frequency_gear);
            vol_ave = cur_vol;
            confirm_vol_segment();/* ȷ����ѹ�� */
			bbd_load_gear();
			break;
		}
		case CC:
		{
            if(zeo_t <= 1)
            {
                zeo_t = 1;
            }
            
			if(g_cur_file->work_mode == N_MODE)
			{
				one_t = zeo_t + 0;
				two_t = one_t + 1;
				thr_t = two_t + 0;
				for_t = thr_t + 0;
                
                int_t = 0;
                cha_t = 0;
			}
            
            tes_t = two_t - one_t;
            ris_t = 0;
            fal_t = 0;
			
			steps_con = pun->cc.steps_cont;
			steps_pass = pun->cc.steps_pass;
			
			cur_vol = pun->cc.testing_voltage;
			cur_gear = pun->cc.gear_i;
			cur_high = pun->cc.upper_limit;
			cur_low = pun->cc.lower_limit;
			cur_frequency_gear = pun->cc.output_freq;
			cur_real_cur = pun->cc.ac_real_cur; /* ��ǰ��ʵ���� */
			cur_intensity = pun->cc.cur_intensity;/* ����ǿ�� */
			cur_port = pun->cc.port;
            
			cur_frequency = load_cur_frequency(cur_frequency_gear);
            
            vol_ave = cur_vol;
            cur_ave = cur_low;
            
			switch(cur_gear)
			{
				case AC_200uA:
					cur_gear_comm = AC_0_GEAR_COMM;
					break;
				case AC_2mA:
					cur_gear_comm = AC_1_GEAR_COMM;
					break;
				case AC_10mA:
				case AC_20mA:
					cur_gear_comm = AC_2_GEAR_COMM;
					break;
				case AC_50mA:
				case AC_100mA:
					cur_gear_comm = AC_3_GEAR_COMM;
					break;
			}
            
            if(cur_vol > 500)
            {
                vol_segment = 1;
            }
            else
            {
                vol_segment = 0;
            }
			break;
		}
	}
	
	cur_gear_bak = cur_gear;
    load_ratio(cur_mode);
    
    transform_test_vol_string();
    transform_test_loop_string();
    transform_test_time_string(tes_t);
}
///////////////////////////////////////////////////////////
//                                                       //
//				    ���Գ����ڲ��ӿ�                     //
//                                                       //
///////////////////////////////////////////////////////////
/*
 * �������� install_test_irq_fun
 * ����  ����ʼ������״̬������ Nģʽ Gģʽ ����� ����ÿ��ģʽ���в�ͬ�Ĳ������� �����Ҫ����
 *         ʵ�ֲ�ͬ��״̬����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void install_test_irq_fun(void)
{
    /* ����ݲ��� */
    if(cur_mode == DCW)
    {
        if(type_spe.dcw_big_cap_en == ENABLE)
        {
            test_irq_fun = test_big_cap_irq;
            cs99xx_test_fun = cs99xx_big_cap_test;
            return;
        }
    }
    
	/* Nģʽ */
	if(g_cur_file->work_mode == N_MODE)
	{
		test_irq_fun = test_irq;
		cs99xx_test_fun = cs99xx_test;
	}
	/* Gģʽ */
	else if(g_cur_file->work_mode == G_MODE)
	{
        test_irq_fun = test_g_irq;
		cs99xx_test_fun = cs99xx_g_test;
	}
    
    
    switch(cur_mode)
    {
        case ACW:
            break;
        case DCW:
            break;
        case IR:
            break;
        case GR:
            break;
    }
}

/*
 * ��������test_details
 * ����  ������ϸ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void test_details(void)
{
	switch(test_flag.gradation)
	{
		case STAGE_READY:/* �ک��׶� */
        {
			cur_status = ST_TESTING;
			if(test_flag.do_only_one)
			{
				test_flag.do_only_one = 0;
				test_ready();
			}
            break;
        }
		case STAGE_RISE:/* ��һ�׶� ��ѹ���� */
			cur_status = ST_VOL_RISE;
			HIGH = 1;
			
			test_dis();
			break;
		case STAGE_TEST:/* �ڶ��׶� ���ڲ��� */
			cur_status = ST_TESTING;
//            cur_cylinder_ctrl_status = CY_TESTING;
			test_dis();
			break;
		case STAGE_FALL:/* �����׶� ��ѹ�½� */
			cur_status = ST_VOL_FALL;
            count_fall_vol_step_value();/* �����ѹ�½�����ֵ */
			test_dis();
			break;
		case STAGE_INTER:/* ���Ľ׶� ����ȴ� */
			cur_status = ST_INTER_WAIT;
			WAIT = 1;
			
			if(TEST)
			{
				TEST = 0;
				HIGH = 0;
// 				test_reset();
                irq_stop_relay_motion();
				exit_test_relay_motion();/* �̵����������״̬ */
				
				if(!CUR_FAIL)
				{
					test_pass();
                    save_cur_result(&cur_result);
					if(steps_pass)/* �������pass�򿪾͵��� pass�� */
					{
						LED_PASS = LED_ON;/* ����pass�� */
					}
				}
				else
				{
					LED_FAIL = LED_ON;
				}
				
				if(fal_t > 0)
				{
                    vol_ave = 0;
                    cur_ave = 0;
                    test_flag.vol_change_flag = 1;
                    test_dis();
                    test_flag.vol_change_flag = 0;
				}
			}
			break;
	}
}

/*
 * ��������test_big_cap_details
 * ����  ������ϸ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void test_big_cap_details(void)
{
	switch(test_flag.gradation)
	{
		case STAGE_READY:/* ���� */
			if(test_flag.do_only_one)
			{
				test_flag.do_only_one = 0;
				test_ready();
			}
			break;
		case STAGE_CHARGE:/* ��� */
            test_vref(cur_high);	/* �������׼ */
            dcw_big_cap_start_charge();
			cur_status = ST_VOL_RISE;/* ��ѹ���� */
            
			HIGH = 1;
			test_dis();
			break;
        case STAGE_STAB:/* ��ѹ */
			test_dis();
            break;
		case STAGE_TEST:/* ���� */
            test_vref(cur_high);	/* �������׼ */
			cur_status = ST_TESTING;
//            cur_cylinder_ctrl_status = CY_TESTING;
            
			test_dis();
			break;
		case STAGE_FALL:/* �½� */
			cur_status = ST_VOL_FALL;
            count_rise_vol_step_value();/* �����ѹ��������ֵ */
			test_dis();
			break;
        case STAGE_DIS_CHA:/* �ŵ� */
            SHORT_INT(DISABLE);	/* �رն�·�ж� */
            GFI_INT(DISABLE);	/* �ر�GFI�ж� */
            ARC_INT(DISABLE);	/* �ر�ARC�ж� */
			test_dis();
            set_sample_rate(20);
            on_sample_task();
            resume_sample_task();
            dcw_big_cap_execute_dis_charge();
            break;
		case STAGE_INTER:/* ����ȴ� */
			cur_status = ST_INTER_WAIT;
			WAIT = 1;
			
			if(TEST)
			{
				TEST = 0;
				HIGH = 0;
// 				test_reset();
				exit_test_relay_motion();/* �̵����������״̬ */
				
				if(!CUR_FAIL)
				{
					test_pass();
                    
					if(steps_pass)/* �������pass�򿪾͵��� pass�� */
					{
						LED_PASS = LED_ON;/* ����pass�� */
					}
				}
				else
				{
					LED_FAIL = LED_ON;
				}
				
				if(fal_t > 0)
				{
                    vol_ave = 0;
                    cur_ave = 0;
                    test_flag.vol_change_flag = 1;
                    test_dis();
                    test_flag.vol_change_flag = 0;
				}
			}
			break;
	}
}

/*
 * ��������test_state_dis
 * ����  ����ʾ����ʱ��״̬
 * ����  ����
 * ���  ����
 * ����  ����
 */
void test_status_dis(void)
{
    dis_test_status(test_flag.dis_status, &GUI_FontHZ_SimSun_16);
    LCD_REFRESH();
}

/*
 * ��������cs99xx_test
 * ����  �����Գ���������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void cs99xx_test(void)
{
	/* ��ʾ����״̬ */
	test_status_dis();
	
	/* ����ϸ�� */
	test_details();
	
	/* ����ʱ�� */
	updata_time(U_TEST_TIME, g_dis_time);
}
/*
 * ��������cs99xx_big_cap_test
 * ����  �����Գ���������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void cs99xx_big_cap_test(void)
{
	/* ��ʾ����״̬ */
	test_status_dis();
	
	/* ����ϸ�� */
	test_big_cap_details();
	
	/* ����ʱ�� */
	updata_time(U_TEST_TIME, g_dis_time);
}

///////////////////////////////////////////////////////////
//                                                       //
//				        Gģʽ                            //
//                                                       //
///////////////////////////////////////////////////////////
/*
 * ��������count_vol_step_value
 * ����  �����㵱ǰ���Ļ����ѹ����ֵ
 * ����  ����
 * ���  ����
 * ����  ����
 */
// void count_vol_step_value1(void)
// {
// 	int16_t vol_1 = 0;/* ��ʼ��ѹֵ */
// 	int16_t vol_2 = 0;/* Ŀ���ѹֵ */
// 	uint16_t temp_inter_t = 0;/* ���ʱ�� */
// 	
// 	if(g_cur_step->next != NULL)
// 	{
// 		switch(cur_mode)
// 		{
// 			case ACW:
// 				vol_1        = g_cur_step->one_step.acw.testing_voltage;
// 				vol_2        = g_cur_step->next->one_step.acw.testing_voltage;
// 				temp_inter_t = g_cur_step->one_step.acw.interval_time;
// 				break;
// 			case DCW:
// 				vol_1        = g_cur_step->one_step.dcw.testing_voltage;
// 				vol_2        = g_cur_step->next->one_step.dcw.testing_voltage;
// 				temp_inter_t = g_cur_step->one_step.dcw.interval_time;
// 				break;
// 		}
// 	}
// 	else
// 	{
// 		switch(g_cur_step->one_step.com.mode)
// 		{
// 			case ACW:
// 				vol_1        = g_cur_step->one_step.acw.testing_voltage;
// 				temp_inter_t = g_cur_step->one_step.acw.interval_time;
// 				break;
// 			case DCW:
// 				vol_1        = g_cur_step->one_step.dcw.testing_voltage;
// 				temp_inter_t = g_cur_step->one_step.dcw.interval_time;
// 				break;
// 		}
// 		vol_2 = 0;
// 	}
//     
// 	vol_ch_base = vol_1;
//     vol_ch_target = vol_2;
// 	
// 	if(temp_inter_t != 0)
// 	{
// 		vol_ch_step = (float)(vol_2 - vol_1)/temp_inter_t;
// 	}
// }
/*
 * ��������count_vol_step_value
 * ����  ��������һ���Ļ����ѹ����ֵ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void count_vol_step_value(void)
{
	int16_t vol_1 = 0;/* ��ʼ��ѹֵ */
	int16_t vol_2 = 0;/* Ŀ���ѹֵ */
	uint16_t temp_inter_t = 0;/* ����ʱ�� */
	
    /* �������һ�� */
	if(g_cur_step->next != NULL)
	{
		switch(cur_mode)
		{
			case ACW:
				vol_1        = g_cur_step->one_step.acw.testing_voltage;
				vol_2        = g_cur_step->next->one_step.acw.testing_voltage;
				temp_inter_t = g_cur_step->next->one_step.acw.interval_time;
				break;
			case DCW:
				vol_1        = g_cur_step->one_step.dcw.testing_voltage;
				vol_2        = g_cur_step->next->one_step.dcw.testing_voltage;
				temp_inter_t = g_cur_step->next->one_step.dcw.interval_time;
				break;
		}
	}
    /* �����һ�� */
	else
	{
		switch(g_cur_step->one_step.com.mode)
		{
			case ACW:
				vol_1        = g_cur_step->one_step.acw.testing_voltage;
				temp_inter_t = g_cur_step->next->one_step.acw.fall_time;
				break;
			case DCW:
				vol_1        = g_cur_step->one_step.dcw.testing_voltage;
				temp_inter_t = g_cur_step->next->one_step.dcw.fall_time;
				break;
		}
		vol_2 = 0;
	}
    
	vol_ch_base = vol_1;
    vol_ch_target = vol_2;
    
	if(temp_inter_t != 0)
	{
		vol_ch_step = (float)(vol_2 - vol_1) / temp_inter_t;
	}
}
/*
 * ��������count_rise_vol_step_value
 * ����  �����㵱ǰ����ѹ�����ĵ�ѹ����ֵ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void count_rise_vol_step_value(void)
{
	vol_ch_base = 0;
    vol_ch_target = cur_vol;
    
	if(ris_t != 0)
	{
		vol_ch_step = (float)(vol_ch_target - vol_ch_base) / ris_t;
	}
}
/*
 * ��������count_vol_step_value
 * ����  �����㵱ǰ����ѹ�½��ĵ�ѹ����ֵ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void count_fall_vol_step_value(void)
{
	vol_ch_base = cur_vol;
    vol_ch_target = 0;
    
	if(fal_t != 0)
	{
		vol_ch_step = (float)(vol_ch_target - vol_ch_base) / fal_t;
	}
}

void count_vol_ch_step(void)
{
    test_flag.vol_rise_step_t = 0;
    
    switch(test_flag.gradation)
    {
        case STAGE_CHARGE://���
        case STAGE_RISE://����
            count_rise_vol_step_value();
            break;
        case STAGE_FALL://�½�
            count_fall_vol_step_value();
            break;
    }
}
/*
 * ��������vol_change
 * ����  ����ѹ����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void vol_change(void)
{
    float temp_vol = 0.0;
    
	temp_vol = vol_ch_base + vol_ch_step * g_dis_time;
    
	if(vol_ch_step > 0)
	{
		if(temp_vol > vol_ch_target)
		{
			temp_vol = vol_ch_target;
		}
	}
	else
	{
		if(temp_vol < vol_ch_target)
		{
			temp_vol = vol_ch_target;
		}
	}
	
	DAC_Vol[0] = temp_vol * cur_dac_k + cur_dac_b;
}

void vol_change_200hz(uint32_t time)
{
    float temp_vol = 0.0;
    
    float one_step_vol = g_test_data.vol_ch_step / 20.0;
    
	temp_vol = g_test_data.vol_ch_base + one_step_vol * time;
    
	if(g_test_data.vol_ch_step > 0)
	{
		if(temp_vol > g_test_data.vol_ch_target)
		{
			temp_vol = g_test_data.vol_ch_target;
		}
	}
	else
	{
		if(temp_vol < g_test_data.vol_ch_target)
		{
			temp_vol = g_test_data.vol_ch_target;
		}
	}
	
	DAC_Vol[0] = temp_vol * cur_dac_k + cur_dac_b;
}
void vol_change_1000hz(uint32_t time)
{
    float temp_vol = 0.0;
    
    float one_step_vol = vol_ch_step / 100.0;
    
	temp_vol = vol_ch_base + one_step_vol * time;
    
	if(vol_ch_step > 0)
	{
		if(temp_vol > vol_ch_target)
		{
			temp_vol = vol_ch_target;
		}
	}
	else
	{
		if(temp_vol < vol_ch_target)
		{
			temp_vol = vol_ch_target;
		}
	}
	
	DAC_Vol[0] = temp_vol * cur_dac_k + cur_dac_b;
}
/*
 * ��������test_g_details
 * ����  ��gģʽ����ϸ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void test_g_details(void)
{
	switch(test_flag.gradation)
	{
		case STAGE_READY:/* �ک��׶� */
        {
			if(test_flag.do_only_one)
			{
				test_flag.do_only_one = 0;
				test_ready();
			}
            break;
        }
		case STAGE_RISE:/* ��һ�׶� ��ѹ���� */
			cur_status = ST_VOL_RISE;
			HIGH = 1;
            count_rise_vol_step_value();/* �����ѹ��������ֵ */
			test_dis();
			break;
		case STAGE_CHANGE:/* �ڶ��׶� ��ѹ���� */
			cur_status = ST_INTER_WAIT;
//            cur_cylinder_ctrl_status = CY_TESTING;
            test_dis();
			break;
		case STAGE_TEST:/* �����׶�  ���ڲ��� */
			cur_status = ST_TESTING;
//            cur_cylinder_ctrl_status = CY_TESTING;
            
            test_dis();
			break;
		case STAGE_FALL:/* ��һ�׶� ��ѹ�½� */
			cur_status = ST_VOL_FALL;
            count_fall_vol_step_value();/* �����ѹ�½�����ֵ */
			test_dis();
			break;
	}
}

/*
 * ��������cs99xx_g_test
 * ����  ��Gģʽ���Գ���������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void cs99xx_g_test(void)
{
    /* ��ʾ����״̬ */
	test_status_dis();
	
	/* gģʽ����ϸ�� */
	test_g_details();
	
    /* ����ʱ�� */
	updata_time(U_TEST_TIME, g_dis_time);
}

///////////////////////////////////////////////////////////
//                                                       //
//				        ƫ�Ʋ���                         //
//                                                       //
///////////////////////////////////////////////////////////
/*
 * ��������offset_details
 * ����  ��offsetģʽ����ϸ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void offset_details(void)
{
	uint8_t err = 0;
	
	switch(test_flag.gradation)
	{
		case STAGE_READY:/* �ک��׶� ״̬׼�� */
            if(test_flag.do_only_one)
            {
                test_flag.do_only_one = 0;
                test_ready();
                
                g_test_time = 0;
                g_dis_time = 0;
                open_test_timer();/* ����ʱ�� */
                on_schedule_task();
            }
			break;
		case STAGE_RISE:/* ��һ�׶� ��ѹ���� */
            count_rise_vol_step_value();/* �����ѹ��������ֵ */
			offset_dis(cur_step%4==0?4:cur_step%4);
			break;
		case STAGE_TEST:/* �ڶ��׶� ������Խ׶� ���������acw�� test_acw_two */
			err = offset_dis(cur_step%4==0?4:cur_step%4);
			break;
		case STAGE_FALL:/* ��ѹ�½� */
            count_fall_vol_step_value();/* �����ѹ�½�����ֵ */
			offset_dis(cur_step%4==0?4:cur_step%4);
			break;
	}
	
	if(err != ERR_NONE)
	{
		exception_handling(err);
	}
}

static void offset_details_g(void)
{
	uint8_t err = 0;
	
	switch(test_flag.gradation)
	{
		case STAGE_READY:/* �ک��׶� ״̬׼�� */
			
            if(test_flag.do_only_one)
            {
                test_flag.do_only_one = 0;
                test_ready();
                
                g_test_time = 0;
                g_dis_time = 0;
                open_test_timer();/* ����ʱ�� */
                on_schedule_task();
            }
			break;
		case STAGE_RISE:/* ��һ�׶� ��ѹ���� */
            count_rise_vol_step_value();/* �����ѹ��������ֵ */
			offset_dis(cur_step%4==0?4:cur_step%4);
			break;
		case STAGE_TEST:/* �ڶ��׶� ������Խ׶� ���������acw�� test_acw_two */
			err = offset_dis(cur_step%4==0?4:cur_step%4);
			break;
		case STAGE_FALL:/* ��ѹ�½� */
            count_fall_vol_step_value();/* �����ѹ�½�����ֵ */
			vol_change();
			offset_dis(cur_step%4==0?4:cur_step%4);
			break;
	}
	
	if(err != ERR_NONE)
	{
		exception_handling(err);
	}
}
/*
 * ��������offset_state_dis
 * ����  ��offset����״̬��ʾ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void offset_state_dis(void)
{
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)status_str[test_flag.dis_status][!sys_par.language], 150, 51);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	LCD_REFRESH();
}
/*
 * ��������offset_test
 * ����  ��offset��������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void offset_test(void)
{
	/* ��ʾ״̬ */
	offset_state_dis();
	
	/* ����ϸ�� */
    if(g_cur_file->work_mode == G_MODE)
    {
        offset_details_g();
    }
    else
    {
        offset_details();
    }
    
    /* ˢ��ʱ�� */
	updata_time(U_OFFSET_TIME, g_dis_time);
}
//////////////////////////////////////////////////////////////////////////
void clear_test_flag(void)
{
	memset((void*)&test_flag, 0, sizeof test_flag);/* ��ճ������еı�־λ */
}
/*
 * ��������clear_par
 * ����  �����ȫ�ֲ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void clear_par(void)
{
	vol_ave = 0;
	cur_ave = 0;
	res_ave = 0;
	real_ave = 0;
	
    err_vol_bak = 0;
    err_cur_bak = 0;
    err_res_bak = 0;
    
    clear_test_flag();
	memset(&f_buf, 0, sizeof f_buf);/* ����˲������� */
    
    sample_vol = 0;
    sample_cur = 0;
}

void set_sample_rate(uint32_t rate)
{
    if(rate == 0)
    {
        rate = ~0;
        app_flag.sample_cycle = rate;
    }
    else if(rate > 1000)
    {
        app_flag.sample_cycle = 1000;
    }
    else
    {
        app_flag.sample_cycle = 1000 / rate; /* ��λms */
    }
    
	app_flag.sample_rate = rate;
}

void wait_dc_gr_send_task_idle(void)
{
	uint32_t time_out = 0;
	
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,10);
		if(app_flag.dc_gr_task_send_idle)
		{
			if(time_out++ > 10)
			{
				break;
			}
		}
		else
		{
			time_out = 0;
		}
	}
}
/*
 * ��������startup
 * ����  ����������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void startup(void)
{
	int8_t fail_bak = FAIL;/* ���ݲ���ʧ�ܱ�־ */
    uint8_t err = 0;
	
	clear_par();/* ���ȫ�ֱ�����ֵ */
    
    if(cur_step != 1)
    {
        FAIL = fail_bak;/* �ָ�����ʧ�ܱ�־ */
    }
    
	LED_ALL = LED_OFF;
	BUZZER = BUZZER_OFF;
	g_test_time = 0;/* ��ն�ʱ�� */
	g_dis_time = 0;
    g_plc_dly = 0;
	
	off_keyboard();/* �رռ��� */
	save_result(&cur_result, INIT_RESULT);/* ��ʼ���ṹ������Ϣ */
    OSMutexPend(MdbusMutex, 0, &err);
    clear_mdbus_send_queue();/* ���modbus���Ͷ��� */
	clear_slither_data();/* ��ջ���ƽ������������ */
    OSMutexPost(MdbusMutex);
	cpld_write(CPLD_START);/* ����CPLD���� */
	
	test_flag.do_only_one = 1;/* ֻ��1�� test_ready */
    test_flag.vol_change_flag = 1;
    test_flag.allow_dc_gr_fetch = 0;
	test_flag.uart_next = 0;
	cur_status = ST_TESTING;
	
	/*******************************************************/
    
	if(cur_mode == GR && DC_GR_EN)
	{
        on_sample_task();/* ������������ */
        clear_dc_gr_data();
		set_sample_rate(0);
        on_dc_gr_control_relay();
		wait_dc_gr_send_task_idle();//��ʱ�ܿ��̵������
	}
	else
	{
        on_sample_task();/* ������������ */
		set_sample_rate(100);
        resume_sample_task();
	}
    
    if(cur_mode == DCW)
    {
        if(type_spe.dcw_big_cap_en == ENABLE)
        {
            dcw_big_cap_stop_dis_charge();
        }
    }
    
    exit_sw();
    plc_signal_cmd(PLC_SIG_TESTING);/* ��PLC���Ͳ����ź� */
    test_flag.judge_err_en = 1;
}

/*
 * ��������stop_test
 * ����  ��ֹͣ���� �����ǲ��Գɹ�����ʧ�� ��Ҫֹͣʱ��������������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void stop_test(void)
{
    test_flag.judge_err_en = 0;
    close_test_timer();
	close_hv();
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);/* �嶨ʱ���жϱ�־ */
	
    exit_test_relay_motion();/* �̵����������״̬ */
    
	/* ���ж� */
	RTC_INI(ENABLE);
// 	STOP_INT(DISABLE);	/* �رո�λ�ж� */
	SHORT_INT(DISABLE);	/* �رն�·�ж� */
	GFI_INT(DISABLE);	/* �ر�GFI�ж� */
	ARC_INT(DISABLE);	/* �ر�ARC�ж� */
	
	g_test_time = 0;/* ���ʱ��Ƭ */
	GR_OVER = 1;
	
	TEST = 0;	/* ���Ա�־��0 */
	LED_TEST = 0;/* �ز��Ե� */
	HIGH = 0; 	/* ��ѹ��־ */
	
	disable_sample_task();/* ֹͣ�������� */
	
	cpld_write(CPLD_STOP);/* ֹͣCPLD���� */
	
	if(!(sys_par.fail_mode == FAIL_MODE_CON))
	{
		if(g_comm_status == LOCAL_COMM)
		{
			on_keyboard();/* �򿪼��� */
		}
	}
}

/*
 * ��������serve_test_poll_task
 * ����  ��Ϊϵͳ�ṩ��ʱ����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void serve_test_poll_task(void)
{
    static uint32_t count = 0;
    
    /* IR������ʱ */
	if(g_ir_dly)
	{
		g_ir_dly--;
	}
    
    buzzer_delay_off();/* ��������ʱ�ر� */
    buzzer_flicker();/* ���������� */
    
    /* ���׿�����ʱ */
    if(g_cylinder_ctrl_t)
    {
        if(0 == --g_cylinder_ctrl_t)
        {
//            cur_cylinder_ctrl_status = CY_WAIT;
        }
    }
    
	/* PLC��ʱ�ر� */
    plc_delay_off();
    
    /* ���gfi����ʱ���� */
    gfi_cycle_clear_count();
    
    /* ��·�ж�����ѯ�ķ�ʽ�ж� */
    if(cur_mode == IR)
    {
        if(!type_spe.ir_short_int_en && HIGH)
        {
            if(SHORT_PIN == RESET)
            {
                if(SHORT_PIN == RESET)
                {
                    if(SHORT_PIN == RESET)
                    {
                        short_irq();
                    }
                }
            }
        }
    }
    
    if(++count >= 1000)
    {
        test_flag.record_per_second_re = test_flag.re_times_per_second;
        test_flag.re_times_per_second = 0;
        count = 0;
        
        test_flag.err_amp_timer++;//���ű�����ʱ��
    }
}
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/

