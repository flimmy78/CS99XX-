
#ifndef __DC_MODULE_H__
#define __DC_MODULE_H__

#include "module.h"

#define DC_MODULE_NUM       2
#define DC_MODULE_NUM_MAX   15

typedef struct{
    uint16_t da;        /* ���DA */
    uint16_t cur_ad;    /* ����AD */
    uint16_t vol_ad;    /* ��ѹAD */
    uint8_t status;     /* ����״̬ */
    float cur;          /* ����ֵ */
    float vol;          /* ��ѹֵ */
}MODULE_DATA;

typedef struct{
    uint16_t cur;/* ����ֵ */
    uint16_t vol;/* ��ѹֵ */
}MODULE_DATA_SHORT;

typedef struct{
    uint16_t da;        /* ���DA */
    uint8_t gear;       /* ��ѹ��λ */
    uint8_t method;     /* ���Է��� */
}DC_GR_PARAMETER;

typedef struct{
    MODULE_INFO id;     /* ģ��ID��Ϣ */
    MODULE_DATA data;   /* ģ��������Ϣ */
    MODULE_DATA_SHORT short_data;   /* ģ��������Ϣ ������*/
    DC_GR_PARAMETER par;/* ģ�����ò��� */
    uint8_t st;         /* ״̬ */
    uint8_t en;         /* ʹ�� */
    uint8_t sel;        /* ѡ���־���ڵ�������ģʽ��Э������ģʽ */
    uint8_t usable;     /* ���ݿ��ñ�־ */
}DC_GR_MASTER_INFO;


typedef enum {
    DC_MODULE_CMD_START,        /* �������� */
    DC_MODULE_CMD_STOP,         /* ֹͣ���� */
    DC_MODULE_CMD_FETCH,        /* ��ѯ�������� */
    DC_MODULE_CMD_METHOD,       /* ���ò��Է��� */
    DC_MODULE_CMD_GEAR,         /* ���õ�λ */
    DC_MODULE_CMD_DC,           /* ����DA���ֵ */
    DC_MODULE_CMD_INFO,         /* ��ȡģ����Ϣ */
    DC_MODULE_CMD_SET_CAL_DATA, /* ����У׼���� */
    DC_MODULE_CMD_GET_CAL_DATA, /* ��ȡУ׼���� */
    DC_MODULE_CMD_SCAN_MODULES, /* ɨ��ģ�� */
    DC_MODULE_CMD_SET_DATA,     /* ���ò��� */
    DC_MODULE_CMD_FETCH_SHORT,  /* ��ѯ���������ָ�� */
}DC_MODULE_CMD_T;

typedef enum{
    DC_MODULE_ST_STOPING,//ֹͣ����
    DC_MODULE_ST_TESTING,//���ڲ���
    DC_MODULE_ST_OPENERR,//��·����
    DC_MODULE_ST_AMPERR,//���ű���
}DC_MODULE_ST;

enum{
    DC_GR_SEL_ONE,/* ѡ��һ��ģ�� */
    DC_GR_SEL_ALL,/* ѡ�����е�ģ�� */
};

#ifdef   DC_GR_GLOBALS
#define  DC_GR_EXT
#else
#define  DC_GR_EXT  extern
#endif

DC_GR_EXT   DC_GR_MASTER_INFO       dc_gr_master_info[DC_MODULE_NUM_MAX + 1];

extern uint8_t get_dc_module_using_num(void);
extern int32_t get_dc_gr_ad(uint8_t slave_addr, uint16_t *cur_da, uint16_t *vol_da);
extern uint8_t get_cur_cal_dc_module_addr(void);
extern void set_cur_cal_dc_module_addr(uint8_t addr);
extern void set_dc_gr_output_da(uint8_t slove_num, uint16_t da_value);
extern uint32_t get_dc_gr_module_id(uint8_t slave_num);
extern int32_t slelect_cal_dc_gr_module(uint8_t *n);
extern int32_t find_dc_gr_map_ratio_n(uint8_t slove_num, uint8_t *map_r_n);
extern void get_dc_gr_module_inf(MODULE_INFO *pool, uint32_t *count);
extern void select_dc_gr_moduls(uint8_t type, uint8_t slave_addr);
extern void select_dc_gr_n_moduls(uint8_t n);
extern void send_dc_module_cmd(DC_MODULE_CMD_T cmd, uint16_t value);
extern int32_t ask_dc_gr_start_ok(void);
extern void clear_dc_gr_st(void);
extern void check_dc_gr_moduls(void);
extern void get_dc_gr_cal_info(void);
extern void set_dc_gr_cal_info(void);
extern int32_t get_dc_gr_data(float *cur, float *vol);
extern void clear_dc_gr_data(void);
extern void set_dc_gr_vol_gear(uint8_t gear);
extern void set_dc_gr_test_method(uint8_t method);
#endif //__DC_MODULE_H__


/******************* (C) COPYRIGHT 2016 ��ʢ���� *****END OF FILE*******************/

