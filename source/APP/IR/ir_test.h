
#ifndef __ir_TEST_H__
#define __ir_TEST_H__

#include "stm32f10x.h"
#include "cs99xx_struct.h"

typedef struct{
    uint8_t vol_change_flag;
    uint8_t testing;///<ACW���ڲ��Ա��
    uint8_t judge_err_en;///<�ж��쳣���
    uint8_t forever_testing;///<��Զ���ԣ�������ʱ��Ϊ0ʱʹ��
    uint8_t fail_cont;///<������ʧ�ܶ���ʱ��1
}IR_TEST_FLAG;

#ifdef   _IR_TEST_GLOBALS
#define  _IR_TEST
#else
#define  _IR_TEST  extern
#endif

_IR_TEST __IO IR_TEST_FLAG	ir_test_flag;

extern void run_ir_test(NODE_STEP *step, NODE_STEP *next_step, TEST_DATA_STRUCT *test_data);
extern void ir_count_vol_ch_step(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data);
extern void ir_set_da_value(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data);
extern void ir_clear_test_data_fall_time_timeout(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data);
#endif //__ir_TEST_H__

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
