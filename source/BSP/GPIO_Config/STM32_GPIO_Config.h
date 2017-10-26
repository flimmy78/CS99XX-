/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�STM32_GPIO_Config.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#ifndef __STM32_GPIO_CONFIG_H__
#define __STM32_GPIO_CONFIG_H__

#include "stm32f10x.h"


//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum)       ((addr & 0xF0000000)+0x2000000+((addr & 0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)              *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)      MEM_ADDR(BITBAND(addr, bitnum))
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08

#define EXTI_LINE5_ENABLE       BIT_ADDR(EXTI_BASE, 5)	/* ��λ�ж� */
// #define EXTI_LINE9_ENABLE    BIT_ADDR(EXTI_BASE, 9)	/* �����ж� */
#define EXTI_LINE10_ENABLE      BIT_ADDR(EXTI_BASE, 10)	/* ��·�ж� */
#define EXTI_LINE11_ENABLE      BIT_ADDR(EXTI_BASE, 11)	/* ARC�ж� */
#define EXTI_LINE12_ENABLE      BIT_ADDR(EXTI_BASE, 12)	/* GFI�ж� */

#define RTC_INI_ENABLE          BIT_ADDR(RTC_BASE+0, 0)	/* RTC�ж� */


//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)            BIT_ADDR(GPIOA_ODR_Addr,n)  //���
#define PAin(n)             BIT_ADDR(GPIOA_IDR_Addr,n)  //����

#define PBout(n)            BIT_ADDR(GPIOB_ODR_Addr,n)  //���
#define PBin(n)             BIT_ADDR(GPIOB_IDR_Addr,n)  //����

#define PCout(n)            BIT_ADDR(GPIOC_ODR_Addr,n)  //���
#define PCin(n)             BIT_ADDR(GPIOC_IDR_Addr,n)  //����

#define PDout(n)            BIT_ADDR(GPIOD_ODR_Addr,n)  //���
#define PDin(n)             BIT_ADDR(GPIOD_IDR_Addr,n)  //����

#define PEout(n)            BIT_ADDR(GPIOE_ODR_Addr,n)  //���
#define PEin(n)             BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)            BIT_ADDR(GPIOF_ODR_Addr,n)  //���
#define PFin(n)             BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)            BIT_ADDR(GPIOG_ODR_Addr,n)  //���
#define PGin(n)             BIT_ADDR(GPIOG_IDR_Addr,n)  //����

/////////////////////////////////////////////////////////////////

#define LED_TEST    (PCout(6))     //test
#define LED_PASS    (PGout(8))     //pass
#define LED_FAIL    (PGout(7))     //fail
#define LED_ALL     LED_TEST=LED_PASS=LED_FAIL

#define LED_OFF     (0)
#define LED_ON      (1)

#define ROW0	    (PAout(7))
#define ROW1	    (PBout(2))

#define BUZZER	    (PBout(1))

#define	BUZZER_OFF	(0)
#define BUZZER_ON	(1)

#define START_PIN   (PAin(6))   /* ��start���ŵ�ƽ */
#define STOP_PIN	(PAin(5))   /* ��stop���ŵ�ƽ */
#define AMP_PIN		(PAin(9))   /* ���������ŵ�ƽ */
#define SHORT_PIN 	(PAin(10))	/* ����·���ŵ�ƽ */
#define ARC_PIN		(PAin(11))	/* ��ARC ���ŵ�ƽ */
#define GFI_PIN		(PAin(12))	/* ��GFI ���ŵ�ƽ */

#define CAL_PIN		(PGin(15))	/* ��У׼�������� */

extern void GPIO_Configuration(void);

#endif //__STM32_GPIO_CONFIG_H__

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/

