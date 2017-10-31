/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�serve_test.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#ifndef __SERVE_TEST_H__
#define __SERVE_TEST_H__


/******************************* �����ļ� *********************************/
#include "stm32f10x.h"
#include "STM32_GPIO_Config.h"                  /* ����λ�� */
#include "cs99xx_struct.h"
#include "cs99xx_parameter.h"
#include "i2c_ee.h"

/******************************* �궨�� *********************************/




/* ��������ˢ�� */
#define LCD_REFRESH() {/*OS_CPU_SR cpu_sr;OS_ENTER_CRITICAL();*/ReFresh_LCD24064();/*OS_EXIT_CRITICAL();*/}


/* ���Գ���ʹ�� */
#define PI				((float)3.14159)
#define ROUND			((float)0.5)            /* ����AD��������  */

/* ����ָ��С����λ��������ݳ��� */
#define COUNT_STR_LON_1DECS(Data)       (Data < 100? 3 : (Data < 1000? 4 : 5))
#define COUNT_STR_LON_2DECS(Data)       (Data < 1000 ? 4 : 5)
#define COUNT_LONG(data, decs)          (decs == 1? COUNT_STR_LON_1DECS(data) : (decs==2? COUNT_STR_LON_2DECS(data) : 5))

#define COUNT_FLOAT_LON_1DECS(Data)     (Data < 10? 3 : (Data < 100? 4 : 5))
#define COUNT_FLOAT_LON_2DECS(Data)     (Data < 10? 4 : 5)
#define COUNT_FLOAT_LON(data, decs)     (decs == 0? 4 : (decs == 1? COUNT_FLOAT_LON_1DECS(data) : (decs == 2? COUNT_FLOAT_LON_2DECS(data) : 5)))

#define COUNT_FLOAT_DECS(data)          (data < 10? 3 : ((data) < 100? 2 : ((data) < 1000? 1 : 0)))

        
        
/* �����и��ֵ�λ���� */
enum{
	/* ������λ */
	AC_2uA		= 0X1,
	AC_20uA		= 0X2,
	AC_200uA	= 0X3,
	AC_2mA		= 0X4,
	AC_10mA		= 0X5,
	AC_20mA		= 0X6,
	AC_50mA		= 0x7,
	AC_100mA	= 0X8,
	AC_200mA	= 0X9,
    AC_GEAR_END,
	
	/* ֱ��������λ */
	DC_2uA		= 0X1,
	DC_20uA		= 0x2,
	DC_200uA	= 0X3,
	DC_2mA		= 0x4,
	DC_10mA		= 0X5,
	DC_20mA		= 0X6,
	DC_50mA		= 0X7,
	DC_100mA	= 0X8,
    DC_GEAR_END,
	
	/* ��Ե���赵λ */
	IR_10MOHM	= 0X01,
	IR_100MOHM	= 0X02,
	IR_1GOHM	= 0x03,
	IR_10GOHM	= 0X04,
	IR_100GOHM	= 0X05,
	
	/* ֱ���ӵص��赵λ */
	DC_GR_20mV	= 0x2,
	DC_GR_200mV	= 0x3,
	DC_GR_2000mV= 0X4,
	DC_GR_12V	= 0X5,
	DC_GR_12V_100mA  = 0x6,
	DC_GR_12V_1000mA = 0x7,
	
	/* ͨ��ʱ��λ��Ϊ4�� */
	AC_0_GEAR_COMM	= 0X0,
	AC_1_GEAR_COMM	= 0X1,
	AC_2_GEAR_COMM	= 0X2,
	AC_3_GEAR_COMM	= 0X3,
	
	/* ͨ��ʱ��λ��Ϊ6�� */
	DC_0_GEAR_COMM	= 0X0,
	DC_1_GEAR_COMM	= 0X1,
	DC_2_GEAR_COMM	= 0X2,
	DC_3_GEAR_COMM	= 0X3,
	DC_4_GEAR_COMM	= 0X4,
	DC_5_GEAR_COMM	= 0X5,
	
	/* ͨ��ʱIR��λ��Ϊ6�� */
	IR_1_GEAR_COMM	= 0X1,
	IR_2_GEAR_COMM	= 0X2,
	IR_3_GEAR_COMM	= 0X3,
	IR_4_GEAR_COMM	= 0X4,
	IR_5_GEAR_COMM	= 0X5,
	IR_6_GEAR_COMM	= 0X6,
};

/* �����б�־״̬ */
enum{
    /* DC_GR����״̬ */
	DC_GR_STOPING   = 0,
	DC_GR_RUNING    = 1,
    
    /* ��������״̬ */
    TEST_STOPING    = 0,
    TEST_RUNING     = 1,
    
    /* IR�Զ��л���λʱʹ�� */
    GEAR_UP         = 0,    /* ir���ϻ��� */
    GEAR_DOWN       = 1,    /* ir���»��� */
    
    /* ���ñ������ĺ���ʱҪ�����ı�־ INIT_RESULT ��ʼ���ṹ SAVE_RESULT��������eep */
    INIT_RESULT     = 1,    /* ��ʼ�������Ϣ 	*/
    SAVE_RESULT     = 2,    /* ������			*/
    
    OFFSET_CON      = 0,    /* ƫ�Ʋ�������ģʽ */
    OFFSET_SIG      = 1,    /* ƫ�Ʋ��Ե���ģʽ */
    
    U_TEST_TIME     = 0,    /* ˢ�²���ʱ�� */
    U_OFFSET_TIME   = 1,    /* ˢ��ƫ�Ʋ���ʱ�� */
};

/* ������ */
typedef enum err_num_enum{
    ERR_NONE        = 0,    /* û�д��� */
    ERR_HIGH        = 1,    /* �������ޱ��� */
    ERR_LOW         = 2,    /* �������ޱ��� */
    ERR_REAL        = 3,    /* ��ʵ�������� */
    ERR_CHARGE      = 4,    /* ��籨�� */
    ERR_OPEN        = 5,    /* ��·���� */
    ERR_SHORT       = 6,    /* ��·���� */
    ERR_ARC         = 7,    /* ARC ���� */
    ERR_GFI         = 8,    /* GFI ���� */
    ERR_AMP         = 9,    /* ���ű��� */
    ERR_GEAR        = 10,   /* ��λ���� */
    ERR_VOL         = 11,   /* ��ѹ�쳣 */
}ERR_NUM_ENUM;

typedef enum{
    STAGE_READY     = 0,    /* Ԥ���׶� */
    STAGE_RISE      = 1,    /* �����׶� */
    STAGE_TEST      = 2,    /* ���Խ׶� */
    STAGE_FALL      = 3,    /* �½��׶� */
    STAGE_CHANGE    = 4,    /* ����׶� */
    STAGE_CHARGE    = 5,    /* ���׶� */
    STAGE_DIS_CHA   = 6,    /* �ŵ�׶� */
    STAGE_INTER     = 7,    /* ����׶� */
    STAGE_STAB      = 8,    /* ��ѹ�׶� */
}TEST_STAGE;

/************************************************/

/* �ⲿ�ж��߿��غ궨�� */
// #define AMP_INT(s)		(s == ENABLE? (EXTI_LINE9_ENABLE = 1):(EXTI_LINE9_ENABLE =0))   /* �����쳣�жϿ��� */
#define SHORT_INT(s)	(s == ENABLE? (EXTI_LINE10_ENABLE = 1):(EXTI_LINE10_ENABLE =0))     /* ��·�쳣�жϿ��� */
#define ARC_INT(s)		(s == ENABLE? (EXTI_LINE11_ENABLE = 1):(EXTI_LINE11_ENABLE =0))     /* �绡�쳣�жϿ���	*/
#define STOP_INT(s)		(s == ENABLE? (EXTI_LINE5_ENABLE = 1):(EXTI_LINE5_ENABLE =0))       /* ��λ�쳣�жϿ���	*/
#define GFI_INT(s)		(s == ENABLE? (EXTI_LINE12_ENABLE = 1):(EXTI_LINE12_ENABLE = 0))    /* GFI �쳣�жϿ��� */

#define RTC_INI(s)      (s == ENABLE? (RTC_INI_ENABLE = 1):(RTC_INI_ENABLE = 0))            /* RTC �жϿ��� */

/* ��־��д�궨�� */

#define DIS_SPEED       (test_flag.dis_speed)
#define STOP_COMM       (test_flag.stop_comm)
#define STOP            (test_flag.stop)
#define TERMINATE       (test_flag.terminate)
#define PASS            (test_flag.pass)
#define NEXT            (test_flag.next)
#define FAIL            (test_flag.fail)
#define CUR_FAIL        (test_flag.cur_step_fail)
#define TEST            (test_flag.test)
#define TESTING         (test_flag.testing)
#define HIGH            (test_flag.high_vol)
#define WAIT            (test_flag.wait)
#define OVER            (test_flag.test_over)
#define CUR_OVER        (test_flag.step_over)
#define ERR_INT         (test_flag.err_int)
#define CONT            (test_flag.cont)
#define FAIL_RE_START   (test_flag.fail_re_start)
#define CLEAR_F_BUF     (test_flag.clear_f_buf)
#define GR_OVER         (test_flag.gr_over)
#define ERR_NUM         (test_flag.err_num)
#define ERR_NUM_OVER	(test_flag.err_num_test_over)

#define OFFSET_BBD      (app_flag.offset_bbd)
#define OFFSET_NEXT     (app_flag.offset_next)

/******************************* �������Ͷ��� ********************************************/
/* ϵͳ����ʱʹ�õ���ʱ��־ */
typedef struct {
	int8_t stop_comm;             /* ����λ�����͸�λָ�� */
    int8_t start_comm;            /* ����λ����������ָ��ʱ */
	int8_t stop;                  /* �������쳣ʱֹͣ���� */
    int8_t terminate;             /* �����¸�λ�������ֹ���� */
	int8_t gradation;             /* ��־���ԵĲ�ͬ�׶� ȡֵ��ö�� TEST_STAGE */
	int8_t high_vol;              /* ��ѹ��־ 1 ��ѹ */
	int8_t pass;                  /* ���Ժϸ� 1 �ϸ� */
	int8_t fail;                  /* ��־ ���β���ʧ�� 1 ʧ�� */
    int8_t next;                  /* ������һ������ */
	int8_t cur_step_fail;         /* ��ǰ������ʧ�ܱ�־ */
	int8_t test;                  /* ��־�������״̬ */
	int8_t testing;               /* ���ڲ��Ա�־ */
	int8_t wait;                  /* ��־����ȴ� 1 ���ڵȴ� */
// 	int8_t retry;                 /* ��־���Դ��¿�ʼ����ǰ���������� */
	int8_t test_over;             /* ���Խ��� */
	int8_t step_over;             /* �������Խ��� */
    int8_t fail_re_start;         /* ʧ��ʱ�����������ı�� */
	int8_t cont;                  /* ��ǰ������Ϊ�������� */
	int8_t err_amp_cont;          /* ���ű������� */
    uint32_t err_amp_timer;        /* ���ű�����ʱ�� */
	int8_t clear_f_buf;           /* ����˲����� */
	int8_t save_only_one;         /* ÿ�β���ֻ�ܱ���һ�ν�� */
	int8_t do_only_one;           /* ��һ�� */
	int8_t pass_only_one;           /* ��һ�� */
	int8_t updata_state;          /* ���²���״̬ */
	int8_t ir_err_times;          /* ir���ޱ������� */
// 	int8_t stop_re_screen;        /* ֹͣˢ����Ļ */
	int8_t stop_collect;          /* ֹͣ���� */
	int8_t gr_over;	              /* gr���Խ�����־ */
	int8_t refresh_for_comm;      /* ͨ��Ҫ��ˢ������Ļ��Ϣ */
	int16_t dc_discharge_time;    /* ֱ���ŵ�ʱ�� */
	int8_t dis_speed;             /* ������ʾ���ٶ� */
	int8_t vol_change_flag;       /* ��ѹ�ı��־ ����½�ʱ��ѹ����ˢ���ٶ������𿨶� ���������� */
	uint8_t into_set;	          /* �˳���ǰ�˵��������뵽���ý��� */
	uint8_t into_test;	          /* �˳���ǰ�˵��������뵽���Խ��� */
    uint8_t into_auto_cal_ui;     /* �˳���ǰ�˵����� �Զ�У׼���� */
	uint8_t g_start;              /* �б���ʾʱֱ���������� */
	uint8_t g_stop;               /* �б���ʾʱֹͣ���Է��ز��Դ��� */
	
	int8_t confirm_test_init_ok;  /* ȷ�ϲ��Գ�ʼ��OK */
	int8_t stop_uart;             /* ֱֹͣ��grģ���ͨ�� */
// 	int8_t uart_ok;               /* ����ͨ�����ݽ���ok */
	int8_t uart_next;             /* ������һ������ָ�� */
	int8_t err_num;               /* ������ */
	int8_t err_num_test_over;     /* ����ʱ�䵽������ */
    int8_t test_status;           /* ����״̬ RUNING STOPING */
	int8_t bbd_time;              /* bbd��ȡ����ֵʱ�Զ�ѡ��λʱʹ�� */
	int8_t err_times;             /* �Է����Ĵ������������ֵ�ﵽ����ֵ��ͱ��� */
	int8_t re_times_per_second;   /* ��������ˢ�µĴ��� */
    int8_t record_per_second_re;  /* ��¼ÿ��ˢ�µĴ��� */
	int8_t dis_status;            /* ��ʾ״̬ */
    uint8_t dc_gr_open_err;       /* DCGR OPEN ERR */
    uint8_t dc_gr_amp_err;        /* DCGR AMP ERR */
    
    int8_t dis_charge_end;        /* �ŵ������� ���ڴ���ݷ���ʹ�� */
    uint8_t judge_err_en;         /* �쳣�ж�ʹ�� */
    uint8_t gfi_delay_count;      /* GFI���ڵ����gfi���� */ 
    uint8_t sample_task_en;       /* ��������ʹ�ܱ�־ */
    uint8_t mb_syn;               /* modbusͬ����־ */
    uint8_t mb_break_send;        /* modbus��ֹ���Ͳ���շ��Ͷ��� */
    uint8_t mb_wait_ack;          /* modbus����ȴ���Ӧ 1����Ҫ��Ӧ�� 0����Ҫ��Ӧ */
    uint8_t allow_dc_gr_fetch;    /* dc_gr�����ģ���־ */
    uint8_t high_err;             /* ���ޱ�����־ */
    uint8_t mb_master_timeout;    /* modbus������ʱ��־ */
    uint8_t vol_err_count;        /* ��ѹ�쳣�������� */
    uint8_t hight_err_count;        /* ���ޱ������� */
    uint32_t vol_rise_step_t;     /* ��ѹ�����׶�ʱ����� */
    uint8_t err_once;/* ʧ�ܱ���ִֻ��һ�� */
    uint8_t test_led_flag;/* ���Ե���˸��� */
}TEST_FLAG;

enum{
    LOADING_NONE    = 0,
    LOADING_OUT     = 1,
    LOADING_IN      = 2,
};

typedef enum{
    TEST_NORMAL,/* �������� */
    TEST_OFFSET = 1,/* ƫ�Ʋ��Ա�� */
    TEST_BBD    = 2,/* BBD GETC���Ա�� */
}CUR_TEST_STATUS;

//ʧ�ܺ���������
enum{
    FAIL_RE_START_FIRST = 1,//ʧ�ܺ�ӵ�1������
    FAIL_RE_START_NEXT = 2,//ʧ�ܺ����һ������
};

typedef struct{
	uint32_t sample_cycle;         /* ��������Ĳ������� */
	uint32_t sample_rate;         /* ������ */
	uint8_t offset_mode;          /* ��ʾƫ�Ʋ���ģʽ 1���� 0���� */
	int8_t bbd;                   /* bbd����ģʽ */
	CUR_TEST_STATUS offset_bbd;   /* offset����ģʽ */
	int8_t offset_next;           /* ��ʾ������һ��ƫ�Ʋ��� */
	uint8_t goto_menu_page;       /* ����ָ����ҳ */
    uint8_t into_test_ui;         /* ��ǽ�����Խ��� */
    uint8_t usb_loading_status;   /* ���ݵ��뵼��״̬ */
    uint8_t usb_loading_advance;  /* ���ݴ������ */
    uint8_t calibration;          /* ����У׼����ı�־ */
    uint8_t recover_flag;         /* �ָ��������ñ�־ */
    uint8_t scan_module_flag;     /* ɨ��ģ������־ */
    
    uint8_t result_real_usb;      /* ���ʵʱ���ݸ�u�� */
    
    uint16_t power_check;         /* ��Դ��� */
    uint8_t leading_watch_dog;    /* ���ݵ�������ʱ���Ź� */
    uint8_t usb_con_st;           /* U������״̬ */
    uint8_t buzzer_flicker_en;    /* ���������쿪�� */
    uint8_t into_get_cap;         /* �����ȡ���ݽ��沢�Զ����в��� */
    uint8_t into_get_offset;      /* �����ȡƫ�ƽ��沢�Զ����в��� */
	uint8_t auto_cal_st;		  /* �Զ�У׼״̬0�����Զ�У׼״̬ 1���Զ�У׼״̬ */
    
	int8_t dc_gr_module_status;   /* CAL_RUNING CAL_STOPING */
	uint8_t res_list_num_count;   /* ����б����Ѿ�����Ľ����Ŀ */
	uint8_t dc_gr_task_send_idle; /* ֱ������������б�� */
	uint8_t absorptance;/* IR���ձȿ��� */
}APP_FLAG;

/* �˲������� */
typedef struct{
#define SF_DEPTH            3     /* ƽ���˲������ ̫�������������� ���6 */
	u32 sf_buf_u[3][SF_DEPTH];    /* ����ƽ���˲� */
	float af_buf_f[3];            /* �����޷��˲� */
}F_BUF_STR;


/************************* ����ȫ�ֱ��� ***************************/
#ifdef   _SERVE_TEST_GLOBALS
#define  _SERVE_TEST
#else
#define  _SERVE_TEST  extern
#endif

_SERVE_TEST	__IO TEST_FLAG	test_flag;   /* ����ȫ�ֱ�־������ʹ�� */
_SERVE_TEST	__IO APP_FLAG	app_flag;    /* ��������ȫ�ֱ�־������ʹ�� */
_SERVE_TEST	F_BUF_STR	f_buf;	         /* �˲����� */

_SERVE_TEST uint32_t cpld_cmd;           /* cpld������ */
_SERVE_TEST uint32_t cpld_base;          /* cpldƵ�ʻ��� */

/****************************************************************/
//���ʵʱ���ݶ���
#define RES_QUEUE_MAX   5
_SERVE_TEST uint16_t result_r_queue[RES_QUEUE_MAX];
_SERVE_TEST uint8_t result_r_queue_h;/* ����ͷ */
_SERVE_TEST uint8_t result_r_queue_t;/* ����β */

/****************************************************************/

/* ���AD�ɼ��ĵ�ѹ�͵��� */
_SERVE_TEST uint16_t sample_vol;         /* ������ѹ */
_SERVE_TEST uint16_t sample_cur;         /* �������� */
_SERVE_TEST float cur_ad_vol;            /* �������� */
_SERVE_TEST uint16_t adc_value[2];       /* �������� */

_SERVE_TEST uint8_t output_buf[10];      /* ����ɼ�ֵ */
_SERVE_TEST uint8_t loop_buf[10];        /* ��·�ɼ�ֵ */
_SERVE_TEST uint8_t real_buf[10];        /* ��ʵ�����ɼ�ֵ */
_SERVE_TEST uint8_t time_buf[10];        /* ����ʱ�� */

_SERVE_TEST float vol_ave;               /* ��ѹ */
_SERVE_TEST float cur_ave;               /* ���� */
_SERVE_TEST float res_ave;               /* ���� */
_SERVE_TEST float real_ave;              /* ��ʵ���� */
_SERVE_TEST float cap_ave;               /* ���� */
_SERVE_TEST uint16_t g_ad_dog;			 /* ad���Ź� */

_SERVE_TEST float err_vol_bak; /* ����ʱ��ѹֵ */
_SERVE_TEST float err_cur_bak; /* ����ʱ����ֵ */
_SERVE_TEST float err_res_bak; /* ����ʱ���豸�� */
_SERVE_TEST float err_real_bak;/* ����ʱ��ʵ�������� */
_SERVE_TEST float err_cap_bak; /* ����ʱ���ݱ��� */

_SERVE_TEST float ir_15s_res;			 /* IR���� 15sʱ�ĵ���ֵ */
_SERVE_TEST float ir_60s_res;			 /* IR���� 60sʱ�ĵ���ֵ */
_SERVE_TEST float ir_absorptance;		 /* IR�������ձ� */
_SERVE_TEST uint32_t gear_up_limit;      /* ir�Զ������رպ������ֵ */
_SERVE_TEST uint32_t g_ir_dly;           /* ir������ʱ */
_SERVE_TEST uint16_t g_plc_dly;          /* PLC��ʱ�ر� */
_SERVE_TEST void (*test_irq_fun) (void);
_SERVE_TEST void (*cs99xx_test_fun) (void);

_SERVE_TEST uint32_t uart_err_num;       /* ����ͨ��ʧ�ܵĴ��� */ //load_data

_SERVE_TEST	__IO uint32_t g_test_time;   /* ����ʱ��Ƭ �ڶ�ʱ���жϷ�������б��Լ� */
_SERVE_TEST	__IO uint16_t g_dis_time;    /* ������ʾ��ǰ���Խ׶εļ�ʱʱ�� */
_SERVE_TEST	uint32_t g_buzzer_time;	     /* ��������ĳ��� */
_SERVE_TEST	uint32_t g_cylinder_ctrl_t;	     /* ���׿���״̬��ʱ */

_SERVE_TEST uint16_t dly_t;              /* �����ʱ */
_SERVE_TEST uint16_t zeo_t;              /* �ک��׶ε��ۼ�ʱ�� */
_SERVE_TEST uint16_t one_t;              /* ��һ�׶ε��ۼ�ʱ�� */
_SERVE_TEST uint16_t two_t;              /* �ڶ��׶ε��ۼ�ʱ�� */
_SERVE_TEST uint16_t thr_t;              /* �����׶ε��ۼ�ʱ�� */
_SERVE_TEST uint16_t for_t;              /* ���Ľ׶ε��ۼ�ʱ�� */
_SERVE_TEST uint16_t fiv_t;              /* ����׶ε��ۼ�ʱ�� */
_SERVE_TEST uint16_t tes_t;              /* ��ǰ������ʱ�� */
_SERVE_TEST uint16_t ris_t;              /* ����ʱ�� */
_SERVE_TEST uint16_t sta_t;              /* ��ѹʱ�� */
_SERVE_TEST uint16_t fal_t;              /* �½�ʱ�� */
_SERVE_TEST uint16_t int_t;              /* ���ʱ�� */
_SERVE_TEST uint16_t cha_t;              /* ����ʱ�� */

_SERVE_TEST uint16_t cur_step;           /* ��ǰ���Ĳ��� */
_SERVE_TEST uint8_t cur_mode;            /* ��ǰ����ģʽ */
_SERVE_TEST uint8_t cur_offset_result;   /* ��ǰ����ƫ�ƽ�� */
_SERVE_TEST uint8_t cur_getc_result;     /* ��ǰ���Ļ�ȡ���ݽ�� */
_SERVE_TEST uint8_t cur_t_time;          /* ����ʱ�� */
_SERVE_TEST uint8_t cur_status;          /* ��ǰ״̬ */
_SERVE_TEST uint8_t cur_cylinder_ctrl_status;          /* ��ǰ���׿���״̬���ƻ�ר�� */
_SERVE_TEST uint8_t cur_status_plc;      /* ��ǰ״̬ */
_SERVE_TEST uint8_t cur_plc_err_st;		 /* PLC����״̬ */
_SERVE_TEST uint8_t cur_comm_bps;        /* ��ǰ��ͨ�Ų����� */
_SERVE_TEST uint16_t updata_comm_flag;    /* ����ͨ�ű�־ */

_SERVE_TEST uint16_t cur_vol;            /* ��ǰ���ĵ�ѹֵ */
_SERVE_TEST float cur_cap;               /* ��ǰ���ĵ���ֵ */
_SERVE_TEST uint16_t cur_open_ratio;     /* ��ǰ���Ŀ�·����ֵ */
_SERVE_TEST uint16_t cur_short_ratio;    /* ��ǰ���Ķ�·����ֵ */
_SERVE_TEST uint8_t cur_auto;            /* ��ǰir �Զ����� */
_SERVE_TEST int8_t steps_con;            /* �������� */
_SERVE_TEST int8_t steps_pass;           /* ����PASS */
_SERVE_TEST uint8_t cur_gear;            /* ��ǰ��������λ */
_SERVE_TEST uint8_t cur_gear_max;        /* ��ǰ��������λ���ֵ */
_SERVE_TEST uint8_t cur_gear_min;        /* ��ǰ��������λ��Сֵ */
_SERVE_TEST uint8_t cur_soft_gear;       /* ��ǰIR��λ */
_SERVE_TEST uint8_t vol_segment;         /* ��ѹ�� 0 10-500v 1 100-MAX */
_SERVE_TEST uint8_t contact_segment;	 /* ˫�����ѹ������ֶα�� 0 ��һ�� 1�ڶ��� */
_SERVE_TEST uint8_t cur_segment;         /* ������ GR DV */
_SERVE_TEST uint8_t cur_gear_bak;        /* ��λ������λ����ֵ */
_SERVE_TEST uint8_t cur_ir_dis_gear;     /* �Զ����ر�ʱʹ�� */
_SERVE_TEST uint8_t cur_ir_dis_gear_0;   /* �Զ����ر�ʱʹ�� */
_SERVE_TEST uint8_t cur_ir_dis_gear_1;   /* �Զ����ر�ʱʹ�� */
_SERVE_TEST uint32_t g_ir_gear_hold; 	 /* �Զ�����λ����ʱʹ�� */
_SERVE_TEST uint8_t cur_gear_comm;       /* ��ǰ��������λ */
_SERVE_TEST uint8_t cur_real_gear_comm;  /* ��ǰ��������λ */
_SERVE_TEST uint8_t cur_method;          /* GR���Է��� GR_V_CONT_MODEL GR_V_PULSE_MODEL */
_SERVE_TEST uint16_t res_ave_comm;       /* IR����Ϊͨ���� */
_SERVE_TEST uint16_t cur_ave_comm;       /* GR����Ϊͨ���� */
_SERVE_TEST uint8_t cur_output_impedan;  /* DCW ����迹 */
_SERVE_TEST uint16_t cur_intensity;      /* CC��ǰ����ǿ�� */

_SERVE_TEST float cur_high;              /* ��ǰ����ֵ */
_SERVE_TEST float cur_low;               /* ��ǰ����ֵ */
_SERVE_TEST float cur_ir_rang_h;         /* ��ǰ����ֵ */
_SERVE_TEST uint16_t cur_charge_cur;     /* ��ǰ������ */
_SERVE_TEST uint16_t cur_real_cur;       /* ��ǰ��ʵ���� */
_SERVE_TEST uint16_t cur_delay_t;        /* ��ʱ����ʱ�� */
_SERVE_TEST uint8_t cur_auto;            /* ��ǰir �Զ����� */
_SERVE_TEST uint16_t cur_arc_gear;       /* ��ǰ�绡�ȼ� �����ģʽ�µĵ��� */
_SERVE_TEST uint16_t cur_frequency_gear; /* ���Ƶ�ʵ�λѡ�� 1-9 */
_SERVE_TEST uint16_t cur_frequency;      /* ���Ƶ�� */
_SERVE_TEST TEST_PORT cur_port;          /* ��ǰ�Ķ˿� */
_SERVE_TEST uint8_t cur_auto;            /* ��ǰir �Զ����� */

_SERVE_TEST int8_t steps_con;	         /* �������� */
_SERVE_TEST int8_t steps_pass;           /* ����PASS */
_SERVE_TEST uint16_t cur_offset;         /* ��ǰƫ�Ƶ�������� */
_SERVE_TEST uint16_t cur_real_offset;    /* ��ǰ�����ǵ���ƫ�� */
_SERVE_TEST uint8_t cur_vol_gear;        /* ��ǰir��ѹ��λ 0-9 */

_SERVE_TEST uint8_t err_charge;          /* ������ */
_SERVE_TEST uint8_t err_real;            /* ��ʵ�������� */

_SERVE_TEST float cur_dac_k;             /* ��ǰDACУ׼ϵ��k */
_SERVE_TEST float cur_dac_b;             /* ��ǰDACУ׼ϵ��b */
_SERVE_TEST float cur_adc_k;             /* ��ǰADCУ׼ϵ��k */
_SERVE_TEST float cur_adc_b;             /* ��ǰADCУ׼ϵ��b */
_SERVE_TEST float cur_adc_cur_k;         /* ��ǰ ����ADCУ׼ϵ��k */
_SERVE_TEST float cur_arc_k;             /* ��ǰARC����У׼ϵ��k */
_SERVE_TEST float cur_res_k;             /* ��ǰIR���赲λ��У׼ϵ��k */
// _SERVE_TEST float cur_ir_second_cal_k_1; /* ��ǰ IR����У׼ϵ�� */
// _SERVE_TEST float cur_ir_second_cal_b_1; /* ��ǰ IR����У׼ϵ�� */
// _SERVE_TEST float cur_ir_second_cal_k_2; /* ��ǰ IR����У׼ϵ�� */
// _SERVE_TEST float cur_ir_second_cal_b_2; /* ��ǰ IR����У׼ϵ�� */

_SERVE_TEST  float vol_ch_step;	/* ��ѹ���䲽��ֵ */
_SERVE_TEST  float vol_ch_base;	/* ��ѹ�仯����ʼֵ */
_SERVE_TEST  float vol_ch_target;	/* ��ѹ�仯��Ŀ�� */



/******************************* �������� *********************************/
extern void clear_test_flag(void);
extern int32_t dis_charge_remain_vol(void);
extern void record_exception_scene(void);
extern void recover_exception_scene(void);
extern void recover_exception_scene(void);
extern void close_test_timer(void);
extern void open_test_timer(void);
extern void install_test_irq_fun(void);
extern void count_vol_step_value(void);
extern void test_g_details(void);

extern void cur_cylinder_ctrl_stop(void);

extern void offset_state_dis(void);

extern void dis_cur_page(const int16_t n, const int16_t cur_page_num);
extern void dis_test_status(uint8_t status, const GUI_FONT * pNewFont);
extern void dis_test_result_status(uint8_t status, const GUI_FONT * pNewFont);
extern void startup(void);
extern void stop_test(void);
extern void test_pass(void);
extern void test_fail(void);
extern void ir_set_gear(const uint8_t gear, uint8_t soft_gear);
extern void bbd_auto_select_gear(void);
extern void bbd_load_gear(void);
extern void shift_cur_gear(uint8_t mode);
extern void exception_handling(int8_t errnum);
extern void exception_handlingx(int8_t status);

extern void init_sys_flag(void);
extern void count_dis_value(void);

extern void irq_stop_relay_motion(void);
extern void relay_motion(void);
extern void exit_test_relay_motion(void);
extern void updata_time(const int8_t n, uint16_t time);

extern void test_status_dis(void);
extern void test_dis_err_data(void);
extern void test_dis(void);
extern int8_t offset_dis(const int8_t n);

extern void dis_offset_cur(const int8_t n, const float o_cur, const float o_real_cur);
extern void dis_test_ir_res(void);
extern void transform_test_ir_res(void);
extern void transform_test_ir_res_loop(void);
extern void dis_test_vol1(const GUI_FONT * pNewFont);
extern void dis_test_time(const GUI_FONT * pNewFont);
extern void dis_test_gr_cur(void);
extern void dis_test_gr_vol(void);
extern void transform_test_gr_vol(void);
extern u32 smoothing_filtering(const u8 sf_n,const u32 val);

extern void de_reset(void);
extern void test_reset(void);
extern void transform_test_vol_string(void);
extern void transform_test_loop_string(void);
extern void transform_test_time_string(uint16_t time);
extern void count_fall_vol_step_value(void);
extern void count_rise_vol_step_value(void);
extern void count_vol_ch_step(void);
extern void vol_change(void);
extern void vol_change_200hz(uint32_t time);
extern void vol_change_1000hz(uint32_t time);
extern uint8_t get_gr_gear(void);
extern void load_ratio(uint8_t mode);
extern void load_data(void);//test_ready
extern void cs99xx_big_cap_test(void);
extern void cs99xx_test(void);
extern void cs99xx_g_test(void);
extern void offset_test(void);
extern void confirm_vol_segment(void);

extern void set_sample_rate(uint32_t rate);

extern void test_irq(void);
extern void test_big_cap_irq(void);
extern void test_g_irq(void);

extern void open_hv(void);
extern void close_hv(void);

extern void dis_test_data(const GUI_FONT * pNewFont);
extern void dis_test_vol1(const GUI_FONT * pNewFont);
extern int32_t check_test_mode(NODE_STEP * p);

extern void syn_dis_test_vol(const uint8_t x, const uint8_t y, const GUI_FONT* pfont);
extern void syn_dis_test_cur(const uint8_t x, const uint8_t y, const GUI_FONT* pfont);
extern void syn_dis_offset_cur(const int8_t n, const float o_cur, const float o_real_cur);

extern void dis_offset_test_ports(const TEST_PORT *pport);
extern void serve_test_poll_task(void);
extern void wait_dc_gr_send_task_idle(void);

#endif //__SERVE_TEST_H__

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
