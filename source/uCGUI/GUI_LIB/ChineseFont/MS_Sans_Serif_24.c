/* MS_Sans_Serif_24.c - MS_Sans_Serif_24 font for UCGUI font driver */ 
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
extern GUI_FLASH const GUI_FONT GUI_FontHZ_MS_Sans_Serif_24;

GUI_FLASH  const unsigned char acFontHZMS_Sans_Serif_24_0041[58] = {	/* A 0x4100*/
________,________,
________,________,
________,________,
________,________,
________,________,
________,________,
______XX,XX______,
______XX,XX______,
_____XXX,XXX_____,
_____XXX,XXX_____,
_____XXX,XXX_____,
____XXXX,XXXX____,
____XXX_,_XXX____,
____XXX_,_XXX____,
___XXXX_,_XXXX___,
___XXX__,__XXX___,
__XXXX__,__XXXX__,
__XXXXXX,XXXXXX__,
__XXXXXX,XXXXXX__,
_XXXX___,___XXXX_,
_XXX____,____XXX_,
XXXX____,____XXXX,
XXXX____,____XXXX,
________,________,
________,________,
________,________,
________,________,
________,________,
________,________,
};

GUI_FLASH  const unsigned char acFontHZMS_Sans_Serif_24_0050[87] = {	/* P 0x5000*/
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
__XXXXXX,XXXXXX__,________,
__XXXXXX,XXXXXXXX,________,
__XXX___,_____XXX,________,
__XXX___,______XX,X_______,
__XXX___,______XX,X_______,
__XXX___,______XX,X_______,
__XXX___,______XX,X_______,
__XXX___,_____XXX,________,
__XXXXXX,XXXXXXXX,________,
__XXXXXX,XXXXXX__,________,
__XXX___,________,________,
__XXX___,________,________,
__XXX___,________,________,
__XXX___,________,________,
__XXX___,________,________,
__XXX___,________,________,
__XXX___,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
};

GUI_FLASH  const unsigned char acFontHZMS_Sans_Serif_24_0053[87] = {	/* S 0x5300*/
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
_____XXX,XXXX____,________,
___XXXXX,XXXXXX__,________,
___XXX__,___XXXX_,________,
__XXX___,____XXX_,________,
__XXX___,________,________,
__XXX___,________,________,
___XXX__,________,________,
___XXXXX,XXXXX___,________,
_____XXX,XXXXXXX_,________,
________,____XXX_,________,
________,_____XXX,________,
________,_____XXX,________,
_XXX____,_____XXX,________,
_XXXX___,_____XXX,________,
__XXXXX_,____XXX_,________,
___XXXXX,XXXXXXX_,________,
_____XXX,XXXXX___,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
};

GUI_FLASH  const unsigned char acFontHZMS_Sans_Serif_24_d6d0[116] = {	/* 中 0xd6d0*/
________,________,________,________,
________,________,________,________,
________,________,________,________,
________,________,________,________,
________,________,________,________,
________,___XX___,________,________,
________,___XX___,________,________,
________,___XX___,________,________,
________,___XX___,________,________,
___XXXXX,XXXXXXXX,XXXXX___,________,
___XX___,___XX___,__XXX___,________,
___XX___,___XX___,__XXX___,________,
___XX___,___XX___,__XXX___,________,
___XX___,___XX___,__XXX___,________,
___XX___,___XX___,__XXX___,________,
___XX___,___XX___,__XXX___,________,
___XXXXX,XXXXXXXX,XXXXX___,________,
___XX___,___XX___,__XX____,________,
________,___XX___,________,________,
________,___XX___,________,________,
________,___XX___,________,________,
________,___XX___,________,________,
________,___XX___,________,________,
________,___XX___,________,________,
________,___XX___,________,________,
________,________,________,________,
________,________,________,________,
________,________,________,________,
________,________,________,________,
};

GUI_FLASH const GUI_CHARINFO GUI_FontHZMS_Sans_Serif_24_CharInfo[] = {
{ 16, 16, 2, (void GUI_FLASH *)&acFontHZMS_Sans_Serif_24_0041},/* A 0 */
{ 17, 17, 3, (void GUI_FLASH *)&acFontHZMS_Sans_Serif_24_0050},/* P 1 */
{ 17, 17, 3, (void GUI_FLASH *)&acFontHZMS_Sans_Serif_24_0053},/* S 2 */
{ 25, 25, 4, (void GUI_FLASH *)&acFontHZMS_Sans_Serif_24_d6d0},/* 中 96 */
};

/*中--中*/

GUI_FLASH const GUI_FONT_PROP GUI_FontHZMS_Sans_Serif_24_Propd6d0= {
      0xd6d0,
      0xd6d0,
      &GUI_FontHZMS_Sans_Serif_24_CharInfo[3],
      (void *)0 
};
/*S--S*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZMS_Sans_Serif_24_Prop0053= {
      0x0053,
      0x0053,
      &GUI_FontHZMS_Sans_Serif_24_CharInfo[2],
      (void *)&GUI_FontHZMS_Sans_Serif_24_Propd6d0 
};
/*P--P*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZMS_Sans_Serif_24_Prop0050= {
      0x0050,
      0x0050,
      &GUI_FontHZMS_Sans_Serif_24_CharInfo[1],
      (void *)&GUI_FontHZMS_Sans_Serif_24_Prop0053 
};
/*A--A*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZMS_Sans_Serif_24_Prop0041= {
      0x0041,
      0x0041,
      &GUI_FontHZMS_Sans_Serif_24_CharInfo[0],
      (void *)&GUI_FontHZMS_Sans_Serif_24_Prop0050 
};
GUI_FLASH const GUI_FONT GUI_FontHZ_MS_Sans_Serif_24 = {
      GUI_FONTTYPE_PROP_SJIS, 
      25, 
      25, 
      1,  
      1,  
      (void GUI_FLASH *)&GUI_FontHZMS_Sans_Serif_24_Prop0041
};
