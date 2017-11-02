/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_type.c
 * ժ  Ҫ  �������������ͺţ�ֱ�Ӷ���������Ӧ���е�����
 * ��ǰ�汾��V1.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#define _CS99xx_TYPE

static void init_speciality(void);
#include	<string.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include    "cs99xx_struct.h"
#include	"cs99xx_type.h"
#include    "cs99xx_type_config.h"
#include	"calibration.h"

#define __TEST			1
#define __TEST_TYPE		CS9929AX_CC//CS9929AX_CC //CS9927LB//TSK7630//CS9933ET_1//CS9922BX//CS9922BX_DZ//CS9917A_8//CS9950CX //CS9917CX_20//

#define VERSION_NUMBER__    "2.20.74"	/* �汾�� */



#define CUSTOM_NAME_        "9923ECG" //���ƻ�����
#define CUSTOM_TYPE_        CS9923ECG  //���ƻ��ͺ�
#define CONFIG_TYPE_FUN_    config_CS9923ECG //���ƻ����ú���

#ifdef	RH8400AS_DZ
#define CUSTOM_NAME        "RH8400AS" //���ƻ�����
#define CUSTOM_TYPE        CS9933X  //���ƻ��ͺ�
#define CONFIG_TYPE_FUN    config_RH8400AS //���ƻ����ú���
#endif

#ifdef LICHUN_DZ
#define CUSTOM_NAME         "CS9906B_K" //���ƻ�����
#define CUSTOM_TYPE         CS9906B_K     //���ƻ��ͺ�
#define CONFIG_TYPE_FUN     config_cs9906b_k //���ƻ����ú���
#endif

#ifdef VTC9929_DZ   /* ACW DCW IR 8· */
#define CUSTOM_NAME         "VTC9929" //���ƻ�����
#define CUSTOM_TYPE         VTC9929     //���ƻ��ͺ�
#define CONFIG_TYPE_FUN     config_VTC9929 //���ƻ����ú���
#endif

#ifdef TSK7630_DZ   /* ACW DCW IR 8· */
#define CUSTOM_NAME         "TSK7630" //���ƻ�����
#define CUSTOM_TYPE         TSK7630     //���ƻ��ͺ�
#define CONFIG_TYPE_FUN     config_TSK7630 //���ƻ����ú���
#endif

#ifndef CUSTOM_NAME
#define CUSTOM_NAME         CUSTOM_NAME_    //���ƻ�����
#endif
#ifndef CUSTOM_TYPE
#define CUSTOM_TYPE         CUSTOM_TYPE_     //���ƻ��ͺ�
#endif
#ifndef CONFIG_TYPE_FUN
#define CONFIG_TYPE_FUN     CONFIG_TYPE_FUN_ //���ƻ����ú���
#endif

#define TYPE_SEL_HARD   0//1//     /* ͨ��Ӳ��ѡ�� 0��� 1Ӳ�� */

uint16_t get_arc_h(uint8_t mode);

static void init_speciality(void);


/* ����Ƕ��ƻ����ڰ汾�ź����-DZ */
#if CUSTOM_TYPE_EN
#define VERSION_EX      "-DZ"
#else
#define VERSION_EX
#endif

#define   VERSION_NUMBER  (VERSION_NUMBER__ VERSION_EX)
#if CUSTOM_TYPE_EN
const static TYPE_STR custom_type=
{
	/* �ͺ�����       �汾	         ���(ID)        ���ú���    */
    CUSTOM_NAME, VERSION_NUMBER, CUSTOM_TYPE, CONFIG_TYPE_FUN
};

#endif

uint8_t * get_version_number(void)
{
	return VERSION_NUMBER;
}
/*******************************************************************************/
/*******************************************************************************/
/* �汾��Ϣ���� ��ɾ�Ĳ��������� */
const static TYPE_STR type_pool[] __attribute__((at(TYPE_POOL_ADDR))) = {
	/* �ͺ�����       �汾	         ���(ID)        ���ú���    */
	{"DEBUG_TYPE" ,	VERSION_NUMBER, DEBUG_TYPE  , config_debug_type     },/* ����ʹ�� */
    
	{"CS9906A"	  ,	VERSION_NUMBER, CS9906A     , config_CS9906A        },/* �ص� ֱ��GR ѹ�������� ֱ����ʾѹ�� */
	{"CS9906B"	  ,	VERSION_NUMBER, CS9906B     , config_CS9906B        },/* �ص� ֱ��GR ѹ�������� ֱ����ʾѹ�� */
	{"CS9906B-K"  ,	VERSION_NUMBER, CS9906B_K   , config_CS9906B_K      },/* �ص� ֱ��GR ѹ�������� ֱ����ʾѹ�� */
    
	{"CS9912AX"   ,	VERSION_NUMBER, CS9912AX    , config_CS9912AX       },
	{"CS9912BX"   ,	VERSION_NUMBER, CS9912BX    , config_CS9912BX       },/* ACW DCW */
	{"CS9912BX-L" ,	VERSION_NUMBER, CS9912BX_L  , config_CS9912BX_L     },/* ACW DCW */
	{"CS9912BX-T" ,	VERSION_NUMBER, CS9912BX_T  , config_CS9912BX_T     },/* DCW 2kV ˫�����ѹ�� */
	{"CS9912Y"    ,	VERSION_NUMBER, CS9912Y     , config_CS9912Y        },/* ACW DCW */
    
	{"CS9913AX"   ,	VERSION_NUMBER, CS9913AX    , config_CS9913AX       },/* ACW */
	{"CS9913BX"   ,	VERSION_NUMBER, CS9913BX    , config_CS9913BX       },/* ACW DCW */
    
	{"CS9914AM"	  ,	VERSION_NUMBER, CS9914AM    , config_CS9914AM       },/* ACW */
	{"CS9914AX-200", VERSION_NUMBER, CS9914AM   , config_CS9914AM       },/* ACW */
	{"CS9914AM-H" , VERSION_NUMBER, CS9914AM_H  , config_CS9914AM_H     },/* ��ACW  200mA ˫�����ѹ�� */
	{"CS9914AX"   ,	VERSION_NUMBER, CS9914AX    , config_CS9914AX       },/* ACW */
	{"CS9914AX-1" ,	VERSION_NUMBER, CS9914AX_1  , config_CS9914AX_1     },/* ACW */
    
	{"CS9914BM"	  ,	VERSION_NUMBER, CS9914BM    , config_CS9914BM       },/* ACW DCW */
	{"CS9914BX"   ,	VERSION_NUMBER, CS9914BX    , config_CS9914BX       },/* ACW DCW */
	{"CS9914CX"   ,	VERSION_NUMBER, CS9914CX    , config_CS9914CX       },/* DCW */
	{"CS9914D"	  ,	VERSION_NUMBER, CS9914D     , config_CS9914D        },/* ACW */
	{"CS9914CX-C" ,	VERSION_NUMBER, CS9914CX_C  , config_CS9914CX_C     },/* DCW */
	{"CS9914DX"	  ,	VERSION_NUMBER, CS9914DX    , config_CS9914DX       },/* ACW */
	{"CS9914G"    ,	VERSION_NUMBER, CS9914G     , config_CS9914G        },/* ACW DCW */
	{"CS9914Y"    ,	VERSION_NUMBER, CS9914Y     , config_CS9914Y        },/* ACW DCW */
	
	{"CS9915AX"   ,	VERSION_NUMBER, CS9915AX    , config_CS9915AX       },/* ACW PWM */
    
	{"CS9916AX"   ,	VERSION_NUMBER, CS9916AX    , config_CS9916AX       },/* ACW */
	{"CS9916BX"   ,	VERSION_NUMBER, CS9916BX    , config_CS9916BX       },/* DCW */
	
	{"CS9917A-8"  ,	VERSION_NUMBER, CS9917A_8   , config_CS9917A_8      },/* ACW DCW 8· */
	{"CS9919D"	  ,	VERSION_NUMBER, CS9919D     , config_CS9919D        },/* ACW DCW 8· */
	{"CS9917AX"   ,	VERSION_NUMBER, CS9917AX    , config_CS9917AX       },/* ACW DCW 4· */
	{"CS9917B"	  ,	VERSION_NUMBER, CS9917B     , config_CS9917B        },
	{"CS9917BX"   ,	VERSION_NUMBER, CS9917BX    , config_CS9917BX       },/* DCW 10kv ����ݲ��� */
	{"CS9917CX"	  ,	VERSION_NUMBER, CS9917CX    , config_CS9917CX       },
	{"CS9917CX-20",	VERSION_NUMBER, CS9917CX_20 , config_CS9917CX_20    },/* DCW ������©���� */
	{"CS9917CX-C" ,	VERSION_NUMBER, CS9917CX_C  , config_CS9917CX_C     },/* DCW 10kv ����ݲ��� */
	{"CS9917DX"	  ,	VERSION_NUMBER, CS9917DX    , config_CS9917DX       },/* ACW DCW 10kV */
    
	{"CS9919A"    ,	VERSION_NUMBER, CS9919A     , config_CS9919A        },/* ACW DCW 8· */
	{"CS9919A-8"  ,	VERSION_NUMBER, CS9919A_8   , config_CS9919A_8      },/* ACW DCW IR 8· */
	{"CS9919AX"   ,	VERSION_NUMBER, CS9919AX    , config_CS9919AX       },/* ACW DCW 8· */
	{"CS9919B"    ,	VERSION_NUMBER, CS9919B     , config_CS9919B        },/* ACW DCW 4· */
    {"9919B-4"    ,	VERSION_NUMBER, CS9919B_4   , config_CS9919B_4      },
	{"CS9919BX"   ,	VERSION_NUMBER, CS9919BX    , config_CS9919BX       },/* ACW DCW 4· */
	{"CS9919G"    ,	VERSION_NUMBER, CS9919G     , config_CS9919G        },/* ACW DCW 8· */
    
	{"CS9920A"    ,	VERSION_NUMBER, CS9920A     , config_CS9920A        },/* ACW 20kV */
	{"CS9920B"    ,	VERSION_NUMBER, CS9920B     , config_CS9920B        },/* DCW 20kV */
	{"CS9930A-T"  ,	VERSION_NUMBER, CS9930A_T   , config_CS9930A_T      },/* ACW 20kV */
    
	{"CS9921BX"   ,	VERSION_NUMBER, CS9921BX    , config_CS9921BX       },/* ACW DCW GR 32A */
    
	{"CS9922BX"   ,	VERSION_NUMBER, CS9922BX    , config_CS9922BX       },/* ACW DCW IR 10G */
	{"CS9922BXJ"  ,	VERSION_NUMBER, CS9922BXJ   , config_CS9922BXJ      },/* ACW DCW IR 10G */
	{"CS9922BX-1" ,	VERSION_NUMBER, CS9922BX_1  , config_CS9922BX_1     },/* ACW DCW IR 1.5kV 10G */
// 	{"CS9922BX-DZ",	VERSION_NUMBER, CS9922BX_DZ , config_CS9922BX_DZ    },/* ��Ե����������޼�һ��С���� */
    
	{"CS9922LB"   ,	VERSION_NUMBER, CS9922LB    , config_CS9922LB       },/* ACW DCW IR ﮵�ذ�������� */
	{"CS9922CX"   ,	VERSION_NUMBER, CS9922CX    , config_CS9922CX       },/* ACW IR 10G */
	{"CS9922DX"   ,	VERSION_NUMBER, CS9922DX    , config_CS9922DX       },/* ACW DCW IR 10G */
	{"CS9922EX"   ,	VERSION_NUMBER, CS9922EX    , config_CS9922EX       },/* �ص� ACW DCW IR 10G */
	{"CS9922EXJ"  ,	VERSION_NUMBER, CS9922EXJ   , config_CS9922EXJ      },/* �ص� ACW DCW IR 10G */
	{"CS9922FX"   ,	VERSION_NUMBER, CS9922FX    , config_CS9922FX       },/* ACW DCW IR 50G */
	{"CS9922G"    ,	VERSION_NUMBER, CS9922G     , config_CS9922G        },/* ACW DCW IR 50G */
	{"CS9922G-1"  ,	VERSION_NUMBER, CS9922G_1   , config_CS9922G_1      },/* ACW DCW IR 50G */
	{"CS9922G-2"  ,	VERSION_NUMBER, CS9922G_2   , config_CS9922G_2      },/* �ص� ir 2.5kV 50G */
	{"CS2676CH"   ,	VERSION_NUMBER, CS2676CH    , config_CS2676CH       },/* �ص� ir 2.5kV */
	{"CS2676CH-1" ,	VERSION_NUMBER, CS2676CH_1  , config_CS2676CH_1     },/* �ص� ir 5kV */
	{"CS9927LB"   ,	VERSION_NUMBER, CS9927LB    , config_CS9927LB       },/* �ص� BBD 50-500V */
	
	{"CS9922H-50" , VERSION_NUMBER, CS9922H_50  , config_CS9922H_50     },/* �ص� ir 5kV */
	{"CS9922M"    ,	VERSION_NUMBER, CS9922M     , config_CS9922M        },/* ACW DCW IR 10G */
    
	{"CS9923"     ,	VERSION_NUMBER, CS9923      , config_CS9923         },/* ACW DCW 10KV IR 1kV 50G */
	{"CS9923CG"   ,	VERSION_NUMBER, CS9923CG    , config_CS9923CG       },/* ACW DCW 10KV IR 1kV 50G */
	{"CS9923G-1"  ,	VERSION_NUMBER, CS9923G_1   , config_CS9923G_1      },/* �ص� 10kV��ѹ�� ir 1.5kV 50G */
	{"CS9923G-2"  ,	VERSION_NUMBER, CS9923G_2   , config_CS9923G_2      },/* �ص� 10kV��ѹ�� ir 2.5kV 50G */
	{"CS9923G-3"  ,	VERSION_NUMBER, CS9923G_3   , config_CS9923G_3      },/* �ص� 10kV��ѹ�� ir   5kV 50G */
	{"CS9923-5"   ,	VERSION_NUMBER, CS9923_5    , config_CS9923_5       },/* 10kV ACW DCW IR 1kV 50G */
	{"CS9923G"	  ,	VERSION_NUMBER, CS9923G     , config_CS9923G        },/* 10kV DCW IR 1kV 50G */
	{"CS9923ECG"  ,	VERSION_NUMBER, CS9923ECG   , config_CS9923ECG      },/* 10kV ACW DCW IR 2.5kV 50G GR 60A 105mOhM*/
	{"CS9923ECG-1",	VERSION_NUMBER, CS9923ECG_1 , config_CS9923ECG_1    },/* 10kV ACW DCW IR 5kV 50G GR 60A 105mOhM*/
    
	{"CS9924"     ,	VERSION_NUMBER, CS9924      , config_CS9924         },/* ACW ������ ˫�����ѹ�� */
	{"CS9924-200" ,	VERSION_NUMBER, CS9924_200  , config_CS9924_200     },/* ACW ������ ˫�����ѹ�� */
	
	{"CS9929A"    ,	VERSION_NUMBER, CS9929A     , config_CS9929A        },
	{"CS9929A-4"  ,	VERSION_NUMBER, CS9929A_4   , config_CS9929A_4      },/* �ص� ACW DCW IR 4· */
	{"CS9929A-8"  ,	VERSION_NUMBER, CS9929A_8   , config_CS9929A_8      },/* ACW DCW IR 8· */
	{"CS9929AX-8" ,	VERSION_NUMBER, CS9929AX_8  , config_CS9929AX_8     },/* ACW DCW IR 8· */
    
	{"CS9929AX"   ,	VERSION_NUMBER, CS9929AX    , config_CS9929AX       },/* ACW DCW IR */
	{"CS9929AX-CC",	VERSION_NUMBER, CS9929AX_CC , config_CS9929AX_CC    },/* ACW DCW IR */
	{"CS9929AX-T" ,	VERSION_NUMBER, CS9929AX_T  , config_CS9929AX_T     },/* ACW DCW IR CC */
	{"CS9929B"    ,	VERSION_NUMBER, CS9929B     , config_CS9929B        },/* �ص� ACW DCW IR 4· */
	{"CS9929B-4"  ,	VERSION_NUMBER, CS9929B_4   , config_CS9929B_4      },/* ACW DCW IR 5G */
	{"CS9929BX"   ,	VERSION_NUMBER, CS9929BX    , config_CS9929BX       },/* ACW DCW IR 10G */
	{"CS9929CX"   ,	VERSION_NUMBER, CS9929CX    , config_CS9929CX       },/* ACW DCW IR 50G */
	{"CS9929EX"   ,	VERSION_NUMBER, CS9929EX    , config_CS9929EX       },/* ACW DCW IR 50G */
	{"CS9929EX-CC",	VERSION_NUMBER, CS9929EX_CC , config_CS9929EX_CC    },/* ACW DCW IR 50G */
	
	{"CS9933EG"   , VERSION_NUMBER, CS9933EG    , config_CS9933EG       },/* �ص� ֱ��GR IR 1kV 50G */
	{"CS9933EG-1" ,	VERSION_NUMBER, CS9933EG_1  , config_CS9933EG_1     },/* �ص� IR 2.5kV 50G */
	{"CS9933EG-2" ,	VERSION_NUMBER, CS9933EG_2  , config_CS9933EG_2     },/* �ص� IR 5kV 50G */
	{"CS9933ET"	  ,	VERSION_NUMBER, CS9933ET    , config_CS9933ET       },/* �ص� IR 1kV ֱ��GR */
	{"CS9933ET-1" , VERSION_NUMBER, CS9933ET_1  , config_CS9933ET_1     },/* �ص� IR 1.5kV ֱ��GR */
	{"CS9933ET-2" , VERSION_NUMBER, CS9933ET_2  , config_CS9933ET_2     },/* �ص� IR 2.5V ֱ��GR */
	{"CS9933ET-3" , VERSION_NUMBER, CS9933ET_3  , config_CS9933ET_3     },/* �ص� IR 5kV ֱ��GR */
	{"CS9933G"    ,	VERSION_NUMBER, CS9933G     , config_CS9933G        },/* ACW DCW IR 50G GR 32A */
	{"CS9933G-4"  ,	VERSION_NUMBER, CS9933G_4   , config_CS9933G_4      },/* ACW DCW IR 50G GR 40A */
	{"CS9933X"    ,	VERSION_NUMBER, CS9933X     , config_CS9933X        },/* ACW DCW IR GR */
	{"CS9933X-100",	VERSION_NUMBER, CS9933X_100 , config_CS9933X_100    },/* ACW DCW IR 10G GR 32A */
    
	{"CS9939X"	  ,	VERSION_NUMBER, CS9939X     , config_CS9939X        },/* ACW DCW IR 10G ACGR 32A */
    
	{"CS9950CG"   ,	VERSION_NUMBER, CS9950CG    , config_CS9950CG       },/* ��GR AC_32A */
	{"CS9950CG-1" , VERSION_NUMBER, CS9950CG_1  , config_CS9950CG_1     },/* ��GR AC_40A */
	{"CS9950CX"   ,	VERSION_NUMBER, CS9950CX    , config_CS9950CX       },/* ��GR AC_40A */
	{"CS9950DX"   ,	VERSION_NUMBER, CS9950DX    , config_CS9950DX       },/* ��GR AC_60A */
	{"CS9950ED"   ,	VERSION_NUMBER, CS9950ED    , config_CS9950ED       },/* ��GR DC_100A */
	{"CS9933G+"   ,	VERSION_NUMBER, CS9933G_    , config_CS9933G_       },
	{"CS9950X"    ,	VERSION_NUMBER, CS9950X     , config_CS9950X        },
	
	{0},/* ��β�ÿճ�ʼ�� */
};

/**************************** �������� *************************/
/*
 * ��������read_type_pin
 * ����  ����Ӳ�����ͺ�����
 * ����  ����
 * ���  ����
 * ����  ���ͺ�ֵ
 */
uint16_t read_type_pin(void)
{
#if TYPE_SEL_HARD>0&&__TEST==0
	uint16_t data1 = 0;
	uint16_t data2 = 0;
// 	uint16_t data3 = 0;
	
	data1 = GPIO_ReadInputData(GPIOF);/* PIN 6 - 11 */
	data2 = GPIO_ReadInputData(GPIOC);/* PIN 7 8 */
// 	data3 = GPIO_ReadInputData(GPIOA);
	
	data1 >>= 6;
	data1 &= 0x03f;
	data2 >>= 1;
	data2 &= 0xc0;
// 	data3 &= 0x0100;
	data1 |= data2;
#endif
    
#if __TEST>0
	return __TEST_TYPE;
#else
    //ͨ�����ѡ��
#if TYPE_SEL_HARD==0
    return *(uint16_t*)TYPE_NUM_ADDR;
#else
    //��Ӳ��ѡ���ͺ�
	return data1;
    #endif
#endif
}

/*
 * ��������check_gr_mode
 * ����  ����鵱ǰ�����Ƿ�֧��GR
 * ����  ����
 * ���  ����
 * ����  ��0 ��֧��GR ��0 ֧��GR
 */
int32_t check_gr_mode(void)
{
	return ( MODEL_EN & __GR);
}
/*
 * ��������get_first_mode
 * ����  ���ҵ���ǰ�����µ��׸�ģʽ ����˳��ACW DCW IR GR BBD
 * ����  ����
 * ���  ����
 * ����  ���׸�ģʽ
 */
uint8_t get_first_mode(void)
{
	uint8_t mode = 0;
	
	if(MODEL_EN & __ACW)
	{
		mode = ACW;
	}
	else if(MODEL_EN & __DCW)
	{
		mode = DCW;
	}
	else if(MODEL_EN & __IR)
	{
		mode = IR;
	}
	else if( MODEL_EN & __GR)
	{
		mode = GR;
	}
	else if(MODEL_EN & __BBD)
	{
		mode = BBD;
	}
	else if(MODEL_EN & __CC)
	{
		mode = CC;
	}
	else
	{
		while(1); /* ���� */
	}
	return mode;
}
/*
 * ��������judge_support_g_mode
 * ����  ���ж��Ƿ�֧��Gģʽ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void judge_support_g_mode(void)
{
    type_spe.support_g_mode = 0;
    
	if(MODEL_EN & __ACW)
	{
        type_spe.support_g_mode = 1;
	}
	else if(MODEL_EN & __DCW)
	{
        type_spe.support_g_mode = 1;
	}
}
/*
 * ��������init_default_type
 * ����  ����ʼ��Ĭ�ϻ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void init_default_type(void)
{
	g_cur_type = &type_pool[CS9919AX];
	init_speciality();
    
    if(g_cur_type->config_fun)
    {
        g_cur_type->config_fun();
    }
}
/*
 * ��������init_speciality
 * ����  ����ʼ�����͵����Բ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void init_speciality(void)
{
	switch(type_spe.acw_vol_range)
	{
        case ACW_500V:
			type_spe.acw_vol_h = 500;	/* 0.500kV */
            break;
        case ACW_1kV:
			type_spe.acw_vol_h = 1000;	/* 1.000kV */
            break;
		case ACW_5kV:
			type_spe.acw_vol_h = 5000;	/* 5.000kV */
			break;
		case ACW_10kV:
            type_spe.transformer_type = TRANS_10kV;/* ��ѹ������ */
			type_spe.acw_vol_h = 9999;	/* 9.999kV */
			type_spe.acw_vol_l = 500;	/* 500V */
			break;
		case ACW_20kV:
            type_spe.transformer_type = TRANS_20kV;/* ��ѹ������ */
			type_spe.acw_vol_h = 2000;	/* 20.00kV */
			break;
		case ACW_30kV:
            type_spe.transformer_type = TRANS_30kV;/* ��ѹ������ */
			type_spe.acw_vol_h = 3000;	/* 20.00kV */
			break;
	}
	
	switch(type_spe.dcw_vol_range)
	{
        case DCW_500V:
			type_spe.dcw_vol_h = 500;	/* 0.500kV */
            break;
		case DCW_2kV:
			type_spe.dcw_vol_h = 2000;	/* 2.000kV */
			break;
		case DCW_6kV:
			type_spe.dcw_vol_h = 6000;	/* 6.000kV */
			break;
		case DCW_10kV:
            type_spe.transformer_type = TRANS_10kV;/* ��ѹ������ */
			type_spe.dcw_vol_h = 9999;	/* 9.999kV */
			type_spe.dcw_vol_l = 500;	/* 500V */
			break;
		case DCW_20kV:
            type_spe.transformer_type = TRANS_20kV;/* ��ѹ������ */
			type_spe.dcw_vol_h = 2000;	/* 20.00kV */
			break;
	}
	
	switch(type_spe.ir_vol_range)
	{
		case IR_1kV:
			type_spe.ir_vol_h = 1000;	/* 1.000kV */
			break;
		case IR_1_5kV:
			type_spe.ir_vol_h = 1500;	/* 1.500kV */
			break;
		case IR_2_5kV:
			type_spe.ir_vol_h = 2500;	/* 2.500kV */
			break;
		case IR_5kV:
			type_spe.ir_vol_h = 5000;	/* 5.000kV */
			break;
		case IR_10kV:
			type_spe.ir_vol_h = 9999;	/* 10.00kV */
			break;
	}
	
	if(type_spe.mode_en & __ACW)
	{
		type_spe.acw_arc_h = get_arc_h(ACW);
    }
    
    if(type_spe.mode_en & __DCW)
	{
		type_spe.dcw_arc_h = get_arc_h(DCW);
	}
	
	if(type_spe.hz_type == HZ_TYPE_CONT)
	{
		type_spe.freq_h = 4000;/* ����ģʽ��Ƶ��������400.0hz */
		type_spe.freq_l = 400;/* ����ģʽ��Ƶ��������040.0hz */
	}
	else
	{
		type_spe.freq_h = 9;
		type_spe.freq_l = 1;
	}
}

/*
 * ��������get_arc_h
 * ����  ����ȡ�绡�������ֵ
 * ����  ����
 * ���  ����
 * ����  ����
 */
uint16_t get_arc_h(uint8_t mode)
{
	uint16_t arc_upper = 0;
    
	switch(mode)
	{
		case ACW:
			arc_upper = 2000;
			menu_par[GUI_ACW_ARC_SUR].decs = 2;
			break;
		case DCW:
			arc_upper = 2000;
			menu_par[GUI_DCW_ARC_SUR].decs = 2;
			break;
	}
    
    return arc_upper;
}

void judge_single_gr(void)
{
	if(type_spe.mode_en == __GR)
    {
        type_spe.single_gr = 1;
    }
}
void judge_amp_type(void)
{
    if(g_custom_sys_par.amp_type == PWM_AMP)
    {
        type_spe.amp_type = PWM_AMP;
    }
    else
    {
        type_spe.amp_type = LINE_AMP;
    }
}

void judge_u_leading_data_sw(void)
{
    if(g_custom_sys_par.leading_sw == SYS_SW_ON)
    {
        type_spe.interface_config = type_spe.interface_config | INTERFACE_USB_OUT | INTERFACE_USB_IN/**/;
    }
    else
    {
        type_spe.interface_config = type_spe.interface_config /*| INTERFACE_USB_OUT | INTERFACE_USB_IN*/;
    }
}
/*
 * ��������init_other_speciality
 * ����  ����ʼ����������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void init_other_speciality(void)
{
	if(type_spe.mode_en & __ACW)
	{
        if(1 == is_acw_segment())
        {
            type_spe.vol_is_segment = VOL_SEGMENT_NO;
        }
        else
        {
            type_spe.vol_is_segment = VOL_SEGMENT_YES;
        }
    }
    else if(type_spe.mode_en & __DCW)
	{
        if(1 == is_dcw_segment())
        {
            type_spe.vol_is_segment = VOL_SEGMENT_NO;
        }
        else
        {
            type_spe.vol_is_segment = VOL_SEGMENT_YES;
        }
    }
    else if(type_spe.mode_en & __IR)
	{
        if(1 == is_ir_segment())
        {
            type_spe.vol_is_segment = VOL_SEGMENT_NO;
        }
        else
        {
            type_spe.vol_is_segment = VOL_SEGMENT_YES;
        }
    }
    else
    {
        type_spe.vol_is_segment = VOL_SEGMENT_NO;
    }
    
    judge_amp_type();
    judge_u_leading_data_sw();
    
    judge_support_g_mode();/* Gģʽ֧���ж� */
    judge_single_gr();/* ��gr�ж� */
}


char* get_custom_type_name(void)
{
    return (char*)((CUSTOM_TYPE_INFO*)TYPE_NUM_ADDR)->name;
}

void init_custom_type(void)
{
	static TYPE_STR type;
	
    if(is_custom_type() == 1)
    {
        type = (((CUSTOM_TYPE_INFO*)TYPE_NUM_ADDR)->type);
		type.ver = (char*)get_version_number();
		
		g_cur_type = &type;
    }
}
/*
 * ��������check_type
 * ����  ������������ͺ�
 * ����  ����
 * ���  ����
 * ����  ��-1 δ�ҵ�ƥ��Ļ��� 0 ���ͳɹ�ƥ��
 */
int32_t check_type(void)
{
#if CUSTOM_TYPE_EN==0
	uint16_t type_num = 0;
	uint16_t size = sizeof(type_pool)/sizeof(type_pool[0]);
	int32_t i = 0;
#endif
	
    g_cur_type = NULL;
    
#if CUSTOM_TYPE_EN
    g_cur_type = &custom_type;
#else
    type_num = read_type_pin();
    
    if(size >= 2 && type_num >= CS99XX_T_END)
    {
        type_num = CS9919AX;
    }
    
    for(i = 0; i < size; i++)
	{
		if(type_pool[i].num == type_num)
		{
			g_cur_type = &type_pool[i];
            break;
        }
    }
#endif
    
	config_all_init();
    
    if(g_cur_type == NULL)
    {
        return -1;
    }
    
    if(g_cur_type->config_fun)
    {
        g_cur_type->config_fun();
    }
    else
    {
        config_default();
    }
    
    init_other_speciality();//����config��������ʼ��
    init_custom_type();//�����ı����ƵĶ��ƻ���ʼ��
    
    return 0;
}


/*
 * ��������draw_page_type
 * ����  ������һҳ�Ļ��ͱ�
 * ����  ��s 
 * ���  ����
 * ����  ��-1 δ�ҵ�ƥ��Ļ��� 0 ���ͳɹ�ƥ��
 */
void draw_page_type(const int8_t s, const int8_t num)
{
	int8_t i = 0;
	int8_t j = 0;
	int8_t n = 0;
    int8_t n_o = 0;
    
	GUI_ClearRect (0, 0, 200 - 1, 64 - 1);
    
	for(i = s; i < s + num; i++)
	{
        if(0 == strncmp(type_pool[i].name, "CS99", strlen("CS99")))
        {
            n_o = 4;
        }
        else if(0 == strncmp(type_pool[i].name, "CS", strlen("CS")))
        {
            n_o = 2;
        }
        else if(0 == strncmp(type_pool[i].name, "99", strlen("99")))
        {
            n_o = 2;
        }
        else
        {
            n_o = 0;
        }
        
		j = (i - s) / 5;
		GUI_DispStringAt(type_pool[i].name + n_o, 12 + 66 * j, 1 + 13 * n);
        
		if(++n > 4)
		{
			n = 0;
		}
	}
    
	ReFresh_LCD24064();
}

int32_t judge_valid_type(const uint16_t type_num, const int8_t n)
{
	int32_t res = 0;
	
	if(type_pool[n].num == type_num)
	{
		res = 0;
	}
	else
	{
		res = -1;
	}
	
	return res;
}

void prompt_recover_type(const int8_t s)
{
	uint8_t X0	= 40;
	uint8_t Y0	= 8;
	uint8_t X1	= 165;
	uint8_t Y1	= 58;
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	
	if(s == -1)
	{
		GUI_DispStringAt("ϵͳ�ͺ�У���", 0+X0+35, Y0+4);
		GUI_DispStringAt("������Ӳ��", 0+X0+35, Y0+4+14);
		GUI_DispStringAt("���ú����ԡ�", 0+X0+35, Y0+4+14+14);
	}
	else
	{
		GUI_DispStringAt("����ɾ���У���", 0+X0+35, Y0+4);
		GUI_DispStringAt("����Ч����Ҫ��", 0+X0+35, Y0+4+14);
		GUI_DispStringAt("�������Ժ�...", 0+X0+35, Y0+4+14+14);
	}
    
	GUI_DrawRect(X0, Y0, X1, Y1);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmwarning, X0+3, Y0+5);
	LCD_REFRESH();
	
	off_keyboard();
	OSTimeDlyHMSM(0,0,2,0);	/* ��ʾ����ʾ2s */
	on_keyboard();
	GUI_ClearRect (X0, Y0, X1, Y1);
}

void select_type(void)
{
	#define ONE_PAGE_NUM	15
    
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t n = 1;
	int8_t m = 0;
	int8_t o = 0;
	uint16_t type_num = read_type_pin();
	int32_t size = sizeof(type_pool)/sizeof(type_pool[0]) - 1;//ȥ�����һ����Ԫ��
	int8_t pages = size/ONE_PAGE_NUM+(size%ONE_PAGE_NUM==0?0:1);
	int8_t cur_page_num = 0;
	int32_t last_page_num = (size%ONE_PAGE_NUM == 0? ONE_PAGE_NUM:size%ONE_PAGE_NUM);
	int8_t page = 1;
    
	gui_draw_menu();
	
	if(page < pages)
	{
		cur_page_num = ONE_PAGE_NUM;
	}
	else
	{
		cur_page_num = last_page_num;
	}
	
	draw_page_type(ONE_PAGE_NUM*(page-1), cur_page_num);
	
	while(1)
	{
		m = n%5;
		if(m > 4)
		{
			m = 0;
		}
		else if(m == 0)
		{
			m = 5;
		}
		
		if(n < 6)
		{
			o = 0;
		}
		else if(n < 11)
		{
			o = 1;
		}
		else
		{
			o = 2;
		}
		
		GUI_ClearRect (0+66*0, 0, 10+66*0, 64-1);
		GUI_ClearRect (0+66*1, 0, 10+66*1, 64-1);
		GUI_ClearRect (0+66*2, 0, 10+66*2, 64-1);
		GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow_right, 1+66*o, 4+13*(m-1));
		ReFresh_LCD24064();
		
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		
		switch(*pkey_value)
		{
			case KEY_UP:
            {
				if(--n < 1)
				{
					n = 15;
					if(--page < 1)
					{
						page = pages;
						n = last_page_num;
						cur_page_num = last_page_num;
					}
					else
					{
						cur_page_num = ONE_PAGE_NUM;
					}
					draw_page_type(ONE_PAGE_NUM*(page-1), cur_page_num);
				}
				break;
            }
			case KEY_DOWN:
            {
				if(page < pages)
				{
					cur_page_num = ONE_PAGE_NUM;
				}
				else
				{
					cur_page_num = last_page_num;
				}
                
				if(++n > cur_page_num)
				{
					n = 1;
					if(++page > pages)
					{
						page = 1;
					}
					if(page < pages)
					{
						cur_page_num = ONE_PAGE_NUM;
					}
					else
					{
						cur_page_num = last_page_num;
					}
					draw_page_type(ONE_PAGE_NUM*(page-1), cur_page_num);
				}
				break;
            }
			case KEY_SHIFT:
            {
				if(page < pages)
				{
					cur_page_num = ONE_PAGE_NUM;
				}
				else
				{
					cur_page_num = last_page_num;
				}
				
				if(n+5 > cur_page_num)
				{
					if(n-10 > 0)
					{
						n -= 10;
					}
					else if(n-5 > 0)
					{
						n -= 5;
					}
					break;
				}
				else
				{
					n += 5;
				}
				break;
            }
			case KEY_ENTER:
            {
                erase_pages_flash(TYPE_NUM_ADDR, 1);
                type_num = type_pool[ONE_PAGE_NUM*(page-1)+n-1].num;
                write_u16_flash(TYPE_NUM_ADDR, &type_num, 1);
				NVIC_SystemReset();
				break;
            }
            case KEY_0:
            {
                n = 1;
                if(++page > pages)
                {
                    page = 1;
                }
                if(page < pages)
                {
                    cur_page_num = ONE_PAGE_NUM;
                }
                else
                {
                    cur_page_num = last_page_num;
                }
                draw_page_type(ONE_PAGE_NUM*(page-1), cur_page_num);
                break;
            }
            case KEY_1:
            {
                if(--page < 1)
                {
                    page = pages;
                    n = last_page_num;
                    cur_page_num = last_page_num;
                }
                else
                {
                    cur_page_num = ONE_PAGE_NUM;
                }
                n = cur_page_num;
                draw_page_type(ONE_PAGE_NUM*(page-1), cur_page_num);
                break;
            }
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
		}
	}
}

uint32_t define_vol_kinds(uint8_t mode, const uint8_t **gear, uint8_t *flag)
{
	uint32_t kinds = 0;
	
	switch(mode)
	{
		case GR://GR_DC_20mV | GR_DC_200mV | GR_DC_2000mV;
		{
			if(type_spe.gr_vol_gear & GR_DC_20mV)
			{
				flag[kinds] = 1;
				gear[kinds++] = "20mV";
			}
            
			if(type_spe.gr_vol_gear & GR_DC_200mV)
			{
				flag[kinds] = 2;
				gear[kinds++] = "200mV";
			}
            
			if(type_spe.gr_vol_gear & GR_DC_2000mV)
			{
				flag[kinds] = 3;
				gear[kinds++] = "2000mV";
			}
			
			if(type_spe.gr_vol_gear & GR_DC_12V)
			{
				flag[kinds] = 4;
				gear[kinds++] = "12V";
			}
			
			if(type_spe.gr_vol_gear & GR_DC_12V_100mA)
			{
				flag[kinds] = 5;
				gear[kinds++] = "12V_100mA";
			}
			
			if(type_spe.gr_vol_gear & GR_DC_12V_1000mA)
			{
				flag[kinds] = 6;
				gear[kinds++] = "12V_1000mA";
			}
			
			break;
		}
	}
	return kinds;
}

uint32_t define_fail_mode_kinds(const uint8_t *fail_mode_buf[], uint8_t *flag)
{
	uint32_t kinds = 0;
    uint8_t temp = type_spe.def_fail_mode;
    
    if(temp & __STOP)
    {
        flag[kinds] = FAIL_MODE_STOP;
        fail_mode_buf[kinds++] = fail_mode_pool[FAIL_MODE_STOP];
    }
    if(temp & __HALT)
    {
        flag[kinds] = FAIL_MODE_HALT;
        fail_mode_buf[kinds++] = fail_mode_pool[FAIL_MODE_HALT];
    }
    if(temp & __CON)
    {
        flag[kinds] = FAIL_MODE_CON;
        fail_mode_buf[kinds++] = fail_mode_pool[FAIL_MODE_CON];
    }
    if(temp & __RESTART)
    {
        flag[kinds] = FAIL_MODE_RESTART;
        fail_mode_buf[kinds++] = fail_mode_pool[FAIL_MODE_RESTART];
    }
    if(temp & __NEXT)
    {
        flag[kinds] = FAIL_MODE_NEXT;
        fail_mode_buf[kinds++] = fail_mode_pool[FAIL_MODE_NEXT];
    }
    if(temp & __FPDFC)
    {
        flag[kinds] = FAIL_MODE_FPDFC;
        fail_mode_buf[kinds++] = fail_mode_pool[FAIL_MODE_FPDFC];
    }
    
    return kinds;
}

uint32_t define_cur_kinds(uint8_t mode, const uint8_t *gear[], uint8_t *flag)
{
	uint32_t kinds = 0;
    uint8_t temp_gear = 0;
	
    if(mode == ACW)
    {
        temp_gear = type_spe.acw_gear;
    }
    else if(mode == CC)
    {
        temp_gear = type_spe.cc_gear;
    }
    
	switch(mode)
	{
		case ACW:
        case CC:
		{
			if(temp_gear & _AC_200uA)
			{
				flag[kinds] = AC_200uA;
				gear[kinds++] = ac_gear[AC_200uA].name;
			}
			if(temp_gear & _AC_2mA)
			{
				flag[kinds] = AC_2mA;
				gear[kinds++] = ac_gear[AC_2mA].name;
			}
			if(temp_gear & _AC_10mA)
			{
				flag[kinds] = AC_10mA;
				gear[kinds++] = ac_gear[AC_10mA].name;
			}
			if(temp_gear & _AC_20mA)
			{
				flag[kinds] = AC_20mA;
				gear[kinds++] = ac_gear[AC_20mA].name;
			}
			if(temp_gear & _AC_50mA)
			{
				flag[kinds] = AC_50mA;
				gear[kinds++] = ac_gear[AC_50mA].name;
			}
			if(temp_gear & _AC_100mA)
			{
				flag[kinds] = AC_100mA;
				gear[kinds++] = ac_gear[AC_100mA].name;
			}
			if(temp_gear & _AC_200mA)
			{
				flag[kinds] = AC_200mA;
				gear[kinds++] = ac_gear[AC_200mA].name;
			}
			break;
		}
		case DCW:
		{
			if(type_spe.dcw_gear & _DC_2uA)
			{
				flag[kinds] = DC_2uA;
				gear[kinds++] = dc_gear[DC_2uA].name;
			}
			if(type_spe.dcw_gear & _DC_20uA)
			{
				flag[kinds] = DC_20uA;
				gear[kinds++] = dc_gear[DC_20uA].name;
			}
			if(type_spe.dcw_gear & _DC_200uA)
			{
				flag[kinds] = DC_200uA;
				gear[kinds++] = dc_gear[DC_200uA].name;
			}
			if(type_spe.dcw_gear & _DC_2mA)
			{
				flag[kinds] = DC_2mA;
				gear[kinds++] = dc_gear[DC_2mA].name;
			}
			if(type_spe.dcw_gear & _DC_10mA)
			{
				flag[kinds] = DC_10mA;
				gear[kinds++] = dc_gear[DC_10mA].name;
			}
			if(type_spe.dcw_gear & _DC_20mA)
			{
				flag[kinds] = DC_20mA;
				gear[kinds++] = dc_gear[DC_20mA].name;
			}
			if(type_spe.dcw_gear & _DC_50mA)
			{
				flag[kinds] = DC_50mA;
				gear[kinds++] = dc_gear[DC_50mA].name;
			}
			if(type_spe.dcw_gear & _DC_100mA)
			{
				flag[kinds] = DC_100mA;
				gear[kinds++] = dc_gear[DC_100mA].name;
			}
			break;
		}
	}
	return kinds;
}

uint8_t get_max_cur_gear(uint8_t mode)
{
	switch(mode)
	{
		case ACW:
			if(type_spe.acw_gear & _AC_200mA)
			{
				return AC_200mA;
			}
			else if(type_spe.acw_gear & _AC_100mA)
			{
				return AC_100mA;
			}
			else if(type_spe.acw_gear & _AC_50mA)
			{
				return AC_50mA;
			}
			else if(type_spe.acw_gear & _AC_20mA)
			{
				return AC_20mA;
			}
			else if(type_spe.acw_gear & _AC_10mA)
			{
				return AC_10mA;
			}
			else if(type_spe.acw_gear & _AC_2mA)
			{
				return AC_2mA;
			}
			else if(type_spe.acw_gear & _AC_200uA)
			{
				return AC_200uA;
			}
			else
			{
				return 0;
			}
		case DCW:
			if(type_spe.dcw_gear & _DC_100mA)
			{
				return DC_100mA;
			}
			else if(type_spe.dcw_gear & _DC_50mA)
			{
				return DC_50mA;
			}
			else if(type_spe.dcw_gear & _DC_20mA)
			{
				return DC_20mA;
			}
			else if(type_spe.dcw_gear & _DC_10mA)
			{
				return DC_10mA;
			}
			else if(type_spe.dcw_gear & _DC_2mA)
			{
				return DC_2mA;
			}
			else if(type_spe.dcw_gear & _DC_200uA)
			{
				return DC_200uA;
			}
			else if(type_spe.dcw_gear & _DC_20uA)
			{
				return DC_20uA;
			}
			else if(type_spe.dcw_gear & _DC_2uA)
			{
				return DC_2uA;
			}
			else
			{
				return 0;
			}
	}
	return 0;
}
uint32_t define_hz_kinds(uint8_t mode, const uint8_t** gear_buf, uint8_t *flag)
{
	uint8_t kinds = 0;
	uint32_t hz_en = 0;
	switch(mode)
	{
		case ACW:
			hz_en = type_spe.acw_hz;
			break;
		case GR:
			hz_en = type_spe.gr_hz;
			break;
		default:
			return 0;
	}
	
	if(hz_en & _AC_50HZ)
	{
		gear_buf[kinds] = "1 --- 50Hz";
		flag[kinds++] = 1;
	}
	if(hz_en & _AC_60HZ)
	{
		gear_buf[kinds] = "2 --- 60Hz";
		flag[kinds++] = 2;
	}
	if(hz_en & _AC_100HZ)
	{
		gear_buf[kinds] = "3 -- 100Hz";
		flag[kinds++] = 3;
	}
	if(hz_en & _AC_150HZ)
	{
		gear_buf[kinds] = "4 -- 150Hz";
		flag[kinds++] = 4;
	}
	if(hz_en & _AC_200HZ)
	{
		gear_buf[kinds] = "5 -- 200Hz";
		flag[kinds++] = 5;
	}
	if(hz_en & _AC_250HZ)
	{
		gear_buf[kinds] = "6 -- 250Hz";
		flag[kinds++] = 6;
	}
	if(hz_en & _AC_300HZ)
	{
		gear_buf[kinds] = "7 -- 300Hz";
		flag[kinds++] = 7;
	}
	if(hz_en & _AC_350HZ)
	{
		gear_buf[kinds] = "8 -- 350Hz";
		flag[kinds++] = 8;
	}
	if(hz_en & _AC_400HZ)
	{
		gear_buf[kinds] = "9 -- 400Hz";
		flag[kinds++] = 9;
	}
	return kinds;
}

int32_t check_mode(void)
{
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};
	uint8_t flag[MODEL_KINDS_MAX] = {0};
	int16_t kinds = 0;
	int16_t i = 0;
    
    if(NULL == g_cur_step)
    {
        return -1;
    }
	
	define_modes(mode_buf, flag, &kinds);/* ��ȡ��ǰ���͵�ģʽ��Ϣ */
	
    for(i = 1; i <= kinds; i++)
    {
        if(g_cur_step->one_step.com.mode == flag[i])
        {
            return 0;
        }
    }
    
    return -1;
}


int32_t check_this_mode(uint8_t mode)
{
    if(mode < ACW || mode >= MODE_END)
    {
        return -1;
    }
    
    return 0;
}

int32_t check_test_mode(NODE_STEP * p)
{
	const uint8_t *mode_buf[MODEL_KINDS_MAX] = {0};
	uint8_t flag[MODEL_KINDS_MAX] = {0};
	int16_t kinds = 0;
	int16_t i = 0;
    
    if(NULL == p)
    {
        return -1;
    }
	
	define_modes(mode_buf, flag, &kinds);/* ��ȡ��ǰ���͵�ģʽ��Ϣ */
	
    for(i = 1; i <= kinds; i++)
    {
        if(p->one_step.com.mode == flag[i])
        {
            return 0;
        }
    }
    
    return -1;
}
uint32_t define_interface_config(uint8_t *flag)
{
    uint32_t kinds = 0;
    
//     if(type_spe.interface_config & INTERFACE_PLC)
//     {
// 		flag[kinds++] = 0;
//     }
    if(type_spe.interface_config & INTERFACE_COMM)
    {
		flag[kinds++] = 1;
    }
    if(type_spe.interface_config & INTERFACE_USB_OUT)
    {
		flag[kinds++] = 2;
    }
    if(type_spe.interface_config & INTERFACE_USB_IN)
    {
		flag[kinds++] = 3;
    }
    
    return kinds;
}
uint8_t get_acw_max_gear(void)
{
    if(type_spe.acw_gear & _AC_200mA)
    {
        return AC_200mA;
    }
    if(type_spe.acw_gear & _AC_100mA)
    {
        return AC_100mA;
    }
    if(type_spe.acw_gear & _AC_50mA)
    {
        return AC_50mA;
    }
    if(type_spe.acw_gear & _AC_20mA)
    {
        return AC_20mA;
    }
    if(type_spe.acw_gear & _AC_10mA)
    {
        return AC_10mA;
    }
    if(type_spe.acw_gear & _AC_2mA)
    {
        return AC_2mA;
    }
    if(type_spe.acw_gear & _AC_200uA)
    {
        return AC_200uA;
    }
    
    return AC_200mA;
}
uint8_t get_cc_max_gear(void)
{
    if(type_spe.cc_gear & _AC_200mA)
    {
        return AC_200mA;
    }
    if(type_spe.cc_gear & _AC_100mA)
    {
        return AC_100mA;
    }
    if(type_spe.cc_gear & _AC_50mA)
    {
        return AC_50mA;
    }
    if(type_spe.cc_gear & _AC_20mA)
    {
        return AC_20mA;
    }
    if(type_spe.cc_gear & _AC_10mA)
    {
        return AC_10mA;
    }
    if(type_spe.cc_gear & _AC_2mA)
    {
        return AC_2mA;
    }
    if(type_spe.cc_gear & _AC_200uA)
    {
        return AC_200uA;
    }
    
    return AC_200mA;
}

uint8_t get_dcw_max_gear(void)
{
    if(type_spe.dcw_gear & _DC_100mA)
    {
        return DC_100mA;
    }
    if(type_spe.dcw_gear & _DC_50mA)
    {
        return DC_50mA;
    }
    if(type_spe.dcw_gear & _DC_20mA)
    {
        return DC_20mA;
    }
    if(type_spe.dcw_gear & _DC_10mA)
    {
        return DC_10mA;
    }
    if(type_spe.dcw_gear & _DC_2mA)
    {
        return DC_2mA;
    }
    if(type_spe.dcw_gear & _DC_200uA)
    {
        return DC_200uA;
    }
    if(type_spe.dcw_gear & _DC_20uA)
    {
        return DC_20uA;
    }
    if(type_spe.dcw_gear & _DC_2uA)
    {
        return DC_2uA;
    }
    
    return DC_100mA;
}

void save_custom_type_info(uint8_t *buf, uint8_t size)
{
    erase_pages_flash(TYPE_NUM_ADDR, 1);
    write_u16_flash(TYPE_NUM_ADDR, (void*)buf, size);
}

static void check_custom_sys_par(CUSTOM_SYS_PAR *info)
{
	if(info->amp_select != AMP_OLD && info->amp_select != AMP_8833)    /* �������� �Ϲ��� �� 8833���� */
	{
		info->amp_select = AMP_8833;
	}
	if(info->par_medium != PAR_MEDIUM_EEP && info->par_medium != PAR_MEDIUM_FLASH)
	{
		info->par_medium = PAR_MEDIUM_EEP;/* �����洢���� eep �� flash */
	}
	if(info->res_medium != PAR_MEDIUM_EEP && info->res_medium != PAR_MEDIUM_FLASH)
	{
		info->res_medium = PAR_MEDIUM_EEP;/* ����洢���� eep �� flash */
	}
	if(info->cal_medium != PAR_MEDIUM_EEP && info->cal_medium != PAR_MEDIUM_FLASH)
	{
		info->cal_medium = PAR_MEDIUM_EEP;/* У׼�洢���� eep �� flash */
	}
	if(info->buzzer_en != 1 && info->buzzer_en != 0)
	{
		info->buzzer_en = 1;/* ������Ӳ���� */
	}
	if(info->mute_sw != 1 && info->mute_sw != 0)
	{
		info->mute_sw = 0;/* ϵͳ����ģʽ���� ����ʱ���Ա�������һֱ�� */
	}
	if(info->amp_type != 1 && info->amp_type != 0)
	{
		info->amp_type = 0;/* �������� LINE PWM */
	}
	if(info->leading_sw != 1 && info->leading_sw != 0)
	{
		info->leading_sw = 0;/* ���ݵ��뵼����U�̿��� */
	}
	if(info->ir_gear_hold != 1 && info->ir_gear_hold != 0)
	{
		info->ir_gear_hold = 0;/* IR��������ʱ�俪�� */
	}
	if(info->ir_speed_sw != 1 && info->ir_speed_sw != 0)
	{
		info->ir_speed_sw = 0;
	}
	if(info->offset_set_en != 1 && info->offset_set_en != 0)
	{
		info->offset_set_en = 0;//�ֶ�ƫ�ƿ���
	}
}
void read_custom_par(void)
{
    CUSTOM_TYPE_INFO custom_type_info;
    uint8_t *p_addr = (void*)TYPE_NUM_ADDR;
    uint8_t count = sizeof(CUSTOM_TYPE_INFO);
    uint8_t *p_str = (void*)&custom_type_info;
    int32_t i = 0;
    
    /* �����������ݵ��ڴ� */
    for(i = 0; i < count; i++)
    {
        p_str[i] = p_addr[i];
    }
    
    g_custom_sys_par = custom_type_info.custom_sys_par;
	check_custom_sys_par(&g_custom_sys_par);
}

void save_custom_par(void)
{
    CUSTOM_TYPE_INFO custom_type_info;
    uint8_t *p_addr = (void*)TYPE_NUM_ADDR;
    uint8_t count = sizeof(CUSTOM_TYPE_INFO);
    uint8_t *p_str = (void*)&custom_type_info;
    int32_t i = 0;
    
    /* �����������ݵ��ڴ� */
    for(i = 0; i < count; i++)
    {
        p_str[i] = p_addr[i];
    }
    
    custom_type_info.custom_sys_par = g_custom_sys_par;
    
    save_custom_type_info((uint8_t *)&custom_type_info, count);
}

uint8_t is_custom_type(void)
{
    return ((CUSTOM_TYPE_INFO*)TYPE_NUM_ADDR)->custom_en;
}
uint16_t get_cur_type_num(void)
{
    return ((CUSTOM_TYPE_INFO*)TYPE_NUM_ADDR)->type_num;
}

void set_custom_type_info(void)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
    CUSTOM_TYPE_INFO custom_type_info;
    uint8_t *p_addr = (void*)TYPE_NUM_ADDR;
    uint8_t count = sizeof(CUSTOM_TYPE_INFO);
    uint8_t *p_str = (void*)&custom_type_info;
    int32_t i = 0;
    uint8_t x = 70;
    uint8_t y = 30;
    uint8_t file_name[20] = {0};
    uint16_t type_num = 0;
    TYPE_STR *p_type;
    
	DATA_INFO file_name_inf = {
		(void*)0/*pData*/,70/*x*/,32/*y*/,2/*type*/,15/*lon*/,0/*decs*/,0/*total*/,0/*cur*/,0/*ch_bit*/,0/*page*/,3/*bits*/,
	};
    
    for(i = 0; i < count; i++)
    {
        p_str[i] = p_addr[i];
    }
    
    draw_custom_type_gui();
    type_num = get_cur_type_num();
    
#if CUSTOM_TYPE_EN
	if(CUSTOM_TYPE_EN)
	{
		p_type = (TYPE_STR *)&custom_type;
		GUI_DispStringAt(p_type->name, 70, 16);
	}
	else
#endif
	{
		if(type_num < CS99XX_T_END)
		{
			char buf[20] = {0};
			
			for(i = 0; i < CS99XX_T_END; i++)
			{
				if(type_pool[i].num == type_num)
				{
					p_type = (void*)&type_pool[i];
					strcat(buf, (const char*)(type_pool[i].name));
					GUI_DispStringAt(buf, 70, 16);
					break;
				}
			}
		}
	}
    
    if(is_custom_type() == 1)
    {
        GUI_DispStringAt((const char*)custom_type_info.name, 70, 32);
        strncpy((char*)file_name, (const char*)custom_type_info.name, 15);
    }
    
	pmenu_data = &file_name_inf;
	pmenu_data->pData = file_name;
	pmenu_data->ch_bit = 0;
	LCD_REFRESH();
	cursor_pos(x,y);
    
	while(1)
	{
		uint8_t temp = 0;
		uint8_t temp_lon = 0;
        
		if(file_name[0] == '\0')
		{
			pmenu_data->ch_bit = 0;//�˴��ǽ���Ǹ�����ʾ������
		}
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		serve_charkey(*pkey_value);
		temp = pmenu_data->ch_bit;
		temp_lon = strlen((const char*)file_name);
		
		switch(*pkey_value)
		{
			//������������������,���Ƚ�pmenu_data->ch_bit = 0;Ȼ����ȷ�������ƶ�����λ
			case KEY_SHIFT_L:
				pmenu_data->last_ch_bit = pmenu_data->ch_bit;//����last_ch_bit
				shift_cursor_l(pmenu_data);
				break;
			case KEY_SHIFT_R:
				shift_cursor(pmenu_data);
				break;
			case KEY_FILE_DEL:
				for(i = 0; i < temp_lon - temp;i++)
				{
					file_name[temp+i] = file_name[temp+i+1];
				}
				
				GUI_ClearRect (pmenu_data->x, pmenu_data->y, pmenu_data->x+14*6, pmenu_data->y+12);
				GUI_DispStringAt((const char*)file_name, pmenu_data->x, pmenu_data->y);
				LCD_REFRESH();
				
				if(pmenu_data->ch_bit >= temp_lon-1)	/* ���ɾ���������һλ�ͽ��������һ�� */
				{
					shift_cursor_l(pmenu_data);
				}
				else	/* ����ɾ����λ���겻�ƶ� */
				{
					cursor_pos(pmenu_data->x+6*pmenu_data->ch_bit, y);
				}
				break;
			case KEY_ENTER:
				if(file_name[0] == '\0')	/* �ж��ļ����Ƿ�Ϊ�� */
				{
                    custom_type_info.custom_en = 0;
                    save_custom_type_info((uint8_t *)&custom_type_info, count);
                    check_type();
					return;
				}
				
                memset(custom_type_info.name, 0, sizeof(custom_type_info.name));
				strncpy((char*)custom_type_info.name, (const char*)file_name,
                        sizeof(custom_type_info.name) - 1);
                
                custom_type_info.custom_en = 1;
                custom_type_info.type.name = (char*)(TYPE_NUM_ADDR + (uint32_t)&(((CUSTOM_TYPE_INFO *)0)->name));
                custom_type_info.type.ver = p_type->ver;
                custom_type_info.type.num = p_type->num;
                custom_type_info.type.config_fun = p_type->config_fun;
                
                save_custom_type_info((uint8_t *)&custom_type_info, count);
                check_type();
				return;
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return;
			default:
				break;
		}
	}
}

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
