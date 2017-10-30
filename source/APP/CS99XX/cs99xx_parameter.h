/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_parameter.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#ifndef __CS99XX_PARAMETER_H__
#define __CS99XX_PARAMETER_H__

/**************** �����ļ� ***********************/
#include "stm32f10x.h"
#include "cs99xx_struct.h"
#include "GUI_Draw.h"
#include "cs99xx_type.h"
/**************** �궨�� *************************/

#define KEY_START_COUNT		(1)	/* ��START������ */
#define KEY_STOP_COUNT		(2)	/* ��STOP������ */
#define AMP_COUNT			(3)	/* ��AMP���� */
#define BOOT_COUNT			(4)	/* ��BOOT���� */

#define DUMMY			    (0)	/* ����Ҫ���ĵ�һ��ֵ */

#define CHIP0	EEPROM_Block0_ADDRESS
#define CHIP1	EEPROM_Block1_ADDRESS
#define CHIP2	EEPROM_Block2_ADDRESS
#define CHIP3	EEPROM_Block3_ADDRESS

#define CUR_KINDS_MAX		(10) /* ������������ֵ */
#define VOL_KINDS_MAX		(7)	 /* ��ѹ��λ�������ֵ */
#define MODEL_KINDS_MAX		(8)	 /* ����֧�ֵ�ģʽ���� */
/*************** ������Χ ************************/
                                                     /* ���� ˵�� */
#define FREQ_HZ_H		(type_spe.freq_h)
#define FREQ_HZ_L		(type_spe.freq_l)
#define ACW_VOL_H		(type_spe.acw_vol_h)         /* 5Kv */
#define ACW_VOL_L		(type_spe.acw_vol_l)		 /* 50V */
#define ACW_ARC_H		(type_spe.acw_arc_h)
#define ACW_ARC_GEAR	(type_spe.acw_arc_gear)

#define CC_VOL_L		(type_spe.cc_vol_l)		 /* 50V */
#define CC_VOL_H		(type_spe.cc_vol_h)

#define	DCW_VOL_H		(type_spe.dcw_vol_h)         /* 6Kv */
#define DCW_VOL_L		(type_spe.dcw_vol_l)         /* 50v */
#define DCW_ARC_H		(type_spe.dcw_arc_h)
#define DCW_ARC_GEAR	(type_spe.dcw_arc_gear)

#define IR_VOL_H		(type_spe.ir_vol_h)         /* 1Kv */
#define IR_VOL_L		(type_spe.ir_vol_l)         /* 50v */
#define IR_RES_H		(type_spe.ir_res_h)         /* �����������ֵ */
#define IR_RES_L		(type_spe.ir_res_l)         /* ����������Сֵ */
#define IR_RANGE_H      (cur_ir_rang_h)             /* IR�������� */


#define ONE_DCGR_MAX_CUR        (type_spe.one_dc_module_max_cur) //   3000    // 30A
#define GR_CUR_H		        (type_spe.gr_cur_h)         /* 32A */
#define GR_CUR_L		        (type_spe.gr_cur_l)         /* 1A */
#define GR_RES_H(temp_cur)		((float)(type_spe.gr_cur_h / 100.0) / (temp_cur) * 1000.0 * type_spe.gr_max_cur_res_rang + 0.5)/* GR ���������ֵ */
#define GR_RES_L		        (type_spe.gr_res_l)         /* GR ���������ֵ */
#define GR_VOL_DROP_EN	        (type_spe.gr_vol_drop_mode) /* GR ѹ��ģʽ���� 1�� 0�ر� */
#define GR_VOL_DIV_SEGMENT_EN   (type_spe.gr_cur_divide_gear_en)/* GR��ѹ�ֶ�ʹ�� */
#define DC_GR_EN		        (type_spe.gr_dc_en)
#define SYN_ROADS_EN            (type_spe.syn_roads_en) /* ��·ͬ��ʹ�� */
#define BBD_VOL_H		        (type_spe.bbd_vol_h)		                /* ACW_VOL_H */
#define BBD_VOL_L		        (type_spe.bbd_vol_l)		                /* 100V */
#define BBD_OPEN_L              (menu_par[GUI_BBD_OPEN_CHECK].lower)  /* ��·������� */
#define BBD_OPEN_H              (menu_par[GUI_BBD_OPEN_CHECK].upper)  /* ��·������� */
#define BBD_SHORT_L             (menu_par[GUI_BBD_SHORT_CHECK].lower)  /* ��·������� */
#define BBD_SHORT_H             (menu_par[GUI_BBD_SHORT_CHECK].upper)  /* ��·������� */

/**************************************/
enum{
    OUT_AC,/* ������� */
    OUT_DC,/* ���ֱ�� */
};
typedef struct {
    uint8_t* name;
    const uint8_t* high_str;
    const uint8_t* low_str;
    uint8_t unit;
    uint32_t high_max;/* �������ֵ */
    uint32_t high_min;/* ������Сֵ */
    uint32_t low_max;/* �������ֵ */
    uint32_t low_min;/* ������Сֵ */
    uint8_t decs;/* С����λ�� */
    uint8_t comm;///<ͨ�ŵ�λ
}GEAR_STR;
extern GEAR_STR ac_gear[];
extern GEAR_STR dc_gear[];
extern GEAR_STR ir_gear[];

typedef struct {
    uint8_t rang_t;
    uint8_t unit;
    uint8_t decs;/* С����λ�� */
}VOL_RANGE_INFO;
extern VOL_RANGE_INFO acw_vol_info[];
extern VOL_RANGE_INFO dcw_vol_info[];
extern VOL_RANGE_INFO ir_vol_info[];
// extern MENU_INFO sys_menu[9];
/**************** �������� ***********************/
extern int32_t count_gr_cur_no_dis_bits(uint32_t val);
extern uint8_t get_gr_cur_no_dis_bits(void);

extern void serve_charkey(const uint16_t keyvalue);
extern void set_subtask(const int8_t w);
extern void menu_subtask(void);
extern void test_subtask(void);
extern void help_subtask(void);

extern void init_par_info(void);
extern void init_test_ui_info(uint8_t whitch);
extern void dis_test_ui(void);
extern void test_res_dis(void);
extern void serve_count(const int8_t n);
extern void set_contrast(const int8_t gear);

extern void save_as_cur_group(const uint8_t file_num);
extern void init_mode(NODE_STEP *p);
extern uint16_t define_modes(const uint8_t **mode_buf, uint8_t *flag, int16_t *kinds);
extern uint16_t define_g_modes(const uint8_t **mode_buf, uint8_t *flag, int16_t *kinds);

extern void updata_ac_cur_gear_affect_par_info(uint8_t mode, uint8_t gear);
extern void updata_dc_cur_gear_affect_par_info(uint8_t mode, uint8_t gear);

extern void updata_acw_cur_gear_affect_par_info(UN_STRUCT *p);
extern void updata_dcw_cur_gear_affect_par_info(UN_STRUCT *p);
extern void updata_gr_cur_affect_par_info(UN_STRUCT *p);
extern void updata_gr_vol_gear_affect_par_info(UN_STRUCT *p);

extern DATA_INFO*  server_combination_key(const uint16_t keyvalue);
extern void clear_menu(void);
extern void prompt_recover_pw(void);
extern void dis_program_info(void);
extern void dis_interface_info(void);
extern void dis_exception_log(void);
extern void serve_numkey(const uint16_t keyvalue);
extern void serve_pwkey(const uint16_t keyvalue, int8_t *m);
extern void test_unit_dis(uint8_t mode, uint8_t gear, const GUI_FONT * pNewFont);
extern void res_dis_vol(const RESULT_STR *p, const int16_t x, const int16_t y);
extern void res_dis_cur(const RESULT_STR *p, const int16_t x, const int16_t y);

/*************************** �ⲿ���� *************************/
extern void updata_title_step(UN_STRUCT *p);
extern void updata_set_step(UN_STRUCT *p);
extern void serve_test_shortcut(void);
extern void res_list_dis(void);
extern void init_int(void);
extern uint8_t check_file_exist(const uint8_t n);/* ����ļ��Ƿ���� */
extern TEST_FILE *pos_file(const uint8_t n);/* ��λ�ļ� ���ظ��ļ�ָ�� */
extern void init_sys_par_list(SYS_PAR 	*p_sys);
extern void set_either_or_sys_par(void *p);
extern void set_float_sys_par(void *psys);
extern void set_option_sys_par(void *psys);
extern void set_plc_option_sys_par(void *psys);
extern void set_custom_options_par(void *pcus);

extern void sys_lockkeyboard(uint32_t w);
extern void sys_recover(uint32_t w);
extern void sys_offset(uint32_t w);
extern void sys_bbd(uint32_t w);
extern void sys_result(uint32_t w);
extern void sys_memory(uint32_t w);
extern void sys_sys(uint32_t w);
extern void sys_interface(uint32_t w);
extern void sys_password(uint32_t w);
extern void sys_settime(uint32_t w);
extern void sys_updata(uint32_t w);

extern void init_custom_par_list(CUSTOM_SYS_PAR *p_custom);
extern void init_par_info_list(NODE_STEP *p_node);
extern void serve_test(void);
extern void init_99xx_list(void);
extern void init_offset_par(NODE_STEP *p);
extern void set_step(void *p);
extern void set_mode(void *p);
extern void set_voltage(void *p);
extern void set_all_ports(void *p);
extern void set_either_or_par(void *p);
extern void set_float_par(void *p);
extern void set_float_par_for_syn(void *pun);
extern void set_int_sys_par(void *pun);
extern void sys_check_date(void);
extern void set_test_time(void *p);
extern void set_frequency(void *p);
extern void set_ac_cur_gear(void *p);
extern void set_dcw_cur_gear(void *p);
extern void set_gr_output_cur(void *p);
extern void set_gr_vol_gear(void *p);
extern void set_gr_res_upper(void *p);
extern void set_gr_res_lower(void *p);
extern void set_gr_vol_upper(void *p);
extern void set_gr_vol_lower(void *p);
extern void set_gr_test_method(void *p);
extern void updata_gr_vol_gear_affect_par_info(UN_STRUCT *p);
extern DATA_INFO* find_menu(uint8_t n);
extern void set_option_par(void *p_node);

#endif //__CS99XX_PARAMETER_H__
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE*******************/
