/*
 * Copyright(c) 2015,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�on_chip_flash_api.h
 * ժ  Ҫ  ��Ƭ��flash��̽ӿ�
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#ifndef __ON_CHIP_FLASH_H__
#define __ON_CHIP_FLASH_H__


#define F_PAGE_SIZE_I   (1024*2)      //Ƭ��flashҳ��С

#define TYPE_NUM_ADDR       0x0807F800  /* �ͺŵ�ַ */
#define SYS_LANGUAGE_ADDR   0x0807F000  //ϵͳ���� CHINESE,ENGLISH
#define EXCEP_INFO_ADDR     0x0807E800  //�쳣��Ϣ�洢λ��

#define TYPE_POOL_ADDR      0x0804ea48  //�ͺųص�ַ


#define MAX_EXCE_INFO_NUM   ((F_PAGE_SIZE_I - 2) / sizeof(EXCEP_INFO_STR))           /* ϵͳ֧�����3�������¼����ȡ��ֵ�� 1�� 2 ��3 */

extern uint16_t on_chip_flash_page_buf[F_PAGE_SIZE_I/2];

extern void erase_pages_flash(uint32_t addr, uint32_t pages);
extern void write_u16_flash(uint32_t addr, uint16_t *buf, uint16_t size);
extern void write_one_page_bin(uint16_t *buf,uint32_t addr);

#endif //__ON_CHIP_FLASH_H__

/******************* (C) COPYRIGHT 2015 ��ʢ���� *****END OF FILE****/
