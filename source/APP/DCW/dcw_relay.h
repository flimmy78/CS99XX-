
#ifndef __DCW_RELAY_H__
#define __DCW_RELAY_H__

#include "stm32f10x.h"
#include "cs99xx_struct.h"

extern void dcw_into_test_relay_ready(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data);
extern void dcw_exit_test_relay_motion(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data);
extern void dcw_relay_ready(void);
extern void shift_dcw_cur_gear(uint8_t gear);

#endif //__DCW_RELAY_H__


/******************* (C) COPYRIGHT 2017 ³¤Ê¢ÒÇÆ÷ *****END OF FILE****/
