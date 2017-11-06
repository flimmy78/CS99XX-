/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_exception.c
 * ժ  Ҫ  ��Ϊͨ�ų����ṩ������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#include    "cs99xx_exception.h"
#include	<string.h>
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include	"cs99xx_it.h"
#include 	"mb_server.h"
#include    "test_com.h"
#include    "cs99xx_result.h"
#include 	"cs99xx_plc.h"

static void dis_exception_str(const uint8_t err_num, const GUI_FONT * pNewFont)
{
    uint8_t exc_index = 0;
	
    GUI_SetFont(pNewFont);
	
	
    /* ���� */
	if(sys_par.language == CHINESE)
	{
		exc_index = 0;
	}
    /* Ӣ�� */
	else
	{
		if(OFFSET_BBD)
		{
            exc_index = 2;
		}
		else
		{
            exc_index = 1;
		}
	}
    
	GUI_ClearRect(test_ui_ele_set[TEST_ELE_5].x,
                  test_ui_ele_set[TEST_ELE_5].y,
                  test_ui_ele_set[TEST_ELE_5].u_x,// + test_ui_ele_set[TEST_ELE_5].cw,
                  test_ui_ele_set[TEST_ELE_5].y
                + test_ui_ele_set[TEST_ELE_5].o_y);
    
    GUI_DispStringAt_BTP_STR_(except_buf[err_num][exc_index],
                    test_ui_ele_set[TEST_ELE_5].x,
                    test_ui_ele_set[TEST_ELE_5].u_x,
                    test_ui_ele_set[TEST_ELE_5].y,
                    test_ui_ele_set[TEST_ELE_5].cw);
	LCD_REFRESH();
}

// static void dis_exception_str(const uint8_t err_num)
// {
// 	uint8_t x1 = 0;
// 	uint8_t x2 = 0;
// 	uint8_t y = 0;
//     uint8_t o = 0;
//     uint8_t cw = 0;/* �ַ�ƽ����� ����Ϊһ�� */
//     uint8_t exc_index = 0;
// 	
//     /* ƫ�Ʋ����� */
//     if(OFFSET_BBD)
//     {
//         x1 = 145+1;
//         x2 = 200-1;
//         y  = 51;
//         o  = 12;
//         cw = 6;
//         GUI_SetFont(&GUI_FontHZ_SimSun_12);
//     }
//     else
//     {
//         o  = 16;
//         x1 = GUI_GEN_STATUS_X1;
//         x2 = GUI_GEN_STATUS_X2;
//         y  = GUI_GEN_STATUS_Y;
// 		GUI_SetFont(&GUI_FontHZ_SimSun_16);//С��
//     }
//     
//     /* ���� */
// 	if(sys_par.language == CHINESE)
// 	{
// 		/* �������� */
// 		if(!OFFSET_BBD)
// 		{
//             cw = GUI_GEN_STATUS_CW;
// 		}
// 	}
//     /* Ӣ�� */
// 	else
// 	{
// 		if(OFFSET_BBD)
// 		{
//             exc_index = 2;
// 		}
// 		else
// 		{
//             cw = 7;
//             exc_index = 1;
// 		}
// 	}
//     
//     if(cur_mode == DCW)
//     {
//         if(type_spe.dcw_big_cap_en == ENABLE)
//         {
//             x1 = 100;
//             x2 = 87+12*8;
//             y  = 42;
//             o  = 20;
//         }
//     }
// 	
//     GUI_ClearRect (x1, y, x2, y+o);
//     
//     /* ��ʾ�������Ϣ */
//     GUI_DispStringAt_BTP_STR_(except_buf[err_num][exc_index], x1, x2, y, cw);
//     
// 	LCD_REFRESH();
// }

static void fail_mode_stop_dispose(void)
{
    LED_TEST = LED_OFF;
	LED_FAIL = LED_ON;/* ����FAIL�� */
    
    if(BUZZER_EN)
    {
        BUZZER_SOUND(0);/* ����ϵͳ���ؿ��� */
    }
    
	if(g_cur_step->next == NULL)
	{
        OVER = 1;
        
		/* �б���ʾ�� */
		if(sys_par.is_table_dis)
		{
			res_list_dis();
			gui_draw_test();
		}
	}
	
    while(1)
    {
		if(RESET == STOP_PIN || TERMINATE)/* ���¸�λ�������˳� */
		{
            app_flag.buzzer_flicker_en = 0;
			LED_FAIL = 0;
			STOP = 1;
			g_return = 1;
			LED_FAIL = LED_OFF;/* Ϩ��FAIL�� */
            if(BUZZER_EN)
            {
                BUZZER = BUZZER_OFF;/* ����ϵͳ���ؿ��� */
            }
			return;
		}
        
        OSTimeDlyHMSM(0,0,0,10);
    }
}

static void fail_mode_halt_dispose(void)
{
    fail_mode_stop_dispose();
    
    /* �����ǰ���������һ���´������ͻ����һ����ʼ */
    if(cur_step != g_cur_file->total)
    {
        NEXT  = 1;
    }
}

static void fail_mode_con_dispose(void)
{
    if(g_cur_step->next == NULL)
    {
        OVER = 1;
        if(sys_par.plc_signal != EACH_STEP)
        {
            return;/* ����ȥ��ʾ����ʧ�� */
        }
    }
    
	/* ʧ�ܼ������� ��ǰ�����˲������� ����ģʽΪN */
	if(g_cur_step->next != NULL && steps_con && (g_cur_file->work_mode == N_MODE))
	{
        uint32_t time_out = 0;
        
        BUZZER_SOUND(300);/* ����300ms */
        LED_FAIL = LED_ON;
        LED_TEST = LED_OFF;
        
        while(++time_out < 50)
        {
            OSTimeDlyHMSM(0,0,0,10);
            
            if(RESET == STOP_PIN)
            {
                TERMINATE = 1;
                return;
            }
        }
        
        g_test_data.gradation = STAGE_FAIL_CONT;
	}
    /* ֹͣ���� */
    else
    {
        fail_mode_stop_dispose();
    }
}
static void fail_mode_restart_dispose(void)
{
    LED_TEST = LED_OFF;
	LED_FAIL = LED_ON;/* ����FAIL�� */
    if(BUZZER_EN)
    {
        BUZZER_SOUND(0);/* ����ϵͳ���ؿ��� */
    }
    
	if(g_cur_step->next == NULL)
	{
		/* �б���ʾ�� */
		if(sys_par.is_table_dis)
		{
			res_list_dis();
			gui_draw_test();
		}
	}
	
    while(1)
    {
        if(RESET == START_PIN || test_flag.start_comm)
        {
            test_flag.start_comm = 0;
            FAIL_RE_START = FAIL_RE_START_FIRST;
            return;
        }
        if(RESET == STOP_PIN || TERMINATE)
        {
            STOP = 1;
            return;
        }
        
        OSTimeDlyHMSM(0,0,0,10);
    }
}
static void fail_mode_next_dispose(void)
{
	LED_FAIL = LED_ON;/* ����FAIL�� */
    if(BUZZER_EN)
    {
        BUZZER_SOUND(0);/* ����ϵͳ���ؿ��� */
    }
    
	if(g_cur_step->next == NULL)
	{
		/* �б���ʾ�� */
		if(sys_par.is_table_dis)
		{
			res_list_dis();
			gui_draw_test();
		}
	}
	
    while(1)
    {
        if(RESET == START_PIN || test_flag.start_comm)
        {
            test_flag.start_comm = 0;
            FAIL_RE_START = FAIL_RE_START_NEXT;
			BUZZER = BUZZER_OFF;
            return;
        }
        if(RESET == STOP_PIN || TERMINATE)
        {
            STOP = 1;
			BUZZER = BUZZER_OFF;
            return;
        }
        
        OSTimeDlyHMSM(0,0,0,10);
    }
}

static void fail_mode_fpdfc_dispose(void)
{
    fail_mode_con_dispose();
}

/*
 * ��������exception_handling
 * ����  ���쳣������
 * ����  ��errnum ������
 * ���  ����
 * ����  ����
 */
void exception_handling(int8_t errnum)
{
	int8_t l_err_num = errnum;
    
    close_test_timer();/* �ض�ʱ�� */
	irq_stop_relay_motion();
    
	if(STOP)
	{
		return;
	}
    
    exit_test_relay_motion();
    
	CUR_FAIL = 1;/* ��ǰ������ʧ�� */
    FAIL = 1;/* ����ʧ�� */
	cur_result.err_num = test_flag.dis_status;
    updata_result(cur_mode);
    save_cur_result(&cur_result);/* ������ */
	cur_status = l_err_num;/* ����λ�����Ĵ���״̬ */
    
    test_flag.test_led_flag = 0;
	test_fail();
    
	if(OFFSET_BBD)
	{
		dis_exception_str(test_flag.dis_status, &GUI_FontHZ_SimSun_12);
	}
	else
	{
		dis_exception_str(test_flag.dis_status, &GUI_FontHZ_SimSun_16);
	}
    
    if(cur_mode == DCW)
    {
        if(type_spe.dcw_big_cap_en == ENABLE)
        {
            STOP = 1;
            /* ���������� */
            if(BUZZER_EN)
            {
                BUZZER = BUZZER_ON;/* ����ϵͳ���ؿ��� */
            }
            
            ERR_NUM = ERR_NONE;
            ERR_NUM_OVER = ERR_NONE;
            return;
        }
    }
    
	if(OFFSET_BBD)
	{
		goto l;
	}
    
	if(cur_mode == BBD)
	{
		if(errnum == ERR_SHORT)
		{
			cur_plc_err_st = PLC_HIGH_FAIL;
		}
		else if(errnum == ERR_OPEN)
		{
			cur_plc_err_st = PLC_LOW_FAIL;
		}
	}
	else
	{
		if(errnum == ERR_HIGH)
		{
			cur_plc_err_st = PLC_HIGH_FAIL;
		}
		else if(errnum == ERR_LOW)
		{
			cur_plc_err_st = PLC_LOW_FAIL;
		}
	}
	
    if(cur_mode == CC)
    {
        if(FAIL_MODE_CON == sys_par.fail_mode)
        {
            if(l_err_num == ERR_OPEN)
            {
                app_flag.buzzer_flicker_en = 1;
            }
            goto l;
        }
        else if(FAIL_MODE_CON != sys_par.fail_mode && FAIL_MODE_FPDFC != sys_par.fail_mode)
        {
            if(l_err_num == ERR_OPEN)
            {
                app_flag.buzzer_flicker_en = 1;
            }
        }
    }
	
    switch(sys_par.fail_mode)
    {
        case FAIL_MODE_STOP:
    l:
            fail_mode_stop_dispose();
            break;
        case FAIL_MODE_HALT:
            if(l_err_num == ERR_OPEN)
            {
                app_flag.buzzer_flicker_en = 1;
            }
            fail_mode_halt_dispose();
            break;
        case FAIL_MODE_CON:
            fail_mode_con_dispose();
            break;
        case FAIL_MODE_RESTART:
            fail_mode_restart_dispose();
            break;
        case FAIL_MODE_NEXT:
            fail_mode_next_dispose();
            break;
        case FAIL_MODE_FPDFC:
            fail_mode_fpdfc_dispose();
            break;
    }
    
    app_flag.buzzer_flicker_en = 0;
	ERR_NUM = ERR_NONE;
    ERR_NUM_OVER = ERR_NONE;
}

void exception_handlingx(int8_t status)
{
    #define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
	int8_t l_err_num = status;
    int32_t i = 0;
    uint8_t num = 0;
	
	uint8_t err_status_bool[]=
	{
        ST_ERR_H, ST_ERR_L, ST_ERR_REAL, ST_ERR_CHAR, ST_ERR_FAIL,
        ST_ERR_SHORT, ST_ERR_ARC, ST_ERR_GFI, ST_ERR_AMP, ST_ERR_GEAR, ST_ERR_VOL_ABNORMAL,
	};/* ����λ�����Ĵ���״̬ */
	
    num = ARRAY_SIZE(err_status_bool);
    
    for(i = 0; i < num; i++)
    {
        if(l_err_num == err_status_bool[i])
        {
            break;
        }
    }
    
    /* û�з����쳣 */
    if(i == num)
    {
        return;
    }
    
	irq_stop_relay_motion();
    close_test_timer();/* �ض�ʱ�� */
    
	if(STOP)
	{
		return;
	}
    
    
    exit_test_relay_motion();/* ��λ�ŵ� */
	res_ave = err_res_bak;
    test_flag.vol_change_flag = 1;
    test_dis();/* �������� */
    
	CUR_FAIL = 1;/* ��ǰ������ʧ�� */
    FAIL = 1;/* ����ʧ�� */
	cur_result.err_num = l_err_num;
    recover_exception_scene();
    updata_result(cur_mode);
    save_cur_result(&cur_result);/* ������ */
    
    test_flag.judge_err_en == DISABLE;
    
	cur_status = err_status_bool[l_err_num%12];/* ����λ�����Ĵ���״̬ */
    cur_cylinder_ctrl_stop();
    
    test_flag.test_led_flag = 0;
	test_fail();
    
    updata_time(U_TEST_TIME, g_dis_time);
	
	if(OFFSET_BBD)
	{
		dis_exception_str(l_err_num, &GUI_FontHZ_SimSun_12);
	}
	else
	{
		dis_exception_str(l_err_num, &GUI_FontHZ_SimSun_16);
	}
    
    if(cur_mode == DCW)
    {
        if(type_spe.dcw_big_cap_en == ENABLE)
        {
            STOP = 1;
            /* ���������� */
            if(BUZZER_EN)
            {
                BUZZER = BUZZER_ON;/* ����ϵͳ���ؿ��� */
            }
            
            ERR_NUM = ERR_NONE;
            ERR_NUM_OVER = ERR_NONE;
            return;
        }
    }
    
	if(OFFSET_BBD)
	{
		goto l;
	}
    
	if(cur_mode == BBD)
	{
		if(l_err_num == ERR_SHORT)
		{
			cur_plc_err_st = PLC_HIGH_FAIL;
		}
		else if(l_err_num == ERR_OPEN)
		{
			cur_plc_err_st = PLC_LOW_FAIL;
		}
	}
	else
	{
		if(l_err_num == ERR_HIGH)
		{
			cur_plc_err_st = PLC_HIGH_FAIL;
		}
		else if(l_err_num == ERR_LOW)
		{
			cur_plc_err_st = PLC_LOW_FAIL;
		}
	}
	
    if(cur_mode == CC)
    {
        if(FAIL_MODE_CON == sys_par.fail_mode)
        {
            if(l_err_num == ERR_OPEN)
            {
                app_flag.buzzer_flicker_en = 1;
            }
            goto l;
        }
        else if(FAIL_MODE_CON != sys_par.fail_mode && FAIL_MODE_FPDFC != sys_par.fail_mode)
        {
            if(l_err_num == ERR_OPEN)
            {
                app_flag.buzzer_flicker_en = 1;
            }
        }
    }
	
    switch(sys_par.fail_mode)
    {
        case FAIL_MODE_STOP:
    l:
            fail_mode_stop_dispose();
            break;
        case FAIL_MODE_HALT:
            if(l_err_num == ERR_OPEN)
            {
                app_flag.buzzer_flicker_en = 1;
            }
            fail_mode_halt_dispose();
            break;
        case FAIL_MODE_CON:
            fail_mode_con_dispose();
            break;
        case FAIL_MODE_RESTART:
            fail_mode_restart_dispose();
            break;
        case FAIL_MODE_NEXT:
            fail_mode_next_dispose();
            break;
        case FAIL_MODE_FPDFC:
            fail_mode_fpdfc_dispose();
            break;
    }
    
    app_flag.buzzer_flicker_en = 0;
	ERR_NUM = ERR_NONE;
    ERR_NUM_OVER = ERR_NONE;
}
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
