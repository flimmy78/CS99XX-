/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_par_dcw.c
 * 摘  要  ：设置DCW的参数函数
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
#include	"cs99xx_type.h"

void updata_dc_cur_gear_affect_par_info(uint8_t mode, uint8_t gear)
{
    gear %= DC_GEAR_END;
    
    switch(mode)
    {
        case DCW:
            menu_par[GUI_DCW_CUR_GEAR].unit[0]      = dc_gear[gear].name;
            menu_par[GUI_DCW_CUR_GEAR].unit[1]      = dc_gear[gear].name;
            
            menu_par[GUI_DCW_CUR_H].unit[0]         = unit_pool[dc_gear[gear].unit];
            menu_par[GUI_DCW_CUR_H].decs            = dc_gear[gear].decs;
            menu_par[GUI_DCW_CUR_H].upper_str       = dc_gear[gear].high_str;
            menu_par[GUI_DCW_CUR_H].lower_str       = dc_gear[gear].low_str;
            menu_par[GUI_DCW_CUR_H].upper           = dc_gear[gear].high_max;
            menu_par[GUI_DCW_CUR_H].lower           = dc_gear[gear].high_min;
            
            menu_par[GUI_DCW_CUR_L].unit[0]         = unit_pool[dc_gear[gear].unit];
            menu_par[GUI_DCW_CUR_L].decs            = dc_gear[gear].decs;
            menu_par[GUI_DCW_CUR_L].upper_str       = dc_gear[gear].high_str;
            menu_par[GUI_DCW_CUR_H].lower_str       = dc_gear[gear].low_str;
            menu_par[GUI_DCW_CUR_H].upper           = dc_gear[gear].low_max;
            menu_par[GUI_DCW_CUR_L].lower           = dc_gear[gear].low_min;
            
            menu_par[GUI_DCW_CHARGE_CUR].unit[0]    = unit_pool[dc_gear[gear].unit];
            menu_par[GUI_DCW_CHARGE_CUR].decs       = dc_gear[gear].decs;
            menu_par[GUI_DCW_CHARGE_CUR].upper_str  = dc_gear[gear].high_str;
            menu_par[GUI_DCW_CHARGE_CUR].lower_str  = dc_gear[gear].low_str;
            menu_par[GUI_DCW_CHARGE_CUR].upper      = dc_gear[gear].low_max;
            menu_par[GUI_DCW_CHARGE_CUR].lower      = dc_gear[gear].low_min;
			
			/* 电流偏移 */
			if(g_custom_sys_par.offset_set_en)
			{
				menu_par[GUI_COM_CUR_OFFSET].unit[0]         = unit_pool[dc_gear[gear].unit];
				menu_par[GUI_COM_CUR_OFFSET].decs            = dc_gear[gear].decs;
				menu_par[GUI_COM_CUR_OFFSET].upper_str       = dc_gear[gear].high_str;
				menu_par[GUI_COM_CUR_OFFSET].lower_str       = dc_gear[gear].low_str;
				menu_par[GUI_COM_CUR_OFFSET].upper           = dc_gear[gear].low_max;
				menu_par[GUI_COM_CUR_OFFSET].lower           = dc_gear[gear].low_min;
			}
			
            break;
    }
}

void set_dcw_cur_gear(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	uint8_t gear_bak = 0;
	
	uint32_t cur_kinds = 0;/* 电流的种类 */
	const uint8_t *gear_buf[CUR_KINDS_MAX] = {0};/* 电流档位 */
	uint8_t flag[CUR_KINDS_MAX] = {0};/* 标志 */
	int32_t i = 0;
	
	updata_dc_cur_gear_affect_par_info(DCW, p->dcw.gear_i);
	
	GUI_ClearRect(0,13,200-1,63);
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
	
	gear_bak = g_cur_val[pmenu_data->cur];
    
	cur_kinds = define_cur_kinds(DCW, gear_buf, flag);
	
	for(i = 0; i < cur_kinds; i++)
	{
		if(flag[i] == g_cur_val[pmenu_data->cur])
		{
			g_cur_val[pmenu_data->cur] = i+1;/* 对应的档位转换为flag对应的下标然后通过修改下标确定新的档位 */
			break;
		}
	}
	
	/* 检查数据 */
	if(i == cur_kinds)
	{
		g_cur_val[pmenu_data->cur] = 1;
        save_par_to_ram(pmenu_data, flag[g_cur_val[pmenu_data->cur]-1]);
        
        if(flag[g_cur_val[pmenu_data->cur]-1] != gear_bak)	/* 换挡后的电流上下限要恢复默认 */
        {
            save_par_to_ram(&menu_par[GUI_DCW_CUR_H], 500);
            save_par_to_ram(&menu_par[GUI_DCW_CUR_L], 0);
            save_par_to_ram(&menu_par[GUI_DCW_CHARGE_CUR], 0);
        }
		
		g_cur_step->one_step = *p;
		save_cur_step();
	}
	
	GUI_ClearRect(pmenu_data->x, pmenu_data->y, pmenu_data->x+6*4, pmenu_data->y+12);
	
	while(1)
	{
		dcw_cur_sel_gui(cur_kinds, gear_buf, g_cur_val[pmenu_data->cur]);
		
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
                save_par_to_ram(pmenu_data, flag[g_cur_val[pmenu_data->cur]-1]);
				
				if(flag[g_cur_val[pmenu_data->cur]-1] != gear_bak)	/* 换挡后的电流上下限要恢复默认 */
				{
                    save_par_to_ram(&menu_par[GUI_DCW_CUR_H], 500);
                    save_par_to_ram(&menu_par[GUI_DCW_CUR_L], 0);
                    save_par_to_ram(&menu_par[GUI_DCW_CHARGE_CUR], 0);
					
                    save_par_to_ram(&menu_par[GUI_COM_CUR_OFFSET], 0);
				}
                
				g_cur_step->one_step = *p;
				GUI_ClearRect(60,15,200-1,28);
				save_cur_step();
				updata_dc_cur_gear_affect_par_info(DCW, p->dcw.gear_i);
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
				if(++g_cur_val[pmenu_data->cur] > cur_kinds) 
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
				if(cur_kinds >= 1)
				{
					g_cur_val[pmenu_data->cur] = 1;
				}
				break;
			case KEY_2:
				if(cur_kinds >= 2)
				{
					g_cur_val[pmenu_data->cur] = 2;
				}
				break;
			case KEY_3:
				if(cur_kinds >= 3)
				{
					g_cur_val[pmenu_data->cur] = 3;
				}
				break;
			case KEY_4:
				if(cur_kinds >= 4)
				{
					g_cur_val[pmenu_data->cur] = 4;
				}
				break;
			case KEY_5:
				if(cur_kinds >= 5)
				{
					g_cur_val[pmenu_data->cur] = 5;
				}
				break;
		}
        if(g_exit)
        {
            return;
        }
	}
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
