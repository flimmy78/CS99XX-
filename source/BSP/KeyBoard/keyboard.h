/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�keyboard.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#define KEY_START	0XFFF0
#define KEY_STOP	0XFFF1
#define KEY_0	    0XFFF7	//1111 0111
#define KEY_1	    0XFFEF	//1110 1111
#define KEY_2	    0XFFDF	//1101 1111
#define KEY_3	    0XFFBF	//1011 1111
#define KEY_4	    0XFF7F	//0111 1111
#define KEY_5	    0XFFFE	//1111 1110
#define KEY_6	    0XFFFD	//1111 1101
#define KEY_7	    0XFFFB	//1111 1011
#define KEY_8	    0XF7FF	//1111 0111
#define KEY_9	    0XEFFF	//1110 1111
#define KEY_EXIT	0XDFFF  //1101 1111
#define KEY_ENTER	0XBFFF  //1011 1111
#define KEY_F1		0X7FFF  //0111 1111
#define KEY_F2		0XFEFF  //1111 1110
#define KEY_F3		0XFDFF  //1111 1101
#define KEY_F4		0XFBFF  //1111 1011
#define KEY_NONE	0XFFFF

#define KEY_UP		KEY_F1
#define KEY_DOWN	KEY_F2
#define KEY_SHIFT	KEY_F3
#define KEY_BACK	KEY_F4

#define KEY_SET		KEY_UP
#define KEY_MENU	KEY_DOWN
#define KEY_TEST	KEY_SHIFT
#define KEY_HELP	KEY_BACK
#define KEY_SHORTCUT	KEY_SHIFT

#define KEY_INSERT	KEY_UP
#define KEY_DEL		KEY_DOWN
#define KEY_MORE	KEY_F3

#define KEY_ADVANCE KEY_UP
#define KEY_RETREAT KEY_DOWN//retreat
#define KEY_SWAP	KEY_SHIFT

#define KEY_SAVE	KEY_F1
#define KEY_NEW		KEY_F2

#define KEY_SHIFT_L KEY_F1
#define KEY_SHIFT_R	KEY_F2
#define KEY_FILE_DEL		KEY_F3

#define KEY_READ	KEY_F2
#define KEY_DEL_FILE		KEY_F1
#define KEY_EDIT_FILE		KEY_F2

#define KEY_DEL_PW	KEY_F3

/* ���Խ�����ʹ�� */
#define KEY_TEST_MEM	KEY_F1
#define KEY_TEST_SET	KEY_F2
#define KEY_TEST_MORE	KEY_F3
#define KEY_TEST_RES	KEY_F2

/* �������ʹ�� */
#define KEY_RES_TRANS	KEY_F1
#define KEY_RES_JUMP	KEY_F2

#define KEY_RES_CLEAR	KEY_F1
#define KEY_RES_CENSUS	KEY_F2


#define KEY_DELAY_NORMAL	(100)
#define KEY_DELAY_CAL		(100)

#ifdef   KEY_GLOBALS
#define  KEY_EXT
#else
#define  KEY_EXT  extern
#endif

KEY_EXT int16_t key_buzzer_time;/* ������������ */

#define BUTTON_FILTER_TIME 	(12)		/* ����ʱ�� */
#define BUTTON_LONG_TIME 	(100)		/* ����1�룬��Ϊ�����¼� */

typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t Count;			/* �˲��������� */
	uint8_t FilterTime;		/* �˲�ʱ��(���255,��ʾ2550ms) */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint16_t State;			/* ������ǰ״̬�����»��ǵ��� */
	uint16_t KeyCodeUp;		/* ��������ļ�ֵ����, 0��ʾ����ⰴ������ */
	uint16_t KeyCodeDown;	/* �������µļ�ֵ����, 0��ʾ����ⰴ������ */
	uint16_t KeyCodeLong;	/* ���������ļ�ֵ����, 0��ʾ����ⳤ�� */
}KEY_T;

extern void init_keyboard(void);
extern uint16_t scan_keyboard(void);
extern void report_key_value(void);

#endif //__KEYBOARD_H__

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
