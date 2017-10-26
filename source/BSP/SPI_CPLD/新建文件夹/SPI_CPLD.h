/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�SPI_CPLD.H
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�������
 * ��ʷ�汾��
 * �޸ļ�¼��2014.8.12 ����
 *
 */
#ifndef _SPI_CPLD_h
#define _CPI_CPLD_h

#include "stm32f10x.h"

/* �Ĵ���λ */
#define CPLD_GND_SELECT		0X80
#define	CPLD_IR_GR			0X40
#define CPLD_IR_F5			0x20
#define CPLD_IR_F4			0X10
#define	CPLD_IR_F3			0x08
#define CPLD_IR_F6			0X04
#define	CPLD_IR_F2			0X02
#define CPLD_IR_F1			0x01

#define	CPLD_W				0X01000000	/* д */
#define	CPLD_R				0X02000000	/* �� */

/* ���� */
#define CPLD_START				0X0101AAAA 	/* ���������� */
#define CPLD_STOP				0X01015555	/* ֹͣ���� */
#define	CPLD_READ_DIFFER		0X02000000 	/* ����λ�� */
#define CPLD_CUR_CUNT			0x02010000	/* ����������ֵ */


/*********************** �ⲿ�������� ************************/
void cpld_init(void);
void cpld_write(u32 SendData);
u16 ReadDataFromCPLD(u32 SendData);


#endif


/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
