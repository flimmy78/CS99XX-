/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_offset.c
 * 摘  要  ：偏移测试功能
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
#include    "math.h"


/*************************** 外部变量 **************************/

/*
 * 函数名：offset_manual
 * 描述  ：对偏移测试进行细节说明
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static void offset_manual(void)
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
	
	while(1)
	{
		GUI_ClearRect (0, 0, 200-1, 12-1);
		if(n<3)
		{
			if(sys_par.language == CHINESE)
			{
				GUI_DispStringAt("I. 偏移测量说明", 60, 0);
			}
			else
			{
				GUI_SetFont(&GUI_Font13_1);
				GUI_DispStringAt("I. Offset specification", 50, 0);
				GUI_SetFont(&GUI_FontHZ_SimSun_12);
			}
		}
		else
		{
			if(sys_par.language == CHINESE)
			{
				GUI_DispStringAt("II. 偏移测量模式", 60, 0);
			}
			else
			{
				GUI_SetFont(&GUI_Font13_1);
				GUI_DispStringAt("II. Offset mode", 60, 0);
				GUI_SetFont(&GUI_FontHZ_SimSun_12);
			}
		}
		GUI_ClearRect (0, 13+1, 200-1, 64-1);
		switch(n)
		{
			case 0:
				if(sys_par.language == CHINESE)
				{
					GUI_DispStringAt("1.保障本机与被测件可靠连接", 5, 18);
					GUI_DispStringAt("2.按下启动键可开始进行偏移测量", 5, 32);
					GUI_DispStringAt("3.按下复位键可暂停偏移测量", 5, 46);
				}
				else
				{
					GUI_SetFont(&GUI_Font13_1);
					GUI_DispStringAt("1.Ensure proper connection.", 5, 18);
					GUI_DispStringAt("2.Press start key to perform offset.", 5, 32);
					GUI_DispStringAt("3.Press stop key to stop offset.", 5, 46);
					GUI_SetFont(&GUI_FontHZ_SimSun_12);
				}
				break;
			case 1:
				if(sys_par.language == CHINESE)
				{
					GUI_DispStringAt("4.当一个测试步骤的模式或电压（", 5, 18);
					GUI_DispStringAt("电流）有一个参数改变时，以前", 5+12, 32);
					GUI_DispStringAt("的偏移测量结果自动无效", 5+12, 46);
				}
				else
				{
					GUI_SetFont(&GUI_Font13_1);
					GUI_DispStringAt("4.When a param. of test step is", 5, 18);
					GUI_DispStringAt("changed,the last offset result", 5+12, 32);
					GUI_DispStringAt("doesn't effect.", 5+12, 46);
					GUI_SetFont(&GUI_FontHZ_SimSun_12);
				}
				break;
			case 2:
				if(sys_par.language == CHINESE)
				{
					GUI_DispStringAt("5.绝缘电阻（IR）模式无偏移测量", 5, 18);
					GUI_DispStringAt("功能", 5+12, 32);
				}
				else
				{
					GUI_SetFont(&GUI_Font13_1);
					GUI_DispStringAt("5.IR without offset measurement", 5, 18);
					GUI_DispStringAt("function", 5+12, 32);
					GUI_SetFont(&GUI_FontHZ_SimSun_12);
				}
				break;
			case 3:
				if(sys_par.language == CHINESE)
				{
					GUI_DispStringAt("1.单次模式：系统对当前测试步骤", 5, 18);
					GUI_DispStringAt("进行偏移测量后停止，需按下启", 5+12, 32);
					GUI_DispStringAt("动键进行下一步偏移测量", 5+12, 46);
				}
				else
				{
					GUI_SetFont(&GUI_Font13_1);
					GUI_DispStringAt("1.Single mode:Systerm stops testing", 5, 18);
					GUI_DispStringAt("after the test on the current step,", 5+12, 32);
					GUI_DispStringAt("press the start key for the next step", 5+12, 46);
					GUI_SetFont(&GUI_FontHZ_SimSun_12);
				}
				break;
			case 4:
				if(sys_par.language == CHINESE)
				{
					GUI_DispStringAt("2.连续模式：系统自动对当前的文件", 5, 18);
					GUI_DispStringAt("所有测试步骤进行偏移测量后停", 5+12, 32);
					GUI_DispStringAt("止", 5+12, 46);
				}
				else
				{
					GUI_SetFont(&GUI_Font13_1);
					GUI_DispStringAt("2.Series mode:Systerm auto stops", 5, 18);
					GUI_DispStringAt("testing after the test on all steps", 5+12, 32);
					GUI_DispStringAt("of the current file.", 5+12, 46);
					GUI_SetFont(&GUI_FontHZ_SimSun_12);
				}
				break;
			case 5:
				if(sys_par.language == CHINESE)
				{
					GUI_DispStringAt("3.从菜单界面进入偏移测量界面", 5, 18);
					GUI_DispStringAt("系统默认为连续测量模式，可在", 5+12, 32);
					GUI_DispStringAt("模式功能键菜单下更改测量模式", 5+12, 46);
				}
				else
				{
					GUI_SetFont(&GUI_Font13_1);
					GUI_DispStringAt("3.Enter offset measurement interface", 5, 18);
					GUI_DispStringAt("from menu,and systerm default is", 5+12, 32);
					GUI_DispStringAt("series mode.", 5+12, 46);
					GUI_SetFont(&GUI_FontHZ_SimSun_12);
				}
				break;
		}
		LCD_REFRESH();
		pkey_value = OSQPend(KeyboardQSem, 0,&err);
		
		switch(*pkey_value)
		{
			case KEY_UP:/* 模式 */
				if(--n < 0)
				{
					n = 5;
				}
				break;
			case KEY_DOWN:/* 说明 */
				if(++n > 5)
				{
					n = 0;
				}
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
 * 函数名：offset_sel_mode
 * 描述  ：偏移测试分两种模式：连续，单次 该函数就是选择的
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static int8_t offset_sel_mode(void)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t n = app_flag.offset_mode;
	
	GUI_ClearRect(0, 0, 200-1, 64-1);
    
    draw_short_cut_bm(__RB);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DrawLine(20, 14   , 180  , 14);
	GUI_DispStringAt("：", 3+8*6, 18);
	GUI_DispStringAt("：", 3+8*6, 32);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("偏移测量模式更改", 50, 2);
		GUI_DispStringAt("测量模式", 3, 18);
		GUI_DispStringAt("单次", 3+75, 18);
		GUI_DispStringAt("连续", 3+75+50, 18);
		
		GUI_DispStringAt("说   明", 3, 32);
		GUI_DispStringAt("选择后的测量模式立",3+10*6, 32);
		GUI_DispStringAt("即生效", 3+60, 46);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Offset mode selection", 40, 0);
		GUI_DispStringAt("Mode", 3, 18);
		GUI_DispStringAt("Single", 3+75, 18);
		GUI_DispStringAt("Series", 3+75+50, 18);
		
		GUI_DispStringAt("Manual", 3, 32);
		GUI_DispStringAt("Selected mode takes",3+60, 32);
		GUI_DispStringAt("effect immediately.", 3+60, 46);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	while(1)
	{
		switch(n)
		{
			case OFFSET_CON:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+65, 18+3 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+65+50, 18+3 );
				cursor_pos(3+65+50+1, 18);
				break;
			case OFFSET_SIG:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,3+65, 18+3 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,3+65+50, 18+3 );
				cursor_pos(3+65+1, 18);
				break;
		}
        
		pkey_value = OSQPend(KeyboardQSem,0,&err);
        
		switch(*pkey_value)
		{
			case KEY_SHIFT:
                if(n == OFFSET_CON)
                {
                    n = OFFSET_SIG;
                }
                else
                {
                    n = OFFSET_CON;
                }
				break;
			case KEY_ENTER:
				g_return = 1;
				return n;
			case KEY_BACK:
				g_return = 1;
				return -1;
			case KEY_EXIT:
				g_exit = 1;
				return -1;
		}
	}
}
/*
 * 函数名：serve_offset_shortcut
 * 描述  ：服务与偏移测试的更名键功能
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static void serve_offset_shortcut(void)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t m = 0;
	
	clear_menu();
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("模式", 208, 2);
		GUI_DispStringAt("说明", 208, 18);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Mode", 208-4, 2);
		GUI_DispStringAt("Manual", 208-4, 18);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	LCD_REFRESH();
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		
		switch(*pkey_value)
		{
			case KEY_F1:/* 模式 */
				m = offset_sel_mode();
				if(m != -1)
				{
					app_flag.offset_mode = m;
				}
				break;
			case KEY_F2:/* 说明 */
				offset_manual();
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
void dis_offset_test_ports(const TEST_PORT *pport)
{
    uint16_t *p = NULL;
    int32_t i = 0;
    uint8_t index = 0;
    uint8_t value = 0;
    uint8_t x = 0;
    uint8_t y = 0;
    
    if(pport == NULL || type_spe.port_num == 0)
    {
        return;
    }
    
    GUI_SetFont(&GUI_FontHZ_SimSun_12);
    p = (uint16_t *)pport;
    
    for(i = 0; i < type_spe.port_num; i++)
    {
        y = UI_OFFSET_PORTS_BASE_Y + UI_OFFSET_PORT_Y_O * ( i / 8);
        
        switch(type_spe.port_num)
        {
            case PORT_NUM14:
                x = UI_OFFSET_PORTS_BASE_X + UI_OFFSET_PORT_X_O * i;
                GUI_DispCharAt("- "[i % 2], x + 6, y);
                break;
            case PORT_NUM4:
                x = 10 + 12 * i;
                break;
            case PORT_NUM8:
            {
                if(i < 4)
                {
                    x = 10 + 12 * i;
                }
                else
                {
                    x = 10 + 12 * i + 6;
                }
                break;
            }
            default:
            {
                return;
            }
        }
        
        index = i % 8;
        value = ((p[i / 8]) >> (index * 2)) & 3;
        GUI_DispCharAt(PORT_STATUS_STR[value], x, y);
    }
}
/*
 * 函数名：offset_dis_step
 * 描述  ：画出一步的数据信息
 * 输入  ：n 传入第几行
 * 输出  ：无
 * 返回  ：无
 */
static void offset_dis_step(const uint8_t n)
{
	char dis[10] = {0};
	
    if(type_spe.port_num <= PORT_NUM8)
    {
        GUI_SetFont(&GUI_FontHZ_SimSun_12);
        GUI_DispStringAt((const char*)"：" , 4+35+38+15+20+4*6, 0);
        
        if(sys_par.language == CHINESE)
        {
            GUI_DispStringAt((const char*)"文件" , 4+35+38+15+25-3, 0);
        }
        else
        {
            GUI_SetFont(&GUI_FontHZ_SimSun_13);
            GUI_DispStringAt((const char*)"File" , 4+35+38+15+25, -1);
            GUI_SetFont(&GUI_FontHZ_SimSun_12);
        }
        
        if(strlen((const char*)g_cur_file->name) > 9)
        {
            strncpy(dis, (const char*)g_cur_file->name, 7);
            strcat(dis, "..");
        }
        else
        {
            strcpy(dis, (const char*)g_cur_file->name);
        }
        
        GUI_SetFont(&GUI_FontHZ_SimSun_12);
        GUI_DispStringAt((const char*)dis, 4+35+38+15+20+6*5+3, 0);
        GUI_DispStringAt((const char*)work_mode_buf[g_cur_file->work_mode], 150+7, 15);
        GUI_ClearRect(150+7+12, 15, 150+7+12+6*3, 15+10);
        GUI_DispStringAt((const char*)mode_pool[GR_VOL_DROP_EN][cur_mode], 150+7+12, 15);
        
        dis_test_vol1(&GUI_FontHZ_SimSun_12);
        dis_test_time(&GUI_FontHZ_SimSun_12);
        
        test_ui_ele_set[TEST_ELE_2].y = OFFSET_ELE_2_Y + 12 * (n - 1);
        test_ui_ele_set[TEST_ELE_3].y = OFFSET_ELE_3_Y + 12 * (n - 1);
        test_unit_dis(cur_mode, cur_gear, &GUI_FontHZ_SimSun_12);/* 显示单位 */
        
    }
    
    dis_offset_test_ports(&cur_port);
    
	LCD_REFRESH();
}
/*
 * 函数名：dis_one_step
 * 描述  ：画出一步的偏移测试信息
 * 输入  ：p 传入当前步的指针 ， n 位置信息
 * 输出  ：无
 * 返回  ：无
 */
static void dis_one_offset_step(const int8_t n)
{
	uint8_t X	= 30;
	uint8_t Y	= 15 + 12 * (n - 1);
	
	GUI_DispDecAt (cur_step, 12, Y, 2);
	GUI_DispStringAt(".", 12+6*2, Y);
    test_ui_ele_set[TEST_ELE_2].y = OFFSET_ELE_2_Y + 12 * (n - 1);
    test_ui_ele_set[TEST_ELE_3].y = OFFSET_ELE_3_Y + 12 * (n - 1);
	offset_dis(n);
    test_unit_dis(cur_mode, cur_gear, &GUI_FontHZ_SimSun_12);
	GUI_DispStringAt(offset_res_buf[cur_offset_result], X+8*6+8*6, Y);
	
	LCD_REFRESH();/* 这里不能刷屏 会造成屏幕闪烁 */
}

/*
 * 函数名：draw_offset_one_page
 * 描述  ：画出一页的偏移测试信息
 * 输入  ：p 传入当前页的第一个步的指针
 * 输出  ：无
 * 返回  ：无
 */
static void draw_offset_one_page(NODE_STEP *p)
{
	int8_t i = 0;
	NODE_STEP *p_bak = g_cur_step;
	
	g_cur_step = p;
	GUI_ClearRect (0, 15, 145-1, 64-1);
	
	LCD_REFRESH();
	for(i = 0; i < 4; i++)
	{
		if(g_cur_step != NULL)
		{
			load_data();
            
			dis_one_offset_step(i+1);
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

/*
 * 函数名：sys_offset
 * 描述  ：系统偏移功能
 * 输入  ：const int8_t w 标记调用函数
 * 输出  ：无
 * 返回  ：无
 */
void sys_offset(uint32_t w)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t n = 1;/* 行号 */
	int8_t p = 1;/* 页码 */
	int8_t cur_p_num = 0;/* 当前页的步数 */
	int8_t size = g_cur_file->total;/* 总步数 */
	int8_t pages = size/4+(size%4==0? 0:1);/* 一共的页数 */
	int8_t last_page_num = size%4==0? 4:size%4;/* 最后一页的个数 */
	NODE_STEP *p_step = list_99xx.head;
	
    list_lon = 5;
    
	if(type_spe.offset_fun_en == OFFSET_OFF)
	{
		return;
	}
    
    if(g_cur_file->work_mode == G_MODE)
    {
        test_irq_fun = test_g_irq;
    }
    else
    {
        test_irq_fun = test_irq;
    }
	
	OFFSET_BBD = TEST_OFFSET;/* 标志偏移模式 */
    init_test_ui_info(1);
	clear_();
    GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	if(w == DUMMY)
	{
		app_flag.offset_mode = 0;/* 每次进入偏移测试界面都要将测试模式设为连续状态 */
	}
	
	memset((void*)&test_flag, 0, sizeof test_flag);
l:	gui_draw_offset();
	n = 1;
    p = 1;
    load_steps_to_list(1);
    p_step = list_99xx.head;
    g_cur_step = p_step;
    load_data();
	draw_offset_one_page(p_step);
	offset_dis_step(n);
	
    /* 通信控制获取偏移，自动启动测试 */
    if(app_flag.into_get_offset)
    {
        goto ll;
    }
    
	while(1)
	{
		GUI_ClearRect (0, 15, 10, 60);
		GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+3+12*(n-1));
        
		offset_dis_step(n);
		pkey_value = OSQPend(KeyboardQSem,20,&err);
		
		if(STOP_PIN == 0)
		{
			p = g_cur_step->one_step.com.step/4+(g_cur_step->one_step.com.step%4==0? 0:1);
			if(n != 1 || p != 1)
			{
				n = 1;
				p = 1;
                
                load_steps_to_list(1);
				p_step = list_99xx.head;
				g_cur_step = list_99xx.head;
				draw_offset_one_page(p_step);
			}
			memset((void*)&test_flag, 0, sizeof test_flag);
		}
		
		switch(*pkey_value)
		{
			case KEY_UP:
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
                        
                        load_steps_to_list(4*(p-1)+1);
						p_step = list_99xx.head;
						draw_offset_one_page(p_step);
					}
				}
                
				g_cur_step = p_step + n - 1;
                load_data();
				break;
			case KEY_DOWN:
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
                        load_steps_to_list(4*(p-1)+1);
						p_step = list_99xx.head;
						draw_offset_one_page(p_step);
					}
					n = 1;
				}
				g_cur_step = p_step + n - 1;
                load_data();
				break;
			case KEY_SHORTCUT:
				de_reset();
				serve_offset_shortcut();
				draw_short_cut_bm(UDSB_RE);
				break;
			case KEY_START:
                /* 计算当前页步骤数 */
ll:       	    if(p == pages)
				{
					cur_p_num = last_page_num;
				}
				else
				{
					cur_p_num = 4;
				}
                
                load_steps_to_list(4*(p-1)+1);
				p_step = list_99xx.head;/* p_step指向当前页的第一步 */
				
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
					draw_offset_one_page(p_step);
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
                                draw_offset_one_page(p_step);
                            }
                            n = 1;
                        }
					}
				}
                
				GUI_ClearRect (0, 15, 10, 60);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+3+12*(n-1));
				offset_dis_step(n);
				g_cur_step = p_step+n-1;
				load_data();/* 更新当前步后要重新加载数据 */
				
				/* 判断一下当前步的模式是否是IR BBD CC */
				if(cur_mode == IR || cur_mode == BBD || cur_mode == CC)
				{
					/* 如果下一步是空就结束测试 */
					if(cur_step >= g_cur_file->total)
					{
                        load_steps_to_list(1);
						p_step = list_99xx.head;
						on_keyboard();/* 打开键盘 */
						dis_test_status(TEST_WAIT_INDEX, &GUI_FontHZ_SimSun_12);
                        if(app_flag.into_get_offset)
                        {
                            return;
                        }
                        
						break;/* 结束测试 */
					}
					/* 如果下一步不是空就继续 */
					else
					{
                        LCD_REFRESH();
                        OSTimeDlyHMSM(0,0,0,500);
                        OFFSET_NEXT = 1;
						goto ll;
					}
				}
				
				GUI_DispStringAt(" ",30+8*6+8*6, 15+12*(n-1));
				offset_dis_step(n);
				LCD_REFRESH();
				startup();
				
				/* 进入测试大循环 */
				for(;;)
				{
                    /* 计算显示值 */
                    if(test_flag.sample_task_en && !ERR_NUM)
                    {
                        count_dis_value();
                    }
                    
					offset_test();
                    OSTimeDlyHMSM(0,0,0,10);
					
					if(OFFSET_NEXT)
					{
                        irq_stop_relay_motion();/* 关闭电子开关 高压 */
						dis_one_offset_step(n);
						save_cur_step();/* 保存偏移结果 */
                        goto ll;
					}
					
					if(STOP)
					{
                        irq_stop_relay_motion();/* 关闭电子开关 高压 */
						STOP = 0;
						LED_ALL = LED_OFF;
						on_keyboard();/* 打开键盘 */
						dis_test_status(TEST_WAIT_INDEX, &GUI_FontHZ_SimSun_12);
                        
                        if(app_flag.into_get_offset)
                        {
                            return;
                        }
                        
						break;
					}
					
					if(PASS)
					{
						n = cur_step % 4;
						LED_PASS = 1;
						if(n == 0)
						{
							n = 4;
						}
                        
						save_cur_step();/* 保存偏移结果 */
                        irq_stop_relay_motion();/* 关闭电子开关 高压 */
						OFFSET_NEXT = 0;
						dis_one_offset_step(n);
						
						on_keyboard();/* 打开键盘 */
						dis_test_status(TEST_WAIT_INDEX, &GUI_FontHZ_SimSun_12);
                        
                        if(app_flag.into_get_offset)
                        {
                            return;
                        }
						break;
					}
					LCD_REFRESH();
				}//for(;;)
				break;
			case KEY_BACK:
				de_reset();
				g_return = 1;
				OFFSET_BBD = TEST_NORMAL;/* 标志偏移模式 */
				return ;
			case KEY_EXIT:
				OFFSET_BBD = TEST_NORMAL;/* 标志偏移模式 */
				g_exit = 1;
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
			OFFSET_BBD = TEST_NORMAL;/* 标志偏移模式 */
			return;
		}
	}//end while(1)
}


/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
