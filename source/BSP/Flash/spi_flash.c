/*
 * Copyright(c) 2014,
 * All rights reserved
 * �ļ����ƣ�spi_flash.C
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V1.0����д
 * �޸ļ�¼��
 */

/******************* �����ļ� *********************/
#include    "spi_flash.h"
#include    "STM32_GPIO_Config.h"
#include	"BSP_config.h"

#define			FLASH_CS1_EN()		chipselect=1
#define			FLASH_CS2_EN()		chipselect=2
#define			FLASH_CS3_EN()		chipselect=3

#define			FLASH_WP_EN()		GPIO_ResetBits(GPIOF    ,GPIO_Pin_5)
#define			FLASH_WP_DIS()		GPIO_SetBits(GPIOF      ,GPIO_Pin_5)

#define			FLASH_CS1_DIS()		GPIO_SetBits(GPIOF      ,GPIO_Pin_3)
#define			FLASH_CS2_DIS()		GPIO_SetBits(GPIOF      ,GPIO_Pin_6)
#define			FLASH_CS3_DIS()		GPIO_SetBits(GPIOF      ,GPIO_Pin_7)

#define			FLASH_DI_HIGH()		GPIO_SetBits(GPIOF      ,GPIO_Pin_9)
#define			FLASH_DI_LOW()		GPIO_ResetBits(GPIOF    ,GPIO_Pin_9)

#define			FLASH_CLK_HIGH()	GPIO_SetBits(GPIOF      ,GPIO_Pin_8)
#define			FLASH_CLK_LOW()		GPIO_ResetBits(GPIOF    ,GPIO_Pin_8)

#define			FLASH_DI()			GPIO_ReadInputDataBit(GPIOF  ,GPIO_Pin_4)

// #define			FLASH_DI()			GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)
#define CMD_AAI       0xAD  	/* AAI �������ָ��(FOR SST25VF016B) */
#define CMD_DISWR	  0x04		/* ��ֹд, �˳�AAI״̬ */
#define CMD_EWRSR	  0x50		/* ����д״̬�Ĵ��������� */
#define CMD_WRSR      0x01  	/* д״̬�Ĵ������� */
#define CMD_WREN      0x06		/* дʹ������ */
#define CMD_READ      0x03  	/* ������������ */
#define CMD_RDSR      0x05		/* ��״̬�Ĵ������� */
#define CMD_RDID      0x9F		/* ������ID���� */
#define CMD_SE        0x20		/* ������������ */
#define CMD_BE        0xC7		/* ������������ */
#define DUMMY_BYTE    0xA5		/* ���������Ϊ����ֵ�����ڶ����� */

#define WIP_FLAG      0x01		/* ״̬�Ĵ����е����ڱ�̱�־��WIP) */

SFLASH_T    g_tSF;
char		chipselect=1;
// uint8_t s_spiBuf[4 * 1024];	/* ����д�������ȶ�������page���޸Ļ�������������page��д */
static uint8_t spi_flash_buf[4 * 1024];
static void sf_WaitForWriteEnd(void);
static void FLASH_CS_EN(void);
static void FLASH_CS_DIS(void);
static void spi_gpio_init(void);
static void sf_WriteEnable(void);
static uint8_t sf_AutoWritePage(uint8_t *_ucpSrc, uint32_t _uiWrAddr, uint16_t _usWrLen);

// #include <stdlib.h>
// uint8_t *malloc_spi_buf(void)
// {
//     uint8_t *p = NULL;
//     
//     p = malloc(4*1024);
//     
//     return p;
// }

// void free_spi_buf(uint8_t *p)
// {
//     free(p);
// }

void FLASH_CS_SET(char n)
{
	switch(n)
	{
		case 1:
			FLASH_CS1_EN();
			break;
		case 2:
			FLASH_CS2_EN();
			break;
		case 3:
			FLASH_CS2_EN();
			break;
	}
}

static void FLASH_CS_EN(void)
{
	switch(chipselect)
	{
		case 1:
			GPIO_ResetBits(GPIOF,   GPIO_Pin_3);
			break;
		case 2:
			GPIO_ResetBits(GPIOF,   GPIO_Pin_6);
			break;
		case 3:
			GPIO_ResetBits(GPIOF,   GPIO_Pin_7);
			break;
	}
}

static void FLASH_CS_DIS(void)
{
	FLASH_CS1_DIS();
	FLASH_CS2_DIS();
	FLASH_CS3_DIS();
}

/*
 * ��������spi_gpio_init
 * ����  ����ʼ���ܽ�
 * ����  ����
 * ���  ����
 */
static void spi_gpio_init(void)
{
	/*************************    �����ʼ���ṹ����    *************************/
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/*************************    ��������ʱ��    *************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);
    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /*********************************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			 //��������
	
    /* SDO MDI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    /*********************************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //�������
    
	/* SDI MDO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    /* WP */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    /* CLK */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    /* CS1 CS2 CS3 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
    
	FLASH_CS_DIS();
}

/*
 * ��������spi_flash_init
 * ����  ����ʼ��flash
 * ����  ����
 * ���  ����
 */
void spi_flash_init(void)
{
	spi_gpio_init();
    chipselect=1;
	flash_unlock();
	
	g_tSF.ChipID = sf_ReadID();
    
	if(g_tSF.ChipID == 0xEF4018)
	{
		strcpy(g_tSF.ChipName, "W25Q128BV");
		g_tSF.TotalSize = 16 * 1024 * 1024;	/* ������ = 16M */
		g_tSF.PageSize = 4 * 1024;			/* ҳ���С = 4K */
	}
	
	flash_lock();
    
    chipselect=2;
	flash_unlock();
	
	g_tSF.ChipID = sf_ReadID();
    
	if(g_tSF.ChipID == 0xEF4018)
	{
		strcpy(g_tSF.ChipName, "W25Q128BV");
		g_tSF.TotalSize = 16 * 1024 * 1024;	/* ������ = 16M */
		g_tSF.PageSize = 4 * 1024;			/* ҳ���С = 4K */
	}
	
    chipselect=3;
	flash_unlock();
	
	g_tSF.ChipID = sf_ReadID();
    
	if(g_tSF.ChipID == 0xEF4018)
	{
		strcpy(g_tSF.ChipName, "W25Q128BV");
		g_tSF.TotalSize = 16 * 1024 * 1024;	/* ������ = 16M */
		g_tSF.PageSize = 4 * 1024;			/* ҳ���С = 4K */
	}
	flash_lock();
}

/*
 * ��������sf_SendByte
 * ����  ����flashд���������һ�ֽ�
 * ����  ������
 * ���  ������ֵ
 */
int sf_SendByte(int cmd)
{
    #define  DELAY_SF()   //{int j; for(j = 0; j < 1000; j++);}//soft_delay_us(1);
	int result=0;
	int i;
	int pos=0x80;
	
	for(i=0;i<8;i++)
	{
		FLASH_CLK_LOW();
        DELAY_SF();
		if(cmd & pos)
        {
            FLASH_DI_HIGH();
        }
		else
        {
            FLASH_DI_LOW();
        }
        
		pos >>= 1;
		result <<= 1;
		FLASH_CLK_HIGH();
        DELAY_SF();
		result |= FLASH_DI();
	}
	
	return result;
}

void flash_lock(void)
{
	FLASH_WP_EN();
}

void flash_unlock(void)
{
	FLASH_WP_DIS();
}


/*
*********************************************************************************************************
*	�� �� ��: sf_ReadID
*	����˵��: ��ȡ����ID
*	��    ��:  ��
*	�� �� ֵ: 32bit������ID (���8bit��0����ЧIDλ��Ϊ24bit��
*********************************************************************************************************
*/
uint32_t sf_ReadID(void)
{
	uint32_t uiID;
	uint8_t id1, id2, id3;
	
	FLASH_CS_EN();
	sf_SendByte(CMD_RDID);								/* ���Ͷ�ID���� */
	id1 = sf_SendByte(DUMMY_BYTE);					/* ��ID�ĵ�1���ֽ� */
	id2 = sf_SendByte(DUMMY_BYTE);					/* ��ID�ĵ�2���ֽ� */
	id3 = sf_SendByte(DUMMY_BYTE);					/* ��ID�ĵ�3���ֽ� */
	FLASH_CS_DIS();
	
	uiID = ((uint32_t)id1 << 16) | ((uint32_t)id2 << 8) | id3;
    
	return uiID;
}

/*
*********************************************************************************************************
*	�� �� ��: sf_WriteEnable
*	����˵��: ����������дʹ������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void sf_WriteEnable(void)
{
	FLASH_CS_EN();									/* ʹ��Ƭѡ */
	sf_SendByte(CMD_WREN);								/* �������� */
	FLASH_CS_DIS();									/* ����Ƭѡ */
}

/*
*********************************************************************************************************
*	�� �� ��: sf_EraseChip
*	����˵��: ��������оƬ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sf_EraseChip(void)
{
	sf_WriteEnable();								/* ����дʹ������ */
    
	/* ������������ */
	FLASH_CS_EN();									/* ʹ��Ƭѡ */
	sf_SendByte(CMD_BE);							/* ������Ƭ�������� */
	FLASH_CS_DIS();									/* ����Ƭѡ */
    
	sf_WaitForWriteEnd();							/* �ȴ�����Flash�ڲ�д������� */
}

/*
*********************************************************************************************************
*	�� �� ��: sf_EraseSector
*	����˵��: ����ָ��������
*	��    ��:  _uiSectorAddr : ������ַ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sf_EraseSector(uint32_t _uiSectorAddr)
{
	sf_WriteEnable();								/* ����дʹ������ */
    
	/* ������������ */
	FLASH_CS_EN();									/* ʹ��Ƭѡ */
	sf_SendByte(CMD_SE);								/* ���Ͳ������� */
	sf_SendByte((_uiSectorAddr & 0xFF0000) >> 16);	/* ����������ַ�ĸ�8bit */
	sf_SendByte((_uiSectorAddr & 0xFF00) >> 8);		/* ����������ַ�м�8bit */
	sf_SendByte(_uiSectorAddr & 0xFF);				/* ����������ַ��8bit */
	FLASH_CS_DIS();									/* ����Ƭѡ */
    
	sf_WaitForWriteEnd();							/* �ȴ�����Flash�ڲ�д������� */
}

/*
*********************************************************************************************************
*	�� �� ��: sf_PageWrite
*	����˵��: ��һ��page��д�������ֽڡ��ֽڸ������ܳ���ҳ���С��4K)
*	��    ��:  	_pBuf : ����Դ��������
*				_uiWriteAddr ��Ŀ�������׵�ַ
*				_usSize �����ݸ��������ܳ���ҳ���С
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sf_PageWrite(uint8_t * _pBuf, uint32_t _uiWriteAddr, uint16_t _usSize)
{
	uint32_t i, j;
    
	for (j = 0; j < _usSize / 256; j++)
	{
		sf_WriteEnable();								/* ����дʹ������ */
        
		FLASH_CS_EN();									/* ʹ��Ƭѡ */
		sf_SendByte(0x02);								/* ����AAI����(��ַ�Զ����ӱ��) */
		sf_SendByte((_uiWriteAddr & 0xFF0000) >> 16);	/* ����������ַ�ĸ�8bit */
		sf_SendByte((_uiWriteAddr & 0xFF00) >> 8);		/* ����������ַ�м�8bit */
		sf_SendByte(_uiWriteAddr & 0xFF);				/* ����������ַ��8bit */
        
		for (i = 0; i < 256; i++)
		{
			sf_SendByte(*_pBuf++);					/* �������� */
		}
        
		FLASH_CS_DIS();								/* ��ֹƬѡ */
        
		sf_WaitForWriteEnd();						/* �ȴ�����Flash�ڲ�д������� */
        
		_uiWriteAddr += 256;
	}
    
	/* ����д����״̬ */
	FLASH_CS_EN();
	sf_SendByte(CMD_DISWR);
	FLASH_CS_DIS();
    
	sf_WaitForWriteEnd();							/* �ȴ�����Flash�ڲ�д������� */
}

/*
*********************************************************************************************************
*	�� �� ��: sf_NeedErase
*	����˵��: �ж�дPAGEǰ�Ƿ���Ҫ�Ȳ�����
*	��    ��:   _ucpOldBuf �� ������
*			   _ucpNewBuf �� ������
*			   _uiLen �����ݸ��������ܳ���ҳ���С
*	�� �� ֵ: 0 : ����Ҫ������ 1 ����Ҫ����
*********************************************************************************************************
*/
static uint8_t sf_NeedErase(uint8_t * _ucpOldBuf, uint8_t *_ucpNewBuf, uint16_t _usLen)
{
	uint16_t i;
	uint8_t ucOld;
    
	/*
	�㷨��1����old ��, new ����
	      old    new
		  1101   0101
	~     1111
		= 0010   0101

	�㷨��2��: old �󷴵Ľ���� new λ��
		  0010   old
	&	  0101   new
		 =0000

	�㷨��3��: ���Ϊ0,���ʾ�������. �����ʾ��Ҫ����
	*/
    
	for (i = 0; i < _usLen; i++)
	{
		ucOld = *_ucpOldBuf++;
		ucOld = ~ucOld;
        
		/* ע������д��: if (ucOld & (*_ucpNewBuf++) != 0) */
		if ((ucOld & (*_ucpNewBuf++)) != 0)
		{
			return 1;
		}
	}
    
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: sf_CmpData
*	����˵��: �Ƚ�Flash������.
*	��    ��:  	_ucpTar : ���ݻ�����
*				_uiSrcAddr ��Flash��ַ
*				_uiSize �����ݸ���, ���Դ���PAGE_SIZE,���ǲ��ܳ���оƬ������
*	�� �� ֵ: 0 = ���, 1 = ����
*********************************************************************************************************
*/
static uint8_t sf_CmpData(uint32_t _uiSrcAddr, uint8_t *_ucpTar, uint32_t _uiSize)
{
	uint8_t ucValue;
    
	/* �����ȡ�����ݳ���Ϊ0���߳�������Flash��ַ�ռ䣬��ֱ�ӷ��� */
	if ((_uiSrcAddr + _uiSize) > g_tSF.TotalSize)
	{
		return 1;
	}
    
	if (_uiSize == 0)
	{
		return 0;
	}
    
	FLASH_CS_EN();									/* ʹ��Ƭѡ */
	sf_SendByte(CMD_READ);							/* ���Ͷ����� */
	sf_SendByte((_uiSrcAddr & 0xFF0000) >> 16);		/* ����������ַ�ĸ�8bit */
	sf_SendByte((_uiSrcAddr & 0xFF00) >> 8);		/* ����������ַ�м�8bit */
	sf_SendByte(_uiSrcAddr & 0xFF);					/* ����������ַ��8bit */
    
	while (_uiSize--)
	{
		/* ��һ���ֽ� */
		ucValue = sf_SendByte(DUMMY_BYTE);
		if (*_ucpTar++ != ucValue)
		{
			FLASH_CS_DIS();
			return 1;
		}
	}
    
	FLASH_CS_DIS();
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: sf_AutoWritePage
*	����˵��: д1��PAGE��У��,�������ȷ������д���Ρ��������Զ���ɲ���������
*	��    ��:  	_pBuf : ����Դ��������
*				_uiWriteAddr ��Ŀ�������׵�ַ
*				_usSize �����ݸ��������ܳ���ҳ���С
*	�� �� ֵ: 0 : ���� 1 �� �ɹ�
*********************************************************************************************************
*/
static uint8_t sf_AutoWritePage(uint8_t *_ucpSrc, uint32_t _uiWrAddr, uint16_t _usWrLen)
{
	uint16_t i;
	uint16_t j;					/* ������ʱ */
	uint32_t uiFirstAddr;		/* ������ַ */
	uint8_t ucNeedErase;		/* 1��ʾ��Ҫ���� */
	uint8_t cRet;
    
	/* ����Ϊ0ʱ����������,ֱ����Ϊ�ɹ� */
	if (_usWrLen == 0)
	{
		return 1;
	}
    
	/* ���ƫ�Ƶ�ַ����оƬ�������˳� */
	if (_uiWrAddr >= g_tSF.TotalSize)
	{
		return 0;
	}
    
	/* ������ݳ��ȴ����������������˳� */
	if (_usWrLen > g_tSF.PageSize)
	{
		return 0;
	}
    
	/* ���FLASH�е�����û�б仯,��дFLASH */
	sf_ReadBuffer(spi_flash_buf, _uiWrAddr, _usWrLen);
	if (memcmp(spi_flash_buf, _ucpSrc, _usWrLen) == 0)
	{
		return 1;
	}
    
	/* �ж��Ƿ���Ҫ�Ȳ������� */
	/* ����������޸�Ϊ�����ݣ�����λ���� 1->0 ���� 0->0, ���������,���Flash���� */
	ucNeedErase = 0;
	if (sf_NeedErase(spi_flash_buf, _ucpSrc, _usWrLen))
	{
		ucNeedErase = 1;
	}
    
	uiFirstAddr = _uiWrAddr & (~(g_tSF.PageSize - 1));
    
    /* ������������д */
	if (_usWrLen == g_tSF.PageSize)
	{
		for	(i = 0; i < g_tSF.PageSize; i++)
		{
			spi_flash_buf[i] = _ucpSrc[i];
		}
	}
    /* ��д�������� */
	else
	{
		/* �Ƚ��������������ݶ��� */
		sf_ReadBuffer(spi_flash_buf, uiFirstAddr, g_tSF.PageSize);
        
		/* ���������ݸ��� */
		i = _uiWrAddr & (g_tSF.PageSize - 1);
		memcpy(&spi_flash_buf[i], _ucpSrc, _usWrLen);
	}
    
	/* д��֮�����У�飬�������ȷ����д�����3�� */
	cRet = 0;
	for (i = 0; i < 3; i++)
	{
		/* ����������޸�Ϊ�����ݣ�����λ���� 1->0 ���� 0->0, ���������,���Flash���� */
		if (ucNeedErase == 1)
		{
			sf_EraseSector(uiFirstAddr);		/* ����1������ */
		}
        
		/* ���һ��PAGE */
		sf_PageWrite(spi_flash_buf, uiFirstAddr, g_tSF.PageSize);

		if (sf_CmpData(_uiWrAddr, _ucpSrc, _usWrLen) == 0)
		{
			cRet = 1;
			break;
		}
		else
		{
			if (sf_CmpData(_uiWrAddr, _ucpSrc, _usWrLen) == 0)
			{
				cRet = 1;
				break;
			}
            
			/* ʧ�ܺ��ӳ�һ��ʱ�������� */
			for (j = 0; j < 10000; j++);
		}
	}
    
	return cRet;
}

/*
*********************************************************************************************************
*	�� �� ��: sf_WriteBuffer
*	����˵��: д1��������У��,�������ȷ������д���Ρ��������Զ���ɲ���������
*	��    ��:  	_pBuf : ����Դ��������
*				_uiWrAddr ��Ŀ�������׵�ַ
*				_usSize �����ݸ��������ܳ���ҳ���С
*	�� �� ֵ: 1 : �ɹ��� 0 �� ʧ��
*********************************************************************************************************
*/
uint8_t sf_WriteBuffer(uint8_t* _pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize)
{
	uint16_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
    
	Addr = _uiWriteAddr % g_tSF.PageSize;
	count = g_tSF.PageSize - Addr;
	NumOfPage =  _usWriteSize / g_tSF.PageSize;
	NumOfSingle = _usWriteSize % g_tSF.PageSize;
    
	if (Addr == 0) /* ��ʼ��ַ��ҳ���׵�ַ  */
	{
        /* ���ݳ���С��ҳ���С */
		if (NumOfPage == 0)
		{
			if (sf_AutoWritePage(_pBuf, _uiWriteAddr, _usWriteSize) == 0)
			{
				return 0;
			}
		}
        /* ���ݳ��ȴ��ڵ���ҳ���С */
		else
		{
			while (NumOfPage--)
			{
				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, g_tSF.PageSize) == 0)
				{
					return 0;
				}
				_uiWriteAddr +=  g_tSF.PageSize;
				_pBuf += g_tSF.PageSize;
			}
            
			if (sf_AutoWritePage(_pBuf, _uiWriteAddr, NumOfSingle) == 0)
			{
				return 0;
			}
		}
	}
    /* ��ʼ��ַ����ҳ���׵�ַ */
	else
	{
		if (NumOfPage == 0) /* ���ݳ���С��ҳ���С */
		{
			if (NumOfSingle > count) /* (_usWriteSize + _uiWriteAddr) > SPI_FLASH_PAGESIZE */
			{
				temp = NumOfSingle - count;
                
				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, count) == 0)
				{
					return 0;
				}
                
				_uiWriteAddr +=  count;
				_pBuf += count;
                
				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, temp) == 0)
				{
					return 0;
				}
			}
			else
			{
				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, _usWriteSize) == 0)
				{
					return 0;
				}
			}
		}
        /* ���ݳ��ȴ��ڵ���ҳ���С */
		else
		{
			_usWriteSize -= count;
			NumOfPage =  _usWriteSize / g_tSF.PageSize;
			NumOfSingle = _usWriteSize % g_tSF.PageSize;
            
			if (sf_AutoWritePage(_pBuf, _uiWriteAddr, count) == 0)
			{
				return 0;
			}
            
			_uiWriteAddr +=  count;
			_pBuf += count;
            
			while (NumOfPage--)
			{
				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, g_tSF.PageSize) == 0)
				{
					return 0;
				}
				_uiWriteAddr +=  g_tSF.PageSize;
				_pBuf += g_tSF.PageSize;
			}
            
			if (NumOfSingle != 0)
			{
				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, NumOfSingle) == 0)
				{
					return 0;
				}
			}
		}
	}
    
	return 1;	/* �ɹ� */
}

/*
*********************************************************************************************************
*	�� �� ��: sf_ReadBuffer
*	����˵��: ������ȡ�����ֽڡ��ֽڸ������ܳ���оƬ������
*	��    ��:  	_pBuf : ����Դ��������
*				_uiReadAddr ���׵�ַ
*				_usSize �����ݸ���, ���Դ���PAGE_SIZE,���ǲ��ܳ���оƬ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void sf_ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize)
{
	/* �����ȡ�����ݳ���Ϊ0���߳�������Flash��ַ�ռ䣬��ֱ�ӷ��� */
	if ((_uiSize == 0) ||(_uiReadAddr + _uiSize) > g_tSF.TotalSize)
	{
		return;
	}
    
	/* ������������ */
	FLASH_CS_EN();									/* ʹ��Ƭѡ */
	sf_SendByte(CMD_READ);							/* ���Ͷ����� */
	sf_SendByte((_uiReadAddr & 0xFF0000) >> 16);	/* ����������ַ�ĸ�8bit */
	sf_SendByte((_uiReadAddr & 0xFF00) >> 8);		/* ����������ַ�м�8bit */
	sf_SendByte(_uiReadAddr & 0xFF);				/* ����������ַ��8bit */
    
	while (_uiSize--)
	{
		*_pBuf++ = sf_SendByte(DUMMY_BYTE);			/* ��һ���ֽڲ��洢��pBuf�������ָ���Լ�1 */
	}
    
	FLASH_CS_DIS();									/* ����Ƭѡ */
}

/*
*********************************************************************************************************
*	�� �� ��: sf_WaitForWriteEnd
*	����˵��: ����ѭ����ѯ�ķ�ʽ�ȴ������ڲ�д�������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void sf_WaitForWriteEnd(void)
{
	FLASH_CS_EN();              /* ʹ��Ƭѡ */
	sf_SendByte(CMD_RDSR);      /* ������� ��״̬�Ĵ��� */
    /* ��FLASH�����ṩʱ�� */
	while((sf_SendByte(DUMMY_BYTE) & WIP_FLAG) == SET);/* �ж�״̬�Ĵ�����æ��־λ */
    
	FLASH_CS_DIS();             /* ����Ƭѡ */
}


// struct rt_device _flash_device;
// static rt_err_t flash_init(rt_device_t dev)
// {
//     return RT_EOK;
// }

// static rt_err_t flash_open(rt_device_t dev, rt_uint16_t oflag)
// {
//     return RT_EOK;
// }

// static rt_err_t flash_close(rt_device_t dev)
// {
//     return RT_EOK;
// }

// static rt_size_t flash_read(rt_device_t dev,
//                           rt_off_t    pos,
//                           void       *buffer,
//                           rt_size_t   size)
// {
// 	sf_ReadBuffer((uint8_t *)buffer,pos,size);
//     return size;
// }

// static rt_size_t flash_write(rt_device_t dev,
//                           rt_off_t    pos,
//                           const void *buffer,
//                           rt_size_t   size)
// {
// 	flash_unlock();
// 	sf_WriteBuffer((uint8_t *)buffer,pos,size);
// 	flash_lock();
//     return size;
// }

// static rt_err_t flash_control(rt_device_t dev, rt_uint8_t cmd, void *args)
// {
//     switch (cmd)
//     {
//     case RTGRAPHIC_CTRL_GET_INFO:
//     {
//         
//     }
//     break;


//     default:
//         break;
//     }

//     return RT_EOK;
// }

// void rt_hw_flash_init(void)
// {
//     /* register flash device */
//     _flash_device.type  = RT_Device_Class_Block;
//     _flash_device.init  = flash_init;
//     _flash_device.open  = flash_open;
//     _flash_device.close = flash_close;
//     _flash_device.control = flash_control;
//     _flash_device.read  = flash_read;
//     _flash_device.write = flash_write;

//     _flash_device.user_data = RT_NULL;
// 	spi_flash_init();
// 	
//     rt_device_register(&_flash_device, "flash", RT_DEVICE_FLAG_RDWR);
// }
/********************************************************************************************/
