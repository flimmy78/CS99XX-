#ifndef  __LCD24064__H
#define  __LCD24064__H

/**********************************************************
*   包含的头文件                                         *
***********************************************************/
#include "gui.h"


/**********************************************************
*   系统自带字体说明                                      *
***********************************************************/

//比例字体
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8_ASCII,        GUI_Font8_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font10S_ASCII,      GUI_Font10S_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font10_ASCII,       GUI_Font10_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font13_ASCII,       GUI_Font13_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font13B_ASCII,      GUI_Font13B_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font13H_ASCII,      GUI_Font13H_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font13HB_ASCII,     GUI_Font13HB_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16_ASCII,       GUI_Font16_1,       GUI_Font16_HK,    GUI_Font16_1HK;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16B_ASCII,      GUI_Font16B_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font24_ASCII,       GUI_Font24_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font24B_ASCII,      GUI_Font24B_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font32_ASCII,       GUI_Font32_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font32B_ASCII,      GUI_Font32B_1;

//等距字体
extern GUI_CONST_STORAGE GUI_FONT GUI_Font4x6;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font6x8,            GUI_Font6x9;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x8,            GUI_Font8x9;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x10_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x12_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x13_ASCII,     GUI_Font8x13_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x15B_ASCII,    GUI_Font8x15B_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x16,           GUI_Font8x17,       GUI_Font8x18;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x16x1x2,       GUI_Font8x16x2x2,   GUI_Font8x16x3x3;

//数值
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD24x32;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD32;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD36x48;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD48;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD48x64;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD64;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD60x80;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD80;

//艺术字
extern GUI_CONST_STORAGE GUI_FONT GUI_FontComic18B_ASCII, GUI_FontComic18B_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontComic24B_ASCII, GUI_FontComic24B_1;




/**********************************************************
*   接口函数申明和说明                                   *
***********************************************************/

//  二维图形接口函数

int  GUI_BMP_Draw         (const void * pFileData, int x0, int y0);
int  GUI_BMP_GetXSize     (const void * pFileData);
int  GUI_BMP_GetYSize     (const void * pFileData);
void GUI_Clear            (void);
void GUI_ClearRect        (int x0, int y0, int x1, int y1);
void GUI_ClearRectEx      (const GUI_RECT* pRect);
void GUI_DrawArc          (int x0, int y0, int rx, int ry, int a0, int a1);
void GUI_DrawBitmap       (const GUI_BITMAP GUI_UNI_PTR * pBM, int x0, int y0);
void GUI_DrawBitmapMag    (const GUI_BITMAP GUI_UNI_PTR * pBM, int x0, int y0, int XMul, int YMul);
void GUI_DrawBitmapEx     (const GUI_BITMAP GUI_UNI_PTR * pBitmap, int x0, int y0, int xCenter, int yCenter, int xMag, int yMag);
void GUI_DrawBitmapExp    (int x0, int y0, int XSize, int YSize, int XMul,  int YMul, int BitsPerPixel, int BytesPerLine, const U8 GUI_UNI_PTR * pData, const GUI_LOGPALETTE GUI_UNI_PTR * pPal);
void GUI_DrawCircle       (int x0, int y0, int r);
void GUI_DrawEllipse      (int x0, int y0, int rx, int ry);
void GUI_DrawGraph        (I16 *pay, int NumPoints, int x0, int y0);
void GUI_DrawHLine        (int y0, int x0, int x1);
void GUI_DrawLine         (int x0, int y0, int x1, int y1);
void GUI_DrawLineRel      (int dx, int dy);
void GUI_DrawLineTo       (int x, int y);
void GUI_DrawPie          (int x0, int y0, int r, int a0, int a1, int Type);
void GUI_DrawPixel        (int x, int y);
void GUI_DrawPoint        (int x, int y);//使用当前尺寸的笔尖绘一个点
void GUI_DrawPolygon      (const GUI_POINT* pPoints, int NumPoints, int x0, int y0);
void GUI_DrawPolyLine     (const GUI_POINT* pPoints, int NumPoints, int x0, int y0);
void GUI_DrawFocusRect    (const GUI_RECT *pRect, int Dist);
void GUI_DrawRect         (int x0, int y0, int x1, int y1);
void GUI_DrawRectEx       (const GUI_RECT *pRect);
void GUI_DrawVLine        (int x0, int y0, int y1);
void GUI_FillCircle       (int x0, int y0, int r);
void GUI_FillEllipse      (int x0, int y0, int rx, int ry);
void GUI_FillPolygon      (const GUI_POINT* pPoints, int NumPoints, int x0, int y0);
void GUI_FillRect         (int x0, int y0, int x1, int y1);
void GUI_FillRectEx       (const GUI_RECT* pRect);
void GUI_GetClientRect    (GUI_RECT* pRect);
void GUI_InvertRect       (int x0, int y0, int x1, int y1);
void GUI_MoveRel          (int dx, int dy);
void GUI_MoveTo           (int x, int y);


//文本显示程序

void  GUI_DispCEOL (void);
void  GUI_DispChar  (U16 c);
void  GUI_DispChars (U16 c, int Cnt);
void  GUI_DispCharAt(U16 c, I16P x, I16P y);
void  GUI_DispString         (const char GUI_UNI_PTR *s);
void  GUI_DispStringAt       (const char GUI_UNI_PTR *s, int x, int y);
void  GUI_DispStringAtCEOL   (const char GUI_UNI_PTR *s, int x, int y);
void  GUI_DispStringHCenterAt(const char GUI_UNI_PTR *s, int x, int y);
void  GUI__DispStringInRect  (const char GUI_UNI_PTR *s, GUI_RECT* pRect, int TextAlign, int MaxNumChars);
void  GUI_DispStringInRect   (const char GUI_UNI_PTR *s, GUI_RECT* pRect, int Flags);
#if GUI_SUPPORT_ROTATION
  void  GUI_DispStringInRectEx (const char GUI_UNI_PTR *s, GUI_RECT* pRect, int TextAlign, int MaxLen, const GUI_ROTATION * pLCD_Api);
#endif
void  GUI_DispStringInRectMax(const char GUI_UNI_PTR *s, GUI_RECT* pRect, int TextAlign, int MaxLen); /* Not to be doc. */
void  GUI_DispStringLen      (const char GUI_UNI_PTR *s, int Len);
void  GUI_GetTextExtend(GUI_RECT* pRect, const char GUI_UNI_PTR * s, int Len);
int   GUI_GetYAdjust(void);
int   GUI_GetDispPosX(void);
int   GUI_GetDispPosY(void);
const GUI_FONT GUI_UNI_PTR * GUI_GetFont(void);
int   GUI_GetCharDistX(U16 c);
int   GUI_GetStringDistX(const char GUI_UNI_PTR *s);
int   GUI_GetFontDistY(void);
int   GUI_GetFontSizeY(void);
void  GUI_GetFontInfo   (const GUI_FONT GUI_UNI_PTR * pFont, GUI_FONTINFO* pfi);
int   GUI_GetYSizeOfFont(const GUI_FONT GUI_UNI_PTR * pFont);
int   GUI_GetYDistOfFont(const GUI_FONT GUI_UNI_PTR * pFont);
int   GUI_GetTextAlign(void);
int   GUI_GetTextMode(void);
char  GUI_IsInFont(const GUI_FONT GUI_UNI_PTR * pFont, U16 c);
int   GUI_SetTextAlign(int Align);
int   GUI_SetTextMode(int Mode);
char  GUI_SetTextStyle(char Style);
int   GUI_SetLBorder(int x);
void  GUI_SetOrg(int x, int y);
const GUI_FONT GUI_UNI_PTR * GUI_SetFont(const GUI_FONT GUI_UNI_PTR * pNewFont);
char  GUI_GotoXY(int x, int y);
char  GUI_GotoX(int x);
char  GUI_GotoY(int y);
void  GUI_DispNextLine(void);

//独立位置字体显示

void GUI_SIF_CreateFont(void * pFontData, GUI_FONT * pFont, const GUI_SIF_TYPE * pFontType);
void GUI_SIF_DeleteFont(GUI_FONT * pFont);

//数值显示

void GUI_DispBin  (U32  v, U8 Len);
void GUI_DispBinAt(U32  v, I16P x, I16P y, U8 Len);
void GUI_DispDec  (I32 v, U8 Len);
void GUI_DispDecAt (I32 v, I16P x, I16P y, U8 Len);
void GUI_DispDecMin(I32 v);
void GUI_DispDecShift(I32 v, U8 Len, U8 Shift);
void GUI_DispDecSpace(I32 v, U8 MaxDigits);
void GUI_DispHex  (U32 v, U8 Len);
void GUI_DispHexAt(U32 v, I16P x, I16P y, U8 Len);
void GUI_DispSDec(I32 v, U8 Len);
void GUI_DispSDecShift(I32 v, U8 Len, U8 Shift);

//浮点数显示

void GUI_DispFloat    (float v, char Len);
void GUI_DispFloatFix (float v, char Len, char Fract);
void GUI_DispFloatFixAt (float v, char Len, char Fract, I16P unit, I16P x , I16P y);
void GUI_DispFloatMin (float v, char Fract);
void GUI_DispSFloatFix(float v, char Len, char Fract);
void GUI_DispSFloatMin(float v, char Fract);

//LCD操作

void ReFresh_LCD24064(void);
void Lcd24064_Clr(void);
void LCD_24064ReFresh_EN(void);
void LCD24064_Refresh_Sever(void);
void LCD24064_Refresh_Rect(uint16_t sX,uint16_t sY,uint16_t eX,uint16_t eY);

#endif
