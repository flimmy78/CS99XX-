
#include "dcw_relay.h"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "MC14094.h"
#include "test_tools.h"

/**
  * @brief  切换DCW电流档位
  * @param  [in] gear 要切换的电流档位
  * @retval 无
  */
void shift_dcw_cur_gear(uint8_t gear)
{
    switch(gear)
    {
        case DC_2uA:
            MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);
            mc14094_set_gear(MC14094_C_2uA);
            break;
        case DC_20uA:
            MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);
            mc14094_set_gear(MC14094_C_20uA);
            break;
        case DC_200uA:
            MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);
            mc14094_set_gear(MC14094_C_200uA);
            break;
        case DC_2mA:
            MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_ON);
            MC14094_CMD(MC14094_B, MC14094_W_FILES, RELAY_ON);/* 2mA */
            break;
        case DC_10mA:
        case DC_20mA:
            MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_ON);
            MC14094_CMD(MC14094_B, MC14094_W_FILES, RELAY_OFF);/* 10mA */
            break;
        case DC_50mA:
        case DC_100mA:
            MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);
            mc14094_set_gear(MC14094_C_100mA);
            break;
    }
}
/**
  * @brief  DCW继电器进入测试就绪状态
  * @param  [in] dcw_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void dcw_into_test_relay_ready(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_OFF);/* ACW/GR 置1，DCW/IR置0 */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);
    
    MC14094_Updata();// 更新输出缓冲区
    cs_delay_ms(100);//直流放电100ms
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 复位状态下切换到交流 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
    
    shift_dcw_cur_gear(dcw_par->gear_i);
}

/**
  * @brief  DCW继电器退出测试就绪状态
  * @param  [in] dcw_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void dcw_exit_test_relay_motion(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
    irq_stop_relay_motion();//关高压
    
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_OFF);/* ACW/GR 置1，DCW/IR置0 */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);
    
    shift_dcw_cur_gear(DC_20mA);//切换到20mA档
    MC14094_Updata();// 更新输出缓冲区
    cs_delay_ms(100);//直流放电100ms
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 复位状态下切换到交流 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
    
    shift_dcw_cur_gear(dcw_par->gear_i);
}

/**
  * @brief  DCW测试电子开关打开就绪
  * @param  无
  * @retval 无
  */
void dcw_relay_ready(void)
{
    MC14094_CMD(MC14094_A, MC14094_CD4053_B, RELAY_OFF);/* 启动测试时置0 */
    MC14094_CMD(MC14094_A, MC14094_CD4053_A, RELAY_ON);/* 启动时置1 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_ON);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    on_switch_hv();/* 开高压 */
    MC14094_Updata();/* 向4094发送数据 更新继电器状态 */
}

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
