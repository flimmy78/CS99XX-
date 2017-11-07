
#include "dc_gr_relay.h"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "MC14094.h"

void dc_gr_into_test_relay_ready(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data)
{
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR 置1，DCW/IR置0 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);/* 切换高压与接地测试用的继电器 1 接地测试，0 ACW/DCW/IR */
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
    
//    shift_dc_gr_cur_gear(dc_gr_par->gear_i);
}

void dc_gr_exit_test_relay_motion(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data)
{
    irq_stop_relay_motion();//关高压
    
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR 置1，DCW/IR置0 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);/* 切换高压与接地测试用的继电器 1 接地测试，0 ACW/DCW/IR */
    
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
    
//    shift_dc_gr_cur_gear(dc_gr_par->gear_i);
}


void dc_gr_relay_ready(void)
{
    MC14094_CMD(MC14094_A, MC14094_CD4053_B, RELAY_OFF);/* 启动测试时置0 */
    MC14094_CMD(MC14094_A, MC14094_CD4053_A, RELAY_ON);/* 启动时置1 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    on_switch_hv();/* 开高压 */
    MC14094_Updata();/* 向4094发送数据 更新继电器状态 */
}

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
