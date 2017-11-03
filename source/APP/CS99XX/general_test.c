/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：general_test.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#include    "cs99xx_vref.h"
#include 	<math.h>
#include	<string.h>
#include	<stdio.h>
#include	"keyboard.h"
#include 	"MC14094.h"
#include 	"app.h"
#include    "cs99xx_result.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_relay_motion.h"
#include	"cs99xx_type.h"
#include	"cs99xx_it.h"
#include 	"mb_server.h"
#include 	"SysTemConfig.h"
#include    "cs99xx_plc.h"
#include    "cs99xx_led.h"
#include    "cs99xx_collect.h"
#include    "test_com.h"
#include    "acw_test_g.h"
#include    "acw_test.h"



void update_info(void)
{
    clear_();
	load_data();
	gui_draw_test();
	dis_test_ui();	/* 画出当前步的测试数据 */
}

void dis_gear_up_limit(float f, uint16_t x, uint16_t y, uint8_t dec)
{
	uint8_t CW	= 9;	/* 单字符宽度 */
    uint8_t temp_long = 0;
    
    temp_long = dec == 1? COUNT_FLOAT_LON_1DECS(f) : (dec==2? COUNT_FLOAT_LON_2DECS(f) : 5);
    
    GUI_DispStringAt((const char*)">", x + (5-temp_long-1)*CW, y);
    GUI_GotoXY(x+(5-temp_long)*CW, y);
    GUI_DispFloatFix (f, temp_long, dec);
}

/* 电阻大于量程 */
uint8_t judge_ir_rang(void)
{
    uint8_t res_buf[10] = {0};
    
    if(res_ave > cur_ir_rang_h)
    {
        if(cur_ir_rang_h < 100 * 1000)
        {
            mysprintf(res_buf, NULL, 41, cur_ir_rang_h / 1000 * 10);
        }
        else
        {
            mysprintf(res_buf, NULL, 40, cur_ir_rang_h / 1000);
        }
        
        sprintf((char*)loop_buf, ">%s", res_buf);
        return 1;
    }
    
    return 0;
}

void tr_ir_gear_up_limit_to_string(void)
{
    uint8_t res_buf[20] = {0};
    
    if(cur_gear == cur_ir_dis_gear_1)
    {
        switch(cur_ir_dis_gear_1)
        {
            case IR_10MOHM:
                mysprintf(res_buf, NULL, 41, gear_up_limit * 10);
                break;
            case IR_100MOHM:// > 10.0M
                mysprintf(res_buf, NULL, 41, gear_up_limit * 10);
                break;
            case IR_1GOHM:// > 100M
                mysprintf(res_buf, NULL, 40, gear_up_limit);//>100M
                break;
            case IR_10GOHM:
                if(cur_gear == IR_1GOHM)
                {
                    mysprintf(res_buf, NULL, 40, gear_up_limit);//>1000M
                }
                else
                {
                    mysprintf(res_buf, NULL, 42, gear_up_limit * 100/1000.0);//>1.00G
                }
                break;
            case IR_100GOHM:
                mysprintf(res_buf, NULL, 41, gear_up_limit/1000 * 10);//>10.0G
                break;
            default:
                mysprintf((uint8_t*)loop_buf, NULL, 40, gear_up_limit/1000);//>100G
                break;
        }
        
        sprintf((char*)loop_buf, ">%s", res_buf);
        res_ave_comm = 9999;
    }
    else
    {
        switch(cur_gear)
        {
            case IR_10MOHM:
                mysprintf(res_buf, NULL, 41, gear_up_limit * 10);
                break;
            case IR_100MOHM:
                mysprintf(res_buf, NULL, 40, gear_up_limit);//>100M
                break;
            case IR_1GOHM:
                mysprintf(res_buf, NULL, 40, gear_up_limit);//>1000M
                break;
            case IR_10GOHM:
                mysprintf(res_buf, NULL, 41, gear_up_limit/1000 * 10);//>10.0G
                break;
            case IR_100GOHM:
                mysprintf(res_buf, NULL, 40, gear_up_limit/1000);//>100G
                break;
            default:
                mysprintf(res_buf, NULL, 40, gear_up_limit/1000);//>100G
                break;
        }
        
        sprintf((char*)loop_buf, ">%s", res_buf);
        res_ave_comm = 9999;
    }
}

void transform_test_ir_res_loop(void)
{
    uint8_t flag = 0;
    uint8_t res_buf[10] = {0};
    
	if(app_flag.absorptance)
	{
        mysprintf(real_buf, NULL, 51, ir_absorptance * 10);
	}
	else
	{
		memset(real_buf, 0, sizeof(real_buf));
	}

    if(test_flag.gradation == STAGE_READY || TERMINATE)
    {
        if(cur_gear < 4)
        {
            mysprintf(loop_buf, NULL, ir_gear[cur_gear].decs + 50, res_ave * pow(10, ir_gear[cur_gear].decs));
        }
        else if(cur_gear == 4)
        {
            if(res_ave < 10 * 1000)
            {
                mysprintf(loop_buf, NULL, ir_gear[cur_gear].decs + 50, res_ave);
            }
            else
            {
                mysprintf(loop_buf, NULL, ir_gear[cur_gear].decs + 50 - 1, res_ave / 10);
            }
        }
        else
        {
            mysprintf(loop_buf, NULL, ir_gear[cur_gear].decs + 50, res_ave / 10);
        }
		return;
    }
    
	/* 电压上升阶段不显示电阻值 */
	if(test_flag.gradation == STAGE_RISE)
	{
        strcpy((char*)loop_buf, res_infinite_str[cur_gear]);
		return;
	}
    
    if(ERR_NUM == ERR_SHORT)
    {
        mysprintf(res_buf, NULL, 41, type_spe.ir_res_l * 10);
        sprintf((char*)loop_buf, "<%s", res_buf);
        return;
    }
    
	/*
	开路 
		条件是：1 电流很小
				2 电压在允许的波动范围内+-10v
	*/
    if(cur_ave <= 1.0 && (((vol_ave + 10) > cur_vol) || (vol_ave < (cur_vol + 10))))
	{
        if(cur_auto == AUTO_SHIFT_OFF)
        {
            tr_ir_gear_up_limit_to_string();
        }
        else
        {
            if(cur_gear >= IR_10GOHM)
            {
                /* 电阻大于量程 */
                judge_ir_rang();
            }
            else
            {
                sprintf((char*)loop_buf, "%s", res_infinite_str[cur_gear]);
            }
        }
	}
	else
	{
        if(cur_auto == AUTO_SHIFT_OFF)
        {
            if(res_ave > gear_up_limit)
            {
                tr_ir_gear_up_limit_to_string();
                flag = 1;
            }
        }
        
        if(flag == 0)
		switch(cur_gear)
		{
			case IR_10MOHM:/* 1-10M */
			{
                if(res_ave < 10)
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 53, res_ave * 1000);
                }
                else
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 52, res_ave * 100);
                }
				res_ave_comm = res_ave * 1000;
				break;
			}
			case IR_100MOHM:/* 10-100M */
			{
                if(res_ave < 100)
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 152, res_ave * 100);
                }
                else
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 151, res_ave * 10);
                }
                
                res_ave_comm = res_ave * 100;
				break;
			}
			case IR_1GOHM:/* 100-1000M */
			{
                if(res_ave < 1000)
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 51, res_ave * 10);
                }
                else
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 50, res_ave);
                }
                
                res_ave_comm = res_ave * 10;
				break;
			}
			case IR_10GOHM:/* 1-10G */
			{
                /* 电阻大于量程 */
                if(judge_ir_rang())
                {
                    break;
                }
                
                if(res_ave < 10 * 1000)
                {
                    mysprintf((uint8_t*)loop_buf, NULL, ir_gear[cur_gear].decs + 50, res_ave);
                }
                else
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 52, res_ave / 10);
                }
                
                res_ave_comm = res_ave / 1;
				break;
			}
			case IR_100GOHM:
			{
                /* 电阻大于量程 */
                if(judge_ir_rang())
                {
                    break;
                }
                
                if(res_ave < 1000 * 1000)
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 52, res_ave / 10);
                }
                else
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 52, res_ave / 10);
                }
                
                res_ave_comm = res_ave / 100;
				break;
			}/* end case */
		}//end switch(cur_gear)
	}//end else
}




void dis_gr_vol(float gr_vol)
{
	uint8_t X	= 10;
	uint8_t Y	= 13;
// 	uint8_t X1	= X;
// 	uint8_t Y1	= 46;
// 	uint8_t X2	= 90;
// 	uint8_t	Y2 	= 42;
	uint8_t O	= 13;
	uint8_t CW	= 9;	/* 单字符宽度 */
	
// 	GUI_GotoXY(X+13*CW, Y);
// 	GUI_DispFloatFix ((float)0, 3, 1);
	if(!GR_VOL_DROP_EN)
	{
		return;
	}
	
	if(type_spe.gr_dis_opt & GR_DIS_RES)
	{
		GUI_DispStringAt((const char*)"---.-", X+11*CW, Y+O);
	}
	
	if(type_spe.gr_dis_opt & GR_DIS_VOL)
	{
		switch(cur_gear)
		{
			case DC_GR_20mV:/* 20mV */
				if(gr_vol < 1000)
				{
					GUI_GotoXY(X+11*CW+CW*1, Y);
					GUI_DispFloatFix ((float)gr_vol/100, 4, 2);
				}
				else
				{
					GUI_GotoXY(X+11*CW+CW*0, Y);
					GUI_DispFloatFix ((float)gr_vol/100, 5, 2);
				}
				break;
			case DC_GR_200mV:/* 200mV */
				if(gr_vol < 100)
				{
					GUI_GotoXY(X+11*CW+CW*2, Y);
					GUI_DispFloatFix ((float)gr_vol/10, 3, 1);
				}
				else if(gr_vol < 1000)
				{
					GUI_GotoXY(X+11*CW+CW*1, Y);
					GUI_DispFloatFix ((float)gr_vol/10, 4, 1);
				}
				else
				{
					GUI_GotoXY(X+11*CW+CW*0, Y);
					GUI_DispFloatFix ((float)gr_vol/10, 5, 1);
				}
				break;
			case DC_GR_2000mV:/* 2000mV */
				if(gr_vol < 10)
				{
					GUI_GotoXY(X+11*CW+CW*4, Y);
					GUI_DispFloatFix ((float)gr_vol, 1, 0);
				}
				else if(gr_vol < 100)
				{
					GUI_GotoXY(X+11*CW+CW*3, Y);
					GUI_DispFloatFix ((float)gr_vol, 2, 0);
				}
				else if(gr_vol < 1000)
				{
					GUI_GotoXY(X+11*CW+CW*2, Y);
					GUI_DispFloatFix ((float)gr_vol, 3, 0);
				}
				else
				{
					GUI_GotoXY(X+11*CW+CW*1, Y);
					GUI_DispFloatFix ((float)gr_vol, 4, 0);
				}
				break;
			case DC_GR_12V:/* 12V */
			case DC_GR_12V_100mA:/* 12V */
			case DC_GR_12V_1000mA:/* 12V */
				if(gr_vol < 1000)
				{
					GUI_GotoXY(X+11*CW+CW*1, Y);
					GUI_DispFloatFix ((float)gr_vol/100, 4, 2);
				}
				else
				{
					GUI_GotoXY(X+11*CW+CW*0, Y);
					GUI_DispFloatFix ((float)gr_vol/100, 5, 2);
				}
				break;
		}
	}
}
void dis_test_gr_cur(void)
{
	uint8_t X	= 10;
	uint8_t Y	= 13;
	uint8_t O	= 13;
	uint8_t CW	= 9;	/* 单字符宽度 */
	
	GUI_ClearRect(X+0*CW, Y, X+0*CW+CW*5, Y+O);
    
	switch(type_spe.gr_cur_unit)
	{
		case CUR_U_A:
            mysprintf((uint8_t*)output_buf, NULL, 152, cur_ave * 100 + ROUND);
            
			if(cur_ave < 10)
			{
				GUI_GotoXY(X+CW*1, Y);
				GUI_DispFloatFix ((float)cur_ave, 5-1, 2);
			}
			else
			{
				GUI_GotoXY(X, Y);
				GUI_DispFloatFix ((float)cur_ave, 5, 2);
			}
			break;
		case CUR_U_mA:
            mysprintf((uint8_t*)output_buf, NULL, 151, cur_ave * 10 + ROUND);
			if(cur_ave < 10)
			{
				GUI_GotoXY(X+CW*2, Y);
				GUI_DispFloatFix ((float)cur_ave, 5-2, 1);
			}
			else if(cur_ave < 100)
			{
				GUI_GotoXY(X+CW*1, Y);
				GUI_DispFloatFix ((float)cur_ave, 5-1, 1);
			}
			else if(cur_ave < 1000)
			{
				GUI_GotoXY(X+CW*0, Y);
				GUI_DispFloatFix ((float)cur_ave, 5, 1);
			}
			else
			{
				GUI_GotoXY(X+CW*0, Y);
				GUI_DispFloatFix ((float)cur_ave, 5, 2);
			}
			break;
	}
}

void dis_test_gr_vol(void)
{
    uint8_t X	= 10;
	uint8_t Y	= 13;
	uint8_t O	= 13;
	uint8_t CW	= 9;	/* 单字符宽度 */
	
	GUI_SetFont(&GUI_FontHZ_SimSun_15);
	
	/* 进入测试前0.2s不显示电阻 */
	if(g_dis_time < 2 && !GR_OVER)
	{
		if(GR_VOL_DROP_EN && type_spe.gr_dis_opt & GR_DIS_RES)
		{
			GUI_DispStringAt((const char*)"---.-", X+11*CW, Y+O);
		}
		return;
	}
    
	if(GR_VOL_DROP_EN)
	{
		/* 显示电压 */
		if(type_spe.gr_dis_opt & GR_DIS_VOL)
		{
			/* 压降模式打开后显示电压 */
			if(GR_VOL_DROP_EN)
			{
				GUI_ClearRect(X+11*CW, Y+O, X+11*CW+CW*5, Y+2*O);
				switch(cur_gear)
				{
					case DC_GR_20mV:
					{
						if(vol_ave < 1000)
						{
							GUI_GotoXY(X+11*CW+CW*1, Y);
							GUI_DispFloatFix ((float)vol_ave, 4, 2);
						}
						else
						{
							GUI_GotoXY(X+11*CW+CW*0, Y);
							GUI_DispFloatFix ((float)vol_ave, 5, 2);
						}
						break;
					}
					case DC_GR_200mV:
					{
						if(vol_ave < 100)
						{
							GUI_GotoXY(X+11*CW+CW*2, Y);
							GUI_DispFloatFix ((float)vol_ave, 3, 1);
						}
						else if(vol_ave < 1000)
						{
							GUI_GotoXY(X+11*CW+CW*1, Y);
							GUI_DispFloatFix ((float)vol_ave, 4, 1);
						}
						else
						{
							GUI_GotoXY(X+11*CW+CW*0, Y);
							GUI_DispFloatFix ((float)vol_ave, 5, 1);
						}
						break;
					}
					case DC_GR_2000mV:
					{
						if(vol_ave < 10)
						{
							GUI_GotoXY(X+11*CW+CW*4, Y);
							GUI_DispFloatFix ((float)vol_ave, 1, 0);
						}
						else if(vol_ave < 100)
						{
							GUI_GotoXY(X+11*CW+CW*3, Y);
							GUI_DispFloatFix ((float)vol_ave, 2, 0);
						}
						else if(vol_ave < 1000)
						{
							GUI_GotoXY(X+11*CW+CW*2, Y);
							GUI_DispFloatFix ((float)vol_ave, 3, 0);
						}
						else
						{
							GUI_GotoXY(X+11*CW+CW*1, Y);
							GUI_DispFloatFix ((float)vol_ave, 4, 0);
						}
						break;
					}
					case DC_GR_12V:
					case DC_GR_12V_100mA:
					case DC_GR_12V_1000mA:
					{
						if(vol_ave < 10000)
						{
							GUI_GotoXY(X+11*CW+CW*1, Y);
							GUI_DispFloatFix ((float)vol_ave/1000, 4, 2);
						}
						else
						{
							GUI_GotoXY(X+11*CW+CW*0, Y);
							GUI_DispFloatFix ((float)vol_ave/1000, 5, 2);
						}
						break;
					}
				}
			}

		}
		/* 显示电阻 */
		else if(type_spe.gr_dis_opt & GR_DIS_RES)
		{
			/* 接地电阻为零时要显示出来 2014.10.20 */
			if(res_ave < 100)
			{
				GUI_GotoXY(X+11*CW+2*CW, Y+O);
				GUI_DispFloatFix ((float)res_ave/10, 3, 1);
			}
			else if(res_ave < 1000)
			{
				GUI_GotoXY(X+11*CW+1*CW, Y+O);
				GUI_DispFloatFix ((float)res_ave/10, 4, 1);
			}
			else
			{
				GUI_GotoXY(X+11*CW+0*CW, Y+O);
				GUI_DispFloatFix ((float)res_ave/10, 5, 1);
			}
		}
	}
	else
	{
        char buf[10] = {0};
        
        if(ERR_NUM == ERR_OPEN)
        {
            strcpy((char*)buf, "> 510");
        }
        else
        {
            mysprintf((uint8_t*)loop_buf, NULL, 151/*"%.1f"*/, (uint32_t)res_ave);
        }
        
        strcpy((char*)buf, div_str_pre_zero((char*)loop_buf));
        
        GUI_DispStringAt((const char*)(const char*)buf, X+11*CW+(5-strlen((const char*)buf)) * CW, Y);
	}
}
void transform_test_gr_vol(void)
{
	/* 进入测试前0.2s不显示电阻 */
    if(test_flag.gradation == STAGE_TEST)
    {
        if(g_dis_time < 2 && !GR_OVER)
        {
            if(GR_VOL_DROP_EN && type_spe.gr_dis_opt & GR_DIS_RES)
            {
                strcpy((char*)loop_buf, "---.-");
            }
            return;
        }
    }
    
	if(GR_VOL_DROP_EN)
	{
		/* 显示电压 */
		if(type_spe.gr_dis_opt & GR_DIS_VOL)
		{
			/* 压降模式打开后显示电压 */
			if(GR_VOL_DROP_EN)
			{
				switch(cur_gear)
				{
					case DC_GR_20mV:
					{
                        mysprintf(loop_buf, NULL, 152, vol_ave * 100);
						break;
					}
					case DC_GR_200mV:
					{
                        mysprintf(loop_buf, NULL, 151, vol_ave * 100);
						break;
					}
					case DC_GR_2000mV:
					{
                        mysprintf(loop_buf, NULL, 150, vol_ave * 100);
						break;
					}
					case DC_GR_12V:
					case DC_GR_12V_100mA:
					case DC_GR_12V_1000mA:
					{
                        mysprintf(loop_buf, NULL, 150, vol_ave / 1000 * 100);
						break;
					}
				}
			}
		}
		/* 显示电阻 */
		else if(type_spe.gr_dis_opt & GR_DIS_RES)
		{
            mysprintf(loop_buf, NULL, 151, res_ave);
		}
	}
	else
	{
        if(ERR_NUM == ERR_OPEN)
        {
            strcpy((char*)loop_buf, "> 510");
        }
        else
        {
            mysprintf((uint8_t*)loop_buf, NULL, 151/*"%.1f"*/, (uint32_t)res_ave);
        }
	}
}

void dis_pass(void)
{
    uint8_t flag = 0;
    
    exit_test_relay_motion();/* 复位放电 */
    
	if(steps_pass)
	{
        led_cmd(LED_ST_PASS);
    }
	
	if(g_comm_status == LOCAL_COMM)
	{
		on_keyboard();/* 打开键盘 */
	}
	
    /* 大电容测试放电完成后电压电流都显示0 */
    if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW)
    {
        cur_ave = 0;
        vol_ave = 0;
        transform_test_vol_string();
        transform_test_loop_string();
    }
    
	if(!OFFSET_BBD)
	{
		test_dis();/* 将最后采集的数据显示出来 */
	}
    
	/* 列表显示打开 并且 最后一步测完 */
	if(sys_par.is_table_dis == ENABLE && g_cur_step->next == NULL)
	{
		res_list_dis();
        
		if(g_exit)
		{
			de_reset();
			return;
		}
		
		gui_draw_test();
		g_cur_step = list_99xx.head;
		load_data();
		PASS = 0;
		dis_test_ui(); /* 画出当前步的测试数据 */
		
		/* 返回并停止测试 */
		if(test_flag.g_stop)
		{
			clear_keyboard();
			return;
		}
		
		/* 返回并启动测试 */
		if(test_flag.g_start)
		{
			return;
		}
	}
    
	/* 当下降时间或缓变时间不为零时就将输出电压和电流显示为0 */
    if(g_cur_file->work_mode == G_MODE)
    {
        if(fal_t != 0)
        {
            flag = 1;
        }
    }
    else
    {
        if(fal_t != 0)
        {
            flag = 1;
        }
    }
	
    if(flag != 0)
    {
        GUI_SetFont(&GUI_FontHZ_SimSun_15);
        vol_ave = 0;
        cur_ave = 0;
        
        transform_test_vol_string();
        transform_test_loop_string();
        dis_test_data(&GUI_FontHZ_SimSun_15);
    }
    
    if(cur_mode == DCW && type_spe.dcw_big_cap_en == ENABLE)
    {
        dis_test_status(TEST_DISCHARGE_OVER_INDEX, &GUI_FontHZ_SimSun_16);
        dis_test_result_status(TEST_PASS_INDEX, &GUI_FontHZ_SimSun_16);
    }
    else
    {
        dis_test_status(TEST_PASS_INDEX, &GUI_FontHZ_SimSun_16);
    }
	
	LCD_REFRESH();
}
void dis_fail(void)
{
    exit_test_relay_motion();/* 复位放电 */
    
    LED_ALL = LED_OFF;
	LED_FAIL = LED_ON;
	STOP_COMM = 0;
	cur_status = ST_ERR_FAIL;
	
    plc_signal_cmd(PLC_SIG_FAIL);
    
	/* 蜂鸣器长鸣 */
    if(BUZZER_EN>0)
    {
        BUZZER = BUZZER_ON;
    }
    
    if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW)
    {
        dis_test_status(TEST_DISCHARGE_OVER_INDEX, &GUI_FontHZ_SimSun_16);
    }
    else
    {
        dis_test_status(TEST_FAIL_INDEX, &GUI_FontHZ_SimSun_16);
    }
    
    /* 大电容测试放电完成后电压电流都显示0 */
    if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW)
    {
        cur_ave = 0;
        vol_ave = 0;
        transform_test_vol_string();
        transform_test_loop_string();
    }
    
    test_dis();
    
	LCD_REFRESH();
	/* 列表显示打开 */
	if(sys_par.is_table_dis)
	{
		res_list_dis();
		if(g_exit)
		{
			de_reset();
			return;
		}
		gui_draw_test();
	}
	
//     LED_TEST = LED_OFF;
    
	while(STOP_PIN && !STOP_COMM)
    {
        if(TERMINATE)
        {
            break;
        }
    }
	
	if(g_comm_status == LOCAL_COMM)
	{
		on_keyboard();/* 打开键盘 */
	}
	
    dis_test_status(TEST_WAIT_INDEX, &GUI_FontHZ_SimSun_16);
    
    load_steps_to_list(1);//加载第一步 2017.5.20 
	g_cur_step = list_99xx.head;
	load_data();
	dis_test_ui(); /* 画出当前步的测试数据 */
	
	FAIL = 0;
	LED_FAIL = LED_OFF;
	
    if(BUZZER_EN>0)
    {
        BUZZER = BUZZER_OFF;
    }
	
	g_cur_step = list_99xx.head;
}
void dis_discharge_over(void)
{
    dis_test_status(TEST_DISCHARGE_OVER_INDEX, &GUI_FontHZ_SimSun_16);
    
    vol_ave = 0;
    cur_ave = 0;
    
    transform_test_vol_string();
    transform_test_loop_string();
    dis_test_data(&GUI_FontHZ_SimSun_15);
    
    LCD_REFRESH();
    
    if(CUR_FAIL && TERMINATE == 0)
    {
        while(1)
        {
            if(STOP_PIN == RESET)
            {
                test_reset();
                return;
            }
        }
    }
    
	if(g_comm_status == LOCAL_COMM)
	{
		on_keyboard();/* 打开键盘 */
	}
}

void dis_stop(void)
{
	serve_count(KEY_STOP_COUNT);
	STOP = 0;
	PASS = 0;
	TEST = 0;
	LED_TEST = 0;
	BUZZER = BUZZER_OFF;
	
	g_return = 0;
	
	stop_test();
    plc_signal_cmd(PLC_SIG_RESET);/* plc复位 */
    test_reset();/* 进入测试就绪状态 */
    
    if(sys_par.fail_mode != FAIL_MODE_HALT || cur_step == g_cur_file->total)
    {
        load_steps_to_list(1);
        g_cur_step = list_99xx.head;
        load_data();
    }
    
	test_flag.gradation = STAGE_READY;
	
	if(g_comm_status == LOCAL_COMM)
	{
		on_keyboard();/* 打开键盘 */
	}
	
    dis_test_status(TEST_WAIT_INDEX, &GUI_FontHZ_SimSun_16);
    
    /* 大电容测试放电完成后电压电流都显示0 */
    if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW)
    {
        cur_ave = 0;
        vol_ave = 0;
        transform_test_vol_string();
        transform_test_loop_string();
    }
    
	dis_test_ui(); /* 画出当前步的测试数据 */
	
	LCD_REFRESH();
}

void stop_server_for_comm(void)
{
	g_dis_time = tes_t;
	vol_ave = cur_vol;
	cur_ave = cur_high;
	
	if(cur_mode == GR)
	{
		cur_ave_comm = cur_vol;
		res_ave_comm = cur_high;
	}
    else if(cur_mode == IR)
	{
		res_ave_comm = cur_low;
		
		if(res_ave_comm < 10)
		{
			res_ave_comm *= 1000;
		}
		else if(res_ave_comm < 100)
		{
			res_ave_comm *= 100;
		}
		else if(res_ave_comm < 1000)
		{
			res_ave_comm *= 10;
		}
		else if(res_ave_comm < 10*1000)
		{
			res_ave_comm *= 1;
		}
		else if(res_ave_comm < 100*1000)
		{
			res_ave_comm /= 10;
		}
		else
		{
			res_ave_comm /= 100;
		}
	}
	
	cur_status = ST_WAIT;
}

int32_t dis_charge_remain_vol(void)
{
    uint32_t temp_delay = 0;
    int32_t res = 0;
    
    g_test_time = 0;
    test_flag.dis_status = TEST_DISCHARGE_INDEX;/* 当前的显示状态发电 */
    
    test_flag.vol_change_flag = 1;/* 数据显示不限速 */
    set_sample_rate(20);
    
    on_sample_task();
    
    if(vol_ave < 50)
    {
        return 0;
    }
    
    CUR_FAIL = 0;
    TERMINATE = 0;
    cur_mode = DCW;
    off_schedule_task();
    open_test_timer();/* 开定时器 */
    resume_sample_task();
    
    while(1)
    {
        /* 计算显示值 */
        if(test_flag.sample_task_en)
        {
            count_dis_value();
        }
        
        g_dis_time = g_test_time;
        
        test_status_dis();/* 显示测试状态 */
        
        test_dis();/* 测试细节 */
        
        if(STOP_PIN == RESET)
        {
            res = -1;
        }
        
        OSTimeDlyHMSM(0,0,0,10);
        
        dcw_big_cap_execute_dis_charge();
        
        updata_time(U_TEST_TIME, g_dis_time);/* 更新时间 */
        
        if(test_flag.dis_charge_end == 1)
        {
            if(++temp_delay > 5)
            {
                disable_sample_task();
                close_test_timer();
                dis_discharge_over();
                return res;
            }
        }
    }
}


void cur_cylinder_ctrl_over(void)
{
//    CYLINDER_CTRL(2000);
}

void clear_test_falg(void)
{
    memset((void*)&test_flag, 0, sizeof test_flag);/* 清空标志位 */
}

static void g_mode_next_step(void)
{
    save_cur_result(&cur_result);
    test_pass();
    
    load_steps_to_list(g_cur_step->one_step.com.step);
    g_cur_step = list_99xx.head->next;/* 进入下一步 */
    
    /* 画出当前步的测试数据 */
    load_data();
    dis_test_ui_x(); /* 画出当前步的测试数据 */
    save_result(&cur_result, INIT_RESULT);/* 初始化保存步 */
    test_flag.save_only_one = 0;/* 清除保存一次标志 */
    
    LED_PASS = LED_OFF;/* 关闭LED灯 */
}

void re_draw_test_ui(void)
{
    init_test_ui_info(0);
    
    if(type_spe.dcw_big_cap_en == ENABLE)
    {
        if(cur_mode == DCW)
        {
            dcw_big_cap_output_Impedan_sel(cur_output_impedan);
        }
    }
    
	clear_menu();
	update_info();
	
	cur_status = ST_WAIT;
	
    if(cur_mode == DCW)
    if(type_spe.dcw_big_cap_en == ENABLE)
    {
        set_sample_rate(50);/* 设置采样率 */
        on_sample_task();
        off_schedule_task();
    }
}

/*
 * 函数名：transform_test_time_string
 * 描述  ：将测试时间转换为字符串
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
// void transform_test_time_string(uint16_t time)
// {
//     mysprintf((uint8_t*)time_buf, NULL, 1 + 150, time);
// }
/*
 * 函数名：into_output_delay
 * 描述  ：输出延时
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
static int32_t into_output_delay(void)
{
    LED_ALL = LED_OFF;
    
    while(1)
    {
        test_status_dis();
        g_dis_time = g_test_time;
        transform_test_time_string(g_dis_time);
        dis_test_time(&GUI_FontHZ_SimSun_15);
        LCD_REFRESH();/* 刷新屏幕 */
        
        if(STOP_PIN == RESET)
        {
            return -1;
        }
        
        if(g_dis_time > dly_t)
        {
            return 0;
        }
    }
}
enum{
    TEST_NULL,/* 空闲 */
    TEST_RE_DRAW,/* 重绘界面 */
    TEST_PREV_STEP,
    TEST_NEXT_STEP,
    TEST_SHORTCUT,/* 快捷键 */
    TEST_RESET_UI,
    TEST_STOP_UI,
    TEST_INTO_GETC,/* 进入获取容值界面，并自动进行测试 */
    TEST_INTO_GETO,/* 进入获取偏移界面，并自动进行测试 */
    
    TEST_NULL_CONTROL,/* 空闲 */
    TEST_NEXT_CONTROL,
    TEST_PASS_CONTROL,
    TEST_STOP_CONTROL,
    TEST_CON_CONTROL,
    TEST_START_CONTROL,/* 按下启动键测试开始 */
    TEST_TESTING_CONTROL,/* 正在测试 */
    TEST_EXCEPTION_CONTROL,
    TEST_QUIT_CONTROL,
    TEST_FAIL_RE_START_CONTROL,
    TEST_CONT_TEST_CONTROL,
    
};

enum{
EXIT_SHORT,
EXIT_ARC,
EXIT_STOP,
EXIT_GFI,
EXIT_RTC,
};
/*
 * 函数名：app_ctrl_exit_sw
 * 描述  ：应用层 用于控制外部中断开关
 * 输入  ：witch 指定的外部中断
 *         st  中断状态 ENABLE DISABLE
 * 输出  ：无
 * 返回  ：无
 */
void app_ctrl_exit_sw(uint8_t witch, uint8_t st)
{
    switch(witch)
    {
        case EXIT_SHORT:
            SHORT_INT(st);
            break;
        case EXIT_ARC:
            ARC_INT(st);
            break;
        case EXIT_STOP:
            STOP_INT(st);
            break;
        case EXIT_GFI:
            GFI_INT(st);
            break;
        case EXIT_RTC:
            RTC_INI(st);
            break;
    }
}

void uninstall_test_irq_fun(void)
{
	test_irq_fun = NULL;
	cs99xx_test_fun = NULL;
}
#include "acw_test.h"
void install_run_test_fun(void)
{
    run_cs99xx_test_fun = NULL;
    
    switch(cur_mode)
    {
        case ACW:
            /* 当是G模式时 */
            if(g_cur_file->work_mode == G_MODE)
            {
                run_cs99xx_test_fun = run_acw_test_g;
            }
            /* N模式 */
            else
            {
                run_cs99xx_test_fun = run_acw_test;
            }
            break;
        case DCW:
            break;
        case IR:
            break;
        case GR:
            break;
    }
}

void get_test_dis_status(TEST_DATA_STRUCT *test_data)
{
    switch(test_data->test_status)
    {
        case ST_CHANGE:
            test_flag.dis_status = TEST_CHANGE_INDEX;
            break;
        case ST_CHARGE:
            test_flag.dis_status = TEST_CHARGE_INDEX;
            break;
        case ST_STAB:
            test_flag.dis_status = TEST_STAB_INDEX;
            break;
        case ST_DISCHARGE:
            test_flag.dis_status = TEST_DISCHARGE_INDEX;
            break;
        case ST_DISCHARGE_OVER:
            test_flag.dis_status = TEST_DISCHARGE_OVER_INDEX;
            break;
        case ST_VOL_RISE:
            test_flag.dis_status = TEST_RISE_INDEX;
            break;
        case ST_TESTING:
            test_flag.dis_status = TEST_TEST_INDEX;
            break;
        case ST_VOL_FALL:
            test_flag.dis_status = TEST_FALL_INDEX;
            break;
        case ST_INTER_WAIT:
            test_flag.dis_status = TEST_INTER_INDEX;
            break;
        case ST_WAIT:
            test_flag.dis_status = TEST_WAIT_INDEX;
            break;
        case ST_PASS:
            test_flag.dis_status = TEST_PASS_INDEX;
            break;
        case ST_STOP:
            test_flag.dis_status = TEST_WAIT_INDEX;
            break;
        case ST_ERR_FAIL:
            test_flag.dis_status = TEST_FAIL_INDEX;
            break;
        case ST_OUTPUT_DELAY:
            test_flag.dis_status = TEST_OUTPUT_DELAY;
            break;
        
        case ST_ERR_H:
            test_flag.dis_status = ERR_HIGH;
            break;
        case ST_ERR_L:
            test_flag.dis_status = ERR_LOW;
            break;
        case ST_ERR_SHORT:
            test_flag.dis_status = ERR_SHORT;
            break;
        case ST_ERR_VOL_ABNORMAL:
            test_flag.dis_status = ERR_VOL;
            break;
        case ST_ERR_ARC:
            test_flag.dis_status = ERR_ARC;
            break;
        case ST_ERR_GFI:
            test_flag.dis_status = ERR_GFI;
            break;
        case ST_ERR_REAL:
            test_flag.dis_status = ERR_REAL;
            break;
        case ST_ERR_CHAR:
            test_flag.dis_status = ERR_CHARGE;
            break;
        case ST_ERR_GEAR:
            test_flag.dis_status = ERR_GEAR;
            break;
        case ST_ERR_AMP:
            test_flag.dis_status = ERR_AMP;
            break;
        case ST_ERR_OPEN:
            test_flag.dis_status = ERR_OPEN;
            break;
    }
}
void testing_process_control(uint8_t *st)
{
    uint8_t state = 0;
    
    if(st == NULL)
    {
        return;
    }
    
    state = *st;
    *st = TEST_NULL_CONTROL;
    
    /* 退出状态机 */
    if(STOP)
    {
        *st = TEST_STOP_CONTROL;//TEST_QUIT_CONTROL;
    }
    
    switch(state)
    {
        case TEST_START_CONTROL:/* 启动控制 */
        {
			cur_status = ST_TESTING;
            serve_count(KEY_START_COUNT);/* 功放计数 */
            
            /* 如果下一步标记置位就加载新的测试步 */
            if(NEXT)
            {
                /* 如果当前步不是最后一步 就加载下一步 */
                if(g_cur_step->next != NULL)
                {
                    load_steps_to_list(g_cur_step->next->one_step.com.step);
                    g_cur_step = list_99xx.head;
                }
                /* 否则就加载第一步 */
                else
                {
                    load_steps_to_list(1);
                    g_cur_step = list_99xx.head;
                }
            }
            
            /* 当是G模式时每次测试只能从第一步开始 */
            if(g_cur_file->work_mode == G_MODE)
            {
                load_steps_to_list(1);
                g_cur_step = list_99xx.head;
            }
            
            /* 画出当前步的测试数据 */
            load_data();
            dis_test_ui(); /* 画出当前步的测试数据 */
            
            /* 如果是大电容测试需要先进行放电 */
            if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW)
            {
                /* 如果放电过程中按下了复位键就要停止测试 */
                if(-1 == dis_charge_remain_vol())
                {
                   *st = TEST_QUIT_CONTROL;
                    return;
                }
            }
            
            LCD_REFRESH();
            
			uninstall_test_irq_fun();/* 卸载测试状态机 */
            /* 当前步是第一步并且输出延时时间不为零 */
            if(cur_step == 1 && sys_par.output_delay > 0)
            {
                test_flag.dis_status = TEST_OUTPUT_DELAY;
                g_test_time = 0;
                open_test_timer();/* 开定时器 */
                cur_status = ST_OUTPUT_DELAY;
				plc_signal_cmd(PLC_SIG_TESTING);
                
                if(-1 == into_output_delay())/* 进入输出延时 */
				{
					*st = TEST_QUIT_CONTROL;
                    cur_status = ST_WAIT;
                    close_test_timer();/* 关定时器 */
                    plc_signal_cmd(PLC_SIG_RESET);
					return;
				}
                close_test_timer();/* 关定时器 */
            }
            
            install_test_irq_fun();/* 安装测试状态机 */
            install_run_test_fun();
            
            /* 检查数据如果为空就退出 */
            if(cs99xx_test_fun == NULL)
            {
                return;
            }
            
            if(run_cs99xx_test_fun == NULL)
            {
                return;
            }
            
            startup();/* 启动测试 */
            serve_count(AMP_COUNT);/* 功放继电器吸合次数更新 */
            
            g_test_data.gradation = STAGE_READY;
            g_test_data.test_time = 0;
            g_test_data.ready_ok = 0;//标记
            g_test_data.fail_num = ST_ERR_NONE;//初始化
            *st = TEST_TESTING_CONTROL;
            break;
        }
        case TEST_TESTING_CONTROL:/* 测试控制 */
        {
            run_cs99xx_test_fun(g_cur_step, g_cur_step->next, &g_test_data);
            cur_status = g_test_data.test_status;
            get_test_dis_status(&g_test_data);
            
            /* 显示测试状态 */
            test_status_dis();
            
            /* 显示测试数据 */
            update_test_data(&g_test_data);
            
            /* 更新时间 */
            updata_time(U_TEST_TIME, g_test_data.dis_time);
            
            /* 测试过程出现异常 */
            if(g_test_data.fail_num != ST_ERR_NONE)
            {
                exception_handling(g_test_data.fail_num);
                g_test_data.fail_num = ST_ERR_NONE;
                g_test_data.test_over = 0;
                
                /* 满足失败继续条件，继续测试 */
                if(g_test_data.gradation == STAGE_FAIL_CONT)
                {
                    *st = TEST_TESTING_CONTROL;
                    return;
                }
                
                *st = TEST_QUIT_CONTROL;
                return;
            }
            
            /* 复位打断测试 */
            if(TERMINATE)
            {
                *st = TEST_QUIT_CONTROL;
                return;
            }
            
            /* 测试结束 */
            if(g_test_data.test_over == 1)
            {
                /* 测试合格 */
                if(g_test_data.fail_num == ST_ERR_NONE)
                {
                    *st = TEST_PASS_CONTROL;
                    return;
                }
                /* 测试失败 */
                else
                {
                    *st = TEST_EXCEPTION_CONTROL;
                    return;
                }
            }
            /* 继续下一步测试 */
            else if(g_test_data.cont == 1)
            {
                *st = TEST_CONT_TEST_CONTROL;
                return;
            }
            
            /* 在间隔等待阶段，要判断是否要跑间隔等待 */
            if(g_test_data.test_status == ST_INTER_WAIT)
            {
                if(sys_par.fail_mode == FAIL_MODE_FPDFC && cur_step == 1)
                {
                    if(g_test_data.fail_num == ERR_NONE)
                    {
                        *st = TEST_PASS_CONTROL;
                        return;
                    }
                }
                /* 测试到最后一步并且测试过程中发生过异常 */
                else if(FAIL && g_cur_step->next == NULL)
                {
                     *st = TEST_EXCEPTION_CONTROL;
                    return;
                }
                
                test_pass();
                save_cur_result(&cur_result);
            }
            
            /* 断续测试 */
            *st = TEST_TESTING_CONTROL;
            break;
        }
        case TEST_FAIL_RE_START_CONTROL:/* 测试失败重启控制 */
        {
            /* 失败重启 */
            switch(FAIL_RE_START)
            {
                /* 重启从第一步开始 */
                case FAIL_RE_START_FIRST:
                {
                    load_steps_to_list(1);
                    g_cur_step = list_99xx.head;
                    clear_test_falg();/* 清空标志位 */
                    *st = TEST_START_CONTROL;
                    break;
                }
                /* 重启从下一步开始 */
                case FAIL_RE_START_NEXT:
                {
                    NEXT = 1;
                    *st = TEST_START_CONTROL;
                    break;
                }
                default:
                {
                    *st = TEST_QUIT_CONTROL;
                    break;
                }
            }
            break;
        }
        case TEST_EXCEPTION_CONTROL:/* 异常处理 */
        {
            save_cur_result(&cur_result);
            
            /* 大电容测试等待电容发电完成 */
            if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW
                && test_flag.dis_charge_end == 0)
            {
                dis_charge_remain_vol();
            }
            
            app_ctrl_exit_sw(EXIT_STOP, DISABLE);/* 关闭复位中断 */
            dis_fail();
            clear_keyboard();
            *st = TEST_QUIT_CONTROL;
//             else if(CONT)
//             {
//                 CONT = 0;
//                 *st = TEST_TESTING_CONTROL;
//             }
            break;
        }
        case TEST_PASS_CONTROL:
        {
            test_pass();
            save_cur_result(&cur_result);
            cur_status = ST_PASS;/* 当前状态为pass */
            dis_pass();
            
            NEXT = 1;
            
            if(sys_par.fail_mode == FAIL_MODE_FPDFC && cur_step == 1)
            {
                NEXT = 0;
            }
            
            /* 退出测试 */
            if(test_flag.g_stop)
            {
                dis_stop();
                LCD_REFRESH();
                app_ctrl_exit_sw(EXIT_STOP, DISABLE);	/* 关闭复位中断 */
                clear_keyboard();
                *st = TEST_QUIT_CONTROL;
            }
            /* 启动测试 */
            else if(test_flag.g_start)
            {
                clear_keyboard();
                *st = TEST_START_CONTROL;
            }
			/* 退出测试 */
            else
            {
//                 dis_stop();
//                 LCD_REFRESH();
//                 app_ctrl_exit_sw(EXIT_STOP, DISABLE);	/* 关闭复位中断 */
//                 clear_keyboard();
                *st = TEST_QUIT_CONTROL;
            }
            break;
        }
        case TEST_STOP_CONTROL:
        {
            plc_signal_cmd(PLC_SIG_RESET);
            
            /* 大电容测试等待电容发电完成 */
            if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW
                && test_flag.dis_charge_end == 0)
            {
                STOP = 0;
                
				disable_stop_exit();/* 关闭复位中断 */
                g_test_time = for_t + 1;
                on_sample_task();
                open_test_timer();/* 开定时器 */
                enable_stop_exit();
                ERR_NUM = ERR_NONE;
                test_flag.judge_err_en = 0;
                *st = TEST_TESTING_CONTROL;
            }
            /* 正常测试停止 */
            else
            {
                app_ctrl_exit_sw(EXIT_STOP, DISABLE);	/* 关闭复位中断 */
                stop_server_for_comm();
                dis_stop();
                clear_keyboard();
                TERMINATE = 0;
                *st = TEST_QUIT_CONTROL;/* 退出测试 */
            }
            break;
        }
        case TEST_CONT_TEST_CONTROL:
        {
            /* 允许继续测试下一步 G模式 没有停止测试 */
            if(g_test_data.cont == 1 && g_cur_file->work_mode == G_MODE && !STOP)
            {
                g_test_data.cont = 0;
                g_mode_next_step();
                *st = TEST_TESTING_CONTROL;
            }
            /* 允许继续测试下一步 N模式 没有停止测试 */
            else if(g_test_data.cont && g_cur_file->work_mode == N_MODE && !STOP)
            {
                g_test_data.cont = 0;
                save_cur_result(&cur_result);
                stop_test();/* 停止测试 */
                
                /* 当前步不是最后一步 */
                if(g_cur_step->next != NULL)
                {
                    /* 当前步没有发生异常 */
                    if(!CUR_FAIL)
                    {
                        test_pass();
                    }
                    
                    /* 加载下一步 */
                    load_steps_to_list(g_cur_step->one_step.com.step + 1);
                    g_cur_step = list_99xx.head;
                }
                /* 当前步是最后一步 */
                else
                {
                    /* 间隔时间为0 */
                    if(int_t == 0)
                    {
                        /* 当前步没有发生异常 结束测试退出 */
                        if(!CUR_FAIL)
                        {
                            PASS = 1;/* 当不再进行测试时就将pass标志写1 */
                            *st = TEST_PASS_CONTROL;
                            break;
                        }
                    }
                    /* 加载第一步 */
                    load_steps_to_list(1);
                    g_cur_step = list_99xx.head;
                }
                
                load_data();/* 加载数据 */
                update_info();/* 更新界面 */
                
                OSTimeDlyHMSM(0,0,0,5);/* 延时 */
                
                /* 如果按下了复位键就停止测试 */
                if(TERMINATE)
                {
                    *st = TEST_STOP_CONTROL;
                }
                /* 启动测试 */
                else
                {
                    *st = TEST_START_CONTROL;
                }
            }
            break;
        }
    }
}
/*
 * 函数名：testing_gui_control
 * 描述  ：测试程序界面控制
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void testing_gui_control(uint8_t *st)
{
    uint8_t state = 0;
    
    if(st == NULL)
    {
        return;
    }
    
    state = *st;
    *st = TEST_NULL;
    
    switch(state)
    {
        case TEST_RE_DRAW:
        {
            re_draw_test_ui();
            break;
        }
        
        case TEST_INTO_GETO:
        {
            sys_offset(0);
            OFFSET_BBD =TEST_NORMAL;
            app_flag.into_get_offset = 0;
            *st = TEST_RE_DRAW;
            clear_test_falg();/* 清空标志位 */
            load_steps_to_list(1);
            g_cur_step = list_99xx.head;
            break;
        }
        case TEST_INTO_GETC:
        {
            if(type_spe.mode_en & __BBD)
            {
                sys_bbd(0);
                OFFSET_BBD = TEST_NORMAL;
                app_flag.into_get_cap = 0;
                *st = TEST_RE_DRAW;
                clear_test_falg();/* 清空标志位 */
                load_steps_to_list(1);
                g_cur_step = list_99xx.head;
            }
            break;
        }
        case TEST_PREV_STEP:
        {
            clear_test_falg();/* 清空标志位 */
            
            if(g_cur_step->one_step.com.step > 1)
            {
                load_steps_to_list(g_cur_step->one_step.com.step - 1);
                g_cur_step = list_99xx.head;
            }
            else
            {
                load_steps_to_list(g_cur_file->total);
                g_cur_step = list_99xx.head;
            }
            
            update_info();
            break;
        }
        case TEST_NEXT_STEP:
        {
            clear_test_falg();/* 清空标志位 */
            
            if(g_cur_step->next != NULL)
            {
                load_steps_to_list(g_cur_step->next->one_step.com.step);
                g_cur_step = list_99xx.head;
            }
            else
            {
                load_steps_to_list(1);
                g_cur_step = list_99xx.head;
            }
            
            update_info();
            break;
        }
        case TEST_SHORTCUT:
        {
            clear_test_falg();/* 清空标志位 */
            de_reset();
			plc_signal_cmd(PLC_SIG_EXIT);/* PLC状态更新为未就绪 */
            serve_test_shortcut();
            
            if(g_exit)
            {
                return;
            }
            
            if(g_return)
            {
                g_return = 0;
                test_reset();
                *st = TEST_RE_DRAW;
                break;
            }
            
            test_reset();
            
            /* 大电容测试输出阻抗设置 */
            if(type_spe.dcw_big_cap_en == ENABLE)
            {
                if(cur_mode == DCW)
                {
                    dcw_big_cap_output_Impedan_sel(cur_output_impedan);
                }
            }
            
            *st = TEST_RE_DRAW;
            break;
        }
        case TEST_RESET_UI:
        {
            if(sys_par.fail_mode != FAIL_MODE_HALT || cur_step == g_cur_file->total)
            {
                clear_test_falg();/* 清空标志位 */
            }
            
			dis_stop();
            LED_ALL = LED_OFF;
			LCD_REFRESH();
            break;
        }
        case TEST_STOP_UI:
        {
			stop_server_for_comm();
			dis_stop();
            break;
        }
        default:
        {
            off_schedule_task();/* 关闭调度器 */
            break;
        }
    }
}
/*
 * 函数名:test_running
 * 描述  :
 * 输入  :无
 * 输出  :无
 * 返回  :无
 */
void test_running(void)
{
    uint8_t test_state = TEST_START_CONTROL;
    
    off_schedule_task();
    
    while(1)
    {
        testing_process_control(&test_state);
        OSTimeDlyHMSM(0,0,0,5);/* 把运行权交出 */
        
        /* 退出测试 */
        if(TEST_QUIT_CONTROL == test_state)
        {
            break;
        }
    }
}
enum{
    GENERAL_TEST_UI,/* 普通测试界面 */
    OFFSEET_TEST_UI,/* 偏移测试界面 */
};
/*
 * 函数名:serve_test
 * 描述  :
 * 输入  :无
 * 输出  :无
 * 返回  :无
 */
void serve_test(void)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
    uint8_t test_state = TEST_RE_DRAW;
    
    if(g_comm_status != LOCAL_COMM)
    {
        off_keyboard();/*远程控制 关闭键盘 */
    }
	
    init_test_ui_info(GENERAL_TEST_UI);/* 初始化测试界面数据信息 */
    load_steps_to_list(1);/* 加载第一步到链表中 */
    g_cur_step = list_99xx.head;
    load_data();/* 加载第一步 */
	
	install_test_irq_fun();/* 安装测试状态机 */
	exit_off();
    clear_test_falg();/* 清空标志位 */
	test_reset();/* 切换继电器 */
    
	clear_();/* 清屏 */
    
	while(1)
	{
		/* 测试界面控制 */
        testing_gui_control(&test_state);
        
        if(app_flag.into_get_cap == 1)
        {
            app_flag.into_get_cap = 2;
            test_state = TEST_INTO_GETC;
        }
        
        if(app_flag.into_get_offset == 1)
        {
            app_flag.into_get_offset = 2;
            test_state = TEST_INTO_GETO;
        }
        
        /* 退到主界面 */
		if(g_exit)
		{
			goto l_exit;
		}
        /* 下级菜单返回来需要重绘界面 */
		if(g_return)
		{
			g_return = 0;
			test_state = TEST_RE_DRAW;
		}
		/* 测试停止 显示等待测试界面 */
		if(STOP)
		{
            test_state = TEST_STOP_UI;
		}
		/* 按下复位键后 显示等待测试见面 可能由通信控制 */
		if(RESET == STOP_PIN || TERMINATE/* && CUR_OVER*/)
		{
            test_state = TEST_RESET_UI;
		}
        
        /* 上位机更新了数据需要刷新显示 */
		if(test_flag.refresh_for_comm == 1)
		{
			test_flag.refresh_for_comm = 0;
			test_state = TEST_RE_DRAW;
		}
        
        /* 从按键缓冲取键值 */
		pkey_value = OSQPend(KeyboardQSem,2,&err);
        
        if(pkey_value == NULL)
        {
            continue;
        }
        
		switch(*pkey_value)
		{
			case KEY_UP:
                test_state = TEST_PREV_STEP;
				break;
			case KEY_DOWN:
                test_state = TEST_NEXT_STEP;
				break;
			case KEY_SHORTCUT:
                test_state = TEST_SHORTCUT;
				break;
			case KEY_BACK:
			case KEY_EXIT:
				de_reset();
				g_exit = 1;
				break;
            case KEY_STOP:
                cur_cylinder_ctrl_stop();
                break;
			case KEY_START:
				if(STOP_PIN == RESET)
				{
					break;
				}
                
                test_running();/* 进入测试程序 */
				break;
		}//end switch()
	}
	
l_exit:
	plc_signal_cmd(PLC_SIG_EXIT);
}
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
