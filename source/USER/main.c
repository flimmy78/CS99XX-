/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�main.c
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

/******************** �����ļ� ************************/

#include    "app.h"
#include    "BSP_config.h"

/******************** �������� ************************/

int main(void)
{
    uint8_t err = 0;
	
    init_4094();/* ����������ʼ��4094 */
	
	OSInit();//ϵͳ��ʼ��
    
	/* ������Ϣ����Ϊ���̷��� */
	KeyboardQSem  = OSQCreate(QMsgTbl, OS_MAX_QS);
	MdbusQSem  = OSQCreate(MdbusQMsgTbl, MDBUS_BUF_SIZE);
	
	/* �����ź���Ϊ���Բ������ */
	ScheduleSem		= OSSemCreate(0);
    
	/*���������������ٽ��*/
	MemMutex = OSMutexCreate(PIP_PRIO,&err);
	MdbusMutex = OSMutexCreate(PIP_PRIO,&err);
    
	/* ������������ */
	OSTaskCreate(startup_task,(void*)0,
				&Startup_Task_STK[STARTUP_TASK_STK_SIZE-1],
				STARTUP_TASK_PRIO);
	
	OSStart(); /* ��������ϵͳ */
	
  	return 0;
}

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
