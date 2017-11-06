
#include "ir_relay.h"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "MC14094.h"

void shift_ir_cur_gear(uint8_t gear)
{
    switch(gear)
    {
        case AC_2uA:
            mc14094_set_gear(MC14094_C_2uA);
            MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);/* 切换到100mA 200uA 20uA 2uA 电路 */
            break;
        case AC_20uA:
            mc14094_set_gear(MC14094_C_20uA);
            MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);/* 切换到100mA 200uA 20uA 2uA 电路 */
            break;
        case AC_200uA:
            mc14094_set_gear(MC14094_C_200uA);
            MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);/* 切换到100mA 200uA 20uA 2uA 电路 */
            break;
        case AC_2mA:
            MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_ON);/* 切换到2mA 20mA的电路 */
            MC14094_CMD(MC14094_B, MC14094_W_FILES, RELAY_ON);/* 2mA 挡 */
            break;
        case AC_10mA:
        case AC_20mA:
            MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_ON);/* 切换到2mA 20mA的电路 */
            MC14094_CMD(MC14094_B, MC14094_W_FILES, RELAY_OFF);/* 20mA 挡 */
            break;
        case AC_50mA:
        case AC_100mA:
        case AC_200mA:
            MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);/* 切换到100mA 200uA 20uA 2uA 电路 */
            mc14094_set_gear(MC14094_C_100mA);
            break;
    }
}
void ir_into_test_relay_ready(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR 置1，DCW/IR置0 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);/* 切换高压与接地测试用的继电器 1 接地测试，0 ACW/DCW/IR */
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
    
//    shift_ir_cur_gear(ir_par->gear_i);
}

void ir_exit_test_relay_motion(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    irq_stop_relay_motion();//关高压
    
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR 置1，DCW/IR置0 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);/* 切换高压与接地测试用的继电器 1 接地测试，0 ACW/DCW/IR */
    
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
    
//    shift_ir_cur_gear(ir_par->gear_i);
}


void ir_relay_ready(void)
{
    MC14094_CMD(MC14094_A, MC14094_CD4053_B, RELAY_OFF);/* 启动测试时置0 */
    MC14094_CMD(MC14094_A, MC14094_CD4053_A, RELAY_ON);/* 启动时置1 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    on_switch_hv();/* 开高压 */
    MC14094_Updata();/* 向4094发送数据 更新继电器状态 */
}

/*
 * 函数名：ir_set_gear
 * 描述  ：ir档位设置
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */

void ir_set_gear_s(const uint8_t gear)
{
	switch(gear)
	{
		case IR_10MOHM:
			mc14094_set_gear(MC14094_C_10M);
			break;
		case IR_100MOHM:
			mc14094_set_gear(MC14094_C_100M);
			break;
		case IR_1GOHM:
			mc14094_set_gear(MC14094_C_1G);
			break;
		case IR_10GOHM:
			mc14094_set_gear(MC14094_C_10G);
			break;
		case IR_100GOHM:
			mc14094_set_gear(MC14094_C_100G);
			break;
	}
}

void ir_set_gear(const uint8_t gear)
{
    ir_set_gear_s(gear);
	
	clear_slither_data();/* 换挡后要清空滤波缓冲区 */
    
//    cur_gear_comm = gear;
}

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
