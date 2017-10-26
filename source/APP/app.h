/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�app.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#ifndef __APP_H__
#define __APP_H__

/*********************** ����ͷ�ļ� *************************/
#include <ucos_ii.h>
#include "stm32f10x.h"
#include "cs99xx_config.h"
#include "cs99xx_struct.h"

/*********************** �궨�� ****************************/



/* ���������� */
// #if MUTE_EN
// #define BUZZER_SOUND(n)     {BUZZER = BUZZER_ON;g_buzzer_time = (n);}
// #else
#define RT_STRING(S1, S2)	(sys_par.language==CHINESE? S1:S2)
#define BUZZER_SOUND(n)     { \
            if(ENABLE != sys_par.silent_sw) \
            { \
                BUZZER = BUZZER_ON; \
                g_buzzer_time = (n); \
            } \
        }
// #endif
#define CYLINDER_CTRL(n)    {cur_cylinder_ctrl_status = CY_STOP; g_cylinder_ctrl_t = (n);}

#define PASS_WORD 		"888888"	/* ϵͳ���� */
#define CALIBRATE_PW 	"000000"	/* У׼���� */
#define AUTHEN_PW_CAL   1   /* ��֤У׼���� */
#define AUTHEN_PW_SYS   0   /* ��֤ϵͳ���� */

#ifndef NULL
#define NULL    0
#endif

/********************** ���Ͷ��� *************************/
typedef struct Sys_St_Flag{
	int8_t ratio_state;/* ϵͳУ׼����״̬ */
}SYS_ST_FLAG;
/********************** ȫ�ֱ��� *************************/

typedef uint8_t (*UART_FUN)(void);

#ifdef   APP_GLOBALS
#define  APP_EXT
#else
#define  APP_EXT  extern
#endif

enum {
REMOTE_COMM = 1,	/* Զ��״̬ */
LOCAL_COMM = 0,		/* ����״̬ */
};


APP_EXT SYS_ST_FLAG 	g_sys_state;	/* ϵͳ״̬ */
APP_EXT uint8_t g_buzzer_sw;	/* ���������� */
APP_EXT uint8_t plain_code; /* �������뿪�� 1Ϊ���� */
APP_EXT uint8_t gunlock;	/* ���򿪼�����ʱ������ֵ��1��һ�ز����Ͳ��������������� */

APP_EXT uint8_t g_exit;	/* �˻ص������� */
APP_EXT uint8_t g_return;	/* ���ص��ϼ��˵� */
APP_EXT uint8_t g_skip_page;/* ��תҳ */

APP_EXT uint32_t g_serial_num;/* ����Ʒ����ˮ�� */
APP_EXT uint32_t g_comm_status;/* ����״̬��־ */
APP_EXT uint32_t g_short_delay;/* ��·�ж���ʱ ACW��DCW������������ */
APP_EXT uint8_t  key_status;/* ��ǰ�״̬��ҪΪͨ���ṩ */

APP_EXT		char		 gUpdateTime[20];

APP_EXT 	OS_EVENT     *KeyboardQSem;	/* ��Ϣ���� */
APP_EXT	 	void     	 *QMsgTbl[OS_MAX_QS];

#define     MDBUS_BUF_SIZE      20
APP_EXT 	OS_EVENT     *MdbusQSem;	/* Mdbus������Ϣ���� */
APP_EXT	 	void     	 *MdbusQMsgTbl[MDBUS_BUF_SIZE];

APP_EXT 	OS_EVENT     *ScheduleSem;/* �����ź��� */
APP_EXT 	OS_EVENT     *MemMutex; /* ����memory�Ļ��⻥����� */
APP_EXT 	OS_EVENT     *MdbusMutex;/* ����mdbus���Ͷ��еĻ��⻥����� */

//APP_EXT		uint16_t	 power_check_flag;	/* ��Դ����־ */

#if SYNCHRO_ROADS_EN>0
APP_EXT		OS_TMR		 * syn_tmr;			/* Ϊ��·ͬ�����Է�����ʱ�ɼ�����ֵ */
APP_EXT		OS_TMR		 * syn_scan_tmr;			/* Ϊ��·ͬ�����Է�����ʱɨ�� */
#endif

/*****************************************************************
*					task stack
*****************************************************************/

APP_EXT OS_STK  Startup_Task_STK[STARTUP_TASK_STK_SIZE];
APP_EXT OS_STK  Keyscan_Task_STK[KEY_TASK_STK_SIZE];
APP_EXT OS_STK  Sample_Task_STK[SAMPLE_TASK_STK_SIZE];
APP_EXT OS_STK  Modbus_Send_STK[MODBUS_SEND_STK_SIZE];
APP_EXT OS_STK  Modbus_M_Send_STK[MODBUS_M_SEND_STK_SIZE];
APP_EXT OS_STK  Modbus_Master_Send_STK[MODBUS_MASTER_SEND_STK_SIZE];
APP_EXT OS_STK  Main_Task_STK[MAIN_TASK_STK_SIZE];

#if SYNCHRO_ROADS_EN>0
APP_EXT OS_STK  SYN_Roads_Task_STK[SYN_ROADS_TASK_STK_SIZE];
APP_EXT OS_STK  SYN_Send_Task_STK[SYN_SEND_TASK_STK_SIZE];
#endif



APP_EXT UART_FUN  p_prvvUARTRxISR;
APP_EXT UART_FUN  p_prvvUARTTxReadyISR;


/********************* �ⲿ�������� *******************/
extern int8_t authenticate_pw(const int8_t n);

/********************* �������� ********************/

extern uint8_t get_cal_pin(void);
extern void off_keyboard(void);
extern void on_keyboard(void);
extern void off_sample_task(void);
extern void on_sample_task(void);
extern void disable_sample_task(void);
extern void resume_sample_task(void);
extern void off_schedule_task(void);
extern void on_schedule_task(void);
extern void clear_mdbus_send_queue(void);
extern void clear_keyboard(void);

extern void startup_task(void* p_arg);
extern uint8_t get_key_task_state(void);

extern void mdbus_init_s(void);
extern void mdbus_init_m(void);

#endif //__APP_H__
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
