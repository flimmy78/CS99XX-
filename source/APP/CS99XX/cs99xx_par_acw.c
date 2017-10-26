/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_par_acw.c
 * 摘  要  ：设置ACW的参数.
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

void updata_ac_cur_gear_affect_par_info(uint8_t mode, uint8_t gear)
{
    gear %= AC_GEAR_END;
    
    switch(mode)
    {
        case ACW:
            menu_par[GUI_ACW_CUR_GEAR].unit[0]       = ac_gear[gear].name;
            menu_par[GUI_ACW_CUR_GEAR].unit[1]       = ac_gear[gear].name;
            
            menu_par[GUI_ACW_CUR_H].unit[0]          = unit_pool[ac_gear[gear].unit];
            menu_par[GUI_ACW_CUR_H].decs             = ac_gear[gear].decs;
            menu_par[GUI_ACW_CUR_H].upper_str        = ac_gear[gear].high_str;
            menu_par[GUI_ACW_CUR_H].lower_str        = ac_gear[gear].low_str;
            menu_par[GUI_ACW_CUR_H].upper            = ac_gear[gear].high_max;
            menu_par[GUI_ACW_CUR_H].lower            = ac_gear[gear].high_min;
            
            menu_par[GUI_ACW_CUR_L].unit[0]          = unit_pool[ac_gear[gear].unit];
            menu_par[GUI_ACW_CUR_L].decs             = ac_gear[gear].decs;
            menu_par[GUI_ACW_CUR_L].upper_str        = ac_gear[gear].high_str;
            menu_par[GUI_ACW_CUR_L].lower_str        = ac_gear[gear].low_str;
            menu_par[GUI_ACW_CUR_L].upper            = ac_gear[gear].low_max;
            menu_par[GUI_ACW_CUR_L].lower            = ac_gear[gear].low_min;
        
            menu_par[GUI_ACW_REAL_CUR].unit[0]       = unit_pool[ac_gear[gear].unit];
            menu_par[GUI_ACW_REAL_CUR].upper_str     = ac_gear[gear].high_str;
            menu_par[GUI_ACW_REAL_CUR].lower_str     = ac_gear[gear].low_str;
            menu_par[GUI_ACW_REAL_CUR].decs          = ac_gear[gear].decs;
            menu_par[GUI_ACW_REAL_CUR].upper         = ac_gear[gear].low_max;
            menu_par[GUI_ACW_REAL_CUR].lower         = ac_gear[gear].low_min;
			
			/* 电流偏移 */
			if(g_custom_sys_par.offset_set_en)
			{
				menu_par[GUI_COM_CUR_OFFSET].unit[0]       = unit_pool[ac_gear[gear].unit];
				menu_par[GUI_COM_CUR_OFFSET].upper_str     = ac_gear[gear].high_str;
				menu_par[GUI_COM_CUR_OFFSET].lower_str     = ac_gear[gear].low_str;
				menu_par[GUI_COM_CUR_OFFSET].decs          = ac_gear[gear].decs;
				menu_par[GUI_COM_CUR_OFFSET].upper         = ac_gear[gear].low_max;
				menu_par[GUI_COM_CUR_OFFSET].lower         = ac_gear[gear].low_min;
			}
			
            break;
        case CC:
            menu_par[GUI_CC_CUR_GEAR].unit[0]       = ac_gear[gear].name;
            menu_par[GUI_CC_CUR_GEAR].unit[1]       = ac_gear[gear].name;
            
            menu_par[GUI_CC_CUR_H].unit[0]          = unit_pool[ac_gear[gear].unit];
            menu_par[GUI_CC_CUR_H].decs             = ac_gear[gear].decs;
            menu_par[GUI_CC_CUR_H].upper_str        = ac_gear[gear].high_str;
            menu_par[GUI_CC_CUR_H].lower_str        = ac_gear[gear].low_str;
            menu_par[GUI_CC_CUR_H].upper            = ac_gear[gear].high_max;
            menu_par[GUI_CC_CUR_H].lower            = ac_gear[gear].high_min;
            
            menu_par[GUI_CC_CUR_L].unit[0]          = unit_pool[ac_gear[gear].unit];
            menu_par[GUI_CC_CUR_L].decs             = ac_gear[gear].decs;
            menu_par[GUI_CC_CUR_L].upper_str        = ac_gear[gear].high_str;
            menu_par[GUI_CC_CUR_L].lower_str        = ac_gear[gear].low_str;
            menu_par[GUI_CC_CUR_L].upper            = 500;//ac_gear[gear].low_max;
            menu_par[GUI_CC_CUR_L].lower            = 0;//ac_gear[gear].low_min;
            
            menu_par[GUI_CC_REAL_CUR].unit[0]       = unit_pool[ac_gear[gear].unit];
            menu_par[GUI_CC_REAL_CUR].upper_str     = ac_gear[gear].high_str;
            menu_par[GUI_CC_REAL_CUR].lower_str     = ac_gear[gear].low_str;
            menu_par[GUI_CC_REAL_CUR].decs          = ac_gear[gear].decs;
            menu_par[GUI_CC_REAL_CUR].upper         = ac_gear[gear].low_max;
            menu_par[GUI_CC_REAL_CUR].lower         = ac_gear[gear].low_min;
            
            break;
    }
}

void set_ac_cur_gear(void *p_node)
{
	UN_STRUCT *p = &((NODE_STEP *)p_node)->one_step;
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	uint8_t gear_bak = 0;
	
	uint32_t cur_kinds = 0;/* 电流的种类 */
	const uint8_t *gear_buf[CUR_KINDS_MAX] = {0};/* 电流档位 */
	uint8_t flag[CUR_KINDS_MAX] = {0};/* 标志 */
	int32_t i = 0;
    uint8_t l_gear = 0;
	void *pdata_bak = pmenu_data->pData;
    
    switch(p->com.mode)
    {
        case ACW:
            l_gear = p->acw.gear_i;
            break;
        case CC:
            l_gear = p->cc.gear_i;
            break;
    }
	
	updata_ac_cur_gear_affect_par_info(p->com.mode, l_gear);
	GUI_ClearRect(0,13,200-1,63);
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	load_this_page_info(g_cur_info);
	load_this_page_value();
	draw_colon();
	
	gear_bak = g_cur_val[pmenu_data->cur];
	
	cur_kinds = define_cur_kinds(p->com.mode, gear_buf, flag);
	
	for(i = 0; i < cur_kinds; i++)
	{
		if(flag[i] == g_cur_val[pmenu_data->cur])
		{
			g_cur_val[pmenu_data->cur] = i+1;/* 对应的档位转换为flag对应的下标然后通过修改下标确定新的档位 */
			break;
		}
	}
	
	/* 如果没有找到对应的电流档 */
	if(i == cur_kinds)
    {
		g_cur_val[pmenu_data->cur] = 1;
        switch(p->com.mode)
        {
            case ACW:
                p->acw.gear_i = flag[g_cur_val[pmenu_data->cur]-1];
                p->acw.upper_limit = 500;
                p->acw.lower_limit = 0;
                p->acw.ac_real_cur = 0;
                break;
            case CC:
                p->cc.gear_i = flag[g_cur_val[pmenu_data->cur]-1];
                p->cc.upper_limit = 500;
                p->cc.lower_limit = 0;
                p->cc.ac_real_cur = 0;
                break;
        }
		g_cur_step->one_step = *p;
		save_cur_step();
    }
	
	GUI_ClearRect(pmenu_data->x, pmenu_data->y,pmenu_data->x+6*5, pmenu_data->y+12);
	
	while(1)
	{
		ac_cur_sel_gui(cur_kinds, gear_buf, g_cur_val[pmenu_data->cur]);
		
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
				pmenu_data->pData = pdata_bak;
                save_par_to_ram(pmenu_data, flag[g_cur_val[pmenu_data->cur]-1]);
				
				if(gear_bak != flag[g_cur_val[pmenu_data->cur]-1])
				{
                    switch(p->com.mode)
                    {
                        case ACW:
                            p->acw.upper_limit = 500;
                            p->acw.lower_limit = 0;
                            p->acw.ac_real_cur = 0;
							save_par_to_ram(&menu_par[GUI_COM_CUR_OFFSET], 0);
                            break;
                        case CC:
                            p->cc.upper_limit = 500;
                            p->cc.lower_limit = 0;
                            p->cc.ac_real_cur = 0;
                            break;
                    }
				}
                
				updata_ac_cur_gear_affect_par_info(p->com.mode, flag[g_cur_val[pmenu_data->cur]-1]);
				
				g_cur_step->one_step = *p;
				GUI_ClearRect(60,15,200-1,28);
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
		}
        if(g_exit)
        {
            return;
        }
	}
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
