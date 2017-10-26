/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*
* C-file generated by
*
*        �C/GUI-BitmapConvert V3.96.
*        Compiled Jul 19 2005, 13:50:35
*          (c) 2002-2005  Micrium, Inc.
  www.micrium.com

  (c) 1998-2005  Segger
  Microcontroller Systeme GmbH
  www.segger.com
*
**********************************************************************
*
* Source file: rightshift
* Dimensions:  24 * 13
* NumColors:   2
*
**********************************************************************
*/

#include "stdlib.h"

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/*   Palette
The following are the entries of the palette table.
Every entry is a 32-bit value (of which 24 bits are actually used)
the lower   8 bits represent the Red component,
the middle  8 bits represent the Green component,
the highest 8 bits (of the 24 bits used) represent the Blue component
as follows:   0xBBGGRR
*/

static GUI_CONST_STORAGE GUI_COLOR Colorsrightshift[] = {
     0x000000,0xFFFFFF
};

static GUI_CONST_STORAGE GUI_LOGPALETTE Palrightshift = {
  2,	/* number of entries */
  0, 	/* No transparency */
  &Colorsrightshift[0]
};

static GUI_CONST_STORAGE unsigned char acrightshift[] = {
  ________, ________, ________,
  _______X, X_______, ________,
  _______X, XXXX____, ________,
  _______X, XXXXXX__, ________,
  ___XXXXX, XXXXXXXX, XXXX____,
  __X____X, XXXXXX__, ____X___,
  _X_____X, XXXX____, _____X__,
  X______X, X_______, ______X_,
  X_______, ________, ______X_,
  X_______, ________, ______X_,
  _X______, ________, _____X__,
  __X_____, ________, ____X___,
  ___XXXXX, XXXXXXXX, XXXX____
};

GUI_CONST_STORAGE GUI_BITMAP bmrightshift = {
  24, /* XSize */
  13, /* YSize */
  3, /* BytesPerLine */
  1, /* BitsPerPixel */
  acrightshift,  /* Pointer to picture data (indices) */
  &Palrightshift  /* Pointer to palette */
};

/* *** End of file *** */