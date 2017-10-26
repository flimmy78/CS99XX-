/*
 * Copyright(c) 2015,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_err_log.h
 * ժ  Ҫ  ��������־��صĽӿ�
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#ifndef __CS99XX_ERR_LOG_H__
#define __CS99XX_ERR_LOG_H__


#include <stdio.h>

/* ��¼�����쳣 �ú겻��ʵ��Ϊ���� ԭ�������� ��˲������� */
#define RECORD_EXCEP(info)  \
        {   \
            sprintf(g_excep_t.excep_buf, "%s %d %s", __FUNCTION__, __LINE__, (info));   \
            updata_excep_info(&g_excep_t); \
        }

/* ��s��ֵǰ�ȼ����t�Ƿ��ǺϷ��ĸ��������ٸ�ֵ */
#define SAFE_FLOAT_VOLU_CAL(s , t) \
        if(!isnan(t) && !isinf(t)) \
        { \
            s = t; \
        } \
        else \
        { \
            s = 1; \
        }
//             RECORD_EXCEP("cal_ratio err"); \
        
typedef struct{
    char excep_buf[40];                 /* �����쳣�ֳ� */
    uint32_t excep_time;                /* �쳣������ʱ�� */
}EXCEP_INFO_STR;//��¼�쳣�ṹ
#ifdef   _99xxERR_LOG_GLOBALS
#define  _99xxERR_LOG_EXT
#else
#define  _99xxERR_LOG_EXT  extern
#endif


_99xxERR_LOG_EXT EXCEP_INFO_STR         g_excep_t;


void updata_excep_info(EXCEP_INFO_STR *p_excep);
void clear_excep_info(void);


#endif //__CS99XX_ERR_LOG_H__

/******************* (C) COPYRIGHT 2015 ��ʢ���� *****END OF FILE****/
