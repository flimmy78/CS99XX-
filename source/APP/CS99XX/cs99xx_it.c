/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_it.c
 * ժ  Ҫ  ����ʼ���ⲿ�жϲ�Ϊ��Щ�ж��ṩ����ӿ�
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#include "cs99xx_it.h"
#include "serve_test.h"
#include "cs99xx_type.h"
#include "BSP_config.h"
#include "stm32f10x.h"
#include "STM32_ADC.H"


/*
 * ��������init_exit
 * ����  ���ⲿ�ж�����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void init_exit(void)
{
	/*************************    �����ʼ���ṹ����    *************************/
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure; //�������ݽṹ�ı���
	NVIC_InitTypeDef NVIC_InitStructure;//�������ݽṹ�ı��� //ѡ��������
	
	/*************************    ��������ʱ��    *************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD
							| RCC_APB2Periph_GPIOE, ENABLE);  //����GPIOB��GPIOEʱ��
	
	/*************** �������� *********************/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			 //��������
	
	/* STOP������AMP,SHORT,ARC,GFI������������ */
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/**************** �ж��������� ****************/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=EXIT_IT_PRE_PRIO;    //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;              //������ͨ�����ж�
	
	/*************** �ⲿ�ж����ȼ����� ************/
	/* STOP--KEY PA6  AMP PA9 */
	NVIC_InitStructure.NVIC_IRQChannel=(EXTI9_5_IRQn);
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=STOP_IT_SUB_PRIO;
	NVIC_Init(&NVIC_InitStructure);
	
	/* SHORT,ARC,GFI * PA10,PA11,PA12 */
	NVIC_InitStructure.NVIC_IRQChannel=(EXTI15_10_IRQn);
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=SAG_IT_SUB_PRIO;
	NVIC_Init(&NVIC_InitStructure);
	
	/**************** �ⲿ�ж����� ****************/
	EXTI_DeInit();//��ʼ��Ϊȱʡֵ
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//ѡ���½�_Falling������_Rising�ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);  //�ܽ�ѡ��
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;//��������ڷ���EXTI_Line5��������
	EXTI_Init(&EXTI_InitStructure);
	
	/* Place protection exti lines */
	/* PA9���ű������ò�ѯ�ķ��� */
	// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource9);  //�ܽ�ѡ��
	// 	EXTI_InitStructure.EXTI_Line = EXTI_Line9;//��������ڷ���EXTI_Linex��������
	// 	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource10);  //�ܽ�ѡ��
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;//��������ڷ���EXTI_Linex��������
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);  //�ܽ�ѡ��
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;//��������ڷ���EXTI_Linex��������
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);  //�ܽ�ѡ��
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;//��������ڷ���EXTI_Linex��������
	EXTI_Init(&EXTI_InitStructure);
	
	/* ���ж� */
	STOP_INT(DISABLE);	/* ��λ�ж� */
	SHORT_INT(DISABLE);	/* ��·�ж� */
	GFI_INT(DISABLE);	/* GFI �ж� */
	ARC_INT(DISABLE);	/* ARC �ж� */
}
/*
 * ��������exit_sw
 * ����  ������ʱ�ⲿ�жϵ��л�����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void exit_sw(void)
{
    exit_off();
	/********************************* ���ж� ****************************/
	RTC_INI(DISABLE);
	/* �ж��Ƿ��GFI�ж�  �ӵ�ʱ�رո���ʱ�� */
	if(sys_par.test_method == FLOAT_MODE && sys_par.is_gfi_protect == GFI_ON)
	{
		GFI_INT(ENABLE);
	}
	else
	{
		GFI_INT(DISABLE);
	}
	
    STOP_INT(ENABLE);	/* ��λ�ж� */
    
	ADC_WatchdogOFF();
	
	switch(cur_mode)
	{
		case ACW:
			SHORT_INT(ENABLE);	/* ��·�ж� */
			
			/* �绡��� */
			if(cur_arc_gear)
			{
				ARC_INT(ENABLE);
			}
			else
			{
				ARC_INT(DISABLE);
			}
			
			ADC_WatchdogON();
			break;
		case DCW:
			STOP_INT(ENABLE);	/* ��λ�ж� */
			SHORT_INT(ENABLE);	/* ��·�ж� */
			
			/* �绡��� */
			if(cur_arc_gear)
			{
				ARC_INT(ENABLE);
			}
			else
			{
				ARC_INT(DISABLE);
			}
			/* ֱ����ѹ �绡���ر� �����绡�ж�����·���� */
// 			ARC_INT(ENABLE);
			
			ADC_WatchdogON();
			break;
		case IR:
			STOP_INT(ENABLE);	/* ��λ�ж� */
            
			if(type_spe.ir_short_int_en == ENABLE)
			{
				SHORT_INT(ENABLE);	/* ��·�ж� */
			}
			else
			{
				SHORT_INT(DISABLE);	/* ��·�ж� */
			}
			
			ARC_INT(DISABLE);	/* �绡��� */
			break;
		case GR:
			STOP_INT(ENABLE);	/* ��λ�ж� */
			SHORT_INT(DISABLE);	/* ��·�ж� */
			ARC_INT(DISABLE);	/* �绡��� */
			break;
		case BBD:
			STOP_INT(ENABLE);	/* ��λ�ж� */
			SHORT_INT(DISABLE);	/* ��·�ж� */
			ARC_INT(DISABLE);	/* �绡��� */
			break;
        case CC:
			STOP_INT(ENABLE);	/* ��λ�ж� */
			SHORT_INT(DISABLE);	/* ��·�ж� */
			ARC_INT(DISABLE);	/* �绡��� */
            break;
	}
}

/*
 * ��������exit_off
 * ����  ���ر��ⲿ�ж�
 * ����  ����
 * ���  ����
 * ����  ����
 */
void exit_off(void)
{
	ARC_INT(DISABLE);	/* �ص绡��� */
	STOP_INT(DISABLE);  /* �رո�λ�ж� */
	GFI_INT(DISABLE);   /* �ر�GFI�ж� */
}
/*
 * ��������exit_off_all
 * ����  ���ر�ȫ���ⲿ�ж�
 * ����  ����
 * ���  ����
 * ����  ����
 */
void exit_off_all(void)
{
	ARC_INT(DISABLE);	/* �ص绡��� */
	SHORT_INT(DISABLE);  /* �رո�λ�ж� */
	GFI_INT(DISABLE);   /* �ر�GFI�ж� */
}
/*
 * ��������disable_stop_exit
 * ����  ���ر��ⲿ�ж�
 * ����  ����
 * ���  ����
 * ����  ����
 */
void disable_stop_exit(void)
{
	STOP_INT(DISABLE);  /* �رո�λ�ж� */
}
/*
 * ��������enable_stop_exit
 * ����  ���ر��ⲿ�ж�
 * ����  ����
 * ���  ����
 * ����  ����
 */
void enable_stop_exit(void)
{
	STOP_INT(ENABLE);  /* �򿪸�λ�ж� */
}

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE*******************/
