/*
 * Copyright(c) 2013,
 * All rights reserved
 * �ļ����ƣ�DAC_VREF.c
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V1.0�������ɱ�д
 * �޸ļ�¼��2014.7 �޸��ˣ�����
 *
 */
/********************* ����ͷ�ļ� ************************/
#include "DAC_VREF.h"
#include "ucos_ii.h"
#include "BSP_config.h"

/************************* Ƭѡ���Ž���λ�� **********************/
#define		CD4051_Pin_A		(*(u8 *)((u32)0x42218194)) /* PB5 */
#define		CD4051_Pin_B		(*(u8 *)((u32)0x422401B4)) /* PG13 */

/************************* ȫ�ֱ������� ****************************/
u16	DAC_Vol[3] = {HV_VREF_RESET,SHORT_VREF_RESET,ARC_VREF_RESET};/* ͨ���ı������Ԫ�ص�ֵ������da���������ͬ�ĵ�ѹ*/
																	/* HV_VREF_RESET ��ѹ��׼*/
																	/* SHORT_VREF_RESET ��·��׼ */
																	/* ARC_VREF_RESET �绡����׼ */

/************************* �ڲ��������� **************************/
static void Dac1_Init(void);
static void CD4051_Init(void);
static void TIM3_Int_Init(u16 arr,u16 psc);
// static void DAC_Delay_ms(unsigned long dly_ms);

/************************* �������� ******************************/

void dac_vref_init(void)
{
	Dac1_Init();
	CD4051_Init();
	TIM3_Int_Init(10-1,7200-1);	/* 10Khz�ļ���Ƶ�ʣ�������10Ϊ1ms */
}

static void Dac1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	/* ʹ��GPIOAͨ��ʱ�� */
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	/* ʹ��DACͨ��ʱ�� */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	/* �˿����� */
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	/* ģ������ */
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4)	;	/* PA.4 ����� */
	
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	/* ��ʹ�ô������� TEN1=0 */
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;	/* ��ʹ�ò��η��� */
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;	/* ���Ρ���ֵ���� */
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	/* DAC1�������ر� BOFF1=1 */
    DAC_Init(DAC_Channel_1,&DAC_InitType);	/* ��ʼ��DACͨ��1 */
	
	DAC_Cmd(DAC_Channel_1, ENABLE);	/* ʹ��DAC1 */
	
    DAC_SetChannel1Data(DAC_Align_12b_R, 0);	/* 12λ�Ҷ������ݸ�ʽ����DACֵ */
}

static void CD4051_Init(void)
{
	/*�����ṹ�����*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*����ӦIOʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE);
	/*����IO*/
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;	/* ����������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	/* ��ʼ���˿�B */
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
	GPIO_Init(GPIOG, &GPIO_InitStructure);	/* ��ʼ���˿�G */
}

static void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); /* ʱ��ʹ�� */
	
	/* ��ʱ��TIM3��ʼ�� */
	TIM_TimeBaseStructure.TIM_Period = arr;	/* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ */
	TIM_TimeBaseStructure.TIM_Prescaler =psc; /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	/* ����ʱ�ӷָ�:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	/* TIM���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); /* ����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); /* ʹ��ָ����TIM3�ж�,��������ж� */
	
	/* �ж����ȼ�NVIC���� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  /* TIM3�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM3_IT_PRE_PRIO;  /* ��ռ���ȼ�0�� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM3_IT_SUB_PRIO;  /* �����ȼ�4�� ����TIM2���� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; /* IRQͨ����ʹ�� */
	NVIC_Init(&NVIC_InitStructure);  /* ��ʼ��NVIC�Ĵ��� */
	
	TIM_Cmd(TIM3, ENABLE);  /* ʹ��TIMx */
}

// static void DAC_Delay_ms(unsigned long dly_ms)
// {
//   unsigned int dly_i;
//   while(dly_ms--)
//     for(dly_i=0;dly_i<8020;dly_i++);
// }

/* ��ʱ��3�жϷ������ */
// void VREF_Scan(void)   /* TIM3�ж� */
// {
// 	static int8_t   DAC_VREF_Change=0;
// 	if(DAC_VREF_Change > 2)DAC_VREF_Change=0;
// 	CD4051_Pin_B = 1;
// 	CD4051_Pin_A = 1;
// 	DAC_Delay_ms(1);
// 	DAC_SetChannel1Data(DAC_Align_12b_R, DAC_Vol[DAC_VREF_Change]);
// 	DAC_Delay_ms(1);
// 	CD4051_Pin_B = (DAC_VREF_Change & 0x02)>>1;
// 	CD4051_Pin_A = DAC_VREF_Change & 0x01;
// 	DAC_VREF_Change++;
// }


void vref_scan_fun(void)   /* TIM3�ж� */
{
	static int32_t flag;
	static int32_t count;
	
	CD4051_Pin_B = 1; CD4051_Pin_A = 1;/* �ر�������׼ͨ�� */
	if(count == 0)
	{
		count = 1;
		return;
	}
	
	switch(flag)
	{
		case 0:
// 		case 1:
// 		case 3:
// 		case 4:
			if(count == 2)
			{
				CD4051_Pin_B = 0; CD4051_Pin_A = 0;/* �򿪸�ѹ��׼ͨ�� */
			}
			else
			{
                DAC_SetChannel1Data(DAC_Align_12b_R, DAC_Vol[0]);
				count = 2;
				return;
			}
			break;
		case 1:
			if(count == 2)
			{
				CD4051_Pin_B = 0; CD4051_Pin_A = 1;/* �򿪶�·��׼ͨ�� */
			}
			else
			{
                DAC_SetChannel1Data(DAC_Align_12b_R, DAC_Vol[1]);
				count = 2;
				return;
			}
			
			break;
		case 2:
			if(count == 2)
			{
				CD4051_Pin_B = 1; CD4051_Pin_A = 0;/* �򿪵绡��׼ͨ�� */
			}
			else
			{
                DAC_SetChannel1Data(DAC_Align_12b_R, DAC_Vol[2]);
				count = 2;
				return;
			}
			
			break;
		default:
			flag = 3;
			CD4051_Pin_B = 1; CD4051_Pin_A = 1;/* �ر�������׼ͨ�� */
			break;
	}
	
	count = 0;
	if(++flag > 2)
	{
		flag = 0;
	}
}
/*
 *�������ܣ�DAC�Ķ�ʱɨ���л�
 *�����������
 *�����������
 *��    ע����
 */
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	OSIntEnter();
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		vref_scan_fun();
	}
	OSIntExit();
}

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
