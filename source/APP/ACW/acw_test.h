
#ifndef __ACW_TEST_H__
#define __ACW_TEST_H__

#include "stm32f10x.h"
#include "cs99xx_struct.h"

typedef struct{
    uint8_t vol_change_flag;
    uint8_t testing;///<ACW正在测试标记
    uint8_t judge_err_en;///<判断异常标记
    uint8_t forever_testing;///<永远测试，当测试时间为0时使用
}ACW_TEST_FLAG;

#ifdef   _ACW_TEST_GLOBALS
#define  _ACW_TEST
#else
#define  _ACW_TEST  extern
#endif

_ACW_TEST __IO ACW_TEST_FLAG	acw_test_flag;

extern void run_acw_test(NODE_STEP *step, NODE_STEP *next_step, TEST_DATA_STRUCT *test_data);
extern void acw_count_vol_ch_step(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data);
extern void acw_set_da_value(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data);
extern void clear_test_data_fall_time_timeout(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data);
#endif //__ACW_TEST_H__

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
