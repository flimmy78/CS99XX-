/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_it.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#ifndef __CS99XX_EXTI__
#define __CS99XX_EXTI__

/******************** �����ļ� ************************/
#include <stm32f10x.h>

/********************  �������� ************************/
extern void init_exit(void);
extern void exit_sw(void);
extern void exit_off(void);
extern void exit_off_all(void);
extern void disable_stop_exit(void);
extern void enable_stop_exit(void);

#endif //__CS99XX_EXTI__

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE*******************/
