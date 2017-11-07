
#ifndef __dc_gr_TEST_H__
#define __dc_gr_TEST_H__

#include "stm32f10x.h"
#include "cs99xx_struct.h"

typedef struct{
    uint8_t vol_change_flag;
    uint8_t testing;///<ACW正在测试标记
    uint8_t judge_err_en;///<判断异常标记
    uint8_t forever_testing;///<永远测试，当测试时间为0时使用
    uint8_t fail_cont;///<当发生失败断续时置1
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

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
