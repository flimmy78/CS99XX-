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
* Source file: del
* Dimensions:  24 * 11
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

static GUI_CONST_STORAGE GUI_COLOR Colorsdel[] = {
     0x000000,0xFFFFFF
};

static GUI_CONST_STORAGE GUI_LOGPALETTE Paldel = {
  2,	/* number of entries */
  0, 	/* No transparency */
  &Colorsdel[0]
};

static GUI_CONST_STORAGE unsigned char acdel[] = {
  __XXXXXX, XXXXXXXX, XXX_____,
  _X______, ________, ___X____,
  X____XXX, _______X, ____X___,
  X____X__, X__XX__X, ____X___,
  X____X__, X_X__X_X, ____X___,
  X____X__, X_XXXX_X, ____X___,
  X____X__, X_X____X, ____X___,
  X____X__, X_X__X_X, ____X___,
  X_____XX, ___XX__X, ____X___,
  _X______, ________, ___X____,
  __XXXXXX, XXXXXXXX, XXX_____
};

GUI_CONST_STORAGE GUI_BITMAP bmdel = {
  24, /* XSize */
  11, /* YSize */
  3, /* BytesPerLine */
  1, /* BitsPerPixel */
  acdel,  /* Pointer to picture data (indices) */
  &Paldel  /* Pointer to palette */
};

/* *** End of file *** */