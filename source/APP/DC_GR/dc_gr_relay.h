
#ifndef __dc_gr_RELAY_H__
#define __dc_gr_RELAY_H__

#include "stm32f10x.h"
#include "cs99xx_struct.h"

extern void dc_gr_into_test_relay_ready(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data);
extern void dc_gr_exit_test_relay_motion(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data);
extern void dc_gr_relay_ready(void);
extern void shift_dc_gr_cur_gear(uint8_t gear);

#endif //__dc_gr_RELAY_H__

