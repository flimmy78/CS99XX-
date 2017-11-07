
#include "ir_relay.h"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "MC14094.h"

/**
  * @brief  IR�̵���������Ծ���״̬
  * @param  [in] dcw_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void ir_into_test_relay_ready(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR ��1��DCW/IR��0 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* ��ѹ��Ե���Խ�ֱ���л� 1 ֱ��DCW/IR 0���� ACW/GR */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);/* �л���ѹ��ӵز����õļ̵��� 1 �ӵز��ԣ�0 ACW/DCW/IR */
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* �޷�������· ACW DCW IR Ϊ1; GRΪ0 */
    
    ir_set_gear(test_data->cur_gear);
}

/**
  * @brief  IR�̵����˳����Ծ���״̬
  * @param  [in] dcw_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void ir_exit_test_relay_motion(IR_STRUCT *ir_par, TEST_DATA_STRUCT *test_data)
{
    irq_stop_relay_motion();//�ظ�ѹ
    
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_OFF);/* ACW/GR ��1��DCW/IR��0 */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);
    
    /******************************************/
    /* �л���20mA�����зŵ� */
    MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_ON);/* �л���2mA 20mA�ĵ�· */
    MC14094_CMD(MC14094_B, MC14094_W_FILES, RELAY_OFF);/* 20mA �� */
    /******************************************/
    MC14094_Updata();// �������������
    cs_delay_ms(100);//ֱ���ŵ�100ms
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* ��λ״̬���л������� ��ѹ��Ե���Խ�ֱ���л� 1 ֱ��DCW/IR 0���� ACW/GR */
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* �޷�������· ACW DCW IR Ϊ1; GRΪ0 */
    
    ir_set_gear(test_data->cur_gear);
}

/**
  * @brief  IR���Ե��ӿ��ش�
  * @param  ��
  * @retval ��
  */
void ir_relay_ready(void)
{
    MC14094_CMD(MC14094_A, MC14094_CD4053_B, RELAY_OFF);/* ��������ʱ��0 */
    MC14094_CMD(MC14094_A, MC14094_CD4053_A, RELAY_ON);/* ����ʱ��1 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_ON);/* ��ѹ��Ե���Խ�ֱ���л� 1 ֱ��DCW/IR 0���� ACW/GR */
    on_switch_hv();/* ����ѹ */
    MC14094_Updata();/* ��4094�������� ���¼̵���״̬ */
}

/**
  * @brief  �л�IR���赵λ
  * @param  [in] gear Ҫ�л��ĵ�����λ
  * @retval ��
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
  * @brief  �л�IR���赵λ
  * @param  [in] gear Ҫ�л��ĵ�����λ
  * @retval ��
  */
void ir_set_gear(const uint8_t gear)
{
    ir_set_gear_s(gear);
	
//	clear_slither_data();/* ������Ҫ����˲������� */
}

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
