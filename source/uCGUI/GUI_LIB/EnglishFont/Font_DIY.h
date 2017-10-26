/******************************************************************************
 * 文件信息 :  
 *
 * 创 建 者 :  
 *
 * 创建日期 :  
 * 
 * 原始版本 : 
 *     
 * 修改版本 :  
 *    
 * 修改日期 : 
 *
 * 修改内容 :
 * 
 * 审 核 者 :
 *
 * 附    注 :
 *
 * 描    述 :   源代码
 *
 * 版    权 :   南京长盛仪器有限公司 , Copyright Reserved
 * 
******************************************************************************/

/******************************************************************************
 *                              头文件卫士
******************************************************************************/
 
#ifndef    FONT_DIY_H
#define    FONT_DIY_H


#if defined(__cplusplus)

    extern "C" {     /* Make sure we have C-declarations in C++ programs */

#endif

/******************************************************************************
 *                             包含文件声明
******************************************************************************/ 

#ifndef    INCLUDES_H

#include "GUI.H" 
#include "picture.h"
    
#endif

// 英文字体
extern const GUI_FONT GUI_FontSize1;
extern const GUI_FONT GUI_FontSize2;
extern const GUI_FONT GUI_FontSize3;
extern const GUI_FONT GUI_FontSize4;
        

// 中文字体
extern const GUI_FONT GUI_FontHZ_SimSun_12;     /* 主要用于设置界面显示中文 */
extern const GUI_FONT GUI_FontHZ_SimSun_13;     /* 主要用于设置界面显示英文 */
extern const GUI_FONT GUI_FontHZ_SimSun_15;     /* 测试程序使用数字 */
extern const GUI_FONT GUI_FontHZ_SimSunF_15;    /* 测试程序使用单位 */
extern const GUI_FONT GUI_FontHZ_SimSun_16;     /* 测试界面状态显示 */

extern const GUI_FONT GUI_FontHZ_Garamond_16;   /* 用于 英文主界面 */
extern const GUI_FONT GUI_FontHZ_FangZhengXingKai_24;	/* 主界面‘长盛仪器’ */
extern const GUI_FONT GUI_FontHZ_MS_Sans_Serif_24;//自动校准界面显示PASS


/******************************************************************************
 *                            文件接口宏定义
******************************************************************************/

// 英文字体1X、Y方向尺寸
#define    FONT_SIZE1_NUM_XSIZE                          8	    // 数字
#define    FONT_SIZE1_NUM_XDIST			                 8
#define    FONT_SIZE1_NUM_BYTES_PER_LINE                 1

#define    FONT_SIZE1_LOWERCASE_XSIZE                    8	    // 小写字母
#define    FONT_SIZE1_LOWERCASE_XDIST			         8
#define    FONT_SIZE1_LOWERCASE_BYTES_PER_LINE           1

#define    FONT_SIZE1_CAPITAL_XSIZE                      8	    // 大写字母
#define    FONT_SIZE1_CAPITAL_XDIST			             8
#define    FONT_SIZE1_CAPITAL_BYTES_PER_LINE             1

#define    FONT_SIZE1_YSIZE			                     16
#define    FONT_SIZE1_YDIST			                     16


// 英文字体2X、Y方向尺寸
#define    FONT_SIZE2_LOWERCASE_XSIZE                    24	    // 小写字母
#define    FONT_SIZE2_LOWERCASE_XDIST			         24
#define    FONT_SIZE2_LOWERCASE_BYTES_PER_LINE           3

#define    FONT_SIZE2_CAPITAL_XSIZE                      24	    // 大写字母
#define    FONT_SIZE2_CAPITAL_XDIST			             24
#define    FONT_SIZE2_CAPITAL_BYTES_PER_LINE             3

#define    FONT_SIZE2_YSIZE			                     41
#define    FONT_SIZE2_YDIST			                     38
 

// 英文字体3X、Y方向尺寸
#define    FONT_SIZE3_NUM_XSIZE                          32	    // 数字
#define    FONT_SIZE3_NUM_XDIST			                 32
#define    FONT_SIZE3_NUM_BYTES_PER_LINE                 4

#define    FONT_SIZE3_LOWERCASE_XSIZE                    32	    // 小写字母
#define    FONT_SIZE3_LOWERCASE_XDIST			         32
#define    FONT_SIZE3_LOWERCASE_BYTES_PER_LINE           4

#define    FONT_SIZE3_CAPITAL_XSIZE                      40	    // 大写字母
#define    FONT_SIZE3_CAPITAL_XDIST			             40
#define    FONT_SIZE3_CAPITAL_BYTES_PER_LINE             5

#define    FONT_SIZE3_YSIZE			                     65
#define    FONT_SIZE3_YDIST			                     65


// 英文字体4X、Y方向尺寸
#define    FONT_SIZE4_NUM_XSIZE                          50	    // 数字
#define    FONT_SIZE4_NUM_XDIST			                 50
#define    FONT_SIZE4_NUM_BYTES_PER_LINE                 7

#define    FONT_SIZE4_YSIZE			                     111
#define    FONT_SIZE4_YDIST			                     100


// 中文字体1X、Y方向尺寸
#define    FONT_CHINESE_SIZE1_XSIZE			             16
#define    FONT_CHINESE_SIZE1_XDIST			             16
#define    FONT_CHINESE_SIZE1_BYTES_PER_LINE             2

#define    FONT_CHINESE_SIZE1_YSIZE			             16
#define    FONT_CHINESE_SIZE1_YDIST			             16


// 中文字体2X、Y方向尺寸
#define    FONT_CHINESE_SIZE2_XSIZE			             24
#define    FONT_CHINESE_SIZE2_XDIST			             21
#define    FONT_CHINESE_SIZE2_BYTES_PER_LINE             3

#define    FONT_CHINESE_SIZE2_YSIZE			             21
#define    FONT_CHINESE_SIZE2_YDIST			             21


// 中文字体3X、Y方向尺寸
#define    FONT_CHINESE_SIZE3_XSIZE			             32
#define    FONT_CHINESE_SIZE3_XDIST			             28
#define    FONT_CHINESE_SIZE3_BYTES_PER_LINE             4

#define    FONT_CHINESE_SIZE3_YSIZE			             27
#define    FONT_CHINESE_SIZE3_YDIST			             27


// 中文字体4X、Y方向尺寸
#define    FONT_CHINESE_SIZE4_XSIZE			             40
#define    FONT_CHINESE_SIZE4_XDIST			             37
#define    FONT_CHINESE_SIZE4_BYTES_PER_LINE             5

#define    FONT_CHINESE_SIZE4_YSIZE			             37
#define    FONT_CHINESE_SIZE4_YDIST			             37
/******************************************************************************
 *                            文件接口函数声明
******************************************************************************/




#if defined(__cplusplus)

    }

#endif 


#endif
/******************************************************************************
 *                             END  OF  FILE                                                                          
******************************************************************************/
