/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_result.c
 * 摘  要  ：结果信息管理与显示
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#include	<string.h>
#include 	"stdlib.h"
#include	<stdio.h>
#include 	<math.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include	"RTC_Config.h"
#include    "cs99xx_result.h"
#include 	"mb_server.h"
#include 	"SysTemConfig.h"
#include    "mb_cmd_define.h"


uint8_t get_ir_res_uint(const RESULT_STR *res)
{
    if(res->un.ir.gear <= IR_3_GEAR_COMM)
    {
        return RES_U_MOHM;
    }
//     else if(res->un.ir.gear == IR_4_GEAR_COMM)
//     {
//         if(res->un.ir.res < 1000) 
//         {
//             return RES_U_MOHM;
//         }
//         else
//         {
//             return RES_U_GOHM;
//         }
//     }
    else
    {
        return RES_U_GOHM;
    }
}

void transfer_ir_res_loop_to_string(const RESULT_STR *res, char* buf)
{
    uint32_t t = 1;
    uint8_t decs = 0;
    
    switch(res->un.ir.gear)
    {
        case IR_1_GEAR_COMM:
            if(res->un.ir.res < 10 * 1000)
            {
                decs = 3;
                t = 1;
            }
            else
            {
                decs = 2;
                t = 10;
            }
            break;
        case IR_2_GEAR_COMM:
            if(res->un.ir.res < 10 * 1000)
            {
                decs = 2;
                t = 1;
            }
            else
            {
                decs = 1;
                t = 10;
            }
            break;
        case IR_3_GEAR_COMM:
            if(res->un.ir.res < 10 * 1000)
            {
                decs = 1;
                t = 1;
            }
            else
            {
                decs = 0;
                t = 10;
            }
            break;
        case IR_4_GEAR_COMM:
            if(res->un.ir.res < 10 * 1000)
            {
                decs = 3;
                t = 1;
            }
            else
            {
                decs = 2;
                t = 10;
            }
            break;
        case IR_5_GEAR_COMM:
            if(res->un.ir.res < 10 * 1000)
            {
                decs = 2;
                t = 1;
            }
            else
            {
                decs = 1;
                t = 10;
            }
            break;
//                     case IR_6_GEAR_COMM:
//                         mysprintf((uint8_t*)res_str->test_loop  , unit_pool[RES_U_GOHM], 51, res->un.ir.res);
//                         break;
    }
    
    mysprintf((uint8_t*)buf  , NULL, decs + 150, res->un.ir.res / t);
}

void transfer_ir_res_out_range_loop_to_string(const RESULT_STR *res, char* buf)
{
    uint32_t t = 1;
    uint32_t b =  1;
    uint32_t decs = 0;
    
    switch(res->un.ir.gear)
    {
        case IR_1_GEAR_COMM:
            decs = 1;
            b = 1;
            break;
        case IR_2_GEAR_COMM:
            decs = 0;
            b = 1;
            break;
        case IR_3_GEAR_COMM:
            decs = 0;
            b = 1;
            break;
        case IR_4_GEAR_COMM:
            if(res->un.ir.res < 1000) // >1000M
            {
                decs = 0;
                b = 1;
            }
            else if(res->un.ir.res < 10 * 1000)
            {
                decs = 2;
                b = 1000; // > 1.00G
            }
            else
            {
                decs = 1;
                b = 1000; // > 10.0G
            }
            break;
        case IR_5_GEAR_COMM:
            decs = 0;
            b = 1000; // > 100G
            break;
//         case IR_6_GEAR_COMM:
//             decs = 0;
//             b = 1000;
//             break;
    }
    
    t = ten_power(decs);
    
    mysprintf((uint8_t*)buf, NULL, decs + 140, res->un.ir.res / b * t);
}

void ch_out_result_to_string(const RESULT_STR *res, RESULT_STRING_STR* res_str)
{
	switch(res->mode)
	{
		case ACW:
        case CC:
		{
			switch(type_spe.acw_vol_range)
			{
                default:
                case ACW_500V:
				case ACW_1kV:
                case ACW_5kV:
                    mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 53, res->un.acw.vol);
					break;
				case ACW_10kV:
                    if(res->un.acw.vol < 10000)
                    {
                        mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 53, res->un.acw.vol);
                    }
                    else
                    {
                        mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 52, res->un.acw.vol/10);
                    }
					break;
				case ACW_20kV:
				case ACW_30kV:
                    mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 52, res->un.acw.vol/10);
                    break;
			}
			break;
		}
		case DCW:
		{
			switch(type_spe.dcw_vol_range)
			{
                default:
                case DCW_500V:
                case DCW_2kV:
				case DCW_6kV:
                    mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 53, res->un.dcw.vol);
                    break;
				case DCW_10kV:
                    if(res->un.dcw.vol < 10000)
                    {
                        mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 53, res->un.dcw.vol);
                    }
                    else
                    {
                        mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 52, res->un.dcw.vol/10);
                    }
					break;
				case DCW_20kV:
                    mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 52, res->un.dcw.vol/10);
					break;
			}
			break;
		}
		case IR:
		{
            mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 53, res->un.ir.vol);
            break;
		}
		case GR:
		{
            switch(type_spe.gr_cur_unit)
            {
                case CUR_U_A:
                    mysprintf((uint8_t*)res_str->test_out  , unit_pool[CUR_U_A], 52, res->un.gr.cur);
                    break;
                case CUR_U_mA:
                    mysprintf((uint8_t*)res_str->test_out  , unit_pool[CUR_U_mA], 51, res->un.gr.cur);
                    break;
            }
			break;
		}
		case BBD:
		{
            mysprintf((uint8_t*)res_str->test_out  , unit_pool[VOL_U_kV], 53, res->un.bbd.vol);
            break;
		}
    }
}

void add_other_option(const RESULT_STR *res, char* buf)
{
	switch(res->mode)
	{
        case IR:
		{
			/* 吸收比不为0时添加 */
			if(res->un.ir.absorptance > 0)
			{
				mysprintf((uint8_t*)buf  , NULL, 141, res->un.ir.absorptance * 10);
			}
			break;
		}
	}
}

void ch_out_result_to_string_comm(const RESULT_STR *res, char* buf)
{
	switch(res->mode)
	{
        default:
        case ACW:
		{
			switch(type_spe.acw_vol_range)
			{
                default:
                case ACW_500V:
				case ACW_1kV:
                case ACW_5kV:
                    mysprintf((uint8_t*)buf  , NULL, 153, res->un.acw.vol);
					break;
				case ACW_10kV:
                    if(res->un.acw.vol < 10000)
                    {
                        mysprintf((uint8_t*)buf  , NULL, 153, res->un.acw.vol);
                    }
                    else
                    {
                        mysprintf((uint8_t*)buf  , NULL, 152, res->un.acw.vol/10);
                    }
					break;
				case ACW_20kV:
				case ACW_30kV:
                    mysprintf((uint8_t*)buf  , NULL, 152, res->un.acw.vol/10);
                    break;
			}
			break;
		}
		case DCW:
		{
			switch(type_spe.dcw_vol_range)
			{
                default:
                case DCW_500V:
                case DCW_2kV:
				case DCW_6kV:
                    mysprintf((uint8_t*)buf  , NULL, 153, res->un.dcw.vol);
                    break;
				case DCW_10kV:
                    if(res->un.acw.vol < 10000)
                    {
                        mysprintf((uint8_t*)buf  , NULL, 153, res->un.dcw.vol);
                    }
                    else
                    {
                        mysprintf((uint8_t*)buf  , NULL, 152, res->un.dcw.vol/10);
                    }
					break;
				case DCW_20kV:
                    mysprintf((uint8_t*)buf  , NULL, 152, res->un.dcw.vol/10);
					break;
			}
			break;
		}
		case IR:
		{
            mysprintf((uint8_t*)buf  , NULL, 153, res->un.ir.vol);
            break;
		}
		case GR:
		{
            switch(type_spe.gr_cur_unit)
            {
                case CUR_U_A:
                    mysprintf((uint8_t*)buf  , NULL, 152, res->un.gr.cur);
                    break;
                case CUR_U_mA:
                    mysprintf((uint8_t*)buf  , NULL, 151, res->un.gr.cur);
                    break;
            }
			break;
		}
		case BBD:
		{
            mysprintf((uint8_t*)buf  , NULL, 153, res->un.bbd.vol);
            break;
		}
    }
}


void ch_loop_result_to_string(const RESULT_STR *res, RESULT_STRING_STR* res_str)
{
    uint8_t buf[20] = {0};
    uint8_t c = 0;
    uint8_t unit = RES_U_MOHM;
    
    sprintf((char*)res_str->real_cur   , "--");
    
    switch(res->mode)
    {
        default:
        case ACW:
        {
            switch(res->un.acw.real_gear)
            {
                case AC_200uA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool_[CUR_U_uA], 51, res->un.acw.cur);
                    mysprintf((uint8_t*)res_str->real_cur  , unit_pool_[CUR_U_uA], 51, res->un.acw.real_cur);
                    break;
                case AC_2mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 53, res->un.acw.cur);
                    mysprintf((uint8_t*)res_str->real_cur  , unit_pool[CUR_U_mA], 53, res->un.acw.real_cur);
                    break;
                case AC_10mA:
                case AC_20mA:
                case AC_50mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 52, res->un.acw.cur);
                    mysprintf((uint8_t*)res_str->real_cur  , unit_pool[CUR_U_mA], 52, res->un.acw.real_cur);
                    break;
                case AC_100mA:
                case AC_200mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 51, res->un.acw.cur);
                    mysprintf((uint8_t*)res_str->real_cur  , unit_pool[CUR_U_mA], 51, res->un.acw.real_cur);
                    break;
            }
            
            /* 表示真实电流功能关闭 */
            if(res->un.acw.real_cur == 0xffff)
            {
                    sprintf((char*)res_str->real_cur   , "--");
            }
            break;
        }
        case DCW:
        {
            switch(res->un.dcw.real_gear)
            {
                case DC_2uA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool_[CUR_U_uA], 53, res->un.dcw.cur);
                    break;
                case DC_20uA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool_[CUR_U_uA], 52, res->un.dcw.cur);
                    break;
                case DC_200uA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool_[CUR_U_uA], 51, res->un.dcw.cur);
                    break;
                case DC_2mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 53, res->un.dcw.cur);
                    break;
                case DC_10mA:
                case DC_20mA:
                case DC_50mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 52, res->un.dcw.cur);
                    break;
                case DC_100mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 51, res->un.dcw.cur);
                    break;
            }
            break;
        }
        case IR:
        {
            /* 正常显示 */
            if(res->un.ir.out_range == 0)
			{
                transfer_ir_res_loop_to_string(res, (char*)buf);
                unit = get_ir_res_uint(res);
                mystrcat((uint8_t*)res_str->test_loop, (uint8_t*)div_str_pre_zero((char*)buf), (uint8_t *)unit_pool[unit]);
            }
            /* 超量程 */
			else if(res->un.ir.out_range == 1)
			{
                transfer_ir_res_out_range_loop_to_string(res, (char*)buf);
                
                unit = get_ir_res_uint(res);
                mystrcat((uint8_t*)buf, (uint8_t*)div_str_pre_zero((char*)buf), (uint8_t *)unit_pool[unit]);
                c = strlen((const char*)buf) - strlen((const char*)unit_pool[unit]);
                
                if(c < 4)
                {
                    front_add_space_at_string(buf, 4 - c);
                }
                
                mystrcat((uint8_t*)res_str->test_loop, ">", (uint8_t*)buf);
			}
            /* 短路报警 */
            else
            {
                sprintf((char*)res_str->test_loop  , "< %dMΩ", res->un.ir.res);
            }
            break;
        }
        case GR:
        {
			switch(res->un.gr.gear)
			{
				case DC_GR_20mV:
					break;
				case DC_GR_200mV:
					break;
				case DC_GR_2000mV:
					break;
				case DC_GR_12V:
                case DC_GR_12V_100mA:
                case DC_GR_12V_1000mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[VOL_U_V], 52, res->un.gr.vol);
					break;
				default:
                    if(res->err_num == ERR_OPEN)
                    {
                        sprintf((char*)res_str->test_loop  , "> %dmΩ"        , res->un.gr.res / 10);
                    }
                    else
                    {
                        mysprintf((uint8_t*)res_str->test_loop  , unit_pool[RES_U_mOHM], 51, res->un.gr.res);
                    }
					break;
			}
            break;
        }
        case BBD:
        {
			float temp = res->un.bbd.cap/1000.0;
            uint8_t decs = 0;
			
			if(res->err_num == ERR_SHORT || res->err_num == ERR_OPEN )
			{
				strcpy((char*)res_str->test_loop, "----nF");
			}
			else
			{
				if(temp < 10)
				{
					decs = 3;
				}
				else if(temp < 100)
				{
					decs = 2;
				}
				else
				{
					decs = 1;
				}
				
				mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CAP_U_nF], decs + 50, temp * ten_power(decs));
			}
			break;
        }
        case CC:
        {
            switch(res->un.cc.real_gear)
            {
                case AC_200uA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool_[CUR_U_uA], 51, res->un.cc.cur);
                    break;
                case AC_2mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 53, res->un.cc.cur);
                    break;
                case AC_10mA:
                case AC_20mA:
                case AC_50mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 52, res->un.cc.cur);
                    break;
                case AC_100mA:
                case AC_200mA:
                    mysprintf((uint8_t*)res_str->test_loop  , unit_pool[CUR_U_mA], 51, res->un.cc.cur);
                    break;
            }
            break;
        }
    }
}

void ch_loop_result_to_string_comm(const RESULT_STR *res, char* buf)
{
    switch(res->mode)
    {
        case ACW:
        {
            mysprintf((uint8_t*)buf, NULL, ac_gear[res->un.acw.real_gear].decs + 150, res->un.acw.cur);
            break;
        }
        case DCW:
        {
            mysprintf((uint8_t*)buf, NULL, dc_gear[res->un.dcw.real_gear].decs + 150, res->un.dcw.cur);
            break;
        }
        case IR:
        {
            if(res->un.ir.out_range == 0)
			{
                mysprintf((uint8_t*)buf, NULL, ir_gear[res->un.ir.gear].decs + 150, res->un.ir.res);
			}
			else if(res->un.ir.out_range == 1)
			{
                uint8_t t_buf[20] = {0};
                
                transfer_ir_res_out_range_loop_to_string(res, (char*)t_buf);
                mystrcat((uint8_t*)buf, ">", (uint8_t*)t_buf);
			}
            else
            {
                sprintf(buf  , "< %d"        , res->un.ir.res);
            }
            break;
        }
        case GR:
        {
			switch(res->un.gr.gear)
			{
				case DC_GR_20mV:
					break;
				case DC_GR_200mV:
					break;
				case DC_GR_2000mV:
					break;
				case DC_GR_12V:
                case DC_GR_12V_100mA:
                case DC_GR_12V_1000mA:
                    mysprintf((uint8_t*)buf, NULL, 152, res->un.gr.vol);
					break;
				default:
                    if(res->err_num == ERR_OPEN)
                    {
                        sprintf(buf  , "> %d"        , res->un.gr.res / 10);
                    }
                    else
                    {
                        mysprintf((uint8_t*)buf, NULL, 151, res->un.gr.res);
                    }
					break;
			}
            break;
        }
        case BBD:
        {
			float temp = res->un.bbd.cap/1000.0;
            uint8_t decs = 0;
			
			if(temp < 10)
			{
                decs = 3;
			}
			else if(temp < 100)
			{
                decs = 2;
			}
			else if(temp < 1000)
			{
                decs = 1;
			}
            
            mysprintf((uint8_t*)buf  , NULL, 150 + decs, temp * ten_power(decs));
            break;
        }
    }
}
void ch_real_result_to_string_comm(const RESULT_STR *res, char* buf)
{
    switch(res->mode)
    {
        case ACW:
        {
            /* 表示真实电流功能关闭 */
            if(res->un.acw.real_cur == 0xffff)
            {
                sprintf(buf   , "-----");
            }
            else
            {
                mysprintf((uint8_t*)buf, NULL, ac_gear[res->un.acw.real_gear].decs + 150, res->un.acw.real_cur);
            }
            break;
        }
        case DCW:
        case IR:
        case GR:
        {
            sprintf(buf   , "-----");
            break;
        }
        /* BBD的基准电容 */
        case BBD:
        {
			float temp = res->un.bbd.base_cap/1000.0;
            uint8_t decs = 0;
			
			if(temp < 10)
			{
                decs = 3;
			}
			else if(temp < 100)
			{
                decs = 2;
			}
			else if(temp < 1000)
			{
                decs = 1;
			}
            
            mysprintf((uint8_t*)buf  , NULL, 150 + decs, temp * ten_power(decs));
            break;
        }
    }
}

void ch_open_ratio_to_string_comm(const RESULT_STR *res, char* buf)
{
    switch(res->mode)
    {
        case BBD:
        {
            uint16_t temp = res->un.bbd.open_r;
            mysprintf((uint8_t*)buf  , NULL, 130, temp);
            break;
        }
    }
}

void ch_short_ratio_to_string_comm(const RESULT_STR *res, char* buf)
{
    switch(res->mode)
    {
        case BBD:
        {
            uint16_t temp = res->un.bbd.short_r;
            mysprintf((uint8_t*)buf  , NULL, 130, temp);
            break;
        }
    }
}

typedef struct{
    const char *gear;
}RES_GEAR_STR;

const RES_GEAR_STR res_ac_gear[]=
{
    {0},//0
    {"2uA"   },//2uA
    {"20uA"  },//20uA
    {"200uA" },//200uA
    {"2mA"   },
    {"10mA"  },
    {"20mA"  },
    {"50mA"  },
    {"100mA" },
    {"200mA" },
};

const RES_GEAR_STR res_dc_gear[]=
{
    {0},//0
    {"2uA"   },//2uA
    {"20uA"  },//20uA
    {"200uA" },//200uA
    {"2mA"   },
    {"10mA"  },
    {"20mA"  },
    {"50mA"  },
    {"100mA" },
    {"200mA" },
};

void ch_range_result_to_string(const RESULT_STR *res, RESULT_STRING_STR* res_str)
{
    switch(res->mode)
    {
        case ACW:
            sprintf((char*)res_str->range  , "%s"        , res_ac_gear[res->un.acw.real_gear].gear);
            break;
        case DCW:
            sprintf((char*)res_str->range  , "%s"        , res_dc_gear[res->un.dcw.real_gear].gear);
            break;
        case IR:
            sprintf((char*)res_str->range  , "--");
            break;
        case GR:
            sprintf((char*)res_str->range  , "--");
            break;
        case BBD:
//             sprintf((char*)res_str->range  , "%s"        , res_ac_gear[res->un.bbd.gear].gear);
            break;
        case CC:
            sprintf((char*)res_str->range  , "%s"        , res_ac_gear[res->un.cc.gear].gear);
            break;
    }
}

void ch_range_result_to_string_comm(const RESULT_STR *res, char* buf)
{
    switch(res->mode)
    {
        default:
        case ACW:
            sprintf(buf  , "%1d"        , res->un.acw.gear);
            break;
        case DCW:
            sprintf(buf  , "%1d"        , res->un.dcw.gear);
            break;
        case IR:
        {
            uint8_t temp = res->un.ir.gear;
            if(res->un.ir.gear > 3)
            {
                temp = 5;
            }
            sprintf(buf  , "%1d"        , temp);
            break;
        }
        case GR:
            sprintf(buf  , "");
            break;
        case BBD:
            sprintf(buf  , "");
//             sprintf(buf  , "%1d"        , res->un.bbd.gear);
            break;
        case CC:
            sprintf(buf  , "%1d"        , res->un.cc.gear);
            break;
    }
}

void change_result_to_string(const RESULT_STR *res, RESULT_STRING_STR* res_str, uint16_t n)
{
    sprintf((char*)res_str->num        , "%d"        , n);
    
    switch(sys_par.num_rule)
    {
        case 0:
        case 2:
            sprintf((char*)res_str->dut_name   , "%s"        , res->dut_name);
            break;
        case 1:
            if(res->step == 1)
            {
                sprintf((char*)res_str->dut_name   , "%s"        , res->dut_name);
            }
            else
            {
                sprintf((char*)res_str->dut_name   , "");
            }
            break;
    }
    
    sprintf((char*)res_str->file_name  , "%s"        , res->file_name);
    sprintf((char*)res_str->step       , "`%02d/%02d"     , res->step, res->total);
    sprintf((char*)res_str->work_mode  , "%s"        , work_mode_buf[res->work_mode]);
    sprintf((char*)res_str->test_mode  , "%s"        , mode_pool[GR_VOL_DROP_EN][res->mode]);
    mysprintf((uint8_t*)res_str->test_time  , "s", 51, res->test_time);
    
    ch_out_result_to_string(res, res_str);
    ch_loop_result_to_string(res, res_str);
    ch_range_result_to_string(res, res_str);
    
    if(res->result == RES_PASS)
	{
        sprintf((char*)res_str->result  , "P");
	}
    else
    {
        sprintf((char*)res_str->result  , "F");
    }
    
    sprintf((char*)res_str->time  , "%s", transform_time_to_string(res->time, NULL));
}

void change_result_to_string_comm(const RESULT_STR *res, char* res_buf)
{
    char buf[20] = {0};
    
    switch(sys_par.num_rule)
    {
        case 0:
        case 2:
            sprintf(buf   , "%s"        , res->dut_name);
            break;
        case 1:
            if(res->step == 1)
            {
                sprintf(buf   , "%s"        , res->dut_name);
            }
            else
            {
                sprintf(buf   , "");
            }
            break;
    }
    
    strcat(res_buf, buf);
    strcat(res_buf, ",");
    
    sprintf(buf       , "%02d,%02d,"     , res->step, res->total);
    strcat(res_buf, buf);
    
    sprintf(buf  , "%s,"        , work_mode_buf[res->work_mode]);
    strcat(res_buf, buf);
    
    sprintf(buf  , "%1d,"        , getTestMode(res->mode));
    strcat(res_buf, buf);
    
    sprintf(buf  , "\"%s\","        , res->file_name);
    strcat(res_buf, buf);
    
    ch_out_result_to_string_comm(res, buf);
    strcat(res_buf, buf);
    strcat(res_buf, ",");
    
    ch_range_result_to_string_comm(res, buf);
    strcat(res_buf, buf);
    if(res_buf[strlen(res_buf) - 1] != ',')
    {
        strcat(res_buf, ",");
    }
    
    ch_loop_result_to_string_comm(res, buf);
    strcat(res_buf, buf);
    strcat(res_buf, ",");
    
	/* BBD增加了 基准电容 */
	ch_real_result_to_string_comm(res, buf);
	strcat(res_buf, buf);
	strcat(res_buf, ",");
	
    /* BBD增加了 开路检测和短路检测 */
    if(res->mode == BBD)
    {
        ch_open_ratio_to_string_comm(res, buf);
        strcat(res_buf, buf);
        strcat(res_buf, ",");
        ch_short_ratio_to_string_comm(res, buf);
        strcat(res_buf, buf);
        strcat(res_buf, ",");
    }
    
    mysprintf((uint8_t*)buf  , NULL, 151, res->test_time);
    strcat(res_buf, buf);
    strcat(res_buf, ",");
    
    if(res->result == RES_PASS)
	{
        sprintf(buf  , "P");
	}
    else
    {
        sprintf(buf  , "F");
    }
    
    strcat(res_buf, buf);
//    #ifdef SUZHOU_MUTE
    strcat(res_buf, ",");
    
    sprintf(buf, "%s", transform_time_to_string(res->time, NULL));
    strcat(res_buf, buf);
	
	buf[0] = 0;
	add_other_option(res, buf);
	
	if(buf[0] != 0)
	{
		strcat(res_buf, ",");
		strcat(res_buf, buf);
	}
//    #endif
}

void res_dis_vol(const RESULT_STR *p, const int16_t x, const int16_t y)
{
    RESULT_STRING_STR t_res_str;
    
    ch_out_result_to_string(p, &t_res_str);
    GUI_DispStringAt((const char*)(const char*)t_res_str.test_out, x, y);
    return;
}

void res_dis_cur(const RESULT_STR *p, const int16_t x, const int16_t y)
{
    uint8_t temp_buf[5] = {0};
    uint8_t lon = 0;
    RESULT_STRING_STR t_res_str;
	
    ch_loop_result_to_string(p, &t_res_str);
    lon = strlen((const char*)t_res_str.test_loop);
    
    strcpy((char*)temp_buf, (const char*)&t_res_str.test_loop[lon-2]);
    
    if(0 == strcmp((const char*)temp_buf, "Ω"))
    {
        GUI_DispStringAt((const char*)(const char*)t_res_str.test_loop, x + (8 - lon) * CW12, y);
    }
    else
    {
        GUI_DispStringAt((const char*)(const char*)t_res_str.test_loop, x + (7 - lon) * CW12, y);
    }
    
    return;
}


// /*
//  * 函数名：test_dis_one_res
//  * 描述  ：显示结果列表的一条数据
//  * 输入  ：p 指向要显示的结果数据
//  *			n 标志当前结果数据在第几行显示
//  * 输出  ：无
//  * 调用函数：GUI_SetFont，GUI_DispDecAt，GUI_GotoXY，GUI_DispFloatFix，GUI_DispStringAt
//  * 被调函数：res_list_dis
//  */
void test_dis_one_res(const RESULT_STR *p, const int8_t n)
{
	uint8_t X0  = 5;
	uint8_t Y0	= 3;
	uint8_t X	= (X0+15+3*6);
	uint8_t Y	= (Y0+18+10*(n-1));
	uint8_t mode = p->mode;
    uint8_t index = 2;
	
	char str_time[10] = {0};
	
	transform_time_to_str(p->time, str_time);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
	GUI_DispDecAt (p->step, X0 + 15, Y, 2);
	res_dis_vol(p, X, Y);
	res_dis_cur(p, X + 8 * CW12, Y);
	
	if(p->result == RES_PASS)
	{
		GUI_DispStringAt((const char*)"P", X + 8 * CW12 * 3 + 1 * CW12, Y);
        GUI_DispStringAt((const char*)str_time,	X + 8 * CW12 * 2, Y);
	}
	else
	{
		/* 显示异常信息 */
		if((p->result==0) && (mode >= ACW) && (mode <= BBD))
		{
			GUI_DispStringAt((const char*)except_buf[p->err_num][index],	X + 8 * CW12 * 2, Y);
		}
		
		GUI_DispStringAt((const char*)"F", X + 8 * CW12 * 3 + 1 * CW12, Y);
	}
	
	return;
}


// /*
//  * 函数名：res_list_dis
//  * 描述  ：显示结果列表,每页最多显示四条，可以上下键翻页
//  * 输入  ：无
//  * 输出  ：无
//  * 调用函数：GUI_SetFont，GUI_DispDecAt，GUI_GotoXY，GUI_DispFloatFix，GUI_DispStringAt，GUI_DrawLine，GUI_ClearRect，GUI_FillRect
//  *			OSQPend，GUI_DrawBitmap，clear_menu，test_dis_one_res，
//  * 被调函数：test_subtask
//  */
void res_draw_one_page(const RESULT_STR *p, int32_t cur_page_num)
{
	uint8_t X0	= 10;
	uint8_t X1	= 195;
	uint8_t Y0	= 3;
	uint8_t Y1	= 63;
	
	int32_t i = 0;
	
	GUI_ClearRect(X0+1, Y0+18, X1-1, Y1-1);
	for(i = 0; i < cur_page_num; i++)
	{
		test_dis_one_res(p+i, i+1);
	}
}
// /*
//  * 函数名：res_list_dis
//  * 描述  ：结果列表显示
//  * 输入  ：无
//  * 输出  ：无
//  * 返回  ：无
//  */
void res_list_dis(void)
{
	uint8_t X0	= 10;
	uint8_t Y0	= 3;
	uint8_t X1	= 195;
	uint8_t Y1	= 63;
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t pages = g_cur_file->total/4 + (g_cur_file->total % 4 == 0? 0:1);/* 总页数 */
	int8_t last_page_num = g_cur_file->total % 4 == 0? 4:g_cur_file->total % 4;/* 最后一页的个数 */
	int8_t page = 1;/* 页码 */
	int8_t cur_page_num = 0;/* 当前页的个数 */
	int32_t m = 1;/* 更新标志 */
    uint8_t flag = 0;
	
	
    while(0 == test_flag.save_only_one)
    {
        if(++flag > 5)
        {
            return;
        }
        
        OSTimeDlyHMSM(0,0,0,10);
    }
    
	if(g_comm_status == LOCAL_COMM)
	{
		on_keyboard();/* 打开键盘 */
	}
	
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	
	GUI_DrawLine(200, 0, 200, 64-1);
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt_BTP_STR("结果列表",X0, X1, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt_BTP_STR("Result List",X0, X1, Y0+3);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	
	if(page == pages)
	{
		cur_page_num = last_page_num;
	}
	else
	{
		cur_page_num = 4;
	}
    read_result_list_dis_one_page(result_list_buf, page, cur_page_num);
	res_draw_one_page(result_list_buf, cur_page_num);
	
	while(1)
	{
		if(m)
		{
			m = 0;
			res_draw_one_page(result_list_buf, cur_page_num);
			LCD_REFRESH();
		}
		
		pkey_value = OSQPend(KeyboardQSem, 10, &err);
		
		if(STOP_PIN == 0 || TERMINATE)
		{
			GUI_ClearRect (X0, Y0, X1, Y1);
			clear_menu();
			test_flag.g_stop = 1;
			return;
		}
		
		switch(*pkey_value)
		{
			case KEY_UP:
				m = 1;
				if(--page<1)
				{
					page = pages;
					cur_page_num = last_page_num;
				}
				else
				{
					cur_page_num = 4;
				}
                read_result_list_dis_one_page(result_list_buf, page, cur_page_num);
                res_draw_one_page(result_list_buf, cur_page_num);
				break;
			case KEY_DOWN:
				m = 1;
				if(++page>pages)
				{
					page = 1;
				}
				
				if(page == pages)
				{
					cur_page_num = last_page_num;
				}
				else
				{
					cur_page_num = 4;
				}
                read_result_list_dis_one_page(result_list_buf, page, cur_page_num);
                res_draw_one_page(result_list_buf, cur_page_num);

				break;
			case KEY_F3:
				break;
			case KEY_START:
				if(FAIL)
				{
					break;
				}
				GUI_ClearRect (X0, Y0, X1, Y1);
				clear_menu();
				test_flag.g_start = 1;/* 进入start 测试 */
				return;
			case KEY_BACK:
				if(FAIL)
				{
					break;
				}
				GUI_ClearRect (X0, Y0, X1, Y1);
				clear_menu();
				test_flag.g_stop = 1;
				return;
			case KEY_EXIT:
				if(FAIL)
				{
					break;
				}
				g_exit = 1;
				return;
			default:
				break;
		}
		
		if(g_return || g_exit)
		{
			GUI_ClearRect (X0, Y0, X1, Y1);
			clear_menu();
			return;
		}
		
        /* 当PLC信号设为总步时 */
        if(sys_par.plc_signal == ALL_STEP)
        {
            if(FAIL)
            {
                BUZZER = BUZZER_ON;
                if(STOP_PIN == 0 || STOP_COMM || TERMINATE)
                {
                    GUI_ClearRect (X0, Y0, X1, Y1);
                    clear_menu();
                    test_flag.g_stop = 1;
                    return;
                }
            }
        }
	}
}

uint32_t get_res_total(void)
{
    if(g_custom_sys_par.res_medium == RES_MEDIUM_FLASH)
    {
		return F_RES_TOTAL;
    }
    else
    {
        return E_RES_TOTAL;
    }
}
void save_cur_result(RESULT_STR *res)
{
    RESULT_STR temp_res;
    
    if(CUR_FAIL)
    {
        res->test_time = g_dis_time;
        res->result = RES_FAIL;/* 测试结果 1 P,0 N */
    }
    else
    {
        res->result = RES_PASS;/* 测试结果 1 P,0 N */
    }
    
	/* 每次测试只能保存一次 */
	if(test_flag.save_only_one == 0)
	{
		test_flag.save_only_one++;
	}
    else
	{
		return;
	}
    
//////////////////////////////////////////////////////////////////////
	
    /* 当前结果无条件保存在结果列表里面 */
    save_one_result_list_dis(res, cur_step - 1);
    app_flag.res_list_num_count++;
	save_sys_flag();/* 保存结果列表中的结果编号最后一个值 每天自动清理一次 */
//////////////////////////////////////////////////////////////////////
    
    /* 结果保存关闭 */
	if(!sys_par.is_save_res)
	{
		return;
	}
    
	/* 保存结果 */
	if(sys_par.used_res_num >= get_res_total())
	{
		if(!sys_par.is_overflow_cover)
		{
			return;/* 如果不允许覆盖就直接返回 */
		}
        
		if(sys_par.cover_res_num >= get_res_total())
		{
			sys_par.cover_res_num = 0;/*如果覆盖计数达到n就清0 n为最大值 */
		}
		sys_par.cover_res_num++;
		
        read_one_result(&temp_res, sys_par.cover_res_num - 1);
        
        if(res->result != temp_res.result)
        {
            if(res->result == RES_PASS)
            {
                sys_par.pass_res_num += 1;
            }
            else
            {
                sys_par.pass_res_num -= 1;
            }
        }
        
        save_sys_par();
		save_result(res, SAVE_RESULT);
	}
	else
	{
		sys_par.used_res_num += 1;
        
        if(res->result == RES_PASS)
        {
            sys_par.pass_res_num += 1;
        }
        
        save_sys_par();
		
		save_result(res, SAVE_RESULT);
	}
}

// /*
//  * 函数名：save_result
//  * 描述  ：保存测试结果
//  * 输入  ：stat 可以取INIT_RESULT，SAVE_RESULT 分别表示初始化结果数据 和 保存结果
//  * 输出  ：无
//  * 返回  ：无
//  */
void save_result(RESULT_STR *res, const int8_t stat)
{
	int32_t i = 0;
	
	i = i;
	
    if(res == NULL)
    {
        return;
    }
    
	if(stat == INIT_RESULT)
	{
		uint8_t buf1[20] = {0};
		uint8_t buf2[20] = {0};
		uint32_t cur_date;
		
		cur_date = Time_GetTime();/* 保存当前RTC时间 */
		
		transform_data_to_str(sys_flag.last_res_date, (char*)buf1);
		transform_data_to_str(cur_date, (char*)buf2);
		
		if(0 != strcmp((const char*)buf1, (const char*)buf2))
		{
			sys_flag.res_count = 0;
		}
		
		sys_flag.last_res_date = cur_date;
		
		g_serial_num = sys_flag.res_count;
        
		memset(res, 0, sizeof(RESULT_STR));
		res->step = g_cur_step->one_step.com.step;	/* 结果当前步 */
		res->total = g_cur_file->total;	/* 总步数 */
		res->mode = g_cur_step->one_step.com.mode;	/* 当前步模式 */
		res->work_mode = g_cur_file->work_mode;/* 工作模式 由测试时的文件工作模式决定 */
		res->time = cur_date;/* 保存当前RTC时间 */
		res->test_time = tes_t;
		
		if(res->step == 1)
		{
			app_flag.res_list_num_count = 0;
		}
		
		if(strlen((const char*)sys_par.dut_name) > NAME_LON)
		{
			sys_par.dut_name[NAME_LON] = 0;
			save_sys_par();
		}
        
		strcpy((char*)res->file_name, (const char*)g_cur_file->name);
		
		/* 编号规则 */
		switch(sys_par.num_rule)
		{
			case 0:
				sprintf((char*)res->dut_name, "%05d", ++g_serial_num);
				break;
			case 1:
				if(cur_step == 1)
				{
					++g_serial_num;
				}
				sprintf((char*)res->dut_name, "%05d", g_serial_num);
				break;
			case 2:
				strcpy((char*)res->dut_name, (const char*)sys_par.dut_name);
				break;
		}
		
		sys_flag.res_count = g_serial_num;
		
		res->result = RES_PASS;/* 测试结果 1 P,0 N */
		
		switch(cur_mode)
		{
			case ACW:
				res->un.acw.gear = cur_gear_comm;
				res->un.acw.real_gear = cur_gear;
                result_acw();
				break;
            case CC:
				res->un.cc.gear = cur_gear_comm;
				res->un.cc.real_gear = cur_gear;
                break;
			case DCW:
				res->un.dcw.real_gear = cur_gear;
				res->un.dcw.gear = cur_gear_comm;
				break;
			case IR:
				res->un.ir.gear = cur_gear_comm;
				break;
			case GR:
				res->un.gr.gear = cur_gear;
				break;
			case BBD:
				break;
		}
	}
	else if(stat == SAVE_RESULT)
	{
		uint16_t n = 0;
		uint16_t num = 0;
        
		if(res->step == 0)
		{
			return;
		}
		else if(res->mode >= MODE_END)
		{
			return;
		}
        
		if(!sys_par.is_save_res)
		{
			return;
		}
		
		if(sys_par.used_res_num >= get_res_total() && sys_par.cover_res_num)
		{
			num = sys_par.cover_res_num;
		}
		else
		{
			num = sys_par.used_res_num;
		}
        
        num = num;
        
        n = num;
        
        if(n < 1 || n > get_res_total())
        {
            return;
        }
        
        save_one_result(res, n - 1);
	}
}
void updata_result(uint8_t mode)
{
    switch(mode)
    {
        case ACW:
            result_acw();
            break;
        case DCW:
            result_dcw();
            break;
        case IR:
            result_ir();
            break;
        case GR:
            result_gr();
            break;
        case BBD:
            result_bbd();
            break;
        case CC:
            result_cc();
            break;
    }
}

void result_acw(void)
{
	cur_result.un.acw.vol = vol_ave + ROUND;        /* 测试电压 */
	cur_result.un.acw.cur = cur_ave + ROUND;        /* 测试时的电流或电阻 */
    
	if(0 == cur_real_cur)
	{
		cur_result.un.acw.real_cur = 0xffff;
	}
	else
	{
		cur_result.un.acw.real_cur = real_ave + ROUND;
	}
}

void result_dcw(void)
{
	cur_result.un.dcw.vol = vol_ave + ROUND;        /* 测试电压 */
	cur_result.un.dcw.cur = cur_ave + ROUND;        /* 测试时的电流或电阻 */
}
// /*
//  * 函数名：result_ir
//  * 描述  ：更新ir的测试结果
//  * 输入  ：无
//  * 输出  ：无
//  * 返回  ：无
//  */
void result_ir(void)
{
	cur_result.un.ir.vol = vol_ave + ROUND;         /* 测试电压 */
	
    cur_result.un.ir.out_range = 0;
	cur_result.un.ir.absorptance = ir_absorptance;//吸收比
    
    if(ERR_NUM == ERR_SHORT)
    {
        cur_result.un.ir.res = type_spe.ir_res_l;
        cur_result.un.ir.out_range = 2;
    }
    else if((cur_auto == AUTO_SHIFT_OFF) && (res_ave > gear_up_limit))
    {
        cur_result.un.ir.res = gear_up_limit;
        cur_result.un.ir.out_range = 1;
    }
    else if(res_ave > cur_ir_rang_h)
    {
        cur_result.un.ir.res = cur_ir_rang_h;
        cur_result.un.ir.out_range = 1;
    }
    else
    {
        switch(cur_gear_comm)
        {
            case IR_1_GEAR_COMM:
                cur_result.un.ir.res = res_ave*1000;
                break;
            case IR_2_GEAR_COMM:
                cur_result.un.ir.res = res_ave*100;
                break;
            case IR_3_GEAR_COMM:
                cur_result.un.ir.res = res_ave*10;
                break;
            case IR_4_GEAR_COMM:
                cur_result.un.ir.res = res_ave*1;
                break;
            case IR_5_GEAR_COMM:
                cur_result.un.ir.res = res_ave/10;
                break;
            case IR_6_GEAR_COMM:
                cur_result.un.ir.res = res_ave/100;
                break;
        }
    }
    
	cur_result.un.ir.gear = cur_gear_comm;
}

void result_gr(void)
{
	/* 压降模式 */
	if(GR_VOL_DROP_EN)
	{
		cur_result.un.gr.cur = cur_ave*10 + ROUND;
		cur_result.un.gr.gear = cur_gear;
		switch(cur_gear)
		{
			case DC_GR_20mV:
				break;
			case DC_GR_200mV:
				break;
			case DC_GR_2000mV:
				break;
            case DC_GR_12V:
            case DC_GR_12V_100mA:
            case DC_GR_12V_1000mA:
				cur_result.un.gr.vol = (vol_ave+5)/10;      /* 测试时的电压 */
				break;
		}
	}
	/* GR模式 */
	else
	{
		cur_result.un.gr.cur = cur_ave*100 + ROUND;         /* 测试电流 */
		cur_result.un.gr.res = res_ave;                     /* 测试时的电阻 */
	}
}

void result_bbd(void)
{
	cur_result.un.bbd.vol = vol_ave + ROUND;                /* 测试电压 */
    cur_result.un.bbd.base_cap = cur_cap * 1000 + 0.5;      /* 测试时的 基准电容 单位pF */
    cur_result.un.bbd.open_r = cur_open_ratio;              /* 开路检测 单位% */
    cur_result.un.bbd.short_r = cur_short_ratio;            /* 短路检测 单位% */
    
	if(cap_ave > 0)
	{
		cur_result.un.bbd.cap = cap_ave * 1000 + 0.5;       /* 测试时的电容 单位pF */
	}
}

void result_cc(void)
{
	cur_result.un.cc.vol = vol_ave + ROUND;                /* 测试电压 */
	cur_result.un.cc.cur = cur_ave + ROUND;                /* 测试时的电流或电阻 */
}
static int8_t is_res_clear(void)
{
	uint8_t X0	= 60;
	uint8_t X1	= 130;
	uint8_t Y0	= 15;
	uint8_t Y1	= 49;
	
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	
	uint8_t n = 1;//pf->work_mode;
	GUI_DrawLine(X0, Y0, X1, Y0);
	GUI_DrawLine(X0, Y1, X1, Y1);
	GUI_DrawLine(X0, Y0, X0, Y1);
	GUI_DrawLine(X1, Y0, X1, Y1);
	GUI_ClearRect(X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine(X0, Y0+17, X1, Y0+17);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"是否清空", X0+12, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Clear", X0+20, Y0+3);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);
	GUI_ClearRect (200+1, 15+16+1, 240-1, 15+16+16-1);
	
	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y1-14+3);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+35, Y1-14+3);
	cursor_pos(X0+5+1, Y1-14);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"是", X0+15, Y1-14);
		GUI_DispStringAt((const char*)"否", X0+15+35, Y1-14);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Yes", X0+15, Y1-14);
		GUI_DispStringAt((const char*)"No", X0+15+35, Y1-14);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	while(1)
	{
		switch(n)
		{
			case 1:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_,X0+5, Y1-14+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle, X0+5+35, Y1-14+3);
				cursor_pos(X0+5+1, Y1-14);
				break;
			case 0:
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle,X0+5, Y1-14+3);
				GUI_DrawBitmap((const GUI_BITMAP*) &bmcircle_, X0+5+35, Y1-14+3);
				cursor_pos(X0+5+35+1, Y1-14);
				break;
			default:
				break;
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
				return n;
			case KEY_BACK:
				GUI_ClearRect (40, 5, 170, 57);
				g_return = 1;
				return 0;
			case KEY_EXIT:
				g_exit = 1;
				return 0;
			default:
				break;
		}
	}
}
static void res_census(void)
{
	uint8_t X0	= 40;
	uint8_t X1	= 170;
	uint8_t Y0	= 3;
	uint8_t Y1	= 63;
    
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t n = 1;
	int16_t used_num = sys_par.used_res_num;
	int16_t pass_num = sys_par.pass_res_num;
	int16_t fail_num = used_num - pass_num;
	float used_utilize = (float)sys_par.used_res_num / get_res_total()*100;
	float pass_rate = (float)sys_par.pass_res_num / sys_par.used_res_num*100;
	int8_t lon1 = 0;
	int8_t lon2 = 0;
	
	if(used_utilize >= 100.0)
	{
		lon1 = 5;
	}
	else if(used_utilize >= 10.0)
	{
		lon1 = 4;
	}
	else
	{
		lon1 = 3;
	}
	
	lon1 = used_utilize >= 100.0? 5:(used_utilize >= 10.0? 4:3);
	
	if(pass_rate >= 100.0)
	{
		lon2 = 5;
	}
	else if(pass_rate >= 10.0)
	{
		lon2 = 4;
	}
	else
	{
		lon2 = 3;
	}
	lon2 = pass_rate >= 100.0? 5:(pass_rate >= 10.0? 4:3);
	
	GUI_DrawLine         (X0, Y0, X1, Y0);
	GUI_DrawLine         (X0, Y1, X1, Y1);
	GUI_DrawLine         (X0, Y0, X0, Y1);
	GUI_DrawLine         (X1, Y0, X1, Y1);
	GUI_ClearRect (X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine         (X0, Y0+17, X1, Y0+17);
// 	GUI_DrawLine         ((X0+X1)/2, Y0+21, (X0+X1)/2, Y1-3);
	
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt (":", X0+5+12*4, Y0+19);
	GUI_DispStringAt (":", X0+5+12*4, Y0+19+14);
	GUI_DispStringAt (":", X0+5+12*4, Y0+19+14+14);
	
	/* 画单位 */
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt ("4500(条)", X0+5+12*5, Y0+19);
		GUI_DispStringAt ("1(条)", X0+5+12*5+3*6, Y0+19+14);
		GUI_DispStringAt ("0.0%", X0+5+12*5+2*6, Y0+19+14+14);
	}
	else
	{
		GUI_DispStringAt ("4500(item)", X0+5+12*5, Y0+19);
		GUI_DispStringAt ("1(item)", X0+5+12*5+3*6, Y0+19+14);
		GUI_DispStringAt ("0.0%", X0+5+12*5+2*6, Y0+19+14+14);
	}
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"结果容量统计", X0+28, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Res. capacity stat.", X0+8, Y0+2);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);
	
	clear_menu();
	GUI_DrawBitmap((const GUI_BITMAP*) &bmup, 215, 2);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmdown, 215, 20);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmrightshift, 210, 32);
// 	GUI_DrawBitmap((const GUI_BITMAP*) &bmshortcut, 210, 34);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	
	
	while(1)
	{
		GUI_ClearRect (X0+5+12*5, Y0+19, X0+5+12*5+4*6, Y0+19+14+14);
		GUI_ClearRect (X0+5+12*5, Y0+19+14+14, X0+5+12*5+4*6+5, Y0+19+14+14+12);
		
		GUI_ClearRect (X0+5, Y0+18, X0+5+6*8, Y1-1);
		if(n)
		{
			int8_t temp = used_num>=1000?4:(used_num>=100?3:(used_num>=10?2:1));
			GUI_DispDecAt(get_res_total(), X0+5+12*5, Y0+19, 4);
			GUI_DispDecAt(used_num, X0+5+12*5+(4-temp)*6, Y0+19+14, temp);
			GUI_GotoXY(X0+5+12*5+(5-lon1)*6, Y0+19+14+14);
			GUI_DispFloatFix (used_utilize, lon1, 1);

			if(sys_par.language == CHINESE)
			{
				GUI_DispStringAt ("总容量", X0+10, Y0+19);
				GUI_DispStringAt ("已使用", X0+10, Y0+19+14);
				GUI_DispStringAt ("使用率", X0+10, Y0+19+14+14);
			}
			else
			{
				GUI_DispStringAt ("Total", X0+5, Y0+19);
				GUI_DispStringAt ("Used", X0+5, Y0+19+14);
				GUI_DispStringAt ("Utilize", X0+5, Y0+19+14+14);
			}
		}
		else
		{
			int8_t temp = pass_num>=1000?4:(pass_num>=100?3:(pass_num>=10?2:1));
			GUI_DispDecAt(pass_num, X0+5+12*5+(4-temp)*6, Y0+19, temp);
			temp = fail_num>1000?4:(fail_num>100?3:(fail_num>10?2:1));
			GUI_DispDecAt(fail_num, X0+5+12*5+(4-temp)*6, Y0+19+14, temp);
			GUI_GotoXY(X0+5+12*5+(5-lon2)*6, Y0+19+14+14);
			GUI_DispFloatFix (pass_rate, lon2, 1);

			if(sys_par.language == CHINESE)
			{
				GUI_DispStringAt ("合格数", X0+10, Y0+19);
				GUI_DispStringAt ("失败数", X0+10, Y0+19+14);
				GUI_DispStringAt ("合格率", X0+10, Y0+19+14+14);
			}
			else
			{
				GUI_DispStringAt ("Pass", X0+5, Y0+19);
				GUI_DispStringAt ("Fail", X0+5, Y0+19+14);
				GUI_DispStringAt ("PassRate", X0+5, Y0+19+14+14);
			}
		}
		LCD_REFRESH();
		
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
			case KEY_DOWN:
				n = !n;
				break;
			case KEY_BACK:
				GUI_ClearRect (X0, Y0, X1, Y1);
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


static void serve_result_more(void)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	
	clear_menu();
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"清空", 208, 2);
		GUI_DispStringAt((const char*)"统计", 208, 18);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Clear", 202, 2);
		GUI_DispStringAt((const char*)"Stat.", 208, 18);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	LCD_REFRESH();
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,0,&err);
		switch(*pkey_value)
		{
			case KEY_RES_CLEAR:
				if(is_res_clear())
				{
					sys_par.used_res_num = 0;
					sys_par.pass_res_num = 0;
					sys_par.cover_res_num = 0;
					
                    save_sys_par();
					
					sys_flag.res_count = 0;
					sys_flag.last_res_date = 0;
					save_sys_flag();
					
					memset(&cur_result, 0, sizeof(cur_result));
					g_return = 1;
				}
				break;
			case KEY_RES_CENSUS:
				res_census();
				break;
			case KEY_F3:
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
        
		if(g_return || g_exit)
		{
			return;
		}
	}
}

static uint16_t result_jump(void)
{
	#define X0	40
	#define X1	170
	#define Y0	5
	#define Y1	57
    
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint8_t temp = 0;
	uint16_t t = 0;//存放权数
	uint16_t temp_jump_num = 1;
	
	const uint8_t x = ((X0+(X0+X1)/2)/2-15);
	const uint8_t y = Y1-25;
	
	DATA_INFO jump_num_info = {
		(void*)0/*pData*/,x/*x*/,y/*y*/,0/*type*/,4/*lon*/,0/*decs*/,0/*total*/,0/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
	};
	
	GUI_DrawLine         (X0, Y0, X1, Y0);
	GUI_DrawLine         (X0, Y1, X1, Y1);
	GUI_DrawLine         (X0, Y0, X0, Y1);
	GUI_DrawLine         (X1, Y0, X1, Y1);
	GUI_ClearRect        (X0+1, Y0+1, X1-1, Y1-1);
	GUI_DrawLine         (X0, Y0+17, X1, Y0+17);
	GUI_DrawLine         ((X0+X1)/2, Y0+21, (X0+X1)/2, Y1-3);
	
	GUI_DispStringAt ("0001", (X0+(X0+X1)/2)/2-15, Y1-25);
	
	GUI_DispStringAt ("0001", (X1+(X0+X1)/2)/2-10, Y0+18);
    
	GUI_DispDecAt (sys_par.used_res_num, (X1+(X0+X1)/2)/2-10, Y1-12, 4);
	GUI_DrawBitmap((const GUI_BITMAP*) &bmarrow, (X1+(X0+X1)/2)/2-3, Y0+30);
	
	GUI_FillRect(X0+2, Y0+2, X1-2, Y0+15);
	GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
    
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"跳至结果索引号", X0+20, Y0+3);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"The number of result", X0+8, Y0+2);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
	
	GUI_ClearRect (200+1, 0, 240-1, 15-1);
	GUI_ClearRect (200+1, 15+1, 240-1, 15+16-1);
    
    draw_short_cut_bm(UDRB);
	
	pmenu_data = &jump_num_info;
	pmenu_data->pData = (uint16_t*)pmenu_data->pData;
	pmenu_data->pData = &temp_jump_num;
	pmenu_data->ch_bit = 0;
	LCD_REFRESH();
	cursor_pos(x,y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem, 0, &err);
		
		serve_numkey(*pkey_value);
		switch(*pkey_value)
		{
			case KEY_UP:
				t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				t = temp_jump_num/t;
				temp = t%10;
				
				if(++temp > 9)
				{
					temp = 0;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(x+pmenu_data->ch_bit*6,y);//显示光标
				break;
			case KEY_DOWN:
				t = pow(10, (pmenu_data->lon - pmenu_data->ch_bit -1));
				t = temp_jump_num/t;
				temp = t%10;

				if(temp == 0)
				{
					temp = 9;
				}
				else
				{
					temp--;
				}
				change_data(temp);
				update_data(pmenu_data);
				cursor_pos(x+pmenu_data->ch_bit*6,y);//显示光标
				break;
			case KEY_SHIFT:
				shift_cursor(pmenu_data);
				temp = 0;
				break;
			case KEY_ENTER:
				if(temp_jump_num > sys_par.used_res_num)
				{
					temp_jump_num = 1;
					update_data(pmenu_data);
					pmenu_data->ch_bit = 0;
					cursor_pos(x,y);
					break;
				}
				
				return temp_jump_num;
			case KEY_BACK:
				GUI_ClearRect (40, 5, 170, 57);
				g_return = 1;
				return 0;
			case KEY_EXIT:
				g_exit = 1;
				return 0;
			default:
				break;
		}
	}
	#undef X0
	#undef X1
	#undef Y0
	#undef Y1
}

int32_t wait_usb_connect(void)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
    uint8_t retry_times = 0;
    
    updata_comm_flag |= CHECK_USB_CONNECT;
    
    draw_wait_usb_connect_gui();
    app_flag.usb_con_st = USB_CON_NG;
    
    while(app_flag.usb_con_st != USB_CON_OK)
    {
        OSTimeDlyHMSM(0,0,1,0);
        if(++retry_times > 10)
        {
            draw_no_connect_usb_gui();
            
            pkey_value = OSQPend(KeyboardQSem,0,&err);
            
            switch(*pkey_value)
            {
                case KEY_RES_TRANS:
                    break;
                case KEY_EXIT:
                    return -1;
            }
        }
    }
    
    return 0;
}
static uint16_t result_short_cut(void)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	uint16_t temp_jump_num = 0;
l:
	clear_menu();
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"传输", 208, 2);
		GUI_DispStringAt((const char*)"跳至", 208, 18);
		GUI_DispStringAt((const char*)"更多", 208, 34);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Transmit", 202, 2);
		GUI_DispStringAt((const char*)"Jump", 208, 18);
		GUI_DispStringAt((const char*)"More", 208, 34);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
    
	GUI_DrawBitmap((const GUI_BITMAP*) &bmback, 210, 50);
	LCD_REFRESH();
	
	while(1)
	{
        
        pkey_value = OSQPend(KeyboardQSem,0,&err);
        switch(*pkey_value)
        {
            case KEY_RES_TRANS:
                /* 等待U盘连接 */
                if(-1 == wait_usb_connect())
                {
                    g_return = 1;
                    return 0;
                }
                
                /* 导出结果 */
                updata_comm_flag |= RESULT_LEAD_OUT_USB;
                app_flag.usb_loading_status = LOADING_OUT;
                app_flag.usb_loading_advance = 0;
                app_flag.leading_watch_dog = 0;
                rate_of_advance();
				g_return = 1;
				return 0;
			case KEY_RES_JUMP:
				temp_jump_num = result_jump();
				g_return = 1;
				return temp_jump_num;
			case KEY_MORE:
				serve_result_more();
				if(g_return)
				{
					return 0;
				}
				break;
			case KEY_BACK:
				g_return = 1;
				return 0;
			case KEY_EXIT:
				g_exit = 1;
				return 0;
			default:
				break;
		}
        
		if(g_return)
		{
			g_return = 0;
			goto l;
		}
        
		if(g_exit)
		{
			return 0;
		}
	}
}
// /*
//  * 函数名：dis_title
//  * 描述  ：显示传入结果的表头信息
//  * 输入  ：const RESULT_STR *p 指向当前一条结果
//  * 输出  ：无
//  * 返回  ：无
//  */
static void dis_title(const RESULT_STR *p)
{	
	uint8_t mode = p->mode;
	char dis[10] = {0};
	
	if(strlen((const char*)p->file_name) > 9)
	{
		strncpy(dis, (const char*)p->file_name, 7);
		strcat(dis, "..");
	}
	else
	{
		strcpy(dis, (const char*)p->file_name);
	}
	
	GUI_ClearRect (0, 0, 200-1, 11);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_DispStringAt((const char*)"/"  , 4+35+2*6, 0);
	GUI_DispStringAt((const char*)"：" , 4+4*6, 0);
	GUI_DispStringAt((const char*)"：" , 4+35+38+15+20+4*6, 0);
	
	if(sys_par.language == CHINESE)
	{
		GUI_DispStringAt((const char*)"步骤" , 4, 0);
		GUI_DispStringAt((const char*)"文件" , 4+35+38+15+25-3, 0);
	}
	else
	{
		GUI_SetFont(&GUI_FontHZ_SimSun_13);
		GUI_DispStringAt((const char*)"Step" , 4+3, -1);
		GUI_DispStringAt((const char*)"File" , 4+35+38+15+25, -1);
		GUI_SetFont(&GUI_FontHZ_SimSun_12);
	}
	
	if(p == NULL)
	{
		return;
	}
	
	GUI_DispDecAt(p->step, 4+35, 0, 2);
	GUI_DispDecAt(p->total, 4+35+3*6, 0, 2);
    
    GUI_DispStringAt((const char*)work_mode_buf[p->work_mode], 4+35+38-1, 0);
	GUI_DispStringAt((const char*)mode_pool[GR_VOL_DROP_EN][mode], 4+35+38+15-2, 0);
	GUI_DispStringAt((const char*)(const char*)dis, 4+35+38+15+20+6*5+3, 0);
    
	LCD_REFRESH();
}

// /*
//  * 函数名：dis_one_res
//  * 描述  ：显示一条结果信息
//  * 输入  ：const RESULT_STR *p 指向当前一条结果
//  *			const int8_t n 1-4
//  * 输出  ：无
//  * 返回  ：无
//  */
static void dis_one_res(const RESULT_STR *p, const int8_t n)
{
	#define X	12+5*6
	#define Y	15+12*(n-1)
	#define CW	6
	
	char str_time[10] = {0};
	uint8_t x = 12+5*6+7*6+2*6+7*6;
	uint8_t y = 15+12*(n-1);
	uint8_t mode = p->mode;
	
	LCD_REFRESH();
	transform_time_to_str(p->time, str_time);
	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	
	res_dis_vol(p, X, Y);
	res_dis_cur(p, X+8*CW, Y);
    
	if(p->result)
	{
		GUI_DispStringAt((const char*)str_time,	12+5*6+7*6+2*6+7*6, 15+12*(n-1));
		GUI_DispStringAt((const char*)"P",		12+5*6+7*6+2*6+7*6+1*6+6*6+1*6+3, 15+12*(n-1));
	}
	else
	{
		GUI_DispStringAt((const char*)"F",      12+5*6+7*6+2*6+7*6+1*6+6*6+1*6+3, 15+12*(n-1));
	}
	
	/* 显示异常信息 */
	if((p->result==0) && (mode >= ACW) && (mode < MODE_END))
	{
		if(sys_par.language == CHINESE)
		{
			GUI_DispStringAt((const char*)except_buf[p->err_num][0], x, y);
		}
		else
		{
			GUI_SetFont(&GUI_FontHZ_SimSun_12);
			GUI_DispStringAt((const char*)except_buf[p->err_num][2], x, y);
		}
	}
	
	LCD_REFRESH();
    
	#undef X
	#undef Y
	#undef CW
}

void sys_result(uint32_t w)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t n = 1;
	int16_t p = 1;
	uint8_t i = 0;
	uint16_t temp_jump_num = sys_par.used_res_num;
	int16_t cur_page_num = 0;
	int8_t s = 1;
	
	if(sys_par.used_res_num >= get_res_total())
	{
		temp_jump_num = sys_par.cover_res_num;
	}
	
	if(temp_jump_num)
	{
		p = temp_jump_num/PER_PAGE_RES_NUM;
		n = temp_jump_num%PER_PAGE_RES_NUM;
		
		if(n != 0)
		{
			p += 1;
		}
		else
		{
			n = PER_PAGE_RES_NUM;
		}
	}
	else
	{
		p = 1;
		n = 1;
	}
	
	gui_draw_test_result();
	dis_one_res(&result_buf[0],1);
	
	while(1)
	{
		for(i = 1; i < 5; i++)
		{
            if(i == n)
            {
                GUI_SetTextMode(GUI_TEXTMODE_REV);//设置反转文本
            }
			GUI_DispDecAt ((p-1)*PER_PAGE_RES_NUM + i, 12-6, 15+12*(i-1), 5);
            GUI_SetTextMode(GUI_TEXTMODE_NORMAL);//设置正常文本
		}
        LCD_REFRESH();
		if(s)
		{
			s = 0;
			GUI_ClearRect (40, 15, 200-1, 64-1);
            
			if((p-1)*4+sys_par.used_res_num%4 <= sys_par.used_res_num)
			{
				if((sys_par.used_res_num/PER_PAGE_RES_NUM) >= p)
				{
					cur_page_num = PER_PAGE_RES_NUM;
				}
				else if(sys_par.used_res_num%4 != 0 && p == sys_par.used_res_num/PER_PAGE_RES_NUM + 1)
				{
					cur_page_num = sys_par.used_res_num%PER_PAGE_RES_NUM;
				}
				else
				{
					cur_page_num = 0;
				}
                
				if(cur_page_num > 0)
				{
                    read_n_results(result_buf, (p-1)*PER_PAGE_RES_NUM + 0, PER_PAGE_RES_NUM);
				}
				else
				{
					memset(result_buf, 0, sizeof(RESULT_STR)*PER_PAGE_RES_NUM);
				}
				
				for(i = 0; i < cur_page_num; i++)
				{
					dis_one_res(&result_buf[i], i+1);
				}
			}
			else
			{
				cur_page_num = 0;
			}
			
			if(cur_page_num >= 1 && cur_page_num >= n)
			{
				dis_title(&result_buf[n-1]);
			}
			else
			{
				dis_title((void*)0);/* 当前页为0时就传入空指针 */
			}
			LCD_REFRESH();
		}
		
		pkey_value = OSQPend(KeyboardQSem,0,&err);
        
		switch(*pkey_value)
		{
			case KEY_UP:
            {
				if(--n < 1)
				{
					n=4;
					if(--p < 1)
					{
						p = get_res_total()/PER_PAGE_RES_NUM;
					}
					GUI_ClearRect (40, 15, 200-1, 64-1);
					s = 1;
				}
				
				if(cur_page_num >= n)
				{
					dis_title(&result_buf[n-1]);
				}
				else
				{
					dis_title((void*)0);
				}
// 				for(i = 1; i < 5; i++)
// 				{
// 					GUI_DispDecAt ((p-1)*PER_PAGE_RES_NUM + i, 12, 15+12*(i-1), 4);
// 				}
				
				LCD_REFRESH();
				break;
            }
			case KEY_DOWN:
            {
				if(++n > PER_PAGE_RES_NUM)
				{
					n=1;
					if(++p > get_res_total()/PER_PAGE_RES_NUM)
					{
						p = 1;
					}
					s = 1;
					GUI_ClearRect (40, 15, 200-1, 64-1);
				}
				else
				{
					if(cur_page_num >= n)
					{
						dis_title(&result_buf[n-1]);
					}
					else
					{
						dis_title((void*)0);
					}
				}
				
// 				for(i = 1; i < 5; i++)
// 				{
// 					GUI_DispDecAt ((p-1)*PER_PAGE_RES_NUM + i, 12, 15+12*(i-1), 4);
// 				}
//                 
// 				LCD_REFRESH();
				break;
            }
			case KEY_SHORTCUT:
            {
				if(n > cur_page_num)
				{
					g_cur_res = NULL;
				}
				else
				{
					g_cur_res = &result_buf[n-1];
				}
                
				temp_jump_num = result_short_cut();
                
				if(temp_jump_num)
				{
					p = temp_jump_num/PER_PAGE_RES_NUM;
					n = temp_jump_num%PER_PAGE_RES_NUM;
                    
					if(n != 0)
					{
						p += 1;
					}
					else
					{
						n = PER_PAGE_RES_NUM;
					}
				}
                
				if(g_return)
				{
					g_return = 0;
					s = 1;
					gui_draw_test_result();
				}
				break;
            }
			case KEY_BACK:
				g_return = 1;
				return;
			case KEY_EXIT:
				g_exit = 1;
				return ;
			default:
				break;
		}
		
		if(g_return || g_exit)
		{
			return;
		}
	}
}
 
 
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
