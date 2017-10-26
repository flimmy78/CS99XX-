/*
 * Copyright(c) 2015,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�on_chip_flash_api.c
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#include "stm32f10x.h"
#include "cs99xx_struct.h"
#include "on_chip_flash_api.h"
#include "stm32f10x_flash.h"
#include "RTC_Config.h"


uint16_t on_chip_flash_page_buf[F_PAGE_SIZE_I/2];

/*
 * ��������erase_pages_flash
 * ����  �������ڲ�flashָ����ҳ
 * ����  ��addr Ҫ������ҳ��ַ
 *         pages  Ҫ������ҳ��
 * ���  ����
 * ����  ����
 */
void erase_pages_flash(uint32_t addr, uint32_t pages)
{
    FLASH_Status FLASHStatus = FLASH_COMPLETE;
    uint32_t i = 0;
    
    FLASH_Unlock();               //����FLASH�������FLASH��д����
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR |FLASH_FLAG_WRPRTERR);
    
    for (i = 0; (i < pages) && (FLASHStatus == FLASH_COMPLETE); i++)
    {
        while(FLASH_GetFlagStatus(FLASH_FLAG_BSY));
        FLASHStatus = FLASH_ErasePage(addr + (F_PAGE_SIZE_I * i));
    }
    
    FLASH_Lock();
}

/*
 * ��������write_u16_flash
 * ����  ���԰��ֵķ�ʽ��Ƭ��flashд�����ɸ�����
 * ����  ��addr Ҫд��ĵ�ַ
 *         buf  ���ݻ���
 *         size ���ݸ���
 * ���  ����
 * ����  ����
 */
void write_u16_flash(uint32_t addr, uint16_t *buf, uint16_t size)
{
    int32_t i = 0;
    
    FLASH_Unlock();
    
    for(i = 0; i < size; i++)
    {
        FLASH_ProgramHalfWord(addr + i * 2, buf[i]);
    }
    
    FLASH_Lock();
}

/*
 * ��������updata_sys_language_par
 * ����  ����Ƭ��flash����������Ϣ����bootloader����ʹ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void updata_sys_language_par(void)
{
    uint16_t langu_t = *(uint16_t*)(SYS_LANGUAGE_ADDR);
    uint32_t fpage = (SYS_LANGUAGE_ADDR - 0X08000000) / F_PAGE_SIZE_I;
    
    if(langu_t == sys_par.language)
    {
        return;
    }
    
    langu_t = sys_par.language;
    
    erase_pages_flash(fpage * F_PAGE_SIZE_I + 0X08000000, 1);
    
    write_u16_flash(SYS_LANGUAGE_ADDR, &langu_t, 1);
}


/*
 * ��������write_one_page_bin
 * ����  ����Ƭ��flashд��1ҳ����
 * ����  ��buf ���ݻ��� addr Ҫд��flash�ĵ�ַ
 * ���  ����
 * ����  ����
 */
void write_one_page_bin(uint16_t *buf,uint32_t addr)
{
    int32_t i = 0;
    uint32_t temp = F_PAGE_SIZE_I / 2;
    
    FLASH_Unlock();
    
    for(i = 0; i < temp; i++)
    {
        FLASH_ProgramHalfWord(addr + i * 2, buf[i]);
    }
    
    FLASH_Lock();
}

/******************* (C) COPYRIGHT 2015 ��ʢ���� *****END OF FILE****/

