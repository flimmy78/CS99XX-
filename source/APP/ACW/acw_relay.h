
#ifndef __ACW_RELAY_H__
#define __ACW_RELAY_H__

#include "stm32f10x.h"
#include "cs99xx_struct.h"

extern void acw_into_test_relay_ready(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data);
extern void acw_exit_test_relay_motion(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data);

#endif //__ACW_RELAY_H__

