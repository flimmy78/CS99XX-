
#ifndef __TEST_COM_H__
#define __TEST_COM_H__

#include "cs99xx_struct.h"
#include "cs99xx_it.h"

#ifndef NULL
#define NULL 0
#endif

typedef enum {
	ST_VOL_RISE     = 0,
	ST_TESTING      = 1,
	ST_VOL_FALL     = 2,
	ST_INTER_WAIT   = 3,
	ST_WAIT         = 4,
	ST_PASS         = 5,
	ST_STOP         = 6,            /* 复位状态 */
	ST_ERR_H        = 7,
	ST_ERR_L        = 8,
	ST_ERR_SHORT    = 9,
	ST_ERR_VOL_ABNORMAL = 10,       /* 电压异常 */
	ST_ERR_ARC      = 11,
	ST_ERR_GFI      = 12,
	ST_ERR_FAIL     = 13,
	ST_ERR_REAL     = 14,           /* 真实电流报警 */
	ST_ERR_CHAR     = 15,           /* 充电报警 */
	ST_ERR_GEAR     = 16,           /* 量程报警 / 档位报警 */
	ST_ERR_AMP      = 17,
	ST_OUTPUT_DELAY = 18,
	ST_ERR_OPEN     = 19,           /* 开路报警 */
    
    ST_ERR_NONE     ,///<没有发生异常
    ST_CHANGE       ,///<电压缓变
    ST_CHARGE       ,///<正在充电
    ST_STAB         ,///<稳压时间
    ST_DISCHARGE    ,///<正在放电
    ST_DISCHARGE_OVER,///<放电结束
    
}TestingStatusTypedef;  /*运行时状态*/

typedef enum{
    STAGE_READY     = 0,    /* 预备阶段 */
    STAGE_RISE      = 1,    /* 上升阶段 */
    STAGE_TEST      = 2,    /* 测试阶段 */
    STAGE_FALL      = 3,    /* 下降阶段 */
    STAGE_CHANGE    = 4,    /* 缓变阶段 */
    STAGE_CHARGE    = 5,    /* 充电阶段 */
    STAGE_DIS_CHA   = 6,    /* 放电阶段 */
    STAGE_INTER     = 7,    /* 间隔阶段 */
    STAGE_STAB      = 8,    /* 稳压阶段 */
    STAGE_RE_READY  = 9,    /* 再次预备阶段 */
    STAGE_FAIL_CONT = 10,   /* 失败继续阶段 */
}TEST_STAGE;

extern void cs_delay_ms(int time);
extern void cs_delay_us(int time);
extern void close_test_timer(void);
extern void open_test_timer(void);
extern void init_com_test_env(TEST_DATA_STRUCT *test_data);

#endif //__TEST_COM_H__

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
