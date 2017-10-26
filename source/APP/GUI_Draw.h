/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�GUI_Draw.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
 
#ifndef __GUI_DRAW_H__
#define __GUI_DRAW_H__

#include "stm32f10x.h"
#include "gui.h"
#include "cs99xx_struct.h"
#include "Font_DIY.h"
#include "LCD24064.H"


#define GX	       ((int16_t)60)
#define GY	       ((int16_t)15)
#define OX 	       ((int16_t)4)
#define OY 	       ((int16_t)14)
#define CW12       ((int16_t)6) /* 12�������� Ӣ���ַ� */
#define CW15       ((int16_t)9) /* 15�������� Ӣ���ַ� */
#define CW16       ((int16_t)9) /* 16�������� Ӣ���ַ� */
#define CH12       ((int16_t)12) /* 12������߶� */
#define CH15       ((int16_t)15) /* 16������߶� */
#define CH16       ((int16_t)16) /* 16������߶� */

#define PX	       ((int16_t)(GX+OX))
#define PY	       ((int16_t)(0))
#define SX	       ((int16_t)(PX+18))
#define SY	       ((int16_t)(0))
#define SOX	       ((int16_t)(58))
#define SOY	       ((int16_t)(3))


#define SYS_MENU_X      ((int16_t)15)
#define SYS_MENU_Y      ((int16_t)2)
#define SYS_MENU_OX1    ((int16_t)75)
#define SYS_MENU_OX2    ((int16_t)73)
#define SYS_MENU_OY     ((int16_t)16)

/* ״̬��ʾ���� ���� */
#define GUI_GEN_STATUS_CW   ((int16_t)9)
#define GUI_GEN_STATUS_CH   ((int16_t)16)
#define GUI_GEN_STATUS_X	((int16_t)6)
#define GUI_GEN_STATUS_X1	((int16_t)6)
#define GUI_GEN_STATUS_X2	((int16_t)86)
#define GUI_GEN_STATUS_Y	((int16_t)42)

#define GUI_SYN_STATUS_CW	((int16_t)6)
#define GUI_SYN_STATUS_CH	((int16_t)12)
#define GUI_SYN_STATUS_X	((int16_t)10)
#define GUI_SYN_STATUS_X1	((int16_t)10)
#define GUI_SYN_STATUS_X2	((int16_t)10+8*6)
#define GUI_SYN_STATUS_Y	((int16_t)46)

/* ���������� ���� */
#if (MAX_STEPS < 100)
#define TITLE_STEP_X        ((int16_t)(4))
#define TITLE_STEP_COLON_X  ((int16_t)(27))//:
#define TITLE_STEP_NUM_X    ((int16_t)(39))
#define TITLE_STEP__X       ((int16_t)(51))
#define TITLE_STEP_TOTAL_X  ((int16_t)(57))
#else
#define TITLE_STEP_X        ((int16_t)(-100))//����ʾ
#define TITLE_STEP_COLON_X  ((int16_t)(-100))//:����ʾ
#define TITLE_STEP_NUM_X    ((int16_t)(15))
#define TITLE_STEP__X       ((int16_t)(39))
#define TITLE_STEP_TOTAL_X  ((int16_t)(45)) //4+35+18-2*6
#endif


#define TITLE_WORK_MODE_X   ((int16_t)(76))//4+35+37, 0
#define TITLE_MODE_X        ((int16_t)(90))//4+35+38+13
#define TITLE_FILE_X        ((int16_t)(112))// 4+35+38+15+22
#define TITLE_FILE_COLON_X  ((int16_t)(134))//:4+35+38+15+20+4*6
#define TITLE_FILE_NAME_X   ((int16_t)(143))// 4+35+38+15+22+6*5+1

#define TITLE_Y             ((int16_t)(0))


#define MODE1	   ((uint8_t)1)
#define MODE2	   ((uint8_t)2)
#define MODE3	   ((uint8_t)3)
#define MODE4	   ((uint8_t)4)
#define MODE5	   ((uint8_t)5)

/* ���м���x����ֻ�������ֿ�6������ */
#define	BTP_STR(s, x1, x2) (((x1)+(x2))/2 - (strlen(s)*6/2))
#define GUI_DispStringAt_BTP_STR(s, x1, x2, y)  GUI_DispStringAt(s, BTP_STR(s,x1, x2), (y))

/* �������cw ָ��ƽ�������� ����������� */
#define	BTP_STR_(s, x1, x2, cw) (((x1)+(x2))/2 - (strlen(s)*(cw)/2))
#define GUI_DispStringAt_BTP_STR_(s, x1, x2, y, cw)  GUI_DispStringAt((s), BTP_STR_((s), (x1), (x2), (cw)), (y))
/*********************** ���� **************************/
#define TEST_ORIGIN_X       ((int16_t)10)
#define TEST_ORIGIN_Y       ((int16_t)13)
#define TEST_UI_OFFSET      ((int16_t)13)

/********************* λͼ���� ************************/
extern const GUI_BITMAP bmup;
extern const GUI_BITMAP bmlogo;
extern const GUI_BITMAP bmallwin;
extern const GUI_BITMAP bmback;
extern const GUI_BITMAP bmshortcut;
extern const GUI_BITMAP bmrightshift;
extern const GUI_BITMAP bmleftshift;
extern const GUI_BITMAP bmdel;
extern const GUI_BITMAP bmdown;
extern const GUI_BITMAP bmarrow;
extern const GUI_BITMAP bmarrow_right;
extern const GUI_BITMAP bmcircle;
extern const GUI_BITMAP bmcircle_;
extern const GUI_BITMAP bm16bit;
extern const GUI_BITMAP bmwarning;
extern const GUI_BITMAP bmindex;
extern const GUI_BITMAP bmcolon;



/***************** ���ݽṹ���� ***********************/
enum{
	INT_T,			/* �������� */
	FLOAT_T, 		/* ���������� */
	STRING_T,		/* �ַ������� */
	OPTION_T,		/* ѡ������ ������һ�������ϵ�ѡ�� */
	EITHER_OR, 		/* ��ѡһ */
    STRUCT_T,       /* �ṹ���� */
};
typedef enum{
	UD_B,	  /* up down �� back */
	UD_B_RE,  /* UD_B+ˢ�� */
	UDSB,	  /* up down short_cut back */
	UDSB_RE,  /* UDSB+ˢ�� */
	UDRB,	  /* up down round_shift back */
	UDRB_RE,  /* UDRB+ˢ�� */
	UDR_,
	UDR_RE,
	___B,
	___B_RE,
	LRDB,    /* lift_shift right_shift del back */
	LRDB_RE,
    __RB,
    __RB_RE,
}SHORT_CUT_TYPE;

/* ����������Ϣ�����±� */
typedef enum{
	/********** COM 12 0 ************/
	GUI_COM_STEP = 0,       /* ���Բ��� */
	GUI_COM_MODE,           /* ����ģʽ */
	GUI_COM_DELAY_T,        /* ��ʱʱ�� */
	GUI_COM_RISE_T,         /* ����ʱ�� */
	GUI_COM_STAB_T,         /* ��ѹʱ�� */
	GUI_COM_TEST_T,         /* ����ʱ�� */
	GUI_COM_FALL_T,         /* �½�ʱ�� */
	GUI_COM_INTER_T,        /* ���ʱ�� */
	GUI_COM_STEPS_PASS,     /* ����pass */
	GUI_COM_STEPS_CONT,     /* �������� */
	GUI_COM_OUTPUT_FREQ,    /* ���Ƶ�� */
	GUI_COM_PORT,           /* ��·�˿� */
	GUI_COM_CUR_OFFSET,		/* ����ƫ�� */
	GUI_COM_RES_OFFSET,		/* ����ƫ�� */
	
	/********** ACW 8  12 ************/
	GUI_ACW_OUTPUT_VOL,     /* �����ѹ */
	GUI_ACW_CUR_GEAR,       /* ������λ */
	GUI_ACW_CUR_H,          /* �������� */
	GUI_ACW_CUR_L,          /* �������� */
	GUI_ACW_REAL_CUR,       /* ��ʵ���� */
	GUI_ACW_ARC_SUR,        /* �绡��� */
	
	/********** DCW 8 20 ************/
	GUI_DCW_OUTPUT_VOL,     /* �����ѹ */
	GUI_DCW_CUR_GEAR,       /* ������λ */
	GUI_DCW_CUR_H,          /* �������� */
	GUI_DCW_CUR_L,          /* �������� */
	GUI_DCW_CHARGE_CUR,     /* ������ */
	GUI_DCW_ARC_SUR,        /* �绡��� */
    GUI_DCW_OUTPUT_IMPEDANCE, /* ����迹 */
	
	/********** IR 4 28 ************/
	GUI_IR_OUTPUT_VOL,      /* �����ѹ */
	GUI_IR_AUTO_GEAR,       /* �Զ����� */
	GUI_IR_RES_H,           /* �������� */
	GUI_IR_RES_L,           /* �������� */
	
	/********** GR 7 32 ************/
	GUI_GR_VOL_GEAR,        /* ��ѹ��λ */
	GUI_GR_OUTPUT_CUR,      /* ������� */
	GUI_GR_RES_H,           /* �������� */
	GUI_GR_RES_L,           /* �������� */
	GUI_GR_VOL_H,           /* ��ѹ���� */
	GUI_GR_VOL_L,           /* ��ѹ���� */
	GUI_GR_TEST_METHOD,     /* ���Է��� �������� ������� */
	
	/********** BBD 3 39 ************/
	GUI_BBD_OUTPUT_VOL,     /* �����ѹ */
	GUI_BBD_OPEN_CHECK,     /* ��·��� */
	GUI_BBD_SHORT_CHECK,    /* ��·��� */
	GUI_BBD_CAP_H,			/* �������� */
	GUI_BBD_CAP_L,			/* �������� */
	
	/********** CC 6 42 ************/
	GUI_CC_OUTPUT_VOL,      /* �����ѹ */
	GUI_CC_CUR_GEAR,        /* ������λ */
	GUI_CC_CUR_H,           /* �������� */
	GUI_CC_CUR_L,           /* �������� */
	GUI_CC_REAL_CUR,        /* ��ʵ���� */
	GUI_CC_CUR_INTENSITY,   /* ����ǿ�� */
	
	/********** SYS 19 48 ************/
	GUI_SYS_CONTRAST,       /* Һ���ֱ��� */
	GUI_SYS_REMAIN_HINT,    /* ������ʾ */
	GUI_SYS_RES_SAVE,       /* ������� */
	GUI_SYS_VOER_COVER,     /* ������� */
	GUI_SYS_GFI_PROT,       /* GFI���� */
	GUI_SYS_SELF_CHECK,     /* �Լ����� */
	GUI_SYS_LANGUAGE,       /* ϵͳ���� */
	GUI_SYS_FAIL_CONT,      /* ʧ�ܼ��� */
	GUI_SYS_LIST_DIS,       /* �б���ʾ */
	GUI_SYS_TEST_PORT,      /* ���Զ˿� �ӵ�ģʽ ����ģʽ */
	GUI_SYS_TEST_SIGNAL,    /* �����ź� */
	GUI_SYS_BUZZER_SW,      /* �������� */
	GUI_SYS_CHECK_DATE,     /* �������� */
	GUI_SYS_NO_RULE,        /* ��Ź��� */
    GUI_SYS_FAIL_MODE,      /* ʧ��ģʽ */
    GUI_SYS_SILENT_SW,      /* �������� */
    GUI_SYS_PLC_SIGNAL,     /* PLC�ź� */
    GUI_SYS_OUTPUT_DELAY,   /* �����ʱ */
	GUI_SYS_IR_GEAR_HOLD,	/* ��λ���� */
	GUI_SYS_IR_SPEED,		/* IR�����ٶ� �� �� �� ͨ���˲���Ȳ�ͬ��ʵ�� */
    
	/********** CUSTOM 9 67 ************/
    GUI_CUSTOM_AMP_SELECT,/* ����ѡ���Ϲ����¹��� */
    GUI_CUSTOM_AMP_TYPE,    /* ���ƹ������� LINE PWM */
    GUI_CUSTOM_PAR_MEDIUM,/* �����洢���� */
    GUI_CUSTOM_RES_MEDIUM,/* ����洢���� */
    GUI_CUSTOM_CAL_MEDIUM,/* У׼�洢���� */
    GUI_CUSTOM_BUZZER_EN,/* ������Ӳ��ʹ�� */
    GUI_CUSTOM_MUTE_SW,/* �������� */
    GUI_CUSTOM_LEADING_SW,/* ���ݵ��뵼������ */
    GUI_CUSTOM_IR_G_HOLD_SW,/* IR�������ֿ��� */
	GUI_CUSTOM_IR_SPEED_SW,/* IR�����ٶȿ��� */
	GUI_CUSTOM_OFFSET_SW,/* ƫ�����ÿ��� */
	
	GUI_MENU_END,/* ���� */
}MENU_ENUM;
typedef enum{
    GUI_U8,         /* 8λ */
    GUI_U16,        /* 16λ */
    GUI_U32,        /* 32λ */
    GUI_STRING,     /* �ַ��� */
    GUI_STRUCT,     /* �ṹ���� */
    GUI_U8_ARRAY,   /* ���� */
    GUI_U16_ARRAY,  /* ���� */
    GUI_U32_ARRAY,  /* ���� */
}DATE_LON_ENUM;
typedef struct Data_Info{
	void* pData;           /* ����ָ�� */
	uint8_t x;             /* ���ݵ�x���� */
	uint8_t y;             /* ���ݵ�y���� */
	uint8_t type;          /* 0��ʾint,1��ʾfloat,2��ʾ�ַ��� 3 ��ʾ����ѡ���� 4 ��ʾ��ѡһ */
	uint8_t lon;           /* ��ʾ���ݳ��� */
	uint8_t decs;          /* С�����λ */
	uint8_t total;         /* ��ǵ�ǰҳ��ʾ���������� */
	uint8_t cur;           /* ��ǵ�ǰ�������ǵڼ������� */
	uint8_t ch_bit;        /* ��ǰ�޸ĵ�λ ������ 0-n ex:123.4 0->1 ,1->2 ,2->3 ,3->. ,4->4 */
	uint8_t page;	       /* ��ǰҳ�� */
	uint8_t bits;          /* ������ǰֵ��8λ��16λ��32λ��0����8λ��1����16λ��2����32λ,3�ַ���, 4�ṹ���� DATE_LON_ENUM */
	uint8_t last_ch_bit;   /* �������һ�θ��Ĺ���λ */
	uint32_t upper;        /* ��������ֵ */
	uint32_t lower;        /* ��������ֵ */
	
	const uint8_t *name[2];   /* ���� */
	const uint8_t *unit[10];  /* ��λ��ѡ�����������ѡ�� */
	void (*fun) (void *p); /* ������ */
    uint8_t no_dis_bits;/* ����ʾ��λ��,���ұ�������ʾ��λ�� ��lon = 5 xx.xx no_dis_bits = 1 ��� xx.x */
	MENU_ENUM num;         /* ��� ��Ӧö�� MENU_ENUM */
    SHORT_CUT_TYPE short_cut;     /* �༭�ñ�����Ҫ�Ĺ��ܼ����� */
	const uint8_t *upper_str;
    const uint8_t *lower_str;
	uint8_t x_offset;      /* x�����ƫ��ֵ */
	uint8_t y_offset;      /* y�����ƫ��ֵ */
	uint8_t range_flag;    /* ������Χ�Ƿ���Ҫ���� 1��ʾ������ 0���� */
	
    void *user_data;       /* �û��Զ���ʹ�� ��ָ�� Я������ */
	struct Data_Info *next;/* ��һ������ */
	struct Data_Info *prev;/* ��һ������ */
}DATA_INFO;


typedef struct {
	struct Data_Info *head;/* ����ͷ */
	struct Data_Info *tail;/* ����β */
	uint32_t size;         /* �ڵ���� */
}PAR_INFO_LIST;

typedef struct{
    const char* name[2];       /* ���� ���� Ӣ�� */
    void (*fun)(uint32_t opt); /* ������ */
    uint32_t data;             /* ���� */
    uint32_t size;             /* �û����ݸ��� */
}MENU_INFO;

typedef struct{
    uint8_t dis_buf[15];/* ��ʾ���ݵĻ��� */
    uint8_t lon;/* ��󳤶� */
}DIS_AREA_T_;
/* ��ʾ����ṹ���� */
typedef struct{
    uint8_t uint[5];/* ��λ��Ϣ */
    uint8_t x;/* x���� */
    uint8_t y;/* y���� */
    const GUI_FONT *font;/* ���� */
    uint8_t lon2;/* ��󳤶� */
}DIS_AREA_T;

/* ��ѡ�����ò����Ľṹ���� */
/* ������ACW DCW������λʱ���õ� */
typedef struct{
    const uint8_t **opt_buf;   /* λ����Ϣ �����еĹ�����ʹ�� */
    uint8_t x;              /* x���� */
    uint8_t y;              /* y���� */
    uint8_t x_o[10];        /* x�����n��ƫ�� */
}MORE_SEL_STR;


typedef struct More_Options_Sele{
    const uint8_t* name[2];
    uint8_t x;
    uint8_t y;
    uint8_t opt;
    struct More_Options_Sele *next;
    struct More_Options_Sele *prev;
    uint8_t cur;/* ��ǰ�еڼ��� */
    uint8_t total;/* ��ǰ��һ���м��� */
}MORE_OPTIONS_STR;

typedef struct more_options_list{
    struct More_Options_Sele *head;
    struct More_Options_Sele *tail;
    uint8_t size;
}MORE_OPTIONS_LIST;
/********************* ȫ�ֱ������� *******************/
#ifdef MENU_GLOBAL
	#define EXTERN
#else
	#define EXTERN extern
#endif
/*********************************************/
EXTERN MORE_OPTIONS_LIST plc_signal_list;/* PLC�������� */
EXTERN MORE_OPTIONS_LIST cc_cur_gear_list;/* ����ǿ������������ */
EXTERN MORE_OPTIONS_LIST amp_type_list;/* �������� */
EXTERN MORE_OPTIONS_LIST par_medium_list;/* �����洢���� eep �� flash */
EXTERN MORE_OPTIONS_LIST res_medium_list;/* ����洢���� eep �� flash */
EXTERN MORE_OPTIONS_LIST cal_medium_list;/* У׼�洢���� eep �� flash */

EXTERN MORE_OPTIONS_LIST ir_filtering_deep_list;/* �� �� �� ���ھ�Ե���Ե����˲��� */


/* PLC�ź� */
EXTERN MORE_OPTIONS_STR plc_signal_option_pool[];
typedef enum{
    EACH_STEP, /* ���� */
    ALL_STEP, /* �ܲ� ����ģʽ */
    TEST_HOLD,/* TEST�źű��� */
}PLC_SIGNAL_ENUM;

/* �������� */
EXTERN MORE_OPTIONS_STR amp_type_option_pool[];
typedef enum{
    AMP_8833,
    AMP_OLD,
}AMP_TYPE_ENUM;

EXTERN MORE_OPTIONS_STR par_medium_option_pool[];
typedef enum{
    PAR_MEDIUM_EEP,
    PAR_MEDIUM_FLASH,
}PAR_TYPE_ENUM;

EXTERN MORE_OPTIONS_STR res_medium_option_pool[];
typedef enum{
    RES_MEDIUM_EEP,
    RES_MEDIUM_FLASH,
}RES_TYPE_ENUM;

EXTERN MORE_OPTIONS_STR cal_medium_option_pool[];
typedef enum{
    CAL_MEDIUM_EEP,
    CAL_MEDIUM_FLASH,
}CAL_TYPE_ENUM;

/* CC�������ǿ�� */
enum{
    CC_CUR_HIGH,/* �� */
    CC_CUR_MID,/* �� */
    CC_CUR_LOW,/* �� */
};
EXTERN 	MORE_OPTIONS_STR 	cc_cur_gear_option_pool[];

/* IR�˲���� ֱ��Ӱ��Ĳ��Գ���Ŀ����� */
enum{
    IR_FILTERING_DEEP_FAST,/* �� */
    IR_FILTERING_DEEP_MID,/* �� */
    IR_FILTERING_DEEP_SLOW,/* �� */
};
EXTERN	 MORE_OPTIONS_STR 	ir_filtering_deep_option_pool[];

/* ��ѡ�˵����� */
EXTERN MORE_SEL_STR mor_sel_menu[];
enum{
    ACW_GEAR_SEL_MENU,      /* ACW������λ */
    DCW_GEAR_SEL_MENU,      /* DCW������λ */
    GR_VOL_GEAR_SEL_MENU,   /* GR��ѹ��λ */
    MODE_SET_SEL_MENU,      /* ����ģʽ */
    SYS_OPT_SET_MENU,       /* ϵͳ�����ѡ�� */
};

/* ʧ��ģʽ��  */
EXTERN const uint8_t* fail_mode_pool[];
enum{
    FAIL_MODE_STOP,/* stop�� start�� �ٴ������ӵ�һ����ʼ */
    FAIL_MODE_HALT,/* stop�� start�� �ٴ���������һ����ʼ */
    FAIL_MODE_CON,/* ����������������򿪾��Զ�������һ�� */
    FAIL_MODE_RESTART,/* start�� �ӵ�һ����ʼ */
    FAIL_MODE_NEXT,/* start�� ����һ����ʼ */
    FAIL_MODE_FPDFC,/* First pass done fail continue ��һ��pass�������� ʧ�ܾͼ������� 
                    ��һ�����Ժϸ����ɲ��ԣ���һ��ʧ�ܾͼ�����������Ĳ��� 
                    ע�⣺Ӧ���ڶ�·ɨ����ԣ�ͨ����һ����Զ�·�˿�ͬʱ���ϸ�ѹ
                    ����һ�����Ժϸ��ˣ��ͱ���ÿ���˿ڵĲ�Ʒ���Ǻϸ�ġ��������
                    ·ɨ��ÿһ���˿ڣ��ҳ����ϸ�Ĳ�Ʒ
                    */
    FAIL_MODE_END,
};

/* ����迹ѡ�� */
typedef struct{
    const uint8_t *opts[4];/* ѡ���� ���� */
    const uint8_t size;/* ѡ����� */
}OUTPUT_IMPEDAN_OPTS_T;

EXTERN const OUTPUT_IMPEDAN_OPTS_T output_impedan_opt[];
typedef enum{
    OUTPUT_IMPEDAN_OPT_1,
    OUTPUT_IMPEDAN_OPT_2,
}OUTPUT_IMPEDAN_OPTS;



EXTERN DATA_INFO menu_par[];
EXTERN DATA_INFO* pmenu_data;

EXTERN DATA_INFO* g_cur_info[6];/* ��ǰҳ�ı�����Ϣ */
EXTERN uint32_t g_cur_val[6];/* ��ǰҳ����ֵ */
EXTERN PAR_INFO_LIST par_info_list;/* ������Ϣ���� */

EXTERN uint32_t colon_x;/* �������ƺ���ð�ŵ�x���� ���ڲ���������ý��� �� ϵͳ�������ý��� �������ط���ֵ��һ�� */

/*************** ö�����Ӧ�����鶨�� ******************/
/* GFI ARC HV ״̬ �ڲ��Լ�����ʾ ��û�ж�·ɨ��ʱ�� */
EXTERN const char* sys_sw_status_buf[][2];
enum{
	GFI_STATUS,
	ARC_STATUS,
	HV_STATUS,
};
/***************************************/
/* ����ģʽ�� */
EXTERN const uint8_t *mode_pool[][10];

enum{
    NUL     = 0,
    ACW     = 1,
    DCW     = 2,
    IR      = 3,
    GR      = 4,
    BBD     = 5,
    CC      = 6,
    MODE_END,
};


/* ������������Ŀ */
EXTERN const uint8_t *opt_pool[];
enum{
    NUL_OPT,
    VOL_OPT,
    CUR_OPT,
    RES_OPT,
    CAP_OPT,
};

/* ����ģʽ */
EXTERN const char *work_mode_buf[];
/***************************************/
/* ����״̬�� */
EXTERN const char* status_str[][2];

enum{
	TEST_WAIT_INDEX,
	TEST_RISE_INDEX,
	TEST_TEST_INDEX,
	TEST_FALL_INDEX,
	TEST_CHANGE_INDEX,
	TEST_INTER_INDEX,
	TEST_PASS_INDEX,
	TEST_FAIL_INDEX,
	TEST_CHARGE_INDEX,
	TEST_STAB_INDEX,
	TEST_DISCHARGE_INDEX,
	TEST_DISCHARGE_OVER_INDEX,
    TEST_OUTPUT_DELAY,
};
/***************************************/
/* �쳣��Ϣ */
extern const char* except_buf[][3];
/********* ������λ���� *********/
/* ��λ�� */
EXTERN const uint8_t *unit_pool[];
EXTERN const uint8_t *unit_pool_[];
enum{
    NULL_U_NULL,
    VOL_U_mV,
    VOL_U_V,
    VOL_U_kV,
    CUR_U_uA,
    CUR_U_mA,
    CUR_U_A,
    RES_U_mOHM,
    RES_U_OHM,
    RES_U_MOHM,
    RES_U_GOHM,
    CAP_U_pF,
    CAP_U_nF,
    CAP_U_uF,
};

/********************************/
/* �������Ƶ�� */
enum{
    AC_50HZ = 1,
    AC_60HZ,
    AC_100HZ,
    AC_150HZ,
    AC_200HZ,
    AC_250HZ,
    AC_300HZ,
    AC_350HZ,
    AC_400HZ,
};

/********************************/
EXTERN DIS_AREA_T test_gui_area[6];
enum{
    TEST_GUI_AREA_1,
    TEST_GUI_AREA_2,
    TEST_GUI_AREA_3,
    TEST_GUI_AREA_4,
    TEST_GUI_AREA_5,
    TEST_GUI_AREA_6,
};
    
/********************************/
/* IR��ʾ����ʱ����ʱʹ�õ� */
EXTERN const char* res_infinite_str[];

/********************************/
/* ��·ɨ��˿����� */
#define PORT_STATUS_STR     ((const char*)"XLH")

/**************************************/
/* ƫ�ƽ����ʾ�� */
EXTERN const char *offset_res_buf[4];
enum{
	OFFSET_PASS		= 2,
	OFFSET_FAIL		= 1,
	OFFSET_NONE		= 0,
};

/**************************************/
typedef enum{
    EXE_NULL,//�ղ���
    RE_MAIN,//�ػ�������
    INTO_TEST,//������Խ���
    INTO_SET,//�������ý���
    INTO_MENU,//����˵�����
    INTO_HELP,//�����������
    INTO_AUTO_CAL,//�����Զ�У׼
    INTO_SCAN_MODULES,//����ɨ��ģ�����
}EXE_STATUS_ENUM;

/**************************************/

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t u_x;
    uint8_t o_y;
    uint8_t cw;
    uint8_t *str;
}ELEMENT_INFO;

// typedef struct{
//     ELEMENT_INFO info[4];//��ѹ ���� ��ʵ���� ʱ��
// }TEST_UI_ELE_SET;

EXTERN ELEMENT_INFO test_ui_ele_set[6];/* ���Խ����µ�Ԫ����Ϣ���� */

enum{
    TEST_ELE_1,/* ���Խ����Ԫ��1 ACW DCW IR�ĵ�ѹ GR�ĵ��� */
    TEST_ELE_2,/* ���Խ����Ԫ��2 ACW DCW �ĵ��� IR GR �ĵ��� */
    TEST_ELE_3,/* ���Խ����Ԫ��3 ACW ��ʵ���� �� ���� */
    TEST_ELE_4,/* ���Խ����Ԫ��4 ����ʱ�� */
    TEST_ELE_5,/* ���Խ����Ԫ��5 ����״̬�� */
    TEST_ELE_6,/* ���Խ����Ԫ��6 ���Խ��״̬�� */
    
    OFFSET_ELE_1 = 0,/* ƫ�ƽ����Ԫ��1 ACW DCW IR�ĵ�ѹ GR�ĵ��� */
    OFFSET_ELE_2,/* ƫ�ƽ����Ԫ��2 ACW DCW �ĵ��� IR GR �ĵ��� */
    OFFSET_ELE_3,/* ƫ�ƽ����Ԫ��3 ACW ��ʵ���� �� ���� */
    OFFSET_ELE_4,/* ƫ�ƽ����Ԫ��4 ����ʱ�� */
    OFFSET_ELE_5,/* ƫ�ƽ����Ԫ��4 ����״̬�� */
};

typedef enum{
    /* ���ý��� */
    UI_SET_PORTS_BASE_X = 10,/* x�����ֵ */
    UI_SET_PORTS_BASE_Y = 35,/* y�����ֵ */
    UI_SET_PORT_X_O = 12,/* x��������ͨ��ƫ��ֵ */
    UI_SET_PORT_Y_O = 12,/* y��������ͨ��ƫ��ֵ */
    /* ���Խ��� */
    UI_TEST_PORTS_BASE_X = 90,/* x�����ֵ */
    UI_TEST_PORTS_BASE_Y = 41,/* y�����ֵ */
    UI_TEST_PORT_X_O = 9,/* x��������ͨ��ƫ��ֵ */
    UI_TEST_PORT_Y_O = 12,/* y��������ͨ��ƫ��ֵ */
    /* ƫ�ƽ��� */
    UI_OFFSET_PORTS_BASE_X = 5,/* x�����ֵ */
    UI_OFFSET_PORTS_BASE_Y = 0,/* y�����ֵ */
    UI_OFFSET_PORT_X_O = 12,/* x��������ͨ��ƫ��ֵ */
    UI_OFFSET_PORT_Y_O = 0,/* y��������ͨ��ƫ��ֵ */
}UI_PORTS_ENUM;/* ��·ɨ��ͨ���Ľ�����Ϣ */

#define TEST_ELE_1_X        10
#define TEST_ELE_1_Y        13
#define TEST_ELE_1_U_X(cw)  (TEST_ELE_1_X + 5 * cw + 1)

#define TEST_ELE_2_X        109
#define TEST_ELE_2_Y        13
#define TEST_ELE_2_U_X(cw)  (TEST_ELE_2_X + 5 * cw + 1)

#define TEST_ELE_3_X        109
#define TEST_ELE_3_Y        26
#define TEST_ELE_3_U_X(cw)  (TEST_ELE_3_X + 5 * cw + 1)

#define TEST_ELE_4_X        10
#define TEST_ELE_4_Y        26
#define TEST_ELE_4_U_X(cw)  (TEST_ELE_4_X + 5 * cw + 1)

// GUI_FontHZ_SimSun_16 
#define TEST_ELE_5_X        (GUI_GEN_STATUS_X1)
#define TEST_ELE_5_Y        (GUI_GEN_STATUS_Y)
#define TEST_ELE_5_U_X(cw)  (GUI_GEN_STATUS_X2)

#define TEST_ELE_6_X        (100)
#define TEST_ELE_6_Y        (42)
#define TEST_ELE_6_U_X(cw)  (87+12*8)
//////////////////////OFFSET///////////////////////////
#define OFFSET_ELE_1_X        152 //���Ե�ѹ
#define OFFSET_ELE_1_Y        27
#define OFFSET_ELE_1_U_X(cw)  (OFFSET_ELE_1_X + 5 * cw)

#define OFFSET_ELE_2_X        30  //ƫ�Ƶ���
#define OFFSET_ELE_2_Y        15
#define OFFSET_ELE_2_U_X(cw)  (OFFSET_ELE_2_X + 5 * cw)

#define OFFSET_ELE_3_X        78   //ƫ�Ƶ���ʵ����
#define OFFSET_ELE_3_Y        15
#define OFFSET_ELE_3_U_X(cw)  (OFFSET_ELE_3_X + 5 * cw)

#define OFFSET_ELE_4_X        152   //ƫ��ʱ��
#define OFFSET_ELE_4_Y        38
#define OFFSET_ELE_4_U_X(cw)  (OFFSET_ELE_4_X + 5 * cw)

#define OFFSET_ELE_5_X        (150)  //ƫ��״̬
#define OFFSET_ELE_5_Y        51
#define OFFSET_ELE_5_U_X(cw)  (OFFSET_ELE_5_X + 8 * cw)
/********************* �������� ***********************/
extern void draw_more_options_gui(MORE_OPTIONS_LIST *list, MORE_OPTIONS_STR *iteration);
extern void draw_short_cut_bm(uint8_t type);
extern void draw_short_cut_str(uint8_t *str1, uint8_t *str2, uint8_t *str3, uint8_t *str4);

extern void draw_menu_frame_line(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2);

extern uint8_t * mystrcat(uint8_t *buf, uint8_t *str1, uint8_t *str2);
extern void front_add_space_at_string(uint8_t *buf, uint8_t c);
extern uint32_t ten_power(u8 n);
extern char* div_str_pre_zero(char *str);
extern void mysprintf(uint8_t *buf, const uint8_t* unit, uint8_t type, uint32_t value);
extern void draw_colon(void);
extern void draw_one_par_info(DATA_INFO* p, void *pdata, const uint8_t unit);
extern void load_this_page_info(DATA_INFO* data_info[]);
extern void load_this_page_value(void);
extern void cursor_pos(int16_t x, int16_t y);
extern void dis_cursor_pos(void);
extern void cursor_pos_n(int16_t x, int16_t y);/* ����ˢ����Ļ */
extern void dis_cursor(DATA_INFO * p);
extern void shift_cursor (DATA_INFO * p);
extern void shift_cursor_l (DATA_INFO * p);
extern void change_data(uint8_t base);
extern void my_sprintf(char *buf, DATA_INFO * p, uint32_t data);
extern void update_data(DATA_INFO * p);
extern void save_par_to_ram(DATA_INFO *data_info, uint32_t value);
extern void save_par_to_array(DATA_INFO *data_info, uint32_t value, uint8_t array_num);
extern uint32_t read_par_to_buf(DATA_INFO* data_info, uint32_t * value);
// void dis_test_time(uint16_t time, const GUI_FONT *font, uint16_t x, uint16_t y);

extern void clear_(void);
extern void clear_menu(void);
extern void MenuTitle(u8 which);
extern void syn_test_menu_title(void);
extern void set_syn_roads_cur_upper (void);
extern void test_menu_title(u8 which);
extern void draw_port_for_set(TEST_PORT *p);
extern void dis_port(void);
extern void gui_draw_menu(void);

extern void dis_set_port(const uint8_t val,const uint8_t n, const uint8_t ports);
extern void ports_cursor_pos(uint8_t n, uint8_t ports);

extern void gui_dis_float_at(const float v, const uint8_t decs, const uint8_t x, const uint8_t y);

extern void acw_gr_hz_sel_gui(uint8_t kinds, uint8_t n, const uint8_t** gear_buf);
extern void ac_cur_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n);
extern void gr_vol_sel_gui(uint8_t options, const uint8_t** opt_buf, uint8_t n);
extern void dcw_cur_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n);
extern void mode_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n);
extern void sys_option_sel_gui(uint8_t options, const uint8_t** opt_buf, uint8_t n);

extern void auto_cal_pass(void);
extern void gui_par_inval(void); 
extern void gui_g_mode_inval(void);
extern void ratio_broken(void);
extern void cal_warning(void);
extern void rate_of_advance(void);
extern void type_mismatch(void);
extern void gui_draw_main(void);

extern void cal_sw_on_warning(void);
extern void draw_wait_usb_connect_gui(void);
extern void draw_auto_calibration_gui(void);
extern void draw_no_connect_usb_gui(void);
extern void dcgr_commuincation_err(void);
extern void gui_draw_build_connect_to_board(void);
extern void gui_draw_build_connect_fail(void);
extern void gui_draw_dc_gr_module_comm_fail(void);
extern void gui_draw_dc_gr_module_being_in_comm(void);
extern void gui_draw_self_check(void);
extern void gui_draw_set_sys_cur_limit(void);

extern void gui_draw_test (void);
extern void gui_drow_test_syn (void);

extern void gui_draw_menu_sys(MENU_INFO* p_sys_menu, uint8_t lon);

extern void draw_custom_type_gui(void);
extern void gui_draw_help_1(void);
extern void gui_draw_help_2(void);
extern void gui_draw_help_3(void);

extern void gui_draw_file_management(void);
extern void gui_draw_communication_interface(MENU_INFO* pmenu_info);
extern void gui_draw_password_manage(void);
extern void gui_draw_time_manage(void);

extern void gui_draw_offset(void);
extern void gui_draw_bbd(void);
extern void gui_draw_test_result(void);
extern void gui_draw_keyboard_lock_manage(void);
extern void gui_draw_recover(void);

extern void dis_syn_cur_ui(const uint8_t n);

extern void dis_scan_modules_gui(void);

#endif //__GUI_Draw_H__
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
