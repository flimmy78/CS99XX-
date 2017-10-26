
#include "stm32f10x.h"
#include "cs99xx_buzzer.h"
#include "app.h"
#include "STM32_GPIO_Config.h"
#include "serve_test.h"



/*
 *��������buzzer_flicker
 *����������������
 *���룺��
 *��� :��
 *���أ���
*/
void buzzer_flicker(void)
{
    static int n = 0;
    
    if(app_flag.buzzer_flicker_en)
    {
        if(n == 80)
        {
            BUZZER = BUZZER_ON;
        }
        
        if(n == 200)
        {
            BUZZER = BUZZER_OFF;
        }
        
        if(++n > 200)
        {
            n = 0;
        }
    }
}

/*
 *��������buzzer_delay_off
 *��������ʱ�رշ�����
 *���룺��
 *��� :��
 *���أ���
*/
void buzzer_delay_off(void)
{
    if(g_buzzer_time)
    {
        if(0 == --g_buzzer_time)/* ��������ʱ�� */
        {
            BUZZER = BUZZER_OFF;
        }
    }
}

