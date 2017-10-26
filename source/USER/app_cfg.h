/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�app_cfg.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#ifndef __APP_CFG_H__
#define __APP_CFG_H__


/************** Ӳ��ѡ������ *****************************/

#define DMA_FOR_ADC_EN	0 /* 1 ʹ��DMA �ɼ����� 0ʹ����ͨ���� */


/*************** ������� ********************************/
enum{
    /* �������ȼ� */
    PIP_PRIO					    = 1,	/* �������ȼ� */
    
    TIMER_TASK_PRIO				    = 2,    /* �������ȼ�0�������ʱ��ʹ���� */
    SCHEDULE_TASK_PRIO			    = 4, 	/* ����ɨ�����ȼ� */
    MODBUS_SEND_TASK_PRIO		    = 3,	/* MODBUS�������� */
    MODBUS_M_SEND_TASK_PRIO		    = 18,	/* MODBUS�������� */
    MODBUS_MASTER_SEND_TASK_PRIO    = 22,   /* Modebus ������������ */
    MODBUS_RECEIVE_TASK_PRIO	    = 21,	/* ֱ��ģ�����ȼ� */
    STARTUP_TASK_PRIO     		    = 24,	/* �����������ȼ� */
    Keyscan_TASK_PRIO			    = 26, 	/* ����ɨ�����ȼ� */
    Main_TASK_PRIO				    = 27,	/* ���������ȼ� */
    SAMPLE_TASK_PRIO			    = 25,	/* �����������ȼ� */
    
    /* ����ID */
    MODBUS_SEND_TASK_ID             = 0,
    Main_TASK_ID			        = 1,
    Keyscan_TASK_ID			        = 2,
    SAMPLE_TASK_ID			        = 3,
    MODBUS_RECEIVE_TASK_ID	        = 4,
    SCHEDULE_TASK_ID		        = 7,
    MODBUS_MASTER_SEND_TASK_ID      = 8,
    MODBUS_M_SEND_TASK_ID      		= 9,
    
    /* ����ջ�Ĵ�С */
    STARTUP_TASK_STK_SIZE 			= 200,	/* 200*4Byte  */
    MODBUS_MASTER_SEND_STK_SIZE     = 300,
    MODBUS_SEND_STK_SIZE			= 300,
    MODBUS_M_SEND_STK_SIZE			= 300,
    MAIN_TASK_STK_SIZE				= 1024,	/* 900 * 4Byte */
    KEY_TASK_STK_SIZE				= 50,
    SCHEDULE_TASK_STK_SIZE			= STARTUP_TASK_STK_SIZE,
    SAMPLE_TASK_STK_SIZE			= 200,
    MODBUS_RECEIVE_TASK_STK_SIZE	= 300,
};

#endif



/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
