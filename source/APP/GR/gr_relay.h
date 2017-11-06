
#ifndef __GR_RELAY_H__
#define __GR_RELAY_H__

#include "stm32f10x.h"
#include "cs99xx_struct.h"

extern void gr_into_test_relay_ready(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data);
extern void gr_exit_test_relay_motion(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data);
extern void gr_relay_ready(void);
extern void shift_gr_cur_gear(uint8_t gear);

#endif //__GR_RELAY_H__

