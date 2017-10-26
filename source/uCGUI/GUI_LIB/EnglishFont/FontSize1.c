/****************************************************************************** 
 *  项目名称 :                      
 *                                                               
 *  项目功能 :  
 *                                                                          
 *  应用软件 :                                                 
 *                                                                       
 *  版    本 :  
 *                                                          
 *  硬    件 :  
 *                                                                
 *  创建时间 :  
 *               
 *  编    写 :             
 *               
 *  附    注 :                                                                 
 *                                                                                     
 *                                                                                           
******************************************************************************/ 

/******************************************************************************
 *                          本文件所引用的头文件
******************************************************************************/ 
 
#include "Font_DIY.h"

/******************************************************************************
 *                            本文件内部宏定义
******************************************************************************/ 

#ifndef GUI_FLASH
   #define GUI_FLASH 
#endif

/******************************************************************************
 *                       本文件所定义的静态全局变量
******************************************************************************/ 

/******************************************************************************/

//空格
static GUI_FLASH  const unsigned char acFontSize1_0020[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
//'0'
static GUI_FLASH  const unsigned char acFontSize1_0030[] = {
0x00,0x00,0x1C,0x22,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x22,0x1C,0x00,0x00,
};
//'1'
static GUI_FLASH  const unsigned char acFontSize1_0031[] = {
0x00,0x00,0x08,0x38,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,
};
//'2'
static GUI_FLASH  const unsigned char acFontSize1_0032[] = {
0x00,0x00,0x3C,0x42,0x42,0x02,0x02,0x04,0x08,0x10,0x20,0x40,0x40,0x7E,0x00,0x00,
};
//'3'
static GUI_FLASH  const unsigned char acFontSize1_0033[] = {
0x00,0x00,0x3C,0x42,0x42,0x02,0x02,0x3C,0x02,0x02,0x02,0x42,0x42,0x3C,0x00,0x00,
};
//'4'
static GUI_FLASH  const unsigned char acFontSize1_0034[] = {
0x00,0x00,0x06,0x06,0x0A,0x0A,0x12,0x12,0x22,0x22,0x7F,0x02,0x02,0x02,0x00,0x00,
};
//'5'
static GUI_FLASH  const unsigned char acFontSize1_0035[] = {
0x00,0x00,0x7E,0x40,0x40,0x40,0x7C,0x42,0x02,0x02,0x02,0x42,0x42,0x3C,0x00,0x00,
};
//'6'
static GUI_FLASH  const unsigned char acFontSize1_0036[] = {
0x00,0x00,0x3C,0x42,0x42,0x40,0x40,0x7C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00,
};
//'7'
static GUI_FLASH  const unsigned char acFontSize1_0037[] = {
0x00,0x00,0x7E,0x02,0x04,0x04,0x08,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x00,0x00,
};
//'8'
static GUI_FLASH  const unsigned char acFontSize1_0038[] = {
0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00,
};
//'9'
static GUI_FLASH  const unsigned char acFontSize1_0039[] = {
0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x3E,0x02,0x02,0x42,0x42,0x3C,0x00,0x00,
};
//'A'
static GUI_FLASH  const unsigned char acFontSize1_0041[] = {
0x00,0x00,0x08,0x08,0x08,0x14,0x14,0x14,0x22,0x22,0x3E,0x41,0x41,0x41,0x00,0x00,
};
//'B'
static GUI_FLASH  const unsigned char acFontSize1_0042[] = {
0x00,0x00,0x7C,0x42,0x41,0x41,0x42,0x7C,0x42,0x41,0x41,0x41,0x42,0x7C,0x00,0x00,
};
//'C'
static GUI_FLASH  const unsigned char acFontSize1_0043[] = {
0x00,0x00,0x1C,0x22,0x41,0x41,0x40,0x40,0x40,0x40,0x41,0x41,0x22,0x1C,0x00,0x00,
};
//'D'
static GUI_FLASH  const unsigned char acFontSize1_0044[] = {
0x00,0x00,0x7C,0x42,0x42,0x41,0x41,0x41,0x41,0x41,0x41,0x42,0x42,0x7C,0x00,0x00,
};
//'E'
static GUI_FLASH  const unsigned char acFontSize1_0045[] = {
0x00,0x00,0x7F,0x40,0x40,0x40,0x40,0x7E,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00,
};
//'F'
static GUI_FLASH  const unsigned char acFontSize1_0046[] = {
0x00,0x00,0x7F,0x40,0x40,0x40,0x40,0x7E,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,
};
//'G'
static GUI_FLASH  const unsigned char acFontSize1_0047[] = {
0x00,0x00,0x1C,0x22,0x41,0x40,0x40,0x40,0x47,0x41,0x41,0x41,0x23,0x1D,0x00,0x00,
};
//'H'
static GUI_FLASH  const unsigned char acFontSize1_0048[] = {
0x00,0x00,0x41,0x41,0x41,0x41,0x41,0x7F,0x41,0x41,0x41,0x41,0x41,0x41,0x00,0x00,
};
//'I'
static GUI_FLASH  const unsigned char acFontSize1_0049[] = {
0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,
};
//'J'
static GUI_FLASH  const unsigned char acFontSize1_004A[] = {
0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x44,0x44,0x38,0x00,0x00,
};
//'K'
static GUI_FLASH  const unsigned char acFontSize1_004B[] = {
0x00,0x00,0x41,0x42,0x44,0x48,0x50,0x60,0x60,0x50,0x48,0x44,0x42,0x41,0x00,0x00,
};
//'L'
static GUI_FLASH  const unsigned char acFontSize1_004C[] = {
0x00,0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7E,0x00,0x00,
};
//'M'
static GUI_FLASH  const unsigned char acFontSize1_004D[] = {
0x00,0x00,0x41,0x41,0x63,0x63,0x55,0x55,0x49,0x49,0x49,0x41,0x41,0x41,0x00,0x00,
};
//'N'
static GUI_FLASH  const unsigned char acFontSize1_004E[] = {
0x00,0x00,0x41,0x61,0x61,0x51,0x51,0x49,0x49,0x45,0x45,0x43,0x43,0x41,0x00,0x00,
};
//'O'
static GUI_FLASH  const unsigned char acFontSize1_004F[] = {
0x00,0x00,0x1C,0x22,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x22,0x1C,0x00,0x00,
};
//'P'
static GUI_FLASH  const unsigned char acFontSize1_0050[] = {
0x00,0x00,0x7C,0x42,0x41,0x41,0x41,0x42,0x7C,0x40,0x40,0x40,0x40,0x40,0x00,0x00,
};
//'Q'
static GUI_FLASH  const unsigned char acFontSize1_0051[] = {
0x00,0x00,0x1C,0x22,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x45,0x22,0x1D,0x00,0x00,
};
//'R'
static GUI_FLASH  const unsigned char acFontSize1_0052[] = {
0x00,0x00,0x7C,0x42,0x41,0x41,0x41,0x42,0x7C,0x42,0x41,0x41,0x41,0x41,0x00,0x00,
};
//'S'
static GUI_FLASH  const unsigned char acFontSize1_0053[] = {
0x00,0x00,0x3E,0x41,0x41,0x40,0x20,0x18,0x04,0x02,0x01,0x41,0x41,0x3E,0x00,0x00,
};
//'T'
static GUI_FLASH  const unsigned char acFontSize1_0054[] = {
0x00,0x00,0x7F,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,
};
//'U'
static GUI_FLASH  const unsigned char acFontSize1_0055[] = {
0x00,0x00,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x3E,0x00,0x00,
};
//'V'
static GUI_FLASH  const unsigned char acFontSize1_0056[] = {
0x00,0x00,0x41,0x41,0x41,0x22,0x22,0x22,0x14,0x14,0x14,0x08,0x08,0x08,0x00,0x00,
};
//'W'
static GUI_FLASH  const unsigned char acFontSize1_0057[] = {
0x00,0x00,0x49,0x49,0x49,0x49,0x49,0x55,0x55,0x55,0x22,0x22,0x22,0x22,0x00,0x00,
};
//'X'
static GUI_FLASH  const unsigned char acFontSize1_0058[] = {
0x00,0x00,0x41,0x41,0x22,0x22,0x14,0x08,0x14,0x14,0x22,0x22,0x41,0x41,0x00,0x00,
};
//'Y'
static GUI_FLASH  const unsigned char acFontSize1_0059[] = {
0x00,0x00,0x41,0x41,0x22,0x22,0x14,0x14,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,
};
//'Z'
static GUI_FLASH  const unsigned char acFontSize1_005A[] = {
0x00,0x00,0x7E,0x02,0x04,0x04,0x08,0x08,0x10,0x10,0x20,0x20,0x40,0x7E,0x00,0x00,
};
//'a'
static GUI_FLASH  const unsigned char acFontSize1_0061[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x02,0x3E,0x42,0x42,0x42,0x3D,0x00,0x00,
};
//'b'
static GUI_FLASH  const unsigned char acFontSize1_0062[] = {
0x00,0x00,0x40,0x40,0x40,0x40,0x5C,0x62,0x41,0x41,0x41,0x41,0x62,0x5C,0x00,0x00,
};
//'c'
static GUI_FLASH  const unsigned char acFontSize1_0063[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x21,0x40,0x40,0x40,0x40,0x21,0x1E,0x00,0x00,
};
//'d'
static GUI_FLASH  const unsigned char acFontSize1_0064[] = {
0x00,0x00,0x01,0x01,0x01,0x01,0x1D,0x23,0x41,0x41,0x41,0x41,0x23,0x1D,0x00,0x00,
};
//'e'
static GUI_FLASH  const unsigned char acFontSize1_0065[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x22,0x41,0x7F,0x40,0x40,0x21,0x1E,0x00,0x00,
};
//'f'
static GUI_FLASH  const unsigned char acFontSize1_0066[] = {
0x00,0x00,0x0E,0x10,0x10,0x10,0x7E,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,
};
//'g'
static GUI_FLASH  const unsigned char acFontSize1_0067[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x1A,0x26,0x42,0x42,0x42,0x26,0x1A,0x42,0x3C,0x00,
};
//'h'
static GUI_FLASH  const unsigned char acFontSize1_0068[] = {
0x00,0x00,0x40,0x40,0x40,0x40,0x5C,0x62,0x42,0x42,0x42,0x42,0x42,0x42,0x00,0x00,
};
//'i'
static GUI_FLASH  const unsigned char acFontSize1_0069[] = {
0x00,0x00,0x08,0x08,0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,
};
//'j'
static GUI_FLASH  const unsigned char acFontSize1_006A[] = {
0x00,0x00,0x08,0x08,0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x30,0x00,
};
//'k'
static GUI_FLASH  const unsigned char acFontSize1_006B[] = {
0x00,0x00,0x40,0x40,0x40,0x42,0x44,0x48,0x50,0x60,0x50,0x48,0x44,0x42,0x00,0x00,
};
//'l'
static GUI_FLASH  const unsigned char acFontSize1_006C[] = {
0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,
};
//'m'
static GUI_FLASH  const unsigned char acFontSize1_006D[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x76,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x00,0x00,
};
//'n'
static GUI_FLASH  const unsigned char acFontSize1_006E[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x5C,0x62,0x42,0x42,0x42,0x42,0x42,0x42,0x00,0x00,
};
//'o'
static GUI_FLASH  const unsigned char acFontSize1_006F[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x22,0x41,0x41,0x41,0x41,0x22,0x1C,0x00,0x00,
};
//'p'
static GUI_FLASH  const unsigned char acFontSize1_0070[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x5C,0x62,0x41,0x41,0x41,0x62,0x5C,0x40,0x40,0x00,
};
//'q'
static GUI_FLASH  const unsigned char acFontSize1_0071[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x1D,0x23,0x41,0x41,0x41,0x23,0x1D,0x01,0x01,0x00,
};
//'r'
static GUI_FLASH  const unsigned char acFontSize1_0072[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x2E,0x30,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,
};
//'s'
static GUI_FLASH  const unsigned char acFontSize1_0073[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x40,0x30,0x0C,0x02,0x42,0x3C,0x00,0x00,
};
//'t'
static GUI_FLASH  const unsigned char acFontSize1_0074[] = {
0x00,0x00,0x10,0x10,0x10,0x10,0x7E,0x10,0x10,0x10,0x10,0x10,0x10,0x0E,0x00,0x00,
};
//'u'
static GUI_FLASH  const unsigned char acFontSize1_0075[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x3E,0x00,0x00,
};
//'v'
static GUI_FLASH  const unsigned char acFontSize1_0076[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x41,0x41,0x22,0x22,0x14,0x14,0x08,0x08,0x00,0x00,
};
//'w'
static GUI_FLASH  const unsigned char acFontSize1_0077[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x41,0x49,0x49,0x49,0x55,0x36,0x22,0x22,0x00,0x00,
};
//'x'
static GUI_FLASH  const unsigned char acFontSize1_0078[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x41,0x22,0x14,0x08,0x08,0x14,0x22,0x41,0x00,0x00,
};
//'y'
static GUI_FLASH  const unsigned char acFontSize1_0079[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x41,0x41,0x22,0x22,0x14,0x14,0x08,0x08,0x70,0x00,
};
//'z'
static GUI_FLASH  const unsigned char acFontSize1_007A[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x02,0x04,0x08,0x10,0x20,0x40,0x7E,0x00,0x00,
};
//'.'
static GUI_FLASH  const unsigned char acFontSize1_002E[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00,0x00,
};
//":"
static GUI_FLASH  const unsigned char acFontSize1_003A[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,
};
//"+"
static GUI_FLASH  const unsigned char acFontSize1_002B[] = {
0x00,0x00,0x00,0x00,0x10,0x10,0x10,0x10,0xFE,0x10,0x10,0x10,0x10,0x00,0x00,0x00,
};
//"-"
static GUI_FLASH  const unsigned char acFontSize1_002D[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
//"<"
static GUI_FLASH  const unsigned char acFontSize1_003C[] = {
0x00,0x00,0x00,0x02,0x0C,0x10,0x60,0x80,0x60,0x10,0x0C,0x02,0x00,0x00,0x00,0x00,
};
//">"
static GUI_FLASH  const unsigned char acFontSize1_003E[] = {
0x00,0x00,0x00,0x40,0x30,0x08,0x06,0x01,0x06,0x08,0x30,0x40,0x00,0x00,0x00,0x00,
};
//"/"
static GUI_FLASH  const unsigned char acFontSize1_002F[] = {
0x00,0x00,0x02,0x02,0x04,0x04,0x08,0x08,0x10,0x10,0x20,0x20,0x40,0x40,0x00,0x00,
};
// "  
static GUI_FLASH  const unsigned char acFontSize1_0027[] = {
0x00,0x00,0x24,0x24,0x24,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
// %
static GUI_FLASH  const unsigned char acFontSize1_0025[] = {
0x00,0x00,0x31,0x49,0x4A,0x4C,0x34,0x08,0x10,0x16,0x29,0x49,0x49,0x06,0x00,0x00,
};
// 简 
static GUI_FLASH  const unsigned char acFontSize1_BCF2[] = {
0x10,0x40,0x1E,0xFC,0x29,0x20,0x44,0x10,0x93,0xF8,0x28,0x08,0x27,0xC8,0x24,0x48,
0x24,0x48,0x27,0xC8,0x24,0x48,0x27,0xC8,0x20,0x28,0x20,0x10,0x00,0x00,0x00,0x00,
};
// 体 
static GUI_FLASH  const unsigned char acFontSize1_CCE5[] = {
0x08,0x20,0x08,0x20,0x08,0x20,0x17,0xFF,0x10,0x20,0x30,0x70,0x30,0x70,0x50,0xA8,
0x10,0xA8,0x11,0x24,0x12,0x22,0x14,0xF9,0x10,0x20,0x10,0x20,0x10,0x20,0x00,0x00,
};
// 中 
static GUI_FLASH  const unsigned char acFontSize1_D6D0[] = {
0x00,0x80,0x00,0x80,0x00,0x80,0x3F,0xFE,0x20,0x82,0x20,0x82,0x20,0x82,0x20,0x82,
0x20,0x82,0x3F,0xFE,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x00,
};
// 文 
static GUI_FLASH  const unsigned char acFontSize1_CEC4[] = {
0x00,0x80,0x00,0x80,0x7F,0xFF,0x08,0x08,0x08,0x08,0x08,0x08,0x04,0x10,0x04,0x10,
0x02,0x20,0x01,0x40,0x00,0x80,0x01,0x40,0x06,0x30,0x18,0x0C,0x60,0x03,0x00,0x00,
};
/******************************************************************************/
/******************************************************************************/

static GUI_FLASH   const GUI_CHARINFO GUI_FontSize1_CharInfo[] = {
    // 空格
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE,  (const unsigned char *)acFontSize1_0020 },
	// 数字
	{  FONT_SIZE1_NUM_XSIZE,     FONT_SIZE1_NUM_XDIST,     FONT_SIZE1_NUM_BYTES_PER_LINE,    (const unsigned char *)acFontSize1_0030 },
	{  FONT_SIZE1_NUM_XSIZE,     FONT_SIZE1_NUM_XDIST,     FONT_SIZE1_NUM_BYTES_PER_LINE,    (const unsigned char *)acFontSize1_0031 },
	{  FONT_SIZE1_NUM_XSIZE,     FONT_SIZE1_NUM_XDIST,     FONT_SIZE1_NUM_BYTES_PER_LINE,    (const unsigned char *)acFontSize1_0032 },
	{  FONT_SIZE1_NUM_XSIZE,     FONT_SIZE1_NUM_XDIST,     FONT_SIZE1_NUM_BYTES_PER_LINE,    (const unsigned char *)acFontSize1_0033 },
	{  FONT_SIZE1_NUM_XSIZE,     FONT_SIZE1_NUM_XDIST,     FONT_SIZE1_NUM_BYTES_PER_LINE,    (const unsigned char *)acFontSize1_0034 },
	{  FONT_SIZE1_NUM_XSIZE,     FONT_SIZE1_NUM_XDIST,     FONT_SIZE1_NUM_BYTES_PER_LINE,    (const unsigned char *)acFontSize1_0035 },
	{  FONT_SIZE1_NUM_XSIZE,     FONT_SIZE1_NUM_XDIST,     FONT_SIZE1_NUM_BYTES_PER_LINE,    (const unsigned char *)acFontSize1_0036 },
	{  FONT_SIZE1_NUM_XSIZE,     FONT_SIZE1_NUM_XDIST,     FONT_SIZE1_NUM_BYTES_PER_LINE,    (const unsigned char *)acFontSize1_0037 },
	{  FONT_SIZE1_NUM_XSIZE,     FONT_SIZE1_NUM_XDIST,     FONT_SIZE1_NUM_BYTES_PER_LINE,    (const unsigned char *)acFontSize1_0038 },
	{  FONT_SIZE1_NUM_XSIZE,     FONT_SIZE1_NUM_XDIST,     FONT_SIZE1_NUM_BYTES_PER_LINE,    (const unsigned char *)acFontSize1_0039 },
	// 大写字母
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0041 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0042 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0043 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0044 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0045 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0046 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0047 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0048 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0049 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_004A },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_004B },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_004C },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_004D },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_004E },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_004F },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0050 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0051 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0052 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0053 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0054 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0055 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0056 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0057 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0058 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0059 },
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE, (const unsigned char *)acFontSize1_005A },
	// 小写字母
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0061 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0062 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0063 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0064 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0065 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0066 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0067 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0068 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0069 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_006A },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_006B },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_006C },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_006D },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_006E },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_006F },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0070 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0071 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0072 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0073 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0074 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0075 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0076 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0077 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0078 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_0079 },
	{  FONT_SIZE1_LOWERCASE_XSIZE,  FONT_SIZE1_LOWERCASE_XDIST,  FONT_SIZE1_LOWERCASE_BYTES_PER_LINE, (const unsigned char *)acFontSize1_007A },
    // "."
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE,  (const unsigned char *)acFontSize1_002E },
	// ":"
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE,  (const unsigned char *)acFontSize1_003A },
	// "+"
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE,  (const unsigned char *)acFontSize1_002B },
	// "-"
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE,  (const unsigned char *)acFontSize1_002D },
	// "<"
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE,  (const unsigned char *)acFontSize1_003C },
	// ">"
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE,  (const unsigned char *)acFontSize1_003E },
	// "/"
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE,  (const unsigned char *)acFontSize1_002F },
	// "
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE,  (const unsigned char *)acFontSize1_0027 },
	// %
	{  FONT_SIZE1_CAPITAL_XSIZE,  FONT_SIZE1_CAPITAL_XDIST,  FONT_SIZE1_CAPITAL_BYTES_PER_LINE,  (const unsigned char *)acFontSize1_0025 },
    // 简 
	{  16,  16,  2, (const unsigned char *)acFontSize1_BCF2 },
	// 体 
	{  16,  16,  2, (const unsigned char *)acFontSize1_CCE5 },
	// 中 
	{  16,  16,  2, (const unsigned char *)acFontSize1_D6D0 },
	// 文 
	{  16,  16,  2, (const unsigned char *)acFontSize1_CEC4 },

}; 

/******************************************************************************/
/******************************************************************************/
// 文
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa17= {
      0xCEC4, 
      0xCEC4, 
      &GUI_FontSize1_CharInfo[75], 
      (const GUI_FONT_PROP *)0//&GUI_FontSize1_Propa18
};
// 中
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa16= {
      0xD6D0, 
      0xD6D0, 
      &GUI_FontSize1_CharInfo[74], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa17
};
// 体
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa15= {
      0xCCE5, 
      0xCCE5, 
      &GUI_FontSize1_CharInfo[73], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa16
};
// 简
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa14= {
      0xBCF2, 
      0xBCF2, 
      &GUI_FontSize1_CharInfo[72], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa15
};
// %
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa13= {
      0x0025, 
      0x0025, 
      &GUI_FontSize1_CharInfo[71], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa14
};
// "
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa12= {
      0x0027, 
      0x0027, 
      &GUI_FontSize1_CharInfo[70], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa13
};
// "/"
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa11= {
      0x002F, 
      0x002F, 
      &GUI_FontSize1_CharInfo[69], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa12
};
// ">"
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa10= {
      0x003E, 
      0x003E, 
      &GUI_FontSize1_CharInfo[68], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa11
};
// "<"
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa9= {
      0x003C, 
      0x003C, 
      &GUI_FontSize1_CharInfo[67], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa10
};
// "-"
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa8= {
      0x002D, 
      0x002D, 
      &GUI_FontSize1_CharInfo[66], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa9
};
// "+"
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa7= {
      0x002B, 
      0x002B, 
      &GUI_FontSize1_CharInfo[65], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa8
};
// ":"
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa6= {
      0x003A, 
      0x003A, 
      &GUI_FontSize1_CharInfo[64], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa7
};
// "."
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa5= {
      0x002E, 
      0x002E, 
      &GUI_FontSize1_CharInfo[63], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa6
};
// a ~ z
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa4= {
      0x0061, 
      0x007A, 
      &GUI_FontSize1_CharInfo[37], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa5
};
// A ~ Z
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa3= {
      0x0041, 
      0x005A, 
      &GUI_FontSize1_CharInfo[11], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa4
};
// 0 ~ 9
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa2= {
      0x0030, 
      0x0039, 
      &GUI_FontSize1_CharInfo[1], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa3
};
//空格
static GUI_FLASH  const GUI_FONT_PROP GUI_FontSize1_Propa1= {
      0x0020, 
      0x0020, 
      &GUI_FontSize1_CharInfo[0], 
      (const GUI_FONT_PROP *)&GUI_FontSize1_Propa2
};

/******************************************************************************/
/******************************************************************************/

/* GulimChe 常规 小四  */
GUI_FLASH const GUI_FONT GUI_FontSize1 = {
      GUI_FONTTYPE_PROP_SJIS, 
      FONT_SIZE1_YSIZE, 
      FONT_SIZE1_YDIST, 
      1,  
      1,  
      (void GUI_FLASH *)&GUI_FontSize1_Propa1
};

/******************************************************************************/
/******************************************************************************
 *                             END  OF  FILE                                                                          
******************************************************************************/

