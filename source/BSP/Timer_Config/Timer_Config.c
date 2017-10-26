/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�Timer-Config.c
 * ժ  Ҫ  �����ó������õ��Ķ�ʱ��
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

/**********************����ͷ�ļ� ************************/
#define _TIM_CONF_GLOBALS

#include    "ucos_ii.h"
#include    "stm32f10x.h"
#include	"BSP_config.h"
#include	"Timer_Config.h"
#include 	"app.h"
#include 	"serve_test.h"

#include 	"SysTemConfig.h"
#include 	"cs99xx_type.h"
#include    "irq.h"

/************************�������� ***********************/
/*
 *
 *��������TIM2_Int_Init
 *������	����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
 *������	arr���Զ���װֵ��
 *		psc��ʱ��Ԥ��Ƶ��
 *˵��������ʹ�õ��Ƕ�ʱ��2Ϊ���Թ����ṩʱ��
 *
*/
void TIM2_it_init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);/* �ر��ж� */
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIM2�����жϱ�־
	
	/* �ж����ȼ�NVIC���� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM2_IT_PRE_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM2_IT_SUB_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);/* ���ж� */
	TIM_Cmd(TIM2, DISABLE);/* �������ÿ��������� */
	return;
}

/*
 *��������bsp_tim2_close
 *�������رն�ʱ��
 *��������
 *���أ���
*/
void bsp_tim2_close(void)
{
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
    TIM_Cmd(TIM2, DISABLE);
}

/*
 *��������bsp_tim2_open
 *�������򿪶�ʱ��
 *��������
 *���أ���
*/
void bsp_tim2_open(void)
{
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}
/*
 *��������TIM2_IRQHandler
 *��������ʱ��2�жϷ������
 *��������
 *���أ���
*/
void TIM2_IRQHandler(void)
{
    static int32_t c = 0;
    
	OSIntEnter();
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx�����жϱ�־
		
        if(++c >= 20)
        {
            c = 0;
            g_test_time++;
            
            OSSemPost(ScheduleSem);
        }
        
        if(test_flag.gradation == STAGE_RISE //����
            || test_flag.gradation == STAGE_FALL //�½�
            || test_flag.gradation == STAGE_CHARGE //���
            || test_flag.gradation == STAGE_CHANGE) //����
        {
            test_flag.vol_rise_step_t++;
            vol_change_200hz(test_flag.vol_rise_step_t);
//             vol_change_1000hz(test_flag.vol_rise_step_t);
        }
	}
	OSIntExit();
}

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
