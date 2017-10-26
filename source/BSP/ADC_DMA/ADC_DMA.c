/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�ADC_DMA.c
 * ժ  Ҫ  ����DMAʵ��AD����
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#include	<stdio.h>
#include    "ADC_DMA.h"
#include 	"STM32_ADC.H"
#include 	"serve_test.h"

#define ADC1_DR_Address ((uint32_t)0x4001244C)//����Ӳ��ADC1�������ַ

__IO uint32_t ADCConvertedValue[2];//ת����5ͨ��ADֵ
__IO uint16_t ADCTem;
__IO uint16_t ADC_VOL;
__IO uint16_t ADC_CUR;
__IO uint16_t dma_buf[ADC_CHANNLE*ADC_BUF_DEEP];
/**********************************************************
** ������: void All_GPIO_Config(void)
** ��������: ���е�GPIO�����÷�������
** �������: ��
** �������: ��
***********************************************************/
// void All_GPIO_Config(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;  //����GPIO�ṹ��
// 	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
// 	/*��PB0����Ϊ�������*/
// 	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;//PB0
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50MHzʱ���ٶ�
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
// }
/**********************************************************
** ������: ADC1_DMA_Config
** ��������: ADC1��DMA��ʽ����
** �������: ��
** �������: ��
***********************************************************/
void ADC1_DMA_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;//����ADC�ṹ��
	DMA_InitTypeDef DMA_InitStructure;//����DMA�ṹ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��DMA1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1| RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE ); //ʹ��ADC1��GPIOAʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);/* ����Ҫ */
	
	/*��ΪADC1��6ͨ��ģ�������GPIO��ʼ������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//Pc2,3����Ϊģ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*DMA1��ͨ��1����*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (ADC1_BASE + 0x4c);//�����Դͷ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&dma_buf;//Ŀ���ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //������Դͷ
	DMA_InitStructure.DMA_BufferSize = ADC_CHANNLE*ADC_BUF_DEEP;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ���������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���贫���԰���Ϊ��λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�ڴ��� ����Ϊ��λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//4���ȼ�֮һ��(������)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //���ڴ浽�ڴ�
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//�������ϲ�����ʼ��DMA_InitStructure
	DMA_Cmd(DMA1_Channel1, ENABLE);//ʹ��DMA1��ͨ��1
	
	/*����ΪADC1������*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ģ��ת��������ɨ��ģʽ����ͨ����
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//ģ��ת������������ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;//ת����ADCͨ������ĿΪ2
	
	ADC_Init(ADC1, &ADC_InitStructure);//Ҫ�����²�����ʼ��ADC_InitStructure
	
	/* ����ADC1��3��������ͨ�����������ǵ�ת��˳��Ͳ���ʱ��*/
	//ת��ʱ��Tconv=����ʱ��+12.5������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_239Cycles5); //ADC1ͨ��0ת��˳��Ϊ1������ʱ��Ϊ7.5������ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 2, ADC_SampleTime_239Cycles5);
	
	ADC_DMACmd(ADC1, ENABLE);/*ʹ��ADC1��DMA���䷽ʽ*/
	
	ADC_Cmd(ADC1, ENABLE);/*ʹ��ADC1 */
	
	ADC_ResetCalibration(ADC1);/*����ADC1��У׼�Ĵ��� */
	
	while(ADC_GetResetCalibrationStatus(ADC1));/*��ȡADC����У׼�Ĵ�����״̬*/
	ADC_StartCalibration(ADC1); /*��ʼУ׼ADC1*/
	while(ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼���
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ��ADC1���ת��
}

void ADC_DMA_Config(void)
{
	ADC1_DMA_Config();
}
/* ð������ */
void bubble_sort (uint16_t data[], size_t size) 
{
	uint32_t i = 0;
	
	for (i = 0; i < size - 1; ++i) 
	{
		int ordered = 1;
		uint32_t j;
		
		for (j = 0; j < size - 1 - i; ++j)
		{
			if (data[j+1] < data[j]) 
			{
				int swap = data[j];
				data[j] = data[j+1];
				data[j+1] = swap;
				ordered = 0;
			}
		}
		
		if(ordered)
		{
			break;
		}
	}
}

void read_adc_from_dma(void)
{
#define DEEP 31
    
    int32_t i = 0;
	int32_t j = 0;
	int32_t k = 0;
	int32_t sum[2] = {0};
	static uint16_t temp_buf[ADC_CHANNLE][DEEP];
	static uint16_t adc_buf[ADC_CHANNLE][ADC_BUF_DEEP];
	
	for(k = 0; k < DEEP; k++)
	{
		for(j = 0; j < ADC_CHANNLE; j++)
		{
			for(i = 0; i<ADC_BUF_DEEP; i++)
			{
				adc_buf[j][i] = dma_buf[i*ADC_CHANNLE+j];
			}
		}
		
		/* ��ֵ�˲� */
		for(i = 0; i < ADC_CHANNLE; i++)
		{
			bubble_sort(adc_buf[i], ADC_BUF_DEEP);
			temp_buf[i][k] = adc_buf[i][ADC_BUF_DEEP/2];
		}
	}
	
	/* ��ֵ�˲� */
	for(i=0; i<ADC_CHANNLE; i++)
	{
		for(j = 0; j < DEEP; j++)
		{
			sum[i]+=temp_buf[i][j];
		}
		adc_value[i] = sum[i] / DEEP;
	}
}

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
