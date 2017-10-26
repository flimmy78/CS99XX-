/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx.c
 * 摘  要  ：服务于整个测试程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

/****************** 包含头文件 **************************/
#include "cs99xx_config.h"


#include	"SPI_CPLD.h"
#include    "cs99xx_result.h"
#include 	"serve_test.h"
#include	"cs99xx_collect.h"
#include    "cs99xx_type.h"
#include    "cs99xx_relay_motion.h"
#include    <string.h>

/************************* 初始化全局标志 *************************/

/************************* 函数定义 *************************/


void dis_test_status(uint8_t status, const GUI_FONT * pNewFont)
{
    GUI_SetFont(pNewFont);
    
	GUI_ClearRect(test_ui_ele_set[TEST_ELE_5].x,
                  test_ui_ele_set[TEST_ELE_5].y,
                  test_ui_ele_set[TEST_ELE_5].u_x,// + test_ui_ele_set[TEST_ELE_5].cw,
                  test_ui_ele_set[TEST_ELE_5].y
                + test_ui_ele_set[TEST_ELE_5].o_y);
    
    GUI_DispStringAt_BTP_STR_(status_str[status][!sys_par.language],
                    test_ui_ele_set[TEST_ELE_5].x,
                    test_ui_ele_set[TEST_ELE_5].u_x,
                    test_ui_ele_set[TEST_ELE_5].y,
                    test_ui_ele_set[TEST_ELE_5].cw);
}
void dis_test_result_status(uint8_t status, const GUI_FONT * pNewFont)
{
    GUI_SetFont(pNewFont);
    
	GUI_ClearRect(test_ui_ele_set[TEST_ELE_6].x,
                  test_ui_ele_set[TEST_ELE_6].y,
                  test_ui_ele_set[TEST_ELE_6].u_x,
                  test_ui_ele_set[TEST_ELE_6].y
                + test_ui_ele_set[TEST_ELE_6].o_y);
    
    GUI_DispStringAt_BTP_STR_(status_str[status][!sys_par.language],
                    test_ui_ele_set[TEST_ELE_6].x,
                    test_ui_ele_set[TEST_ELE_6].u_x,
                    test_ui_ele_set[TEST_ELE_6].y,
                    test_ui_ele_set[TEST_ELE_6].cw);
}

void dis_test_vol1(const GUI_FONT * pNewFont)
{
    uint8_t buf[10] = {0};
    uint8_t CW = test_ui_ele_set[TEST_ELE_1].cw;
    
    GUI_SetFont(pNewFont);
    
    strcpy((char*)buf, div_str_pre_zero((char*)test_ui_ele_set[TEST_ELE_1].str));
    
	GUI_ClearRect(test_ui_ele_set[TEST_ELE_1].x,
                  test_ui_ele_set[TEST_ELE_1].y,
                  test_ui_ele_set[TEST_ELE_1].u_x,
                  test_ui_ele_set[TEST_ELE_1].y
                + test_ui_ele_set[TEST_ELE_1].o_y);
    
    GUI_DispStringAt((const char*)(const char*)buf,
                    test_ui_ele_set[TEST_ELE_1].x + (5 - strlen((const char*)buf)) * CW,
                    test_ui_ele_set[TEST_ELE_1].y);
}

void dis_test_time(const GUI_FONT * pNewFont)
{
    uint8_t buf[10] = {0};
    uint8_t CW = test_ui_ele_set[TEST_ELE_4].cw;
    
    GUI_SetFont(pNewFont);
    
    strcpy((char*)buf, div_str_pre_zero((char*)test_ui_ele_set[TEST_ELE_4].str));
    
	GUI_ClearRect(test_ui_ele_set[TEST_ELE_4].x,
                  test_ui_ele_set[TEST_ELE_4].y,
                  test_ui_ele_set[TEST_ELE_4].u_x - 1,
                  test_ui_ele_set[TEST_ELE_4].y
                + test_ui_ele_set[TEST_ELE_4].o_y);
    
    GUI_DispStringAt((const char*)(const char*)buf,
                    test_ui_ele_set[TEST_ELE_4].x + (5 - strlen((const char*)buf)) * CW,
                    test_ui_ele_set[TEST_ELE_4].y);
}
#include "dc_module.h"
void dis_test_data(const GUI_FONT * pNewFont)
{
    uint8_t buf[10] = {0};
    uint8_t CW = 9;
    
    GUI_SetFont(pNewFont);
    
    if(pNewFont == &GUI_FontHZ_SimSun_12)
    {
        CW = 6;
    }
    
	GUI_ClearRect(test_ui_ele_set[TEST_ELE_1].x,
                  test_ui_ele_set[TEST_ELE_1].y,
                  test_ui_ele_set[TEST_ELE_1].u_x - 1,
                  test_ui_ele_set[TEST_ELE_1].y
                + test_ui_ele_set[TEST_ELE_1].o_y);
    
	GUI_ClearRect(test_ui_ele_set[TEST_ELE_2].x,
                  test_ui_ele_set[TEST_ELE_2].y,
                  test_ui_ele_set[TEST_ELE_2].u_x - 1,
                  test_ui_ele_set[TEST_ELE_2].y
                + test_ui_ele_set[TEST_ELE_2].o_y);
    
	GUI_ClearRect(test_ui_ele_set[TEST_ELE_3].x,
                  test_ui_ele_set[TEST_ELE_3].y,
                  test_ui_ele_set[TEST_ELE_3].u_x - 1,
                  test_ui_ele_set[TEST_ELE_3].y
                + test_ui_ele_set[TEST_ELE_3].o_y);
    
    
    strcpy((char*)buf, div_str_pre_zero((char*)test_ui_ele_set[TEST_ELE_1].str));
    
    if(cur_mode != CC)
    GUI_DispStringAt((const char*)(const char*)buf,
                    test_ui_ele_set[TEST_ELE_1].x + (5 - strlen((const char*)buf)) * CW,
                    test_ui_ele_set[TEST_ELE_1].y);
    
    strcpy((char*)buf, div_str_pre_zero((char*)test_ui_ele_set[TEST_ELE_2].str));
    
    GUI_DispStringAt((const char*)(const char*)buf,
                    test_ui_ele_set[TEST_ELE_2].x + (5 - strlen((const char*)buf)) * CW,
                    test_ui_ele_set[TEST_ELE_2].y);
    
    strcpy((char*)buf, div_str_pre_zero((char*)test_ui_ele_set[TEST_ELE_3].str));
	
    GUI_DispStringAt((const char*)(const char*)buf,
                    test_ui_ele_set[TEST_ELE_3].x + (5 - strlen((const char*)buf)) * CW,
                    test_ui_ele_set[TEST_ELE_3].y);
}

/*
 * 函数名：updata_time
 * 描述  ：更新测试显示时间
 * 输入  ：n 标记当前是偏移还是测试 0 测试 1偏移
 * 输出  ：无
 * 返回  ：无
 */
void updata_time(const int8_t n, uint16_t time)
{
    transform_test_time_string(time);
    
	switch(n)
	{
        /* 测试界面 */
		case U_TEST_TIME:
            dis_test_time(&GUI_FontHZ_SimSun_15);
			break;
        /* 偏移界面 */
		case U_OFFSET_TIME:
            dis_test_time(&GUI_FontHZ_SimSun_12);
			break;
	}
}

/*
 * 函数名：test_dis
 * 描述  ：服务于测试程序
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void test_dis(void)
{
    /* IR自动换挡 */
    if(cur_mode == IR)
    {
        ir_auto_find_gear();
    }
    
    /* 更新结果 */
    if(test_flag.gradation == STAGE_TEST)
    {
        if(tes_t && ((g_dis_time + 5) > tes_t))
        {
            updata_result(cur_mode);
        }
    }
    
    /* 显示限速 */
    if(test_flag.vol_change_flag == 0)
    {
        /* 在刚开始的时候要快速显示数据不能限速 */
        if(DIS_SPEED < 20)
        {
            DIS_SPEED++;
        }
        /* 控制显示的速度 */
        else
        {
            if(++DIS_SPEED > 38)
            {
                DIS_SPEED = 28;
            }
            else
            {
                return;
            }
        }
    }
    
	test_flag.re_times_per_second++;
    
    test_unit_dis(cur_mode, cur_gear, &GUI_FontHZ_SimSunF_15);
    transform_test_vol_string();
    transform_test_loop_string();
    dis_test_data(&GUI_FontHZ_SimSun_15);
    
	LCD_REFRESH();/* 刷新屏幕 */
    
	if(ERR_NUM)
	{
        disable_sample_task();/* 关闭采样任务 */
    }
	
	return;
}

/*
 * 函数名：offset_dis
 * 描述  ：服务于偏移测试
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
int8_t offset_dis(const int8_t n)
{
	int8_t err_num = 0;
    
    if(OFFSET_BBD == TEST_BBD)
    {
        bbd_auto_select_gear();
    }
    
    transform_test_vol_string();
    transform_test_loop_string();
    dis_test_data(&GUI_FontHZ_SimSun_12);
    
	LCD_REFRESH();
	
	return err_num;
}

void dis_test_ui(void)
{
    /* 显示单位 */
    test_unit_dis(cur_mode, cur_gear, &GUI_FontHZ_SimSunF_15);
    /* 多路端口显示 */
	dis_port();
    /* 标题栏 */
    updata_title_step(&g_cur_step->one_step);
    /* 设置数据显示 */
    dis_test_data(&GUI_FontHZ_SimSun_15);
    /* 设置测试时间显示 */
    updata_time(U_TEST_TIME, tes_t);
	/* 刷屏 */
	LCD_REFRESH();
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
