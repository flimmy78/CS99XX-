/*
 * Copyright(c) 2013,
 * All rights reserved
 * �ļ����ƣ�DAC_VREF.h
 * ժ  Ҫ  ��ͷ�ļ�
 * ��ǰ�汾��V1.0�������ɱ�д
 * �޸ļ�¼��2014.7 �޸���:����
 *
 */
 
#ifndef __DAC_VREF_H
#define __DAC_VREF_H


#if defined(__cplusplus)
    extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/******************* <Include> ********************/
#include "stm32f10x.h"
			
/******************* <define> *********************/
#define		HV_VREF_RESET		(0)		/* 0V */
#define		SHORT_VREF_RESET	(4095)//(3120)	/* 5V */
#define		ARC_VREF_RESET		(4095)//(3120)	/* 5V */

typedef enum{
    DAC_OUTPUT_VREF,/* ��ѹ�����׼ */
    DAC_SHORT_VREF,/* ��·���ݻ�׼ */
    DAC_ARC_VREF,/* ARC�����׼ */
}DAC_VREF_ENUM;

extern 	u16		DAC_Vol[3];

/******************* �������� *********************/
extern void dac_vref_init(void);
extern void set_output_da_vref(uint16_t hv_da_value);

/**************************************************/
#if defined(__cplusplus)
    }
#endif

#endif

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
