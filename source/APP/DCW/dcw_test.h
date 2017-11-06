
#ifndef __DCW_TEST_H__
#define __DCW_TEST_H__

#include "stm32f10x.h"
#include "cs99xx_struct.h"

typedef struct{
    uint8_t vol_change_flag;
    uint8_t testing;///< ���ڲ��Ա��
    uint8_t judge_err_en;///<�ж��쳣���
    uint8_t forever_testing;///<��Զ���ԣ�������ʱ��Ϊ0ʱʹ��
    uint8_t err_charge;///<dcw ��籨�����
    uint8_t fail_cont;///<������ʧ�ܶ���ʱ��1
}DCW_TEST_FLAG;

#ifdef   _DCW_TEST_GLOBALS
#define  _DCW_TEST
#else
#define  _DCW_TEST  extern
#endif

_DCW_TEST __IO DCW_TEST_FLAG	dcw_test_flag;

extern void run_dcw_test(NODE_STEP *step, NODE_STEP *next_step, TEST_DATA_STRUCT *test_data);
extern void dcw_count_vol_ch_step(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data);
extern void dcw_set_da_value(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data);
extern void dcw_clear_test_data_fall_time_timeout(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data);
#endif //__DCW_TEST_H__

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
