#ifndef __SYSTEM_CONFIG_H
#define __SYSTEM_CONFIG_H



//#include "Schedul_cfg.h" 
#include "stdint.h"
// #include "General.h"
#include "cs99xx_struct.h"

typedef enum {ErrorType = 0, SuccessType = !ErrorType} ErrorStatusType;

                         
#define     PORT_MAX_NUM                    ( 16 )

typedef enum { UN_CONNECT = 0,REMOTE_CONTROL, LOCAL_CONTROL,  }CONTROL_STATUS;

enum {
ACW_MODE = 0,
DCW_MODE = 1,
IR_MODE  = 2,
GR_MODE  = 3,
BBD_MODE = 7,
};

#define     STEP_TIME_MAX                   ( 9999 )        /*�������ʱ������õ����ֵ*/

/*���豸����ֱ�Ӷ�ȡ���ݣ�������Ҫͨ���·����ӻ���������λ��*/
//#define SELF_GET_DATA               

/*���ӵؿ���*/
//#define     SINGLE_GND        

/*ModBus�������ݽ���ʱ�� 5mS*/
#define  MODBUS_RECEIVE_OVER_TIME           ( 5 )

// /*�����������ȼ�����*/

// #define MODBUS_CLIENT_REFRESH_TASK_PRIO     ( TASK_PRIO_0 )
// #define MODBUS_SERVER_REFRESH_TASK_PRIO     ( TASK_PRIO_1 )
// #define MODBUS_CLIENT_DISPOSE_TASK_PRIO     ( TASK_PRIO_2 )
// #define MODBUS_SERVER_DISPOSE_TASK_PRIO     ( TASK_PRIO_3 )
// #define MODBUS_CLIENT_SEND_TASK_PRIO        ( TASK_PRIO_4 )
// #define MODBUS_SERVER_SEND_TASK_PRIO        ( TASK_PRIO_5 )

// #define LED_TASK_PRIO                       ( TASK_PRIO_63 )
// #define LED1_TASK_PRIO                       ( TASK_PRIO_62 )
    

#define     FILE_NAME_MAX_SIZE              ( 14 )



#ifndef SINGLE_GND
typedef enum {G_TYPE, N_TYPE, FILE_WORK_MODE_TYPE_ERROR}FILE_WORK_MODE_TYPE;
typedef enum {SCALE_TYPE, CURRENT_TYPE, FILE_ARC_TYPE_ERROR}FILE_ARC_TYPE;
typedef enum {WHOLE_TYPE, SINGLE_TYPE, FILE_DUT_TYPE_ERROR}FILE_DUT_TYPE;

#endif

#define     FILE_NUM_MAX    ( 30 )          /*�ļ�������ֵ*/
#define     FILE_NUM_MIN    ( 1 )           /*�ļ������Сֵ*/

#define     FILE_NAME_CAPITAL               /*�ļ��������д��ĸ����*/
#define     FILE_NAME_LOWER_CASE            /*�ļ�������Сд��ĸ����*/
#define     FILE_NAME_FIGURE                /*�ļ���������������*/

/*
д���ļ���ʱ���ļ��ļ�¼��Ϊ0�����Բ�����ļ���¼�Ų�Ϊ1����STEP_MAX��
������д������ļ����ǲ��Բ���
*/
#define     FILE_NOTE_NUM       ( 0 )
#define     STEP_MAX            ( 9999 )
#define     TIME_MAX            ( 9999 )
#define     FILE_NAME_LEN       ( 16 )

//#define     STEP_MAX            ( 99 )

typedef struct {
    uint16_t file_num;                      /*�ļ����*/
    uint16_t step_sum;                      /*�ܲ�����*/
    uint8_t file_name[FILE_NAME_LEN];       /*�ļ���*/
    uint16_t pass_keep_time;                /*PASS�źű���ʱ��*/
    uint16_t pass_beep_time;                /*PASS�źŷ�����ʱ��*/
#ifndef SINGLE_GND
    FILE_WORK_MODE_TYPE mode_type;          /*����ģʽ*/
    FILE_ARC_TYPE arc_type;                 /*�绡���ģʽ*/
#endif

}FILE_MSG;

typedef enum {
    START_STATUS = 1  ,
    STOP_STATUS       ,
	PASS_STATUS		  ,
    RESET_STATUS      ,
}TestStatusTypedef;

typedef enum {
	ST_VOL_RISE     = 0,
	ST_TESTING      = 1,
	ST_VOL_FALL     = 2,
	ST_INTER_WAIT   = 3,
	ST_WAIT         = 4,
	ST_PASS         = 5,
	ST_STOP         = 6,            /* ��λ״̬ */
	ST_ERR_H        = 7,
	ST_ERR_L        = 8,
	ST_ERR_SHORT    = 9,
	ST_ERR_VOL_ABNORMAL = 10,       /* ��ѹ�쳣 */
	ST_ERR_ARC      = 11,
	ST_ERR_GFI      = 12,
	ST_ERR_FAIL     = 13,
	ST_ERR_REAL     = 14,           /* ��ʵ�������� */
	ST_ERR_CHAR     = 15,           /* ��籨�� */
	ST_ERR_GEAR     = 16,           /* ���̱��� / ��λ���� */
	ST_ERR_AMP      = 17,
	ST_OUTPUT_DELAY = 18,
	ST_ERR_OPEN     = 19,           /* ��·���� */
}TestingStatusTypedef;  /*����ʱ״̬*/

typedef enum{
    CY_TESTING      = 1,
    CY_STOP,
    CY_WAIT,
    CY_OVER,                           /* ���Խ������׿��� */
}CYLINDER_TEST_STATUS;/* ���׿���״̬ */


enum{
	UPDATE_PLC_SW 	            = 1<<0,
	UPDATE_COMM_SW	            = 1<<1,
	UPDATE_COMM_BPS	            = 1<<2,
    
	UPDATE_LEAD_IN_DATA_USB	    = 1<<3,
	UPDATE_LEAD_OUT_DATA_USB    = 1<<4,
    
    RESET_INTERFACE_MCU         = 1<<5,//��λ�ӿڰ�ĵ�Ƭ��
    
    RESULT_LEAD_OUT_USB         = 1<<6,//���������U��
    
    READ_ONE_RESULT_USB         = 1<<7,//��������һ�������ͨ�Ű����д��U��
    
    CHECK_USB_CONNECT           = 1<<8,//���U���Ƿ�����
};

typedef struct {
    uint16_t v_test;                        /*���Ե�ѹ*/
    uint8_t current_grade;                  /*������λ*/
    uint8_t real_gear;                      /* ��ʵ�ĵ�����λ */
    uint16_t i_test;                        /*���Ե���*/   
}W_MSG;                                     /*��ѹ����*/

typedef struct {
    W_MSG w_msg;                            /*��ѹ��������*/
    uint8_t real_current_butten;            /*��ʵ��������*/
    uint16_t real_current;                  /*��ʵ����*/                                                    
}ACW_MSG;                                   /*ֱ����ѹ����*/

typedef struct {
    W_MSG w_msg;                            /*��ѹ��������*/
}DCW_MSG;

typedef struct {
    uint16_t v_test;                        /*���Ե�ѹ*/
    uint8_t res_grade;                      /*���赵λ*/ 
    uint16_t iso_res;                       /*��Ե����*/           
}IR_MSG;

typedef struct {
    uint16_t i_test;                        /*���Ե���*/
    uint16_t res_gnd;                       /*�ӵص���*/
}GR_MSG;

typedef struct {
    uint8_t step;                           /*���Բ���*/
    uint8_t test_mode;                      /*����ģʽ*/
    uint16_t test_time;                     /*����ʱ��*/
    uint8_t test_status;                    /*����״̬*/

    union {
        ACW_MSG acw_msg;
        DCW_MSG dcw_msg;
        IR_MSG ir_msg;
        GR_MSG gr_msg;
    }STATUS_MSG;
}TEST_FETCH_MSG;                            /*������Ϣ*/


typedef struct{
    uint8_t buf[36];/* TEST_FETCH */
    uint32_t plc_status;/* plc״̬ */
}TEST_FETCH_T;

typedef struct {
    uint16_t test_voltage;                  /*���Ե�ѹ*/
    uint8_t current_grade;                  /*������λ*/
    uint8_t real_gear;                      /* ��ʵ�ĵ�����λ Ϊ���������е�λ�ؼ���� wangxin 2015.08.10 */
    uint16_t i_test;                        /*���Ե���*/ 
    uint16_t real_current;                  /*��ʵ����*/  
}W_Result_Msg;

typedef struct {
    uint16_t test_voltage;                  /*���Ե�ѹ*/
    uint8_t res_grade;                      /*���赵λ*/
    uint16_t iso_res;                       /*��Ե����*/ 
    uint16_t real_current;                  /*��ʵ����*/  
}IR_Result_Msg;

typedef struct {
    uint16_t test_current;                  /*���Ե���*/
    uint16_t res_gnd;                       /*�ӵص���*/
    uint16_t real_current;                  /*��ʵ����*/
}GR_Result_Msg;

typedef struct {
    uint8_t target_name[FILE_NAME_LEN];     /*�������*/
    uint8_t test_file_name[FILE_NAME_LEN];  /*�����ļ�����*/
    uint16_t test_num;                      /*���β��ԵĲ�����*/
    uint16_t test_all_num;                  /*�ܲ��Բ�����*/
    uint8_t test_file_mode;                 /*�����ļ��Ĺ���ģʽ*/
    uint8_t test_mote_mode;                 /*���Բ���Ĺ���ģʽ*/
    
    union {
        W_Result_Msg w_msg;
        IR_Result_Msg ir_msg;
        GR_Result_Msg gr_msg;
    }status_msg;
    
    uint16_t test_time;                     /*����ʱ��*/      
    uint8_t test_result;                    /*���Խ��*/
}RESULT_FETCH_MSG;

typedef struct {
    uint16_t i_high;                        /*��������*/
    uint16_t i_low;                         /*��������*/
    uint16_t arc;                           /* �绡��� */
    uint16_t arc_type;                      /* �绡���ģʽ */
    uint16_t upper_time;                    /*����ʱ��*/
    uint16_t drop_time;                     /*�½�ʱ��*/      
    
    uint16_t test_port;                     /*���Զ˿�*/ 
}STEP_W_MSG;

typedef struct {
    STEP_W_MSG w_msg;
    uint16_t out_freq;                      /*���Ƶ��*/     
}STEP_ACW_MSG;

typedef struct {
    STEP_W_MSG w_msg;                        
    uint16_t i_charge;                      /*������*/
    uint16_t delay_time;                    /*��ʱʱ��*/ 
}STEP_DCW_MSG;

typedef struct {
    uint8_t auto_change_grade;              /*�Զ�����*/
    uint16_t res_high;                      /*��������*/
    uint16_t res_low;                       /*��������*/
    uint16_t upper_time;                    /*����ʱ��*/
    uint16_t delay_time;                    /*��ʱʱ��*/
    uint16_t test_port;                     /*���Զ˿�*/
}STEP_IR_MSG;

typedef struct {
    uint16_t res_high;                      /*��������*/
    uint16_t res_low;                       /*��������*/ 
    uint16_t out_freq;                      /*���Ƶ��*/
}STEP_GR_MSG;

typedef struct {
    TEST_FETCH_MSG test_msg;

    union {
        STEP_ACW_MSG acw_msg;
        STEP_DCW_MSG dcw_msg;
        STEP_IR_MSG  ir_msg;
        STEP_GR_MSG  gr_msg;  
    }step_status_msg;

    uint16_t test_time;                     /*����ʱ��*/ 
    uint16_t step_pass;                     /*����pass*/
    uint16_t space_time;                    /*���ʱ��*/
    uint16_t step_continue;                 /*��������*/

}STEP_MSG_TYPEDEF;


typedef struct {
    uint8_t year;                           //0~99��ĺ���λ
    uint8_t month;                          //1~12�·�
    uint8_t day;                            //1~31����
    uint8_t hour;                           //0~23Сʱ
    uint8_t minute;                         //0~59����
    uint8_t second;                         //0~59����
}SYS_TIME;

typedef struct {  
    uint16_t file_num;                      /*���β��ԵĲ�����*/
    uint8_t file_name[FILE_NAME_LEN];       /*�����ļ�����*/
    uint16_t step_sum;                      /*�ܲ�����*/
  
    uint8_t mode_type;                      /*�����ļ��Ĺ���ģʽ*/
    uint16_t pass_keep_time;                /*PASS�źű���ʱ��*/
    uint16_t pass_beep_time;                /*PASS�źŷ�����ʱ��*/
 
#ifndef SINGLE_GND
    FILE_ARC_TYPE arc_type;                 /*�绡���ģʽ*/
#endif

}CATALOG_FILE;

#define	PASS_WORD_LEN		(8)

/* ����ṹ�� */
typedef struct {
	char old[PASS_WORD_LEN+1];
	char new[PASS_WORD_LEN+1];
}PASS_WORD_T;

/* ����Ʒ�� */
typedef struct {
	char name[FILE_NAME_LEN];
}RESULT_DUT_T;

typedef struct {
	char idn_data[50];
} IDN_MSG;


#endif
