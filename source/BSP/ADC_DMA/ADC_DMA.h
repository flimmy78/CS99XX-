/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�ADC_DMA.h
 * ժ  Ҫ  ����DMAʵ��AD����
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#ifndef __ADC__DMA__INCLUDED__
#define __ADC__DMA__INCLUDED__

#include "stm32f10x.h"

#define ADC_BUF_DEEP		10
#define ADC_CHANNLE			2

extern __IO uint32_t ADCConvertedValue[2];
extern __IO uint16_t ADCTem;
extern __IO uint16_t ADC_VOL;
extern __IO uint16_t ADC_CUR;

extern __IO uint16_t dma_buf[ADC_CHANNLE*ADC_BUF_DEEP];

extern void ADC_DMA_Config(void);
extern void read_adc_from_dma(void);
extern void adc_sample(void);

extern void on_dma(void);
extern void off_dma(void);

#endif 

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
