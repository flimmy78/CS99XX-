/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_result.c
 * ժ  Ҫ  ��ϵͳ�������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */



#include	<string.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"

/*
 * ��������authenticate_pw
 * ����  ���������봰�� ��server_more_file��menu_subtask,main_task����
 * ����  ��int8_t n	���ֲ�ͬģʽ�µ����벻һ������У׼ģʽʱ������ϵͳ���벻ͬ
 *			У׼�������룺 calibration//CALIBRATE_PW
 * ���  ����
 * ����  ��int8_t	�����ɹ�����0��ʧ�ܻ��������-1
 */
int8_t authenticate_pw(const int8_t n)
{
	#define X0	60
	#define X1	130
	#define Y0	15
	#define Y1	49
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	char temp_pw[9] = {0};
	int8_t m = 0;
	int8_t i = 0;
	/* ����������������Ļ�ϵ��Ի�����Ϣ */
	DATA_INFO pw_info = 
	{
		(void*)0/*pData*/,X0+8/*x*/,Y1-14/*y*/,2/*type*/,8/*lon*/,0/*decs*/,0/*total*/,1/*cur*/,0/*ch_bit*/,0/*page*/,3/*bits*/,
	};
    
	GUI_DrawRect(X0, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//���÷�ת�ı�
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt("����", (X0+X1)/2-6*2, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt("Password", X0+10, Y0+3);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//���������ı�
	
	draw_short_cut_bm(LRDB);
	
	pmenu_data = &pw_info;	/* ��pmenu_dataָ��ԭʼ�������Ϣ */
	pmenu_data->pData = temp_pw;
	pmenu_data->ch_bit = 0;
	cursor_pos(pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		uint8_t temp = 0;	/* ��ŵ�ǰ�޸ĵ�λ */
		uint8_t temp_lon = 0;	/* ��ŵ�ǰ�ַ��������ĳ��� */
		uint8_t x = pmenu_data->x;	/* ��ŵ�ǰ���ݵ����� */
		uint8_t y = pmenu_data->y;
		
		pkey_value = OSQPend(KeyboardQSem,700,&err);
		temp = pmenu_data->ch_bit;
		temp_lon = strlen((const char*)pmenu_data->pData);
		
		/* ��ǰ�ַ���Ϊ��ʱһ��Ҫ��m��ch_bit���� */
		if(temp_lon == 0)
		{
			m = 0;
			pmenu_data->ch_bit = 0;
			cursor_pos(x,y);
		}
		
		serve_pwkey(*pkey_value, &m);	/* ����������̷������ */
		
		switch(*pkey_value)
		{
			case KEY_SHIFT_L:	/* ѭ������ */
				pmenu_data->last_ch_bit = pmenu_data->ch_bit;	/* ����last_ch_bit ֻ�д������һ���ַ�ǰ�޷���������� */
				shift_cursor_l(pmenu_data);
				break;
			case KEY_SHIFT_R:	/* ѭ������ */
				shift_cursor(pmenu_data);
				break;
			case KEY_DEL_PW:	/* ɾ����ǰλ�� */
				for(i = 0; i < temp_lon - temp;i++)
				{
					pmenu_data->pData = (uint8_t *)pmenu_data->pData;
					*((uint8_t*)pmenu_data->pData+temp+i) = *((uint8_t*)pmenu_data->pData+temp+i+1);					
				}
				
				m = strlen((const char*)pmenu_data->pData);
				GUI_ClearRect (pmenu_data->x, pmenu_data->y, pmenu_data->x+6*8, pmenu_data->y+12);
				
				/* ������Ļ��ʾ */
				if (plain_code)	/* ������ʾ */
				{
					GUI_DispStringAt((const char*)pmenu_data->pData, pmenu_data->x, pmenu_data->y);
				}
				else	/* ��������ʾ */
				{
					GUI_GotoXY(x, y);
					for(i = 0; i < m; i++)
					{
						GUI_DispChar('*');
					}
				}
				
				/* ���¹��λ�� */
				if(pmenu_data->ch_bit >= temp_lon -1)
				{
					shift_cursor_l(pmenu_data);
				}
				else
				{
					cursor_pos(x+pmenu_data->ch_bit*6, y);
				}
				break;
			case KEY_ENTER:
			{
				int8_t sign = 0;
				if(n == 0)
				{
					sign = strcmp((const char*)pmenu_data->pData, (const char*)sys_par.pass_word);
				}
				else
				{
					sign = strcmp((const char*)pmenu_data->pData, CALIBRATE_PW);
				}
				if(sign != 0)
				{
					memset((char*)pmenu_data->pData, 0, 9);
					pmenu_data->ch_bit = 0;
					GUI_ClearRect (pmenu_data->x, pmenu_data->y, pmenu_data->x+6*8, pmenu_data->y+12);
					cursor_pos(pmenu_data->x, pmenu_data->y);
					break;	/* ������� */
				}
				else
				{
					return 0;
				}
			}
			case KEY_BACK:
				g_return = 1;
				GUI_ClearRect (X0, Y0, X1, Y1);
				LCD_REFRESH();
				return -1;
			case KEY_EXIT:
				GUI_ClearRect (X0, Y0, X1, Y1);
				LCD_REFRESH();
				g_exit = 1;
				return -1;
			case KEY_ENTER & KEY_0:
				plain_code = !plain_code;
				/* ������Ļ��ʾ */
				if (plain_code)	/* ������ʾ */
				{
					GUI_DispStringAt((const char*)pmenu_data->pData, pmenu_data->x,  pmenu_data->y);
				}
				else	/* ��������ʾ */
				{
					m = strlen((const char*)pmenu_data->pData);
					GUI_GotoXY(pmenu_data->x, pmenu_data->y);
					for(i = 0; i < m; i++)
					{
						GUI_DispChar('*');
					}
				}
				LCD_REFRESH();
				break;
		}
	}
	
	#undef X0
	#undef X1
	#undef Y0
	#undef Y1
}

/*
 * ��������serve_pwkey
 * ����  ����������������ģ�� ��һ���������� ��sys_passward����
 * ����  ��uint16_t keyvalue ��ֵ
 *		   uint8_t *m m�Ǽ�¼��ǰ�Ѿ����������λ��
 * ���  ����
 */
static __inline void serve_pwkey(const uint16_t keyvalue, int8_t *m)
{
	uint8_t flg = 0;
	switch(keyvalue)
	{
		case KEY_0:
			change_data('0');
			break;
		case KEY_1:
			change_data('1');
			break;
		case KEY_2:
			change_data('2');
			break;
		case KEY_3:
			change_data('3');
			break;
		case KEY_4:
			change_data('4');
			break;
		case KEY_5:
			change_data('5');
			break;
		case KEY_6:
			change_data('6');
			break;
		case KEY_7:
			change_data('7');
			break;
		case KEY_8:
			change_data('8');
			break;
		case KEY_9:
			change_data('9');
			break;
		default:
			flg = 1;
			break;
	}
	
	if(flg == 0 && plain_code)
	{
// 		*m += 1;
		*m = strlen((const char*)pmenu_data->pData);
		update_data(pmenu_data);
		if(pmenu_data->ch_bit < 8 && *m <= 8)
		{
			if(*m < 8)
			{
				shift_cursor(pmenu_data);
			}
			else if(*m == 8)
			{
				cursor_pos(pmenu_data->x+pmenu_data->ch_bit*6, pmenu_data->y);
			}
		}
	}

	if(flg == 0 && !plain_code)
	{
// 		*m += 1;
		*m = strlen((const char*)pmenu_data->pData);
		GUI_DispCharAt('*', pmenu_data->x + pmenu_data->ch_bit * 6, pmenu_data->y);
		LCD_REFRESH();
		if(pmenu_data->ch_bit < 8 && *m <= 8)
		{
			if(*m < 8)
			{
				shift_cursor(pmenu_data);
			}
			else if(*m == 8)
			{
				cursor_pos(pmenu_data->x+pmenu_data->ch_bit*6, pmenu_data->y);
			}
		}
	}
}


/*
 * ��������prompt_for_pw
 * ����  ����������������и��û�����ʾ��Ϣ
 * ����  ��uint8_t s �����ڲ�ͬ״̬�¸��û���һЩ��ʾ��Ϣ
 * ���  ����
 */
static void prompt_for_pw(const uint8_t s)
{
	#define X0	40
	#define X1	165
	#define Y0	8
	#define Y1	58
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	if(sys_par.language == CHINESE)
	{
		switch(s)
		{
			case 0:
				GUI_DispStringAt("ԭʼ�������", 0+X0+35, Y0+8);
				GUI_DispStringAt("���������롣", 0+X0+35, Y0+8+18);
				break;
			case 1:
				GUI_DispStringAt("ȷ���������", 0+X0+35, Y0+8);
				GUI_DispStringAt("���������롣", 0+X0+35, Y0+8+18);
				break;
			case 2:
				GUI_DispStringAt("�������óɹ���", 0+X0+35, Y0+8);
				GUI_DispStringAt("���ص��˵����档", 0+X0+35, Y0+8+18);
				break;
		}
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		switch(s)
		{
			case 0:
				GUI_DispStringAt("Original Passw-", 0+X0+32, Y0+8);
				GUI_DispStringAt("ord error!", 0+X0+32, Y0+8+13);
				GUI_DispStringAt("Please input again.", 0+X0+10, Y0+8+13+13);
				break;
			case 1:
				GUI_DispStringAt("Confirm Passw-", 0+X0+32, Y0+8);
				GUI_DispStringAt("ord error!", 0+X0+32, Y0+8+13);
				GUI_DispStringAt("Please input again.", 0+X0+10, Y0+8+13+13);
				break;
			case 2:
				GUI_DispStringAt("Password set-", 0+X0+32, Y0+8);
				GUI_DispStringAt("ing succeed!", 0+X0+32, Y0+8+13);
				GUI_DispStringAt("Return to menu.", 0+X0+10, Y0+8+13+13);
				break;
		}
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	GUI_DrawLine         (X0, Y0, X1, Y0);
	GUI_DrawLine         (X0, Y1, X1, Y1);
	GUI_DrawLine         (X0, Y0, X0, Y1);
	GUI_DrawLine         (X1, Y0, X1, Y1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, X0+3, Y0+5);
	LCD_REFRESH();
	
	OSTimeDlyHMSM(0,0,2,0);	/* ��ʾ����ʾ2s */
	GUI_ClearRect (X0, Y0, X1, Y1);
	
	#undef X0
	#undef X1
	#undef Y0
	#undef Y1
}
/*
 * ��������sys_passward
 * ����  ���������� �� menu_subtask���� ����û����ʽ�ĵ��ö���ͨ������ָ��
 * ����  ����
 * ���  ����
 */
void sys_password(uint32_t w)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t n = 0;	/* ��ǵ�ǰ���õ��ǵڼ������� 0ԭʼ����� 1��������� 2ȷ������ */
	int8_t m = 0;//��ʾ��ǰ�����˼�λ����
	uint8_t i = 0;	/* Ϊfor���� */
	char old_pw[PW_LON+1] = {0};	/* �����ԭʼ������ʱ�ռ� */
	char new_pw[PW_LON+1] = {0};	/* ����������������ʱ�ռ� */
	char ok_pw[PW_LON+1]  = {0};	/* �����ȷ���������ʱ�ռ� */
	
	/* ����3��������������Ļ�ϵ��Ի�����Ϣ */
	DATA_INFO pw_info[3] = {
		{//ԭʼ����
			(void*)0/*pData*/,65/*x*/,2/*y*/,STRING_T/*type*/,8/*lon*/,0/*decs*/,0/*total*/,1/*cur*/,0/*ch_bit*/,0/*page*/,3/*bits*/,
		},
		{//��������
			(void*)0/*pData*/,65/*x*/,25/*y*/,STRING_T/*type*/,8/*lon*/,0/*decs*/,0/*total*/,2/*cur*/,0/*ch_bit*/,0/*page*/,3/*bits*/,
		},
		{//ȷ������
			(void*)0/*pData*/,65/*x*/,46/*y*/,STRING_T/*type*/,8/*lon*/,0/*decs*/,0/*total*/,3/*cur*/,0/*ch_bit*/,0/*page*/,3/*bits*/,
		},
	};
	
	Lcd24064_Clr();
	pw_info[0].pData = old_pw;
	pw_info[1].pData = new_pw;
	pw_info[2].pData = ok_pw;
l:	gui_draw_password_manage();	/* �������汳�� */
	
	memset(old_pw, 0, 9);
	pmenu_data = &pw_info[0];	/* ��pmenu_dataָ��ԭʼ�������Ϣ */
	pmenu_data->pData = (uint8_t *)pmenu_data->pData;
	pmenu_data->pData = old_pw;
	pmenu_data->ch_bit = 0;
    
	GUI_ClearRect (pw_info[0].x, pw_info[0].y, pw_info[0].x+6*9, pw_info[0].y+8);
	GUI_ClearRect (pw_info[1].x, pw_info[1].y, pw_info[1].x+6*9, pw_info[1].y+8);
	GUI_ClearRect (pw_info[2].x, pw_info[2].y, pw_info[2].x+6*9, pw_info[2].y+8);
    
	cursor_pos(pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		uint8_t temp = 0;	/* ��ŵ�ǰ�޸ĵ�λ */
		uint8_t temp_lon = 0;	/* ��ŵ�ǰ�ַ��������ĳ��� */
		uint8_t x = pmenu_data->x;	/* ��ŵ�ǰ���ݵ����� */
		uint8_t y = pmenu_data->y;
		
		pkey_value = OSQPend(KeyboardQSem,700,&err);
		temp = pmenu_data->ch_bit;
		temp_lon = strlen((const char*)pmenu_data->pData);
		
		/* ��ǰ�ַ���Ϊ��ʱһ��Ҫ��m��ch_bit���� */
		if(temp_lon == 0) 
		{
			m = 0;
			pmenu_data->ch_bit = 0;
			cursor_pos(x,y);
		}
		
		serve_pwkey(*pkey_value, &m);	/* ����������̷������ */
		
		switch(*pkey_value)
		{
			case KEY_SHIFT_L:	/* ѭ������ */
				pmenu_data->last_ch_bit = pmenu_data->ch_bit;	/* ����last_ch_bit ֻ�д������һ���ַ�ǰ�޷���������� */
				shift_cursor_l(pmenu_data);
				break;
			case KEY_SHIFT_R:	/* ѭ������ */
				shift_cursor(pmenu_data);
				break;
			case KEY_DEL_PW:	/* ɾ����ǰλ�� */
            {
				for(i = 0; i < temp_lon - temp;i++)
				{
					pmenu_data->pData = (uint8_t *)pmenu_data->pData;
					*((uint8_t*)pmenu_data->pData+temp+i) = *((uint8_t*)pmenu_data->pData+temp+i+1);					
				}
				
				m = strlen((const char*)pmenu_data->pData);
				GUI_ClearRect (pmenu_data->x, pmenu_data->y, pmenu_data->x+14*7, pmenu_data->y+12);
				
				/* ������Ļ��ʾ */
				if (plain_code)	/* ������ʾ */
				{
					GUI_DispStringAt((const char*)pmenu_data->pData, pmenu_data->x, pmenu_data->y);
				}
				else	/* ��������ʾ */
				{
					GUI_GotoXY(x, y);
					for(i = 0; i < m; i++)
					{
						GUI_DispChar('*');
					}
				}
				
				/* ���¹��λ�� */
				if(pmenu_data->ch_bit >= temp_lon -1)
				{
					shift_cursor_l(pmenu_data);
				}
				else
				{
					cursor_pos(x+pmenu_data->ch_bit*6, y);
				}
				break;
            }
			case KEY_ENTER:
            {
				if(n == 0)
				{
					int32_t sign = 0;
					sign = strcmp((const char*)old_pw, sys_par.pass_word);
					if(sign != 0)
					{
						prompt_for_pw(0);
						goto l;
					}
				}
				else if(n == 1)
				{
					if(strlen((const char*)new_pw) == 0)
					{
						break;
					}
				}
				else if(n == 2)
				{
					if(0 != strcmp((const char*)new_pw, (const char*)ok_pw))
					{
						prompt_for_pw(1);	/* ȷ��������� */
						memset((char*)pmenu_data->pData, 0, 9);	/* ��� ok_pw���� */
						pmenu_data->ch_bit = 0;
						GUI_ClearRect (pmenu_data->x, pmenu_data->y, pmenu_data->x+6*9, pmenu_data->y+8);
						
						pmenu_data = &pw_info[1];
						memset((char*)pmenu_data->pData, 0, 9);
						pmenu_data->ch_bit = 0;
						GUI_ClearRect (pmenu_data->x, pmenu_data->y, pmenu_data->x+6*9, pmenu_data->y+8);
						
						n = 1;
						gui_draw_password_manage();	/* �������汳�� */
						
						/* ������Ļ��ʾ */
						GUI_ClearRect (pw_info[0].x, pw_info[0].y, pw_info[0].x+6*9, pw_info[0].y+8);
						GUI_ClearRect (pw_info[1].x, pw_info[1].y, pw_info[1].x+6*9, pw_info[1].y+8);
						GUI_ClearRect (pw_info[2].x, pw_info[2].y, pw_info[2].x+6*9, pw_info[2].y+8);
						if (plain_code)	/* ������ʾ */
						{
							GUI_DispStringAt((const char*)pw_info[0].pData, pw_info[0].x, pw_info[0].y);
						}
						else	/* ��������ʾ */
						{
							m = strlen((const char*)old_pw);
							GUI_GotoXY(pw_info[0].x, pw_info[0].y);
                            
							for(i = 0; i < m; i++)
							{
								GUI_DispChar('*');
							}
						}
						cursor_pos(pmenu_data->x, pmenu_data->y);
						break;
					}
					else
					{
						strcpy(sys_par.pass_word, (const char*)ok_pw);	/* ������ڴ� */
                        save_sys_par();/* ���浽eep */
						prompt_for_pw(2);	/* �����޸ĳɹ� */
					}
					g_return = 1;
					return;
				}
                
				if(n < 2)
				{
					n++;
					pmenu_data = &pw_info[n];
					if(n == 1)//��������
					{
						pmenu_data->pData = (uint32_t *)pmenu_data->pData;
						pmenu_data->pData = &new_pw;
						cursor_pos(pmenu_data->x, pmenu_data->y);
					}
					else if(n == 2)//ȷ������
					{
						pmenu_data->pData = (uint32_t *)pmenu_data->pData;
						pmenu_data->pData = &ok_pw;
						cursor_pos(pmenu_data->x, pmenu_data->y);
					}
				}
				break;
            }
			case KEY_BACK:
				g_return = 1;
				return ;
			case KEY_EXIT:
				g_exit = 1;
				return ;
			case KEY_ENTER & KEY_0:
            {
				plain_code = !plain_code;
				/* ������Ļ��ʾ */
				if (plain_code)	/* ������ʾ */
				{
					GUI_DispStringAt((const char*)old_pw, pw_info[0].x, pw_info[0].y);
					GUI_DispStringAt((const char*)new_pw, pw_info[1].x, pw_info[1].y);
					GUI_DispStringAt((const char*)ok_pw , pw_info[2].x, pw_info[2].y);
				}
				else /* ��������ʾ */
				{
					int8_t j;
					for(j = 0; j < 3; j++)
					{
						m = strlen((const char*)pw_info[j].pData);
						GUI_GotoXY(pw_info[j].x, pw_info[j].y);
						for(i = 0; i < m; i++)
						{
							GUI_DispChar('*');
						}
					}
				}
                
				LCD_REFRESH();
				break;
            }
		}
	}
}



/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE*******************/
