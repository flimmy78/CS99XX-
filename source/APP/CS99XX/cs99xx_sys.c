/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_sys.c
 * 摘  要  ：系统参数设置管理
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#include	<string.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"RTC_Config.h"
#include    "cs99xx_type.h"

void set_float_sys_par(void *psys)
{
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	void *pdata_bak = pmenu_data->pData;
	
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	GUI_ClearRect(0,0,200-1,63);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
    
    GUI_DispStringAt((const char*)pmenu_data->upper_str, pmenu_data->x + 6 * 7, pmenu_data->y);
	
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
				else if(g_cur_val[pmenu_data->cur] < pmenu_data->lower)
				{
					g_cur_val[pmenu_data->cur] = pmenu_data->lower;
				}
                
                save_par_to_ram(pmenu_data, g_cur_val[pmenu_data->cur]);
                save_sys_par();
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
void set_either_or_sys_par(void *psys)
{
	SYS_PAR *p = psys;
	uint16_t *pkey_value = 0;
	INT8U err = 0;
	int32_t i = 0;
	void* pdata_bak = pmenu_data->pData;
	
	p = p;
	GUI_ClearRect(0,0,200-1,63);
	
l:	
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
	
 	/* 范围 */
	for(i = 1; i < 5; i++)
	{
		if(g_cur_info[i] && g_cur_info[i]->type == INT_T)
		{
			GUI_DispStringAt((const char*)g_cur_info[i]->upper_str, 140, g_cur_info[i]->y);
		}
	}
	
	pmenu_data->pData = &g_cur_val[pmenu_data->cur];
	
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
		
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
		
		/* 当上位机设置参数时同步改变数据 */
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
				if(g_cur_val[pmenu_data->cur]>1)
				{
					g_cur_val[pmenu_data->cur] = 0;
				}
// 				*(int8_t*)pmenu_data->pData = g_cur_val[pmenu_data->cur];
                save_par_to_ram(pmenu_data, g_cur_val[pmenu_data->cur]);
				save_sys_par();
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
	}
}

/*
 * 函数名：set_option_sys_par
 * 描述  ：设置系统多选参数
 * 输入  ：n 标记当前是偏移还是测试 0 测试 1偏移
 * 输出  ：无
 * 返回  ：无
 */
void set_option_sys_par(void *psys)
{
	SYS_PAR *p = psys;
	uint16_t *pkey_value = 0;
	INT8U err = 0;
    int32_t i = 0;
	void* pdata_bak = pmenu_data->pData;
    
    uint8_t options = 0;
	const uint8_t *fail_mode_buf[FAIL_MODE_END] = {0};
	uint8_t flag[FAIL_MODE_END] = {0};/* 标志 */
    
	p = p;
    
    
	GUI_ClearRect(0,0,200-1,63);
	
l:	
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
    
	options = define_fail_mode_kinds(fail_mode_buf, flag);
	
	for(i = 0; i < options; i++)
	{
		if(flag[i] == g_cur_val[pmenu_data->cur])
		{
			g_cur_val[pmenu_data->cur] = i;/* 对应的档位转换为flag对应的下标然后通过修改下标确定新的档位 */
			break;
		}
	}
	
	if(i == options)
    {
        g_cur_val[pmenu_data->cur] = 0;
        menu_par[GUI_SYS_FAIL_MODE].unit[0]	= fail_mode_buf[g_cur_val[pmenu_data->cur]];
        menu_par[GUI_SYS_FAIL_MODE].unit[1]	= fail_mode_buf[g_cur_val[pmenu_data->cur]];
    }
    
	pmenu_data->pData = &g_cur_val[pmenu_data->cur];
	
	while(1)
	{
        sys_option_sel_gui(options, fail_mode_buf, g_cur_val[pmenu_data->cur]);
		
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
		
		/* 当上位机设置参数时同步改变数据 */
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
				if(g_cur_val[pmenu_data->cur] >= options)
				{
					g_cur_val[pmenu_data->cur] = 0;
				}
                
                if(read_par_to_buf(&menu_par[GUI_SYS_FAIL_CONT], NULL) == 1)
                {
                    save_par_to_ram(&menu_par[GUI_SYS_FAIL_MODE], FAIL_MODE_CON);
                    menu_par[GUI_SYS_FAIL_MODE].unit[0]       = fail_mode_pool[FAIL_MODE_CON];
                    menu_par[GUI_SYS_FAIL_MODE].unit[1]       = fail_mode_pool[FAIL_MODE_CON];
                }
                else
                {
                    save_par_to_ram(pmenu_data, flag[g_cur_val[pmenu_data->cur]]);
                    menu_par[GUI_SYS_FAIL_MODE].unit[0]       = fail_mode_buf[g_cur_val[pmenu_data->cur]];
                    menu_par[GUI_SYS_FAIL_MODE].unit[1]       = fail_mode_buf[g_cur_val[pmenu_data->cur]];
                }
                
				save_sys_par();
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
                if(++g_cur_val[pmenu_data->cur] >= options)
                {
                    g_cur_val[pmenu_data->cur] = 0;
                }
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
void set_plc_option_sys_par(void *psys)
{
	SYS_PAR *p = psys;
	uint16_t *pkey_value = 0;
	INT8U err = 0;
	void* pdata_bak = pmenu_data->pData;
	uint8_t flag = 0;/* 标志 */
    
    MORE_OPTIONS_LIST *list = pmenu_data->user_data;
    MORE_OPTIONS_STR *iteration = list->head;
    
	p = p;
    
	GUI_ClearRect(0,0,200-1,63);
	
l:	
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
    
    /* 定位 */
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
// 	options = define_fail_mode_kinds(fail_mode_buf, flag);
//     plc_signal_options
	
// 	for(i = 0; i < options; i++)
// 	{
// 		if(flag[i] == g_cur_val[pmenu_data->cur])
// 		{
// 			g_cur_val[pmenu_data->cur] = i;/* 对应的档位转换为flag对应的下标然后通过修改下标确定新的档位 */
// 			break;
// 		}
// 	}
// 	
// 	if(i == options)
//     {
//         g_cur_val[pmenu_data->cur] = 0;
//         menu_par[GUI_SYS_FAIL_MODE].unit[0]       = fail_mode_buf[g_cur_val[pmenu_data->cur]];
//         menu_par[GUI_SYS_FAIL_MODE].unit[1]       = fail_mode_buf[g_cur_val[pmenu_data->cur]];
//     }
    
// 	pmenu_data->pData = &g_cur_val[pmenu_data->cur];
	
	while(1)
	{
		draw_more_options_gui(list, iteration);
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
		
		/* 当上位机设置参数时同步改变数据 */
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
				save_sys_par();
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

void set_int_sys_par(void *pun)
{
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	uint32_t i = 0;
	void *pdata_bak = pmenu_data->pData;
	
l:	
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	GUI_ClearRect(0,0,200-1,63);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
	
 	/* 范围 */
	for(i = 1; i < 5; i++)
	{
		if(g_cur_info[i] && g_cur_info[i]->type == INT_T)
		{
			GUI_DispStringAt((const char*)g_cur_info[i]->upper_str, 140, g_cur_info[i]->y);
		}
	}
	
	pmenu_data->pData = &g_cur_val[pmenu_data->cur];
	pmenu_data->ch_bit = 0;
	cursor_pos (pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
		
		/* 当上位机设置参数时同步改变数据 */
		if(test_flag.refresh_for_comm == 1)
		{
			test_flag.refresh_for_comm = 0;
			goto l;
		}
		
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
				pmenu_data->pData = pdata_bak;
				if(pmenu_data->num != GUI_SYS_CHECK_DATE)
				{
					if(g_cur_val[pmenu_data->cur] < pmenu_data->lower)
					{
						g_cur_val[pmenu_data->cur] = pmenu_data->lower;
					}
					else if(g_cur_val[pmenu_data->cur] > pmenu_data->upper)
					{
						g_cur_val[pmenu_data->cur] = pmenu_data->upper;
					}
					
// 					*(uint8_t*)pmenu_data->pData = g_cur_val[pmenu_data->cur];
                    save_par_to_ram(pmenu_data, g_cur_val[pmenu_data->cur]);
					save_sys_par();
				}
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
	}
}

/*
 * 函数名：prompt_for_check_data
 * 描述  ：当检验人员成功更新检验时间时弹出OK提示窗 每次开机仅能修改一次
 * 输入  ：无
 * 输出  ：无
 */
void prompt_for_check_data(void)
{
	#define X0	40
	#define Y0	5
	#define X1	170
	#define Y1	57
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);

	GUI_DispStringAt("检验时间设置OK!", 0+X0+35, Y0+5+15);
	GUI_DrawRect(X0, Y0, X1, Y1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, X0+3, Y0+5);
	
	LCD_REFRESH();
    
	OSTimeDlyHMSM(0,0,1,500);
	GUI_ClearRect (X0, Y0, X1, Y1);
	
	#undef X0
	#undef Y0
	#undef X1
	#undef Y1
}

void sys_check_date(void)
{
	GetTime();
	strcpy(sys_par.data_of_survey, (const char*)gUpdateTime);
	GUI_DispStringAt((const char*)sys_par.data_of_survey,5+75, 3);
	prompt_for_check_data();
	save_sys_par();
	g_return = 1;
}
// #AUTO_TYPE_CHANGE()     (uint8_t*)
void sys_sys(uint32_t w)
{
    draw_short_cut_bm(UDRB);
	init_sys_par_list(&sys_par);
	pmenu_data = par_info_list.head;
	colon_x = 5+6*10;/* 确定参数X坐标的位置 */
    
	for(;;)
	{
		pmenu_data->fun(&sys_par);
		
		if(read_par_to_buf(&menu_par[GUI_SYS_TEST_PORT], NULL) == GND_MODE)
		{
            save_par_to_ram(&menu_par[GUI_SYS_GFI_PROT], GFI_OFF);
            save_sys_par();
		}
        
        /* 失败继续打开，会影响的失败模式只能为 FAIL_MODE_CON */
        if(read_par_to_buf(&menu_par[GUI_SYS_FAIL_CONT], NULL) == 1)
        {
            save_par_to_ram(&menu_par[GUI_SYS_FAIL_MODE], FAIL_MODE_CON);
            menu_par[GUI_SYS_FAIL_MODE].unit[0]       = fail_mode_pool[FAIL_MODE_CON];
            menu_par[GUI_SYS_FAIL_MODE].unit[1]       = fail_mode_pool[FAIL_MODE_CON];
            save_sys_par();
        }
        /* 失败继续关闭 */
        else// if(read_par_to_buf(&menu_par[GUI_SYS_FAIL_CONT], NULL) != 1)
        {
            /* 失败继续关闭 失败模式就不应该等于 FAIL_MODE_CON */
            if(read_par_to_buf(&menu_par[GUI_SYS_FAIL_MODE], NULL) == FAIL_MODE_CON)
            {
                save_par_to_ram(&menu_par[GUI_SYS_FAIL_MODE], FAIL_MODE_STOP);
                menu_par[GUI_SYS_FAIL_MODE].unit[0]       = fail_mode_pool[FAIL_MODE_STOP];
                menu_par[GUI_SYS_FAIL_MODE].unit[1]       = fail_mode_pool[FAIL_MODE_STOP];
            }
            
            /* 失败继续关闭 PLC信号默认值为0 不可设 */
//             if(read_par_to_buf(&menu_par[GUI_SYS_PLC_SIGNAL], NULL) == ALL_STEP)
//             {
//                 save_par_to_ram(&menu_par[GUI_SYS_FAIL_MODE], FAIL_MODE_STOP);
//             }
//             save_par_to_ram(&menu_par[GUI_SYS_PLC_SIGNAL], 0);
            save_sys_par();
        }
        
		if(g_return || g_exit)
		{
			return;
		}
		
		if(app_flag.goto_menu_page)
		{
			DATA_INFO*	p_temp_info = NULL;
			
			p_temp_info = find_menu(app_flag.goto_menu_page);
			app_flag.goto_menu_page = 0;
			if(p_temp_info != NULL)
			{
				pmenu_data = p_temp_info;
			}
		}
	}
}


/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
