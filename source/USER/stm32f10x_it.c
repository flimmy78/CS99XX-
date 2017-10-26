/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include	"BSP_config.h"
#include 	"app.h"
#include	"irq.h"
#include 	"serve_test.h"


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  
  /* ��������ʱ����رո�ѹ����Ҫ */
  irq_stop_relay_motion();
  
  while (1)
  {
    if(BUZZER_EN > 0)
    {
        int i = 0;
        BUZZER = !BUZZER;
        for(i=0;i < 1000000;i++);
    }
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	OSIntEnter();
	
	OSTimeTick(); /*CalluC/OS-II'sOSTimeTick()*/
	
	OSIntExit();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
/* �ж���ʱ���� */
#define SOFT_DELAY_STOP		200
#define SOFT_DELAY_SHORT	1
#define SOFT_DELAY_GFI		5
#define SOFT_DELAY_ARC		5
#define DC_ARC_DELAY        30
#define DC_SHORT_DELAY      30

/*
 * ��������EXTI9_5_IRQHandler
 * ����  ����ͣ�����жϷ������+�����жϷ������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void EXTI9_5_IRQHandler(void)
{
	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)/* ��λ�����ж� */
	{
		soft_delay_10us(SOFT_DELAY_STOP);
		if(STOP_PIN == 0)
		{
			stop_irq();
			close_test_timer();
		}
		EXTI_ClearFlag(EXTI_Line5);
	}
	
	/* ���Ų�����ѯ���� */
// 	if(EXTI_GetITStatus(EXTI_Line9) != RESET)/* ���ű����ж� */
// 	{
// 		soft_delay_us(SOFT_DELAY);
// 		if(AMP_PIN == 0)
// 		{
// 			amp_irq();
// 		}
// 		EXTI_ClearFlag(EXTI_Line9);
// 	}
    
    /* �����쳣���������е��жϱ�־,��ֹ���� */
    {
        EXTI_ClearFlag(EXTI_Line5);
        EXTI_ClearFlag(EXTI_Line6);
        EXTI_ClearFlag(EXTI_Line7);
        EXTI_ClearFlag(EXTI_Line8);
        EXTI_ClearFlag(EXTI_Line9);
    }
	OSIntExit();
}

/*
 * ��������EXTI15_10_IRQHandler
 * ����  ��ARC,SHORT,GFI�жϷ������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void EXTI15_10_IRQHandler(void)
{
	OSIntEnter();
	
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)/* ARC�ж� */
	{
        if(cur_mode == DCW && 0 == cur_arc_gear)
        {
            soft_delay_us(DC_ARC_DELAY);
        }
        else
        {
            soft_delay_us(SOFT_DELAY_ARC);
        }
        
		if(ARC_PIN == 0)
		{
			arc_irq();
		}
        
		EXTI_ClearFlag(EXTI_Line11);
	}
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)/* ��·�ж� */
	{
        if(cur_mode == DCW)
        {
            soft_delay_us(DC_SHORT_DELAY);
        }
        else
        {
            soft_delay_10us(g_short_delay);
        }
        
		if(SHORT_PIN == 0)
		{
			short_irq();
		}
        
		EXTI_ClearFlag(EXTI_Line10);
	}
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)/* GFI�ж� */
	{
		soft_delay_10us(SOFT_DELAY_GFI);
		if(GFI_PIN == 0)
		{
			gfi_irq();
		}
		EXTI_ClearFlag(EXTI_Line12);
	}
	
    /* �����쳣���������е��жϱ�־ */
    {
        EXTI_ClearFlag(EXTI_Line10);
        EXTI_ClearFlag(EXTI_Line11);
        EXTI_ClearFlag(EXTI_Line12);
        EXTI_ClearFlag(EXTI_Line13);
        EXTI_ClearFlag(EXTI_Line14);
        EXTI_ClearFlag(EXTI_Line15);
    }
    
	OSIntExit();
}


/*******************************************************************************
 * Function Name  : USART2_IRQHandler
 * Description    : This function handles USART2 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
int debug_led;
void USART2_IRQHandler(void)
{
	OSIntEnter();
    
	//�����ж�
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		p_prvvUARTRxISR();
		debug_led++;
	}
	//�����ж�
	else if (USART_GetITStatus(USART2, USART_IT_TXE) == SET)
	{
		p_prvvUARTTxReadyISR();
	}
	else
	{
	}
    
	OSIntExit();
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
