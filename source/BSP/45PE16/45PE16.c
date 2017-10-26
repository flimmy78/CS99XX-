/****************************************************************************** 
 *  ��Ŀ���� :                      
 *                                                               
 *  ��Ŀ���� :  
 *                                                                          
 *  Ӧ����� :  WinAVR                                                
 *                                                                       
 *  ��    �� :  WinAVR-20060214-install 
 *                                                          
 *  Ӳ    �� :  
 *                                                                
 *  ����ʱ�� :  
 *               
 *  ��    д :             
 *               
 *  ��    ע :                                                                 
 *                                                                                     
 *                                                                                           
******************************************************************************/ 

/******************************************************************************
 *                          ���ļ������õ�ͷ�ļ�
******************************************************************************/ 
 
#include    "45PE16.h"

/******************************************************************************
 *                       ���ļ�������ľ�̬ȫ�ֱ���
******************************************************************************/ 

static uint8 _ChipSelectForM45PE16  = 0;

/******************************************************************************
 *                       ���ļ��������ȫ��ȫ�ֱ���
******************************************************************************/ 

/******************************************************************************
 *                       ���ļ��������ȫ�ֽṹ��
******************************************************************************/

/******************************************************************************
 *                       ���ļ�������ľ�̬���ݽṹ
******************************************************************************/ 

/******************************************************************************
 *                            ���ļ��ڲ��궨��
******************************************************************************/

/******************************************************************************
 *                          ���ļ��ڲ��꺯������
******************************************************************************/
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

#define SPI_SCK_HIGH()              GPIO_SetBits(M45_FLASH_CLK_GPIO, M45_FLASH_CLK_PIN)
#define SPI_SCK_LOW()               GPIO_ResetBits(M45_FLASH_CLK_GPIO, M45_FLASH_CLK_PIN)

#define SPI_OUTPUT_HIGH()           GPIO_SetBits(M45_FLASH_MOSI_GPIO, M45_FLASH_MOSI_PIN)
#define SPI_OUTPUT_LOW()            GPIO_ResetBits(M45_FLASH_MOSI_GPIO, M45_FLASH_MOSI_PIN)

//#define SPI_INPUT_PORT              (FIO0PIN)               // SPI����˿�
//#define SPI_INPUT_PORT_INDEX        (17)                    // ����˿�������

#define PARAM_MEM_WRT_EN()          GPIO_ResetBits(M45_FLASH_WP_GPIO, M45_FLASH_WP_PIN)
#define PARAM_MEM_WRT_DISEN()       GPIO_SetBits(M45_FLASH_WP_GPIO, M45_FLASH_WP_PIN)

#define PARAM_MEM_CS_EN()           GPIO_ResetBits(M45_FLASH_CS1_GPIO,   M45_FLASH_CS1_PIN)
#define PARAM_MEM_CS_DISEN()        GPIO_SetBits(M45_FLASH_CS1_GPIO, M45_FLASH_CS1_PIN)

#define CAL_PARAM_MEM_CS_EN()       GPIO_ResetBits(M45_FLASH_CS2_GPIO,   M45_FLASH_CS2_PIN)
#define CAL_PARAM_MEM_CS_DISEN()    GPIO_SetBits(M45_FLASH_CS2_GPIO, M45_FLASH_CS2_PIN)

#define RESULT_MEM1_CS_EN()         GPIO_ResetBits(M45_FLASH_CS3_GPIO,   M45_FLASH_CS3_PIN)
#define RESULT_MEM1_CS_DISEN()      GPIO_SetBits(M45_FLASH_CS1_GPIO, M45_FLASH_CS1_PIN)

#define RESULT_MEM2_CS_EN()         GPIO_ResetBits(M45_FLASH_CS3_GPIO,   M45_FLASH_CS3_PIN)
#define RESULT_MEM2_CS_DISEN()      GPIO_SetBits(M45_FLASH_CS3_GPIO, M45_FLASH_CS3_PIN)

#define M45_FLASH_DI()			GPIO_ReadInputDataBit(M45_FLASH_MISO_GPIO, M45_FLASH_MISO_PIN)


/******************************************************************************
 *                           ���ļ���̬��������
******************************************************************************/

static uint8 _M45PE16BusyStateQuery(void);

static void _M45PE16ChipSelServ(uint8 cmd); 

/******************************************************************************
 *                            �ļ��ӿں�������
******************************************************************************/ 

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  ģ��SPI�ӿڷ���һ�ֽ�                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/
static void _SPISendByte(uint8  data)
{
    uint8   i;
    
    for (i = 8; i != 0; i-- )
    {
        SPI_SCK_LOW();
        if (data & 0x80)
        {
            SPI_OUTPUT_HIGH();
        }                                                                
        else
        {
            SPI_OUTPUT_LOW();
        }                                                                
        SPI_SCK_HIGH();                                          
        data <<= 1;                                                      
        
    }                                                                    
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  ģ��SPI�ӿڽ���һ�ֽ�                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/
static uint8 _SPIGetByte(void)
{
    uint8   i, data = 0;                                                   
    
    for (i = 8; i != 0; i--)
    {
        SPI_SCK_LOW();
        data <<= 1;
        SPI_SCK_HIGH();
        if (M45_FLASH_DI() != RESET)
        {
            data++;
        }
    }
    
    return data;                                                         
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  45PE16дʹ�ܱ�־λ��Ϊʹ��״̬                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static void _M45PE16WriteEnable(void)
{
    _M45PE16ChipSelServ(EN);
    _SPISendByte(M45PE16_CMD_WREN);
    _M45PE16ChipSelServ(DISEN);                                              
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  45PE16дʹ�ܱ�־λ��Ϊ����״̬                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static void _M45PE16WriteDisenable(void)
{
    _M45PE16ChipSelServ(EN);
    _SPISendByte(M45PE16_CMD_WRDI);
    _M45PE16ChipSelServ(DISEN);                                              
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  ��ȡ����ID                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

uint32 HAL_ReadM45PE16ID(void)
{
    uint8   data2, data1, data0;                                         
    uint32  data    = 0;                                                        
    
    _M45PE16ChipSelServ(EN);
    _SPISendByte( M45PE16_CMD_RDID );                          
    data2 = _SPIGetByte();                                
    data1 = _SPIGetByte();                                
    data0 = _SPIGetByte();
    _M45PE16ChipSelServ(DISEN);                                              
    
    data  = ((uint32)data2)<<16;                                         
    data += ((uint32)data1)<<8;                                          
    data += data0;                                                       
    
    return data;                                                         
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  ��ȡ����״̬�Ĵ���                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static uint8 _ReadM45PE16State(void)
{
    uint8   data;                                                        
    
    _M45PE16ChipSelServ(EN);
    _SPISendByte( M45PE16_CMD_RDSR );                          
    data            = _SPIGetByte();
    _M45PE16ChipSelServ(DISEN);                                              

    return data;                                                         
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  ��ȡָ�������ַ��һҳ����                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static uint8 _M45PE16ReadPhyPage(uint16 page, uint8 *pdata)
{
    uint16  i;                                                           

    // ҳ�����ַ����
    if (page >= M45PE16_MAX_PHY_PAGE)
    {                                
        return M45PE16_ERROR;                                            
    }                                                                    
    
    _M45PE16ChipSelServ(EN);
    _SPISendByte( M45PE16_CMD_READ );                          
    _SPISendByte( (uint8)(page>>8) );                      
    _SPISendByte( (uint8)(page&0x00FF) );                  
    _SPISendByte( 0x00 );                                  

    for (i = 0; i < 256; i++ )
    {                                           
        pdata[i] = _SPIGetByte();                          
    }
    _M45PE16ChipSelServ(DISEN);                                              
    
    return M45PE16_OK;                                                   
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  ����ָ�������ַ��һҳ����                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static uint8 _M45PE16ErasePhyPage(uint16 page)
{
    uint8   rdata;                                                       
    
    // ҳ�����ַ����
    if (page >= M45PE16_MAX_PHY_PAGE)
    {                                
        return M45PE16_ERROR;                                            
    }                                                                    
        
    // ���� FLASH д����
    _M45PE16WriteEnable();                                         
    rdata = _ReadM45PE16State();                                    
    // д����δ�򿪣����ش���
    if ((rdata & 0x02) == 0x00)
    {                                    
        _M45PE16WriteDisenable();                                           
        return M45PE16_ERROR;                                            
    }                                                                    

    _M45PE16ChipSelServ(EN);
    // ҳ����ָ��                                               
    _SPISendByte( M45PE16_CMD_PE );                           
    _SPISendByte( (uint8)(page>>8) );                      
    _SPISendByte( (uint8)(page&0x00FF) );                  
    _SPISendByte( 0x00 );
    _M45PE16ChipSelServ(DISEN);
    
    return _M45PE16BusyStateQuery();                                                
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  ���ָ�������ַ��һҳ����                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static uint8 _M45PE16ProgPhyPage(uint16 page, uint8 *pdata)
{
    uint16  i;                                                           
    uint8   rdata;                                                       
    
    // ҳ�����ַ����
    if (page >= M45PE16_MAX_PHY_PAGE)
    {                                
        return M45PE16_ERROR;                                            
    }                                                                    
    
    // ���� FLASH д����
    _M45PE16WriteEnable();                                         
    rdata = _ReadM45PE16State();                                    
    // д����δ�򿪣����ش���
    if ((rdata & 0x02) == 0x00)
    {                                    
        _M45PE16WriteDisenable();                                           
        return M45PE16_ERROR;                                            
    }                                                                    

    _M45PE16ChipSelServ(EN);
    // ҳ���ָ��
    _SPISendByte( M45PE16_CMD_PP );                           
    _SPISendByte( (uint8)(page>>8) );                      
    _SPISendByte( (uint8)(page&0x00FF) );                  
    _SPISendByte( 0x00 );                                  
    for (i = 0; i < 256; i++)
    {                                             
        _SPISendByte(pdata[i]);                           
    }
    _M45PE16ChipSelServ(DISEN);
    
    return _M45PE16BusyStateQuery();                                                
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  дָ�������ַҳ ������������̹���                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static uint8 _M45PE16WritePhyPage(uint16 page, uint8 addrInPage, uint8 data[256], uint16 dataLen)
{
    uint16          i;                                                           
    uint8           rdata;                                                       
    
    // ҳ�����ַ����
    if (page >= M45PE16_MAX_PHY_PAGE)
    {                                
        return M45PE16_ERROR;                                            
    }                                                                    
    
    // ���� FLASH д����
    _M45PE16WriteEnable();                                         
    rdata = _ReadM45PE16State();                                    
    // д����δ�򿪣����ش���
    if ((rdata & 0x02) == 0x00)
    {                                    
        _M45PE16WriteDisenable();                                           
        return M45PE16_ERROR;                                            
    }                                                                    

    _M45PE16ChipSelServ(EN);
    // ҳд��ָ��
    _SPISendByte( M45PE16_CMD_PW );                           
    _SPISendByte( (uint8)(page>>8) );                      
    _SPISendByte( (uint8)(page&0x00FF) );                  
    _SPISendByte( addrInPage );                                  
    for (i = 0; i < dataLen; i++)
    {                                             
        _SPISendByte(data[i]);                           
    }
    _M45PE16ChipSelServ(DISEN);
    
    return _M45PE16BusyStateQuery();                                                
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  �ж�M45PE16�Ƿ����ڲ�����ʱ�� ��æ״̬                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                               
 *                                                                            
 *                                                                            
******************************************************************************/

static uint8 _M45PE16BusyStateQuery(void)
{
    uint16          i;
    uint8           rdata;

    // ȷ�� 11ms �ӳ٣���� д�� ��ϱ�־
    i = 50000;                                                          
    while (i--)
    {                                                       
        rdata = _ReadM45PE16State();                                
        if ((rdata & 0x01) == 0x00)
        {                                   
            _M45PE16WriteDisenable();                                       
            return M45PE16_OK;                                           
        }                                                                
        NOP();                                             
        NOP();                                             
        NOP();                                             
        NOP();                                             
        NOP();                                             
    }                                                                    
    
    // ��ʱ��Ӧ�����
    _M45PE16WriteDisenable();                                               
    return M45PE16_ERROR;
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  Ƭѡ�źŵ�ʹ�������                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/

static void _M45PE16ChipSelServ(uint8 cmd)
{
    //ָ���洢����Ƭѡʹ��
    if (EN == cmd)
    {
        switch (_ChipSelectForM45PE16)
        {
            case 0:
            
                CAL_PARAM_MEM_CS_EN();

                break;

            case 1:
            
                PARAM_MEM_CS_EN();

                break;

            case 2:
            
                RESULT_MEM1_CS_EN();

                break;

            case 3:
            
                RESULT_MEM2_CS_EN();

                break;

            default:
                break;
        }
    }
    //ָ���洢����Ƭѡ����
    else
    {
        switch (_ChipSelectForM45PE16)
        {
            case 0:
            
                CAL_PARAM_MEM_CS_DISEN();

                break;

            case 1:
            
                PARAM_MEM_CS_DISEN();

                break;

            case 2:
            
                RESULT_MEM1_CS_DISEN();

                break;

            case 3:
            
                RESULT_MEM2_CS_DISEN();

                break;

            default:
                break;
        }
    }
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  д�����źŵ�ʹ�������                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/

void HAL_M45PE16WriteProtectCtrl(uint8 cmd)
{
    // ָ���洢����Ƭѡʹ��
    if (M45PE16_WP_ENABLE == cmd)
    {
        switch (_ChipSelectForM45PE16)
        {
            case 0:
            case 1:
            case 2:
            
                PARAM_MEM_WRT_EN();
                break;

            default:
                break;
        }
    }
    // ָ���洢����Ƭѡ����
    else
    {
        switch (_ChipSelectForM45PE16)
        {
            case 0:
            case 1:
            case 2:
            
                PARAM_MEM_WRT_DISEN();
                break;

            default:
                break;
        }
    }
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  M45PE16�����������                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/

void HAL_M45PE16IoCtrl(uint8 cmd, void *pparam)
{
    if (M45PE16_CHIP_SELECT == cmd)
    {
        _ChipSelectForM45PE16       = (uint8)pparam;
    }
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  M45PE16�����ֽ�д��                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/
uint8   M45PE16_ContinuousBytesWrt(uint32 destAddr, uint8 *prscData, uint32 dataLen)
{
    uint16      writeLen;
    uint16      startPage                   = destAddr / M45PE16_PAGE_SIZE;
    uint8       addrInPage                  = destAddr % M45PE16_PAGE_SIZE;

    // дʹ�ܹܽ���Ϊʹ��״̬
    HAL_M45PE16WriteProtectCtrl(M45PE16_WP_ENABLE);

    while (dataLen > 0)
    {
        writeLen                            = M45PE16_PAGE_SIZE - addrInPage;
        if (writeLen > dataLen)
        {
            writeLen                        = dataLen;
        }
        if (M45PE16_ERROR == _M45PE16WritePhyPage(startPage, addrInPage, prscData, writeLen))
        {
            return M45PE16_ERROR;
        }
        startPage++;
        prscData                           += writeLen;
        addrInPage                          = 0;
        dataLen                            -= writeLen;
    }

    // дʹ�ܹܽ���Ϊд����״̬
    HAL_M45PE16WriteProtectCtrl(M45PE16_WP_DISENABLE);

    return M45PE16_OK;
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  �����ַ��ȡ���ⳤ������                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/

void   M45PE16_ContinuousBytesRd(uint32 rscAddr, uint8 *pdestData, uint32 dataLen)
{
    uint32      i;

    _M45PE16ChipSelServ(EN);
    // ҳд��ָ��
    _SPISendByte( M45PE16_CMD_READ );                           
    _SPISendByte( (uint8)(rscAddr>>16) );                      
    _SPISendByte( (uint8)(rscAddr>>8)&0x000000FF );                  
    _SPISendByte( (uint8)(rscAddr&0x000000FF) );                                  
    for (i = 0; i < dataLen; i++)
    {                                             
        pdestData[i]    = _SPIGetByte();
    }
    _M45PE16ChipSelServ(DISEN);
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  ģ��SPI�ӿ�����GPIO�˿ڳ�ʼ��                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/
static void _GPIOForM45PE16Init(void)
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
    
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  M45PE16 ������ʼ��                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/
void M45PE16Init(void)
{
    _GPIOForM45PE16Init();    

    //��ʹ�ö˿ڳ�ʼ��
    PARAM_MEM_CS_DISEN();
    CAL_PARAM_MEM_CS_DISEN();
    RESULT_MEM1_CS_DISEN();
    RESULT_MEM2_CS_DISEN();
    
    HAL_ReadM45PE16ID();
}

/******************************************************************************
 *  �������� :                                                                
 *                                                                           
 *  �������� :  45PE16���ܵ��Ժ���                                                            
 *                                                                           
 *  ��ڲ��� :                                                                
 *                                                                             
 *  ���ڲ��� :                                                                
 *                                                                              
 *  �� д �� :                                                                
 *                                                                                 
 *  ��    �� :                                                                 
 *                                                                              
 *  �� �� �� :                                                                                                                               
 *                                                                             
 *  ��    �� :                                                                
 *                                                                                                                                       
 *  ��    ע :                                                                
 *                                                                            
 *                                                                            
******************************************************************************/
#ifdef M45PE16_DEBUG
uint8       txdData[1000]     = {0};
uint8       rxdData[2000]    = {0};
void M45PE16_M45PE16FuncDebug(void)
{
    uint16      i;
    uint32      memId;

    HAL_M45PE16IoCtrl(M45PE16_CHIP_SELECT, (void *)3);

    memId   = HAL_ReadM45PE16ID();

    for (i = 0; i < 1000; i++)
    {
        txdData[i] = 1;
    }
    HAL_M45PE16WriteProtectCtrl(M45PE16_WP_ENABLE);
    M45PE16_ContinuousBytesWrt(250, txdData, sizeof(txdData));
    HAL_M45PE16WriteProtectCtrl(M45PE16_WP_DISENABLE);

    M45PE16_ContinuousBytesRd(250, rxdData, sizeof(txdData));
}

#endif

/******************************************************************************
 *                             END  OF  FILE                                                                          
******************************************************************************/
