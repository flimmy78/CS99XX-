/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�GUI_Draw.c
 * ժ  Ҫ  ����������������Ҫ����
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
 
 /********************* ����ͷ�ļ� ***********************/
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



/************************* ȫ�ֱ�����ʼ�� ******************/
const uint8_t *unit_pool[]=
{
	"","mV","V","kV","~A","mA","A","m��","��","M��","G��","pF","nF","~F",
};
const uint8_t *unit_pool_[]=
{
	"","mV","V","kV","uA","mA","A","m��","��","M��","G��","pF","nF","uF",
};
const uint8_t *mode_pool[][10]=
{
	{"", "ACW", "DCW", "IR ", "GR ", "BBD", "CC"},
	{"", "ACW", "DCW", "IR ", "DV ", "BBD", "CC"},
};

const uint8_t *opt_pool[]=
{
    "","��ѹ", "����", "����", "����",
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
	{"�ȴ�����", "TestWait"},
	{"��ѹ����", "  Rise  "},
	{"���ڲ���", "Testing "},
	{"��ѹ�½�", "  Fall  "},
	{"��ѹ����", "Change  "},
	{"����ȴ�", "Interval"},
	{"���Ժϸ�", "  PASS  "},
	{"����ʧ��", "TestFail"},
	{"���ڳ��", " Charge "},
	{"��ѹʱ��", "Sta.Time"},
	{"���ڷŵ�", "Discharg"},
	{"�ŵ����", "Disch.OK"},
	{"�����ʱ", "Out.Dly"},
};

const char* except_buf[][3]=
{
	{"","",""},
	{"���ޱ���", "High Fail", "High"   },
	{"���ޱ���", "Low Fail" , "Low"    },
	{"��ʵ����", "Real Fail", "Real"   },
	{"��籨��", "Char Fail", "Charge" },
	{"��·����", "Open Fail", "Open"   },
	{"��·����", "ShortFail", "Short"  },
	{"ARC����" , "ARC Fail" , "ARC"    },
	{"GFI����" , "GFI Fail" , "GFI"    },
	{"���ű���", "AMP Fail" , "AMP"    },
	{"��λ����", "GEAR Fail", "GEAR"   },
	{"��ѹ�쳣", "VOL. Fail", "VOL."   },
	{"δ����"  , "Undefine ", "Undef." },
};
const char* sys_sw_status_buf[][2] =
{
	{"GFI:DISABLE"	,"GFI:ENABLE "},
	{"ARC:DISABLE" 	,"ARC:ENABLE "},
	{" HV:ZERO"		," HV:HIGH   "},
};
/* IR��ʾ����ʱ����ʱʹ�õ� */
const char* res_infinite_str[]=
{
    "-----","-.---","--.--","---.-","-.---","--.--","---.-"
};

MORE_OPTIONS_STR cc_cur_gear_option_pool[]=
{
    {{"��", "High"},PX,0,CC_CUR_HIGH,NULL,NULL},
    {{"��", "Mid."},PX,0,CC_CUR_MID ,NULL,NULL},
    {{"��", "Low "},PX,0,CC_CUR_LOW ,NULL,NULL},
};

MORE_OPTIONS_STR ir_filtering_deep_option_pool[]=
{
    {{"��", "Fast"},PX,0,IR_FILTERING_DEEP_FAST,NULL,NULL},
    {{"��", "Mid."},PX,0,IR_FILTERING_DEEP_MID ,NULL,NULL},
    {{"��", "slow "},PX,0,IR_FILTERING_DEEP_SLOW ,NULL,NULL},
};

MORE_OPTIONS_STR step_purpose_option_pool[]=
{
    {{"����", "Norm."},PX,0,STEP_PURPOSE_NORMAL,NULL,NULL},
    {{"ɨ��", "Scan"  },PX,0,STEP_PURPOSE_SCAN  ,NULL,NULL},
};

MORE_SEL_STR mor_sel_menu[]=
{
    {0, PX/*x*/,0/*y*/,{0, 50, 50 + 40, 0, 50, 50 + 40, 0}},/* ACW������λ */
    {0, PX/*x*/,0/*y*/,{0, 35, 40 + 35, 0, 35, 40 + 35, 0}},/* DCW������λ */
    {0, PX/*x*/,0/*y*/,{0, 35, 40 + 35, 0, 35, 40 + 35, 0}},/* GR������λ */
    {0, PX/*x*/,0/*y*/,{0, 28, 28 + 28, 28 + 28 + 23, (28 + 28 + 23 + 23), 0}},/* ģʽ���� */
    {0, SX/*x*/,0/*y*/,{0, SOX, 0, SOX, 0, SOX, 0, SOX, 0}},/* ϵͳʧ��ģʽ���� */
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
    {{"����", "EachStep"},SX,0,EACH_STEP,NULL,NULL},
    {{"�ܲ�", "AllSteps"},SX,0,ALL_STEP ,NULL,NULL},
    {{"����", "Hold"    },SX,0,TEST_HOLD,NULL,NULL},
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
            "0 - 0.00k��",
            "1 - 22.4k��",
            "2 - 7.60k��",
            "3 - 4.00k��",
        },
        4,
    },
    {
        {
            "0 - 4.40k��",
            "1 - 2.80k��",
            "2 - 1.10k��",
        },
        3,
    },
};

DATA_INFO menu_par[]=
{
	/************* COM ************/
	{// 1   ���Բ���
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,4/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		1,0,{"���Բ���","Step"},{"","",""},set_step,0/*no_dis_bits*/,GUI_COM_STEP,UDSB,"99  ","01  ",
	},
	
	{// 2   ����ģʽ
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		1,0,{"����ģʽ","Mode"},{"","ACW","DCW","IR","GR","BBD"},set_mode,0/*no_dis_bits*/,GUI_COM_MODE,UDRB,
	},
	
	{// 3   ��ʱʱ��
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		9999,3,{"��ʱʱ��","Delay"},{"s",""},set_test_time,0/*no_dis_bits*/,GUI_COM_DELAY_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 4   ����ʱ��
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,1/*bits*/,0,
		9999,3,{"����ʱ��","Rise"},{"s",""},set_test_time,0/*no_dis_bits*/,GUI_COM_RISE_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
    
	{// 5   ��ѹʱ��
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,3,{"��ѹʱ��","Stabili."},{"s","",""},set_test_time,0/*no_dis_bits*/,GUI_COM_STAB_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 6   ����ʱ��
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,3,{"����ʱ��","Test"},{"s","",""},set_test_time,0/*no_dis_bits*/,GUI_COM_TEST_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 7	�½�ʱ��
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,4/*page*/,1/*bits*/,0,
		9999,3,{"�½�ʱ��","Fall"},{"s",""},set_test_time,0/*no_dis_bits*/,GUI_COM_FALL_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 8	���ʱ�� \ ����ʱ��
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		9999,0,{"���ʱ��","Interval"},{"s","",""},set_test_time,0/*no_dis_bits*/,GUI_COM_INTER_T,UDRB,"999.9s","000.0s",0,0,1/*rang_flag*/,
	},
	
	{// 9 ����PASS
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,EITHER_OR/*type*/,5/*lon*/,1/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,1,{"����PASS","StepPass"},{"��","��","Yes","No"},set_either_or_par,0/*no_dis_bits*/,GUI_COM_STEPS_PASS,UDRB,"","",35,3,
	},
	
	{// 10 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,EITHER_OR/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,1,{"��������","StepSer."},{"��","��","Yes","No"},set_either_or_par,0/*no_dis_bits*/,GUI_COM_STEPS_CONT,UDRB,"","",35,3,
	},
	
	{// 11 ���Ƶ��
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,1/*bits*/,0,
		0,10,{"���Ƶ��","Freq."},{"","Hz",""},set_frequency,0/*no_dis_bits*/,GUI_COM_OUTPUT_FREQ,UDRB,"   9  ","   1  ",0,0,1/*rang_flag*/,
	},
	
	{// 12	���Զ˿�
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,STRUCT_T/*type*/,0/*lon*/,0/*decs*/,8/*total*/,1/*cur*/,0/*ch_bit*/,6/*page*/,4/*bits*/,0,
		0,10,{"���Զ˿�","Port"},{"",""},set_all_ports,0/*no_dis_bits*/,GUI_COM_PORT,UDRB,"","",40,3
	},
	
	{// 13	������;
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,8/*total*/,1/*cur*/,0/*ch_bit*/,6/*page*/,0/*bits*/,0,
		0,10,{"������;","Purpose"},{0},set_option_par,0/*no_dis_bits*/,GUI_COM_STEPS_PURPOSE,UDRB,"","",35,3
	},
    
	/************* ACW ************/
	{// 1 ���Ե�ѹ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"�����ѹ","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_ACW_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 ������λ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"������λ","Range"},{"",""},set_ac_cur_gear,0/*no_dis_bits*/,GUI_ACW_CUR_GEAR,UDRB,
	},
	
	{// 3 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"��������","High"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_CUR_H,UDRB,
	},
	
	{// 4 ��������
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"��������","Low"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_CUR_L,UDRB,
	},
	
	{// 5 ��ʵ����
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"��ʵ����","Real"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_REAL_CUR,UDRB,
	},
	
	{// 6 �绡���
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"�绡���","Arc"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_ARC_SUR,UDRB,
	},
    
	/************* DCW ************/
	{// 1 ���Ե�ѹ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"�����ѹ","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_DCW_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 ������λ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"������λ","Range"},{"",""},set_dcw_cur_gear,0/*no_dis_bits*/, GUI_DCW_CUR_GEAR,UDRB,
	},
	
	{// 3 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"��������","High"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_CUR_H,UDRB,
	},
	
	{// 4 ��������
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"��������","Low"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_CUR_L,UDRB,
	},
	
	{// 6 ������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"������","Charge_I"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_CHARGE_CUR,UDRB,
	},
    
	{// 7 �绡���
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"�绡���","Arc"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_ARC_SUR,UDRB,
	},
    
	{// 8 ����迹
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		3,0,{"����迹","Impedan"},{"","0 - 0.00k��","1 - 22.4k��","2 - 7.60k��","3 - 4.00k��"},set_float_par,0/*no_dis_bits*/, GUI_DCW_OUTPUT_IMPEDANCE,UDRB,"","",0,0,2/*rang_flag*/,
	},
    
	/************* IR ************/
	{// 1 ���Ե�ѹ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"�����ѹ","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_IR_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 �Զ�����
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"�Զ�����","Auto.Lev"},{"��","��","YES","NO"},set_either_or_par,0/*no_dis_bits*/, GUI_IR_AUTO_GEAR,UDRB,"","",35,3,
	},
	
	{// 3 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,2/*bits*/,0,
		9999,0,{"��������","High"},{"M��","M��"},set_float_par,0/*no_dis_bits*/, GUI_IR_RES_H,UDRB,"09999M��","00000M��",
	},
	
	{// 4 ��������
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,0/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,2/*bits*/,0,
		9999,1,{"��������","Low"},{"M��","M��"},set_float_par,0/*no_dis_bits*/, GUI_IR_RES_L,UDRB,"09999M��","00000M��",0,0,0/*rang_flag*/,
	},
    
	/************* GR ************/
	{// 1 ��ѹ��λ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,OPTION_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"��ѹ��λ","Gear"},{"","","20mV","20mV","200mV","200mV","2000mV""2000mV","12V","12V"},set_gr_vol_gear,0/*no_dis_bits*/,GUI_GR_VOL_GEAR,UDRB,
	},
	
	{// 2 �������
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		1,0,{"�������","Cur."},{"A","mA",""},set_gr_output_cur,1/*no_dis_bits*/,GUI_GR_OUTPUT_CUR,UDRB,
	},
	
	{// 3 ��������
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"��������","High"},{"m��","��",""},set_gr_res_upper,0/*no_dis_bits*/,GUI_GR_RES_H,UDRB,"510.0m��","001.0m��",
	},
	
	{// 4 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"��������","Low"},{"m��","��",""},set_gr_res_lower,0/*no_dis_bits*/,GUI_GR_RES_L,UDRB,
	},
	
	{// 5 ��ѹ����
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"��ѹ����","High"},{"mV","V",""},set_gr_vol_upper,0/*no_dis_bits*/,GUI_GR_VOL_H,UDRB,"00.00V","00.00V",
	},
	
	{// 6 ��ѹ����
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"��ѹ����","Low"},{"mV","v",""},set_gr_vol_lower,0/*no_dis_bits*/,GUI_GR_VOL_L,UDRB,"00.00V","00.00V",
	},
	
	{// 7 ����ģʽ
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,OPTION_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,0/*bits*/,0,
		1,0,{"���Է���","Method"},{"����","����","Cont.","Pulse"},set_gr_test_method,0/*no_dis_bits*/,GUI_GR_TEST_METHOD,UDRB,"","",40,3
	},
    
	/************* BBD************/
	{// 1 ���Ե�ѹ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"�����ѹ","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_BBD_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 ��·���
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,3/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		100,10,{"��·���","Open"},{"%","",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_OPEN_CHECK,UDRB,"  100%","  10%",0,0,1/*rang_flag*/,
	},
	
	{// 3 ��·���
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,INT_T/*type*/,3/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		500,100,{"��·���","Short"},{"%","v",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_SHORT_CHECK,UDRB," 500%","0,100%",0,0,1/*rang_flag*/,
	},
	
	{// 4 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		500,100,{"��������","High"},{"~F","",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_CAP_H,UDRB,"0.000~F","0.000~F",0,0,0/*rang_flag*/,
	},
	
	{// 5 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		500,100,{"��������","Low"},{"~F","",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_CAP_L,UDRB,"0.000~F","0.000~F",0,0,0/*rang_flag*/,
	},
	
	/************* CC ************/
	{// 1 ���Ե�ѹ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"�����ѹ","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_CC_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 ������λ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"������λ","Range"},{"",""},set_ac_cur_gear,0/*no_dis_bits*/,GUI_CC_CUR_GEAR,UDRB,
	},
	
	{// 3 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"��������","High"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_CC_CUR_H,UDRB,
	},
	
	{// 4 ��������
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"��������","Low"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_CC_CUR_L,UDRB,
	},
	
	{// 5 ��ʵ����
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"��ʵ����","Real"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_CC_REAL_CUR,UDRB,
	},
	
	{// 6 ����ǿ��
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,OPTION_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"����ǿ��","CurInten."},{"��","Mid"},set_option_par,0/*no_dis_bits*/, GUI_CC_CUR_INTENSITY,UDRB,
	},
    
	/************* SYS ************/
	{//	1 Һ���Աȶ�
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		9,1,{"Һ���Աȶ�","Contrast"},{"",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_CONTRAST,UDRB,"(1-9)",""
	},
	
	{//	2 ������ʾ
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,2/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		9,0,{"������ʾ","RemainHint"},{"",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_REMAIN_HINT,UDRB,"(0-9)",""
	},
	
	{//	3 �������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		0,10,{"�������","ResultSave"},{"��","��","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_RES_SAVE,UDRB,"","",SOX,SOY,
	},
	
	{//	4 �������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		0,10,{"�������","OverCover"},{"��","��","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_VOER_COVER,UDRB,"","",SOX,SOY,
	},
	
	{//	5 GFI����
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"GFI ����","GFI Prot."},{"��","��","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_GFI_PROT,UDRB,"","",SOX,SOY,
	},
	
	{//	6 �Լ�����
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"�Լ�����","SelfCheck"},{"��","��","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_SELF_CHECK,UDRB,"","",SOX,SOY,
	},
	
	{//	7 ����ѡ��
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"����ѡ��","Language"},{"����","Ӣ��","Chinese","English",},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_LANGUAGE,UDRB,"","",SOX,SOY,
	},
	
	{//	8 ʧ�ܼ���
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"ʧ�ܼ���","FailCont."},{"��","��","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_FAIL_CONT,UDRB,"","",SOX,SOY,
	},
	
	{//	9 �б���ʾ
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"�б���ʾ","ListDisplay"},{"��","��","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_LIST_DIS,UDRB,"","",SOX,SOY,
	},
	
	{//	10 ���Զ˿�
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"���Զ˿�","TestPort"},{"�ӵ�","����","GND","Float"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_TEST_PORT,UDRB,"","",SOX,SOY,
	},
    
	{//	11 �����ź�
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,3/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"�����ź�","TestSignal"},{"��ƽ","����","Level","Point"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_TEST_SIGNAL,UDRB,"","",SOX,SOY,
	},
	
	{//	12 ��������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"��������","Buzzer SW."},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_BUZZER_SW,UDRB,"","",SOX,SOY,
	},
	
	{//	13  ��������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,STRING_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,3/*bits*/,0,
		0,10,{"��������","CheckDate"},{"2015-1-1 12:12:12",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_CHECK_DATE,UDRB,"","",SOX,SOY,
	},
	
	{//	14  ��Ź���
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,0/*bits*/,0,
		2,0,{"��Ź���","NO.Rule"},{"",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_NO_RULE,UDRB,"(0-2)","",SOX,SOY,
	},
	
	{//	15 ʧ��ģʽ
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"ʧ��ģʽ","FailMode"},{"STOP","STOP","Yes","No"},set_option_sys_par,0/*no_dis_bits*/, GUI_SYS_FAIL_MODE,UDRB,"","",SOX,SOY,
	},
    
	{//	16 ��������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"��������","Silent SW."},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_SILENT_SW,UDRB,"","",SOX,SOY,
	},
    
	{//	17 PLC �ź�
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"PLC �ź�","PLC Signal"},{"����","�ܲ�","EachStep","AllSteps"},set_plc_option_sys_par,0/*no_dis_bits*/, GUI_SYS_PLC_SIGNAL,UDRB,"","",SOX,SOY,
	},
    
	{//	18 �����ʱ
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,0,{"�����ʱ","Out.Delay"},{"s (0.0-999.9s)","s (0.0-999.9s)"},set_float_sys_par,0/*no_dis_bits*/, GUI_SYS_OUTPUT_DELAY,UDRB,"(0.0-999.9s)","",SOX,SOY,
	},
    
	{//	19 ��λ����
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,200,{"IR��λ����","IRGearHold"},{"s (0.2-9.999s)","s (0.2-9.999s)"},set_float_sys_par,0/*no_dis_bits*/, GUI_SYS_IR_GEAR_HOLD,UDRB,"(0.2-9.999s)","",SOX,SOY,
	},
    
	{//	20 IR �����ٶ�
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,5/*lon*/,3/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,200,{"IR�����ٶ�","IR SPEED"},{"",""},set_option_par,0/*no_dis_bits*/, GUI_SYS_IR_SPEED,UDRB,"(0.2-9.999s)","",SOX,SOY,
	},
    
	/************* CUSTOM(���Ʋ���) ************/
	{//	1 ����ѡ��
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"����ѡ��","8833AMP"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_AMP_SELECT,UDRB,"","",SOX,SOY,
	},
	{//	2 ��������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"��������","AMP_TYPE"},{"PWM","LINE","PWM","LINE"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_AMP_TYPE,UDRB,"","",SOX,SOY,
	},
	{//	3 �����洢����
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"�����洢","PAR_MEDIUM"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_PAR_MEDIUM,UDRB,"","",SOX,SOY,
	},
	{//	4 ����洢����
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"����洢","RES_MEDIUM"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_RES_MEDIUM,UDRB,"","",SOX,SOY,
	},
	{//	5 У׼�洢����
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"У׼�洢","CAL_MEDIUM"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_CAL_MEDIUM,UDRB,"","",SOX,SOY,
	},
	{//	6  ������ʹ��
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"������ʹ��","BUZZER-EN"},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_BUZZER_EN,UDRB,"","",SOX,SOY,
	},
	{//	7  ��������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"��������","MUTE-SW"},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_MUTE_SW,UDRB,"","",SOX,SOY,
	},
	{//	8  ���뵼��
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"���뵼��","LeadingSW"},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_LEADING_SW,UDRB,"","",SOX,SOY,
	},
	{//	9  ������ʱ
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"IR��������","IR_DELAY"},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_IR_G_HOLD_SW,UDRB,"","",SOX,SOY,
	},
	{//	10  IR�����ٶ�
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"IR�����ٶ�","IR_SPEED"},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_IR_SPEED_SW,UDRB,"","",SOX,SOY,
	},
};


void more_options_sel_gui(uint8_t options, uint8_t n, MORE_SEL_STR *mor_sel_info);
/*************************** �������� *************************/
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
 * ��������GUI_DispFloatFixAt
 * ����  ����ָ��������ʾһ�������� ����ʾ��λ
 * ����  ��float v, char Len, char Fract, I16P unit, I16P x , I16P y
 * ���  ����
 * ����  ����
 */
void GUI_DispFloatFixAt (float v, char Len, char Fract, I16P unit, I16P x , I16P y)
{
	GUI_GotoXY(x, y);
	GUI_DispFloatFix (v, Len, Fract);
	GUI_DispString((const char*)unit_pool[unit]);
}

/*
 * ��������draw_colon
 * ����  ��������ҳ���·���ʾð��
 * ����  ����
 * ���  ����
 * ����  ����
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
 * ��������save_par_to_array
 * ����  ��������ֵ���µ�һ��������
 * ����  ��data_info �˵������ݽṹ
 *         value �и��µ�����
 *         array_num �����Ա�ĸ���
 * ���  ����
 * ����  ����
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
 * ��������save_par_to_ram
 * ����  ��������ֵ���µ��ڴ�
 * ����  ��data_info �˵������ݽṹ
 *         value �и��µ�����
 * ���  ����
 * ����  ����
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
    
    /* ��ֵ */
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
 * ��������draw_one_par_info
 * ����  ����ʾһ���˵����ֵ
 * ����  ��p �˵������ݽṹ
 *         pdata ��ʾ������
 *         unit  ��λ
 * ���  ����
 * ����  ����
 */
void draw_one_par_info(DATA_INFO* p, void *pdata, const uint8_t unit)
{
	int32_t data = 0;
	char *ch = (char*)pdata;
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"��", colon_x, p->y);
    
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
			ch = (char*)*(uint32_t*)pdata;//�����ת��
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
 * ��������read_par_to_buf
 * ����  ������ָ���˵��������
 * ����  ��data_info
 * ���  ��value �˵������� ���ΪNULL�Ͳ���ֵ
 * ����  ���˵�������
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
            temp = (uint32_t)data_info->pData;/* �ѵ�ַת��Ϊ���� */
            break;
        case GUI_STRUCT:
            temp = (uint32_t)data_info->pData;/* �ѵ�ַת��Ϊ���� */
            break;
    }
    
    if(value != NULL)
    {
        *value = temp;
    }
    
    return temp;
}

/*
 * ��������load_this_page_info
 * ����  �����ص�ǰ����ҳ�����������Ϣ
 * ����  ����
 * ���  ��data_info
 * ����  ����
 */
void load_this_page_info(DATA_INFO* data_info[])
{
    int32_t i = 0;
    DATA_INFO* temp = pmenu_data;
    
    /* �ҵ���ҳ�ĵ�һ�� */
    for(i = 1; i < pmenu_data->cur; i++)
    {
        temp = temp->prev;
    }
    
    memset(data_info, 0, sizeof data_info);
    /* �ӵ�һ����ʼ */
    for(i = 1; i <= pmenu_data->total; i++, temp = temp->next)
    {
        data_info[i] = temp;
    }
}
/*
 * ��������load_this_page_value
 * ����  ���Ѽ��ص��ڴ��������Ϣ��ʾ����
 * ����  ����
 * ���  ����
 * ����  ����
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
 *ȥ��ǰ����
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
 * ��������ten_power
 * ����  ��10����
 * ����  ����
 * ���  ����
 * ����  ����
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
//������mysprintf                                                                                /
//����type :153 ��ʾ�����Ǹ����� 1��ǰ����0����ǰ���� ��󳤶�Ϊ5����С���㣬3λС�� ��󳤶Ȳ��ܳ���5��С��λ���ܳ���3 /
//    unit:Ϊ��������׷�ӵĵ�λ�ַ���                                                            /
//    value:Ҫת��Ϊ�����͵���������                                                             /
//////////////////////////////////////////////////////////////////////////////////////////////////
void mysprintf(uint8_t *buf, const uint8_t* unit, uint8_t type, uint32_t value)
{
    uint8_t decs_n = (type / 1) % 10;/* С��λ */
    uint8_t lon    = (type / 10) % 10;/* ���� */
    uint8_t app_0  = (type / 100) % 10;/* ǰ�����־ */
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
        
        /* ȥ��ǰ���� */
        if(app_0 == 0)
        {
            /* ���һ������λ */
            if(int_n - i == 1)
            {
                buf[j++] = temp + '0';
            }
            /* �������һ������λ */
            else
            {
                if(temp || j)
                {
                    buf[j++] = temp + '0';
                }
            }
        }
        /* ��ȥ��ǰ���� */
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
    
    /* С������ */
    for(i = 0; i < decs_n; i++)
    {
        temp = (value / ten_power(decs_n - i - 1)) % 10;
        buf[i + temp_l] = temp + '0';
    }
    
    buf[i + temp_l] = 0;
    
    /* ׷�Ӻ�׺��Ϣ */
    if(unit != NULL)
    {
        strcat((char*)buf, (const char*)unit);
    }
}
/*
 * ��������cursor_pos
 * ����  ����ʾ��Ļ��� ���������һ����ʾ�Ĺ��
 * ����  ���������� �������궨��Ϊ�з�������Ϊ�˿��Դ��븺ֵ���꣬���ǽ�����Ƴ���Ļ
 * ���  ����
 * ����  ����
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
#define XX_//С������

#ifdef	XX_
    #define Y_	(y+11)
    #define Y__ (pre_y+11)
#else
    #define Y_	(y+8)
    #define Y__ (pre_y+8)
#endif

	static int16_t pre_x = 0;//�ñ���Ϊ�˼�¼�ϴι���λ��
	static int16_t pre_y = 0; 
	
	GUI_ClearRect(pre_x, Y__, pre_x+6, Y__);
	pre_x = x;
	pre_y = y;
	
#ifdef X_
	GUI_DrawLine (x  , Y_, x+4      , Y_);
#endif

#ifdef XX_//�������
// 	GUI_ClearRect(x-6, y+11, x+6+4, y+11);
	GUI_DrawLine (x  , y+11, x+4  , y+11);
#endif

#undef X_
#undef Y_
#undef Y__
}
/*
 * ��������shift_cursor_l
 * ����  �����������λ�� ����ʾ����
 * ����  ��p ָ��ǰ���ò�������Ϣ�ṹ
 * ���  ����
 * ����  ����
 */
void dis_cursor(DATA_INFO * p)
{
	cursor_pos(p->x+6*p->ch_bit, p->y);
	ReFresh_LCD24064();
}
/*
 * ��������shift_cursor
 * ����  �����ѭ������
 * ����  ��p ָ��ǰ���ò�������Ϣ�ṹ
 * ���  ����
 * ����  ����
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
    
	if (p->type == FLOAT_T)//������
	{
        /* ���С�������Ϊ0�͵��������� */
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
				p->ch_bit++;//����С����
			}
			cursor_pos(x+6*p->ch_bit, y);
		}
		else
		{
			cursor_pos(x-6*(lon-1), y);
		}
	}
	else if(p->type == INT_T)//������
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
	else if(p->type == STRING_T)//�ַ���
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
        
		cursor_pos(x+6*p->ch_bit, y);//���������λ��
	}
}

/*
 * ��������shift_cursor_l
 * ����  �����ѭ������
 * ����  ��p ָ��ǰ���ò�������Ϣ�ṹ
 * ���  ����
 * ����  ����
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
    
    //������
	if (p->type == FLOAT_T)
	{
        /* ��С��λΪ0ʱ���������� */
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
				p->ch_bit++;//����С����
			}
			cursor_pos(x+6*p->ch_bit, y);
		}
		else
		{
			cursor_pos(x-6*(lon-1), y);
		}
	}
    //����
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
    //�ַ���
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
 * ��������change_data
 * ����  ����������
 * ����  ��base ���ڲ�����λ������ֵ0-9
 * ���  ����
 * ����  ����
 */
void change_data(uint8_t base)
{
	uint32_t t = 0;
	uint32_t temp = 0;
    
	if ( pmenu_data->type == FLOAT_T && pmenu_data->decs != 0)//����
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
	else if(pmenu_data->type == INT_T || pmenu_data->decs == 0)//����
	{
		t = ten_power(pmenu_data->lon - pmenu_data->ch_bit - 1);
	}
    
	switch(pmenu_data->bits)
	{
		case GUI_U8://8λ
        case GUI_U8_ARRAY:
			temp = *(uint8_t*)pmenu_data->pData;
			temp = temp - ((temp/t)%10)*t + base*t;
			*(uint8_t*)pmenu_data->pData = temp;
			break;
		case GUI_U16://16λ
        case GUI_U16_ARRAY:
			temp = *(uint16_t*)pmenu_data->pData;
			temp = temp - ((temp/t)%10)*t + base*t;
			*(uint16_t*)pmenu_data->pData = temp;
			break;
		case GUI_U32://32λ
		case GUI_U32_ARRAY:
			temp = *(uint32_t*)pmenu_data->pData;
			temp = temp - ((temp/t)%10)*t + base*t;
			*(uint32_t*)pmenu_data->pData = temp;
			break;
		case GUI_STRING://�ַ��� �ļ��� ����
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
				flag = 0;//��ִ�е����ʱ�ͱ�ʾ�µ�λ��������״̬
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
					flag = 1;//�ı䵱ǰλ
				}
				else
				{
					flag = 2;//����
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
 * ��������my_sprintf
 * ����  ���������ݵ���ʾ��
 * ����  ��p ָ��ǰ�������ݵ���Ϣ�ṹ
 * ���  ����
 * ����  ����
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
    
    //��������
    for (i = 0; i < temp1; i++)
    {
        t = (data / ten_power(temp1 + temp2 - i - 1)) % 10;
        buf[i] = '0' + t;
    }
    //С����
    if(temp2)
    {
        buf[i++] = '.';
    }
    //С������
    for(j = 0; j < temp2; j++)
    {
        t = (data / ten_power(temp2 - j - 1)) % 10;
        buf[i++] = '0' + t;
    }
    //��λ
    strcat(buf, (const char*)p->unit[0]);
}
/*
 * ��������update_data
 * ����  ���������ݵ���ʾ��
 * ����  ��p ָ��ǰ�������ݵ���Ϣ�ṹ
 * ���  ����
 * ����  ����
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
		case FLOAT_T://����
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
		case INT_T://����
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
						t = (*(uint16_t*)p->pData/ten_power(p->lon-i-1))%10;//֮ǰת����uint8_t*���ƻ�16λ��32λ������
						break;
					case 2:
						t = (*(uint32_t*)p->pData/ten_power(p->lon-i-1))%10;//֮ǰת����uint8_t*���ƻ�16λ��32λ������
						break;
					default:
						break;
				}

				GUI_DispDec(t, 1);
			}
			break;
        }
		case STRING_T://�ַ���
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
 * ��������clear_
 * ����  ��ֻ��������ʾ��� ��ֹ��Ļ��˸
 * ����  ����
 * ���  ����
 * ����  ����
 */
void clear_(void)
{
	GUI_ClearRect (0, 0, 240-1, 64-1);
}

/*
 * ��������clear_menu
 * ����  �������Ļ�Ҳ�Ĳ˵��� ֻ��������ʾ��� ��ֹ��Ļ��˸
 * ����  ����
 * ���  ����
 * ����  ����
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
 * ��������gui_par_inval
 * ����  ��˵������������Ч
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_par_inval(void)
{
	GUI_ClearRect(130+1,18,200-1,63);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"��������", 145, 22);
		GUI_DispStringAt((const char*)"������Ч", 145, 36);
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
 * ��������gui_par_inval
 * ����  ����gģʽʱ�еĲ���������Ҫ�����ض�����²������õ�
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_g_mode_inval(void)
{
	GUI_ClearRect(130+1,18,200-1,63);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"ϵͳĬ��", 145, 22);
		GUI_DispStringAt((const char*)"���ܸı�", 145, 36);
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
// 		GUI_DispStringAt((const char*)"G ģʽ��", 145, 22);
// 		GUI_DispStringAt((const char*)"������Ч", 145, 36);
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
 * ��������ratio_broken
 * ����  ��У׼ϵ����ʧ��������
 * ����  ����
 * ���  ����
 * ����  ����
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
		GUI_DispStringAt((const char*)"ϵͳУ׼ϵ����", 0+X0+35, Y0+4+4);
		GUI_DispStringAt((const char*)"����������!", 0+X0+35, Y0+4+16);
		GUI_DispStringAt((const char*)"����λ������������", 0+X0+35, Y0+4+28);
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
 * ��������cal_warning
 * ����  ��У׼���ش򿪺���ʾ
 * ����  ����
 * ���  ����
 * ����  ����
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
		GUI_DispStringAt((const char*)"ϵͳУ׼���ش�", 0+X0+35, Y0+4+4);
		GUI_DispStringAt((const char*)"��,�뽫��رպ�", 0+X0+35, Y0+4+16);
		GUI_DispStringAt((const char*)"��������!", 0+X0+35, Y0+4+28);
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
 * ��������auto_cal_pass
 * ����  ���Զ�У׼ͨ��
 * ����  ����
 * ���  ����
 * ����  ����
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
 * ��������rate_of_advance
 * ����  ����ɽ���
 * ����  ����
 * ���  ����
 * ����  ����
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
		GUI_DispStringAt((const char*)"����", (X0 + X1)/2 - 6 * 5, Y1-12);
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
                BUZZER_SOUND(300);/* ��������300ms */
                break;
            }
            
            flag++;
        }
    }
}
/*
 * ��������type_mismatch
 * ����  �����Ͳ�ƥ������ʾ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void type_mismatch(void)
{
	uint8_t X0	= 40;
	uint8_t Y0	= 8;
	uint8_t X1	= 165;
	uint8_t Y1	= 58;
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	
	GUI_DispStringAt((const char*)"���Ͳ�ƥ��", 0+X0+35, Y0+4+14);
	
    GUI_DrawRect(X0, Y0, X1, Y1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, X0+3, Y0+5);
	LCD_REFRESH();
}
/*
 * ��������dis_set_port
 * ����  �����ý�����ʾ�˿�״̬
 * ����  ��val ״ֵ̬ n λ�� ports �˿���Ŀ 4 8
 * ���  ����
 * ����  ����
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
 * ��������ports_cursor_pos
 * ����  �����ý������ö˿�ʱ������ʾλ��
 * ����  ��n λ�� ports �˿���Ŀ 4 8
 * ���  ����
 * ����  ����
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
 * ��������dcw_cur_sel_gui
 * ����  ��dcw ������λѡ���õĽ���������
 * ����  ��kinds ������λ������ n �ڼ���������
 * ���  ����
 * ����  ����
 */
void dcw_cur_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n)
{
    mor_sel_menu[DCW_GEAR_SEL_MENU].y = pmenu_data->y;
    mor_sel_menu[DCW_GEAR_SEL_MENU].opt_buf = gear_buf;
    more_options_sel_gui(kinds, n - 1, &mor_sel_menu[DCW_GEAR_SEL_MENU]);
    LCD_REFRESH();
}

/*
 * ��������acw_gr_hz_sel_gui
 * ����  ��Ƶ������ʱ�� ��ʾ�������
 * ����  ��kinds Ƶ�ʵ����� n �ڼ����� gear_buf ��λ�����ƻ���
 * ���  ����
 * ����  ����
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
 * ��������acw_cur_sel_gui
 * ����  ��acw ����ѡ�����������
 * ����  ��kinds ������λ������ n �ڼ���������
 * ���  ����
 * ����  ����
 */
void ac_cur_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n)
{
    mor_sel_menu[ACW_GEAR_SEL_MENU].y = pmenu_data->y;
    mor_sel_menu[ACW_GEAR_SEL_MENU].opt_buf = gear_buf;
    more_options_sel_gui(kinds, n - 1, &mor_sel_menu[ACW_GEAR_SEL_MENU]);
    LCD_REFRESH();
}
/*
 * ��������sys_option_sel_gui
 * ����  ��acw ����ѡ�����������
 * ����  ��kinds ������λ������ n �ڼ���������
 * ���  ����
 * ����  ����
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
    uint8_t p_num[10] = {0};/* ͳ�Ƹ�������ʾ�ĸ��� */
    int32_t j,k;
    uint8_t size = sizeof mor_sel_info->x_o;
    uint32_t temp = 0;
    uint32_t temp1 = 0;
    const uint8_t** opt_buf = mor_sel_info->opt_buf;
    
    /* ͳ�Ƹ�������ʾ�ĸ��� */
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
 * ��������draw_more_options_gui
 * ����  ����ʾ��ѡ��Լ���ǰѡ����
 * ����  ��list ��ѡ�������
 *         iteration ��ǰ��ѡ��
 * ���  ����
 * ����  ����
 */
void draw_more_options_gui(MORE_OPTIONS_LIST *list, MORE_OPTIONS_STR *iteration)
{
    MORE_OPTIONS_STR *first = NULL;
    uint8_t i = 0;
    
    /* �ҵ���ǰ�еĵ�һ�� */
    for(first = iteration; first->cur != 1; first = first->prev);
    
    /* �����ʷ��ʾ */
    GUI_ClearRect(first->x, first->y, 200 - 1, first->y + 12);
    
    /* ��ʾѡ�� */
    for(i = 0; i < iteration->total; i++, first = first->next)
    {
        /* ѡ��������ʾ */
        GUI_DispStringAt((const char*)first->name[!sys_par.language==CHINESE], first->x + 10, first->y);
        /* �Ƿ�ѡ�е���ʾλͼ */
        draw_bitmap_circle(first->x, first->y, first->cur == iteration->cur);
    }
    /* ��ʾ��� */
    cursor_pos_n(iteration->x + 1, iteration->y);
    /* ˢ�� */
    LCD_REFRESH();
}

/*
 * ��������gr_vol_sel_gui
 * ����  ��GR ֱ��ѹ��ģʽ�� ѡ���ѹ��λ
 * ����  ��kinds ������λ������ n �ڼ����絵
 * ���  ����
 * ����  ����
 */
void gr_vol_sel_gui(uint8_t options, const uint8_t** opt_buf, uint8_t n)
{
    mor_sel_menu[GR_VOL_GEAR_SEL_MENU].y = pmenu_data->y;
    mor_sel_menu[GR_VOL_GEAR_SEL_MENU].opt_buf = opt_buf;
    more_options_sel_gui(options, n - 1, &mor_sel_menu[GR_VOL_GEAR_SEL_MENU]);
    LCD_REFRESH();
}

/*
 * ��������mode_sel_gui
 * ����  ��ѡ��ģʽ������ʾ�������
 * ����  ��kinds ģʽ������ n λ��
 * ���  ����
 * ����  ����
 */
void mode_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n)
{
    mor_sel_menu[MODE_SET_SEL_MENU].y = pmenu_data->y;
    mor_sel_menu[MODE_SET_SEL_MENU].opt_buf = gear_buf + 1;
    more_options_sel_gui(kinds, n - 1, &mor_sel_menu[MODE_SET_SEL_MENU]);
}
/*
 * ��������dcgr_commuincation_err
 * ����  ��ֱ��gr����ͨ��ʧ����ʾ��
 * ����  ����
 * ���  ����
 * ����  ����
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
		GUI_DispStringAt((const char*)"ͨѶʧ��!", X0+35, Y0+4);
		GUI_DispStringAt((const char*)"ֱ��ģ��ͨ��ʧ��", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"����Ӳ��...", X0+35, Y0+28);
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
 * ��������cal_sw_on_warning
 * ����  ��У׼���ش򿪺�ͱ�����ʾ
 * ����  ����
 * ���  ����
 * ����  ����
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
		GUI_DispStringAt((const char*)"ϵͳУ׼����", X0+35, Y0+4);
		GUI_DispStringAt((const char*)"�򿪣���ر�", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"��������", X0+35, Y0+28);
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
		GUI_DispStringAt((const char*)"���ڼ��U��", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"...", X0+35, Y0+28);
// 		GUI_DispStringAt((const char*)"��������", X0+35, Y0+28);
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
		GUI_DispStringAt((const char*)"�����Զ�У׼", X0+100, 0);
        
		GUI_DispStringAt((const char*)"000"       , 0 , 0);
		GUI_DispStringAt((const char*)"ģʽ:ACW"  , 20, 0);
		GUI_DispStringAt((const char*)"��Ŀ:��ѹ" , 80, 0);
        
		GUI_DispStringAt((const char*)"DAֵ:4096"        , 0, Y0+12*0);
		GUI_DispStringAt((const char*)"ADֵ:4096   4095" , 0, Y0+12*1);
		GUI_DispStringAt((const char*)"��λ:"         , 0, Y0+12*2);
		GUI_DispStringAt((const char*)"У׼��:100V"      , 0, Y0+12*3);
        
		GUI_DispStringAt((const char*)"��ѹ:5.00kV" , 150, Y0+12*0);
		GUI_DispStringAt((const char*)"����:2.00mA" , 150, Y0+12*1);
// 		GUI_DispStringAt((const char*)"����:2.00M��" , 150, Y0+12*2);
		GUI_DispStringAt((const char*)"����:2.00M��" , 150, Y0+12*3);
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
		GUI_DispStringAt((const char*)"δ��⵽U��", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"��EXIT�˳�", X0+35, Y0+28);
// 		GUI_DispStringAt((const char*)"��������", X0+35, Y0+28);
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
 * ��������GUI_Draw_build_connect_to_board
 * ����  ����������ͨ�ŵ���ʾ��
 * ����  ����
 * ���  ����
 * ����  ����
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
		GUI_DispStringAt((const char*)"ϵͳ������ͨѶ", X0+35, Y0+4);
		GUI_DispStringAt((const char*)"�ӿڰ彨������", X0+35, Y0+16);
		GUI_DispStringAt((const char*)"�У����Ժ�...", X0+35, Y0+28);
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
 * ��������GUI_Draw_build_connect_fail
 * ����  ��ͨ������ʧ����ʾ��
 * ����  ����
 * ���  ����
 * ����  ����
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
		GUI_DispStringAt((const char*)" ͨ��δ���ӣ�", X0+35, Y0+10);
		GUI_DispStringAt((const char*)"", X0+35, Y0+16);
		GUI_DispStringAt((const char*)" �밴Enter������", X0+35, Y0+28);
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
 * ��������ProgressBar
 * ����  ����������ʾ����
 * ����  ����
 * ���  ����
 * ����  ����
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
 * ��������GUI_Draw_self_check
 * ����  �������Լ�
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_self_check(void)
{
	uint8_t sign = 0;
    
	clear_();
	GUI_SetFont(&GUI_FontHZ_SimSun_12);

	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"1.��Դ"   , 0+15, 0);
		GUI_DispStringAt((const char*)"2.����"   , 0+15, 15);
		GUI_DispStringAt((const char*)"3.ʱ��"   , 0+15, 30);
		GUI_DispStringAt((const char*)"4.�洢��"	, 120, 0);
		GUI_DispStringAt((const char*)"5.��ѹ���", 120, 15);
		GUI_DispStringAt((const char*)"6.�������", 120, 30);
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
    
	ProgressBar();/* ��Դ */
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
	ProgressBar();/* ���� */
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
	ProgressBar();/* ʱ�� */
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
	ProgressBar();/* �洢�� */
	
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
	ProgressBar();/* ��ѹ */
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
	ProgressBar();/* ���� */
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
			GUI_DispStringAt((const char*)"���󣡰����������...", 50, 50);
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
 * ��������li_chun_dingzhi_main_ui
 * ����  ��������������������
 * ����  ����
 * ���  ����
 * ����  ����
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
        sprintf(t_buf,"������������   Ver:%s",g_cur_type->ver);
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
        GUI_DispStringAt((const char*)"�Ϻ����������Ǳ�", 0, 0);
        GUI_SetFont(&GUI_FontHZ_SimSun_12);
        GUI_DispStringAt((const char*)"http://www.shlcjm.com", 20, 52);
        
        draw_short_cut_str("����", "�˵�", "����", "����");
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
    GUI_DispStringAt((const char*)t_buf, 20+6*0, 28);/* ��ʾ���� */
    
    GUI_DispStringAt((const char*)gUpdateTime, 20, 40);/* ��ʾʱ�� */
    
    draw_menu_frame_line(0, 23, 200, 23);/* �����˵������ */
    
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
    
    /* ���������ֹ����Խ�� */
    strncpy(n_buf, g_cur_type->name, 20);
    strncpy(v_buf, g_cur_type->ver, 20);
    
    sprintf(t_buf,"%s   Ver:%s", n_buf, v_buf);
    
	if(sys_par.language == CHINESE)
	{
		GUI_SetFont(&GUI_FontHZ_FangZhengXingKai_24);
		GUI_DispStringAt((const char*)"�� ʢ �� ��", 45, 0);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
		GUI_DispStringAt((const char*)"http://www.csallwin.com", 20, 52);
		GUI_DrawBitmap((const GUI_BITMAP*) &bmlogo, 10, 0);
        
        draw_short_cut_str("����", "�˵�", "����", "����");
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
    GUI_DispStringAt((const char*)t_buf, 20+6*0, 28);/* ��ʾ���� */
    
    GUI_DispStringAt((const char*)gUpdateTime, 20, 40);/* ��ʾʱ�� */
    
    draw_menu_frame_line(0, 23, 200, 23);/* �����˵������ */
    
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
// 		GUI_DispStringAt((const char*)"�� ʢ �� ��", 45, 0);
// 		GUI_SetFont(&GUI_FontHZ_SimSun_12);
// 		GUI_DispStringAt((const char*)"http://www.csallwin.com", 20, 52);
// 		GUI_DrawBitmap((const GUI_BITMAP*) &bmlogo, 10, 0);
        
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
        draw_short_cut_str("����", "�˵�", "����", "����");
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
    GUI_DispStringAt((const char*)t_buf, 20+6*0, 28);/* ��ʾ���� */
    
    GUI_DispStringAt((const char*)gUpdateTime, 20, 40);/* ��ʾʱ�� */
    
    draw_menu_frame_line(0, 23, 200, 23);/* �����˵������ */
    
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
// 		GUI_DispStringAt((const char*)"�� ʢ �� ��", 45, 0);
// 		GUI_SetFont(&GUI_FontHZ_SimSun_12);
// 		GUI_DispStringAt((const char*)"http://www.csallwin.com", 20, 52);
// 		GUI_DrawBitmap((const GUI_BITMAP*) &bmlogo, 10, 0);
        
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
        draw_short_cut_str("����", "�˵�", "����", "����");
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
    GUI_DispStringAt((const char*)t_buf, 20+6*0, 28);/* ��ʾ���� */
    
    GUI_DispStringAt((const char*)gUpdateTime, 20, 40);/* ��ʾʱ�� */
    
    draw_menu_frame_line(0, 23, 200, 23);/* �����˵������ */
    
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
            li_chun_dingzhi_main_ui();//�Ϻ����������������ƻ�
        }
    }
    #endif
    
    #ifdef VTC9929_DZ
    if(flag == 0)
    {
        if(g_cur_type->num == VTC9929)
        {
            flag = 1;
            vtc9929_mian_ui();//����������ֻֻ��ʾVTC9929 �汾�� ʱ��
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
 * ��������GUI_Draw_main
 * ����  ������������
 * ����  ����
 * ���  ����
 * ����  ����
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
 * ��������MenuTitle
 * ����  �����±���������Ϣ ���ý���ʹ��
 * ����  ��which ģʽ
 * ���  ����
 * ����  ����
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
    GUI_DispStringAt((const char*)"��" , TITLE_STEP_COLON_X, TITLE_Y);
	GUI_DispStringAt((const char*)"��" , TITLE_FILE_COLON_X, TITLE_Y);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"����" , TITLE_STEP_X, TITLE_Y);
		GUI_DispStringAt((const char*)"�ļ�" , TITLE_FILE_X, TITLE_Y);
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
 * ��������syn_test_menu_title
 * ����  �����±���������Ϣ ���Խ���ʹ��
 * ����  ��which ģʽ
 * ���  ����
 * ����  ����
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
 * ��������test_menu_title
 * ����  �����±���������Ϣ ���Խ���ʹ��
 * ����  ��which ģʽ
 * ���  ����
 * ����  ����
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
// 			GUI_SetTextMode(GUI_TEXTMODE_REV);//���÷�ת�ı�
// 		}
// 	}
// 	GUI_SetFont(&GUI_FontHZ_SimSun_12);
// 	GUI_DispStringAt((const char*)"��" , 4+4*6-1, 0);
// 	GUI_DispStringAt((const char*)"��" , 4+35+38+15+20+4*6, 0);
// 	GUI_DispDecAt(g_cur_step->one_step.com.step, 4+35, 0, 2);
// 	GUI_DispDecAt(g_cur_file->total, 4+35+18, 0, 2);
//     
// 	if(sys_par.language == CHINESE)
// 	{
// 		GUI_DispStringAt((const char*)"����" , 4, 0);
// 		GUI_DispStringAt((const char*)"�ļ�" , 4+35+38+15+25-3, 0);
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
// 	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//���������ı�
// }

/*
 * ��������draw_port_for_set
 * ����  �������˿�������Ϣ ���ý���ʹ��
 * ����  ��which ģʽ
 * ���  ����
 * ����  ����
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
                /* �Ƕ�· */
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
 * ��������draw_port
 * ����  �������˿�������Ϣ ���Խ���ʹ��
 * ����  ��which ģʽ
 * ���  ��P x y
 * ����  ����
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
                /* �Ƕ�· */
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
 * ��������dis_port
 * ����  �����¶˿�״̬��Ϣ ���Խ���ʹ��
 * ����  ����
 * ���  ����
 * ����  ����
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
 * ��������gui_draw_set_sys_cur_limit
 * ����  ������֧·����ʱ˵��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_set_sys_cur_limit(void)
{
	if(sys_par.language == CHINESE)
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
		GUI_DispStringAt((const char*)"˵����֧·����", 4, 15);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Illus.:Branch CUR.", 4, 15);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
}
/*
 * ��������draw_short_cut_bm
 * ����  ���ұ߿�ݼ��� ͼ����ʾ����
 * ����  ��type ���ݲ�ͬ�����;�����ʾ��Щͼ��
 * ���  ����
 * ����  ����
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
 * ��������draw_short_cut_str
 * ����  ���ұ߿�ݼ��� ������ʾ
 * ����  ��str1 str2 str3 str4 �ֱ����4�������
 * ���  ����
 * ����  ����
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
 * ��������draw_menu_frame_line
 * ����  �������˵������ ��������һ��ֱ�� ����ͨ����������
 * ����  ��x1 x2 y ָ������ֱ�ߵ����� ��� x1>x2�����ٻ�������
 * ���  ����
 * ����  ����
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
 * ��������gui_draw_menu_sys
 * ����  ���˵����ý����²˵���
 * ����  ����
 * ���  ����
 * ����  ����
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
    
    draw_menu_frame_line(0, 0, 0, 0);/* �����˵������ */
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 3, 4);
	draw_short_cut_bm(UDRB_RE);
}

/*
 * ��������gui_draw_menu
 * ����  ����ʾһ��������Ľ����� cs99xx_type.c��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_menu(void)
{
	clear_();
    
	draw_menu_frame_line(0, 0, 0, 0);/* �����˵������ */
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 1, 4);
    draw_short_cut_bm(UDRB_RE);
}

void draw_custom_type_gui(void)
{
	clear_();
    
	draw_menu_frame_line(0, 0, 0, 0);/* �����˵������ */
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 1, 4);
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"��", 5+6*4, 16);
	GUI_DispStringAt((const char*)"��", 5+6*4, 32);

    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"ԭ�ͺ�  ��", 5, 16);
		GUI_DispStringAt((const char*)"�����ͺţ�", 5, 32);
	}
	else
	{
		GUI_DispStringAt((const char*)"Cur.", 5, 16);
		GUI_DispStringAt((const char*)"Set", 5, 32);
	}
    
	draw_menu_frame_line(0, 0, 0, 0);/* �����˵������ */
    
    draw_short_cut_bm(LRDB);
}
/*
 * ��������gui_draw_help_1
 * ����  �������˵�1
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_help_1(void)
{
	GUI_ClearRect (0, 0, 200-1, 64-1);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"��������ע������", 40, 2);
		GUI_DispStringAt((const char*)"1/3", 50+120, 2);

		GUI_DispStringAt((const char*)"��������������˿ڼ��������", 8, 18);
		GUI_DispStringAt((const char*)"���������쳣״��������رյ�Դ", 8, 34);
		GUI_DispStringAt((const char*)"�����������ƽӵأ�������˵����", 8, 50);
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
    
	draw_menu_frame_line(40, 15, 135, 15);/* �����˵������ */
    draw_short_cut_bm(UD_B_RE);
}

/*
 * ��������gui_draw_help_2
 * ����  �������˵�1
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_help_2(void)
{
	GUI_ClearRect (0, 0, 200-1, 64-1);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"ϵͳͼ��˵��", 53, 2);
		GUI_DispStringAt((const char*) "2/3", 50+120, 2);
		GUI_DispStringAt((const char*)"�Ϸ���", 40, 18);
		GUI_DispStringAt((const char*)"�·���", 40+100, 18);

		GUI_DispStringAt((const char*)"��ݼ�", 40, 34);
		GUI_DispStringAt((const char*)"���Ƽ�", 40+100, 34);

		GUI_DispStringAt((const char*)"ɾ����", 40, 50);
		GUI_DispStringAt((const char*)"���ؼ�", 40+100, 50);
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
    
	draw_menu_frame_line(40, 15, 135, 15);/* �����˵������ */
    draw_short_cut_bm(UD_B_RE);
}

/*
 * ��������gui_draw_help_3
 * ����  �������˵�1
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_help_3 (void)
{
	GUI_ClearRect (0, 0, 200-1, 64-1);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"ϵͳ��ݼ�˵��", 45, 2);
		GUI_DispStringAt((const char*)"3/3", 40+130, 2);
		
		GUI_DispStringAt((const char*)"��Ч��Χ: ", 10, 20);
		GUI_DispString("������Ͱ���������");
		
		GUI_DispStringAt((const char*)"Enter+0: ", 10, 36);
		GUI_DispString("ϵͳ��Ӣ�Ŀ����л���");
		
		GUI_DispStringAt((const char*)"Enter+1: ", 10, 52);
		GUI_DispString("ϵͳ��ʼ������ٻָ���");
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
	
	draw_menu_frame_line(40, 15, 135, 15);/* �����˵������ */
    draw_short_cut_bm(UD_B_RE);
}


/*
 * ��������gui_draw_file_management
 * ����  ���ļ��������
 * ����  ����
 * ���  ����
 * ����  ����
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
	draw_menu_frame_line(0, 0, 0, 0);/* �����˵������ */
    draw_short_cut_bm(UDSB_RE);
}

/*
 * ��������gui_draw_communication_interface
 * ����  ��ͨ�Ž���
 * ����  ����
 * ���  ����
 * ����  ����
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
    
    draw_menu_frame_line(0, 0, 0, 0);/* �����˵������ */
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 3, 4);
    
    draw_short_cut_bm(UDRB_RE);
}

/*
 * ��������gui_draw_communication_plc
 * ����  ��PLCͨ�ſ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_communication_plc(void)
{
	clear_();
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"PLC", 100, 2);
	
	GUI_DispStringAt((const char*) "PLC����:", 3, 18);
	GUI_DispStringAt((const char*)"��", 3+80, 18);
	GUI_DispStringAt((const char*)"�ر�", 3+80+50, 18);
	
	GUI_DispStringAt((const char*)"˵   ��:", 3, 32);
	GUI_DispStringAt((const char*)"ͨ��PLC�ӿڽ��п��ƣ�",3+50, 32);
	GUI_DispStringAt((const char*)"ֻ�и�λ����Ч��", 3+50, 46);
	
    draw_menu_frame_line(20, 14, 180, 14);/* �����˵������ */
	
 	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,65, 20 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,65+50, 20 );
    
    draw_short_cut_bm(__RB_RE);
}
/*
 * ��������gui_draw_communication_agreement
 * ����  ��ͨ�ſ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_communication_agreement(void)
{
	clear_();
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"ͨ��", 100, 2);
	
	GUI_DispStringAt((const char*)"ͨѶ����:", 3, 18);
	GUI_DispStringAt((const char*)"��", 3+70, 18);
	GUI_DispStringAt((const char*) "�ر�", 3+70+40, 18);
	
	GUI_DispStringAt((const char*)"�� �� ��:", 3, 33);
	GUI_SetFont(&GUI_Font10_1);
	GUI_DispStringAt((const char*)"9600", 3+70, 33);
	GUI_DispStringAt((const char*)"14400", 3+70+40, 33);
 	GUI_DispStringAt((const char*)"19200", 3+70+40+50, 33);
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"������ַ:", 3, 48);
	GUI_SetFont(&GUI_Font10_1);
	GUI_DispStringAt((const char*)"001", 3+60, 48);
	GUI_DispStringAt((const char*)"(1-255)",3+60+30, 48);
	
    draw_menu_frame_line(20, 14, 180, 14);/* �����˵������ */
	
 	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,60, 20 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,60+40, 20 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,60, 34 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,60+40, 34 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,60+40+50, 34 );
    
    draw_short_cut_bm(UDRB_RE);
}

/*
 * ��������gui_draw_communication_agreement
 * ����  ��ͨ�ſ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_password_manage(void)
{
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"��", 5+6*8, 2);	
	GUI_DispStringAt((const char*)"��", 5+6*8, 25);	
	GUI_DispStringAt((const char*)"��", 5+6*8, 46);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"ԭʼ����", 5, 2);	
		GUI_DispStringAt((const char*)"��������", 5, 25);	
		GUI_DispStringAt((const char*)"ȷ������", 5, 46);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Old", 5+10, 2);
		GUI_DispStringAt((const char*)"New", 5+10, 25);
		GUI_DispStringAt((const char*)"Enter", 5+10, 46);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
	draw_menu_frame_line(0, 0, 0, 0);/* �����˵������ */
	
    draw_short_cut_bm(LRDB_RE);
}

/*
 * ��������gui_draw_time_manage
 * ����  ��ϵͳʱ�����ý���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_time_manage(void)
{
	char strtemp[20];
    
	clear_();
	strcpy(strtemp, gUpdateTime);
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"��", 5+6*4, 16);
	GUI_DispStringAt((const char*)"��", 5+6*4, 32);
	GUI_DispStringAt((const char*)strtemp, 5+40, 17);
	GUI_DispStringAt((const char*)strtemp, 5+40, 32);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"��ǰ��", 5, 16);
		GUI_DispStringAt((const char*)"���裺", 5, 32);
		GUI_DispStringAt((const char*)"����ʱ�䣺0+1 ��ϼ�", 5, 32 + 15);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Cur.", 5, 16);
		GUI_DispStringAt((const char*)"Set", 5, 32);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
		GUI_DispStringAt((const char*)"ResetTime:0+1 CombinationKey", 5, 32 + 15);
	}
    
	draw_menu_frame_line(0, 0, 0, 0);/* �����˵������ */
    
    draw_short_cut_bm(UDRB_RE);
}

/*
 * ��������gui_draw_offset
 * ����  ��ƫ�Ʋ��Խ�����ʾ
 * ����  ����
 * ���  ����
 * ����  ����
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
	
    draw_menu_frame_line(145, 18, 145, 64-5);/* �����˵������ */
    
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+3);
	
	draw_short_cut_bm(UDSB_RE);
}
/*
 * ��������gui_draw_bbd
 * ����  ��BBD���Խ�����ʾ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_bbd(void)
{
	clear_();
	
	GUI_DrawLine(0  , 12  , 200  , 12  );
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	GUI_DispStringAt((const char*)"��" , 4+35+38+15+20+4*6, 0);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"�ļ�" , 4+35+38+15+25-3, 0);
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
    
    draw_menu_frame_line(145, 18, 145, 64-5);/* �����˵������ */
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+3);
	
	draw_short_cut_bm(UDSB_RE);
}

/*
 * ��������gui_draw_test_result
 * ����  �����Խ����ʾ����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_test_result(void)
{
	clear_();/* ��Ļ����ˢ�� ���в���˸ */
	MenuTitle(NUL);
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"0001.", 12, 15);
	GUI_DispStringAt((const char*)"0002.", 12, 27);
	GUI_DispStringAt((const char*)"0003.", 12, 39);
	GUI_DispStringAt((const char*)"0004.", 12, 51);
// 	GUI_DispStringAt((const char*)"----" , 20+30, 51);
// 	GUI_DispStringAt((const char*)"----" , 20+30+80, 51);
// 	GUI_DispStringAt((const char*)"P"    , 20+30+60+80, 51);
	
	draw_menu_frame_line(0, 12, 200, 12);/* �����˵������ */
    
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 0, 15+3);
	
	draw_short_cut_bm(UDSB_RE);
}

/*
 * ��������gui_draw_keyboard_lock_manage
 * ����  ����������ʾ����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_keyboard_lock_manage(void)
{
	clear_();
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"��", 3+6*7, 5);
	GUI_DispStringAt((const char*)"��", 3+6*7, 20);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"��������", 3, 5);
		GUI_DispStringAt((const char*)"��", 3+70, 5);
		GUI_DispStringAt((const char*)"�ر�", 3+70+50, 5);
        
		GUI_DispStringAt((const char*)"˵  ����", 3, 20);
		GUI_DispStringAt((const char*)"�����������Ƿ���������", 3+50, 20);
		GUI_DispStringAt((const char*)"���룬����������Ϊ��", 3+50, 34);
		GUI_DispStringAt((const char*)"���߹رա�", 3+50, 48);
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
    
    draw_menu_frame_line(0, 0, 0, 0);/* �����˵������ */
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, 3+60, 2+5+1 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, 3+60+50, 2+5+1 );
	
	draw_short_cut_bm(UDRB_RE);
}

/*
 * ��������gui_draw_recover
 * ����  ���ָ�Ĭ��ֵ��ʾ����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_recover(void)
{
	clear_();
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"��", 3+6*7, 5);
	GUI_DispStringAt((const char*)"��", 3+6*7, 20);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"�ָ�Ĭ��ֵ��", 3, 5);
		GUI_DispStringAt((const char*)"ȷ��", 3+70 + 12, 5);
		GUI_DispStringAt((const char*)"ȡ��", 3+70+50 + 12, 5);
        
		GUI_DispStringAt((const char*)"˵  ����", 3, 20);
		GUI_DispStringAt((const char*)"�ָ�Ĭ��ֵ������������", 3+50, 20);
		GUI_DispStringAt((const char*)"���ᶪʧ��", 3+50, 34);
// 		GUI_DispStringAt((const char*)"���߹رա�", 3+50, 48);
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
    
    draw_menu_frame_line(0, 0, 0, 0);/* �����˵������ */
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, 3+60 + 12, 2+5+1 );
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, 3+60+50 + 12, 2+5+1 );
	
	draw_short_cut_bm(UDRB_RE);
}

/*
 * ��������gui_draw_test
 * ����  �����Խ�����ʾ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_test (void)
{
	uint8_t X	= TEST_ORIGIN_X;
	uint8_t Y	= TEST_ORIGIN_Y;
	uint8_t O	= TEST_UI_OFFSET;
	uint8_t CW	= 9;	/* ���ַ���� */
    
// 	MenuTitle(NUL);
	
	/* ע�͵���Ϊ��ȥ�� ��ʾ����˸���� */
// 	GUI_SetFont(&GUI_FontHZ_SimSun_15);
// 	GUI_DispStringAt((const char*)"0.050"	, X, Y);//�����ѹ
// 	GUI_DispStringAt((const char*)"0.500"	, X+11*CW, Y);//�������
// 	GUI_DispStringAt((const char*)"3.0"	, X+2*CW, Y+O);//ͬ��ʱ��
	
	GUI_SetFont(&GUI_FontHZ_SimSunF_15);
	GUI_DispStringAt((const char*)"kV", X+CW*5+1, Y);
	GUI_DispStringAt((const char*)"s"	, X+CW*5, Y+O);
// 	GUI_DispStringAt((const char*)"mA", X+CW*16, Y);
    
	GUI_SetFont(&GUI_FontHZ_SimSun_16);//С��
	GUI_ClearRect (GUI_GEN_STATUS_X1, GUI_GEN_STATUS_Y, GUI_GEN_STATUS_X2, GUI_GEN_STATUS_Y + GUI_GEN_STATUS_CH);
    
    GUI_DispStringAt_BTP_STR_(status_str[TEST_WAIT_INDEX][!sys_par.language],
        GUI_GEN_STATUS_X1, GUI_GEN_STATUS_X2, GUI_GEN_STATUS_Y, GUI_GEN_STATUS_CW);
    
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
    draw_menu_frame_line(0, 12, 200, 12);/* �����˵������ */
    
	GUI_DrawLine(200 - 6 * 5 - 2, 12   , 200 - 6 * 5 - 2, 30);
	GUI_DrawLine(200 - 6 * 5 - 2, 30   , 200  , 30);
	
    if(g_comm_status == LOCAL_COMM)
    {
        draw_short_cut_bm(UDSB);
    }
    else
    {
        draw_short_cut_str("", "", (uint8_t*)RT_STRING("Զ��", "Remote"), "");
    }
}

/*
 * ��������gui_drow_test_syn
 * ����  ����·ͬ�����Խ�����ʾ
 * ����  ����
 * ���  ����
 * ����  ����
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
	
// 	GUI_DispStringAt       ("0.050", 30, 13+10*0);//�����ѹ
// 	GUI_DispStringAt       ("0.500", 30, 13+10*1);//�������
// 	GUI_DispStringAt       ("3.0", 30+6*2, 13+10*2);//ͬ��ʱ��
	
// 	GUI_SetFont(&GUI_FontHZ_SimSun_16);//С��
// 	GUI_DispStringAt       ("�ȴ�����", 10, 45);//�ȴ�����
	
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
    
    draw_menu_frame_line(0, 12, 82, 12);/* �����˵������ */
    
    draw_short_cut_bm(UDSB);
}



/*
 * ��������prompt_recover_pw
 * ����  �����û�����ENTER+1��ϼ�ʱ��ʾ�û�ϵͳ�����Ѿ���ԭΪĬ��ֵ �ú�����help_subtask,main_task����
 * ����  ��uint8_t s �����ڲ�ͬ״̬�¸��û���һЩ��ʾ��Ϣ
 * ���  ����
 * ���ú�����GUI_SetFont��GUI_DispStringAt��GUI_DrawLine��GUI_ClearRect��
 *			OSQPend��GUI_DrawBitmap
 * ����������test_subtask
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
		GUI_DispStringAt((const char*)"ϵͳ�����Ѿ���", 0+X0+35, Y0+4);
		GUI_DispStringAt((const char*)"ԭΪ��ʼ���룡", 0+X0+35, Y0+4+14);
		GUI_DispStringAt((const char*)"��ο�˵���顣", 0+X0+35, Y0+4+14+14);
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
	OSTimeDlyHMSM(0,0,2,0);	/* ��ʾ����ʾ2s */
	on_keyboard();
	
	GUI_ClearRect (X0, Y0, X1, Y1);
}

/*
 * ��������dis_program_info
 * ����  ����ʾ��¼�ļ���ʱ����Ϣ
 * ����  ����
 * ���  ����
 * ����  ����
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
		GUI_DispStringAt((const char*)"����:"__DATE__, 0+X0+35, Y0+4);
		GUI_DispStringAt((const char*)"ʱ��:"__TIME__, 0+X0+35, Y0+4+14);
		GUI_DispStringAt((const char*)"�����������" , 0+X0+35, Y0+4+14+18);
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
		GUI_DispStringAt((const char*)"����汾:v1.19.01", 0+X0+35, Y0+4);
		GUI_DispStringAt((const char*)"Ӳ���汾:v1.9.1", 0+X0+35, Y0+4+14);
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
 * ��������dis_scan_modules_gui
 * ����  ����ʾɨ��ģ�������Ϣ
 * ����  ����
 * ���  ����
 * ����  ����
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
		GUI_DispStringAt((const char*)"ɨ", 0+X0+OFFSET_X, Y0+2+12*0);
		GUI_DispStringAt((const char*)"��", 0+X0+OFFSET_X, Y0+2+12*1);
		GUI_DispStringAt((const char*)"ģ", 0+X0+OFFSET_X, Y0+2+12*2);
		GUI_DispStringAt((const char*)"��", 0+X0+OFFSET_X, Y0+2+12*3);
		GUI_DispStringAt((const char*)"01" , 0+X0+OFFSET_X, Y0+2+12*4);
// 		GUI_DispStringAt((const char*)"1.DC_GR  ID:01", 0+X0+2+18, Y0+2+12*0);
// 		GUI_DispStringAt((const char*)"2.", 0+X0+OFFSET_X+18, Y0+OFFSET_Y+12*1);
// 		GUI_DispStringAt((const char*)"3.", 0+X0+OFFSET_X+18, Y0+OFFSET_Y+12*2);
// 		GUI_DispStringAt((const char*)"4.", 0+X0+OFFSET_X+18, Y0+OFFSET_Y+12*3);
// 		GUI_DispStringAt((const char*)"5.", 0+X0+OFFSET_X+18, Y0+OFFSET_Y+12*4);
// 		GUI_DispStringAt((const char*)"ģ���ַ:", 0+X0+2, Y0+4+14);
// 		GUI_DispStringAt((const char*)"ģ������:" , 0+X0+2, Y0+4+14+18);
        draw_short_cut_str(NULL, NULL, "����", NULL);
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

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
