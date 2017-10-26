/*
 * Copyright(c) 2015,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_err_log.c
 * ժ  Ҫ  ��������־��صĽӿ�
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#define _99xxERR_LOG_GLOBALS
#include "stm32f10x.h"
#include "cs99xx_struct.h"
#include "on_chip_flash_api.h"
#include "stm32f10x_flash.h"
#include "RTC_Config.h"
#include "cs99xx_err_log.h"


/*
 * ��������updata_excep_info
 * ����  �������쳣��Ϣ
 * ����  ��p_excep ָ��Ҫд����쳣��Ϣ
 * ���  ����
 * ����  ����
 */
void updata_excep_info(EXCEP_INFO_STR *p_excep)
{
    uint32_t fpage = (EXCEP_INFO_ADDR - 0X08000000) / F_PAGE_SIZE_I;
    uint32_t i = 0;
    uint32_t n = 1024;
    uint16_t *total_exce = on_chip_flash_page_buf;
    uint8_t *p1 = NULL;
    uint8_t *p2 = NULL;
    uint32_t temp = 0;
    EXCEP_INFO_STR *p_e = NULL;
    
    /* ���������쳣��Ϣ */
    for(i = 0; i < n; i++)
    {
        on_chip_flash_page_buf[i] = ((uint16_t*)EXCEP_INFO_ADDR)[i];
    }
    
    /* �쳣��Ϣ���� �������ֵ������ */
    if(*total_exce >= MAX_EXCE_INFO_NUM)
    {
        *total_exce = 0;
    }
    
    p_e = ((EXCEP_INFO_STR *)&on_chip_flash_page_buf[1]) + *total_exce; /* ��λ���µ�λ�� */
    
    p_excep->excep_time = Time_GetTime();/* ��¼ϵͳʱ�� */
    
    temp = sizeof(EXCEP_INFO_STR);
    p1 = (uint8_t *)p_e;
    p2 = (uint8_t *)p_excep;
    
    for(i = 0; i < temp; i++)
    {
        p1[i] = p2[i];/* �����쳣��Ϣ�������� */
    }
    
    *total_exce = (*total_exce + 1) % MAX_EXCE_INFO_NUM;/* ���쳣������ */
    
    erase_pages_flash(fpage * F_PAGE_SIZE_I + 0X08000000, 1);/* ����flashҳ */
    
    write_one_page_bin(on_chip_flash_page_buf, EXCEP_INFO_ADDR);/* д��flash */
}

/*
 * ��������clear_excep_info
 * ����  ��������е��쳣��Ϣ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void clear_excep_info(void)
{
    uint32_t fpage = (EXCEP_INFO_ADDR - 0X08000000) / F_PAGE_SIZE_I;
    
    erase_pages_flash(fpage * F_PAGE_SIZE_I + 0X08000000, 1);
}

/******************* (C) COPYRIGHT 2015 ��ʢ���� *****END OF FILE****/
