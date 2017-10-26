
#ifndef __AUTO_CALIBRATION_H__
#define __AUTO_CALIBRATION_H__


#ifndef NULL
#define NULL 0
#endif

typedef enum{
    /* ACW 0-12 */
    /* ��ѹ */
    ACW_VOL_SEG_11,
    ACW_VOL_SEG_12,
    ACW_VOL_SEG_13,
    ACW_VOL_SEG_21,
    ACW_VOL_SEG_22,
    ACW_VOL_SEG_23,
    /* ���� */
    ACW_CUR_G_200uA,
    ACW_CUR_G_2mA,
    ACW_CUR_G_10mA,
    ACW_CUR_G_20mA,
    ACW_CUR_G_50mA,
    ACW_CUR_G_100mA,
    ACW_CUR_G_200mA,
    
    /* DCW 13-26 */
    /* ��ѹ */
    DCW_VOL_SEG_11,
    DCW_VOL_SEG_12,
    DCW_VOL_SEG_13,
    DCW_VOL_SEG_21,
    DCW_VOL_SEG_22,
    DCW_VOL_SEG_23,
    /* ���� */
    DCW_CUR_G_2uA,
    DCW_CUR_G_20uA,
    DCW_CUR_G_200uA,
    DCW_CUR_G_2mA,
    DCW_CUR_G_10mA,
    DCW_CUR_G_20mA,
    DCW_CUR_G_50mA,
    DCW_CUR_G_100mA,
    
    /* IR 27-48 */
    /* ��ѹ */
    IR_VOL_SEG_11,
    IR_VOL_SEG_12,
    IR_VOL_SEG_13,
    IR_VOL_SEG_21,
    IR_VOL_SEG_22,
    IR_VOL_SEG_23,
    /* ���� */
    IR_RES_G_11,
    IR_RES_G_12,
    IR_RES_G_13,
    IR_RES_G_21,
    IR_RES_G_22,
    IR_RES_G_23,
    IR_RES_G_31,
    IR_RES_G_32,
    IR_RES_G_33,
    IR_RES_G_41,
    IR_RES_G_42,
    IR_RES_G_43,
    IR_RES_G_51,
    IR_RES_G_52,
    IR_RES_G_53,
    IR_RES_G_54,
    
    /* GR 49-54 */
    /* ���� */
    GR_CUR_SEG_11,
    GR_CUR_SEG_12,
    GR_CUR_SEG_13,
	
    GR_CUR_SEG_21,
    GR_CUR_SEG_22,
    GR_CUR_SEG_23,
    
    GR_CUR_SEG_31,
    GR_CUR_SEG_32,
    GR_CUR_SEG_33,
	
    GR_CUR_SEG_41,
    GR_CUR_SEG_42,
    GR_CUR_SEG_43,
	
    GR_CUR_SEG_51,
    GR_CUR_SEG_52,
    GR_CUR_SEG_53,
	
    GR_CUR_SEG_61,
    GR_CUR_SEG_62,
    GR_CUR_SEG_63,
    
    GR_CUR_SEG_71,
    GR_CUR_SEG_72,
    GR_CUR_SEG_73,
	
    GR_CUR_SEG_81,
    GR_CUR_SEG_82,
    GR_CUR_SEG_83,
	
    ORDER_SIZE,
}CAL_ORDER_EUME;
/* У׼���ݵĻ�����Ϣ */
typedef struct{
    uint8_t *name;/* У׼��Ŀ���� */
    float ad;/* �ɼ���ADֵ */
    float ad_vol;/* �ɼ��ĵ�ѹADֵ */
    float ad_cur;/* �ɼ��ĵ���ADֵ */
    float da;/* �����DAֵ */
    float dis;/* У׼�����ʾֵ */
    uint8_t gear;/* Ŀ�굵λ */
    uint32_t measure;/* ����ֵ */
}CAL_BASE_INFO;

/* У׼��ѹʹ�õ����ݽṹ */
typedef struct{
    CAL_BASE_INFO base[3];
    float *da_k;/* ָ��DAУ׼ϵ��ȫ�ֱ�����k */
    float *da_b;/* ָ��DAУ׼ϵ��ȫ�ֱ�����b */
    float *ad_k;/* ָ��ADУ׼ϵ��ȫ�ֱ�����k */
    float *ad_b;/* ָ��ADУ׼ϵ��ȫ�ֱ�����b */
    void (*count_ratio)(void* p_this);/* ����ϵ�� */
    void (*save)(const uint8_t mode);/* ����ϵ�� */
}AUTO_CAL_VOL;

/* У׼�ӵص���ʹ�õ����ݽṹ */
typedef struct{
    CAL_BASE_INFO base[3];
    float *da_k;/* ָ��DAУ׼ϵ��ȫ�ֱ�����k */
    float *da_b;/* ָ��DAУ׼ϵ��ȫ�ֱ�����b */
    float *ad_v_k;/* ָ��ADУ׼ϵ��ȫ�ֱ�����k */
    float *ad_v_b;/* ָ��ADУ׼ϵ��ȫ�ֱ�����b */
    float *ad_i_k;/* ָ��ADУ׼ϵ��ȫ�ֱ�����k */
    float *ad_i_b;/* ָ��ADУ׼ϵ��ȫ�ֱ�����b */
    void (*count_ratio)(void* p_this);/* ����ϵ�� */
    void (*save)(const uint8_t mode);/* ����ϵ�� */
}AUTO_CAL_GR_CUR;

/* У׼����ʹ�õ����ݽṹ */
typedef struct{
    CAL_BASE_INFO base;
    float da_k;/* ��������DAֵ */
    float da_b;/* ��������DAֵ */
    float *ad_i_k;/* ָ��У׼ϵ��ȫ�ֱ�����k */
    uint16_t output;/* �����ѹ */
    void (*count_ratio)(void* p_this);/* ����ϵ�� */
    void (*save)(const uint8_t mode);/* ����ϵ�� */
}AUTO_CAL_CUR;

/* У׼IR����ʹ�õ����ݽṹ */
typedef struct{
    CAL_BASE_INFO base;
    float *k;/* ָ��У׼ϵ��ȫ�ֱ�����k */
    uint8_t count;/* ���� */
    void (*count_ratio)(void* p_this);/* ����ϵ�� */
    void (*save)(const uint8_t mode);/* ����ϵ�� */
}AUTO_CAL_IR_RES;

/* У׼����ʹ�õ����ݽṹ */
typedef union{
    AUTO_CAL_VOL vol;
    AUTO_CAL_CUR cur;
    AUTO_CAL_IR_RES ir_res;
    AUTO_CAL_GR_CUR gr;
}CAL_INFO_UN;


typedef struct{
    uint16_t num;/* ������ */
    uint16_t type;/* ���� */
    uint8_t mode;/* ����ģʽ */
    void (*fun)(void);/* ������ */
    uint32_t data;/* �û����� */
    uint32_t range;/* ���ݷ�Χ */
    void (*get_cal_info)(CAL_BASE_INFO *info);/* ��ȡУ׼����Ϣ */
}CAL_ORDER;

typedef struct{
    uint8_t state;/* У׼״̬ */
    uint16_t cal_total_points;/* �ܹ���У׼���� */
}ATUTO_CAL_FLAG;

#ifdef   AUTO_CALIBRATE_GLOBALS
#define  AUTO_CALIBRATE_EXT
#else
#define  AUTO_CALIBRATE_EXT  extern
#endif

// AUTO_CALIBRATE_EXT uint8_t frame_buf[20];

AUTO_CALIBRATE_EXT CAL_ORDER *cal_order;//[ORDER_SIZE];
AUTO_CALIBRATE_EXT ATUTO_CAL_FLAG auto_cal;/* �Զ�У׼ʹ�õ����� */

AUTO_CALIBRATE_EXT float auto_measure;  /* �Զ�У׼����ֵ */

AUTO_CALIBRATE_EXT uint16_t g_cur_cal_opt_num;/* ��ǰУ׼��Ŀ��� */
AUTO_CALIBRATE_EXT CAL_INFO_UN cal_info_un;/* У׼��Ϣ������ */

extern void select_cur_range_pool(uint8_t mode);
extern void auto_calibration(void);


#endif //__AUTO_CALIBRATION_H__

