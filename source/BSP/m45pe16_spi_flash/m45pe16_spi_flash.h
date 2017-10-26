/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�m45pe16_spi_flash.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V1.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
 
#ifndef __M45PE16_SPI_FLASH_H__
#define __M45PE16_SPI_FLASH_H__

typedef struct
{
	uint32_t ChipID;		/* оƬID */
	char ChipName[16];		/* оƬ�ͺ��ַ�������Ҫ������ʾ */
	uint32_t TotalSize;		/* ������ */
	uint16_t PageSize;		/* ҳ���С */
}SFLASH_INFO;

extern void init_m45pe16_spi_flash(void);
extern void m45_read_buffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize);
extern void m45_page_erase(uint32_t _uiReadAddr, uint32_t _uiSize);
extern void m45_page_write(uint8_t * _pBuf, uint32_t _uiWriteAddr);
extern void m45_buffer_write(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);

#endif //__M45PE16_SPI_FLASH_H__

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
