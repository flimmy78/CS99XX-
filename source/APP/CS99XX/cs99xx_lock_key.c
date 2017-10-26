/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_result.c
 * ժ  Ҫ  ��ϵͳ����������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */


#include <string.h>
#include "GUI_Draw.h"
#include "keyboard.h"
#include "app.h"
#include "cs99xx_mem_api.h"
#include "serve_test.h"

void sys_lockkeyboard(uint32_t w)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t n = sys_par.keyboard_lock;
	
	gui_draw_keyboard_lock_manage();
	while(1)
	{
		switch(n)
		{
			case 0:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, 3+60, 2+5+1);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, 3+60+50, 2+5+1);
				cursor_pos(3+60+50+1, 5);
				break;
			case 1:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, 3+60, 2+5+1);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, 3+60+50, 2+5+1);
				cursor_pos(3+60+1, 5);
				break;
		}
        
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		switch(*pkey_value)
		{
			case KEY_SHIFT:
				if(++n > 1)
				{
					n = 0;
				}
				break;
			case KEY_ENTER:
				if(!n)
				{
					gunlock = 0;	/* ���رռ�����ʱ��ȫ�ֱ�־���� */
				}
				sys_par.keyboard_lock = n;
                save_sys_par();
				g_return = 1;
				return;
			case KEY_BACK:
				g_return = 1;
				return ;
			case KEY_EXIT:
				g_exit = 1;
				return ;
		}
	}
}

void sys_recover(uint32_t w)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t n = sys_par.keyboard_lock;
	
	gui_draw_recover();
    
	while(1)
	{
		switch(n)
		{
			case 0:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, 3+60 + 12, 2+5+1);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, 3+60+50 +12, 2+5+1);
				cursor_pos(3+60+50+1 + 12, 5);
				break;
			case 1:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, 3+60 + 12, 2+5+1);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, 3+60+50 + 12, 2+5+1);
				cursor_pos(3+60+1 + 12, 5);
				break;
		}
        
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		switch(*pkey_value)
		{
			case KEY_SHIFT:
				if(++n > 1)
				{
					n = 0;
				}
				break;
			case KEY_ENTER:
                if(n)
                {
                    app_flag.recover_flag = 1;
                    init_par();
                    app_flag.recover_flag = 0;
                }
				g_return = 1;
				return;
			case KEY_BACK:
				g_return = 1;
				return ;
			case KEY_EXIT:
				g_exit = 1;
				return ;
		}
	}
}
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE*******************/
