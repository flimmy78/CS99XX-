/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�RTC_Config.c
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
 
 /****************** ����ͷ�ļ� *****************/
#define  RTC_GLOBALS
#include    "stm32f10x_rtc.h"
#include    "stm32f10x_pwr.h"
#include    "stm32f10x_bkp.h"
#include	<string.h>
#include	<stdio.h>

#include 	"GUI_Draw.h"
#include	"Font_DIY.h"
#include	"BSP_config.h"
#include	"RTC_Config.h"
#include 	"app.h"
#include 	"serve_test.h"

/******************** �����ڲ����� ***************/
static void RTC_NVIC_Conf(void);

void Time_Adjust(void);

/******************** �������� ******************/

void on_rtc_int(void)
{
    RTC_INI(ENABLE);
}

void off_rtc_int(void)
{
    RTC_INI(DISABLE);
}
/*
******************************************************************************
* ������  		: rtc_wait_for_synchro
* ��������    	: rtc�ȴ�ͬ��������ʱ
* �������      : ��
* ������      : ��
* ����ֵ        : ��
******************************************************************************
*/
void rtc_wait_for_synchro(void)
{
	uint32_t temp_time_out = 0;
  /* Clear RSF flag */
  RTC->CRL &= (uint16_t)~RTC_FLAG_RSF;
  /* Loop until RSF flag is set */
  while ((RTC->CRL & RTC_FLAG_RSF) == (uint16_t)RESET)
  {
	  if(temp_time_out++ > 0xffff)
	  {
          BKP_WriteBackupRegister(BKP_DR1, 0x0);
		  break;
	  }
  }
}

/*
******************************************************************************
* ������  		: rtc_wait_for_last_task
* ��������    	: rtc�ȴ����һ�β�����ɣ�����ʱ
* �������      : ��
* ������      : ��
* ����ֵ        : ��
******************************************************************************
*/
void rtc_wait_for_last_task(void)
{
	uint32_t temp_time_out = 0;
  /* Loop until RTOFF flag is set */
  while ((RTC->CRL & RTC_FLAG_RTOFF) == (uint16_t)RESET)
  {
	  if(temp_time_out++ > 0xffff)
	  {
          BKP_WriteBackupRegister(BKP_DR1, 0x0);
		  break;
	  }
  }
}
/*******************************************************************************
* ������ 		: set_data
* ��������    	: ����RTCʱ��
* �������    	: ��
* ������     	: ��
* ����ֵ       	: ��
*******************************************************************************/
void set_sys_time(void)
{
	time_now.tm_year = TM_YEAR;
	time_now.tm_mon  = TM_MONTH;
	time_now.tm_mday = TM_MDAY;
	time_now.tm_hour = TM_HOUR;
	time_now.tm_min  = TM_MIN;
	time_now.tm_sec  = TM_SEC;
	
	Time_Adjust();
}

void Time_Adjust(void)
{
	/* Wait until last write operation on RTC registers has finished */
// 	RTC_WaitForLastTask();
	rtc_wait_for_last_task();
	/* Change the current time */
	RTC_SetCounter(Time_ConvCalendar(time_now));
	/* Wait until last write operation on RTC registers has finished */
// 	RTC_WaitForLastTask();
	rtc_wait_for_last_task();
}

/*******************************************************************************
* ������ 		: RTC_Configuration
* ��������    	: ����RTC
* �������    	: ��
* ������     	: ��
* ����ֵ       	: ��
*******************************************************************************/
void RTC_Configuration(void)
{
	uint32_t temp_time_out = 0;
// #define RCC_CLK_LSI /* �Ƿ�ʹ���ڲ�ʱ������ */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	BKP_DeInit();
	
#ifdef RCC_CLK_LSI/*�����ڲ�ʱ�ӵ�Ƶ����30-60kHz֮�䣬����޷�׼ȷ�õ�32.768kHzʱ�������ж� */
	RCC_LSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{
		if(temp_time_out++>0xffff)
		{
			break;
		}
	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
#else	/* ʹ���ⲿ32.768kHz��ʱ�ӣ����ܵõ�׼ȷ�����ж� */
	RCC_LSEConfig(RCC_LSE_ON);
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
		if(temp_time_out++>0xffff)
		{
			break;
		}
	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
#endif
	
	RCC_RTCCLKCmd(ENABLE);
	
// 	RTC_WaitForSynchro();
// 	RTC_WaitForLastTask();
	
	rtc_wait_for_synchro();
	rtc_wait_for_last_task();
	
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
// 	RTC_WaitForLastTask();
	rtc_wait_for_last_task();
	
	RTC_SetPrescaler(32767);
	
// 	RTC_WaitForLastTask();
	rtc_wait_for_last_task();
	
	RTC_NVIC_Conf();
    soft_delay_us(2000);
//     temp_time_out = 0xfffff;
//     while(--temp_time_out);/* ��ʱ */
        
}

/*
******************************************************************************
* ������  		: RTC_NVIC_Conf
* ��������    	: ����NVIC����
* �������      : ��
* ������      : ��
* ����ֵ        : ��
******************************************************************************
*/
static void RTC_NVIC_Conf(void)
{
	/* ���� NVIC ��ʼ���ṹ�� */
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* ʹ�� RTC �ж� */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RTC_IT_PRE_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = RTC_IT_SUB_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void rtc_init(void)
{
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		/* ����RTC */ 
		RTC_Configuration();
		
		/* �趨ʱ�� */
		set_sys_time();
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		//����PWR��BKP��ʱ�ӣ�from APB1�� 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); 
		/* ������� */
		PWR_BackupAccessCmd(ENABLE);
		
		/* �ȴ��Ĵ���ͬ�� */
// 		RTC_WaitForSynchro(); /* �ȴ��ϴ�RTC�Ĵ���д������� ���� */
		rtc_wait_for_synchro();/* �ȴ�������ɴ���ʱ */
		
		/* ����RTC���ж� */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		
// 		RTC_WaitForLastTask(); /* �ȴ��ϴ�RTC�Ĵ���д������� ���� */
		rtc_wait_for_last_task();/* �ȴ�������ɴ���ʱ */
		RTC_NVIC_Conf();
	}
    
	/* ������ʱ������꣬������У��ʱ�������PC13 */
	#ifdef RTCClockOutput_Enable 
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); 
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE); 

	/* Disable the Tamper Pin */
	BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper 
	functionality must be disabled */ 

	/* Enable RTC Clock Output on Tamper Pin */
	BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock); 
	#endif 

	/* Clear reset flags */
	RCC_ClearFlag();
}

void GetTime(void)
{
	u32 CurrenTime = 0;
	CurrenTime = Time_GetTime();
	time_now = Time_ConvToCalendar(CurrenTime);
	
	sprintf(gUpdateTime, "%4d-%02d-%02d %02d:%02d:%02d",
		time_now.tm_year,
		time_now.tm_mon+1,
		time_now.tm_mday,
		time_now.tm_hour,
		time_now.tm_min,
		time_now.tm_sec);
}

char *transform_time_to_string(uint32_t time, char* str)
{
// 	u32 CurrenTime = 0;
// 	CurrenTime = Time_GetTime();
    static char temp_ch[21];
	time_now = Time_ConvToCalendar(time);
	
	sprintf(temp_ch, "'%4d-%02d-%02d %02d:%02d:%02d",
		time_now.tm_year,
		time_now.tm_mon+1,
		time_now.tm_mday,
		time_now.tm_hour,
		time_now.tm_min,
		time_now.tm_sec);
    if(str != NULL)
        strcpy(str, (const char*)temp_ch);
	return temp_ch;
}

char *transform_data_to_str(uint32_t time, char *str)
{
    static char temp_ch[15];
	time_now = Time_ConvToCalendar(time);
	sprintf(temp_ch, "%04d-%02d-%02d",
        time_now.tm_year,
		time_now.tm_mon+1,
		time_now.tm_mday);
    if(str != NULL)
        strcpy(str, (const char*)temp_ch);
	return temp_ch;
}
char *transform_time_to_str(uint32_t time, char *str)
{
    static char temp_ch[15];
	time_now = Time_ConvToCalendar(time);
	sprintf(temp_ch, "%02d:%02d:%02d",
		time_now.tm_hour,
		time_now.tm_min,
		time_now.tm_sec);
    if(str != NULL)
        strcpy(str, (const char*)temp_ch);
	return temp_ch;
}

char *transform_time_to_str_for_mem(uint32_t time)
{
	static char temp_ch[15];
	time_now = Time_ConvToCalendar(time);
	sprintf(temp_ch, "%02d-%02d-%02d %02d:%02d",
		time_now.tm_year%100,
		time_now.tm_mon+1,
		time_now.tm_mday,
		time_now.tm_hour,
		time_now.tm_min);
	
	return temp_ch;
}
/*
******************************************************************************
* ������  		: Time_Show
* ��������    	: �õ���ǰʱ���ַ���
* �������      : ��
* ������      : ��
* ����ֵ        : ��
******************************************************************************
*/
void Time_Show(u8 x, u8 y)
{
	GetTime();
	if(TimeDisplay)
	{
		TimeDisplay = 0;
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
		
		GUI_DispStringAt (gUpdateTime, x, y);
		LCD24064_Refresh_Rect(x,y,x+160,y+9);/* �ֲ�ˢ�� */
	}
}
/*
******************************************************************************
* ������  		: Time_SetCalendarTime
* ��������    	: ����NVIC����
* �������      : ��
* ������      : ��
* ����ֵ        : ��
******************************************************************************
*/
void Time_SetCalendarTime(struct tm t)
{
	Time_SetTime(Time_ConvCalendar(t));
	return;
}
/*
******************************************************************************
* ������  		: Time_ConvCalendar
* ��������    	: ����NVIC����
* �������      : ��
* ������      : ��
* ����ֵ        : ��
******************************************************************************
*/
u32 Time_ConvCalendar(struct tm t)
{
	t.tm_year -= 1900;  //�ⲿtm�ṹ��洢�����Ϊ2008��ʽ
						//��time.h�ж������ݸ�ʽΪ1900�꿪ʼ�����
						//���ԣ�������ת��ʱҪ���ǵ�������ء�
	t.tm_mon -= 1;
	return mktime(&t);
}
/*
******************************************************************************
* ������  		: Time_GetTime
* ��������    	: ��ȡrtc 32bit����ֵ
* �������      : ��
* ������      : ��
* ����ֵ        : ��
******************************************************************************
*/
u32 Time_GetTime(void)
{
	return (u32)RTC_GetCounter();
}
/*
******************************************************************************
* ������  		: Time_ConvToCalendar
* ��������    	: 
* �������      : time_t
* ������      : ��
* ����ֵ        : struct tm
******************************************************************************
*/
struct tm Time_ConvToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);
	t_tm->tm_year += 1900;	//localtimeת�������tm_year�����ֵ����Ҫת�ɾ���ֵ
// 	t_tm->tm_mon -= 1;
	return *t_tm;
}
/*
******************************************************************************
* ������  		: Time_SetTime
* ��������    	: ����ʱ��
* �������      : time_t
* ������      : ��
* ����ֵ        : ��
******************************************************************************
*/
void Time_SetTime(time_t t)
{
	RTC_WaitForLastTask();
	RTC_SetCounter((u32)t);
	RTC_WaitForLastTask();
	return;
}
/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
	OSIntEnter();
	if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
  	{
    	/* ��� RTC ���ж� */
    	RTC_ClearITPendingBit(RTC_IT_SEC);
    	/* ����ʱ����ʾ��־λ */
		TimeDisplay = 1;
	}
	OSIntExit();
}
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
