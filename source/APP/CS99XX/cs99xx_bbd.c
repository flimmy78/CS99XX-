/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_bbd.c
 * ժ  Ҫ  ��bbd����
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
#include	"calibration.h"
/*
 * ��������bbd_dis_step
 * ����  ������һ����������Ϣ
 * ����  ��p ���뵱ǰ����ָ��
 * ���  ����
 * ����  ����
 */
static void bbd_dis_step(const uint8_t n)
{
	char dis[10] = {0};
	
	if(strlen((const char*)g_cur_file->name) > 9)
	{
		strncpy(dis, (const char*)g_cur_file->name, 7);
		strcat(dis, "..");
	}
	else
	{
		strcpy(dis, (const char*)g_cur_file->name);
	}
	
	GUI_DispStringAt((const char*)dis, 4+35+38+15+20+6*5+3, 0);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
	GUI_DispStringAt((const char*)dis, 4+35+38+15+20+6*5+3, 0);
    GUI_DispStringAt((const char*)work_mode_buf[g_cur_file->work_mode], 150+7, 15);
    GUI_ClearRect(150+7+12, 15, 150+7+12+6*3, 15+10);
	GUI_DispStringAt((const char*)mode_pool[GR_VOL_DROP_EN][cur_mode], 150+7+12, 15);
    
    dis_test_vol1(&GUI_FontHZ_SimSun_12);
    dis_test_time(&GUI_FontHZ_SimSun_12);
    
    test_ui_ele_set[TEST_ELE_2].y = OFFSET_ELE_2_Y + 12 * (n - 1);
    test_ui_ele_set[TEST_ELE_3].y = OFFSET_ELE_3_Y + 12 * (n - 1);
    
    if(cur_mode == BBD)
    {
        test_unit_dis(cur_mode, cur_gear, &GUI_FontHZ_SimSun_12);/* ��ʾ��λ */
    }
    
	dis_offset_test_ports(&cur_port);
	LCD_REFRESH();
}


/*
 * ��������offset_manual
 * ����  ����ƫ�Ʋ��Խ���ϸ��˵��
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void bbd_manual(void)
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
	
	GUI_ClearRect (0, 0, 200-1, 12-1);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("BBD����˵��", 60, 0);
	}
	else
	{
		GUI_SetFont(&GUI_Font13_1);
		GUI_DispStringAt("BBD specification", 50, 0);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	while(1)
	{
		GUI_ClearRect (0, 13+1, 200-1, 64-1);
		switch(n)
		{
			case 0:
				if(sys_par.language == CHINESE)
				{
					GUI_DispStringAt("1.���ϱ����뱻����ɿ�����", 5, 18);
					GUI_DispStringAt("2.�����������ɿ�ʼ����BBD����", 5, 32);
					GUI_DispStringAt("3.���¸�λ������ͣBBD����", 5, 46);
				}
				else
				{
					GUI_SetFont(&GUI_Font13_1);
					GUI_DispStringAt("1.Ensure proper connection.", 5, 18);
					GUI_DispStringAt("2.Press start key to perform BBD.", 5, 32);
					GUI_DispStringAt("3.Press stop key to stop BBD.", 5, 46);
					GUI_SetFont(&GUI_FontHZ_SimSun_12);
				}
				break;
		}
		LCD_REFRESH();
		pkey_value = OSQPend(KeyboardQSem, 0,&err);
		
		switch(*pkey_value)
		{
			case KEY_UP:/* ģʽ */
				n = 0;
				break;
			case KEY_DOWN:/* ˵�� */
				n = 0;
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
 * ��������serve_offset_shortcut
 * ����  ��������ƫ�Ʋ��Եĸ���������
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void serve_bbd_shortcut(void)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	
	clear_menu();
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("˵��", 208, 2);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Manual", 208-4, 2);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	LCD_REFRESH();
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		
		switch(*pkey_value)
		{
			case KEY_F1:/* ˵�� */
				bbd_manual();
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
/*
 * ��������dis_one_step
 * ����  ������һ����ƫ�Ʋ�����Ϣ
 * ����  ��p ���뵱ǰ����ָ�� �� n λ����Ϣ
 * ���  ����
 * ����  ����
 */
static void dis_one_bbd_step(const UN_STRUCT *p, const int8_t n)
{
	uint8_t X	= 30;
	uint8_t Y	= 15+12*(n-1);
	
	GUI_DispDecAt (p->com.step, 12, Y, 2);
	GUI_DispStringAt(".", 12+6*2, Y);
	
	if(p->com.mode == BBD)
	{
        test_ui_ele_set[TEST_ELE_2].y = OFFSET_ELE_2_Y + 12 * (n - 1);
        test_ui_ele_set[TEST_ELE_3].y = OFFSET_ELE_3_Y + 12 * (n - 1);
        offset_dis(n);
        test_unit_dis(cur_mode, cur_gear, &GUI_FontHZ_SimSun_12);
        GUI_DispStringAt(offset_res_buf[cur_getc_result], X+8*6+8*6, Y);
	}
	else
	{
		GUI_DispStringAt("-------", X, Y);
	}
	LCD_REFRESH();
}

/*
 * ��������draw_one_page
 * ����  ������һҳ��ƫ�Ʋ�����Ϣ
 * ����  ��p ���뵱ǰҳ�ĵ�һ������ָ��
 * ���  ����
 * ����  ����
 */
static void draw_bbd_one_page(NODE_STEP *p)
{
	int8_t i = 0;
	NODE_STEP *p_bak = g_cur_step;
	
	g_cur_step = p;
	GUI_ClearRect (0, 15, 145-1, 64-1);
    
	for(i = 0; i < 4; i++)
	{
		if(g_cur_step != NULL)
		{
			load_data();
			cap_ave = cur_cap;
			dis_one_bbd_step(&g_cur_step->one_step, i+1);
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
void clear_bbd_ok_flag(void)
{
	NODE_STEP *p_bak = g_cur_step;
	
	for(;g_cur_step;g_cur_step = g_cur_step->next)
	{
		if(g_cur_step->one_step.com.mode == BBD)
		{
			g_cur_step->one_step.bbd.get_cs_ok = 0;
		}
	}
    
	g_cur_step = p_bak;
}

void update_cap_limit(void)
{
	if(cap_ave * 5 < 10)
	{
		g_cur_step->one_step.bbd.upper_limit = cap_ave * 1000 * 1.2;
		g_cur_step->one_step.bbd.lower_limit = cap_ave * 1000 * 0.8;
	}
	else if(cap_ave * 5 < 100)
	{
		g_cur_step->one_step.bbd.upper_limit = cap_ave * 100 * 1.2;
		g_cur_step->one_step.bbd.lower_limit = cap_ave * 100 * 0.8;
	}
	else if(cap_ave * 5 < 1000)
	{
		g_cur_step->one_step.bbd.upper_limit = cap_ave * 10 * 1.2;
		g_cur_step->one_step.bbd.lower_limit = cap_ave * 10 * 0.8;
	}
	else
	{
		g_cur_step->one_step.bbd.upper_limit = cap_ave * 1 * 1.2;
		g_cur_step->one_step.bbd.lower_limit = cap_ave * 1 * 0.8;
	}
}
/*
 * ��������sys_bbd
 * ����  ����ȡ��·��·������ֵ
 * ����  ��const int8_t w ��ǵ��ú���
 * ���  ����
 * ����  ����
 */
void sys_bbd(uint32_t w)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t n = 1;/* �к� */
	int8_t p = 1;/* ҳ�� */
	int8_t cur_p_num = 0;
	int8_t size = g_cur_file->total;
	int8_t pages = size/4+(size%4==0? 0:1);/* һ����ҳ�� */
	int8_t last_page_num = size%4==0? 4:size%4;/* ���һҳ�ĸ��� */
	NODE_STEP *p_step = list_99xx.head;
	
    list_lon = 5;
    
	
    init_test_ui_info(1);
	test_irq_fun = test_irq;
// 	g_cur_step->one_step.bbd.get_cs_ok = 0;
	test_flag.bbd_time = 0;
	OFFSET_BBD = TEST_BBD;/* ��־BBDģʽ */
	clear_bbd_ok_flag();
	
	clear_();
	memset((void*)&test_flag, 0, sizeof test_flag);
l:	gui_draw_bbd();
	n = 1;
    p = 1;
	bbd_dis_step(n);
	
    load_steps_to_list(1);
    p_step = list_99xx.head;
    g_cur_step = p_step;
    load_data();
	draw_bbd_one_page(p_step);
    
    if(app_flag.into_get_cap)
    {
        goto ll;
    }
	
	while(1)
	{
		GUI_ClearRect (0, 15, 10, 60);
		GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+3+12*(n-1));
		bbd_dis_step(n);
		LCD_REFRESH();
		pkey_value = OSQPend(KeyboardQSem,20,&err);
		
		if(STOP_PIN == 0)
		{
			bbd_dis_step(n);
			p = g_cur_step->one_step.com.step/4+(g_cur_step->one_step.com.step%4==0? 0:1);
            
			if(n != 1 || p != 1)
			{
				n = 1;
				p = 1;
                load_steps_to_list(1);
                p_step = list_99xx.head;
                g_cur_step = p_step;
                load_data();
				draw_bbd_one_page(p_step);
			}
			memset((void*)&test_flag, 0, sizeof test_flag);
		}
		
		switch(*pkey_value)
		{
			case KEY_UP:
            {
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
                        load_steps_to_list(4 * (p - 1) + 1);
						p_step = list_99xx.head;
						draw_bbd_one_page(p_step);
					}
				}
                
				g_cur_step = p_step + n - 1;
                load_data();
                break;
            }
			case KEY_DOWN:
			{
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
                        load_steps_to_list(4 * (p - 1) + 1);
						p_step = list_99xx.head;
						draw_bbd_one_page(p_step);
					}
					n = 1;
				}
                
				g_cur_step = p_step + n - 1;
                load_data();
				break;
			}
			case KEY_SHORTCUT:
				de_reset();
				serve_bbd_shortcut();
				draw_short_cut_bm(UDSB_RE);
				break;
			case KEY_START:
            {
                /* ���㵱ǰҳ������ */
ll:       	    if(p == pages)
				{
					cur_p_num = last_page_num;
				}
				else
				{
					cur_p_num = 4;
				}
                
                load_steps_to_list(4 * (p - 1) + 1);
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
					draw_bbd_one_page(p_step);
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
                                draw_bbd_one_page(p_step);
                            }
                            n = 1;
                        }
					}
				}
                
				GUI_ClearRect (0, 15, 10, 60);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+3+12*(n-1));
				bbd_dis_step(n);
				g_cur_step = p_step + n - 1;
				load_data();/* ���µ�ǰ����Ҫ���¼������� */
				
				/* �ж�һ�µ�ǰ����ģʽ�Ƿ���IR */
				if(cur_mode != BBD)
				{
					/* �����һ���ǿվͽ������� */
					if(cur_step >= g_cur_file->total)
					{
                        load_steps_to_list(1);
						p_step = list_99xx.head;
						on_keyboard();/* �򿪼��� */
						dis_test_status(TEST_WAIT_INDEX, &GUI_FontHZ_SimSun_12);
						break;/* �������� */
					}
					/* �����һ�����ǿվͼ��� */
					else
					{
                        LCD_REFRESH();
                        OSTimeDlyHMSM(0, 0, 0, 500);
                        OFFSET_NEXT = 1;
						goto ll;
					}
				}
				
				GUI_DispStringAt(" ",30+8*6+8*6, 15+12*(n-1));
				bbd_dis_step(n);
				LCD_REFRESH();
				startup();
                
				if(2 != g_cur_step->one_step.bbd.get_cs_ok)
				{
					cur_gear = AC_20mA;
					cur_adc_cur_k = ratio_acw.adc_cur_k[AC_20mA];
					g_ir_dly = 1000;
// 					two_t = 30;
				}
				else
				{
// 					two_t = 10;
				}
                
                g_cur_step->one_step.bbd.get_cs_ok = 0;
				
				while(1)
				{
					offset_test();
                    /* ������ʾֵ */
                    if(test_flag.sample_task_en && !ERR_NUM)
                    {
                        count_dis_value();
                    }
                    OSTimeDlyHMSM(0,0,0,10);/* ������Ȩ���� */
					
					if(OFFSET_NEXT)
					{
						uint8_t num = 0;
						
                        g_cur_step->one_step.bbd.get_cs_ok = 2;
                        g_cur_step->one_step.bbd.cap_value = cap_ave;
						update_cap_limit();
						
						save_cur_step();/* �������ֵ */
                        irq_stop_relay_motion();/* �رյ��ӿ��� ��ѹ */
						n = cur_step%4;
						
						if(n == 0)
						{
							n = 4;
						}
						
						OFFSET_NEXT = 0;
						
						dis_one_bbd_step(&g_cur_step->one_step, n);
						
						g_cur_step = g_cur_step->next;/* ������һ�� */
						load_data();/* ���µ�ǰ����Ҫ���¼������� */
						num = g_cur_step->one_step.com.step;
						p = num/4+(num%4==0? 0:1);/* �������ǰ�ڼ�ҳ */
						
						/* �ս����µ�һҳ */
						if(num%4 == 1)
						{
							/* �����һҳ�� */
							if(p == pages)
							{
								cur_p_num = last_page_num;
							}
							else
							{
								cur_p_num = 4;
							}
							
							/* ��ʾ����ǰҳ�����в� */
							GUI_ClearRect (0, 15, 145-1, 64-1);
							p_step = list_99xx.head + 4*(p-1);
							draw_bbd_one_page(p_step);
							n = 1;
						}
						
						n = g_cur_step->one_step.com.step % 4;
                        
						if(n == 0)
						{
							n = 4;
						}
						
						GUI_ClearRect (0, 15, 10, 60);
						GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+3+12*(n-1));
						bbd_dis_step(n);
						
						OSTimeDly(300);
						/* �ж�һ�µ�ǰ����ģʽ�Ƿ���IR */
						if(cur_mode != BBD)
						{
							if(g_cur_step->next == NULL)
							{
								on_keyboard();/* �򿪼��� */
								
								dis_test_status(TEST_WAIT_INDEX, &GUI_FontHZ_SimSun_12);
                                
                                if(app_flag.into_get_cap)
                                {
                                    OFFSET_BBD = TEST_NORMAL;
                                    return;
                                }
                                
								break;/* �������� */
							}
							else
							{
								g_cur_step = g_cur_step->next;
								load_data();/* ���µ�ǰ����Ҫ���¼������� */
								goto ll;
							}
						}
						
						GUI_DispStringAt(" ",30+8*6+8*6, 15+12*(n-1));
						LCD_REFRESH();
						if(!g_cur_step->one_step.bbd.get_cs_ok)
						{
							cur_gear = AC_20mA;
							cur_adc_cur_k = ratio_acw.adc_cur_k[AC_20mA];
							g_ir_dly = 1000;
							two_t = 30;
						}
						else
						{
							two_t = 5;
						}
						startup(); /* �������� 2 */
                        
                        if(2 == g_cur_step->one_step.bbd.get_cs_ok)
                        {
                            cur_gear = AC_20mA;
                            cur_adc_cur_k = ratio_acw.adc_cur_k[AC_20mA];
                            g_ir_dly = 1000;
                            two_t = 10;
                        }
                        else
                        {
                            two_t = 10;
                        }
                        
                        g_cur_step->one_step.bbd.get_cs_ok = 0;
					}
					
					if(STOP)
					{
						STOP = 0;
                        irq_stop_relay_motion();/* �رյ��ӿ��� ��ѹ */
						on_keyboard();/* �򿪼��� */
						
						dis_test_status(TEST_WAIT_INDEX, &GUI_FontHZ_SimSun_12);
                        if(app_flag.into_get_cap)
                        {
                            return;
                        }
						break;
					}
					
					if(PASS)
					{
						n = cur_step % 4;
						LED_PASS = LED_ON;
						if(n == 0)
						{
							n = 4;
						}
                        
						OFFSET_NEXT = 0;
                        g_cur_step->one_step.bbd.get_cs_ok = 2;
						dis_one_bbd_step(&g_cur_step->one_step, n);
                        g_cur_step->one_step.bbd.cap_value = cap_ave;
						update_cap_limit();
						save_cur_step();/* �������ֵ */
                        irq_stop_relay_motion();/* �رյ��ӿ��� ��ѹ */
						
						on_keyboard();/* �򿪼��� */
						
						dis_test_status(TEST_WAIT_INDEX, &GUI_FontHZ_SimSun_12);
						
						if(g_cur_step->next == NULL)
						{
							p_step = list_99xx.head;
						}
						LCD_REFRESH();
                        
                        if(app_flag.into_get_cap)
                        {
                            OFFSET_BBD = TEST_NORMAL;
                            return;
                        }
                        
						break;
					}
					LCD_REFRESH();
				}
				break;
            }
			case KEY_BACK:
				de_reset();
				g_return = 1;
				OFFSET_BBD = TEST_NORMAL;
				return ;
			case KEY_EXIT:
				g_exit = 1;
				OFFSET_BBD = TEST_NORMAL;
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
			OFFSET_BBD = TEST_NORMAL;
			return;
		}
	}//end while(1)
}


/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE*******************/
