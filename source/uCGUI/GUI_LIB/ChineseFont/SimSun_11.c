/* SimSun_11.c - SimSun_11 font for UCGUI font driver */ 
/********************************************************************************************************
*                                            UCGUI
*                                       http://www.ucgui.com
*
*                             (c) Copyright 2005-2006, UCGUI专业论坛, 学习交流UCGUI
*
*                               	ucgui@16.com 2005.07.31
*                                           作者: ucgui
*********************************************************************************************************
*/

#include "GUI.H" 
#ifndef GUI_FLASH 
	#define GUI_FLASH 
#endif 
extern GUI_FLASH const GUI_FONT GUI_FontHZ_SimSun_11;

GUI_FLASH  const unsigned char acFontHZSimSun_11_002f[11] = {	/* / 0x2f00*/
________,
____X___,
___X____,
___X____,
__X_____,
__X_____,
_X______,
_X______,
_X______,
X_______,
________,
};

GUI_FLASH  const unsigned char acFontHZSimSun_11_0030[11] = {	/* 0 0x3000*/
________,
________,
__XX____,
_X__X___,
_X__X___,
_X__X___,
_X__X___,
_X__X___,
__XX____,
________,
________,
};

GUI_FLASH  const unsigned char acFontHZSimSun_11_0031[11] = {	/* 1 0x3100*/
________,
________,
__X_____,
_XX_____,
__X_____,
__X_____,
__X_____,
__X_____,
_XXX____,
________,
________,
};

GUI_FLASH  const unsigned char acFontHZSimSun_11_0032[11] = {	/* 2 0x3200*/
________,
________,
__XX____,
_X__X___,
____X___,
___X____,
__X_____,
_X______,
_XXXX___,
________,
________,
};

GUI_FLASH  const unsigned char acFontHZSimSun_11_0033[11] = {	/* 3 0x3300*/
________,
________,
__XX____,
_X__X___,
____X___,
__XX____,
____X___,
_X__X___,
__XX____,
________,
________,
};

GUI_FLASH  const unsigned char acFontHZSimSun_11_0034[11] = {	/* 4 0x3400*/
________,
___X____,
___X____,
__XX____,
_X_X____,
X__X____,
XXXXX___,
___X____,
___X____,
________,
________,
};

GUI_FLASH  const unsigned char acFontHZSimSun_11_0035[11] = {	/* 5 0x3500*/
________,
________,
_XXXX___,
_X______,
_XXX____,
____X___,
____X___,
_X__X___,
__XX____,
________,
________,
};

GUI_FLASH  const unsigned char acFontHZSimSun_11_0036[11] = {	/* 6 0x3600*/
________,
________,
_XXX____,
X_______,
X_______,
XXXX____,
X___X___,
X___X___,
_XXX____,
________,
________,
};

GUI_FLASH  const unsigned char acFontHZSimSun_11_0037[11] = {	/* 7 0x3700*/
________,
________,
_XXXX___,
___X____,
___X____,
__X_____,
__X_____,
__X_____,
__X_____,
________,
________,
};

GUI_FLASH  const unsigned char acFontHZSimSun_11_0038[11] = {	/* 8 0x3800*/
________,
________,
__XX____,
_X__X___,
_X__X___,
__XX____,
_X__X___,
_X__X___,
__XX____,
________,
________,
};

GUI_FLASH  const unsigned char acFontHZSimSun_11_0039[11] = {	/* 9 0x3900*/
________,
________,
__XX____,
_X__X___,
_X__X___,
__XXX___,
____X___,
____X___,
__XX____,
________,
________,
};

GUI_FLASH  const unsigned char acFontHZSimSun_11_c7eb[22] = {	/* 请 0xc7eb*/
_X____X_,________,
__X_XXXX,XX______,
______X_,X_______,
__X__XXX,_X______,
_XX_____,X_______,
__X_XXXX,X_______,
__X_X___,X_______,
__XX_XXX,X_______,
__X_X___,X_______,
________,X_______,
________,________,
};

GUI_FLASH const GUI_CHARINFO GUI_FontHZSimSun_11_CharInfo[] = {
{ 5, 5, 1, (void GUI_FLASH *)&acFontHZSimSun_11_002f},/* / 0 */
{ 6, 6, 1, (void GUI_FLASH *)&acFontHZSimSun_11_0030},/* 0 1 */
{ 6, 6, 1, (void GUI_FLASH *)&acFontHZSimSun_11_0031},/* 1 2 */
{ 6, 6, 1, (void GUI_FLASH *)&acFontHZSimSun_11_0032},/* 2 3 */
{ 6, 6, 1, (void GUI_FLASH *)&acFontHZSimSun_11_0033},/* 3 4 */
{ 6, 6, 1, (void GUI_FLASH *)&acFontHZSimSun_11_0034},/* 4 5 */
{ 6, 6, 1, (void GUI_FLASH *)&acFontHZSimSun_11_0035},/* 5 6 */
{ 6, 6, 1, (void GUI_FLASH *)&acFontHZSimSun_11_0036},/* 6 7 */
{ 6, 6, 1, (void GUI_FLASH *)&acFontHZSimSun_11_0037},/* 7 8 */
{ 6, 6, 1, (void GUI_FLASH *)&acFontHZSimSun_11_0038},/* 8 9 */
{ 6, 6, 1, (void GUI_FLASH *)&acFontHZSimSun_11_0039},/* 9 10 */
{ 11, 11, 2, (void GUI_FLASH *)&acFontHZSimSun_11_c7eb},/* 请 96 */
};

/*请--请*/

GUI_FLASH const GUI_FONT_PROP GUI_FontHZSimSun_11_Propc7eb= {
      0xc7eb,
      0xc7eb,
      &GUI_FontHZSimSun_11_CharInfo[11],
      (void *)0 
};
/*/--9*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZSimSun_11_Prop002f= {
      0x002f,
      0x0039,
      &GUI_FontHZSimSun_11_CharInfo[0],
      (void *)&GUI_FontHZSimSun_11_Propc7eb 
};
GUI_FLASH const GUI_FONT GUI_FontHZ_SimSun_11 = {
      GUI_FONTTYPE_PROP_SJIS, 
      11, 
      11, 
      1,  
      1,  
      (void GUI_FLASH *)&GUI_FontHZSimSun_11_Prop002f
};
