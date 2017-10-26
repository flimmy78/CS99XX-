/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_offset.c
 * ժ  Ҫ  ��ƫ�Ʋ��Թ���
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */


#include	<string.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include    "math.h"


/*************************** �ⲿ���� **************************/

/*
 * ��������offset_manual
 * ����  ����ƫ�Ʋ��Խ���ϸ��˵��
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void offset_manual(void)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t n = 0;
	
	clear_menu();
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (0, 0, 200-1, 64-1);
	GUI_DrawLine         (20  , 13  , 180  , 13  );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup,   215, 2 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	
	while(1)
	{
		GUI_ClearRect (0, 0, 200-1, 12-1);
		if(n<3)
		{
			if(sys_par.language == CHINESE)
			{
				GUI_DispStringAt("I. ƫ�Ʋ���˵��", 60, 0);
			}
			else
			{
				GUI_SetFont(&GUI_Font13_1);
				GUI_DispStringAt("I. Offset specification", 50, 0);
				GUI_SetFont(&GUI_FontHZ_SimSun_12);
			}
		}
		else
		{
			if(sys_par.language == CHINESE)
			{
				GUI_DispStringAt("II. ƫ�Ʋ���ģʽ", 60, 0);
			}
			else
			{
				GUI_SetFont(&GUI_Font13_1);
				GUI_DispStringAt("II. Offset mode", 60, 0);
				GUI_SetFont(&GUI_FontHZ_SimSun_12);
			}
		}
		GUI_ClearRect (0, 13+1, 200-1, 64-1);
		switch(n)
		{
			case 0:
				if(sys_par.language == CHINESE)
				{
					GUI_DispStringAt("1.���ϱ����뱻����ɿ�����", 5, 18);
					GUI_DispStringAt("2.�����������ɿ�ʼ����ƫ�Ʋ���", 5, 32);
					GUI_DispStringAt("3.���¸�λ������ͣƫ�Ʋ���", 5, 46);
				}
				else
				{
					GUI_SetFont(&GUI_Font13_1);
					GUI_DispStringAt("1.Ensure proper connection.", 5, 18);
					GUI_DispStringAt("2.Press start key to perform offset.", 5, 32);
					GUI_DispStringAt("3.Press stop key to stop offset.", 5, 46);
					GUI_SetFont(&GUI_FontHZ_SimSun_12);
				}
				break;
			case 1:
				if(sys_par.language == CHINESE)
				{
					GUI_DispStringAt("4.��һ�����Բ����ģʽ���ѹ��", 5, 18);
					GUI_DispStringAt("��������һ�������ı�ʱ����ǰ", 5+12, 32);
					GUI_DispStringAt("��ƫ�Ʋ�������Զ���Ч", 5+12, 46);
				}
				else
				{
					GUI_SetFont(&GUI_Font13_1);
					GUI_DispStringAt("4.When a param. of test step is", 5, 18);
					GUI_DispStringAt("changed,the last offset result", 5+12, 32);
					GUI_DispStringAt("doesn't effect.", 5+12, 46);
					GUI_SetFont(&GUI_FontHZ_SimSun_12);
				}
				break;
			case 2:
				if(sys_par.language == CHINESE)
				{
					GUI_DispStringAt("5.��Ե���裨IR��ģʽ��ƫ�Ʋ���", 5, 18);
					GUI_DispStringAt("����", 5+12, 32);
				}
				else
				{
					GUI_SetFont(&GUI_Font13_1);
					GUI_DispStringAt("5.IR without offset measurement", 5, 18);
					GUI_DispStringAt("function", 5+12, 32);
					GUI_SetFont(&GUI_FontHZ_SimSun_12);
				}
				break;
			case 3:
				if(sys_par.language == CHINESE)
				{
					GUI_DispStringAt("1.����ģʽ��ϵͳ�Ե�ǰ���Բ���", 5, 18);
					GUI_DispStringAt("����ƫ�Ʋ�����ֹͣ���谴����", 5+12, 32);
					GUI_DispStringAt("����������һ��ƫ�Ʋ���", 5+12, 46);
				}
				else
				{
					GUI_SetFont(&GUI_Font13_1);
					GUI_DispStringAt("1.Single mode:Systerm stops testing", 5, 18);
					GUI_DispStringAt("after the test on the current step,", 5+12, 32);
					GUI_DispStringAt("press the start key for the next step", 5+12, 46);
					GUI_SetFont(&GUI_FontHZ_SimSun_12);
				}
				break;
			case 4:
				if(sys_par.language == CHINESE)
				{
					GUI_DispStringAt("2.����ģʽ��ϵͳ�Զ��Ե�ǰ���ļ�", 5, 18);
					GUI_DispStringAt("���в��Բ������ƫ�Ʋ�����ͣ", 5+12, 32);
					GUI_DispStringAt("ֹ", 5+12, 46);
				}
				else
				{
					GUI_SetFont(&GUI_Font13_1);
					GUI_DispStringAt("2.Series mode:Systerm auto stops", 5, 18);
					GUI_DispStringAt("testing after the test on all steps", 5+12, 32);
					GUI_DispStringAt("of the current file.", 5+12, 46);
					GUI_SetFont(&GUI_FontHZ_SimSun_12);
				}
				break;
			case 5:
				if(sys_par.language == CHINESE)
				{
					GUI_DispStringAt("3.�Ӳ˵��������ƫ�Ʋ�������", 5, 18);
					GUI_DispStringAt("ϵͳĬ��Ϊ��������ģʽ������", 5+12, 32);
					GUI_DispStringAt("ģʽ���ܼ��˵��¸��Ĳ���ģʽ", 5+12, 46);
				}
				else
				{
					GUI_SetFont(&GUI_Font13_1);
					GUI_DispStringAt("3.Enter offset measurement interface", 5, 18);
					GUI_DispStringAt("from menu,and systerm default is", 5+12, 32);
					GUI_DispStringAt("series mode.", 5+12, 46);
					GUI_SetFont(&GUI_FontHZ_SimSun_12);
				}
				break;
		}
		LCD_REFRESH();
		pkey_value = OSQPend(KeyboardQSem, 0,&err);
		
		switch(*pkey_value)
		{
			case KEY_UP:/* ģʽ */
				if(--n < 0)
				{
					n = 5;
				}
				break;
			case KEY_DOWN:/* ˵�� */
				if(++n > 5)
				{
					n = 0;
				}
				break;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
	}
}
/*
 * ��������offset_sel_mode
 * ����  ��ƫ�Ʋ��Է�����ģʽ������������ �ú�������ѡ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
static int8_t offset_sel_mode(void)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t n = app_flag.offset_mode;
	
	GUI_ClearRect(0, 0, 200-1, 64-1);
    
    draw_short_cut_bm(__RB);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DrawLine(20, 14   , 180  , 14);
	GUI_DispStringAt("��", 3+8*6, 18);
	GUI_DispStringAt("��", 3+8*6, 32);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("ƫ�Ʋ���ģʽ����", 50, 2);
		GUI_DispStringAt("����ģʽ", 3, 18);
		GUI_DispStringAt("����", 3+75, 18);
		GUI_DispStringAt("����", 3+75+50, 18);
		
		GUI_DispStringAt("˵   ��", 3, 32);
		GUI_DispStringAt("ѡ���Ĳ���ģʽ��",3+10*6, 32);
		GUI_DispStringAt("����Ч", 3+60, 46);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Offset mode selection", 40, 0);
		GUI_DispStringAt("Mode", 3, 18);
		GUI_DispStringAt("Single", 3+75, 18);
		GUI_DispStringAt("Series", 3+75+50, 18);
		
		GUI_DispStringAt("Manual", 3, 32);
		GUI_DispStringAt("Selected mode takes",3+60, 32);
		GUI_DispStringAt("effect immediately.", 3+60, 46);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	while(1)
	{
		switch(n)
		{
			case OFFSET_CON:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+65, 18+3 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+65+50, 18+3 );
				cursor_pos(3+65+50+1, 18);
				break;
			case OFFSET_SIG:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+65, 18+3 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+65+50, 18+3 );
				cursor_pos(3+65+1, 18);
				break;
		}
        
		pkey_value = OSQPend(KeyboardQSem,0,&err);
        
		switch(*pkey_value)
		{
			case KEY_SHIFT:
                if(n == OFFSET_CON)
                {
                    n = OFFSET_SIG;
                }
                else
                {
                    n = OFFSET_CON;
                }
				break;
			case KEY_ENTER:
				g_return = 1;
				return n;
			case KEY_BACK:
				g_return = 1;
				return -1;
			case KEY_EXIT:
				g_exit = 1;
				return -1;
		}
	}
}
/*
 * ��������serve_offset_shortcut
 * ����  ��������ƫ�Ʋ��Եĸ���������
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void serve_offset_shortcut(void)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t m = 0;
	
	clear_menu();
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("ģʽ", 208, 2);
		GUI_DispStringAt("˵��", 208, 18);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Mode", 208-4, 2);
		GUI_DispStringAt("Manual", 208-4, 18);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	LCD_REFRESH();
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		
		switch(*pkey_value)
		{
			case KEY_F1:/* ģʽ */
				m = offset_sel_mode();
				if(m != -1)
				{
					app_flag.offset_mode = m;
				}
				break;
			case KEY_F2:/* ˵�� */
				offset_manual();
				break;
			case KEY_BACK:
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		if(g_exit)
		{
			return;
		}
		if(g_return)
		{
			return;
		}
	}
}
void dis_offset_test_ports(const TEST_PORT *pport)
{
    uint16_t *p = NULL;
    int32_t i = 0;
    uint8_t index = 0;
    uint8_t value = 0;
    uint8_t x = 0;
    uint8_t y = 0;
    
    if(pport == NULL || type_spe.port_num == 0)
    {
        return;
    }
    
    GUI_SetFont(&GUI_FontHZ_SimSun_12);
    p = (uint16_t *)pport;
    
    for(i = 0; i < type_spe.port_num; i++)
    {
        y = UI_OFFSET_PORTS_BASE_Y + UI_OFFSET_PORT_Y_O * ( i / 8);
        
        switch(type_spe.port_num)
        {
            case PORT_NUM14:
                x = UI_OFFSET_PORTS_BASE_X + UI_OFFSET_PORT_X_O * i;
                GUI_DispCharAt("- "[i % 2], x + 6, y);
                break;
            case PORT_NUM4:
                x = 10 + 12 * i;
                break;
            case PORT_NUM8:
            {
                if(i < 4)
                {
                    x = 10 + 12 * i;
                }
                else
                {
                    x = 10 + 12 * i + 6;
                }
                break;
            }
            default:
            {
                return;
            }
        }
        
        index = i % 8;
        value = ((p[i / 8]) >> (index * 2)) & 3;
        GUI_DispCharAt(PORT_STATUS_STR[value], x, y);
    }
}
/*
 * ��������offset_dis_step
 * ����  ������һ����������Ϣ
 * ����  ��n ����ڼ���
 * ���  ����
 * ����  ����
 */
static void offset_dis_step(const uint8_t n)
{
	char dis[10] = {0};
	
    if(type_spe.port_num <= PORT_NUM8)
    {
        GUI_SetFont(&GUI_FontHZ_SimSun_12);
        GUI_DispStringAt((const char*)"��" , 4+35+38+15+20+4*6, 0);
        
        if(sys_par.language == CHINESE)
        {
            GUI_DispStringAt((const char*)"�ļ�" , 4+35+38+15+25-3, 0);
        }
        else
        {
            GUI_SetFont(&GUI_FontHZ_SimSun_13);
            GUI_DispStringAt((const char*)"File" , 4+35+38+15+25, -1);
            GUI_SetFont(&GUI_FontHZ_SimSun_12);
        }
        
        if(strlen((const char*)g_cur_file->name) > 9)
        {
            strncpy(dis, (const char*)g_cur_file->name, 7);
            strcat(dis, "..");
        }
        else
        {
            strcpy(dis, (const char*)g_cur_file->name);
        }
        
        GUI_SetFont(&GUI_FontHZ_SimSun_12);
        GUI_DispStringAt((const char*)dis, 4+35+38+15+20+6*5+3, 0);
        GUI_DispStringAt((const char*)work_mode_buf[g_cur_file->work_mode], 150+7, 15);
        GUI_ClearRect(150+7+12, 15, 150+7+12+6*3, 15+10);
        GUI_DispStringAt((const char*)mode_pool[GR_VOL_DROP_EN][cur_mode], 150+7+12, 15);
        
        dis_test_vol1(&GUI_FontHZ_SimSun_12);
        dis_test_time(&GUI_FontHZ_SimSun_12);
        
        test_ui_ele_set[TEST_ELE_2].y = OFFSET_ELE_2_Y + 12 * (n - 1);
        test_ui_ele_set[TEST_ELE_3].y = OFFSET_ELE_3_Y + 12 * (n - 1);
        test_unit_dis(cur_mode, cur_gear, &GUI_FontHZ_SimSun_12);/* ��ʾ��λ */
        
    }
    
    dis_offset_test_ports(&cur_port);
    
	LCD_REFRESH();
}
/*
 * ��������dis_one_step
 * ����  ������һ����ƫ�Ʋ�����Ϣ
 * ����  ��p ���뵱ǰ����ָ�� �� n λ����Ϣ
 * ���  ����
 * ����  ����
 */
static void dis_one_offset_step(const int8_t n)
{
	uint8_t X	= 30;
	uint8_t Y	= 15 + 12 * (n - 1);
	
	GUI_DispDecAt (cur_step, 12, Y, 2);
	GUI_DispStringAt(".", 12+6*2, Y);
    test_ui_ele_set[TEST_ELE_2].y = OFFSET_ELE_2_Y + 12 * (n - 1);
    test_ui_ele_set[TEST_ELE_3].y = OFFSET_ELE_3_Y + 12 * (n - 1);
	offset_dis(n);
    test_unit_dis(cur_mode, cur_gear, &GUI_FontHZ_SimSun_12);
	GUI_DispStringAt(offset_res_buf[cur_offset_result], X+8*6+8*6, Y);
	
	LCD_REFRESH();/* ���ﲻ��ˢ�� �������Ļ��˸ */
}

/*
 * ��������draw_offset_one_page
 * ����  ������һҳ��ƫ�Ʋ�����Ϣ
 * ����  ��p ���뵱ǰҳ�ĵ�һ������ָ��
 * ���  ����
 * ����  ����
 */
static void draw_offset_one_page(NODE_STEP *p)
{
	int8_t i = 0;
	NODE_STEP *p_bak = g_cur_step;
	
	g_cur_step = p;
	GUI_ClearRect (0, 15, 145-1, 64-1);
	
	LCD_REFRESH();
	for(i = 0; i < 4; i++)
	{
		if(g_cur_step != NULL)
		{
			load_data();
            
			dis_one_offset_step(i+1);
		}
		else
		{
			break;
		}
		g_cur_step = g_cur_step->next;
	}
	g_cur_step = p_bak;
	load_data();
}

/*
 * ��������sys_offset
 * ����  ��ϵͳƫ�ƹ���
 * ����  ��const int8_t w ��ǵ��ú���
 * ���  ����
 * ����  ����
 */
void sys_offset(uint32_t w)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t n = 1;/* �к� */
	int8_t p = 1;/* ҳ�� */
	int8_t cur_p_num = 0;/* ��ǰҳ�Ĳ��� */
	int8_t size = g_cur_file->total;/* �ܲ��� */
	int8_t pages = size/4+(size%4==0? 0:1);/* һ����ҳ�� */
	int8_t last_page_num = size%4==0? 4:size%4;/* ���һҳ�ĸ��� */
	NODE_STEP *p_step = list_99xx.head;
	
    list_lon = 5;
    
	if(type_spe.offset_fun_en == OFFSET_OFF)
	{
		return;
	}
    
    if(g_cur_file->work_mode == G_MODE)
    {
        test_irq_fun = test_g_irq;
    }
    else
    {
        test_irq_fun = test_irq;
    }
	
	OFFSET_BBD = TEST_OFFSET;/* ��־ƫ��ģʽ */
    init_test_ui_info(1);
	clear_();
    GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	if(w == DUMMY)
	{
		app_flag.offset_mode = 0;/* ÿ�ν���ƫ�Ʋ��Խ��涼Ҫ������ģʽ��Ϊ����״̬ */
	}
	
	memset((void*)&test_flag, 0, sizeof test_flag);
l:	gui_draw_offset();
	n = 1;
    p = 1;
    load_steps_to_list(1);
    p_step = list_99xx.head;
    g_cur_step = p_step;
    load_data();
	draw_offset_one_page(p_step);
	offset_dis_step(n);
	
    /* ͨ�ſ��ƻ�ȡƫ�ƣ��Զ��������� */
    if(app_flag.into_get_offset)
    {
        goto ll;
    }
    
	while(1)
	{
		GUI_ClearRect (0, 15, 10, 60);
		GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+3+12*(n-1));
        
		offset_dis_step(n);
		pkey_value = OSQPend(KeyboardQSem,20,&err);
		
		if(STOP_PIN == 0)
		{
			p = g_cur_step->one_step.com.step/4+(g_cur_step->one_step.com.step%4==0? 0:1);
			if(n != 1 || p != 1)
			{
				n = 1;
				p = 1;
                
                load_steps_to_list(1);
				p_step = list_99xx.head;
				g_cur_step = list_99xx.head;
				draw_offset_one_page(p_step);
			}
			memset((void*)&test_flag, 0, sizeof test_flag);
		}
		
		switch(*pkey_value)
		{
			case KEY_UP:
                PASS = 0;
				if(--n < 1)
				{
					n = 4;
					if(--p < 1)
					{
						p = pages;
						n = last_page_num;
					}
                    
					if(pages > 1)
					{
                        
                        load_steps_to_list(4*(p-1)+1);
						p_step = list_99xx.head;
						draw_offset_one_page(p_step);
					}
				}
                
				g_cur_step = p_step + n - 1;
                load_data();
				break;
			case KEY_DOWN:
                PASS = 0;
				if(p == pages)
				{
					cur_p_num = last_page_num;
				}
				else
				{
					cur_p_num = 4;
				}
				if(++n > cur_p_num)
				{
					if(++p > pages)
					{
						p = 1;
					}
					
					if(pages > 1)
					{
                        load_steps_to_list(4*(p-1)+1);
						p_step = list_99xx.head;
						draw_offset_one_page(p_step);
					}
					n = 1;
				}
				g_cur_step = p_step + n - 1;
                load_data();
				break;
			case KEY_SHORTCUT:
				de_reset();
				serve_offset_shortcut();
				draw_short_cut_bm(UDSB_RE);
				break;
			case KEY_START:
                /* ���㵱ǰҳ������ */
ll:       	    if(p == pages)
				{
					cur_p_num = last_page_num;
				}
				else
				{
					cur_p_num = 4;
				}
                
                load_steps_to_list(4*(p-1)+1);
				p_step = list_99xx.head;/* p_stepָ��ǰҳ�ĵ�һ�� */
				
				if(g_cur_file->total == cur_step)
				{
                    PASS = 0;
                    OFFSET_NEXT = 0;
					n = 1;
					p = 1;
                    load_steps_to_list(4 * (p - 1) + 1);
					p_step = list_99xx.head;
                    g_cur_step = p_step;
                    load_data();
					draw_offset_one_page(p_step);
				}
				
				if(PASS | OFFSET_NEXT)
				{
					PASS = 0;
                    OFFSET_NEXT = 0;
                    
					if(g_cur_file->total > cur_step)
					{
                        if(++n > cur_p_num)
                        {
                            if(++p > pages)
                            {
                                p = 1;
                            }
                            
                            if(pages > 1)
                            {
                                load_steps_to_list(4 * (p - 1) + 1);
                                p_step = list_99xx.head;
                                draw_offset_one_page(p_step);
                            }
                            n = 1;
                        }
					}
				}
                
				GUI_ClearRect (0, 15, 10, 60);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+3+12*(n-1));
				offset_dis_step(n);
				g_cur_step = p_step+n-1;
				load_data();/* ���µ�ǰ����Ҫ���¼������� */
				
				/* �ж�һ�µ�ǰ����ģʽ�Ƿ���IR BBD CC */
				if(cur_mode == IR || cur_mode == BBD || cur_mode == CC)
				{
					/* �����һ���ǿվͽ������� */
					if(cur_step >= g_cur_file->total)
					{
                        load_steps_to_list(1);
						p_step = list_99xx.head;
						on_keyboard();/* �򿪼��� */
						dis_test_status(TEST_WAIT_INDEX, &GUI_FontHZ_SimSun_12);
                        if(app_flag.into_get_offset)
                        {
                            return;
                        }
                        
						break;/* �������� */
					}
					/* �����һ�����ǿվͼ��� */
					else
					{
                        LCD_REFRESH();
                        OSTimeDlyHMSM(0,0,0,500);
                        OFFSET_NEXT = 1;
						goto ll;
					}
				}
				
				GUI_DispStringAt(" ",30+8*6+8*6, 15+12*(n-1));
				offset_dis_step(n);
				LCD_REFRESH();
				startup();
				
				/* ������Դ�ѭ�� */
				for(;;)
				{
                    /* ������ʾֵ */
                    if(test_flag.sample_task_en && !ERR_NUM)
                    {
                        count_dis_value();
                    }
                    
					offset_test();
                    OSTimeDlyHMSM(0,0,0,10);
					
					if(OFFSET_NEXT)
					{
                        irq_stop_relay_motion();/* �رյ��ӿ��� ��ѹ */
						dis_one_offset_step(n);
						save_cur_step();/* ����ƫ�ƽ�� */
                        goto ll;
					}
					
					if(STOP)
					{
                        irq_stop_relay_motion();/* �رյ��ӿ��� ��ѹ */
						STOP = 0;
						LED_ALL = LED_OFF;
						on_keyboard();/* �򿪼��� */
						dis_test_status(TEST_WAIT_INDEX, &GUI_FontHZ_SimSun_12);
                        
                        if(app_flag.into_get_offset)
                        {
                            return;
                        }
                        
						break;
					}
					
					if(PASS)
					{
						n = cur_step % 4;
						LED_PASS = 1;
						if(n == 0)
						{
							n = 4;
						}
                        
						save_cur_step();/* ����ƫ�ƽ�� */
                        irq_stop_relay_motion();/* �رյ��ӿ��� ��ѹ */
						OFFSET_NEXT = 0;
						dis_one_offset_step(n);
						
						on_keyboard();/* �򿪼��� */
						dis_test_status(TEST_WAIT_INDEX, &GUI_FontHZ_SimSun_12);
                        
                        if(app_flag.into_get_offset)
                        {
                            return;
                        }
						break;
					}
					LCD_REFRESH();
				}//for(;;)
				break;
			case KEY_BACK:
				de_reset();
				g_return = 1;
				OFFSET_BBD = TEST_NORMAL;/* ��־ƫ��ģʽ */
				return ;
			case KEY_EXIT:
				OFFSET_BBD = TEST_NORMAL;/* ��־ƫ��ģʽ */
				g_exit = 1;
				return ;
		}//end switch()
		
		if(STOP_PIN == 0 && LED_PASS)
		{
			LED_PASS = 0;
		}
		if(g_return)
		{
			g_return = 0;
			goto l;
		}
		if(g_exit)
		{
			OFFSET_BBD = TEST_NORMAL;/* ��־ƫ��ģʽ */
			return;
		}
	}//end while(1)
}


/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE*******************/
