/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�STM32_GPIO_Config.c
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#include "STM32_GPIO_Config.h"

/*
*************************************************
*����: void GPIO_Configuration(void��
*����: GPIO����
*����: ��
*����: ��
*************************************************
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOG, ENABLE);
	
	/************************** ���� ***************************/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	/* �������� */
	
	/* �ж��������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* �ͺ�ѡ���������� */
	/* TYPE1-6 */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
//	GPIO_Init(GPIOF, &GPIO_InitStructure);
//	/* TYPE7-10 */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 /* | GPIO_Pin_9 | GPIO_Pin_10 */;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
// 	/* TYPE12 */
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* У׼����PG15 */
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode	=GPIO_Mode_IPU;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	/************************** ��� *****************************/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //�������
	
	//����FALL,PASS,TEST ��PC6��PG7��PG8Ϊ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//����FMQ ������PB1Ϊ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    LED_ALL  = LED_OFF;/* �ر� LED */
	
	BUZZER = BUZZER_OFF;/* ������ */
	
	return;
}

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
