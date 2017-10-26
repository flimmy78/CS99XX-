/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�m45pe16_spi_flash.c
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V1.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#include "stm32f10x.h"
#include "STM32_GPIO_Config.h"
#include "m45pe16_spi_flash.h"
#include "string.h"
#include "cs99xx_err_log.h"

static uint8_t      chipselect = 1;
static SFLASH_INFO  m45pe16_info;
static void m45_wait_for_write_end(void);

/**************************** ��ֲ�Ķ�Ӳ�� *********************************/
#define         M45_FLASH_RCC_APB2Periph_GPIO   RCC_APB2Periph_GPIOF
/* WP */
#define         M45_FLASH_WP_GPIO       GPIOF
#define         M45_FLASH_WP_PIN        GPIO_Pin_5
/* MOSI */
#define         M45_FLASH_MOSI_GPIO     GPIOF
#define         M45_FLASH_MOSI_PIN      GPIO_Pin_9
/* MISO */
#define         M45_FLASH_MISO_GPIO     GPIOF
#define         M45_FLASH_MISO_PIN      GPIO_Pin_4
/* CLK */
#define         M45_FLASH_CLK_GPIO      GPIOF
#define         M45_FLASH_CLK_PIN       GPIO_Pin_8

/* CS1 */
#define         M45_FLASH_CS1_GPIO      GPIOF
#define         M45_FLASH_CS1_PIN       GPIO_Pin_3
/* CS2 */
#define         M45_FLASH_CS2_GPIO      GPIOF
#define         M45_FLASH_CS2_PIN       GPIO_Pin_6
/* CS3 */
#define         M45_FLASH_CS3_GPIO      GPIOF
#define         M45_FLASH_CS3_PIN       GPIO_Pin_7

/*************************************************************************/


#define M45_FLASH_WP_EN()		GPIO_ResetBits(M45_FLASH_WP_GPIO, M45_FLASH_WP_PIN)
#define M45_FLASH_WP_DIS()		GPIO_SetBits(M45_FLASH_WP_GPIO, M45_FLASH_WP_PIN)

#define M45_FLASH_CS1_EN()      GPIO_ResetBits(M45_FLASH_CS1_GPIO,   M45_FLASH_CS1_PIN)
#define M45_FLASH_CS2_EN()      GPIO_ResetBits(M45_FLASH_CS2_GPIO,   M45_FLASH_CS2_PIN)
#define M45_FLASH_CS3_EN()      GPIO_ResetBits(M45_FLASH_CS3_GPIO,   M45_FLASH_CS3_PIN)
#define M45_FLASH_CS1_DIS()		GPIO_SetBits(M45_FLASH_CS1_GPIO, M45_FLASH_CS1_PIN)
#define M45_FLASH_CS2_DIS()		GPIO_SetBits(M45_FLASH_CS2_GPIO, M45_FLASH_CS2_PIN)
#define M45_FLASH_CS3_DIS()		GPIO_SetBits(M45_FLASH_CS3_GPIO, M45_FLASH_CS3_PIN)

#define M45_FLASH_DI_HIGH()		GPIO_SetBits(M45_FLASH_MOSI_GPIO, M45_FLASH_MOSI_PIN)
#define M45_FLASH_DI_LOW()		GPIO_ResetBits(M45_FLASH_MOSI_GPIO, M45_FLASH_MOSI_PIN)

#define M45_FLASH_CLK_HIGH()	GPIO_SetBits(M45_FLASH_CLK_GPIO, M45_FLASH_CLK_PIN)
#define M45_FLASH_CLK_LOW()		GPIO_ResetBits(M45_FLASH_CLK_GPIO, M45_FLASH_CLK_PIN)

#define M45_FLASH_DI()			GPIO_ReadInputDataBit(M45_FLASH_MISO_GPIO, M45_FLASH_MISO_PIN)

#define M45_CMD_WREN            0x06    /* дʹ������ */
#define M45_CMD_RDID            0x9F    /* ������ID���� */
#define M45_CMD_DISWR           0x04    /* ��ֹд, �˳�AAI״̬ */
#define M45_CMD_RDSR            0x05    /* ��״̬�Ĵ������� */
#define M45_CMD_READ            0x03  	/* ������������ */
#define M45_CMD_FAST_READ       0x0B  	/* ������������ */
#define M45_CMD_PAGE_WRITE      0x0A  	/* ҳд���� */
#define M45_CMD_PAGE_PROGRAM    0x02  	/* ҳд���� */
#define M45_CMD_PAGE_ERASE      0xDB  	/* ҳ�������� */
#define M45_CMD_SECTOR_ERASE    0xD8  	/* ��������� */
#define M45_CMD_DUMMY_BYTE      0xA5    /* ���������Ϊ����ֵ�����ڶ����� */

#define M45_WIP_FLAG            0x01		/* ״̬�Ĵ����е����ڱ�̱�־��WIP) */


/*
 * ��������flash_cs_set_m45pe16
 * ����  ��FLASHƬѡ����
 * ����  ��n 1��2��3
 * ���  ����
 * ����  ����
 */
static void flash_cs_set_m45pe16(char n)
{
	switch(n)
	{
		case 1:
			chipselect = 1;
			break;
		case 2:
			chipselect = 2;
			break;
		case 3:
			chipselect = 3;
			break;
	}
}

/*
 * ��������flash_cs_en_m45pe16
 * ����  ��FLASHƬѡʹ��
 * ����  ��n 1��2��3
 * ���  ����
 * ����  ����
 */
static void flash_cs_en_m45pe16(void)
{
    M45_FLASH_CS1_DIS();
    M45_FLASH_CS2_DIS();
    M45_FLASH_CS3_DIS();
    
	switch(chipselect)
	{
		case 1:
            M45_FLASH_CS1_EN();
			break;
		case 2:
            M45_FLASH_CS2_EN();
			break;
		case 3:
            M45_FLASH_CS3_EN();
			break;
	}
}

/*
 * ��������flash_cs_dis_m45pe16
 * ����  ��FLASHƬѡʹ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void flash_cs_dis_m45pe16(void)
{
	M45_FLASH_CS1_DIS();
	M45_FLASH_CS2_DIS();
	M45_FLASH_CS3_DIS();
}

/*
 * ��������m45_flash_lock
 * ����  ��FLASHд��������
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void m45_flash_lock(void)
{
	M45_FLASH_WP_EN();
}

/*
 * ��������m45_flash_unlock
 * ����  ��FLASHд��������
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void m45_flash_unlock(void)
{
	M45_FLASH_WP_DIS();
}
/*
 * ��������m45_gpio_init
 * ����  ����ʼ���ܽ�
 * ����  ����
 * ���  ����
 */
static void m45_gpio_init(void)
{
	/*************************    �����ʼ���ṹ����    *************************/
	GPIO_InitTypeDef  GPIO_InitStructure;
    
	
	/*************************    ��������ʱ��    *************************/
	RCC_APB2PeriphClockCmd(M45_FLASH_RCC_APB2Periph_GPIO, ENABLE);
    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /*********************************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			 //��������
	
    /* SDO MDI */
	GPIO_InitStructure.GPIO_Pin = M45_FLASH_MISO_PIN;
	GPIO_Init(M45_FLASH_MISO_GPIO, &GPIO_InitStructure);
    
    /*********************************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //�������
    
	/* SDI MDO */
	GPIO_InitStructure.GPIO_Pin = M45_FLASH_MOSI_PIN;
	GPIO_Init(M45_FLASH_MOSI_GPIO, &GPIO_InitStructure);
    
    /* WP */
	GPIO_InitStructure.GPIO_Pin = M45_FLASH_WP_PIN;
	GPIO_Init(M45_FLASH_WP_GPIO, &GPIO_InitStructure);
    
    /* CLK */
	GPIO_InitStructure.GPIO_Pin = M45_FLASH_CLK_PIN;
	GPIO_Init(M45_FLASH_CLK_GPIO, &GPIO_InitStructure);
    
    /* CS1 */
	GPIO_InitStructure.GPIO_Pin = M45_FLASH_CS1_PIN;
	GPIO_Init(M45_FLASH_CS1_GPIO, &GPIO_InitStructure);
    
    /* CS2 */
	GPIO_InitStructure.GPIO_Pin = M45_FLASH_CS2_PIN;
	GPIO_Init(M45_FLASH_CS2_GPIO, &GPIO_InitStructure);
    
    /* CS3 */
	GPIO_InitStructure.GPIO_Pin = M45_FLASH_CS3_PIN;
	GPIO_Init(M45_FLASH_CS3_GPIO, &GPIO_InitStructure);
    
	flash_cs_dis_m45pe16();
}

/*
 * ��������m45_send_byte
 * ����  ��data ���͵�����
 * ����  ����
 * ���  ����
 * ����  ���յ�������
 */
static void m45_send_byte(unsigned char data)
{
    uint8_t   i;
    
    for (i = 8; i != 0; i-- )
    {
        M45_FLASH_CLK_LOW();
        
        if (data & 0x80)
        {
            M45_FLASH_DI_HIGH();
        }
        else
        {
            M45_FLASH_DI_LOW();
        }
        
        M45_FLASH_CLK_HIGH();
        data <<= 1;
    }
    
    M45_FLASH_CLK_LOW();
}
//static int m45_send_byte(unsigned char data)
//{
//    #define  DELAY_SF()   {int j; for(j = 0; j < 200; j++);}
//	int result = 0;
//	int i = 0;
//	int pos = 0x8;
//	
////    M45_FLASH_CLK_HIGH();
//        M45_FLASH_CLK_LOW();
//        DELAY_SF();

//	for(i = 0; i < 8; i++)
//	{
//		if(data & pos)
//        {
//            M45_FLASH_DI_HIGH();
//        }
//		else
//        {
//            M45_FLASH_DI_LOW();
//        }
//        
//        
//		M45_FLASH_CLK_HIGH();
//        DELAY_SF();
//        
//		pos >>= 1;
//		result <<= 1;
//        
//		M45_FLASH_CLK_LOW();
//        DELAY_SF();
//        
//		result |= M45_FLASH_DI();
//	}
//	
////    M45_FLASH_CLK_HIGH();
//    
//	return result;
//}
static int m45_read_byte(void)
{
    #define  DELAY_SF()   {int j; for(j = 0; j < 50000; j++);}

    uint8_t   i, data = 0;
    
    for (i = 8; i != 0; i--)
    {
        M45_FLASH_CLK_LOW();
        data <<= 1;
        M45_FLASH_CLK_HIGH();
        
        data |= M45_FLASH_DI();
    }
    
    M45_FLASH_CLK_LOW();
    
    return data;
}
/*
 * ��������read_id_m45pe16
 * ����  ����FLASH ID
 * ����  ����
 * ���  ����
 * ����  ��FLASH ID
 */
static uint32_t read_id_m45pe16(void)
{
	uint32_t uiID;
	uint8_t id1, id2, id3;
	
	flash_cs_en_m45pe16();
	m45_send_byte(M45_CMD_RDID);            /* ���Ͷ�ID���� */
	id1 = m45_read_byte();					/* ��ID�ĵ�1���ֽ� */
	id2 = m45_read_byte();					/* ��ID�ĵ�2���ֽ� */
	id3 = m45_read_byte();					/* ��ID�ĵ�3���ֽ� */
    
	flash_cs_dis_m45pe16();
	
	uiID = ((uint32_t)id1 << 16) | ((uint32_t)id2 << 8) | id3;
    
	return uiID;
}

/*
 * ��������init_m45pe16_spi_flash
 * ����  ����ʼ��FLASH
 * ����  ����
 * ���  ����
 * ����  ����
 */
void init_m45pe16_spi_flash(void)
{
	m45_gpio_init();
    
    flash_cs_set_m45pe16(2);
//	m45_flash_unlock();
	
	m45pe16_info.ChipID = read_id_m45pe16();
    
    
	if(m45pe16_info.ChipID == 0x204015)
	{
		strcpy(m45pe16_info.ChipName, "M45PE16");
		m45pe16_info.TotalSize = 2 * 1024 * 1024; /* ������ = 2M */
		m45pe16_info.PageSize = 256; /* ҳ���С = 256B */
	}
    
//   m45_flash_lock();
}

/*
 * ��������m45_write_enable
 * ����  ������m45дʹ�ܴ�
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void m45_write_enable(void)
{
	flash_cs_en_m45pe16(); /* ʹ��Ƭѡ */
	m45_send_byte(M45_CMD_WREN); /* �������� */
	flash_cs_dis_m45pe16(); /* ����Ƭѡ */
}

/*
 * ��������m45_write_disable
 * ����  ������m45дʹ�ܹر�
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void m45_write_disable(void)
{
	flash_cs_en_m45pe16(); /* ʹ��Ƭѡ */
	m45_send_byte(M45_CMD_DISWR); /* �������� */
	flash_cs_dis_m45pe16(); /* ����Ƭѡ */
}

/*
 * ��������m45_wait_for_write_end
 * ����  ������m45��״ָ̬����ȴ�FLASH��������
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void m45_wait_for_write_end(void)
{
	flash_cs_en_m45pe16(); /* ʹ��Ƭѡ */
	m45_send_byte(M45_CMD_RDSR); /* ������� ��״̬�Ĵ��� */
    /* ��FLASH�����ṩʱ�� */
    while((m45_read_byte() & M45_WIP_FLAG) == SET);/* �ж�״̬�Ĵ�����æ��־λ */
    
	flash_cs_dis_m45pe16(); /* ����Ƭѡ */
}

/*
 * ��������m45_page_write
 * ����  ��дһ��ҳ���ݣ���ַ��ҳ�����
 * ����  ��_pBuf ���ݻ��� _uiWriteAddr ��ַ��ҳ�����
 * ���  ����
 * ����  ����
 */
void m45_page_write(uint8_t * _pBuf, uint32_t _uiWriteAddr)
{
	uint32_t i = 0;
    
    /* ����������ҳ����ľ��˳� */
    if(_uiWriteAddr % m45pe16_info.PageSize != 0)
    {
        return;
    }
    
    m45_flash_unlock();/* ����дʹ�� */
    
    m45_write_enable(); /* ����дʹ������ */
    
    flash_cs_en_m45pe16(); /* ʹ��Ƭѡ */
    
    m45_send_byte(M45_CMD_PAGE_PROGRAM); /* ����ҳдָ�� M45_CMD_PAGE_WRITE*/
    m45_send_byte((_uiWriteAddr & 0xFF0000) >> 16);	/* ����������ַ�ĸ�8bit */
    m45_send_byte((_uiWriteAddr & 0xFF00) >> 8); /* ����������ַ�м�8bit */
    m45_send_byte(_uiWriteAddr & 0xFF); /* ����������ַ��8bit */
    
    for (i = 0; i < 256; i++)
    {
        m45_send_byte(*_pBuf++); /* �������� */
    }
    
    flash_cs_dis_m45pe16(); /* ��ֹƬѡ */
    
    m45_flash_lock();/* ����дʹ�� */
    
    m45_wait_for_write_end(); /* �ȴ�����Flash�ڲ�д������� */
	
	m45_write_disable();/* �˳�д����״̬ */
    
	m45_wait_for_write_end(); /* �ȴ�����Flash�ڲ�д������� */
}

/*
 * ��������m45_read_buffer
 * ����  �������ݣ���ַ�������
 * ����  ��_pBuf ���ݻ��� _uiWriteAddr ��ַ _uiSize ���Ĵ�С
 * ���  ����
 * ����  ����
 */
void m45_read_buffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize)
{
	/* �����ȡ�����ݳ���Ϊ0���߳�������Flash��ַ�ռ䣬��ֱ�ӷ��� */
	if ((_uiSize == 0) ||(_uiReadAddr + _uiSize) > m45pe16_info.TotalSize)
	{
		return;
	}
    
	/* ������������ */
	flash_cs_en_m45pe16(); /* ʹ��Ƭѡ */
	m45_send_byte(M45_CMD_READ); /* ���Ͷ����� */
	m45_send_byte((_uiReadAddr & 0xFF0000) >> 16); /* ����������ַ�ĸ�8bit */
	m45_send_byte((_uiReadAddr & 0xFF00) >> 8); /* ����������ַ�м�8bit */
	m45_send_byte(_uiReadAddr & 0xFF); /* ����������ַ��8bit */
    
	while (_uiSize--)
	{
		*_pBuf++ = m45_read_byte(); /* ��һ���ֽڲ��洢��pBuf�������ָ���Լ�1 */
	}
    
	flash_cs_dis_m45pe16(); /* ����Ƭѡ */
}

/*
 * ��������m45_page_erase
 * ����  ������ҳ����ַ������ĵ������¶�ҳȡ����
 * ����  ��_pBuf ���ݻ��� _uiWriteAddr ��ַ _uiSize ������ҳ��
 * ���  ����
 * ����  ����
 */
void m45_page_erase(uint32_t _uiReadAddr, uint32_t _uiSize)
{
    int32_t i = 0;
	/* �����ȡ�����ݳ���Ϊ0���߳�������Flash��ַ�ռ䣬��ֱ�ӷ��� */
	if ((_uiSize == 0) ||(_uiReadAddr + _uiSize) > m45pe16_info.TotalSize)
	{
		return;
	}
    
    m45_flash_unlock();/* ����дʹ�� */
    m45_write_enable(); /* ����дʹ������ */
    m45_wait_for_write_end(); /* �ȴ�����Flash�ڲ�д������� */
    
	/* �������� */
    for(i = 0; i < _uiSize; i++)
    {
        flash_cs_en_m45pe16(); /* ʹ��Ƭѡ */
        
        m45_send_byte(M45_CMD_PAGE_ERASE); /* ����ҳ�������� */
        m45_send_byte((_uiReadAddr & 0xFF0000) >> 16);	/* ����������ַ�ĸ�8bit */
        m45_send_byte((_uiReadAddr & 0xFF00) >> 8);		/* ����������ַ�м�8bit */
        m45_send_byte(_uiReadAddr & 0xFF);				/* ����������ַ��8bit */
        
        flash_cs_dis_m45pe16(); /* ����Ƭѡ */
        
        m45_wait_for_write_end(); /* �ȴ�����Flash�ڲ�д������� */
        
    }
    
	m45_write_disable();/* �˳�д����״̬ */
    m45_flash_lock();/* ����дʹ�� */
    
	m45_wait_for_write_end(); /* �ȴ�����Flash�ڲ�д������� */
}

/*
 * ��������write_page_and_check_ok
 * ����  ��д��һҳ����д������ݽ��м��
 * ����  ��buf ���ݻ��� addr ��ַҳ�����
 * ���  ����
 * ����  ����
 */
static void write_page_and_check_ok(uint8_t *buf, uint32_t addr)
{
    uint8_t buf_cmp[256];
    uint32_t page_size = m45pe16_info.PageSize;
    uint32_t retry_count = 0;
    
    do
    {
        m45_page_erase(addr, 1);
        m45_page_write(buf, addr);
        m45_read_buffer(buf_cmp, addr, page_size);
        if(++retry_count > 5)
        {
            RECORD_EXCEP("M45PE16 WRITE ERR");
            break;
        }
    }while(0 != memcmp(buf, buf_cmp, page_size));
}

/*
 * ��������m45_buffer_write
 * ����  ������д����ַ��2Mb�������ַ��
 * ����  ��pBuffer ���ݻ��� WriteAddr ��ַҳ����� NumByteToWrite д����ֽ���ֻҪ�ռ��㹻�������ֽ���
 * ���  ����
 * ����  ����
 */
void m45_buffer_write(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
	uint32_t num_of_p_first = 0;//��һҳ����
	uint32_t num_of_p_n = 0;//��ҳ��
	uint32_t num_of_p_last = 0;//���һҳ����
    uint32_t addr_of_p_first = 0;//��һҳ���׵�ַ
	uint32_t addr = 0;
    uint32_t page_size = m45pe16_info.PageSize;
    static uint8_t buf[256];
    
    int32_t i = 0;
    uint32_t start;
    uint32_t first_addr_of_page;//ҳ���׵�ַ
    
    if(page_size == 0)
    {
        return;
    }
    
    addr_of_p_first = WriteAddr / page_size * page_size;
	addr = WriteAddr % page_size;//д���ַ��ÿҳ�ĵڼ�λ
    
	num_of_p_first = (addr==0? 0:(page_size - addr));//�ڿ�ʼ��һҳ������д��ĸ���
    
    /* �����ʼҳ�Ų��¾ͼ��������Ҫ��ҳ��Ϣ */
    if(NumByteToWrite > num_of_p_first)
    {
        num_of_p_n = (NumByteToWrite - num_of_p_first) / page_size;//Ҫд�����ҳ��
        num_of_p_last = (NumByteToWrite - num_of_p_first) % page_size;//���һҳ�ĸ���
    }
    
    first_addr_of_page = addr_of_p_first;
    
    /* ��һҳ */
    if(num_of_p_first > 0)
    {
        m45_read_buffer(buf, first_addr_of_page, page_size);
        
        start = addr;
        
        for(i = 0; i < num_of_p_first; i++)
        {
            buf[start + i] = *pBuffer++;
        }
        
        write_page_and_check_ok(buf, first_addr_of_page);
        
        first_addr_of_page += page_size;
    }
    /* �м�nҳ */
    if(num_of_p_n > 0)
    {
        for(i = 0; i < num_of_p_n; i++)
        {
            write_page_and_check_ok(pBuffer, first_addr_of_page);
            pBuffer += page_size;
            first_addr_of_page += page_size;
        }
    }
    /* ���һҳ */
    if(num_of_p_last > 0)
    {
        m45_read_buffer(buf, first_addr_of_page, page_size);
        
        for(i = 0; i < num_of_p_last; i++)
        {
            buf[i] = *pBuffer++;
        }
        
        write_page_and_check_ok(buf, first_addr_of_page);
    }
}


/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
