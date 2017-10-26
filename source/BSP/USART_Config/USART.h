/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�usart.h
 * ժ  Ҫ  ������ͨ��
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */


#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "string.h"
#include "cs99xx_config.h"

#define __USART2_ENABLE

#define     BUFFER_SIZE     (200)	//�������ݻ����С
#define 	SYN_TIM_OUT		(100)


#ifdef   USART_GLOBALS
#define  USART_EXT
#else
#define  USART_EXT  extern
#endif

USART_EXT FlagStatus receive_ok;
USART_EXT uint8_t receive_buf[BUFFER_SIZE];
USART_EXT uint8_t receive_data_num;
USART_EXT uint8_t syn_receive_buf[2][BUFFER_SIZE];
USART_EXT uint16_t syn_data_num[2];
USART_EXT uint8_t receive_data_num;
USART_EXT int8_t cur_point;/* ָ�򻺳�ĵ�ǰλ�� */

// USART_EXT uint16_t cur_data_lon;
USART_EXT uint16_t uart5_timeout;
USART_EXT void (*p_uart5_irq) (void);
USART_EXT uint8_t ReceiveCmdData[BUFFER_SIZE];

void Uart5_Init(void);

void Uart5_SendStr(char str[]);

void Com_Client_PutStr(uint8_t *str, uint8_t len);
uint8_t Read_Usart_data(uint8_t *data);
void Clear_Usart_data(void);


#endif

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
