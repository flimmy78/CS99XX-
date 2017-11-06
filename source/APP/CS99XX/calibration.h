/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�calibration.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#ifndef __CALIBRATION_H__
#define __CALIBRATION_H__


/**************** �궨�� ********************/

#define		EEP_MONITOR		0X5A5AA5A5

/**************** ���ݽṹ���� ********************/
#define MAX_SYN_TOTAL_NUM 60  //
typedef struct{
	float dac_k[2];/* dac��ѹϵ�� */
	float dac_b[2];
	float adc_v_k[2];/* ��ѹϵ�� */
	float adc_v_b[2];
    
	float adc_cur_k[10];
	
	float arc_k;/* �绡У׼ֵ ��ʹ��ʱҪ��һ��ϵ��k=arc_value/4095 */
	
	float adc_r_k;/* ��ʵ����ϵ�� */
	float adc_r_b;
	
	float syn_adc_2mA[MAX_SYN_TOTAL_NUM];
	float syn_adc_20mA[MAX_SYN_TOTAL_NUM];
	float syn_adc_100mA[MAX_SYN_TOTAL_NUM];
    
    float free_space[10];/* �����ռ� �Ա��������� ��Ӱ�������С �������� �ṹsize ���� */
    
    uint32_t crc_val;/* crcУ��ֵ ����������һ���ֶ� */
}RATIO_ACW;

typedef struct{
	float dac_k[2];/* dac��ѹϵ�� */
	float dac_b[2];
	float adc_v_k[2];/* ��ѹϵ�� */
	float adc_v_b[2];
    
	float adc_cur_k[10];
	
	float arc_k;/* �绡У׼ֵ ��ʹ��ʱҪ��һ��ϵ��k=arc_value/4095 */
	
	float adc_r_k;/* ��ʵ����ϵ�� */
	float adc_r_b;
    
    float free_space[10];/* �����ռ� �Ա��������� ��Ӱ�������С �������� �ṹsize ���� */
    
    uint32_t crc_val;/* crcУ��ֵ ����������һ���ֶ� */
}RATIO_BBD;

typedef struct{
	float dac_k[2];/* dac��ѹϵ�� */
	float dac_b[2];
	float adc_v_k[2];/* ��ѹϵ�� */
	float adc_v_b[2];
	
	float adc_cur_k[10];
	
	float arc_k;/* �绡У׼ֵ ��ʹ��ʱҪ��һ��ϵ��k=arc_value/4095 */
	
	float adc_a_b;
	float adc_r_k;/* ��ʵ����ϵ�� */
	float adc_r_b;
    
	float syn_adc_2mA[MAX_SYN_TOTAL_NUM];
	float syn_adc_20mA[MAX_SYN_TOTAL_NUM];
	float syn_adc_100mA[MAX_SYN_TOTAL_NUM];
    
    float free_space[10];/* �����ռ� �Ա��������� ��Ӱ�������С �������� �ṹsize ���� */
    
    uint32_t crc_val;/* crcУ��ֵ ����������һ���ֶ� */
}RATIO_DCW;


typedef struct{
#define SECT_VOL     22  //��ѹ����������ֵ
#define CAL_POINTS   18  //У׼�����Ŀ���ֵ
    
	float dac_k[2];/* dac��ѹϵ�� */
	float dac_b[2];
	float adc_v_k[2];/* ��ѹϵ�� */
	float adc_v_b[2];
	float da_ad_k[2];/* da��ad�Ķ�Ӧϵ��k */
	float da_ad_b[2];
	
	float res_3_k[CAL_POINTS][SECT_VOL];//16������У׼�� 5����λ 22����ѹ��
	float cur_ad_v[CAL_POINTS][SECT_VOL];//16������У׼�� 5����λ 22����ѹ�� �ĵ���������ѹֵ��¼
	uint8_t gear_b[CAL_POINTS][SECT_VOL];//16������У׼�� 22����ѹ�� ��У׼ʱ���ڵ�Ӳ����λ��¼
	
    uint32_t crc_val;/* crcУ��ֵ ����������һ���ֶ� */
}RATIO_IR;

typedef struct{
#define GR_VOL_GEARS 8	/* 0 ����GR; 1 20mV; 2 200mV; 3 2000mV; 4 12V; 5 12V-100mA; 6 12V-1000mA */
	
	float dac_k[GR_VOL_GEARS];/* dac��ѹϵ�� */
	float dac_b[GR_VOL_GEARS];
	float adc_v_k[GR_VOL_GEARS];/* ��ѹϵ�� */
	float adc_v_b[GR_VOL_GEARS];
	float adc_i_k[GR_VOL_GEARS];/* ����ϵ�� */
	float adc_i_b[GR_VOL_GEARS];
    
    float free_space[10];/* �����ռ� �Ա��������� ��Ӱ�������С �������� �ṹsize ���� */
    
    uint32_t crc_val;/* crcУ��ֵ ����������һ���ֶ� */
}RATIO_GR;


typedef struct{
	float dac_k;/* dac��ѹϵ�� */
	float dac_b;
	float adc_v_k;/* ��ѹϵ�� */
	float adc_v_b;
	float adc_i_k;/* ����ϵ�� */
	float adc_i_b;
    
    uint32_t crc_val;/* crcУ��ֵ ����������һ���ֶ� */
}RATIO_DC_GR;

typedef struct {
	char *point_menu[8][5][2];/* 8 ���Է�8����ѹ������ 5 */
	float point_da[8][5][2];
}VOL_CAL, CUR_CAL;

typedef struct {
	char *point_menu[5][16];/* У׼�� ������ʾ */
	float point_da[5][16];/* ��׼����ֵ */
    uint8_t gear[5][16];/* ��Ӧ��Ӳ����λ */
}RES_CAL;

/******************* ȫ�ֱ������� *********************/
#ifdef   CALIBRATE_GLOBALS
#define  CALIBRATE_EXT
#else
#define  CALIBRATE_EXT  extern
#endif

CALIBRATE_EXT uint8_t cur_set_meal;/* ��ǰ�ײ� */
CALIBRATE_EXT uint8_t cur_cal_ir_res_meal;/* ��ǰ�ײ� IR �����ײ� */
CALIBRATE_EXT const VOL_CAL acw_vol_cal[][2];
CALIBRATE_EXT const VOL_CAL dcw_vol_cal[][2];
CALIBRATE_EXT const VOL_CAL ir_vol_cal[][2];
CALIBRATE_EXT const CUR_CAL gr_cur_cal[];
CALIBRATE_EXT const RES_CAL ir_cur_cal;
CALIBRATE_EXT const float default_ir_res_k[];

CALIBRATE_EXT RATIO_ACW ratio_acw;
CALIBRATE_EXT RATIO_DCW ratio_dcw;
CALIBRATE_EXT RATIO_IR ratio_ir;
CALIBRATE_EXT RATIO_GR ratio_gr;
CALIBRATE_EXT RATIO_DC_GR (*ratio_dc_gr)[5];
CALIBRATE_EXT RATIO_BBD ratio_bbd;


/******************* �������� *********************/
extern uint8_t get_cal_dcw_vol_meal(void);
extern uint8_t get_cal_acw_vol_meal(void);
extern uint8_t get_cal_ir_meal(void);
extern uint8_t geat_ir_res_cal_points_num(void);
extern uint8_t get_ir_res_meal(void);
extern uint8_t select_gr_cal_menu(void);
extern void clear_menu(void);
extern void serve_numkey(const uint16_t keyvalue);
void startup_cal(const int8_t mode, const int8_t gear);
void stop_cal(const int8_t mode);

void init_ratio(uint8_t mode);
void init_ratio_all(void);
void correctness(void);
uint8_t is_acw_segment(void);
uint8_t is_dcw_segment(void);
uint8_t is_ir_segment(void);
void confirm_vol_segment_s(void);
void get_acw_vol_cal_point_range(uint32_t *range);
void get_dcw_vol_cal_point_range(uint32_t *range);
void get_ir_vol_cal_point_range(uint32_t *range);

#endif //__CALIBRATION_H__
 /******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
