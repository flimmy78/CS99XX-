
#ifndef __dc_gr_TEST_H__
#define __dc_gr_TEST_H__

#include "stm32f10x.h"
#include "cs99xx_struct.h"

typedef struct{
    uint8_t vol_change_flag;
    uint8_t testing;///<ACW���ڲ��Ա��
    uint8_t judge_err_en;///<�ж��쳣���
    uint8_t forever_testing;///<��Զ���ԣ�������ʱ��Ϊ0ʱʹ��
    uint8_t fail_cont;///<������ʧ�ܶ���ʱ��1
}DC_GR_TEST_FLAG;

#ifdef   _DC_GR_TEST_GLOBALS
#define  _DC_GR_TEST
#else
#define  _DC_GR_TEST  extern
#endif

_DC_GR_TEST __IO DC_GR_TEST_FLAG	dc_gr_test_flag;

extern void run_dc_gr_test(NODE_STEP *step, NODE_STEP *next_step, TEST_DATA_STRUCT *test_data);
extern void dc_gr_count_vol_ch_step(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data);
extern void dc_gr_set_da_value(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data);
#endif //__dc_gr_TEST_H__

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
