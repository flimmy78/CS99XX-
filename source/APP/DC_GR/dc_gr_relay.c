
#include "dc_gr_relay.h"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "MC14094.h"

void dc_gr_into_test_relay_ready(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data)
{
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR ��1��DCW/IR��0 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* ��ѹ��Ե���Խ�ֱ���л� 1 ֱ��DCW/IR 0���� ACW/GR */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);/* �л���ѹ��ӵز����õļ̵��� 1 �ӵز��ԣ�0 ACW/DCW/IR */
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* �޷�������· ACW DCW IR Ϊ1; GRΪ0 */
    
//    shift_dc_gr_cur_gear(dc_gr_par->gear_i);
}

void dc_gr_exit_test_relay_motion(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data)
{
    irq_stop_relay_motion();//�ظ�ѹ
    
    MC14094_CMD(MC14094_C, MC14094_C_GR, RELAY_OFF);/* ACW DCW IR */
    MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !test_data->vol_segment);
    
    MC14094_CMD(MC14094_A, MC14094_CD4053_C, RELAY_ON);/* ACW/GR ��1��DCW/IR��0 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* ��ѹ��Ե���Խ�ֱ���л� 1 ֱ��DCW/IR 0���� ACW/GR */
    MC14094_CMD(MC14094_B, MC14094_GR_W, RELAY_OFF);/* �л���ѹ��ӵز����õļ̵��� 1 �ӵز��ԣ�0 ACW/DCW/IR */
    
    MC14094_CMD(MC14094_A, MC14094_PRO_CHANGE, RELAY_ON);/* �޷�������· ACW DCW IR Ϊ1; GRΪ0 */
    
//    shift_dc_gr_cur_gear(dc_gr_par->gear_i);
}


void dc_gr_relay_ready(void)
{
    MC14094_CMD(MC14094_A, MC14094_CD4053_B, RELAY_OFF);/* ��������ʱ��0 */
    MC14094_CMD(MC14094_A, MC14094_CD4053_A, RELAY_ON);/* ����ʱ��1 */
    MC14094_CMD(MC14094_B, MC14094_AC_DC, RELAY_OFF);/* ��ѹ��Ե���Խ�ֱ���л� 1 ֱ��DCW/IR 0���� ACW/GR */
    on_switch_hv();/* ����ѹ */
    MC14094_Updata();/* ��4094�������� ���¼̵���״̬ */
}

/******************* (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
