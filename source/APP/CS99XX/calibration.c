/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：calibration.c
 * 摘  要  ：仪器校准程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

/******************************* 加载文件 *********************************/
#define CALIBRATE_GLOBALS
#include    <string.h>
#include 	<math.h>
#include 	"GUI_Draw.h"
#include	"keyboard.h"
#include 	"MC14094.h"
#include 	"DAC_VREF.h"
#include	"SPI_CPLD.h"
#include	"ADC_DMA.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_collect.h"
#include	"cs99xx_relay_motion.h"
#include	"cs99xx_type.h"
#include	"calibration.h"
#include    "dc_module.h"
#include    "ir_shift_gear.h"

#define CALIB_JUDGE_ERR(m) if(-1 == c_judge_err(m)) { return;}


RATIO_DC_GR (*ratio_dc_gr)[5] = (RATIO_DC_GR (*)[5])&ratio_gr;

const VOL_CAL acw_vol_cal[][2]=
{
    /* 线性功放 */
    {
        /* 第一段电压 */
        {
            {
                {0},                                                                     //500V
                {{"10V"     ,"0.010kV"}, {"200V"    ,"0.200kV"}, {"500V"    ,"0.500kV"}},//1kV
                {{"20V"     ,"0.020kV"}, {"200V"    ,"0.200kV"}, {"500V"    ,"0.500kV"}},//5kV
                {{0}},                                                                   //10kV
                {{0}},                                                                   //20kV     
                {{0}},                                                                   //30kV     
            },
            {
                {0},                                               //500V
                {{25    ,10     },{550  ,200    },{1375 ,500    }},//1kV
                {{112   ,20     },{1140 ,200    },{2828 ,500    }},//5kV
                {{0}},                                             //10kV
                {{0}},                                             //20kV
                {{0}},                                             //30kV
            },
        },
        /* 第二段电压 */
        /* 10kV以上不分段 */
        {
            {
                {{"50V"     ,"0.050kV"}, {"200V"    ,"0.200kV"}, {"500V"    ,"0.500kV"}},//500V
                {{"500V"    ,"0.500kV"}, {"700V"    ,"0.700kV"}, {"1.0kV"   ,"1.000kV"}},//1kV
                {{"500V"    ,"0.500kV"}, {"2.0kV"   ,"2.000kV"}, {"5.0kV"   ,"5.000kV"}},//5kV
                {{"500V"    ,"0.500kV"}, {"5.0kV"   ,"5.000kV"}, {"9.0kV"   ,"9.000kV"}},//10kV
                {{"1000V"    ,"1.000kV"}, {"10kV"   ,"10.000kV"}, {"15kV"   ,"15.000kV"}},//20kV
                {{"3000V"    ,"3.000kV"}, {"10kV"   ,"10.000kV"}, {"15kV"   ,"15.000kV"}},//30kV
            },
            {
                {{240	 ,50    },{1000	 ,200   },{2800 ,500    }},//500V
                {{1400	 ,500   },{2100	 ,700   },{2828 ,1000   }},//1kV
                {{56*5	 ,500   },{1140	 ,2000  },{2828 ,5000   }},//5kV
                {{125    ,500   },{1300  ,5000  },{2500 ,9000   }},//10kV
                {{140    ,1000  },{1350  ,10000  },{1900 ,15000   }},//20kV
                {{250    ,3000   },{850   ,10000  },{1200 ,15000   }},//30kV
            },
        },
    },
    /* PWM功放 *//* pwm功放在变压器端分段 */
    {
        /* 第一段电压 */
        {0},
        /* 第二段电压 */
        {
            {
                {0},                                                                    //500V
                {{"10V"     ,"0.010kV"}, {"200V"    ,"0.200kV"}, {"500V"    ,"0.500kV"}},//1kV
                {{"10V"     ,"0.010kV"}, {"200V"    ,"0.200kV"}, {"500V"    ,"0.500kV"}},//5kv
                {{"500V"    ,"0.500kV"}, {"5.0kV"   ,"5.000kV"}, {"9.0kV"   ,"9.000kV"}},//10kV
                {{"500V"    ," 0.50kV"}, {"5.0kV"   ," 5.00kV"}, {" 20kV"   ,"20.00kV"}},//20kV
            },
            {
                {0},                                               //500V
                {{25	,10     },{550	,200    },{1375 ,500    }},//1kV
                {{56	,10     },{1140	,200    },{2828 ,500    }},//5kV
                {{175	,500    },{1691	,5000   },{2700 ,9000   }},//10kV
                {{175	,500    },{1691	,1000   },{3050 ,2000   }},//20kV
            },
        },
    }
};

const VOL_CAL dcw_vol_cal[][2]=
{
    /* 线性功放 */
    {
        /* 第一段电压 */
        {
            {
                {0},                                                                        //500V
                {{"10V"     , "0.010kV"}, {"200V"    , "0.200kV"}, {"500V"    , "0.500kV"}},//2kV
                {{"10V"     , "0.010kV"}, {"200V"    , "0.200kV"}, {"500V"    , "0.500kV"}},//6kV
                {0},                                                                        //10kV
                {0},                                                                        //20kV
            },
            {
                {0},                                               //500V
                {{56    ,10     },{1270 , 200   },{3700 , 500   }},//2kV
                {{56    ,10     },{1270 , 200   },{3700 , 500   }},//6kV
                {0},                                               //10kV
                {0},                                               //20kV
            },
        },
        /* 第二段电压 */
        /* 10kV以上不分段 */
        {
            {
                {{"50V"     , "0.050kV"}, {"200V"    , "0.200kV"}, {"500V"    , "0.500kV"}},//500V
                {{"500V"    , "0.500kV"}, {"1.0kV"   , "1.000kV"}, {"2.0kV"   , "2.000kV"}},//2kV
                {{"500V"    , "0.500kV"}, {"2.0kV"   , "2.000kV"}, {"6.0kV"   , "6.000kV"}},//6kV
                {{"500V"    , "0.500kV"}, {"5.0kV"   , "5.000kV"}, {"9.0kV"   , "9.000kV"}},//10kV
                {{"1000V"	, "1.000kV"}, {"10kV"	 , "10.000kV"}, {"15kV"   ,"15.000kV"}},//20kV
            },
            {
                {{150   ,50     },{600	 ,200   },{1500 , 500   }},//500V
                {{550   ,500    },{1270 , 1000  },{2500 , 2000  }},//2kV
                {{56*5  ,500    },{1270 , 2000  },{3802 , 6000  }},//6kV
                {{139   ,500    },{1582 , 5000  },{2865 , 9000  }},//10kV
                {{175   ,1000   },{1820 , 10000 },{2520 , 15000 }},//20kV
            },
        }
    },
    /* PWM功放 *//* pwm功放在变压器端分段 */
    {
        /* 第一段电压 */
        {0},
        /* 第二段电压 */
        {
            {
                {0},                                                                      //500V
                {0},                                                                      //2kV
                {{"100V"    , "0.100kV"}, {"2.0kV"  , "2.000kV"}, {"6.0kV"  , "6.000kV"}},//6kV
                {{"500V"    , "0.500kV"}, {"5.0kV"  , "5.000kV"}, {"9.0kV"  , "9.000kV"}},//10kV
                {{"500V"    , " 0.50kV"}, {"5.0kV"  , " 5.00kV"}, {" 20kV"  , "20.00kV"}},//20kV
            },
            {
                {0},                                               //500V
                {0},                                               //2kV
                {{50    ,100    },{1000 , 2000  },{2600 , 6000  }},//6kV
                {{150	,500    },{1300 , 5000  },{2500 , 9000  }},//10kV
                {{56    ,500    },{1270 , 1000  },{3802 , 2000  }},//20kV
            },
        },
    }
};

const VOL_CAL ir_vol_cal[][2]=
{
    /* 线性功放 */
    {
        /* 第一段电压 */
        {
            {
                {{"50V"     , "0.050kV" }, {"200V"  , "0.200kV"}, {"500V"    , "0.500kV"}},//750V
                {{"50V"     , "0.050kV" }, {"200V"  , "0.200kV"}, {"500V"    , "0.500kV"}},/* 1kV */
                {{"500V"    , "0.500kV" }, {"700V"  , "0.700kV"}, {"1.0kV"   , "1.000kV"}},/* 1.5kV */
                {{"50V"     , " 0.050kV"}, {"200V"  , "0.200kV"}, {"500V"    , "0.500kV"}},/* 2.5kV */
                {{"50V"     , " 0.050kV"}, {"200V"  , "0.200kV"}, {"500V"    , "0.500kV"}},/* 5kV */
                {0},                                                                       //10kV
            },
            {
                {{300   , 50     },{1300 , 200   },{3200 , 500   }},//750V
                {{310   , 50     },{1270 , 200   },{3175 , 500   }},//1kV
                {{150	, 500    },{1300 , 5000  },{3000 , 1000  }},//1.5kV
                {{310   , 50     },{1270 , 5000  },{3175 , 500   }},//2.5kV
                {{310   , 50     },{1270 , 5000  },{3175 , 500   }},//5kV
                {0},                                                //10kV
            },
        },
        /* 第二段电压 */
        {
            {
                {{"500V"  , "0.500kV"}, {"700V"     , "0.700kV"}, {"1.0kV" , "1.000kV"}},//750V
                {{"500V"  , "0.500kV"}, {"700V"     , "0.700kV"}, {"1.0kV" , "1.000kV"}},/* 1kV */
                {{0}},                                                                   /* 1.5kV */
                {{"500V"  , "0.500kV"}, {"1.0kV"    , "1.000kV"}, {"2.5kV" , "2.500kV"}},/* 2.5kV */
                {{"500V"  , "0.500kV"}, {"2.0kV"    , "2.000kV"}, {"5.0kV" , "5.000kV"}},/* 5kV */
                {{"500V"  , "0.500kV"}, {"5.0kV"    , "5.000kV"}, {"9.0kV" , "9.000kV"}},//10kV
            },
            {
                {{1500  ,500 },{2100 , 700  },{3000 , 1000  }},//750V
                {{300   ,500 },{430 , 700   },{620  , 1000  }},//1kV
                {0},                                           //1.5kV
                {{300   ,500 },{700 , 1000  },{1500 , 2500  }},//2.5kV
                {{300   ,500 },{1270 , 2000 },{3302 , 5000  }},//5kV
                {{150   ,500 },{1500 , 5000 },{3000 , 9000  }},//10kV
            },
        }
    },
    /* PWM功放 *//* pwm功放在变压器端分段 */
    {
        /* 第一段电压 */
        {0},
        /* 第二段电压 */
        {
            {
                {0},                                                                     //1kV
                {0},                                                                     //1.5kV
                {0},                                                                     //2.5kV
                {{"500V"    ,"0.500kV"}, {"5.0kV"   ,"5.000kV"}, {"9.0kV"   ,"9.000kV"}},//5kV
                {{"500V"    ,"0.500kV"}, {"5.0kV"   ,"5.000kV"}, {"9.0kV"   ,"9.000kV"}},//10kV
                {{"500V"    ," 0.50kV"}, {"5.0kV"   ," 5.00kV"}, {" 20kV"   ,"20.00kV"}},//20kV
            },
            {
                {0},                                               //1kV
                {0},                                               //1.5kV
                {0},                                               //2.5kV
                {{150	,500    },{1300 , 5000  },{3000 , 9000  }},//5kV
                {{150	,500    },{1300 , 5000  },{3000 , 9000  }},//10kV
                {{56    ,500    },{1270 , 1000  },{3802 , 2000  }},//20kV
            },
        },
    }
};

DATA_INFO par_info =
{
	(void*)0/*pData*/,0/2+10/*x*/,0/*y*/,1/*type*/,5/*lon*/,3/*decs*/,0/*total*/,1/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
};

void cal_acw_out_vol_1(uint32_t opt);
void cal_acw_out_vol_2(uint32_t opt);
void serve_correct_acw_out_v(uint32_t opt);
void serve_correct_acw_cur_uA(uint32_t opt);
void serve_correct_acw_cur_mA(uint32_t opt);
void serve_correct_acw_arc(uint32_t opt);
void serve_correct_acw_r_cur(uint32_t opt);

static MENU_INFO acw_cal_opt_info[]=
{
    {{"输出电压-1",""}, cal_acw_out_vol_1},
    {{"输出电压-2",""}, cal_acw_out_vol_2},
    {{"~A测量电流",""}, serve_correct_acw_cur_uA},
    {{"mA测量电流",""}, serve_correct_acw_cur_mA},
    {{"AC电弧侦测",""}, serve_correct_acw_arc},
//     {{"AC真实电流"     ,""}, serve_correct_acw_r_cur},
};
void serve_correct_bbd_cur_uA(uint32_t opt);
void serve_correct_bbd_cur_mA(uint32_t opt);
static MENU_INFO bbd_cal_opt_info[]=
{
    {{"~A测量电流",""}, serve_correct_bbd_cur_uA},
    {{"mA测量电流",""}, serve_correct_bbd_cur_mA},
};
void cal_dcw_out_vol_1(uint32_t opt);
void cal_dcw_out_vol_2(uint32_t opt);
void serve_correct_dcw_out_v(uint32_t opt);
void serve_correct_dcw_cur_uA(uint32_t opt);
void serve_correct_dcw_cur_mA(uint32_t opt);
void serve_correct_dcw_arc(uint32_t opt);

static MENU_INFO dcw_cal_opt_info[]=
{
    {{"输出电压-1",""}, cal_dcw_out_vol_1},
    {{"输出电压-2",""}, cal_dcw_out_vol_2},
    {{"~A测量电流",""}, serve_correct_dcw_cur_uA},
    {{"mA测量电流",""}, serve_correct_dcw_cur_mA},
    {{"DC电弧侦测",""}, serve_correct_dcw_arc},
};

void serve_correct_ir_out_v(uint32_t opt);
void cal_ir_out_vol_1(uint32_t opt);
void cal_ir_out_vol_2(uint32_t opt);
void serve_correct_ir_res(uint32_t opt);

const float default_ir_res_k[]={
1.8,18.0,180.0,1800.0,16000
};
static MENU_INFO ir_cal_opt_info[]=
{
    {{"输出电压-1" ,""}, cal_ir_out_vol_1},
    {{"输出电压-2" ,""}, cal_ir_out_vol_2},
    {{"测量电阻-1" ,""}, serve_correct_ir_res, 0, 3},
    {{"测量电阻-2" ,""}, serve_correct_ir_res, 3, 3},
    {{"测量电阻-3" ,""}, serve_correct_ir_res, 6, 3},
    {{"测量电阻-4" ,""}, serve_correct_ir_res, 9, 3},
    {{"测量电阻-5" ,""}, serve_correct_ir_res, 12, 3},
    {{"测量电阻-6" ,""}, serve_correct_ir_res, 15, 1},
};

const RES_CAL ir_cur_cal=
{
    {
        /* 1.5kV 量程10G */
        {
            "1MΩ"    , "3MΩ"  , "5MΩ",
            "8MΩ"    , "20MΩ" , "50MΩ",
            "80MΩ"   , "200MΩ", "500MΩ",
            "800MΩ"  , "2GΩ"  , "5GΩ",
            "8GΩ"    ,
        },
        /* 1.5kV 量程100G */
        {
            "1MΩ"    , "3MΩ"  , "5MΩ",
            "8MΩ"    , "20MΩ" , "50MΩ",
            "80MΩ"   , "200MΩ", "500MΩ",
            "800MΩ"  , "2GΩ"  , "5GΩ",
            "8GΩ"    , "30GΩ" , "60GΩ",
            "80GΩ"   ,
        },
        /* 5kV 量程100G */
        {
            "5MΩ"  , "7MΩ"  , "9MΩ",
            "30MΩ" , "60MΩ" , "90MΩ",
            "300MΩ", "600MΩ", "900MΩ",
            "3GΩ"  , "6GΩ"  , "9GΩ",
            "30GΩ" , "50GΩ" , "70GΩ",
            "90GΩ" ,
        },
        /* 10kV 量程100G */
        {
            "600MΩ", "1GΩ"    , "3GΩ",
            "6GΩ"  , "10GΩ"   , "30GΩ",
            "60GΩ" , "70GΩ"   , "90GΩ",
        },
        /* 1.5kV 量程100G */
        {
            "2MΩ",
            "20MΩ",
            "200MΩ",
            "2GΩ",
            "10GΩ",
            "10GΩ",
            "50GΩ",
        },
    },
    {
        /* 1.5kV 量程10G */
        {
            1  , 3   , 5,
            8  , 20  , 50,
            80 , 200 , 500,
            800, 2  * 1000, 5 * 1000, 8 * 1000, 
        },
        /* 1.5kV 量程100G */
        {
            1  , 3   , 5,
            8  , 20  , 50,
            80 , 200 , 500,
            800, 2  * 1000, 5 * 1000,
            8 * 1000, 30 * 1000, 60 * 1000, 80 * 1000,
        },
        /* 5kV 量程100G */
        {
            5   , 7   , 9,
            30  , 60  , 90,
            300 , 600 , 900,
            3  * 1000, 6  * 1000, 9 * 1000,
            30 * 1000, 50 * 1000, 70 * 1000, 90 * 1000,
        },
        /* 10kV 量程100G */
        {
            600        , 1 * 1000    , 3  * 1000,
            6  * 1000  , 10 * 1000   , 30 * 1000,
            60 * 1000  , 70 * 1000   , 90 * 1000,
        },
        /* 1.5kV 量程100G */
        {
            2,
            20,
            200,
            2  * 1000,
            10 * 1000,
            10 * 1000,
            50 * 1000,
        },
    },
/* 硬件档位:  1       2        3       4        5  */
/* 采样电阻: 1.5k     15k     150k    1.5M     15M */
    {
        /* 1.5kV 量程10G */
        {
            1,1,1,
            2,2,2,
            3,3,3,
            4,4,4,
            4,
        },
        {
            1,1,1,
            2,2,2,
            3,3,3,
            4,4,4,
            5,5,5,
            5,
        },
        {
            1,1,1,
            2,2,2,
            3,3,3,
            4,4,4,
            5,5,5,
            5,
        },
        {
            3,3,3,
            4,4,4,
            5,5,5,
        },
        {
            1,
            2,
            3,
            4, 4,
            5, 5,
        },
    },
};

void serve_correct_gr_out_cur(uint32_t opt);

static MENU_INFO gr_ac_cal_opt_info[]=
{
    {{"输出电流-AC"    ,""}, serve_correct_gr_out_cur, 0},
};
static MENU_INFO gr_dc_cal_opt_info[]=
{
    {{"输出电流-DC"    ,""}, serve_correct_gr_out_cur, 1},
};
static MENU_INFO dv_cal_opt_info[]=
{
    {{"20mV输出电流"    ,""}, serve_correct_gr_out_cur, 2},
    {{"200mV输出电流"   ,""}, serve_correct_gr_out_cur, 3},
    {{"2000mV输出电流"  ,""}, serve_correct_gr_out_cur, 4},
    {{"12V 输出电流"    ,""}, serve_correct_gr_out_cur, 5},
    {{"12V 100mA"       ,""}, serve_correct_gr_out_cur, 6},
    {{"12V 1000mA"      ,""}, serve_correct_gr_out_cur, 7},
};

const CUR_CAL gr_cur_cal[]=
{
    /* 0 用于交流 GR */
    {
        {
            {{"3A"}, {"15A"}, {"30A"}},
            {{"3A"}, {"20A"}, {"40A"}},
        },
        {
            #define GR_CAL_BASE
            #define GR_CAL_RES_POINT    100.0 /* 100mohm */
            
            {{200  , GR_CAL_RES_POINT},{1000 , GR_CAL_RES_POINT},{2000 , GR_CAL_RES_POINT}},
            {{200  , GR_CAL_RES_POINT},{1300 , GR_CAL_RES_POINT},{2700 , GR_CAL_RES_POINT}},
            
            #undef GR_CAL_BASE
            #undef GR_CAL_RES_POINT
        },
    },
    
    /* 1 用于直流 GR */
    {
        {
            {{"3A"}, {"20A"}, {"40A"}},/* 用于直流 GR */
        },
        {
            #define GR_CAL_BASE
            #define GR_CAL_RES_POINT    100.0 /* 100mohm */
            
            {{370  , GR_CAL_RES_POINT},{2000 , GR_CAL_RES_POINT},{4000 , GR_CAL_RES_POINT}},
            {{220  , GR_CAL_RES_POINT},{1500 , GR_CAL_RES_POINT},{2248 , GR_CAL_RES_POINT}},
            
            #undef GR_CAL_BASE
            #undef GR_CAL_RES_POINT
        },
    },
    
    /*****************************************************************************/
    /* 2 交流 20mV */
    {
        {
            {{"100mA"}, {"300mA"}, {"500mA"}},/* 用于直流压降测试 */
        },
        {
            #define DV_CAL_BASE 690
            #define DV_CAL_RES_POINT 10.0 /* 10ohm */
            
            {{DV_CAL_BASE	, DV_CAL_RES_POINT},{3 * DV_CAL_BASE	, DV_CAL_RES_POINT},{5 * DV_CAL_BASE , DV_CAL_RES_POINT}},
            
            #undef DV_CAL_BASE
            #undef DV_CAL_RES_POINT
        },
    },
    /* 3 交流 200mV */
    {
        {
            {{"100mA"}, {"300mA"}, {"500mA"}},/* 用于直流压降测试 */
        },
        {
            #define DV_CAL_BASE 690
            #define DV_CAL_RES_POINT 10.0 /* 10ohm */
            
            {{DV_CAL_BASE	, DV_CAL_RES_POINT},{3 * DV_CAL_BASE	, DV_CAL_RES_POINT},{5 * DV_CAL_BASE , DV_CAL_RES_POINT}},
            
            #undef DV_CAL_BASE
            #undef DV_CAL_RES_POINT
        },
    },
    /* 4 交流 2000mV */
    {
        {
            {{"100mA"}, {"300mA"}, {"500mA"}},/* 用于直流压降测试 */
        },
        {
            #define DV_CAL_BASE 690
            #define DV_CAL_RES_POINT 10.0 /* 10ohm */
            
            {{DV_CAL_BASE	, DV_CAL_RES_POINT},{3 * DV_CAL_BASE	, DV_CAL_RES_POINT},{5 * DV_CAL_BASE , DV_CAL_RES_POINT}},
            
            #undef DV_CAL_BASE
            #undef DV_CAL_RES_POINT
        },
    },
    /***********************************压降测试仪*************************************************/
    
    /* 5 用于直流 DV 不分段 */
    {
        {
            {{"100mA"}, {"300mA"}, {"500mA"}},/* 用于直流压降测试 */
        },
        {
            #define DV_CAL_BASE 690
            #define DV_CAL_RES_POINT 10.0 /* 10ohm */
            
            {{DV_CAL_BASE	, DV_CAL_RES_POINT},{3 * DV_CAL_BASE	, DV_CAL_RES_POINT},{5 * DV_CAL_BASE , DV_CAL_RES_POINT}},
            
            #undef DV_CAL_BASE
            #undef DV_CAL_RES_POINT
        },
    },
/***********************************************************************************************/
    /* 以下是分段校准 用于直流压降测试 */
    
    /* 6 第一段电流 */
    {
        {
            {{"10mA"}, {"50mA"}, {"90mA"}},
        },
        {
            #define DV_CAL_BASE         380
            #define DV_CAL_RES_POINT    100.0 /* 100ohm */
            
            {{DV_CAL_BASE * 1  , DV_CAL_RES_POINT},{DV_CAL_BASE * 5 , DV_CAL_RES_POINT},{DV_CAL_BASE * 9 , DV_CAL_RES_POINT}},
            
            #undef DV_CAL_BASE
            #undef DV_CAL_RES_POINT
        },
    },
    /* 7 第二段电流 */
    {
        {
            {{"100mA"}, {"500mA"}, {"900mA"}},
        },
        {
            #define DV_CAL_BASE         380
            #define DV_CAL_RES_POINT    10.0 /* 10ohm */
            
            {{DV_CAL_BASE * 1  , DV_CAL_RES_POINT},{DV_CAL_BASE * 5 , DV_CAL_RES_POINT},{DV_CAL_BASE * 9 , DV_CAL_RES_POINT}},
            
            #undef DV_CAL_BASE
            #undef DV_CAL_RES_POINT
        },
    }
};
uint8_t cur_set_meal;/* 当前套餐 */
uint8_t cur_cal_ir_res_meal;/* 当前套餐 IR 电阻套餐 */

/******************************* 函数定义 *********************************/
uint8_t geat_ir_res_cal_points_num(void)
{
    uint8_t i = 0;
    uint8_t meal = get_ir_res_meal();
    
    for(i = 0; i < 16; i++)
    {
        if(0 == ir_cur_cal.point_da[meal][i])
        {
            break;
        }
    }
    
    return i;
}

void confirm_vol_segment_s(void)
{
    uint32_t temp_h = type_spe.vol_segment_point;
    
    if(type_spe.vol_is_segment == VOL_SEGMENT_NO)
    {
        vol_segment = 1;
    }
    else
    {
        if(cur_vol > temp_h)
        {
            vol_segment = 1;
        }
        else
        {
            vol_segment = 0;
        }
    }
}

uint8_t is_acw_segment(void)
{
    uint8_t res1 = 0;
    uint8_t res2 = 0;
    
    res1 = (acw_vol_cal[type_spe.amp_type][0].point_menu[type_spe.acw_vol_range][0][0] != NULL); /* 判断电压段1是否可用 */
    res2 = (acw_vol_cal[type_spe.amp_type][1].point_menu[type_spe.acw_vol_range][0][0] != NULL); /* 判断电压段2是否可用 */
    
    return (res1 + res2);
}
uint8_t is_dcw_segment(void)
{
    uint8_t res1 = 0;
    uint8_t res2 = 0;
    
    res1 = (dcw_vol_cal[type_spe.amp_type][0].point_menu[type_spe.dcw_vol_range][0][0] != NULL); /* 判断电压段1是否可用 */
    res2 = (dcw_vol_cal[type_spe.amp_type][1].point_menu[type_spe.dcw_vol_range][0][0] != NULL); /* 判断电压段2是否可用 */
    
    return (res1 + res2);
}
uint8_t is_ir_segment(void)
{
    uint8_t res1 = 0;
    uint8_t res2 = 0;
    
    res1 = (ir_vol_cal[type_spe.amp_type][0].point_menu[type_spe.ir_vol_range][0][0] != NULL); /* 判断电压段1是否可用 */
    res2 = (ir_vol_cal[type_spe.amp_type][1].point_menu[type_spe.ir_vol_range][0][0] != NULL); /* 判断电压段2是否可用 */
    
    return (res1 + res2);
}
void cal_acw_out_vol_1(uint32_t opt)
{
    vol_segment = 0;
    serve_correct_acw_out_v(0);
}
void cal_acw_out_vol_2(uint32_t opt)
{
    vol_segment = 1;
    serve_correct_acw_out_v(1);
}
void cal_dcw_out_vol_1(uint32_t opt)
{
    vol_segment = 0;
    serve_correct_dcw_out_v(0);
}
void cal_dcw_out_vol_2(uint32_t opt)
{
    vol_segment = 1;
    serve_correct_dcw_out_v(1);
}
void cal_ir_out_vol_1(uint32_t opt)
{
    vol_segment = 0;
    serve_correct_ir_out_v(0);
}
void cal_ir_out_vol_2(uint32_t opt)
{
    vol_segment = 1;
    serve_correct_ir_out_v(1);
}
static void dis_contens_acw(void)
{
    uint8_t opt = sizeof acw_cal_opt_info / sizeof acw_cal_opt_info[0];
    int32_t i = 0;
    
    for(i = 0; i < opt; i++)
    {
        GUI_DispStringAt((const char*)acw_cal_opt_info[i].name[0], 13 + 115 * (i / 4) , 15 + 12 * (i % 4));
    }
}

static void dis_contens_bbd(void)
{
    uint8_t opt = sizeof bbd_cal_opt_info / sizeof bbd_cal_opt_info[0];
    int32_t i = 0;
    
    for(i = 0; i < opt; i++)
    {
        GUI_DispStringAt((const char*)bbd_cal_opt_info[i].name[0], 13 + 115 * (i / 4) , 15 + 12 * (i % 4));
    }
}
static void dis_contens_dcw(void)
{
    uint8_t opt = sizeof dcw_cal_opt_info / sizeof dcw_cal_opt_info[0];
    int32_t i = 0;
    
    for(i = 0; i < opt; i++)
    {
        GUI_DispStringAt((const char*)dcw_cal_opt_info[i].name[0], 13 + 115 * (i / 4) , 15 + 12 * (i % 4));
    }
}
static void dis_contens_ir(void)
{
    uint8_t opt = sizeof ir_cal_opt_info / sizeof ir_cal_opt_info[0];
    int32_t i = 0;
    
    for(i = 0; i < opt; i++)
    {
        GUI_DispStringAt((const char*)ir_cal_opt_info[i].name[0], 13 + 115 * (i / 4) , 15 + 12 * (i % 4));
    }
}
static void dis_contens_gr(void)
{
    uint8_t opt = 0;
    int32_t i = 0;
    
    GUI_ClearRect(0,15,200-1,64-1);
    if(GR_VOL_DROP_EN > 0)
    {
        opt = sizeof dv_cal_opt_info / sizeof dv_cal_opt_info[0];
        
        for(i = 0; i < opt; i++)
        {
            GUI_DispStringAt((const char*)dv_cal_opt_info[i].name[0], 13 + 115 * (i / 4) , 15 + 12 * (i % 4));
        }
    }
    else
    {
        if(DC_GR_EN == ENABLE)
        {
            opt = sizeof gr_dc_cal_opt_info / sizeof gr_dc_cal_opt_info[0];
            
            for(i = 0; i < opt; i++)
            {
                GUI_DispStringAt((const char*)gr_dc_cal_opt_info[i].name[0], 13 + 115 * (i / 4) , 15 + 12 * (i % 4));
            }
        }
        else
        {
            opt = sizeof gr_ac_cal_opt_info / sizeof gr_ac_cal_opt_info[0];
            
            for(i = 0; i < opt; i++)
            {
                GUI_DispStringAt((const char*)gr_ac_cal_opt_info[i].name[0], 13 + 115 * (i / 4) , 15 + 12 * (i % 4));
            }
        }
    }
}
void init_ratio_all(void)
{
    init_ratio(ACW);
    init_ratio(DCW);
    init_ratio(IR);
    init_ratio(GR);
}
// /*
//  * 函数名：init_ratio
//  * 描述  ：初始化校准系数
//  * 输入  ：void
//  * 输出  ：无
//  * 返回  ：无
//  */
void init_ratio(uint8_t mode)
{
	if(mode == ACW)
	/* ACW */
	{
	ratio_acw.dac_k[0] = 0.5611042 * 10;/* dac电压系数 */
	ratio_acw.dac_b[0] = -3.072428;
	ratio_acw.adc_v_k[0] = 1.78737;/* 电压系数 */
	ratio_acw.adc_v_b[0] = 6.882162;
    
	ratio_acw.dac_k[1] = 0.5611042;/* dac电压系数 */
	ratio_acw.dac_b[1] = -3.072428;
	ratio_acw.adc_v_k[1] = 1.78737;/* 电压系数 */
	ratio_acw.adc_v_b[1] = 6.882162;
	
	ratio_acw.adc_cur_k[AC_200uA] = 0.6099935;/* 200uA电流系数 */
	ratio_acw.adc_cur_k[AC_2mA]   = 1.179548;/* 2mA电流系数 */
	ratio_acw.adc_cur_k[AC_10mA]  = 0.593058;/* 10mA电流系数 */
	ratio_acw.adc_cur_k[AC_20mA]  = 0.593058;/* 20mA电流系数 */
	ratio_acw.adc_cur_k[AC_50mA]  = 0.593058;/* 50mA电流系数 */
	ratio_acw.adc_cur_k[AC_100mA] = 0.593058;/* 100mA电流系数 */
	ratio_acw.adc_cur_k[AC_200mA] = 0.593058;/* 200mA电流系数 */
	
	ratio_acw.arc_k = 1.0;/* 电弧系数 */
	ratio_acw.adc_r_k = 1.0;/* 真实电流系数 */
	ratio_acw.adc_r_b = 0.0;
	}
	/* DCW */
    else if(mode == DCW)
	{
	ratio_dcw.dac_k[0] = 0.6336942 * 10;/* dac电压系数 */
	ratio_dcw.dac_b[0] = 7.366048;
	ratio_dcw.adc_v_k[0] = 1.58793;/* 电压系数 */
	ratio_dcw.adc_v_b[0] = -12.26546;
    
	ratio_dcw.dac_k[1] = 0.6336942;/* dac电压系数 */
	ratio_dcw.dac_b[1] = 7.366048;
	ratio_dcw.adc_v_k[1] = 1.58793;/* 电压系数 */
	ratio_dcw.adc_v_b[1] = -12.26546;
    
	ratio_dcw.adc_cur_k[DC_2uA] = 1.0;
	ratio_dcw.adc_cur_k[DC_20uA] = 1.0;
	ratio_dcw.adc_cur_k[DC_200uA] = 1.0;
	ratio_dcw.adc_cur_k[DC_2mA] = 0.5377155;/* 2mA电流系数 */
	ratio_dcw.adc_cur_k[DC_10mA] = 0.5382956;/* 10mA电流系数 */
	ratio_dcw.adc_cur_k[DC_20mA] = 0.5382956;/* 20mA电流系数 */
	ratio_dcw.adc_cur_k[DC_50mA] = 0.5382956;/* 50mA电流系数 */
	ratio_dcw.adc_cur_k[DC_100mA] = 0.5382956;/* 100mA电流系数 */
	
	ratio_dcw.arc_k = 1.0;/* 电弧系数 */
	ratio_dcw.adc_a_b = 0.0;
	ratio_dcw.adc_r_k = 1.0;/* 真实电流系数 */
	ratio_dcw.adc_r_b = 0.0;
	}
	/* 	IR */
    else if(mode == IR)
    {
        int32_t i = 0;
        int32_t j = 0;
        
	ratio_ir.dac_k[0] = 0.6336942 * 10;/* dac电压系数 */
	ratio_ir.dac_b[0] = 7.366048;
	ratio_ir.adc_v_k[0] = 1.58793;/* 电压系数 */
	ratio_ir.adc_v_b[0] = -12.26546;
    
	ratio_ir.dac_k[1] = 0.6336942;/* dac电压系数 */
	ratio_ir.dac_b[1] = 7.366048;
	ratio_ir.adc_v_k[1] = 1.58793;/* 电压系数 */
	ratio_ir.adc_v_b[1] = -12.26546;
    
//    for(i = 0; i < 3; i++)
//    {
//        for(j = 0; j < SECT_VOL; j++)
//        {
//            ratio_ir.res_k[i][j] = default_ir_res_k[0];
//        }
//    }
//    
//    for(i = 3; i < 6; i++)
//    {
//        for(j = 0; j < SECT_VOL; j++)
//        {
//            ratio_ir.res_k[i][j] = default_ir_res_k[1];
//        }
//    }
//    
//    for(i = 6; i < 9; i++)
//    {
//        for(j = 0; j < SECT_VOL; j++)
//        {
//            ratio_ir.res_k[i][j] = default_ir_res_k[2];
//        }
//    }
//    
//    for(i = 9; i < 12; i++)
//    {
//        for(j = 0; j < SECT_VOL; j++)
//        {
//            ratio_ir.res_k[i][j] = default_ir_res_k[3];
//        }
//    }
//    
//    for(i = 12; i < CAL_POINTS; i++)
//    {
//        for(j = 0; j < SECT_VOL; j++)
//        {
//            ratio_ir.res_k[i][j] = default_ir_res_k[4];
//        }
//    }
    }
	/* GR DC */
    else if(mode == GR)
	{
		if(DC_GR_EN)
		{
        ratio_gr.dac_k[0]   = 0.9819072;/* dac电压系数 */
        ratio_gr.dac_b[0]   = 5.779623;
        ratio_gr.adc_v_k[0] = 1.976271;/* 电压系数 */
        ratio_gr.adc_v_b[0] = -7.708008;
        ratio_gr.adc_i_k[0] = 0.01021449;/* 电流系数 */
        ratio_gr.adc_i_b[0] = -0.1292979;
        
        ratio_gr.dac_k[1]   = 0.9819072;/* dac电压系数 */
        ratio_gr.dac_b[1]   = 5.779623;
        ratio_gr.adc_v_k[1] = 1.976271;/* 电压系数 */
        ratio_gr.adc_v_b[1] = -7.708008;
        ratio_gr.adc_i_k[1] = 0.01021449;/* 电流系数 */
        ratio_gr.adc_i_b[1] = -0.1292979;
        
        ratio_gr.dac_k[2]   = 0.9819072;/* dac电压系数 */
        ratio_gr.dac_b[2]   = 5.779623;
        ratio_gr.adc_v_k[2] = 1.976271;/* 电压系数 */
        ratio_gr.adc_v_b[2] = -7.708008;
        ratio_gr.adc_i_k[2] = 0.01021449;/* 电流系数 */
        ratio_gr.adc_i_b[2] = -0.1292979;
        
        ratio_gr.dac_k[3]   = 0.9819072;/* dac电压系数 */
        ratio_gr.dac_b[3]   = 5.779623;
        ratio_gr.adc_v_k[3] = 1.976271;/* 电压系数 */
        ratio_gr.adc_v_b[3] = -7.708008;
        ratio_gr.adc_i_k[3] = 0.01021449;/* 电流系数 */
        ratio_gr.adc_i_b[3] = -0.1292979;
        
        ratio_gr.dac_k[4]   = 1;/* dac电压系数 */
        ratio_gr.dac_b[4]   = 0;
        ratio_gr.adc_v_k[4] = 1;/* 电压系数 */
        ratio_gr.adc_v_b[4] = 0;
        ratio_gr.adc_i_k[4] = 1;/* 电流系数 */
        ratio_gr.adc_i_b[4] = 0;
        
        ratio_gr.dac_k[5]   = 3.745226;/* dac电压系数 */
        ratio_gr.dac_b[5]   = -2.949219;
        ratio_gr.adc_v_k[5] = 4.800114;/* 电压系数 */
        ratio_gr.adc_v_b[5] = 26.48014;
        ratio_gr.adc_i_k[5] = 0.02677111;/* 电流系数 */
        ratio_gr.adc_i_b[5] = -0.1443431;
        
        ratio_gr.dac_k[6]   = 0.3643337;/* dac电压系数 */
        ratio_gr.dac_b[6]   = -4.97933;
        ratio_gr.adc_v_k[6] = 4.80092;/* 电压系数 */
        ratio_gr.adc_v_b[6] = 24.46875;
        ratio_gr.adc_i_k[6] = 0.2751979;/* 电流系数 */
        ratio_gr.adc_i_b[6] = -0.8348796;
		}
		/* GR AC */
		else
		{
        ratio_gr.dac_k[0]   = 1.0;/* dac电压系数 */
        ratio_gr.dac_b[0]   = 0;
        ratio_gr.adc_v_k[0] = 1.0;/* 电压系数 */
        ratio_gr.adc_v_b[0] = 0;
        ratio_gr.adc_i_k[0] = 1.0;/* 电流系数 */
        ratio_gr.adc_i_b[0] = 0;
		}
	}
}

// /*
//  * 函数名：startup_cal
//  * 描述  ：启动校准
//  * 输入  ：mode 模式 gear 电流档位
//  * 输出  ：无
//  * 返回  ：无
//  */
void startup_cal(const int8_t mode, const int8_t gear) 
{
	/* 切换档位继电器 */
	cur_mode = mode;
	cur_gear = gear;
    clear_test_flag();
    clear_dc_gr_data();
	relay_motion();
    reset_posrts_mc14094_ctrl();
	
    if(!(mode == GR && DC_GR_EN))
    {
		amp_relay_ctrl_on();/* 功放继电器 */
	}
	MC14094_Updata();// 更新输出缓冲区
    OSTimeDlyHMSM(0,0,0,50);
    
    relay_ready();
	
	MC14094_CMD(MC14094_A, MC14094_PLC_TEST, 0);/* 在复位状态下PLC这三个继电器都不吸合 */
	MC14094_CMD(MC14094_A, MC14094_PLC_FAIL, 0);
	MC14094_CMD(MC14094_A, MC14094_PLC_PASS, 0);
	
	/* 测试模式选择 */
	if(sys_par.test_method == GND_MODE)
	{
		MC14094_CMD(MC14094_B, MC14094_GFI_RELAY, 1); /* k8 */
		MC14094_CMD(MC14094_C, MC14094_C_GND_FLOAT, 1);/* 接地模式 */
	}
	else
	{
		MC14094_CMD(MC14094_B, MC14094_GFI_RELAY, 0); /* k8 */
		MC14094_CMD(MC14094_C, MC14094_C_GND_FLOAT, 0);/* 浮地模式 */
	}
	
	if(mode == GR)
	{
		MC14094_CMD(MC14094_C, MC14094_C_GR, 1);/* GR */
	}
	else
	{
		MC14094_CMD(MC14094_C, MC14094_C_GR, 0);/* ACW DCW IR */
	}
	
	switch(mode)
	{
		case ACW:
		case GR:
		case BBD:
			open_sine(60);/* 60HZ */
			break;
		case DCW:
			if(type_spe.dcw_big_cap_en)
			{
				dcw_big_cap_cal();
			}
		case IR:
			MC14094_CMD(MC14094_B, MC14094_AC_DC, 1);
			open_sine(400);/* 400HZ */
			break;
	}
	
	MC14094_Updata();// 更新输出缓冲区
	
    set_sample_rate(50);
    
    if(mode == GR && DC_GR_EN)
    {
        off_sample_task();
        test_flag.allow_dc_gr_fetch = 1;
        test_flag.uart_next = 0;
		OSTimeDlyHMSM(0,0,1,0);
		amp_relay_ctrl_on();/* 功放继电器 */
    }
    else
    {
        on_sample_task();
        resume_sample_task();
    }
	
}

// /*
//  * 函数名：stop_cal
//  * 描述  ：停止校准
//  * 输入  ：mode 模式
//  * 输出  ：无
//  * 返回  ：无
//  */
void stop_cal(const int8_t mode)
{
	app_flag.dc_gr_module_status = DC_GR_STOPING;
	DAC_Vol[0] = 0;
	DAC_Vol[1] = SHORT_VREF_RESET;
	DAC_Vol[2] = ARC_VREF_RESET;
    disable_sample_task();
    MC14094_CMD(MC14094_C, MC14094_C_GR, 0);/* ACW DCW IR */
	irq_stop_relay_motion();/* 关闭电子开关 高压 */
    
	if(mode == GR)
	{
		/* 直流 */
		if(DC_GR_EN)
		{
			/* 停止测试 */
            send_dc_module_cmd(DC_MODULE_CMD_STOP, 0);
			return;
		}
	}
    
	key_buzzer_time = KEY_DELAY_NORMAL;
    
	LED_TEST = LED_OFF;
}

int32_t correct_exception(const uint16_t errnum, const uint8_t mode)
{
	uint8_t x = 0;
	uint8_t y = 0;
	
// 	if(STOP)
// 	{
// 		STOP=STOP;
// 		return 0;
// 	}
// 	
	
	x = 200-30;
	y = 42;
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (x, y, x+6*4, y+12);
	
	/* 显示错误的信息 */
	switch(errnum)
	{
		case 1:/* 上限报警 */
			GUI_DispStringAt((const char*)"上限", x, y);
			break;
		case 2:/* 下限报警 */
			GUI_DispStringAt((const char*)"下限", x, y);
			break;
		case 3:/* 真实电流报警 */
			GUI_DispStringAt((const char*)"真实", x, y);
			break;
		case 4:/* 充电电流报警 */
			GUI_DispStringAt((const char*)"充电", x, y);
			break;
		case 5:/* 开路报警 */
			GUI_DispStringAt((const char*)"开路", x, y);
			break;
		case 6:/* 短路报警 */
			GUI_DispStringAt((const char*)"短路", x, y);
			break;
		case 7:/* ARC报警 */
			GUI_DispStringAt((const char*)"ARC", x, y);
			break;
		case 8:/* GFI报警 */
			GUI_DispStringAt((const char*)"GFI", x, y);
			break;
		case 9:/* 功放报警 */
			GUI_DispStringAt((const char*)"功放", x, y);
			break;
		case 10:/* 挡位报警 */
			GUI_DispStringAt((const char*)"档位", x, y);
			break;
		default:
			GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
			return 0;/* 不是合法错误 */
	}
	stop_cal(mode);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	LCD_REFRESH();
	LED_FAIL = LED_ON;
	BUZZER = BUZZER_ON;
	
	while(1)
	{
		if(0 == STOP_PIN)/* 按下复位键才能退出 */
		{
			LED_FAIL = LED_OFF;
// 			STOP = 1;
            if(BUZZER_EN)
            {
                BUZZER = BUZZER_OFF;/* 不受系统开关控制 */
            }
            
			GUI_ClearRect (x, y, x+6*4, y+12);
			LCD_REFRESH();
			return -1;
		}
	}
}

static int16_t c_judge_err(const uint8_t mode)
{
#define JUDGE_SHORT() {if(SHORT_PIN == RESET){if(SHORT_PIN == RESET){if(SHORT_PIN == RESET){errnum = ERR_SHORT;}}}}
#define JUDGE_GFI() {if(GFI_PIN == RESET){if(GFI_PIN == RESET){if(GFI_PIN == RESET){errnum = ERR_GFI;}}}}
#define JUDGE_AMP() {if(AMP_PIN == RESET){if(AMP_PIN == RESET){if(AMP_PIN == RESET){errnum = ERR_AMP;}}}}
#define JUDGE_ARC() {if(ARC_PIN == RESET){if(ARC_PIN == RESET){if(ARC_PIN == RESET){errnum = ERR_ARC;}}}}
	int16_t errnum = 0;
	int8_t *perr = NULL;
	
	switch(mode)
	{
		case ACW:
			JUDGE_SHORT();
			break;
		case DCW:
			JUDGE_SHORT();
			break;
		case IR:
			JUDGE_SHORT();
			break;
		case GR:
			if(perr)
			{
				errnum = *perr;
			}
			break;
	}
	
	if(errnum)
	{
		return correct_exception(errnum, mode);
	}
	
	return 0;
}
////////////////////////////////////////////////////////////////
// 		   **             ******        ****  ****  ***       //
// 		   **            **   **         **    **    *        //
// 		   ***           **   **         **    **    *        //
// 		  ****          **                **   ***   *        //
// 		  ****          **                **  * **  *         //
// 		  ** **         **                **  * **  *         //
// 		  *****         **                 ** *  *  *         //
// 		 **  **         **                 ** *  *  *         //
// 		 **   **         **   **            **    **          //
// 		 **   **         **  **             **    **          //
// 		**** ****         ****              **    **          //
////////////////////////////////////////////////////////////////
// /*
//  * 函数名：start_correct_acw_vol
//  * 描述  ：开始校准ACW电压
//  * 输入  ：vol 模式
//  * 输出  ：无
//  * 返回  ：无
//  */
void start_correct_acw_vol(const uint16_t times)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint32_t temp_vol = 0;
	int8_t temp = 0;
	int16_t t = 0;
    float div_t = 1.0;
	
	static u16	advlue[3] = {0};
	static float buf[3] = {0.0};
	
	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 16);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	GUI_DispStringAt((const char*)"重校", 210, 50);
    
	pmenu_data->pData = &temp_vol;
	pmenu_data->ch_bit = 0;
	cursor_pos(pmenu_data->x, pmenu_data->y);
	cur_vol = 5100;//acw_vol_cal[type_spe.amp_type][vol_segment].point_da[cur_set_meal][times][1];
    
    if(pmenu_data->decs == 4)
    {
        div_t = 10.0;
    }
    
	startup_cal(ACW, AC_20mA);
	
	while(1)
	{
		DAC_Vol[0] = acw_vol_cal[type_spe.amp_type][vol_segment].point_da[cur_set_meal][times][0];
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		
		/* 按下复位键时就要返回 */
		if(0 == STOP_PIN)
		{
            buf[times] = temp_vol/div_t;
            
			advlue[times] = sample_vol;
			
            // 数据系数曲线拟合算法
			if(times == 2)
			{
				u8 i=0,n=3;
				float sumx=0.0,sumy=0.0,sumz=0.0,sumxx=0.0,sumzz=0.0,sumxy=0.0,sumxz=0.0;
				
				/* 检查输入数据是否为0 如果有就不做计算直接停止 */
				for(i = 0; i < 3; i++)
				{
					if(buf[i] == 0)
					{
						goto l;
					}
				}
				
				for(i=0;i<n;i++)
				{
					sumx 	+= buf[i];
					sumy 	+= acw_vol_cal[type_spe.amp_type][vol_segment].point_da[cur_set_meal][i][0];
					sumz	+= advlue[i];
					sumxx	+= buf[i] * buf[i];
					sumzz	+= advlue[i] * advlue[i];
					sumxy	+= buf[i] * acw_vol_cal[type_spe.amp_type][vol_segment].point_da[cur_set_meal][i][0];
					sumxz	+= buf[i] * advlue[i];
				}
                
				ratio_acw.dac_k[vol_segment]	 	= ((float)sumxy*n-sumx*sumy)/(n*sumxx-sumx*sumx);
				ratio_acw.dac_b[vol_segment]	 	= ((float)sumy-ratio_acw.dac_k[vol_segment] * sumx)/n;
				ratio_acw.adc_v_k[vol_segment]	= ((float)sumxz*n-sumx*sumz)/(n*sumzz-sumz*sumz);
				ratio_acw.adc_v_b[vol_segment]	= ((float)sumx-ratio_acw.adc_v_k[vol_segment] * sumz)/n;
				
				save_ratio(ACW);
			}
		l:
			stop_cal(ACW);
			temp_vol = 0;
			pmenu_data->ch_bit = 0;
			update_data(pmenu_data);
			cursor_pos(-5, 0);/* 将光标移出屏幕 */
			return;
		}
		
// 		CALIB_JUDGE_ERR(ACW);
		
		serve_numkey(*pkey_value);
        
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_vol/t;
				temp = t%10;
                
				if(++temp > 9)
				{
					temp = 0;
				}
                
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_DOWN:
				if(pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
                
				t = temp_vol/t;
				temp = t%10;
                
				if(--temp < 0)
				{
					temp = 9;
				}
                
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_F4:/* 重校 */
				temp_vol = 0;
				pmenu_data->ch_bit = 0;
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x, pmenu_data->y);
				break;
			default:
				break;
		}
	}
}

void correct_acw_out_v(const uint16_t times)
{
	uint8_t X0	= 40;
	uint8_t X1	= 160;
	uint8_t Y0	= 15;
	uint8_t Y1	= 63;
    
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t m = 1;
	uint8_t n = 1;//pf->work_mode;
    
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_FillRect(X0+2, Y0+2, (X0+X1)/2-2, Y0+15);
	GUI_FillRect((X0+X1)/2+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"校准电压", X0+6, Y0+3);
	GUI_DispStringAt((const char*)"实际电压", (X0+X1)/2+6, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
    
	pmenu_data = &par_info;	/* 将pmenu_data指向原始密码的信息 */
    
	switch(cur_set_meal)
	{
        default:
        case ACW_500V:
		case ACW_1kV:
		case ACW_5kV:
			pmenu_data->decs = 3;
            pmenu_data->x = (X0+X1)/2+10-3;
            pmenu_data->y = Y1-22;
            /*********************************/
            pmenu_data->decs = 4;
            pmenu_data->lon = 6;
            pmenu_data->bits = 2;
            /********************************/
            break;
		case ACW_10kV:
            pmenu_data->lon = 5;
            pmenu_data->x = (X0+X1)/2+10;
            pmenu_data->y = Y1-22;
			pmenu_data->decs = 3;
			break;
		case ACW_20kV:
		case ACW_30kV:
            pmenu_data->lon = 6;
            pmenu_data->x = (X0+X1)/2+10;
            pmenu_data->y = Y1-22;
			pmenu_data->decs = 3;
			break;
	}
    
	GUI_DispStringAt((const char*)acw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][times][1], X0+10, Y1-22);
	switch(cur_set_meal)
	{
        default:
        case ACW_500V:
		case ACW_1kV:
		case ACW_5kV:
		case ACW_10kV:
            if(pmenu_data->lon == 6)
            {
                GUI_DispStringAt((const char*)"0.0000kV", pmenu_data->x, pmenu_data->y);
            }
            else
            {
                GUI_DispStringAt((const char*)"0.000kV", pmenu_data->x, pmenu_data->y);
            }
			break;
		case ACW_20kV:
		case ACW_30kV:
			GUI_DispStringAt((const char*)"00.000kV", pmenu_data->x, pmenu_data->y);
			break;
	}
	
	while(1)
	{
		if(m)
		{
			m = 0;
			clear_menu();
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
			ReFresh_LCD24064();
		}
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);
        
		switch(*pkey_value)
		{
			case KEY_SHIFT:
				if(++n > 1)
				{
					n = 0;
				}
				break;
			case KEY_BACK:
				GUI_ClearRect (X0, Y0, X1, Y1);
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			case KEY_START:
				start_correct_acw_vol(times);
				m = 1;
				break;
			default:
				break;
		}
	}
}
void serve_correct_acw_out_v(uint32_t opt)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t m = 1;
	
    if(NULL == acw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][0][0])
    {
        return;
    }
    
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_acw();
			
			clear_menu();
			GUI_DispStringAt((const char*)acw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][0][0], 208, 2);
			GUI_DispStringAt((const char*)acw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][1][0], 208, 18);
			GUI_DispStringAt((const char*)acw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][2][0], 208, 34);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
// 			GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+2+12*0);
			ReFresh_LCD24064();
		}
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
                if(NULL != acw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][0][0])
                    correct_acw_out_v(0);
				m = 1;/* 重绘背景 */
				break;
			case KEY_F2:
                if(NULL != acw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][1][0])
                    correct_acw_out_v(1);
				m = 1;/* 重绘背景 */
				break;
			case KEY_F3:
                if(NULL != acw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][2][0])
                    correct_acw_out_v(2);
				m = 1;/* 重绘背景 */
				break;
			case KEY_BACK:
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}

uint16_t set_out_vol(void)
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_vol = 500;/* 默认500v */
	int8_t temp = 0;
	int16_t t = 0;
	
	DATA_INFO vol_info =
	{
		(void*)0/*pData*/,X0+10/*x*/,Y1-22/*y*/,1/*type*/,5/*lon*/,3/*decs*/,0/*total*/,1/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
    
	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 16);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
// 	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_DispStringAt((const char*)"0.500kV", X0 + 10, Y1-22);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, (X0+X1)/2 + 25, Y1-20);
	GUI_DispStringAt((const char*)"0.050kV", (X0+X1)/2 + 8, Y1-30);
	GUI_DispStringAt((const char*)"5.000kV", (X0+X1)/2 + 8, Y1-12);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"设置校准输出电压", X0+15, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	pmenu_data = &vol_info;	/* 将pmenu_data指向原始密码的信息 */
	pmenu_data->pData = &temp_vol;
	pmenu_data->ch_bit = 0;
	cursor_pos(pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
// 		if(0 == STOP_PIN)
// 		{/* 按下复位键时就要返回 */
// 			cursor_pos(0, -1);/* 将光标移出屏幕 */
// 			return;
// 		}
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_vol/t;
				temp = t%10;

				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_DOWN:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_vol/t;
				temp = t%10;

				if(--temp < 0)
				{
					temp = 9;
				}

				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_BACK:
				GUI_ClearRect(X0, Y0, X1, Y1);
				g_return = 1;
				return 0;
			case KEY_EXIT:
				GUI_ClearRect(X0, Y0, X1, Y1);
				g_exit = 1;
				return 0;
			case KEY_START:
				return temp_vol;
			default:
				break;
			
		}
	}
}


//////////////////////////////////////

__inline int16_t dis_cur(const int8_t mode, const int8_t gear)
{
	uint8_t X0	= 40;
	uint8_t Y0	= 15;
	
	static int8_t n = 0;
	uint16_t temp_cur = 0;
	
	temp_cur = sample_cur;/* 采集的电流值 */
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_ClearRect(X0+10, Y0+26, X0+10+6*5-1,Y0+26+12);/* 见鬼了 非得减个1才行 */
	switch(mode)
	{
		case BBD:
		case ACW:
			switch(gear)
			{
				case AC_2uA:
				{
					if(ratio_acw.adc_cur_k[AC_2uA] <= 0)
					{
						ratio_acw.adc_cur_k[AC_2uA] = 1.0;
					}
					cur_ave = temp_cur*ratio_acw.adc_cur_k[AC_2uA];
					GUI_GotoXY(X0+10, Y0+26);
					GUI_DispFloatFix ((float)cur_ave/1000, 5, 3);
					break;
				}
				case AC_20uA:
				{
					if(ratio_acw.adc_cur_k[AC_20uA] <= 0)
					{
						ratio_acw.adc_cur_k[AC_20uA] = 1.0;
					}
					cur_ave = temp_cur*ratio_acw.adc_cur_k[AC_20uA];
					if(cur_ave < 1000)
					{
						GUI_GotoXY(X0+10+6*1, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/100, 4, 2);
					}
					else
					{
						GUI_GotoXY(X0+10, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/100, 5, 2);
					}
					break;
				}
				case AC_200uA:
				{
					if(ratio_acw.adc_cur_k[AC_200uA] <= 0)
					{
						ratio_acw.adc_cur_k[AC_200uA] = 1.0;
					}
					cur_ave = temp_cur*ratio_acw.adc_cur_k[AC_200uA];
					if(cur_ave < 100)
					{
						GUI_GotoXY(X0+10+6*2, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/10, 3, 1);
					}
					else if(cur_ave < 1000)
					{
						GUI_GotoXY(X0+10+6*1, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/10, 4, 1);
					}
					else
					{
						GUI_GotoXY(X0+10, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/10, 5, 1);
					}
					break;
				}
				case AC_2mA:
				{
					if(ratio_acw.adc_cur_k[AC_2mA] <= 0)
					{
						ratio_acw.adc_cur_k[AC_2mA] = 1.0;
					}
					cur_ave = temp_cur*ratio_acw.adc_cur_k[AC_2mA];
					GUI_GotoXY(X0+10, Y0+26);
					GUI_DispFloatFix ((float)cur_ave/1000, 5, 3);
					break;
				}
				case AC_10mA:
				{
					if(ratio_acw.adc_cur_k[AC_10mA] <= 0)
					{
						ratio_acw.adc_cur_k[AC_10mA] = 1.0;
					}
					cur_ave = temp_cur*ratio_acw.adc_cur_k[AC_10mA];
					if(cur_ave < 1000)
					{
						GUI_GotoXY(X0+10+6*1, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/100, 4, 2);
					}
					else
					{
						GUI_GotoXY(X0+10, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/100, 5, 2);
					}
					break;
				}
				case AC_20mA:
				{
					if(ratio_acw.adc_cur_k[AC_20mA] <= 0)
					{
						ratio_acw.adc_cur_k[AC_20mA] = 1.0;
					}
					cur_ave = temp_cur*ratio_acw.adc_cur_k[AC_20mA];
					if(cur_ave < 1000)
					{
						GUI_GotoXY(X0+10+6*1, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/100, 4, 2);
					}
					else
					{
						GUI_GotoXY(X0+10, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/100, 5, 2);
					}
					break;
				}
				case AC_50mA:
				{
					if(ratio_acw.adc_cur_k[AC_50mA] <= 0)
					{
						ratio_acw.adc_cur_k[AC_50mA] = 1.0;
					}
					cur_ave = temp_cur*ratio_acw.adc_cur_k[AC_50mA];
					if(cur_ave < 1000)
					{
						GUI_GotoXY(X0+10+6*1, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/100, 4, 2);
					}
					else
					{
						GUI_GotoXY(X0+10, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/100, 5, 2);
					}
					break;
				}
				case AC_100mA:
				{
					if(ratio_acw.adc_cur_k[AC_100mA] <= 0)
					{
						ratio_acw.adc_cur_k[AC_100mA] = 1.0;
					}
					cur_ave = temp_cur*ratio_acw.adc_cur_k[AC_100mA];
					if(cur_ave < 100)
					{
						GUI_GotoXY(X0+10+6*2, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/10, 3, 1);
					}
					else if(cur_ave < 1000)
					{
						GUI_GotoXY(X0+10+6*1, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/10, 4, 1);
					}
					else
					{
						GUI_GotoXY(X0+10, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/10, 5, 1);
					}
					break;
				}
				case AC_200mA:
				{
					if(ratio_acw.adc_cur_k[AC_200mA] <= 0)
					{
						ratio_acw.adc_cur_k[AC_200mA] = 1.0;
					}
					cur_ave = temp_cur*ratio_acw.adc_cur_k[AC_200mA];
					if(cur_ave < 100)
					{
						GUI_GotoXY(X0+10+6*2, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/10, 3, 1);
					}
					else if(cur_ave < 1000)
					{
						GUI_GotoXY(X0+10+6*1, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/10, 4, 1);
					}
					else
					{
						GUI_GotoXY(X0+10, Y0+26);
						GUI_DispFloatFix ((float)cur_ave/10, 5, 1);
					}
					break;
				}
				default:
					break;
			}
			break;
		case DCW:
			if(ratio_dcw.adc_cur_k[gear] <= 0)
			{
				ratio_dcw.adc_cur_k[gear] = 1.0;
			}
			cur_ave = temp_cur*ratio_dcw.adc_cur_k[gear];
			
			switch(gear)
			{
				case DC_2uA:
					GUI_GotoXY(50, 41);
					GUI_DispFloatFix ((float)cur_ave/1000, 5, 3);
					break;
				case DC_20uA:
					if(cur_ave < 1000)
					{
						GUI_GotoXY(50+6*1, 41);
						GUI_DispFloatFix ((float)cur_ave/100, 4, 2);
					}
					else
					{
						GUI_GotoXY(50, 41);
						GUI_DispFloatFix ((float)cur_ave/100, 5, 2);
					}
					break;
				case DC_200uA:
					if(cur_ave < 100)
					{
						GUI_GotoXY(50+6*2, 41);
						GUI_DispFloatFix ((float)cur_ave/10, 3, 1);
					}
					else if(cur_ave < 1000)
					{
						GUI_GotoXY(50+6*1, 41);
						GUI_DispFloatFix ((float)cur_ave/10, 4, 1);
					}
					else
					{
						GUI_GotoXY(50, 41);
						GUI_DispFloatFix ((float)cur_ave/10, 5, 1);
					}
					break;
				case DC_2mA:
					GUI_GotoXY(50, 41);
					GUI_DispFloatFix ((float)cur_ave/1000, 5, 3);
					break;
				case DC_10mA:
					if(cur_ave < 1000)
					{
						GUI_GotoXY(50+6*1, 41);
						GUI_DispFloatFix ((float)cur_ave/100, 4, 2);
					}
					else
					{
						GUI_GotoXY(50, 41);
						GUI_DispFloatFix ((float)cur_ave/100, 5, 2);
					}
					break;
				case DC_20mA:
					if(cur_ave < 1000)
					{
						GUI_GotoXY(50+6*1, 41);
						GUI_DispFloatFix ((float)cur_ave/100, 4, 2);
					}
					else
					{
						GUI_GotoXY(50, 41);
						GUI_DispFloatFix ((float)cur_ave/100, 5, 2);
					}
					break;
				case DC_50mA:
					if(cur_ave < 1000)
					{
						GUI_GotoXY(50+6*1, 41);
						GUI_DispFloatFix ((float)cur_ave/100, 4, 2);
					}
					else
					{
						GUI_GotoXY(50, 41);
						GUI_DispFloatFix ((float)cur_ave/100, 5, 2);
					}
					break;
				case DC_100mA:
					if(cur_ave < 100)
					{
						GUI_GotoXY(50+6*2, 41);
						GUI_DispFloatFix ((float)cur_ave/10, 3, 1);
					}
					else if(cur_ave < 1000)
					{
						GUI_GotoXY(50+6*1, 41);
						GUI_DispFloatFix ((float)cur_ave/10, 4, 1);
					}
					else
					{
						GUI_GotoXY(50, 41);
						GUI_DispFloatFix ((float)cur_ave/10, 5, 1);
					}
					break;
			}
			break;
	}/* end switch(mode) */
	
// 		GUI_GotoXY(50, 41);
// 		GUI_DispFloatFix ((float)cur_ave/1000, 5, 3);
	if(++n > 5)
	{
		n = 0;
		ReFresh_LCD24064();
	}
	
	return temp_cur;
}
void start_correct_acw_cur(const int16_t vol, const int8_t gear)
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_cur = 0;
	int8_t temp = 0;
	int16_t t = 0;
	
	static u16	advlue[CUR_KINDS_MAX] = {0};
	static float buf[CUR_KINDS_MAX] = {0.0};
	
	DATA_INFO cur_info =
	{
		(void*)0/*pData*/,(X0+X1)/2+10/*x*/,Y1-22/*y*/,1/*type*/,5/*lon*/,3/*decs*/,0/*total*/,1/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
	
	switch(gear)
	{
		case AC_2uA:
			cur_info.decs = 3;
			break;
		case AC_20uA:
			cur_info.decs = 2;
			break;
		case AC_200uA:
			cur_info.decs = 1;
			break;
		case AC_2mA:
			cur_info.decs = 3;
			break;
		case AC_10mA:
			cur_info.decs = 2;
			break;
		case AC_20mA:
			cur_info.decs = 2;
			break;
		case AC_50mA:
			cur_info.decs = 2;
			break;
		case AC_100mA:
			cur_info.decs = 1;
			break;
		case AC_200mA:
			cur_info.decs = 1;
			break;
		default:
			break;
	}
	
	pmenu_data = &cur_info;	/* 将pmenu_data指向原始密码的信息 */
	pmenu_data->pData = &temp_cur;
	pmenu_data->ch_bit = 0;
	cursor_pos(pmenu_data->x, pmenu_data->y);
	goto ll;
    
l:	
	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	ReFresh_LCD24064();
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		
		switch(*pkey_value)
		{
			case KEY_START:
				goto ll;
			case KEY_BACK:
			cursor_pos(-5, 0);/* 将光标移出屏幕 */
			g_return = 1;
			GUI_ClearRect(X0, Y0, X1, Y1);
				return;
			default:
				break;
		}
	}
    
ll:	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 16);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	GUI_DispStringAt((const char*)"重校", 210, 50);
	ReFresh_LCD24064();
	
    
    confirm_vol_segment();/* 确定电压段 */
    
	startup_cal(ACW, gear);
	
	while(1)
	{
		DAC_Vol[0] = vol * ratio_acw.dac_k[vol_segment] + ratio_acw.dac_b[vol_segment];
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		
		advlue[gear] = dis_cur(ACW, gear);/* 显示AD采样电流 */
		
		/* 按下复位键时就要返回 */
		if(0 == STOP_PIN)
		{
			save_ratio(ACW);
			
			DAC_Vol[0] = 0;
			
			stop_cal(ACW);
// 			cursor_pos(-5, 0);/* 将光标移出屏幕 */
// 			g_return = 1;
// 			GUI_ClearRect(X0, Y0, X1, Y1);
			goto l;
		}
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_cur/t;
				temp = t%10;
				
				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_DOWN:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_cur/t;
				temp = t%10;

				if(--temp < 0)
				{
					temp = 9;
				}
				
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_F4:/* 重校 */
				ratio_acw.adc_cur_k[gear] = 1.0;
				temp_cur = 0;
				pmenu_data->ch_bit = 0;
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x, pmenu_data->y);
				break;
			case KEY_ENTER:
				if(temp_cur == 0)
				{
					break;
				}
				buf[gear] = (float)temp_cur;
				ratio_acw.adc_cur_k[gear] = ((float)buf[gear])/advlue[gear];
				break;
			case KEY_START:
				break;
			default:
				break;
		}
	}
}
void correct_acw_measure_uA(const int16_t vol)
{
	uint8_t X0	= 40;
	uint8_t X1	= 160;
	uint8_t Y0	= 15;
	uint8_t Y1	= 63;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t m = 1;
// 	int16_t vol;

	uint8_t n = 1;//pf->work_mode;
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_FillRect(X0+2, Y0+2, (X0+X1)/2-2, Y0+15);
	GUI_FillRect((X0+X1)/2+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"测量电流", X0+6, Y0+3);
	GUI_DispStringAt((const char*)"实际电流", (X0+X1)/2+6, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	GUI_DispStringAt((const char*)"000.0~A", X0+10, Y1-22);
	GUI_DispStringAt((const char*)"000.0~A", (X0+X1)/2+10, Y1-22);
	GUI_GotoXY(X0+10, Y1-22);
// 	GUI_DispFloatFix ((float)vol/1000, 5, 3);
	start_correct_acw_cur(vol, AC_200uA);/* 1挡是200uA挡 */
	while(1)
	{
		if(m)
		{
			m = 0;
			clear_menu();
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
			ReFresh_LCD24064();
		}
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);
        
		switch(*pkey_value)
		{
			case KEY_SHIFT:
				n = !n;
				break;
			case KEY_ENTER:
				return;
			case KEY_BACK:
				GUI_ClearRect (X0, Y0, X1, Y1);
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			case KEY_START:
				start_correct_acw_cur(vol, AC_200uA);
				m = 1;
				break;
			default:
				break;
		}
		if(g_exit || g_return)
		{
			return;
		}
	}
}
void correct_acw_cur_gear(const uint8_t gear, const uint16_t vol)
{
	uint8_t X0	= 40;
	uint8_t X1	= 160;
	uint8_t Y0	= 15;
	uint8_t Y1	= 63;
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_FillRect(X0+2, Y0+2, (X0+X1)/2-2, Y0+15);
	GUI_FillRect((X0+X1)/2+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"测量电流", X0+6, Y0+3);
	GUI_DispStringAt((const char*)"实际电流", (X0+X1)/2+6, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	switch(gear)
	{
		case AC_2uA:
			GUI_DispStringAt((const char*)"0.000~A", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"0.000~A", (X0+X1)/2+10, Y1-22);
			break;
		case AC_20uA:
			GUI_DispStringAt((const char*)"00.00~A", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"00.00~A", (X0+X1)/2+10, Y1-22);
			break;
		case AC_200uA:
			GUI_DispStringAt((const char*)"000.0~A", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"000.0~A", (X0+X1)/2+10, Y1-22);
			break;
		case AC_2mA:
			GUI_DispStringAt((const char*)"0.000mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"0.000mA", (X0+X1)/2+10, Y1-22);
			break;
		case AC_10mA:
			GUI_DispStringAt((const char*)"00.00mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"00.00mA", (X0+X1)/2+10, Y1-22);
			break;
		case AC_20mA:
			GUI_DispStringAt((const char*)"00.00mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"00.00mA", (X0+X1)/2+10, Y1-22);
			break;
		case AC_50mA:
			GUI_DispStringAt((const char*)"00.00mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"00.00mA", (X0+X1)/2+10, Y1-22);
			break;
		case AC_100mA:
			GUI_DispStringAt((const char*)"000.0mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"000.0mA", (X0+X1)/2+10, Y1-22);
			break;
		case AC_200mA:
			GUI_DispStringAt((const char*)"000.0mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"000.0mA", (X0+X1)/2+10, Y1-22);
			break;
	}
	
	GUI_GotoXY(X0+10, Y1-22);
	
	start_correct_acw_cur(vol, gear);
}
void serve_correct_acw_cur_uA(uint32_t opt)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t m = 1;
	uint16_t temp_vol = 0;
	
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_acw();
			
			clear_menu();
			GUI_DispStringAt((const char*)"200~A", 208, 2);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
			ReFresh_LCD24064();
		}
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					correct_acw_cur_gear(AC_200uA, temp_vol);
				}
				
				m = 1;/* 重绘背景 */
				break;
			case KEY_F2:
// 				temp_vol = set_out_vol();
// 				if(g_exit)
// 				{
// 					return;
// 				}
// 				
// 				if(!g_return)
// 				{
// 					correct_acw_cur_gear(AC_20uA, temp_vol);
// 				}
// 				
				m = 1;/* 重绘背景 */
				break;
			case KEY_F3:
// 				temp_vol = set_out_vol();
// 				if(g_exit)
// 				{
// 					return;
// 				}
// 				
// 				if(!g_return)
// 				{
// 					correct_acw_cur_gear(AC_200uA, temp_vol);
// 				}
// 				
// 				m = 1;/* 重绘背景 */
				break;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}



void cal_acw_more_more_cur(const uint8_t **gear_buf, uint8_t cur_kinds, uint8_t *flag)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_vol;
	
	while(1)
	{
		dis_contens_acw();
		clear_menu();
		GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
		
		switch(cur_kinds)
		{
			case 6:
				GUI_DispStringAt((const char*)gear_buf[4], 208, 2);
				GUI_DispStringAt((const char*)gear_buf[5], 208, 18);
				break;
			case 7:
				GUI_DispStringAt((const char*)gear_buf[4], 208, 2);
				GUI_DispStringAt((const char*)gear_buf[5], 208, 18);
				GUI_DispStringAt((const char*)gear_buf[6], 208, 34);
				break;
		}
		ReFresh_LCD24064();
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
				if(cur_kinds<6)
				{
					break;
				}
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					correct_acw_cur_gear(flag[4], temp_vol);
				}
				break;
			case KEY_F2:
				if(cur_kinds<6)
				{
					break;
				}
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					correct_acw_cur_gear(flag[5], temp_vol);
				}
				break;
			case KEY_F3:
				if(cur_kinds<6)
				{
					break;
				}
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					correct_acw_cur_gear(flag[6], temp_vol);
				}
				break;
			case KEY_BACK:
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}
void cal_acw_more_cur(const uint8_t **gear_buf, uint8_t cur_kinds, uint8_t *flag)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_vol;
	
	while(1)
	{
		dis_contens_acw();
		clear_menu();
		GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
		if(cur_kinds > 5)
		{
			GUI_DispStringAt((const char*)gear_buf[2], 208, 2);
			GUI_DispStringAt((const char*)gear_buf[3], 208, 18);
			GUI_DispStringAt((const char*)"更多", 208, 34);
		}
		else
		{
			switch(cur_kinds)
			{
				case 4:
					GUI_DispStringAt((const char*)gear_buf[2], 208, 2);
					GUI_DispStringAt((const char*)gear_buf[3], 208, 18);
					break;
				case 5:
					GUI_DispStringAt((const char*)gear_buf[2], 208, 2);
					GUI_DispStringAt((const char*)gear_buf[3], 208, 18);
					GUI_DispStringAt((const char*)gear_buf[4], 208, 34);
					break;
			}
		}
		ReFresh_LCD24064();
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					if(cur_kinds>=4)
						correct_acw_cur_gear(flag[2], temp_vol);
				}
				break;
			case KEY_F2:
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					if(cur_kinds>=4)
						correct_acw_cur_gear(flag[3], temp_vol);
				}
				break;
			case KEY_F3:
				if(cur_kinds>5)
				{
					cal_acw_more_more_cur(gear_buf, cur_kinds, flag);
				}
				else
				{
					temp_vol = set_out_vol();
					if(g_exit)
					{
						return;
					}
					
					if(!g_return)
					{
						if(cur_kinds>=4)
							correct_acw_cur_gear(flag[4], temp_vol);
					}
				}
				break;
			case KEY_BACK:
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}
void serve_correct_acw_cur_mA(uint32_t opt)/* 测量电流 */
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t m = 1;
	uint16_t temp_vol = 0;
	
	uint32_t cur_kinds = 0;/* 电流的种类 */
	const uint8_t *gear_buf[CUR_KINDS_MAX] = {0};/* 电流档位 */
	uint8_t flag[CUR_KINDS_MAX] = {0};/* 标志 */
	int32_t i = 0;
	
	cur_kinds = define_cur_kinds(ACW, gear_buf, flag);
	if(strcmp((const char*)(const char*)gear_buf[0], "200~A") == 0)
	{
		cur_kinds -= 1;
		for(i = 0; i < cur_kinds; i++)
		{
			gear_buf[i] = gear_buf[i+1];
			flag[i] = flag[i+1];
		}
        gear_buf[i] = "";
        flag[i] = 0;
	}
	
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_acw();
			
			clear_menu();
			
			if(cur_kinds <= 3)
			{
				switch(cur_kinds)
				{
					case 1:
						GUI_DispStringAt((const char*)gear_buf[0], 208, 2);
						break;
					case 2:
						GUI_DispStringAt((const char*)gear_buf[0], 208, 2);
						GUI_DispStringAt((const char*)gear_buf[1], 208, 18);
						break;
					case 3:
						GUI_DispStringAt((const char*)gear_buf[0], 208, 2);
						GUI_DispStringAt((const char*)gear_buf[1], 208, 18);
						GUI_DispStringAt((const char*)gear_buf[2], 208, 34);
						break;
				}
			}
			else
			{
				GUI_DispStringAt((const char*)gear_buf[0], 208, 2);
				GUI_DispStringAt((const char*)gear_buf[1], 208, 18);
				GUI_DispStringAt((const char*)"更多", 208, 34);
			}
            
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
			ReFresh_LCD24064();
		}
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					if(cur_kinds>=1)
						correct_acw_cur_gear(flag[0], temp_vol);
				}
				
				m = 1;/* 重绘背景 */
				break;
			case KEY_F2:
				if(cur_kinds <= 1)
				{
                    break;
                }
                
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					if(cur_kinds>=2)
						correct_acw_cur_gear(flag[1], temp_vol);
				}
				
				m = 1;/* 重绘背景 */
				break;
			case KEY_F3:
				if(cur_kinds <= 2)
				{
                    break;
                }
                
				if(cur_kinds <= 3)
				{
					temp_vol = set_out_vol();
					if(g_exit)
					{
						return;
					}
					
					if(!g_return)
					{
						if(cur_kinds>=3)
							correct_acw_cur_gear(flag[2], temp_vol);
					}
					
					m = 1;/* 重绘背景 */
				}
				/* 更多 */
				else
				{
					cal_acw_more_cur(gear_buf, cur_kinds, flag);
				}
				m = 1;/* 重绘背景 */
				break;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			default:
				break;
		}
		if(g_return)
		{
			g_return = 0;
			m = 1;
		}
		if(g_exit)
		{
			return;
		}
	}
}
/////////////////////////////////////
// /*
//  * 函数名：correct_acw_arc_g
//  * 描述  ：电弧侦测等级模式
//  * 输入  ：无
//  * 输出  ：无
//  * 返回  ：无
//  */
void correct_acw_arc_g(void)
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_arc = ratio_acw.arc_k * 4096;
	int16_t temp = 0;
	int16_t t = 0;
	
	DATA_INFO arc_info =
	{
		(void*)0/*pData*/,X0+15/*x*/,Y1-22/*y*/,0/*type*/,4/*lon*/,0/*decs*/,0/*total*/,0/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 16);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
// 	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_DispStringAt((const char*)"0000", X0 + 15, Y1-22);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, (X0+X1)/2 + 25, Y1-20);
// 	GUI_DispStringAt((const char*)"   0", (X0+X1)/2 + 8, Y1-30);
	GUI_DispStringAt((const char*)"(0-4096)", (X0+X1)/2 + 5, Y1-22);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"电弧侦测校准", X0+25, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	pmenu_data = &arc_info;	/* 将pmenu_data指向原始密码的信息 */
	pmenu_data->pData = &temp_arc;
	pmenu_data->ch_bit = 0;
	GUI_DispDecAt (*(uint32_t*)pmenu_data->pData, pmenu_data->x, pmenu_data->y, pmenu_data->lon);
	cursor_pos(pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		if(0 == STOP_PIN)
		{/* 按下复位键时就要返回 */
			cursor_pos(0, -1);/* 将光标移出屏幕 */
			GUI_ClearRect(X0, Y0, X1, Y1);
			return;
		}
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				t = temp_arc/t;
				temp = t%10;
				
				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_DOWN:
				t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				t = temp_arc/t;
				temp = t%10;
				
				if(--temp < 0)
				{
					temp = 9;
				}

				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_ENTER:
				if(temp_arc > 4095)
				{
					temp_arc = 4095;
				}
				
				ratio_acw.arc_k = (float)temp_arc/4096;
				save_ratio(ACW);
				GUI_ClearRect(X0, Y0, X1, Y1);
				return;
			case KEY_BACK:
				GUI_ClearRect(X0, Y0, X1, Y1);
				g_return = 1;
				return;
			case KEY_EXIT:
				GUI_ClearRect(X0, Y0, X1, Y1);
				g_exit = 1;
				return;
			default:
				break;
		}
		
		if(g_return || g_exit)
		{
			return;
		}
	}
}
// /*
//  * 函数名：correct_acw_arc_c
//  * 描述  ：电弧侦测电流模式
//  * 输入  ：无
//  * 输出  ：无
//  * 返回  ：无
//  */
void correct_acw_arc_c(void)
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_cur = ratio_acw.arc_k * 2000;
	int8_t temp = 0;
	int16_t t = 0;
	
	DATA_INFO cur_info =
	{
		(void*)0/*pData*/,X0+10/*x*/,Y1-22/*y*/,1/*type*/,5/*lon*/,2/*decs*/,0/*total*/,0/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 16);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
// 	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_DispStringAt((const char*)"05.00mA", X0 + 10, Y1-22);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, (X0+X1)/2 + 25, Y1-20);
	GUI_DispStringAt((const char*)"00.10mA", (X0+X1)/2 + 8, Y1-30);
	GUI_DispStringAt((const char*)"20.00mA", (X0+X1)/2 + 8, Y1-12);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"设置电弧侦测电流", X0+15, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	pmenu_data = &cur_info;	/* 将pmenu_data指向原始密码的信息 */
	pmenu_data->pData = &temp_cur;
	pmenu_data->ch_bit = 0;
	GUI_GotoXY(pmenu_data->x, pmenu_data->y);
	GUI_DispFloatFix ((float)temp_cur/100, 5, 2);
	cursor_pos(pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		if(0 == STOP_PIN)
		{/* 按下复位键时就要返回 */
			cursor_pos(0, -1);/* 将光标移出屏幕 */
			return;
		}
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_cur/t;
				temp = t%10;
				
				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_DOWN:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_cur/t;
				temp = t%10;
				
				if(--temp < 0)
				{
					temp = 9;
				}
				
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_ENTER:
				if(temp_cur < 10)
				{
					temp_cur = 10;
				}
				if(temp_cur > 2000)
				{
					temp_cur = 2000;
				}
				
				ratio_acw.arc_k = (float)temp_cur/2000;
				save_ratio(ACW);
				GUI_ClearRect(X0, Y0, X1, Y1);
				return;
			case KEY_BACK:
				GUI_ClearRect(X0, Y0, X1, Y1);
				g_return = 1;
				return;
			case KEY_EXIT:
				GUI_ClearRect(X0, Y0, X1, Y1);
				g_exit = 1;
				return;
			default:
				break;
		}
		if(g_return || g_exit)
		{
			return;
		}
	}
}
void serve_correct_acw_arc(uint32_t opt)/* 电弧侦测 */
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t m = 1;
    int8_t n = opt;
	
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_acw();
			
			clear_menu();
			GUI_DispStringAt((const char*)"等级", 208, 2);
			GUI_DispStringAt((const char*)"电流", 208, 18);
// 			GUI_DispStringAt((const char*)"5.0kV", 208, 34);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
            GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0+115 * (n / 4), 15 + 2 + 12 * ( n % 4));
			ReFresh_LCD24064();
		}
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
				correct_acw_arc_g();/* 等级模式 */
				m = 1;/* 重绘背景 */
				break;
			case KEY_F2:
				correct_acw_arc_c();/* 电流模式 */
				if(!g_return)
				{
					correct_acw_arc_g();
				}
				m = 1;/* 重绘背景 */
				break;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}


////////////////////////////////////
void start_correct_acw_vol_real_cur(int8_t n)/* n :1-200uA,2-2mA,3-20mA */
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
// 	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_cur = 0;
	int8_t temp = 0;
	int16_t t = 0;
	
	DATA_INFO cur_info =
	{
		(void*)0/*pData*/,(X0+X1)/2+10/*x*/,Y1-22/*y*/,1/*type*/,5/*lon*/,1/*decs*/,0/*total*/,0/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
	switch(n)
	{
		case 1:
			cur_info.decs = 1;
			break;
		case 2:
			cur_info.decs = 3;
			break;
		case 3:
			cur_info.decs = 2;
			break;
	}
    
	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 16);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	GUI_DispStringAt((const char*)"重校", 210, 50);
	ReFresh_LCD24064();
	
	pmenu_data = &cur_info;	/* 将pmenu_data指向原始密码的信息 */
	pmenu_data->pData = &temp_cur;
	pmenu_data->ch_bit = 0;
	cursor_pos(pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		if(0 == STOP_PIN)
		{/* 按下复位键时就要返回 */
			cursor_pos(-5, 0);/* 将光标移出屏幕 */
			return;
		}
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_cur/t;
				temp = t%10;

				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_DOWN:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_cur/t;
				temp = t%10;
				
				if(--temp < 0)
				{
					temp = 9;
				}
				
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_F4:
				temp_cur = 0;
				pmenu_data->ch_bit = 0;
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x, pmenu_data->y);
				break;
			default:
				break;
			
		}
	}
}
////////////////////////////////////

void correct_acw(int8_t *pm)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t n = 0;
	int8_t m = 1;
	int8_t opt = sizeof acw_cal_opt_info / sizeof acw_cal_opt_info[0];/* 项目数 */
	
	cur_set_meal  = get_cal_acw_vol_meal();
	
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_acw();
		}
		
		GUI_ClearRect(0,15,12,64-1);
		GUI_ClearRect(13+100,15,13+113,64-1);
		
		GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0+115 * (n / 4), 15 + 2 + 12 * ( n % 4));
		
		ReFresh_LCD24064();
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_UP:
				if(--n < 0)
				{
					n = opt - 1;
				}
				break;
			case KEY_DOWN:
				if(++n > opt - 1)
				{
					n = 0;
				}
				break;
			case KEY_SHIFT:
                if(n+4 <= opt - 1)
                {
                    n += 4;
                }
                else if(n >= 4)
                {
                    n -= 4;
                }
				break;
			case KEY_ENTER:
                if(NULL !=acw_cal_opt_info[n].fun)
                {
                    acw_cal_opt_info[n].fun(n);
                }
                
				m = 1;/* 重绘背景 */
                draw_short_cut_bm(UDRB);
				break;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			case KEY_1:
				*pm = ACW;
				return;
			case KEY_2:
				*pm = DCW;
				return;
			case KEY_3:
				*pm = IR;
				return;
			case KEY_4:
				*pm = GR;
				return;
			case KEY_5:
				*pm = BBD;
				return;
		}
		
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}
/*++++++++++++++++++++++++++++++++++++++++++++*/
///////////////////////////////////////////////////////////////
// 		******            ******        ****  ****  ***      //
// 		 **  **          **   **         **    **    *       //
// 		 **   **         **   **         **    **    *       //
// 		 **   **        **                **   ***   *       //
// 		 **   **        **                **  * **  *        //
// 		 **   **        **                **  * **  *        //
// 		 **   **        **                 ** *  *  *        //
// 		 **   **        **                 ** *  *  *        //
// 		 **   **         **   **            **    **         //
// 		 **  **          **  **             **    **         //
// 		******            ****              **    **         //
///////////////////////////////////////////////////////////////
// /*
//  * 函数名：start_correct_dcw_vol
//  * 描述  ：开始直流耐压电压校准
//  * 输入  ：vol 校准的电压点
//  * 输出  ：无
//  * 返回  ：无
//  */
void start_correct_dcw_vol(const uint8_t times)
{
	uint8_t err = 0;
	uint16_t* pkey_value = NULL;
	uint32_t temp_vol = 0;
	int8_t temp = 0;
	int16_t t = 0;
	static u16 advlue[3] = {0};
	static float buf[3] = {0.0};
	float div_t = 1.0;
    
	clear_menu();
	draw_short_cut_bm(UDR_);
	GUI_DispStringAt((const char*)"重校", 210, 50);
	ReFresh_LCD24064();
    
	pmenu_data->pData = &temp_vol;
	pmenu_data->ch_bit = 0;
	cursor_pos(pmenu_data->x, pmenu_data->y);
    
	cur_vol = 5100;//dcw_vol_cal[type_spe.amp_type][vol_segment].point_da[cur_set_meal][times][1];
    cur_gear = DC_20mA;/* 切换当前档位到20mA 方便使用万用表进行电压校准 */
	
    if(pmenu_data->decs == 4)
    {
        div_t = 10.0;
    }
    
    startup_cal(DCW, cur_gear);
	
	while(1)
	{
		DAC_Vol[0] = dcw_vol_cal[type_spe.amp_type][vol_segment].point_da[cur_set_meal][times][0];
        
		pkey_value = OSQPend(KeyboardQSem,10,&err);
        
        /* 按下复位键时就要返回 */
		if(0 == STOP_PIN)
		{
            buf[times] = temp_vol/div_t;
			advlue[times] = sample_vol;
			
            // 数据系数曲线拟合算法
			if(times == 2)
			{
				u8 i=0,n=3;
				float sumx=0.0,sumy=0.0,sumz=0.0,sumxx=0.0,sumzz=0.0,sumxy=0.0,sumxz=0.0;
				
				/* 检查输入数据是否为0 如果有就不做计算直接停止 */
				for(i = 0; i < 3; i++)
				{
					if(buf[i] == 0)
					{
						goto l;
					}
				}
				
				for(i=0;i<n;i++)
				{
					sumx 	+= buf[i];
					sumy 	+= dcw_vol_cal[type_spe.amp_type][vol_segment].point_da[cur_set_meal][i][0];
					sumz	+= advlue[i];
					sumxx	+= buf[i] * buf[i];
					sumzz	+= advlue[i] * advlue[i];
					sumxy	+= buf[i] * dcw_vol_cal[type_spe.amp_type][vol_segment].point_da[cur_set_meal][i][0];
					sumxz	+= buf[i] * advlue[i];
				}
                
				ratio_dcw.dac_k[vol_segment]	 	= ((float)sumxy*n-sumx*sumy)/(n*sumxx-sumx*sumx);
				ratio_dcw.dac_b[vol_segment]	 	= ((float)sumy-ratio_dcw.dac_k[vol_segment]*sumx)/n;
				
				ratio_dcw.adc_v_k[vol_segment]	= ((float)sumxz*n-sumx*sumz)/(n*sumzz-sumz*sumz);
				ratio_dcw.adc_v_b[vol_segment]	= ((float)sumx-ratio_dcw.adc_v_k[vol_segment]*sumz)/n;
				
				save_ratio(DCW);
			}
			
	l:		stop_cal(DCW);
			temp_vol = 0;
			pmenu_data->ch_bit = 0;
			update_data(pmenu_data);
			cursor_pos(-5, 0);/* 将光标移出屏幕 */
			return;
		}
		
		CALIB_JUDGE_ERR(DCW);
		
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_vol/t;
				temp = t%10;
                
				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_DOWN:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
                
				t = temp_vol/t;
				temp = t%10;
				
				if(--temp < 0)
				{
					temp = 9;
				}
				
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_F4:
				temp_vol = 0;
				pmenu_data->ch_bit = 0;
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x, pmenu_data->y);
				break;
			default:
				break;
			
		}
	}
}

void correct_dcw_out_v(const uint8_t times)
{
	uint8_t X0	= 40;
	uint8_t X1	= 160;
	uint8_t Y0	= 15;
	uint8_t Y1	= 63;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t m = 1;
	uint8_t n = 1;//pf->work_mode;
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_FillRect(X0+2, Y0+2, (X0+X1)/2-2, Y0+15);
	GUI_FillRect((X0+X1)/2+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"校准电压", X0+6, Y0+3);
	GUI_DispStringAt((const char*)"实际电压", (X0+X1)/2+6, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	pmenu_data = &par_info;	/* 将pmenu_data指向原始密码的信息 */
    
    
	switch(cur_set_meal)
	{
//         case DCW_2kV:
// 		case DCW_6kV:
        DCW_TRANSFORMER_GROUP_1:
			pmenu_data->decs = 3;
            pmenu_data->x = (X0+X1)/2+10-3;
            pmenu_data->y = Y1-22;
            /*********************************/
            pmenu_data->decs = 4;
            pmenu_data->lon = 6;
            pmenu_data->bits = 2;
            /********************************/
            break;
        DCW_TRANSFORMER_GROUP_2:
            pmenu_data->x = (X0+X1)/2+10;
            pmenu_data->y = Y1-22;
			pmenu_data->decs = 3;
			break;
        DCW_TRANSFORMER_GROUP_3:
            pmenu_data->x = (X0+X1)/2+10;
            pmenu_data->y = Y1-22;
			pmenu_data->decs = 3;
            pmenu_data->lon = 6;
			break;
	}
    
	GUI_DispStringAt((const char*)dcw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][times][1], X0+10, Y1-22);
	switch(cur_set_meal)
	{
        default:
        case DCW_2kV:
		case DCW_6kV:
		case DCW_10kV:
            if(pmenu_data->lon == 6)
            {
                GUI_DispStringAt((const char*)"0.0000kV", pmenu_data->x, pmenu_data->y);
            }
            else
            {
                GUI_DispStringAt((const char*)"0.000kV", pmenu_data->x, pmenu_data->y);
            }
			break;
		case DCW_20kV:
			GUI_DispStringAt((const char*)"00.000kV", pmenu_data->x, pmenu_data->y);
			break;
	}
	
	while(1)
	{
		if(m)
		{
			m = 0;
			clear_menu();
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
			ReFresh_LCD24064();
		}
		
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
        
		switch(*pkey_value)
		{
			case KEY_SHIFT:
				if(++n > 1)
				{
					n = 0;
				}
				break;
			case KEY_ENTER:
				return;
			case KEY_BACK:
				GUI_ClearRect (X0, Y0, X1, Y1);
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			case KEY_START:
				start_correct_dcw_vol(times);
				m = 1;
				break;
			default:
				break;
		}
        
		if(g_exit)
		{
			return;
		}
	}
}
void serve_correct_dcw_out_v(uint32_t opt)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t m = 1;
    int8_t n = opt;
	
    if(NULL == dcw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][0][0])
    {
        return;
    }
    
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_dcw();
			
			clear_menu();
			GUI_DispStringAt((const char*)dcw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][0][0], 208, 2);
			GUI_DispStringAt((const char*)dcw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][1][0], 208, 18);
			GUI_DispStringAt((const char*)dcw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][2][0], 208, 34);
			
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
            GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0+115 * (n / 4), 15 + 2 + 12 * ( n % 4));
			ReFresh_LCD24064();
		}
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
                if(NULL != dcw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][0][0])
                    correct_dcw_out_v(0);
				m = 1;/* 重绘背景 */
				break;
			case KEY_F2:
                if(NULL != dcw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][1][0])
                    correct_dcw_out_v(1);
				m = 1;/* 重绘背景 */
				break;
			case KEY_F3:
                if(NULL != dcw_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][2][0])
                    correct_dcw_out_v(2);
				m = 1;/* 重绘背景 */
				break;
			case KEY_BACK:
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
        
		if(g_return)
		{
			g_return = 0;
		}
        
		if(g_exit)
		{
			return;
		}
	}
}
void set_out_dcw_vol(void)
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_vol = 0;
	int8_t temp = 0;
	int16_t t = 0;
	
	DATA_INFO vol_info =
	{
		(void*)0/*pData*/,X0+10/*x*/,Y1-22/*y*/,1/*type*/,5/*lon*/,3/*decs*/,0/*total*/,1/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 16);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
// 	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_DispStringAt((const char*)"0.500kV", X0 + 10, Y1-22);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, (X0+X1)/2 + 25, Y1-20);
	GUI_DispStringAt((const char*)"0.050kV", (X0+X1)/2 + 8, Y1-30);
	GUI_DispStringAt((const char*)"5.000kV", (X0+X1)/2 + 8, Y1-12);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"设置校准输出电压", X0+15, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	pmenu_data = &vol_info;	/* 将pmenu_data指向原始密码的信息 */
	pmenu_data->pData = &temp_vol;
	pmenu_data->ch_bit = 0;
	cursor_pos(pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		if(0 == STOP_PIN)
		{/* 按下复位键时就要返回 */
			cursor_pos(0, -1);/* 将光标移出屏幕 */
			return;
		}
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_vol/t;
				temp = t%10;

				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_DOWN:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_vol/t;
				temp = t%10;

				if(--temp < 0)
				{
					temp = 9;
				}

				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_BACK:
				GUI_ClearRect(X0, Y0, X1, Y1);
				g_return = 1;
				return;
			case KEY_START:
				return;
			default:
				break;
			
		}
	}
}

///////////////////////////////////
void start_correct_dcw_cur(const int16_t vol, const int8_t gear)
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_cur = 0;
	int8_t temp = 0;
	int16_t t = 0;
	
	static u16	advlue[CUR_KINDS_MAX] = {0};
	static float buf[CUR_KINDS_MAX] = {0.0};
	
	DATA_INFO cur_info =
	{
		(void*)0/*pData*/,(X0+X1)/2+10/*x*/,Y1-22/*y*/,1/*type*/,5/*lon*/,3/*decs*/,0/*total*/,1/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
	
	switch(gear)
	{
		case DC_2uA:
			cur_info.decs = 3;
			break;
		case DC_20uA:
			cur_info.decs = 2;
			break;
		case DC_200uA:
			cur_info.decs = 1;
			break;
		case DC_2mA:
			cur_info.decs = 3;
			break;
		case DC_10mA:
			cur_info.decs = 2;
			break;
		case DC_20mA:
			cur_info.decs = 2;
			break;
		case DC_50mA:
			cur_info.decs = 2;
			break;
		case DC_100mA:
			cur_info.decs = 1;
			break;
	}
	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 16);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	GUI_DispStringAt((const char*)"重校", 210, 50);
	ReFresh_LCD24064();
	
	pmenu_data = &cur_info;	/* 将pmenu_data指向原始密码的信息 */
	pmenu_data->pData = &temp_cur;
	pmenu_data->ch_bit = 0;
	cursor_pos(pmenu_data->x, pmenu_data->y);
	
//     if(type_spe.amp_type == PWM_AMP)
//     {
//         vol_segment = 0;
//     }
//     else
//     {
//     
//         switch(type_spe.dcw_vol_range)
//         {
//             case DCW_6kV:
//                 vol_segment = vol > 500? 1:0;
//                 break;
//             case DCW_10kV:
//                 vol_segment = 1;
//                 break;
//         }
//     }
    
    confirm_vol_segment_s();
    
	startup_cal(DCW, gear);
	while(1)
	{
		
		DAC_Vol[0] = vol*ratio_dcw.dac_k[vol_segment] + ratio_dcw.dac_b[vol_segment];
		
		
		advlue[gear] = dis_cur(DCW, gear);/* 显示AD采样电流 */
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		/* 按下复位键时就要返回 */
		if(0 == STOP_PIN)
		{
			save_ratio(DCW);
			
			stop_cal(DCW);
			cursor_pos(-5, 0);/* 将光标移出屏幕 */
			g_return = 1;
			GUI_ClearRect(X0, Y0, X1, Y1);
			g_return = 1;
			return;
		}
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_cur/t;
				temp = t%10;
				
				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_DOWN:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_cur/t;
				temp = t%10;

				if(--temp < 0)
				{
					temp = 9;
				}

				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_F4:
				ratio_dcw.adc_cur_k[gear] = 1.0;
				temp_cur = 0;
				pmenu_data->ch_bit = 0;
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x, pmenu_data->y);
				break;
			case KEY_ENTER:
				buf[gear] = (float)temp_cur;
				ratio_dcw.adc_cur_k[gear] = ((float)buf[gear])/advlue[gear];
				break;
			default:
				break;
		}
	}
}

void correct_dcw_cur_gear(const uint8_t gear, const uint16_t vol)
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_FillRect(X0+2, Y0+2, (X0+X1)/2-2, Y0+15);
	GUI_FillRect((X0+X1)/2+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"测量电流", X0+6, Y0+3);
	GUI_DispStringAt((const char*)"实际电流", (X0+X1)/2+6, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	switch(gear)
	{
		case DC_2uA:
			GUI_DispStringAt((const char*)"0.000~A", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"0.000~A", (X0+X1)/2+10, Y1-22);
			break;
		case DC_20uA:
			GUI_DispStringAt((const char*)"00.00~A", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"00.00~A", (X0+X1)/2+10, Y1-22);
			break;
		case DC_200uA:
			GUI_DispStringAt((const char*)"000.0~A", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"000.0~A", (X0+X1)/2+10, Y1-22);
			break;
		case DC_2mA:
			GUI_DispStringAt((const char*)"0.000mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"0.000mA", (X0+X1)/2+10, Y1-22);
			break;
		case DC_10mA:
			GUI_DispStringAt((const char*)"00.00mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"00.00mA", (X0+X1)/2+10, Y1-22);
			break;
		case DC_20mA:
			GUI_DispStringAt((const char*)"00.00mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"00.00mA", (X0+X1)/2+10, Y1-22);
			break;
		case DC_50mA:
			GUI_DispStringAt((const char*)"00.00mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"00.00mA", (X0+X1)/2+10, Y1-22);
			break;
		case DC_100mA:
			GUI_DispStringAt((const char*)"000.0mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"000.0mA", (X0+X1)/2+10, Y1-22);
			break;
	}
	
	GUI_GotoXY(X0+10, Y1-22);
	
	start_correct_dcw_cur(vol, gear);
}
void serve_correct_dcw_cur_uA(uint32_t opt)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t m = 1;
	int16_t temp_vol = 0;
    int8_t n = opt;
	
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_dcw();
			
			clear_menu();
			GUI_DispStringAt((const char*)"2~A", 208, 2);
			GUI_DispStringAt((const char*)"20~A", 208, 18);
			GUI_DispStringAt((const char*)"200~A", 208, 34);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
            GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0+115 * (n / 4), 15 + 2 + 12 * ( n % 4));
			ReFresh_LCD24064();
		}
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				if(!g_return)
				{
					correct_dcw_cur_gear(DC_2uA, temp_vol);
				}
				m = 1;/* 重绘背景 */
				break;
			case KEY_F2:
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				if(!g_return)
				{
					correct_dcw_cur_gear(DC_20uA, temp_vol);
				}
				m = 1;/* 重绘背景 */
				break;
			case KEY_F3:
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				if(!g_return)
				{
					correct_dcw_cur_gear(DC_200uA, temp_vol);
				}
				m = 1;/* 重绘背景 */
				break;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}

void dcw_cur_more(const uint8_t **gear_buf, uint8_t cur_kinds, uint8_t *flag)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t m = 1;
	int16_t temp_vol = 0;
	
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_dcw();
			
			clear_menu();
			switch(cur_kinds)
			{
				case 4:
					GUI_DispStringAt((const char*)gear_buf[2], 208, 2);
					GUI_DispStringAt((const char*)gear_buf[3], 208, 18);
					break;
				case 5:
					GUI_DispStringAt((const char*)gear_buf[2], 208, 2);
					GUI_DispStringAt((const char*)gear_buf[3], 208, 18);
					GUI_DispStringAt((const char*)gear_buf[4], 208, 34);
					break;
			}
// 			GUI_DispStringAt((const char*)"20mA", 208, 2);
// 			GUI_DispStringAt((const char*)"50mA", 208, 18);
// 			GUI_DispStringAt((const char*)"更多", 208, 34);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
			ReFresh_LCD24064();
		}
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
				if(cur_kinds<4)
				{
					break;
				}
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				if(!g_return)
				{
					correct_dcw_cur_gear(flag[2], temp_vol);
				}
				m = 1;/* 重绘背景 */
				break;
			case KEY_F2:
				if(cur_kinds<4)
				{
					break;
				}
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				if(!g_return)
				{
					correct_dcw_cur_gear(flag[3], temp_vol);
				}
				m = 1;/* 重绘背景 */
				break;
			case KEY_F3:
				if(cur_kinds<4)
				{
					break;
				}
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				if(!g_return)
				{
					correct_dcw_cur_gear(flag[4], temp_vol);
				}
				m = 1;/* 重绘背景 */
				break;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}


void serve_correct_dcw_cur_mA(uint32_t opt)/* 测量电流 */
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t m = 1;
	int16_t temp_vol = 0;
	
	uint32_t cur_kinds = 0;/* 电流的种类 */
	const uint8_t *gear_buf[CUR_KINDS_MAX] = {0};/* 电流档位 */
	uint8_t flag[CUR_KINDS_MAX] = {0};/* 标志 */
	int32_t i = 0;
    int8_t n = opt;
	
	cur_kinds = define_cur_kinds(DCW, gear_buf, flag);
	if(strcmp((const char*)gear_buf[0], "2~A") == 0)
	{
		cur_kinds -= 1;
		for(i = 0; i < cur_kinds; i++)
		{
			gear_buf[i] = gear_buf[i+1];
			flag[i] = flag[i+1];
		}
        gear_buf[i] = "";
        flag[i] = 0;
	}
	if(strcmp((const char*)gear_buf[0], "20~A") == 0)
	{
		cur_kinds -= 1;
		for(i = 0; i < cur_kinds; i++)
		{
			gear_buf[i] = gear_buf[i+1];
			flag[i] = flag[i+1];
		}
        gear_buf[i] = "";
        flag[i] = 0;
	}
	if(strcmp((const char*)gear_buf[0], "200~A") == 0)
	{
		cur_kinds -= 1;
		for(i = 0; i < cur_kinds; i++)
		{
			gear_buf[i] = gear_buf[i+1];
			flag[i] = flag[i+1];
		}
        gear_buf[i] = "";
        flag[i] = 0;
	}
	
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_dcw();
			
			clear_menu();
			if(cur_kinds<4)
			{
				switch(cur_kinds)
				{
					case 3:
						GUI_DispStringAt((const char*)gear_buf[2], 208, 34);
					case 2:
						GUI_DispStringAt((const char*)gear_buf[1], 208, 18);
					case 1:
						GUI_DispStringAt((const char*)gear_buf[0], 208, 2);
				}
			}
			else
			{
				GUI_DispStringAt((const char*)gear_buf[0], 208, 2);
				GUI_DispStringAt((const char*)gear_buf[1], 208, 18);
				GUI_DispStringAt((const char*)"更多", 208, 34);
			}
            
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
            GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0+115 * (n / 4), 15 + 2 + 12 * ( n % 4));
			ReFresh_LCD24064();
		}
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
            {
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				if(!g_return)
				{
					correct_dcw_cur_gear(flag[0], temp_vol);
				}
				m = 1;/* 重绘背景 */
				break;
            }
			case KEY_F2:
            {
				if(cur_kinds < 2)
                {
                    break;
                }
                
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				if(!g_return)
				{
					correct_dcw_cur_gear(flag[1], temp_vol);
				}
				m = 1;/* 重绘背景 */
				break;
            }
			case KEY_F3:
            {
				if(cur_kinds < 3)
                {
                    break;
                }
                
				if(cur_kinds < 4)
				{
					temp_vol = set_out_vol();
					if(g_exit)
					{
						return;
					}
					if(!g_return)
					{
						correct_dcw_cur_gear(flag[2], temp_vol);
					}
				}
				else
				{
					dcw_cur_more(gear_buf, cur_kinds, flag);
				}
				m = 1;/* 重绘背景 */
				break;
            }
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}

//////////////////////////////////
/* 电弧侦测等级模式 */
void correct_dcw_arc_g(void)
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_arc = ratio_dcw.arc_k * 4096;
	int16_t temp = 0;
	int16_t t = 0;
	
	DATA_INFO arc_info =
	{
		(void*)0/*pData*/,X0+15/*x*/,Y1-22/*y*/,0/*type*/,4/*lon*/,0/*decs*/,0/*total*/,0/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 16);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
// 	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_DispStringAt((const char*)"0000", X0 + 15, Y1-22);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, (X0+X1)/2 + 25, Y1-20);
// 	GUI_DispStringAt((const char*)"   0", (X0+X1)/2 + 8, Y1-30);
	GUI_DispStringAt((const char*)"(0-4096)", (X0+X1)/2 + 5, Y1-22);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"电弧侦测校准", X0+25, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	pmenu_data = &arc_info;	/* 将pmenu_data指向原始密码的信息 */
	pmenu_data->pData = &temp_arc;
	pmenu_data->ch_bit = 0;
	GUI_DispDecAt (*(uint32_t*)pmenu_data->pData, pmenu_data->x, pmenu_data->y, pmenu_data->lon);
	cursor_pos(pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		if(0 == STOP_PIN)
		{/* 按下复位键时就要返回 */
			cursor_pos(0, -1);/* 将光标移出屏幕 */
			GUI_ClearRect(X0, Y0, X1, Y1);
			return;
		}
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				t = temp_arc/t;
				temp = t%10;

				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_DOWN:
				t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				t = temp_arc/t;
				temp = t%10;

				if(--temp < 0)
				{
					temp = 9;
				}

				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_ENTER:
				if(temp_arc > 4096)
				{
					temp_arc = 4096;
				}
				
				ratio_dcw.arc_k = (float)temp_arc/4096;
				
				save_ratio(DCW);
				GUI_ClearRect(X0, Y0, X1, Y1);
				return;
			case KEY_BACK:
				GUI_ClearRect(X0, Y0, X1, Y1);
				g_return = 1;
				return;
			default:
				break;
		}
	}
}
/* 电弧侦测电流模式 */
void correct_dcw_arc_c(void)
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_cur = ratio_dcw.arc_k * 2000;
	int8_t temp = 0;
	int16_t t = 0;
	
	DATA_INFO cur_info =
	{
		(void*)0/*pData*/,X0+10/*x*/,Y1-22/*y*/,1/*type*/,5/*lon*/,2/*decs*/,0/*total*/,0/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 16);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
// 	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_DispStringAt((const char*)"05.00mA", X0 + 10, Y1-22);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, (X0+X1)/2 + 25, Y1-20);
	GUI_DispStringAt((const char*)"00.10mA", (X0+X1)/2 + 8, Y1-30);
	GUI_DispStringAt((const char*)"20.00mA", (X0+X1)/2 + 8, Y1-12);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"设置电弧侦测电流", X0+15, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	pmenu_data = &cur_info;	/* 将pmenu_data指向原始密码的信息 */
	pmenu_data->pData = &temp_cur;
	pmenu_data->ch_bit = 0;
	GUI_GotoXY(pmenu_data->x, pmenu_data->y);
	GUI_DispFloatFix ((float)temp_cur/100, 5, 2);
	cursor_pos(pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		if(0 == STOP_PIN)
		{/* 按下复位键时就要返回 */
			cursor_pos(0, -1);/* 将光标移出屏幕 */
			return;
		}
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_cur/t;
				temp = t%10;

				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_DOWN:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_cur/t;
				temp = t%10;
				
				if(--temp < 0)
				{
					temp = 9;
				}
				
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_ENTER:
				if(temp_cur < 10)
				{
					temp_cur = 10;
				}
				else if(temp_cur > 2000)
				{
					temp_cur = 2000;
				}
				
				ratio_dcw.arc_k = (float)temp_cur/2000;
				GUI_ClearRect(X0, Y0, X1, Y1);
				return;
			case KEY_BACK:
				GUI_ClearRect(X0, Y0, X1, Y1);
				g_return = 1;
				return;
			default:
				break;
			
		}
	}
}
void serve_correct_dcw_arc(uint32_t opt)/* 电弧侦测 */
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t m = 1;
    int8_t n = opt;
	
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_dcw();
			
			clear_menu();
			GUI_DispStringAt((const char*)"等级", 208, 2);
			GUI_DispStringAt((const char*)"电流", 208, 18);
            
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
            GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0+115 * (n / 4), 15 + 2 + 12 * ( n % 4));
			ReFresh_LCD24064();
		}
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
				correct_dcw_arc_g();/* 等级模式 */
				m = 1;/* 重绘背景 */
				break;
			case KEY_F2:
				correct_dcw_arc_c();/* 电流模式 */
				if(!g_return)
				{
					correct_dcw_arc_g();
				}
				m = 1;/* 重绘背景 */
				break;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}



//////////////////////////////////
void correct_dcw(int8_t *pm)
{
    #define COLUMN_NUM   4
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t n = 0;
	int8_t m = 1;
    int8_t opt = sizeof dcw_cal_opt_info / sizeof dcw_cal_opt_info[0];
    
	cur_set_meal  = get_cal_dcw_vol_meal();
    
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_dcw();
			
            draw_short_cut_bm(UDRB);
		}
        
		GUI_ClearRect(0,15,12,64-1);
		GUI_ClearRect(13+100,15,13+113,64-1);
        
		GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0+115 * (n / 4), 15 + 2 + 12 * ( n % 4));
        
		ReFresh_LCD24064();
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_UP:
				if(--n < 0)
				{
					n = opt - 1;
				}
				break;
			case KEY_DOWN:
				if(++n > opt -1)
				{
					n = 0;
				}
				break;
            case KEY_SHIFT:
				if(n + COLUMN_NUM <= opt - 1)
				{
					n += COLUMN_NUM;
				}
                else if(n >= COLUMN_NUM)
                {
                    n -= COLUMN_NUM;
                }
                break;
			case KEY_ENTER:
                n = n % opt;
                if(NULL !=dcw_cal_opt_info[n].fun)
                {
                    dcw_cal_opt_info[n].fun(n);
                }
                
				m = 1;/* 重绘背景 */
				break;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			case KEY_1:
				*pm = ACW;
				return;
			case KEY_2:
				*pm = DCW;
				return;
			case KEY_3:
				*pm = IR;
				return;
			case KEY_4:
				*pm = GR;
				return;
			case KEY_5:
				*pm = BBD;
				return;
		}
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}
/*++++++++++++++++++++++++++++++++++++++++++*/
/////////////////////////////////////////
// 		 ******         *******        //
// 		   **            **   **       //
// 		   **            **   **       //
// 		   **            **   **       //
// 		   **            ******        //
// 		   **            ** **         //
// 		   **            ** **         //
// 		   **            **  **        //
// 		   **            **  **        //
// 		   **            **   **       //
// 		 ******         ****  ***      //
/////////////////////////////////////////
void start_correct_ir_vol(const uint8_t times)
{
	uint8_t err = 0;
	uint16_t* pkey_value = NULL;
	uint32_t temp_vol = 0;
	int8_t temp = 0;
	int16_t t = 0;
    float div_t = 1.0;
	static u16 advlue[3] = {0};
	static float buf[3] = {0.0};
	
	clear_menu();
	draw_short_cut_bm(UDR_);
	GUI_DispStringAt((const char*)"重校", 210, 50);
	ReFresh_LCD24064();
    
	pmenu_data->pData = &temp_vol;
	pmenu_data->ch_bit = 0;
	cursor_pos(pmenu_data->x, pmenu_data->y);
    
	cur_vol = ir_vol_cal[type_spe.amp_type][vol_segment].point_da[cur_set_meal][times][1];
    cur_gear = IR_10MOHM;
	
    if(pmenu_data->decs == 4)
    {
        div_t = 10.0;
    }
    
    startup_cal(IR, cur_gear);
    MC14094_CMD(MC14094_B, MC14094_W_IR, 1);/* 切换当前档位到20mA 方便使用万用表进行电压校准 */
	MC14094_Updata();// 更新输出缓冲区
	
	while(1)
	{
		DAC_Vol[0] = ir_vol_cal[type_spe.amp_type][vol_segment].point_da[cur_set_meal][times][0];
        
		pkey_value = OSQPend(KeyboardQSem,10,&err);
        
        /* 按下复位键时就要返回 */
		if(0 == STOP_PIN)
		{
            buf[times] = temp_vol/div_t;
            
			advlue[times] = sample_vol;
			
            // 数据系数曲线拟合算法
			if(times == 2)
			{
				u8 i=0,n=3;
				float sumx=0.0,sumy=0.0,sumz=0.0,sumxx=0.0,sumzz=0.0,sumxy=0.0,sumxz=0.0;
				
				/* 检查输入数据是否为0 如果有就不做计算直接停止 */
				for(i = 0; i < 3; i++)
				{
					if(buf[i] == 0)
					{
						goto l;
					}
				}
				
				for(i=0;i<n;i++)
				{
					sumx 	+= buf[i];
					sumy 	+= ir_vol_cal[type_spe.amp_type][vol_segment].point_da[cur_set_meal][i][0];
					sumz	+= advlue[i];
					sumxx	+= buf[i] * buf[i];
					sumzz	+= advlue[i] * advlue[i];
					sumxy	+= buf[i] * ir_vol_cal[type_spe.amp_type][vol_segment].point_da[cur_set_meal][i][0];
					sumxz	+= buf[i] * advlue[i];
				}
                
				ratio_ir.dac_k[vol_segment]	 	= ((float)sumxy * n - sumx * sumy) / (n * sumxx - sumx * sumx);
				ratio_ir.dac_b[vol_segment]	 	= ((float)sumy - ratio_ir.dac_k[vol_segment] * sumx) / n;
				
				ratio_ir.adc_v_k[vol_segment]	= ((float)sumxz * n - sumx * sumz) / (n * sumzz - sumz * sumz);
				ratio_ir.adc_v_b[vol_segment]	= ((float)sumx - ratio_ir.adc_v_k[vol_segment] * sumz) / n;
				
				save_ratio(IR);
			}
			
	l:		stop_cal(IR);
			temp_vol = 0;
			pmenu_data->ch_bit = 0;
			update_data(pmenu_data);
			cursor_pos(-5, 0);/* 将光标移出屏幕 */
			return;
		}
		
// 		CALIB_JUDGE_ERR(DCW);
		
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_vol/t;
				temp = t%10;
                
				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_DOWN:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
                
				t = temp_vol/t;
				temp = t%10;
				
				if(--temp < 0)
				{
					temp = 9;
				}
				
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_F4:
				temp_vol = 0;
				pmenu_data->ch_bit = 0;
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x, pmenu_data->y);
				break;
			default:
				break;
			
		}
	}
}


void correct_ir_out_v(const uint8_t times)
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t m = 1;
	uint8_t n = 1;//pf->work_mode;
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_FillRect(X0+2, Y0+2, (X0+X1)/2-2, Y0+15);
	GUI_FillRect((X0+X1)/2+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"校准电压", X0+6, Y0+3);
	GUI_DispStringAt((const char*)"实际电压", (X0+X1)/2+6, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	pmenu_data = &par_info;	/* 将pmenu_data指向原始密码的信息 */
    
// 	switch(cur_set_meal)
	{
// 		case DCW_6kV:
			pmenu_data->decs = 3;
            pmenu_data->x = (X0+X1)/2+10-3;
            pmenu_data->y = Y1-22;
            /*********************************/
            pmenu_data->decs = 4;
            pmenu_data->lon = 6;
            pmenu_data->bits = 2;
            /********************************/
//             break;
// 		case DCW_10kV:
//             pmenu_data->x = (X0+X1)/2+10;
//             pmenu_data->y = Y1-22;
// 			pmenu_data->decs = 3;
// 			break;
// 		case DCW_20kV:
//             pmenu_data->x = (X0+X1)/2+10;
//             pmenu_data->y = Y1-22;
// 			pmenu_data->decs = 2;
// 			break;
	}
    
	GUI_DispStringAt((const char*)ir_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][times][1], X0+10, Y1-22);
// 	switch(cur_set_meal)
// 	{
// 		case DCW_6kV:
// 		case DCW_10kV:
            if(pmenu_data->lon == 6)
            {
                GUI_DispStringAt((const char*)"0.0000kV", pmenu_data->x, pmenu_data->y);
            }
            else
            {
                GUI_DispStringAt((const char*)"0.000kV", pmenu_data->x, pmenu_data->y);
            }
// 			break;
// 		case DCW_20kV:
// 			GUI_DispStringAt((const char*)"00.00kV", pmenu_data->x, pmenu_data->y);
// 			break;
// 	}
	
	while(1)
	{
		if(m)
		{
			m = 0;
			clear_menu();
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
			ReFresh_LCD24064();
		}
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);
        
		switch(*pkey_value)
		{
			case KEY_SHIFT:
				if(++n > 1)
				{
					n = 0;
				}
				break;
			case KEY_ENTER:
				return;
			case KEY_BACK:
				GUI_ClearRect (X0, Y0, X1, Y1);
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			case KEY_START:
				start_correct_ir_vol(times);
				m = 1;
				break;
			default:
				break;
		}
		if(g_exit)
		{
			return;
		}
	}
}
void correct_ir_out_v1(const uint16_t vol)
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t m = 1;
	uint8_t n = 1;//pf->work_mode;
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_FillRect(X0+2, Y0+2, (X0+X1)/2-2, Y0+15);
	GUI_FillRect((X0+X1)/2+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"校准电压", X0+6, Y0+3);
	GUI_DispStringAt((const char*)"实际电压", (X0+X1)/2+6, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	GUI_DispStringAt((const char*)"0.000kV", X0+10, Y1-22);
	GUI_DispStringAt((const char*)"0.000kV", (X0+X1)/2+10, Y1-22);
	GUI_GotoXY(X0+10, Y1-22);
	GUI_DispFloatFix ((float)vol/1000, 5, 3);
	
	while(1)
	{
		if(m)
		{
			m = 0;
			clear_menu();
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
			ReFresh_LCD24064();
		}
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);

		switch(*pkey_value)
		{
			case KEY_SHIFT:
				if(++n > 1)
				{
					n = 0;
				}
				break;
			case KEY_ENTER:
				return;
			case KEY_BACK:
				GUI_ClearRect (X0, Y0, X1, Y1);
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			case KEY_START:
				start_correct_ir_vol(0);
				m = 1;
				break;
			default:
				break;
		}
	}
}

void serve_correct_ir_out_v(uint32_t opt)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t m = 1;
    int8_t n = opt;
	
    if(NULL == ir_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][0][0])
    {
        return;
    }
    
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_ir();
			
			clear_menu();
			GUI_DispStringAt((const char*)ir_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][0][0], 208, 2);
			GUI_DispStringAt((const char*)ir_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][1][0], 208, 18);
			GUI_DispStringAt((const char*)ir_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][2][0], 208, 34);
			
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
            GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0+115 * (n / 4), 15 + 2 + 12 * ( n % 4));
			ReFresh_LCD24064();
		}
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
                if(NULL != ir_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][0][0])
                    correct_ir_out_v(0);
				m = 1;/* 重绘背景 */
				break;
			case KEY_F2:
                if(NULL != ir_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][1][0])
                    correct_ir_out_v(1);
				m = 1;/* 重绘背景 */
				break;
			case KEY_F3:
                if(NULL != ir_vol_cal[type_spe.amp_type][vol_segment].point_menu[cur_set_meal][2][0])
                    correct_ir_out_v(2);
				m = 1;/* 重绘背景 */
				break;
			case KEY_BACK:
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}




void dis_correct_ir_vol(void)
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
// 	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	
	uint16_t temp_vol = 0;
	
	temp_vol = sample_vol * ratio_ir.adc_v_k[vol_segment] + ratio_ir.adc_v_b[vol_segment];
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_GotoXY((X0+X1)/2+10, Y1-22);
	GUI_DispFloatFix ((float)temp_vol/1000, 5, 3);
	ReFresh_LCD24064();
}

////////////////////////////////////////////

uint8_t confirm_vol_segment_ss(uint16_t vol)
{
    uint8_t temp = 0;
    
    if(type_spe.vol_is_segment == VOL_SEGMENT_NO)
    {
        temp = 1;
    }
    else
    {
        if(vol > 500)
        {
            temp = 1;
        }
        else
        {
            temp = 0;
        }
    }
    
    return temp;
}

void start_correct_ir_res(const int8_t gear)
{
	uint8_t X0	= 40;
	uint8_t X1	= 160;
	uint8_t Y1	= 63;
	
	uint16_t ir_vol = 0;
	uint16_t temp_cur = 0;
	uint16_t temp_vol = 0;
	uint16_t cal_time = 0;
	float std_res = 0.0;/* 标准的电阻值 */
	int i = 0;
	int j = 0;
    uint8_t temp_delay = 0;
    uint8_t temp = 0;
    uint8_t target_gear = 0;/* 目标档位 */
    uint8_t vol_gear_num;/* 电压档位个数 */
    uint8_t vol_base;/* 电压起始点 */
    uint8_t delay_1 = 0;
    uint8_t delay_2 = 0;
    
	clear_menu();
	ReFresh_LCD24064();
    
    std_res = ir_cur_cal.point_da[cur_cal_ir_res_meal][gear - 1];
    cal_time = 100;//delay_cal[gear];
	vol_segment = 1;
    
    target_gear = ir_cur_cal.gear[cur_cal_ir_res_meal][gear - 1];
    
    cur_gear = target_gear;
    
    
    delay_1 = 40;
    delay_2 = 90;
    
	memset(&ratio_ir.res_3_k[gear - 1], 0, sizeof(ratio_ir.res_3_k[0]));
	
    switch(type_spe.ir_vol_range)
    {
        case IR_1kV:
            vol_gear_num = 10;
            vol_base = 0;/* 100v起始 */
            break;
        case IR_1_5kV:
            vol_gear_num = 15;
            vol_base = 0;/* 100v起始 */
            break;
        case IR_2_5kV:
            vol_gear_num = 17;
            vol_base = 0;/* 100v起始 32khz 功放从100v*/
            break;
        case IR_5kV:
            vol_gear_num = 22;
            vol_base = 4;/* 500v起始 */
            break;
        case IR_10kV:
            vol_gear_num = 10;
            vol_base = 0;/* 5000v起始 */
            delay_1 = 15;
            delay_2 = 80;
            break;
    }
    
	startup_cal(IR, cur_gear);
    
	while(1)
	{
		/* 按下复位键时就要返回 */
		if(0 == STOP_PIN)
		{
			stop_cal(IR);
			return;
		}
		
		ir_vol = 0;
        
        MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, 0);/* DA电压段选择 */
        MC14094_Updata();
        
        if(type_spe.amp_type == LINE_AMP)
        {
            MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, 0);/* DA电压段选择 */
            MC14094_Updata();
        }
        
		for(i = vol_base; i < vol_gear_num; i++)
		{
            if(type_spe.ir_vol_range == IR_10kV)
            {
                ir_vol = 5000 + i * 500;/* 每次递增500v */
            }
            else
            {
                if(i < 15)
                {
                    ir_vol = 100 + i * 100;/* 每次递增100v */
                }
                else
                {
                    ir_vol = 1500 + (i - 14) * 500;/* 每次递增500v */
                }
            }
            
            temp = 5;
            
            vol_segment = confirm_vol_segment_ss(ir_vol);
            
            MC14094_CMD(MC14094_A, MC14094_A_VOL_SEL, !vol_segment);
            MC14094_Updata();
            
            DAC_Vol[0] = ir_vol * ratio_ir.dac_k[vol_segment] + ratio_ir.dac_b[vol_segment];/* 输出电压 使用dcw的校准系数 */
            g_ir_dly += 200;
			OSTimeDlyHMSM(0,0,1,0);
            
			for(j = 0; j < cal_time; j++)
			{
				temp_vol = sample_vol * ratio_ir.adc_v_k[vol_segment] + ratio_ir.adc_v_b[vol_segment];
				temp_cur = sample_cur;
				
                if(cur_gear > 4)
                {
                    OSTimeDlyHMSM(0,0,0,delay_2);
                }
                else
                {
                    OSTimeDlyHMSM(0,0,0,delay_1);
                }
                
                if(++temp_delay > temp)
                {
					ir_auto_find_gear(&g_test_data);
                    temp_delay = 0;
                    dis_correct_ir_vol();
                    GUI_DispDecAt(cur_gear, (X0 + X1) / 2 + 10, Y1 - 22 + 10, 1);
                    
                    GUI_GotoXY((X0 + X1) / 2 + 10 + 10, Y1 - 22 + 10);
                    GUI_DispFloatFix(g_test_data.cur_ad_vol, 5, 3);
                }
                
				CALIB_JUDGE_ERR(IR);
				
				if(STOP_PIN == 0)
				{
					stop_cal(IR);
					GUI_DispStringAt((const char*)"0.000", (X0 + X1) / 2 + 10, Y1 - 22);
					ReFresh_LCD24064();
					return;
				}
			}
            
            if(temp_vol == 0 || temp_cur == 0)
            {
                ratio_ir.res_3_k[gear-1][i] = default_ir_res_k[gear-1];
            }
            else
            {
				ratio_ir.res_3_k[gear-1][i] = (float)std_res * temp_cur / (float)temp_vol;
				ratio_ir.cur_ad_v[gear-1][i] = g_test_data.cur_ad_vol;
				ratio_ir.gear_b[gear-1][i] = cur_gear;
            }
		}
		
		stop_cal(IR);
		GUI_DispStringAt((const char*)"0.000", (X0+X1)/2+10, Y1-22);
		ReFresh_LCD24064();
		
		save_ratio(IR);
		return;
	}
}
void correct_ir_measure(const uint8_t gear)/* 测量电阻 */
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t m = 1;
	uint8_t n = 1;//pf->work_mode;
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_FillRect(X0+2, Y0+2, (X0+X1)/2-2, Y0+15);
	GUI_FillRect((X0+X1)/2+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"校准电阻", X0+6, Y0+3);
	GUI_DispStringAt((const char*)"输出电压", (X0+X1)/2+6, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
    GUI_DispStringAt((const char*)"0.000kV", (X0+X1)/2+10, Y1-22);
    GUI_DispStringAt((const char*)ir_cur_cal.point_menu[cur_cal_ir_res_meal][gear - 1], X0 + 20, Y1 - 22);
    
	while(1)
	{
		if(m)
		{
			m = 0;
			clear_menu();
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
			ReFresh_LCD24064();
		}
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		
		switch(*pkey_value)
		{
			case KEY_SHIFT:
				if(++n > 1)
				{
					n = 0;
				}
				break;
			case KEY_BACK:
				GUI_ClearRect (X0, Y0, X1, Y1);
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			case KEY_START:
                start_correct_ir_res(gear);
//                 switch(cur_cal_ir_res_meal)
//                 {
//                     case 0://1kV  1_5kV
//                         
//                         break;
//                     case 1://2.5kV
//                         start_correct_ir_res_1(gear);
// //                         start_correct_ir_res_2(gear);
//                         break;
//                     case 2://5.0kV
//                         start_correct_ir_res_1(gear);
//                         break;
//                 }
                
				m = 1;
				break;
			default:
				break;
		}
	}
}


void serve_correct_ir_res(uint32_t opt)/* 测量电阻 范围是1M-1G */
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t m = 1;
    int8_t n = opt;
    uint8_t opt1 = ir_cal_opt_info[n].data;
    uint8_t opt2 = opt1 + 1;
    uint8_t opt3 = opt1 + 2;
    
    uint8_t opt1_en = 1;
    uint8_t opt2_en = 1;
    uint8_t opt3_en = 1;
    
    if(0 == ir_cal_opt_info[n].size)
    {
        opt1_en = 0;
        opt2_en = 0;
        opt3_en = 0;
    }
    if(1 == ir_cal_opt_info[n].size)
    {
        opt2_en = 0;
        opt3_en = 0;
    }
    else if(2 == ir_cal_opt_info[n].size)
    {
        opt3_en = 0;
    }
	
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_ir();
			
			clear_menu();
            if(opt1_en)
			GUI_DispStringAt((const char*)ir_cur_cal.point_menu[cur_cal_ir_res_meal][opt1], 208, 2);
			if(opt2_en)
            GUI_DispStringAt((const char*)ir_cur_cal.point_menu[cur_cal_ir_res_meal][opt2], 208, 18);
			if(opt3_en)
            GUI_DispStringAt((const char*)ir_cur_cal.point_menu[cur_cal_ir_res_meal][opt3], 206, 34);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
            GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0+115 * (n / 4), 15 + 2 + 12 * ( n % 4));
			ReFresh_LCD24064();
		}
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
                if(NULL != ir_cur_cal.point_menu[cur_cal_ir_res_meal][opt1])
                    correct_ir_measure(opt1 + 1);
				break;
			case KEY_F2:
                if(NULL != ir_cur_cal.point_menu[cur_cal_ir_res_meal][opt2])
                    correct_ir_measure(opt2 + 1);
				break;
			case KEY_F3:
                if(NULL != ir_cur_cal.point_menu[cur_cal_ir_res_meal][opt3])
                    correct_ir_measure(opt3 + 1);
				break;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		m = 1;/* 重绘背景 */
		
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}
/////////////////////////////////
uint8_t get_ir_res_meal(void)
{
    uint8_t temp = 0;
    
    switch(type_spe.transformer_type)
    {
        default:
        case TRANS_750V:
        case TRANS_1kV:
        case TRANS_2kV:
            temp = 0;
            break;
        case TRANS_10kV:
            switch(type_spe.ir_vol_range)
            {
                case IR_1kV:
                case IR_1_5kV:
                    if(type_spe.ir_res_h <= 10*1000)
                    {
                        temp = 0;
                    }
                    else
                    {
                        temp = 1;
                    }
                    break;
                case IR_2_5kV:
                    temp = 2;
                    break;
                case IR_5kV:
                    temp = 2;
                    break;
                case IR_10kV:
                    temp = 3;
                    break;
            }
            break;
        case TRANS_20kV:
            break;
        case TRANS_5kV:
            switch(type_spe.ir_vol_range)
            {
                case IR_1kV:
                case IR_1_5kV:
                    if(type_spe.ir_res_h <= 10*1000) //单位Gohm
                    {
                        temp = 0;
                    }
                    else
                    {
                        temp = 1;
                    }
                    break;
                case IR_2_5kV:
                    temp = 2;
                    break;
                case IR_5kV:
                    temp = 2;
                    break;
            }
            break;
    }
    
    /* 调试使用 */
    if(g_cur_type->num == DEBUG_TYPE)
    {
        temp = 4;
    }
    
    return temp;
}

uint8_t get_cal_dcw_vol_meal(void)
{
    return type_spe.dcw_vol_range;
}
uint8_t get_cal_acw_vol_meal(void)
{
    return type_spe.acw_vol_range;
}
void get_acw_vol_cal_point_range(uint32_t *range)
{
	uint32_t i = 0;
	uint32_t b = 0;
	
	if(type_spe.vol_is_segment == VOL_SEGMENT_YES)
	{
		for(i = 0; i < 3; i++)
		{
			range[i] = acw_vol_cal[type_spe.amp_type][0].point_da[type_spe.acw_vol_range][b + i][1];
		}
		
		b = 3;
	}
	
	for(i = 0; i < 3; i++)
	{
		range[b + i] = acw_vol_cal[type_spe.amp_type][1].point_da[type_spe.acw_vol_range][i][1];
	}
}
void get_dcw_vol_cal_point_range(uint32_t *range)
{
	uint32_t i = 0;
	uint32_t b = 0;
	
	if(type_spe.vol_is_segment == VOL_SEGMENT_YES)
	{
		for(i = 0; i < 3; i++)
		{
			range[i] = dcw_vol_cal[type_spe.amp_type][0].point_da[type_spe.dcw_vol_range][b + i][1];
		}
		
		b = 3;
	}
	
	for(i = 0; i < 3; i++)
	{
		range[b + i] = dcw_vol_cal[type_spe.amp_type][1].point_da[type_spe.dcw_vol_range][i][1];
	}
}
void get_ir_vol_cal_point_range(uint32_t *range)
{
	uint32_t i = 0;
	uint32_t b = 0;
	
    cur_set_meal = get_cal_ir_meal();//电压
	
	if(type_spe.vol_is_segment == VOL_SEGMENT_YES)
	{
		for(i = 0; i < 3; i++)
		{
			range[i] = ir_vol_cal[type_spe.amp_type][0].point_da[cur_set_meal][b + i][1];
		}
		
		b = 3;
	}
	
	for(i = 0; i < 3; i++)
	{
		range[b + i] = ir_vol_cal[type_spe.amp_type][1].point_da[cur_set_meal][i][1];
	}
}

uint8_t get_cal_ir_meal(void)
{
    uint8_t temp = 0;
    
    if(type_spe.transformer_type == TRANS_10kV)
    {
        temp = IR_10kV;
    }
    else
    {
		switch(type_spe.transformer_type)
		{
			case TRANS_5kV:
				switch(type_spe.ir_vol_range)
				{
					case IR_2_5kV:
					case IR_5kV:
						temp = type_spe.ir_vol_range;
						break;
					default:
						temp = IR_1kV;
						break;
				}
				break;
			default:
				temp = type_spe.ir_vol_range;
				break;
		}
    }
    
    return temp;
}

void correct_ir(int8_t *pm)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t n = 0;
	int8_t m = 1;
    int8_t opt = sizeof ir_cal_opt_info / sizeof ir_cal_opt_info[0];
	
    cur_set_meal = get_cal_ir_meal();//电压
    
    cur_cal_ir_res_meal = get_ir_res_meal();//电阻
    
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_ir();
			
			draw_short_cut_bm(UD_B);
		}
		
		GUI_ClearRect(0, 15, 12, 64-1);
		GUI_ClearRect(115, 15, 115+12, 64-1);
        GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0 + 115 * (n / 4), 15 + 2 + 12 * (n % 4));
		
		ReFresh_LCD24064();
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_UP:
				if(--n < 0)
				{
					n = opt - 1;
				}
				break;
			case KEY_DOWN:
				if(++n >= opt)
				{
					n = 0;
				}
				break;
			case KEY_ENTER:
                if(NULL != ir_cal_opt_info[n].fun)
                {
                    ir_cal_opt_info[n].fun(n);
                }
                
				m = 1;/* 重绘背景 */
				break;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			case KEY_1:
				*pm = ACW;
				return;
			case KEY_2:
				*pm = DCW;
				return;
			case KEY_3:
				*pm = IR;
				return;
			case KEY_4:
				*pm = GR;
				return;
			case KEY_5:
				*pm = BBD;
				return;
		}
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}

/*++++++++++++++++++++++++++++++++++++++++++*/
////////////////////////////////////////
// 		  *****         *******       //
// 		 **  **          **   **      //
// 		 **  **          **   **      //
// 		**               **   **      //
// 		**               ******       //
// 		**               ** **        //
// 		**  ****         ** **        //
// 		**   **          **  **       //
// 		 **  **          **  **       //
// 		 **  **          **   **      //
// 		  ****          ****  ***     //
////////////////////////////////////////
void start_correct_gr_cur(const uint16_t times)
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
// 	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	
	/* 选取的是100mΩ 电阻作为校准点 */
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_vol = 0;
	int8_t temp = 0;
	int16_t t = 0;
	int8_t n = times;
	int32_t i = 0;
    static uint32_t x;
	
	float cal_point_res = 1.0;
	
	static float buf[3] = {0.0};
	static uint16_t ad_cur[3] = {0};
	static uint16_t ad_vol[3] = {0};
	uint16_t temp_cur_ad = 0;
	uint16_t temp_vol_ad = 0;
    uint8_t temp_delay = 0;
	
	DATA_INFO vol_info =
	{
		(void*)0/*pData*/,(X0+X1)/2+10/*x*/,Y1-22/*y*/,1/*type*/,5/*lon*/,3/*decs*/,0/*total*/,1/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
    
    cal_point_res = gr_cur_cal[cur_segment].point_da[cur_set_meal][times][1];
    
	draw_short_cut_bm(UDR_);
	GUI_DispStringAt((const char*)"重校", 210, 50);
	ReFresh_LCD24064();
	
	pmenu_data = &vol_info;	/* 将pmenu_data指向原始密码的信息 */
	pmenu_data->pData = &temp_vol;
	pmenu_data->ch_bit = 0;
	cursor_pos(pmenu_data->x, pmenu_data->y);
	
	/* 2. 输出DA值 */
	if(DC_GR_EN)
	{
		app_flag.dc_gr_module_status = DC_GR_RUNING;
		cur_vol = gr_cur_cal[cur_segment].point_da[cur_set_meal][times][0];
        set_dc_gr_output_da(get_cur_cal_dc_module_addr(), cur_vol);
        if(GR_VOL_DROP_EN > 0)
        {
            /* 设置电压档位，测试方法 */
            set_dc_gr_vol_gear(cur_gear);
            set_dc_gr_test_method(cur_method);
        }
        
        send_dc_module_cmd(DC_MODULE_CMD_SET_DATA, 0);
	}
	/* 交流 */
	else
	{
		DAC_Vol[0] = gr_cur_cal[cur_segment].point_da[cur_set_meal][times][0];
	}
    OSTimeDlyHMSM(0,0,0,100);
	startup_cal(GR, cur_gear);
    OSTimeDlyHMSM(0,0,0,100);
	
	while(1)
	{
		if(DC_GR_EN)
		{
            OSTimeDlyHMSM(0,0,0,100);
            
			if(test_flag.uart_next == 0)
			{
				test_flag.uart_next = 1;
                x = 0;
                
                send_dc_module_cmd(DC_MODULE_CMD_FETCH, 0);/* 获取测试数据 */
			}
            
            if(0 == get_dc_gr_ad(get_cur_cal_dc_module_addr(), &temp_cur_ad, &temp_vol_ad))
            {
                test_flag.uart_next = 0;
                sample_cur = temp_cur_ad;/* 采集的电流值 */
                sample_vol = temp_vol_ad;/* 采集电压值 */
                
                if(ERR_OPEN == get_dc_gr_open_err())
                {
                    BUZZER = BUZZER_ON;
                    stop_cal(GR);
                    GUI_DispStringAt((const char*)"OPEN", (X0 + X1) / 2 + 10 + 10, Y1 - 22 + 10);
                    
                    ReFresh_LCD24064();
                    
                    while(1)
                    {
                        if(0 == STOP_PIN)
                        {
                            break;
                        }
                    }
                    
                    ReFresh_LCD24064();
                    BUZZER = BUZZER_OFF;
                    return;
                }
            }
            
            if(++x > 100)
            {
                x = 0;
            }
		}
        
        g_test_data.cur_ad_vol = sample_vol * 3.3 / 4095;
        
        if(++temp_delay > 5)
        {
            temp_delay = 0;
//             dis_correct_ir_vol();
//             GUI_DispDecAt(cur_gear, (X0 + X1) / 2 + 10, Y1 - 22 + 10, 1);
            
            GUI_GotoXY((X0 + X1) / 2 + 10 + 10, Y1 - 22 + 10);
            GUI_DispFloatFix(g_test_data.cur_ad_vol, 5, 3);
            ReFresh_LCD24064();
        }
        
		/* 按下复位键时就要返回 */
		if(0 == STOP_PIN)
		{
			if(DC_GR_EN)
			{
				if (1)
				{
					buf[n] = temp_vol;
                    ad_cur[n] = sample_cur;
                    ad_vol[n] = sample_vol;
				}
			}
			else
			{
				buf[n] = temp_vol;
				ad_cur[n] = sample_cur;
				ad_vol[n] = sample_vol;
			}
			
			for(i = 0; i < 3; i++)
			{
				if(buf[i] <= 0)
				{
					goto l;
				}
			}
			
			// 数据系数曲线拟合算法
			if(n == 2)
			{
				u8 i=0,n=3;
				float sumx=0.0,sumy=0.0,sumz=0.0,sumxx=0.0,sumzz=0.0,sumxy=0.0,sumxz=0.0;
				
				for(i=0;i<n;i++)
				{
					sumx 	+= buf[i];
					sumy 	+= gr_cur_cal[cur_segment].point_da[cur_set_meal][i][0];
					sumz	+= ad_vol[i];
					sumxx	+= buf[i] * buf[i];
					sumzz	+= ad_vol[i] * ad_vol[i];
					sumxy	+= buf[i] * gr_cur_cal[cur_segment].point_da[cur_set_meal][i][0];
					sumxz	+= buf[i] * ad_vol[i];
				}
				
                if(DC_GR_EN)
                {
                    uint8_t index = 0;
                    
                    if(-1 == find_dc_gr_map_ratio_n(get_cur_cal_dc_module_addr(), &index))
                    {
                        return;
                    }
                    
                    ratio_dc_gr[0][index].dac_k = ((float)sumxy * n - sumx * sumy) / (n * sumxx - sumx * sumx);
                    ratio_dc_gr[0][index].dac_b = ((float)sumy - ratio_dc_gr[0][index].dac_k * sumx) / n;
                    
                    ratio_dc_gr[0][index].adc_v_k	= ((float)sumxz * n - sumx * sumz) / (n * sumzz - sumz * sumz);
                    ratio_dc_gr[0][index].adc_v_b	= ((float)sumx - ratio_dc_gr[0][index].adc_v_k * sumz) / n;
                }
                else
                {
                    ratio_gr.dac_k[cur_gear] = ((float)sumxy * n - sumx * sumy) / (n * sumxx - sumx * sumx);
                    ratio_gr.dac_b[cur_gear] = ((float)sumy - ratio_gr.dac_k[cur_gear] * sumx) / n;
                    if(cur_gear == 5)
                    {
                        ratio_gr.dac_k[cur_gear] *= 10;
                    }
                    
                    ratio_gr.adc_v_k[cur_gear]	= ((float)sumxz * n - sumx * sumz) / (n * sumzz - sumz * sumz);
                    ratio_gr.adc_v_b[cur_gear]	= ((float)sumx - ratio_gr.adc_v_k[cur_gear] * sumz) / n;
                }
				
                sumx=0.0,sumy=0.0,sumz=0.0,sumxx=0.0,sumzz=0.0,sumxy=0.0,sumxz=0.0;
				for(i=0;i<n;i++)
				{
					sumx 	+= (buf[i] / cal_point_res);
					sumz	+= ad_cur[i];
					sumxx	+= (buf[i]/cal_point_res * buf[i] / cal_point_res);
					
					sumzz	+= (ad_cur[i] * ad_cur[i]);
					sumxz	+= (buf[i] / cal_point_res * ad_cur[i]);
				}
				
                if(DC_GR_EN)
                {
                    uint8_t index = 0;
                    
                    if(-1 == find_dc_gr_map_ratio_n(get_cur_cal_dc_module_addr(), &index))
                    {
                        return;
                    }
                    
                    ratio_dc_gr[0][index].adc_i_k = ((float)sumxz * n- sumx * sumz)/(n * sumzz - sumz * sumz);
                    ratio_dc_gr[0][index].adc_i_b = ((float)sumx - ratio_dc_gr[0][index].adc_i_k * sumz) / n;
                }
                else
                {
                    ratio_gr.adc_i_k[cur_gear] = ((float)sumxz * n- sumx * sumz)/(n * sumzz - sumz * sumz);
                    ratio_gr.adc_i_b[cur_gear] = ((float)sumx - ratio_gr.adc_i_k[cur_gear] * sumz) / n;
                }
                
                if(DC_GR_EN)
                {
                    uint8_t index = 0;
                    
                    if(-1 == find_dc_gr_map_ratio_n(get_cur_cal_dc_module_addr(), &index))
                    {
                        return;
                    }
                    
                    CRC_CalcBlockCRC((uint32_t *)&ratio_dc_gr[index], sizeof(ratio_dc_gr[0]) / 4 - 1);
                    send_dc_module_cmd(DC_MODULE_CMD_SET_CAL_DATA, 0);/* 获取测试数据 */
                }
                else
                {
                    save_ratio(GR);
                }
			}
		l:	
			stop_cal(GR);
			temp_vol = 0;
			pmenu_data->ch_bit = 0;
			update_data(pmenu_data);
			cursor_pos(-5, 0);/* 将光标移出屏幕 */
			return;
		}
		
		CALIB_JUDGE_ERR(GR);	/* 检测开路错误 */
		
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				
				t = temp_vol/t;
				temp = t%10;
				
				if(++temp > 9)
				{
					temp = 0;
				}
				
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_DOWN:
				if(pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				
				t = temp_vol/t;
				temp = t%10;
				
				if(--temp < 0)
				{
					temp = 9;
				}
				
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit * 6, pmenu_data->y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_F4:
				temp_vol = 0;
				pmenu_data->ch_bit = 0;
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x, pmenu_data->y);
				break;
			default:
				break;
		}
	}
}

void correct_gr_out_cur(const uint8_t times)
{
	const uint8_t X0	=40;
	const uint8_t X1	=160;
	const uint8_t Y0	=15;
	const uint8_t Y1	=63;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t m = 1;
	uint8_t n = 1;//pf->work_mode;
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_FillRect(X0+2, Y0+2, (X0+X1)/2-2, Y0+15);
	GUI_FillRect((X0+X1)/2+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"校准电流", X0+6, Y0+3);
	GUI_DispStringAt((const char*)"实际电压", (X0+X1)/2+6, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	GUI_DispStringAt((const char*)"0.000V", (X0+X1)/2+10, Y1-22);
	
    GUI_DispStringAt_BTP_STR(gr_cur_cal[cur_segment].point_menu[cur_set_meal][times][0], X0, (X0+X1)/2, Y1-22);
//     GUI_DispStringAt((const char*), X0+10+6*4, );
	
	while(1)
	{
		if(m)
		{
			m = 0;
			clear_menu();
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
			ReFresh_LCD24064();
		}
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		
		switch(*pkey_value)
		{
			case KEY_SHIFT:
				if(++n > 1)
				{
					n = 0;
				}
				break;
			case KEY_ENTER:
				return;
			case KEY_BACK:
				GUI_ClearRect (X0, Y0, X1, Y1);
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			case KEY_START:
				start_correct_gr_cur(times);
				m = 1;
				break;
			default:
				break;
		}
		
		if(g_return)
		{
			g_return = 0;
		}
		
		if(g_exit)
		{
			return;
		}
	}
}
void serve_correct_gr_out_cur(uint32_t opt)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t m = 1;
	uint32_t i = 0;
	
	while(1)
	{
		if(m)
		{
			m = 0;
			GUI_ClearRect(0,15,200-1,64-1);
			dis_contens_gr();
			
			clear_menu();
            for(i = 0; i < 3; i++)
            {
                GUI_DispStringAt_BTP_STR(gr_cur_cal[cur_segment].point_menu[cur_set_meal][i][0], 200, 240, 2 + 16 * i);
            }
			
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
			ReFresh_LCD24064();
		}
        
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
				correct_gr_out_cur(0);
				break;
			case KEY_F2:
				correct_gr_out_cur(1);
				break;
			case KEY_F3:
				correct_gr_out_cur(2);
				break;
			case KEY_BACK:
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		
		m = 1;/* 重绘背景 */
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}
//////////////////////////////////////

uint8_t select_gr_cal_menu(void)
{
    uint8_t temp = 0;
    
    if(GR_VOL_DROP_EN > 0)
    {
        temp = 0;
        cur_method = GR_V_CONT_MODEL;/* 校准时开启连续测试模式 */
        send_dc_module_cmd(DC_MODULE_CMD_GEAR, cur_gear);
        send_dc_module_cmd(DC_MODULE_CMD_METHOD, cur_method);
    }
    else
    {
        /* 直流 GR */
        if(DC_GR_EN == ENABLE)
        {
            temp = 0;
        }
        /* 交流 GR */
        else
        {
            switch(type_spe.gr_amp_type)
            {
                default:
                case GR_AMP_32A:
                    temp = 0;
                    break;
                case GR_AMP_40A:
                    temp = 1;
                    break;
            }
        }
    }
    
    return temp;
}



void correct_gr(int8_t *pm)
{
	INT8U err = 0;
	uint16_t* pkey_value;
	int8_t n = 0;
	int8_t m = 1;
	int8_t opt = 0;
	MENU_INFO *pm_info = NULL;
    uint8_t cur_cal_mod_num = 0;/* 当前校准的模块编号 */
    
    /* 直流 */
    if(DC_GR_EN == ENABLE)
    {
        /* 压降模式 */
        if(GR_VOL_DROP_EN > 0)
        {
            opt = sizeof dv_cal_opt_info / sizeof dv_cal_opt_info[0];
            pm_info = dv_cal_opt_info;
        }
        else
        {
            opt = sizeof gr_dc_cal_opt_info / sizeof gr_dc_cal_opt_info[0];
            pm_info = gr_dc_cal_opt_info;
        }
    }
    /* 交流 */
    else
    {
        opt = sizeof gr_ac_cal_opt_info / sizeof gr_ac_cal_opt_info[0];
        pm_info = gr_ac_cal_opt_info;
    }
    
	while(1)
	{
		if(m)
		{
			m = 0;
            
			dis_contens_gr();
            
			draw_short_cut_bm(UDRB);
		}
		
		GUI_ClearRect(0,15,12,64-1);
		GUI_ClearRect(13+115-13, 15, 13+115, 64-1);
        
        GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0 + 115 * (n / 4), 17 + 12 * (n % 4));
        
		ReFresh_LCD24064();
		
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_UP:
				if(--n < 0)
				{
					n = opt - 1;
				}
				break;
			case KEY_DOWN:
				if(++n >= opt)
				{
					n = 0;
				}
				break;
			case KEY_ENTER:
            {
                int32_t res = 0;
                
                n = n % opt;
                
                if(pm_info[n].fun != NULL)
                {
                    cur_gear = pm_info[n].data;
                }
                
                /********************选择校准电流段*****************************/
                cur_segment = cur_gear;
                
                /**************************** 选择校准用的菜单 ***************************/
                cur_set_meal = select_gr_cal_menu();
                /*******************************************************************/
				
				/* 直流 */
				if(DC_GR_EN == ENABLE)
				{
					do
					{
						res = slelect_cal_dc_gr_module(&cur_cal_mod_num);
						if(-1 == res)
						{
							break;
						}
						else if(-2 == res)
						{
							break;
						}
						
						serve_correct_gr_out_cur(0);
						
					}while(g_exit==0);
				}
				else
				{
					serve_correct_gr_out_cur(0);
				}
                
				m = 1;/* 重绘背景 */
				break;
            }
			case KEY_BACK:
			case KEY_EXIT:
				g_exit = 1;
				return;
			case KEY_1:
				*pm = ACW;
				return;
			case KEY_2:
				*pm = DCW;
				return;
			case KEY_3:
				*pm = IR;
				return;
			case KEY_4:
				*pm = GR;
				return;
			case KEY_5:
				*pm = BBD;
				return;
		}
        
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}
/*++++++++++++++++++++++++++++++++++++++++++*/
// 			╭──╮╭──╮┌──╮
// 			│╭╮││╭╮││╭╮│
// 			│╰╯╯│╰╯╯││││
// 			│╭╮╮│╭╮╮││││
// 			│╰╯││╰╯││╰╯│
// 			╰──╯╰──╯└──╯
/*++++++++++++++++++++++++++++++++++++++++++*/

void start_correct_bbd_cur(const int16_t vol, const int8_t gear)
{
	const uint8_t X0	= 40;
	const uint8_t X1	= 160;
	const uint8_t Y0	= 15;
	const uint8_t Y1	= 63;
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_cur = 0;
	int8_t temp = 0;
	int16_t t = 0;
	
	static u16	advlue[CUR_KINDS_MAX] = {0};
	static float buf[CUR_KINDS_MAX] = {0.0};
	
	DATA_INFO cur_info =
	{
		(void*)0/*pData*/,(X0+X1)/2+10/*x*/,Y1-22/*y*/,1/*type*/,5/*lon*/,3/*decs*/,0/*total*/,1/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
	
	switch(gear)
	{
		case AC_2uA:
			cur_info.decs = 3;
			break;
		case AC_20uA:
			cur_info.decs = 2;
			break;
		case AC_200uA:
			cur_info.decs = 1;
			break;
		case AC_2mA:
			cur_info.decs = 3;
			break;
		case AC_10mA:
			cur_info.decs = 2;
			break;
		case AC_20mA:
			cur_info.decs = 2;
			break;
		case AC_50mA:
			cur_info.decs = 2;
			break;
		case AC_100mA:
			cur_info.decs = 1;
			break;
		case AC_200mA:
			cur_info.decs = 1;
			break;
		default:
			break;
	}
	
	pmenu_data = &cur_info;	/* 将pmenu_data指向原始密码的信息 */
	pmenu_data->pData = &temp_cur;
	pmenu_data->ch_bit = 0;
	cursor_pos(pmenu_data->x, pmenu_data->y);
	goto ll;
    
l:	
	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	ReFresh_LCD24064();
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		
		switch(*pkey_value)
		{
			case KEY_START:
				goto ll;
			case KEY_BACK:
			cursor_pos(-5, 0);/* 将光标移出屏幕 */
			g_return = 1;
			GUI_ClearRect(X0, Y0, X1, Y1);
				return;
			default:
				break;
		}
	}
    
ll:	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 16);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	GUI_DispStringAt((const char*)"重校", 210, 50);
	ReFresh_LCD24064();
	
    
    confirm_vol_segment();/* 确定电压段 */
    
	startup_cal(BBD, gear);
	
	while(1)
	{
		DAC_Vol[0] = vol * ratio_acw.dac_k[vol_segment] + ratio_acw.dac_b[vol_segment];
		
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		
		advlue[gear] = dis_cur(BBD, gear);/* 显示AD采样电流 */
		
		/* 按下复位键时就要返回 */
		if(0 == STOP_PIN)
		{
			save_ratio(BBD);
			
			DAC_Vol[0] = 0;
			
			stop_cal(BBD);
// 			cursor_pos(-5, 0);/* 将光标移出屏幕 */
// 			g_return = 1;
// 			GUI_ClearRect(X0, Y0, X1, Y1);
			goto l;
		}
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_cur/t;
				temp = t%10;
				
				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_DOWN:
				if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				}
				else
				{
					t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit - 1 - 1));
				}
				t = temp_cur/t;
				temp = t%10;

				if(--temp < 0)
				{
					temp = 9;
				}
				
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x + pmenu_data->ch_bit*6, pmenu_data->y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				break;
			case KEY_F4:/* 重校 */
				ratio_bbd.adc_cur_k[gear] = 1.0;
				temp_cur = 0;
				pmenu_data->ch_bit = 0;
				update_data(pmenu_data);
				cursor_pos(pmenu_data->x, pmenu_data->y);
				break;
			case KEY_ENTER:
				if(temp_cur == 0)
				{
					break;
				}
				buf[gear] = (float)temp_cur;
				ratio_bbd.adc_cur_k[gear] = ((float)buf[gear]) / advlue[gear];
				break;
			case KEY_START:
				break;
			default:
				break;
		}
	}
}
void correct_bbd_cur_gear(const uint8_t gear, const uint16_t vol)
{
	uint8_t X0	= 40;
	uint8_t X1	= 160;
	uint8_t Y0	= 15;
	uint8_t Y1	= 63;
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0, (X0+X1)/2, Y0+17);
	GUI_DrawLine((X0+X1)/2, Y0+17+5, (X0+X1)/2, Y1-5);
	
	GUI_FillRect(X0+2, Y0+2, (X0+X1)/2-2, Y0+15);
	GUI_FillRect((X0+X1)/2+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"测量电流", X0+6, Y0+3);
	GUI_DispStringAt((const char*)"实际电流", (X0+X1)/2+6, Y0+3);
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	switch(gear)
	{
		case AC_2uA:
			GUI_DispStringAt((const char*)"0.000~A", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"0.000~A", (X0+X1)/2+10, Y1-22);
			break;
		case AC_20uA:
			GUI_DispStringAt((const char*)"00.00~A", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"00.00~A", (X0+X1)/2+10, Y1-22);
			break;
		case AC_200uA:
			GUI_DispStringAt((const char*)"000.0~A", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"000.0~A", (X0+X1)/2+10, Y1-22);
			break;
		case AC_2mA:
			GUI_DispStringAt((const char*)"0.000mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"0.000mA", (X0+X1)/2+10, Y1-22);
			break;
		case AC_10mA:
			GUI_DispStringAt((const char*)"00.00mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"00.00mA", (X0+X1)/2+10, Y1-22);
			break;
		case AC_20mA:
			GUI_DispStringAt((const char*)"00.00mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"00.00mA", (X0+X1)/2+10, Y1-22);
			break;
		case AC_50mA:
			GUI_DispStringAt((const char*)"00.00mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"00.00mA", (X0+X1)/2+10, Y1-22);
			break;
		case AC_100mA:
			GUI_DispStringAt((const char*)"000.0mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"000.0mA", (X0+X1)/2+10, Y1-22);
			break;
		case AC_200mA:
			GUI_DispStringAt((const char*)"000.0mA", X0+10, Y1-22);
			GUI_DispStringAt((const char*)"000.0mA", (X0+X1)/2+10, Y1-22);
			break;
	}
	
	GUI_GotoXY(X0+10, Y1-22);
	
	start_correct_bbd_cur(vol, gear);
}
void cal_bbd_more_more_cur(const uint8_t **gear_buf, uint8_t cur_kinds, uint8_t *flag)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_vol;
	
	while(1)
	{
		dis_contens_bbd();
		clear_menu();
		GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
		
		switch(cur_kinds)
		{
			case 6:
				GUI_DispStringAt((const char*)gear_buf[4], 208, 2);
				GUI_DispStringAt((const char*)gear_buf[5], 208, 18);
				break;
			case 7:
				GUI_DispStringAt((const char*)gear_buf[4], 208, 2);
				GUI_DispStringAt((const char*)gear_buf[5], 208, 18);
				GUI_DispStringAt((const char*)gear_buf[6], 208, 34);
				break;
		}
		ReFresh_LCD24064();
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
				if(cur_kinds<6)
				{
					break;
				}
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					correct_bbd_cur_gear(flag[4], temp_vol);
				}
				break;
			case KEY_F2:
				if(cur_kinds<6)
				{
					break;
				}
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					correct_bbd_cur_gear(flag[5], temp_vol);
				}
				break;
			case KEY_F3:
				if(cur_kinds<6)
				{
					break;
				}
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					correct_bbd_cur_gear(flag[6], temp_vol);
				}
				break;
			case KEY_BACK:
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}
void cal_bbd_more_cur(const uint8_t **gear_buf, uint8_t cur_kinds, uint8_t *flag)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	uint16_t temp_vol;
	
	while(1)
	{
		dis_contens_bbd();
		clear_menu();
		GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
		if(cur_kinds > 5)
		{
			GUI_DispStringAt((const char*)gear_buf[2], 208, 2);
			GUI_DispStringAt((const char*)gear_buf[3], 208, 18);
			GUI_DispStringAt((const char*)"更多", 208, 34);
		}
		else
		{
			switch(cur_kinds)
			{
				case 4:
					GUI_DispStringAt((const char*)gear_buf[2], 208, 2);
					GUI_DispStringAt((const char*)gear_buf[3], 208, 18);
					break;
				case 5:
					GUI_DispStringAt((const char*)gear_buf[2], 208, 2);
					GUI_DispStringAt((const char*)gear_buf[3], 208, 18);
					GUI_DispStringAt((const char*)gear_buf[4], 208, 34);
					break;
			}
		}
		ReFresh_LCD24064();
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					if(cur_kinds>=4)
						correct_bbd_cur_gear(flag[2], temp_vol);
				}
				break;
			case KEY_F2:
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					if(cur_kinds>=4)
						correct_bbd_cur_gear(flag[3], temp_vol);
				}
				break;
			case KEY_F3:
				if(cur_kinds>5)
				{
					cal_bbd_more_more_cur(gear_buf, cur_kinds, flag);
				}
				else
				{
					temp_vol = set_out_vol();
					if(g_exit)
					{
						return;
					}
					
					if(!g_return)
					{
						if(cur_kinds>=4)
							correct_bbd_cur_gear(flag[4], temp_vol);
					}
				}
				break;
			case KEY_BACK:
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}
void serve_correct_bbd_cur_mA(uint32_t opt)/* 测量电流 */
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t m = 1;
	uint16_t temp_vol = 0;
	
	uint32_t cur_kinds = 0;/* 电流的种类 */
	const uint8_t *gear_buf[CUR_KINDS_MAX] = {0};/* 电流档位 */
	uint8_t flag[CUR_KINDS_MAX] = {0};/* 标志 */
	int32_t i = 0;
	
	cur_kinds = define_cur_kinds(BBD, gear_buf, flag);
	
	if(strcmp((const char*)(const char*)gear_buf[0], "200~A") == 0)
	{
		cur_kinds -= 1;
		for(i = 0; i < cur_kinds; i++)
		{
			gear_buf[i] = gear_buf[i+1];
			flag[i] = flag[i+1];
		}
        gear_buf[i] = "";
        flag[i] = 0;
	}
	
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_bbd();
			
			clear_menu();
			
			if(cur_kinds <= 3)
			{
				switch(cur_kinds)
				{
					case 1:
						GUI_DispStringAt((const char*)gear_buf[0], 208, 2);
						break;
					case 2:
						GUI_DispStringAt((const char*)gear_buf[0], 208, 2);
						GUI_DispStringAt((const char*)gear_buf[1], 208, 18);
						break;
					case 3:
						GUI_DispStringAt((const char*)gear_buf[0], 208, 2);
						GUI_DispStringAt((const char*)gear_buf[1], 208, 18);
						GUI_DispStringAt((const char*)gear_buf[2], 208, 34);
						break;
				}
			}
			else
			{
				GUI_DispStringAt((const char*)gear_buf[0], 208, 2);
				GUI_DispStringAt((const char*)gear_buf[1], 208, 18);
				GUI_DispStringAt((const char*)"更多", 208, 34);
			}
            
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
			ReFresh_LCD24064();
		}
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					if(cur_kinds>=1)
						correct_bbd_cur_gear(flag[0], temp_vol);
				}
				
				m = 1;/* 重绘背景 */
				break;
			case KEY_F2:
				if(cur_kinds <= 1)
				{
                    break;
                }
                
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					if(cur_kinds>=2)
						correct_bbd_cur_gear(flag[1], temp_vol);
				}
				
				m = 1;/* 重绘背景 */
				break;
			case KEY_F3:
				if(cur_kinds <= 2)
				{
                    break;
                }
                
				if(cur_kinds <= 3)
				{
					temp_vol = set_out_vol();
					if(g_exit)
					{
						return;
					}
					
					if(!g_return)
					{
						if(cur_kinds>=3)
							correct_bbd_cur_gear(flag[2], temp_vol);
					}
					
					m = 1;/* 重绘背景 */
				}
				/* 更多 */
				else
				{
					cal_bbd_more_cur(gear_buf, cur_kinds, flag);
				}
				m = 1;/* 重绘背景 */
				break;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			default:
				break;
		}
		if(g_return)
		{
			g_return = 0;
			m = 1;
		}
		if(g_exit)
		{
			return;
		}
	}
}
void serve_correct_bbd_cur_uA(uint32_t opt)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int8_t m = 1;
	uint16_t temp_vol = 0;
	
	while(1)
	{
		if(m)
		{
			m = 0;
			dis_contens_bbd();
			
			clear_menu();
			GUI_DispStringAt((const char*)"200~A", 208, 2);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
			ReFresh_LCD24064();
		}
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_F1:
				temp_vol = set_out_vol();
				if(g_exit)
				{
					return;
				}
				
				if(!g_return)
				{
					correct_bbd_cur_gear(AC_200uA, temp_vol);
				}
				
				m = 1;/* 重绘背景 */
				break;
			case KEY_F2:
// 				temp_vol = set_out_vol();
// 				if(g_exit)
// 				{
// 					return;
// 				}
// 				
// 				if(!g_return)
// 				{
// 					correct_acw_cur_gear(AC_20uA, temp_vol);
// 				}
// 				
				m = 1;/* 重绘背景 */
				break;
			case KEY_F3:
// 				temp_vol = set_out_vol();
// 				if(g_exit)
// 				{
// 					return;
// 				}
// 				
// 				if(!g_return)
// 				{
// 					correct_acw_cur_gear(AC_200uA, temp_vol);
// 				}
// 				
// 				m = 1;/* 重绘背景 */
				break;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}

void correct_bbd(int8_t *pm)
{
	INT8U err = 0;
	uint16_t* pkey_value;
	int8_t n = 0;
	int8_t m = 1;
	int8_t opt = 0;
// 	MENU_INFO *pm_info = NULL;
//     uint8_t cur_cal_mod_num = 0;/* 当前校准的模块编号 */
    
	opt = sizeof bbd_cal_opt_info / sizeof bbd_cal_opt_info[0];
// 	pm_info = bbd_cal_opt_info;
    
	while(1)
	{
		if(m)
		{
			m = 0;
            
			dis_contens_bbd();
            
			draw_short_cut_bm(UDRB);
		}
		
		GUI_ClearRect(0,15,12,64-1);
		GUI_ClearRect(13+115-13, 15, 13+115, 64-1);
        
        GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0 + 115 * (n / 4), 17 + 12 * (n % 4));
        
		ReFresh_LCD24064();
		
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		switch(*pkey_value)
		{
			case KEY_UP:
				if(--n < 0)
				{
					n = opt - 1;
				}
				break;
			case KEY_DOWN:
				if(++n >= opt)
				{
					n = 0;
				}
				break;
			case KEY_ENTER:
            {
                if(NULL !=bbd_cal_opt_info[n].fun)
                {
                    bbd_cal_opt_info[n].fun(n);
                }
                
				m = 1;/* 重绘背景 */
                draw_short_cut_bm(UDRB);
				break;
            }
			case KEY_BACK:
			case KEY_EXIT:
				g_exit = 1;
				return;
			case KEY_1:
				*pm = ACW;
				return;
			case KEY_2:
				*pm = DCW;
				return;
			case KEY_3:
				*pm = IR;
				return;
			case KEY_4:
				*pm = GR;
				return;
			case KEY_5:
				*pm = BBD;
				return;
		}
        
		if(g_return)
		{
			g_return = 0;
		}
		if(g_exit)
		{
			return;
		}
	}
}
/*
 * 函数名：correctness
 * 描述  ：校准参数
 * 输入  ：无
 * 输出  ：无
 */
void correctness(void)
{
	int8_t m = 1;
	int8_t n = m;
	
	Lcd24064_Clr();
	GUI_DrawLine         (0  , 12  , 200  , 12  );
	GUI_DrawLine         (200, 0   , 200  , 64-1);
	GUI_DrawLine         (200, 16-1, 240-1, 15  );
	GUI_DrawLine         (200, 32-1, 240-1, 32-1);
	GUI_DrawLine         (200, 48-1, 240-1, 48-1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+2);
	
	GUI_DispStringAt((const char*)"ACW"		, 8+10, 0);
	GUI_DispStringAt((const char*)"DCW"		, 8+10+35, 0);
	GUI_DispStringAt((const char*)"IR"		, 8+10+35+35, 0);
	GUI_DispStringAt((const char*)"GR"		, 8+10+35+35+30, 0);
	GUI_DispStringAt((const char*)"BBD"		, 8+10+35+35+30+30, 0);
	GUI_DispStringAt((const char*)"(1-5)"	, 8+10+35+35+30+30+20, 0);
	
	ReFresh_LCD24064();
	
	while(1)
	{
		if(m != n)
		{
			n = m;
			GUI_ClearRect(0,15,200-1,64-1);
		}
		
		switch(m)
		{
			case ACW:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_   , 8, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35+35, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35+35+30, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35+35+30+30, 2 );
				cursor_pos(8+1,2-3);
				correct_acw(&m);
				break;
			case DCW:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_   , 8+35, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35+35, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35+35+30, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35+35+30+30, 2 );
				cursor_pos(8+35+1,2-3);
				correct_dcw(&m);
				break;
			case IR:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_   , 8+35+35, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35+35+30, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35+35+30+30, 2 );
				cursor_pos(8+35+35+1,2-3);
				correct_ir(&m);
				break;
			case GR:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35+35, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_   , 8+35+35+30, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35+35+30+30, 2 );
				cursor_pos(8+35+35+30+1,2-3);
				correct_gr(&m);
				break;
			case BBD:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35+35, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle    , 8+35+35+30, 2 );
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_	, 8+35+35+30+30, 2 );
				cursor_pos(8+35+35+30+1,2-3);
				correct_bbd(&m);
				break;
		}
		
		if(g_exit || g_return)
		{
			return;
		}
        
		ReFresh_LCD24064();
	}
}


/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
