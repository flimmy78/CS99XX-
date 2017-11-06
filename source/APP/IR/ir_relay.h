
#ifndef __ir_RELAY_H__
#define __ir_RELAY_H__

#include "stm32f10x.h"
#include "cs99xx_struct.h"

extern void ir_into_test_relay_ready(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data);
extern void ir_exit_test_relay_motion(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data);
extern void ir_relay_ready(void);
extern void shift_ir_cur_gear(uint8_t gear);

#endif //__ir_RELAY_H__

