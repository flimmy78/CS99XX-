
#include "stm32f10x.h"
#include "Timer5_Config.h"
#include "BSP_config.h"



static void (*timer5_call_back)(void);

/*
 *��������set_timer5_callback
 *��������ʼ����ʱ��5�Ļص�����
 *���룺��
 *��� :��
 *���أ���
*/
void init_timer5_callback(void(*fun)(void))
{
    timer5_call_back = fun;
}
/*
 *��������TIM5_it_init
 *��������ʱ��5����
 *���룺arr ����ֵ
 *      psc ��Ƶ��
 *��� :��
 *���أ���
*/
void TIM5_it_init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;	//�Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ������ȡֵ������0x0000��0xFFFF֮�䡣
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;//������ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//ѡ���˼�����ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE);/* �ر��ж� */
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //���TIM2�����жϱ�־
	
	/* �ж����ȼ�NVIC���� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM5_IT_PRE_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM5_IT_SUB_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);/* ���ж� */
	TIM_Cmd(TIM5, ENABLE);/* �������ÿ��������� */
	return;
}


#define TIM5_MAX_SERVER_FUN     30 ///< tim3���ķ������ĸ���


/**
  * @brief  ��ʱ��3������ָ���
  */
static TIM_SERVER_FUN  tim_server_fun_pool[TIM5_MAX_SERVER_FUN];

/**
  * @brief  ע�ᶨʱ��3�ķ�����
  * @param  [in] fun ��ʱ��������
  * @retval 0 �ɹ� 1��ʾʧ�ܶ�ʱ��������������
  */
uint8_t register_tim5_server_fun(TIM_SERVER_FUN fun)
{
    int32_t i = 0;
    
    for(i = 0; i < TIM5_MAX_SERVER_FUN; i++)
    {
        if(tim_server_fun_pool[i] == NULL)
        {
            tim_server_fun_pool[i] = fun;
            break;
        }
    }
    
    if(i == TIM5_MAX_SERVER_FUN)
    {
        return 1;
    }
    
    return 0;
}

/**
  * @brief  ע����ʱ��3�ķ�����
  * @param  [in] fun ��ʱ��������
  * @retval 0 �ɹ� 1��ʾʧ�ܶ�ʱ��������������
  */
uint8_t unregister_tim5_server_fun(TIM_SERVER_FUN fun)
{
    int32_t i = 0;
    
    for(i = 0; i < TIM5_MAX_SERVER_FUN; i++)
    {
        if(tim_server_fun_pool[i] == fun)
        {
            tim_server_fun_pool[i] = NULL;
        }
    }
    
    if(i == TIM5_MAX_SERVER_FUN)
    {
        return 1;
    }
    
    return 0;
}

/*
 *��������TIM5_IRQHandler
 *��������ʱ��5�жϷ������
 *��������
 *���أ���
*/
void TIM5_IRQHandler(void)
{
    int32_t i = 0;
    
	OSIntEnter();
	
    
    for(i = 0; i < TIM5_MAX_SERVER_FUN; i++)
    {
        /* ִ�з����� */
        if(tim_server_fun_pool[i] != NULL)
        {
            tim_server_fun_pool[i]();
        }
    }
    
    
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		TIM_ClearFlag(TIM5, TIM_FLAG_Update);	     //���жϱ��
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //���TIMx�����жϱ�־ 
        
        if(NULL != timer5_call_back)
        {
            timer5_call_back();
        }
	}
	
	OSIntExit();
}
