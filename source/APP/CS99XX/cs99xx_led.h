/*
 * Copyright(c) 2016,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_led.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V1.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#ifndef __CS99XX_LED_H__
#define __CS99XX_LED_H__

enum{
    LED_ST_PASS,
    LED_ST_PASS_OFF,
    LED_ST_FAIL,
    LED_ST_RESET,
};

extern void led_cmd(uint8_t st);

#endif //__CS99XX_LED_H__

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
