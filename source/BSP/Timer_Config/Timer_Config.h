/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�Timer_Config.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
 
/*************** ����ȫ�ֱ��� *****************/
#ifndef __TIMER_CONFIG_H__
#define __TIMER_CONFIG_H__

#ifdef   _TIM_CONF_GLOBALS
#define  _TIM_EXT
#else
#define  _TIM_EXT  extern
#endif


/********************** �������� ***************/
extern void TIM2_it_init(u16 arr,u16 psc);
extern void TIM2_IRQHandler(void);
extern void bsp_tim2_close(void);
extern void bsp_tim2_open(void);

extern void TIM5_it_init(u16 arr,u16 psc);
extern void TIM5_IRQHandler(void);

#endif	//__TIMER_CONFIG_H__
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
