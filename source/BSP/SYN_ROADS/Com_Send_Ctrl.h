#ifndef __COM_SEND_CTRL_H
#define __COM_SEND_CTRL_H

// #include "bsp.h"
#include "Com_Server.h"
#include "stm32f10x.h"

typedef enum {
/*  
    ���� ���������������ź�
    ״̬ ��ֹͣ���߸�λ״̬
    ���� ����
    ���� ����ʼ�ɼ��ź�
    ���� ���ɹ� No Error    ʧ�ܶ�Ӧ��ʧ����Ϣ
*/
    TEST_START = 1,     
 
/*
    ���� ����������ֹͣ�ź�ֹͣ����
    ״̬ ������״̬
    ���� ����
    ���� ��ָʾ�ơ�PLC����
    ���� ���ɹ� No Error    ʧ�ܶ�Ӧ��ʧ����Ϣ
*/
    TEST_STOP  ,        
    
/*
    ���� ���������͸�λ�ź�
    ״̬ ������״̬��ֹͣ״̬
    ���� ����
    ���� ��Ϩ��ָʾ�ơ�PLC
    ���� ���ɹ� No Error    ʧ�ܶ�Ӧ��ʧ����Ϣ
*/
    TEST_RESET ,        

/*
    ���� ����������У׼����
    ״̬ ����λ״̬
    ���� ��ʮ·���Ե�У׼ֵ 20mA������ʽ��float�ͣ�
    ���� ���޸�ÿһ·��У׼����  
    ���� ���ɹ� No Error    ʧ�ܶ�Ӧ��ʧ����Ϣ
*/
    SET_20mA_CALIB ,  

/*
    ���� ����������У׼����
    ״̬ ����λ״̬
    ���� ��ʮ·���Ե�У׼ֵ 20mA������ʽ��float�ͣ�
    ���� ���޸�ÿһ·��У׼����  
    ���� ���ɹ� No Error    ʧ�ܶ�Ӧ��ʧ����Ϣ
*/
    SET_2mA_CALIB ,  

/*
    ���� ����ȡ���Խ��
    ״̬ ���������봦������״̬
    ���� ����
    ���� ����ȡ���Խ��
    ���� ���ɹ� ���Խ��    ����ʧ�ܶ�Ӧ��ʧ����Ϣ
*/
    READ_RESULT,

/*
    ���� �����õ���������
    ״̬ ����λ״̬
    ���� �����Ե�����������(float)
    ���� ������֮ǰ�޸ĵ�����������
    ���� ���ɹ� No Error    ʧ�ܶ�Ӧ��ʧ����Ϣ
*/
    SET_LIMIT,
	
/*
	���ã���ȡģ���ADC����
    ״̬ ���������봦������״̬
    ���� ����
    ���� ����ȡ���Թ�����ADC�����У׼ʱʹ�ã�
    ���� ���ɹ� �� ADC���    ����ʧ�ܶ�Ӧ��ʧ����Ϣ	
*/	
	READ_ADC_VAL,
	
	SET_2mA_OFF_SET,
	
	SET_20mA_OFF_SET,
    
}CTRL_CMD_LIST;

extern void Write_Cmd(CTRL_CMD_LIST cmd);

extern void Send_Dispose(void);
extern void Send_Rear_Dispose(void);
extern void Write_char_data(uint8_t *data, uint8_t num);

extern void Comm_OK(void);




#endif
