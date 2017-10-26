/****************************************************************************
* Copyright (C), 2009-2010, www.armfly.com
*
* �ļ���: button.h
* ���ݼ���: ͷ�ļ�
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.1    2009-12-28 armfly  �������ļ�
*
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BUTTON_H
#define __BUTTON_H

/* �����˲�ʱ��50ms, ��λ10ms
 ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
*/
#define BUTTON_FILTER_TIME 	5
#define BUTTON_LONG_TIME 	100		/* ����1�룬��Ϊ�����¼� */

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
	���Ա������ʵ���˲��Ͷ��ְ���״̬�������
*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t Count;			/* �˲��������� */
	uint8_t FilterTime;		/* �˲�ʱ��(���255,��ʾ2550ms) */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t KeyCodeUp;		/* ��������ļ�ֵ����, 0��ʾ����ⰴ������ */
	uint8_t KeyCodeDown;	/* �������µļ�ֵ����, 0��ʾ����ⰴ������ */
	uint8_t KeyCodeLong;	/* ���������ļ�ֵ����, 0��ʾ����ⳤ�� */
}BUTTON_T;

/* �����ֵ����
	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2)	�������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	/* Ϊ����ʾ����Ҫ���USER�������¼��ͳ����¼� */
	KEY_DOWN_USER,			/* User������ */
	KEY_UP_USER,			/* User������ */
	KEY_HOLD_USER,			/* User������ */

	KEY_DOWN_WAKEUP,		/* WakeUp������ */
	KEY_DOWN_TAMPER,		/* Tamper������ */

	KEY_DOWN_JOY_UP,		/* ҡ��UP������ */
	KEY_DOWN_JOY_DOWN,		/* ҡ��DOWN������ */
	KEY_DOWN_JOY_LEFT,		/* ҡ��LEFT������ */
	KEY_DOWN_JOY_RIGHT,		/* ҡ��RIGHT������ */
	KEY_DOWN_JOY_OK			/* ҡ��OK������ */
}KEY_ENUM;

/*
	����FIFO�õ�����
*/
#define KEY_FIFO_SIZE	20
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;	/* ��������ָ�� */
	uint8_t Write;	/* ������дָ�� */
}KEY_FIFO_T;

void InitButtonVar(void);
void PutKey(uint8_t _KeyCode);
uint8_t GetKey(void);
void KeyPro(void);

#endif


