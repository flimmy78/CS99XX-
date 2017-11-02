/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�99xx_struct.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#ifndef __99xx_STURCT_H__
#define __99xx_STURCT_H__

#if defined(__cplusplus)
    extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/******************* �����ļ� *********************/
#include "stm32f10x.h"
#include "cs99xx_config.h"

/******************* �궨��***********************/
#define FLAG_FLAG			((uint32_t)0x5A5AA5A5)  /* eep������� ������ʱ���� */
#define MAX_STEPS           (99)          /* �����ļ���������� */
#define TEST_PORTS_MAX      (8 * 2) /* ���Զ˿ڵ������Ŀ ������8�ı��� */
#define STEP_LIST_SIZE      ((uint16_t)5) /* �������������ĸ��� */

#define MAX_FILES			((uint16_t)31) /* ����ļ���Ŀ����һ��Ĭ�ϵ��ļ� */
#define NAME_LON			((uint16_t)14) /* �ļ����ĳ��� */

#define PER_PAGE_RES_NUM 	((uint16_t)4) /* ��Ļÿҳ����ʾ�Ľ���� */
#define PW_LON				((uint16_t)8) /* ����Ϊ 8���ַ��������볤��PW_LON */


/****************���ݽṹ����********************/
enum{
	CHINESE = 1,            /* ���� */
	ENGLISH = 0,            /* Ӣ�� */
	
	G_MODE = 0,             /* ����ģʽ Gģʽ */
	N_MODE = 1,             /* ����ģʽ Nģʽ */
	
	ARC_CUR_MODEL 	= 0,    /* ����ģʽ */
	ARC_GRADE_MODEL = 1,    /* �ȼ�ģʽ */
	
	FLOAT_MODE	= 0,        /* ���� ģʽ */
	GND_MODE	= 1,        /* �ӵ� ģʽ */
	
	GFI_OFF		= 0,        /* GFI�ر� */
	GFI_ON		= 1,        /* GFI�� */
	
	PLC_OFF		= 0,        /* GFI�ر� */
	PLC_ON		= 1,        /* GFI�� */
    
	DC_GR_PULSE = 0,        /* ֱ��GR������� */
	DC_GR_CONT	= 1,        /* ֱ��GR�������� */
	
	GR_V_PULSE_MODEL = 0,   /* �������ģʽ */
	GR_V_CONT_MODEL  = 1,   /* ��������ģʽ */
};
enum{
    AUTO_SHIFT_OFF,         /* IR�Զ������ر� */
    AUTO_SHIFT_ON,          /* IR�Զ������� */
};
typedef enum {
	NO = 0, 
	YES = !NO ,
} Single_Switch;

/* ����8�����Զ˿����� */
typedef struct {
	uint16_t port1:2;
	uint16_t port2:2;
	uint16_t port3:2;
	uint16_t port4:2;
	uint16_t port5:2;
	uint16_t port6:2;
	uint16_t port7:2;
	uint16_t port8:2;
}_TEST_PORT;

typedef struct{
    _TEST_PORT ports[TEST_PORTS_MAX / 8];
    uint8_t num;
    uint8_t mode;/* ͨ��ʱʹ�� */
}TEST_PORT;

/* ACW���� */
typedef struct {
	uint16_t 	step;                       /* ���Բ� */
	uint8_t 	mode;                       /* ģʽ */
	uint16_t 	testing_voltage;            /* ���Ե�ѹ */
	uint8_t 	gear_i;                     /* ������λ  200uA   2mA   20mA   10mA   100mA */
	uint16_t	upper_limit;				/* �������� */
	uint16_t	lower_limit;				/* �������� */
	uint16_t	ac_real_cur;                /* ��ʵ��������ֵ */
	uint16_t	arc_sur;					/* �绡��� �����ǵȼ�ģʽ������ ����Ϊ����ģʽ��Ϊ������ ����е绡���ģʽ���� */
	uint16_t	output_freq;				/* ���Ƶ��ѡ�� ��������Ҳ�����ǵȼ� */
	uint16_t	rise_time;                  /* ����ʱ�� */
	uint16_t	testing_time;               /* ����ʱ�� */
	uint16_t	fall_time;                  /* �½�ʱ�� */
	uint16_t	interval_time;              /* ���ʱ�� */
	uint8_t 	steps_pass;					/* ����pass */
	uint8_t 	steps_cont;					/* �������� */
	TEST_PORT	port;                       /* �˿� */
	
	uint16_t	offset_cur;                 /* ƫ�Ƶ��� */
	uint16_t 	offset_real;                /* ƫ����ʵ���� */
	uint8_t 	offset_result;              /* ƫ�Ƶ���������� OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}ACW_STRUCT;

/* CC���� */
typedef struct {
	uint16_t 	step;                       /* ���Բ� */
	uint8_t 	mode;                       /* ģʽ */
	uint16_t 	testing_voltage;            /* ���Ե�ѹ */
	uint8_t 	gear_i;                     /* ������λ  200uA   2mA   20mA   10mA   100mA */
	uint16_t	upper_limit;				/* �������� */
	uint16_t	lower_limit;				/* �������� */
	uint16_t	ac_real_cur;                /* ��ʵ��������ֵ */
	uint16_t	cur_intensity;              /* ����ǿ�� �� �� �� */
	uint16_t	output_freq;      			/* ���Ƶ��ѡ�� ��������Ҳ�����ǵȼ� */
	uint16_t	rise_time;                  /* ����ʱ�� */
	uint16_t	testing_time;               /* ����ʱ�� */
	uint16_t	fall_time;                  /* �½�ʱ�� */
	uint16_t	interval_time;              /* ���ʱ�� */
	uint8_t 	steps_pass;         		/* ����pass */
	uint8_t 	steps_cont; 				/* �������� */
	TEST_PORT	port;                       /* �˿� */
	
	uint16_t	offset_cur;                 /* ƫ�Ƶ��� */
	uint16_t 	offset_real;                /* ƫ����ʵ���� */
	uint8_t 	offset_result;              /* ƫ�Ƶ���������� OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}CC_STRUCT;

/* DCW �Ĳ��� */
typedef struct {
	uint16_t     step;                      /* ���Բ� */
	uint8_t     mode;                       /* ���Բ� */
	uint16_t 	testing_voltage;            /* ���Ե�ѹ */
	uint8_t 	gear_i;                     /* ������λ   2uA ,  20uA , 200uA ,  2mA,   10mA */
	uint16_t	upper_limit;				/* �������� */
	uint16_t	lower_limit;				/* �������� */
	uint16_t	charge_cur;         		/* ������ */
	uint16_t	arc_sur;   					/* �绡��� */
    uint8_t     output_impedance;           /* ����迹 */
	uint16_t	delay_time;                 /* ��ʱʱ�� ��ʱʱ�䵽�˲��ж� */
	uint16_t	rise_time;                  /* ����ʱ�� */
	uint16_t	stab_time;                  /* ��ѹʱ�� */
	uint16_t	testing_time;               /* ����ʱ�� */
	uint16_t	fall_time;                  /* �½�ʱ�� */
	uint16_t	interval_time;              /* ���ʱ�� */
	uint8_t 	steps_pass;         		/* ����pass */
	uint8_t		steps_cont; 				/* �������� */
	TEST_PORT	port;                       /* �˿� */
	
	uint16_t	offset_cur;                 /* ƫ�Ƶ��� */
	uint8_t 	offset_result;              /* ƫ�Ƶ���������� OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}DCW_STRUCT;

/* IR ���� */
typedef struct {
	uint16_t 	step;                       /* ���Բ� */
	uint8_t 	mode;                       /* ģʽ */
	uint16_t 	testing_voltage;			/* �����ѹ */
	uint8_t 	auto_shift_gears;           /* �Զ����� */
	uint32_t 	upper_limit;     			/* �������� */
	uint32_t 	lower_limit;	    		/* �������� */
	uint16_t	rise_time;	                /* ����ʱ�� */
	uint16_t	testing_time;	            /* ����ʱ�� */
	uint16_t	delay_time;	                /* ��ʱʱ�� */
	uint16_t	interval_time;	            /* ���ʱ�� */
	uint8_t 	steps_pass;	        		/* ����pass */
	uint8_t 	steps_cont;					/* �������� */
	TEST_PORT	port;	                    /* �˿� */
}IR_STRUCT;

/* GR���� */
typedef struct {
	uint16_t 	step;
	uint8_t 	mode;
	uint8_t		voltage_gear;               /* ��ѹ��λ 1 20mv 2 200mv 3 2000mv */
	uint16_t	testing_cur;                /* ������� */
	uint16_t 	upper_limit;				/* �������� */
	uint16_t 	lower_limit;				/* �������� */
	uint16_t	testing_time;               /* ����ʱ�� */
	uint16_t	interval_time;              /* ���ʱ�� */
	uint8_t 	steps_pass;					/* ����pass */
	uint8_t 	steps_cont; 				/* �������� */
	uint16_t	output_freq;				/* ���Ƶ�� */
	uint16_t	test_method;                /* ���������ģʽʹ�����Ժ� 0��ʾ������� 1��ʾ�������� */
	
	uint16_t	offset_res;                 /* ƫ�Ƶ��� */
	uint8_t	    offset_result;              /* ƫ�Ƶ���������� OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}GR_STRUCT;

/* BBD���� */
typedef struct {
	uint16_t 	step;                       /* ���ò� */
	uint8_t 	mode;                       /* ģʽ */
	uint16_t 	testing_voltage;            /* �����ѹ */
	uint16_t	open_ratio;                 /* ��·���� 0��10%-100% */
	uint16_t	short_ratio;                /* ��·���� 100% - 500% */
	uint16_t 	upper_limit;				/* �������� */
	uint16_t 	lower_limit;				/* �������� */
	uint16_t	output_freq;      			/* ���Ƶ�� */
	uint16_t	testing_time;               /* ����ʱ�� */
	uint16_t	interval_time;	            /* ���ʱ�� */
	uint8_t 	steps_pass;	        		/* ����pass */
	uint8_t 	steps_cont;					/* �������� */
	float		cap_value;                  /* ����ֵ */
	uint8_t 	gear;                       /* �����ȡ����ֵʱ�ĵ�����λ Ĭ�� 20mA */
	TEST_PORT	port;	                    /* �˿� */
	
	uint8_t 	get_cs_ok;                  /* ��ǻ�ȡ�����Ѿ�OK */
    
	float   	offset_cap;                 /* ƫ�Ƶ��� */
	uint8_t	    offset_result;              /* ƫ�Ƶ���������� OFFSET_PASS OFFSET_FAIL OFFSET_NONE */
}BBD_STRUCT;

/* �������� */
typedef struct {
	uint16_t step;                           /* ���ò� */
	uint8_t mode;                            /* ģʽ */
}COMMON;

/* ��ÿһ��������Ϊһ��������ʵ��ͳһ���� */
typedef union{
	COMMON      com;
	ACW_STRUCT 	acw;
	DCW_STRUCT 	dcw;
	IR_STRUCT 	ir;
	GR_STRUCT 	gr;
	BBD_STRUCT  bbd;
    CC_STRUCT   cc;
}UN_STRUCT,*PUN_STRUCT,**PPUN_STRUCT;

/* ��·ͬʱ�����õ��������� */


/* �����Ľڵ� */
typedef struct Node_Step{
	UN_STRUCT 	one_step;                   /* ���ṹ */
	struct Node_Step *prev;                 /* ǰָ�� */
	struct Node_Step *next;                 /* ��ָ�� */

}NODE_STEP;

/* ���Բ��������� */
typedef struct List_Step{
	NODE_STEP	*head;                      /* ͷ */
	NODE_STEP	*tail;                      /* β */
	uint8_t 	size;                       /* ��С */
}LIST_99xx;

/* �����鶨�� */
typedef struct TestGroup{
	NODE_STEP test_steps[STEP_LIST_SIZE];
}TESTGROUP;

/* �ļ�ͷ�Ķ��� */
typedef struct Test_File{
	uint8_t num;                            /* �ļ���ţ�ÿ���ļ�����Ψһ�ı�� 0 Ĭ���ļ� 1-30�û��ļ� */
	char name[NAME_LON+1];	                /* �ļ����� */
	uint8_t work_mode;	                    /* 1��ʾN 2��ʾG */
	uint16_t hold_pass_time;                /* pass�źű���ʱ�� */
	uint16_t pass_buzzing_time;             /* pass����������ʱ�� */
	uint8_t arc_mode;	                    /* 1����ģʽ2�ȼ�ģʽ */
	uint16_t total;	                        /* ��Ÿ��ļ���һ�������˼��� */
	uint32_t create_time;                   /* ����ʱ�� */
}TEST_FILE;

typedef struct{
    uint8_t addr[16];/* ��ַ���� */
    uint8_t count;/* ģ��������� */
}DC_MODULE_USING_INFO;

/* ϵͳ�������� */
typedef struct Sys_Parameter{
	uint8_t contrast;			/* Һ���Աȶ�(0-9) */
	uint8_t allowance;			/* ������ʾ (0-9)*/
	uint8_t is_save_res;		/* �Ƿ񱣴�����0�����棬1���� */
	uint8_t is_overflow_cover;	/* �Ƿ�������� */
	
	uint8_t is_gfi_protect;		/* �Ƿ�GFI���� */
	uint8_t is_self_check;		/* �Ƿ�򿪿����Լ칦�� */
	uint8_t language;			/* �����л���1���ģ�0Ӣ�� */
	uint8_t is_falt_continue;	/* ʧ�ܼ����Ƿ�ʧ�ܼ�����0��������1���� */
    uint8_t fail_mode;          /* ʧ��ģʽ */
	uint8_t test_method;		/* ���Է��� 1�ӵ� 0���� */
	uint8_t test_level;			/* ���Ե�ƽ 1��ƽ 0���� */
	
	char 	data_of_survey[21];	/* ����ʱ�� */
	uint8_t is_table_dis;		/* �Ƿ���б���ʾ���� 0���رգ�1�� */
	uint8_t num_rule;			/* ��Ź��� (0-2) */
	uint8_t buzzer_sw;			/* ���������� */
	
	uint8_t commu_on_off;		/* ͨ�ſ��� 1�� 0�ر�*/
	uint8_t baud_rate_gear;		/* �����ʣ�1��ʾ9600 2��ʾ14400 3��ʾ19200 */
	uint8_t local_addr;			/*������ַ (1-255) */
	
	uint8_t plc_on_off;			/* PLC���� 1�򿪣�0�ر� */
	char pass_word[PW_LON+1];	/* �������8λ������1λ */
	
	uint16_t used_res_num;		/* �Ѿ���¼�Ľ������ ����ͳ�ƽ��*/
	uint16_t pass_res_num;		/* �ϸ���� ����ͳ�ƽ�� */
	uint16_t cover_res_num;		/* ��used_res_num����n�������������Ǿ������ü����������� �������4500�������� */
	
	uint8_t keyboard_lock;		/* ������ */
	
    char dut_name[NAME_LON+1];	/* ������� */
	uint8_t silent_sw;			/* �������� */
    uint8_t plc_signal;         /* plc �ź� ��ʧ�ܼ����򿪺����plc_signal=each_step ��ʾÿ�����ᷢPlc�ź�
                                   ���һ��Ҳ�ǣ���� plc_signal=all_steps ��ʾ���һ����������plc�ź�
                                   ������һ���ۺ��ж� */
    uint16_t output_delay;		/* �����ʱ �û�Ҫ��������ȴ�һ��ʱ���������ѹ */
    uint16_t ir_gear_hold;		/* IR�л���λ��ʱ */
	uint8_t ir_speed_sw;		/* IR�����ٶ� �� �� �� ͨ�������˲����ʵ�� */
    DC_MODULE_USING_INFO    dc_module_using_info;/* ����ʹ�õ�DCģ���ַ��Ϣ */
}SYS_PAR;


typedef struct {
    uint16_t vol;		/* ���Ե�ѹ */
    uint8_t gear;		/* ������λ */
    uint8_t real_gear;  /* ��ʵ�ĵ�����λ Ϊ���������е�λ�ؼ���� wangxin 2015.08.10 */
    uint16_t cur;		/* ���Ե��� */ 
    uint16_t real_cur;	/* ��ʵ���� */
}ACW_RES;

typedef struct {
    uint16_t vol;		/* ���Ե�ѹ */
    uint8_t gear;		/* ������λ */
    uint8_t real_gear;  /* ��ʵ�ĵ�����λ Ϊ���������е�λ�ؼ���� wangxin 2015.08.10 */
    uint16_t cur;		/* ���Ե��� */
    uint16_t real_cur;	/* ��ʵ���� */
}CC_RES;

typedef struct {
    uint16_t vol;		/* ���Ե�ѹ */
    uint8_t gear;		/* ������λ */
    uint8_t real_gear;  /* ��ʵ�ĵ�����λ Ϊ���������е�λ�ؼ���� wangxin 2015.08.10 */
    uint16_t cur;		/* ���Ե��� */
    uint16_t real_cur;	/* ��ʵ���� */
}DCW_RES;

typedef struct {
    uint16_t vol;		/* ���Ե�ѹ */
    uint8_t gear;		/* ���赵λ */
    uint32_t res;		/* ��Ե���� */
    uint16_t out_range;	/* ���賬�� */
	float absorptance;/* ���ձ� */
}IR_RES;

typedef struct {
    uint16_t cur;		/* ���Ե��� */
    uint16_t vol;		/* ���Ե�ѹ */
    uint8_t gear;		/* ��ѹ��λ */
    uint16_t res;		/* �ӵص��� */
    uint16_t real_cur;	/* ��ʵ���� */
}GR_RES;

typedef struct {
    uint16_t vol;		/* ���Ե�ѹ */
    uint16_t open_r;	/* ��·��� */
    uint16_t short_r;   /* ��·��� */
    uint32_t cap;		/* ���ݵ�λpF */
    uint32_t base_cap;  /* ��׼���ݵ�λ pF */
    
}BBD_RES;

typedef enum{
	RES_FAIL,
	RES_PASS,
}RES_STR;

typedef struct{
    uint8_t num[5];
    uint8_t step[11];
    uint8_t dut_name[16];
    uint8_t file_name[16];
    uint8_t work_mode[2];
    uint8_t test_mode[4];
    uint8_t test_out[9];//���
    uint8_t test_loop[9];//��·�ɼ�
    uint8_t real_cur[9];//��ʵ����
    uint8_t range[9];//������λ
    uint8_t result[2];
    uint8_t test_time[9];//����ʱ��
    uint8_t time[21];
}RESULT_STRING_STR;

typedef struct {
    char dut_name[NAME_LON+1];			/* ������� */
    uint8_t file_name[NAME_LON+1];		/* �����ļ����� */
    uint16_t step;						/* ���β��ԵĲ����� */
    uint16_t total;						/* �ܲ��Բ����� */
    uint8_t work_mode;					/* ����ģʽ */
    uint8_t mode;						/* ����ģʽ */
    
    union {
        ACW_RES acw;
        DCW_RES dcw;
        IR_RES ir;
        GR_RES gr;
		BBD_RES bbd;
		CC_RES cc;
    }un;
    
    uint16_t test_time;					/* ����ʱ�� */
    RES_STR result;						/* ���Խ�� */
	uint8_t err_num;					/* ������ */
	uint32_t time;						/* �����������ʱ�� */
	

}RESULT_STR;



/* ϵͳ��פ��־ */
typedef struct {
	uint8_t mem_init;		            /* �洢����ʼ����־ */
	uint8_t last_group;		            /* ���һ��ʹ�õļ����� */
	uint32_t boot_times;	            /* �������� */
	uint32_t amp_times;		            /* ���ż̵������ϴ���ͳ�� */
	uint32_t key_start_times;           /* ���������µĴ��� */
	uint32_t key_stop_times;            /* ��λ�����µĴ��� */
	uint32_t flag_flag;		            /* ��������ñ�������Ԥ��ֵ��ͬ�ͳ������� */
	uint32_t res_count;					/* ������� */
	uint32_t last_res_date;				/* ���һ������ı���ʱ�� ��ʱ�������ж��Ƿ�Ҫ���� res_count */
}SYS_FLAG;

typedef struct {
	uint8_t mem_init;		            /* �洢����ʼ����־ */
	uint8_t last_group;		            /* ���һ��ʹ�õļ����� */
	uint32_t boot_times;	            /* �������� */
	uint32_t amp_times;		            /* ���ż̵������ϴ���ͳ�� */
	uint32_t key_start_times;           /* ���������µĴ��� */
	uint32_t key_stop_times;            /* ��λ�����µĴ��� */
	uint32_t flag_flag;		            /* ��������ñ�������Ԥ��ֵ��ͬ�ͳ������� */
}SYS_FLAG_T;
    

/* ϵͳ���ݲ����ĳ��� */
typedef struct{
    uint8_t sys_par;
    uint8_t sys_flag;
    uint8_t test_file;
    uint16_t group_bitmap;
    uint32_t group_map_table;
    uint8_t step;
    uint8_t result;
}SYS_DATA_LEN_T;

typedef struct{
    uint16_t used_res_num;		/* �Ѿ���¼�Ľ������ ����ͳ�ƽ��*/
    uint16_t pass_res_num;		/* �ϸ���� ����ͳ�ƽ�� */
    uint16_t res_str_size;      /* ��Ž����Ϣ�ṹ���С */
}RESULT_INFO_T;


typedef struct{
    uint16_t sample_vol_ad;///<��ѹ����AD
    uint16_t sample_cur_ad;///<��������AD
    float vol_value;///<��ѹ��ʾֵ
    float cur_value;///<������ʾֵ
    float real_value;///<��ʵ������ʾֵ
    float res_value;///<������ʾֵ
    float cap_value;///<������ʾֵ
    uint8_t vol_segment;///<��ѹ��
    uint16_t output_da;///<���DAֵ
    float out_da_k;///<��ѹ/����DAУ׼kֵ
    float out_da_b;///<��ѹ/����DAУ׼bֵ
    float vol_adc_k;///<��ѹADУ׼kֵ
    float vol_adc_b;///<��ѹADУ׼bֵ
    float cur_adc_k;///<����ADУ׼kֵ
    uint8_t fail_num;///<�쳣��
    uint8_t test_over_fail;///<����ʱ�䵽�쳣��
    uint8_t test_status;///<����״̬
    uint16_t test_time;///<����ʱ��
    uint16_t dis_time;///<��ʾʱ��
    uint8_t test_over;///<���Խ���
    uint8_t gradation;///<���Խ׶�
    uint8_t danger;///<��ѹ���
    uint8_t cont;///<�������Ա��
    uint8_t vol_err_count;///<��ѹ�쳣����
    uint8_t low_err_count;///<���ޱ�������
    uint32_t vol_rise_step_t;///<��ѹ����/�½��׶�ʱ�����
    uint8_t err_real;///< acw ��ʵ����������� 
    float vol_ch_step;///< ��ѹ���䲽��ֵ
    float vol_ch_base;///< ��ѹ�仯����ʼֵ
    float vol_ch_target;///< ��ѹ�仯��Ŀ��
}TEST_DATA_STRUCT;

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
/******************* ȫ�ֱ������� *********************/
#ifdef   _99xxSTR_GLOBALS
#define  _99xxSTR_EXT
#else
#define  _99xxSTR_EXT  extern
#endif

_99xxSTR_EXT TEST_DATA_STRUCT g_test_data;
_99xxSTR_EXT SYS_FLAG	sys_flag;       /* ����ȫ�ֱ�־��ϵͳ����ʱʹ�� */
_99xxSTR_EXT SYS_PAR 	sys_par;	    /* ���ϵͳ���� */

_99xxSTR_EXT TESTGROUP test_group;      /* �˽ṹ��ʾһ�������ļ����� */

_99xxSTR_EXT NODE_STEP *free_list_steps;
_99xxSTR_EXT LIST_99xx list_99xx;
_99xxSTR_EXT uint8_t list_lon;/* ������󳤶� �ڲ��� ʱΪ2 ��ƫ�Ʋ���ʱΪ5 */

_99xxSTR_EXT LIST_99xx cs99xx_list;
_99xxSTR_EXT NODE_STEP *free_list_head;

_99xxSTR_EXT LIST_99xx cs99xx_cache_list;/* �������� */

_99xxSTR_EXT NODE_STEP *g_cur_step;     /* ָ��ǰ����ָ�� */
_99xxSTR_EXT NODE_STEP cur_node;        /* ȫ�ֽڵ��������ò���ʱ������ʱ���� */

_99xxSTR_EXT TEST_FILE file_table[MAX_FILES];/* �൱��һ�����Ŀ¼ */
_99xxSTR_EXT TEST_FILE default_file;
_99xxSTR_EXT TEST_FILE *g_cur_file;     /* ָ��ǰ�ļ� */

_99xxSTR_EXT NODE_STEP steps_buf[5];/* �������������Ĳ��� */


_99xxSTR_EXT RESULT_STR cur_result;     /* ��ǰ�����Խ����Ϣ�ͷ������� */
_99xxSTR_EXT RESULT_STR *g_cur_res;     /* ָ��ǰ�Ľ����Ϣ */
_99xxSTR_EXT RESULT_STR result_buf[PER_PAGE_RES_NUM];   /* ÿһҳ����ʾ4�����Բ���� */


_99xxSTR_EXT RESULT_STR result_list_buf[4];     /* ������в��Բ��Ĳ��Խ���б���ʾ��Ϣ */






/******************* �������� *********************/
extern void init_sys_par(SYS_PAR *p);	        /* ��ʼ��ϵͳ���� */
extern void init_file_table(void);	            /* ��ʼ���ļ��� */
extern void init_one_file(uint8_t n);
extern void clear_one_file(uint8_t n);
extern void init_default_file(void);
extern void del_file(const uint8_t file_num);
extern void del_all_file(void);

extern void init_99xx_list(void);	            /* ��ʼ�����Բ����� */

extern void clear_step_used_flag(void);
extern void insert_step(uint8_t pos, uint8_t mode);
extern void insert_node(uint8_t step);
extern void insert_step(uint8_t pos, uint8_t mode);
extern void del_step(uint8_t step);
extern void del_all_steps(void);
extern void swap_step(const uint16_t one, const uint16_t two);
extern void swap_node(PUN_STRUCT one,PUN_STRUCT two);
extern int32_t check_step_exist_for_comm(uint16_t pos);
extern NODE_STEP *position_step(uint16_t pos);

extern void init_cc_step(NODE_STEP * p);
extern void init_acw_step(NODE_STEP * p);
extern void init_dcw_step(NODE_STEP * p);
extern void init_ir_step(NODE_STEP * p);
extern void init_gr_step(NODE_STEP * p);
extern void init_bbd_step(NODE_STEP * p);
extern uint16_t get_cur_file_total(void);
/**************************************************/

#if defined(__cplusplus)
    }
#endif

#endif //__99xx_STURCT_H__

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
