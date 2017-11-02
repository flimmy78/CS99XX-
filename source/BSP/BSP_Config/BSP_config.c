/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�BSP_config.c
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
 
/***********����ͷ�ļ� ************/
#include 	"STM32_GPIO_Config.h"
#include 	"STM32_FSMC.H"

#include	"BSP_config.h"
#include 	"GUI_Draw.h"
#include	"keyboard.h"
#include 	"MC14094.h"
#include 	"Timer_Config.h"
#include 	"STM32_ADC.H"
#include 	"DAC_VREF.h"
#include	"SPI_CPLD.h"
#include	"i2c_ee.h"
#include	"cs99xx_it.h"
#include    "Timer5_Config.h"
#include    "serve_test.h"
#include    "cs99xx_update.h"
#include    "spi_flash.h"



/*********** �������� *************/
/*
 * ��������init_gpio_switch_hv
 * ����  ����ʼ����ѹ����IO
 * ����  ����
 * ���  ����
 * ����  ����
 */
void init_gpio_switch_hv(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	/************************** ���� ***************************/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* �������� */
	
	/* �ж��������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/*
 * ��������off_switch_hv
 * ����  ���رո�ѹ����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void off_switch_hv(void)
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_0);
}
/*
 * ��������CS99XX_NVIC_Group_Init
 * ����  ����ʼ��ϵͳ�жϷ��飬�ڳ�����ֻ�����1��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void init_cs99xx_nvic_group(void)
{
	NVIC_PriorityGroupConfig(CS99XX_IT_GROUP);
}
void set_vector_table(void)
{
#ifndef BOOT_LOADER
	NVIC_SetVectorTable(NVIC_VectTab_FLASH , 0);//���ô���Ĵ�������ַƫ����
#endif
}
/*
 * ��������init_4094
 * ����  ��4094����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void init_4094(void)
{
	MC14094_Init(); /*  */
}

/*
 * ��������bsp_init
 * ����  ��Ӳ����ʼ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void  bsp_init(void)
{
    set_vector_table();/* �ж��������ʼ�� */
	init_cs99xx_nvic_group();/* ��ʼ��NVIC���� */
	FSMC_Configuration();/* FSMC��ʼ�� */
	GUI_Init();			/* GUI ��ʼ�� */
    
	GPIO_Configuration();	/* LED �ж����� �궨�� */
    
    CRC_Config();/* crc ���� */
    
	init_exit();/* ��ʼ���ⲿ�ж� */
    
	TIM2_it_init((50-1), 7200-1);/* 0.1*50=5ms 1000Hz 0.1 * 1000 = 100ms */
    
    init_timer5_callback(serve_test_poll_task);/* ��ʱ���ص����� */
    TIM5_it_init((10-1), 7200-1);/* 0.1ms*10=1ms 1000Hz */
    
	init_keyboard();/* ������ʼ�� */
	
	dac_vref_init();/* DAC��ʼ�� */
	
	init_spi_cpld();/* ��ʼ����cpldͨ������ */
	
#if DMA_FOR_ADC_EN>0
	ADC_DMA_Config();/* DMAͨ�� */
#else
	ADC_Config();/* ADC��ͨ���� */
#endif
    
    init_flash();/* ��ʼ��flash */
    I2C_EE_Init();/* eep��ʼ�� */
    
    init_gpio_switch_hv();/* ��ʼ����ѹ����GPIO */
    
	systick_init();		/* ���ʱ������ */
}

/*
 * ��������systick_init
 * ����  ��ϵͳ�δ�ʱ����ʼ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void systick_init(void)
{
	RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);
	SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC);//1ms 1000Hz
}
/*
 * ��������CRC_Config
 * ����  ��ʹ��Ӳ��CRC��Ҫ��������ʱ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void CRC_Config(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}

//void soft_delay_us(u32 dly_us)
//{
//	unsigned int dly_i;
//	while(dly_us--)
//	{
//		for(dly_i=0;dly_i<802;dly_i++);
//	}
//}

void soft_delay_us(u32 dly_us)
{
	unsigned int dly_i = 8;
    
    dly_i *= dly_us;
    
	while(dly_i--);
}
void soft_delay_10us(u32 dly_us)
{
    soft_delay_us(dly_us);
}

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
