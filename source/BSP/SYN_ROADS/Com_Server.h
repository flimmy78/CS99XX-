#ifndef __COM_SERVER_H
#define __COM_SERVER_H

// #include "bsp.h"
#include "Com_Send_Ctrl.h"
#include "my_tools.h"
#include "Com_Receive_Ctrl.h"
#include "stm32f10x.h"
#include "Com_Server.h"

typedef enum {
    GOOD,
    HIGH_ERROR,
    LOW_ERROR,
	SHORT_ERROR,
}CURRENT_ERROR_TYPE;
#define INPUT_NUM           ( 10 )

typedef struct {
    CURRENT_ERROR_TYPE Road0 :2;
    CURRENT_ERROR_TYPE Road1 :2;
    CURRENT_ERROR_TYPE Road2 :2;
    CURRENT_ERROR_TYPE Road3 :2;
    CURRENT_ERROR_TYPE Road4 :2;
    CURRENT_ERROR_TYPE Road5 :2;
    CURRENT_ERROR_TYPE Road6 :2;
    CURRENT_ERROR_TYPE Road7 :2;
    CURRENT_ERROR_TYPE Road8 :2;
    CURRENT_ERROR_TYPE Road9 :2;
}Road_Error_Msg;

typedef struct {
    uint16_t Road_current[INPUT_NUM];
    
    union {
        Road_Error_Msg  road_msg;
        uint32_t road_int_msg;
    }Road_Msg;

}Result_Inf;

extern Result_Inf result_inf;

#define     CMD_PLACE       (0)

#define		PARAM_PLACE		(CMD_PLACE + 1)

#define		I_MAX			(10000)

#define		R_I_MAX			(1.2*I_MAX)

/*
    �����ⲿʹ��
*/



/*��Ҫ����������ɨ��*/
extern int32_t Syn_Scan_usart(void);

/*���ú�ʼ����*/
extern void Syn_Start_Test(void);

/*���ú�ֹͣ����*/
extern void Syn_Stop_Test(void);

/*���ú�λ����*/
extern void Syn_Reset_Test(void);

/*���ú�����У׼����*/
extern void Syn_Set_2mACalib(float * calib_data); 

/*���ú�����У׼����*/
extern void Syn_Set_20mACalib(float * calib_data); 

/*���ú��ȡģ��ĵ������*/
extern void Syn_Read_Result(void);

/*���ú��ȡģ��ĵ�ADC���*/
extern void Syn_Read_Adc_Val(void);

/*���ú�����������*/
/*������������mAΪ��λ*/
extern void Syn_Set_limit_f(float high_limit, float low_limit);

/*���ú�����������*/
/*������������ 10uA Ϊ��λ*/
extern void Syn_Set_limit_i(uint16_t high_limit, uint16_t low_limit);


/*
    ͨ���ڲ�ʹ�����ݺͺ���
*/

// /*��ѡ���*/
// typedef enum {
//     YES,
//     NO,
// }Single_Switch;


extern uint8_t over_data[2]; /*�ַ������д�Ž�����*/

/*��ȡУ��ֵ*/
uint16_t read_check_data(uint8_t * data, uint8_t data_num);

void Send_Data(void);
#endif
