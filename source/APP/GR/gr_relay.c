
#include "gr_relay.h"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "MC14094.h"

/**
  * @brief  GR继电器进入测试就绪状态
  * @param  [in] dcw_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void gr_into_test_relay_ready(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data)
{
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, RELAY_OFF);
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_OFF);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR/BBD 置1，DCW/IR置0 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);/* 切换到外部扩展板采样 */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_ON);
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_ON);/* 切换到GR采样继电器 */
}

/**
  * @brief  GR继电器退出测试就绪状态
  * @param  [in] dcw_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void gr_exit_test_relay_motion(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data)
{
    irq_stop_relay_motion();//关高压
    
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, RELAY_OFF);
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_OFF);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR/BBD 置1，DCW/IR置0 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);/* 切换到外部扩展板采样 */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_ON);
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_ON);/* 切换到GR采样继电器 */
}

/**
  * @brief  GR测试电子开关打开
  * @param  无
  * @retval 无
  */
void gr_relay_ready(void)
{
    MC14094_CMD(MC14094_A, MC14094_CD4053_B, RELAY_OFF);/* 启动测试时置0 */
    MC14094_CMD(MC14094_A, MC14094_CD4053_A, RELAY_ON);/* 启动时置1 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    on_switch_hv();/* 开高压 */
    MC14094_Updata();/* 向4094发送数据 更新继电器状态 */
}

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
