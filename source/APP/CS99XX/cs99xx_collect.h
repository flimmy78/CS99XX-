/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_vref.c
 * ժ  Ҫ  ��Ϊͨ�ų����ṩ������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#ifndef __CS99XX_COLLECT_H__
#define __CS99XX_COLLECT_H__

#include "stm32f10x.h"

extern void clear_slither_data(void);
extern int8_t judge_err(void);
extern void adc_sample(void);
extern void set_dc_gr_open_err(void);
extern uint8_t get_dc_gr_open_err(void);
extern void set_dc_gr_amp_err(void);
extern uint8_t get_dc_gr_amp_err(void);
extern void clear_slither_data(void);

#endif //__CS99XX_COLLECT_H__

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
