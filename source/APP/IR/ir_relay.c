
#include "ir_relay.h"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "MC14094.h"

/**
  * @brief  IR继电器进入测试就绪状态
  * @param  [in] dcw_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void ir_into_test_relay_ready(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR 置1，DCW/IR置0 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);/* 切换高压与接地测试用的继电器 1 接地测试，0 ACW/DCW/IR */
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
    
    ir_set_gear(test_data->cur_gear);
}

/**
  * @brief  IR继电器退出测试就绪状态
  * @param  [in] dcw_par 当前步的设置数据
  * @param  [in] test_data 测试数据
  * @retval 无
  */
void ir_exit_test_relay_motion(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    irq_stop_relay_motion();//关高压
    
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_OFF);/* ACW/GR 置1，DCW/IR置0 */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);
    
    /******************************************/
    /* 切换到20mA档进行放电 */
    MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_ON);/* 切换到2mA 20mA的电路 */
    MC14094_CMD(MC14094_B, MC14094_W_FILES, RELAY_OFF);/* 20mA 挡 */
    /******************************************/
    MC14094_Updata();// 更新输出缓冲区
    cs_delay_ms(100);//直流放电100ms
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* 复位状态下切换到交流 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* 限幅保护电路 ACW DCW IR 为1; GR为0 */
    
    ir_set_gear(test_data->cur_gear);
}

/**
  * @brief  IR测试电子开关打开
  * @param  无
  * @retval 无
  */
void ir_relay_ready(void)
{
    MC14094_CMD(MC14094_A, MC14094_CD4053_B, RELAY_OFF);/* 启动测试时置0 */
    MC14094_CMD(MC14094_A, MC14094_CD4053_A, RELAY_ON);/* 启动时置1 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_ON);/* 耐压绝缘测试交直流切换 1 直流DCW/IR 0交流 ACW/GR */
    on_switch_hv();/* 开高压 */
    MC14094_Updata();/* 向4094发送数据 更新继电器状态 */
}

/**
  * @brief  切换IR电阻档位
  * @param  [in] gear 要切换的电流档位
  * @retval 无
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

/**
  * @brief  切换IR电阻档位
  * @param  [in] gear 要切换的电流档位
  * @retval 无
  */
void ir_set_gear(const uint8_t gear)
{
    ir_set_gear_s(gear);
	
//	clear_slither_data();/* 换挡后要清空滤波缓冲区 */
}

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
