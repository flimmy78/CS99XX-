
#include	<string.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"RTC_Config.h"
#include    "cs99xx_type.h"

void set_custom_options_par(void *pcus)
{
	uint16_t *pkey_value = 0;
	INT8U err = 0;
	void* pdata_bak = pmenu_data->pData;
	uint8_t flag = 0;/* 标志 */
    
    MORE_OPTIONS_LIST *list = pmenu_data->user_data;
    MORE_OPTIONS_STR *iteration = list->head;
    
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


/******************* (C) COPYRIGHT 2016 长盛仪器 *****END OF FILE*******************/
