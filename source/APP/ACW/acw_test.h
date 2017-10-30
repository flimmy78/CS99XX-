
#ifndef __ACW_TEST_H__
#define __ACW_TEST_H__

#include "stm32f10x.h"
#include "cs99xx_struct.h"

typedef struct{
    uint8_t gradation;
    uint8_t dis_status;
    uint8_t vol_change_flag;
    uint8_t test_over;///<���Խ���
    uint8_t testing;///<ACW���ڲ��Ա��
    uint8_t judge_err_en;///<�ж��쳣���
}ACW_TEST_FLAG;

#ifdef   _ACW_TEST_GLOBALS
#define  _ACW_TEST
#else
#define  _ACW_TEST  extern
#endif

_ACW_TEST __IO ACW_TEST_FLAG	acw_test_flag;
extern void run_acw_test(TEST_FILE *test_file, ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data);
#endif //__ACW_TEST_H__

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
