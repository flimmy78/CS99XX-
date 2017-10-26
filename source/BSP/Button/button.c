/****************************************************************************
* Copyright (C), 2009-2010, www.armfly.com
*
* �ļ���: sysytick.c
* ���ݼ���: ��ģ��ʵ��7�������ļ�⣬�����˲����ƣ�֧�ְ��£�����ͳ���������
*			ͨ��GetKey()�������Զ�ȡ��ֵ
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.1    2009-12-27 armfly  �������ļ�
*
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>

#include "button.h"

static BUTTON_T s_BtnUser;		/* USER �� */
static BUTTON_T s_BtnTamper;	/* TAMPER �� */
static BUTTON_T s_BtnWakeUp;	/* WAKEUP �� */
static BUTTON_T s_BtnUp;		/* ҡ��UP�� */
static BUTTON_T s_BtnDown;		/* ҡ��DOWN�� */
static BUTTON_T s_BtnLeft;		/* ҡ��LEFT�� */
static BUTTON_T s_BtnRight;		/* ҡ��RIGHT�� */
static BUTTON_T s_BtnOk;		/* ҡ��OK�� */

static KEY_FIFO_T s_Key;		/* ����FIFO����,�ṹ�� */

/*
	������STM32F103ZE-EK �������߷��䣺
	USER��     : PG8  (�͵�ƽ��ʾ����)
	TAMPEER��  : PC13 (�͵�ƽ��ʾ����)
	WKUP��     : PA0  (!!!�ߵ�ƽ��ʾ����)
	ҡ��UP��   : PG15 (�͵�ƽ��ʾ����)
	ҡ��DOWN�� : PD3  (�͵�ƽ��ʾ����)
	ҡ��LEFT�� : PG14 (�͵�ƽ��ʾ����)
	ҡ��RIGHT��: PG13 (�͵�ƽ��ʾ����)
	ҡ��OK��   : PG7 (�͵�ƽ��ʾ����)

	����7���������жϰ����Ƿ��£�����ֵ1 ��ʾ���£�0��ʾδ����
*/
static uint8_t IsKeyDownUser(void) 		{if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_8) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDownTamper(void) 	{if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDownWakeUp(void) 	{if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET) return 1; return 0;}
static uint8_t IsKeyDownUp(void) 		{if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_15) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDownDown(void) 		{if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDownLeft(void) 		{if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDownRight(void) 	{if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDownOk(void) 		{if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7) == Bit_SET) return 0; return 1;}
																						  
/*******************************************************************************
	������: InitButtonVar
	��  ��:
	��  ��:
	����˵������ʼ�����еİ������������������Ҫ��systic�ж�����Ǯ����1��
*/
void InitButtonVar(void)
{
	/* �԰���FIFO��дָ������ */
	s_Key.Read = 0;
	s_Key.Write = 0;

	/* ��ʼ��USER�������� */
	s_BtnUser.IsKeyDownFunc = IsKeyDownUser;	/* �жϰ������µĺ��� */
	s_BtnUser.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_BtnUser.LongTime = BUTTON_LONG_TIME;		/* ����ʱ�� */
	s_BtnUser.Count = s_BtnUser.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnUser.State = 0;						/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnUser.KeyCodeDown = KEY_DOWN_USER;		/* �������µļ�ֵ���� */
	s_BtnUser.KeyCodeUp = KEY_UP_USER;			/* ��������ļ�ֵ���� */
	s_BtnUser.KeyCodeLong = KEY_HOLD_USER;		/* �������������µļ�ֵ���� */

	/* ��ʼ��TAMPER�������� */
	s_BtnTamper.IsKeyDownFunc = IsKeyDownTamper;	/* �жϰ������µĺ��� */
	s_BtnTamper.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_BtnTamper.LongTime = 0;						/* ����ʱ��, 0��ʾ����� */
	s_BtnTamper.Count = s_BtnTamper.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnTamper.State = 0;							/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnTamper.KeyCodeDown = KEY_DOWN_TAMPER;		/* �������µļ�ֵ���� */
	s_BtnTamper.KeyCodeUp = 0;						/* ��������ļ�ֵ���� */
	s_BtnTamper.KeyCodeLong = 0;					/* �������������µļ�ֵ���� */

	/* ��ʼ��WAKEUP�������� */
	s_BtnWakeUp.IsKeyDownFunc = IsKeyDownWakeUp;	/* �жϰ������µĺ��� */
	s_BtnWakeUp.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_BtnWakeUp.LongTime = 0;						/* ����ʱ�� */
	s_BtnWakeUp.Count = s_BtnWakeUp.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnWakeUp.State = 0;							/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnWakeUp.KeyCodeUp = 0;						/* ��������ļ�ֵ���룬0��ʾ����� */
	s_BtnWakeUp.KeyCodeDown = KEY_DOWN_WAKEUP;		/* �������µļ�ֵ���� */
	s_BtnWakeUp.KeyCodeLong = 0;					/* �������������µļ�ֵ���룬0��ʾ����� */

	/* ��ʼ��UP�������� */
	s_BtnUp.IsKeyDownFunc = IsKeyDownUp;		/* �жϰ������µĺ��� */
	s_BtnUp.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_BtnUp.LongTime = 0;						/* ����ʱ�� */
	s_BtnUp.Count = s_BtnUp.FilterTime / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnUp.State = 0;							/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnUp.KeyCodeDown = KEY_DOWN_JOY_UP;		/* �������µļ�ֵ���� */
	s_BtnUp.KeyCodeUp = 0;						/* ��������ļ�ֵ���룬0��ʾ����� */
	s_BtnUp.KeyCodeLong = 0;					/* �������������µļ�ֵ���룬0��ʾ����� */

	/* ��ʼ��DOWN�������� */
	s_BtnDown.IsKeyDownFunc = IsKeyDownDown;	/* �жϰ������µĺ��� */
	s_BtnDown.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_BtnDown.LongTime = 0;						/* ����ʱ�� */
	s_BtnDown.Count = s_BtnDown.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnDown.State = 0;						/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnDown.KeyCodeDown = KEY_DOWN_JOY_DOWN;	/* �������µļ�ֵ���� */
	s_BtnDown.KeyCodeUp = 0;					/* ��������ļ�ֵ���룬0��ʾ����� */
	s_BtnDown.KeyCodeLong = 0;					/* �������������µļ�ֵ���룬0��ʾ����� */

	/* ��ʼ��LEFT�������� */
	s_BtnLeft.IsKeyDownFunc = IsKeyDownLeft;	/* �жϰ������µĺ��� */
	s_BtnLeft.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_BtnLeft.LongTime = 0;						/* ����ʱ�� */
	s_BtnLeft.Count = s_BtnLeft.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnLeft.State = 0;						/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnLeft.KeyCodeDown = KEY_DOWN_JOY_LEFT;	/* �������µļ�ֵ���� */
	s_BtnLeft.KeyCodeUp = 0;					/* ��������ļ�ֵ���룬0��ʾ����� */
	s_BtnLeft.KeyCodeLong = 0;					/* �������������µļ�ֵ���룬0��ʾ����� */

	/* ��ʼ��RIGHT�������� */
	s_BtnRight.IsKeyDownFunc = IsKeyDownRight;	/* �жϰ������µĺ��� */
	s_BtnRight.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_BtnRight.LongTime = 0;					/* ����ʱ�� */
	s_BtnRight.Count = s_BtnRight.FilterTime / 2;/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnRight.State = 0;						/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnRight.KeyCodeDown = KEY_DOWN_JOY_RIGHT;/* �������µļ�ֵ���� */
	s_BtnRight.KeyCodeUp = 0;					/* ��������ļ�ֵ���룬0��ʾ����� */
	s_BtnRight.KeyCodeLong = 0;					/* �������������µļ�ֵ���룬0��ʾ����� */

	/* ��ʼ��OK�������� */
	s_BtnOk.IsKeyDownFunc = IsKeyDownOk;		/* �жϰ������µĺ��� */
	s_BtnOk.FilterTime = BUTTON_FILTER_TIME;	/* �����˲�ʱ�� */
	s_BtnOk.LongTime = 0;						/* ����ʱ�� */
	s_BtnOk.Count = s_BtnOk.FilterTime / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnOk.State = 0;							/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnOk.KeyCodeDown = KEY_DOWN_JOY_OK;		/* �������µļ�ֵ���� */
	s_BtnOk.KeyCodeUp = 0;						/* ��������ļ�ֵ���룬0��ʾ����� */
	s_BtnOk.KeyCodeLong = 0;					/* �������������µļ�ֵ���룬0��ʾ����� */
}

/*******************************************************************************
	������: PutKey
	��  ��: ��ֵ
	��  ��:
	����˵������1����ֵѹ�밴��FIFO������
*/
void PutKey(uint8_t _KeyCode)
{
	s_Key.Buf[s_Key.Write] = _KeyCode;

	if (++s_Key.Write  >= KEY_FIFO_SIZE)
	{
		s_Key.Write = 0;
	}
}

/*******************************************************************************
	������: GetKey
	��  ��:
	��  ��: ���ؼ�ֵ, KEY_NONE ( = 0) ��ʾ�޼�����
	����˵�����Ӱ���FIFOȡ1����ֵ
*/
uint8_t GetKey(void)
{
	uint8_t ret;

	if (s_Key.Read == s_Key.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_Key.Buf[s_Key.Read];

		if (++s_Key.Read >= KEY_FIFO_SIZE)
		{
			s_Key.Read = 0;
		}
		return ret;
	}
}

/*******************************************************************************
	��������DetectButton
	��  ��: �����ṹ����ָ��
	��  ��:
	����˵�������ָ���İ���
*/
static void DetectButton(BUTTON_T *_pBtn)
{
	/* ���û�г�ʼ�������������򱨴�
	if (_pBtn->IsKeyDownFunc == 0)
	{
		printf("Fault : DetectButton(), _pBtn->IsKeyDownFunc undefine");
	}
	*/

	if (_pBtn->IsKeyDownFunc())
	{
		if (_pBtn->Count < _pBtn->FilterTime)
		{
			_pBtn->Count = _pBtn->FilterTime;
		}
		else if(_pBtn->Count < 2 * _pBtn->FilterTime)
		{
			_pBtn->Count++;
		}
		else
		{
			if (_pBtn->State == 0)
			{
				_pBtn->State = 1;

				/* ���Ͱ�ť���µ���Ϣ */
				if (_pBtn->KeyCodeDown > 0)
				{
					/* ��ֵ���밴��FIFO */
					PutKey(_pBtn->KeyCodeDown);
				}
			}

			if (_pBtn->LongTime > 0)
			{
				if (_pBtn->LongCount < _pBtn->LongTime)
				{
					/* ���Ͱ�ť�������µ���Ϣ */
					if (++_pBtn->LongCount == _pBtn->LongTime)
					{
						/* ��ֵ���밴��FIFO */
						PutKey(_pBtn->KeyCodeLong);
					}
				}
			}
		}
	}
	else
	{
		if(_pBtn->Count > _pBtn->FilterTime)
		{
			_pBtn->Count = _pBtn->FilterTime;
		}
		else if(_pBtn->Count != 0)
		{
			_pBtn->Count--;
		}
		else
		{
			if (_pBtn->State == 1)
			{
				_pBtn->State = 0;

				/* ���Ͱ�ť�������Ϣ */
				if (_pBtn->KeyCodeUp > 0)
				{
					/* ��ֵ���밴��FIFO */
					PutKey(_pBtn->KeyCodeUp);
				}
			}
		}

		_pBtn->LongCount = 0;
	}
}

/*******************************************************************************
	��������KeyPro
	��  ��: 
	��  ��:
	����˵����������еİ������������Ҫ��systic���жϷ���������
*/
void KeyPro(void)
{
	DetectButton(&s_BtnUser);	/* USER �� */
	DetectButton(&s_BtnTamper);	/* TAMPER �� */
	DetectButton(&s_BtnWakeUp);	/* WAKEUP �� */
	DetectButton(&s_BtnUp);		/* ҡ��UP�� */
	DetectButton(&s_BtnDown);	/* ҡ��DOWN�� */
	DetectButton(&s_BtnLeft);	/* ҡ��LEFT�� */
	DetectButton(&s_BtnRight);	/* ҡ��RIGHT�� */
	DetectButton(&s_BtnOk);		/* ҡ��OK�� */
}
