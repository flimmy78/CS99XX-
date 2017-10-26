/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_par_com.c
 * ժ  Ҫ  �����ø���ģʽ���еı���
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
 

#include	<string.h>
#include    <stdio.h>
#include 	<math.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include 	"MC14094.h"
#include    "cs99xx_result.h"

void draw_one_par_info(DATA_INFO* p, void *pdata, const uint8_t unit);
// void init_offset_par(UN_STRUCT *p);
// static DATA_INFO*  server_combination_key(const uint16_t keyvalue);


/*
 * ��������set_contrast
 * ����  ������Һ�����ĶԱȶ�
 * ����  ��const int8_t gear �Աȶȵĵ�λѡ��
 * ���  ����
 */
void set_contrast(const int8_t gear)
{
	uint8_t data = 0xbb;
	MC14094_CMD_RA(data+4*gear);
	MC14094_Updata_RA();
}
/*
 * ��������define_modes
 * ����  ��ȷ��ģʽ ��ģʽ����Ŀ
 * ����  ����
 * ���  ��mode_buf ��ģʽ�� flag �Ŷ�Ӧmode_buf��ģʽ���� kinds ����ģʽ����Ŀ
 * ����  ����
 */
uint16_t define_modes(const uint8_t **mode_buf, uint8_t *flag, int16_t *kinds)
{
	int32_t k = 0;
	
	if(MODEL_EN & __ACW)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][ACW];
		flag[k] = ACW;
	}
	if(MODEL_EN & __DCW)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][DCW];
		flag[k] = DCW;
	}
	if(MODEL_EN & __IR)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][IR];
		flag[k] = IR;
	}
	if(MODEL_EN & __GR)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][GR];
		flag[k] = GR;
	}
	if(MODEL_EN & __CC)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][CC];
		flag[k] = CC;
	}
	if(MODEL_EN & __BBD)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][BBD];
		flag[k] = BBD;
	}
    
	if(kinds != NULL)
	{
		*kinds = k;
	}
	
	mode_buf[0]=mode_buf[1];
	flag[0] = flag[1];
    
	return k;
}
uint16_t define_g_modes(const uint8_t **mode_buf, uint8_t *flag, int16_t *kinds)
{
	int32_t k = 0;
	if(MODEL_EN & __ACW)
	{
		mode_buf[++k]="ACW";
		flag[k] = ACW;
	}
	if(MODEL_EN & __DCW)
	{
		mode_buf[++k]="DCW";
		flag[k] = DCW;
	}
	if(kinds != NULL)
	{
		*kinds = k;
	}
	mode_buf[0]=mode_buf[1];
	flag[0] = flag[1];
	return k;
}
void init_mode(NODE_STEP *p)
{
	switch(p->one_step.com.mode)
	{
		case ACW:
			init_acw_step(p);
			break;
		case DCW:
			init_dcw_step(p);
			break;
		case IR:
			init_ir_step(p);
			break;
		case GR:
			init_gr_step(p);
			break;
		case BBD:
			init_bbd_step(p);
			break;
        case CC:
            init_cc_step(p);
            break;
	}
	
	init_par_info_list(&cur_node);
	init_offset_par(p);/* ģʽ���޸����Ժ�Ҫ���ƫ�Ʋ�����Ϣ */
}

void updata_title_step(UN_STRUCT *p)
{
    uint32_t t = 0;
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
    t = get_res_total();
    t = sys_par.used_res_num * 10 / t;
    if(t >= sys_par.allowance)
    {
        GUI_FillRect(0, 0, 200, 12);
        GUI_SetTextMode(GUI_TEXTMODE_REV);//���÷�ת�ı�
    }
    
    MenuTitle(NUL);
    
    GUI_DispDecAt(p->com.step       , TITLE_STEP_NUM_X   , TITLE_Y, menu_par[GUI_COM_STEP].lon);
    GUI_DispDecAt(g_cur_file->total , TITLE_STEP_TOTAL_X , TITLE_Y, menu_par[GUI_COM_STEP].lon);
    
//     GUI_ClearRect (TITLE_MODE_X, TITLE_Y, TITLE_MODE_X + 6 * 3, TITLE_Y + 10);
	GUI_DispStringAt((const char*)mode_pool[GR_VOL_DROP_EN][p->com.mode], TITLE_MODE_X, TITLE_Y);/* ��������ģʽ */
    
// 	GUI_SetFont(&GUI_FontHZ_SimSun_12);
//     
// 	GUI_DispDecAt(p->com.step, 4+60, 15, menu_par[GUI_COM_STEP].lon);
//     
//     GUI_ClearRect (4+35+38+15-2, 0, 4+35+38+15-2+6*3, 10);
// 	GUI_DispStringAt((const char*)mode_pool[GR_VOL_DROP_EN][p->com.mode], 4+35+38+15-2, 0);/* ��������ģʽ */

	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//���������ı�
}

void updata_step(UN_STRUCT *p)
{
	updata_title_step(p);
}

void serve_numkey(const uint16_t keyvalue)
{
	uint8_t flag = 0;

	switch(keyvalue)
	{
		case KEY_0:
			change_data(0);
			break;
		case KEY_1:
			change_data(1);
			break;
		case KEY_2:
			change_data(2);
			break;
		case KEY_3:
			change_data(3);
			break;
		case KEY_4:
			change_data(4);
			break;
		case KEY_5:
			change_data(5);
			break;
		case KEY_6:
			change_data(6);
			break;
		case KEY_7:
			change_data(7);
			break;
		case KEY_8:
			change_data(8);
			break;
		case KEY_9:
			change_data(9);
			break;
		default:
			flag = 1;
			break;
	}

	if(flag == 0)
	{
		update_data(pmenu_data);
		shift_cursor(pmenu_data);
	}
}

DATA_INFO* find_menu(uint8_t n)
{
	uint32_t i = 0;
	DATA_INFO* p = par_info_list.head;
	
	if(n > par_info_list.tail->page)
	{
		return 0;
	}
	
	for(i=1; i < 20; i++)
	{
		if(p->page == n)
		{
			return p;
		}
		p = p->next;
	}
    
	return NULL;
}
DATA_INFO*  server_combination_key(const uint16_t keyvalue)
{
	DATA_INFO* temp = NULL;
	switch(keyvalue)
	{
		case KEY_0&KEY_1:
			app_flag.goto_menu_page = 1;
			break;
		case KEY_0&KEY_2:
			app_flag.goto_menu_page = 2;
			break;
		case KEY_0&KEY_3:
			app_flag.goto_menu_page = 3;
			break;
		case KEY_0&KEY_4:
			app_flag.goto_menu_page = 4;
			break;
		case KEY_0&KEY_5:
			app_flag.goto_menu_page = 5;
			break;
		case KEY_0&KEY_6:
			app_flag.goto_menu_page = 6;
			break;
		default:
			app_flag.goto_menu_page = 0;
			g_skip_page = 0;
			break;
	}
    
	if(app_flag.goto_menu_page)
	{
		g_skip_page = 1;
	}
    
	return temp;
}
static uint16_t serve_swap(void)
{
	#define X0	40
	#define Y0	5
	#define X1	170
	#define Y1	57
    
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t temp_step = 1;
	const uint8_t x = ((X0+(X0+X1)/2)/2-5-6);
	const uint8_t y = Y0+28;
	DATA_INFO *menu_bak = pmenu_data;
	DATA_INFO swap_inf = {
		(void*)0/*pData*/,x/*x*/,y/*y*/,0/*type*/,4/*lon*/,0/*decs*/,0/*total*/,0/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
	
    if(MAX_STEPS < 100)
    {
        swap_inf.lon = 2;
    }
    
    GUI_DrawRect(X0, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+18, (X0+X1)/2, Y1-3);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//���÷�ת�ı�
	/*******************************************************
	ͨ�����ñ���ɫ��ǰ��ɫҲ���ԴﵽĿ��
	*******************************************************/
// 	GUI_SetBkColor(GUI_WHITE);
// 	GUI_SetColor(GUI_BLACK);

// 	GUI_SetBkColor(GUI_BLACK);
// 	GUI_SetColor(GUI_WHITE);
// 	GUI_DispStringAt((const char*)"fan se", X0+1, Y0+13);
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
        GUI_DispStringAt_BTP_STR_("���뻥�������", X0, X1, Y0+3, 6);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
        GUI_DispStringAt_BTP_STR("Input swap step", X0, X1, Y0+2);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//���������ı�
	
	GUI_DispDecAt (1, (X0+(X0+X1)/2)/2-5-6, Y0+28, menu_par[GUI_COM_STEP].lon);
	GUI_DispDecAt (1, (X1+(X0+X1)/2)/2-5-6, Y0+18, menu_par[GUI_COM_STEP].lon);
	GUI_DispDecAt (g_cur_file->total, (X1+(X0+X1)/2)/2-5-6, Y1-12, menu_par[GUI_COM_STEP].lon);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, (X1+(X0+X1)/2)/2-3, Y0+30);
	
	swap_inf.pData = &temp_step;
	pmenu_data = &swap_inf;
	cursor_pos(x,y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		
		serve_numkey(*pkey_value);
        
		switch(*pkey_value)
		{
			case KEY_ENTER:
				if(temp_step > 0 && temp_step > g_cur_file->total)
				{
					temp_step = g_cur_file->total;
				}
				else if(temp_step == 0)
				{
					temp_step = 1;
				}
                
				if(temp_step > 0 && temp_step <= g_cur_file->total)
				{
                    g_return = 1;
                    GUI_ClearRect (X0, Y0, X1, Y1);
                    pmenu_data = menu_bak;
					return temp_step;
				}
                break;
			case KEY_BACK:
				g_return = 1;
				GUI_ClearRect (X0, Y0, X1, Y1);
				pmenu_data = menu_bak;
				return 0;
			case KEY_EXIT:
				g_exit = 1;
				pmenu_data = menu_bak;
				return 0;
			default:
				break; 
		}
	}
    
	#undef X0
	#undef Y0
	#undef X1
	#undef Y1
}

static void serve_more(UN_STRUCT *p)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
    uint16_t temp_step = 0;
	
	GUI_ClearRect(200+1, 0, 240-1, 15-1);
	GUI_ClearRect(200+1,15+1,240-1,31-1);
	GUI_ClearRect(200+1,31+1,240-1,47-1);
    
	if(g_cur_step->one_step.com.step != 1)
	{
		if(sys_par.language == CHINESE)
		{
			GUI_DispStringAt((const char*)"ǰ��", 208, 2);
		}
		else
		{
			GUI_SetFont(&GUI_FontHZ_SimSun_13);
			GUI_DispStringAt((const char*)"Before", 208-4, 2);
			GUI_SetFont(&GUI_FontHZ_SimSun_12);
		}
	}
	else
	{
		GUI_ClearRect (208, 2 , 230, 12);
	}
    
	if(g_cur_step->one_step.com.step != g_cur_file->total)
	{
		if(sys_par.language == CHINESE)
		{
			GUI_DispStringAt((const char*)"����", 208, 18);
		}
		else
		{
			GUI_SetFont(&GUI_FontHZ_SimSun_13);
			GUI_DispStringAt((const char*)"After", 208-4, 18);
			GUI_SetFont(&GUI_FontHZ_SimSun_12);
		}
	}
	else
	{
		GUI_ClearRect (208, 18, 230, 28);
	}
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"����", 208, 34);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Swap", 208-4, 34);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
	LCD_REFRESH();
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		
		//�ж�err״̬
		switch(*pkey_value)
		{
			case KEY_ADVANCE:/* ǰ�� */
				if(1 == p->com.step)
				{
					break;
				}
                
				g_cur_step->one_step = *p;//�������ʱ���������ݸ��µ���ǰ������
                save_list();/* ���������������ݸ��µ�eep�� */
                
                swap_step(g_cur_step->one_step.com.step, g_cur_step->one_step.com.step - 1);
                load_steps_to_list(g_cur_step->one_step.com.step - 1);
				g_cur_step = list_99xx.head;
				*p = g_cur_step->one_step;
				updata_step(p);
				
				load_this_page_info(g_cur_info);
				load_this_page_value();
				
				save_list();/* ���������������ݸ��µ�eep�� */
				g_return = 1;
				return;
			case KEY_RETREAT:/* ���� */
				if(g_cur_file->total == p->com.step)
				{
					break;
				}
                
				g_cur_step->one_step = *p;//�������ʱ���������ݸ��µ���ǰ������
                save_list();/* ���������������ݸ��µ�eep�� */
                
                swap_step(g_cur_step->one_step.com.step, g_cur_step->one_step.com.step + 1);
                
				load_steps_to_list(g_cur_step->one_step.com.step + 1);
				g_cur_step = list_99xx.head;
				*p = g_cur_step->one_step;
                
				updata_step(p);
				load_this_page_info(g_cur_info);
				load_this_page_value();
				save_list();/* ���������������ݸ��µ�eep�� */
				g_return = 1;
				return;
			case KEY_SWAP:/* ���� */
				temp_step = serve_swap();
                
				if(0 != temp_step && temp_step<= g_cur_file->total)
				{
                    g_cur_step->one_step = *p;//�������ʱ���������ݸ��µ���ǰ������
                    save_list();/* ���������������ݸ��µ�eep�� */
                    
                    swap_step(g_cur_step->one_step.com.step, temp_step);
                    load_steps_to_list(temp_step);
                    g_cur_step = list_99xx.head;
					*p = g_cur_step->one_step;//��������¾ֲ����ݵ�����
                    
					updata_step(p);
					load_this_page_info(g_cur_info);
					load_this_page_value();
					save_list();/* ���������������ݸ��µ�eep�� */
				}
                
				if(g_return || g_exit)
				{
					return;
				}
				break;
			case KEY_BACK:
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			default:
				break;
		}
	}
}

static void serve_shortcut(UN_STRUCT *p)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t m = 0;
	
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};/* ��ģʽ������ */
	uint8_t flag[MODEL_KINDS_MAX] = {0};/* �������ģʽ������ */
	int16_t kinds = 0;/* ��ģʽ����Ŀ */
	
	define_modes(mode_buf, flag, &kinds);
	
	GUI_ClearRect(200+1, 0, 240-1, 15-1);
	GUI_ClearRect(200+1,15+1,240-1,31-1);
	GUI_ClearRect(200+1,31+1,240-1,47-1);
	
	if(g_cur_file->total < MAX_STEPS)
	{
		if(sys_par.language == CHINESE)
		{
			GUI_DispStringAt((const char*)"����", 208, 2);
		}
		else
		{
			GUI_SetFont(&GUI_FontHZ_SimSun_13);
			GUI_DispStringAt((const char*)"Insert", 208-4, 2);
			GUI_SetFont(&GUI_FontHZ_SimSun_12);
		}
	}
	
	while(1)
	{
		if(1 != g_cur_file->total)
		{
			if(sys_par.language == CHINESE)
			{
				GUI_DispStringAt((const char*)"ɾ��", 208, 18);
				GUI_DispStringAt((const char*)"����", 208, 34);
			}
			else
			{
				GUI_SetFont(&GUI_FontHZ_SimSun_13);
				GUI_DispStringAt((const char*)"Delete", 208-4, 18);
				GUI_DispStringAt((const char*)"More", 208-4, 34);
				GUI_SetFont(&GUI_FontHZ_SimSun_12);
			}
		}
		else
		{
			GUI_ClearRect (208, 18, 230, 28);
			GUI_ClearRect (208, 34, 230, 44);
		}
		LCD_REFRESH();
		
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		
		//�ж�err״̬
		switch(*pkey_value)
		{
			case KEY_INSERT:
			{
				if(g_cur_file->total >= MAX_STEPS)
				{
					break;
				}
                
                g_cur_step->one_step = *p;//����֮ǰ���õ�����,����ͻᶪʧ
                m = get_first_step_mode();/* ���ݵ�һ����ģʽ */
                
                save_cur_step();	/* �ڸ���g_cur_step֮ǰҪ��֮ǰ������д�뵽eep�� */
                
                insert_step(p->com.step, flag[1]);/* �ղ�����²���ģʽӦ�����ͺ����еĵ�һ���ͺ� */
                
                load_steps_to_list(p->com.step+1);
                
                g_cur_step = list_99xx.head;
                
				if(g_cur_file->work_mode == G_MODE)
				{
					g_cur_step->one_step.com.mode = m;/* Gģʽ�²�����²�Ҫ���һ����ģʽ����һ�� */
				}
                
				init_mode(g_cur_step);/* ��ʼ���ող���Ĳ� */
				*p = g_cur_step->one_step;
				updata_step(p);
				
				load_this_page_info(g_cur_info);
				load_this_page_value();
				
				save_list();/* ���������������ݸ��µ�eep�� */
				return;
			}
			case KEY_DEL:
			{
				if(1 == g_cur_file->total)
				{
					break;
				}
                
				if(g_cur_file->total > 1)
				{
                    del_step(g_cur_step->one_step.com.step);
                    
                    if(g_cur_file->total < g_cur_step->one_step.com.step)
                    {
                        load_steps_to_list(p->com.step-1);
                    }
                    else
                    {
                        load_steps_to_list(p->com.step);
                    }
                    
                    g_cur_step = list_99xx.head;
					*p = g_cur_step->one_step;
					updata_step(p);
					
					load_this_page_info(g_cur_info);
					load_this_page_value();
					save_cur_step();/* ���������������ݸ��µ�eep�� */
				}
				return;
			}
			case KEY_MORE:
				if(1 == g_cur_file->total)
				{
					break;
				}
                
				serve_more(p);
                
				if(g_exit)
				{
					return;
				}
				if(g_return)
				{
					updata_step(p);
					load_this_page_info(g_cur_info);
					load_this_page_value();
					return;
				}
				if(sys_par.language == CHINESE)
				{
					GUI_DispStringAt((const char*)"����", 208, 2);
				}
				else
				{
					GUI_SetFont(&GUI_FontHZ_SimSun_13);
					GUI_DispStringAt((const char*)"Insert", 208-4, 2);
					GUI_SetFont(&GUI_FontHZ_SimSun_12);
				}
				if(1 != g_cur_file->total)
				{
					if(sys_par.language == CHINESE)
					{
						GUI_DispStringAt((const char*)"ɾ��", 208, 18);
						GUI_DispStringAt((const char*)"����", 208, 34);
					}
					else
					{
						GUI_SetFont(&GUI_FontHZ_SimSun_13);
						GUI_DispStringAt((const char*)"Delete", 208-4, 18);
						GUI_DispStringAt((const char*)"More", 208-4, 34);
						GUI_SetFont(&GUI_FontHZ_SimSun_12);
					}
				}
				else
				{
					GUI_ClearRect (200+1, 15+1, 240-1, 31-1);
					GUI_ClearRect (200+1, 31+1, 240-1, 47-1);
				}
				
				LCD_REFRESH();
				break;
			case KEY_BACK:
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
 * ��������set_step
 * ����  ����ʾһ���˵����ֵ
 * ����  ��p_node ָ���м�����ṹ�����û���Ҫ��������ʱ�Ż���µ����Բ��Ĳ�����
 * ���  ����
 * ����  ����
 */
void set_step(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t m = 0;
	void *pdata_bak = NULL;/* ��ǰ�˵���ָ�뱸�� */
	NODE_STEP *p_temp_step;/* ��ʱ���� ������������Ƿ���� */
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};/* ��ģʽ������ */
	uint8_t flag[MODEL_KINDS_MAX] = {0};/* �������ģʽ������ */
	int16_t kinds = 0;/* ��ģʽ����Ŀ */
	
	define_modes(mode_buf, flag, &kinds);/* ��ȡ�����ģʽ */
	pmenu_data = par_info_list.head;/* ��ǰ�˵�����ָ��ָ��˵�����ͷ */
	pdata_bak = pmenu_data->pData;/* �޸�ǰ��������ָ�� */
    
    list_lon = 2;
    
	GUI_ClearRect (4, 15, 200-1, 63);
	
	clear_();
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
l:
	MenuTitle(p->com.mode);
    draw_menu_frame_line(0, 12, 200, 12);
	GUI_DrawLine(130, 20  , 130, 64-10);
	draw_colon();
    
    /* �������������Ϊ1 �Ͳ���ʾSHORTCUTͼ�� */
	if(MAX_STEPS != 1)
	{
        draw_short_cut_bm(UDSB);
	}
	else
	{
        draw_short_cut_bm(UDRB);
	}
	
	//��ʾȡֵ��Χ
    if(MAX_STEPS < 100)
    {
        GUI_DispStringAt((const char*)"01  ", 3*6+145-3, 18);
        GUI_DispStringAt((const char*)"99  ", 3*6+145-3, 45);
        GUI_DispDecAt (MAX_STEPS, 3*6+145-3, 45, menu_par[GUI_COM_STEP].lon);
    }
    else
    {
        GUI_DispDecAt (1        , 3*6+145-3-6, 18, menu_par[GUI_COM_STEP].lon);
        GUI_DispDecAt (MAX_STEPS, 3*6+145-3-6, 45, menu_par[GUI_COM_STEP].lon);
    }
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, 162, 33);
    
    /* ָ���м���� */
	pmenu_data->pData = &g_cur_val[pmenu_data->cur];
	pmenu_data->ch_bit = 0;
	updata_step(p);
	
	cursor_pos(pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		dis_cursor(pmenu_data);
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);/* �Ӷ����л�ȡ��ֵ */
		
        /* ��ϼ����� */
		server_combination_key(*pkey_value);
        /* ��������ϼ���Ҫ��ת������ҳ�� */
		if(g_skip_page)
		{
			pmenu_data->pData = pdata_bak;
			g_skip_page = 0;
			return;
		}
        
		serve_numkey(*pkey_value); /* �������� */
        
		switch(*pkey_value)
		{
			case KEY_UP:
			{
				if(g_cur_val[pmenu_data->cur] > 0 && g_cur_val[pmenu_data->cur] < MAX_STEPS)
				{
					//ֱ���������뵱ǰ���������Ĳ���ʱ���������
					if(g_cur_val[pmenu_data->cur] <= g_cur_file->total && g_cur_val[pmenu_data->cur] != p->com.step)
					{
						g_cur_step->one_step = *p;//����֮ǰ���õ�����,����ͻᶪʧ
						save_cur_step();	/* �ڸ���g_cur_step֮ǰҪ��֮ǰ������д�뵽eep�� */
                        
                        p_temp_step = position_step(g_cur_val[pmenu_data->cur]);//���µ����貽
                        
                        if(p_temp_step != NULL)
                        {
                            g_cur_step = p_temp_step;
                        }
						
                        load_steps_to_list(g_cur_step->one_step.com.step);
                        g_cur_step = list_99xx.head;
						*p = g_cur_step->one_step;//������ʱ��Ϊ��ǰ������
					}
					else if(g_cur_file->total == g_cur_step->one_step.com.step)
					{
						g_cur_step->one_step = *p;//����֮ǰ���õ�����,����ͻᶪʧ
                        
                        m = get_first_step_mode();/* ���ݵ�һ����ģʽ */
                        
						save_cur_step();	/* �ڸ���g_cur_step֮ǰҪ��֮ǰ������д�뵽eep�� */
                        
                        insert_step(p->com.step, flag[1]);/* �ղ�����²���ģʽӦ�����ͺ����еĵ�һ���ͺ� */
                        
                        load_steps_to_list(p->com.step+1);
                        
						g_cur_step = list_99xx.head;
                        
						if(g_cur_file->work_mode == G_MODE)
						{
							g_cur_step->one_step.com.mode = m;/* Gģʽ�²�����²�Ҫ���һ����ģʽ����һ�� */
						}
                        
						init_mode(g_cur_step);/* ��ʼ���ող���Ĳ� */
						
						*p = g_cur_step->one_step;//������ʱ��Ϊ��ǰ������
						save_list();/* ���ı��Ľ�������eep�� */
					}
					else// if(NULL != g_cur_step->next)
					{
						g_cur_step->one_step = *p;//����֮ǰ���õ�����,����ͻᶪʧ
						save_cur_step();	/* �ڸ���g_cur_step֮ǰҪ��֮ǰ������д�뵽eep�� */
                        load_steps_to_list(g_cur_step->one_step.com.step+1);
						g_cur_step = list_99xx.head;
						*p = g_cur_step->one_step;//������ʱ��Ϊ��ǰ������
					}
				}
				else if(g_cur_val[pmenu_data->cur] >= MAX_STEPS)
				{
					g_cur_step->one_step = *p;//����֮ǰ���õ�����,����ͻᶪʧ
                    save_cur_step();	/* �ڸ���g_cur_step֮ǰҪ��֮ǰ������д�뵽eep�� */
                    
                    load_steps_to_list(1);
					g_cur_step = list_99xx.head;
					*p = g_cur_step->one_step;//������ʱ��Ϊ��ǰ������
					pmenu_data->ch_bit = 0;
				}
				else
				{
					pmenu_data->ch_bit = 0;
				}
                
				updata_step(p);
				init_par_info_list(&cur_node);
				load_this_page_info(g_cur_info);
				load_this_page_value();
				cursor_pos(4+60,15);
                
                g_cur_val[pmenu_data->cur] = p->com.step;/* �������µĲ���ŵ������� */
                pmenu_data->pData = &g_cur_val[pmenu_data->cur];
				break;
			}
			case KEY_DOWN:
			{
				if(g_cur_val[pmenu_data->cur] > 0 && g_cur_val[pmenu_data->cur] <= MAX_STEPS)
				{
					if(g_cur_val[pmenu_data->cur] <= g_cur_file->total && g_cur_val[pmenu_data->cur] != p->com.step)
					{
						g_cur_step->one_step = *p;//����֮ǰ���õ�����,����ͻᶪʧ
						
                        p_temp_step = position_step(g_cur_val[pmenu_data->cur]);
                        /* һ��Ҫ�����ݽ����ϸ��� */
                        if(NULL != p_temp_step)
                        {
                            g_cur_step = p_temp_step;
                        }
                        
						*p = g_cur_step->one_step;
						updata_step(p);
					}
				}
                
                g_cur_val[pmenu_data->cur] = p->com.step;/* �������µĲ���ŵ������� */
                
				updata_step(p);
				init_par_info_list(&cur_node);
				GUI_ClearRect (120, 15, 200-1, 63);
                
				pmenu_data->pData = pdata_bak;
                save_cur_step();
				pmenu_data = pmenu_data->next;
				goto l_end;
			}
			case KEY_SHORTCUT:
			{
				if(MAX_STEPS!=1)
				{
					serve_shortcut(p);
					updata_step(p);
					init_par_info_list(&cur_node);
					load_this_page_info(g_cur_info);
					load_this_page_value();
					cursor_pos(4+60,15);
                    
                    /* �������������Ϊ1 �Ͳ���ʾSHORTCUTͼ�� */
                    if(MAX_STEPS!=1)
                    {
                        draw_short_cut_bm(UDSB);
                    }
                    else
                    {
                        draw_short_cut_bm(UDRB);
                    }
					
					LCD_REFRESH();
					pmenu_data = &menu_par[GUI_COM_STEP];//����serve_shortcut()�иı���pmenu_dataʱ���غ�Ҫ����ָ���һ������
					pdata_bak = pmenu_data->pData;/* �޸�ǰ���� */
                    
                    g_cur_val[pmenu_data->cur] = p->com.step;
                    pmenu_data->pData = &g_cur_val[pmenu_data->cur];
					pmenu_data->ch_bit = 0;//һ��Ҫ�ǵ�Ҫ��λch_bit
                    save_par_to_ram(pmenu_data, g_cur_val[pmenu_data->cur]);
					if(g_return)
					{
						g_return = 0;
						goto l;
					}
				}
				else
				{
					shift_cursor(pmenu_data);
				}
				break;
			}
			case KEY_ENTER:
			{
                /* temp_step ����0С���������ܲ��� ������temp���ǵ�ǰ�� */
				if(g_cur_val[pmenu_data->cur] > 0 && g_cur_val[pmenu_data->cur] <= g_cur_file->total && g_cur_val[pmenu_data->cur] != p->com.step)
				{
                    p_temp_step = position_step(g_cur_val[pmenu_data->cur]);
                    
                    if(p_temp_step != NULL)
                    {
                        g_cur_step = p_temp_step;
                    }
                    
                    load_steps_to_list(g_cur_step->one_step.com.step);
					g_cur_step = list_99xx.head;
                    
					*p = g_cur_step->one_step;
					init_par_info_list(&cur_node);
					pmenu_data->pData = &g_cur_val[pmenu_data->cur];
					pmenu_data->ch_bit = 0;
					updata_step(p);
					init_par_info_list(&cur_node);
					load_this_page_info(g_cur_info);
					load_this_page_value();
				}
                /* ����ǵ�ǰ����ָ����һ������������ */
				else if(g_cur_val[pmenu_data->cur] == p->com.step)
				{
					pmenu_data->pData = pdata_bak;
					pmenu_data = pmenu_data->next;//ָ����һ������
					GUI_ClearRect (120, 15, 200-1, 63);
					goto l_end;
				}
                /* temp_step�Ƿ���Ч��ֵ����Ҫ�������� */
				else
				{
					//�ϴ�������Ч����ʱ�����ָ���ʼֵ���Է�ֹ�������뷢������
					g_cur_val[pmenu_data->cur] = g_cur_step->one_step.com.step;
					pmenu_data->ch_bit = 0;
					updata_step(p);
				}
                
				cursor_pos(4+60,15);
                
                g_cur_val[pmenu_data->cur] = p->com.step;/* �������µĲ���ŵ������� */
                pmenu_data->pData = &g_cur_val[pmenu_data->cur];
                save_cur_step();
				break;
			}
			case KEY_BACK:
				g_return = 1;
				goto l_end;
			case KEY_EXIT:
				g_exit = 1;
				goto l_end;
			default:
				break;
		}
        
        if(g_exit)
        {
            return;
        }
	}
    
l_end:
    save_group_info(g_cur_file->num);
}

static enter_mode(NODE_STEP *p)
{
	/* ���ģʽ���޸� */
	if(p->one_step.com.mode != g_cur_step->one_step.com.mode)
	{
		init_mode(p);
		pmenu_data = par_info_list.head->next;/* ָ��ǰ���ڵ�����ģʽ��һ�� */
	}
    
	init_par_info_list(&cur_node);
}

void set_mode(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t n = p->com.mode;
	uint8_t m = n;
	uint8_t work_mode = g_cur_file->work_mode;
	
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};
	uint8_t flag[MODEL_KINDS_MAX] = {0};
	int16_t kinds = 0;
	int32_t i = 0;
	
	GUI_ClearRect(0,13,200-1,63);
	load_this_page_info(g_cur_info);
	load_this_page_value();
    draw_colon();
    
	/* ȷ����ǰ��ģʽ���� */
	if(work_mode == N_MODE)
	{
		kinds = define_modes(mode_buf, flag, &kinds);/* ��ȡ��ǰ���͵�ģʽ��Ϣ */
	}
	else if(work_mode == G_MODE)
	{
		kinds = define_g_modes(mode_buf, flag, &kinds);/* ��ȡ��ǰ���͵�ģʽ��Ϣ */
	}
	
	
	/* ���㵱ǰģʽ�ǵڼ���ģʽ */
	for(i = 1; i <= kinds; i++)
	{
		if(flag[i] == n)
		{
			n = i;
			break;
		}
	}
	
	while(1)
	{
		if(work_mode == N_MODE || (work_mode == G_MODE && p->com.step == 1))
		{
			mode_sel_gui(kinds, mode_buf, n);
		}
        else
        {
            mode_sel_gui(1, &mode_buf[n - 1], 1);
            GUI_DrawLine(130, 20  , 130, 64-10);
            gui_g_mode_inval();
        }
        LCD_REFRESH();
		
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
		
		server_combination_key(*pkey_value);
        
		if(g_skip_page)
		{
			g_skip_page = 0;
			return;
		}
        
        switch(*pkey_value)
		{
			case KEY_UP:
			case KEY_DOWN:
			case KEY_ENTER:
				if(work_mode == G_MODE)
				{
					if(flag[n] != m)
					{
                        del_all_steps();/* ʵ�ʻ�����һ�� */
                        load_steps_to_list(1);
						g_cur_step = list_99xx.head;
						g_cur_step->one_step.com.mode = flag[n];/* ���� */
                        init_mode(g_cur_step);
                        *p = g_cur_step->one_step;
					}
				}
				p->com.mode = flag[n];//�Ѹ��½����������
				enter_mode(p_node);
                updata_step(p);
				g_cur_step->one_step = *p;
				save_cur_step();
                save_group_info(g_cur_file->num);
				menu_par[GUI_COM_MODE].unit[0] = mode_buf[n];
				menu_par[GUI_COM_MODE].unit[1] = mode_buf[n];
				cursor_pos(-5,0);
                break;
        }
        
		//�ж�err״̬
		switch(*pkey_value)
		{
			case KEY_UP:
				pmenu_data = pmenu_data->prev;
				return;
			case KEY_DOWN:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_SHIFT://ѡ��ģʽ
				if(work_mode == N_MODE)
				{
					if(++n > kinds)
					{
						n = 1;
					}
				}
				else
				{
					/* Gģʽ��ֻ�е�һ����������ģʽ */
					if(g_cur_step->one_step.com.step == 1)
					{
						if(++n > kinds)
						{
							n = 1;
						}
					}
				}
				
				p->com.mode = flag[n];//ʵʱ����ģʽ
				updata_step(p);//�����½����ʾ����
				break;
			case KEY_ENTER:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			case KEY_1:
				if(kinds >= 1)
				{
					n = 1;
					p->com.mode = flag[n];//ʵʱ����ģʽ
					updata_step(p);//�����½����ʾ����
				}
				break;
			case KEY_2:
				if(kinds >= 2)
				{
					n = 2;
					p->com.mode = flag[n];//ʵʱ����ģʽ
					updata_step(p);//�����½����ʾ����
				}
				break;
			case KEY_3:
				if(kinds >= 3)
				{
					n = 3;
					p->com.mode = flag[n];//ʵʱ����ģʽ
					updata_step(p);//�����½����ʾ����
				}
				break;
			case KEY_4:
				if(kinds >= 4)
				{
					n = 4;
					p->com.mode = flag[n];//ʵʱ����ģʽ
					updata_step(p);//�����½����ʾ����
				}
				break;
			case KEY_5:
				if(kinds >= 5)
				{
					n = 5;
					p->com.mode = flag[n];//ʵʱ����ģʽ
					updata_step(p);//�����½����ʾ����
				}
				break;
		}
        
        if(g_exit)
        {
            return;
        }
	}
}


void init_offset_par(NODE_STEP *p)
{
	
    if(p == NULL)
    {
        return;
    }
    
	switch(p->one_step.com.mode)
	{
		case ACW:
			p->one_step.acw.offset_cur = 0;/* ƫ�Ƶ��� */
			p->one_step.acw.offset_real = 0;/* ƫ����ʵ���� */
			p->one_step.acw.offset_result = 0;/* ƫ�Ƶ���������� 0ʧ�� 1�ϸ� */

			break;
		case DCW:
			p->one_step.dcw.offset_cur = 0;/* ƫ�Ƶ��� */
			p->one_step.dcw.offset_result = 0;/* ƫ�Ƶ���������� 0ʧ�� 1�ϸ� */
			break;
		case GR:
			p->one_step.gr.offset_res = 0;/* ƫ�Ƶ��� */
			p->one_step.gr.offset_result = 0;/* ƫ�Ƶ���������� 0ʧ�� 1�ϸ� */
			break;
		default:
			break;
	}
}

void set_voltage(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	uint16_t vol_ele_bak = 0;
	void* pdata_bak = pmenu_data->pData;
	
	//���ӵڶ�ҳ����ʱ��Ҫ�ػص�һҳ�ı���
	GUI_ClearRect(0,13,200-1,63);
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	
	draw_colon();
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, 162, 33);
	GUI_DrawLine         (130, 20, 130, 64-10);
	
	/* ��Χ */
	GUI_DispStringAt((const char*)pmenu_data->upper_str, 145, 45);
	GUI_DispStringAt((const char*)pmenu_data->lower_str, 145, 18);
	GUI_GotoXY(145, 45);
	GUI_DispFloatFix ((float)pmenu_data->upper/pow(10,pmenu_data->decs), pmenu_data->lon, pmenu_data->decs);
	GUI_GotoXY(145, 18);
	GUI_DispFloatFix ((float)pmenu_data->lower/pow(10,pmenu_data->decs), pmenu_data->lon, pmenu_data->decs);
	
	vol_ele_bak = g_cur_val[pmenu_data->cur];/* ���ݵ�ѹֵ�������ֵ�Ƿ�ı䣬����ı�ͻ����ƫ��ֵ */
	
    /* ���ݼ�� */
    if(g_cur_val[pmenu_data->cur] < pmenu_data->lower || g_cur_val[pmenu_data->cur] > pmenu_data->upper)
    {
        g_cur_val[pmenu_data->cur] = pmenu_data->lower;
//         *(uint16_t*)pmenu_data->pData = g_cur_val[pmenu_data->cur];
        save_par_to_ram(pmenu_data, g_cur_val[pmenu_data->cur]);
        init_offset_par(p_node);
        g_cur_step->one_step = *p;
        save_cur_step();
    }
    
	pmenu_data->pData = &g_cur_val[pmenu_data->cur];
	pmenu_data->ch_bit = 0;
	cursor_pos(pmenu_data->x,pmenu_data->y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		serve_numkey(*pkey_value);
		server_combination_key(*pkey_value);
		if(g_skip_page)
		{
			g_skip_page = 0;
			pmenu_data->pData = pdata_bak;
			return;
		}
		
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				GUI_ClearRect (120, 15, 200-1, 63);
				pmenu_data->pData = pdata_bak;
				
				if(g_cur_val[pmenu_data->cur] < pmenu_data->lower)
				{
					g_cur_val[pmenu_data->cur] = pmenu_data->lower;
				}
				else if(g_cur_val[pmenu_data->cur] > pmenu_data->upper)
				{
					g_cur_val[pmenu_data->cur] = pmenu_data->upper;
				}
// 				*(uint16_t*)pmenu_data->pData = g_cur_val[pmenu_data->cur];
                save_par_to_ram(pmenu_data, g_cur_val[pmenu_data->cur]);
				
				/* ����ѹ������ı�ʱƫ��ֵ���� */
				if(g_cur_val[pmenu_data->cur] != vol_ele_bak)
				{
					init_offset_par(p_node);
				}
//				g_cur_step->one_step = *p;
//				save_cur_step();
				break;
		}
		
		switch(*pkey_value)
		{
			case KEY_UP:
				pmenu_data = pmenu_data->prev;
				return;
			case KEY_DOWN:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_ENTER:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			default:
				break;
		}
        if(g_exit)
        {
            return;
        }
	}
}

void set_all_ports(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	uint32_t n = 0;
	uint32_t port_num = type_spe.port_num;
	void* pdata_bak = pmenu_data->pData;
    uint16_t *p_port = (uint16_t*)pmenu_data->pData;
    uint8_t ch_line_flag = 0;/* ���б�Ƿ� */
	
	/* ����˿���Ϊ0 �ͷ��� */
	if(port_num == PORT_NUM0)
	{
		pmenu_data = pmenu_data->next;
		return;
	}
	
	GUI_ClearRect(0,13,200-1,63);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	memset(g_cur_info, 0, sizeof g_cur_info);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	
	if(g_cur_file->work_mode == G_MODE)
	{
		if(sys_par.language == CHINESE)
		{
			GUI_DispStringAt((const char*)"ϵͳĬ��", 145, 22);
			GUI_DispStringAt((const char*)"���ܸı�", 145, 36);
		}
		else
		{
			GUI_SetFont(&GUI_FontHZ_SimSun_13);
			GUI_DispStringAt((const char*)"Systerm", 145, 22);
			GUI_DispStringAt((const char*)"default", 145, 36);
			GUI_SetFont(&GUI_FontHZ_SimSun_12);
		}
	}
	else if(g_cur_file->work_mode == N_MODE)
	{
		GUI_DispStringAt ("0 - X",  140+15, 20);
		GUI_DispStringAt ("1 - L", 140+15, 32);
		GUI_DispStringAt ("2 - H", 140+15, 44);
	}
	
	GUI_DrawLine (140, 20, 140, 64-10);
	
	if(port_num == PORT_NUM4)
	{
		draw_port_for_set(pmenu_data->pData);
	}
	else if(port_num == PORT_NUM8)
	{
		draw_port_for_set(pmenu_data->pData);
	}
    else if(port_num == PORT_NUM14)
    {
		draw_port_for_set(pmenu_data->pData);
    }
    
	ports_cursor_pos(n, port_num);/* ��ʾ��� */
	
// 	pmenu_data->pData = &g_cur_val[pmenu_data->cur];
	
// 	LCD_REFRESH();
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
		
		server_combination_key(*pkey_value);
		if(g_skip_page)
		{
			g_skip_page = 0;
			pmenu_data->pData = pdata_bak;
			return;
		}
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				pmenu_data->pData = pdata_bak;
				/* Gģʽ�²�֧�ֶ�·ɨ�� */
				if(g_cur_file->work_mode == N_MODE)
				{
					*(TEST_PORT*)pmenu_data->pData = *(TEST_PORT*)p_port;
//					g_cur_step->one_step = *p;
//					save_cur_step();
				}
				break;
		}
		
		switch(*pkey_value)
		{
			case KEY_UP:
                if(n > 7)
                {
                    n -= 8;
                    ch_line_flag = 1;
                }
                else
                {
                    pmenu_data = pmenu_data->prev;
                    return;
                }
                break;
			case KEY_DOWN:
                if(port_num > 8 && n < 8)
                {
                    if(n + 8 < port_num)
                    {
                        n += 8;
                    }
                    else
                    {
                        n = port_num - 1;
                    }
                    
                    ch_line_flag = 1;
                }
                else
                {
                    pmenu_data = pmenu_data->next;
                    return;
                }
                break;
			case KEY_SHIFT:/* ���case����ʡ */
				break;
			case KEY_ENTER:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			case KEY_0:
				if(g_cur_file->work_mode == N_MODE)
				{
					p_port[n / 8] &= ~(3 << 2 * (n % 8));
				}
				break;
			case KEY_1:
				if(g_cur_file->work_mode == N_MODE)
				{
					p_port[n / 8] &= ~(3 << 2 * (n % 8));
					p_port[n / 8] |=  (1 << 2 * (n % 8));
				}
				break;
			case KEY_2:
				if(g_cur_file->work_mode == N_MODE)
				{
					p_port[n / 8] &= ~(3 << 2 * (n % 8));
					p_port[n / 8] |=  (2 << 2 * (n % 8));
				}
				break;
			default:
				continue;//��������Ƿ���������
		}
		
        if(!ch_line_flag)
        {
            if(++n >= port_num)
            {
                n = 0;
            }
        }
        else
        {
            ch_line_flag = 0;
        }
		
		draw_port_for_set((void*)p_port);
        
		if(g_cur_file->work_mode == N_MODE)
		{
			ports_cursor_pos(n, port_num);
		}
		
        if(g_exit)
        {
            return;
        }
        
		LCD_REFRESH();
	}
}

void set_test_time(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	int32_t invalid_flag = 0;
	void *pdata_bak = pmenu_data->pData;
	
	GUI_ClearRect(0,13,200-1,63);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	memset(g_cur_info, 0, sizeof g_cur_info);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	
	draw_colon();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, 162, 33);
	/* ��Χ */
	GUI_DispStringAt((const char*)pmenu_data->upper_str, 145, 45);
	GUI_DispStringAt((const char*)pmenu_data->lower_str, 145, 18);
	GUI_DrawLine         (130, 20, 130, 64-10);
    
	if((g_cur_file->work_mode == G_MODE))
	{
		uint8_t temp_flag = 0;
		/* �ǵ�һ��������ʱ�� */
		if(pmenu_data == &menu_par[GUI_COM_RISE_T] && (p->com.step != 1))
		{
			temp_flag = 1;
		}
		/* �½�ʱ�� */
		else if(pmenu_data == &menu_par[GUI_COM_FALL_T] && (list_99xx.tail != g_cur_step))
		{
			temp_flag = 1;
		}
		else if(pmenu_data == &menu_par[GUI_COM_INTER_T] && (p->com.step == 1))
		{
			temp_flag = 1;
		}
		
		if(temp_flag)
		{
			invalid_flag = 1;
			gui_g_mode_inval();
		}
	}
    else
    {
        if(type_spe.dcw_big_cap_en == ENABLE && g_cur_step->one_step.com.mode == DCW)
        {
            if(pmenu_data == &menu_par[GUI_COM_FALL_T])
            {
                invalid_flag = 1;
                gui_g_mode_inval();
            }
        }
    }
	
	pmenu_data->pData = &g_cur_val[pmenu_data->cur];
	pmenu_data->ch_bit = 0;
	cursor_pos (pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
		
		server_combination_key(*pkey_value);
		if(g_skip_page)
		{
			g_skip_page = 0;
			pmenu_data->pData = pdata_bak;
			return;
		}
		serve_numkey(*pkey_value);
		
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				pmenu_data->pData = pdata_bak;
				if(g_cur_val[pmenu_data->cur] > pmenu_data->upper)
				{
					g_cur_val[pmenu_data->cur] = pmenu_data->upper;
				}
				else if(g_cur_val[pmenu_data->cur]>0 && g_cur_val[pmenu_data->cur]<pmenu_data->lower)
				{
					g_cur_val[pmenu_data->cur] = pmenu_data->lower;
				}
				
				if(invalid_flag)
				{
					g_cur_val[pmenu_data->cur] = 0;
				}
                
                save_par_to_ram(pmenu_data, g_cur_val[pmenu_data->cur]);
//				g_cur_step->one_step = *p;
//				save_cur_step();
				break;
		}
		
		switch(*pkey_value)
		{
			case KEY_UP:
				pmenu_data = pmenu_data->prev;
				return;
			case KEY_DOWN:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_ENTER:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			default:
				break;
		}
        if(g_exit)
        {
            return;
        }
	}
}

void set_either_or_par(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = 0;
	INT8U err = 0;
	int32_t invalid_flag = 0;
	void *pdata_bak = pmenu_data->pData;
	
	GUI_ClearRect(0,13,200-1,63);
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
	
	if((g_cur_file->work_mode == G_MODE))
	{
		if(pmenu_data == &menu_par[GUI_COM_STEPS_PASS]
			|| pmenu_data == &menu_par[GUI_COM_STEPS_CONT])
		{
			GUI_DrawLine(130, 20, 130, 64-10);
			invalid_flag = 1;
			gui_g_mode_inval();
		}
	}
	
	pmenu_data->pData = &g_cur_val[pmenu_data->cur];
	pmenu_data->ch_bit = 0;
	
	while(1)
	{
		switch(g_cur_val[pmenu_data->cur])
		{
			case 1:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, pmenu_data->x, pmenu_data->y+pmenu_data->y_offset);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,pmenu_data->x+pmenu_data->x_offset, pmenu_data->y+pmenu_data->y_offset);
				cursor_pos (pmenu_data->x+1, pmenu_data->y);
				break;
			case 0:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, pmenu_data->x, pmenu_data->y+pmenu_data->y_offset);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,pmenu_data->x+pmenu_data->x_offset, pmenu_data->y+pmenu_data->y_offset);
				cursor_pos (pmenu_data->x+pmenu_data->x_offset+1, pmenu_data->y);
				break;
			default:
				break;
		}
		
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		server_combination_key(*pkey_value);
		if(g_skip_page)
		{
			g_skip_page = 0;
			pmenu_data->pData = pdata_bak;
			return;
		}
		serve_numkey(*pkey_value);
		
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				pmenu_data->pData = pdata_bak;
				if(g_cur_val[pmenu_data->cur]>1)
				{
					g_cur_val[pmenu_data->cur] = 0;
				}
				
				if(invalid_flag)
				{
					if(pmenu_data == &menu_par[GUI_COM_STEPS_CONT])
					{
						g_cur_val[pmenu_data->cur] = 1;
					}
					else
					{
						g_cur_val[pmenu_data->cur] = 0;
					}
				}
				
// 				*(uint8_t*)pmenu_data->pData = g_cur_val[pmenu_data->cur];
                save_par_to_ram(pmenu_data, g_cur_val[pmenu_data->cur]);
//				g_cur_step->one_step = *p;
//				save_cur_step();
				break;
		}
		
		switch(*pkey_value)
		{
			case KEY_UP:
				pmenu_data = pmenu_data->prev;
				return;
			case KEY_DOWN:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_SHIFT:
				g_cur_val[pmenu_data->cur] = !g_cur_val[pmenu_data->cur];
				break;
			case KEY_ENTER:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			default:
				break;
		}
        if(g_exit)
        {
            return;
        }
	}
}

void set_float_par(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	int32_t invalid_flag = 0;
	void *pdata_bak = pmenu_data->pData;
    
	
	GUI_ClearRect(0,13,200-1,63);
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, 162, 33);
	GUI_DrawLine(130, 20, 130, 64-10);
	
	/* ��Χ ÿ���������в�ͬ�ķ�Χ��ʾ */
    switch(pmenu_data->range_flag)
    {
        case 0:
            GUI_GotoXY(145, 45);
            GUI_DispFloatFix ((float)pmenu_data->upper/pow(10, pmenu_data->decs), pmenu_data->lon, pmenu_data->decs);
            GUI_DispString((const char*)pmenu_data->unit[0]);
            GUI_GotoXY(145, 18);
            GUI_DispFloatFix ((float)pmenu_data->lower/pow(10, pmenu_data->decs), pmenu_data->lon, pmenu_data->decs);
            GUI_DispString((const char*)pmenu_data->unit[0]);
            break;
        case 1:
            GUI_DispStringAt((const char*)pmenu_data->upper_str, 145, 45);
            GUI_DispStringAt((const char*)pmenu_data->lower_str, 145, 18);
            break;
        case 2:
            /* ����迹 */
            if(pmenu_data->num == GUI_DCW_OUTPUT_IMPEDANCE)
            {
                uint8_t size = output_impedan_opt[type_spe.dcw_output_impedan_opt_index].size;
                const uint8_t * const* p = output_impedan_opt[type_spe.dcw_output_impedan_opt_index].opts;
                uint8_t x = 133;
                uint8_t y = 16;
                uint8_t o = 12;
                int32_t i = 0;
                
                if(size == 4)
                {
                    y = 16;
                }
                else if(size == 3)
                {
                    y = 20;
                }
                
                for(i = 0; i < size; i++)
                {
                    GUI_DispStringAt((const char*)p[i], x, y + o * i);
                }
//                 GUI_DispStringAt((const char*)pmenu_data->unit[1], 133, 16 + 12 * 0);
//                 GUI_DispStringAt((const char*)pmenu_data->unit[2], 133, 16 + 12 * 1);
//                 GUI_DispStringAt((const char*)pmenu_data->unit[3], 133, 16 + 12 * 2);
//                 GUI_DispStringAt((const char*)pmenu_data->unit[4], 133, 16 + 12 * 3);
            }
            break;
        case 3:
            GUI_GotoXY(145+6*3, 45);
            GUI_DispFloatFix ((float)pmenu_data->upper, pmenu_data->lon, pmenu_data->decs);
            GUI_GotoXY(145+6*3, 18);
            GUI_DispFloatFix ((float)pmenu_data->lower, pmenu_data->lon, pmenu_data->decs);
            break;
        case 4:
        {
            char temp_buf[20] = {0};
            
            my_sprintf(temp_buf, pmenu_data, pmenu_data->upper);
            GUI_DispStringAt_BTP_STR(temp_buf, 135, 200-1, 45);
            memset(temp_buf, 0, sizeof temp_buf);
            my_sprintf(temp_buf, pmenu_data, pmenu_data->lower);
            GUI_DispStringAt_BTP_STR(temp_buf, 135, 200-1, 18);
            break;
        }
    }
	
	/* �����ʵ�����Ƿ���Ч */
	if(pmenu_data == &menu_par[GUI_ACW_REAL_CUR] && type_spe.acw_real == 0)
	{
		invalid_flag = 1;
		gui_par_inval();
	}
	
	
	pmenu_data->pData = &g_cur_val[pmenu_data->cur];
	pmenu_data->ch_bit = 0;
	cursor_pos (pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
		
		server_combination_key(*pkey_value);
		if(g_skip_page)
		{
			g_skip_page = 0;
			pmenu_data->pData = pdata_bak;
			return;
		}
		serve_numkey(*pkey_value);
		
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				pmenu_data->pData = pdata_bak;
				if(g_cur_val[pmenu_data->cur] < pmenu_data->lower)
				{
					g_cur_val[pmenu_data->cur] = pmenu_data->lower;
				}
				else if(g_cur_val[pmenu_data->cur] > pmenu_data->upper)
				{
					g_cur_val[pmenu_data->cur] = pmenu_data->upper;
				}
				if(invalid_flag)
				{
					g_cur_val[pmenu_data->cur] = 0;
				}
                
                save_par_to_ram(pmenu_data, g_cur_val[pmenu_data->cur]);
//				g_cur_step->one_step = *p;
//				save_cur_step();
				break;
		}
		
		switch(*pkey_value)
		{
			case KEY_UP:
				pmenu_data = pmenu_data->prev;
				return;
			case KEY_DOWN:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_ENTER:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			default:
				break;
		}
        if(g_exit)
        {
            return;
        }
	}
}

void set_float_par_for_syn(void *p_node)
{
	NODE_STEP *p = (NODE_STEP *)p_node;
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	void *pdata_bak = pmenu_data->pData;
	
	p = p;
	
	GUI_ClearRect(0,13,200-1,63);
	memset(g_cur_info, 0, sizeof g_cur_info);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, 162, 33);
	GUI_DrawLine         (130, 20, 130, 64-10);
	
	gui_draw_set_sys_cur_limit();
	
	/* ��Χ */
	GUI_DispStringAt((const char*)pmenu_data->upper_str, 145, 45);
	GUI_DispStringAt((const char*)pmenu_data->lower_str, 145, 18);
	if(!pmenu_data->range_flag)
	{
		GUI_GotoXY(145, 45);
		GUI_DispFloatFix ((float)pmenu_data->upper/pow(10,pmenu_data->decs), pmenu_data->lon, pmenu_data->decs);
		GUI_GotoXY(145, 18);
		GUI_DispFloatFix ((float)pmenu_data->lower/pow(10,pmenu_data->decs), pmenu_data->lon, pmenu_data->decs);
	}
	
	pmenu_data->pData = &g_cur_val[pmenu_data->cur];
	pmenu_data->ch_bit = 0;
	cursor_pos (pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
		
		server_combination_key(*pkey_value);
		if(g_skip_page)
		{
			g_skip_page = 0;
			pmenu_data->pData = pdata_bak;
			return;
		}
		serve_numkey(*pkey_value);
		
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				pmenu_data->pData = pdata_bak;
				if(g_cur_val[pmenu_data->cur] < pmenu_data->lower)
				{
					g_cur_val[pmenu_data->cur] = pmenu_data->lower;
				}
				else if(g_cur_val[pmenu_data->cur] > pmenu_data->upper)
				{
					g_cur_val[pmenu_data->cur] = pmenu_data->upper;
				}
                
                save_par_to_ram(pmenu_data, g_cur_val[pmenu_data->cur]);
                
				break;
		}
		
		switch(*pkey_value)
		{
			case KEY_UP:
				pmenu_data = pmenu_data->prev;
				return;
			case KEY_DOWN:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_ENTER:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			default:
				break;
		}
		
		if(g_return || g_exit)
		{
			g_return = 0;
			pmenu_data->pData = pdata_bak;
			return;
		}
	}
}


void set_frequency(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value;
	INT8U err;
	const uint8_t *gear_buf[10] = {0};
	uint8_t flag[10] = {0};
	uint8_t kinds = 0;
	uint32_t i = 0;
	void *pdata_bak = pmenu_data->pData;
	
	memset(g_cur_info, 0, sizeof g_cur_info);
	GUI_ClearRect(0,13,200-1,63);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
	
	switch(p->com.mode)
	{
        default:
		case ACW:
			kinds = define_hz_kinds(ACW, gear_buf, flag);
			break;
		case GR:
			kinds = define_hz_kinds(GR, gear_buf, flag);
			break;
	}
	
	/* ������� */
	for(i = 0; i < kinds; i++)
	{
		if(flag[i] == g_cur_val[pmenu_data->cur])
		{
			g_cur_val[pmenu_data->cur] = i+1;/* ��Ӧ�ĵ�λת��Ϊflag��Ӧ���±�Ȼ��ͨ���޸��±�ȷ���µĵ�λ */
			break;
		}
	}
	
	if(i == kinds)
	{
		g_cur_val[pmenu_data->cur] = 1;
		p->acw.output_freq = flag[g_cur_val[pmenu_data->cur]-1];
//		g_cur_step->one_step = *p;
//		save_cur_step();
	}
	
	GUI_DrawLine(130, 20, 130, 64-10);
	
	pmenu_data->pData = &g_cur_val[pmenu_data->cur];
	pmenu_data->ch_bit = 0;
	cursor_pos (pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		if(g_cur_val[pmenu_data->cur] != 0)
		{
			acw_gr_hz_sel_gui(kinds, g_cur_val[pmenu_data->cur], gear_buf);
		}
		
		LCD_REFRESH();
		
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
		
		server_combination_key(*pkey_value);
		if(g_skip_page)
		{
			g_skip_page = 0;
			pmenu_data->pData = pdata_bak;
			return;
		}
		serve_numkey(*pkey_value);
		
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				pmenu_data->pData = pdata_bak;
				if(g_cur_val[pmenu_data->cur] > 0 && g_cur_val[pmenu_data->cur] <= kinds)
				{
					*(uint16_t*)pmenu_data->pData = g_cur_val[pmenu_data->cur];
				}
				
				g_cur_step->one_step = *p;
				save_cur_step();
				break;
		}
		
		switch(*pkey_value)
		{
			case KEY_UP:
				pmenu_data = pmenu_data->prev;
				return;
			case KEY_DOWN:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_ENTER:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			default:
				break;
		}
        if(g_exit)
        {
            return;
        }
	}
}

void set_option_par(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = 0;
	INT8U err = 0;
	void* pdata_bak = pmenu_data->pData;
	uint8_t flag = 0;/* ��־ */
    MORE_OPTIONS_LIST *list = pmenu_data->user_data;
    MORE_OPTIONS_STR *iteration = list->head;
    
    p_node = p_node;/* �ò���δ�� */
	GUI_ClearRect(0,13,200-1,63);
	
l:	
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
    
    /* ��λ */
    for(iteration = list->head; iteration->opt != g_cur_val[pmenu_data->cur]; iteration = iteration->next)
    {
        if(iteration == list->head)
        {
            if(++flag >= 2)
            {
                break;
            }
        }
    }
	
	while(1)
	{
		draw_more_options_gui(list, iteration);
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
		
		/* ����λ�����ò���ʱͬ���ı����� */
		if(test_flag.refresh_for_comm == 1)
		{
			test_flag.refresh_for_comm = 0;
			goto l;
		}
		
		server_combination_key(*pkey_value);
		if(g_skip_page)
		{
			g_skip_page = 0;
			pmenu_data->pData = pdata_bak;
			return;
		}
        
		serve_numkey(*pkey_value);
		
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				pmenu_data->pData = pdata_bak;
                pmenu_data->unit[0] = iteration->name[0];
                pmenu_data->unit[1] = iteration->name[1];
                
                save_par_to_ram(pmenu_data, iteration->opt);
//				g_cur_step->one_step = *p;
//				save_cur_step();
				break;
		}
		
		switch(*pkey_value)
		{
			case KEY_UP:
				pmenu_data = pmenu_data->prev;
				if(pmenu_data->num == GUI_SYS_CHECK_DATE)
				{
					pmenu_data = pmenu_data->prev;
				}
				return;
			case KEY_DOWN:
				pmenu_data = pmenu_data->next;
				if(pmenu_data->num == GUI_SYS_CHECK_DATE)
				{
					pmenu_data = pmenu_data->next;
				}
				return;
			case KEY_SHIFT:
                iteration = iteration->next;
				break;
			case KEY_ENTER:
				pmenu_data = pmenu_data->next;
				return;
			case KEY_BACK:
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


/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
