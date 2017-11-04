
#ifndef __TEST_COM_H__
#define __TEST_COM_H__

#include "cs99xx_struct.h"
#include "cs99xx_it.h"

#ifndef NULL
#define NULL 0
#endif

typedef enum {
	ST_VOL_RISE     = 0,
	ST_TESTING      = 1,
	ST_VOL_FALL     = 2,
	ST_INTER_WAIT   = 3,
	ST_WAIT         = 4,
	ST_PASS         = 5,
	ST_STOP         = 6,            /* ��λ״̬ */
	ST_ERR_H        = 7,
	ST_ERR_L        = 8,
	ST_ERR_SHORT    = 9,
	ST_ERR_VOL_ABNORMAL = 10,       /* ��ѹ�쳣 */
	ST_ERR_ARC      = 11,
	ST_ERR_GFI      = 12,
	ST_ERR_FAIL     = 13,
	ST_ERR_REAL     = 14,           /* ��ʵ�������� */
	ST_ERR_CHAR     = 15,           /* ��籨�� */
	ST_ERR_GEAR     = 16,           /* ���̱��� / ��λ���� */
	ST_ERR_AMP      = 17,
	ST_OUTPUT_DELAY = 18,
	ST_ERR_OPEN     = 19,           /* ��·���� */
    
    ST_ERR_NONE     ,///<û�з����쳣
    ST_CHANGE       ,///<��ѹ����
    ST_CHARGE       ,///<���ڳ��
    ST_STAB         ,///<��ѹʱ��
    ST_DISCHARGE    ,///<���ڷŵ�
    ST_DISCHARGE_OVER,///<�ŵ����
    
}TestingStatusTypedef;  /*����ʱ״̬*/

typedef enum{
    STAGE_READY     = 0,    /* Ԥ���׶� */
    STAGE_RISE      = 1,    /* �����׶� */
    STAGE_TEST      = 2,    /* ���Խ׶� */
    STAGE_FALL      = 3,    /* �½��׶� */
    STAGE_CHANGE    = 4,    /* ����׶� */
    STAGE_CHARGE    = 5,    /* ���׶� */
    STAGE_DIS_CHA   = 6,    /* �ŵ�׶� */
    STAGE_INTER     = 7,    /* ����׶� */
    STAGE_STAB      = 8,    /* ��ѹ�׶� */
    STAGE_RE_READY  = 9,    /* �ٴ�Ԥ���׶� */
    STAGE_FAIL_CONT = 10,   /* ʧ�ܼ����׶� */
}TEST_STAGE;

extern void cs_delay_ms(int time);
extern void cs_delay_us(int time);
extern void close_test_timer(void);
extern void open_test_timer(void);
extern void init_com_test_env(TEST_DATA_STRUCT *test_data);

#endif //__TEST_COM_H__

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
