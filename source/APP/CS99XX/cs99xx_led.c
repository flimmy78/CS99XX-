/*
 * Copyright(c) 2016,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_led.c
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V1.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#include "stm32f10x.h"
#include "cs99xx_led.h"
#include "STM32_GPIO_Config.h"

void led_cmd(uint8_t st)
{
    switch(st)
    {
        case LED_ST_PASS:
            LED_TEST = LED_OFF;/* �رղ��Ե� */
            LED_PASS = LED_ON;/* ��PASS�� */
            break;
        case LED_ST_PASS_OFF:
            
            break;
        case LED_ST_FAIL:
            
            break;
        case LED_ST_RESET:
            
            break;
    }
}


/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/

