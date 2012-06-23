/*********************************************************************************
 *                                                                               *
 * nagra.c                                                                       *
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

#include <windows.h>       /* required for all Windows applications */
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <process.h>       /* for _beginthread                      */
#include <stdlib.h>        /* atoi                                  */
#include <io.h>         
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ddraw.h>
#include "resource.h"
#include "nagra.h"

// Nagra
BOOL Init_Nagra_Decoding( int Lines );
void Free_Nagra_Decoding(void);

BOOL LeseIndexDatei( void );
BOOL SchreibeIndexDatei();
BOOL LeseKeyDatei(HWND hWnd);
void PaarMerken(int nPaarDavorIndex);
BOOL BaueIndex(void);

BOOL GetMatrix(int BlackWert,int evenodd );
void BaueMatrix(int BlackWert,int evenodd);
BOOL LeseMatrixDatei( int BlackWert,int evenodd );
BOOL SchreibeMatrixDatei(int BlackWert,int evenodd);
void Secam_UVValue(short *pQuelle, int Count , int *U, int *V);
void Secam_UVMix(short *pQuelle, short *pZiel ,int Count );

int PaarBewerten(int nPaarDavorIndex);
void BerechnePermutation(int   nKey,int  *pZeilen);
int SucheKeyPalPlus(int nPalPlusZeile);
int SucheKeyPalPlusMore( int nPalPlusZeile, BOOL Black1,BOOL Black285,BOOL Black286 );
int SucheKeyPalPlusMatrix( int   nPalPlusZeile,unsigned int MatrixKey ,int evenodd);

int SucheKeyBlack();
int SucheKeyBlackMore( BOOL Black1,BOOL Black285,BOOL Black286 );
int SucheKeyBlackMatrix(unsigned int MatrixKey,int evenodd);

void FindYUVLineOrder(short **pLinesAddress ,int *pSortLines,  int  *nPALplusZeile,int  *nPALplusData);
void FindYUVSecondLineOrder(short **pLinesAddress ,int *pSortLines);

BOOL APIENTRY NagraSetupProc(HWND hDlg,UINT message,UINT wParam,LONG lParam);

int FindePALplus(short **pLinesAddress,int  *nPALplusData);
int FindePalPlusZeile(short *pLine,int  *nPALplusData);

void Black_Matrix(short **pLinesAddress, int *OneLine,  int *Count ,unsigned int *MatrixKey,int FirstWidth,int Until);

#ifdef USE_BT8X8

void Free_Nagra_Decoding(void)
{
	if ( pKombinationen != NULL ) free(pKombinationen);
    pKombinationen=NULL;
	if ( nPaarIndexA != NULL ) free(nPaarIndexA);
    nPaarIndexA=NULL;
	if ( nPaarIndexB != NULL ) free(nPaarIndexB);
    nPaarIndexB=NULL;
}	

BOOL Init_Nagra_Decoding( int Lines )
{


    LastMatrix[0]=0;
    LastMatrix[1]=0;
if (!LeseKeyDatei(hWnd)) return(FALSE);
	Free_Nagra_Decoding();

	nTestzeilen = Lines;
	MoreTestZeilen= nTestzeilen+7;
	pKombinationen = calloc(KOMBINATIONEN*MoreTestZeilen,sizeof(unsigned short));
	if ( LeseIndexDatei() == FALSE ) {
      BaueIndex();
      SchreibeIndexDatei();
	}
	return(TRUE);
};


BOOL LeseKeyDatei(HWND hWnd)
{
   // Permutationstabelle aus Datei einlesen
   int   nZlr;
   int   nInputValue;
   FILE *sFile;
   char Name[128];
	  if (  OPKEY[ActiveKey].Name[0] == 0x00 ) {
		  sprintf(Name,"KEY-Set Nummer %d nicht definiert",ActiveKey);
          SetWindowText(hwndTextField,Name);
          MessageBeep(MB_ICONEXCLAMATION);
		  return(FALSE);
		};



	  sFile = fopen(OPKEY[ActiveKey].Name, "r");

   // Für den Fall das die Datei nicht gelesen werden kann,
   // pKey vorbelegen sonst gibts einen UAE
   for(nZlr=0; nZlr<256; nZlr++)
      pKeys[nZlr] = (char)(nZlr&0x1f);

   if(sFile == NULL)
   {
       sprintf(Name,"KEY-Datei %s Datei nicht gefunden",OPKEY[ActiveKey].Name);
       SetWindowText(hwndTextField,Name);
       MessageBeep(MB_ICONEXCLAMATION);
       ActiveKey=0;
	   return FALSE;
   }
   for(nZlr=0; nZlr<256; nZlr++)
   {
      if(fscanf(sFile, "%d\n", &nInputValue) == EOF)
      {
 	   sprintf(Name,"KEY-Datei %s fehlerhaft",OPKEY[ActiveKey].Name);
       SetWindowText(hwndTextField,Name);
       MessageBeep(MB_ICONEXCLAMATION);
       ActiveKey=0;
       ActiveKey=0;
	   fclose(sFile);
	  return FALSE;
      }
      pKeys[nZlr] = (char)(nInputValue&0x1f);
   }
   fclose(sFile);
   return TRUE;
}


BOOL GetMatrix(int BlackWert,int evenodd)
{
if ( LastMatrix[evenodd] == BlackWert ) return(TRUE);

	if ( LeseMatrixDatei(BlackWert,evenodd) == FALSE ) {
      BaueMatrix(BlackWert,evenodd);
      SchreibeMatrixDatei(BlackWert,evenodd);
	}
LastMatrix[evenodd] = BlackWert; 
return(TRUE);
}


void BaueMatrix(int BlackWert,int evenodd)
{
int nKey;
int pZeilen[ZEILENZAHL];
int i,j,k,l;
unsigned int *pM;

for(nKey=0; nKey<KOMBINATIONEN; nKey++)
   {
	   
      BerechnePermutation(nKey, pZeilen);
      pM=(unsigned int *)&pKombinationen[MoreTestZeilen*nKey+evenodd*2];
	  *pM=0;

	  for ( i=0; i < BlackWert; i++ ) {
		  j= pZeilen[i];
		  if ( j < 64 ) {
			  k=j/32;
			  l=( j - ( k*32 ));
               if ( k == 1 ) {
				   *pM=*pM+( 1 << l );
			  }
			  }
		  }

	  for ( i=ZEILENZAHL-BlackWert; i < ZEILENZAHL; i++ ) {
		  j= pZeilen[i];
		  if ( j < 64 ) {
			  k=j/32;
			  l=( j - ( k*32 ));
              if ( k == 1 ) {
				   *pM=*pM+( 1 << l );
			  }
			  }
		  }
		}
};


BOOL LeseMatrixDatei( int Matrixsize, int evenodd)
{
   FILE *sFile;
   int nKey;

   unsigned int *pM;
   unsigned int Matrix[KOMBINATIONEN];


   // Datei mit vorberechneten Tabellen einlesen
   char pDateiName[20];
   
   sprintf(pDateiName, "Matrix%02d.%s",Matrixsize,OPKEY[ActiveKey].Extension);

   // Datei zum lesen öffnen
   sFile = fopen(pDateiName, "rb");
   if(sFile == NULL) {
	   return FALSE;
	  }
   if ( fread(&Matrix[0], sizeof(unsigned int), KOMBINATIONEN, sFile) != (size_t)KOMBINATIONEN)  {
	   fclose(sFile);
      return FALSE;
   }
   fclose(sFile);



for(nKey=0; nKey<KOMBINATIONEN; nKey++)
   {
      pM=(unsigned int *)&pKombinationen[MoreTestZeilen*nKey+evenodd*2];
     *pM=Matrix[nKey];
};

return(TRUE);
};

BOOL SchreibeMatrixDatei( int Matrixsize,int evenodd)
{
   FILE *sFile;
   int nKey;

   // Datei mit vorberechneten Tabellen einlesen
   char pDateiName[20];
   unsigned int *pM;
   unsigned int Matrix;

   sprintf(pDateiName, "Matrix%02d.%s",Matrixsize,OPKEY[ActiveKey].Extension);

   // Datei zum schreiben öffnen
   sFile = fopen(pDateiName, "wb");
   if(sFile == NULL) {
	   return FALSE;
	  }
for(nKey=0; nKey<KOMBINATIONEN; nKey++)
   {
      pM=(unsigned int *)&pKombinationen[MoreTestZeilen*nKey+evenodd*2];
	Matrix=*pM;
   if(fwrite(&Matrix, sizeof(unsigned int), 1, sFile) != (size_t)1)
   {
	   fclose(sFile);
      return FALSE;
   }
};
   fclose(sFile);
return(TRUE);
};





BOOL BaueIndex()
{
   int nZeilenIndex;
   int nPaarDavorIndex;
   int nPaarDanachIndex;
   int nKey;
   int pZeilen[ZEILENZAHL];
   int nZeileA;
   int nZeileB;
   int nCount = 0;
   int nTreffer;
   int nMaxTreffer;


   char Text[128];
   MSG msg;
   
 
   nAnzahlPaare = GROESSTE_ZEILE - KLEINSTE_ZEILE;


   nPaarNummer    = calloc(KOMBINATIONEN,sizeof(int));
   // In dieser Tabelle stehen alle Testzeilen einer Kombination hintereinander
   nPaarDanach    = calloc(KOMBINATIONEN*nAnzahlPaare,sizeof(int));
   nPaarDavor     = calloc(ZEILENZAHL*ZEILENZAHL,sizeof(int));
   nPaarTreffer   = calloc(ZEILENZAHL*ZEILENZAHL,sizeof(int));
   nPaarTreffer   = calloc(ZEILENZAHL*ZEILENZAHL,sizeof(int));
   nPaarInsert    = calloc(ZEILENZAHL*ZEILENZAHL,sizeof(int));

   nPaarIndexA    = calloc(INDEXSIZE,sizeof(int));
   nPaarIndexB    = calloc(INDEXSIZE,sizeof(int));


   // Kennzeichen für nicht belegt
   for(nZeilenIndex=0; nZeilenIndex<ZEILENZAHL*ZEILENZAHL; nZeilenIndex++) {
      nPaarDavor[nZeilenIndex] = -1;
	  nPaarInsert[nZeilenIndex]= 0;
	  }

   // Anzahl Treffer auf Null setzen
   for(nZeilenIndex=0; nZeilenIndex<ZEILENZAHL*ZEILENZAHL; nZeilenIndex++)
      nPaarTreffer[nZeilenIndex] = 0;

 
   // Schleife über alle Kombination
   for(nKey=0; nKey<KOMBINATIONEN; nKey++)
   {
      // Zeilenfolge für diese Kombination finden
if ( Display_Status_Bar == TRUE ) {
	   if (( nKey&4 ) || ( nKey+1==KOMBINATIONEN)) {
        sprintf(Text,"%s Kombination %d ",OPKEY[ActiveKey].Name,nKey);
        SetWindowText(hwndTextField,Text);
       }
};
	    if ( PeekMessage(&msg,hwndTextField,0,0xffffffff,PM_REMOVE) == TRUE ) {
               			     SendMessage(hwndTextField,msg.message,msg.wParam,msg.lParam);
						   };
        if ( PeekMessage(&msg,hWnd,0,0xffffffff,PM_REMOVE) == TRUE ) {
               			     SendMessage(hWnd,msg.message,msg.wParam,msg.lParam);
			};

	   
      BerechnePermutation(nKey, pZeilen);
	  pKombinationen[MoreTestZeilen*nKey+4] = (unsigned short)pZeilen[1];
	  pKombinationen[MoreTestZeilen*nKey+5] = (unsigned short)pZeilen[286];
	  pKombinationen[MoreTestZeilen*nKey+6] = (unsigned short)pZeilen[285];

	  // Erste Zeile eines Paars
      nZeileA = pZeilen[KLEINSTE_ZEILE];
      for(nZeilenIndex=KLEINSTE_ZEILE+1; nZeilenIndex<=GROESSTE_ZEILE; nZeilenIndex++)
      {
         // Zweite Zeile eines Paars
         nZeileB = pZeilen[nZeilenIndex];

         // Index immer aus "kleine Zeile * Zeilenzahl + große Zeile" berechnen
         if(nZeileB < nZeileA)
            nPaarDavorIndex = nZeileB*ZEILENZAHL + nZeileA;
         else
            nPaarDavorIndex = nZeileA*ZEILENZAHL + nZeileB;

         // Index auf die Paar danach Tabelle
         nPaarDanachIndex = nKey*nAnzahlPaare + nZeilenIndex - (KLEINSTE_ZEILE+1);

         // Zeiger auf das Paar danach
         nPaarDanach[nPaarDanachIndex] = nPaarDavor[nPaarDavorIndex];
         
         // Zeiger auf das Paar davor
         nPaarDavor[nPaarDavorIndex] = nPaarDanachIndex;

         // Anzahl der Treffer von diesem Paars erhöhen
         nPaarTreffer[nPaarDavorIndex]++;


         // Erste Zeile ist gleich zweiter Zeile
         nZeileA = nZeileB;
      }
   }

   for(nZeilenIndex=0; nZeilenIndex<KOMBINATIONEN; nZeilenIndex++)
      nPaarNummer[nZeilenIndex] = 0;


   nPaarIndexAnzahl = 0;
   do
   {
      // Das Paar mit den meisten Treffern suchen
      nMaxTreffer = 0;
      nPaarDavorIndex = 0;
      for(nZeilenIndex=0; nZeilenIndex<ZEILENZAHL*ZEILENZAHL; nZeilenIndex++)
      {
         if(nPaarTreffer[nZeilenIndex] > nMaxTreffer)
         {
            nMaxTreffer = nPaarTreffer[nZeilenIndex];
            nPaarDavorIndex = nZeilenIndex;
         }
      }

      // Wenn das Paar benutzt wird dann merken
      nTreffer = PaarBewerten(nPaarDavorIndex);
      if(nTreffer > 0)
      {
         nCount += nTreffer;
         PaarMerken(nPaarDavorIndex);
      }

      nPaarTreffer[nPaarDavorIndex] = 0;
   }
   while(nCount < KOMBINATIONEN*nTestzeilen);


   nPaarIndexLOW=0xffff;
   nPaarIndexHIGH=0x0000;

   for ( nCount = 0 ; nCount < nPaarIndexAnzahl ; nCount++ ) {
	 if ( nPaarIndexA[nCount] < nPaarIndexLOW ) nPaarIndexLOW=nPaarIndexA[nCount];
	 if ( nPaarIndexB[nCount] < nPaarIndexLOW ) nPaarIndexLOW=nPaarIndexB[nCount];
	 if ( nPaarIndexA[nCount] > nPaarIndexHIGH ) nPaarIndexHIGH=nPaarIndexA[nCount];
	 if ( nPaarIndexB[nCount] > nPaarIndexHIGH ) nPaarIndexHIGH=nPaarIndexB[nCount];
   }


   
   free(nPaarNummer);
   free(nPaarTreffer);
   free(nPaarDavor);
   free(nPaarDanach);
   return(TRUE);
}



void BerechnePermutation(int   nKey,int *pZeilen)
{
   int   nStartwert = nKey>>7;
   int   nSchrittweite = (nKey&0x7f)*2+1;
   int   nZeile;
   int   nBuffer[32];
   //
   // Diese Funktion berechnet die Zeilenpermutation für eine Kombination 
   // aus nStartwert und nSchrittweite.
   //
   for(nZeile=0; nZeile<32; nZeile++)
      nBuffer[nZeile] = nZeile;

   for(nZeile=0; nZeile<ZEILENZAHL-32; nZeile++)
   {
      pZeilen[nZeile] = nBuffer[pKeys[nStartwert]];
      nBuffer[pKeys[nStartwert]] = nZeile+32;
      nStartwert += nSchrittweite;
      nStartwert &= 0xff;
   }

   for(nZeile=0;nZeile<32;nZeile++)
      pZeilen[nZeile+ZEILENZAHL-32] = nBuffer[nZeile];

}



void FindYUVLineOrder(short **pLinesAddress ,int *pSortLines, int  *nPALplusZeile,int  *nPALplusData)
{


   short *pTestPixelptr = &pTestPixel_firstfield[nPaarIndexLOW][0];
   short *m1,*m2;
   unsigned int nKorrelation;
  int j=0;

*nPALplusZeile = FindePALplus(pLinesAddress, nPALplusData);
if (( Line0BlackEven == TRUE ) || (FirstWidthEven > 3 ) || ( Line1BlackEven == TRUE ) || ( Line285BlackEven == TRUE ) || ( Line286BlackEven == TRUE )) {
Black_Matrix(pLinesAddress, &j, &BLData,&MatrixKey,FirstWidthEven,UntilEven);
if (( BLData == 1) && (*nPALplusZeile < 0 )) {
	                  *nPALplusZeile =j;
}
} else {
	BLData=0;
	FirstWidthEven=0;
};
_asm {
	   movq  mm4,qword ptr[nMaske] 
       mov   edx,nPaarIndexLOW
       mov   eax,pLinesAddress
	   mov   edi,dword ptr [eax+4*edx]
       mov   eax,pTestPixelptr
	   mov   esi,eax
       mov   ecx,4
	   mov   eax,nPixelSchritt
       mov   ebx,eax
	   shr   eax,1
	   movd  mm1,eax
	   jmp   MoreToGet
NextLine:
	   mov   ecx,4
	   mov   edi,dword ptr [eax+4*edx]
       movd  eax,mm1
MoreToGet: 
	   movq  mm0,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm0,mm4
	   movq  mm3,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm3,mm4
	   movq  mm5,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm5,mm4
	   movq  mm6,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm6,mm4
	   packuswb mm0,mm3
       packuswb mm5,mm6
	   movq  mm3,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm3,mm4
	   movq  mm6,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm6,mm4
	   movq  mm7,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm7,mm4
       packuswb mm3,mm6
	   movq  mm6,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm6,mm4
       packuswb mm6,mm7
	   movq  qword ptr [esi],mm0
	   movq  qword ptr [esi+8],mm3
	   movq  qword ptr [esi+16],mm5
	   movq  qword ptr [esi+24],mm6
       add   esi,32
	   loop  MoreToGet
       inc   edx
       mov   eax,pLinesAddress
       cmp   edx,nPaarIndexHIGH
	   jle    NextLine
   }

for(j=0; j<nPaarIndexAnzahl; j++)
{
m1=&pTestPixel_firstfield[nPaarIndexA[j]][0];
m2=&pTestPixel_firstfield[nPaarIndexB[j]][0];

	_asm {
 	    mov        ecx, 0x10
		mov        edx, m1
        mov        ebx, m2
        pxor       mm7, mm7
        pxor       mm4, mm4

NextQuadPixel:
		movq       mm0, qword ptr [edx+8*ecx-8] 
		movq       mm1, qword ptr [edx+8*ecx-16]
		movq       mm5, qword ptr [ebx+8*ecx-8] 
		movq       mm6, qword ptr [ebx+8*ecx-16] 
        dec        ecx
        movq       mm2, mm0             
        movq       mm3, mm1             
        psubusb    mm0, mm5             
        psubusb    mm1, mm6             
        psubusb    mm5, mm2
        psubusb    mm6, mm3
        por        mm0,mm5
        por        mm1,mm6
        movq       mm2,mm0
        movq       mm3,mm1
        punpckhbw  mm0,mm4
        punpckhbw  mm1,mm4
        punpcklbw  mm2,mm4
        punpcklbw  mm3,mm4
        paddw      mm0,mm1
        paddw      mm2,mm3
		paddw      mm7, mm0
        paddw      mm7, mm2
        loop       NextQuadPixel
        
        pmaddwd	   mm7,qword ptr[One] 
		movq       mm0, mm7
        psrlq      mm7, 32 
	    paddd      mm7, mm0	
		movd       nKorrelation,mm7
		emms
};
nZeilenPaarKorrelation[j] = nKorrelation;
};

  if ( *nPALplusZeile >= 0 ) {
    if (MatrixModeEven) BerechnePermutation(SucheKeyPalPlusMatrix(*nPALplusZeile,MatrixKey,0) , pSortLines); 
	else if  ((Line1BlackEven)   || 
			  (Line285BlackEven)   || 
		      (Line286BlackEven)) BerechnePermutation(SucheKeyPalPlusMore(*nPALplusZeile,Line1BlackEven,Line285BlackEven,Line286BlackEven) , pSortLines);  
		    else BerechnePermutation(SucheKeyPalPlus(*nPALplusZeile) , pSortLines);
} else {
	if (MatrixModeEven) BerechnePermutation(SucheKeyBlackMatrix(MatrixKey,0) , pSortLines); 
	else if  ((Line1BlackEven)   || 
			  (Line285BlackEven)   || 
 			  (Line286BlackEven)) BerechnePermutation(SucheKeyBlackMore(Line1BlackEven,Line285BlackEven,Line286BlackEven) , pSortLines);
	   else BerechnePermutation(SucheKeyBlack() , pSortLines);
		   
}
}


void FindYUVSecondLineOrder(short **pLinesAddress ,int *pSortLines)
{
   short *pTestPixelptr = &pTestPixel_firstfield[nPaarIndexLOW][0];
   short *m1,*m2;
   unsigned int nKorrelation;
   int j;
   int mp;

mp=-1;
if (( Line0BlackOdd == TRUE ) || (FirstWidthOdd > 3 ) || ( Line1BlackOdd == TRUE ) || ( Line285BlackOdd == TRUE ) || ( Line286BlackOdd == TRUE )) {
Black_Matrix(pLinesAddress, &j, &BLData,&MatrixKey,FirstWidthOdd,UntilOdd);
if ( Line0BlackOdd == FALSE )  BLData=0;
if ( BLData==1 ) mp =j; 
} else {
BLData=0;
FirstWidthOdd=0;
}


_asm {
	   movq  mm4,qword ptr[nMaske] 
       mov   edx,nPaarIndexLOW
       mov   eax,pLinesAddress
	   mov   edi,dword ptr [eax+4*edx]
       mov   eax,pTestPixelptr
	   mov   esi,eax
       mov   ecx,4
	   mov   eax,nPixelSchritt
       mov   ebx,eax
	   shr   eax,1
	   movd  mm1,eax
	   jmp   MoreToGet
NextLine:
	   mov   ecx,4
	   mov   edi,dword ptr [eax+4*edx]
       movd  eax,mm1
MoreToGet: 
	   movq  mm0,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm0,mm4
	   movq  mm3,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm3,mm4
	   movq  mm5,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm5,mm4
	   movq  mm6,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm6,mm4
	   packuswb mm0,mm3
       packuswb mm5,mm6
	   movq  mm3,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm3,mm4
	   movq  mm6,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm6,mm4
	   movq  mm7,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm7,mm4
       packuswb mm3,mm6
	   movq  mm6,qword ptr [edi+4*eax];
       add   eax,ebx
	   pand  mm6,mm4
       packuswb mm6,mm7
	   movq  qword ptr [esi],mm0
	   movq  qword ptr [esi+8],mm3
	   movq  qword ptr [esi+16],mm5
	   movq  qword ptr [esi+24],mm6
       add   esi,32
	   loop  MoreToGet
       inc   edx
       mov   eax,pLinesAddress
       cmp   edx,nPaarIndexHIGH
	   jle    NextLine
   }

for(j=0; j<nPaarIndexAnzahl; j++)
{
m1=&pTestPixel_firstfield[nPaarIndexA[j]][0];
m2=&pTestPixel_firstfield[nPaarIndexB[j]][0];

	_asm {
 	    mov        ecx, 0x10
		mov        edx, m1
        mov        ebx, m2
        pxor       mm7, mm7
        pxor       mm4, mm4

NextQuadPixel:
		movq       mm0, qword ptr [edx+8*ecx-8] 
		movq       mm1, qword ptr [edx+8*ecx-16]
		movq       mm5, qword ptr [ebx+8*ecx-8] 
		movq       mm6, qword ptr [ebx+8*ecx-16] 
        dec        ecx
        movq       mm2, mm0             
        movq       mm3, mm1             
        psubusb    mm0, mm5             
        psubusb    mm1, mm6             
        psubusb    mm5, mm2
        psubusb    mm6, mm3
        por        mm0,mm5
        por        mm1,mm6
        movq       mm2,mm0
        movq       mm3,mm1
        punpckhbw  mm0,mm4
        punpckhbw  mm1,mm4
        punpcklbw  mm2,mm4
        punpcklbw  mm3,mm4
        paddw      mm0,mm1
        paddw      mm2,mm3
		paddw      mm7, mm0
        paddw      mm7, mm2
        loop       NextQuadPixel
        
        pmaddwd	   mm7,qword ptr[One] 
		movq       mm0, mm7
        psrlq      mm7, 32 
	    paddd      mm7, mm0	
		movd       nKorrelation,mm7
		emms
};
nZeilenPaarKorrelation[j] = nKorrelation;
};

if ( mp >= 0 ) {
    if (MatrixModeOdd) BerechnePermutation(SucheKeyPalPlusMatrix(mp,MatrixKey,1) , pSortLines); 
	else if  ((Line1BlackOdd)   || 
			  (Line285BlackOdd)   || 
		      (Line286BlackOdd)) BerechnePermutation(SucheKeyPalPlusMore(mp,Line1BlackOdd,Line285BlackOdd,Line286BlackOdd) , pSortLines);  
              else BerechnePermutation(SucheKeyPalPlus(mp) , pSortLines);

				  
} else {
	if (MatrixModeOdd) BerechnePermutation(SucheKeyBlackMatrix(MatrixKey,1) , pSortLines); 
	else if  ((Line1BlackOdd)   || 
			  (Line285BlackOdd)   || 
			  (Line286BlackOdd)) BerechnePermutation(SucheKeyBlackMore(Line1BlackOdd,Line285BlackOdd,Line286BlackOdd) , pSortLines);
				else BerechnePermutation(SucheKeyBlack() , pSortLines);
}
}


int FindePALplus(short **pLinesAddress,int  *nPALplusData)
{
   int      nBit;
   int      nPixel;
   int      nZeile ;
   int      nOffset;
   short   *pLine;

   // Daten initialisieren
   *nPALplusData = 0;
   // Die 32 möglichen Zeilen durchsuchen
   for(nZeile=0; nZeile<32; nZeile++)
   {  
      pLine = (short*)&pLinesAddress[nZeile][0];

      // Auf das Startbit warten
      nOffset = 0;
      for(nPixel=0; nPixel<20*CurrentX/768; nPixel++)
      {
         // Startbit gefunden
         if((*(pLine+nOffset)&0xff) > nLevelHigh)
            break;
         
         // Abstand Zeilenanfang bis Startbit
         nOffset++;
      }

      // Bitcode überprüfen
      for(nPixel=0; nPixel<14; nPixel+=2)
      {
          if((*(pLine+nOffset+pPALplusCode[nPixel]*CurrentX/768)&0xff) > nLevelLow)
		  break;

         if((*(pLine+nOffset+pPALplusCode[nPixel+1]*CurrentX/768)&0xff) < nLevelHigh)

		  break;
      }
      // Wenn alle 7 Bit richtig sind dann wurde die Zeile gefunden
      if(nPixel == 14)
      {  
         // Daten auslesen
         nBit = 0x0001;

         for(nPixel=0; nPixel<14; nPixel++)
         {

			 if((*(pLine+nOffset+pPALplusData[nPixel]*CurrentX/768)&0xff) > nLevelHigh)

			 *nPALplusData |= nBit;

            nBit <<= 1;
         }

         return nZeile;
      }
   }

 	  // Zeile nicht gefunden
return -1;
}


void Black_Matrix(short **pLinesAddress, int *OneLine, int *Count ,unsigned int *MatrixKey,int FirstWidth,int Until)
{
int AnzahlSchwarzeZeilen=0;

const __int64  ShiftAdd = 0x0000000080000000;
_asm {
	   movq  mm5,qword ptr[CurrentLowValue]
	   movq  mm4,qword ptr[nMaske] 
       movq  mm6,qword ptr[ShiftAdd]
	   xor   edx,edx
       mov   dword ptr BlackSet[edx*4],0
       mov   eax,pLinesAddress
	   mov   edi,dword ptr [eax+4*edx]
       mov   ecx,FirstWidth
	   movd  mm7,ecx
	   mov   eax,nPixelSchritt
	   shr   eax,1
	   mov   esi,eax
	   jmp   MoreToGet

NextLine:
       mov   dword ptr BlackSet[edx*4],0
       mov   eax,pLinesAddress
       movd  ecx,mm7
	   mov   edi,dword ptr [eax+4*edx]
	   mov   eax,esi

MoreToGet: 
	   movq  mm0,qword ptr [edi+4*eax];
       pand  mm0,mm4
       psubusb  mm0,mm5   
	   add   eax,nPixelSchritt
       packsswb mm0,mm0
       movd   ebx,mm0
	   cmp    ebx,0
	   jnz    NichtSchwarz
	   loop   MoreToGet
       mov    dword ptr BlackSet[edx*4],1
       inc   AnzahlSchwarzeZeilen
       mov   ebx,dword ptr [OneLine]
	   mov   dword ptr [ebx],edx 
	   
NichtSchwarz:
	   inc   edx
       cmp   edx,32
	   jl    NextLine
	   mov   ebx,dword ptr [Count]
	   mov   ecx,AnzahlSchwarzeZeilen
       mov   dword ptr [ebx],ecx 

       cmp   Until,1
       jz    Ende 
// Next 32 für MatrixKey
       mov   ecx,0x10
	   pxor  mm3,mm3
       mov   dword ptr BlackSet[edx*4],0
       mov   eax,pLinesAddress
	   mov   edi,dword ptr [eax+4*edx]
	   mov   eax,esi
	   jmp   MoreToGet1

NextLine1:
       psrld mm3,1 
       mov   dword ptr BlackSet[edx*4],0
       mov   eax,pLinesAddress
	   mov   ecx,0x10
	   mov   edi,dword ptr [eax+4*edx]
	   mov   eax,esi

MoreToGet1: 
	   movq  mm0,qword ptr [edi+4*eax];
	   add   eax,nPixelSchritt
       pand  mm0,mm4
       psubusb  mm0,mm5   
	   add   eax,nPixelSchritt
       packsswb mm0,mm0
       movd   ebx,mm0
	   cmp    ebx,0
	   jnz    NichtSchwarz1
	   loop   MoreToGet1
       paddw mm3,mm6
       mov    dword ptr BlackSet[edx*4],1
	   
NichtSchwarz1:
	   inc   edx
       cmp   edx,64
	   jl    NextLine1
       mov   eax,dword ptr [MatrixKey];
	   movd  dword ptr [eax],mm3


// Rest 

       mov   dword ptr BlackSet[edx*4],0
       mov   eax,pLinesAddress
	   mov   ecx,0x10
	   mov   edi,dword ptr [eax+4*edx]
	   mov   eax,esi
	   jmp   MoreToGet2

NextLine2:
       mov   dword ptr BlackSet[edx*4],0
       mov   eax,pLinesAddress
	   mov   ecx,0x10
	   mov   edi,dword ptr [eax+4*edx]
	   mov   eax,esi
	   
MoreToGet2: 
	   movq  mm0,qword ptr [edi+4*eax];
	   add   eax,nPixelSchritt
       pand  mm0,mm4
       psubusb  mm0,mm5   
	   add   eax,nPixelSchritt
       packsswb mm0,mm0
       movd   ebx,mm0
	   cmp    ebx,0
	   jnz    NichtSchwarz2
	   loop   MoreToGet2
       mov    dword ptr BlackSet[edx*4],1
	   
NichtSchwarz2:
	   inc   edx
       cmp   edx,287
	   jl    NextLine2
Ende:
	   emms
   }
  
}
#endif


int FindePalPlusZeile(short *pLine,int  *nPALplusData)
{
   int      nBit;
   int      nPixel;
   int      nZeile=-1;
   int      nOffset;

   // Daten initialisieren
   *nPALplusData = 0;
   // Die 32 möglichen Zeilen durchsuchen
      // Auf das Startbit warten
      nOffset = 0;
      for(nPixel=0; nPixel<20*CurrentX/768; nPixel++)
//      for(nPixel=0; nPixel<20; nPixel++)
      {
         // Startbit gefunden
         if((*(pLine+nOffset)&0xff) > nLevelHigh)
            break;
         
         // Abstand Zeilenanfang bis Startbit
         nOffset++;
      }

      // Bitcode überprüfen
      for(nPixel=0; nPixel<14; nPixel+=2)
      {
         // Für andere horizontale Auflösungen als 384, Pixelabstand angleichen
//         if((*(pLine+nOffset+pPALplusCode[nPixel]*m_nBreiteQuelle/768)&0xff) > nLow)
          if((*(pLine+nOffset+pPALplusCode[nPixel]*CurrentX/768)&0xff) > nLevelLow)
		  break;

//         if((*(pLine+nOffset+pPALplusCode[nPixel+1]*m_nBreiteQuelle/768)&0xff) < nHigh)
         if((*(pLine+nOffset+pPALplusCode[nPixel+1]*CurrentX/768)&0xff) < nLevelHigh)

		  break;
      }
      // Wenn alle 7 Bit richtig sind dann wurde die Zeile gefunden
      if(nPixel == 14)
      {  
         // Daten auslesen
         nBit = 0x0001;

         for(nPixel=0; nPixel<14; nPixel++)
         {
//            if((*(pLine+nOffset+pPALplusData[nPixel]*m_nBreiteQuelle/768)&0xff) > nHigh)

			 if((*(pLine+nOffset+pPALplusData[nPixel]*CurrentX/768)&0xff) > nLevelHigh)

			 *nPALplusData |= nBit;

            nBit <<= 1;
         }
         return nZeile;
      }

return -1;
}


#ifdef USE_BT8X8

BOOL LeseIndexDatei()
{
   FILE *sFile;
   int fd;
   unsigned int nbytes;

   // Datei mit vorberechneten Tabellen einlesen
   char pDateiName[20];


   // Dateiname erzeugen
   sprintf(pDateiName, "Nagra%d.%s", nTestzeilen,OPKEY[ActiveKey].Extension);

   // Datei zum Lesen öffnen
   sFile = fopen(pDateiName, "rb");

   // Wenn die Datei nicht vorhanden ist, Tabellen neu berechnen
   if(sFile == NULL)
	   return(FALSE);

    fd = open(pDateiName, _O_RDONLY | _O_BINARY);
    if ( fd < 0 ) {
 	    fclose(sFile);
		return(FALSE);
	}
	nbytes=_filelength(fd);
    close(fd);

   
   if(fread(&nPaarIndexAnzahl, sizeof(int), 1, sFile) != 1)
   {
    
	   fclose(sFile);
      return (FALSE);
   }



   if ( (signed)nbytes != 12+(8*nPaarIndexAnzahl)+(KOMBINATIONEN*MoreTestZeilen*2) ) {
	   fclose(sFile);
       return (FALSE);
   }


   nPaarIndexA    = calloc(nPaarIndexAnzahl,sizeof(int));
   nPaarIndexB    = calloc(nPaarIndexAnzahl,sizeof(int));

   if(fread(&nPaarIndexLOW, sizeof(int), 1, sFile) != 1)
   {
	   fclose(sFile);
	   return FALSE;
   }
   if(fread(&nPaarIndexHIGH, sizeof(int), 1, sFile) != 1)
   {
	   fclose(sFile);
	   return FALSE;
   }
   // Daten einlesen      
   if(fread(pKombinationen, sizeof(unsigned short), KOMBINATIONEN*MoreTestZeilen, sFile) != (size_t)(KOMBINATIONEN*MoreTestZeilen))
   {
      fclose(sFile);
      return (FALSE);
   }

   if(fread(nPaarIndexA, sizeof(int), nPaarIndexAnzahl, sFile) !=(size_t) nPaarIndexAnzahl)
   {
      fclose(sFile);
      return (FALSE);
   }

   if(fread(nPaarIndexB, sizeof(int), nPaarIndexAnzahl, sFile) !=(size_t) nPaarIndexAnzahl)
   {
      fclose(sFile);
      return (FALSE);
   }
   fclose(sFile);
 return (TRUE);
}



int PaarBewerten(int nPaarDavorIndex)
{
   int nKey;
   int nTreffer = 0;
   int nNaechstesPaarIndex = nPaarDavor[nPaarDavorIndex];

   while(nNaechstesPaarIndex != -1)
   {
      nKey = nNaechstesPaarIndex/nAnzahlPaare;

      if(nPaarNummer[nKey] < nTestzeilen)
         nTreffer++;

      nNaechstesPaarIndex = nPaarDanach[nNaechstesPaarIndex];
   }

   return nTreffer;
}


void PaarMerken(int nPaarDavorIndex)
{
   int nNaechstesPaarIndex;
   int nKey;
   int q;
   int nZeileA;
   int nZeileB;
   int nb;

   nZeileA = nPaarDavorIndex / ZEILENZAHL;
   nZeileB = nPaarDavorIndex % ZEILENZAHL;
   nPaarIndexA[nPaarIndexAnzahl] = nZeileA;
   nPaarIndexB[nPaarIndexAnzahl] = nZeileB;
 

   nNaechstesPaarIndex = nPaarDavor[nPaarDavorIndex];
   while(nNaechstesPaarIndex != -1)
   {
      nKey = nNaechstesPaarIndex/nAnzahlPaare;
      nb = nPaarNummer[nKey]++;
      q = nPaarDanach[nNaechstesPaarIndex];

      if(nb < nTestzeilen) {
		  pKombinationen[MoreTestZeilen*nKey+nb+7] = (unsigned short)nPaarIndexAnzahl;
	  }
	   nNaechstesPaarIndex = q;
   }

   nPaarIndexAnzahl++;
}




BOOL SchreibeIndexDatei()
{
   FILE *sFile;
   char Text[128];

   // Vorberechneten Tabellen in Datei schreiben
   char pDateiName[20];
   // Dateiname erzeugen

   sprintf(pDateiName, "Nagra%d.%s", nTestzeilen,OPKEY[ActiveKey].Extension);

   // Datei zum Schreiben öffnen
   sFile = fopen(pDateiName, "wb");

   if(sFile == NULL) {
       MessageBox(hWnd,"Nagra-Index-Datei kann nicht geschrieben werden","Multi-Decode",MB_ICONSTOP|MB_OK);
	   return FALSE;
	  }

   if(fwrite(&nPaarIndexAnzahl, sizeof(int), 1, sFile) != 1)
   {
       MessageBox(hWnd,"Fehler beim Schreiben der Nagra-Index-Datei","Multi-Decode",MB_ICONSTOP|MB_OK);
	   fclose(sFile);
       remove(pDateiName);
	   return FALSE;
   }

   if(fwrite(&nPaarIndexLOW, sizeof(int), 1, sFile) != 1)
   {
       MessageBox(hWnd,"Fehler beim Schreiben der Nagra-Index-Datei","Multi-Decode",MB_ICONSTOP|MB_OK);
	   fclose(sFile);
       remove(pDateiName);
	   return FALSE;
   }
   
   if(fwrite(&nPaarIndexHIGH, sizeof(int), 1, sFile) != 1)
   {
       MessageBox(hWnd,"Fehler beim Schreiben der Nagra-Index-Datei","Multi-Decode",MB_ICONSTOP|MB_OK);
	   fclose(sFile);
       remove(pDateiName);
	   return FALSE;
   }


   if(fwrite(pKombinationen, sizeof(unsigned short), KOMBINATIONEN*MoreTestZeilen, sFile) != (size_t)KOMBINATIONEN*MoreTestZeilen)
   {
       MessageBox(hWnd,"Fehler beim Schreiben der Nagra-Index-Datei","Multi-Decode",MB_ICONSTOP|MB_OK);
	   fclose(sFile);
	    remove(pDateiName);
      return FALSE;
   }


   if(fwrite(nPaarIndexA, sizeof(int), nPaarIndexAnzahl, sFile) != (size_t)nPaarIndexAnzahl)
   {
       MessageBox(hWnd,"Fehler beim Schreiben der Nagra-Index-Datei ","Multi-Decode",MB_ICONSTOP|MB_OK);
	   fclose(sFile);
       remove(pDateiName);
	   return FALSE;
   }

   if(fwrite(nPaarIndexB, sizeof(int), nPaarIndexAnzahl, sFile) != (size_t)nPaarIndexAnzahl)
   {
       MessageBox(hWnd,"Fehler beim Schreiben der Nagra-Index-Datei ","Multi-Decode",MB_ICONSTOP|MB_OK);
	   fclose(sFile);
       remove(pDateiName);
	   return FALSE;
   }

   fclose(sFile);
   sprintf(Text,"Schreibe Index-Datei ");
   SetWindowText(hwndTextField,Text);

   return TRUE;
}


int SucheKeyPalPlus(  int   nPalPlusZeile )
{
   int      nBesterKandidat=0;
   int      nStartwert=0;
   int      nSchrittweite=0;
   int      mt=MoreTestZeilen*2;
   int      mts=mt*128;
   int      nt=nTestzeilen/2;

   __asm {
	      mov edi,pKombinationen
          add edi,14
          mov esi,0x0fffffff
InitLoopMitPalPlus:
          mov eax,nStartwert
		  mov edx,nPalPlusZeile
		  cmp edx,0
		  jl  Init_Correl_2
LoopMitPalPlus:  
		  movsx ecx, byte ptr pKeys[eax]
		  cmp ecx,edx
          je  Init_Correl
          add edi,mts
		  inc eax
		  cmp eax,256
		  jz Ende
		  jmp LoopMitPalPlus

Init_Correl:
          mov nStartwert,eax
Init_Correl_2:
          mov nSchrittweite,0
Do_Correl:
		  xor ecx,ecx
          xor eax,eax

SchrittLoop:
          mov ebx,dword ptr [edi+4*ecx]
          mov edx,ebx
		  and ebx,0x0000ffff;
		  shr edx,16
		  add eax,dword ptr nZeilenPaarKorrelation[4*ebx]
		  add eax,dword ptr nZeilenPaarKorrelation[4*edx]
          inc   ecx
		  cmp eax,esi
		  jge  ZuViel
          cmp ecx,nt
		  jl  SchrittLoop
		  mov esi,eax

          mov ebx,nSchrittweite
		  mov eax,nStartwert
		  shl eax,7
		  add eax,ebx
		  mov nBesterKandidat,eax
ZuViel:
          add edi,mt
          inc nSchrittweite
		  cmp nSchrittweite,128
		  jl Do_Correl
          inc nStartwert
		  cmp nStartwert,256
		  jl InitLoopMitPalPlus
Ende:   
   };
  return (nBesterKandidat);
  }

int SucheKeyBlack(  )
{
   int      nBesterKandidat=0;
   int      nStartwert=0;
   int      nSchrittweite=0;
   int      nt=nTestzeilen/2;
   int      mt=MoreTestZeilen*2;
   int      mts=mt*128;

   __asm {
	      mov edi,pKombinationen
          mov esi,0x0fffffff
          add edi,14

InitLoopMitSchwarz:
          mov eax,nStartwert
		  cmp BLData,0
		  jle Init_Correl_2
LoopMitSchwarz:  
		  movsx ecx, byte ptr pKeys[eax]
          cmp dword ptr BlackSet[4*ecx],1
          je  Init_Correl
          add edi,mts
		  inc eax
		  cmp eax,256
		  jz Ende
		  jmp LoopMitSchwarz

Init_Correl:
          mov nStartwert,eax
Init_Correl_2:
          mov nSchrittweite,0
Do_Correl:
		  xor ecx,ecx
          xor eax,eax
SchrittLoop:
          mov ebx,dword ptr [edi+4*ecx]
          mov edx,ebx
		  and ebx,0x0000ffff;
		  shr edx,16
          inc   ecx
		  add eax,dword ptr nZeilenPaarKorrelation[4*ebx]
		  add eax,dword ptr nZeilenPaarKorrelation[4*edx]
		  cmp eax,esi
		  jge  ZuViel
          cmp ecx,nt
		  jl  SchrittLoop
		  mov esi,eax
          mov ebx,nSchrittweite
		  mov eax,nStartwert
		  shl eax,7
		  add eax,ebx
		  mov nBesterKandidat,eax
ZuViel:

          add edi,mt
          inc nSchrittweite
		  cmp nSchrittweite,128
		  jl Do_Correl
          inc nStartwert
		  cmp nStartwert,256
		  jl InitLoopMitSchwarz 
Ende:   
   };
  return (nBesterKandidat);
  }




int SucheKeyPalPlusMatrix( int   nPalPlusZeile,unsigned int MatrixKey ,int evenodd)
{
   int      nBesterKandidat=0;
   int      nStartwert=0;
   int      nSchrittweite=0;
   int      mt=MoreTestZeilen*2;
   int      mts=MoreTestZeilen*256;
   int      nt=nTestzeilen/2;

   __asm {
          mov esi,0x0fffffff
	      mov edi,pKombinationen

InitLoopMitPalPlus:
          mov eax,nStartwert
		  mov edx,nPalPlusZeile
		  cmp edx,0
		  jl  Init_Correl_2
LoopMitPalPlus:  
		  movsx ecx, byte ptr pKeys[eax]
		  cmp ecx,edx
          je  Init_Correl
		  inc eax
          add edi,mts
		  cmp eax,256
		  jz Ende
		  jmp LoopMitPalPlus

Init_Correl:
          mov nStartwert,eax
Init_Correl_2:
          mov nSchrittweite,0
Do_Correl:
		  mov ebx,MatrixKey
          mov eax,evenodd
NextMatrix:
          mov ecx,dword ptr [edi+eax*4]
          mov edx,ebx
		  xor edx,ecx
		  and ecx,edx
          cmp ecx,0            
          je  Start_Correl
          add edi,mt          
          inc nSchrittweite
		  cmp nSchrittweite,128
          jl NextMatrix
          inc nStartwert
		  cmp nStartwert,256
		  jl  InitLoopMitPalPlus
		  jmp Ende
          

Start_Correl:
          xor ecx,ecx
          xor eax,eax

SchrittLoop:
          mov ebx,dword ptr [edi+4*ecx+14]
          mov edx,ebx
		  and ebx,0x0000ffff;
		  shr edx,16
		  add eax,dword ptr nZeilenPaarKorrelation[4*ebx]
		  add eax,dword ptr nZeilenPaarKorrelation[4*edx]
          inc   ecx
		  cmp eax,esi
		  jge ZuViel
          cmp ecx,nt
		  jl SchrittLoop
		  mov esi,eax
          mov ebx,nSchrittweite
		  mov eax,nStartwert
		  shl eax,7
		  add eax,ebx
		  mov nBesterKandidat,eax
ZuViel:
          add edi,mt
          inc nSchrittweite
		  cmp nSchrittweite,128
		  jl Do_Correl
          inc nStartwert
		  cmp nStartwert,256
		  jl InitLoopMitPalPlus
Ende:   
   };
  return nBesterKandidat;
  }



int SucheKeyPalPlusMore( int   nPalPlusZeile,BOOL Black1,BOOL Black285,BOOL Black286 )
{
   int      nBesterKandidat=0;
   int      nStartwert=0;
   int      nt=nTestzeilen/2;
   int      nSchrittweite=0;
   int      mt=MoreTestZeilen*2;
   int      mts=MoreTestZeilen*256;
   __asm {
          mov esi,0x0fffffff
	      mov edi,pKombinationen

InitLoopMitPalPlus:
          mov eax,nStartwert
		  mov edx,nPalPlusZeile
		  cmp edx,0
		  jl  Init_Correl_2
LoopMitPalPlus:  
		  movsx ecx, byte ptr pKeys[eax]
		  cmp ecx,edx
          je  Init_Correl
		  inc eax
          add edi,mts
		  cmp eax,256
		  jz Ende
		  jmp LoopMitPalPlus

Init_Correl:
          mov nStartwert,eax
Init_Correl_2:
          mov nSchrittweite,0

NextBlackSet:
          cmp Black1,1
          jnz TestBlack2
          movsx ebx,word ptr [edi+8]
          cmp dword ptr BlackSet[4*ebx],0
		  jz FalschesBlackSet 
TestBlack2:
          cmp Black286,1
          jnz TestBlack3
          movsx ebx,word ptr [edi+10]
          cmp dword ptr BlackSet[4*ebx],0
		  jz FalschesBlackSet 
TestBlack3:
          cmp Black285,1
          jnz Start_Correl
          movsx ebx,word ptr [edi+12]
          cmp dword ptr BlackSet[4*ebx],0
		  jz FalschesBlackSet 
    	  jmp  Start_Correl

FalschesBlackSet: 
          add edi,mt          
          inc nSchrittweite
		  cmp nSchrittweite,128
          jl NextBlackSet
          inc nStartwert
		  cmp nStartwert,256
		  jl  InitLoopMitPalPlus
		  jmp Ende
          

Start_Correl:
          xor ecx,ecx
          xor eax,eax

SchrittLoop:
          mov ebx,dword ptr [edi+4*ecx+14]
          mov edx,ebx
		  and ebx,0x0000ffff;
		  shr edx,16
		  add eax,dword ptr nZeilenPaarKorrelation[4*ebx]
		  add eax,dword ptr nZeilenPaarKorrelation[4*edx]
          inc   ecx
		  cmp eax,esi
		  jge ZuViel
          cmp ecx,nt
		  jl SchrittLoop
		  mov esi,eax
          mov ebx,nSchrittweite
		  mov eax,nStartwert
		  shl eax,7
		  add eax,ebx
		  mov nBesterKandidat,eax
ZuViel:

          add edi,mt
          inc nSchrittweite
		  cmp nSchrittweite,128
		  jl NextBlackSet
          inc nStartwert
		  cmp nStartwert,256
		  jl InitLoopMitPalPlus
Ende:   
   };
  return (nBesterKandidat);
  }


int SucheKeyBlackMatrix(unsigned int MatrixKey,int evenodd )
{
   int      nBesterKandidat=0;
   int      nt=nTestzeilen/2;
   int      mt=MoreTestZeilen*2;
   int      mts=mt*128;
   int      nStartwert=0;
   int      nSchrittweite=0;

   __asm {
          mov esi,0x0fffffff
	      mov edi,pKombinationen

InitLoopMitSchwarz:
          mov eax,nStartwert
		  cmp BLData,0
		  jle Init_Correl_2
LoopMitSchwarz:  
		  movsx ecx, byte ptr pKeys[eax]
          cmp dword ptr BlackSet[4*ecx],1
          je  Init_Correl
          inc eax
		  add edi,mts
		  cmp eax,256
		  jz Ende
		  jmp LoopMitSchwarz

Init_Correl:
          mov nStartwert,eax
Init_Correl_2:
          mov nSchrittweite,0
Do_Correl:
		  mov ebx,MatrixKey
          mov eax,evenodd
NextMatrix:
          mov ecx,dword ptr [edi+eax*4]
          mov edx,ebx
		  xor edx,ecx
		  and ecx,edx
          cmp  ecx,0
          je  Start_Correl
          inc nSchrittweite
          add edi,mt          
		  cmp nSchrittweite,128
          jl NextMatrix
          inc nStartwert
		  cmp nStartwert,256
		  jl  InitLoopMitSchwarz
		  jmp Ende
          

Start_Correl:
          xor  ecx,ecx
		  xor  eax,eax
		  
SchrittLoop:
          mov ebx,dword ptr [edi+4*ecx+14]
          mov edx,ebx
		  and ebx,0x0000ffff;
		  shr edx,16
		  add eax,dword ptr nZeilenPaarKorrelation[4*ebx]
		  add eax,dword ptr nZeilenPaarKorrelation[4*edx]
          inc   ecx
		  cmp eax,esi
		  jge ZuViel
          cmp ecx,nt
		  jl SchrittLoop
		  mov esi,eax
          mov ebx,nSchrittweite
		  mov eax,nStartwert
		  shl eax,7
		  add eax,ebx
		  mov nBesterKandidat,eax
ZuViel:
          add edi,mt
          inc nSchrittweite
		  cmp nSchrittweite,128
		  jl Do_Correl
          inc nStartwert
		  cmp nStartwert,256
		  jl InitLoopMitSchwarz
Ende:   
   };
  return nBesterKandidat;
  }


int SucheKeyBlackMore( BOOL Black1,BOOL Black285, BOOL Black286 )
{
   int      nBesterKandidat=0;
   int      mt=MoreTestZeilen*2;
   int      mts=mt*128;
   int      nStartwert=0;
   int      nSchrittweite=0;
   int      nt=nTestzeilen/2;

   __asm {
          mov esi,0x0fffffff
	      mov edi,pKombinationen

InitLoopMitSchwarz:
          mov eax,nStartwert
		  cmp BLData,0
		  jle Init_Correl_2
LoopMitSchwarz:  
		  movsx ecx, byte ptr pKeys[eax]
          cmp dword ptr BlackSet[4*ecx],1
          je  Init_Correl
		  inc eax
          add edi,mts
		  cmp eax,256
		  jz Ende
		  jmp LoopMitSchwarz

Init_Correl:
          mov nStartwert,eax
Init_Correl_2:
          mov nSchrittweite,0

NextBlackSet:
          cmp Black1,1
          jnz TestBlack2
          movsx ebx,word ptr [edi+8]
          cmp dword ptr BlackSet[4*ebx],0
		  jz FalschesBlackSet 
TestBlack2:
          cmp Black286,1
          jnz TestBlack3
          movsx ebx,word ptr [edi+10]
          cmp dword ptr BlackSet[4*ebx],0
		  jz FalschesBlackSet 
TestBlack3:
          cmp Black285,1
          jnz Start_Correl
          movsx ebx,word ptr [edi+12]
          cmp dword ptr BlackSet[4*ebx],0
		  jz FalschesBlackSet 
    	  jmp  Start_Correl

FalschesBlackSet: 
          add edi,mt          
          inc nSchrittweite
		  cmp nSchrittweite,128
          jl NextBlackSet
          inc nStartwert
		  cmp nStartwert,256
		  jl  InitLoopMitSchwarz
		  jmp Ende
          
Start_Correl:
          xor ecx,ecx
		  xor eax,eax

SchrittLoop:
          mov ebx,dword ptr [edi+4*ecx+14]
          mov edx,ebx
		  and ebx,0x0000ffff;
		  shr edx,16
		  add eax,dword ptr nZeilenPaarKorrelation[4*ebx]
		  add eax,dword ptr nZeilenPaarKorrelation[4*edx]
          inc   ecx
		  cmp eax,esi
		  jge ZuViel
          cmp ecx,nt
		  jl SchrittLoop
		  mov esi,eax
          mov ebx,nSchrittweite
		  mov eax,nStartwert
		  shl eax,7
		  add eax,ebx
		  mov nBesterKandidat,eax
ZuViel:

          inc nSchrittweite
          add edi,mt
		  cmp nSchrittweite,128
		  jl NextBlackSet
          inc nStartwert
		  cmp nStartwert,256
		  jl InitLoopMitSchwarz
		  mov eax,nBesterKandidat
Ende:   
   };
  return (nBesterKandidat);
  }
#endif


void Init_HQTabelle()
{
   int      nZeilenAbstand;
   double   dPhase;
   short   *pTabelle;


   int i=0;


   // Das sind die -90 Grad Phasendrehung pro Zeile plus eine 
   // Schwingung Offset pro Vollbild. 
   double   dPIHalbe  =  -1.56074323; //-1.56071095712045105;
   double   dFarbton  =    0.321699087;//0.1047197551196597746;
   double   dUVFaktor =   0.708661417; // 0.7058823529411764706;// 
   double   Offset    =  8192.0; //10240.0; //8192.0; //12288.0;


   pTabelle = &HQTabelle[0];

   for(nZeilenAbstand=0; nZeilenAbstand<345; nZeilenAbstand++)
   {
      dPhase = (double)(nZeilenAbstand-344)*dPIHalbe + dFarbton;

      *pTabelle++ = (short)( cos(dPhase) * Offset);
      *pTabelle++ = (short)( sin(dPhase) * Offset);
      *pTabelle++ = (short)(-sin(dPhase) * Offset * dUVFaktor);
      *pTabelle++ = (short)( cos(dPhase) * Offset * dUVFaktor);

      // nQuelleInvers
      *pTabelle++ = (short)( cos(dPhase) * Offset);
      *pTabelle++ = (short)(-sin(dPhase) * Offset);
      *pTabelle++ = (short)(-sin(dPhase) * Offset * dUVFaktor);
      *pTabelle++ = (short)(-cos(dPhase) * Offset * dUVFaktor);

      // nZielInvers
      *pTabelle++ = (short)( cos(dPhase) * Offset);
      *pTabelle++ = (short)( sin(dPhase) * Offset);
      *pTabelle++ = (short)( sin(dPhase) * Offset * dUVFaktor);
      *pTabelle++ = (short)(-cos(dPhase) * Offset * dUVFaktor);

      // nQuelleInvers + nZielInvers
      *pTabelle++ = (short)( cos(dPhase) * Offset);
      *pTabelle++ = (short)(-sin(dPhase) * Offset);
      *pTabelle++ = (short)( sin(dPhase) * Offset * dUVFaktor);
      *pTabelle++ = (short)( cos(dPhase) * Offset * dUVFaktor);
   }

}


void __fastcall HQFillRGB()
{
   _asm
   {

      mov eax,dword ptr [ColFaktor]
      mov ecx,BytesOut
      movq mm7,qword ptr [eax]         // mm7 = *pColorFaktor (64Bit)
      movq mm4,qword ptr[nMaske]       // mm4 = 0x00ff00ff00ff00ff
      movq mm6,qword ptr [nMinus128]     
      shr ecx,02                
      xor eax,eax 
	  align 16
 
   NaechsterPixel:
      movq mm0,qword ptr[edx+eax]          // mm0 = *pQuelle 
      psubb mm0,mm6   // U-128 V-128
      psraw mm0,8
      movq mm1,mm0
      punpckldq mm0,mm0
      pmaddwd mm0,mm7
      punpckhdq mm1,mm1
      pmaddwd mm1,mm7
      psrad mm0,0x0b  // 0x0c
      psrad mm1,0x0b  // 0x0c
      packssdw mm0,mm1
      movq qword ptr [edi+eax],mm0
	  add eax,8
      loop NaechsterPixel
      emms                             // MMX zurücksetzen
   }
}

void __fastcall HQFillYUV2()
{
    _asm
   {
      mov eax,dword ptr [ColFaktor]
      mov ecx,BytesOut
      movq mm7,qword ptr [eax]         // mm7 = *pColorFaktor (64Bit)
      movq mm4,qword ptr[nMaske]       // mm4 = 0x00ff00ff00ff00ff
      movq mm6,qword ptr [nMinus128]     
      shr ecx,02                       // nBreiteQuelle /= 4
      xor eax,eax
	  align 16

   NaechsterPixel:
      movq mm0,qword ptr[edx+eax]          
      psubb mm0,mm6                    
      psraw mm0,8                      
      movq mm1,mm0                     
      punpckldq mm0,mm0                
      punpckhdq mm1,mm1                
      pmaddwd mm0,mm7                  
      pmaddwd mm1,mm7                  
      psrad mm0,14                     
      psrad mm1,14                     
      packssdw mm0,mm1                 
      movq qword ptr[edi+eax],mm0  
      add eax,8                        
      loop NaechsterPixel          
      emms                             
   }
}

void __fastcall HQAusgebenRGB16DVB()
{
  _asm
   {
      mov esi,dword ptr [ColFaktor]
      mov ecx,BytesOut
      movq mm7,qword ptr [esi]         // mm7 = *pColorFaktor (64Bit)
      movq mm4,qword ptr[nMaske]       // mm4 = 0x00ff00ff00ff00ff
      mov esi,TmpPtr
      xor eax,eax
   align 16

   NaechsterPixel:
      movq mm0,qword ptr[edx+eax]          // mm0 = *pQuelle 
	  movq mm2,mm0
      pslld mm0,8                      // Für DVB - Byteswapping
      psrld mm2,8
      psubb mm0,qword ptr[nMinus128]   // U-128 V-128
      psraw mm0,0x08
      pand mm2,mm4                     // UV ausmaskieren
      movq mm1,mm0
      punpckldq mm0,mm0
      psrlw mm2,01
      pmaddwd mm0,mm7                  // mm0 += *pColorFaktor
      punpckhdq mm1,mm1
      movq mm5,mm2
      pmaddwd mm1,mm7
      punpcklbw mm2,mm2
      psrad mm0,0x0b  // 0x0c
      psrad mm1,0x0b  // 0x0c
      movq mm6,qword ptr[edi+eax]  // mm6 = pLinebuffer+nBreiteQuelle*8
      packssdw mm0,mm1
      movq qword ptr[edi+eax],mm0  // mm6 = pLinebuffer+nBreiteQuelle*8
      paddsw mm0,mm6
      punpckhbw mm5,mm5
      movq mm6,mm0
      movq mm3,qword ptr[nFaktoren]
      punpckldq mm6,mm6
      pmulhw mm6,mm3
      punpckhdq mm0,mm0
      pmulhw mm0,mm3
      movq mm1,mm2
      psllq mm2,0x08
      movq mm3,mm6
      psrlq mm6,0x30
      por mm2,mm1
      paddsw mm6,mm3
      movq mm3,mm5
      psllq mm5,0x08
      packsswb mm6,mm6
      movq mm1,mm0
      psrlq mm0,0x30
      por mm5,mm3
      paddsw mm0,mm1
      pxor mm1,mm1
      packsswb mm0,mm0
      pxor mm3,mm3
      paddsb mm5,mm0
      paddsb mm2,mm6
      pcmpgtb mm3,mm5
      pandn mm3,mm5
      pcmpgtb mm1,mm2
      movq mm0,mm3
      pand mm3,mm4
      psrld mm0,0x0A
      pandn mm1,mm2
      pslld mm0,0x1B
      movq mm5,mm3
      psrlw mm3,2
      movq mm2,mm1
      psrld mm5,1
      pand mm1,mm4
      pslld mm5,0x15
      por mm5,mm0
      movq mm0,mm1
      psrlw mm1,2
      por mm5,mm3
      psrld mm0,1
      pslld mm0,0x15
      psrad mm5,0x10
      por mm1,mm0
      psrld mm2,0x0A
      pslld mm2,0x1B
      por mm2,mm1
      psrad mm2,0x10
      packssdw mm2,mm5
      movq qword ptr [esi+eax],mm2          // Pixel ausgeben
      add eax,8       
      cmp eax,ecx                          // nBreiteQuelle--
      jl near NaechsterPixel          // Sind noch weitere Pixel in dieser Zeile

	  mov eax,LinesAnzahl
      cmp eax,1
	  jz  NixMehr
	  mov ecx,BytesOut
	  mov edx,CurrentPitch
      sub edx,ecx
      mov ebx,ecx
	  mov edi,ScreenPtr
	  shr ecx,2

NextLine:
	  rep movsd
	  mov ecx,ebx
	  sub esi,ebx
	  shr ecx,2
	  add edi,edx
	  dec eax
	  jnz NextLine

NixMehr: 
      emms            
}
}


void __fastcall HQAusgebenRGB16()
{
  _asm
   {
      mov esi,dword ptr [ColFaktor]
      mov ecx,BytesOut
      movq mm7,qword ptr [esi]         // mm7 = *pColorFaktor (64Bit)
      movq mm4,qword ptr[nMaske]       // mm4 = 0x00ff00ff00ff00ff
      mov esi,TmpPtr
      xor eax,eax
   align 16

   NaechsterPixel:
      movq mm0,qword ptr[edx+eax]          // mm0 = *pQuelle 
      movq mm2,mm0
      psubb mm0,qword ptr[nMinus128]   // U-128 V-128
      psraw mm0,0x08
      pand mm2,mm4                     // UV ausmaskieren
      movq mm1,mm0
      punpckldq mm0,mm0
      psrlw mm2,01
      pmaddwd mm0,mm7                  // mm0 += *pColorFaktor
      punpckhdq mm1,mm1
      movq mm5,mm2
      pmaddwd mm1,mm7
      punpcklbw mm2,mm2
      psrad mm0,0x0b  // 0x0c
      psrad mm1,0x0b  // 0x0c
      movq mm6,qword ptr[edi+eax]  // mm6 = pLinebuffer+nBreiteQuelle*8
      packssdw mm0,mm1
      movq qword ptr[edi+eax],mm0  // mm6 = pLinebuffer+nBreiteQuelle*8
      paddsw mm0,mm6
      punpckhbw mm5,mm5
      movq mm6,mm0
      movq mm3,qword ptr[nFaktoren]
      punpckldq mm6,mm6
      pmulhw mm6,mm3
      punpckhdq mm0,mm0
      pmulhw mm0,mm3
      movq mm1,mm2
      psllq mm2,0x08
      movq mm3,mm6
      psrlq mm6,0x30
      por mm2,mm1
      paddsw mm6,mm3
      movq mm3,mm5
      psllq mm5,0x08
      packsswb mm6,mm6
      movq mm1,mm0
      psrlq mm0,0x30
      por mm5,mm3
      paddsw mm0,mm1
      pxor mm1,mm1
      packsswb mm0,mm0
      pxor mm3,mm3
      paddsb mm5,mm0
      paddsb mm2,mm6
      pcmpgtb mm3,mm5
      pandn mm3,mm5
      pcmpgtb mm1,mm2
      movq mm0,mm3
      pand mm3,mm4
      psrld mm0,0x0A
      pandn mm1,mm2
      pslld mm0,0x1B
      movq mm5,mm3
      psrlw mm3,2
      movq mm2,mm1
      psrld mm5,1
      pand mm1,mm4
      pslld mm5,0x15
      por mm5,mm0
      movq mm0,mm1
      psrlw mm1,2
      por mm5,mm3
      psrld mm0,1
      pslld mm0,0x15
      psrad mm5,0x10
      por mm1,mm0
      psrld mm2,0x0A
      pslld mm2,0x1B
      por mm2,mm1
      psrad mm2,0x10
      packssdw mm2,mm5
      movq qword ptr [esi+eax],mm2          // Pixel ausgeben
      add eax,8       
      cmp eax,ecx                          // nBreiteQuelle--
      jl near NaechsterPixel          // Sind noch weitere Pixel in dieser Zeile

	  mov eax,LinesAnzahl
      cmp eax,1
	  jz  NixMehr
	  mov ecx,BytesOut
	  mov edx,CurrentPitch
      sub edx,ecx
      mov ebx,ecx
	  mov edi,ScreenPtr
	  shr ecx,2

NextLine:
	  rep movsd
	  mov ecx,ebx
	  sub esi,ebx
	  shr ecx,2
	  add edi,edx
	  dec eax
	  jnz NextLine

NixMehr: 
      emms            
}
}

void __fastcall HQAusgebenRGB15DVB()
{
  _asm
   {
      mov eax,dword ptr [ColFaktor]
      mov ecx,BytesOut
      movq mm7,qword ptr [eax]         // mm7 = *pColorFaktor (64Bit)
      movq mm4,qword ptr[nMaske]       // mm4 = 0x00ff00ff00ff00ff
      mov esi,TmpPtr
      xor eax,eax
   align 16

   NaechsterPixel:
      movq mm0,qword ptr[edx+eax]          // mm0 = *pQuelle 
	  movq mm2,mm0
      pslld mm0,8                      // Für DVB - Byteswapping
      psrld mm2,8
      psubb mm0,qword ptr[nMinus128]   // U-128 V-128
      psraw mm0,0x08
      pand mm2,mm4                     // UV ausmaskieren
      movq mm1,mm0
      punpckldq mm0,mm0
      psrlw mm2,01
      pmaddwd mm0,mm7                  // mm0 += *pColorFaktor
      punpckhdq mm1,mm1
      movq mm5,mm2
      pmaddwd mm1,mm7
      punpcklbw mm2,mm2
      psrad mm0,0x0b  // 0x0c
      psrad mm1,0x0b  // 0x0c
      movq mm6,qword ptr[edi+eax]  // mm6 = pLinebuffer+nBreiteQuelle*8
      packssdw mm0,mm1
      movq qword ptr[edi+eax],mm0  // mm6 = pLinebuffer+nBreiteQuelle*8
      paddsw mm0,mm6
      punpckhbw mm5,mm5
      movq mm6,mm0
      movq mm3,qword ptr[nFaktoren]
      punpckldq mm6,mm6
      pmulhw mm6,mm3
      punpckhdq mm0,mm0
      pmulhw mm0,mm3
      movq mm1,mm2
      psllq mm2,0x08
      movq mm3,mm6
      psrlq mm6,0x30
      por mm2,mm1
      paddsw mm6,mm3
      movq mm3,mm5
      psllq mm5,0x08
      packsswb mm6,mm6
      movq mm1,mm0
      psrlq mm0,0x30
      por mm5,mm3
      paddsw mm0,mm1
      pxor mm1,mm1
      packsswb mm0,mm0
      pxor mm3,mm3
      paddsb mm5,mm0
      paddsb mm2,mm6
      pcmpgtb mm3,mm5
      pandn mm3,mm5
      pcmpgtb mm1,mm2
      movq mm0,mm3
      pand mm3,mm4
      psrld mm0,0x0A
      pandn mm1,mm2
      pslld mm0,0x1A
      movq mm5,mm3
      psrlw mm3,2
      movq mm2,mm1
      psrld mm5,2
      pand mm1,mm4
      pslld mm5,0x15
      por mm5,mm0
      movq mm0,mm1
      psrlw mm1,2
      por mm5,mm3
      psrld mm0,2
      pslld mm0,0x15
      psrad mm5,0x10
      por mm1,mm0
      psrld mm2,0x0A
      pslld mm2,0x1A
      por mm2,mm1
      psrad mm2,0x10
      packssdw mm2,mm5
      movq qword ptr [esi+eax],mm2          // Pixel ausgeben
      add eax,8       
      cmp eax,ecx                          // nBreiteQuelle--
      jl near NaechsterPixel          // Sind noch weitere Pixel in dieser Zeile

	  mov eax,LinesAnzahl
      cmp eax,1
	  jz  NixMehr
	  mov ecx,BytesOut
	  mov edx,CurrentPitch
      sub edx,ecx
      mov ebx,ecx
	  mov edi,ScreenPtr
	  shr ecx,2

NextLine:
	  rep movsd
	  mov ecx,ebx
	  sub esi,ebx
	  shr ecx,2
	  add edi,edx
	  dec eax
	  jnz NextLine

NixMehr: 
      emms            
}
}



void __fastcall HQAusgebenRGB15()
{
  _asm
   {
      mov eax,dword ptr [ColFaktor]
      mov ecx,BytesOut
      movq mm7,qword ptr [eax]         // mm7 = *pColorFaktor (64Bit)
      movq mm4,qword ptr[nMaske]       // mm4 = 0x00ff00ff00ff00ff
      mov esi,TmpPtr
      xor eax,eax
   align 16

   NaechsterPixel:
      movq mm0,qword ptr[edx+eax]          // mm0 = *pQuelle 
	  movq mm2,mm0
      psubb mm0,qword ptr[nMinus128]   // U-128 V-128
      psraw mm0,0x08
      pand mm2,mm4                     // UV ausmaskieren
      movq mm1,mm0
      punpckldq mm0,mm0
      psrlw mm2,01
      pmaddwd mm0,mm7                  // mm0 += *pColorFaktor
      punpckhdq mm1,mm1
      movq mm5,mm2
      pmaddwd mm1,mm7
      punpcklbw mm2,mm2
      psrad mm0,0x0b  // 0x0c
      psrad mm1,0x0b  // 0x0c
      movq mm6,qword ptr[edi+eax]  // mm6 = pLinebuffer+nBreiteQuelle*8
      packssdw mm0,mm1
      movq qword ptr[edi+eax],mm0  // mm6 = pLinebuffer+nBreiteQuelle*8
      paddsw mm0,mm6
      punpckhbw mm5,mm5
      movq mm6,mm0
      movq mm3,qword ptr[nFaktoren]
      punpckldq mm6,mm6
      pmulhw mm6,mm3
      punpckhdq mm0,mm0
      pmulhw mm0,mm3
      movq mm1,mm2
      psllq mm2,0x08
      movq mm3,mm6
      psrlq mm6,0x30
      por mm2,mm1
      paddsw mm6,mm3
      movq mm3,mm5
      psllq mm5,0x08
      packsswb mm6,mm6
      movq mm1,mm0
      psrlq mm0,0x30
      por mm5,mm3
      paddsw mm0,mm1
      pxor mm1,mm1
      packsswb mm0,mm0
      pxor mm3,mm3
      paddsb mm5,mm0
      paddsb mm2,mm6
      pcmpgtb mm3,mm5
      pandn mm3,mm5
      pcmpgtb mm1,mm2
      movq mm0,mm3
      pand mm3,mm4
      psrld mm0,0x0A
      pandn mm1,mm2
      pslld mm0,0x1A
      movq mm5,mm3
      psrlw mm3,2
      movq mm2,mm1
      psrld mm5,2
      pand mm1,mm4
      pslld mm5,0x15
      por mm5,mm0
      movq mm0,mm1
      psrlw mm1,2
      por mm5,mm3
      psrld mm0,2
      pslld mm0,0x15
      psrad mm5,0x10
      por mm1,mm0
      psrld mm2,0x0A
      pslld mm2,0x1A
      por mm2,mm1
      psrad mm2,0x10
      packssdw mm2,mm5
      movq qword ptr [esi+eax],mm2          // Pixel ausgeben
      add eax,8       
      cmp eax,ecx                          // nBreiteQuelle--
      jl near NaechsterPixel          // Sind noch weitere Pixel in dieser Zeile

	  mov eax,LinesAnzahl
      cmp eax,1
	  jz  NixMehr
	  mov ecx,BytesOut
	  mov edx,CurrentPitch
      sub edx,ecx
      mov ebx,ecx
	  mov edi,ScreenPtr
	  shr ecx,2

NextLine:
	  rep movsd
	  mov ecx,ebx
	  sub esi,ebx
	  shr ecx,2
	  add edi,edx
	  dec eax
	  jnz NextLine

NixMehr: 
      emms            
}
}




void __fastcall HQAusgebenYUV2()
{
  _asm
   {
      mov eax,dword ptr [ColFaktor]
      mov ecx,BytesOut
      shr ecx,3
      movq mm7,qword ptr [eax]         // mm7 = *pColorFaktor (64Bit)
      movq mm4,qword ptr[nMaske]      
      movq mm5,qword ptr[nMaskeUV]    
      movq mm6,qword ptr[nMinus128]   
      mov esi,TmpPtr
      xor eax,eax
   align 16

   NaechsterPixel:
      movq mm0,qword ptr[edx+eax]          
      movq mm2,mm0                     
      psubb mm0,mm6                    
      pand mm2,mm4                     
      psraw mm0,8                      
      movq mm1,mm0                     
      punpckldq mm0,mm0                
      punpckhdq mm1,mm1                
      pmaddwd mm0,mm7                  
      pmaddwd mm1,mm7                  
      movq mm3,qword ptr[edi+eax]  
      psrad mm0,14                     
      psrad mm1,14                     
      packssdw mm0,mm1                 
      movq qword ptr[edi+eax],mm0  
      paddsw mm0,mm3                   
      packsswb mm0,mm0                 
      punpcklbw mm0,mm0                
      pand mm0,mm5                     
      psubb mm0,mm6                    
      por mm2,mm0                      
      movq qword ptr[esi+eax],mm2          
      add eax,8                        
      loop NaechsterPixel          

	  mov eax,LinesAnzahl
      cmp eax,1
	  jz  NixMehr
	  mov ecx,BytesOut
	  mov edx,CurrentPitch
      sub edx,ecx
      mov ebx,ecx
	  mov edi,ScreenPtr
	  shr ecx,2

NextLine:
	  rep movsd
	  mov ecx,ebx
	  sub esi,ebx
	  shr ecx,2
	  add edi,edx
	  dec eax
	  jnz NextLine

NixMehr: 
      emms            
}
}

void __fastcall HQAusgebenYUV2DVB()
{

  _asm
   {
      mov eax,dword ptr [ColFaktor]
      mov ecx,BytesOut
      shr ecx,3
      movq mm7,qword ptr [eax]         // mm7 = *pColorFaktor (64Bit)
      movq mm4,qword ptr[nMaske]      
      movq mm5,qword ptr[nMaskeUV]    
      movq mm6,qword ptr[nMinus128]   
      mov esi,TmpPtr
      xor eax,eax
   align 16

   NaechsterPixel:
      movq mm0,qword ptr[edx+eax]          
	  movq mm2,mm0                     
      pslld mm0,8                      // Für DVB - Byteswapping
      psrld mm2,8
      psubb mm0,mm6                    
      pand mm2,mm4                     
      psraw mm0,8                      
      movq mm1,mm0                     
      punpckldq mm0,mm0                
      punpckhdq mm1,mm1                
      pmaddwd mm0,mm7                  
      pmaddwd mm1,mm7                  
      movq mm3,qword ptr[edi+eax]  
      psrad mm0,14                     
      psrad mm1,14                     
      packssdw mm0,mm1                 
      movq qword ptr[edi+eax],mm0  
      paddsw mm0,mm3                   
      packsswb mm0,mm0                 
      punpcklbw mm0,mm0                
      pand mm0,mm5                     
      psubb mm0,mm6                    
      por mm2,mm0                      
      movq qword ptr[esi+eax],mm2          
      add eax,8                        
      loop NaechsterPixel          

	  mov eax,LinesAnzahl
      cmp eax,1
	  jz  NixMehr
	  mov ecx,BytesOut
	  mov edx,CurrentPitch
      sub edx,ecx
      mov ebx,ecx
	  mov edi,ScreenPtr
	  shr ecx,2

NextLine:
	  rep movsd
	  mov ecx,ebx
	  sub esi,ebx
	  shr ecx,2
	  add edi,edx
	  dec eax
	  jnz NextLine

NixMehr: 
      emms            
}
}

#ifdef USE_BT8X8
BOOL APIENTRY NagraSetupProc(HWND hDlg,UINT message,UINT wParam,LONG lParam)
{

	int i,j;
	BOOL Swap=TRUE;

    switch (message) {
        case WM_INITDIALOG :
               SetDlgItemInt(hDlg, IDC_TESTZEILEN, MasterTestzeilen, TRUE);
				SetDlgItemInt(hDlg, IDC_EDIT1,TZ[0],TRUE);
				SetDlgItemInt(hDlg, IDC_EDIT2,TZ[1],TRUE);
				SetDlgItemInt(hDlg, IDC_EDIT3,TZ[2],TRUE);
				SetDlgItemInt(hDlg, IDC_EDIT4,TZ[3],TRUE);
				SetDlgItemInt(hDlg, IDC_EDIT5,TZ[4],TRUE);
               CheckDlgButton(hDlg, IDC_CHECK1, NagraFullField);
			   break;
		case WM_COMMAND:
                   switch (wParam) {
             		 
				      case IDOK :
				            MasterTestzeilen=GetDlgItemInt(hDlg, IDC_TESTZEILEN, NULL,
                                      TRUE);
							if (MasterTestzeilen < 8 ) MasterTestzeilen=8;
							  else if (MasterTestzeilen > 80 ) MasterTestzeilen=80;

							 TZ[0]=GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
							 TZ[1]=GetDlgItemInt(hDlg, IDC_EDIT2, NULL,TRUE);
							 TZ[2]=GetDlgItemInt(hDlg, IDC_EDIT3, NULL,TRUE);
							 TZ[3]=GetDlgItemInt(hDlg, IDC_EDIT4, NULL,TRUE);
							 TZ[4]=GetDlgItemInt(hDlg, IDC_EDIT5, NULL,TRUE);
							  
                             Swap=TRUE;
							 while ( Swap == TRUE ) {
							 Swap=FALSE;
							 for ( i=0; i < 4 ; i++ ) {
								 if ( TZ[i] > TZ[i+1] ) {
									 Swap=TRUE;
                                     j=TZ[i];
									 TZ[i]=TZ[i+1];
									 TZ[i+1]=j;
								 }
							 }
							 }

                              for ( i=0; i < 4 ; i++ ) {
							if (TZ[i] < 8 ) TZ[i]=8;
							  else if (TZ[i] > 80 ) TZ[i]=80;
							  };
                             NagraFullField=IsDlgButtonChecked(hDlg, IDC_CHECK1);
					  case IDENDE :
									EndDialog(hDlg, TRUE) ;
									break;
           }
            break ;
    }

    return FALSE ;
}



// Secam Functions by Fantasio
// Replace MainLoops by MMX-Function ( Espresso )
void FilterBlueLines(DWORD **pLinesAddress, int *pSortLines, BOOL bEven)
{
	int	iLine,iLastGoodULine;
	int nSumU,nSumV;
	int nUThreshold,nVThreshold;
	int iLineBack,iLastLineBack, nLinesBack;


	// should 384 not be CurrentX/2

	nUThreshold= (int) (iUFilterThresholdPCent*0.01*(CurrentX/2)*0x0f);
	nVThreshold= (int) (iVFilterThresholdPCent*0.01*(CurrentX/2)*0x0f);

	// first pass : detect red or blue lines and setup the correcting line offset in itOffsetUGood.
	iLastGoodULine=-288;
	for ( iLine=1+MovieScale; iLine < ZEILENZAHL-MovieScale; iLine++ ) {
        Secam_UVValue((short *)pLinesAddress[pSortLines[iLine]], CurrentX, &nSumU, &nSumV);
		if ((nSumU >= nUThreshold) || (nSumV >= nVThreshold)) {
			itOffsetUGood[iLine]=iLastGoodULine-iLine;
		} else {
			if (iLastGoodULine<(iLine-1)) {
				iLastLineBack=iLastGoodULine+1;
				nLinesBack=(iLine-iLastLineBack) >> 1;
				iLastLineBack = iLine-nLinesBack;
				if (iLastLineBack<1+MovieScale) iLastLineBack=1+MovieScale;
				for (iLineBack = iLine-1; iLineBack>=iLastLineBack; --iLineBack) {
					itOffsetUGood[iLineBack]=iLine-iLineBack;
				}
			}
			itOffsetUGood[iLine]=0;
			iLastGoodULine=iLine;
		}
	}
	// second pass : copy the right lines.
	for ( iLine=1+MovieScale; iLine < ZEILENZAHL-MovieScale; iLine++ ) {
		iLastGoodULine=itOffsetUGood[iLine];
		if (iLastGoodULine) Secam_UVMix((short *)pLinesAddress[pSortLines[iLine+iLastGoodULine]], (short *)pLinesAddress[pSortLines[iLine]] ,CurrentX );
	}
}


// Just perhaps a better way // Test 

void Secam_UVValue(short *pQuelle, int Count , int *U, int *V)
{
	__int64  Mask=0x0000FFFF0000FFFF;
	int xu,xv;
    _asm
   {

	  mov edx,pQuelle
      movq mm7,qword ptr [Mask] 
	  pxor mm4,mm4
	  pxor mm5,mm5
      mov  ecx,Count
	  shr  ecx,2
      align 16

   NaechsterPixel:
      movq mm0,qword ptr[edx]          // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0
      add edx,8  
      psrlw mm0,12                     // mm0 = 00 V2 00 U2 00 Y1 00 U1    // High 4 Bit  / same like X >> 12 & 0x0f
	  paddw mm4,mm0                    // mm4 = word mm4 + word mm0
      loop NaechsterPixel
      movq mm0,mm4
	  pand mm4,mm7                     // mm4 = 00 00 U2 U2 00 00 U1 U1
      psrlq mm0,16
	  pand mm0,mm7                     // mm0 = 00 00 V2 V2 00 00 V1 V1

      movq  mm5,mm4
      movq  mm2,mm0
	  psrlq mm5,32                     // mm5 = 00 00 00 00 U2 U2 U2 U2
	  psrlq mm2,32                     // mm2 = 00 00 00 00 V2 V2 V2 V2
      paddd mm4,mm5
      paddd mm0,mm2
	  movd  xu,mm4
	  movd  xv,mm0
      emms                             
   }
*U=xu;
*V=xv;
}


// Just perhaps a better way // Test 

void Secam_UVMix(short *pQuelle, short *pZiel ,int Count )
{
    _asm
   {

	  mov edi,pQuelle
      movq mm4,qword ptr[nMaske]      
	  movq mm5,qword ptr[nMaskeUV]    
	  mov esi,pZiel
      mov  ecx,Count
	  xor eax,eax
	  shr  ecx,2
      align 16

   NaechsterPixel:
      movq mm0,qword ptr[edi+eax]          // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0
      movq mm1,qword ptr[esi+eax]          // mm1 = V2 Y3 U2 Y2 V0 Y1 U0 Y0
      pand mm0,mm5                         // mm0 = V2 00 U2 00 V0 00 U0 00
      pand mm1,mm4                         // mm1 = 00 Y3 00 Y2 00 Y1 00 Y0
      por mm1,mm0
	  movq qword ptr[esi+eax],mm1    
	  add eax,8
	  loop NaechsterPixel
      emms                             
   }
}

#endif
