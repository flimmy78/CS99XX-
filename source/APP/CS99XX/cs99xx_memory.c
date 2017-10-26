/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：synchro_memory.c
 * 摘  要  ：记忆组数据存储于管理
 * 当前版本：V1.0，编写者：王鑫
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
#include	"RTC_Config.h"

uint8_t check_file_exist(const uint8_t n)
{
    if(n >= MAX_FILES)
    {
        return 0;
    }
    
	return (file_table[n].name[0] != 0);
}

TEST_FILE *pos_file(const uint8_t n)
{
	if(check_file_exist(n) == 0)
	{
		return NULL;
	}
    
	return &file_table[n];
}

void serve_charkey(const uint16_t keyvalue)
{
	#define DELAY_TIME_KEY 70 //延时时间
	static uint32_t g_delay_time_key;
	static uint8_t n[10] = {0};
	static uint8_t flag = 0;
	uint8_t s = 0;
	uint8_t i = 0;
	uint8_t temp_lon = strlen((const char*)pmenu_data->pData);
	
	if(g_delay_time_key)
	{
		g_delay_time_key--;//给键盘提供延时用
	}
	
	for( i = 0; i < 10; i++)
	{
		if(n[i] != 0) s = 1;//检查是否有按键按下s=1有键按下
	}
    
	if(g_delay_time_key == 0 && s)
	{
		flag = 0;
		memset(n,0,10);
		if(temp_lon < NAME_LON)
		shift_cursor(pmenu_data);
	}
    
	switch(keyvalue)
	{
		case KEY_1:
			if(flag == 0)
			{
				flag = 1;
			}
			else if(flag != 1)
			{
				flag = 1;
				memset(n,0,10);
				if(temp_lon < NAME_LON)
				shift_cursor(pmenu_data);
			}
            
			if(n[1] == 0)
			{
				g_delay_time_key = DELAY_TIME_KEY;//1s
				n[1] = 1;
			}
            
			if(g_delay_time_key)
			{
				g_delay_time_key = DELAY_TIME_KEY;
                
				if(n[1] == 1)
				{
					change_data('1');
				}
				else if(n[1] > 1)
				{
					change_data('A'+n[1]-2);
				}
                
				if(++n[1] > 4)
				{
					n[1] = 1;
				}
			}
            
			update_data(pmenu_data);
			break;
		case KEY_2:
			if(flag == 0)
			{
				flag = 2;
			}
			else if(flag != 2)
			{
				flag = 2;
				memset(n,0,10);
				if(temp_lon < NAME_LON)
				shift_cursor(pmenu_data);
			}
            
			if(n[2] == 0)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				n[2] = 1;
			}
            
			if(g_delay_time_key)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				if(n[2] == 1)
				{
					change_data('2');
				}
				else if(n[2] > 1)
				{
					change_data('D'+n[2]-2);
				}
				if(++n[2] > 4)
				{
					n[2] = 1;
				}
			}
            
			update_data(pmenu_data);
			break;
		case KEY_3:
			if(flag == 0)
			{
				flag = 3;
			}
			else if(flag != 3)
			{
				flag = 3;
				memset(n,0,10);
				if(temp_lon < NAME_LON)
				shift_cursor(pmenu_data);
			}
			if(n[3] == 0)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				n[3] = 1;
			}
			if(g_delay_time_key)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				if(n[3] == 1)
				{
					change_data('3');
				}
				else if(n[3] > 1)
				{
					change_data('G'+n[3]-2);
				}
				if(++n[3] > 4)
				{
					n[3] = 1;
				}
			}
			update_data(pmenu_data);
			break;
		case KEY_4:
			if(flag == 0)
			{
				flag = 4;
			}
			else if(flag != 4)
			{
				flag = 4;
				memset(n,0,10);
				if(temp_lon < NAME_LON)
				shift_cursor(pmenu_data);
			}
			if(n[4] == 0)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				n[4] = 1;
			}
			if(g_delay_time_key)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				if(n[4] == 1)
				{
					change_data('4');
				}
				else if(n[4] > 1)
				{
					change_data('J'+n[4]-2);
				}
				if(++n[4] > 4)
				{
					n[4] = 1;
				}
			}
			update_data(pmenu_data);
			break;
		case KEY_5:
			if(flag == 0)
			{
				flag = 5;
			}
			else if(flag != 5)
			{
				flag = 5;
				memset(n,0,10);
				if(temp_lon < NAME_LON)
				shift_cursor(pmenu_data);
			}
			if(n[5] == 0)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				n[5] = 1;
			}
			if(g_delay_time_key)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				if(n[5] == 1)
				{
					change_data('5');
				}
				else if(n[5] > 1)
				{
					change_data('M'+n[5]-2);
				}
				if(++n[5] > 4)
				{
					n[5] = 1;
				}
			}
			update_data(pmenu_data);
			break;
		case KEY_6:
			if(flag == 0)
			{
				flag = 6;
			}
			else if(flag != 6)
			{
				flag = 6;
				memset(n,0,10);
				if(temp_lon < NAME_LON)
				shift_cursor(pmenu_data);
			}
			if(n[6] == 0)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				n[6] = 1;
			}
			if(g_delay_time_key)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				if(n[6] == 1)
				{
					change_data('6');
				}
				else if(n[6] > 1)
				{
					change_data('P'+n[6]-2);
				}
				if(++n[6] > 4)
				{
					n[6] = 1;
				}
			}
			update_data(pmenu_data);
			break;
		case KEY_7:
			if(flag == 0)
			{
				flag = 7;
			}
			else if(flag != 7)
			{
				flag = 7;
				memset(n,0,10);
				if(temp_lon < NAME_LON)
				shift_cursor(pmenu_data);
			}
			if(n[7] == 0)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				n[7] = 1;
			}
			if(g_delay_time_key)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				if(n[7] == 1)
				{
					change_data('7');
				}
				else if(n[7] > 1)
				{
					change_data('S'+n[7]-2);
				}
				if(++n[7] > 4)
				{
					n[7] = 1;
				}
			}
			update_data(pmenu_data);
			break;
		case KEY_8:
			if(flag == 0)
			{
				flag = 8;
			}
			else if(flag != 8)
			{
				flag = 8;
				memset(n,0,10);
				if(temp_lon < NAME_LON)
				shift_cursor(pmenu_data);
			}
			if(n[8] == 0)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				n[8] = 1;
			}
			if(g_delay_time_key)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				if(n[8] == 1)
				{
					change_data('8');
				}
				else if(n[8] > 1)
				{
					change_data('V'+n[8]-2);
				}
				if(++n[8] > 4)
				{
					n[8] = 1;
				}
			}
			update_data(pmenu_data);
			break;
		case KEY_9:
			if(flag == 0)
			{
				flag = 9;
			}
			else if(flag != 9)
			{
				flag = 9;
				memset(n,0,10);
				if(temp_lon < NAME_LON)
				shift_cursor(pmenu_data);
			}
			if(n[9] == 0)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				n[9] = 1;
			}
			if(g_delay_time_key)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				if(n[9] == 1)
				{
					change_data('9');
				}
				else if(n[9] > 1)
				{
					change_data('Y'+n[9]-2);
				}
				if(++n[9] > 3)
				{
					n[9] = 1;
				}
			}
			update_data(pmenu_data);
			break;
		case KEY_0:
			if(flag == 0)
			{
				flag = 10;
			}
			else if(flag != 10)
			{
				flag = 10;
				memset(n,0,10);
				if(temp_lon < NAME_LON)
				shift_cursor(pmenu_data);
			}
			if(n[0] == 0)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				n[0] = 1;
			}
			if(g_delay_time_key)
			{
				g_delay_time_key = DELAY_TIME_KEY;
				if(n[0] == 1)
				{
					change_data('-');
				}
				else if(n[0] == 2)
				{
					change_data('0');
				}
				else if(n[0] == 3)
				{
					change_data('/');
				}
				if(++n[0] > 3)
				{
					n[0] = 1;
				}
			}
			update_data(pmenu_data);
			break;
		default:
			break;
	}
}

static void server_save(TEST_FILE * pf)
{
    uint8_t X0	= 40;
    uint8_t X1	= 170;
    uint8_t Y0	= 5;
    uint8_t Y1	= 57;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t i = 0;
	
	char file_name[NAME_LON+1] = {0};
    uint8_t x = 0;
    uint8_t y = 0;
	
	DATA_INFO file_name_inf = {
		(void*)0/*pData*/,0/*x*/,0/*y*/,2/*type*/,NAME_LON/*lon*/,0/*decs*/,0/*total*/,0/*cur*/,0/*ch_bit*/,0/*page*/,3/*bits*/,
	};
    
    file_name_inf.x =  ((X0 + (X0 + X1) / 2) / 2 - 5);
    file_name_inf.y = Y0 + 28;
    x = file_name_inf.x;
    y = file_name_inf.y;
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("请输入文件名", X0+30, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Input the file name", X0+6, Y0+2);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);
	GUI_ClearRect (200+1, 15+16+1, 240-1, 15+16+16-1);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmleftshift, 210, 0);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 16);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdel, 210, 34);
	
	strcpy(file_name, (const char*)pf->name);
	GUI_DispStringAt(file_name, x, y);
	pmenu_data = &file_name_inf;
	pmenu_data->pData = (uint8_t*)pmenu_data->pData;
	pmenu_data->pData = file_name;
	pmenu_data->ch_bit = 0;
	LCD_REFRESH();
	cursor_pos(x,y);
    
	while(1)
	{
		uint8_t temp = 0;
		uint8_t temp_lon = 0;
		if(file_name[0] == '\0')
		{
			pmenu_data->ch_bit = 0;//此处是解决那个不显示的问题
		}
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		serve_charkey(*pkey_value);
		temp = pmenu_data->ch_bit;
		temp_lon = strlen((const char*)file_name);
		
		switch(*pkey_value)
		{
			//利用右移来处理左移,首先将pmenu_data->ch_bit = 0;然后再确定向右移动多少位
			case KEY_SHIFT_L:
				pmenu_data->last_ch_bit = pmenu_data->ch_bit;//更新last_ch_bit
				shift_cursor_l(pmenu_data);
				break;
			case KEY_SHIFT_R:
				shift_cursor(pmenu_data);
				break;
			case KEY_FILE_DEL:
				for(i = 0; i < temp_lon - temp;i++)
				{
					file_name[temp+i] = file_name[temp+i+1];
				}
				
				GUI_ClearRect (pmenu_data->x, pmenu_data->y, pmenu_data->x+14*6, pmenu_data->y+12);
				GUI_DispStringAt((const char*)file_name, pmenu_data->x, pmenu_data->y);
				LCD_REFRESH();
				
				if(pmenu_data->ch_bit >= temp_lon-1)	/* 如果删除的是最后一位就将光标左移一下 */
				{
					shift_cursor_l(pmenu_data);
				}
				else	/* 否则删除该位后光标不移动 */
				{
					cursor_pos(pmenu_data->x+6*pmenu_data->ch_bit, y);
				}
				break;
			case KEY_ENTER:
				if(file_name[0] == '\0')	/* 判断文件名是否为空 */
				{
					break;
				}
				
				strcpy((char*)pf->name, (const char*)file_name);
				pf->create_time = Time_GetTime();/* 保存当前RTC时间 */
				GetTime();	/* 更新下gUpdataTime的内容 */
				(gUpdateTime+2)[14] = 0;	/* 截取数据 */
				strcpy((char*)transform_time_to_str_for_mem(pf->create_time), (const char*)(gUpdateTime+2));
				GUI_ClearRect (X0, Y0, X1, Y1);
				return;
			case KEY_BACK:
				GUI_ClearRect (X0, Y0, X1, Y1);
				g_return = 1;
				memset(pf->name, 0, 15);//退出前取消已输入的文件名
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			default:
				break;
		}
	}
}
static void server_new1(TEST_FILE * pf, TEST_FILE * pgf)
{
	server_save(pf);
    
    /* 只在新建文件是才执行 */
    if(pgf->name[0] == '\0')
    {
        /* 当文件名设置有效时，就将文件的其他参数恢复为默认值 */
        if(pf->name[0] != '\0')
        {
            uint8_t n = pgf->num;
            char str[15]={0};
            uint32_t temp_time = 0;
            
            strcpy(str, pf->name);
            temp_time = pf->create_time;
            *pf = default_file;
            pf->num = n;
            strcpy(pf->name, str);
            pf->create_time = temp_time;
        }
    }
}

static int8_t prompt_for_ch_work_mode(void)
{
	uint8_t X0 = 20;
	uint8_t X1 = 180;
	uint8_t Y0 = 5;
	uint8_t Y1 = 57;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;

	uint8_t n = 1;
    uint8_t res = 0;
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("更改模式会使文", 0+X0+35, Y0+5);
		GUI_DispStringAt("件测试信息丢失！", 0+X0+35, Y0+5+15);
		GUI_DispStringAt("继续", 0+X0+25+10, Y0+5+15+17);
		GUI_DispStringAt("取消", 0+X0+25+10+50, Y0+5+15+17);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Mode change will", 0+X0+35, Y0+5);
		GUI_DispStringAt("make test info lost!", 0+X0+35, Y0+5+15);
		GUI_DispStringAt("Cont.", 0+X0+25+10, Y0+5+15+17);
		GUI_DispStringAt("Cancel", 0+X0+25+10+50, Y0+5+15+17);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, X0+3, Y0+5);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, X0+25, Y0+5+15+17+3);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,  X0+25+50, Y0+5+15+17+3);
	cursor_pos(X0+35+1, Y0+5+15+17);
	LCD_REFRESH();
	while(1)
	{
		switch(n)
		{
			case 1:/* 继续 */
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, X0+25, Y0+5+15+17+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,  X0+25+50, Y0+5+15+17+3);
				cursor_pos(X0+25+1, Y0+5+15+17);
				break;
			case 0:/* 取消 */
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+25, Y0+5+15+17+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,  X0+25+50, Y0+5+15+17+3);
				cursor_pos(X0+25+50+1, Y0+5+15+17);
				break;
			default:
				break;
		}
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);

		switch(*pkey_value)
		{
			case KEY_SHIFT:
				if(++n > 1)
				{
					n = 0;
				}
				break;
			case KEY_ENTER:
                res = n;
                goto l_end;
			case KEY_BACK:
				g_return = 1;
                res = 0;
                goto l_end;
			case KEY_EXIT:
                res = 0;
				g_exit = 1;
                goto l_end;
			default:
				break;
		}
	}
    
l_end:
    GUI_ClearRect (X0, Y0, X1, Y1);
    return res;
}
static void server_new2(TEST_FILE * pf, TEST_FILE * pgf)
{
	uint8_t X0 = 40;
	uint8_t X1 = 170;
	uint8_t Y0 = 5;
	uint8_t Y1 = 57;
    
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t n = pf->work_mode;
	uint8_t m = n;
l:
	n = pf->work_mode;
	m = n;
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("设置工作模式", X0+30, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Set mode", X0+40, Y0+3);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
    draw_short_cut_bm(__RB);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("正常模式", X0+15, Y0+30);
		GUI_DispStringAt("梯度模式", X0+15+60, Y0+30);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Normal", X0+15, Y0+30);
		GUI_DispStringAt("Gradient", X0+15+60, Y0+30);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y0+30+3);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+60, Y0+30+3);
	
	while(1)
	{
		switch(n)
		{
			case N_MODE:/* N模式 */
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y0+30+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+60, Y0+30+3);
				cursor_pos(X0+5+1, Y0+30);
				break;
			case G_MODE:/* G模式 */
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,X0+5, Y0+30+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, X0+5+60, Y0+30+3);
				cursor_pos(X0+5+60+1, Y0+30);
				break;
			default:
				break;
		}
        
		pkey_value = OSQPend(KeyboardQSem,10,&err);
        
		switch(*pkey_value)
		{
			case KEY_SHIFT:
				/* 当前机型是否支持G模式 */
				if(type_spe.g_mode_en == ENABLE)
				{
                    if(n == N_MODE)
                    {
                        n = G_MODE;
                    }
                    else
                    {
                        n = N_MODE;
                    }
				}
				break;
			case KEY_ENTER:
				if(pgf->name[0]!='\0' && n!= m)
				{
					/* 询问是否要更改模式 */
					if(0 == prompt_for_ch_work_mode())
					{
						goto l;/* 放弃 */
					}
				}
				pf->work_mode = n;
				return;
			case KEY_BACK:
				GUI_ClearRect (40, 5, 170, 57);
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
static void server_new3(TEST_FILE * pf, TEST_FILE * pgf)
{
	uint8_t X0 = 40;
	uint8_t X1 = 170;
	uint8_t Y0 = 5;
	uint8_t Y1 = 57;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t temp = 0;
	uint16_t t = 0;//存放权数
	uint16_t temp_pass_time = pf->hold_pass_time;
	
    uint8_t x = ((X0+(X0+X1)/2)/2-15);
    uint8_t y = Y1-25;
	
	DATA_INFO pass_time = {
		(void*)0,//uint32_t* pData;//数据
		0,	//数据的x坐标
		0,	//数据的y坐标
		1,	//0表示int,1表示float,2表示字符串char[]
		5,	//显示数据长度
		1,	//小数点后几位
		0,	//标记当前页显示的数据总数
		0,  //标记当前操作的是第几个数据
		0,	//当前修改的位
		0,	//当前页码
		1,	//表明当前值是8位，16位，32位，0代表8位，1代表16位，2代表32位,3表示字符串
	};
    
    pass_time.x = x;
    pass_time.y = y;
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+21, (X0+X1)/2, Y1-3);
	
	GUI_DispStringAt ("999.9s", (X0+(X0+X1)/2)/2-15, Y1-25);
	GUI_DispStringAt ("000.0s", (X1+(X0+X1)/2)/2-18, Y0+18);
	GUI_DispStringAt ("999.9s", (X1+(X0+X1)/2)/2-18, Y1-12);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, (X1+(X0+X1)/2)/2-3, Y0+30);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("设置PASS信号保持", X0+20, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Set pass keep", X0+30, Y0+2);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmshortcut, 210, 34);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	
	GUI_GotoXY(x, y);
	GUI_DispFloatFix ((float)temp_pass_time/10, 5, 1);
	
	pmenu_data = &pass_time;
	pmenu_data->pData = (uint16_t*)pmenu_data->pData;
	pmenu_data->pData = &temp_pass_time;
	pmenu_data->ch_bit = 0;
	LCD_REFRESH();
	cursor_pos(x,y);
	while(1)
	{
		
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_pass_time/t;
				temp = t%10;

				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(x+pmenu_data->ch_bit*6,y);//显示光标
				break;
			case KEY_DOWN:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_pass_time/t;
				temp = t%10;

				if(temp == 0)
				{
					temp = 9;
				}
				else
				{
					temp--;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(x+pmenu_data->ch_bit*6,y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				temp = 0;
				break;
			case KEY_ENTER:
				if(temp_pass_time > 9999)
				{
					temp_pass_time = 9999;
				}
				pf->hold_pass_time = temp_pass_time;
				return;
			case KEY_BACK:
				if(temp_pass_time > 9999)
				{
					temp_pass_time = 9999;
				}
				pf->hold_pass_time = temp_pass_time;
				GUI_ClearRect (40, 5, 170, 57);
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
static void server_new4(TEST_FILE * pf, TEST_FILE * pgf)
{
	uint8_t X0	= 40;
	uint8_t X1	= 170;
	uint8_t Y0	= 5;
	uint8_t Y1	= 57;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t temp = 0;
	uint16_t t = 0;//存放权数
	uint16_t temp_buzz_time = pf->pass_buzzing_time;
	
    uint8_t x = ((X0+(X0+X1)/2)/2-15);
    uint8_t y = Y1-25;
	
	DATA_INFO puzz_time =
	{
		(void*)0/*pData*/,0/*x*/,0/*y*/,1/*type*/,5/*lon*/,1/*decs*/,0/*total*/,0/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
    
    puzz_time.x = x;
    puzz_time.y = y;
	
	GUI_DrawLine         (X0, Y0, X1, Y0);
	GUI_DrawLine         (X0, Y1, X1, Y1);
	GUI_DrawLine         (X0, Y0, X0, Y1);
	GUI_DrawLine         (X1, Y0, X1, Y1);
	GUI_ClearRect 		 (X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine         (X0, Y0+17, X1, Y0+17);
	GUI_DrawLine         ((X0+X1)/2, Y0+18, (X0+X1)/2, Y1-3);
	
	GUI_DispStringAt ("999.9s", (X0+(X0+X1)/2)/2-15, Y1-25);
	GUI_DispStringAt ("000.0s", (X1+(X0+X1)/2)/2-18, Y0+18);
	GUI_DispStringAt ("999.9s", (X1+(X0+X1)/2)/2-18, Y1-12);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, (X1+(X0+X1)/2)/2-3, Y0+30);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("编辑PASS蜂鸣保持", X0+20, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Set pass buzzer keep", X0+6, Y0+2);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmshortcut, 210, 34);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	
	GUI_GotoXY(x, y);
	GUI_DispFloatFix ((float)temp_buzz_time/10, 5, 1);
	
	pmenu_data = &puzz_time;
	pmenu_data->pData = (uint16_t*)pmenu_data->pData;
	pmenu_data->pData = &temp_buzz_time;
	pmenu_data->ch_bit = 0;
	LCD_REFRESH();
	cursor_pos(x,y);
	while(1)
	{
		
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_buzz_time/t;
				temp = t%10;

				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(x+pmenu_data->ch_bit*6,y);//显示光标
				break;
			case KEY_DOWN:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_buzz_time/t;
				temp = t%10;

				if(temp == 0)
				{
					temp = 9;
				}
				else
				{
					temp--;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(x+pmenu_data->ch_bit*6,y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				temp = 0;
				break;
			case KEY_ENTER:
				if(temp_buzz_time > 9999)
				{
					temp_buzz_time = 9999;
				}
				pf->pass_buzzing_time = temp_buzz_time;
				return;
			case KEY_BACK:
				GUI_ClearRect (40, 5, 170, 57);
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
static void server_new5(TEST_FILE * pf, TEST_FILE * pgf)
{
	uint8_t X0	= 40;
	uint8_t X1	= 170;
	uint8_t Y0	= 5;
	uint8_t Y1	= 57;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t n = pf->arc_mode;
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};/* 放模式的名字 */
	uint8_t flag[MODEL_KINDS_MAX] = {0};/* 他里面放模式的排序 */
	int16_t kinds = 0;/* 放模式的数目 */
	
	define_modes(mode_buf, flag, &kinds);
	
	GUI_DrawLine         (X0, Y0, X1, Y0);
	GUI_DrawLine         (X0, Y1, X1, Y1);
	GUI_DrawLine         (X0, Y0, X0, Y1);
	GUI_DrawLine         (X1, Y0, X1, Y1);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine         (X0, Y0+17, X1, Y0+17);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("编辑电弧侦测模式", X0+20, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Set arc mode", X0+28, Y0+3);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);

	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("电流模式", X0+15, Y0+30);
		GUI_DispStringAt("等级模式", X0+15+60, Y0+30);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Current", X0+15, Y0+30);
		GUI_DispStringAt("Scale", X0+15+60, Y0+30);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y0+30+3);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+60, Y0+30+3);
	
	while(1)
	{
		switch(n)
		{
			case ARC_CUR_MODEL:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y0+30+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+60, Y0+30+3);
				cursor_pos(X0+5+1, Y0+30);
				break;
			case ARC_GRADE_MODEL:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,X0+5, Y0+30+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, X0+5+60, Y0+30+3);
				cursor_pos(X0+5+60+1, Y0+30);
				break;
			default:
				n = ARC_CUR_MODEL;
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y0+30+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+60, Y0+30+3);
				cursor_pos(X0+5+1, Y0+30);
				break;
		}
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		
		switch(*pkey_value)
		{
			case KEY_SHIFT:
			{
				if(n == ARC_CUR_MODEL)
				{
					n = ARC_GRADE_MODEL;
				}
				else
				{
					n = ARC_CUR_MODEL;
				}
				break;
			}
			case KEY_ENTER:
			{
				pf->arc_mode = n;				
				return;
			}
			case KEY_BACK:
				GUI_ClearRect (40, 5, 170, 57);
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

static void save_and_updata_new_file(TEST_FILE * pf, TEST_FILE * pgf)
{
	int8_t num_bak = pgf->num;/* 备份文件编号 */
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};/* 放模式的名字 */
	uint8_t flag[MODEL_KINDS_MAX] = {0};/* 他里面放模式的排序 */
	int16_t kinds = 0;/* 放模式的数目 */
	
	define_modes(mode_buf, flag, &kinds);
	/* 如果临时文件中的文件名为空说明出错立即返回 */
	if(pf->name[0] == '\0')
	{
		return;
	}
    
	/* 新建文件时就走这里 内存中的文件名为空而临时文件中的文件名不空就说明是新建的文件了 */
	if(pgf->name[0] == '\0' && pf->name[0] != '\0')
	{
		*pgf = *pf;//更新到内存
		pgf->num = num_bak;/* 恢复文件编号 */
		
		g_cur_file = pgf;//更新文件全局指针
        g_cur_file->total = 0;/* 必须再清一次否则会出意外 */
        
        clear_step_used_flag();
        clear_group_table();
        
        insert_step(0, flag[1]);
        
        save_group_info(g_cur_file->num);
        
		sys_flag.last_group = g_cur_file->num;/* 更新最后使用的文件标志 */
		save_sys_flag();
		
        load_steps_to_list(1);
        g_cur_step = list_99xx.head;
		return;
	}
}

static void edit_and_updata_old_file(TEST_FILE * pf, TEST_FILE * pgf)
{
	int8_t num_bak = pgf->num;/* 备份文件编号 */
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};/* 放模式的名字 */
	uint8_t flag[MODEL_KINDS_MAX] = {0};/* 他里面放模式的排序 */
	int16_t kinds = 0;/* 放模式的数目 */
	
	define_modes(mode_buf, flag, &kinds);
	
	/* 编辑文件工作模式改变了 */
	if(pf->work_mode != pgf->work_mode)/* 编辑文件走这里 */
	{
        TEST_FILE *p_file = g_cur_file;
		/* 如果改变 已有文件的工作模式那么，文件中原有的测试步骤就需要删除，*/
		
        *pgf = *pf;//更新到内存
        pgf->num = num_bak;/* 恢复文件编号 */
        
        g_cur_file = pgf;
        
        clear_step_used_flag();
        clear_group_table();
        g_cur_file->total = 0;
        
        insert_step(0, flag[1]);
        
        save_group_info(g_cur_file->num);
        
		/* 编辑的文件是当前文件指针所指的文件需要将步骤号更新到 */
		if(p_file == pgf)
		{
            load_steps_to_list(1);
			g_cur_step = list_99xx.head;	/* 加载当前步 */
		}
        
        g_cur_file = p_file;
	}
	/* 编辑的工作模式未改变 */
	else if(pf->work_mode == pgf->work_mode)/* 编辑文件走这里 */
	{
        *pgf = *pf;//更新到内存
        pgf->num = num_bak;/* 恢复文件编号 */
		save_file(pgf->num);
	}
}


static void init_new_and_edit_path(void (*n_file_fun[])(TEST_FILE * pf, TEST_FILE * pgf), uint8_t *n)
{
    uint8_t temp = 0;
    
    if(GR_VOL_DROP_EN)
    {
        n_file_fun[temp++] = server_new1;
        n_file_fun[temp++] = server_new3;
    }
    else if(type_spe.single_gr)
    {
        n_file_fun[temp++] = server_new1;
        n_file_fun[temp++] = server_new3;
        n_file_fun[temp++] = server_new4;
    }
    else if(type_spe.dcw_big_cap_en == ENABLE)
    {
        n_file_fun[temp++] = server_new1;
        n_file_fun[temp++] = server_new3;
        n_file_fun[temp++] = server_new4;
        n_file_fun[temp++] = server_new5;
    }
    else if(type_spe.support_g_mode == 0)
    {
        n_file_fun[temp++] = server_new1;
        n_file_fun[temp++] = server_new3;
        n_file_fun[temp++] = server_new4;
    }
    else
    {
        n_file_fun[temp++] = server_new1;
        n_file_fun[temp++] = server_new2;
        n_file_fun[temp++] = server_new3;
        n_file_fun[temp++] = server_new4;
        n_file_fun[temp++] = server_new5;
    }
    
    *n = temp;
}
static void server_mem_shortcut_nonexist(TEST_FILE * pf, TEST_FILE * pgf)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t step = 0;
	uint8_t n = 0;
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("存贮", 208, 2);
		GUI_DispStringAt("新建", 208, 18);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Save", 208, 2);
		GUI_DispStringAt("New", 208, 18);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
	LCD_REFRESH();
    
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		switch(*pkey_value)
		{
			case KEY_SAVE:
			{
				server_save(pf);
				//更新到file_table
				if(pf->name[0] != '\0')
				{
					n = pgf->num;/* 备份文件编号 */
                    
					*pgf = *pf;
					pgf->num = n;/* 恢复文件编号 */
					*pf = default_file;/* 再将临时文件恢复为默认值 */
					memset(pf->name, 0, 15);/* 清空文件名 */
					
					/* 将当前文件记忆组中的内容复制到保存文件记忆组中 */
					pgf->total = g_cur_file->total;
					copy_cur_file_to_new_pos(pgf->num);
				}
				return;
			}
			case KEY_NEW://新建一个文件
			{
                uint8_t temp = 0;
                void (*n_file_fun[5])(TEST_FILE * pf, TEST_FILE * pgf);
                
                memset(n_file_fun, 0, sizeof n_file_fun);
                
				if(sys_par.keyboard_lock && gunlock == 0)
				{
					if(0 != authenticate_pw(0))
					{
						break;
					}
					else
					{
						gunlock = 1;
					}
				}
                
//                 if(GR_VOL_DROP_EN)
//                 {
//                     n_file_fun[temp++] = server_new1;
//                     n_file_fun[temp++] = server_new3;
//                 }
//                 else
//                 {
//                     n_file_fun[temp++] = server_new1;
//                     n_file_fun[temp++] = server_new2;
//                     n_file_fun[temp++] = server_new3;
//                     n_file_fun[temp++] = server_new4;
//                     n_file_fun[temp++] = server_new5;
//                 }
                init_new_and_edit_path(n_file_fun, &temp);
                
                step = 0;
                
				while(1)
				{
                    if(n_file_fun[step] != NULL)
                    {
                        n_file_fun[step](pf, pgf);
                    }
                    
					if(g_return)
					{
                        g_return = 0;
                        
                        if(--step < 0)
                        {
                            return;
                        }
					}
					else
					{
						if(++step >= temp)
                        {
							if(g_return == 0 && g_exit == 0)
							{
								save_and_updata_new_file(pf, pgf);
								test_flag.into_set = 1;//标示新建工作已完成马上进入设置界面
								g_exit = 1;//这两个标志要同时置1,意思是返回到主界面然后再进入设置界面
								return;
							}
                        }
					}
                    
					if(g_exit)
					{
						return;
					}
				}
			}
			case KEY_ENTER:
				break;
			case KEY_BACK:
				return;
			case KEY_EXIT:
				return;
		}
		if(g_return || g_exit)
		{
			g_return = 0;
			return;
		}
	}
}

static int8_t is_cover_or_read_or_del(const uint8_t w)
{
	uint8_t X0 = 60;
	uint8_t X1 = 130;
	uint8_t Y0 = 15;
	uint8_t Y1 = 49;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;

	uint8_t n = 1;//pf->work_mode;
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		switch(w)
		{
			case 0:
				GUI_DispStringAt("是否覆盖", X0+12, Y0+3);
				break;
			case 1:
				GUI_DispStringAt("是否读取", X0+12, Y0+3);
				break;
			case 2:
				GUI_DispStringAt("是否删除", X0+12, Y0+3);
				break;
			default:
				break;
		}
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		switch(w)
		{
			case 0:
				GUI_DispStringAt("Cover", X0+20, Y0+3);
				break;
			case 1:
				GUI_DispStringAt("Read", X0+20, Y0+3);
				break;
			case 2:
				GUI_DispStringAt("Delete", X0+20, Y0+3);
				break;
			default:
				break;
		}
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);
	GUI_ClearRect (200+1, 15+16+1, 240-1, 15+16+16-1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y1-14+3);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+35, Y1-14+3);
	cursor_pos(X0+5+1, Y1-14);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("是", X0+15, Y1-14);
		GUI_DispStringAt("否", X0+15+35, Y1-14);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Yes", X0+15, Y1-14);
		GUI_DispStringAt("No", X0+15+35, Y1-14);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	while(1)
	{
		switch(n)
		{
			case 1:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y1-14+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+35, Y1-14+3);
				cursor_pos(X0+5+1, Y1-14);
				break;
			case 0:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,X0+5, Y1-14+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, X0+5+35, Y1-14+3);
				cursor_pos(X0+5+35+1, Y1-14);
				break;
			default:
				break;
		}
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);

		switch(*pkey_value)
		{
			case KEY_SHIFT:
				if(++n > 1)
				{
					n = 0;
				}
				break;
			case KEY_ENTER:
				return n;
			case KEY_BACK:
				GUI_ClearRect (40, 5, 170, 57);
				g_return = 1;
				return 0;
			case KEY_EXIT:
				g_exit = 1;
				return 0;
			default:
				break;
		}
	}
}

static uint8_t set_del_mode(void)
{
	uint8_t X0 = 40;
	uint8_t X1 = 170;
	uint8_t Y0 = 5;
	uint8_t Y1 = 57;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;

	uint8_t n = 1;//pf->work_mode;
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);

	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("设置删除模式", X0+30, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Set delete mode", X0+20, Y0+3);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);
	GUI_ClearRect (200+1, 15+16+1, 240-1, 15+16+16-1);

	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y0+30+3);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+64, Y0+30+3);
	cursor_pos(X0+5+1, Y0+30);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("删除单项", X0+15, Y0+30);
		GUI_DispStringAt("删除全部", X0+15+64, Y0+30);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Del sigle", X0+15, Y0+30);
		GUI_DispStringAt("Del all", X0+15+64, Y0+30);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	while(1)
	{
		switch(n)
		{
			case 1:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y0+30+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+64, Y0+30+3);
				cursor_pos(X0+5+1, Y0+30);
				break;
			case 2:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,X0+5, Y0+30+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, X0+5+64, Y0+30+3);
				cursor_pos(X0+5+64+1, Y0+30);
				break;
			default:
				break;
		}
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);

		switch(*pkey_value)
		{
			case KEY_SHIFT:
				if(++n > 2)
				{
					n = 1;
				}
				break;
			case KEY_ENTER:
				return n;
			case KEY_BACK:
				GUI_ClearRect (40, 5, 170, 57);
				g_return = 1;
				return 0;	/* 返回0表示放弃 */
			case KEY_EXIT:
				g_exit = 1;
				return 0;
			default:
				break;
		}
	}
}

static void prompt_for_new_del(void)
{
	uint8_t X0 = 40;
	uint8_t X1 = 170;
	uint8_t Y0 = 5;
	uint8_t Y1 = 57;
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("数据操作中，按", 0+X0+35, Y0+5);
		GUI_DispStringAt("键无效，不要关", 0+X0+35, Y0+5+15);
		GUI_DispStringAt("机！请稍候...", 0+X0+35, Y0+5+15+15);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Operating!", 0+X0+35, Y0+5);
		GUI_DispStringAt("Don't shut down!", 0+X0+35, Y0+5+15);
		GUI_DispStringAt("Please wait!", 0+X0+35, Y0+5+15+15);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	GUI_DrawLine         (X0, Y0, X1, Y0);
	GUI_DrawLine         (X0, Y1, X1, Y1);
	GUI_DrawLine         (X0, Y0, X0, Y1);
	GUI_DrawLine         (X1, Y0, X1, Y1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, X0+3, Y0+5);
	
	LCD_REFRESH();
	OSTimeDlyHMSM(0,0,0,200);
}
static int8_t prompt_for_del_all(void)
{
	uint8_t X0 = 40;
	uint8_t X1 = 180;
	uint8_t Y0 = 5;
	uint8_t Y1 = 57;
    
	uint16_t* pkey_value = NULL;
	INT8U err = 0;

	uint8_t n = 1;
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("删除全部会使文", 0+X0+35, Y0+5);
		GUI_DispStringAt("件所有信息丢失！", 0+X0+35, Y0+5+15);
		GUI_DispStringAt("继续", 0+X0+35+10, Y0+5+15+17);
		GUI_DispStringAt("取消", 0+X0+35+10+50, Y0+5+15+17);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("File delete will", 0+X0+35, Y0+5);
		GUI_DispStringAt("make file lost!", 0+X0+35, Y0+5+15);
		GUI_DispStringAt("Cont.", 0+X0+25+10, Y0+5+15+17);
		GUI_DispStringAt("Cancel", 0+X0+25+10+50, Y0+5+15+17);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
	GUI_DrawLine         (X0, Y0, X1, Y0);
	GUI_DrawLine         (X0, Y1, X1, Y1);
	GUI_DrawLine         (X0, Y0, X0, Y1);
	GUI_DrawLine         (X1, Y0, X1, Y1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, X0+3, Y0+5);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, X0+25, Y0+5+15+17+3);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,  X0+25+50, Y0+5+15+17+3);
	cursor_pos(X0+35+1, Y0+5+15+17);
	LCD_REFRESH();
    
	while(1)
	{
		switch(n)
		{
			case 1:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, X0+25, Y0+5+15+17+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,  X0+25+50, Y0+5+15+17+3);
				cursor_pos(X0+25+1, Y0+5+15+17);
				break;
			case 0:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+25, Y0+5+15+17+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,  X0+25+50, Y0+5+15+17+3);
				cursor_pos(X0+25+50+1, Y0+5+15+17);
				break;
			default:
				break;
		}
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);
        
		switch(*pkey_value)
		{
			case KEY_SHIFT:
				if(++n > 1)
				{
					n = 0;
				}
				break;
			case KEY_ENTER:
				return n;
			case KEY_BACK:
				GUI_ClearRect (40, 5, 170, 57);
				g_return = 1;
				return 0;
			case KEY_EXIT:
				g_exit = 1;
				return 0;
			default:
				break;
		}
	}
}


/*
 * 函数名：server_more_file
 * 描述  ：当检验人员成功更新检验时间时弹出OK提示窗 
 * 输入  ：TEST_FILE * pf, 指向临时的文件结构数据
 *			TEST_FILE * pgf 指向全局的文件结构数据
 * 输出  ：TEST_FILE * pgf 当按下ENTER键时将临时数据保存到全局数据中
 * 返回  ：int8_t	：-1表示放弃操作如在询问是否删除时选择了否或按下了返回键
 *					  0表示在当前界面未做任何操作，直接按下返回键或exit键
 */
static int8_t server_more_file(TEST_FILE * pf, TEST_FILE * pgf)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t step = 0;
	int8_t n = 1;
	
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 31-1);
	GUI_ClearRect (200+1, 31+1, 240-1, 47-1);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("删除", 208, 2);
		GUI_DispStringAt("编辑", 208, 18);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Delete", 206, 2);
		GUI_DispStringAt("Edit", 206, 18);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
	LCD_REFRESH();
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		switch(*pkey_value)
		{
			case KEY_DEL_FILE:
			{
				if(sys_par.keyboard_lock && gunlock==0)	/* 如果键盘锁打开就进行身份认证 */
				{
					if(0 != authenticate_pw(0))
					{
						return -1;	/* 密码错误返回 */
					}
					else
					{
						gunlock = 1;
					}
				}
                
				n = is_cover_or_read_or_del(2);
                
				if(n)	/* n为非零时表示允许操作继续进行 */
				{
			l:
					n = set_del_mode();
					 /* 放弃操作 */
					if(n == 0)
					{
						return -1; /* give up */
					}
					//删除单项
					else if(n == 1)
					{
                        del_file(pgf->num);
						
						/* 读出最新的记忆组 */
						if(read_list(g_cur_file->num))
						{
							sys_flag.mem_init = 0;
                            save_sys_flag();
						}
						
						g_cur_step = list_99xx.head;
					}
					else if(n == 2)
					{
						n = prompt_for_del_all();
						if(g_exit)
						{
							return -1;	/* give up */
						}
						//删除全部
						if(n)
						{
							del_all_file();
                            
							/* 读出最新的记忆组 */
							if(read_list(g_cur_file->num))
							{
								sys_flag.mem_init = 0;
								save_sys_flag();
							}
							
							g_cur_step = list_99xx.head;
						}
						else
						{
							goto l;//如果返回0就再去问一次
						}
					}
                    
					if(n==1 || n==2)
					{
						prompt_for_new_del();
					}
				}
				return -1;
			}
			case KEY_EDIT_FILE:
			{
                uint8_t temp = 0;
                void (*n_file_fun[5])(TEST_FILE * pf, TEST_FILE * pgf);
                
				if(sys_par.keyboard_lock && gunlock==0)	/* 如果键盘锁打开就进行身份认证 */
				{
					if(0 != authenticate_pw(0))
					{
						return -1;	/* 密码错误返回 */
					}
					else
					{
						gunlock = 1;	/* 密码正确后将全局标志置一 */
					}
				}
                
                init_new_and_edit_path(n_file_fun, &temp);
                
                step = 0;
                
				while(1)
				{
                    if(n_file_fun[step] != NULL)
                    {
                        /* pf是指向临时文件结构的指针，pgf是指向当前全局文件的指针 */
                        n_file_fun[step](pf, pgf);
                    }
                    
					/* 在操作过程中按下了返回键返回 */
					if(g_return)
					{
                        g_return = 0;
                        
                        if(--step < 0)
                        {
                            return -1;	/* give up */
                        }
					}
					/* 在操作中按下确认键返回 */
					else
					{
						if(++step >= temp)
                        {
                            edit_and_updata_old_file(pf, pgf);
                            return 1;
                        }
					}
					
					if(g_exit)
					{
						return -1;	/* give up */
					}
				}
// 				break;
			}
			case KEY_BACK:
				g_return = 1;
				return 0;
			case KEY_EXIT:
				g_exit = 1;
				return -1;
			default:
				break;
		}
	}
}

/* w标志调用函数的路径 0从系统菜单进入 1从测试程序进入 */
static void server_mem_shortcut_exist(TEST_FILE * pf, TEST_FILE * pgf, const int8_t w)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t n = 1;
    
l:
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 31-1);
	GUI_ClearRect (200+1, 31+1, 240-1, 47-1);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	if(w)
	{/* 从测试程序进入时就走这里 */
		if(sys_par.language == CHINESE)
		{
			GUI_DispStringAt("读取", 208, 2);
		}
		else
		{
			GUI_SetFont(&GUI_FontHZ_SimSun_13);
			GUI_DispStringAt("Read", 208, 2);
			GUI_SetFont(&GUI_FontHZ_SimSun_12);
		}
	}
	else
	{
		if(sys_par.language == CHINESE)
		{
			GUI_DispStringAt("存贮", 208, 2);
			GUI_DispStringAt("读取", 208, 18);
			GUI_DispStringAt("更多", 208, 34);
		}
		else
		{
			GUI_SetFont(&GUI_FontHZ_SimSun_13);
			GUI_DispStringAt("Save", 208, 2);
			GUI_DispStringAt("Read", 208, 18);
			GUI_DispStringAt("More", 208, 34);
			GUI_SetFont(&GUI_FontHZ_SimSun_12);
		}
	}
	
	LCD_REFRESH();
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		switch(*pkey_value)
		{
			case KEY_SAVE:
			{
				/* 从测试界面进来的 只允许读文件 */
				if(w)
				{
					goto ll;
				}
				n = is_cover_or_read_or_del(0);//询问是否要覆盖
				if(n)
				{
					server_save(pf);
					*pgf = *pf;//更新内存
					copy_cur_file_to_new_pos(pf->num);
				}
				return;
			}
			case KEY_READ:
			{
				if(w)
				{
					break;
				}
				
		ll:		n = is_cover_or_read_or_del(1);//询问是否读取
				if(n)
				{
					g_cur_file = pgf;	/* 将当前文件指针指向读取的文件 */
					sys_flag.last_group = g_cur_file->num;/* 更新最后使用的文件标志 */
					
                    read_step_used_flag(g_cur_file->num);
                    read_group_table(g_cur_file->num);
                    
					/* 读出最近使用的记忆组 */
					if(read_list(g_cur_file->num))
					{
						sys_flag.mem_init = 0;
					}
                    
					save_sys_flag();
					
					g_cur_step = list_99xx.head;
					
					test_flag.into_test = 1;//返回到测试界面中去
					g_exit = 1;
				}
				return;
			}
			case KEY_MORE:
			{
				if(w)
				{
					break;
				}
                
				n = server_more_file(pf, pgf);
				if(-1 == n) /* 判断返回值如果是-1就说明是放弃操作返回，应该继续向上返回 */
				{
					return;
				}

				if(n == 0) /* 在子页中按下了返回键返回的 */
				{
					g_return = 0;
					goto l;	/* 子页中按下返回键后返回到父页后要重绘菜单 */
				}
				else	/* 子页中没有按下返回键返回时就继续向上返回 */
				{
					g_return = 1;
					return;
				}
			}
			case KEY_BACK:
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			default:
				break;
		}
        
		if(g_return || g_exit)
		{
			return;
		}
	}
}
/*
 * 函数名：roll_txt
 * 描述  ：文件名滚动显示
 * 输入  ：TEST_FILE * pf, 指向临时的文件结构数据
 *			char *ch 指向一个临时开辟的空间，用来存放文件名+三个空格，
 *			uint8_t *m	此参数是为了记忆在ch字符串中第几个字符大头，同 时 在调用函数中
 *						通过该变m=0来显示一个新的名称确保就从头开始而不是m中残留的上一个文件名的数据
 * 输出  ：char *dis 将最新的显示字符串带回给调用函数以作显示用
 * 返回  ：无
 */
static void roll_txt(const TEST_FILE * pf, char *ch, char *dis, uint8_t *m)
{
	uint8_t lon = 0;
	char *p1 = NULL;
	
	memset(dis, 0, 9);
	strcpy(ch, (const char*)pf->name);
	strcat(ch, "   "); /* 添加3个空格 */
	lon = strlen((const char*)ch);
	p1 = ch+*m;
	if(*m >= lon)
	{
		*m=0;
	}
	lon = strlen((const char*)p1);
	if(lon >= 8)
	{
		strncpy(dis, p1, 8);
	}
	else
	{
		strncpy(dis, p1, lon);
		strncat(dis, ch, 8-lon);
	}
	*m += 1;
}
static void draw_one_file_info(TEST_FILE *p, uint8_t n)
{
	char dis[9] = {0};
	int8_t x = 35;
	int8_t y = 2+12*(n-1);
	
	GUI_DispDecAt (p->num, 15, 2+12*(n-1), 2);	/* 显示编码 */
	
	if(p->name[0] == 0)
	{
		return;
	}
	strncpy(dis, (const char*)p->name, 8);
	GUI_DispStringAt(dis, x, y);	/* 显示文件名 */
	
	if(p->work_mode == N_MODE)	/* 显示工作模式 */
	{
		GUI_DispCharAt('N', x+10*6, y);
	}
	else if(p->work_mode == G_MODE)
	{
		GUI_DispCharAt('G', x+10*6, y);
	}
	
	GUI_DispStringAt((const char*)transform_time_to_str_for_mem(p->create_time), x+10*6+15, y);/* 显示创建时间 */
}
static void draw_one_page_files(TEST_FILE *p)
{
	int8_t i = 0;//给for使用
	
	GUI_ClearRect (15, 0, 200-1, 64-1);
	for(i = 0; i < 5; i++)
	{
		draw_one_file_info(p+i, i+1);
	}
	LCD_REFRESH();
}
/* w标志调用函数的路径 0从系统菜单进入 1从测试程序进入 */
void sys_memory(uint32_t w)
{
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	
	TEST_FILE file = default_file;
	char temp_ch[18]={0};//为文件名滚动显示提供临时存储
	char dis[9] = {0};//存放显示文件名的临时空间
	uint8_t m = 1;//控制文件名滚动从第几个字符开始(0-lon)
	uint8_t n = 1;//页内第n行的文件
	uint8_t p = 1;//页编号
	uint8_t lon = 0;//存储字符串长度
	
	memset(file.name, 0, 15);
	gui_draw_file_management();
	draw_one_page_files(&file_table[(p-1)*5+1]);
    
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,500,&err);
		
		lon = strlen((const char*)file_table[(p-1)*5+n].name);
        
		if(lon > 8)
		{
			roll_txt(&file_table[(p-1)*5+n], temp_ch, dis, &m);
			GUI_DispStringAt((const char*)dis, 35, 2+12*(n-1));
			LCD_REFRESH();
		}
        
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
        
		switch(*pkey_value)
		{
			case KEY_UP:
			{
				m = 1;//当文件名过长时滚动从第1个字符开始
				if(--n<=0)
				{
					n=5;
					if(--p < 1)
					{
						p = MAX_FILES/5;
					}
					GUI_ClearRect (35, 0, 200-1, 64-1);
				}
				
				draw_one_page_files(&file_table[(p-1)*5+1]);
				break;
			}
			case KEY_DOWN:
			{
				m = 1;//当文件名过长时滚动从第1个字符开始
				if(++n>5)
				{
					n=1;
					if(++p > MAX_FILES/5)
					{
						p = 1;
					}
					GUI_ClearRect (35, 0, 200-1, 64-1);
				}
				draw_one_page_files(&file_table[(p-1)*5+1]);
				break;
			}
			case KEY_SHORTCUT:
			{
				strncpy(dis, (const char*)file_table[(p-1)*5+n].name, 8);	/* 为了保证超过8个字符的文件名能显示前8个字符 */
				GUI_DispStringAt((const char*)dis, 35, 2+12*(n-1));
				m = 1;	/* 配合上面从头开始显示文件名,当然了只对长于8个字符的文件名有作用 */
				
				GUI_ClearRect (201, 32, 239, 46);
				
				/* 文件 不存在 */
				if(file_table[(p-1)*5+n].name[0] == '\0')
				{
					server_mem_shortcut_nonexist(&file, &file_table[(p-1)*5+n]);
					if(test_flag.into_set)
					{
						return;
					}
				}
				/* 文件存在 */
				else
				{
					file = file_table[(p-1)*5+n];
					server_mem_shortcut_exist(&file, &file_table[(p-1)*5+n], w);
					if(test_flag.into_test)
					{
						return;
					}
				}
				
				draw_short_cut_bm(UDSB_RE);
				
				draw_one_page_files(&file_table[(p-1)*5+1]);
				break;
			}
			case KEY_BACK:
				g_return = 1;
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
			g_return = 0;
		}
		GUI_ClearRect (0, 0, 12, 60);
		GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 4+12*(n-1));
		
		LCD24064_Refresh_Rect(0, 0, 12, 60);
	}
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
