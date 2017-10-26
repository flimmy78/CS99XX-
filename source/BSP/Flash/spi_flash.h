/*
 * Copyright(c) 2014,
 * All rights reserved
 * �ļ����ƣ�spi_flash.H
 * ժ  Ҫ  ��ͷ�ļ�
 * ��ǰ�汾��V1.0����д
 * �޸ļ�¼��
 *
 */
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#if defined(__cplusplus)
    extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#include "stm32f10x.h"
#include "string.h"

/******************************* �ṹ����� ***********************************/
struct sflash_head
{
	uint32_t	bkp;
	uint32_t	list;
	uint32_t	null_list;
};

typedef struct
{
	uint32_t ChipID;		/* оƬID */
	char ChipName[16];		/* оƬ�ͺ��ַ�������Ҫ������ʾ */
	uint32_t TotalSize;		/* ������ */
	uint16_t PageSize;		/* ҳ���С */
}SFLASH_T;

/********** ȫ�ֱ��� *********************/
// extern uint8_t s_spiBuf[4*1024];
/******************* �������� *********************/
void flash_lock(void);
void flash_unlock(void);
		
void spi_flash_init(void);

void FLASH_CS_SET(char n);
uint32_t sf_ReadID(void);
void sf_EraseChip(void);
void sf_EraseSector(uint32_t _uiSectorAddr);
void sf_PageWrite(uint8_t * _pBuf, uint32_t _uiWriteAddr, uint16_t _usSize);
uint8_t sf_WriteBuffer(uint8_t* _pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize);
void sf_ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize);

extern SFLASH_T g_tSF;


/********************** �ⲿ�õ��ı��� **************************/

		

/**************************************************/
#if defined(__cplusplus)
    }
#endif 
/******************************************************************************
 *                             END  OF  FILE                                                                          
******************************************************************************/
#endif
