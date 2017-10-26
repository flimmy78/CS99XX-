/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�keyboard.c
 * ժ  Ҫ  �����̹���ģ��
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#define KEY_GLOBALS

#include 	"STM32_GPIO_Config.h"
#include 	"app.h"
#include 	"serve_test.h"
#include 	"keyboard.h"


int16_t key_buzzer_time = KEY_DELAY_NORMAL;

static uint8_t IsKeyDown_key_start(void)   {return START_PIN==RESET;}
static uint8_t IsKeyDown_key_stop(void)    {return STOP_PIN==RESET; }

static uint8_t IsKeyDown_key_F1(void)   {return (scan_keyboard() == KEY_UP);	}
static uint8_t IsKeyDown_key_F2(void)   {return (scan_keyboard() == KEY_DOWN);	}
static uint8_t IsKeyDown_key_F3(void)   {return (scan_keyboard() == KEY_SHIFT);	}
static uint8_t IsKeyDown_key_F4(void)   {return (scan_keyboard() == KEY_BACK);	}
static uint8_t IsKeyDown_key_exit(void) {return (scan_keyboard() == KEY_EXIT);	}
static uint8_t IsKeyDown_key_enter(void){return (scan_keyboard() == KEY_ENTER);	}
static uint8_t IsKeyDown_key_0(void) {return (scan_keyboard() == KEY_0);	}
static uint8_t IsKeyDown_key_1(void) {return (scan_keyboard() == KEY_1);	}
static uint8_t IsKeyDown_key_2(void) {return (scan_keyboard() == KEY_2);	}
static uint8_t IsKeyDown_key_3(void) {return (scan_keyboard() == KEY_3);	}
static uint8_t IsKeyDown_key_4(void) {return (scan_keyboard() == KEY_4);	}
static uint8_t IsKeyDown_key_5(void) {return (scan_keyboard() == KEY_5);	}
static uint8_t IsKeyDown_key_6(void) {return (scan_keyboard() == KEY_6);	}
static uint8_t IsKeyDown_key_7(void) {return (scan_keyboard() == KEY_7);	}
static uint8_t IsKeyDown_key_8(void) {return (scan_keyboard() == KEY_8);	}
static uint8_t IsKeyDown_key_9(void) {return (scan_keyboard() == KEY_9);	}

static uint8_t IsKeyDown_key_01(void) {return (scan_keyboard() == (KEY_0 & KEY_1));}
static uint8_t IsKeyDown_key_02(void) {return (scan_keyboard() == (KEY_0 & KEY_2));}//ע�⣡(KEY_0 & KEY_2)��������ŷ���ͻ���ֻ����һ����ֵ�ȽϵĴ����������
static uint8_t IsKeyDown_key_03(void) {return (scan_keyboard() == (KEY_0 & KEY_3));}
static uint8_t IsKeyDown_key_04(void) {return (scan_keyboard() == (KEY_0 & KEY_4));}
static uint8_t IsKeyDown_key_05(void) {return (scan_keyboard() == (KEY_0 & KEY_5));}
static uint8_t IsKeyDown_key_06(void) {return (scan_keyboard() == (KEY_0 & KEY_6));}
static uint8_t IsKeyDown_key_EN0(void) {return (scan_keyboard() == (KEY_ENTER & KEY_0));}
static uint8_t IsKeyDown_key_EN1(void) {return (scan_keyboard() == (KEY_ENTER & KEY_1));}
static uint8_t IsKeyDown_key_F4_8(void) {return (scan_keyboard() == (KEY_F4 & KEY_8));}
static uint8_t IsKeyDown_key_F4_9(void) {return (scan_keyboard() == (KEY_F4 & KEY_9));}
static uint8_t IsKeyDown_key_F4_5(void) {return (scan_keyboard() == (KEY_F4 & KEY_5));}

static KEY_T s_Key_start;		/* KEY_START */
static KEY_T s_Key_stop;		/* KEY_STOP */

static KEY_T s_Key_up;		/* KEY_F1 */
static KEY_T s_Key_down;	/* KEY_F2 */
static KEY_T s_Key_shift;	/* KEY_F3 */
static KEY_T s_Key_back;	/* KEY_F4 */
static KEY_T s_Key_exit;	/* KEY_EXIT */
static KEY_T s_Key_enter;	/* KEY_ENTER */
static KEY_T s_Key_0;		/* KEY_0 */
static KEY_T s_Key_1;		/* KEY_1 */
static KEY_T s_Key_2;		/* KEY_2 */
static KEY_T s_Key_3;		/* KEY_3 */
static KEY_T s_Key_4;		/* KEY_4 */
static KEY_T s_Key_5;		/* KEY_5 */
static KEY_T s_Key_6;		/* KEY_6 */
static KEY_T s_Key_7;		/* KEY_7 */
static KEY_T s_Key_8;		/* KEY_8 */
static KEY_T s_Key_9;		/* KEY_9 */

static KEY_T s_Key_01;		/* KEY_1 & KEY_0 */
static KEY_T s_Key_02;		/* KEY_1 & KEY_2 */
static KEY_T s_Key_03;		/* KEY_1 & KEY_3 */
static KEY_T s_Key_04;		/* KEY_1 & KEY_4 */
static KEY_T s_Key_05;		/* KEY_1 & KEY_5 */
static KEY_T s_Key_06;		/* KEY_1 & KEY_6 */
static KEY_T s_Key_en0;		/* KEY_ENTER & KEY_0 */
static KEY_T s_Key_en1;		/* KEY_ENTER & KEY_1 */
static KEY_T s_Key_F4_8;		/* KEY_F4 & KEY_8 */
static KEY_T s_Key_F4_9;		/* KEY_F4 & KEY_9 */
static KEY_T s_Key_F4_5;		/* KEY_F4 & KEY_5 */

/*
 * ��������InitKeyStr
 * ����  ����ʼ������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void InitKeyStr(void)
{
	/************************��������******************************/	
	s_Key_start.IsKeyDownFunc = IsKeyDown_key_start;	/* �жϰ������µĺ��� */
	s_Key_start.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_start.LongTime = 0;		/* ����ʱ�� */
	s_Key_start.Count = s_Key_start.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_start.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_start.KeyCodeDown = KEY_START;		/* �������µļ�ֵ���� */
	s_Key_start.KeyCodeUp = 0;	/* ��������ļ�ֵ���� */
	s_Key_start.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	/************************��λ����******************************/	
	s_Key_stop.IsKeyDownFunc = IsKeyDown_key_stop;	/* �жϰ������µĺ��� */
	s_Key_stop.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_stop.LongTime = 0;		/* ����ʱ�� */
	s_Key_stop.Count = s_Key_start.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_stop.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_stop.KeyCodeDown = KEY_STOP;		/* �������µļ�ֵ���� */
	s_Key_stop.KeyCodeUp = 0;	/* ��������ļ�ֵ���� */
	s_Key_stop.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	/**********************��ϼ�****************************/
	s_Key_F4_5.IsKeyDownFunc = IsKeyDown_key_F4_5;	/* �жϰ������µĺ��� */
	s_Key_F4_5.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_F4_5.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_F4_5.Count = s_Key_F4_5.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_F4_5.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_F4_5.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_F4_5.KeyCodeUp = KEY_F4 & KEY_5;	/* ��������ļ�ֵ���� */
	s_Key_F4_5.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_F4_9.IsKeyDownFunc = IsKeyDown_key_F4_9;	/* �жϰ������µĺ��� */
	s_Key_F4_9.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_F4_9.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_F4_9.Count = s_Key_F4_9.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_F4_9.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_F4_9.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_F4_9.KeyCodeUp = KEY_F4 & KEY_9;	/* ��������ļ�ֵ���� */
	s_Key_F4_9.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_F4_8.IsKeyDownFunc = IsKeyDown_key_F4_8;	/* �жϰ������µĺ��� */
	s_Key_F4_8.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_F4_8.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_F4_8.Count = s_Key_F4_8.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_F4_8.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_F4_8.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_F4_8.KeyCodeUp = KEY_F4 & KEY_8;	/* ��������ļ�ֵ���� */
	s_Key_F4_8.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_en1.IsKeyDownFunc = IsKeyDown_key_EN1;	/* �жϰ������µĺ��� */
	s_Key_en1.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_en1.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_en1.Count = s_Key_en1.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_en1.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_en1.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_en1.KeyCodeUp = KEY_1 & KEY_ENTER;	/* ��������ļ�ֵ���� */
	s_Key_en1.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_en0.IsKeyDownFunc = IsKeyDown_key_EN0;	/* �жϰ������µĺ��� */
	s_Key_en0.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_en0.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_en0.Count = s_Key_en0.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_en0.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_en0.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_en0.KeyCodeUp = KEY_0 & KEY_ENTER;	/* ��������ļ�ֵ���� */
	s_Key_en0.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	
	s_Key_06.IsKeyDownFunc = IsKeyDown_key_06;	/* �жϰ������µĺ��� */
	s_Key_06.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_06.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_06.Count = s_Key_06.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_06.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_06.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_06.KeyCodeUp = KEY_0 & KEY_6;	/* ��������ļ�ֵ���� */
	s_Key_06.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_05.IsKeyDownFunc = IsKeyDown_key_05;	/* �жϰ������µĺ��� */
	s_Key_05.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_05.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_05.Count = s_Key_05.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_05.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_05.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_05.KeyCodeUp = KEY_0 & KEY_5;	/* ��������ļ�ֵ���� */
	s_Key_05.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_04.IsKeyDownFunc = IsKeyDown_key_04;	/* �жϰ������µĺ��� */
	s_Key_04.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_04.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_04.Count = s_Key_04.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_04.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_04.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_04.KeyCodeUp = KEY_0 & KEY_4;	/* ��������ļ�ֵ���� */
	s_Key_04.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	
	s_Key_03.IsKeyDownFunc = IsKeyDown_key_03;	/* �жϰ������µĺ��� */
	s_Key_03.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_03.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_03.Count = s_Key_03.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_03.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_03.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_03.KeyCodeUp = KEY_0 & KEY_3;	/* ��������ļ�ֵ���� */
	s_Key_03.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_02.IsKeyDownFunc = IsKeyDown_key_02;	/* �жϰ������µĺ��� */
	s_Key_02.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_02.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_02.Count = s_Key_02.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_02.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_02.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_02.KeyCodeUp = KEY_0 & KEY_2;	/* ��������ļ�ֵ���� */
	s_Key_02.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_01.IsKeyDownFunc = IsKeyDown_key_01;	/* �жϰ������µĺ��� */
	s_Key_01.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_01.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_01.Count = s_Key_01.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_01.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_01.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_01.KeyCodeUp = KEY_0 & KEY_1;	/* ��������ļ�ֵ���� */
	s_Key_01.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	/***************************16����ͨ������********************************/
	
	s_Key_up.IsKeyDownFunc = IsKeyDown_key_F1;	/* �жϰ������µĺ��� */
	s_Key_up.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_up.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_up.Count = s_Key_up.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_up.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_up.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_up.KeyCodeUp = KEY_UP;	/* ��������ļ�ֵ���� */
	s_Key_up.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_down.IsKeyDownFunc = IsKeyDown_key_F2;	/* �жϰ������µĺ��� */
	s_Key_down.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_down.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_down.Count = s_Key_down.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_down.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_down.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_down.KeyCodeUp = KEY_DOWN;	/* ��������ļ�ֵ���� */
	s_Key_down.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_shift.IsKeyDownFunc = IsKeyDown_key_F3;	/* �жϰ������µĺ��� */
	s_Key_shift.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_shift.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_shift.Count = s_Key_shift.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_shift.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_shift.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_shift.KeyCodeUp = KEY_SHIFT;	/* ��������ļ�ֵ���� */
	s_Key_shift.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_back.IsKeyDownFunc = IsKeyDown_key_F4;	/* �жϰ������µĺ��� */
	s_Key_back.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_back.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_back.Count = s_Key_back.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_back.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_back.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_back.KeyCodeUp = KEY_BACK;	/* ��������ļ�ֵ���� */
	s_Key_back.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */

	s_Key_exit.IsKeyDownFunc = IsKeyDown_key_exit;	/* �жϰ������µĺ��� */
	s_Key_exit.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_exit.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_exit.Count = s_Key_exit.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_exit.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_exit.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_exit.KeyCodeUp = KEY_EXIT;	/* ��������ļ�ֵ���� */
	s_Key_exit.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_enter.IsKeyDownFunc = IsKeyDown_key_enter;	/* �жϰ������µĺ��� */
	s_Key_enter.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_enter.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_enter.Count = s_Key_enter.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_enter.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_enter.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_enter.KeyCodeUp = KEY_ENTER;	/* ��������ļ�ֵ���� */
	s_Key_enter.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */

	s_Key_0.IsKeyDownFunc = IsKeyDown_key_0;	/* �жϰ������µĺ��� */
	s_Key_0.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_0.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_0.Count = s_Key_0.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_0.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_0.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_0.KeyCodeUp = KEY_0;	/* ��������ļ�ֵ���� */
	s_Key_0.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_1.IsKeyDownFunc = IsKeyDown_key_1;	/* �жϰ������µĺ��� */
	s_Key_1.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_1.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_1.Count = s_Key_1.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_1.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_1.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_1.KeyCodeUp = KEY_1;	/* ��������ļ�ֵ���� */
	s_Key_1.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_2.IsKeyDownFunc = IsKeyDown_key_2;	/* �жϰ������µĺ��� */
	s_Key_2.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_2.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_2.Count = s_Key_2.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_2.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_2.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_2.KeyCodeUp = KEY_2;	/* ��������ļ�ֵ���� */
	s_Key_2.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_3.IsKeyDownFunc = IsKeyDown_key_3;	/* �жϰ������µĺ��� */
	s_Key_3.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_3.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_3.Count = s_Key_3.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_3.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_3.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_3.KeyCodeUp = KEY_3;	/* ��������ļ�ֵ���� */
	s_Key_3.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_4.IsKeyDownFunc = IsKeyDown_key_4;	/* �жϰ������µĺ��� */
	s_Key_4.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_4.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_4.Count = s_Key_4.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_4.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_4.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_4.KeyCodeUp = KEY_4;	/* ��������ļ�ֵ���� */
	s_Key_4.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_5.IsKeyDownFunc = IsKeyDown_key_5;	/* �жϰ������µĺ��� */
	s_Key_5.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_5.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_5.Count = s_Key_5.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_5.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_5.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_5.KeyCodeUp = KEY_5;	/* ��������ļ�ֵ���� */
	s_Key_5.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_6.IsKeyDownFunc = IsKeyDown_key_6;	/* �жϰ������µĺ��� */
	s_Key_6.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_6.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_6.Count = s_Key_6.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_6.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_6.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_6.KeyCodeUp = KEY_6;	/* ��������ļ�ֵ���� */
	s_Key_6.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_7.IsKeyDownFunc = IsKeyDown_key_7;	/* �жϰ������µĺ��� */
	s_Key_7.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_7.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_7.Count = s_Key_7.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_7.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_7.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_7.KeyCodeUp = KEY_7;	/* ��������ļ�ֵ���� */
	s_Key_7.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_8.IsKeyDownFunc = IsKeyDown_key_8;	/* �жϰ������µĺ��� */
	s_Key_8.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_8.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_8.Count = s_Key_8.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_8.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_8.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_8.KeyCodeUp = KEY_8;	/* ��������ļ�ֵ���� */
	s_Key_8.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
	
	s_Key_9.IsKeyDownFunc = IsKeyDown_key_9;	/* �жϰ������µĺ��� */
	s_Key_9.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_Key_9.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_Key_9.Count = s_Key_9.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_Key_9.State = 0;		/* ����ȱʡ״̬��0Ϊδ���� */
	s_Key_9.KeyCodeDown = 0;		/* �������µļ�ֵ���� */
	s_Key_9.KeyCodeUp = KEY_9;	/* ��������ļ�ֵ���� */
	s_Key_9.KeyCodeLong = 0;		/* �������������µļ�ֵ���� */
}


/*
 * ��������DetectKey
 * ����  ����ذ����Ƿ��� ������¾Ͷ���Ӧ�ı�־λ��λ
 * ����  ��p ָ��ǰ��ذ����Ľṹ
 * ���  ����
 * ����  ����
 */
static void DetectKey(KEY_T *p)
{
	/* ���û�г�ʼ�������������򱨴�*/
	if (p->IsKeyDownFunc == NULL)
	{
		return;
	}
    
	if (p->IsKeyDownFunc())
	{
		if (p->Count < p->FilterTime)
		{
			p->Count = p->FilterTime;
		}
		else if(p->Count < 2 * p->FilterTime)
		{
			p->Count++;
		}
		else
		{
			if (p->State == 0)
			{
				p->State = 1;
				
				if(BUZZER_EN)
                {
                    if(p != &s_Key_start && p != &s_Key_stop)/* ������������ */
                    {
                        if(sys_par.buzzer_sw)
                        {
                            BUZZER_SOUND(key_buzzer_time);
                        }
                    }
                }
				/* ���Ͱ�ť���µ���Ϣ */
				if (p->KeyCodeDown > 0)
				{
					/* ��ֵ���밴��FIFO */
					OSQPost(KeyboardQSem,&p->KeyCodeDown);
				}
			}
			
			if (p->LongTime > 0)
			{
				if (p->LongCount < p->LongTime)
				{
					/* ���Ͱ�ť�������µ���Ϣ */
					if (++p->LongCount == p->LongTime)
					{
						/* ��ֵ���밴��FIFO */
// 						OSQPost(KeyboardQSem,&key_value);
					}
				}
			}
		}	
	}
	else if(scan_keyboard() == KEY_NONE)
	{
		if(p->Count > p->FilterTime)
		{
			p->Count = p->FilterTime;
		}
		else if(p->Count != 0)
		{
			p->Count--;
		}
		else
		{
			if (p->State == 1)
			{
				p->State = 0;
                
				/* ���Ͱ�ť�������Ϣ */
				if (p->KeyCodeUp > 0)
				{
					/* ��ֵ���밴��FIFO */
					OSQPost(KeyboardQSem,&p->KeyCodeUp);
				}
			}
		}
		p->LongCount = 0;
	}
}

/*
 * ��������DetectKey
 * ����  �������ϼ��Ƿ��� ������¾Ͷ���Ӧ�ı�־λ��λ
 * ����  ��p ָ��ǰ��ذ����Ľṹ
 * ���  ����
 * ����  ����
 */
static void Det_combination(KEY_T *p)
{
	/* ���û�г�ʼ�������������򲻴��� */
	if (p->IsKeyDownFunc == NULL)
	{
		return;//2014.10.28
	}
	
	/* ��ⰴ������ */
	if (p->IsKeyDownFunc())
	{
		/* �˲� */
		if (p->Count < p->FilterTime)
		{
			p->Count = p->FilterTime;
		}
		else if(p->Count < 2 * p->FilterTime)
		{
			p->Count++;
		}
		/* ��ǰ����Ѱ��� */
		else
		{
			if (p->State == 0)
			{
				p->State = 1;
				
				/* �������� */
				if(BUZZER_EN)
                {
                    if(p != &s_Key_start && p != &s_Key_stop)/* ������������ */
                    {
                        if(sys_par.buzzer_sw)
                        {
                            BUZZER_SOUND(key_buzzer_time);
                        }
                    }
                }
			}
		}
	}
	
	if(p->State)
	{
		s_Key_up.State = 0;		/* KEY_UP */
		s_Key_down.State = 0;	/* KEY_DOWN */
		s_Key_shift.State = 0;	/* KEY_SHIFT */
		s_Key_back.State = 0;	/* KEY_BACK */
		s_Key_exit.State = 0;	/* KEY_EXIT */
		s_Key_enter.State = 0;	/* KEY_ENTER */
		s_Key_0.State = 0;		/* KEY_0 */
		s_Key_1.State = 0;		/* KEY_1 */
		s_Key_2.State = 0;		/* KEY_2 */
		s_Key_3.State = 0;		/* KEY_3 */
		s_Key_4.State = 0;		/* KEY_4 */
		s_Key_5.State = 0;		/* KEY_5 */
		s_Key_6.State = 0;		/* KEY_6 */
		s_Key_7.State = 0;		/* KEY_7 */
		s_Key_8.State = 0;		/* KEY_8 */
		s_Key_9.State = 0;		/* KEY_9 */
        
		while(scan_keyboard() != KEY_NONE)
		{
            OSTimeDlyHMSM(0,0,0,10);//�ó�CPU
		}
        
        p->State = 0;
        OSQPost(KeyboardQSem,&p->KeyCodeUp);
	}
}

/*
 * ��������bsp_keyboard_init
 * ����  ������Ӳ����ʼ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void bsp_keyboard_init(void)
{
	/*************************    �����ʼ���ṹ����    *************************/
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/*************************    ��������ʱ��    *************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD 
							| RCC_APB2Periph_GPIOE, ENABLE);  //����GPIOB��GPIOEʱ��
    
	/*************************    ���þ������    *************************/
	/************** 8�� **************/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			 //��������
	
	//����PD8��PD9��PD10Ϊ����(3��)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//����PE11��PE12��PE13��PE14��PE15 (5��)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/************** 2�� **************/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	
	//PA7��0�� (C0)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PB2��1�� (C1)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/************************* ������������ *************************/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	
	/************* ����PA6Ϊ���룬START���� **************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void init_keyboard(void)
{
    bsp_keyboard_init();
    InitKeyStr();
}
/*
 * ��������scan_keyboard
 * ����  ������ɨ�� 
 * ����  ����
 * ���  ����
 * ����  ������ɨ��ļ���ֵ
 */
uint16_t scan_keyboard(void)
{
	uint16_t data1 = 0, data2 = 0,data3 = 0;
    
	ROW0 = 0;ROW1 = 1;
	data1 = GPIO_ReadInputData(GPIOE);
	data2 = GPIO_ReadInputData(GPIOD);
	data1 &= 0xf800;
	data2 &= 0x0700;
	data3 = (data1 | data2);
	ROW0 = 1;ROW1 = 0;
	data1 = GPIO_ReadInputData(GPIOE);
	data2 = GPIO_ReadInputData(GPIOD);
	data1 &= 0xf800;
	data2 &= 0x0700;
	data2 = (data1 | data2);
    
	return (data3 | (data2>>8));
}

/*
 * ��������report_key_value
 * ����  ���û�����ͨ���˺���������Ƿ��а������� ����а��¾ͼ�¼������������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void report_key_value(void)
{
	/* ��ϼ� */
	Det_combination(&s_Key_01);
	Det_combination(&s_Key_02);
	Det_combination(&s_Key_03);
	Det_combination(&s_Key_04);
	Det_combination(&s_Key_05);
	Det_combination(&s_Key_06);
	Det_combination(&s_Key_en0);
	Det_combination(&s_Key_en1);
	Det_combination(&s_Key_F4_8);
	Det_combination(&s_Key_F4_9);
	Det_combination(&s_Key_F4_5);
	
	/* ���� */
	DetectKey(&s_Key_start);
	DetectKey(&s_Key_stop);
	
	/* ���󵥼� */
	DetectKey(&s_Key_up);
	DetectKey(&s_Key_down);
	DetectKey(&s_Key_shift);
	DetectKey(&s_Key_back);
	DetectKey(&s_Key_exit);
	DetectKey(&s_Key_enter);
	DetectKey(&s_Key_0);
	DetectKey(&s_Key_1);
	DetectKey(&s_Key_2);
	DetectKey(&s_Key_3);
	DetectKey(&s_Key_4);
	DetectKey(&s_Key_5);
	DetectKey(&s_Key_6);
	DetectKey(&s_Key_7);
	DetectKey(&s_Key_8);
	DetectKey(&s_Key_9);
}



/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
