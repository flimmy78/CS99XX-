/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_par_gr.c
 * 摘  要  ：设置GR的参数函数
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
 
#include	<string.h>
#include 	<math.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"

void dir_gr_range_str(uint32_t v_h, uint32_t v_l)
{
    uint8_t lon = pmenu_data->lon - pmenu_data->no_dis_bits;
    uint8_t decs = pmenu_data->decs - pmenu_data->no_dis_bits;
    uint8_t unit = type_spe.gr_cur_unit;
    
    GUI_ClearRect(130,15,200-1,63);
    
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, 162, 33);
	GUI_DrawLine         (130, 20, 130, 64-10);
    GUI_GotoXY(145, 45);
    GUI_DispFloatFix ((float)v_h/pow(10,pmenu_data->decs), lon, decs);
    GUI_DispString((const char*)unit_pool[unit]);
    GUI_GotoXY(145, 18);
    GUI_DispFloatFix ((float)v_l/pow(10,pmenu_data->decs), lon, decs);
    GUI_DispString((const char*)unit_pool[unit]);
}
void set_gr_output_cur(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	uint16_t vol_ele_bak = 0;
	
	uint32_t vol_cur_h	= 0; /* 电压或电流上限 */
	uint32_t vol_cur_l	= 0; /* 电压或电流下限 */
	
	//当从第二页返回时需要重回第一页的背景
	GUI_ClearRect(0,13,200-1,63);
	
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	
	draw_colon();
	
	vol_cur_h = GR_CUR_H;
	vol_cur_l = GR_CUR_L;
	vol_ele_bak = p->gr.testing_cur;
	
	/* 范围 */
    dir_gr_range_str(vol_cur_h, vol_cur_l);
	
	pmenu_data->pData = &g_cur_val[pmenu_data->cur];
	pmenu_data->ch_bit = 0;
	cursor_pos(pmenu_data->x,pmenu_data->y);
    
    dis_cursor_pos();
    
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		server_combination_key(*pkey_value);
		if(g_skip_page)
		{
			g_skip_page = 0;
			return;
		}
		serve_numkey(*pkey_value);
		
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				GUI_ClearRect (120, 15, 200-1, 63);
				
				if(g_cur_val[pmenu_data->cur] < GR_CUR_L)
				{
					g_cur_val[pmenu_data->cur] = GR_CUR_L;
				}
				else if(g_cur_val[pmenu_data->cur] > GR_CUR_H)
				{
					g_cur_val[pmenu_data->cur] = GR_CUR_H;
				}
				
				p->gr.testing_cur = g_cur_val[pmenu_data->cur];
// 				updata_vol_ele(p,pmenu_data);
				
				/* 当电压或电流改变时偏移值清零 */
				if(g_cur_val[pmenu_data->cur] != vol_ele_bak)
				{
					init_offset_par(p_node);
				}
				
				g_cur_step->one_step = *p;
				save_cur_step();
				pmenu_data->pData = &p->gr.testing_cur;
				break;
		}
		
        if(0 != count_gr_cur_no_dis_bits(g_cur_val[pmenu_data->cur]))
        {
            dir_gr_range_str(vol_cur_h, vol_cur_l);
            load_this_page_info(g_cur_info);
            load_this_page_value();
            dis_cursor_pos();
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
			return;
		}
	}
}
void set_gr_res_upper(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	uint32_t temp_cur = p->gr.testing_cur;
	uint32_t temp_r_ulimit = GR_RES_H(temp_cur);/* 计算上限电阻最大值 最大电流32A */
	
	GUI_ClearRect(0,13,200-1,63);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	
	if(temp_r_ulimit > 5100)
	{
		temp_r_ulimit = 5100;
	}
	
	/* 检查参数合法性 */
	if(g_cur_val[pmenu_data->cur] > temp_r_ulimit)
	{
		g_cur_val[pmenu_data->cur] = temp_r_ulimit;
		p->gr.upper_limit = temp_r_ulimit;
		g_cur_step->one_step.gr.upper_limit = temp_r_ulimit;
	}
	
	//显示取值范围
	GUI_DispStringAt("001.0mΩ", 145, 18);
	GUI_DispStringAt("510.0mΩ", 145, 45);
	
	GUI_GotoXY(145, 45);
	GUI_DispFloatFix ((float)temp_r_ulimit/10, 5, 1);
	
	GUI_DrawLine         (130, 20, 130, 64-10);
	draw_colon();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, 162, 33);
	
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
			return;
		}
		serve_numkey(*pkey_value);
		
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				if(g_cur_val[pmenu_data->cur] < 10)
				{
					g_cur_val[pmenu_data->cur] = 10;
				}
				else if(g_cur_val[pmenu_data->cur] > temp_r_ulimit)
				{
					g_cur_val[pmenu_data->cur] = temp_r_ulimit;
				}
				
				p->gr.upper_limit = g_cur_val[pmenu_data->cur];
				g_cur_step->one_step = *p;
				save_cur_step();
				pmenu_data->pData = &p->gr.upper_limit;
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

void set_gr_res_lower(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	uint16_t temp_r_u = p->gr.upper_limit;
	
	GUI_ClearRect(0,13,200-1,63);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	
	
	//显示取值范围
	GUI_DispStringAt("000.0mΩ", 145, 18);
	GUI_DispStringAt("510.0mΩ", 145, 45);
	GUI_GotoXY(145, 45);
	GUI_DispFloatFix ((float)temp_r_u/10, 5, 1);
	GUI_DrawLine(130, 20, 130, 64-10);
	draw_colon();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, 162, 33);
	
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
			return;
		}
        
		serve_numkey(*pkey_value);
		
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				if(g_cur_val[pmenu_data->cur] > temp_r_u)
				{
					g_cur_val[pmenu_data->cur] = temp_r_u;
				}
				
				p->gr.lower_limit = g_cur_val[pmenu_data->cur];
				g_cur_step->one_step = *p;
				save_cur_step();
				pmenu_data->pData = &p->gr.lower_limit;
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



void set_gr_vol_upper(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
// 	uint16_t temp_vol_ulimit = GR_RES_H;
// 	uint16_t temp_vol_llimit = GR_RES_L;
	
	GUI_ClearRect(0,13,200-1,63);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
	
	/* 检查参数合法性 */
	if(g_cur_val[pmenu_data->cur] > pmenu_data->upper)
	{
		g_cur_val[pmenu_data->cur] = pmenu_data->upper;
		p->gr.upper_limit = g_cur_val[pmenu_data->cur];
		g_cur_step->one_step = *p;
		save_cur_step();
	}
	
	/* 范围 */
	GUI_DispStringAt((const char*)pmenu_data->upper_str, 145, 45);
	GUI_DispStringAt((const char*)pmenu_data->lower_str, 145, 18);
	GUI_GotoXY(145, 45);
	GUI_DispFloatFix ((float)pmenu_data->upper/pow(10,menu_par[GUI_GR_VOL_H].decs), menu_par[GUI_GR_VOL_H].lon, menu_par[GUI_GR_VOL_H].decs);
	GUI_GotoXY(145, 18);
	GUI_DispFloatFix ((float)pmenu_data->lower/pow(10,menu_par[GUI_GR_VOL_H].decs), menu_par[GUI_GR_VOL_H].lon, menu_par[GUI_GR_VOL_H].decs);
	
	GUI_DrawLine         (130, 20, 130, 64-10);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 80, 56);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, 162, 33);
	
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
			return;
		}
		serve_numkey(*pkey_value);
		
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				if(g_cur_val[pmenu_data->cur] < pmenu_data->lower)
				{
					g_cur_val[pmenu_data->cur] = pmenu_data->lower;
				}
				else if(g_cur_val[pmenu_data->cur] > pmenu_data->upper)
				{
					g_cur_val[pmenu_data->cur] = pmenu_data->upper;
				}
				
				p->gr.upper_limit = g_cur_val[pmenu_data->cur];
				g_cur_step->one_step = *p;
				save_cur_step();
				pmenu_data->pData = &p->gr.upper_limit;
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
void set_gr_vol_lower(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	uint16_t temp_vol_u = p->gr.upper_limit;
	uint16_t temp_vol_llimit = GR_RES_L;
	
	GUI_ClearRect(0,13,200-1,63);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
	
	/* 检查参数合法性 */
	if(g_cur_val[pmenu_data->cur] > temp_vol_u)
	{
		g_cur_val[pmenu_data->cur] = temp_vol_u;
		p->gr.lower_limit = g_cur_val[pmenu_data->cur];
		g_cur_step->one_step = *p;
		save_cur_step();
	}
	
	GUI_DrawLine(130, 20, 130, 64-10);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, 162, 33);
	
	/* 范围 */
	GUI_DispStringAt((const char*)pmenu_data->upper_str, 145, 45);
	GUI_DispStringAt((const char*)pmenu_data->lower_str, 145, 18);
	GUI_GotoXY(145, 45);
	GUI_DispFloatFix ((float)temp_vol_u/pow(10,menu_par[GUI_GR_VOL_H].decs), menu_par[GUI_GR_VOL_H].lon, menu_par[GUI_GR_VOL_H].decs);
	GUI_GotoXY(145, 18);
	GUI_DispFloatFix ((float)temp_vol_llimit/pow(10,menu_par[GUI_GR_VOL_H].decs), menu_par[GUI_GR_VOL_H].lon, menu_par[GUI_GR_VOL_H].decs);
	
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
			return;
		}
		serve_numkey(*pkey_value);
		
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				if(g_cur_val[pmenu_data->cur] > temp_vol_u)
				{
					g_cur_val[pmenu_data->cur] = temp_vol_u;
				}
				
				p->gr.lower_limit = g_cur_val[pmenu_data->cur];
				g_cur_step->one_step = *p;
				save_cur_step();
				pmenu_data->pData = &p->gr.lower_limit;/* 指针要重新指向它 */
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

void updata_gr_cur_affect_par_info(UN_STRUCT *p)
{
    menu_par[GUI_GR_OUTPUT_CUR].upper = GR_CUR_H;
    menu_par[GUI_GR_OUTPUT_CUR].lower = GR_CUR_L;
}

void updata_gr_vol_gear_affect_par_info(UN_STRUCT *p)
{
	uint8_t temp = *(uint8_t*)menu_par[GUI_GR_TEST_METHOD].pData;
	switch(temp)
	{
		case 1:
			menu_par[GUI_GR_TEST_METHOD].unit[0] = "连续";
			menu_par[GUI_GR_TEST_METHOD].unit[1] = "Cont.";
			menu_par[GUI_GR_TEST_METHOD].unit[2] = "脉冲";
			menu_par[GUI_GR_TEST_METHOD].unit[3] = "Pulse";
			break;
		case 0:
			menu_par[GUI_GR_TEST_METHOD].unit[0] = "脉冲";
			menu_par[GUI_GR_TEST_METHOD].unit[1] = "Pulse";
			menu_par[GUI_GR_TEST_METHOD].unit[2] = "连续";
			menu_par[GUI_GR_TEST_METHOD].unit[3] = "Cont.";
			break;
	}
	
	menu_par[GUI_COM_MODE].unit[0] = "GR";
	menu_par[GUI_COM_MODE].unit[1] = "GR";
    
	switch(p->gr.voltage_gear)
	{
		case 0:
        {
			menu_par[GUI_GR_VOL_GEAR].unit[0] = "20mV";
			menu_par[GUI_GR_VOL_GEAR].unit[1] = "20mV";
			
			menu_par[GUI_GR_OUTPUT_CUR].unit[0] = "A";
			menu_par[GUI_GR_OUTPUT_CUR].decs = 2;
			menu_par[GUI_GR_OUTPUT_CUR].upper_str = "40.00A";
			menu_par[GUI_GR_OUTPUT_CUR].lower_str = "03.00A";
			
			menu_par[GUI_GR_VOL_H].upper_str = "20.00mV";
			menu_par[GUI_GR_VOL_H].lower_str = "00.01mV";
			menu_par[GUI_GR_VOL_L].upper_str = "00.00mV";
			menu_par[GUI_GR_VOL_L].lower_str = "00.00mV";
			menu_par[GUI_GR_VOL_H].unit[0] = "mV";
			menu_par[GUI_GR_VOL_L].unit[0] = "mV";
			menu_par[GUI_GR_VOL_H].decs = 2;
			menu_par[GUI_GR_VOL_L].decs = 2;
			menu_par[GUI_GR_VOL_H].lon = 5;
			menu_par[GUI_GR_VOL_L].lon = 5;
			menu_par[GUI_GR_VOL_H].type = 1;
			menu_par[GUI_GR_VOL_L].type = 1;
			break;
        }
		case 1:
        {
			menu_par[GUI_GR_VOL_GEAR].unit[0] = "20mV";
			menu_par[GUI_GR_VOL_GEAR].unit[1] = "20mV";
			
			menu_par[GUI_GR_OUTPUT_CUR].unit[0] = "A";
			menu_par[GUI_GR_OUTPUT_CUR].decs = 2;
			menu_par[GUI_GR_OUTPUT_CUR].upper_str = "40.00A";
			menu_par[GUI_GR_OUTPUT_CUR].lower_str = "03.00A";
			
			menu_par[GUI_GR_VOL_H].upper		= 2000;
			menu_par[GUI_GR_VOL_H].lower		= 1;
			menu_par[GUI_GR_VOL_H].upper_str = "20.00mV";
			menu_par[GUI_GR_VOL_H].lower_str = "00.01mV";
			menu_par[GUI_GR_VOL_L].upper_str = "00.00mV";
			menu_par[GUI_GR_VOL_L].lower_str = "00.00mV";
			menu_par[GUI_GR_VOL_H].unit[0] = "mV";
			menu_par[GUI_GR_VOL_L].unit[0] = "mV";
			menu_par[GUI_GR_VOL_H].decs = 2;
			menu_par[GUI_GR_VOL_L].decs = 2;
			menu_par[GUI_GR_VOL_H].lon = 5;
			menu_par[GUI_GR_VOL_L].lon = 5;
			menu_par[GUI_GR_VOL_H].type = 1;
			menu_par[GUI_GR_VOL_L].type = 1;
			break;
        }
		case 2:
        {
			menu_par[GUI_GR_VOL_GEAR].unit[0] = "200mV";
			menu_par[GUI_GR_VOL_GEAR].unit[1] = "200mV";
			
			menu_par[GUI_GR_OUTPUT_CUR].unit[0] = "A";
			menu_par[GUI_GR_OUTPUT_CUR].decs = 2;
			menu_par[GUI_GR_OUTPUT_CUR].upper_str = "40.00A";
			menu_par[GUI_GR_OUTPUT_CUR].lower_str = "03.00A";
			
			menu_par[GUI_GR_VOL_H].upper		= 2000;
			menu_par[GUI_GR_VOL_H].lower		= 1;
			menu_par[GUI_GR_VOL_H].upper_str = "200.0mV";
			menu_par[GUI_GR_VOL_H].lower_str = "000.1mV";
			menu_par[GUI_GR_VOL_L].upper_str = "000.0mV";
			menu_par[GUI_GR_VOL_L].lower_str = "000.0mV";
			menu_par[GUI_GR_VOL_H].unit[0] = "mV";
			menu_par[GUI_GR_VOL_L].unit[0] = "mV";
			menu_par[GUI_GR_VOL_H].decs = 1;
			menu_par[GUI_GR_VOL_L].decs = 1;
			menu_par[GUI_GR_VOL_H].lon = 5;
			menu_par[GUI_GR_VOL_L].lon = 5;
			menu_par[GUI_GR_VOL_H].type = 1;
			menu_par[GUI_GR_VOL_L].type = 1;
			break;
        }
		case 3:
        {
			menu_par[GUI_GR_VOL_GEAR].unit[0] = "2000mV";
			menu_par[GUI_GR_VOL_GEAR].unit[1] = "2000mV";
			
			menu_par[GUI_GR_OUTPUT_CUR].unit[0] = "A";
			menu_par[GUI_GR_OUTPUT_CUR].decs = 2;
			menu_par[GUI_GR_OUTPUT_CUR].upper_str = "40.00A";
			menu_par[GUI_GR_OUTPUT_CUR].lower_str = "03.00A";
			
			menu_par[GUI_GR_VOL_H].upper		= 2000;
			menu_par[GUI_GR_VOL_H].lower		= 1;
			menu_par[GUI_GR_VOL_H].upper_str = "2000mV";
			menu_par[GUI_GR_VOL_H].lower_str = "0001mV";
			menu_par[GUI_GR_VOL_L].upper_str = "0000mV";
			menu_par[GUI_GR_VOL_L].lower_str = "0000mV";
			menu_par[GUI_GR_VOL_H].unit[0] = "mV";
			menu_par[GUI_GR_VOL_L].unit[0] = "mV";
			menu_par[GUI_GR_VOL_H].decs = 0;
			menu_par[GUI_GR_VOL_L].decs = 0;
			menu_par[GUI_GR_VOL_H].lon = 4;
			menu_par[GUI_GR_VOL_L].lon = 4;
			menu_par[GUI_GR_VOL_H].type = 0;
			menu_par[GUI_GR_VOL_L].type = 0;
			break;
        }
		case 4:
        {
			menu_par[GUI_GR_VOL_GEAR].unit[0] = "12V";
			menu_par[GUI_GR_VOL_GEAR].unit[1] = "12V";
			
			menu_par[GUI_GR_OUTPUT_CUR].unit[0] = "mA";
			menu_par[GUI_GR_OUTPUT_CUR].decs = 1;
			menu_par[GUI_GR_OUTPUT_CUR].upper_str = "500.0mA";
			menu_par[GUI_GR_OUTPUT_CUR].lower_str = "100.0mA";
			
			menu_par[GUI_GR_VOL_H].upper		= 1200;
			menu_par[GUI_GR_VOL_H].lower		= 1;
			menu_par[GUI_GR_VOL_H].upper_str = "12.00V";
			menu_par[GUI_GR_VOL_H].lower_str = "00.01V";
			menu_par[GUI_GR_VOL_L].upper_str = "12.00V";
			menu_par[GUI_GR_VOL_L].lower_str = "00.10V";
			menu_par[GUI_GR_VOL_H].unit[0] = "V";
			menu_par[GUI_GR_VOL_L].unit[0] = "V";
			menu_par[GUI_GR_VOL_H].decs = 2;
			menu_par[GUI_GR_VOL_L].decs = 2;
			menu_par[GUI_GR_VOL_H].lon = 5;
			menu_par[GUI_GR_VOL_L].lon = 5;
			menu_par[GUI_GR_VOL_H].type = 1;
			menu_par[GUI_GR_VOL_L].type = 1;
			
			menu_par[GUI_COM_TEST_T].upper_str = "999.9s";
			menu_par[GUI_COM_TEST_T].lower_str = "0,0.4s";
			menu_par[GUI_COM_TEST_T].lower = 4;
			break;
        }
	}
}

void set_gr_vol_gear(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	uint8_t m = p->gr.voltage_gear;
	
	uint32_t vol_kinds = 0;/* 电流的种类 */
	const uint8_t *gear_buf[VOL_KINDS_MAX] = {0};/* 电流档位 */
	uint8_t flag[VOL_KINDS_MAX] = {0};/* 标志 */
	int32_t i = 0;
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
	GUI_ClearRect (64, 28, 200-1, 28+10);
	
	vol_kinds = define_vol_kinds(GR, gear_buf, flag);
	
	for(i = 0; i < vol_kinds; i++)
	{
		if(flag[i] == g_cur_val[pmenu_data->cur])
		{
			g_cur_val[pmenu_data->cur] = i+1;
			break;
		}
	}
	
	/* 如果没有找到对应的电压档 */
	if(i == vol_kinds)
	{
		g_cur_val[pmenu_data->cur] = 1;
		p->gr.voltage_gear = flag[g_cur_val[pmenu_data->cur]-1];
		
		p->gr.upper_limit = 500;
		p->gr.lower_limit = 0;
		
		g_cur_step->one_step = *p;
		save_cur_step();
	}
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	
// 	switch(vol_kinds)
// 	{
// 		case 1:
// 			GUI_DispStringAt(gear_buf[0], 4+70, 28);
// 			break;
// 		case 2:
// 			GUI_DispStringAt(gear_buf[0], 4+70, 28);
// 			GUI_DispStringAt(gear_buf[1], 4+70+40, 28);
// 			break;
// 		case 3:
// 			GUI_DispStringAt(gear_buf[0], 4+70, 28);
// 			GUI_DispStringAt(gear_buf[1], 4+70+40, 28);
// 			GUI_DispStringAt(gear_buf[2], 4+70+40+45, 28);
// 			break;
// 	}
	
	while(1)
	{	
// 		gr_vol_sel_gui(vol_kinds, g_cur_val[pmenu_data->cur]);
		gr_vol_sel_gui(vol_kinds, gear_buf, g_cur_val[pmenu_data->cur]);
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		
		server_combination_key(*pkey_value);
		
		if(g_skip_page)
		{
			g_skip_page = 0;
			return;
		}
        
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				p->gr.voltage_gear = flag[g_cur_val[pmenu_data->cur]-1];
				updata_gr_vol_gear_affect_par_info(p);
				
				if(m != flag[g_cur_val[pmenu_data->cur]-1])
				{
					p->gr.upper_limit = 500;
					p->gr.lower_limit = 0;
				}
				
				g_cur_step->one_step = *p;
				GUI_ClearRect(60,15,200-1,28);
				save_cur_step();
// 				pmenu_data->pData = &p->gr.voltage_gear;
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
				if(++g_cur_val[pmenu_data->cur] > vol_kinds)
				{
					g_cur_val[pmenu_data->cur] = 1;
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
			case KEY_1:
				if(vol_kinds >= 1)
				{
					g_cur_val[pmenu_data->cur] = 1;
				}
				break;
			case KEY_2:
				if(vol_kinds >= 2)
				{
					g_cur_val[pmenu_data->cur] = 2;
				}
				break;
			case KEY_3:
				if(vol_kinds >= 3)
				{
					g_cur_val[pmenu_data->cur] = 3;
				}
				break;
		}
        if(g_exit)
        {
            return;
        }
	}
}

void set_gr_test_method(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	
	memset(g_cur_info, 0, sizeof g_cur_info);
	GUI_ClearRect(0,13,200-1,63);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	memset(g_cur_info, 0, sizeof g_cur_info);
	memset(g_cur_val, 0, sizeof g_cur_val);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
	
	if(pmenu_data->type == OPTION_T)
	{
		GUI_ClearRect(pmenu_data->x, pmenu_data->y, pmenu_data->x+10+8*2, pmenu_data->y+12);
		if(sys_par.language == CHINESE)
		{
			GUI_DispStringAt((const char*)"连续",pmenu_data->x+10, pmenu_data->y);
			GUI_DispStringAt((const char*)"脉冲",pmenu_data->x+10+pmenu_data->x_offset, pmenu_data->y);
		}
		else
		{
			GUI_DispStringAt((const char*)"Cont.",pmenu_data->x+10, pmenu_data->y);
			GUI_DispStringAt((const char*)"Pulse",pmenu_data->x+10+pmenu_data->x_offset, pmenu_data->y);
		}
	}
	
	/* 数据检查 */
	if(g_cur_val[pmenu_data->cur] > 1)
	{
		g_cur_val[pmenu_data->cur] = 0;
		p->gr.test_method = g_cur_val[pmenu_data->cur];
		
		g_cur_step->one_step = *p;
		save_cur_step();
	}
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	pmenu_data->pData = &g_cur_val[pmenu_data->cur];
	
	while(1)
	{
		switch(g_cur_val[pmenu_data->cur])
		{
			case 1:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,pmenu_data->x, pmenu_data->y+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,pmenu_data->x+pmenu_data->x_offset, pmenu_data->y+pmenu_data->y_offset);
				cursor_pos (pmenu_data->x+1,pmenu_data->y);
				break;
			case 0:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,pmenu_data->x, pmenu_data->y+pmenu_data->y_offset);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,pmenu_data->x+pmenu_data->x_offset, pmenu_data->y+pmenu_data->y_offset);
				cursor_pos (pmenu_data->x + pmenu_data->x_offset+1, pmenu_data->y);
				break;
		}
		
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		
		server_combination_key(*pkey_value);
		if(g_skip_page)
		{
			g_skip_page = 0;
			return;
		}
		switch(*pkey_value)
		{
			case KEY_F1:
			case KEY_F2:
			case KEY_ENTER:
				if(g_cur_val[pmenu_data->cur] > 1)
				{
					g_cur_val[pmenu_data->cur] = 0;
				}
				p->gr.test_method = g_cur_val[pmenu_data->cur];
				
				g_cur_step->one_step = *p;
				GUI_ClearRect(60,15,200-1,28);
				save_cur_step();
				pmenu_data->pData = &p->gr.test_method;
				if(pmenu_data->type == OPTION_T)
				{
					switch(g_cur_val[pmenu_data->cur])
					{
						case 1:
							pmenu_data->unit[0] = "连续";
							pmenu_data->unit[1] = "Cont.";
							pmenu_data->unit[2] = "脉冲";
							pmenu_data->unit[3] = "Pulse";
							break;
						case 0:
							pmenu_data->unit[0] = "脉冲";
							pmenu_data->unit[1] = "Pulse";
							pmenu_data->unit[2] = "连续";
							pmenu_data->unit[3] = "Cont.";
							break;
					}
				}
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
		}
	}
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
