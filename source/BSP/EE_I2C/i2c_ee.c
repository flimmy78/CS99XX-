/* Includes ------------------------------------------------------------------*/
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "i2c_ee.h"

#define I2C_SETCLK()				(GPIO_SetBits(GPIOC,GPIO_Pin_11))
#define I2C_CLRCLK()				(GPIO_ResetBits(GPIOC,GPIO_Pin_11))
#define I2C_SETSDA()				(GPIO_SetBits(GPIOC,GPIO_Pin_13))
#define I2C_CLRSDA()				(GPIO_ResetBits(GPIOC,GPIO_Pin_13))
#define I2C_READSDA()				(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13))
#define I2C_SETWP()					(GPIO_SetBits(GPIOD,GPIO_Pin_3))
#define I2C_CLRWP()					(GPIO_ResetBits(GPIOD,GPIO_Pin_3))

u16 EEPROM_ADDRESS;

static void I2C_GPIO_SDA_IN(void)
{ 
	GPIO_InitTypeDef	GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode	=GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
static void I2C_GPIO_SDA_OUT(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;	       // ��©���
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void I2C_GPIO_CAL_IN(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode	=GPIO_Mode_IPU;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}
static void I2C_Delay(u32 t)
{
	while(t--);
}
static void I2C_Start(void)
{
	/* ��ʼ״̬׼�� */
	I2C_CLRCLK();/* ʱ�� �� */
	I2C_SETSDA();//���� ��
	I2C_SETCLK();/* ʱ�Ӹ� */
	
	/* ��start�ź� */
	I2C_Delay(I2C_Dly);
	I2C_CLRSDA();// ���� ��         ---\___
	I2C_Delay(I2C_Dly);
	I2C_CLRCLK();//ʱ�� ��
	I2C_Delay(I2C_Dly);
}
static void I2C_Stop(void)
{
	/* ׼�� */
	I2C_CLRSDA();/* ���� �� */
	I2C_SETCLK();/* ʱ�� �� */
	
	/* ��stop�ź� */
	I2C_Delay(I2C_Dly);
	I2C_SETSDA();// ���ݸ�         ___/---
	I2C_Delay(I2C_Dly);
	I2C_CLRCLK();/* ʱ�ӵ� */
	I2C_Delay(I2C_Dly);
}
/*����Ӧ��λ*//*ֻҪ����һ�����壬������û�б�Ҫ��������λ*/
static void I2C_Receive_ACK(void)
{
	I2C_SETCLK();
	I2C_Delay(I2C_Dly);
	I2C_CLRCLK();
	I2C_Delay(I2C_Dly);
}
/*����Ӧ��λ*/
static void I2C_Send_ACK(void)
{
	I2C_CLRSDA();
	I2C_Delay(I2C_Dly);
	I2C_SETCLK();//����Ӧ��λ
	I2C_Delay(I2C_Dly);
	I2C_CLRCLK();
	I2C_Delay(I2C_Dly);
}
/*���ͷ�Ӧ��λ*/
static void I2C_Send_NOACK(void)
{
	I2C_SETSDA();//���ͷ�Ӧ��λ
	I2C_Delay(I2C_Dly);
	I2C_SETCLK();
	I2C_Delay(I2C_Dly);
	I2C_CLRCLK();
	I2C_Delay(I2C_Dly);
}
/*I2Cдһ���ֽ����ݵ�24C512*/
static void I2Cwrite_byte_to24C512(unsigned char d)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{	
		if(d & 0x80)
		{
			I2C_SETSDA();
		}
		else
		{
			I2C_CLRSDA();
		}
		I2C_SETCLK();
		I2C_Delay(I2C_Dly);
		I2C_CLRCLK();
		I2C_Delay(I2C_Dly);
		d=d<<1;
	}
}
/*I2C��һ���ֽ����ݵ�24C512��һ����Ӧ��λ,���SDAΪ���*/
static unsigned char I2Cread_byte_from24C512()
{
	unsigned char i;
	unsigned char d;
	I2C_GPIO_SDA_IN();
	for(i=0;i<8;i++)
	{
		I2C_CLRCLK();
		I2C_Delay(I2C_Dly);
		I2C_SETCLK();
		I2C_Delay(I2C_Dly);
		if(I2C_READSDA() & 0x01)
		{
			d=(d<<1)+1;
		}
		else
		{
			d=d<<1;
		}
	}
	I2C_CLRCLK();
	I2C_Delay(I2C_Dly);
	I2C_GPIO_SDA_OUT();
	// I2C_Send_NOACK();
	return d;
}
static u8 I2C_Wait_Idle(void)
{
	u8 s=0;
	u16 timeover = I2C_TimeOver;
	do{
		I2C_Start();//���� I2Cx���� START����
		I2Cwrite_byte_to24C512(EEPROM_ADDRESS);
		I2C_GPIO_SDA_IN();
		
		I2C_SETCLK();
		I2C_Delay(I2C_Dly);
		s = I2C_READSDA();
		I2C_CLRCLK();
		I2C_Delay(I2C_Dly);

		I2C_GPIO_SDA_OUT();
		I2C_Stop();
		I2C_Delay(I2C_Dly*10);
	}while(s && timeover--);
	if(timeover == 0)
	{
		return I2C_Error_Timeover;
	}
	return I2C_OK;
}
static I2C_SetAddr(u16 Addr)
{
	/* Send STRAT condition */
	I2C_Start();//���� I2Cx���� START����
	/* Send EEPROM address for write */
	I2Cwrite_byte_to24C512(EEPROM_ADDRESS); /* ��λ��0��ʾд */
	I2C_Receive_ACK(); //(sun)����ֻ�������壬�����������λ
	/* Send the EEPROM's internal address to write to */
	I2Cwrite_byte_to24C512((Addr>>8) & 0xFF);//��8λ
	I2C_Receive_ACK();
	I2Cwrite_byte_to24C512(Addr & 0xFF);//��8λ
	I2C_Receive_ACK();
}
/*
 * ��������I2C_GPIO_Config
 * ����  ��I2C1 I/O����
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void I2C_GPIO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    /* ʹ���� I2C1 �йص�ʱ�� */
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD,ENABLE);
    
    /* PC11-I2C1_SCL��PC12-I2C1_SDA*/
    GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_11 | GPIO_Pin_13 ;
    GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_3 ;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOD,GPIO_Pin_3);/* д���� */
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
    
    /* PC11-I2C1_SCL��PC12-I2C1_SDA*/
//   GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_15 ;
//   GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
//   GPIO_Init(GPIOG, &GPIO_InitStructure);
}


/*
 * ��������I2C_EE_Init
 * ����  ��I2C ����(EEPROM)��ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void I2C_EE_Init(void)
{

  I2C_GPIO_Config(); 
	
/* ����ͷ�ļ�i2c_ee.h�еĶ�����ѡ��EEPROMҪд��ĵ�ַ */
#ifdef EEPROM_Block0_ADDRESS
  /* ѡ�� EEPROM Block0 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
// #endif

#elif defined EEPROM_Block1_ADDRESS  
	/* ѡ�� EEPROM Block1 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
// #endif

#elif defined EEPROM_Block2_ADDRESS  
	/* ѡ�� EEPROM Block2 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
// #endif

#elif defined EEPROM_Block3_ADDRESS  
	/* ѡ�� EEPROM Block3 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif
}

/*
 * ��������I2C_EE_BufferWrite
 * ����  �����������е�����д��I2C EEPROM��
 * ����  ��-pBuffer ������ָ��
 *         -WriteAddr �������ݵ�EEPROM�ĵ�ַ
 *         -NumByteToWrite Ҫд��EEPROM���ֽ���
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
void I2C_EE_BufferWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
	Addr = WriteAddr % I2C_PageSize;//д���ַ��ÿҳ�ĵڼ�λ
	count = I2C_PageSize - Addr;//�ڿ�ʼ��һҳ������д��ĸ���
	NumOfPage = NumByteToWrite / I2C_PageSize;//Ҫд���ҳ��
	NumOfSingle = NumByteToWrite % I2C_PageSize;//����һҳ�ĸ���
    
	I2C_EE_WriteProtect(0);
	/* If WriteAddr is I2C_PageSize aligned  */
	if(Addr == 0) //д���ַ��ҳ�Ŀ�ʼ
	{
	    /* If NumByteToWrite < I2C_PageSize */
	    if(NumOfPage == 0) //����С��һҳ
	    {
	      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);//д����һҳ������
	    }
	    /* If NumByteToWrite > I2C_PageSize */
	    else //���ݴ��ڵ���һҳ
	    {
	      while(NumOfPage--)//Ҫд���ҳ��
	      {
	        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); //дһҳ������
	        WriteAddr += I2C_PageSize;
	        pBuffer += I2C_PageSize;
	      }	
	      if(NumOfSingle!=0)//ʣ������С��һҳ
	      {
	        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);//д����һҳ������
	      }
	    }
	}
  /* If WriteAddr is not I2C_PageSize aligned  */
	else //д���ַ����ҳ�Ŀ�ʼ
	{
	    /* If NumByteToWrite < I2C_PageSize */
	    if(NumOfPage== 0) //����С��һҳ
	    {
            /* ��ʼҳֻ�ܴ�count���ֽڣ���Ȼ�Ų��£���Ҫ��ҳ��� */
			if(NumByteToWrite > count)
			{
				I2C_EE_PageWrite(pBuffer, WriteAddr, count);//д����һҳ������
				WriteAddr += count;
				pBuffer += count;
				I2C_EE_PageWrite(pBuffer, WriteAddr, NumByteToWrite-count);//��ʣ�µ�����д����һҳ
			}
            /* ��ʼҳ���Է������е����� */
			else
			{
				I2C_EE_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
	    }
	    /* If NumByteToWrite > I2C_PageSize */
	    else//���ݴ��ڵ���һҳ
	    {
	      NumByteToWrite -= count;
	      NumOfPage = NumByteToWrite / I2C_PageSize; //���¼���Ҫд���ҳ��
	      NumOfSingle = NumByteToWrite % I2C_PageSize;//���¼��㲻��һҳ�ĸ���
	      
	      if(count != 0)//�ڴ˴�countһ����Ϊ0�����ж����������е����
	      { 
	        I2C_EE_PageWrite(pBuffer, WriteAddr, count);//����ʼ�Ŀռ�д��һҳ
	        WriteAddr += count;
	        pBuffer += count;
	      }
          
	      while(NumOfPage--)//Ҫд���ҳ��
	      {
	        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);//дһҳ������
	        WriteAddr += I2C_PageSize;
	        pBuffer += I2C_PageSize;
	      }
          
	      if(NumOfSingle != 0)//ʣ������С��һҳ
	      {
	        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); //д����һҳ������
	      }
	    }
	}
    
	I2C_EE_WriteProtect(1);
}
/*
 * ��������I2C_EE_ByteWrite
 * ����  ��дһ���ֽڵ�I2C EEPROM��
 * ����  ��-pBuffer ������ָ��
 *         -WriteAddr �������ݵ�EEPROM�ĵ�ַ 
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
void I2C_EE_ByteWrite(u8* pBuffer, u16 WriteAddr)//дһ���ֽڵ�EEPROM
{
	I2C_SetAddr(WriteAddr);
	/* Send the byte to be written */
	I2Cwrite_byte_to24C512(*pBuffer);//��������
	I2C_Receive_ACK();
	/* Send STOP condition */
	I2C_Stop();//���� I2Cx���� STOP����
}
/*
 * ��������I2C_EE_PageWrite
 * ����  ����EEPROM��һ��дѭ���п���д����ֽڣ���һ��д����ֽ���
 *         ���ܳ���EEPROMҳ�Ĵ�С��AT24C02ÿҳ��8���ֽڡ�
 * ����  ��-pBuffer ������ָ��
 *         -WriteAddr �������ݵ�EEPROM�ĵ�ַ 
 *         -NumByteToWrite Ҫд��EEPROM���ֽ���
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
void I2C_EE_PageWrite(u8* pBuffer, u16 WriteAddr, u8 NumByteToWrite)//д����һҳ������
{
	I2C_SetAddr(WriteAddr);
	/* While there is data to be written */
	while(NumByteToWrite--) 
	{
		I2Cwrite_byte_to24C512(*pBuffer);//��������
		I2C_Receive_ACK();
	    /* Point to the next byte to be written */
	    pBuffer++;
	}
	/* Send STOP condition */
	I2C_Stop();//���� I2Cx���� STOP����
	if(I2C_Wait_Idle()!=I2C_OK)return;
}
/*
 * ��������I2C_EE_BufferRead
 * ����  ����EEPROM�����ȡһ�����ݡ� 
 * ����  ��-pBuffer ��Ŵ�EEPROM��ȡ�����ݵĻ�����ָ�롣
 *         -WriteAddr �������ݵ�EEPROM�ĵ�ַ�� 
 *         -NumByteToWrite Ҫ��EEPROM��ȡ���ֽ�����
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
void I2C_EE_BufferRead(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead)//��EEPROM�����ݶ��뻺����
{
	I2C_SetAddr(ReadAddr);
	/* Send STRAT condition */
	I2C_Start();//���� I2Cx���� START����
	/* Send EEPROM address for read */
	I2Cwrite_byte_to24C512(EEPROM_ADDRESS | 0x01);
	I2C_Receive_ACK(); //(sun)����ֻ�������壬�����������λ
	while(NumByteToRead--) 
	{
		/* Read a byte from the EEPROM */
		*pBuffer = I2Cread_byte_from24C512();//����ͨ�� I2Cx������յ�����
		/* Point to the next location where the byte read will be saved */
		pBuffer++;
		if(NumByteToRead > 0)
			I2C_Send_ACK();
	}
	I2C_Send_NOACK();
	/* Send STOP condition */
	I2C_Stop();//���� I2Cx���� STOP����
}
void I2C_EE_WriteProtect(u8 wp)
{
	if(wp)
	{
		I2C_SETWP();
	}
	else
	{
		I2C_CLRWP();
	}
}
/*************************END OF FILE*************************************/
