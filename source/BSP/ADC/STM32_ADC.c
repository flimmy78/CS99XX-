/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�STM32_ADC.c
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�������
 * ��ʷ�汾��
 * �޸ļ�¼��2014.11.21 �޸��ˣ����� 
 *
 */

#include "STM32_ADC.H"
#include "serve_test.h"

u16 ADC_Voltage,ADC_Current;



void NVIC_Config_For_ADC_Watchdog(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_Init(&NVIC_InitStructure);
}
void ADC_WatchdogConfig(uint16_t HighThreshold)
{
    ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_13);
    ADC_AnalogWatchdogThresholdsConfig(ADC1, HighThreshold, 0);
    ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
    
    NVIC_Config_For_ADC_Watchdog();
}

void ADC_WatchdogON(void)
{
//     ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);
}
void ADC_WatchdogOFF(void)
{
    ADC_ITConfig(ADC1, ADC_IT_AWD, DISABLE);
}

/*
 * ��������ADC_Config
 * ����  ������ADC
 * ����  ����
 * ���  ����
 */
void ADC_Config(void)
{
	ADC_InitTypeDef 	ADC_InitStructure;
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	//PA1 ��Ϊģ��ͨ����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
	
	
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
	
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
	
    ADC_WatchdogConfig(4095);
}


void ADC1_2_IRQHandler(void)
{
	uint16_t ad = 0;
	
    ADC_ITConfig(ADC1, ADC_IT_AWD, DISABLE);
    
    if(SET == ADC_GetFlagStatus(ADC1, ADC_FLAG_AWD))
    {
        ADC_ClearFlag(ADC1, ADC_FLAG_AWD);
        ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
		
		ad = Get_Adc(13);
		
		if(ad > g_ad_dog + 8)
		{
			ad = g_ad_dog + 3;
		}
		
		cur_ave = ad * cur_adc_cur_k;
		irq_stop_relay_motion();
		record_exception_scene();/* ��¼�쳣�ֳ� */
		ERR_NUM = ERR_HIGH;
		ADC_WatchdogOFF();
    }
    
    ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);
}
//���ADCֵ
u16 Get_Adc(u8 ch)
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	
	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}


#if DMA_FOR_ADC_EN==0
u16 Get_Adc_Average(u8 ch,u8 times)
{
#define DEEP	51
	u32 sum=0;
	u16 buf[DEEP] = {0};
	uint16_t temp = 0;
	u8 i,j,k;
	u8 N = DEEP;
	
	i = j = k = 0;
	for(k=0;k<times;k++)
	{
		/* �ɼ� */
		for(i=0;i<N;i++)
		{
			u8 t;
			buf[i]=Get_Adc(ch);
			for(t=0;t<2;t++);
		}
		/* ��ֵ�˲� */
		for(j=N-1;j>0;j--)
		{
			for(i=0;i<j;i++)
			{
				if(buf[i]>buf[i+1])
				{
					temp	 = buf[i];
					buf[i]	 = buf[i+1];
					buf[i+1] = temp;
				}
			}
		}
		sum+=buf[N/2];
	}
	/* ��ֵ�˲� */
	return sum/times;
}
#endif
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE*******************/
