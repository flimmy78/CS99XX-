
#include "acw_relay.h"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "serve_test.h"
#include "MC14094.h"
#include "app.h"

static void shift_acw_cur_gear(uint8_t gear)
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
void acw_into_test_relay_ready(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_OFF);/* 功放开关 */
    MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);/* 功放开关 */
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR 置1，DCW/IR置0 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);/* 切换高压与接地测试用的继电器 1 接地测试，0 ACW/DCW/IR */
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
    
    shift_acw_cur_gear(acw_par->gear_i);
}

void acw_exit_test_relay_motion(ACW_STRUCT *acw_par, TEST_DATA_STRUCT *test_data)
{
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_B, MC14094_AMP_RELAY1, RELAY_OFF);/* 功放开关 */
    MC14094_CMD(MC14094_B, MC14094_AMP_RELAY2, RELAY_OFF);/* 功放开关 */
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR 置1，DCW/IR置0 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);/* 切换高压与接地测试用的继电器 1 接地测试，0 ACW/DCW/IR */
    
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
    
    shift_acw_cur_gear(acw_par->gear_i);
}

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
