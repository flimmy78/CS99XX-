
#include "dcw_relay.h"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "MC14094.h"
#include "test_tools.h"

/**
  * @brief  �л�DCW������λ
  * @param  [in] gear Ҫ�л��ĵ�����λ
  * @retval ��
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
  * @brief  DCW�̵���������Ծ���״̬
  * @param  [in] dcw_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void dcw_into_test_relay_ready(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_OFF);/* ACW/GR ��1��DCW/IR��0 */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);
    
    MC14094_Updata();// �������������
    cs_delay_ms(100);//ֱ���ŵ�100ms
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* ��λ״̬���л������� ��ѹ��Ե���Խ�ֱ���л� 1 ֱ��DCW/IR 0���� ACW/GR */
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* �޷�������· ACW DCW IR Ϊ1; GRΪ0 */
    
    shift_dcw_cur_gear(dcw_par->gear_i);
}

/**
  * @brief  DCW�̵����˳����Ծ���״̬
  * @param  [in] dcw_par ��ǰ������������
  * @param  [in] test_data ��������
  * @retval ��
  */
void dcw_exit_test_relay_motion(DCW_STRUCT *dcw_par, TEST_DATA_STRUCT *test_data)
{
    irq_stop_relay_motion();//�ظ�ѹ
    
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_OFF);/* ACW/GR ��1��DCW/IR��0 */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);
    
    shift_dcw_cur_gear(DC_20mA);//�л���20mA��
    MC14094_Updata();// �������������
    cs_delay_ms(100);//ֱ���ŵ�100ms
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* ��λ״̬���л������� ��ѹ��Ե���Խ�ֱ���л� 1 ֱ��DCW/IR 0���� ACW/GR */
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* �޷�������· ACW DCW IR Ϊ1; GRΪ0 */
    
    shift_dcw_cur_gear(dcw_par->gear_i);
}

/**
  * @brief  DCW���Ե��ӿ��ش򿪾���
  * @param  ��
  * @retval ��
  */
void dcw_relay_ready(void)
{
    MC14094_CMD(MC14094_A, MC14094_CD4053_B, RELAY_OFF);/* ��������ʱ��0 */
    MC14094_CMD(MC14094_A, MC14094_CD4053_A, RELAY_ON);/* ����ʱ��1 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_ON);/* ��ѹ��Ե���Խ�ֱ���л� 1 ֱ��DCW/IR 0���� ACW/GR */
    on_switch_hv();/* ����ѹ */
    MC14094_Updata();/* ��4094�������� ���¼̵���״̬ */
}

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
