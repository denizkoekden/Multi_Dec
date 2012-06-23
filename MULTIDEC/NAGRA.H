/*********************************************************************************
 *                                                                               *
 * nagra.h                                                                    *
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

#define ZEILENZAHL           287

#define INDEXSIZE           14000


#define TESTZEILEN            16

#define TESTPIXEL             32   //*4
// Anzahl der Möglichkeiten
#define KOMBINATIONEN      32768
// Kleinste Testzeile
#define KLEINSTE_ZEILE        96
// Größte Testzeile
#define GROESSTE_ZEILE    ZEILENZAHL-KLEINSTE_ZEILE-1

extern HWND hWnd;
extern int MasterTestzeilen;
extern int TZ[4];

int nTestzeilen;
int MoreTestZeilen;

extern HWND    Status_Bar;
extern HWND    hwndTextField, hwndPalField;
extern HWND    hwndKeyField,hwndFPSField;
extern int nPixelSchritt;
extern short HQTabelle[345*4*4];
extern const __int64  nMaske ;
extern int  nPALplusZeile  ;
extern int  nPALplusData   ;
extern int  BLData ;
extern int CurrentX;

extern __int64  nMinus128 ;
extern __int64  nFaktoren ;
extern __int64   nMaskeUV ;

extern BOOL NagraFullField;

short pPALplusCode[] = {  18,  27,  36,  45,  54,  63,  72,  81,  90, 100, 110, 120, 134, 149};
short pPALplusData[] = { 160, 178, 196, 214, 232, 250, 268, 286, 304, 322, 340, 358, 376, 394};
short nLevelLow      =  45;
short nLevelHigh     = 135;

short pTestPixel_firstfield[ZEILENZAHL][TESTPIXEL*2];

extern __int64  CurrentLowValue;

extern int HQLinebufferEven[846*4];
extern int HQLinebufferOdd[846*4];

extern int *pLinebufferEven ;
extern int *pLinebufferOdd  ;
extern int *pLinebuffer;

char	pKeys[256];
unsigned short *pKombinationen = NULL;

int	nPaarIndexAnzahl;
int	nPaarIndexLOW;
int	nPaarIndexHIGH;

extern BOOL  BlackSet[288];
BOOL *BlackSetPtr=&BlackSet[0]; 
unsigned int MatrixKey;

int LastMatrix[2]={0,0}; 

extern BOOL MatrixModeEven;
extern BOOL MatrixModeOdd;

extern __int64  One;


extern int  FirstWidthEven;
extern int  FirstWidthOdd;

extern BOOL Line286BlackEven;
extern BOOL Line286BlackOdd;
extern BOOL Line285BlackEven;
extern BOOL Line285BlackOdd;
extern BOOL Line1BlackEven;
extern BOOL Line1BlackOdd;
extern BOOL Line0BlackEven;
extern BOOL Line0BlackOdd;

extern int UntilEven;
extern int UntilOdd;

#ifdef USE_BT8X8
int *nPaarIndexA=NULL;
int *nPaarIndexB=NULL;
unsigned int nZeilenPaarKorrelation[INDEXSIZE]; 
int pSortLinesEven[ZEILENZAHL];
int pSortLinesOdd[ZEILENZAHL];

int    *nPaarNummer;
int    *nPaarDanach;
int    *nPaarDavor;
int    *nPaarTreffer;
int    *nPaarInsert;

int	nAnzahlPaare;

#endif
extern short *ScreenPtr;
extern short *TmpPtr;
extern short *ColFaktor;
extern int LinesAnzahl;
extern int BytesOut;
extern int CurrentPitch;

#ifdef USE_BT8X8
// Fantasio for Secam-Extensions
extern int CurrentFrame;
extern int MovieScale; 
extern int iUFilterThresholdPCent;
extern int iVFilterThresholdPCent;
extern BOOL bFilterBothRedBlue;
extern BOOL bSecamReorderUV;
extern BOOL bNoBlue,bNoRed,bNoGreen;
int itOffsetUGood[288];

#endif

struct TKEY {
	    char Name[20];
		char Extension[4];
};

extern struct TKEY OPKEY[4];

extern int ActiveKey;
extern BOOL Display_Status_Bar;
