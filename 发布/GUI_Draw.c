/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：GUI_Draw.c
 * 摘  要  ：包含服务界面的主要函数
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
 
 /********************* 包含头文件 ***********************/
#define	 MENU_GLOBAL


#include	<stdio.h>
#include	<string.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include    "GUI_Draw.h"
#include    "math.h"
#include    "mb_cmd_define.h"



/************************* 全局变量初始化 ******************/
const uint8_t *unit_pool[]=
{
	"","mV","V","kV","~A","mA","A","mΩ","Ω","MΩ","GΩ","pF","nF","~F",
};
const uint8_t *unit_pool_[]=
{
	"","mV","V","kV","uA","mA","A","mΩ","Ω","MΩ","GΩ","pF","nF","uF",
};
const uint8_t *mode_pool[][10]=
{
	{"", "ACW", "DCW", "IR ", "GR ", "BBD", "CC"},
	{"", "ACW", "DCW", "IR ", "DV ", "BBD", "CC"},
};

const uint8_t *opt_pool[]=
{
    "","电压", "电流", "电阻", "电容",
};

const char *work_mode_buf[]=
{
	"G", "N",
};//G_MODE

const char *offset_res_buf[4]=
{
	" ", "F", "P", "-"
};

const char* status_str[][2] = 
{
	{"等待测试", "TestWait"},
	{"电压上升", "  Rise  "},
	{"正在测试", "Testing "},
	{"电压下降", "  Fall  "},
	{"电压缓变", "Change  "},
	{"间隔等待", "Interval"},
	{"测试合格", "  PASS  "},
	{"测试失败", "TestFail"},
	{"正在充电", " Charge "},
	{"稳压时间", "Sta.Time"},
	{"正在放电", "Discharg"},
	{"放电结束", "Disch.OK"},
	{"输出延时", "Out.Dly"},
};

const char* except_buf[][3]=
{
	{"","",""},
	{"上限报警", "High Fail", "High"   },
	{"下限报警", "Low Fail" , "Low"    },
	{"真实报警", "Real Fail", "Real"   },
	{"充电报警", "Char Fail", "Charge" },
	{"开路报警", "Open Fail", "Open"   },
	{"短路报警", "ShortFail", "Short"  },
	{"ARC报警" , "ARC Fail" , "ARC"    },
	{"GFI报警" , "GFI Fail" , "GFI"    },
	{"功放报警", "AMP Fail" , "AMP"    },
	{"档位报警", "GEAR Fail", "GEAR"   },
	{"电压异常", "VOL. Fail", "VOL."   },
	{"未定义"  , "Undefine ", "Undef." },
};
const char* sys_sw_status_buf[][2] =
{
	{"GFI:DISABLE"	,"GFI:ENABLE "},
	{"ARC:DISABLE" 	,"ARC:ENABLE "},
	{" HV:ZERO"		," HV:HIGH   "},
};
/* IR显示电阻时换档时使用的 */
const char* res_infinite_str[]=
{
    "-----","-.---","--.--","---.-","-.---","--.--","---.-"
};

MORE_OPTIONS_STR cc_cur_gear_option_pool[]=
{
    {{"高", "High"},PX,0,CC_CUR_HIGH,NULL,NULL},
    {{"中", "Mid."},PX,0,CC_CUR_MID ,NULL,NULL},
    {{"低", "Low "},PX,0,CC_CUR_LOW ,NULL,NULL},
};

MORE_OPTIONS_STR ir_filtering_deep_option_pool[]=
{
    {{"快", "Fast"},PX,0,IR_FILTERING_DEEP_FAST,NULL,NULL},
    {{"中", "Mid."},PX,0,IR_FILTERING_DEEP_MID ,NULL,NULL},
    {{"慢", "slow "},PX,0,IR_FILTERING_DEEP_SLOW ,NULL,NULL},
};

MORE_OPTIONS_STR step_purpose_option_pool[]=
{
    {{"正常", "Norm."},PX,0,STEP_PURPOSE_NORMAL,NULL,NULL},
    {{"扫描", "Scan"  },PX,0,STEP_PURPOSE_SCAN  ,NULL,NULL},
};

MORE_SEL_STR mor_sel_menu[]=
{
    {0, PX/*x*/,0/*y*/,{0, 50, 50 + 40, 0, 50, 50 + 40, 0}},/* ACW电流档位 */
    {0, PX/*x*/,0/*y*/,{0, 35, 40 + 35, 0, 35, 40 + 35, 0}},/* DCW电流档位 */
    {0, PX/*x*/,0/*y*/,{0, 35, 40 + 35, 0, 35, 40 + 35, 0}},/* GR电流档位 */
    {0, PX/*x*/,0/*y*/,{0, 28, 28 + 28, 28 + 28 + 23, (28 + 28 + 23 + 23), 0}},/* 模式设置 */
    {0, SX/*x*/,0/*y*/,{0, SOX, 0, SOX, 0, SOX, 0, SOX, 0}},/* 系统失败模式设置 */
};

const uint8_t* fail_mode_pool[]=
{
    "STOP"      ,
    "HALT"      ,
    "CON."      ,
    "RESTART"   ,
    "NEXT"      ,
    "FPDFC"     ,
	"SCAN"		,
};

MORE_OPTIONS_STR plc_signal_option_pool[]=
{
    {{"单步", "EachStep"},SX,0,EACH_STEP,NULL,NULL},
    {{"总步", "AllSteps"},SX,0,ALL_STEP ,NULL,NULL},
    {{"保持", "Hold"    },SX,0,TEST_HOLD,NULL,NULL},
};


MORE_OPTIONS_STR amp_type_option_pool[]=
{
    {{"8833", "8833"} ,SX,0,AMP_8833,NULL,NULL},
    {{"Old", "Old"  } ,SX,0,AMP_OLD ,NULL,NULL},
};
MORE_OPTIONS_STR par_medium_option_pool[]=
{
    {{"eep", "eep"      } ,SX,0, PAR_MEDIUM_EEP,NULL,NULL},
    {{"flash", "flash"  } ,SX,0, PAR_MEDIUM_FLASH ,NULL,NULL},
};
MORE_OPTIONS_STR res_medium_option_pool[]=
{
    {{"eep", "eep"      } ,SX,0, RES_MEDIUM_EEP,NULL,NULL},
    {{"flash", "flash"  } ,SX,0, RES_MEDIUM_FLASH ,NULL,NULL},
};
MORE_OPTIONS_STR cal_medium_option_pool[]=
{
    {{"eep", "eep"      } ,SX,0, CAL_MEDIUM_EEP,NULL,NULL},
    {{"flash", "flash"  } ,SX,0, CAL_MEDIUM_FLASH ,NULL,NULL},
};


const OUTPUT_IMPEDAN_OPTS_T output_impedan_opt[]=
{
    {
        {
            "0 - 0.00kΩ",
            "1 - 22.4kΩ",
            "2 - 7.60kΩ",
            "3 - 4.00kΩ",
        },
        4,
    },
    {
        {
            "0 - 4.40kΩ",
            "1 - 2.80kΩ",
            "2 - 1.10kΩ",
        },
        3,
    },
};

DATA_INFO menu_par[]=
{
	/************* COM ************/
	{// 1   测试步骤
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,4/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		1,0,{"测试步骤","Step"},{"","",""},set_step,0/*no_dis_bits*/,GUI_COM_STEP,UDSB,"99  ","01  ",
	},
	
	{// 2   测试模式
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		1,0,{"测试模式","Mode"},{"","ACW","DCW","IR","GR","BBD"},set_mode,0/*no_dis_bits*/,GUI_COM_MODE,UDRB,
	},
	
	{// 3   延时时间
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		9999,3,{"延时时间","Delay"},{"s",""},set_test_time,0/*no_dis_bits*/,GUI_COM_DELAY_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 4   上升时间
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,1/*bits*/,0,
		9999,3,{"上升时间","Rise"},{"s",""},set_test_time,0/*no_dis_bits*/,GUI_COM_RISE_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
    
	{// 5   稳压时间
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,3,{"稳压时间","Stabili."},{"s","",""},set_test_time,0/*no_dis_bits*/,GUI_COM_STAB_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 6   测试时间
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,3,{"测试时间","Test"},{"s","",""},set_test_time,0/*no_dis_bits*/,GUI_COM_TEST_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 7	下降时间
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,4/*page*/,1/*bits*/,0,
		9999,3,{"下降时间","Fall"},{"s",""},set_test_time,0/*no_dis_bits*/,GUI_COM_FALL_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 8	间隔时间 \ 缓变时间
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		9999,0,{"间隔时间","Interval"},{"s","",""},set_test_time,0/*no_dis_bits*/,GUI_COM_INTER_T,UDRB,"999.9s","000.0s",0,0,1/*rang_flag*/,
	},
	
	{// 9 步间PASS
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,EITHER_OR/*type*/,5/*lon*/,1/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,1,{"步间PASS","StepPass"},{"是","否","Yes","No"},set_either_or_par,0/*no_dis_bits*/,GUI_COM_STEPS_PASS,UDRB,"","",35,3,
	},
	
	{// 10 步间连续
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,EITHER_OR/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,1,{"步间连续","StepSer."},{"是","否","Yes","No"},set_either_or_par,0/*no_dis_bits*/,GUI_COM_STEPS_CONT,UDRB,"","",35,3,
	},
	
	{// 11 输出频率
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,1/*bits*/,0,
		0,10,{"输出频率","Freq."},{"","Hz",""},set_frequency,0/*no_dis_bits*/,GUI_COM_OUTPUT_FREQ,UDRB,"   9  ","   1  ",0,0,1/*rang_flag*/,
	},
	
	{// 12	测试端口
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,STRUCT_T/*type*/,0/*lon*/,0/*decs*/,8/*total*/,1/*cur*/,0/*ch_bit*/,6/*page*/,4/*bits*/,0,
		0,10,{"测试端口","Port"},{"",""},set_all_ports,0/*no_dis_bits*/,GUI_COM_PORT,UDRB,"","",40,3
	},
	
	{// 13	测试用途
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,8/*total*/,1/*cur*/,0/*ch_bit*/,6/*page*/,0/*bits*/,0,
		0,10,{"测试用途","Purpose"},{0},set_option_par,0/*no_dis_bits*/,GUI_COM_STEPS_PURPOSE,UDRB,"","",35,3
	},
    
	/************* ACW ************/
	{// 1 测试电压
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"输出电压","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_ACW_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 电流档位
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"电流档位","Range"},{"",""},set_ac_cur_gear,0/*no_dis_bits*/,GUI_ACW_CUR_GEAR,UDRB,
	},
	
	{// 3 电流上限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"电流上限","High"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_CUR_H,UDRB,
	},
	
	{// 4 电流下限
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"电流下限","Low"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_CUR_L,UDRB,
	},
	
	{// 5 真实电流
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"真实电流","Real"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_REAL_CUR,UDRB,
	},
	
	{// 6 电弧侦测
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"电弧侦测","Arc"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_ARC_SUR,UDRB,
	},
    
	/************* DCW ************/
	{// 1 测试电压
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"输出电压","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_DCW_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 电流档位
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"电流档位","Range"},{"",""},set_dcw_cur_gear,0/*no_dis_bits*/, GUI_DCW_CUR_GEAR,UDRB,
	},
	
	{// 3 电流上限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"电流上限","High"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_CUR_H,UDRB,
	},
	
	{// 4 电流下限
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"电流下限","Low"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_CUR_L,UDRB,
	},
	
	{// 6 充电电流
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"充电电流","Charge_I"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_CHARGE_CUR,UDRB,
	},
    
	{// 7 电弧侦测
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"电弧侦测","Arc"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_ARC_SUR,UDRB,
	},
    
	{// 8 输出阻抗
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		3,0,{"输出阻抗","Impedan"},{"","0 - 0.00kΩ","1 - 22.4kΩ","2 - 7.60kΩ","3 - 4.00kΩ"},set_float_par,0/*no_dis_bits*/, GUI_DCW_OUTPUT_IMPEDANCE,UDRB,"","",0,0,2/*rang_flag*/,
	},
    
	/************* IR ************/
	{// 1 测试电压
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"输出电压","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_IR_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 自动换档
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"自动换档","Auto.Lev"},{"是","否","YES","NO"},set_either_or_par,0/*no_dis_bits*/, GUI_IR_AUTO_GEAR,UDRB,"","",35,3,
	},
	
	{// 3 电阻上限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,2/*bits*/,0,
		9999,0,{"电阻上限","High"},{"MΩ","MΩ"},set_float_par,0/*no_dis_bits*/, GUI_IR_RES_H,UDRB,"09999MΩ","00000MΩ",
	},
	
	{// 4 电阻下限
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,0/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,2/*bits*/,0,
		9999,1,{"电阻下限","Low"},{"MΩ","MΩ"},set_float_par,0/*no_dis_bits*/, GUI_IR_RES_L,UDRB,"09999MΩ","00000MΩ",0,0,0/*rang_flag*/,
	},
    
	/************* GR ************/
	{// 1 电压档位
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,OPTION_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"电压档位","Gear"},{"","","20mV","20mV","200mV","200mV","2000mV""2000mV","12V","12V"},set_gr_vol_gear,0/*no_dis_bits*/,GUI_GR_VOL_GEAR,UDRB,
	},
	
	{// 2 输出电流
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		1,0,{"输出电流","Cur."},{"A","mA",""},set_gr_output_cur,1/*no_dis_bits*/,GUI_GR_OUTPUT_CUR,UDRB,
	},
	
	{// 3 电阻上限
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"电阻上限","High"},{"mΩ","Ω",""},set_gr_res_upper,0/*no_dis_bits*/,GUI_GR_RES_H,UDRB,"510.0mΩ","001.0mΩ",
	},
	
	{// 4 电阻下限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"电阻下限","Low"},{"mΩ","Ω",""},set_gr_res_lower,0/*no_dis_bits*/,GUI_GR_RES_L,UDRB,
	},
	
	{// 5 电压上限
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"电压上限","High"},{"mV","V",""},set_gr_vol_upper,0/*no_dis_bits*/,GUI_GR_VOL_H,UDRB,"00.00V","00.00V",
	},
	
	{// 6 电压下限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"电压下限","Low"},{"mV","v",""},set_gr_vol_lower,0/*no_dis_bits*/,GUI_GR_VOL_L,UDRB,"00.00V","00.00V",
	},
	
	{// 7 测试模式
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,OPTION_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,0/*bits*/,0,
		1,0,{"测试方法","Method"},{"连续","脉冲","Cont.","Pulse"},set_gr_test_method,0/*no_dis_bits*/,GUI_GR_TEST_METHOD,UDRB,"","",40,3
	},
    
	/************* BBD************/
	{// 1 测试电压
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"输出电压","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_BBD_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 开路检测
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,3/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		100,10,{"开路检测","Open"},{"%","",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_OPEN_CHECK,UDRB,"  100%","  10%",0,0,1/*rang_flag*/,
	},
	
	{// 3 短路检测
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,INT_T/*type*/,3/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		500,100,{"短路检测","Short"},{"%","v",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_SHORT_CHECK,UDRB," 500%","0,100%",0,0,1/*rang_flag*/,
	},
	
	{// 4 电容上限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		500,100,{"电容上限","High"},{"~F","",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_CAP_H,UDRB,"0.000~F","0.000~F",0,0,0/*rang_flag*/,
	},
	
	{// 5 电容下限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		500,100,{"电容下限","Low"},{"~F","",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_CAP_L,UDRB,"0.000~F","0.000~F",0,0,0/*rang_flag*/,
	},
	
	/************* CC ************/
	{// 1 测试电压
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"输出电压","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_CC_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 电流档位
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"电流档位","Range"},{"",""},set_ac_cur_gear,0/*no_dis_bits*/,GUI_CC_CUR_GEAR,UDRB,
	},
	
	{// 3 电流上限
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"电流上限","High"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_CC_CUR_H,UDRB,
	},
	
	{// 4 电流下限
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"电流下限","Low"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_CC_CUR_L,UDRB,
	},
	
	{// 5 真实电流
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"真实电流","Real"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_CC_REAL_CUR,UDRB,
	},
	
	{// 6 电流强度
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,OPTION_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"电流强度","CurInten."},{"中","Mid"},set_option_par,0/*no_dis_bits*/, GUI_CC_CUR_INTENSITY,UDRB,
	},
    
	/************* SYS ************/
	{//	1 液晶对比度
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		9,1,{"液晶对比度","Contrast"},{"",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_CONTRAST,UDRB,"(1-9)",""
	},
	
	{//	2 余量提示
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,2/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		9,0,{"余量提示","RemainHint"},{"",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_REMAIN_HINT,UDRB,"(0-9)",""
	},
	
	{//	3 结果保存
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		0,10,{"结果保存","ResultSave"},{"是","否","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_RES_SAVE,UDRB,"","",SOX,SOY,
	},
	
	{//	4 溢出覆盖
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		0,10,{"溢出覆盖","OverCover"},{"是","否","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_VOER_COVER,UDRB,"","",SOX,SOY,
	},
	
	{//	5 GFI保护
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"GFI 保护","GFI Prot."},{"是","否","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_GFI_PROT,UDRB,"","",SOX,SOY,
	},
	
	{//	6 自检允许
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"自检允许","SelfCheck"},{"是","否","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_SELF_CHECK,UDRB,"","",SOX,SOY,
	},
	
	{//	7 语言选择
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"语言选择","Language"},{"中文","英文","Chinese","English",},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_LANGUAGE,UDRB,"","",SOX,SOY,
	},
	
	{//	8 失败继续
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"失败继续","FailCont."},{"是","否","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_FAIL_CONT,UDRB,"","",SOX,SOY,
	},
	
	{//	9 列表显示
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"列表显示","ListDisplay"},{"是","否","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_LIST_DIS,UDRB,"","",SOX,SOY,
	},
	
	{//	10 测试端口
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"测试端口","TestPort"},{"接地","浮地","GND","Float"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_TEST_PORT,UDRB,"","",SOX,SOY,
	},
    
	{//	11 测试信号
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,3/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"测试信号","TestSignal"},{"电平","触点","Level","Point"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_TEST_SIGNAL,UDRB,"","",SOX,SOY,
	},
	
	{//	12 蜂鸣开关
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"蜂鸣开关","Buzzer SW."},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_BUZZER_SW,UDRB,"","",SOX,SOY,
	},
	
	{//	13  检验日期
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,STRING_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,3/*bits*/,0,
		0,10,{"检验日期","CheckDate"},{"2015-1-1 12:12:12",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_CHECK_DATE,UDRB,"","",SOX,SOY,
	},
	
	{//	14  编号规则
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,0/*bits*/,0,
		2,0,{"编号规则","NO.Rule"},{"",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_NO_RULE,UDRB,"(0-2)","",SOX,SOY,
	},
	
	{//	15 失败模式
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"失败模式","FailMode"},{"STOP","STOP","Yes","No"},set_option_sys_par,0/*no_dis_bits*/, GUI_SYS_FAIL_MODE,UDRB,"","",SOX,SOY,
	},
    
	{//	16 静音开关
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"静音开关","Silent SW."},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_SILENT_SW,UDRB,"","",SOX,SOY,
	},
    
	{//	17 PLC 信号
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"PLC 信号","PLC Signal"},{"单步","总步","EachStep","AllSteps"},set_plc_option_sys_par,0/*no_dis_bits*/, GUI_SYS_PLC_SIGNAL,UDRB,"","",SOX,SOY,
	},
    
	{//	18 输出延时
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,0,{"输出延时","Out.Delay"},{"s (0.0-999.9s)","s (0.0-999.9s)"},set_float_sys_par,0/*no_dis_bits*/, GUI_SYS_OUTPUT_DELAY,UDRB,"(0.0-999.9s)","",SOX,SOY,
	},
    
	{//	19 档位保持
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,200,{"IR档位保持","IRGearHold"},{"s (0.2-9.999s)","s (0.2-9.999s)"},set_float_sys_par,0/*no_dis_bits*/, GUI_SYS_IR_GEAR_HOLD,UDRB,"(0.2-9.999s)","",SOX,SOY,
	},
    
	{//	20 IR 测试速度
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,5/*lon*/,3/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,200,{"IR测试速度","IR SPEED"},{"",""},set_option_par,0/*no_dis_bits*/, GUI_SYS_IR_SPEED,UDRB,"(0.2-9.999s)","",SOX,SOY,
	},
    
	/************* CUSTOM(定制参数) ************/
	{//	1 功放选择
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"功放选择","8833AMP"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_AMP_SELECT,UDRB,"","",SOX,SOY,
	},
	{//	2 功放类型
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"功放类型","AMP_TYPE"},{"PWM","LINE","PWM","LINE"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_AMP_TYPE,UDRB,"","",SOX,SOY,
	},
	{//	3 参数存储介质
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"参数存储","PAR_MEDIUM"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_PAR_MEDIUM,UDRB,"","",SOX,SOY,
	},
	{//	4 结果存储介质
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"结果存储","RES_MEDIUM"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_RES_MEDIUM,UDRB,"","",SOX,SOY,
	},
	{//	5 校准存储介质
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"校准存储","CAL_MEDIUM"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_CAL_MEDIUM,UDRB,"","",SOX,SOY,
	},
	{//	6  蜂鸣器使能
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"蜂鸣器使能","BUZZER-EN"},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_BUZZER_EN,UDRB,"","",SOX,SOY,
	},
	{//	7  静音开关
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"静音开关","MUTE-SW"},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_MUTE_SW,UDRB,"","",SOX,SOY,
	},
	{//	8  导入导出
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"导入导出","LeadingSW"},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_LEADING_SW,UDRB,"","",SOX,SOY,
	},
	{//	9  换挡延时
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"IR换挡保持","IR_DELAY"},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_IR_G_HOLD_SW,UDRB,"","",SOX,SOY,
	},
	{//	10  IR测试速度
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"IR测试速度","IR_SPEED"},{"打开","关闭","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_IR_SPEED_SW,UDRB,"","",SOX,SOY,
	},
};


void more_options_sel_gui(uint8_t options, uint8_t n, MORE_SEL_STR *mor_sel_info);
/*************************** 函数定义 *************************/
uint8_t * mystrcat(uint8_t *buf, uint8_t *str1, uint8_t *str2)
{
    uint8_t t_buf1[20] = {0};
    uint8_t t_buf2[20] = {0};
    
    strcpy((char*)t_buf1, (const char*)str1);
    strcpy((char*)t_buf2, (const char*)str2);
    buf[0] = 0;
    
    strcat((char*)buf, (const char*)t_buf1);
    strcat((char*)buf, (const char*)t_buf2);
    
    return buf;
}

void front_add_space_at_string(uint8_t *buf, uint8_t c)
{
    uint8_t i = 0;
    
    for(i = 0; i < c; i++)
    {
        mystrcat((uint8_t*)buf, " ", (uint8_t*)buf);
    }
}

void init_test_gui_area_info(void)
{
//     test_gui_area[TEST_GUI_AREA_1].x = 10;
//     test_gui_area[TEST_GUI_AREA_1].y = 13;
//     test_gui_area[TEST_GUI_AREA_1].lon = 7;
//     test_gui_area[TEST_GUI_AREA_1].font = 7;
    
}
/*
 * 函数名：GUI_DispFloatFixAt
 * 描述  ：在指定坐标显示一个浮点数 并显示单位
 * 输入  ：float v, char Len, char Fract, I16P unit, I16P x , I16P y
 * 输出  ：无
 * 返回  ：无
 */
void GUI_DispFloatFixAt (float v, char Len, char Fract, I16P unit, I16P x , I16P y)
{
	GUI_GotoXY(x, y);
	GUI_DispFloatFix (v, Len, Fract);
	GUI_DispString((const char*)unit_pool[unit]);
}

/*
 * 函数名：draw_colon
 * 描述  ：在设置页面下方显示冒号
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void draw_colon(void)
{
#if 1
	if(pmenu_data->page != par_info_list.tail->page)
	{
		GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 80, 56);
	}
#else
	GUI_SetFont(&GUI_FontHZ_SimSun_11);
	GUI_DispDecAt(pmenu_data->page, 80, 54, 1);
	GUI_DispStringAt((const char*)"/", 80+6*1, 54);
	GUI_DispDecAt(par_info_list.tail->page, 80+6*2, 54, 1);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
#endif
}

/*
 * 函数名：save_par_to_array
 * 描述  ：将参数值更新到一个数组中
 * 输入  ：data_info 菜单项数据结构
 *         value 有更新的数据
 *         array_num 数组成员的个数
 * 输出  ：无
 * 返回  ：无
 */
void save_par_to_array(DATA_INFO *data_info, uint32_t value, uint8_t array_num)
{
    int32_t i = 0;
    
    if(data_info == NULL)
    {
        return;
    }
    
    switch(data_info->bits)
    {
        case GUI_U8_ARRAY:
            for(i = 0; i < array_num; i++)
            {
                *((uint8_t*)data_info->pData + i) = value;
                *((uint8_t*)data_info->pData + i) = value;
            }
            break;
        case GUI_U16_ARRAY:
            for(i = 0; i < array_num; i++)
            {
                *((uint16_t*)data_info->pData + i) = value;
                *((uint16_t*)data_info->pData + i) = value;
            }
            break;
        case GUI_U32_ARRAY:
            for(i = 0; i < array_num; i++)
            {
                *((uint32_t*)data_info->pData + i) = value;
                *((uint32_t*)data_info->pData + i) = value;
            }
            break;
    }
}
/*
 * 函数名：save_par_to_ram
 * 描述  ：将参数值更新的内存
 * 输入  ：data_info 菜单项数据结构
 *         value 有更新的数据
 * 输出  ：无
 * 返回  ：无
 */
void save_par_to_ram(DATA_INFO *data_info, uint32_t value)
{
    if(data_info == NULL)
    {
        return;
    }
    if(data_info->pData == NULL)
    {
        return;
    }
    
    /* 赋值 */
    switch(data_info->bits)
    {
        case GUI_U8:
            *(uint8_t*)data_info->pData = value;
            break;
        case GUI_U16:
            *(uint16_t*)data_info->pData = value;
            break;
        case GUI_U32:
            *(uint32_t*)data_info->pData = value;
            break;
        case GUI_STRING:
            break;
    }
}

/*
 * 函数名：draw_one_par_info
 * 描述  ：显示一个菜单项的值
 * 输入  ：p 菜单项数据结构
 *         pdata 显示的数据
 *         unit  单位
 * 输出  ：无
 * 返回  ：无
 */
void draw_one_par_info(DATA_INFO* p, void *pdata, const uint8_t unit)
{
	int32_t data = 0;
	char *ch = (char*)pdata;
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"：", colon_x, p->y);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)p->name[0], OX, p->y);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
		GUI_DispStringAt((const char*)p->name[1], OX, p->y);
	}
    
	switch(p->bits)
	{
		case GUI_U8:
			data = *(uint8_t*)pdata;
			break;
		case GUI_U16:
			data = *(uint16_t*)pdata;
			break;
		case GUI_U32:
			data = *(uint32_t*)pdata;
			break;
		case GUI_STRING:
			ch = (char*)*(uint32_t*)pdata;//神奇的转换
			break;
	}
    
	switch(p->type)
	{
		case INT_T:
			GUI_ClearRect (p->x, p->y, p->x + 6 * 8, p->y + 12);
			GUI_DispDecAt (data, p->x, p->y, p->lon);
			GUI_DispString((const char*)p->unit[unit]);
			break;
		case FLOAT_T:
			GUI_ClearRect (p->x, p->y, p->x + 6 * 8, p->y + 12);
			GUI_GotoXY(p->x, p->y);
			GUI_DispFloatFix ((float)data/pow(10,p->decs), p->lon - p->no_dis_bits, p->decs - p->no_dis_bits);
			GUI_DispString((const char*)p->unit[unit]);
			break;
		case STRING_T:
            GUI_ClearRect (p->x, p->y, p->x + 6 * 8, p->y + 12);
			GUI_DispStringAt((const char*)ch,p->x, p->y);
			break;
		case OPTION_T:
			GUI_ClearRect (p->x, p->y, p->x + 6 * 8, p->y + 12);
			if(sys_par.language == CHINESE)
			{
				GUI_DispStringAt((const char*)(const char*)p->unit[0],p->x, p->y);
			}
			else
			{
				GUI_DispStringAt((const char*)(const char*)p->unit[1],p->x, p->y);
			}
			break;
		case EITHER_OR:
			if(sys_par.language == CHINESE)
			{
				GUI_DispStringAt((const char*)(const char*)p->unit[0],p->x + 10, p->y);
				GUI_DispStringAt((const char*)(const char*)p->unit[1],p->x + 10 + p->x_offset, p->y);
			}
			else
			{
				GUI_SetFont(&GUI_FontHZ_SimSun_13);
				GUI_DispStringAt((const char*)(const char*)p->unit[2],p->x + 10, p->y);
				GUI_DispStringAt((const char*)(const char*)p->unit[3],p->x + 10+p->x_offset, p->y);
				GUI_SetFont(&GUI_FontHZ_SimSun_12);
			}
			
			switch(data)
			{
				case 1:
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, p->x, p->y+3 );
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,p->x + p->x_offset, p->y + p->y_offset );
					break;
				case 0:
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, p->x, p->y+3);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,p->x + p->x_offset, p->y + p->y_offset);
					break;
				default:
					break;
			}
			break;
	}
}

/*
 * 函数名：read_par_to_buf
 * 描述  ：读出指定菜单项的数据
 * 输入  ：data_info
 * 输出  ：value 菜单项数据 如果为NULL就不赋值
 * 返回  ：菜单项数据
 */
uint32_t read_par_to_buf(DATA_INFO* data_info, uint32_t *value)
{
    uint32_t temp = 0;
    
    if(data_info == NULL)
    {
        return 0;
    }
    
    if(data_info->pData == NULL)
    {
        return 0;
    }
    
    switch(data_info->bits)
    {
        case GUI_U8:
            temp = *(uint8_t*)data_info->pData;
            break;
        case GUI_U16:
            temp = *(uint16_t*)data_info->pData;
            break;
        case GUI_U32:
            temp = *(uint32_t*)data_info->pData;
            break;
        case GUI_STRING:
            temp = (uint32_t)data_info->pData;/* 把地址转换为整数 */
            break;
        case GUI_STRUCT:
            temp = (uint32_t)data_info->pData;/* 把地址转换为整数 */
            break;
    }
    
    if(value != NULL)
    {
        *value = temp;
    }
    
    return temp;
}

/*
 * 函数名：load_this_page_info
 * 描述  ：加载当前设置页面的数据项信息
 * 输入  ：无
 * 输出  ：data_info
 * 返回  ：无
 */
void load_this_page_info(DATA_INFO* data_info[])
{
    int32_t i = 0;
    DATA_INFO* temp = pmenu_data;
    
    /* 找到本页的第一个 */
    for(i = 1; i < pmenu_data->cur; i++)
    {
        temp = temp->prev;
    }
    
    memset(data_info, 0, sizeof data_info);
    /* 从第一个开始 */
    for(i = 1; i <= pmenu_data->total; i++, temp = temp->next)
    {
        data_info[i] = temp;
    }
}
/*
 * 函数名：load_this_page_value
 * 描述  ：把加载到内存数组的信息显示出来
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void load_this_page_value(void)
{
    int32_t i = 0;
    
    for(i = 1; i <= 4; i++)
    {
        if(g_cur_info[i])
        {
            read_par_to_buf(g_cur_info[i], &g_cur_val[i]);
            draw_one_par_info(g_cur_info[i], &g_cur_val[i],0);
        }
    }
}

/*
 *去除前导零
 */
char* div_str_pre_zero(char *str)
{
    uint8_t len = strlen(str);
    uint32_t i = 0;
    char *p = str;
    
    for(i = 0; i < len - 1; i++)
    {
        if(p[0] == '0' && p[1] != '.')
        {
            p++;
        }
        else
        {
            break;
        }
    }
    
    return p;
}


/*
 * 函数名：ten_power
 * 描述  ：10的幂
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
uint32_t ten_power(u8 n)
{
	u8 i;
	uint32_t temp = 1;
	for(i = 0; i < n; i++)
	{
		temp *= 10;
	}
	return temp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//函数：mysprintf                                                                                /
//参数type :153 表示数据是浮点数 1带前导零0不带前导零 最大长度为5包括小数点，3位小数 最大长度不能超过5，小数位不能超过3 /
//    unit:为浮点数后追加的单位字符串                                                            /
//    value:要转换为浮点型的整数数据                                                             /
//////////////////////////////////////////////////////////////////////////////////////////////////
void mysprintf(uint8_t *buf, const uint8_t* unit, uint8_t type, uint32_t value)
{
    uint8_t decs_n = (type / 1) % 10;/* 小数位 */
    uint8_t lon    = (type / 10) % 10;/* 长度 */
    uint8_t app_0  = (type / 100) % 10;/* 前导零标志 */
    uint8_t int_n = 0;
    uint8_t temp = 0;
    uint8_t temp_l = 0;
    int32_t i = 0;
    int32_t j = 0;
    
    if(buf == NULL)
    {
        return;
    }
    
    if(lon > 7 || decs_n > 3)
    {
        strcpy((char*)buf, "0.000");
        strcat((char*)buf, (const char*)unit);
        return;
    }
    
    if(lon == 4 && decs_n > 2)
    {
        strcpy((char*)buf, "0.000");
        strcat((char*)buf, (const char*)unit);
        return;
    }
    if(lon == 3 && decs_n > 1)
    {
        strcpy((char*)buf, "0.000");
        strcat((char*)buf, (const char*)unit);
        return;
    }
    if(lon == 2 && decs_n > 0)
    {
        strcpy((char*)buf, "0.000");
        strcat((char*)buf, (const char*)unit);
        return;
    }
    
    int_n =  lon - decs_n - (decs_n > 0);
    
    for(i = 0, j = 0; i < int_n; i++)
    {
        if(decs_n > 0)
        {
            temp = (value / ten_power(lon - 2 - i)) % 10;
        }
        else
        {
            temp = (value / ten_power(lon - 1 - i)) % 10;
        }
        
        /* 去除前导零 */
        if(app_0 == 0)
        {
            /* 最后一个整数位 */
            if(int_n - i == 1)
            {
                buf[j++] = temp + '0';
            }
            /* 不是最后一个整数位 */
            else
            {
                if(temp || j)
                {
                    buf[j++] = temp + '0';
                }
            }
        }
        /* 不去除前导零 */
        else
        {
            buf[j++] = temp + '0';
        }
    }
    
    if(decs_n)
    {
        buf[j++] = '.';
    }
    
    temp_l = j;
    
    /* 小数部分 */
    for(i = 0; i < decs_n; i++)
    {
        temp = (value / ten_power(decs_n - i - 1)) % 10;
        buf[i + temp_l] = temp + '0';
    }
    
    buf[i + temp_l] = 0;
    
    /* 追加后缀信息 */
    if(unit != NULL)
    {
        strcat((char*)buf, (const char*)unit);
    }
}
/*
 * 函数名：cursor_pos
 * 描述  ：显示屏幕光标 会清除掉上一次显示的光标
 * 输入  ：光标的坐标 将其坐标定义为有符号类型为了可以传入负值坐标，就是将光标移出屏幕
 * 输出  ：无
 * 返回  ：无
 */
void cursor_pos(int16_t x, int16_t y)
{
    cursor_pos_n(x, y);
	
	ReFresh_LCD24064();
}
void dis_cursor_pos(void)
{
    cursor_pos(pmenu_data->x + 6 * pmenu_data->ch_bit, pmenu_data->y);
}
void cursor_pos_n(int16_t x, int16_t y)
{
#define XX_//小号字体

#ifdef	XX_
    #define Y_	(y+11)
    #define Y__ (pre_y+11)
#else
    #define Y_	(y+8)
    #define Y__ (pre_y+8)
#endif

	static int16_t pre_x = 0;//该变量为了记录上次光标的位置
	static int16_t pre_y = 0; 
	
	GUI_ClearRect(pre_x, Y__, pre_x+6, Y__);
	pre_x = x;
	pre_y = y;
	
#ifdef X_
	GUI_DrawLine (x  , Y_, x+4      , Y_);
#endif

#ifdef XX_//大号字体
// 	GUI_ClearRect(x-6, y+11, x+6+4, y+11);
	GUI_DrawLine (x  , y+11, x+4  , y+11);
#endif

#undef X_
#undef Y_
#undef Y__
}
/*
 * 函数名：shift_cursor_l
 * 描述  ：更新坐标的位置 并显示出来
 * 输入  ：p 指向当前设置参数的信息结构
 * 输出  ：无
 * 返回  ：无
 */
void dis_cursor(DATA_INFO * p)
{
	cursor_pos(p->x+6*p->ch_bit, p->y);
	ReFresh_LCD24064();
}
/*
 * 函数名：shift_cursor
 * 描述  ：光标循环右移
 * 输入  ：p 指向当前设置参数的信息结构
 * 输出  ：无
 * 返回  ：无
 */
void shift_cursor (DATA_INFO * p)
{
	u8 x,y;
    uint8_t lon = p->lon - p->no_dis_bits;
    uint8_t decs = 0;
    
    if(p->decs >= p->no_dis_bits)
    {
        decs = p->decs - p->no_dis_bits;
    }
    
    if(p == NULL)
    {
        return;
    }
    
	x = p->x;
	y = p->y;
    
	if (p->type == FLOAT_T)//浮点数
	{
        /* 如果小数点个数为0就当整数处理 */
        if(decs == 0)
        {
            goto l_int;
        }
        
		if(p->ch_bit >= lon - 1)
		{
			p->ch_bit = 0;
		}
		else
		{
			p->ch_bit++;
		}
        
		if(lon > p->ch_bit)
		{
			if( p->ch_bit == lon - decs - 1 )
			{
				p->ch_bit++;//跳过小数点
			}
			cursor_pos(x+6*p->ch_bit, y);
		}
		else
		{
			cursor_pos(x-6*(lon-1), y);
		}
	}
	else if(p->type == INT_T)//整数或
	{
l_int:
		if(p->ch_bit >= lon - 1)
		{
			p->ch_bit = 0;
		}
		else
		{
			p->ch_bit++;
		}

		if(lon > p->ch_bit)
		{
			cursor_pos(x+6*p->ch_bit, y);
		}
		else
		{
			cursor_pos(x-6*(lon-1), y);
		}
	}
	else if(p->type == STRING_T)//字符串
	{
		uint8_t lonx = strlen((const char*)pmenu_data->pData);

		if(p->ch_bit == lonx && lonx < lon)
		{
			p->ch_bit = 0;
		}
		else if(p->ch_bit == lon-1)
		{
			p->ch_bit = 0;
		}
		else
		{
			p->ch_bit++;
		}
        
		cursor_pos(x+6*p->ch_bit, y);//更新坐标的位置
	}
}

/*
 * 函数名：shift_cursor_l
 * 描述  ：光标循环左移
 * 输入  ：p 指向当前设置参数的信息结构
 * 输出  ：无
 * 返回  ：无
 */
void shift_cursor_l (DATA_INFO * p)
{
	uint8_t x = 0;
    uint8_t y = 0;
    uint8_t lon = p->lon - p->no_dis_bits;
    uint8_t decs = 0;
    
    if(p->decs >= p->no_dis_bits)
    {
        decs = p->decs - p->no_dis_bits;
    }
    
	x = p->x;
	y = p->y;
    
    //浮点数
	if (p->type == FLOAT_T)
	{
        /* 当小数位为0时作整数处理 */
        if(decs == 0)
        {
            goto l_int;
        }
        
		if(p->ch_bit >= lon - 1)
		{
			p->ch_bit = 0;
		}
		else
		{
			p->ch_bit++;
		}
        
		if(lon > p->ch_bit)
		{
			if( p->ch_bit == lon - decs - 1 )
			{
				p->ch_bit++;//跳过小数点
			}
			cursor_pos(x+6*p->ch_bit, y);
		}
		else
		{
			cursor_pos(x-6*(lon-1), y);
		}
	}
    //整数
	else if(p->type == INT_T)
	{
l_int:
		if(p->ch_bit >= lon - 1)
		{
			p->ch_bit = 0;
		}
		else
		{
			p->ch_bit++;
		}

		if(lon > p->ch_bit)
		{
			cursor_pos(x+6*p->ch_bit, y);
		}
		else
		{
			cursor_pos(x-6*(lon-1), y);
		}
	}
    //字符串
	else if( p->type == STRING_T)
	{
		uint8_t lonx = strlen((const char*)pmenu_data->pData);
		if(p->ch_bit == 0)
		{
			if(lonx == lon)
				p->ch_bit = lonx -1;
			else
				p->ch_bit = lonx;
		}
		else
		{
			p->ch_bit--;
		}
        
		cursor_pos(x+6*p->ch_bit, y);
	}
}

/*
 * 函数名：change_data
 * 描述  ：更新数据
 * 输入  ：base 正在操作的位的最新值0-9
 * 输出  ：无
 * 返回  ：无
 */
void change_data(uint8_t base)
{
	uint32_t t = 0;
	uint32_t temp = 0;
    
	if ( pmenu_data->type == FLOAT_T && pmenu_data->decs != 0)//浮点
	{
		if ( pmenu_data->ch_bit >= pmenu_data->lon - pmenu_data->decs)
		{
			t = ten_power(pmenu_data->lon - pmenu_data->ch_bit -1);
		}
		else
		{
			t = ten_power(pmenu_data->lon - pmenu_data->ch_bit - 1 - 1);
		}
	}
	else if(pmenu_data->type == INT_T || pmenu_data->decs == 0)//整数
	{
		t = ten_power(pmenu_data->lon - pmenu_data->ch_bit - 1);
	}
    
	switch(pmenu_data->bits)
	{
		case GUI_U8://8位
        case GUI_U8_ARRAY:
			temp = *(uint8_t*)pmenu_data->pData;
			temp = temp - ((temp/t)%10)*t + base*t;
			*(uint8_t*)pmenu_data->pData = temp;
			break;
		case GUI_U16://16位
        case GUI_U16_ARRAY:
			temp = *(uint16_t*)pmenu_data->pData;
			temp = temp - ((temp/t)%10)*t + base*t;
			*(uint16_t*)pmenu_data->pData = temp;
			break;
		case GUI_U32://32位
		case GUI_U32_ARRAY:
			temp = *(uint32_t*)pmenu_data->pData;
			temp = temp - ((temp/t)%10)*t + base*t;
			*(uint32_t*)pmenu_data->pData = temp;
			break;
		case GUI_STRING://字符串 文件名 密码
		{
			extern uint32_t g_delay_time_key;
			static uint8_t temp_lon;
			int8_t i = 0;
			static uint8_t flag = 0;
			static uint8_t old = 0;
			
			if(strlen((const char*)pmenu_data->pData) == pmenu_data->lon)
			{
				break;
			}
			if(pmenu_data->ch_bit != old)
			{
				flag = 0;//当执行到这句时就表示新的位进入设置状态
			}
			else
			{
				if(pmenu_data->last_ch_bit == pmenu_data->ch_bit + 1)
				{
					flag = 0;
				}
			}
			
			if(flag == 0)
			{
				old = pmenu_data->ch_bit;
				temp_lon = strlen((const char*)pmenu_data->pData);
				if(temp_lon == 0 || temp_lon == (pmenu_data->ch_bit))
				{
					flag = 1;//改变当前位
				}
				else
				{
					flag = 2;//插入
				}
			}
			
			if(flag == 1)
			{
				if(temp_lon < pmenu_data->lon)
				{
					((uint8_t*)pmenu_data->pData)[pmenu_data->ch_bit] = base;
				}
			}
			else if(flag == 2)
			{
				if(temp_lon < pmenu_data->lon)
				{
					for(i = (temp_lon-0); i >= pmenu_data->ch_bit; i--)
					{
						((uint8_t*)pmenu_data->pData)[i+1] =
						((uint8_t*)pmenu_data->pData)[i];
						if(i == 0)
						{
							break;
						}
					}
                    
					((uint8_t*)pmenu_data->pData)[pmenu_data->ch_bit] = base;
					flag = 1;
				}
			}
			break;
		}
		default:
			break;
	}
}
/*
 * 函数名：my_sprintf
 * 描述  ：更新数据到显示器
 * 输入  ：p 指向当前操作数据的信息结构
 * 输出  ：无
 * 返回  ：无
 */
void my_sprintf(char *buf, DATA_INFO * p, uint32_t data)
{
	int32_t i = 0;
	int32_t j = 0;
    uint8_t t = 0;
    
    uint8_t temp1 = p->lon;
    uint8_t temp2 = p->decs;
    
    if(p->decs != 0)
    {
        temp1 = p->lon - p->decs - 1;
    }
    
    //整数部分
    for (i = 0; i < temp1; i++)
    {
        t = (data / ten_power(temp1 + temp2 - i - 1)) % 10;
        buf[i] = '0' + t;
    }
    //小数点
    if(temp2)
    {
        buf[i++] = '.';
    }
    //小数部分
    for(j = 0; j < temp2; j++)
    {
        t = (data / ten_power(temp2 - j - 1)) % 10;
        buf[i++] = '0' + t;
    }
    //单位
    strcat(buf, (const char*)p->unit[0]);
}
/*
 * 函数名：update_data
 * 描述  ：更新数据到显示器
 * 输入  ：p 指向当前操作数据的信息结构
 * 输出  ：无
 * 返回  ：无
 */
void update_data(DATA_INFO * p)
{
	uint8_t i = 0;
    uint8_t t = 0;
    
    if(p == NULL)
    {
        return;
    }
    
	switch(p->type)
	{
		case FLOAT_T://浮点
        {
            if(p->decs == 0)
            {
                goto l_int;
            }
            
			GUI_GotoXY(p->x, p->y);
            
			for (i = 0; i < p->lon - 1 - p->no_dis_bits; i++)
			{
				switch(p->bits)
				{
					case GUI_U8:
                        t = (*(uint8_t*)p->pData/ten_power(p->lon-i-1-1))%10;
                        break;
					case GUI_U16:
                        t = (*(uint16_t*)p->pData/ten_power(p->lon-i-1-1))%10;
                        break;
					case GUI_U32:
                        t = (*(uint32_t*)p->pData/ten_power(p->lon-i-1-1))%10;
                        break;
                }
                
				if ( i == p->lon - p->decs - 1 )
				{
					GUI_DispChar('.');
					GUI_DispDec(t, 1);
				}
				else
				{
					GUI_DispDec(t, 1);
				}
			}
			break;
        }
		case INT_T://整数
        {
l_int:
			GUI_GotoXY(p->x, p->y);
			
			for (i = 0; i < p->lon - p->no_dis_bits; i++)
			{
				switch(p->bits)
				{
					case 0:
						t = (*(uint8_t*)p->pData/ten_power(p->lon-i-1))%10;
						break;
					case 1:
						t = (*(uint16_t*)p->pData/ten_power(p->lon-i-1))%10;//之前转换成uint8_t*会破坏16位和32位的数据
						break;
					case 2:
						t = (*(uint32_t*)p->pData/ten_power(p->lon-i-1))%10;//之前转换成uint8_t*会破坏16位和32位的数据
						break;
					default:
						break;
				}

				GUI_DispDec(t, 1);
			}
			break;
        }
		case STRING_T://字符串
        {
			GUI_DispStringAt((const char*)p->pData, p->x, p->y);
			if(strlen((const char*)p->pData) >= NAME_LON && p->ch_bit == (NAME_LON-1))
			{
				p->ch_bit = NAME_LON - 2;
				shift_cursor(p);
			}
			break;
        }
		default:
			break;
	}

	ReFresh_LCD24064();
}
/*
 * 函数名：clear_
 * 描述  ：只清屏不显示结果 防止屏幕闪烁
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void clear_(void)
{
	GUI_ClearRect (0, 0, 240-1, 64-1);
}

/*
 * 函数名：clear_menu
 * 描述  ：清除屏幕右侧的菜单栏 只清屏不显示结果 防止屏幕闪烁
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void clear_menu(void)
{
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);
	GUI_ClearRect (200+1, 15+16+1, 240-1, 15+16+16-1);
	GUI_ClearRect (200+1, 15+16+16+1, 240-1, 15+16+16+16);
}
void clear_menu_n(uint8_t n)
{
    if(n == 1)
    {
        GUI_ClearRect (200+1, 0, 240-1, 15-1);
    }
    else if(n == 2)
    {
        GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);
    }
    else if(n == 3)
    {
        GUI_ClearRect (200+1, 15+16+1, 240-1, 15+16+16-1);
    }
    else if(n == 4)
    {
        GUI_ClearRect (200+1, 15+16+16+1, 240-1, 15+16+16+16);
    }
}

///////////////////////////////////////////////////////////////////


/*
 * 函数名：gui_par_inval
 * 描述  ：说明参数设置无效
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_par_inval(void)
{
	GUI_ClearRect(130+1,18,200-1,63);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"本机型下", 145, 22);
		GUI_DispStringAt((const char*)"设置无效", 145, 36);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"This Tester", 145-10, 22);
		GUI_DispStringAt((const char*)"Invalid!", 145-10, 36);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
}

/*
 * 函数名：gui_par_inval
 * 描述  ：在g模式时有的参数有特殊要求在特定情况下不能设置的
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_g_mode_inval(void)
{
	GUI_ClearRect(130+1,18,200-1,63);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"系统默认", 145, 22);
		GUI_DispStringAt((const char*)"不能改变", 145, 36);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Systerm", 145, 22);
		GUI_DispStringAt((const char*)"default", 145, 36);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
// 	if(sys_par.language == CHINESE)
// 	{
// 		GUI_DispStringAt((const char*)"G 模式下", 145, 22);
// 		GUI_DispStringAt((const char*)"设置无效", 145, 36);
// 	}
// 	else
// 	{
// 		GUI_SetFont(&GUI_FontHZ_SimSun_13);
// 		GUI_DispStringAt((const char*)"G mode", 145, 22);
// 		GUI_DispStringAt((const char*)"invalid", 145, 36);
// 		GUI_SetFont(&GUI_FontHZ_SimSun_12);
// 	}
}
/*
 * 函数名：ratio_broken
 * 描述  ：校准系数丢失，请送修
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void ratio_broken(void)
{
	uint8_t X0	= 50;
	uint8_t Y0	= 8;
	uint8_t X1	= 200;
	uint8_t Y1	= 58;
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"系统校准系数破", 0+X0+35, Y0+4+4);
		GUI_DispStringAt((const char*)"坏，请送修!", 0+X0+35, Y0+4+16);
		GUI_DispStringAt((const char*)"按复位键进入主界面", 0+X0+35, Y0+4+28);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"System data broken", 0+X0+35, Y0+4+4);
		GUI_DispStringAt((const char*)"please service!", 0+X0+35, Y0+4+16);
		GUI_DispStringAt((const char*)"STOP_KEY into!", 0+X0+35, Y0+4+28);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
	GUI_DrawRect(X0, Y0, X1, Y1);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, X0+3, Y0+5);
	LCD_REFRESH();
}
/*
 * 函数名：cal_warning
 * 描述  ：校准开关打开后提示
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void cal_warning(void)
{
	uint8_t X0	= 40;
	uint8_t Y0	= 8;
	uint8_t X1	= 180;
	uint8_t Y1	= 58;
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"系统校准开关打", 0+X0+35, Y0+4+4);
		GUI_DispStringAt((const char*)"开,请将其关闭后", 0+X0+35, Y0+4+16);
		GUI_DispStringAt((const char*)"重启仪器!", 0+X0+35, Y0+4+28);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"System master", 0+X0+35, Y0+4+4);
		GUI_DispStringAt((const char*)"switcher is on,", 0+X0+35, Y0+4+16);
		GUI_DispStringAt((const char*)"please turn off!", 0+X0+35, Y0+4+28);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
    GUI_DrawRect(X0, Y0, X1, Y1);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, X0+3, Y0+5);
	LCD_REFRESH();
}
/*
 * 函数名：auto_cal_pass
 * 描述  ：自动校准通过
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void auto_cal_pass(void)
{
	uint8_t X0	= 40;
	uint8_t Y0	= 8;
	uint8_t X1	= 180;
	uint8_t Y1	= 58;
    
    clear_();
	GUI_SetFont(&GUI_FontHZ_MS_Sans_Serif_24);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"PASS", 0+X0+35, Y0+4+4);
	}
	else
	{
	}
	
    GUI_DrawRect(X0, Y0, X1, Y1);
	LCD_REFRESH();
}
/*
 * 函数名：rate_of_advance
 * 描述  ：完成进度
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void rate_of_advance(void)
{
	uint8_t X0	= 40;
	uint8_t Y0	= 25;
	uint8_t X1	= 180;
	uint8_t Y1	= 38;
    
    uint8_t flag = 0;
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"进度", (X0 + X1)/2 - 6 * 5, Y1-12);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Advance", (X0 + X1)/2 - 6 * 6, Y0+4+4);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
    GUI_DrawRect(X0, Y0, X1, Y1);
    
    while(1)
    {
        GUI_DispDecAt(app_flag.usb_loading_advance, (X0 + X1)/2, Y1 - 12, 2);
        LCD_REFRESH();
        OSTimeDlyHMSM(0,0,1,0);
        
        if(app_flag.usb_con_st != USB_CON_OK)
        {
            break;
        }
        
        if(++app_flag.leading_watch_dog > 50)
        {
            break;
        }
        
        if(app_flag.usb_loading_advance == 100)
        {
            OSTimeDlyHMSM(0,0,2,0);
            
            if(flag>0)
            {
                BUZZER_SOUND(300);/* 开机蜂鸣300ms */
                break;
            }
            
            flag++;
        }
    }
}
/*
 * 函数名：type_mismatch
 * 描述  ：机型不匹配是提示框
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void type_mismatch(void)
{
	uint8_t X0	= 40;
	uint8_t Y0	= 8;
	uint8_t X1	= 165;
	uint8_t Y1	= 58;
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	
	GUI_DispStringAt((const char*)"机型不匹配", 0+X0+35, Y0+4+14);
	
    GUI_DrawRect(X0, Y0, X1, Y1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, X0+3, Y0+5);
	LCD_REFRESH();
}
/*
 * 函数名：dis_set_port
 * 描述  ：设置界面显示端口状态
 * 输入  ：val 状态值 n 位置 ports 端口数目 4 8
 * 输出  ：无
 * 返回  ：无
 */
void dis_set_port(const uint8_t val,const uint8_t n, const uint8_t ports)
{
	uint32_t offset = 0;
	if(ports == 4)
	{
		offset = 15*4;
	}
	else if(ports == 8)
	{
		offset = 0;
	}
	
    GUI_DispStringAt((const char*)PORT_STATUS_STR[val%3], offset+15*n, 30);
}
/*
 * 函数名：ports_cursor_pos
 * 描述  ：设置界面设置端口时光标的显示位置
 * 输入  ：n 位置 ports 端口数目 4 8
 * 输出  ：无
 * 返回  ：无
 */
void ports_cursor_pos(uint8_t n, uint8_t ports)
{
	int32_t offset = 0;
    uint8_t base_x = UI_SET_PORTS_BASE_X + 6 * 6;
    uint8_t base_y = UI_SET_PORTS_BASE_Y;
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t xo = UI_SET_PORT_X_O;
    uint8_t yo = UI_SET_PORT_Y_O;
    
    if(ports == PORT_NUM14)
    {
        if(n < 8)
        {
            y = base_y;
        }
        else if(n < 16)
        {
            y = base_y + yo;
        }
        
        offset = xo * (n % 8);
        x = base_x + offset;
        
        cursor_pos(x, y);
    }
    else
    {
        if(ports == 4)
        {
            offset = 15 * 4;
        }
        else if(ports == 8)
        {
            offset = 0;
        }
        
        cursor_pos(offset + 15 * (n + 1), 30);
    }
}
void gui_dis_float_at(const float v, const uint8_t decs, const uint8_t x, const uint8_t y)
{
    uint8_t CW = 9;
    uint8_t O = 13;
    uint8_t temp_long = 0;
    uint8_t temp_decs = decs;
    
    if(decs > 3)
    {
        temp_decs = 3;
    }
    
    temp_long = COUNT_FLOAT_LON(v, temp_decs);
    
    GUI_ClearRect(x, y, x + CW * 5, y + O);
    GUI_GotoXY(x + CW * (5 - temp_long), y);
    GUI_DispFloatFix (v, temp_long, temp_decs);
}
/*
 * 函数名：dcw_cur_sel_gui
 * 描述  ：dcw 电流档位选择用的界面服务程序
 * 输入  ：kinds 电流档位的种类 n 第几个电流档
 * 输出  ：无
 * 返回  ：无
 */
void dcw_cur_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n)
{
    mor_sel_menu[DCW_GEAR_SEL_MENU].y = pmenu_data->y;
    mor_sel_menu[DCW_GEAR_SEL_MENU].opt_buf = gear_buf;
    more_options_sel_gui(kinds, n - 1, &mor_sel_menu[DCW_GEAR_SEL_MENU]);
    LCD_REFRESH();
}

/*
 * 函数名：acw_gr_hz_sel_gui
 * 描述  ：频率设置时的 显示服务程序
 * 输入  ：kinds 频率的种类 n 第几个档 gear_buf 档位的名称缓冲
 * 输出  ：无
 * 返回  ：无
 */
void acw_gr_hz_sel_gui(uint8_t kinds, uint8_t n, const uint8_t **gear_buf)
{
	GUI_ClearRect(135, 15,200-1,63);
	
	switch(kinds)
	{
		case 1:
			GUI_DispStringAt((const char*)gear_buf[0], 135, 18+2+12);
			GUI_DispStringAt((const char*)"1", 135, 18+2+12);
			break;
		case 2:
			GUI_DispStringAt((const char*)gear_buf[0], 135, 18+12*0+2+8);
			GUI_DispStringAt((const char*)gear_buf[1], 135, 18+12*1+2+8);
			GUI_DispStringAt((const char*)"1", 135, 18+12*0+2+8);
			GUI_DispStringAt((const char*)"2", 135, 18+12*1+2+8);
			break;
		case 3:
			GUI_DispStringAt((const char*)gear_buf[0], 135, 18+12*0+2);
			GUI_DispStringAt((const char*)gear_buf[1], 135, 18+12*1+2);
			GUI_DispStringAt((const char*)gear_buf[2], 135, 18+12*2+2);
			GUI_DispStringAt((const char*)"1", 135, 18+12*0+2);
			GUI_DispStringAt((const char*)"2", 135, 18+12*1+2);
			GUI_DispStringAt((const char*)"3", 135, 18+12*2+2);
			break;
		case 4:
			switch(n)
			{
				case 1:
				case 2:
					GUI_DispStringAt((const char*)gear_buf[0], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[1], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[2], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"1", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"2", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"3", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 56);
					break;
				case 3:
				case 4:
					GUI_DispStringAt((const char*)gear_buf[2], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[3], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"3", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"4", 135, 18+12*1+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 15);
					break;
			}
			break;
		case 5:
			switch(n)
			{
				case 1:
				case 2:
					GUI_DispStringAt((const char*)gear_buf[0], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[1], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[2], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"1", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"2", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"3", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 56);
					break;
				case 3:
				case 4:
					GUI_DispStringAt((const char*)gear_buf[2], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[3], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[4], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"3", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"4", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"5", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 15);
					break;
			}
			break;
		case 6:
			switch(n)
			{
				case 1:
				case 2:
					GUI_DispStringAt((const char*)gear_buf[0], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[1], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[2], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"1", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"2", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"3", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 56);
					break;
				case 3:
				case 4:
					GUI_DispStringAt((const char*)gear_buf[2], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[3], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[4], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"3", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"4", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"5", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 15);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 56);
					break;
				case 5:
				case 6:
					GUI_DispStringAt((const char*)gear_buf[4], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[5], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"5", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"6", 135, 18+12*1+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 15);
			}
			break;
		case 7:
			switch(n)
			{
				case 1:
				case 2:
					GUI_DispStringAt((const char*)gear_buf[0], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[1], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[2], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"1", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"2", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"3", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 56);
					break;
				case 3:
				case 4:
					GUI_DispStringAt((const char*)gear_buf[2], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[3], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[4], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"3", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"4", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"5", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 15);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 56);
					break;
				case 5:
				case 6:
				case 7:
					GUI_DispStringAt((const char*)gear_buf[4], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[5], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[6], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"5", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"6", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"7", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 15);
					break;
			}
			break;
		case 8:
			switch(n)
			{
				case 1:
				case 2:
					GUI_DispStringAt((const char*)gear_buf[0], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[1], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[2], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"1", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"2", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"3", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 56);
					break;
				case 3:
				case 4:
					GUI_DispStringAt((const char*)gear_buf[2], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[3], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[4], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"3", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"4", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"5", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 15);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 56);
					break;
				case 5:
				case 6:
					GUI_DispStringAt((const char*)gear_buf[4], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[5], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[6], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"5", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"6", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"7", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 15);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 56);
					break;
				case 7:
				case 8:
					GUI_DispStringAt((const char*)gear_buf[6], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[7], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"7", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"8", 135, 18+12*1+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 15);
					break;
			}
			break;
		case 9:
			switch(n)
			{
				case 1:
				case 2:
					GUI_DispStringAt((const char*)gear_buf[0], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[1], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[2], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"1", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"2", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"3", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 56);
					break;
				case 3:
				case 4:
					GUI_DispStringAt((const char*)gear_buf[2], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[3], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[4], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"3", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"4", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"5", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 15);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 56);
					break;
				case 5:
				case 6:
					GUI_DispStringAt((const char*)gear_buf[4], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[5], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[6], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"5", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"6", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"7", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 15);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 56);
					break;
				case 7:
				case 8:
				case 9:
					GUI_DispStringAt((const char*)gear_buf[6], 135, 18+12*0+2);
					GUI_DispStringAt((const char*)gear_buf[7], 135, 18+12*1+2);
					GUI_DispStringAt((const char*)gear_buf[8], 135, 18+12*2+2);
					GUI_DispStringAt((const char*)"7", 135, 18+12*0+2);
					GUI_DispStringAt((const char*)"8", 135, 18+12*1+2);
					GUI_DispStringAt((const char*)"9", 135, 18+12*2+2);
					GUI_DrawBitmap((const GUI_BITMAP*) &bmcolon, 135+15+2, 15);
					break;
			}
			break;
	}
}
/*
 * 函数名：acw_cur_sel_gui
 * 描述  ：acw 电流选择界面服务程序
 * 输入  ：kinds 电流档位的种类 n 第几个电流档
 * 输出  ：无
 * 返回  ：无
 */
void ac_cur_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n)
{
    mor_sel_menu[ACW_GEAR_SEL_MENU].y = pmenu_data->y;
    mor_sel_menu[ACW_GEAR_SEL_MENU].opt_buf = gear_buf;
    more_options_sel_gui(kinds, n - 1, &mor_sel_menu[ACW_GEAR_SEL_MENU]);
    LCD_REFRESH();
}
/*
 * 函数名：sys_option_sel_gui
 * 描述  ：acw 电流选择界面服务程序
 * 输入  ：kinds 电流档位的种类 n 第几个电流档
 * 输出  ：无
 * 返回  ：无
 */
void sys_option_sel_gui(uint8_t options, const uint8_t** opt_buf, uint8_t n)
{
    mor_sel_menu[SYS_OPT_SET_MENU].y = pmenu_data->y;
    mor_sel_menu[SYS_OPT_SET_MENU].opt_buf = opt_buf;
    more_options_sel_gui(options, n, &mor_sel_menu[SYS_OPT_SET_MENU]);
    LCD_REFRESH();
}

void more_options_sel_gui(uint8_t options, uint8_t n, MORE_SEL_STR *mor_sel_info)
{
    #define Y           (mor_sel_info->y)
    #define X           (mor_sel_info->x)
    #define X_O(N)      (mor_sel_info->x + mor_sel_info->x_o[N])
    
    int32_t i = 0;
    uint8_t cur_ui_num = 3;
    uint8_t p_num[10] = {0};/* 统计各行能显示的个数 */
    int32_t j,k;
    uint8_t size = sizeof mor_sel_info->x_o;
    uint32_t temp = 0;
    uint32_t temp1 = 0;
    const uint8_t** opt_buf = mor_sel_info->opt_buf;
    
    /* 统计各行能显示的个数 */
    for(i = 0, j = 0, k = 0; i < size; i++)
    {
        if(mor_sel_info->x_o[i] == 0 && i > 0)
        {
            p_num[j++] = k;
            k = 0;
        }
        
        k++;
    }
    
    for(i = 0; i < j; i++)
    {
        if(temp <= n)
        {
            temp1 = temp;
            temp += p_num[i];
            cur_ui_num = p_num[i];
            
            if(temp > options)
            {
                temp = options;
                cur_ui_num = temp - temp1;
            }
        }
    }
    
    GUI_ClearRect(X, Y,200-1, Y+12);
    
    temp -= cur_ui_num;
    
    for(i = 0; i < cur_ui_num; i++)
    {
        GUI_DispStringAt((const char*)opt_buf[temp + i], X_O(temp + i) + 10, Y);
        
        if(temp + i == n)
        {
            GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, X_O(temp + i), Y + 3);
        }
        else
        {
            GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X_O(temp + i), Y + 3);
        }
    }
    
    cursor_pos_n(X_O(n - 0) + 1, Y);
    
    #undef X
    #undef Y
    #undef X_O
}

void draw_bitmap_circle(uint8_t x, uint8_t y, uint8_t st)
{
    const GUI_BITMAP* p;
    
    if(st)
    {
        p = &bmcircle_;
    }
    else
    {
        p = &bmcircle;
    }
    
    GUI_DrawBitmap(p, x, y + 3);
}
/*
 * 函数名：draw_more_options_gui
 * 描述  ：显示多选项，以及当前选中项
 * 输入  ：list 多选项的链表
 *         iteration 当前的选项
 * 输出  ：无
 * 返回  ：无
 */
void draw_more_options_gui(MORE_OPTIONS_LIST *list, MORE_OPTIONS_STR *iteration)
{
    MORE_OPTIONS_STR *first = NULL;
    uint8_t i = 0;
    
    /* 找到当前行的第一个 */
    for(first = iteration; first->cur != 1; first = first->prev);
    
    /* 清除历史显示 */
    GUI_ClearRect(first->x, first->y, 200 - 1, first->y + 12);
    
    /* 显示选项 */
    for(i = 0; i < iteration->total; i++, first = first->next)
    {
        /* 选项名称显示 */
        GUI_DispStringAt((const char*)first->name[!sys_par.language==CHINESE], first->x + 10, first->y);
        /* 是否选中的显示位图 */
        draw_bitmap_circle(first->x, first->y, first->cur == iteration->cur);
    }
    /* 显示光标 */
    cursor_pos_n(iteration->x + 1, iteration->y);
    /* 刷屏 */
    LCD_REFRESH();
}

/*
 * 函数名：gr_vol_sel_gui
 * 描述  ：GR 直流压降模式下 选择电压档位
 * 输入  ：kinds 电流档位的种类 n 第几个电档
 * 输出  ：无
 * 返回  ：无
 */
void gr_vol_sel_gui(uint8_t options, const uint8_t** opt_buf, uint8_t n)
{
    mor_sel_menu[GR_VOL_GEAR_SEL_MENU].y = pmenu_data->y;
    mor_sel_menu[GR_VOL_GEAR_SEL_MENU].opt_buf = opt_buf;
    more_options_sel_gui(options, n - 1, &mor_sel_menu[GR_VOL_GEAR_SEL_MENU]);
    LCD_REFRESH();
}

/*
 * 函数名：mode_sel_gui
 * 描述  ：选择模式界面显示服务程序
 * 输入  ：kinds 模式的种类 n 位置
 * 输出  ：无
 * 返回  ：无
 */
void mode_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n)
{
    mor_sel_menu[MODE_SET_SEL_MENU].y = pmenu_data->y;
    mor_sel_menu[MODE_SET_SEL_MENU].opt_buf = gear_buf + 1;
    more_options_sel_gui(kinds, n - 1, &mor_sel_menu[MODE_SET_SEL_MENU]);
}
/*
 * 函数名：dcgr_commuincation_err
 * 描述  ：直流gr开机通信失败提示框
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void dcgr_commuincation_err(void)
{
	uint8_t X0	= 50;
	uint8_t Y0	= 8;
	uint8_t X1	= 185;
	uint8_t Y1	= 55;
    
	clear_();
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"通讯失败!", X0+35, Y0+4);
		GUI_DispStringAt((const char*)"直流模块通信失败", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"请检查硬件...", X0+35, Y0+28);
	}
	else
	{
		GUI_DispStringAt((const char*)"Commun. fail", X0+35, Y0+4);
		GUI_DispStringAt((const char*)"DC_GR mode error", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"Please check...", X0+35, Y0+28);
	}
    
    GUI_DrawRect(X0, Y0, X1, Y1);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, 60, 12);
	ReFresh_LCD24064();
	
	OSTimeDlyHMSM(0,0,0,200);
}
/*
 * 函数名：cal_sw_on_warning
 * 描述  ：校准开关打开后就报警提示
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void cal_sw_on_warning(void)
{
	uint8_t X0	= 50;
	uint8_t Y0	= 8;
	uint8_t X1	= 185;
	uint8_t Y1	= 55;
    
	clear_();
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"系统校准开关", X0+35, Y0+4);
		GUI_DispStringAt((const char*)"打开，请关闭", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"后重启。", X0+35, Y0+28);
	}
	else
	{
		GUI_DispStringAt((const char*)"Systerm master", X0+35, Y0+4);
		GUI_DispStringAt((const char*)"switcher is on", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"please turn off", X0+35, Y0+28);
	}
    
    GUI_DrawRect(X0, Y0, X1, Y1);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, 60, 20);
	
	ReFresh_LCD24064();
	OSTimeDlyHMSM(0,0,0,200);
}

void draw_wait_usb_connect_gui(void)
{
	uint8_t X0	= 50;
	uint8_t Y0	= 8;
	uint8_t X1	= 185;
	uint8_t Y1	= 55;
    
    GUI_ClearRect(X0, Y0, X1, Y1);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"正在检测U盘", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"...", X0+35, Y0+28);
// 		GUI_DispStringAt((const char*)"后重启。", X0+35, Y0+28);
	}
	else
	{
		GUI_DispStringAt((const char*)"Checking usb_flash", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"waiting...", X0+35, Y0+28);
// 		GUI_DispStringAt((const char*)"please turn off", X0+35, Y0+28);
	}
    
    GUI_DrawRect(X0, Y0, X1, Y1);
    LCD24064_Refresh_Rect(X0, Y0, X1, Y1);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, 55, 20);
	
	ReFresh_LCD24064();
	OSTimeDlyHMSM(0,0,0,200);
}
void draw_auto_calibration_gui(void)
{
	uint8_t X0	= 50;
	uint8_t Y0	= 15;
    
    clear_();
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"正在自动校准", X0+100, 0);
        
		GUI_DispStringAt((const char*)"000"       , 0 , 0);
		GUI_DispStringAt((const char*)"模式:ACW"  , 20, 0);
		GUI_DispStringAt((const char*)"项目:电压" , 80, 0);
        
		GUI_DispStringAt((const char*)"DA值:4096"        , 0, Y0+12*0);
		GUI_DispStringAt((const char*)"AD值:4096   4095" , 0, Y0+12*1);
		GUI_DispStringAt((const char*)"档位:"         , 0, Y0+12*2);
		GUI_DispStringAt((const char*)"校准点:100V"      , 0, Y0+12*3);
        
		GUI_DispStringAt((const char*)"电压:5.00kV" , 150, Y0+12*0);
		GUI_DispStringAt((const char*)"电流:2.00mA" , 150, Y0+12*1);
// 		GUI_DispStringAt((const char*)"电阻:2.00MΩ" , 150, Y0+12*2);
		GUI_DispStringAt((const char*)"测量:2.00MΩ" , 150, Y0+12*3);
	}
	else
	{
	}
    
    GUI_DrawLine(0, 12, 240 - 1, 12);
}
void draw_no_connect_usb_gui(void)
{
	uint8_t X0	= 50;
	uint8_t Y0	= 8;
	uint8_t X1	= 185;
	uint8_t Y1	= 55;
    
    GUI_ClearRect(X0, Y0, X1, Y1);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"未检测到U盘", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"按EXIT退出", X0+35, Y0+28);
// 		GUI_DispStringAt((const char*)"后重启。", X0+35, Y0+28);
	}
	else
	{
		GUI_DispStringAt((const char*)"Checking usb_flash", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"press EXIT", X0+35, Y0+28);
// 		GUI_DispStringAt((const char*)"please turn off", X0+35, Y0+28);
	}
    
    GUI_DrawRect(X0, Y0, X1, Y1);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, 55, 20);
	
	ReFresh_LCD24064();
	OSTimeDlyHMSM(0,0,0,200);
}
/*
 * 函数名：GUI_Draw_build_connect_to_board
 * 描述  ：开机建立通信的提示框
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_build_connect_to_board(void)
{
	uint8_t X0	= 50;
	uint8_t Y0	= 8;
	uint8_t X1	= 185;
	uint8_t Y1	= 55;
    
	clear_();
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"系统正在与通讯", X0+35, Y0+4);
		GUI_DispStringAt((const char*)"接口板建立连接", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"中，请稍后...", X0+35, Y0+28);
	}
	else
	{
		GUI_DispStringAt((const char*)"Communicating", X0+35, Y0+4);
		GUI_DispStringAt((const char*)"Build connection", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"Please wait...", X0+35, Y0+28);
	}
    
    GUI_DrawRect(X0, Y0, X1, Y1);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, 60, 12);
	
	ReFresh_LCD24064();
}
/*
 * 函数名：GUI_Draw_build_connect_fail
 * 描述  ：通信连接失败提示框
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_build_connect_fail(void)
{
	uint8_t X0	= 50;
	uint8_t Y0	= 8;
	uint8_t X1	= 185;
	uint8_t Y1	= 55;
    
	clear_();
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)" 通信未连接！", X0+35, Y0+10);
		GUI_DispStringAt((const char*)"", X0+35, Y0+16);
		GUI_DispStringAt((const char*)" 请按Enter键继续", X0+35, Y0+28);
	}
	else
	{
		GUI_DispStringAt((const char*)"Connection fail.", X0+35, Y0+4);
		GUI_DispStringAt((const char*)"Press Enter key ", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"to continue.", X0+35, Y0+28);
	}
    
    GUI_DrawRect(X0, Y0, X1, Y1);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, 60, 12);
	ReFresh_LCD24064();
    
	while((scan_keyboard() != KEY_ENTER) && ((app_flag.power_check & 0xf0) != 0xf0));
}
/*
 * 函数名：ProgressBar
 * 描述  ：滚动条显示更新
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static void ProgressBar(void)
{
	uint8_t i = 0;
	
	GUI_ClearRect (16, 50, 226, 59);
	LCD24064_Refresh_Rect(16, 50, 226, 59);
    
	for(i = 0; i < 35; i++)
	{
		GUI_DrawBitmap((const GUI_BITMAP*) &bm16bit, 17+6*i, 50);
		LCD24064_Refresh_Rect(17+6*i, 50,17+6*i+8, 50+9);
		OSTimeDlyHMSM(0,0,0,20);
	}
}

/*
 * 函数名：GUI_Draw_self_check
 * 描述  ：开机自检
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_self_check(void)
{
	uint8_t sign = 0;
    
	clear_();
	GUI_SetFont(&GUI_FontHZ_SimSun_12);

	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"1.电源"   , 0+15, 0);
		GUI_DispStringAt((const char*)"2.键盘"   , 0+15, 15);
		GUI_DispStringAt((const char*)"3.时钟"   , 0+15, 30);
		GUI_DispStringAt((const char*)"4.存储器"	, 120, 0);
		GUI_DispStringAt((const char*)"5.电压检测", 120, 15);
		GUI_DispStringAt((const char*)"6.电流检测", 120, 30);
	}
	else
	{
		GUI_DispStringAt((const char*)"1.Power"  , 0+15  , 0);
		GUI_DispStringAt((const char*)"2.Key"    , 0+15  , 15);
		GUI_DispStringAt((const char*)"3.Clock"  , 0+15  , 30);
		GUI_DispStringAt((const char*)"4.Memory"	, 120   , 0);
		GUI_DispStringAt((const char*)"5.Voltage", 120   , 15);
		GUI_DispStringAt((const char*)"6.Current", 120   , 30);
	}
    
    GUI_DrawRect(14, 48, 230, 60 );
	ReFresh_LCD24064();
    
	ProgressBar();/* 电源 */
	GUI_DrawBitmap((const GUI_BITMAP*) &bmindex, 60, 6);
	if(app_flag.power_check==0xf1)
	{
		GUI_DispStringAt((const char*)"OK", 0+15+45+20, 0);
	}
	else
	{ 
		GUI_DispStringAt((const char*)"NG", 0+15+45+20, 0);
		sign++;
	}
	LCD24064_Refresh_Rect(60, 0,92,12);
	ProgressBar();/* 键盘 */
	GUI_DrawBitmap((const GUI_BITMAP*) &bmindex, 60, 6+15);
	if(scan_keyboard() == KEY_NONE)
	{
		GUI_DispStringAt((const char*)"OK", 0+15+45+20, 15);
	}
	else
	{ 
		GUI_DispStringAt((const char*)"NG", 0+15+45+20, 15);
		sign++;
	}
	LCD24064_Refresh_Rect(60, 15,92,27);
	ProgressBar();/* 时钟 */
	GUI_DrawBitmap((const GUI_BITMAP*) &bmindex, 60, 6+15*2);
	
	if(1==1)
	{
		GUI_DispStringAt((const char*)"OK", 0+15+45+20, 15*2);
	}
	else
	{ 
		GUI_DispStringAt((const char*)"NG", 0+15+45+20, 15*2);
		sign++;
	}
	LCD24064_Refresh_Rect(60, 30,92,43);
	ProgressBar();/* 存储器 */
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmindex, 190, 6);
	if(sys_flag.flag_flag == FLAG_FLAG)
	{
		GUI_DispStringAt((const char*)"OK", 190+20, 0);//0+15+45+130+3*j
	}
	else
	{
		GUI_DispStringAt((const char*)"NG", 190+20, 0);
		sign++;
	}
	
	LCD24064_Refresh_Rect(190, 0,222,12);
	ProgressBar();/* 电压 */
	GUI_DrawBitmap((const GUI_BITMAP*) &bmindex, 190, 6+15);
	
	if(1==1)
	{
		GUI_DispStringAt((const char*)"OK", 190+20, 15);
	}
	else
	{
		GUI_DispStringAt((const char*)"NG", 190+20, 15);
		sign++;
	}
	
	LCD24064_Refresh_Rect(190, 15,222,27);
	ProgressBar();/* 电流 */
	GUI_DrawBitmap((const GUI_BITMAP*) &bmindex, 190, 6+15*2);
	
	if(1==1)
	{
		GUI_DispStringAt((const char*)"OK", 190+20, 15*2);
	}
	else
	{
		GUI_DispStringAt((const char*)"NG", 190+20, 15*2);
		sign++;
	}
	
	LCD24064_Refresh_Rect(190, 30,222,43);
	OSTimeDlyHMSM(0,0,0,200);
    
	if(sign)
	{
		GUI_ClearRect (10, 45, 230, 60);
		if(sys_par.language == CHINESE)
		{
			GUI_DispStringAt((const char*)"错误！按任意键继续...", 50, 50);
		}
		else
		{
			GUI_DispStringAt((const char*)"Error! Press any key to continue.", 30, 50);
		}
		ReFresh_LCD24064();
		while(1)
		{
			if(scan_keyboard() != KEY_NONE || STOP_PIN==0)
			{
				break;
			}
		}
	}
	
	ReFresh_LCD24064();
}


/*
 * 函数名：li_chun_dingzhi_main_ui
 * 描述  ：立春仪器定制主界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
#ifdef LICHUN_DZ
void li_chun_dingzhi_main_ui(void)
{
	char t_buf[50] = {0};

    if(g_cur_type == NULL)
    {
        return;
    }
    
    if(sys_par.language == CHINESE)
    {
        sprintf(t_buf,"硅堆正向测试仪   Ver:%s",g_cur_type->ver);
    }
    else
    {
        GUI_SetFont(&GUI_FontHZ_SimSun_13);
        sprintf(t_buf,"Si.Stack Break.   Ver:%s",g_cur_type->ver);
        GUI_SetFont(&GUI_FontHZ_SimSun_12);
    }
    
	if(sys_par.language == CHINESE)
	{
        GUI_SetFont(&GUI_FontHZ_FangZhengXingKai_24);
        GUI_DispStringAt((const char*)"上海立春仪器仪表", 0, 0);
        GUI_SetFont(&GUI_FontHZ_SimSun_12);
        GUI_DispStringAt((const char*)"http://www.shlcjm.com", 20, 52);
        
        draw_short_cut_str("设置", "菜单", "测试", "帮助");
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_16);
        GUI_DispStringAt((const char*)"SHANG HAI LI CHUN", 15, 5);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
		GUI_DispStringAt((const char*)"http://www.shlcjm.com", 20, 52);
        
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
        draw_short_cut_str("Set", "Menu", "Test", "Help");
	}
    
    GUI_SetFont(&GUI_FontHZ_SimSun_12);
    GUI_DispStringAt((const char*)t_buf, 20+6*0, 28);/* 显示机型 */
    
    GUI_DispStringAt((const char*)gUpdateTime, 20, 40);/* 显示时间 */
    
    draw_menu_frame_line(0, 23, 200, 23);/* 画出菜单框架线 */
    
	ReFresh_LCD24064();
}
#endif
void csallwin_mian_ui(void)
{
	char t_buf[50] = {0};
    char n_buf[21] = {0};
    char v_buf[21] = {0};
    
    if(NULL == g_cur_type)
    {
        return;
    }
    
    /* 这样处理防止数组越界 */
    strncpy(n_buf, g_cur_type->name, 20);
    strncpy(v_buf, g_cur_type->ver, 20);
    
    sprintf(t_buf,"%s   Ver:%s", n_buf, v_buf);
    
	if(sys_par.language == CHINESE)
	{
		GUI_SetFont(&GUI_FontHZ_FangZhengXingKai_24);
		GUI_DispStringAt((const char*)"长 盛 仪 器", 45, 0);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
		GUI_DispStringAt((const char*)"http://www.csallwin.com", 20, 52);
		GUI_DrawBitmap((const GUI_BITMAP*) &bmlogo, 10, 0);
        
        draw_short_cut_str("设置", "菜单", "测试", "帮助");
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_Garamond_16);
		// GUI_SetFont(&GUI_FontHZ_SimSun_16);
		GUI_DispStringAt((const char*)"Allwin Instrument", 45+20, 0+8);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
		GUI_DispStringAt((const char*)"http://www.csallwin.com", 20, 52);
		GUI_DrawBitmap((const GUI_BITMAP*) &bmallwin, 2, 2);
        
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
        draw_short_cut_str("Set", "Menu", "Test", "Help");
	}
    
    GUI_SetFont(&GUI_FontHZ_SimSun_12);
    GUI_DispStringAt((const char*)t_buf, 20+6*0, 28);/* 显示机型 */
    
    GUI_DispStringAt((const char*)gUpdateTime, 20, 40);/* 显示时间 */
    
    draw_menu_frame_line(0, 23, 200, 23);/* 画出菜单框架线 */
    
	ReFresh_LCD24064();
}
#ifdef VTC9929_DZ
void vtc9929_mian_ui(void)
{
	char t_buf[50] = {0};
    
    if(NULL == g_cur_type)
    {
        return;
    }
    
    sprintf(t_buf,"VTC9929   Ver:%s", /*g_cur_type->name,*/ g_cur_type->ver);
    
	if(sys_par.language == CHINESE)
	{
// 		GUI_SetFont(&GUI_FontHZ_FangZhengXingKai_24);
// 		GUI_DispStringAt((const char*)"长 盛 仪 器", 45, 0);
// 		GUI_SetFont(&GUI_FontHZ_SimSun_12);
// 		GUI_DispStringAt((const char*)"http://www.csallwin.com", 20, 52);
// 		GUI_DrawBitmap((const GUI_BITMAP*) &bmlogo, 10, 0);
        
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
        draw_short_cut_str("设置", "菜单", "测试", "帮助");
	}
	else
	{
// 		GUI_SetFont(&GUI_FontHZ_Garamond_16);
// 		// GUI_SetFont(&GUI_FontHZ_SimSun_16);
// 		GUI_DispStringAt((const char*)"Allwin Instrument", 45+20, 0+8);
// 		GUI_SetFont(&GUI_FontHZ_SimSun_12);
// 		GUI_DispStringAt((const char*)"http://www.csallwin.com", 20, 52);
// 		GUI_DrawBitmap((const GUI_BITMAP*) &bmallwin, 2, 2);
        
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
        draw_short_cut_str("Set", "Menu", "Test", "Help");
	}
    
    GUI_SetFont(&GUI_FontHZ_SimSun_12);
    GUI_DispStringAt((const char*)t_buf, 20+6*0, 28);/* 显示机型 */
    
    GUI_DispStringAt((const char*)gUpdateTime, 20, 40);/* 显示时间 */
    
    draw_menu_frame_line(0, 23, 200, 23);/* 画出菜单框架线 */
    
	ReFresh_LCD24064();
}
#endif
#ifdef TSK7630_DZ
void tsk7630_mian_ui(void)
{
	char t_buf[50] = {0};
    
    if(NULL == g_cur_type)
    {
        return;
    }
    
    sprintf(t_buf,"TSK7630   Ver:%s", /*g_cur_type->name,*/ g_cur_type->ver);
    
	if(sys_par.language == CHINESE)
	{
// 		GUI_SetFont(&GUI_FontHZ_FangZhengXingKai_24);
// 		GUI_DispStringAt((const char*)"长 盛 仪 器", 45, 0);
// 		GUI_SetFont(&GUI_FontHZ_SimSun_12);
// 		GUI_DispStringAt((const char*)"http://www.csallwin.com", 20, 52);
// 		GUI_DrawBitmap((const GUI_BITMAP*) &bmlogo, 10, 0);
        
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
        draw_short_cut_str("设置", "菜单", "测试", "帮助");
	}
	else
	{
// 		GUI_SetFont(&GUI_FontHZ_Garamond_16);
// 		GUI_DispStringAt((const char*)"Allwin Instrument", 45+20, 0+8);
// 		GUI_SetFont(&GUI_FontHZ_SimSun_12);
// 		GUI_DispStringAt((const char*)"http://www.csallwin.com", 20, 52);
// 		GUI_DrawBitmap((const GUI_BITMAP*) &bmallwin, 2, 2);
        
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
        draw_short_cut_str("Set", "Menu", "Test", "Help");
	}
    
    GUI_SetFont(&GUI_FontHZ_SimSun_12);
    GUI_DispStringAt((const char*)t_buf, 20+6*0, 28);/* 显示机型 */
    
    GUI_DispStringAt((const char*)gUpdateTime, 20, 40);/* 显示时间 */
    
    draw_menu_frame_line(0, 23, 200, 23);/* 画出菜单框架线 */
    
	ReFresh_LCD24064();
}
#endif
uint8_t custom_type_main_gui(void)
{
    uint8_t flag = 0;
    
    #ifdef LICHUN_DZ
    if(flag == 0)
    {
        if(g_cur_type->num == CS9906B_K)
        {
            flag = 1;
            li_chun_dingzhi_main_ui();//上海立春精密仪器定制机
        }
    }
    #endif
    
    #ifdef VTC9929_DZ
    if(flag == 0)
    {
        if(g_cur_type->num == VTC9929)
        {
            flag = 1;
            vtc9929_mian_ui();//开机主界面只只显示VTC9929 版本号 时间
        }
    }
    #endif
    
    #ifdef TSK7630_DZ
    if(flag == 0)
    {
        if(g_cur_type->num == TSK7630)
        {
            flag = 1;
            tsk7630_mian_ui();
        }
    }
    #endif
    
    return flag;
}
/*
 * 函数名：GUI_Draw_main
 * 描述  ：画出主界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_main(void)
{
    uint8_t flag = 0;
    
#if CUSTOM_TYPE_EN>0
    flag = custom_type_main_gui();
#endif
    
    if(flag == 0)
    {
        csallwin_mian_ui();
    }
}

/*
 * 函数名：MenuTitle
 * 描述  ：更新标题栏的信息 设置界面使用
 * 输入  ：which 模式
 * 输出  ：无
 * 返回  ：无
 */
void MenuTitle(u8 which)
{
	char dis[10] = {0};
	
	if(strlen((const char*)g_cur_file->name) > 9)
	{
		strncpy(dis, (const char*)g_cur_file->name, 7);
		strcat(dis, "..");
	}
	else
	{
		strcpy(dis, (const char*)g_cur_file->name);
	}
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    GUI_DispStringAt((const char*)"：" , TITLE_STEP_COLON_X, TITLE_Y);
	GUI_DispStringAt((const char*)"：" , TITLE_FILE_COLON_X, TITLE_Y);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"步骤" , TITLE_STEP_X, TITLE_Y);
		GUI_DispStringAt((const char*)"文件" , TITLE_FILE_X, TITLE_Y);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
        
        GUI_DispStringAt((const char*)"Step" , TITLE_STEP_X, -1);
		GUI_DispStringAt((const char*)"File" , TITLE_FILE_X, -1);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
    GUI_DispStringAt((const char*)"/"  , TITLE_STEP__X, TITLE_Y);
	
	GUI_DispStringAt((const char*)work_mode_buf[g_cur_file->work_mode] , TITLE_WORK_MODE_X, TITLE_Y);
	
	GUI_DispStringAt((const char*)dis, TITLE_FILE_NAME_X, TITLE_Y);
}
/*
 * 函数名：syn_test_menu_title
 * 描述  ：更新标题栏的信息 测试界面使用
 * 输入  ：which 模式
 * 输出  ：无
 * 返回  ：无
 */
void syn_test_menu_title(void)
{
	char dis[10] = {0};
	
	if(strlen((const char*)g_cur_file->name) > 7)
	{
		strncpy(dis, (const char*)g_cur_file->name, 5);
		strcat(dis, "..");
	}
	else
	{
		strcpy(dis, (const char*)g_cur_file->name);
	}
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"/"  , 1+2*6, 0);
	GUI_DispDecAt(cur_step, 1+0*6, 0, 2);
	GUI_DispDecAt(g_cur_file->total, 1+3*6, 0, 2);
	
	GUI_DispStringAt((const char*)work_mode_buf[g_cur_file->work_mode] , 1+6, 2+12*2);
	
	GUI_DispStringAt((const char*)mode_pool[GR_VOL_DROP_EN][cur_mode], 1, 1+12*1);
	
	GUI_DispStringAt((const char*)(const char*)dis, 1+5*6+5, 0);
}



/*
 * 函数名：test_menu_title
 * 描述  ：更新标题栏的信息 测试界面使用
 * 输入  ：which 模式
 * 输出  ：无
 * 返回  ：无
 */
// void test_menu_title(u8 which)
// {
// 	char dis[10] = {0};
// 	
// 	GUI_ClearRect (0, 0, 200-1, 12-1);
// 	
// 	if(strlen((const char*)g_cur_file->name) > 9)
// 	{
// 		strncpy(dis, (const char*)g_cur_file->name, 7);
// 		strcat(dis, "..");
// 	}
// 	else
// 	{
// 		strcpy(dis, (const char*)g_cur_file->name);
// 	}
// 	
// 	if(sys_par.allowance)
// 	{
// 		if(((float)(sys_par.used_res_num) / RES_TOTAL) > ((float)sys_par.allowance/10))
// 		{
// 			GUI_FillRect(0, 0, 200, 12);
// 			GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
// 		}
// 	}
// 	GUI_SetFont(&GUI_FontHZ_SimSun_12);
// 	GUI_DispStringAt((const char*)"：" , 4+4*6-1, 0);
// 	GUI_DispStringAt((const char*)"：" , 4+35+38+15+20+4*6, 0);
// 	GUI_DispDecAt(g_cur_step->one_step.com.step, 4+35, 0, 2);
// 	GUI_DispDecAt(g_cur_file->total, 4+35+18, 0, 2);
//     
// 	if(sys_par.language == CHINESE)
// 	{
// 		GUI_DispStringAt((const char*)"步骤" , 4, 0);
// 		GUI_DispStringAt((const char*)"文件" , 4+35+38+15+25-3, 0);
// 	}
// 	else
// 	{
// 		GUI_SetFont(&GUI_FontHZ_SimSun_13);
// 		GUI_DispStringAt((const char*)"Step" , 4+3, -1);
// 		GUI_DispStringAt((const char*)"File" , 4+35+38+15+25, -1);
// 		GUI_SetFont(&GUI_FontHZ_SimSun_12);
// 	}
// 	
// 	GUI_SetFont(&GUI_FontHZ_SimSun_12);
// 	GUI_DispStringAt((const char*)"/"  , 4+35+2*6, 0);
// 	
// 	GUI_DispStringAt((const char*)work_mode_buf[g_cur_file->work_mode] , 4+35+38-1, 0);
// 	
// 	GUI_DispStringAt((const char*)mode_pool[GR_VOL_DROP_EN][which], 4+35+38+15-2, 0);
// 	
// 	GUI_DispStringAt((const char*)(const char*)dis, 4+35+38+15+20+6*5+3, 0);
// 	
// 	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
// }

/*
 * 函数名：draw_port_for_set
 * 描述  ：画出端口数据信息 设置界面使用
 * 输入  ：which 模式
 * 输出  ：无
 * 返回  ：无
 */
void draw_port_for_set(TEST_PORT *p)
{
	if(p != NULL)
	{
		uint16_t *pint16 = (void*)p;
		int8_t i = 0;
		int32_t port_num = type_spe.port_num;
		int32_t offset = 0;
        uint8_t x = 10;
        uint8_t y = 35;
        uint8_t index = 0;
        uint8_t value = 0;
        
		
        if(port_num == PORT_NUM14)
        {
            GUI_DispStringAt((const char*)"01-08:", x, y);
            GUI_DispStringAt((const char*)"09-14:", x, y + 12);
            offset = 6 * 6;
            
            for(i = 0; i < port_num; i++)
            {
                index = i % 8;
                
                value = ((pint16[i / 8])>>(index*2)) & 3;
                x = offset + UI_SET_PORTS_BASE_X + UI_SET_PORT_X_O * index;
                y = UI_SET_PORTS_BASE_Y + UI_SET_PORT_Y_O * ( i / 8);
                GUI_DispCharAt("XLH"[value], x, y);
            }
        }
        else
        {
            if(port_num == PORT_NUM4)
            {
                offset = 15*4;
            }
            else if(port_num == PORT_NUM8)
            {
                offset = 0;
            }
            else if(port_num == 0)
            {
                /* 非多路 */
            }
            else
            {
                return;
            }
            
            GUI_SetFont(&GUI_FontHZ_SimSun_12);
            pint16 = (uint16_t *)p;
            
            for(i = 0; i < port_num; i++)
            {
                GUI_DispCharAt("XLH"[((*pint16)>>(i*2)) & 3], offset + 15 + 15*i, 30);
                GUI_DispCharAt("12345678"[i], offset + 15 + 15*i, 30+12);
            }
        }
	}
}
/*
 * 函数名：draw_port
 * 描述  ：画出端口数据信息 测试界面使用
 * 输入  ：which 模式
 * 输出  ：P x y
 * 返回  ：无
 */
static void draw_port(TEST_PORT *p)
{
	if(p != NULL)
	{
		uint16_t *pint16 = (uint16_t *)p;
		int8_t i = 0;
		int32_t port_num = type_spe.port_num;
		int32_t offset = 0;
        uint8_t index = 0;
        uint16_t value = 0;
        uint8_t x = 90;
        uint8_t y = 41;
		
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
        GUI_ClearRect (100, 42, 200-1, 63);
        
        if(port_num == PORT_NUM14)
        {
            GUI_DispStringAt((const char*)"01-08:", x, y);
            GUI_DispStringAt((const char*)"09-14:", x, y + 12);
            offset = 6 * 6;
            
//             for(i = 0; i < 8; i++)
//             {
//                 GUI_DispCharAt("XLH"[0], offset + x + 9 * i, y);
//                 GUI_DispCharAt("XLH"[0], offset + x + 9 * i, y + 12);
//             }
            
            
//             for(i = 0; i < 8; i++)
//             {
//                 GUI_DispCharAt("XLH"[0], offset + x + 9 * i, y);
//                 GUI_DispCharAt("XLH"[0], offset + x + 9 * i, y + 12);
//             }
            
            for(i = 0; i < port_num; i++)
            {
                index = i % 8;
                
                value = ((pint16[i / 8])>>(index*2)) & 3;
                x = offset + UI_TEST_PORTS_BASE_X + UI_TEST_PORT_X_O * index;
                y = UI_TEST_PORTS_BASE_Y + UI_SET_PORT_Y_O * ( i / 8);
                
                GUI_DispCharAt("XLH"[value], x, y);
            }
        }
        else
        {
            if(port_num == PORT_NUM4)
            {
                offset = 14*2;
            }
            else if(port_num == PORT_NUM8)
            {
                offset = 0;
            }
            else if(port_num == 0)
            {
                /* 非多路 */
                if(type_spe.dcw_big_cap_en == DISABLE && cur_mode != BBD)
                {
                    if(cur_mode == ACW || cur_mode == DCW)
                    {
                        GUI_DispStringAt((const char*)sys_sw_status_buf[ARC_STATUS][!!cur_arc_gear], 110, 42);
                    }
                    GUI_DispStringAt((const char*)sys_sw_status_buf[GFI_STATUS][sys_par.is_gfi_protect], 110, 52);
                }
            }
            
            pint16 = (uint16_t *)p;
            for(i = 0; i < port_num; i++)
            {
                GUI_DispCharAt("XLH"[((*(pint16 + i / 8)) >> ((i % 8) * 2)) & 3], offset + x + 14 * i, y);
                GUI_DispCharAt("12345678"[i], offset + x + 14*i, y+12);
            }
        }
	}
}
/*
 * 函数名：dis_port
 * 描述  ：更新端口状态信息 测试界面使用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void dis_port(void)
{
	TEST_PORT *pport = NULL;
	
	switch(g_cur_step->one_step.com.mode)
	{
		case ACW:
			pport = &g_cur_step->one_step.acw.port;
			break;
		case DCW:
			pport = &g_cur_step->one_step.dcw.port;
			break;
		case IR:
			pport = &g_cur_step->one_step.ir.port;
			break;
		case GR:
			break;
		case BBD:
			pport = &g_cur_step->one_step.bbd.port;
			break;
		case CC:
			pport = &g_cur_step->one_step.cc.port;
			break;
	}
	draw_port(pport);
}

/*
 * 函数名：gui_draw_set_sys_cur_limit
 * 描述  ：设置支路电流时说明
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_set_sys_cur_limit(void)
{
	if(sys_par.language == CHINESE)
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
		GUI_DispStringAt((const char*)"说明：支路电流", 4, 15);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Illus.:Branch CUR.", 4, 15);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
}
/*
 * 函数名：draw_short_cut_bm
 * 描述  ：右边快捷键栏 图标显示函数
 * 输入  ：type 根据不同的类型决定显示那些图标
 * 输出  ：无
 * 返回  ：无
 */
void draw_short_cut_bm(uint8_t type)
{
	clear_menu();
	switch(type)
	{
		case UD_B:
		case UD_B_RE:
			GUI_DrawBitmap((const GUI_BITMAP*) &bmup         , 215, 2);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmdown       , 215, 20);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback       , 210, 50);
			break;
		case UDSB:
		case UDSB_RE:
			GUI_DrawBitmap((const GUI_BITMAP*) &bmup	     , 215, 2 );
			GUI_DrawBitmap((const GUI_BITMAP*) &bmdown       , 215, 20);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmshortcut   , 210, 34);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback       , 210, 50);
			break;
		case UDRB:
		case UDRB_RE:
			GUI_DrawBitmap((const GUI_BITMAP*) &bmup         , 215, 2 );
			GUI_DrawBitmap((const GUI_BITMAP*) &bmdown       , 215, 20);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback       , 210, 50);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift , 210, 32);
			break;
		case UDR_:
		case UDR_RE:
			GUI_DrawBitmap((const GUI_BITMAP*) &bmup         , 215, 2 );
			GUI_DrawBitmap((const GUI_BITMAP*) &bmdown       , 215, 20);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift , 210, 32);
			break;
		case ___B:
		case ___B_RE:
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback       , 210, 50);
			break;
		case LRDB:
		case LRDB_RE:
			GUI_DrawBitmap((const GUI_BITMAP*) &bmleftshift, 210, 0 );
			GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 16);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmdel, 210, 34);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
			break;
        case __RB:
        case __RB_RE:
			GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift , 210, 32);
			GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
            break;
	}
	
	if(type%2)
	{
		ReFresh_LCD24064();
	}
}

/*
 * 函数名：draw_short_cut_str
 * 描述  ：右边快捷键栏 文字显示
 * 输入  ：str1 str2 str3 str4 分别代表4个快捷项
 * 输出  ：无
 * 返回  ：无
 */
void draw_short_cut_str(uint8_t *str1, uint8_t *str2, uint8_t *str3, uint8_t *str4)
{
// 	clear_menu();
    
    if(str1 != NULL)
    {
        clear_menu_n(1);
		GUI_DispStringAt_BTP_STR_((const char*)str1, 200, 240, 2, 6);
    }
    if(str2 != NULL)
    {
        clear_menu_n(2);
		GUI_DispStringAt_BTP_STR_((const char*)str2, 200, 240, 18, 6);
    }
    if(str3 != NULL)
    {
        clear_menu_n(3);
		GUI_DispStringAt_BTP_STR_((const char*)str3, 200, 240, 34, 6);
    }
    if(str4 != NULL)
    {
        clear_menu_n(4);
		GUI_DispStringAt_BTP_STR_((const char*)str4, 200, 240, 50, 6);
    }
}

/*
 * 函数名：draw_menu_frame_line
 * 描述  ：画出菜单框架线 并附带画一条直线 坐标通过参数出入
 * 输入  ：x1 x2 y 指定这条直线的坐标 如果 x1>x2即不再画这条线
 * 输出  ：无
 * 返回  ：无
 */
void draw_menu_frame_line(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2)
{
    if(!(x1 == x2 && y1==y2))
    {
        GUI_DrawLine(x1, y1, x2, y2);
    }
    
	GUI_DrawLine(200, 0   , 200  , 64-1);
	GUI_DrawLine(200, 16-1, 240-1, 15  );
	GUI_DrawLine(200, 32-1, 240-1, 32-1);
	GUI_DrawLine(200, 48-1, 240-1, 48-1);
}
/*
 * 函数名：gui_draw_menu_sys
 * 描述  ：菜单设置界面下菜单项
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_menu_sys(MENU_INFO* p_sys_menu, uint8_t lon)
{
    int32_t i = 0;
    uint8_t index = 0;
    
    if(sys_par.language == CHINESE)
    {
        index = 0;
        GUI_SetFont(&GUI_FontHZ_SimSun_12);
    }
    else
    {
        index = 1;
        GUI_SetFont(&GUI_FontHZ_SimSun_13);
    }
    
    for(i = 0; i < lon; i++)
    {
        GUI_DispStringAt((const char*)p_sys_menu[i].name[index],
            SYS_MENU_X + SYS_MENU_OX1 * (i > 3) + SYS_MENU_OX2 * (i > 7), 2 + SYS_MENU_OY * (i % 4));
    }
    
    GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
    draw_menu_frame_line(0, 0, 0, 0);/* 画出菜单框架线 */
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 3, 4);
	draw_short_cut_bm(UDRB_RE);
}

/*
 * 函数名：gui_draw_menu
 * 描述  ：显示一个最基本的界面框架 cs99xx_type.c用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_menu(void)
{
	clear_();
    
	draw_menu_frame_line(0, 0, 0, 0);/* 画出菜单框架线 */
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 1, 4);
    draw_short_cut_bm(UDRB_RE);
}

void draw_custom_type_gui(void)
{
	clear_();
    
	draw_menu_frame_line(0, 0, 0, 0);/* 画出菜单框架线 */
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 1, 4);
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"：", 5+6*4, 16);
	GUI_DispStringAt((const char*)"：", 5+6*4, 32);

    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"原型号  ：", 5, 16);
		GUI_DispStringAt((const char*)"定制型号：", 5, 32);
	}
	else
	{
		GUI_DispStringAt((const char*)"Cur.", 5, 16);
		GUI_DispStringAt((const char*)"Set", 5, 32);
	}
    
	draw_menu_frame_line(0, 0, 0, 0);/* 画出菜单框架线 */
    
    draw_short_cut_bm(LRDB);
}
/*
 * 函数名：gui_draw_help_1
 * 描述  ：帮助菜单1
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_help_1(void)
{
	GUI_ClearRect (0, 0, 200-1, 64-1);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"仪器操作注意事项", 40, 2);
		GUI_DispStringAt((const char*)"1/3", 50+120, 2);

		GUI_DispStringAt((const char*)"请勿触摸仪器输出端口及其测试物", 8, 18);
		GUI_DispStringAt((const char*)"仪器如有异常状况，必须关闭电源", 8, 34);
		GUI_DispStringAt((const char*)"仪器必须妥善接地，请详阅说明书", 8, 50);
	}
	else
	{
		GUI_SetFont(&GUI_Font13_1);
		GUI_DispStringAt((const char*)"Operation Notice", 40+8, 2);
		GUI_DispStringAt((const char*)"1/3", 50+120, 2);
		
		GUI_DispStringAt((const char*)"Don't touch H.V.port and DUT", 8, 18);
		GUI_DispStringAt((const char*)"Power off once an exception occrred", 8, 34);
		GUI_DispStringAt((const char*)"The instrument must be well grounded", 8, 50);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
	draw_menu_frame_line(40, 15, 135, 15);/* 画出菜单框架线 */
    draw_short_cut_bm(UD_B_RE);
}

/*
 * 函数名：gui_draw_help_2
 * 描述  ：帮助菜单1
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_help_2(void)
{
	GUI_ClearRect (0, 0, 200-1, 64-1);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"系统图标说明", 53, 2);
		GUI_DispStringAt((const char*) "2/3", 50+120, 2);
		GUI_DispStringAt((const char*)"上翻键", 40, 18);
		GUI_DispStringAt((const char*)"下翻键", 40+100, 18);

		GUI_DispStringAt((const char*)"快捷键", 40, 34);
		GUI_DispStringAt((const char*)"右移键", 40+100, 34);

		GUI_DispStringAt((const char*)"删除键", 40, 50);
		GUI_DispStringAt((const char*)"返回键", 40+100, 50);
	}
	else 
	{
		GUI_SetFont(&GUI_Font13_1);
		GUI_DispStringAt((const char*)"System Icon", 53+5, 2);
		GUI_DispStringAt((const char*) "2/3", 50+120, 2);
		GUI_DispStringAt((const char*)"Up", 40, 18);
		GUI_DispStringAt((const char*)"Down", 40+100, 18);

		GUI_DispStringAt((const char*)"Shortcut", 40, 34);
		GUI_DispStringAt((const char*)"Cycle", 40+100, 34);

		GUI_DispStringAt((const char*)"Delete", 40, 50);
		GUI_DispStringAt((const char*)"Return", 40+100, 50);
	}
    
    GUI_DrawBitmap((const GUI_BITMAP*) &bmup    , 15, 18);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown  , 15+100, 18);

	GUI_DrawBitmap((const GUI_BITMAP*) &bmshortcut  , 10, 34);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 10+100, 32);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdel   , 10, 50);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback  , 10+100, 50);
    
	draw_menu_frame_line(40, 15, 135, 15);/* 画出菜单框架线 */
    draw_short_cut_bm(UD_B_RE);
}

/*
 * 函数名：gui_draw_help_3
 * 描述  ：帮助菜单1
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_help_3 (void)
{
	GUI_ClearRect (0, 0, 200-1, 64-1);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"系统快捷键说明", 45, 2);
		GUI_DispStringAt((const char*)"3/3", 40+130, 2);
		
		GUI_DispStringAt((const char*)"有效范围: ", 10, 20);
		GUI_DispString("主界面和帮助界面下");
		
		GUI_DispStringAt((const char*)"Enter+0: ", 10, 36);
		GUI_DispString("系统中英文快速切换键");
		
		GUI_DispStringAt((const char*)"Enter+1: ", 10, 52);
		GUI_DispString("系统初始密码快速恢复键");
	}
	else
	{
		GUI_SetFont(&GUI_Font13_1);
		GUI_DispStringAt((const char*)"System shortcutkeys", 45-5, 2);
		GUI_DispStringAt((const char*)"3/3", 40+130, 2);

		GUI_DispStringAt((const char*)"Valid range: ", 10, 20);
		GUI_DispString("main and help interfaces");

		GUI_DispStringAt((const char*)"Enter+0: ", 10, 36);
		GUI_DispString("language quick shift key");

		GUI_DispStringAt((const char*)"Enter+1: ", 10, 52);
		GUI_DispString("init.password recover key");
	}
	
	draw_menu_frame_line(40, 15, 135, 15);/* 画出菜单框架线 */
    draw_short_cut_bm(UD_B_RE);
}


/*
 * 函数名：gui_draw_file_management
 * 描述  ：文件管理界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_file_management(void)
{
	clear_();
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"01.", 15, 2);
// 	GUI_DispStringAt((const char*)"SAMPLE1"	, 15+20, 2);
// 	GUI_DispStringAt((const char*)"N"	, 15+20+58, 2);
// 	GUI_DispStringAt((const char*)"29-01-11 10:59", 15+20+58+15, 2);
	
	GUI_DispStringAt((const char*)"02.", 15, 14);
// 	GUI_DispStringAt((const char*)"SAMPLE2"	, 15+20, 14);
// 	GUI_DispStringAt((const char*)"N"	, 15+20+58, 14);
// 	GUI_DispStringAt((const char*)"29-01-11 10:59", 15+20+58+15, 14);
	
	GUI_DispStringAt((const char*)"03.", 15, 26);
// 	GUI_DispStringAt((const char*)"SAMPLE3"	, 15+20, 26);
// 	GUI_DispStringAt((const char*)"N"	, 15+20+58, 26);
// 	GUI_DispStringAt((const char*)"29-01-11 10:59", 15+20+58+15, 26);
	
	GUI_DispStringAt((const char*)"04.", 15, 38);
// 	GUI_DispStringAt((const char*)"SAMPLE4"	, 15+20, 38);
// 	GUI_DispStringAt((const char*)"N"	, 15+20+58, 38);
// 	GUI_DispStringAt((const char*)"29-01-11 10:59", 15+20+58+15, 38);
	
	GUI_DispStringAt((const char*)"05.", 15, 50);
// 	GUI_DispStringAt((const char*)"SAMPLE5"	, 15+20, 50);
// 	GUI_DispStringAt((const char*)"N"	, 15+20+58, 50);
// 	GUI_DispStringAt((const char*)"29-01-11 10:59", 15+20+58+15, 50);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 4);
	draw_menu_frame_line(0, 0, 0, 0);/* 画出菜单框架线 */
    draw_short_cut_bm(UDSB_RE);
}

/*
 * 函数名：gui_draw_communication_interface
 * 描述  ：通信界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_communication_interface(MENU_INFO* pmenu_info)
{
    int32_t i = 0;
    uint8_t opt_num = 0;
    uint8_t flag_opt[5] = {0};
    
    opt_num = define_interface_config(flag_opt);
    
	clear_();
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
    for(i = 0; i < opt_num; i++)
    {
        GUI_DispStringAt((const char*)pmenu_info[flag_opt[i]].name[sys_par.language != CHINESE], 20, 2+16*i);
    }
    
    draw_menu_frame_line(0, 0, 0, 0);/* 画出菜单框架线 */
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 3, 4);
    
    draw_short_cut_bm(UDRB_RE);
}

/*
 * 函数名：gui_draw_communication_plc
 * 描述  ：PLC通信开关
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_communication_plc(void)
{
	clear_();
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"PLC", 100, 2);
	
	GUI_DispStringAt((const char*) "PLC控制:", 3, 18);
	GUI_DispStringAt((const char*)"打开", 3+80, 18);
	GUI_DispStringAt((const char*)"关闭", 3+80+50, 18);
	
	GUI_DispStringAt((const char*)"说   明:", 3, 32);
	GUI_DispStringAt((const char*)"通过PLC接口进行控制，",3+50, 32);
	GUI_DispStringAt((const char*)"只有复位键有效。", 3+50, 46);
	
    draw_menu_frame_line(20, 14, 180, 14);/* 画出菜单框架线 */
	
 	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,65, 20 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,65+50, 20 );
    
    draw_short_cut_bm(__RB_RE);
}
/*
 * 函数名：gui_draw_communication_agreement
 * 描述  ：通信开关
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_communication_agreement(void)
{
	clear_();
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"通信", 100, 2);
	
	GUI_DispStringAt((const char*)"通讯控制:", 3, 18);
	GUI_DispStringAt((const char*)"打开", 3+70, 18);
	GUI_DispStringAt((const char*) "关闭", 3+70+40, 18);
	
	GUI_DispStringAt((const char*)"波 特 率:", 3, 33);
	GUI_SetFont(&GUI_Font10_1);
	GUI_DispStringAt((const char*)"9600", 3+70, 33);
	GUI_DispStringAt((const char*)"14400", 3+70+40, 33);
 	GUI_DispStringAt((const char*)"19200", 3+70+40+50, 33);
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"本机地址:", 3, 48);
	GUI_SetFont(&GUI_Font10_1);
	GUI_DispStringAt((const char*)"001", 3+60, 48);
	GUI_DispStringAt((const char*)"(1-255)",3+60+30, 48);
	
    draw_menu_frame_line(20, 14, 180, 14);/* 画出菜单框架线 */
	
 	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,60, 20 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,60+40, 20 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,60, 34 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,60+40, 34 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,60+40+50, 34 );
    
    draw_short_cut_bm(UDRB_RE);
}

/*
 * 函数名：gui_draw_communication_agreement
 * 描述  ：通信开关
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_password_manage(void)
{
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"：", 5+6*8, 2);	
	GUI_DispStringAt((const char*)"：", 5+6*8, 25);	
	GUI_DispStringAt((const char*)"：", 5+6*8, 46);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"原始密码", 5, 2);	
		GUI_DispStringAt((const char*)"新设密码", 5, 25);	
		GUI_DispStringAt((const char*)"确认密码", 5, 46);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Old", 5+10, 2);
		GUI_DispStringAt((const char*)"New", 5+10, 25);
		GUI_DispStringAt((const char*)"Enter", 5+10, 46);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
	draw_menu_frame_line(0, 0, 0, 0);/* 画出菜单框架线 */
	
    draw_short_cut_bm(LRDB_RE);
}

/*
 * 函数名：gui_draw_time_manage
 * 描述  ：系统时间设置界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_time_manage(void)
{
	char strtemp[20];
    
	clear_();
	strcpy(strtemp, gUpdateTime);
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"：", 5+6*4, 16);
	GUI_DispStringAt((const char*)"：", 5+6*4, 32);
	GUI_DispStringAt((const char*)strtemp, 5+40, 17);
	GUI_DispStringAt((const char*)strtemp, 5+40, 32);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"当前：", 5, 16);
		GUI_DispStringAt((const char*)"新设：", 5, 32);
		GUI_DispStringAt((const char*)"重置时间：0+1 组合键", 5, 32 + 15);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Cur.", 5, 16);
		GUI_DispStringAt((const char*)"Set", 5, 32);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
		GUI_DispStringAt((const char*)"ResetTime:0+1 CombinationKey", 5, 32 + 15);
	}
    
	draw_menu_frame_line(0, 0, 0, 0);/* 画出菜单框架线 */
    
    draw_short_cut_bm(UDRB_RE);
}

/*
 * 函数名：gui_draw_offset
 * 描述  ：偏移测试界面显示
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_offset(void)
{
	clear_();
	
	GUI_DrawLine(0  , 12  , 200  , 12  );
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"N", 150+7, 15);
	GUI_DispStringAt((const char*)"ACW", 150+7+12, 15);
    
	if(g_cur_step->one_step.com.mode == GR)
	{
		GUI_DispStringAt((const char*)"00.00A", 150+2, 27);
	}
	else
	{
		GUI_DispStringAt((const char*)"0.050kV", 150+2, 27);
	}
    
	GUI_DispStringAt((const char*)"3.0s", 150+2+12, 38);
	
	dis_test_status(TEST_WAIT_INDEX, &GUI_FontHZ_SimSun_12);
	
    draw_menu_frame_line(145, 18, 145, 64-5);/* 画出菜单框架线 */
    
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+3);
	
	draw_short_cut_bm(UDSB_RE);
}
/*
 * 函数名：gui_draw_bbd
 * 描述  ：BBD测试界面显示
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_bbd(void)
{
	clear_();
	
	GUI_DrawLine(0  , 12  , 200  , 12  );
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"：" , 4+35+38+15+20+4*6, 0);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"文件" , 4+35+38+15+25-3, 0);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"File" , 4+35+38+15+25, -1);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	GUI_DispStringAt((const char*)"N", 150+7, 15);
	GUI_DispStringAt((const char*)"ACW", 150+7+12, 15);
	if(g_cur_step->one_step.com.mode == GR)
	{
		GUI_DispStringAt((const char*)"00.00A", 150+2, 27);
	}
	else
	{
		GUI_DispStringAt((const char*)"0.050kV", 150+2, 27);
	}
	GUI_DispStringAt((const char*)"3.0s", 150+2+12, 39);
	
	dis_test_status(TEST_WAIT_INDEX, &GUI_FontHZ_SimSun_12);
    
    draw_menu_frame_line(145, 18, 145, 64-5);/* 画出菜单框架线 */
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+3);
	
	draw_short_cut_bm(UDSB_RE);
}

/*
 * 函数名：gui_draw_test_result
 * 描述  ：测试结果显示界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_test_result(void)
{
	clear_();/* 屏幕不会刷新 所有不闪烁 */
	MenuTitle(NUL);
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"0001.", 12, 15);
	GUI_DispStringAt((const char*)"0002.", 12, 27);
	GUI_DispStringAt((const char*)"0003.", 12, 39);
	GUI_DispStringAt((const char*)"0004.", 12, 51);
// 	GUI_DispStringAt((const char*)"----" , 20+30, 51);
// 	GUI_DispStringAt((const char*)"----" , 20+30+80, 51);
// 	GUI_DispStringAt((const char*)"P"    , 20+30+60+80, 51);
	
	draw_menu_frame_line(0, 12, 200, 12);/* 画出菜单框架线 */
    
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+3);
	
	draw_short_cut_bm(UDSB_RE);
}

/*
 * 函数名：gui_draw_keyboard_lock_manage
 * 描述  ：键盘锁显示界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_keyboard_lock_manage(void)
{
	clear_();
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"：", 3+6*7, 5);
	GUI_DispStringAt((const char*)"：", 3+6*7, 20);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"键盘锁：", 3, 5);
		GUI_DispStringAt((const char*)"打开", 3+70, 5);
		GUI_DispStringAt((const char*)"关闭", 3+70+50, 5);
        
		GUI_DispStringAt((const char*)"说  明：", 3, 20);
		GUI_DispStringAt((const char*)"键盘锁决定是否允许密码", 3+50, 20);
		GUI_DispStringAt((const char*)"输入，您可以设置为打开", 3+50, 34);
		GUI_DispStringAt((const char*)"或者关闭。", 3+50, 48);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"LockKey", 3, 5);
		GUI_DispStringAt((const char*)"Open", 3+70, 5);
		GUI_DispStringAt((const char*)"Close", 3+70+50, 5);

		GUI_DispStringAt((const char*)"Manual", 3, 20);
		GUI_DispStringAt((const char*)"Lock key deciding whe-", 3+55, 20);
		GUI_DispStringAt((const char*)"ther permitting the in-", 3+55, 34);
		GUI_DispStringAt((const char*)"put of password.", 3+55, 48);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
    draw_menu_frame_line(0, 0, 0, 0);/* 画出菜单框架线 */
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, 3+60, 2+5+1 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, 3+60+50, 2+5+1 );
	
	draw_short_cut_bm(UDRB_RE);
}

/*
 * 函数名：gui_draw_recover
 * 描述  ：恢复默认值显示界面
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_recover(void)
{
	clear_();
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"：", 3+6*7, 5);
	GUI_DispStringAt((const char*)"：", 3+6*7, 20);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"恢复默认值：", 3, 5);
		GUI_DispStringAt((const char*)"确定", 3+70 + 12, 5);
		GUI_DispStringAt((const char*)"取消", 3+70+50 + 12, 5);
        
		GUI_DispStringAt((const char*)"说  明：", 3, 20);
		GUI_DispStringAt((const char*)"恢复默认值，仪器的数据", 3+50, 20);
		GUI_DispStringAt((const char*)"将会丢失！", 3+50, 34);
// 		GUI_DispStringAt((const char*)"或者关闭。", 3+50, 48);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Recover", 3, 5);
		GUI_DispStringAt((const char*)"Enter", 3+70+12, 5);
		GUI_DispStringAt((const char*)"Cancel", 3+70+50+12, 5);
        
		GUI_DispStringAt((const char*)"Manual", 3, 20);
		GUI_DispStringAt((const char*)"Recover will make ", 3+55, 20);
		GUI_DispStringAt((const char*)"user info. lost!", 3+55, 34);
// 		GUI_DispStringAt((const char*)"put of password.", 3+55, 48);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
    draw_menu_frame_line(0, 0, 0, 0);/* 画出菜单框架线 */
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, 3+60 + 12, 2+5+1 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, 3+60+50 + 12, 2+5+1 );
	
	draw_short_cut_bm(UDRB_RE);
}

/*
 * 函数名：gui_draw_test
 * 描述  ：测试界面显示
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_draw_test (void)
{
	uint8_t X	= TEST_ORIGIN_X;
	uint8_t Y	= TEST_ORIGIN_Y;
	uint8_t O	= TEST_UI_OFFSET;
	uint8_t CW	= 9;	/* 单字符宽度 */
    
// 	MenuTitle(NUL);
	
	/* 注释掉是为了去掉 显示的闪烁问题 */
// 	GUI_SetFont(&GUI_FontHZ_SimSun_15);
// 	GUI_DispStringAt((const char*)"0.050"	, X, Y);//输出电压
// 	GUI_DispStringAt((const char*)"0.500"	, X+11*CW, Y);//输出电流
// 	GUI_DispStringAt((const char*)"3.0"	, X+2*CW, Y+O);//同步时间
	
	GUI_SetFont(&GUI_FontHZ_SimSunF_15);
	GUI_DispStringAt((const char*)"kV", X+CW*5+1, Y);
	GUI_DispStringAt((const char*)"s"	, X+CW*5, Y+O);
// 	GUI_DispStringAt((const char*)"mA", X+CW*16, Y);
    
	GUI_SetFont(&GUI_FontHZ_SimSun_16);//小四
	GUI_ClearRect (GUI_GEN_STATUS_X1, GUI_GEN_STATUS_Y, GUI_GEN_STATUS_X2, GUI_GEN_STATUS_Y + GUI_GEN_STATUS_CH);
    
    GUI_DispStringAt_BTP_STR_(status_str[TEST_WAIT_INDEX][!sys_par.language],
        GUI_GEN_STATUS_X1, GUI_GEN_STATUS_X2, GUI_GEN_STATUS_Y, GUI_GEN_STATUS_CW);
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
    draw_menu_frame_line(0, 12, 200, 12);/* 画出菜单框架线 */
    
	GUI_DrawLine(200 - 6 * 5 - 2, 12   , 200 - 6 * 5 - 2, 30);
	GUI_DrawLine(200 - 6 * 5 - 2, 30   , 200  , 30);
	
    if(g_comm_status == LOCAL_COMM)
    {
        draw_short_cut_bm(UDSB);
    }
    else
    {
        draw_short_cut_str("", "", (uint8_t*)RT_STRING("远控", "Remote"), "");
    }
}

/*
 * 函数名：gui_drow_test_syn
 * 描述  ：多路同步测试界面显示
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void gui_drow_test_syn (void)
{
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
// 	GUI_DispStringAt       ("01/01", 1, 0);
// 	GUI_DispStringAt       ("DEFAULT", 1+35, 0);
// 	GUI_DispStringAt       ("ACW", 1+0, 1+12*1);
// 	GUI_DispStringAt       ("G", 1+6, 1+12*2);
	
// 	GUI_SetFont(&GUI_FontHZ_SimSun_u_14);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt       ("kV", 30+6*5, 13+10*0);
	GUI_DispStringAt       ("mA", 30+6*5, 13+10*1);
	GUI_DispStringAt       ("s" , 30+6*5, 13+10*2);
	
// 	GUI_DispStringAt       ("0.050", 30, 13+10*0);//输出电压
// 	GUI_DispStringAt       ("0.500", 30, 13+10*1);//输出电流
// 	GUI_DispStringAt       ("3.0", 30+6*2, 13+10*2);//同步时间
	
// 	GUI_SetFont(&GUI_FontHZ_SimSun_16);//小四
// 	GUI_DispStringAt       ("等待测试", 10, 45);//等待测试
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
// 	GUI_DispStringAt       ("A-B:", 60+25, 3+12*0);
// 	GUI_DispStringAt       ("C-D:", 60+25, 3+12*1);
// 	GUI_DispStringAt       ("E-F:", 60+25, 3+12*2);
// 	GUI_DispStringAt       ("G-H:", 60+25, 3+12*3);
// 	GUI_DispStringAt       ("I-J:", 60+25, 3+12*4);
	
// 	GUI_DispStringAt       ("A-B:0.500mA 0.500mA", 60+25, 3+12*0);
// 	GUI_DispStringAt       ("C-D:0.500mA 0.500mA", 60+25, 3+12*1);
// 	GUI_DispStringAt       ("E-F:0.500mA 0.500mA", 60+25, 3+12*2);
// 	GUI_DispStringAt       ("G-H:0.500mA 0.500mA", 60+25, 3+12*3);
// 	GUI_DispStringAt       ("I-J:0.500mA 0.500mA", 60+25, 3+12*4);
	
	GUI_DrawLine         (20	, 12	, 20	, 12+13*2);
	GUI_DrawLine         (0		, 12+13*1	, 0+20	, 12+13*1);
// 	GUI_DrawLine         (8	, 12	, 8	, 12+11);
	GUI_DrawLine         (0		, 12+13*2	, 0+20	, 12+13*2);
	GUI_DrawLine         (82  , 0  , 82  , 63  );
    
    draw_menu_frame_line(0, 12, 82, 12);/* 画出菜单框架线 */
    
    draw_short_cut_bm(UDSB);
}



/*
 * 函数名：prompt_recover_pw
 * 描述  ：当用户按下ENTER+1组合键时提示用户系统密码已经还原为默认值 该函数被help_subtask,main_task调用
 * 输入  ：uint8_t s 区分在不同状态下给用户的一些提示信息
 * 输出  ：无
 * 调用函数：GUI_SetFont，GUI_DispStringAt，GUI_DrawLine，GUI_ClearRect，
 *			OSQPend，GUI_DrawBitmap
 * 被调函数：test_subtask
 */
void prompt_recover_pw(void)
{
	uint8_t X0	= 40;
	uint8_t X1	= 165;
	uint8_t Y0	= 8;
	uint8_t Y1	= 58;
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"系统密码已经还", 0+X0+35, Y0+4);
		GUI_DispStringAt((const char*)"原为初始密码！", 0+X0+35, Y0+4+14);
		GUI_DispStringAt((const char*)"请参考说明书。", 0+X0+35, Y0+4+14+14);
	}
	else
	{
		GUI_DispStringAt((const char*)"System password", 0+X0+35, Y0+2);
		GUI_DispStringAt((const char*)"has recovered!", 0+X0+35, Y0+4+10);
		GUI_DispStringAt((const char*)"Refer to the", 0+X0+35, Y0+4+10+10);
		GUI_DispStringAt((const char*)"manual.", 0+X0+35, Y0+4+10+10+10);
	}
	
	GUI_DrawLine         (X0, Y0, X1, Y0);
	GUI_DrawLine         (X0, Y1, X1, Y1);
	GUI_DrawLine         (X0, Y0, X0, Y1);
	GUI_DrawLine         (X1, Y0, X1, Y1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, X0+3, Y0+5);
	LCD_REFRESH();
	
	off_keyboard();
	OSTimeDlyHMSM(0,0,2,0);	/* 提示框显示2s */
	on_keyboard();
	
	GUI_ClearRect (X0, Y0, X1, Y1);
}

/*
 * 函数名：dis_program_info
 * 描述  ：显示烧录文件的时间信息
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void dis_program_info(void)
{
	uint8_t X0	= 40;
	uint8_t Y0	= 8;
	uint8_t X1	= 180;
	uint8_t Y1	= 58;
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"日期:"__DATE__, 0+X0+35, Y0+4);
		GUI_DispStringAt((const char*)"时间:"__TIME__, 0+X0+35, Y0+4+14);
		GUI_DispStringAt((const char*)"按任意键继续" , 0+X0+35, Y0+4+14+18);
	}
	else
	{
		GUI_DispStringAt((const char*)"DATE:"__DATE__, 0+X0+35, Y0+4);
		GUI_DispStringAt((const char*)"Time:"__TIME__, 0+X0+35, Y0+4+14);
		GUI_DispStringAt((const char*)"Press any key.", 0+X0+35, Y0+4+14+18);
	}
	
    GUI_DrawRect(X0, Y0, X1, Y1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, X0+3, Y0+15);
	LCD_REFRESH();
    
	while(scan_keyboard() == KEY_NONE);
    
	GUI_ClearRect (X0, Y0, X1, Y1);
}

void dis_interface_info(void)
{
	uint8_t X0	= 40;
	uint8_t Y0	= 8;
	uint8_t X1	= 180;
	uint8_t Y1	= 58;
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"软件版本:v1.19.01", 0+X0+35, Y0+4);
		GUI_DispStringAt((const char*)"硬件版本:v1.9.1", 0+X0+35, Y0+4+14);
	}
	else
	{
		GUI_DispStringAt((const char*)"Soft ver:", 0+X0+35, Y0+4);
		GUI_DispStringAt((const char*)"Hard ver:", 0+X0+35, Y0+4+14);
	}
	
    GUI_DrawRect(X0, Y0, X1, Y1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, X0+3, Y0+15);
	LCD_REFRESH();
    
	while(scan_keyboard() == KEY_NONE);
    
	GUI_ClearRect (X0, Y0, X1, Y1);
}

/*
 * 函数名：dis_scan_modules_gui
 * 描述  ：显示扫描模块界面信息
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void dis_scan_modules_gui(void)
{
	uint8_t X0	= 0;
	uint8_t Y0	= 0;
	uint8_t X1	= 200;
	uint8_t Y1	= 64 - 1;
    uint8_t OFFSET_X = 2;
//     uint8_t OFFSET_Y = 2;
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
    draw_short_cut_bm(UD_B);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"扫", 0+X0+OFFSET_X, Y0+2+12*0);
		GUI_DispStringAt((const char*)"描", 0+X0+OFFSET_X, Y0+2+12*1);
		GUI_DispStringAt((const char*)"模", 0+X0+OFFSET_X, Y0+2+12*2);
		GUI_DispStringAt((const char*)"块", 0+X0+OFFSET_X, Y0+2+12*3);
		GUI_DispStringAt((const char*)"01" , 0+X0+OFFSET_X, Y0+2+12*4);
// 		GUI_DispStringAt((const char*)"1.DC_GR  ID:01", 0+X0+2+18, Y0+2+12*0);
// 		GUI_DispStringAt((const char*)"2.", 0+X0+OFFSET_X+18, Y0+OFFSET_Y+12*1);
// 		GUI_DispStringAt((const char*)"3.", 0+X0+OFFSET_X+18, Y0+OFFSET_Y+12*2);
// 		GUI_DispStringAt((const char*)"4.", 0+X0+OFFSET_X+18, Y0+OFFSET_Y+12*3);
// 		GUI_DispStringAt((const char*)"5.", 0+X0+OFFSET_X+18, Y0+OFFSET_Y+12*4);
// 		GUI_DispStringAt((const char*)"模块地址:", 0+X0+2, Y0+4+14);
// 		GUI_DispStringAt((const char*)"模块名称:" , 0+X0+2, Y0+4+14+18);
        draw_short_cut_str(NULL, NULL, "启动", NULL);
	}
	else
	{
		GUI_DispStringAt((const char*)"SCAN_MODULES:"__DATE__, 0+X0+35, Y0+4);
		GUI_DispStringAt((const char*)"MD_ADDR:"__TIME__, 0+X0+35, Y0+4+14);
		GUI_DispStringAt((const char*)"MD_NAME:", 0+X0+35, Y0+4+14+18);
	}
	
    GUI_DrawRect(X0, Y0, X1, Y1);
    GUI_DrawRect(X0, Y0, X0+16, Y1);
    GUI_DrawRect(X0, Y0, 240-1, Y1);
	LCD_REFRESH();
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
