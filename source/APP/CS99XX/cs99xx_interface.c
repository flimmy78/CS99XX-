/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_result.c
 * 摘  要  ：系统通信接口管理
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
#include 	"mb_server.h"
#include 	"SysTemConfig.h"
#include    "cs99xx_result.h"

static void PLC_set(uint32_t opt)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t n = sys_par.plc_on_off;

	GUI_ClearRect(0, 0, 200-1, 64-1);
	GUI_ClearRect(200+1, 0, 240-1, 15-1);
	GUI_ClearRect(200+1, 15+1, 240-1, 31-1);
	GUI_SetFont(&GUI_FontHZ_SimSun_13);
	GUI_DispStringAt("PLC", 90, 2);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DrawLine(20, 14   , 180  , 14);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("PLC控制:", 3, 18);
		GUI_DispStringAt("打开", 3+80, 18);
		GUI_DispStringAt("关闭", 3+80+50, 18);
		
		GUI_DispStringAt("说   明:", 3, 32);
		GUI_DispStringAt("通过PLC接口进行控制，",3+50, 32);
		GUI_DispStringAt("只有复位键有效。", 3+50, 46);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("PLC Control:", 3, 18);
		GUI_DispStringAt("Open", 3+80, 18);
		GUI_DispStringAt("Close", 3+80+50, 18);
		
		GUI_DispStringAt("Manual:", 3, 32);
		GUI_DispStringAt("When PLC is on,only",3+50, 32);
		GUI_DispStringAt("the stop key is valid.", 3+50, 46);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	while(1)
	{
		switch(n)
		{
			case 0:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+70, 18+3 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+70+50, 18+3 );
				cursor_pos(3+70+50+1, 18);
				break;
			case 1:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+70, 18+3 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+70+50, 18+3 );
				cursor_pos(3+70+1, 18);
				break;
		}
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		
		switch(*pkey_value)
		{
			
		}
		
		switch(*pkey_value)
		{
			case KEY_SHIFT:
				n = !n;
				break;
			case KEY_ENTER:
				sys_par.plc_on_off = n;
				updata_comm_flag |= UPDATE_PLC_SW;
				save_sys_par();
				return;
			case KEY_BACK:
				return ;
			case KEY_EXIT:
				g_exit = 1;
				return ;
		}
	}
	
}

static void Communication(uint32_t opt)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t n = 1;
	uint8_t m = 1;
	uint8_t temp_commu_on_off = sys_par.commu_on_off;
	uint8_t temp_rate_gear = sys_par.baud_rate_gear;
	uint16_t temp_local_addr = sys_par.local_addr;
	
	/* 定义地址在屏幕上显示的信息属性 */
	DATA_INFO addr_info = {//
		(void*)0/*pData*/,63/*x*/,48/*y*/,0/*type*/,3/*lon*/,0/*decs*/,0/*total*/,1/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
    
    /* 数据检查 */
    if(temp_commu_on_off > 1 || !(temp_rate_gear > 0 && temp_rate_gear < 4) || temp_local_addr == 0)
    {
        temp_commu_on_off   = 0;
        temp_rate_gear      = 1;
        temp_local_addr     = 1;
        
        sys_par.commu_on_off    = temp_commu_on_off;
        sys_par.baud_rate_gear  = temp_rate_gear;
        sys_par.local_addr      = temp_local_addr;
        
        save_sys_par();
    }    
    
	pmenu_data = &addr_info;
	pmenu_data->pData = (uint16_t*)pmenu_data->pData;
	pmenu_data->pData = &temp_local_addr;
	GUI_ClearRect(0, 0, 200-1, 64-1);
	GUI_DrawLine(20, 14   , 180  , 14);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt("：", 3+2+6*8, 18);
	GUI_DispStringAt("：", 3+2+6*8, 33);
	GUI_DispStringAt("：", 3+2+6*8, 48);
	GUI_DispStringAt("9600", 3+70, 33);
	GUI_DispStringAt("14400", 3+70+40, 33);
	GUI_DispStringAt("19200", 3+70+40+50, 33);
	GUI_DispStringAt("001", 3+60, 48);
	GUI_DispStringAt("(1-255)",3+60+30, 48);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("通信", (180+20)/2 - sizeof("通信")/2*12/2, 2);// (180+20)/2 - sizeof("通信")*12/2
		GUI_DispStringAt("通讯控制", 3, 18);
		GUI_DispStringAt("打开", 3+70, 18);
		GUI_DispStringAt( "关闭", 3+70+40, 18);
		
		GUI_DispStringAt("波 特 率", 3, 33);
		GUI_DispStringAt("本机地址", 3, 48);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Communication", 50, 0);
		GUI_DispStringAt("Control", 3, 18);
		GUI_DispStringAt("Open", 3+70, 18);
		GUI_DispStringAt( "Close", 3+70+50, 18);
		GUI_DispStringAt("BaudRate", 3, 33);
		GUI_DispStringAt("Address", 3, 48);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	switch(temp_commu_on_off)
	{
		case 0:
			GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60, 18+2 );
			GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+60+40, 18+2 );
			break;
		case 1:
			GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+60, 18+2 );
			GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60+40, 18+2 );
			break;
	}
	switch(temp_rate_gear)
	{
		case 1:
			GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+60, 33+2 );
			GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60+40, 33+2 );
			GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60+40+50, 33+2 );
			break;
		case 2:
			GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60, 33+2 );
			GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+60+40, 33+2 );
			GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60+40+50, 33+2 );
			break;
		case 3:
			GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60, 33+2 );
			GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60+40, 33+2 );
			GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+60+40+50, 33+2 );
			break;
	}
	GUI_DispDecAt (temp_local_addr, pmenu_data->x, pmenu_data->y, pmenu_data->lon);
    
    
    
	m = temp_commu_on_off;
	while(1)
	{
		switch(n)
		{
			case 1://第一个参数 通信开关
				switch(m)
				{
					case 0:
						GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60, 18+2 );
						GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+60+40, 18+2 );
						cursor_pos(3+60+40+1, 18-1);
						break;
					case 1:
						GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+60, 18+2 );
						GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60+40, 18+2 );
						cursor_pos(3+60+1, 18-1);
						break;
				}
				break;
			case 2://第二个参数 波特率
				switch(m)
				{
					case 1:
						GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+60, 33+2 );
						GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60+40, 33+2 );
						GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60+40+50, 33+2 );
						cursor_pos(3+60+1, 33-1);
						break;
					case 2:
						GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60, 33+2 );
						GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+60+40, 33+2 );
						GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60+40+50, 33+2 );
						cursor_pos(3+60+40+1, 33-1);
						break;
					case 3:
						GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60, 33+2 );
						GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+60+40, 33+2 );
						GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+60+40+50, 33+2 );
						cursor_pos(3+60+40+50+1, 33-1);
						break;
				}
				break;
			case 3://三个参数 本机地址
				
				switch(m)
				{
					case 0:
						break;
					case 1:
						break;
					case 2:
						break;
				}
				break;
		}
		
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		if(n == 3)
		{
			serve_numkey(*pkey_value);
		}
		switch(*pkey_value)
		{
			case KEY_UP:
			case KEY_DOWN:
			case KEY_ENTER:
				break;
			}
		
		switch(*pkey_value)
		{
			case KEY_UP:
				switch(n)
				{
					case 1:
						updata_comm_flag |= UPDATE_COMM_SW;
						temp_commu_on_off = m;
						break;
					case 2:
						updata_comm_flag |= UPDATE_COMM_BPS;
						temp_rate_gear = m;
						break;
					case 3:
						if(temp_local_addr < 1)
						{
							temp_local_addr = 1;
						}
						else if(temp_local_addr > 255)
						{
							temp_local_addr = 255;
						}
						update_data(pmenu_data);
						break;
				}
				sys_par.commu_on_off = temp_commu_on_off;
				sys_par.baud_rate_gear = temp_rate_gear;
				sys_par.local_addr = temp_local_addr;
				save_sys_par();
				
				if(n == 1)
				{
					n = 3;
				}
				else
				{
					n--;
				}
				switch(n)
				{
					case 1:
						m = temp_commu_on_off;
						break;
					case 2:
						m = temp_rate_gear;
						break;
					case 3:
						cursor_pos(pmenu_data->x, pmenu_data->y);
						pmenu_data->ch_bit = 0;
						break;
				}
				break;
			case KEY_DOWN:
				switch(n)
				{
					case 1:
						updata_comm_flag |= UPDATE_COMM_SW;
						temp_commu_on_off = m;
						break;
					case 2:
						updata_comm_flag |= UPDATE_COMM_BPS;
						temp_rate_gear = m;
						break;
					case 3:
						if(temp_local_addr < 1)
						{
							temp_local_addr = 1;
						}
						else if(temp_local_addr > 255)
						{
							temp_local_addr = 255;
						}
						update_data(pmenu_data);
						break;
				}
				sys_par.commu_on_off = temp_commu_on_off;
				sys_par.baud_rate_gear = temp_rate_gear;
				sys_par.local_addr = temp_local_addr;
				save_sys_par();
				
				if(++n > 3)
				{
					n = 1;
				}
				switch(n)
				{
					case 1:
						m = temp_commu_on_off;
						break;
					case 2:
						m = temp_rate_gear;
						break;
					case 3:
						cursor_pos(pmenu_data->x, pmenu_data->y);
						pmenu_data->ch_bit = 0;
						break;
				}
				break;
			case KEY_SHIFT:
				switch(n)
				{
					case 1://第一个参数 通信开关
						if(++m > 1)
						{
							m = 0;
						}
						break;
					case 2://第二个参数 波特率
						if(++m > 3)
						{
							m = 1;
						}
						break;
					case 3://三个参数 本机地址
						if(++m > 2)
						{
							m = 0;
						}
						break;
				}
				break;
			case KEY_ENTER:
				switch(n)
				{
					case 1:
						temp_commu_on_off = m;
						updata_comm_flag |= UPDATE_COMM_SW;
						break;
					case 2:
						temp_rate_gear = m;
						updata_comm_flag |= UPDATE_COMM_BPS;
						break;
					case 3:
						if(temp_local_addr < 1)
						{
							temp_local_addr = 1;
						}
						else if(temp_local_addr > 255)
						{
							temp_local_addr = 255;
						}
						update_data(pmenu_data);
						break;
				}
				sys_par.commu_on_off = temp_commu_on_off;
				sys_par.baud_rate_gear = temp_rate_gear;
				sys_par.local_addr = temp_local_addr;
				save_sys_par();
				if(++n > 3)
				{
					n = 1;
				}
				if(n == 1)
				{
					
				}
				switch(n)
				{
					case 1:
						m = temp_commu_on_off;
						break;
					case 2:
						m = temp_rate_gear;
						break;
					case 3:
						cursor_pos(pmenu_data->x, pmenu_data->y);
						pmenu_data->ch_bit = 0;
						break;
				}
				break;
			case KEY_BACK:
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}		
	}
}

static void USB_leading_out(uint32_t opt)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t n = 0;
    TEST_FILE *back_file_p = g_cur_file;

	GUI_ClearRect(0, 0, 200-1, 64-1);
    draw_short_cut_bm(__RB);
    
    
    GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DrawLine(20, 14, 180, 14);
    GUI_DispStringAt("：", 3+6*8, 18);
    GUI_DispStringAt("：", 3+6*8, 32);
    
	if(sys_par.language == CHINESE)
	{
        GUI_DispStringAt_BTP_STR("数据导出到U盘", 20, 180, 2);
		GUI_DispStringAt("导出控制", 3, 18);
		GUI_DispStringAt("开始", 3+80, 18);
		GUI_DispStringAt("取消", 3+80+50, 18);
		
		GUI_DispStringAt("说    明", 3, 32);
		GUI_DispStringAt("通过U盘导出数据，",3+56, 32);
		GUI_DispStringAt("请先确认U盘已连接。", 3+56, 46);
	}
	else
	{
        GUI_SetFont(&GUI_FontHZ_SimSun_13);
        GUI_DispStringAt_BTP_STR("USB FLASH LEADING-OUT", 20, 180, 1);
        
		GUI_DispStringAt("LEAD_OUT", 3, 18);
		GUI_DispStringAt("Start", 3+80, 18);
		GUI_DispStringAt("Cancel", 3+80+50, 18);
		
		GUI_DispStringAt("Manual", 3, 32);
		GUI_DispStringAt("Please confirm USB",3+65, 32);
		GUI_DispStringAt("flash connected!", 3+65, 46);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	while(1)
	{
		switch(n)
		{
			case 0:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+70, 18+3 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+70+50, 18+3 );
				cursor_pos(3+70+50+1, 18);
				break;
			case 1:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+70, 18+3 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+70+50, 18+3 );
				cursor_pos(3+70+1, 18);
				break;
		}
        
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		
		switch(*pkey_value)
		{
			
		}
		
		switch(*pkey_value)
		{
			case KEY_SHIFT:
				n = !n;
				break;
			case KEY_ENTER:
                /* 等待U盘连接 */
                if(-1 == wait_usb_connect())
                {
                    g_return = 1;
                    return;
                }
                
				updata_comm_flag |= UPDATE_LEAD_OUT_DATA_USB;
                app_flag.usb_loading_status = LOADING_OUT;
                app_flag.usb_loading_advance = 0;
                rate_of_advance();
                
                g_cur_file = back_file_p;
                
                read_step_used_flag(g_cur_file->num);
                read_group_table(g_cur_file->num);
                read_list(g_cur_file->num);
                
				return;
			case KEY_BACK:
				return ;
			case KEY_EXIT:
				g_exit = 1;
				return ;
		}
	}
}

static void USB_leading_in(uint32_t opt)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t n = 0;
    
	GUI_ClearRect(0, 0, 200-1, 64-1);
	GUI_ClearRect(200+1, 0, 240-1, 15-1);
	GUI_ClearRect(200+1, 15+1, 240-1, 31-1);
    
    GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DrawLine(20, 14   , 180  , 14);
    GUI_DispStringAt("：", 3+6*8, 18);
    GUI_DispStringAt("：", 3+6*8, 32);
    
	if(sys_par.language == CHINESE)
	{
        GUI_DispStringAt_BTP_STR("U盘导入数据", 20, 180, 2);
        
		GUI_DispStringAt("导入控制", 3, 18);
		GUI_DispStringAt("开始", 3+80, 18);
		GUI_DispStringAt("取消", 3+80+50, 18);
		
		GUI_DispStringAt("说    明:", 3, 32);
		GUI_DispStringAt("通过U盘导入数据，",3+56, 32);
		GUI_DispStringAt("请先确认U盘已连接。", 3+56, 46);
	}
	else
	{
        GUI_SetFont(&GUI_FontHZ_SimSun_13);
        GUI_DispStringAt_BTP_STR("USB FLASH LEADING-IN", 20, 180, 1);
        
		GUI_DispStringAt("LEAD_IN", 3, 18);
		GUI_DispStringAt("Start", 3+80, 18);
		GUI_DispStringAt("Cancel", 3+80+50, 18);
		
		GUI_DispStringAt("Manual", 3, 32);
		GUI_DispStringAt("Please confirm USB",3+65, 32);
		GUI_DispStringAt("flash connected!", 3+65, 46);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	while(1)
	{
		switch(n)
		{
			case 0:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+70, 18+3 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+70+50, 18+3 );
				cursor_pos(3+70+50+1, 18);
				break;
			case 1:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+70, 18+3 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+70+50, 18+3 );
				cursor_pos(3+70+1, 18);
				break;
		}
        
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		
		switch(*pkey_value)
		{
			case KEY_SHIFT:
				n = !n;
				break;
			case KEY_ENTER:
                /* 等待U盘连接 */
                if(-1 == wait_usb_connect())
                {
                    g_return = 1;
                    return;
                }
                
				updata_comm_flag |= UPDATE_LEAD_IN_DATA_USB;
                app_flag.usb_loading_status = LOADING_IN;
                app_flag.usb_loading_advance = 0;
                rate_of_advance();
                g_cur_file = pos_file(0);
                read_step_used_flag(0);
                read_group_table(0);
                sys_par.used_res_num = 0;
                sys_par.pass_res_num = 0;
                sys_par.cover_res_num = 0;
                save_sys_par();
                sys_flag.last_group = g_cur_file->num;/* 更新最后使用的文件标志 */
                save_sys_flag();
				return;
			case KEY_BACK:
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
	}
}

MENU_INFO sys_menu_info[]=
{
    {{"PLC", "PLC"}, PLC_set},
    {{"通信", "Communication"}, Communication},
    {{"U盘数据导出", "USB flash leading-out"}, USB_leading_out},
    {{"U盘数据导入", "USB flash leading-in"}, USB_leading_in},
};
void sys_interface(uint32_t w)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t n = 0;
    uint8_t opt_num = 0;
    uint8_t flag_opt[5] = {0};
    uint8_t flag = 0;
    
    if(type_spe.interface_en != ENABLE)
    {
        return;
    }
    
    opt_num = define_interface_config(flag_opt);
l:
	gui_draw_communication_interface(sys_menu_info);
	while(1)
	{
		GUI_ClearRect (0, 0, 12, 63);
		GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 3, 4 + 15 * n);
		ReFresh_LCD24064();
		
		pkey_value = OSQPend(KeyboardQSem,0,&err);
        
		switch(*pkey_value)
		{
			case KEY_UP:
				if(--n < 0)
                {
                    n = opt_num - 1;
                }
				break;
			case KEY_DOWN:
				if(++n >= opt_num)
                {
                    n = 0;
                }
				break;
			case KEY_ENTER:
                flag = 1;
                break;
			case KEY_BACK:
				g_return = 1;
				return ;
			case KEY_EXIT:
				g_exit = 1;
                goto l_end;
			case KEY_1:
				n = 0;
                flag = 1;
                break;
			case KEY_2:
				n = 1;
                flag = 1;
                break;
			case KEY_3:
				n = 2;
                flag = 1;
                break;
			case KEY_4:
				n = 3;
                flag = 1;
                break;
		}
        
        if(flag)
        {
            flag = 0;
            
            if(sys_menu_info[flag_opt[n]].fun != NULL)
            {
                sys_menu_info[flag_opt[n]].fun(0);
            }
             
            if(g_exit)
            {
                goto l_end;
            }
            goto l;
        }
	}
l_end:
    app_flag.usb_loading_status = LOADING_NONE;
}


/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
