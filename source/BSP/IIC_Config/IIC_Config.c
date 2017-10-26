#include "stm32f10x.h"
#include "IIC_Config.h"

u8 MyData = 1;
u8 DspData = 0;




u8 I2C_Start(void)	 //I2C��ʼλ
{
    
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)
	{
	  return 0;	                    //SDA��Ϊ�͵�ƽ������æ,�˳�
	}
	SDA_L;
	I2C_delay();
	if(SDA_read)
	{ 
	  return 0;	                    //SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
	}
	SDA_L;							//SCLΪ�ߵ�ƽʱ��SDA���½��ر�ʾֹͣλ
	I2C_delay();
	return 1;
}

void I2C_Stop(void)			   //I2Cֹͣλ
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;					   //SCLΪ�ߵ�ƽʱ��SDA�������ر�ʾֹͣλ
	I2C_delay();
}

static void I2C_Ack(void)		//I2C��Ӧλ
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

static void I2C_NoAck(void)		//I2C����Ӧλ
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

u8 I2C_WaitAck(void) 	  //I2C�ȴ�Ӧ��λ
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
      return 0;
	}
	SCL_L;
	return 1;
}

 /*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : ���ݴӸ�λ����λ
* Input          : - SendByte: ���͵�����
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_SendByte(u8 SendByte) 
{
    u8 i;

    for(i = 0;i < 8; i++) 
    {
      SCL_L;
      I2C_delay();
      if(SendByte & 0x80)
	  {
        SDA_H;  			  //��SCLΪ�͵�ƽʱ������SDA���ݸı�
	  }
      else
	  { 
        SDA_L;   
	  }
      SendByte <<= 1;
      I2C_delay();
      SCL_H;
      I2C_delay();
    }
    SCL_L;
}

/*******************************************************************************
* Function Name  : I2C_ReceiveByte
* Description    : ���ݴӸ�λ����λ
* Input          : None
* Output         : None
* Return         : I2C���߷��ص�����
*******************************************************************************/
u8 I2C_ReceiveByte(void)  
{ 
    u8 i,ReceiveByte = 0;

    SDA_H;				
    for(i = 0;i < 8; i++) 
    {
      ReceiveByte <<= 1;      
      SCL_L;
      I2C_delay();
	  SCL_H;
      I2C_delay();	
      if(SDA_read)				   //��SCLΪ�ߵ�ƽʱ��SDA�ϵ����ݱ��ֲ��䣬���Զ�����
      {
        ReceiveByte |= 0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}

/*******************************************************************************
* Function Name  : I2C_WriteByte
* Description    : дһ�ֽ�����
* Input          : - SendByte: ��д������
*           	   - WriteAddress: ��д���ַ
*                  - DeviceAddress: ��������
* Output         : None
* Return         : ����Ϊ:=1�ɹ�д��,=0ʧ��
*******************************************************************************/           
u8 I2C_WriteByte(u8 SendByte, u16 WriteAddress, u8 DeviceAddress)
{		
    IIC_WR_Enable;									//дʹ�ܿ�
	if(!I2C_Start())
	{
	  return 0;
	}

 	I2C_SendByte(DeviceAddress);					//������ַд��

    if(!I2C_WaitAck())
	{
	  I2C_Stop(); 
	  return 0;
	}

    I2C_SendByte((u8)(WriteAddress & 0x0700) >>7); 	//д���ַ��λ
    I2C_WaitAck();

    I2C_SendByte((u8)(WriteAddress & 0x00FF));      //���õ���ʼ��ַ
    I2C_WaitAck();	
    I2C_SendByte(SendByte);
    I2C_WaitAck();
	   
    I2C_Stop(); 

	
    Delay_ms(10); 		  //ע�⣺��Ϊ����Ҫ�ȴ�EEPROMд�꣬���Բ��ò�ѯ����ʱ��ʽ(10ms)
	IIC_WR_Disable;			//дʹ�ܹ�
    return 1;
}									 


/*******************************************************************************
* Function Name  : I2C_WriteByte
* Description    : дN�ֽ�����
* Input          : - SendByte: ��д������
*           	   - WriteAddress: ��д���ַ
*                  - DeviceAddress: ��������
* Output         : None
* Return         : ����Ϊ:=1�ɹ�д��,=0ʧ��
*******************************************************************************/           
u8 I2C_Write_N_Byte(u8* pBuffer, u16 length, u16 WriteAddress, u8 DeviceAddress)
{		
    u8 SendByte;
	IIC_WR_Enable;									//дʹ�ܿ�

    if(!I2C_Start())
	{
	  return 0;
	}

    I2C_SendByte(DeviceAddress);					//������ַд��

    if(!I2C_WaitAck())
	{
	  I2C_Stop(); 
	  return 0;
	}
    I2C_SendByte((u8)(WriteAddress & 0x0700) >>7); 	//д���ַ��λ
    I2C_WaitAck();
    I2C_SendByte((u8)(WriteAddress & 0x00FF));      //���õ���ʼ��ַ
    I2C_WaitAck();	

	while(length)
	{
  	  SendByte = *pBuffer;
 	  I2C_SendByte(SendByte);
      if(length == 1)
	  {    
	    I2C_NoAck();
	  }
      else
	  { 
	    I2C_Ack(); 
	  }
      	pBuffer++;
      	length--;
	}//	while(length)   

    I2C_Stop(); 
    Delay_ms(10); 		  //ע�⣺��Ϊ����Ҫ�ȴ�EEPROMд�꣬���Բ��ò�ѯ����ʱ��ʽ(10ms)
	IIC_WR_Disable;			//дʹ�ܹ�	

    return 1;
}


/*******************************************************************************
* Function Name  : I2C_ReadByte
* Description    : ��ȡһ������
* Input          : - pBuffer: ��Ŷ�������
*           	   - length: ����������
*                  - ReadAddress: ��������ַ
*                  - DeviceAddress: ��������
* Output         : None
* Return         : ����Ϊ:=1�ɹ�����,=0ʧ��
*******************************************************************************/          
u8 I2C_ReadByte(u8* pBuffer,   u16 length,   u16 ReadAddress,  u8 DeviceAddress)
{		
    if(!I2C_Start())
	{
	  return 0;
	}
    
	I2C_SendByte(DeviceAddress);					//������ַд��
    if(!I2C_WaitAck())
	{
	  I2C_Stop(); 
	  return 0;
	}

	I2C_SendByte((u8)(ReadAddress & 0x0700) >>7);	//���ø���ʼ��ַ
	I2C_WaitAck();

    I2C_SendByte((u8)(ReadAddress & 0x00FF));      //���õ���ʼ��ַ   
    I2C_WaitAck();

    I2C_Start();
    I2C_SendByte(((ReadAddress & 0x0700) >>7) | DeviceAddress | 0x0001);
    I2C_WaitAck();

    while(length)
    {
      *pBuffer = I2C_ReceiveByte();
      if(length == 1)
	  {
	    I2C_NoAck();
	  }
      else
	  { 
	    I2C_Ack();
	  } 
      pBuffer++;
      length--;
    }//while(length)

    I2C_Stop();

    return 1;
}



