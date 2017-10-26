/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_type.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#ifndef __TYPE_SELECT__
#define __TYPE_SELECT__
#include <stm32f10x.h>
#include "cs99xx_config.h"


/* �����ͺ���Ӳ������һ�� 0x00��ʾ���ͺ�δ���� */
enum{
DEBUG_TYPE, /* ���ͺ��ǵ���ʹ�õ��������������� */

CS9919AX,
CS9919BX,
CS9929AX,
CS9929BX,
CS9922BX,

CS9922CX,
CS9933X,
CS9912AX,
CS9912BX,
CS9913AX,

CS9913BX,
CS9950X,
CS9950CX,
CS9950DX,
CS9922DX,

CS9922EX,
CS9914AX,
CS9914BX,
CS9929CX,
CS9922FX,

CS9921BX,
CS9912Y,
CS9914Y,
CS9922CX_1,
CS9929EX,

CS9919G,
CS9922G,
CS9922G_1,
CS9933G,
CS9950CG,

CS9950CG_1,
CS9914G,
CS9919A,
CS9919B,
CS9929A,

CS9929B,
CS9922M,
CS9915AX,
CS9916AX,
CS9916BX,

CS9917AX,
CS9917BX,
CS9920A,
CS9920B,
CS9923 ,

CS9923A,
CS9933EG,
CS9922H_50,
CS9922H_25,
CS9914CX,

CS9914DX,
CS9914AT_10,/* δʹ�õ��ͺ� */
CS9906B,
CS9929A_8,
CS9919A_8,

CS9917B,
CS9906B_K,
CS9906A,
CS9933ET,
CS9929A_4,
CS9917CX_20,
CS9924_200,
CS9914AM_H,
CS9917A_8,

CS9919B_4,
CS9929B_4,
CS9933G_4,
CS9923G,
CS9939X,
CS9933EG_1,
CS9914AM,
CS9914BM,
CS9917CX,
CS9917DX,
CS9929AX_T,
CS9929AX_F,

CS9922BX_DZ,
CS9933ET_1,
CS9922BX_1,
CS9923_5,
CS9922EXJ,
CS9914AX_1,
CS9922BXJ,
CS9912BX_T,
CS9933EG_2,
CS9923G_1,
CS9933X_100,
CS9924,
CS9922LB,
CS9917CX_C,
CS9912BX_L,/* û����������ʱ������CS9912BX + BBD */
VTC9929,/* ���ƻ� */
TSK7630,/* ���ƻ� */
CS9922G_2,
CS2676CH,
CS9914D,
CS9929AX_8,
CS9919D,
CS9933G_1,
CS9933ET_2,
CS9933ET_3,



///////////////
CS9923ECG,
CS9923G_2,
CS9923G_3,

CS9923ECG_1,
CS9950ED,
CS9929EX_CC,
CS9914CX_C,
CS9927LB,
CS2676CH_1,
CS9933G_,
CS9923CG,
CS9929AX_CC,
CS9930A_T,

CS99XX_T_END,
};

/* ģʽѡ�� */
#define __ACW		0X02
#define __DCW		0X04
#define __IR		0X08
#define __GR		0X10
#define __CC		0X20
#define __BBD		0X40

#define MODEL_EN		(type_spe.mode_en)



/* Ӳ������ */
#define BUZZER_EN	g_custom_sys_par.buzzer_en	/* ������ʹ�� */
#define MUTE_EN     g_custom_sys_par.mute_sw   /* ϵͳ����ģʽ���� */


#define OFFSET_ON	0
#define OFFSET_OFF	1

/* ���͵Ķ˿��� */
typedef enum{
    PORT_NUM0	= 0,
    PORT_NUM4	= 4,
    PORT_NUM8	= 8,
    PORT_NUM14	= 14,
}PORT_NUM_TYPE;

typedef enum{
	HZ_TYPE_GRADE = 1,/* �ȼ�ģʽ */
	HZ_TYPE_CONT  = 0,/* ����ģʽ */
}FREQ_TYPE;/* Ƶ������ */

/* С��λ�� */
enum{
	POINT_0 = 0,/* 0λС�� */
	POINT_1 = 1,
	POINT_2 = 2,
	POINT_3 = 3,
	POINT_4 = 4,
};
#define ACW_TRANSFORMER_GROUP_1     case ACW_500V: \
                                    case ACW_1kV: \
                                    case ACW_5kV
#define ACW_TRANSFORMER_GROUP_2     case ACW_10kV
#define ACW_TRANSFORMER_GROUP_3     case ACW_20kV
/* ��ѹ������� */
typedef enum{
	ACW_500V    = 0,/* 0.5kV */
	ACW_1kV		= 1,/* 1kV */
	ACW_5kV		= 2,/* 5kV */
	ACW_10kV	= 3,/* 10kV ���� */
	ACW_20kV	= 4,/* 20KV ���� */
	ACW_30kV	= 5,/* 20KV ���� */
}ACW_VOL_RANGE_ENUM;
#define DCW_TRANSFORMER_GROUP_1     case DCW_2kV: \
                                    case DCW_6kV
#define DCW_TRANSFORMER_GROUP_2     case DCW_10kV
#define DCW_TRANSFORMER_GROUP_3     case DCW_20kV
typedef enum{
	DCW_500V,
	DCW_2kV,
	DCW_6kV,
	DCW_10kV,
	DCW_20kV,
}DCW_VOL_RANGE_ENUM;
typedef enum{
	IR_750V,
	IR_1kV,
	IR_1_5kV,
	IR_2_5kV,/* 2.5kV */
	IR_5kV,
	IR_10kV,
}IR_VOL_RANGE_ENUM;
/* ֱ��GR ѹ���������е�ѹ�� */
typedef enum{
	GR_NORMAL 		 = 0X0,
	GR_DC_20mV		 = 0x2,
	GR_DC_200mV		 = 0x4,
	GR_DC_2000mV	 = 0x8,
	GR_DC_12V		 = 0x10,
	GR_DC_12V_100mA  = 0x20,
	GR_DC_12V_1000mA = 0x40,
}GR_VOL_GEAR_T;
/* ����������λ */
typedef enum{
	_AC_200uA	= 0X2,
	_AC_2mA		= 0X4,
	_AC_10mA	= 0X8,
	_AC_20mA	= 0X10,
	_AC_50mA	= 0x20,
	_AC_100mA	= 0X40,
	_AC_200mA	= 0X80,

/* ֱ��������λ */
	_DC_2uA		= 0X2,
	_DC_20uA	= 0x4,
	_DC_200uA	= 0X8,
	_DC_2mA		= 0x10,
	_DC_10mA	= 0X20,
	_DC_20mA	= 0X40,
	_DC_50mA	= 0X80,
	_DC_100mA	= 0X100,
}CUR_GEAR_T;
/* ���Ƶ�� */
typedef enum{
	_AC_50HZ	= 0X2,
	_AC_60HZ	= 0X4,
	_AC_100HZ	= 0X8,
	_AC_150HZ	= 0X10,
	_AC_200HZ	= 0x20,
	_AC_250HZ	= 0X40,
	_AC_300HZ	= 0x80,
	_AC_350HZ	= 0X100,
	_AC_400HZ	= 0X200,
}FREQ_GEAR_T;
typedef enum{
	GR_DIS_VOL	= 0X01,
	GR_DIS_RES	= 0X02,
}GR_DIS_OPT;
/* ���ͽṹ�� */
typedef struct{
    char *name;
    char *ver;
    uint16_t num;
    void (*config_fun)(void);
}TYPE_STR;

typedef struct{
    uint8_t amp_select;/* �������� �Ϲ��� �� 8833���� */
    uint8_t par_medium;/* �����洢���� eep �� flash */
    uint8_t res_medium;/* ����洢���� eep �� flash */
    uint8_t cal_medium;/* У׼�洢���� eep �� flash */
    uint8_t buzzer_en;/* ������Ӳ���� */
    uint8_t mute_sw;/* ϵͳ����ģʽ���� ����ʱ���Ա�������һֱ�� */
    uint8_t amp_type;/* �������� LINE PWM */
    uint8_t leading_sw;/* ���ݵ��뵼����U�̿��� */
	uint8_t ir_gear_hold;/* IR��������ʱ�俪�� */
	uint8_t ir_speed_sw;/* IR�����ٶȶ��ƿ��� �򿪺�ϵͳ����������Ӧ�������� �� �� �� */
	uint8_t offset_set_en;/* ƫ���ֶ�����ʹ�� */
}CUSTOM_SYS_PAR;//����ϵͳ����

typedef struct{
    uint16_t type_num;/* ���ƻ���� */
    uint16_t custom_en;//���ƻ�ʹ�ܱ�־
    uint8_t name[20];/* ���ƻ����� */
    TYPE_STR    type;/* ���ƻ� */
    CUSTOM_SYS_PAR custom_sys_par;/* �˵������Ʋ��� */
}CUSTOM_TYPE_INFO;//���ƻ�����Ϣ

typedef enum{
    __STOP     = 0x1,/* stop�� start�� �ٴ������ӵ�һ����ʼ */
    __HALT     = 0x2,/* stop�� start�� �ٴ���������һ����ʼ */
    __CON      = 0x4,/* ����������������򿪾��Զ�������һ�� */
    __RESTART  = 0x8,/* start�� �ӵ�һ����ʼ */
    __NEXT     = 0x10,/* start�� ����һ����ʼ */
    __FPDFC    = 0x20,/* First pass done fail con ��һ��pass�������� ʧ�ܾͼ�������ʣ������в� */
}_FAIL_MODE_;
typedef enum{
    GR_AMP_32A,
    GR_AMP_40A,
    GR_AMP_60A,
    GR_AMP_62A,
}GR_AMP_TYPE;
typedef enum{
    SINGLE_POINT,/* �������ѹ�� */
    DOUBLE_POINT,/* ˫�����ѹ�� */
}TRANSFORMER_POINT;
/* ��ѹ����������� */
typedef enum{
    TRANS_500V,/* 25V 1kV  40  */
    TRANS_750V,/* 25V 1kV  40  */
    TRANS_1kV,/* 25V 1kV  40  */
    TRANS_2kV,/* 25V 2kV  80  */
    TRANS_5kV,/* 25V 5kV  200 */
    TRANS_10kV,/* 25V 10kV 400 */
    TRANS_20kV,/* 25V 20kV 800 */
    TRANS_30kV,/* 25V 20kV 800 */
}TRANSFORMER_TYPE;

typedef enum{
    LINE_AMP,/* ���Թ��� */
    PWM_AMP,/* PWM���� */
}AMP_TYPE;/* �������� */

typedef enum{
    SYS_SW_OFF,/* ϵͳ���عر� */
    SYS_SW_ON,/* ϵͳ���ش� */
}SYS_SW_ENUM;

typedef enum{
    MC14094_CTRL,/* 4094���ƶ�·��ѹ�� */
    CPLD_CTRL,/* CPLD���ƶ�·��ѹ�� */
}PORTS_CTRL_TYPE;

enum{
    INTERFACE_PLC       = 1<<0,
    INTERFACE_COMM      = 1<<1,
    INTERFACE_USB_OUT   = 1<<2,
    INTERFACE_USB_IN    = 1<<3,
};
enum{
    VOL_SEGMENT_NO,  /* ��ѹ���ֶ� */
    VOL_SEGMENT_YES, /* ��ѹ�ֶ� */
};
typedef enum{
    CUR_FORMAT,/* �Ե�������ʽ��ʾ��ʵ���� */
    CAP_FORMAT,/* �Ե��ݵ���ʽ��ʾ��ʵ���� */
}REAL_CUR_FORMAT;
/* �������Խṹ�� */
typedef struct{
/*************** COM ***********************/
	uint8_t mode_en;/* ģʽʹ�ܱ�־ ��1-5λ�ֱ��ʾACW DCW IR GR BBD ��ʹ��λ ��0λû���� */
	
	uint8_t offset_fun_en;/* ƫ�ƹ���ʹ�� */
    TRANSFORMER_POINT transformer_point;/* ��ѹ������ */
    TRANSFORMER_TYPE transformer_type;/* ��ѹ������ */
    AMP_TYPE amp_type;/* �������� */
	
    PORTS_CTRL_TYPE ports_ctrl_type;/* ��·����Ӳ��ѡ�� */
	PORT_NUM_TYPE port_num;/* ���Զ˿��� 0 4 8 */
    
	FREQ_TYPE hz_type;/* Ƶ�ʵ����� ��Ϊ�ȼ������� Ĭ��������ģʽ */
	FREQ_TYPE gr_hz_type;/* Ƶ�ʵ����� ��Ϊ�ȼ������� Ĭ��������ģʽ */
	uint16_t freq_h;/* Ƶ������ֵ */
	uint16_t freq_l;/* Ƶ������ֵ */
	uint8_t comm_check_en;/* ͨ�ż��ʹ�� */
	uint8_t interface_en;/* �˵��ӿ��Ƿ�������� */
    uint8_t interface_config;/* �ӿڹ������� */
    uint8_t def_fail_mode;/* ����֧�ֵ�ʧ��ģʽö�� _FAIL_MODE_ */
    uint8_t vol_is_segment;/* ��ѹ�Ƿ�ֶ�, 0���ֶ� 1���ֶ� */ 
    uint16_t vol_segment_point;/* ��ѹ�ηֽ�� */
    uint8_t support_g_mode;/* ֧��Gģʽ */
    uint16_t contact_segment_point;/* ˫�����ѹ���л���ѹ����ͷ�ĵ�ѹ�� */
    
    uint8_t silent_en;/* ����ģʽ���� */
    uint8_t plc_signal_en;/* plc�ź�ʹ�� ϵͳ�˵��� */
	uint8_t rise_time_min_value;//����ʱ����Сֵ
    
/*************** ACW ***********************/
	uint16_t acw_hz;/* 1-9 ÿ��λ��ʾһ��Ƶ�� ֻ���ڵȼ�ģʽ������ */
	uint8_t acw_real;/* acw �Ƿ�����ʵ���� */
    REAL_CUR_FORMAT real_cur_format;/* acw ��ʵ��������ʾ��ʽ */
	uint16_t acw_gear;/* acw �����3����λ 1 200uA, 2 2mA, 3 20mA ����ÿһ��λ��ʾһ����λ �� ��0λ δʹ�� ��1λ��200uA ��2λ��2mA */
	uint16_t dcw_gear;/* dcw �����5����λ 1-5 2uA 20uA 200uA 2mA 10mA �趨����ͬacw */
	ACW_VOL_RANGE_ENUM  acw_vol_range;/* acw��ѹ��λ 0����1��� 1������� */
	uint16_t acw_vol_h;/* acw ��ѹ���ֵ ��λV */
	uint16_t acw_vol_l;/* acw ��ѹ��Сֵ ��λV */
	uint16_t acw_arc_h;/* acw �绡������ģʽ�µ�����ֵ */
	uint8_t  acw_arc_gear;/* acw �绡����¼�����Ļ�׼ */
    
/*************** CC ***********************/
    uint16_t cc_gear;
    uint16_t cc_vol_h;/* acw ��ѹ���ֵ ��λV */
	uint16_t cc_vol_l;/* acw ��ѹ��Сֵ ��λV */
    
/*************** BBD ***********************/
    uint16_t bbd_vol_h;/* BBD��ѹ����ֵ */
    uint16_t bbd_vol_l;/* BBD��ѹ����ֵ */
/*************** DCW ***********************/
	DCW_VOL_RANGE_ENUM  dcw_vol_range;/* dcw��ѹ��λ 0����1��� 1 1������� */
	uint16_t dcw_vol_h;/* dcw ��ѹ���ֵ ��λv */
	uint16_t dcw_vol_l;/* dcw ��ѹ��Сֵ ��λV */
	uint16_t dcw_arc_h;/* dcw �绡������ģʽ�µ�����ֵ */
	uint8_t  dcw_arc_gear;/* dcw �绡����¼���λ */
    uint8_t  dcw_big_cap_en;/* dcw ������ */
    uint8_t  dcw_output_impedan_opt_index;/* dcw ����ݲ���ʱ��������迹��ѡ������ */
	
/*************** IR ***********************/
	IR_VOL_RANGE_ENUM  ir_vol_range;/* ir��ѹ��λ 0����1��� 1 1������� */
	uint16_t ir_vol_h;/* ir��ѹ�����ֵ ��λv */
	uint16_t ir_vol_l;/* ir��ѹ����Сֵ ��λv */
	float 	 ir_res_h;/* ir�����������ֵ ��λMohm */
	float 	 ir_res_l;/* ir�����������ֵ ��λMohm */
	uint8_t ir_short_int_en;/* �еĻ����ھ�Ե����ʱҪ����·�ж� */
	uint8_t ir_absorptance_sw;/* ir���ձȿ��� */
	
/*************** GR ***********************/
    GR_AMP_TYPE gr_amp_type;/* gr���ŵ����� */
	uint16_t gr_cur_h;/* gr���������ֵ ��λ0.01A*/
	uint16_t gr_cur_l;/* gr���������ֵ ��λ0.01A*/
	uint16_t gr_res_h;/* gr��������ֵ ��λmohm */
	uint16_t gr_res_l;/* gr�������Сֵ ��λmohm */
	uint8_t gr_hz;/* gr�����Ƶ�� ����ʱ��Ҫ ��acwһ�� */
	uint8_t gr_vol_gear;/* ֱ��gr ѹ��������ʹ�� */
    uint16_t gr_max_cur_res_rang;/* ������ʱ�ܲ�ĵ������ֵ */
    uint16_t one_dc_module_max_cur;/* һ��ģ���������������ֵ���ò���������Ϊ���ж���Ҫ����ģ�鹤��
									����Ծ�����Բ�������Ӱ�� */
	
	uint8_t gr_dis_opt; /* gr��Ҫ��ʾ����Ŀ �е�ѹ �� ���蹩���ѡ�� */
	uint8_t gr_cur_unit;/* gr�����ĵ�λ A ��mA */
	uint8_t gr_cur_decs;/* gr������С��λ�� */
	uint8_t gr_res_unit;/* gr����ĵ�λ �� ��m�� */
	uint8_t gr_res_decs;/* gr�����С��λ�� */
	uint8_t gr_vol_unit;/* gr��ѹ��λ */
	uint8_t gr_vol_decs;/* gr��ѹ��С��λ�� */
	
	uint8_t gr_dc_en;/* ���gr �ǽ����Ļ���ֱ���� */
	uint8_t gr_vol_drop_mode;/* grֱ��ѹ��ģʽ 1�� 0�ر� */
	uint8_t gr_cur_divide_gear_en;/* grֱ��ѹ��ģʽ �����ֵ�����ʹ�� */
    uint8_t single_gr;/* ��GR��� */
	
/*************** OTHER ***********************/
	uint8_t g_mode_en;/* �еĻ�����Gģʽ �ñ�־�ɳ����Զ�����Ƿ���ACW\DCWģʽ�������Ƿ�ʹ�� */
	uint8_t syn_roads_en;/* ��·ͬ��ʹ�� */
}TYPE_SPECIALITY;

/***************** ȫ�ֱ������� ******************/
#ifdef   _CS99xx_TYPE
#define  _TYPE_EXT
#else
#define  _TYPE_EXT  extern
#endif

_TYPE_EXT const TYPE_STR        *g_cur_type;/* ָ��ǰ���ͽṹ��ȫ��ָ�� */
_TYPE_EXT TYPE_SPECIALITY       type_spe;  /* �����ͺŵ��ص� */
_TYPE_EXT CUSTOM_SYS_PAR        g_custom_sys_par;/* ����ϵͳ���������ŵ��ڲ�flash�� */
/*************** �������� ***************/
extern uint8_t is_custom_type(void);
extern void set_custom_type_info(void);
extern void updata_sys_language_par(void);
extern void judge_amp_type(void);
extern void read_custom_par(void);
extern void save_custom_par(void);
extern void select_type(void);
extern int32_t check_type(void);
extern void init_machine(const uint8_t mode);
extern void init_default_type(void);
extern void judge_single_gr(void);
extern uint8_t get_max_cur_gear(uint8_t mode);
extern uint32_t define_hz_kinds(uint8_t mode, const uint8_t** gear_buf, uint8_t *flag);
extern uint32_t define_cur_kinds(uint8_t mode, const uint8_t *gear[], uint8_t* flag);
extern uint32_t define_fail_mode_kinds(const uint8_t *fail_mode_buf[], uint8_t *flag);
extern uint32_t define_vol_kinds(uint8_t mode, const uint8_t **gear, uint8_t *flag);
extern int32_t check_mode(void);
extern int32_t check_this_mode(uint8_t mode);
extern uint32_t define_interface_config(uint8_t *flag);
extern uint8_t * get_version_number(void);
extern uint8_t get_acw_max_gear(void);
extern uint8_t get_cc_max_gear(void);
extern uint8_t get_dcw_max_gear(void);
extern int32_t check_gr_mode(void);

#endif //__TYPE_SELECT__

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
