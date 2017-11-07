/*
 * Copyright(c) 2016,南京长盛仪器
 * All rights reserved
 * 文件名称：ir_shift_gear.h
 * 摘  要  ：主程序
 * 当前版本：V1.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#ifndef __IR_SHIFT_GEAR_H__
#define __IR_SHIFT_GEAR_H__

#include "cs99xx_struct.h"

extern void init_set_ir_gear(void);
extern void shift_gear_dly(void);
extern void ir_auto_find_gear(TEST_DATA_STRUCT *test_data);

#endif //__IR_SHIFT_GEAR_H__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
