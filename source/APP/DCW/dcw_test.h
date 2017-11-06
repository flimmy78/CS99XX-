
#ifndef __DCW_TEST_H__
#define __DCW_TEST_H__

#include "stm32f10x.h"
#include "cs99xx_struct.h"

typedef struct{
    uint8_t vol_change_flag;
    uint8_t testing;///< 正在测试标记
    uint8_t judge_err_en;///<判断异常标记
    uint8_t forever_testing;///<永远测试，当测试时间为0时使用
    uint8_t err_charge;///<dcw 充电报警标记
    uint8_t fail_cont;///<当发生失败断续时置1
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

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
