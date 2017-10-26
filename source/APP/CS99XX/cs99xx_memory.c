/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�synchro_memory.c
 * ժ  Ҫ  �����������ݴ洢�ڹ���
 * ��ǰ�汾��V1.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
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
	#define DELAY_TIME_KEY 70 //��ʱʱ��
	static uint32_t g_delay_time_key;
	static uint8_t n[10] = {0};
	static uint8_t flag = 0;
	uint8_t s = 0;
	uint8_t i = 0;
	uint8_t temp_lon = strlen((const char*)pmenu_data->pData);
	
	if(g_delay_time_key)
	{
		g_delay_time_key--;//�������ṩ��ʱ��
	}
	
	for( i = 0; i < 10; i++)
	{
		if(n[i] != 0) s = 1;//����Ƿ��а�������s=1�м�����
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
	GUI_SetTextMode(GUI_TEXTMODE_REV);//���÷�ת�ı�
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("�������ļ���", X0+30, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Input the file name", X0+6, Y0+2);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//���������ı�
	
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
			pmenu_data->ch_bit = 0;//�˴��ǽ���Ǹ�����ʾ������
		}
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		serve_charkey(*pkey_value);
		temp = pmenu_data->ch_bit;
		temp_lon = strlen((const char*)file_name);
		
		switch(*pkey_value)
		{
			//������������������,���Ƚ�pmenu_data->ch_bit = 0;Ȼ����ȷ�������ƶ�����λ
			case KEY_SHIFT_L:
				pmenu_data->last_ch_bit = pmenu_data->ch_bit;//����last_ch_bit
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
				
				if(pmenu_data->ch_bit >= temp_lon-1)	/* ���ɾ���������һλ�ͽ��������һ�� */
				{
					shift_cursor_l(pmenu_data);
				}
				else	/* ����ɾ����λ���겻�ƶ� */
				{
					cursor_pos(pmenu_data->x+6*pmenu_data->ch_bit, y);
				}
				break;
			case KEY_ENTER:
				if(file_name[0] == '\0')	/* �ж��ļ����Ƿ�Ϊ�� */
				{
					break;
				}
				
				strcpy((char*)pf->name, (const char*)file_name);
				pf->create_time = Time_GetTime();/* ���浱ǰRTCʱ�� */
				GetTime();	/* ������gUpdataTime������ */
				(gUpdateTime+2)[14] = 0;	/* ��ȡ���� */
				strcpy((char*)transform_time_to_str_for_mem(pf->create_time), (const char*)(gUpdateTime+2));
				GUI_ClearRect (X0, Y0, X1, Y1);
				return;
			case KEY_BACK:
				GUI_ClearRect (X0, Y0, X1, Y1);
				g_return = 1;
				memset(pf->name, 0, 15);//�˳�ǰȡ����������ļ���
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
    
    /* ֻ���½��ļ��ǲ�ִ�� */
    if(pgf->name[0] == '\0')
    {
        /* ���ļ���������Чʱ���ͽ��ļ������������ָ�ΪĬ��ֵ */
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
		GUI_DispStringAt("����ģʽ��ʹ��", 0+X0+35, Y0+5);
		GUI_DispStringAt("��������Ϣ��ʧ��", 0+X0+35, Y0+5+15);
		GUI_DispStringAt("����", 0+X0+25+10, Y0+5+15+17);
		GUI_DispStringAt("ȡ��", 0+X0+25+10+50, Y0+5+15+17);
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
			case 1:/* ���� */
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, X0+25, Y0+5+15+17+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,  X0+25+50, Y0+5+15+17+3);
				cursor_pos(X0+25+1, Y0+5+15+17);
				break;
			case 0:/* ȡ�� */
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
	GUI_SetTextMode(GUI_TEXTMODE_REV);//���÷�ת�ı�
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("���ù���ģʽ", X0+30, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Set mode", X0+40, Y0+3);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//���������ı�
    draw_short_cut_bm(__RB);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("����ģʽ", X0+15, Y0+30);
		GUI_DispStringAt("�ݶ�ģʽ", X0+15+60, Y0+30);
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
			case N_MODE:/* Nģʽ */
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y0+30+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+60, Y0+30+3);
				cursor_pos(X0+5+1, Y0+30);
				break;
			case G_MODE:/* Gģʽ */
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
				/* ��ǰ�����Ƿ�֧��Gģʽ */
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
					/* ѯ���Ƿ�Ҫ����ģʽ */
					if(0 == prompt_for_ch_work_mode())
					{
						goto l;/* ���� */
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
	uint16_t t = 0;//���Ȩ��
	uint16_t temp_pass_time = pf->hold_pass_time;
	
    uint8_t x = ((X0+(X0+X1)/2)/2-15);
    uint8_t y = Y1-25;
	
	DATA_INFO pass_time = {
		(void*)0,//uint32_t* pData;//����
		0,	//���ݵ�x����
		0,	//���ݵ�y����
		1,	//0��ʾint,1��ʾfloat,2��ʾ�ַ���char[]
		5,	//��ʾ���ݳ���
		1,	//С�����λ
		0,	//��ǵ�ǰҳ��ʾ����������
		0,  //��ǵ�ǰ�������ǵڼ�������
		0,	//��ǰ�޸ĵ�λ
		0,	//��ǰҳ��
		1,	//������ǰֵ��8λ��16λ��32λ��0����8λ��1����16λ��2����32λ,3��ʾ�ַ���
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
	GUI_SetTextMode(GUI_TEXTMODE_REV);//���÷�ת�ı�
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("����PASS�źű���", X0+20, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Set pass keep", X0+30, Y0+2);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//���������ı�
	
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
				cursor_pos(x+pmenu_data->ch_bit*6,y);//��ʾ���
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
				cursor_pos(x+pmenu_data->ch_bit*6,y);//��ʾ���
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
	uint16_t t = 0;//���Ȩ��
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
	GUI_SetTextMode(GUI_TEXTMODE_REV);//���÷�ת�ı�
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("�༭PASS��������", X0+20, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Set pass buzzer keep", X0+6, Y0+2);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//���������ı�
	
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
				cursor_pos(x+pmenu_data->ch_bit*6,y);//��ʾ���
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
				cursor_pos(x+pmenu_data->ch_bit*6,y);//��ʾ���
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
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};/* ��ģʽ������ */
	uint8_t flag[MODEL_KINDS_MAX] = {0};/* �������ģʽ������ */
	int16_t kinds = 0;/* ��ģʽ����Ŀ */
	
	define_modes(mode_buf, flag, &kinds);
	
	GUI_DrawLine         (X0, Y0, X1, Y0);
	GUI_DrawLine         (X0, Y1, X1, Y1);
	GUI_DrawLine         (X0, Y0, X0, Y1);
	GUI_DrawLine         (X1, Y0, X1, Y1);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine         (X0, Y0+17, X1, Y0+17);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//���÷�ת�ı�
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("�༭�绡���ģʽ", X0+20, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Set arc mode", X0+28, Y0+3);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//���������ı�
	
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);

	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("����ģʽ", X0+15, Y0+30);
		GUI_DispStringAt("�ȼ�ģʽ", X0+15+60, Y0+30);
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
	int8_t num_bak = pgf->num;/* �����ļ���� */
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};/* ��ģʽ������ */
	uint8_t flag[MODEL_KINDS_MAX] = {0};/* �������ģʽ������ */
	int16_t kinds = 0;/* ��ģʽ����Ŀ */
	
	define_modes(mode_buf, flag, &kinds);
	/* �����ʱ�ļ��е��ļ���Ϊ��˵�������������� */
	if(pf->name[0] == '\0')
	{
		return;
	}
    
	/* �½��ļ�ʱ�������� �ڴ��е��ļ���Ϊ�ն���ʱ�ļ��е��ļ������վ�˵�����½����ļ��� */
	if(pgf->name[0] == '\0' && pf->name[0] != '\0')
	{
		*pgf = *pf;//���µ��ڴ�
		pgf->num = num_bak;/* �ָ��ļ���� */
		
		g_cur_file = pgf;//�����ļ�ȫ��ָ��
        g_cur_file->total = 0;/* ��������һ�η��������� */
        
        clear_step_used_flag();
        clear_group_table();
        
        insert_step(0, flag[1]);
        
        save_group_info(g_cur_file->num);
        
		sys_flag.last_group = g_cur_file->num;/* �������ʹ�õ��ļ���־ */
		save_sys_flag();
		
        load_steps_to_list(1);
        g_cur_step = list_99xx.head;
		return;
	}
}

static void edit_and_updata_old_file(TEST_FILE * pf, TEST_FILE * pgf)
{
	int8_t num_bak = pgf->num;/* �����ļ���� */
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};/* ��ģʽ������ */
	uint8_t flag[MODEL_KINDS_MAX] = {0};/* �������ģʽ������ */
	int16_t kinds = 0;/* ��ģʽ����Ŀ */
	
	define_modes(mode_buf, flag, &kinds);
	
	/* �༭�ļ�����ģʽ�ı��� */
	if(pf->work_mode != pgf->work_mode)/* �༭�ļ������� */
	{
        TEST_FILE *p_file = g_cur_file;
		/* ����ı� �����ļ��Ĺ���ģʽ��ô���ļ���ԭ�еĲ��Բ������Ҫɾ����*/
		
        *pgf = *pf;//���µ��ڴ�
        pgf->num = num_bak;/* �ָ��ļ���� */
        
        g_cur_file = pgf;
        
        clear_step_used_flag();
        clear_group_table();
        g_cur_file->total = 0;
        
        insert_step(0, flag[1]);
        
        save_group_info(g_cur_file->num);
        
		/* �༭���ļ��ǵ�ǰ�ļ�ָ����ָ���ļ���Ҫ������Ÿ��µ� */
		if(p_file == pgf)
		{
            load_steps_to_list(1);
			g_cur_step = list_99xx.head;	/* ���ص�ǰ�� */
		}
        
        g_cur_file = p_file;
	}
	/* �༭�Ĺ���ģʽδ�ı� */
	else if(pf->work_mode == pgf->work_mode)/* �༭�ļ������� */
	{
        *pgf = *pf;//���µ��ڴ�
        pgf->num = num_bak;/* �ָ��ļ���� */
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
		GUI_DispStringAt("����", 208, 2);
		GUI_DispStringAt("�½�", 208, 18);
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
				//���µ�file_table
				if(pf->name[0] != '\0')
				{
					n = pgf->num;/* �����ļ���� */
                    
					*pgf = *pf;
					pgf->num = n;/* �ָ��ļ���� */
					*pf = default_file;/* �ٽ���ʱ�ļ��ָ�ΪĬ��ֵ */
					memset(pf->name, 0, 15);/* ����ļ��� */
					
					/* ����ǰ�ļ��������е����ݸ��Ƶ������ļ��������� */
					pgf->total = g_cur_file->total;
					copy_cur_file_to_new_pos(pgf->num);
				}
				return;
			}
			case KEY_NEW://�½�һ���ļ�
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
								test_flag.into_set = 1;//��ʾ�½�������������Ͻ������ý���
								g_exit = 1;//��������־Ҫͬʱ��1,��˼�Ƿ��ص�������Ȼ���ٽ������ý���
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
	GUI_SetTextMode(GUI_TEXTMODE_REV);//���÷�ת�ı�
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		switch(w)
		{
			case 0:
				GUI_DispStringAt("�Ƿ񸲸�", X0+12, Y0+3);
				break;
			case 1:
				GUI_DispStringAt("�Ƿ��ȡ", X0+12, Y0+3);
				break;
			case 2:
				GUI_DispStringAt("�Ƿ�ɾ��", X0+12, Y0+3);
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
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//���������ı�
	
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);
	GUI_ClearRect (200+1, 15+16+1, 240-1, 15+16+16-1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y1-14+3);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+35, Y1-14+3);
	cursor_pos(X0+5+1, Y1-14);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("��", X0+15, Y1-14);
		GUI_DispStringAt("��", X0+15+35, Y1-14);
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
	GUI_SetTextMode(GUI_TEXTMODE_REV);//���÷�ת�ı�
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("����ɾ��ģʽ", X0+30, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Set delete mode", X0+20, Y0+3);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//���������ı�
	
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);
	GUI_ClearRect (200+1, 15+16+1, 240-1, 15+16+16-1);

	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y0+30+3);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+64, Y0+30+3);
	cursor_pos(X0+5+1, Y0+30);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("ɾ������", X0+15, Y0+30);
		GUI_DispStringAt("ɾ��ȫ��", X0+15+64, Y0+30);
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
				return 0;	/* ����0��ʾ���� */
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
		GUI_DispStringAt("���ݲ����У���", 0+X0+35, Y0+5);
		GUI_DispStringAt("����Ч����Ҫ��", 0+X0+35, Y0+5+15);
		GUI_DispStringAt("�������Ժ�...", 0+X0+35, Y0+5+15+15);
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
		GUI_DispStringAt("ɾ��ȫ����ʹ��", 0+X0+35, Y0+5);
		GUI_DispStringAt("��������Ϣ��ʧ��", 0+X0+35, Y0+5+15);
		GUI_DispStringAt("����", 0+X0+35+10, Y0+5+15+17);
		GUI_DispStringAt("ȡ��", 0+X0+35+10+50, Y0+5+15+17);
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
 * ��������server_more_file
 * ����  ����������Ա�ɹ����¼���ʱ��ʱ����OK��ʾ�� 
 * ����  ��TEST_FILE * pf, ָ����ʱ���ļ��ṹ����
 *			TEST_FILE * pgf ָ��ȫ�ֵ��ļ��ṹ����
 * ���  ��TEST_FILE * pgf ������ENTER��ʱ����ʱ���ݱ��浽ȫ��������
 * ����  ��int8_t	��-1��ʾ������������ѯ���Ƿ�ɾ��ʱѡ���˷�����˷��ؼ�
 *					  0��ʾ�ڵ�ǰ����δ���κβ�����ֱ�Ӱ��·��ؼ���exit��
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
		GUI_DispStringAt("ɾ��", 208, 2);
		GUI_DispStringAt("�༭", 208, 18);
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
				if(sys_par.keyboard_lock && gunlock==0)	/* ����������򿪾ͽ��������֤ */
				{
					if(0 != authenticate_pw(0))
					{
						return -1;	/* ������󷵻� */
					}
					else
					{
						gunlock = 1;
					}
				}
                
				n = is_cover_or_read_or_del(2);
                
				if(n)	/* nΪ����ʱ��ʾ��������������� */
				{
			l:
					n = set_del_mode();
					 /* �������� */
					if(n == 0)
					{
						return -1; /* give up */
					}
					//ɾ������
					else if(n == 1)
					{
                        del_file(pgf->num);
						
						/* �������µļ����� */
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
						//ɾ��ȫ��
						if(n)
						{
							del_all_file();
                            
							/* �������µļ����� */
							if(read_list(g_cur_file->num))
							{
								sys_flag.mem_init = 0;
								save_sys_flag();
							}
							
							g_cur_step = list_99xx.head;
						}
						else
						{
							goto l;//�������0����ȥ��һ��
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
                
				if(sys_par.keyboard_lock && gunlock==0)	/* ����������򿪾ͽ��������֤ */
				{
					if(0 != authenticate_pw(0))
					{
						return -1;	/* ������󷵻� */
					}
					else
					{
						gunlock = 1;	/* ������ȷ��ȫ�ֱ�־��һ */
					}
				}
                
                init_new_and_edit_path(n_file_fun, &temp);
                
                step = 0;
                
				while(1)
				{
                    if(n_file_fun[step] != NULL)
                    {
                        /* pf��ָ����ʱ�ļ��ṹ��ָ�룬pgf��ָ��ǰȫ���ļ���ָ�� */
                        n_file_fun[step](pf, pgf);
                    }
                    
					/* �ڲ��������а����˷��ؼ����� */
					if(g_return)
					{
                        g_return = 0;
                        
                        if(--step < 0)
                        {
                            return -1;	/* give up */
                        }
					}
					/* �ڲ����а���ȷ�ϼ����� */
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

/* w��־���ú�����·�� 0��ϵͳ�˵����� 1�Ӳ��Գ������ */
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
	{/* �Ӳ��Գ������ʱ�������� */
		if(sys_par.language == CHINESE)
		{
			GUI_DispStringAt("��ȡ", 208, 2);
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
			GUI_DispStringAt("����", 208, 2);
			GUI_DispStringAt("��ȡ", 208, 18);
			GUI_DispStringAt("����", 208, 34);
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
				/* �Ӳ��Խ�������� ֻ������ļ� */
				if(w)
				{
					goto ll;
				}
				n = is_cover_or_read_or_del(0);//ѯ���Ƿ�Ҫ����
				if(n)
				{
					server_save(pf);
					*pgf = *pf;//�����ڴ�
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
				
		ll:		n = is_cover_or_read_or_del(1);//ѯ���Ƿ��ȡ
				if(n)
				{
					g_cur_file = pgf;	/* ����ǰ�ļ�ָ��ָ���ȡ���ļ� */
					sys_flag.last_group = g_cur_file->num;/* �������ʹ�õ��ļ���־ */
					
                    read_step_used_flag(g_cur_file->num);
                    read_group_table(g_cur_file->num);
                    
					/* �������ʹ�õļ����� */
					if(read_list(g_cur_file->num))
					{
						sys_flag.mem_init = 0;
					}
                    
					save_sys_flag();
					
					g_cur_step = list_99xx.head;
					
					test_flag.into_test = 1;//���ص����Խ�����ȥ
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
				if(-1 == n) /* �жϷ���ֵ�����-1��˵���Ƿ����������أ�Ӧ�ü������Ϸ��� */
				{
					return;
				}

				if(n == 0) /* ����ҳ�а����˷��ؼ����ص� */
				{
					g_return = 0;
					goto l;	/* ��ҳ�а��·��ؼ��󷵻ص���ҳ��Ҫ�ػ�˵� */
				}
				else	/* ��ҳ��û�а��·��ؼ�����ʱ�ͼ������Ϸ��� */
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
 * ��������roll_txt
 * ����  ���ļ���������ʾ
 * ����  ��TEST_FILE * pf, ָ����ʱ���ļ��ṹ����
 *			char *ch ָ��һ����ʱ���ٵĿռ䣬��������ļ���+�����ո�
 *			uint8_t *m	�˲�����Ϊ�˼�����ch�ַ����еڼ����ַ���ͷ��ͬ ʱ �ڵ��ú�����
 *						ͨ���ñ�m=0����ʾһ���µ�����ȷ���ʹ�ͷ��ʼ������m�в�������һ���ļ���������
 * ���  ��char *dis �����µ���ʾ�ַ������ظ����ú���������ʾ��
 * ����  ����
 */
static void roll_txt(const TEST_FILE * pf, char *ch, char *dis, uint8_t *m)
{
	uint8_t lon = 0;
	char *p1 = NULL;
	
	memset(dis, 0, 9);
	strcpy(ch, (const char*)pf->name);
	strcat(ch, "   "); /* ���3���ո� */
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
	
	GUI_DispDecAt (p->num, 15, 2+12*(n-1), 2);	/* ��ʾ���� */
	
	if(p->name[0] == 0)
	{
		return;
	}
	strncpy(dis, (const char*)p->name, 8);
	GUI_DispStringAt(dis, x, y);	/* ��ʾ�ļ��� */
	
	if(p->work_mode == N_MODE)	/* ��ʾ����ģʽ */
	{
		GUI_DispCharAt('N', x+10*6, y);
	}
	else if(p->work_mode == G_MODE)
	{
		GUI_DispCharAt('G', x+10*6, y);
	}
	
	GUI_DispStringAt((const char*)transform_time_to_str_for_mem(p->create_time), x+10*6+15, y);/* ��ʾ����ʱ�� */
}
static void draw_one_page_files(TEST_FILE *p)
{
	int8_t i = 0;//��forʹ��
	
	GUI_ClearRect (15, 0, 200-1, 64-1);
	for(i = 0; i < 5; i++)
	{
		draw_one_file_info(p+i, i+1);
	}
	LCD_REFRESH();
}
/* w��־���ú�����·�� 0��ϵͳ�˵����� 1�Ӳ��Գ������ */
void sys_memory(uint32_t w)
{
	uint16_t *pkey_value = NULL;
	INT8U err = 0;
	
	TEST_FILE file = default_file;
	char temp_ch[18]={0};//Ϊ�ļ���������ʾ�ṩ��ʱ�洢
	char dis[9] = {0};//�����ʾ�ļ�������ʱ�ռ�
	uint8_t m = 1;//�����ļ��������ӵڼ����ַ���ʼ(0-lon)
	uint8_t n = 1;//ҳ�ڵ�n�е��ļ�
	uint8_t p = 1;//ҳ���
	uint8_t lon = 0;//�洢�ַ�������
	
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
				m = 1;//���ļ�������ʱ�����ӵ�1���ַ���ʼ
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
				m = 1;//���ļ�������ʱ�����ӵ�1���ַ���ʼ
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
				strncpy(dis, (const char*)file_table[(p-1)*5+n].name, 8);	/* Ϊ�˱�֤����8���ַ����ļ�������ʾǰ8���ַ� */
				GUI_DispStringAt((const char*)dis, 35, 2+12*(n-1));
				m = 1;	/* ��������ͷ��ʼ��ʾ�ļ���,��Ȼ��ֻ�Գ���8���ַ����ļ��������� */
				
				GUI_ClearRect (201, 32, 239, 46);
				
				/* �ļ� ������ */
				if(file_table[(p-1)*5+n].name[0] == '\0')
				{
					server_mem_shortcut_nonexist(&file, &file_table[(p-1)*5+n]);
					if(test_flag.into_set)
					{
						return;
					}
				}
				/* �ļ����� */
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

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
