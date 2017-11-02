/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�BSP_config.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#ifndef  __BSP_CONFIG_H 
#define  __BSP_CONFIG_H

#include "stm32f10x.h" 
#include "ucos_ii.h"

/*********************** �궨�� *****************************/
#define SOFT_DELAY		200

/* �ж����ȼ����� */
enum{
CS99XX_IT_GROUP	  = NVIC_PriorityGroup_3,/* �������ȼ�3λ 0-7 ��Ӧ���ȼ���0-1 */
UART2_IT_PRE_PRIO = 0,
UART2_IT_SUB_PRIO = 0,

UART5_IT_PRE_PRIO = 0,
UART5_IT_SUB_PRIO = 1,

TIM2_IT_PRE_PRIO  = 1,
TIM2_IT_SUB_PRIO  = 0,

EXIT_IT_PRE_PRIO  = 3,
STOP_IT_SUB_PRIO  = 0,
SAG_IT_SUB_PRIO   = 1,

TIM3_IT_PRE_PRIO  = 2,
TIM3_IT_SUB_PRIO  = 0,

TIM4_IT_PRE_PRIO  = 3,
TIM4_IT_SUB_PRIO  = 1,

TIM5_IT_PRE_PRIO  = 4,
TIM5_IT_SUB_PRIO  = 0,

TIM6_IT_PRE_PRIO  = 4,
TIM6_IT_SUB_PRIO  = 1,

RTC_IT_PRE_PRIO	  = 5,
RTC_IT_SUB_PRIO	  = 0,
};

/*********************** �������� *************************/
extern void init_4094(void);
extern void bsp_init(void);
extern void CRC_Config(void);
extern void systick_init(void);
extern void soft_delay_us(u32 dly_us);
extern void soft_delay_10us(u32 dly_us);

extern void on_switch_hv(void);
extern void off_switch_hv(void);

extern void soft_delay_10us(u32 dly_us);

#endif //__BSP_CONFIG_H
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
