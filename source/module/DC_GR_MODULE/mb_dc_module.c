
#include "stm32f10x.h"
#include "mb_dc_module.h"
#include "module.h"
#include "string.h"
#include "dc_module.h"
#include "calibration.h"
#include "mb_cmd_define.h"
#include "cs99xx_struct.h"


/*
 * ��������m_dc_module_cal_data_dispose
 * ����  ��ֱ��ģ��У׼ϵ��ͨѶ����
 * ����  ��slove_num �ӻ���ַ
 *         cmd ��д����
 *         data ����
 * ���  ����
 * ����  ����
 */
void m_dc_module_cal_data_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
    uint8_t n = 0;
    
    if(-1 == find_dc_gr_map_ratio_n(slove_num, &n))
    {
        return;
    }
    
    if(cmd == MB_READ_PAR)
    {
        memcpy(&ratio_dc_gr[0][n], data, sizeof (RATIO_DC_GR));
    }
    else
    {
//         memcpy(data, &ratio_dc_gr[0][n], sizeof (RATIO_DC_GR));
    }
}
/*
 * ��������m_dc_module_dac_dispose
 * ����  ������ֱ��ģ�����DAͨѶ�������
 * ����  ��slove_num �ӻ���ַ
 *         cmd ��д����
 *         data ����
 * ���  ����
 * ����  ����
 */
void m_dc_module_dac_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
}
/*
 * ��������m_dc_module_start_dispose
 * ����  ������ֱ��ģ��ָ��ķ��ش�����
 * ����  ��slove_num �ӻ���ַ
 *         cmd ��д����
 *         data ����
 * ���  ����
 * ����  ����
 */
void m_dc_module_start_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
    dc_gr_master_info[slove_num].st = 1;
}
/*
 * ��������m_dc_module_stop_dispose
 * ����  ��ֱֹͣ��ģ��ָ��ķ��ش�����
 * ����  ��slove_num �ӻ���ַ
 *         cmd ��д����
 *         data ����
 * ���  ����
 * ����  ����
 */
void m_dc_module_stop_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
    dc_gr_master_info[slove_num].st = 0;
}
/*
 * ��������m_dc_module_test_fetch_dispose
 * ����  ��ѯ��ֱ��ģ��������ݵķ��ش�����
 * ����  ��slove_num �ӻ���ַ
 *         cmd ��д����
 *         data ����
 * ���  ����
 * ����  ����
 */
#include "STM32_GPIO_Config.h"
#include "serve_test.h"
void m_dc_module_test_fetch_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
    uint32_t i = 0;
    uint32_t c = sizeof(MODULE_DATA);
    uint8_t *p1 = (void*)&dc_gr_master_info[slove_num].data;
    uint8_t *p2 = (void*)data;
    
    for(i = 0; i < c; i++)
    {
        p1[i] = p2[i];
    }
    dc_gr_master_info[slove_num].usable = 1;
}
void m_dc_module_test_fetch_short_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
    uint32_t i = 0;
    uint32_t c = sizeof(MODULE_DATA_SHORT);
    uint8_t *p1 = (void*)&dc_gr_master_info[slove_num].short_data;
    uint8_t *p2 = (void*)data;
	
    for(i = 0; i < c; i++)
    {
        p1[i] = p2[i];
    }
    dc_gr_master_info[slove_num].usable = 1;
}
/*
 * ��������m_dc_module_vol_gear_dispose
 * ����  ������ֱ��ģ�鵵λָ��ķ��ش�����
 * ����  ��slove_num �ӻ���ַ
 *         cmd ��д����
 *         data ����
 * ���  ����
 * ����  ����
 */
void m_dc_module_vol_gear_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
}
/*
 * ��������m_dc_module_test_method_dispose
 * ����  ������ֱ��ģ����Է���ָ��ķ��ش�����
 * ����  ��slove_num �ӻ���ַ
 *         cmd ��д����
 *         data ����
 * ���  ����
 * ����  ����
 */
void m_dc_module_test_method_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
}

void m_dc_module_set_data_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
//     memcpy(&dc_gr_master_info[slove_num].id, data, sizeof (DC_GR_PARAMETER));
//     dc_gr_master_info[slove_num].en = 1;
//     select_dc_gr_moduls(DC_GR_SEL_ALL,0);
}
/*
 * ��������m_dc_module_info_dispose
 * ����  ����ѯֱ��ģ����Ϣָ��ķ��ش�����
 * ����  ��slove_num �ӻ���ַ
 *         cmd ��д����
 *         data ����
 * ���  ����
 * ����  ����
 */
void m_dc_module_info_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data)
{
    memcpy(&dc_gr_master_info[slove_num].id, data, sizeof (MODULE_INFO));
    dc_gr_master_info[slove_num].en = 1;
    select_dc_gr_moduls(DC_GR_SEL_ALL,0);
}



/******************* (C) COPYRIGHT 2016 ��ʢ���� *****END OF FILE*******************/

