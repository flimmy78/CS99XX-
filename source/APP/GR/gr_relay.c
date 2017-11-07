
#include "gr_relay.h"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "MC14094.h"

/**
  * @brief  GR�̵���������Ծ���״̬
  * @param  [in] dcw_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void gr_into_test_relay_ready(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data)
{
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, RELAY_OFF);
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_OFF);/* �޷�������· ACW DCW IR Ϊ1; GRΪ0 */
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR/BBD ��1��DCW/IR��0 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* ��ѹ��Ե���Խ�ֱ���л� 1 ֱ��DCW/IR 0���� ACW/GR */
    MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);/* �л����ⲿ��չ����� */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_ON);
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_ON);/* �л���GR�����̵��� */
}

/**
  * @brief  GR�̵����˳����Ծ���״̬
  * @param  [in] dcw_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void gr_exit_test_relay_motion(GR_STRUCT *gr_par, TEST_DATA_STRUCT *test_data)
{
    irq_stop_relay_motion();//�ظ�ѹ
    
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, RELAY_OFF);
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_OFF);/* �޷�������· ACW DCW IR Ϊ1; GRΪ0 */
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR/BBD ��1��DCW/IR��0 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* ��ѹ��Ե���Խ�ֱ���л� 1 ֱ��DCW/IR 0���� ACW/GR */
    MC14094_CMD(MC14094_B, MC14094_W_IR, RELAY_OFF);/* �л����ⲿ��չ����� */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_ON);
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_ON);/* �л���GR�����̵��� */
}

/**
  * @brief  GR���Ե��ӿ��ش�
  * @param  ��
  * @retval ��
  */
void gr_relay_ready(void)
{
    MC14094_CMD(MC14094_A, MC14094_CD4053_B, RELAY_OFF);/* ��������ʱ��0 */
    MC14094_CMD(MC14094_A, MC14094_CD4053_A, RELAY_ON);/* ����ʱ��1 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* ��ѹ��Ե���Խ�ֱ���л� 1 ֱ��DCW/IR 0���� ACW/GR */
    on_switch_hv();/* ����ѹ */
    MC14094_Updata();/* ��4094�������� ���¼̵���״̬ */
}

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
