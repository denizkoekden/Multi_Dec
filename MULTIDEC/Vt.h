/*********************************************************************************
 *                                                                               *
 * vt.h                                                                          *
 *                                                                               *
 * Copyright (C) 1999/2000 Espresso                                              *
 *                                                                               *
 *                                                                               *
 * This program is free software; you can redistribute it and/or                 *
 * modify it under the terms of the GNU General Public License                   *
 * as published by the Free Software Foundation; either version 2                *
 * of the License, or (at your option) any later version.                        *
 *                                                                               *
 *                                                                               *          
 * This program is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 *
 * GNU General Public License for more details.                                  *
 *                                                                               *
 *                                                                               *
 * You should have received a copy of the GNU General Public License             *
 * along with this program; if not, write to the Free Software                   *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.    *
 * Or, point your browser to http://www.gnu.org/copyleft/gpl.html                *
 *                                                                               *
 *                                                                               *
 * The author can be reached at echter_espresso@hotmail.com                      *
 *********************************************************************************/ 

#include "globals.h"



#define VTTimeout 60000


#define _MakePage(mag,page) (((mag)?(mag):8)*100+((page)>>4)*10+((page)&0xf))

#define MAXVTDIALOG 8

struct TVT VTFrame[800];


int SubPage=0;

typedef struct TVTDialog {
	HWND Dialog;
	int  Page;
	int  SubPage;
	int  FramePos;
	int  FontNr;
	BOOL PageChange;
	unsigned char AsciiPuffer[26][40];
};

extern int VT_Lang;
extern int VT_Cache;
extern BYTE VT_Header_Line[40];
extern HWND ShowVTInfo;
extern int VBI_FPS;

extern int VideoSource;

extern struct TVTDialog VTDialog[MAXVTDIALOG];

extern BITMAPINFO *pDIB;

extern unsigned short UTPages[12];
extern unsigned short UTCount;

extern COLORREF VTColourTable[16];
extern BOOL Capture_VBI;
extern HWND hWnd;
char Text[128];


// Html - Export 
unsigned char Old_Htm_FG_Color;
unsigned short Old_Htm_Graph; 
BOOL Old_Htm_Blink;

extern int Initial_FontNr;
extern int Initial_Export_Font;
extern char VT_BASE_DIR[256];
extern char VT_EXPORT_WWW_ROOT[256];
extern BOOL VT_ALWAYS_EXPORT;
extern int VT_EXPORT_TYP;
extern BOOL VT_EXPORT_BACKGROUND;
extern BOOL VT_EXPORT_PAGE_LINK;
extern BOOL VT_EXPORT_PAGE_CONTROL;

extern int CurrentProgramm;

extern int VBI_Flags;
#define VBI_VT  1
#define VBI_VPS 2
#define VBI_VC  4
#define VBI_IC  8
#define VBI_VD  16
#define VBI_WINBIS  32

unsigned char Prepared_Bitmap_Header[118] = {0,0,0,0};
COLORREF Ist_Palette[16];
