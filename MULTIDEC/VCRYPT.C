/*********************************************************************************
 *                                                                               *
 * vcrypt.c                                                                      *
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
#include <math.h>
#include "resource.h"
#include <fcntl.h>
#include <io.h>         
#include <stdlib.h>        /* atoi                                  */
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "vcrypt.h"

#define ROLL(x) ((x)>=(710)?(x)-(710):(x)<0?(x)+(710):(x)) 


 // VC
void YFillDownSize(BYTE *pZielY , short Cut);
void YFillNew2Part(short *pQuelle, BYTE *pZielY );
void VC_UVColor(short *pQuelle,BYTE *pZiel,short *pColorFaktor, int Cut);
void VC_UVColorPur(short *pQuelle,BYTE *pZiel,short *pColorFaktor, int Cut);
unsigned int ChrominanzCompare(short *pCompare1,short *pCompare2);
unsigned int ChrominanzComparePur(BYTE *pCompare1,BYTE *pCompare2);
int Find_Shift(BYTE *pQuelle);
void VC_UVValue(short *pQuelle,short *pColorFaktor, int Cut,int *U , int *V);

#ifdef USE_BT8X8

int Get_Decoder_Key(short **pLinesAddressEven)
{
int i,j;

short AlignLocalShifts[287+16];
BYTE AlignyBuffer[712*3+31];
BYTE  *yBuffer0=(BYTE *)(((DWORD)AlignyBuffer+31)&~31);
short *shifts=(short *)((((DWORD)AlignLocalShifts+31)&~31));

short diff[16];
int f=-2;
int BestKey=0x0fffffff;
int CurrentKey=0;
int DecoderPos=0;

for(i=0;i<16;i++)
  {
	YFillNew2Part(pLinesAddressEven[referenzes[i]-1],yBuffer0+712);
	YFillDownSize(yBuffer0,103);
        YFillNew2Part(pLinesAddressEven[referenzes[i]],yBuffer0+712);
		shifts[referenzes[i]]=ROLL(Find_Shift(yBuffer0)-102);
  }
     
for(i=0;i<256 ;i++)
  {
  CurrentKey=0;

  for(j=0;j<16;j++) 
     {
     diff[j]=(short)((cp_odd[i][referenzes[j]+f]-cp_odd[i][referenzes[j]-1+f]));
     if (diff[j]<0) diff[j]=(short)(710+diff[j]);
      CurrentKey=CurrentKey+abs(diff[j]-shifts[referenzes[j]]);
     }
  if ( CurrentKey < BestKey ) {
       BestKey=CurrentKey;
       DecoderPos=i;
  };
}



  return(DecoderPos);     
}  

  
//------------------------------------------------

BOOL APIENTRY VCSetupProc(HWND hDlg,UINT message,UINT wParam,LONG lParam)
{

    switch (message) {
        case WM_INITDIALOG :
              CheckDlgButton(hDlg, IDC_CHECK1, Chroma_Key);
			  break;			
		
		case WM_COMMAND:

                   switch (wParam) {

      				   case IDOK :
							Chroma_Key=IsDlgButtonChecked(hDlg, IDC_CHECK1);
					  case IDENDE :
									EndDialog(hDlg, TRUE) ;
									break;
           }
            break ;
    }

    return FALSE ;
}

/************************************/

void Init_VC_HQTabelle()
{
   int      i;

   double   PI = 3.141592654;
   double   Offset       =  8192.0; //FixFloat Multiplikator

	double m1l=65.993386;
	double m1r=65.994307;
	double m2l=-65.993384;
	double m2r=-65.994433;



	double k1l=-6722.998388;
	double k1r=-6777.322377;
	double k2l=6546.194899;
	double k2r=6603.066144;

/*
	double k1l=159.0;
	double k1r=95.0;
	double k2l=73.0;
	double k2r=132.0;
*/


   for ( i=0; i < 710 ; i++ ) {
	   VCHQTabelleNoBurst[i*8]=(short)(-sin(((m1l*i+k1l)*PI)/180) * Offset);
	   VCHQTabelleNoBurst[i*8+1]=(short)(cos(((m1l*i+k1l)*PI)/180) * Offset);
	   VCHQTabelleNoBurst[i*8+2]=(short)(cos(((m1l*i+k1l)*PI)/180) * Offset);
	   VCHQTabelleNoBurst[i*8+3]=(short)(sin(((m1l*i+k1l)*PI)/180) * Offset);

	   VCHQTabelleNoBurst[i*8+4]=(short)(-sin(((m1r*i+k1r)*PI)/180) * Offset);
	   VCHQTabelleNoBurst[i*8+5]=(short)(cos(((m1r*i+k1r)*PI)/180) * Offset);
	   VCHQTabelleNoBurst[i*8+6]=(short)(cos(((m1r*i+k1r)*PI)/180) * Offset);
	   VCHQTabelleNoBurst[i*8+7]=(short)(sin(((m1r*i+k1r)*PI)/180) * Offset);

	   VCHQTabelleBurst[i*8]=(short)(-sin(((m2l*i+k2l)*PI)/180) * Offset);
	   VCHQTabelleBurst[i*8+1]=(short)(cos(((m2l*i+k2l)*PI)/180) * Offset);
	   VCHQTabelleBurst[i*8+2]=(short)(cos(((m2l*i+k2l)*PI)/180) * Offset);
	   VCHQTabelleBurst[i*8+3]=(short)(sin(((m2l*i+k2l)*PI)/180) * Offset);

	   VCHQTabelleBurst[i*8+4]=(short)(-sin(((m2r*i+k2r)*PI)/180) * Offset);
	   VCHQTabelleBurst[i*8+5]=(short)(cos(((m2r*i+k2r)*PI)/180) * Offset);
	   VCHQTabelleBurst[i*8+6]=(short)(cos(((m2r*i+k2r)*PI)/180) * Offset);
	   VCHQTabelleBurst[i*8+7]=(short)(sin(((m2r*i+k2r)*PI)/180) * Offset);

   
   };

	   VCHQTabelleNoBurst[710*8]=  0;
	   VCHQTabelleNoBurst[710*8+1]=0;
	   VCHQTabelleNoBurst[710*8+2]=0;
	   VCHQTabelleNoBurst[710*8+3]=0;

	   VCHQTabelleNoBurst[710*8+4]=0;
	   VCHQTabelleNoBurst[710*8+5]=0;
	   VCHQTabelleNoBurst[710*8+6]=0;
	   VCHQTabelleNoBurst[710*8+7]=0;

	   VCHQTabelleBurst[710*8]=  0;
	   VCHQTabelleBurst[710*8+1]=0;
	   VCHQTabelleBurst[710*8+2]=0;
	   VCHQTabelleBurst[710*8+3]=0;

	   VCHQTabelleBurst[710*8+4]=0;
	   VCHQTabelleBurst[710*8+5]=0;
	   VCHQTabelleBurst[710*8+6]=0;
	   VCHQTabelleBurst[710*8+7]=0;

}



BOOL Find_Cut_Points_VC(short **pLinesAddressEven,short *RealOffset, int Burst)
{
	short *VCColFaktor;
	BOOL Is_Good=FALSE;
	int best_edge_shift;
    int best_edge_sum;
    int LastChromSet;
	int CurrChromSet;
    int evenodd;
    short AlignLocalShifts[287+16];
    BYTE AlignVCTmpBaseChroma[720+31];
    BYTE AlignVCTmpChroma[720*2+31];
    BYTE AlignyBuffer[712*3+31];
	short VCCut;
    short NextVCCut;
    int NextLine;


	unsigned int MinChromFit;
    unsigned int ChrominanzDiff;

	int ChrominanzShift;
    int NewChrominanzShift;
// Access To Align-Buffer

    BYTE  *yBuffer0=(BYTE *)(((DWORD)AlignyBuffer+31)&~31);
    BYTE *TmpBaseChroma=(BYTE *)(((DWORD)AlignVCTmpBaseChroma+31)&~31);
    BYTE *TmpChroma=(BYTE *)((((DWORD)AlignVCTmpChroma+31)&~31));
	short *LocalShifts=(short *)((((DWORD)AlignLocalShifts+31)&~31));

	int nZeileZiel   ;     
	int i,j,s,k;

	YFillNew2Part(pLinesAddressEven[VCFirstLine],yBuffer0+712);
	YFillDownSize(yBuffer0,103);
    RealOffset[VCFirstLine]=103;  
	
	for ( nZeileZiel=VCFirstLine+1; nZeileZiel < VCCLEARLINES; nZeileZiel++ ) {
	if (pLinesAddressEven[nZeileZiel] != NULL ) {
        YFillNew2Part(pLinesAddressEven[nZeileZiel],yBuffer0+712);
		LocalShifts[nZeileZiel]=RealOffset[nZeileZiel]=Find_Shift(yBuffer0);
	}
}


if ( Chroma_Key == TRUE ) {
for ( evenodd=0; evenodd < 2 ; evenodd++ ) {
NextLine=0;
ChrominanzShift=0;	
CurrChromSet=0;

for ( nZeileZiel=VCFirstLine; nZeileZiel < VCCLEARLINES; nZeileZiel+=2 ) {
	if (pLinesAddressEven[nZeileZiel] != NULL ) {
    	MinChromFit=0x0fffffff;
        VCCut=RealOffset[nZeileZiel]+ChrominanzShift;
		if ( NextLine == 0 ) {
        NextLine=VCFirstLine+2;
		while ((pLinesAddressEven[nZeileZiel] == NULL )) NextLine+=2;
        NextVCCut=RealOffset[NextLine];
        for ( i = -2 ; i <= +2 ; i+=2 ) {
        s=ROLL(VCCut+i);
		if (((nZeileZiel+Burst)&1) == 1 ) VCColFaktor=&VCHQTabelleBurst[s*8];
		 else VCColFaktor=&VCHQTabelleNoBurst[s*8];
		 VC_UVColorPur(pLinesAddressEven[nZeileZiel],TmpBaseChroma,VCColFaktor,s);

		 k=ROLL(NextVCCut+i-2);
		if (((NextLine+Burst)&1) == 1 ) VCColFaktor=&VCHQTabelleBurst[k*8];
		 else VCColFaktor=&VCHQTabelleNoBurst[k*8];
		  VC_UVColorPur(pLinesAddressEven[NextLine],TmpChroma+(CurrChromSet*720),VCColFaktor,k);
		  ChrominanzDiff = ChrominanzComparePur(TmpBaseChroma,TmpChroma+(CurrChromSet*720));
		  if ( ChrominanzDiff < MinChromFit ) {
			  CurrChromSet^=1;
                MinChromFit=ChrominanzDiff;
                RealOffset[nZeileZiel]=s;
                RealOffset[NextLine]=k;
                ChrominanzShift=i-2; 
		  }

		 k=ROLL(NextVCCut+i);
		if (((NextLine+Burst)&1) == 1 ) VCColFaktor=&VCHQTabelleBurst[k*8];
		 else VCColFaktor=&VCHQTabelleNoBurst[k*8];
		  VC_UVColorPur(pLinesAddressEven[NextLine],TmpChroma+(CurrChromSet*720),VCColFaktor,k);
		  ChrominanzDiff = ChrominanzComparePur(TmpBaseChroma,TmpChroma+(CurrChromSet*720));
		  if ( ChrominanzDiff < MinChromFit ) {
			  CurrChromSet^=1;
                MinChromFit=ChrominanzDiff;
                RealOffset[nZeileZiel]=s;
                RealOffset[NextLine]=k;
                ChrominanzShift=i; 
		  }
		 k=ROLL(NextVCCut+i+2);
		if (((NextLine+Burst)&1) == 1 ) VCColFaktor=&VCHQTabelleBurst[k*8];
		 else VCColFaktor=&VCHQTabelleNoBurst[k*8];
		  VC_UVColorPur(pLinesAddressEven[NextLine],TmpChroma+(CurrChromSet*720),VCColFaktor,k);
		  ChrominanzDiff = ChrominanzComparePur(TmpBaseChroma,TmpChroma+(CurrChromSet*720));
		  if ( ChrominanzDiff < MinChromFit ) {
			  CurrChromSet^=1;
                MinChromFit=ChrominanzDiff;
                RealOffset[nZeileZiel]=s;
                RealOffset[NextLine]=k;
                ChrominanzShift=i+2; 
		  }
		}
        nZeileZiel=NextLine;
		} else {   // Ende Ersten 2 Zeilen
             LastChromSet=CurrChromSet^1;  // Da war der letzte Beste 
           
			 if ( LastChromSet == 0 ) {
			 __asm {
				 mov ecx,178
                 mov edi,TmpBaseChroma
				 mov esi,TmpChroma
				 rep movsd
			 };
			 } else {
			 __asm {
				 mov ecx,178
                 mov edi,TmpBaseChroma
				 mov esi,TmpChroma
                 add esi,720
				 rep movsd
			 };
			 }


//			 memcpy(TmpBaseChroma,VCTmpChroma+(LastChromSet*720),720);
			 NewChrominanzShift=0;
		for ( i = -4 ; i <= 4 ; i+=2 ) {
        s=ROLL(VCCut+i);
		if (((nZeileZiel+Burst)&1) == 1 ) VCColFaktor=&VCHQTabelleBurst[s*8];
		 else VCColFaktor=&VCHQTabelleNoBurst[s*8];
		 VC_UVColorPur(pLinesAddressEven[nZeileZiel],TmpChroma+(CurrChromSet*720),VCColFaktor,s);
		  ChrominanzDiff  = ChrominanzComparePur(TmpBaseChroma,TmpChroma+(CurrChromSet*720));
		  if ( ChrominanzDiff < MinChromFit ) {
	    		
	            CurrChromSet^=1;
                MinChromFit=ChrominanzDiff;
                RealOffset[nZeileZiel]=s;
				NewChrominanzShift=i;
		  }
		
}
ChrominanzShift+=NewChrominanzShift;
	}; // ELSE 
}; // Schleife


} ; // Chrom
VCFirstLine++;
}; // Func
i=999;
VCFirstLine-=2;
best_edge_shift=998;
for(nZeileZiel=80;nZeileZiel<200;nZeileZiel+=2) {
if ((pLinesAddressEven[nZeileZiel] != NULL ) &&
   (pLinesAddressEven[nZeileZiel+1] != NULL )) {
     j=(LocalShifts[nZeileZiel+1]-LocalShifts[nZeileZiel]);
     k=(RealOffset[nZeileZiel+1]-RealOffset[nZeileZiel]);
     s=ROLL(j-k);
    
	 if ( s == i ) {
		 if ( s == best_edge_shift ) {
			 break;
		 }
		 best_edge_shift=s;
	 } else {
       best_edge_shift=998;
	 }
     i=s;
	}

}

if ( nZeileZiel != 200 ) {
for ( nZeileZiel=3; nZeileZiel < VCCLEARLINES; nZeileZiel+=2 ) {
	RealOffset[nZeileZiel]=RealOffset[nZeileZiel]+best_edge_shift;
	if ( RealOffset[nZeileZiel] >= 710 ) RealOffset[nZeileZiel]-=710;
};
};
} ; // Chrom

best_edge_sum=best_edge_shift=0;


for(i=708;i>=0;i-=2)                                       
 {
 s=0;
 j=VCTotalLines+3;
	for ( nZeileZiel=VCFirstLine; nZeileZiel < VCCLEARLINES; nZeileZiel++ ) {
        if (pLinesAddressEven[nZeileZiel] != NULL ) {
            k=RealOffset[nZeileZiel]+i;
            if ( k >= 710 ) k-=710;
            if (( k >= 103 ) && ( k <= 613 )) s++;
            if (( s + j ) < best_edge_sum ) {
				break;
			}
			j--;
	}
	}

	if ( s >= best_edge_sum ) {
        best_edge_shift=i;
		if ( s>VCTotalLines) Is_Good=TRUE;
		best_edge_sum=s;
	};
};


if ( best_edge_shift != 0 ) {
for ( nZeileZiel=2; nZeileZiel < VCCLEARLINES; nZeileZiel++ ) {
	RealOffset[nZeileZiel]=RealOffset[nZeileZiel]+best_edge_shift;
	if ( RealOffset[nZeileZiel] >= 710 ) RealOffset[nZeileZiel]-=710;
};
}


s=100;
while (( s < 160 ) && ( pLinesAddressEven[s] == NULL )) s++;
MinChromFit=0x0fffffff;
for ( i=0; i <= 4; i+=2 ) {
ChrominanzDiff=0;
evenodd=0;
j=s;
CurrChromSet=0;
k=RealOffset[j]+i;
if ( k>= 710 ) k-=710;
if (((j+Burst)&1) == 1 ) VCColFaktor=&VCHQTabelleBurst[k*8];
 else VCColFaktor=&VCHQTabelleNoBurst[k*8];
VC_UVColorPur(pLinesAddressEven[j],TmpChroma+(CurrChromSet*720),VCColFaktor,k);
j++;
CurrChromSet^=1;

while (( j < 160 ) && ( evenodd < 6 )) {
	if ( pLinesAddressEven[j] != NULL ) {
	k=RealOffset[j]+i;
if ( k>= 710 ) k-=710;
if (((j+Burst)&1) == 1 ) VCColFaktor=&VCHQTabelleBurst[k*8];
 else VCColFaktor=&VCHQTabelleNoBurst[k*8];
VC_UVColorPur(pLinesAddressEven[j],TmpChroma+(CurrChromSet*720),VCColFaktor,k);
ChrominanzDiff+= ChrominanzComparePur(TmpChroma,TmpChroma+720);
CurrChromSet^=1;
evenodd++;
	}
j++;
};

if ( ChrominanzDiff < MinChromFit ) {
                MinChromFit=ChrominanzDiff;
				best_edge_shift=i;
		  }

};

if ( best_edge_shift != 0 ) {
for ( nZeileZiel=2; nZeileZiel < VCCLEARLINES; nZeileZiel++ ) {
	RealOffset[nZeileZiel]=RealOffset[nZeileZiel]+best_edge_shift;
	if ( RealOffset[nZeileZiel] >= 710 ) RealOffset[nZeileZiel]-=710;
};
}


return(Is_Good);
};



void YFillDownSize(BYTE *pZielY , short Cut)
{

__asm {
	  mov ecx,89
	  movsx eax,Cut
      add eax,712
	  mov esi,pZielY

NaechsterFillPixel:
	  movq mm0,qword ptr[esi+eax] 
	  movq qword ptr[esi],mm0
	  add esi,8
      loop NaechsterFillPixel
	  emms
}
}


void VC_UVValue(short *pQuelle,short *pColorFaktor, int Cut,int *U , int *V)
{
	int xu;
	int xv; 
	__int64 Positiv=0x0000800000008000;

    _asm
   {

	  mov eax,Cut
      mov edi,dword ptr[pColorFaktor]
      shl eax,1
	  mov edx,pQuelle
      add eax,2
      mov ecx,eax
	  movq mm7,qword ptr[edi]         

	  pxor mm4,mm4
	  movq mm5,qword ptr [Positiv]
      movq mm6,qword ptr[nMinus128]   
      align 16

   NaechsterPixelLinks:
      movq mm0,qword ptr[edx+eax]          // mm0 = *pQuelle 
      movq mm2,qword ptr[edx+eax+8]          // mm0 = *pQuelle 
	  add eax,16                        // pQuelle += 8

      psubb mm0,mm6                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      psubb mm2,mm6                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      psraw mm0,8                      // mm0 = V2 V2 U2 U2 V0 V0 U0 U0  U und V auf 16 Bit erweitern
      psraw mm2,8                      // mm0 = V2 V2 U2 U2 V0 V0 U0 U0  U und V auf 16 Bit erweitern
      movq mm1,mm0                     // mm1 = V2 V2 U2 U2 V0 V0 U0 U0  Zweites Pixelpaar nach mm1
      movq mm3,mm2                     // mm1 = V2 V2 U2 U2 V0 V0 U0 U0  Zweites Pixelpaar nach mm1

      punpckldq mm0,mm0                // mm0 = V0 V0 U0 U0 V0 V0 U0 U0  Untere 32 Bit in die oberen 32 Bit kopieren
      punpckhdq mm1,mm1                // mm1 = V2 V2 U2 U2 V2 V2 U2 U2  Obere 32 Bit in die unteren 32 Bit kopieren
      punpckldq mm2,mm2                // mm0 = V0 V0 U0 U0 V0 V0 U0 U0  Untere 32 Bit in die oberen 32 Bit kopieren
      punpckhdq mm3,mm3                // mm1 = V2 V2 U2 U2 V2 V2 U2 U2  Obere 32 Bit in die unteren 32 Bit kopieren

      pmaddwd mm0,mm7                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm1,mm7                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);

      pmaddwd mm2,mm7                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm3,mm7                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);
                                       //                                v2 = U2 * *(pColorFaktor+2) + V2 * *(pColorFaktor+3);
      psrad mm0,13                     // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  Geteilt durch 16384  // Change 14 13
      psrad mm1,13                     // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  Geteilt durch 16384  // Change 14 13
      psrad mm2,13                     // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  Geteilt durch 16384  // Change 14 13
      psrad mm3,13                     // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  Geteilt durch 16384  // Change 14 13
	  paddd mm0,mm1
	  paddd mm2,mm3
      paddd mm0,mm5                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      paddd mm2,mm5                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      paddd mm4,mm0
      paddd mm4,mm2
	  cmp eax,1420
      jl NaechsterPixelLinks         // Sind noch weitere Pixel in dieser Zeile

      sub eax,1420
      movq mm7,qword ptr[edi+8] 
      movq mm7,qword ptr[NormFarbe]

   NaechsterPixelRechts:
      movq mm0,qword ptr[edx+eax]          // mm0 = *pQuelle 
      movq mm2,qword ptr[edx+eax+8]          // mm0 = *pQuelle 

	  add eax,16                        // pQuelle += 8

      psubb mm0,mm6                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      psubb mm2,mm6                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      psraw mm0,8                      // mm0 = V2 V2 U2 U2 V0 V0 U0 U0  U und V auf 16 Bit erweitern
      psraw mm2,8                      // mm0 = V2 V2 U2 U2 V0 V0 U0 U0  U und V auf 16 Bit erweitern
      movq mm1,mm0                     // mm1 = V2 V2 U2 U2 V0 V0 U0 U0  Zweites Pixelpaar nach mm1
      movq mm3,mm2                     // mm1 = V2 V2 U2 U2 V0 V0 U0 U0  Zweites Pixelpaar nach mm1

      punpckldq mm0,mm0                // mm0 = V0 V0 U0 U0 V0 V0 U0 U0  Untere 32 Bit in die oberen 32 Bit kopieren
      punpckhdq mm1,mm1                // mm1 = V2 V2 U2 U2 V2 V2 U2 U2  Obere 32 Bit in die unteren 32 Bit kopieren
      punpckldq mm2,mm2                // mm0 = V0 V0 U0 U0 V0 V0 U0 U0  Untere 32 Bit in die oberen 32 Bit kopieren
      punpckhdq mm3,mm3                // mm1 = V2 V2 U2 U2 V2 V2 U2 U2  Obere 32 Bit in die unteren 32 Bit kopieren

      pmaddwd mm0,mm7                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm1,mm7                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);

      pmaddwd mm2,mm7                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm3,mm7                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);
                                       //                                v2 = U2 * *(pColorFaktor+2) + V2 * *(pColorFaktor+3);
      psrad mm0,13                     // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  Geteilt durch 16384  // Change 14 13
      psrad mm1,13                     // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  Geteilt durch 16384  // Change 14 13
      psrad mm2,13                     // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  Geteilt durch 16384  // Change 14 13
      psrad mm3,13                     // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  Geteilt durch 16384  // Change 14 13
	  paddd mm0,mm1
	  paddd mm2,mm3
      paddd mm0,mm5                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      paddd mm2,mm5                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      paddd mm4,mm0
      paddd mm4,mm2
	  cmp eax,ecx
	  jle NaechsterPixelRechts         // Sind noch weitere Pixel in dieser Zeile

	  movd xu,mm4
	  psrlq mm4,32
	  movd xv,mm4
      emms                             
   }
*U=xu;
*V=xv;
}


void YFillNew2Part(short *pQuelle, BYTE *pZielY )
{

	__int64 PackHeller=0x0000000800000008;


__asm {
	  mov edi,pQuelle
	  mov esi,pZielY
	  xor eax,eax
	  mov ecx,45
      movq mm7,qword ptr[nMaske]      
      movq mm4,qword ptr [PackHeller]     

NaechsterFillPixel:
	  movq mm0,qword ptr[edi+eax] 
	  movq mm1,qword ptr[edi+eax+8] 
	  movq mm2,qword ptr[edi+eax+16] 
	  movq mm3,qword ptr[edi+eax+24] 
	  pand mm0,mm7
	  pand mm1,mm7
	  pand mm2,mm7
	  pand mm3,mm7
      packuswb mm0,mm1
      packuswb mm2,mm3
      add eax,32
	  movq qword ptr[esi+710],mm0
	  movq qword ptr[esi+718],mm2
      psubb mm0,mm4
      psubb mm2,mm4
	  movq qword ptr[esi],mm0
	  movq qword ptr[esi+8],mm2
	  add esi,16
loop NaechsterFillPixel

	  mov esi,pZielY
	  movq mm0,qword ptr[esi] 
	  movq mm1,qword ptr[esi+8] 
      paddb mm0,mm4
      paddb mm2,mm4
	  movq qword ptr[esi+710],mm0 
	  movq qword ptr[esi+718],mm1 
      emms
}
}



int Find_Shift(BYTE *pQuelle)
{
int BestCut;
	__asm {
      mov eax,0x0fffffff
      xor ebx,ebx
	  pxor mm6,mm6
      align 16 

NewTest:
      pxor mm7,mm7
	  mov  ecx,22
	  mov edi,pQuelle
NextQuad:
  	  movq mm0,qword ptr[edi] 
	  movq mm1,qword ptr[edi+8] 
	  movq mm2,qword ptr[edi+16] 
	  movq mm3,qword ptr[edi+24] 
  	  movq mm4,qword ptr[edi+ebx+712] 
	  movq mm5,qword ptr[edi+ebx+720] 
	  psubusb mm4,mm0
	  psubusb mm5,mm1
	  psubusb mm0,qword ptr[edi+ebx+712] 
	  psubusb mm1,qword ptr[edi+ebx+720] 
	  por mm0,mm4
	  por mm1,mm5
  	  movq mm4,qword ptr[edi+ebx+728] 
	  movq mm5,qword ptr[edi+ebx+736] 
	  psubusb mm4,mm2
	  psubusb mm5,mm3
	  psubusb mm2,qword ptr[edi+ebx+728] 
	  psubusb mm3,qword ptr[edi+ebx+736] 
	  por mm2,mm4
	  por mm3,mm5
	  movq       mm4,mm0
      movq       mm5,mm1
      punpckhbw  mm0,mm6
      punpckhbw  mm1,mm6
      punpcklbw  mm4,mm6
      punpcklbw  mm5,mm6
	  paddw      mm0,mm4
	  paddw      mm1,mm5
	  movq       mm4,mm2
      movq       mm5,mm3
      punpckhbw  mm2,mm6
      punpckhbw  mm3,mm6
      punpcklbw  mm4,mm6
      punpcklbw  mm5,mm6
	  paddw      mm2,mm4
	  paddw      mm3,mm5
	  paddw      mm0,mm1
	  paddw      mm2,mm3
      pmaddwd	 mm0,qword ptr[One] 
      pmaddwd	 mm2,qword ptr[One] 
      paddd      mm0,mm2
	  paddd      mm7,mm0
	  psrlq      mm0, 32 
	  paddd      mm7,mm0

      movd       esi,mm7
	  cmp        esi,eax
	  jge        NextCut

	  add        edi,32
      dec        ecx
      jnz        NextQuad
      mov        eax,esi
      mov        edx,ebx

NextCut:

	  add        ebx,12
      cmp        ebx,712
	  jl         NewTest
      mov        BestCut,edx
	  cmp        edx,0
      jnz        NichNull
      
NichNull:
      mov        ebx,edx
      sub ebx,   10

FNewTest:
      pxor mm7,mm7
	  mov  ecx,22
	  mov edi,pQuelle
FNextQuad:
  	  movq mm0,qword ptr[edi] 
	  movq mm1,qword ptr[edi+8] 
	  movq mm2,qword ptr[edi+16] 
	  movq mm3,qword ptr[edi+24] 
  	  movq mm4,qword ptr[edi+ebx+712] 
	  movq mm5,qword ptr[edi+ebx+720] 
	  psubusb mm4,mm0
	  psubusb mm5,mm1
	  psubusb mm0,qword ptr[edi+ebx+712] 
	  psubusb mm1,qword ptr[edi+ebx+720] 
	  por mm0,mm4
	  por mm1,mm5
  	  movq mm4,qword ptr[edi+ebx+728] 
	  movq mm5,qword ptr[edi+ebx+736] 
	  psubusb mm4,mm2
	  psubusb mm5,mm3
	  psubusb mm2,qword ptr[edi+ebx+728] 
	  psubusb mm3,qword ptr[edi+ebx+736] 
	  por mm2,mm4
	  por mm3,mm5
	  movq       mm4,mm0
      movq       mm5,mm1

      punpckhbw  mm0,mm6
      punpckhbw  mm1,mm6
      punpcklbw  mm4,mm6
      punpcklbw  mm5,mm6
	  paddw      mm0,mm4
	  paddw      mm1,mm5
	  movq       mm4,mm2
      movq       mm5,mm3
      punpckhbw  mm2,mm6
      punpckhbw  mm3,mm6
      punpcklbw  mm4,mm6
      punpcklbw  mm5,mm6
	  paddw      mm2,mm4
	  paddw      mm3,mm5
	  paddw      mm0,mm1
	  paddw      mm2,mm3
      pmaddwd	 mm0,qword ptr[One] 
      pmaddwd	 mm2,qword ptr[One] 
      paddd      mm0,mm2
	  paddd      mm7,mm0
	  psrlq      mm0, 32 
	  paddd      mm7,mm0

      movd       esi,mm7
	  cmp        esi,eax
	  jge        FNextCut

	  add        edi,32
      dec        ecx
      jnz        FNextQuad
      mov        eax,esi
      mov        BestCut,ebx

FNextCut:

	  add        ebx,2
      cmp        ebx,edx
	  jnz        FNewTest
      cmp        edx,708
	  jz         DasWars
	  add edx,12
      add ebx,2

LNewTest:
      pxor mm7,mm7
	  mov  ecx,22
	  mov edi,pQuelle
LNextQuad:
  	  movq mm0,qword ptr[edi] 
	  movq mm1,qword ptr[edi+8] 
	  movq mm2,qword ptr[edi+16] 
	  movq mm3,qword ptr[edi+24] 
  	  movq mm4,qword ptr[edi+ebx+712] 
	  movq mm5,qword ptr[edi+ebx+720] 
	  psubusb mm4,mm0
	  psubusb mm5,mm1
	  psubusb mm0,qword ptr[edi+ebx+712] 
	  psubusb mm1,qword ptr[edi+ebx+720] 
	  por mm0,mm4
	  por mm1,mm5
  	  movq mm4,qword ptr[edi+ebx+728] 
	  movq mm5,qword ptr[edi+ebx+736] 
	  psubusb mm4,mm2
	  psubusb mm5,mm3
	  psubusb mm2,qword ptr[edi+ebx+728] 
	  psubusb mm3,qword ptr[edi+ebx+736] 
	  por mm2,mm4
	  por mm3,mm5
	  movq       mm4,mm0
      movq       mm5,mm1

      punpckhbw  mm0,mm6
      punpckhbw  mm1,mm6
      punpcklbw  mm4,mm6
      punpcklbw  mm5,mm6
	  paddw      mm0,mm4
	  paddw      mm1,mm5
	  movq       mm4,mm2
      movq       mm5,mm3
      punpckhbw  mm2,mm6
      punpckhbw  mm3,mm6
      punpcklbw  mm4,mm6
      punpcklbw  mm5,mm6
	  paddw      mm2,mm4
	  paddw      mm3,mm5
	  paddw      mm0,mm1
	  paddw      mm2,mm3
      pmaddwd	 mm0,qword ptr[One] 
      pmaddwd	 mm2,qword ptr[One] 
      paddd      mm0,mm2
	  paddd      mm7,mm0
	  psrlq      mm0, 32 
	  paddd      mm7,mm0

      movd       esi,mm7
	  cmp        esi,eax
	  jge        LNextCut

	  add        edi,32
      dec        ecx
      jnz        LNextQuad
      mov        eax,esi
      mov        BestCut,ebx

LNextCut:

	  add        ebx,2
      cmp        ebx,edx
	  jnz        LNewTest

DasWars:
      mov  edx,BestCut

  	  mov edi,pQuelle
	  mov ecx,44
	  mov eax,edx
      add eax,712

NaechsterFillPixel:
	  movq mm0,qword ptr[edi+eax] 
	  movq mm1,qword ptr[edi+eax+8] 
	  movq qword ptr[edi],mm0
	  movq qword ptr[edi+8],mm1
	  add edi,16
      loop NaechsterFillPixel
	  movq mm0,qword ptr[edi+eax] 
	  movq qword ptr[edi],mm0
	  mov eax,edx
	  cmp eax,0
	  jnz DoDec
      mov eax,710
DoDec:
	  dec eax
	  emms
      mov BestCut,eax
	}
return(BestCut);
};




unsigned int ChrominanzCompare(short *pCompare1,short *pCompare2)
{
	int Wert;
	_asm {
	  mov edi,pCompare1
      mov esi,pCompare2
      pxor  mm4,mm4
      xor eax,eax
      movq  mm5,mm4
	  movq mm7,qword ptr[One] 
      movq mm6,mm4
	  mov ecx,88
      align 16 

   NaechsteChrominanz:
	  movq mm2, qword ptr[esi+eax]
	  movq mm3, qword ptr[esi+eax+8]
	  movq mm0, qword ptr[edi+eax]
	  movq mm1, qword ptr[edi+eax+8]
	  psubusb	mm0,mm2
	  psubusb	mm1,mm3
	  psubusb	mm2,qword ptr[edi+eax]
	  psubusb	mm3,qword ptr[edi+eax+8]

      por        mm0,mm2
      por        mm1,mm3
      movq       mm2,mm0
      movq       mm3,mm1
	  add eax,16
      punpckhbw  mm0,mm5
      punpckhbw  mm1,mm5
      punpcklbw  mm2,mm5
      punpcklbw  mm3,mm5
      paddw      mm0,mm1
      paddw      mm2,mm3
	  pmaddwd	 mm0,mm7
	  pmaddwd	 mm2,mm7
      paddd      mm6, mm0
      paddd      mm4, mm2
      loop NaechsteChrominanz 
      paddd      mm6, mm4           
      movq       mm0, mm6
      psrlq      mm6, 32 
	  paddd      mm6, mm0	
	  movd Wert,mm6
emms
	}
return(Wert);
}

unsigned int ChrominanzComparePur(BYTE *pCompare1,BYTE *pCompare2)
{
	int Wert;
	_asm {
	  mov edi,pCompare1
      mov esi,pCompare2
      pxor  mm4,mm4
      xor eax,eax
      movq  mm5,mm4
	  movq mm7,qword ptr[One] 
      movq mm6,mm4
	  mov ecx,44
      align 16 

   NaechsteChrominanz:
	  movq mm2, qword ptr[esi+eax]
	  movq mm3, qword ptr[esi+eax+8]
	  movq mm0, qword ptr[edi+eax]
	  movq mm1, qword ptr[edi+eax+8]
	  psubusb	mm0,mm2
	  psubusb	mm1,mm3
	  psubusb	mm2,qword ptr[edi+eax]
	  psubusb	mm3,qword ptr[edi+eax+8]

      por        mm0,mm2
      por        mm1,mm3
      movq       mm2,mm0
      movq       mm3,mm1
	  add eax,16
      punpckhbw  mm0,mm5
      punpckhbw  mm1,mm5
      punpcklbw  mm2,mm5
      punpcklbw  mm3,mm5
      paddw      mm0,mm1
      paddw      mm2,mm3
	  pmaddwd	 mm0,mm7
	  pmaddwd	 mm2,mm7
      paddd      mm6, mm0
      paddd      mm4, mm2
      loop NaechsteChrominanz 
      paddd      mm6, mm4           
      movq       mm0, mm6
      psrlq      mm6, 32 
	  paddd      mm6, mm0	
	  movd Wert,mm6
emms
	}
return(Wert);
}


void VC_UVColor(short *pQuelle,BYTE *pZiel,short *pColorFaktor, int Cut)
{
    _asm
   {
	  mov  eax,Cut
      mov edi,dword ptr[pColorFaktor]
      shl eax,1
      mov edx,pQuelle
      add eax,2 
      mov ecx,edx
	  add ecx,eax
      add ecx,4
      movq mm7,qword ptr[edi]         
      movq mm4,qword ptr[nMaske]      
	  movq mm5,qword ptr[nMaskeUV]    
      movq mm6,qword ptr[nMinus128]   
      mov ebx,pZiel
   align 16

   NaechsterPixelLinks:
      movq mm0,qword ptr[edx+eax]          // mm0 = *pQuelle 
      add eax,8                        // pQuelle += 8

      movq mm2,mm0                     // mm2 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  Y nach mm2
      psubb mm0,mm6                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      pand mm2,mm4                     // mm2 = 00 Y3 00 Y2 00 Y1 00 Y0  Y Ausmaskieren
      psraw mm0,8                      // mm0 = V2 V2 U2 U2 V0 V0 U0 U0  U und V auf 16 Bit erweitern
      movq mm1,mm0                     // mm1 = V2 V2 U2 U2 V0 V0 U0 U0  Zweites Pixelpaar nach mm1
      punpckldq mm0,mm0                // mm0 = V0 V0 U0 U0 V0 V0 U0 U0  Untere 32 Bit in die oberen 32 Bit kopieren
      punpckhdq mm1,mm1                // mm1 = V2 V2 U2 U2 V2 V2 U2 U2  Obere 32 Bit in die unteren 32 Bit kopieren
      pmaddwd mm0,mm7                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm1,mm7                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);
                                       //                                v2 = U2 * *(pColorFaktor+2) + V2 * *(pColorFaktor+3);
      psrad mm0,14                     // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  Geteilt durch 16384  // Change 14 13
      psrad mm1,14                     // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  Geteilt durch 16384  // Change 14 13
      packssdw mm0,mm1                 // mm0 = v2 v2 u2 u2 v0 v0 u0 u0  Ergebnis auf 16 bit kürzen
      packsswb mm0,mm0                 // mm0 = v2 u2 v0 u0 v2 u2 v0 u0  Auf 8 Bit kürzen
      punpcklbw mm0,mm0                // mm0 = v2 v2 u2 u2 v0 v0 u0 u0
      pand mm0,mm5                     // mm0 = v2 00 u2 00 v0 00 u0 00  U und V ausmaskieren
	  psubb mm0,mm6                    // mm0 = v2 00 u2 00 v0 00 u0 00  U+128 V+128
	  por mm2,mm0                      // mm2 = v2 Y3 u2 Y2 v0 Y1 u0 Y0
      movq qword ptr[ebx],mm2          // *pZiel = mm2
      add ebx,8                        // pZiel += 8
	  cmp eax,1420
      jl NaechsterPixelLinks         // Sind noch weitere Pixel in dieser Zeile
      sub eax,1420
      sub ebx,eax 
      movq mm7,qword ptr[edi+8] 

   NaechsterPixelRechts:
      movq mm0,qword ptr[edx]          // mm0 = *pQuelle 
      add edx,8                        // pQuelle += 8

      movq mm2,mm0                     // mm2 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  Y nach mm2
      psubb mm0,mm6                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      pand mm2,mm4                     // mm2 = 00 Y3 00 Y2 00 Y1 00 Y0  Y Ausmaskieren
      psraw mm0,8                      // mm0 = V2 V2 U2 U2 V0 V0 U0 U0  U und V auf 16 Bit erweitern
      movq mm1,mm0                     // mm1 = V2 V2 U2 U2 V0 V0 U0 U0  Zweites Pixelpaar nach mm1
      punpckldq mm0,mm0                // mm0 = V0 V0 U0 U0 V0 V0 U0 U0  Untere 32 Bit in die oberen 32 Bit kopieren
      punpckhdq mm1,mm1                // mm1 = V2 V2 U2 U2 V2 V2 U2 U2  Obere 32 Bit in die unteren 32 Bit kopieren
      pmaddwd mm0,mm7                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm1,mm7                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);
                                       //                                v2 = U2 * *(pColorFaktor+2) + V2 * *(pColorFaktor+3);
      psrad mm0,14                     // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  Geteilt durch 16384  // Change 14 13
      psrad mm1,14                     // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  Geteilt durch 16384  // Change 14 13
      packssdw mm0,mm1                 // mm0 = v2 v2 u2 u2 v0 v0 u0 u0  Ergebnis auf 16 bit kürzen
      packsswb mm0,mm0                 // mm0 = v2 u2 v0 u0 v2 u2 v0 u0  Auf 8 Bit kürzen
      punpcklbw mm0,mm0                // mm0 = v2 v2 u2 u2 v0 v0 u0 u0
      pand mm0,mm5                     // mm0 = v2 00 u2 00 v0 00 u0 00  U und V ausmaskieren
	  psubb mm2,qword ptr [Heller]
	  psubb mm0,mm6                    // mm0 = v2 00 u2 00 v0 00 u0 00  U+128 V+128
	  por mm2,mm0                      // mm2 = v2 Y3 u2 Y2 v0 Y1 u0 Y0

      movq qword ptr[ebx],mm2          // *pZiel = mm2
      add ebx,8
	  cmp edx,ecx
	  jle NaechsterPixelRechts         // Sind noch weitere Pixel in dieser Zeile

      emms                             
   }
}


// Ohne Y-Werte 
void VC_UVColorPur(short *pQuelle,BYTE *pZiel,short *pColorFaktor, int Cut)
{
    _asm
   {

	  mov eax,Cut
      mov edi,dword ptr[pColorFaktor]
      shl eax,1
	  mov edx,pQuelle
      add eax,2
      mov ecx,eax
	  movq mm7,qword ptr[edi]         
      movq mm6,qword ptr[nMinus128]   
      mov esi,pZiel
      align 16

   NaechsterPixelLinks:
      movq mm0,qword ptr[edx+eax]          // mm0 = *pQuelle 
      movq mm2,qword ptr[edx+eax+8]          // mm0 = *pQuelle 
	  add eax,16                        // pQuelle += 8

      psubb mm0,mm6                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      psubb mm2,mm6                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      psraw mm0,8                      // mm0 = V2 V2 U2 U2 V0 V0 U0 U0  U und V auf 16 Bit erweitern
      psraw mm2,8                      // mm0 = V2 V2 U2 U2 V0 V0 U0 U0  U und V auf 16 Bit erweitern
      movq mm1,mm0                     // mm1 = V2 V2 U2 U2 V0 V0 U0 U0  Zweites Pixelpaar nach mm1
      movq mm3,mm2                     // mm1 = V2 V2 U2 U2 V0 V0 U0 U0  Zweites Pixelpaar nach mm1

      punpckldq mm0,mm0                // mm0 = V0 V0 U0 U0 V0 V0 U0 U0  Untere 32 Bit in die oberen 32 Bit kopieren
      punpckhdq mm1,mm1                // mm1 = V2 V2 U2 U2 V2 V2 U2 U2  Obere 32 Bit in die unteren 32 Bit kopieren
      punpckldq mm2,mm2                // mm0 = V0 V0 U0 U0 V0 V0 U0 U0  Untere 32 Bit in die oberen 32 Bit kopieren
      punpckhdq mm3,mm3                // mm1 = V2 V2 U2 U2 V2 V2 U2 U2  Obere 32 Bit in die unteren 32 Bit kopieren

      pmaddwd mm0,mm7                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm1,mm7                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);

      pmaddwd mm2,mm7                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm3,mm7                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);
                                       //                                v2 = U2 * *(pColorFaktor+2) + V2 * *(pColorFaktor+3);
      psrad mm0,13                     // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  Geteilt durch 16384  // Change 14 13
      psrad mm1,13                     // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  Geteilt durch 16384  // Change 14 13
      psrad mm2,13                     // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  Geteilt durch 16384  // Change 14 13
      psrad mm3,13                     // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  Geteilt durch 16384  // Change 14 13
      packssdw mm0,mm1                 // mm0 = v2 v2 u2 u2 v0 v0 u0 u0  Ergebnis auf 16 bit kürzen
      packssdw mm2,mm3                 // mm0 = v2 v2 u2 u2 v0 v0 u0 u0  Ergebnis auf 16 bit kürzen
      packsswb mm0,mm0                 // mm0 = v2 u2 v0 u0 v2 u2 v0 u0  Auf 8 Bit kürzen
      packsswb mm2,mm2                 // mm0 = v2 u2 v0 u0 v2 u2 v0 u0  Auf 8 Bit kürzen
      punpcklbw mm0,mm0                // mm0 = v2 v2 u2 u2 v0 v0 u0 u0
      punpcklbw mm2,mm2                // mm0 = v2 v2 u2 u2 v0 v0 u0 u0
	  psubb mm0,mm6                    // mm0 = v2 00 u2 00 v0 00 u0 00  U+128 V+128
	  psubb mm2,mm6                    // mm0 = v2 00 u2 00 v0 00 u0 00  U+128 V+128
      psrlw mm0,8
      psrlw mm2,8
      packuswb mm0,mm2
      movq qword ptr[esi],mm0          // *pZiel = mm2
      add esi,8                        // pZiel += 8
	  cmp eax,1420
      jl NaechsterPixelLinks         // Sind noch weitere Pixel in dieser Zeile
      sub eax,1420
	  mov ebx,eax
      shr ebx,1
      cmp ebx,1
	  jnz N1
	  sub esi,6
      jmp Weiter
N1:
      cmp ebx,4
	  jg N2
	  sub esi,4
      jmp Weiter
N2:
      cmp ebx,6
	  jnz Weiter
	  sub esi,2
Weiter:
      movq mm7,qword ptr[edi+8] 

   NaechsterPixelRechts:
      movq mm0,qword ptr[edx+eax]          // mm0 = *pQuelle 
      movq mm2,qword ptr[edx+eax+8]          // mm0 = *pQuelle 

	  add eax,16                        // pQuelle += 8

      psubb mm0,mm6                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      psubb mm2,mm6                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      psraw mm0,8                      // mm0 = V2 V2 U2 U2 V0 V0 U0 U0  U und V auf 16 Bit erweitern
      psraw mm2,8                      // mm0 = V2 V2 U2 U2 V0 V0 U0 U0  U und V auf 16 Bit erweitern
      movq mm1,mm0                     // mm1 = V2 V2 U2 U2 V0 V0 U0 U0  Zweites Pixelpaar nach mm1
      movq mm3,mm2                     // mm1 = V2 V2 U2 U2 V0 V0 U0 U0  Zweites Pixelpaar nach mm1

      punpckldq mm0,mm0                // mm0 = V0 V0 U0 U0 V0 V0 U0 U0  Untere 32 Bit in die oberen 32 Bit kopieren
      punpckhdq mm1,mm1                // mm1 = V2 V2 U2 U2 V2 V2 U2 U2  Obere 32 Bit in die unteren 32 Bit kopieren
      punpckldq mm2,mm2                // mm0 = V0 V0 U0 U0 V0 V0 U0 U0  Untere 32 Bit in die oberen 32 Bit kopieren
      punpckhdq mm3,mm3                // mm1 = V2 V2 U2 U2 V2 V2 U2 U2  Obere 32 Bit in die unteren 32 Bit kopieren

      pmaddwd mm0,mm7                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm1,mm7                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);

      pmaddwd mm2,mm7                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm3,mm7                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);
                                       //                                v2 = U2 * *(pColorFaktor+2) + V2 * *(pColorFaktor+3);
      psrad mm0,13                     // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  Geteilt durch 16384  // Change 14 13
      psrad mm1,13                     // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  Geteilt durch 16384  // Change 14 13
      psrad mm2,13                     // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  Geteilt durch 16384  // Change 14 13
      psrad mm3,13                     // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  Geteilt durch 16384  // Change 14 13
      packssdw mm0,mm1                 // mm0 = v2 v2 u2 u2 v0 v0 u0 u0  Ergebnis auf 16 bit kürzen
      packssdw mm2,mm3                 // mm0 = v2 v2 u2 u2 v0 v0 u0 u0  Ergebnis auf 16 bit kürzen
      packsswb mm0,mm0                 // mm0 = v2 u2 v0 u0 v2 u2 v0 u0  Auf 8 Bit kürzen
      packsswb mm2,mm2                 // mm0 = v2 u2 v0 u0 v2 u2 v0 u0  Auf 8 Bit kürzen
      punpcklbw mm0,mm0                // mm0 = v2 v2 u2 u2 v0 v0 u0 u0
      punpcklbw mm2,mm2                // mm0 = v2 v2 u2 u2 v0 v0 u0 u0
	  psubb mm0,mm6                    // mm0 = v2 00 u2 00 v0 00 u0 00  U+128 V+128
	  psubb mm2,mm6                    // mm0 = v2 00 u2 00 v0 00 u0 00  U+128 V+128
      psrlw mm0,8
      psrlw mm2,8
      packuswb mm0,mm2
      movq qword ptr[esi],mm0          // *pZiel = mm2
      add esi,8
	  cmp eax,ecx
	  jle NaechsterPixelRechts         // Sind noch weitere Pixel in dieser Zeile

      emms                             
   }
}

void __fastcall VCHQAusgebenYUV2() 
{

	_asm  {

  	  mov edi,dword ptr[ColFaktor]
      mov Roll,0
      pxor mm6,mm6
	  mov Ende1,eax
	  movq qword ptr [SubWert],mm6
      mov ebx,1420
      xor ecx,ecx 
      movq mm7,qword ptr[edi]         
      mov edi,TmpPtr
      movq mm4,qword ptr[nMaske]      
	  movq mm5,qword ptr[nMaskeUV]    
      movq mm6,qword ptr[nMinus128]   
   align 16

   NaechsterPixel:
      movq mm0,qword ptr[edx+eax]          // mm0 = *pQuelle 
      add eax,8                        // pQuelle += 8
      movq mm2,mm0                     // mm2 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  Y nach mm2
      psubb mm0,mm6                    // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      pand mm2,mm4                     // mm2 = 00 Y3 00 Y2 00 Y1 00 Y0  Y Ausmaskieren
      psraw mm0,8                      // mm0 = V2 V2 U2 U2 V0 V0 U0 U0  U und V auf 16 Bit erweitern
      movq mm1,mm0                     // mm1 = V2 V2 U2 U2 V0 V0 U0 U0  Zweites Pixelpaar nach mm1
      punpckldq mm0,mm0                // mm0 = V0 V0 U0 U0 V0 V0 U0 U0  Untere 32 Bit in die oberen 32 Bit kopieren
      punpckhdq mm1,mm1                // mm1 = V2 V2 U2 U2 V2 V2 U2 U2  Obere 32 Bit in die unteren 32 Bit kopieren
      pmaddwd mm0,mm7                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm1,mm7                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);
                                       //                                v2 = U2 * *(pColorFaktor+2) + V2 * *(pColorFaktor+3);
      psrad mm0,13                     // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  Geteilt durch 16384  // Change 14 13
      psrad mm1,13                     // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  Geteilt durch 16384  // Change 14 13
      pmaddwd mm0,qword ptr[NormFarbe]
      pmaddwd mm1,qword ptr[NormFarbe]
      movq mm3,qword ptr[esi+ecx]  
      psrad mm0,14                     
      psrad mm1,14                     
      packssdw mm0,mm1                 
      movq qword ptr[esi+ecx],mm0  
      paddsw mm0,mm3                   
      packsswb mm0,mm0                 
      punpcklbw mm0,mm0                
// Ab jetzt Normal
	  pand mm0,mm5                     // mm0 = v2 00 u2 00 v0 00 u0 00  U und V ausmaskieren
	  psubb mm2,qword ptr [SubWert]
	  psubb mm0,mm6                    // mm0 = v2 00 u2 00 v0 00 u0 00  U+128 V+128
	  por mm2,mm0                      // mm2 = v2 Y3 u2 Y2 v0 Y1 u0 Y0
      movq qword ptr[edi+ecx],mm2          // *pZiel = mm2
      add ecx,8                        // pZiel += 8
	  cmp ecx,BytesOut
	  jge AllesEnde 
	  cmp eax,ebx
      jl NaechsterPixel               // Sind noch weitere Pixel in dieser Zeile
      cmp Roll,1
	  jz AllesEnde
      sub eax,1420
      mov Roll,1
      cmp eax,0
	  jz NoRestore
	  sub ecx,eax            
      psrlq mm3,32
	  movd edi,mm3
      mov dword ptr[esi+ecx],edi
NoRestore:	  
      mov edi,dword ptr[ColFaktor]
	  movq mm0,qword ptr[Heller] 
      movq mm7,qword ptr[edi+8]
	  movq qword ptr [SubWert],mm0
      mov edi,TmpPtr
      mov ebx,Ende1
	  xor eax,eax
      jmp  NaechsterPixel
AllesEnde:	  
	  mov eax,LinesAnzahl
      cmp eax,1
	  jz NixMehr
	  mov ecx,BytesOut
	  mov esi,edi
	  mov edx,CurrentPitch
      sub edx,ecx
	  mov edi,ScreenPtr
      mov ebx,ecx
	  shr ecx,2 
NextLine:
	  rep movsd
      mov ecx,ebx
	  sub esi,ecx
	  shr ecx,2
	  add edi,edx
	  dec eax
	  jnz NextLine 
NixMehr: 
      emms            

	
	}
}

void __fastcall VCHQAusgebenRGB16()
{
	int SaveHQ;
    _asm
   {
  	 mov edi,dword ptr[ColFaktor]
      pxor mm6,mm6
	  mov Roll,0
	  mov Ende1,eax
	  movq qword ptr [SubWert],mm6
      mov ebx,1420
      xor ecx,ecx 
      movq mm7,qword ptr[edi]         
      mov edi,TmpPtr
      movq mm4,qword ptr[nMaske]      
   align 16

   NaechsterPixel:
      movq mm0,qword ptr[edx+eax]          // mm0 = *pQuelle 
      add eax,8                        // pQuelle += 8
      movq mm2,mm0                     // mm2 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  Y nach mm2
      psubb mm0,qword ptr[nMinus128]   // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      pand mm2,mm4                     // mm2 = 00 Y3 00 Y2 00 Y1 00 Y0  Y Ausmaskieren
      psraw mm0,8                      // mm0 = V2 V2 U2 U2 V0 V0 U0 U0  U und V auf 16 Bit erweitern
	  psubb mm2,qword ptr [SubWert]
      movq mm1,mm0                     // mm1 = V2 V2 U2 U2 V0 V0 U0 U0  Zweites Pixelpaar nach mm1
      psrlw mm2,01
	  punpckldq mm0,mm0                // mm0 = V0 V0 U0 U0 V0 V0 U0 U0  Untere 32 Bit in die oberen 32 Bit kopieren
      punpckhdq mm1,mm1                // mm1 = V2 V2 U2 U2 V2 V2 U2 U2  Obere 32 Bit in die unteren 32 Bit kopieren
      pmaddwd mm0,mm7                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm1,mm7                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);
      movq mm5,mm2                     //                                v2 = U2 * *(pColorFaktor+2) + V2 * *(pColorFaktor+3);
      psrad mm0,13                     // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  Geteilt durch 16384  // Change 14 13
      psrad mm1,13                     // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  Geteilt durch 16384  // Change 14 13
      punpcklbw mm2,mm2
      pmaddwd mm0,qword ptr[NormFarbe]                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm1,qword ptr[NormFarbe]                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);
      psrad mm0,0x0b  // 0x0c
      psrad mm1,0x0b  // 0x0c
	  
	  movq mm6,qword ptr[esi+ecx]  
      packssdw mm0,mm1
      movq qword ptr[esi+ecx],mm0  
// Ab jetzt Normal
      paddsw mm0,mm6
      psrlq mm6,32
	  punpckhbw mm5,mm5
	  movd SaveHQ,mm6
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
      movq qword ptr[edi+ecx],mm2          // *pZiel = mm2
      add ecx,8                        // pZiel += 8
	  cmp ecx,BytesOut
	  jge AllesEnde 
	  cmp eax,ebx
      jl NaechsterPixel               // Sind noch weitere Pixel in dieser Zeile
      cmp Roll,1
	  jz AllesEnde
      sub eax,1420
      mov Roll,1
      cmp eax,0
	  jz NoRestore
	  sub ecx,eax            
      mov edi,SaveHQ
      mov dword ptr[esi+ecx],edi
NoRestore:	  
	  mov edi,dword ptr[ColFaktor]
	  movq mm0,qword ptr[Heller] 
      movq mm7,qword ptr[edi+8]
	  movq qword ptr [SubWert],mm0
      mov edi,TmpPtr
      mov ebx,Ende1
	  xor eax,eax
      jmp  NaechsterPixel
AllesEnde:	  
	  mov eax,LinesAnzahl
      cmp eax,1
	  jz NixMehr
	  mov ecx,BytesOut
	  mov esi,edi
	  mov edx,CurrentPitch
      sub edx,ecx
	  mov edi,ScreenPtr
      mov ebx,ecx
	  shr ecx,2 
NextLine:
	  rep movsd
      mov ecx,ebx
	  sub esi,ecx
	  shr ecx,2
	  add edi,edx
	  dec eax
	  jnz NextLine 
NixMehr: 
      emms            
}
}

void __fastcall VCHQAusgebenRGB15()
{
int SaveHQ;
    _asm
   {
      mov edi,dword ptr[ColFaktor]
      pxor mm6,mm6
	  mov Roll,0
	  mov Ende1,eax
	  movq qword ptr [SubWert],mm6
      mov ebx,1420
      xor ecx,ecx 
      movq mm7,qword ptr[edi]         
      mov edi,TmpPtr
      movq mm4,qword ptr[nMaske]      
   align 16

   NaechsterPixel:
      movq mm0,qword ptr[edx+eax]          // mm0 = *pQuelle 
      add eax,8                        // pQuelle += 8
      movq mm2,mm0                     // mm2 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  Y nach mm2
      psubb mm0,qword ptr[nMinus128]   // mm0 = V2 Y3 U2 Y2 V0 Y1 U0 Y0  U-128 V-128
      pand mm2,mm4                     // mm2 = 00 Y3 00 Y2 00 Y1 00 Y0  Y Ausmaskieren
      psraw mm0,8                      // mm0 = V2 V2 U2 U2 V0 V0 U0 U0  U und V auf 16 Bit erweitern
	  psubb mm2,qword ptr [SubWert]
      movq mm1,mm0                     // mm1 = V2 V2 U2 U2 V0 V0 U0 U0  Zweites Pixelpaar nach mm1
      psrlw mm2,01
	  punpckldq mm0,mm0                // mm0 = V0 V0 U0 U0 V0 V0 U0 U0  Untere 32 Bit in die oberen 32 Bit kopieren
      punpckhdq mm1,mm1                // mm1 = V2 V2 U2 U2 V2 V2 U2 U2  Obere 32 Bit in die unteren 32 Bit kopieren
      pmaddwd mm0,mm7                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm1,mm7                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);
      movq mm5,mm2                     //                                v2 = U2 * *(pColorFaktor+2) + V2 * *(pColorFaktor+3);
      psrad mm0,13                     // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  Geteilt durch 16384  // Change 14 13
      psrad mm1,13                     // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  Geteilt durch 16384  // Change 14 13
      punpcklbw mm2,mm2
      pmaddwd mm0,qword ptr[NormFarbe]                  // mm0 = v0 v0 v0 v0 u0 u0 u0 u0  u0 = U0 * *(pColorFaktor+0) + V0 * *(pColorFaktor+1);
                                       //                                v0 = U0 * *(pColorFaktor+2) + V0 * *(pColorFaktor+3);
      pmaddwd mm1,qword ptr[NormFarbe]                  // mm1 = v2 v2 v2 v2 u2 u2 u2 u2  u2 = U2 * *(pColorFaktor+0) + V2 * *(pColorFaktor+1);
      psrad mm0,0x0b  // 0x0c
      psrad mm1,0x0b  // 0x0c
	  
	  movq mm6,qword ptr[esi+ecx]  
      packssdw mm0,mm1
      movq qword ptr[esi+ecx],mm0  
// Ab jetzt Normal
      paddsw mm0,mm6
      psrlq mm6,32
	  punpckhbw mm5,mm5
	  movd SaveHQ,mm6
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
      movq qword ptr[edi+ecx],mm2          // *pZiel = mm2
      add ecx,8                        // pZiel += 8
	  cmp ecx,BytesOut
	  jge AllesEnde 
	  cmp eax,ebx
      jl NaechsterPixel               // Sind noch weitere Pixel in dieser Zeile
      cmp Roll,1
	  jz AllesEnde
      sub eax,1420
      mov Roll,1
      cmp eax,0
	  jz NoRestore
	  sub ecx,eax            
      mov edi,SaveHQ
      mov dword ptr[esi+ecx],edi
NoRestore:	  
      mov edi,dword ptr[ColFaktor]
	  movq mm0,qword ptr[Heller] 
      movq mm7,qword ptr[edi+8]
	  movq qword ptr [SubWert],mm0
      mov edi,TmpPtr
      mov ebx,Ende1
	  xor eax,eax
      jmp  NaechsterPixel
AllesEnde:	  
	  mov eax,LinesAnzahl
      cmp eax,1
	  jz NixMehr
	  mov ecx,BytesOut
	  mov esi,edi
	  mov edx,CurrentPitch
      sub edx,ecx
	  mov edi,ScreenPtr
      mov ebx,ecx
	  shr ecx,2 
NextLine:
	  rep movsd
      mov ecx,ebx
	  sub esi,ecx
	  shr ecx,2
	  add edi,edx
	  dec eax
	  jnz NextLine 
NixMehr: 
      emms            
}
}

#endif

//------------------------------------------------

