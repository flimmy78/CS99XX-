
#define MODULE_GLOBALS
#include "module.h"
#include "app.h"
#include "keyboard.h"
#include "GUI_Draw.h"
#include "dc_module.h"
#include "serve_test.h"
#include "string.h"
#include "cs99xx_mem_api.h"
#include "mb_master.h"

#define MD_ONE_PAGE_NUM 5


void dis_one_module(MODULE_INFO *p, uint8_t n)
{
    GUI_DispStringAt((const char*)"ID:", 38, 2 + 12 * n);
    GUI_DispDecAt(p->id, 56, 2 + 12 * n, 2);
    GUI_DispStringAt((const char*)p->name, 80, 2 + 12 * n);
}

void dis_one_page_modules(MODULE_INFO *p, uint8_t base, uint8_t num)
{
    int32_t i = 0;
    
    GUI_ClearRect (20, 2, 200 - 1, 64 - 2);
    
    for(i = 0; i < num; i++)
    {
        GUI_DispDecAt(base + i + 1 , 20, 2 + 12 * i, 2);
        GUI_DispStringAt((const char*)".", 32, 2 + 12 * i);
        dis_one_module(p + base + i, i);
    }
    
	LCD_REFRESH();
}

extern void start_scan_module(MODULE_INFO *p, uint8_t *num)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
    uint8_t slave_num = 0;
    uint32_t flag = 0;
    uint32_t n = 0;
    uint32_t last_page_num = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    
    memset(&sys_par.dc_module_using_info, 0, sizeof(sys_par.dc_module_using_info));
    save_sys_par();
    
    draw_short_cut_str(NULL, NULL, "停止", NULL);
    GUI_DrawRect(200, 0, 240 - 1, 64 - 1);
	LCD_REFRESH();
    
    dis_one_page_modules(p, t1 * MD_ONE_PAGE_NUM, last_page_num);
    test_flag.mb_break_send = MB_MASETER_SEND_BREAK;/* 打断modbus发送任务 */
    
    while(1)
    {
        OSTimeDlyHMSM(0,0,0,100);
        if(test_flag.mb_break_send == MB_MASETER_SEND_END)
        {
            break;
        }
    }
    
    test_flag.mb_master_timeout = 0;
    memset(dc_gr_master_info, 0, sizeof dc_gr_master_info);
    g_return = 0;
    g_exit = 0;
    
    while(1)
    {
        if(g_return || g_exit)
        {
            return;
        }
        
        pkey_value = OSQPend(KeyboardQSem, 10, &err);
        
        if(flag == 0)
        {
            flag = 1;
            
            if(++slave_num > 15)
            {
                g_return = 1;
                return;
            }
            
            GUI_DispDecAt(slave_num, 2, 2 + 12 * 4, 2);
            LCD_REFRESH();
            dc_gr_master_info[slave_num].en = 0;
            send_dc_module_cmd(DC_MODULE_CMD_SCAN_MODULES, slave_num);
        }
        
        if(dc_gr_master_info[slave_num].en == 1)
        {
            flag = 0;
            sys_par.dc_module_using_info.addr[n] = dc_gr_master_info[slave_num].id.id;
            p[n++] = dc_gr_master_info[slave_num].id;
            *num = n;
            sys_par.dc_module_using_info.count = n;
            save_sys_par();
            
            t1 = n / MD_ONE_PAGE_NUM;
            t2 = n % MD_ONE_PAGE_NUM;
            
            last_page_num = (t2 == 0? MD_ONE_PAGE_NUM : t2);
            dis_one_page_modules(p, t1 * MD_ONE_PAGE_NUM, last_page_num);
        }
        else
        {
            if(test_flag.mb_master_timeout)
            {
                test_flag.mb_master_timeout = 0;
                flag = 0;
            }
        }
        
		switch(*pkey_value)
		{
            case KEY_F3:
                g_return = 1;
                break;
			case KEY_F4:
                g_return = 1;
                break;
            case KEY_EXIT:
                g_exit = 1;
                break;
        }
    }
}

/* 自动扫描模块 */
extern void auto_scan_module(void)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
    uint8_t slave_num = 0;
    uint32_t flag = 0;
    uint32_t n = 0;
    
    memset(&sys_par.dc_module_using_info, 0, sizeof(sys_par.dc_module_using_info));
    save_sys_par();
	
    test_flag.mb_break_send = MB_MASETER_SEND_BREAK;/* 打断modbus发送任务 */
    gui_draw_dc_gr_module_being_in_comm();
	on_keyboard();	/* 打开键盘 */
	
    while(1)
    {
        OSTimeDlyHMSM(0,0,0,100);
        if(test_flag.mb_break_send == MB_MASETER_SEND_END)
        {
            break;
        }
    }
    
    test_flag.mb_master_timeout = 0;
    memset(dc_gr_master_info, 0, sizeof dc_gr_master_info);
    g_return = 0;
    g_exit = 0;
    
    while(1)
    {
        if(flag == 0)
        {
            flag = 1;
            
            if(++slave_num > 15)
            {
                g_return = 1;
                return;
            }
			
            dc_gr_master_info[slave_num].en = 0;
            send_dc_module_cmd(DC_MODULE_CMD_SCAN_MODULES, slave_num);
        }
        
        if(dc_gr_master_info[slave_num].en == 1)
        {
            flag = 0;
            sys_par.dc_module_using_info.addr[n++] = dc_gr_master_info[slave_num].id.id;
            sys_par.dc_module_using_info.count = n;
            save_sys_par();
        }
        else
        {
            if(test_flag.mb_master_timeout)
            {
                test_flag.mb_master_timeout = 0;
                flag = 0;
            }
        }
		
        pkey_value = OSQPend(KeyboardQSem, 10, &err);
        
		if(NULL == pkey_value)
        {
            continue;
        }
        
		switch(*pkey_value)
		{
			case KEY_EXIT:
                g_return = 1;
				return;
		}
    }
}
extern void auto_scan_modules(void)
{
	auto_scan_module();
}

void scan_modules(void)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
    uint32_t t1 = MODULE_MAX_NUM / MD_ONE_PAGE_NUM;
    uint32_t t2 = MODULE_MAX_NUM % MD_ONE_PAGE_NUM;
    uint32_t total_pages = (t2 == 0?t1:t1+1);
    uint32_t page = 0;
    MODULE_INFO module_info_pool[MODULE_MAX_NUM+1];
    uint8_t module_num = 0;
    
    uint32_t last_page_num = 0;
    uint32_t count = 0;
    uint8_t refresh = 1;
    
    memset(module_info_pool, 0, sizeof module_info_pool);
    dis_scan_modules_gui();
    
    while(1)
    {
        if(g_return)
        {
            g_return = 0;
        }
        
        if(g_exit)
        {
            return;
        }
        
        if(refresh == 1)
        {
            refresh = 0;
            get_dc_gr_module_inf(module_info_pool, &count);
            
            if(count > 0)
            {
                t1 = count / MD_ONE_PAGE_NUM;
                t2 = count % MD_ONE_PAGE_NUM;
                total_pages = (t2 == 0 ? t1 : t1 + 1);
                last_page_num = (t2 == 0? MD_ONE_PAGE_NUM : t2);
                
                dis_one_page_modules(module_info_pool, page * MD_ONE_PAGE_NUM, last_page_num);
            }
        }
        
        pkey_value = OSQPend(KeyboardQSem, 10, &err);
        
		if(NULL == pkey_value)
        {
            continue;
        }
        
		switch(*pkey_value)
		{
			case KEY_F1:
                if(page == 0)
                {
                    if(total_pages)
                    {
                        page = total_pages - 1;
                    }
                }
                else
                {
                    page--;
                }
                refresh = 1;
                break;
			case KEY_F2:
                if(page + 1 >= total_pages)
                {
                    page = 0;
                }
                else
                {
                    if(total_pages)
                    {
                        page++;
                    }
                }
                refresh = 1;
                break;
			case KEY_F3:
                start_scan_module(module_info_pool, &module_num);
                draw_short_cut_str(NULL, NULL, "启动", NULL);
                GUI_DrawRect(200, 0, 240 - 1, 64 - 1);
                LCD_REFRESH();
                break;
            case KEY_F4:
                g_return = 1;
                return;
            case KEY_EXIT:
                g_exit = 1;
                break;
        }
    }
}


/******************* (C) COPYRIGHT 2016 长盛仪器 *****END OF FILE*******************/

