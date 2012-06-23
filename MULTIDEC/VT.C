/*********************************************************************************
 *                                                                               *
 * vt.c                                                                          *
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
#include <stdlib.h>        /* atoi                                  */
#include <memory.h>         
#include <io.h>         
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "resource.h"
#include "vt.h"

int New_Dialog_Slot(HWND hwnd);
int Get_Dialog_Slot(HWND hwnd);
int Del_Dialog_Slot(HWND hwnd);
void VT_ChannelChange();
void Export_VT_Page(HWND hwnd, int Page, int SubPage);
void SetDoubleMode(BOOL bHasDouble,HDC *hDC,HFONT *hFontText,HFONT *hFontDoubleText);
void Prepare_Header(BITMAPINFO *Header);
void Prepare_Palette(HDC hDC);
BOOL Create_File_Bitmap(LPSTR FileName,int Page, int SubPage, int FontNr, BOOL Background);
BOOL Make_Txt_Page(FILE *fp , int Page, int SubPage);
BOOL Make_html_Page(FILE *fp , LPSTR LinkRoot,LPSTR Text, int Page, int SubPage, int FontNr);
BOOL Make_html_Background_Page(FILE *fp ,LPSTR LinkRoot, LPSTR Text, int Page, int SubPage, int FontNr);
BOOL Init_HTM_Line(BOOL bDouble, int FontNr,unsigned char Color,unsigned char *ZeilenPuffer);
BOOL SetModeBlink(BOOL Blink, unsigned char *ZeilenPuffer);
BOOL SetModeDouble(BOOL bDouble, int FontNr,unsigned char *ZeilenPuffer);
BOOL SetFGColor(unsigned char Color, unsigned char *ZeilenPuffer);
BOOL Is_Val(unsigned char Byte);
BOOL Is_Link(int Page, int Subpage , int row, int n, int *LinkPage);
BOOL SetCell(FILE *fp , BOOL bDouble, int FontNr,unsigned char FGColor,unsigned char BGColor, BOOL bFlash,BOOL Link , LPSTR LinkTo , unsigned char ch );
BOOL Make_Page_Control( FILE *fp ,int Page, int SubPage, int FontNr,LPSTR LinkRoot);

extern void Restore_Mode();

 // Teletext to MD font translation. This font is designed so that
 // common character mapping is like ASCII, national characters are
 // mapped in the range 0x00-0x31 (not room for all codes, so Swedish
 // chars are not all mapped). Characters from 0x80 to 0xbf are
 // graphic blocks, characters 0xc0 to 0xff are graphic separated blocks.

 static char VTtoFont[8][96] = {
 	" !\"\x23\x24%&'()*+,-./0123456789:;<=>?" // English
 	"\x40" "ABCDEFGHIJKLMNOPQRSTUVWXYZ\x5b\x5c\x5d\x5e\x5f"
 	"\x60" "abcdefghijklmnopqrstuvwxyz\x7b\x7c\x7d\x7e\x7f",

 	" !\"\x5f\x24%&'()*+,-./0123456789:;<=>?" // German
 	"\x00" "ABCDEFGHIJKLMNOPQRSTUVWXYZ\x09\x0a\x0b\x01\x02"
 	"\x30" "abcdefghijklmnopqrstuvwxyz\x1b\x1e\x1f\x04\x7f",

 	" !\"\x5f\x2a%&'()*+,-./0123456789:;<=>?" // Swedish
 	"\x0d" "ABCDEFGHIJKLMNOPQRSTUVWXYZ\x09\x0a\x41\x0b\x02"
 	"\x0d" "abcdefghijklmnopqrstuvwxyz\x1b\x1e\x0c\x1f\x7f",

 	" !\"\x23\x24%&'()*+,-./0123456789:;<=>?" // Italian
 	"\x0d" "ABCDEFGHIJKLMNOPQRSTUVWXYZ\x03\x06\x5d\x5e\x5f"
 	"\x15" "abcdefghijklmnopqrstuvwxyz\x11\x14\x12\x13\x7f",

 	" !\"\x0d\x1d%&'()*+,-./0123456789:;<=>?" // French
 	"\x11" "ABCDEFGHIJKLMNOPQRSTUVWXYZ\x1c\x17\x15\x18\x5f"
 	"\x12" "abcdefghijklmnopqrstuvwxyz\x16\x19\x1a\x06\x7f",

 	" !\"\x06\x24%&'()*+,-./0123456789:;<=>?" // Spanish
 	"\x05" "ABCDEFGHIJKLMNOPQRSTUVWXYZ\x0c\x0d\x0e\x0f\x10"
 	"\x07" "abcdefghijklmnopqrstuvwxyz\x1f\x08\x12\x11\x7f",

 	" !\"\x23\x24%&'()*+,-./0123456789:;<=>?" // English as default
 	"\x40" "ABCDEFGHIJKLMNOPQRSTUVWXYZ\x5b\x5c\x5d\x5e\x5f"
 	"\x60" "abcdefghijklmnopqrstuvwxyz\x7b\x7c\x7d\x7e\x7f",

 	" !\"\x23\x24%&'()*+,-./0123456789:;<=>?" // English as default
 	"\x40" "ABCDEFGHIJKLMNOPQRSTUVWXYZ\x5b\x5c\x5d\x5e\x5f"
 	"\x60" "abcdefghijklmnopqrstuvwxyz\x7b\x7c\x7d\x7e\x7f"
 };




extern void Restore_Mode();


int VT_GetPage(int nPage,BOOL *ErsteSub,int *SubNr)
{
int i;	
if(nPage<100 || nPage>899) return (-1);
if (VTFrame[nPage-100].SubCount==0 ) return(-1);
if ( *ErsteSub == TRUE ) {
     i=0;
	  while (( i < VTFrame[nPage-100].SubCount) && ( VTFrame[nPage-100].SubPage[i].Fill == FALSE )) i++;
      *SubNr=i;
}
*ErsteSub=FALSE;
return nPage-100;
}


void VT_DoUpdate_Page(HWND hWnd, int Page,int SubPage,int Dialog,BOOL bForceUpdate,int VTDisplayMode,int FontNr)
{
int OffsetX;
int StepX;
int StepY;
int CurrX;
int CurrY;

BOOL bGraph,bHoldGraph,bSepGraph,bBox,bFlash,bDouble,bConceal,bHasDouble;
BYTE nLastGraph;
unsigned short CurrentFg, CurrentBkg, RealBkg;
unsigned short OldFg, OldBkg;

BOOL OldDouble;
BYTE c,ch;
int n,row;
unsigned char tmp[41];
unsigned char tmp2[41];

char ZeichenPuffer[256];
int SaveLen;

int SaveX;
int endrow;
BOOL bHideTopLine,bForceShowTopLine;
unsigned short Black,ForceBlack,ForceTransparent;

LOGFONT VTText			= {11,9 ,0,0,0,0,0,0,1,0,0,0,0,"XXXXXXXXXX"};
LOGFONT DoubleText		= {22,9,0,0,0,0,0,0,1,0,0,0,0,"XXXXXXXXXX"};

HFONT hFontText,hFontDoubleText,hOldFont;
BYTE PageLang;

HDC hDCMem;
BOOL MakeAusgabe;

hDCMem=GetDC(hWnd);
 
sprintf(VTText.lfFaceName,"MD-F-%d",FontNr);
sprintf(DoubleText.lfFaceName,"MD-F-%dD",FontNr);
 


if ( FontNr == 1 ) {
			OffsetX=8; StepX=9;
			CurrY=38; StepY=11;
};
if ( FontNr == 2 ) {
			OffsetX=8; StepX=10;
			CurrY=34; StepY=15;
};
if ( FontNr == 3 ) {
			OffsetX=8; StepX=12;
			CurrY=31; StepY=18;
};

            hFontText     = CreateFontIndirect(&VTText);
            hFontDoubleText     = CreateFontIndirect(&DoubleText);


hOldFont   = SelectObject(hDCMem, hFontText);

CurrentFg=0;
OldFg=0xffff;
OldBkg=0xffff;
OldDouble=FALSE;
Black=0; // 0
ForceTransparent=8;
ForceBlack=0;
bHideTopLine=FALSE;
bForceShowTopLine=TRUE;
bHasDouble=FALSE;
endrow=25;
if ( VTFrame[Page].SubCount == 0 ) { PageLang=0;
	                                 endrow=1;
};
ZeichenPuffer[0]=0x00;
SaveLen=0;
for(row=0; row<endrow ; row++)
	{

	    if ( SaveLen > 0 ) TextOut(hDCMem,SaveX,CurrY,ZeichenPuffer,SaveLen);
	    CurrY+=StepY;
        CurrX=OffsetX;

		ZeichenPuffer[0]=0x00;
		SaveLen=0;
		SaveX=CurrX;

		if(bHasDouble) { bHasDouble=FALSE; continue; }
		bGraph=bHoldGraph=bSepGraph=bBox=bFlash=bDouble=bConceal=bHasDouble=FALSE;
		nLastGraph=32;

        if ( VTFrame[Page].SubCount == 0 ) {

		sprintf(tmp2,"  P%-3d \x7",VTDialog[Dialog].Page);
		for(n=0; n<40; n++)
		{
			tmp[n]=VT_Header_Line[n];
			if(tmp[n]==0x0d) bHasDouble=TRUE;
			strncpy(tmp,tmp2,8);
		}


		} else {

		PageLang = VTFrame[Page].SubPage[SubPage].Lang;

		for(n=0; n<40; n++)
		{
			tmp[n]=VTFrame[Page].SubPage[SubPage].Frame[row][n];
			if(tmp[n]==0x0d) bHasDouble=TRUE;
		}
		tmp[n]='\0';
		
		if(row==0)
		{
			sprintf(tmp2,"  P%-3d \x7",VTDialog[Dialog].Page);
			strncpy(tmp,tmp2,8);
		if ( VTFrame[Page].SubPage[SubPage].Fill == FALSE ) {
		for(n=0; n<40; n++)
		{
			tmp[n]=VT_Header_Line[n];
			if(tmp[n]==0x0d) bHasDouble=TRUE;
			strncpy(tmp,tmp2,8);
		}
		} else {
		for(n=30; n<40; n++)
		{
			tmp[n]=VT_Header_Line[n];
			if(tmp[n]==0x0d) bHasDouble=TRUE;
		}

		}
		}
		}

		RealBkg=ForceBlack;
		CurrentBkg=ForceBlack;
		CurrentFg=7;
		for(n=0; n<40; n++)
 		{
 			c=tmp[n];
 			ch=c;
						if(c<' ') {
							if(c<=0x07) {
								CurrentFg=c;
								bGraph=FALSE;
							} else if (c==0x08) {
								bFlash=TRUE;
							} else if (c==0x09) {
								bFlash=FALSE;
							} else if (c==0x0c) {
								bDouble=FALSE;
							} else if (c==0x0d) {
								bDouble=TRUE;
							} else if (c>=0x10 && c<=0x17) {
								bGraph=TRUE;
								CurrentFg=c-0x10;
							} else if (c==0x18) {
								bConceal=TRUE;
							} else if (c==0x19) {
								bSepGraph=FALSE;
							} else if (c==0x1a) {
								bSepGraph=TRUE;
							} else if (c==0x1c) {
								CurrentBkg=bBox?ForceBlack:Black;
								RealBkg=ForceBlack;
							} else if (c==0x1d) {
								CurrentBkg=(bBox || Black!=ForceTransparent) ?
									CurrentFg : ForceTransparent;
								RealBkg=CurrentFg;
							} else if (c==0x1e) {
								bHoldGraph=TRUE;
							} else if (c==0x1f) {
								bHoldGraph=FALSE;
							}
							ch=bHoldGraph?nLastGraph:' ';
 			}
 			if((CurrentFg==7) && (VTFrame[Page].SubCount==0) && (!row && n>7))
 				CurrentFg=2;

			if ( bGraph == FALSE ) {
         if (( ch >= '0' ) && ( ch <= '9')) VTDialog[Dialog].AsciiPuffer[row][n]=ch;
		 else VTDialog[Dialog].AsciiPuffer[row][n]=0x00;
		 if( bDouble == TRUE ) VTDialog[Dialog].AsciiPuffer[row+1][n]=VTDialog[Dialog].AsciiPuffer[row][n];
 			} else VTDialog[Dialog].AsciiPuffer[row][n]=0x00;


			if(bGraph && (ch&0x20)) {
 				nLastGraph=ch;
				ch=(ch&0x1f)|((ch&0x40)>>1) + 128;
				if(bSepGraph) ch+=64;
			} else {
				if (ch < ' ') ch = ' ';
				ch = VTtoFont[PageLang][ch-' '];
 			}

 			if (SaveLen > 0 ) {
 				MakeAusgabe=FALSE;
 				if ( CurrentFg != OldFg ) MakeAusgabe=TRUE;
				if ( CurrentBkg != OldBkg) MakeAusgabe=TRUE;
                if ( OldDouble != bHasDouble) MakeAusgabe=TRUE;
                if ( MakeAusgabe == TRUE ) {
                 TextOut(hDCMem,SaveX,CurrY,ZeichenPuffer,SaveLen);
				 CurrX+=(SaveLen*StepX);
                 SaveX=CurrX;
				 SaveLen=0;
				 ZeichenPuffer[0]=0x00;
				}
			};
            if ( CurrentFg != OldFg ) {
				OldFg=CurrentFg ;
				SetTextColor(hDCMem,VTColourTable[CurrentFg]);
			};
			if ( CurrentBkg != OldBkg) {
				OldBkg=CurrentBkg ;
                SetBkColor(hDCMem,VTColourTable[CurrentBkg]);
			};
            if ( bHasDouble  != OldDouble ) {
                 OldDouble=bHasDouble;
                 SetDoubleMode(bHasDouble,&hDCMem,&hFontText,&hFontDoubleText);
			};
            ZeichenPuffer[SaveLen++] = ch;ZeichenPuffer[SaveLen]=0x00;
	}
if (( row+1 == endrow ) && ( SaveLen > 0 )) TextOut(hDCMem,SaveX,CurrY,ZeichenPuffer,SaveLen);
  }

SelectObject(hDCMem, hOldFont );
DeleteObject(hFontText);
DeleteObject(hFontDoubleText);
ReleaseDC(hWnd,hDCMem);
return;
};



BOOL APIENTRY VideoTextProc(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{

int Slot;
int i;
int x,y;
char Buffer[10];
char Text[40];

int txtwinxSize;
int txtwinYSize;
int charX, charY;
RECT WinRect;


	switch (message) {
        case WM_INITDIALOG:

 	SetDlgItemInt(hDlg,IDC_EDIT1,100,FALSE);
	Slot=New_Dialog_Slot(hDlg);
			if ( Slot < 0 ) {
				          MessageBox(hWnd,"Alle Videotext-Dialoge belegt","MULTI-Decoder",MB_ICONSTOP|MB_OK);
						  EndDialog(hDlg,0);
						  return(TRUE);
			
			};

			if ( Initial_FontNr == 1 ) SetDlgItemText(hDlg,IDC_FONT,"Font 2");
  			  else if ( Initial_FontNr == 2 ) SetDlgItemText(hDlg,IDC_FONT,"Font 3");
     			  else if ( Initial_FontNr == 3 ) SetDlgItemText(hDlg,IDC_FONT,"Font 1");

			GetWindowRect(hDlg,&WinRect);
			if ( Initial_FontNr == 1 ) {
                MoveWindow(hDlg,WinRect.left,WinRect.top,384,388,TRUE);
				MoveWindow(GetDlgItem(hDlg,ID_TXTFRAME),5,40,366,287,TRUE);
				MoveWindow(GetDlgItem(hDlg,ID_CACHTEXT),336,7,36,13,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_TEXT21),336,26,36,13,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_EXPORT),5,333,80,24,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_FONT),152,333,80,24,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDCANCEL),287,333,80,24,TRUE);
			}

			if ( Initial_FontNr == 2 ) {
                MoveWindow(hDlg,WinRect.left,WinRect.top,424,485,TRUE);
				MoveWindow(GetDlgItem(hDlg,ID_TXTFRAME),5,40,406,387,TRUE);
				MoveWindow(GetDlgItem(hDlg,ID_CACHTEXT),377,7,36,13,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_TEXT21),377,26,36,13,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_EXPORT),6,431,80,24,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_FONT),172,431,80,24,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDCANCEL),331,431,80,24,TRUE);
			}

			if ( Initial_FontNr == 3 ) {
                MoveWindow(hDlg,WinRect.left,WinRect.top,508,558,TRUE);
				MoveWindow(GetDlgItem(hDlg,ID_TXTFRAME),5,40,487,462,TRUE);
				MoveWindow(GetDlgItem(hDlg,ID_CACHTEXT),458,7,36,13,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_TEXT21),458,26,36,13,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_EXPORT),6,506,80,24,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_FONT),214,506,80,24,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDCANCEL),412,506,80,24,TRUE);
			}
			
			
			VTDialog[Slot].FontNr=Initial_FontNr;

			VTDialog[Slot].SubPage=0;
            VTDialog[Slot].PageChange = TRUE;
			SetTimer(hDlg,1,333,NULL);
            return(TRUE);
 
        case WM_TIMER:
			   if ( wParam != 1 ) return(TRUE);
			
			   sprintf(Text,"%d",VT_Cache);
			   SetDlgItemText(hDlg,IDC_TEXT21,Text);
			   
			   Slot=Get_Dialog_Slot(hDlg);
				if ( Slot < 0 ) {
				          MessageBox(hDlg,t("Fehler :Unbekannter Videotext-Dialoge"),t("MULTI-Decoder"),MB_ICONSTOP|MB_OK);
						  EndDialog(hDlg,0);
						  return(TRUE);
			};
			   sprintf(Text,"%02d/%02d",VTDialog[Slot].SubPage+1,VTFrame[VTDialog[Slot].FramePos].SubCount);
			   SetDlgItemText(hDlg,IDC_TEXT20,Text);

				if (( VTDialog[Slot].Page >= 100 ) && ( VTDialog[Slot].Page <= 899 )) {

				    VTDialog[Slot].FramePos=VT_GetPage(VTDialog[Slot].Page,&VTDialog[Slot].PageChange,&VTDialog[Slot].SubPage);
					
                    if (( VTDialog[Slot].FramePos >= 0 ) &&
						( VTFrame[VTDialog[Slot].FramePos].SubPage[VTDialog[Slot].SubPage].bUpdated != 0 )) {
	
					VT_DoUpdate_Page(hDlg,VTDialog[Slot].FramePos,VTDialog[Slot].SubPage,Slot,TRUE,0,VTDialog[Slot].FontNr);
					VTFrame[VTDialog[Slot].FramePos].SubPage[VTDialog[Slot].SubPage].bUpdated=0;
					sprintf(Text,t("Seite %d gefunden"),VTDialog[Slot].Page);
					SetDlgItemText(hDlg,IDC_TEXT,Text);
					SetDlgItemText(hDlg,IDC_EDIT1,"");

					sprintf(Text,"MultiDecode VT %d",VTDialog[Slot].Page);
					SetWindowText(hDlg,Text);
					} else {
						if ( VTDialog[Slot].FramePos < 0 ) {
							VT_DoUpdate_Page(hDlg,VTDialog[Slot].Page-100,0,Slot,TRUE,0,VTDialog[Slot].FontNr);
						} else VT_DoUpdate_Page(hDlg,VTDialog[Slot].FramePos,VTDialog[Slot].SubPage,Slot,TRUE,0,VTDialog[Slot].FontNr);
					};
					
				}
				break; 

       
		case IDM_VT_PAGE_MINUS:
           				Slot=Get_Dialog_Slot(hDlg);
                        if ( VTDialog[Slot].Page > 100 ) {
							 SetDlgItemInt(hDlg,IDC_EDIT1,VTDialog[Slot].Page-1 ,FALSE);
						     VTDialog[Slot].PageChange = TRUE;
						}
                        return(TRUE);

		case IDM_VT_PAGE_PLUS:
           				Slot=Get_Dialog_Slot(hDlg);
                        if ( VTDialog[Slot].Page > 100 ) {
							 SetDlgItemInt(hDlg,IDC_EDIT1,VTDialog[Slot].Page+1 ,FALSE);
						     VTDialog[Slot].PageChange = TRUE;
						}
                        return(TRUE);
        

		case WM_LBUTTONUP:
                  y=HIWORD( lParam );
			      x=LOWORD( lParam );
                  if (( x >= 83 ) && ( y >= 13 ) &&
                      ( x <= 107 ) && ( y <= 28 )) {
           				Slot=Get_Dialog_Slot(hDlg);
                        if ( VTDialog[Slot].Page > 100 ) {
							 SetDlgItemInt(hDlg,IDC_EDIT1,VTDialog[Slot].Page-1 ,FALSE);
						     VTDialog[Slot].PageChange = TRUE;
						}
                        return(TRUE);
				  }
                  if (( x >= 113 ) && ( y >= 13 ) &&
                      ( x <= 138 ) && ( y <= 28 )) {
           				Slot=Get_Dialog_Slot(hDlg);
                        if ( VTDialog[Slot].Page < 899 ) {
							SetDlgItemInt(hDlg,IDC_EDIT1,VTDialog[Slot].Page+1 ,FALSE);
						     VTDialog[Slot].PageChange = TRUE;
						}
							return(TRUE);

				  }
                  if (( x >= 143 ) && ( y >= 13 ) &&
                      ( x <= 168 ) && ( y <= 28 )) {
           				Slot=Get_Dialog_Slot(hDlg);
                        SetDlgItemInt(hDlg,IDC_EDIT1,100 ,FALSE);
						VTDialog[Slot].PageChange = TRUE;
                        return(TRUE);
				  }
                  if (( x >= 174 ) && ( y >= 4 ) &&
                      ( x <= 198 ) && ( y <= 19 )) {
           				Slot=Get_Dialog_Slot(hDlg);
						i=VTDialog[Slot].SubPage;
						if ( VTFrame[VTDialog[Slot].FramePos].SubCount == 0 ) return(TRUE);
                        i--;
						if ( i < 0 ) i=VTFrame[VTDialog[Slot].FramePos].SubCount-1;
						  while (( i>= 0 ) && ( VTFrame[VTDialog[Slot].FramePos].SubPage[i].Fill == FALSE )) i--;
                      if ( i >= 0 ) VTDialog[Slot].SubPage=i;
					  SetDlgItemInt(hDlg,IDC_EDIT1,VTDialog[Slot].Page ,FALSE);
					  return(TRUE);
				  }

                  if (( x >= 174 ) && ( y >= 20 ) &&
                      ( x <= 198 ) && ( y <= 35 )) {
           				Slot=Get_Dialog_Slot(hDlg);
						i=VTDialog[Slot].SubPage;
						if ( VTFrame[VTDialog[Slot].FramePos].SubCount == 0 ) return(TRUE);
                        i++;
						if ( i >= VTFrame[VTDialog[Slot].FramePos].SubCount ) i=0;
					    while (( i < VTFrame[VTDialog[Slot].FramePos].SubCount) && ( VTFrame[VTDialog[Slot].FramePos].SubPage[i].Fill == FALSE )) i++;
                      if ( i < VTFrame[VTDialog[Slot].FramePos].SubCount ) VTDialog[Slot].SubPage=i;
  					  SetDlgItemInt(hDlg,IDC_EDIT1,VTDialog[Slot].Page ,FALSE);

					  return(TRUE);
				  }
                  
           		  Slot=Get_Dialog_Slot(hDlg);

				  if ( VTDialog[Slot].FontNr == 1 ) {
					  charX=9; charY=11;txtwinxSize=358;txtwinYSize=264;
				  };
				  if ( VTDialog[Slot].FontNr == 2 ) {
					  charX=10; charY=15;txtwinxSize=402;txtwinYSize=364;
				  };
				  if ( VTDialog[Slot].FontNr == 3 ) {
					  charX=12; charY=18;txtwinxSize=483;txtwinYSize=436;
				  };

					  if (( y >= 50 ) && ( y <= 50+txtwinYSize ) && ( x>=10 ) && ( x<=10+txtwinxSize )) {
						  x=((x-10)/charX)-1;
						  y=(y-50)/charY;
						  if (( x >= 0 ) && ( x <= 39 ) && ( y >= 0 ) && ( y <= 24 )) {
	                        i=0;
							Buffer[i]=0x00;
							while (( x >= 0 ) && ( VTDialog[Slot].AsciiPuffer[y][x] != 0x00 )) x--;
							if  ( x < 0 ) return(TRUE);
							if ( VTDialog[Slot].AsciiPuffer[y][x] == 0x00 ) x++;
                            if ( x > 37 ) return(TRUE);
							while (( i < 9 ) && ( x+i < 40 ) && (VTDialog[Slot].AsciiPuffer[y][x+i] != 0x00 )) {
								Buffer[i]=VTDialog[Slot].AsciiPuffer[y][x+i];
								i++;
								Buffer[i]=0x00;
							}
                            if ( i != 3 ) return(TRUE);
							i=atoi(Buffer);
           					if (( i < 100 ) || ( i > 899 )) return(TRUE);
				               
							   if ( i != VTDialog[Slot].Page )  VTDialog[Slot].PageChange=TRUE; 
							   VTDialog[Slot].Page = i;
					           sprintf(Text,t("Suche Seite %d"),VTDialog[Slot].Page);
                               SetDlgItemText(hDlg,IDC_TEXT,Text);
					           VTDialog[Slot].FramePos=VT_GetPage(VTDialog[Slot].Page,&VTDialog[Slot].PageChange,&VTDialog[Slot].SubPage);
						  }
                   return(TRUE);
				  };



                 break;


		case WM_COMMAND:
            
			if ( (HWND)lParam==GetDlgItem(hDlg,IDC_EDIT1) ){
				Slot=Get_Dialog_Slot(hDlg);
				if ( Slot < 0 ) {
						  return(TRUE);
			};
				i=GetDlgItemInt(hDlg,IDC_EDIT1,NULL,FALSE);	
				GetDlgItemText(hDlg,IDC_EDIT1,&Text[0],4);
                if ( strlen(Text) > 2) SetDlgItemText(hDlg,IDC_EDIT1,"");
					if (( i >= 100 ) && ( i <= 899 )) {
				    if ( i != VTDialog[Slot].Page )  VTDialog[Slot].PageChange=TRUE; 
					VTDialog[Slot].Page = i;
					sprintf(Text,t("Suche Seite %d"),VTDialog[Slot].Page);
                    SetDlgItemText(hDlg,IDC_TEXT,Text);
					VTDialog[Slot].FramePos=VT_GetPage(VTDialog[Slot].Page,&VTDialog[Slot].PageChange,&VTDialog[Slot].SubPage);
                    if ( VTDialog[Slot].FramePos >= 0 ) {
					VTFrame[VTDialog[Slot].FramePos].SubPage[VTDialog[Slot].SubPage].bUpdated =1;
					VT_DoUpdate_Page(hDlg,VTDialog[Slot].FramePos,VTDialog[Slot].SubPage,Slot,TRUE,0,VTDialog[Slot].FontNr);
					VTFrame[VTDialog[Slot].FramePos].SubPage[VTDialog[Slot].SubPage].bUpdated =0;
					sprintf(Text,"%02d/%02d",VTDialog[Slot].SubPage+1,VTFrame[VTDialog[Slot].FramePos].SubCount);
					SetDlgItemText(hDlg,IDC_TEXT20,Text);
					sprintf(Text,t("Seite %d gefunden"),VTDialog[Slot].Page);
                    SetDlgItemText(hDlg,IDC_TEXT,Text);
					SetDlgItemText(hDlg,IDC_EDIT1,"");
					sprintf(Text,"MultiDecode VT %d",VTDialog[Slot].Page);
					SetWindowText(hDlg,Text);
					}

				}
                        return(TRUE);
			}
            
               if ( LOWORD(wParam) == IDC_EXPORT) {
           				Slot=Get_Dialog_Slot(hDlg);
                        Export_VT_Page(hDlg, VTDialog[Slot].Page, VTDialog[Slot].SubPage);
                        return(TRUE);
			   }

			 if ( LOWORD(wParam) == IDC_FONT ) {
           				Slot=Get_Dialog_Slot(hDlg);
				if ( Slot < 0 ) {
				          MessageBox(hDlg,t("Fehler :Unbekannter Videotext-Dialoge"),t("MULTI-Decoder"),MB_ICONSTOP|MB_OK);
				} else {

                        VTDialog[Slot].FontNr++;
						if ( VTDialog[Slot].FontNr > 3 ) VTDialog[Slot].FontNr = 1; 
			if ( VTDialog[Slot].FontNr == 1 ) SetDlgItemText(hDlg,IDC_FONT,"Font 2");
  			  else if ( VTDialog[Slot].FontNr == 2 ) SetDlgItemText(hDlg,IDC_FONT,"Font 3");
     			  else if ( VTDialog[Slot].FontNr == 3 ) SetDlgItemText(hDlg,IDC_FONT,"Font 1");

    		            GetWindowRect(hDlg,&WinRect);
			if ( VTDialog[Slot].FontNr == 1 ) {
                MoveWindow(hDlg,WinRect.left,WinRect.top,384,388,TRUE);
				MoveWindow(GetDlgItem(hDlg,ID_TXTFRAME),5,40,366,287,TRUE);
				MoveWindow(GetDlgItem(hDlg,ID_CACHTEXT),336,7,36,13,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_TEXT21),336,26,36,13,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_EXPORT),5,333,80,24,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_FONT),152,333,80,24,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDCANCEL),287,333,80,24,TRUE);
			}

			if ( VTDialog[Slot].FontNr == 2 ) {
                MoveWindow(hDlg,WinRect.left,WinRect.top,424,485,TRUE);
				MoveWindow(GetDlgItem(hDlg,ID_TXTFRAME),5,40,406,387,TRUE);
				MoveWindow(GetDlgItem(hDlg,ID_CACHTEXT),377,7,36,13,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_TEXT21),377,26,36,13,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_EXPORT),6,431,80,24,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_FONT),172,431,80,24,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDCANCEL),331,431,80,24,TRUE);
			}

			if ( VTDialog[Slot].FontNr == 3 ) {
                MoveWindow(hDlg,WinRect.left,WinRect.top,508,558,TRUE);
				MoveWindow(GetDlgItem(hDlg,ID_TXTFRAME),5,40,487,462,TRUE);
				MoveWindow(GetDlgItem(hDlg,ID_CACHTEXT),458,7,36,13,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_TEXT21),458,26,36,13,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_EXPORT),6,506,80,24,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDC_FONT),214,506,80,24,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDCANCEL),412,506,80,24,TRUE);
			}
	};
                        return(TRUE);

			 }


               if ( LOWORD(wParam) == IDCANCEL) {
                KillTimer(hDlg,1);
				Slot=Del_Dialog_Slot(hDlg);
				if ( Slot < 0 ) {
				          MessageBox(hDlg,t("Fehler :Unbekannter Videotext-Dialoge"),t("MULTI-Decoder"),MB_ICONSTOP|MB_OK);
				} else {
                VTDialog[Slot].Dialog=NULL;
				Restore_Mode();
				EndDialog(hDlg,TRUE);
				 }
                        return(TRUE);
			   };		
			 break;
    }
    return (FALSE);
        UNREFERENCED_PARAMETER(lParam);
}

int Get_Dialog_Slot(HWND hwnd)
{
	int i;
	for ( i = 0; i < MAXVTDIALOG; i++ ) {
		if ( hwnd == VTDialog[i].Dialog ) return(i);
	};

	return(-1);
};

int New_Dialog_Slot(HWND hwnd)
{
	int i;
	for ( i = 0; i < MAXVTDIALOG; i++ ) {
		if ( VTDialog[i].Dialog == NULL ) {
		    VTDialog[i].Dialog=hwnd;
			VTDialog[i].Page=0;
			VTDialog[i].SubPage=0;
            VTDialog[i].FramePos=-1;
			return(i);
		}
	};

	return(-1);
};

int Del_Dialog_Slot(HWND hwnd)
{
	int i;
	for ( i = 0; i < MAXVTDIALOG; i++ ) {
	if ( hwnd == VTDialog[i].Dialog )  {
		    VTDialog[i].Dialog=NULL;
			VTDialog[i].Page=0;
            VTDialog[i].FramePos=-1;
			return(i);
		}
	};

	return(-1);
};


void Export_VT_Page(HWND hwnd, int Page, int SubPage)
{
	char FileName[255];
	char DirName[255];
	char TextName[255];
	char LinkRoot[255];
    FILE *htmpfp;


	Page=Page-100;

    if (VTFrame[Page].SubCount==0 ) return;
    if ( SubPage > VTFrame[Page].SubCount ) return;
	if (  VTFrame[Page].SubPage[SubPage].Fill == FALSE ) return;

     strcpy(DirName,Programm[CurrentProgramm].Name);
     strcpy(TextName,Programm[CurrentProgramm].Name);

	 sprintf(FileName,"%s\\%s",VT_BASE_DIR,DirName);
	 CreateDirectory(FileName,NULL);

if ( VT_EXPORT_TYP == 1 ) {
  sprintf(FileName,"%s\\%s\\VT%03d-%02d.bmp",VT_BASE_DIR,DirName,Page+100,SubPage);
  Create_File_Bitmap(FileName,Page, SubPage, Initial_Export_Font, FALSE);
} else if ( VT_EXPORT_TYP == 2 ) {
  sprintf(FileName,"%s\\%s\\VT%03d-%02d.txt",VT_BASE_DIR,DirName,Page+100,SubPage);
  htmpfp = fopen(FileName, "w");
  if (htmpfp == NULL )   return;
  if ( Make_Txt_Page(htmpfp,Page,SubPage) == FALSE ) {
	  		   fclose(htmpfp);
	 };
		   fclose(htmpfp);
} else if ( VT_EXPORT_TYP == 3 ) {
    if ( SubPage == 0  ) sprintf(FileName,"%s\\%s\\VT%03d.htm",VT_BASE_DIR,DirName,Page+100); 
	else sprintf(FileName,"%s\\%s\\VT%03d-%02d.htm",VT_BASE_DIR,DirName,Page+100,SubPage);
	sprintf(LinkRoot,"%s\\%s\\VT",VT_EXPORT_WWW_ROOT,DirName);
	 htmpfp = fopen(FileName, "w");
     if (htmpfp == NULL )   return;

	 if ( VT_EXPORT_BACKGROUND == FALSE ) {
     if ( Make_html_Page(htmpfp,LinkRoot,TextName,Page,SubPage,Initial_Export_Font) == FALSE ) {
		   fclose(htmpfp);
	 };
	 } else {
     if ( Make_html_Background_Page(htmpfp,LinkRoot,TextName,Page,SubPage,Initial_Export_Font) == FALSE ) {
		   fclose(htmpfp);
	 };
	 }
		   fclose(htmpfp);
};



};


BOOL Create_File_Bitmap(LPSTR FileName,int Page, int SubPage, int FontNr, BOOL Background)
{
int StepX;
int StepY;
int CurrX;
int CurrY;

BOOL bGraph,bHoldGraph,bSepGraph,bBox,bFlash,bDouble,bConceal,bHasDouble;
BYTE nLastGraph;
unsigned char CurrentFg, CurrentBkg, RealBkg,Black,ForceBlack,ForceTransparent;
unsigned char OldFg, OldBkg;
HBITMAP hOldBitmapFormat;
BOOL OldDouble;
BYTE c,ch;
int n,row;
unsigned char tmp[41];

char ZeichenPuffer[256];
int SaveLen;

int PageLang;

int SaveX;
int endrow;
BOOL bHideTopLine,bForceShowTopLine;

LOGFONT VTText			= {11,9 ,0,0,0,0,0,0,1,0,0,0,0,"XXXXXXXXXX"};
LOGFONT DoubleText		= {22,9,0,0,0,0,0,0,1,0,0,0,0,"XXXXXXXXXX"};

HFONT hFontText,hFontDoubleText,hOldFont;

char Zeile[2];
BOOL MakeAusgabe;

HDC hDCMem;
BITMAPINFO *pbmiRGB = (BITMAPINFO *) NULL;      // Ptr to file's BITMAPINFO 
PBYTE       pjBitsRGB; 
HBITMAP     hbmRGB = (HBITMAP) NULL;             // 4BPP RGB bitmap 
HANDLE hFile; 
DWORD dwBytes, dwBytesToWrite; 
int i;


sprintf(VTText.lfFaceName,"MD-F-%d",FontNr);
sprintf(DoubleText.lfFaceName,"MD-F-%dD",FontNr);
 


if ( FontNr == 1 ) {
			StepX=9;
			CurrY=-11; StepY=11;
};
if ( FontNr == 2 ) {
			StepX=10;
			CurrY=-15; StepY=15;
};
if ( FontNr == 3 ) {
			StepX=12;
			CurrY=-18; StepY=18;
};




hFontText     = CreateFontIndirect(&VTText);
hFontDoubleText     = CreateFontIndirect(&DoubleText);


hDCMem     = CreateCompatibleDC(NULL);                /* Create a Memory DC */     
pbmiRGB = (BITMAPINFO *)LocalAlloc(LMEM_FIXED|LMEM_ZEROINIT, sizeof(BITMAPINFO) ); 

pbmiRGB->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER); 
pbmiRGB->bmiHeader.biWidth         = StepX*40; 
pbmiRGB->bmiHeader.biHeight        = StepY*25; 
pbmiRGB->bmiHeader.biPlanes        = 1; 
pbmiRGB->bmiHeader.biBitCount      = 4; 
pbmiRGB->bmiHeader.biCompression   = BI_RGB; 
pbmiRGB->bmiHeader.biSizeImage     = pbmiRGB->bmiHeader.biWidth*pbmiRGB->bmiHeader.biHeight/2; 
 
hbmRGB = CreateDIBSection(hDCMem, pbmiRGB, DIB_RGB_COLORS,  
                              (PVOID *) &pjBitsRGB, NULL, 0); 


hOldBitmapFormat = SelectObject(hDCMem, hbmRGB);
if ( Prepared_Bitmap_Header[0] == 0x00 ) Prepare_Palette(hDCMem);
i=SetDIBColorTable(hDCMem,0,16,(RGBQUAD *)&Ist_Palette[0]);

hOldFont   = SelectObject(hDCMem, hFontText);
CurrentFg=0;
OldFg=0xff;
OldBkg=0xff;
OldDouble=FALSE;
Black=0; // 0
ForceTransparent=8;
ForceBlack=0;
bHideTopLine=FALSE;
bForceShowTopLine=TRUE;
bHasDouble=FALSE;
endrow=25;
if ( VTFrame[Page].SubCount == 0 ) {
	endrow=1;
	PageLang=0;
} else PageLang = VTFrame[Page].SubPage[SubPage].Lang;

ZeichenPuffer[0]=0x00;
SaveLen=0;
for(row=0; row<endrow ; row++)
	{

	    if ( SaveLen > 0 ) TextOut(hDCMem,SaveX,CurrY,ZeichenPuffer,SaveLen);
	    CurrY+=StepY;
        CurrX=0;

		ZeichenPuffer[0]=0x00;
  		SaveLen=0;
  		SaveX=CurrX;

 		if(bHasDouble) {
 			bHasDouble=FALSE;
 			continue;
 		}
  
		bGraph=bHoldGraph=bSepGraph=bBox=bFlash=bDouble=bConceal=bHasDouble=FALSE;
 		nLastGraph=' ';


 		for(n=0; n<40; n++) {
  			tmp[n]=VTFrame[Page].SubPage[SubPage].Frame[row][n];
  			if(tmp[n]==0x0d) bHasDouble=TRUE;
  		}
 		tmp[n]=0;

 		if(row==0) {
 			memcpy(tmp,"MD-VT \x7",8);
  		}

  		RealBkg=ForceBlack;
  		CurrentBkg=ForceBlack;
  		CurrentFg=7;
 		for(n=0; n<40; n++) {
  			c=tmp[n];
  			ch=c;
 			if(c<' ') {
 				if(c<=0x07) {
  					CurrentFg=c;
  					bGraph=FALSE;
 				} else if (c==0x08) {
 					bFlash=TRUE;
 				} else if (c==0x09) {
 					bFlash=FALSE;
 				} else if (c==0x0c) {
 					bDouble=FALSE;
 				} else if (c==0x0d) {
 					bDouble=TRUE;
 				} else if (c>=0x10 && c<=0x17) {
  					bGraph=TRUE;
  					CurrentFg=c-0x10;
 				} else if (c==0x18) {
 					bConceal=TRUE;
 				} else if (c==0x19) {
 					bSepGraph=FALSE;
 				} else if (c==0x1a) {
 					bSepGraph=TRUE;
 				} else if (c==0x1c) {
  					CurrentBkg=bBox?ForceBlack:Black;
  					RealBkg=ForceBlack;
 				} else if (c==0x1d) {
 					CurrentBkg=(bBox || Black!=ForceTransparent) ?
 						CurrentFg : ForceTransparent;
 					RealBkg=CurrentFg;
 				} else if (c==0x1e) {
 					bHoldGraph=TRUE;
 				} else if (c==0x1f) {
 					bHoldGraph=FALSE;
  				}
 				ch=bHoldGraph?nLastGraph:' ';
  			}

 			if (CurrentFg==7 && VTFrame[Page].SubCount==0 && !row && n>7)
 				CurrentFg=2;

 			if (bGraph && (ch&0x20)) {
  				nLastGraph=ch;
 //				ch=(ch&0x1f)|((ch&0x40)>>1);
  				ch+=128;
				if(bSepGraph) ch+=32;
			}


  			if (SaveLen > 0 ) {
 				MakeAusgabe = (CurrentFg != OldFg ||
 							   CurrentBkg != OldBkg ||
 							   OldDouble != bHasDouble);
                 if (MakeAusgabe) {
 					TextOut(hDCMem,SaveX,CurrY,ZeichenPuffer,SaveLen);
 					CurrX+=(SaveLen*StepX);
 					SaveX=CurrX;
 					SaveLen=0;
 					ZeichenPuffer[0]=0x00;
  				}
 			}
              if ( CurrentFg != OldFg ) {
  				OldFg=CurrentFg ;
  				SetTextColor(hDCMem,VTColourTable[CurrentFg]);
 			}
  			if ( CurrentBkg != OldBkg) {
  				OldBkg=CurrentBkg ;
                  SetBkColor(hDCMem,VTColourTable[CurrentBkg]);
 			}
              if ( bHasDouble  != OldDouble ) {
                   OldDouble=bHasDouble;
                   SetDoubleMode(bHasDouble,&hDCMem,&hFontText,&hFontDoubleText);
 			}

			  if (ch >= 128) {
 				Zeile[0]=ch;
			  } else {
 				if (ch < ' ') ch = ' ';
				ch = VTtoFont[PageLang][ch-' '];
			  }
			Zeile[1]=0x00;
 			strcat(ZeichenPuffer,Zeile);
 			SaveLen++;

 		}
 		if (row+1 == endrow && SaveLen > 0)
 			TextOut(hDCMem,SaveX,CurrY,ZeichenPuffer,SaveLen);
  	}
 	SelectObject(hDCMem, hOldFont );
 	DeleteObject(hFontText);
 	DeleteObject(hFontDoubleText);


 	if ( Prepared_Bitmap_Header[0] == 0x00 ) Prepare_Header(pbmiRGB);


 	if( (hFile=CreateFile( FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE )
 	{
 		SelectObject(hDCMem, hOldBitmapFormat);
 		DeleteObject(hbmRGB);
 		LocalFree(pbmiRGB);
 		DeleteDC(hDCMem);
 		return(FALSE);
 	}

 	if( ( ! WriteFile( hFile, Prepared_Bitmap_Header, 118, &dwBytes, NULL )) || ( dwBytes != 118 ) )
 	{
 		CloseHandle( hFile );
 		SelectObject(hDCMem, hOldBitmapFormat);
 		DeleteObject(hbmRGB);
 		LocalFree(pbmiRGB);
 		DeleteDC(hDCMem);
 		return(FALSE);
 	}


 	dwBytesToWrite=pbmiRGB->bmiHeader.biSizeImage;
 	if( ( ! WriteFile( hFile, pjBitsRGB, dwBytesToWrite, &dwBytes, NULL ) ) 
| ( dwBytes != dwBytesToWrite ) )
 	{
 		CloseHandle( hFile );
 		SelectObject(hDCMem, hOldBitmapFormat);
 		DeleteObject(hbmRGB);
 		LocalFree(pbmiRGB);
 		DeleteDC(hDCMem);
 		return(FALSE);
 	}


 	CloseHandle( hFile );
 	SelectObject(hDCMem, hOldBitmapFormat);
 	DeleteObject(hbmRGB);
 	LocalFree(pbmiRGB);
 	DeleteDC(hDCMem);
 	return(TRUE);

  };

void Prepare_Header(BITMAPINFO *Header)
{
	int Pos;
    BITMAPFILEHEADER bfh; 
	bfh.bfType = 0x4d42; 
	bfh.bfReserved1 = 0; 
	bfh.bfReserved2 = 0; 
	bfh.bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + 64; 
	bfh.bfSize = bfh.bfOffBits + Header->bmiHeader.biSizeImage; 
    memcpy(&Prepared_Bitmap_Header[0],&bfh,sizeof(BITMAPFILEHEADER));
	Pos=sizeof(BITMAPFILEHEADER);
    memcpy(&Prepared_Bitmap_Header[Pos],Header,sizeof(BITMAPINFOHEADER));
} 


void Prepare_Palette(HDC hDC )
{
        COLORREF *Value;
        int i;
		unsigned char R,G,B;
		Value=(COLORREF *)&Prepared_Bitmap_Header[54];
		for ( i=0; i < 16 ; i++ ) {
         R=GetRValue(VTColourTable[i]); 
         G=GetGValue(VTColourTable[i]); 
         B=GetBValue(VTColourTable[i]);  
         Ist_Palette[i]=RGB(B,G,R);    
		 Value[i]=Ist_Palette[i];
		};
 
		return;
};



void SetDoubleMode(BOOL bHasDouble,HDC *hDC,HFONT *hFontText,HFONT *hFontDoubleText)
{
	if ( bHasDouble == FALSE ) SelectObject(*hDC, *hFontText);
	else if (  bHasDouble == TRUE ) SelectObject(*hDC, *hFontDoubleText);
};

		
BOOL Make_html_Page(FILE *fp , LPSTR LinkRoot,LPSTR Text, int Page, int SubPage, int FontNr)
{
	BOOL bGraph,bHoldGraph,bSepGraph,bBox,bFlash,bDouble,bConceal,bHasDouble;
	BYTE nLastGraph;
	unsigned char CurrentFg, CurrentBkg, RealBkg,Black,ForceBlack,ForceTransparent;
	BYTE c,ch;
	int n,row;
	char tmp[41];
    char NBSPPuffer[512];
    char LinkPuffer[512];
	unsigned char ZeichenPuffer[2];
	unsigned char ZeilenPuffer[1024];
	int LinkCount ;
	int LinkPageNr;
	BOOL Etwas_Drinnen=FALSE;
	BOOL InLink=FALSE;
	BOOL bHideTopLine,bForceShowTopLine;
	int TableSizeX;

	ZeichenPuffer[0]=0x00;
	ZeichenPuffer[1]=0x00;
    if ( FontNr == 1 ) {
			TableSizeX=9*40;
	} else if ( FontNr == 2 ) {
  			TableSizeX=10*40;
  	} else if ( FontNr == 3 ) {
  			TableSizeX=12*40;
 	}

if (fprintf(fp,"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n") < 0 ) return(FALSE);
if (fprintf(fp,"<HTML><HEAD><TITLE>MultiDec VT %s</TITLE>\n",Text) < 0 ) return(FALSE);
if (fprintf(fp,"<META content=\"text/html; charset=windows-1252\" http-equiv=Content-Type>\n") < 0 ) return(FALSE);
if (fprintf(fp,"<SCRIPT language=JScript src=""></SCRIPT>\n") < 0 ) return(FALSE);
if (fprintf(fp,"<META content=\"MultiDec VT-Generator\" name=GENERATOR></HEAD>\n") < 0 ) return(FALSE);
if (fprintf(fp,"<BODY bgColor=#000000 scroll=no text=#ffffff>\n") < 0 ) return(FALSE);
if (fprintf(fp,"<table border=\"2\" cellpadding=\"0\" cellspacing=\"1\" width=\"%d\" bgcolor=\"#000000\">\n<tr>\n",TableSizeX) < 0 ) return(FALSE);
if (fprintf(fp,"<td><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"%d\" bgcolor=\"#000000\">\n",TableSizeX) < 0 )return(FALSE);

    CurrentFg=0;
    Old_Htm_FG_Color=0xff;
 	Black=0; // 0
 	ForceTransparent=8;
 	ForceBlack=0;
 	bHideTopLine=FALSE;
 	bForceShowTopLine=TRUE;
 	bHasDouble=FALSE;

 	for(row=0; row<25 ; row++) {
  		LinkCount =0;
  		InLink=FALSE;
  		ZeilenPuffer[0]=0x00;
  		if(bHasDouble) { bHasDouble=FALSE; continue; }
  
		bGraph=bHoldGraph=bSepGraph=bBox=bFlash=bDouble=bConceal=bHasDouble=FALSE;
 		nLastGraph=' ';
          NBSPPuffer[0]=0x00;
          Etwas_Drinnen=FALSE;

 		for(n=0; n<40; n++)	{
  			tmp[n]=VTFrame[Page].SubPage[SubPage].Frame[row][n];
  			if(tmp[n]==0x0d) bHasDouble=TRUE;
  		}


 		if (row == 0) memcpy(tmp, "\x03MD-VT  ", 8);
  		RealBkg=ForceBlack;
  		CurrentBkg=ForceBlack;
  		CurrentFg=7;

		Init_HTM_Line(bDouble,FontNr,CurrentFg,ZeilenPuffer);
		for(n=0; n<40; n++)
		{
  			c=tmp[n];
  			ch=c;
 			if(c<' ') {
 				if(c<=0x07) {
  					CurrentFg=c;
  					bGraph=FALSE;
 				} else if (c==0x08) {
 					bFlash=TRUE;
 				} else if (c==0x09) {
 					bFlash=FALSE;
 				} else if (c==0x0c) {
 					bDouble=FALSE;
 				} else if (c==0x0d) {
 					bDouble=TRUE;
 				} else if (c>=0x10 && c<=0x17) {
  					bGraph=TRUE;
  					CurrentFg=c-0x10;
 				} else if (c==0x18) {
 					bConceal=TRUE;
 				} else if (c==0x19) {
 					bSepGraph=FALSE;
 				} else if (c==0x1a) {
 					bSepGraph=TRUE;
 				} else if (c==0x1c) {
  					CurrentBkg=bBox?ForceBlack:Black;
  					RealBkg=ForceBlack;
 				} else if (c==0x1d) {
 					CurrentBkg=(bBox || Black!=ForceTransparent) ?
 						CurrentFg : ForceTransparent;
 					RealBkg=CurrentFg;
 				} else if (c==0x1e) {
 					bHoldGraph=TRUE;
 				} else if (c==0x1f) {
 					bHoldGraph=FALSE;
  				}
 				ch=bHoldGraph?nLastGraph:' ';
  			}
 			if(CurrentFg==7 && VTFrame[Page].SubCount==0 && !row && n>7)
  				CurrentFg=2;

 			if ( CurrentFg == CurrentBkg ) ch=' ';
  			if(bGraph && (ch&0x20))
  			{
  				nLastGraph=ch;
//				ch=(ch&0x1f)|((ch&0x40)>>1);
				ch+=128;
				if(bSepGraph) ch+=32;
			}

			
			
			if ( ch > 32 ) {
	    
				    if (NBSPPuffer[0] != 0x00 ) {
					strcat(ZeilenPuffer,NBSPPuffer);
				    NBSPPuffer[0]=0x00;
					};

                    Etwas_Drinnen=TRUE;
					SetModeDouble(bDouble,FontNr,ZeilenPuffer);
					SetModeBlink(bFlash,ZeilenPuffer);
				    SetFGColor(CurrentFg,ZeilenPuffer);
                    if ( ch == 0xad ) ch = 128;
				
					ZeichenPuffer[0]=ch;

                    if ((InLink == FALSE ) && ( Is_Link(Page,SubPage,row,n,&LinkPageNr) == TRUE )) {
						LinkCount =0;
						InLink=TRUE;
                        sprintf(LinkPuffer,"<a href=\"%s%d.htm\">",LinkRoot,LinkPageNr);
						strcat(ZeilenPuffer,LinkPuffer);

					};
					strcat(ZeilenPuffer,ZeichenPuffer);
					LinkCount++;
                    if (( LinkCount == 3 ) && ( InLink == TRUE )) {
						LinkCount =0;
						InLink=FALSE;
                          sprintf(LinkPuffer,"</a>");
  						strcat(ZeilenPuffer,LinkPuffer);
  					}
 			} else strcat(NBSPPuffer,"&nbsp;");

  	}

  		if ( Etwas_Drinnen == FALSE ) {
 			strcat(ZeilenPuffer,"&nbsp;");
  		};
  		strcat(ZeilenPuffer,"</td></tr>");
  		if (fprintf(fp,"%s\n",ZeilenPuffer) < 0 ) return(FALSE);

 	}
 	if (fprintf(fp,"</table></td>") < 0 ) return(FALSE);
 	if ( VT_EXPORT_PAGE_CONTROL == TRUE ) Make_Page_Control(fp,Page,SubPage,FontNr,LinkRoot);
 	if (fprintf(fp,"</tr></table></body></html>") < 0 ) return(FALSE);
 	return(TRUE);
 }


BOOL Init_HTM_Line(BOOL bDouble, int FontNr,unsigned char Color,unsigned char *ZeilenPuffer)
{
	char Zeile[40];
	unsigned short GraphMode;
	GraphMode = (unsigned short)bDouble;
    Old_Htm_Graph=GraphMode;
 Old_Htm_FG_Color=Color;

     strcat(ZeilenPuffer,"<tr><td>");
    if (bDouble)
		sprintf(Zeile,"<font face=MD-F-%dD ",FontNr);
	else
		sprintf(Zeile,"<font face=MD-F-%d ",FontNr);
     strcat(ZeilenPuffer,Zeile);

	if ( Color == 0 ) strcat(ZeilenPuffer,"color=#000000>");
    else if ( Color == 1 ) strcat(ZeilenPuffer,"color=#FF0000>");
    else if ( Color == 2 ) strcat(ZeilenPuffer,"color=#00FF00>");
    else if ( Color == 3 ) strcat(ZeilenPuffer,"color=#FFFF00>");
    else if ( Color == 4 ) strcat(ZeilenPuffer,"color=#0000FF>");
    else if ( Color == 5 ) strcat(ZeilenPuffer,"color=#00FF00>");
    else if ( Color == 6 ) strcat(ZeilenPuffer,"color=#FF00FF>");
    else if ( Color == 7 ) strcat(ZeilenPuffer,"color=#FFFFFF>");
    else strcat(ZeilenPuffer,">");
	return(TRUE);
 }




BOOL Make_html_Background_Page(FILE *fp ,LPSTR LinkRoot, LPSTR Text, int Page, int SubPage, int FontNr)
{
	BOOL bGraph,bHoldGraph,bSepGraph,bBox,bFlash,bDouble,bConceal,bHasDouble;
	BYTE nLastGraph;
	unsigned char CurrentFg, CurrentBkg, RealBkg, Black,ForceBlack,ForceTransparent;
	BYTE c,ch;
	int n,row;
	char tmp[41];
	BOOL bHideTopLine,bForceShowTopLine;
    char LinkPuffer[256];
	int LinkCount ;
	int LinkPageNr;
	BOOL InLink=FALSE;
	int TableSizeX;

    if ( FontNr == 1 ) {
			TableSizeX=9*40;
	} else if ( FontNr == 2 ) {
   			TableSizeX=10*40;
  	} else if ( FontNr == 3 ) {
  			TableSizeX=12*40;
 	}

 	if (fprintf(fp,"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n") < 0 ) return(FALSE);
 	if (fprintf(fp,"<HTML><HEAD><TITLE>MultiDec VT %s</TITLE>\n",Text) < 0 ) return(FALSE);
 	if (fprintf(fp,"<META content=\"text/html; charset=windows-1252\" http-equiv=Content-Type>\n") < 0 ) return(FALSE);
 	if (fprintf(fp,"<SCRIPT language=JScript src=""></SCRIPT>\n") < 0 ) return(FALSE);
 	if (fprintf(fp,"<META content=\"MultiDec VT-Generator\" name=GENERATOR></HEAD>\n") < 0 ) return(FALSE);
 	if (fprintf(fp,"<BODY bgColor=#000000 scroll=no text=#ffffff>\n") < 0 ) return(FALSE);

 	if (fprintf(fp,"<table border=\"2\" cellpadding=\"0\" cellspacing=\"1\" width=\"%d\" bgcolor=\"#000000\">\n<tr>\n",TableSizeX) < 0 ) return(FALSE);
 	if (fprintf(fp,"<td><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"%d\" bgcolor=\"#000000\">\n",TableSizeX) < 0 ) return(FALSE);

     CurrentFg=0;
     Old_Htm_FG_Color=0xff;
 	Black=0; // 0
 	ForceTransparent=8;
 	ForceBlack=0;
 	bHideTopLine=FALSE;
 	bForceShowTopLine=TRUE;
 	bHasDouble=FALSE;

 	for (row=0; row<25 ; row++)	{
  		LinkCount =0;
  		InLink=FALSE;
 		if (bHasDouble) {
 			bHasDouble=FALSE;
 			continue;
 		}
  
		bGraph=bHoldGraph=bSepGraph=bBox=bFlash=bDouble=bConceal=bHasDouble=FALSE;
 		nLastGraph=' ';

 		for(n=0; n<40; n++) {
  			tmp[n]=VTFrame[Page].SubPage[SubPage].Frame[row][n];
  			if(tmp[n]==0x0d) bHasDouble=TRUE;
  		}

 		if (row == 0) memcpy(tmp, "\x03MD-VT  ", 8);
  		RealBkg=ForceBlack;
  		CurrentBkg=ForceBlack;
  		CurrentFg=7;
  		if (fprintf(fp,"<tr>\n") < 0 ) return(FALSE);
 		for (n=0; n<40; n++) {
  			c=tmp[n];
  			ch=c;
 			if(c<' ') {
 				if(c<=0x07) {
  					CurrentFg=c;
  					bGraph=FALSE;
 				} else if (c==0x08) {
 					bFlash=TRUE;
 				} else if (c==0x09) {
 					bFlash=FALSE;
 				} else if (c==0x0a) {
 					//bEndBox=TRUE;
 				} else if (c==0x0b) {
 					//bStartBox=TRUE;
 				} else if (c==0x0c) {
 					bDouble=FALSE;
 				} else if (c==0x0d) {
 					bDouble=TRUE;
 				} else if (c==0x0e) {
 					//bSO=TRUE;
 				} else if (c==0x0f) {
 					//bSI=TRUE;
 				} else if (c>=0x10 && c<=0x17) {
  					bGraph=TRUE;
  					CurrentFg=c-0x10;
 				} else if (c==0x18) {
 					bConceal=TRUE;
 				} else if (c==0x19) {
 					bSepGraph=FALSE;
 				} else if (c==0x1a) {
 					bSepGraph=TRUE;
 				} else if (c==0x1b) {
 					//bEsc=TRUE;
 				} else if (c==0x1c) {
  					CurrentBkg=bBox?ForceBlack:Black;
  					RealBkg=ForceBlack;
 				} else if (c==0x1d) {
 					CurrentBkg=(bBox || Black!=ForceTransparent) ?
 						CurrentFg : ForceTransparent;
 					RealBkg=CurrentFg;
 				} else if (c==0x1e) {
 					bHoldGraph=TRUE;
 				} else if (c==0x1f) {
 					bHoldGraph=FALSE;
  				}
 				ch=bHoldGraph?nLastGraph:' ';
  			}

 			if(CurrentFg==7 && VTFrame[Page].SubCount==0 && !row && n>7)
  				CurrentFg=2;

 			if(bGraph && (ch&0x20))	{
  				nLastGraph=ch;
  //				ch=(ch&0x1f)|((ch&0x40)>>1);
  				ch+=128;
  				if(bSepGraph) ch+=32;
  			}

 			if (ch == 0xad) ch = 128;

 			if ((InLink == FALSE ) && ( Is_Link(Page,SubPage,row,n,&LinkPageNr) == TRUE )) {
 				LinkCount = 0;
 				InLink=TRUE;
                 sprintf(LinkPuffer,"<a href=\"%s%d.htm\">",LinkRoot,LinkPageNr);
 			}

 			if ( SetCell(fp,bDouble,FontNr,CurrentFg,CurrentBkg,bFlash,InLink ,LinkPuffer, ch ) == FALSE ) return(FALSE);
             LinkCount++;
             if (( LinkCount == 3 ) && ( InLink == TRUE )) {
 				LinkCount =0;
 				InLink=FALSE;
 			}


 		}
  		if (fprintf(fp,"</tr>\n") < 0 ) return(FALSE);
 	}

 	if (fprintf(fp,"</table></td>") < 0 ) return(FALSE);

 	if ( VT_EXPORT_PAGE_CONTROL ) Make_Page_Control(fp,Page,SubPage,FontNr,LinkRoot);


 	if (fprintf(fp,"</tr></table></body></html>") < 0 ) return(FALSE);
 	return(TRUE);
 }



BOOL SetCell(FILE *fp , BOOL bDouble, int FontNr,unsigned char FGColor,unsigned char BGColor, BOOL bFlash,BOOL Link , LPSTR LinkTo , unsigned char ch )
{
	char Zeile[512];
	char Zeile1[127];

  	strcpy(Zeile,"<td ");

 	if ( BGColor == 0 ) strcat(Zeile,"bgcolor=#000000>");
     else if ( BGColor == 1 ) strcat(Zeile,"bgcolor=#FF0000>");
     else if ( BGColor == 2 ) strcat(Zeile,"bgcolor=#00FF00>");
     else if ( BGColor == 3 ) strcat(Zeile,"bgcolor=#FFFF00>");
     else if ( BGColor == 4 ) strcat(Zeile,"bgcolor=#0000FF>");
     else if ( BGColor == 5 ) strcat(Zeile,"bgcolor=#00FF00>");
     else if ( BGColor == 6 ) strcat(Zeile,"bgcolor=#FF00FF>");
     else if ( BGColor == 7 ) strcat(Zeile,"bgcolor=#FFFFFF>");
     else strcat(Zeile,">");

  	strcat(Zeile,"<font ");
 	if ( FGColor == 0 ) strcat(Zeile,"color=#000000");
     else if ( FGColor == 1 ) strcat(Zeile,"color=#FF0000");
     else if ( FGColor == 2 ) strcat(Zeile,"color=#00FF00");
     else if ( FGColor == 3 ) strcat(Zeile,"color=#FFFF00");
     else if ( FGColor == 4 ) strcat(Zeile,"color=#0000FF");
     else if ( FGColor == 5 ) strcat(Zeile,"color=#00FF00");
     else if ( FGColor == 6 ) strcat(Zeile,"color=#FF00FF");
     else if ( FGColor == 7 ) strcat(Zeile,"color=#FFFFFF");
     else strcat(Zeile,"color=#FFFFFF");

     if ( bDouble )
 		sprintf(Zeile1," face=MD-F-%dD>",FontNr);
 	else
 		sprintf(Zeile1," face=MD-F-%d>",FontNr);
 	strcat(Zeile,Zeile1);

 	if ( bFlash ) strcat(Zeile,"<blink>");

 	if ( Link ) strcat(Zeile,LinkTo);

 	if ( ch > ' ' ) {
 		Zeile1[0]=ch;
 		Zeile1[1]=0x00;
 	} else {
 		strcpy(Zeile1,"&nbsp;");
 	}

  	strcat(Zeile,Zeile1);
      if ( Link==TRUE ) strcat(Zeile,"</a>");
  	strcat(Zeile,"</td>");
 	if (fprintf(fp,"%s\n",Zeile) < 0 ) return(FALSE);
 	return(TRUE);
 }



BOOL Is_Link(int Page, int Subpage , int row, int n, int *LinkPage)
{
	unsigned char Buffer[6];
	unsigned char Zahl[4];
	int PageNr;

	if ( VT_EXPORT_PAGE_LINK == FALSE ) return(FALSE);
    if (Is_Val((BYTE)(VTFrame[Page].SubPage[SubPage].Frame[row][n])) == FALSE) return(FALSE);
	if ( n > 37 ) return(FALSE);
	if ( row == 0  ) return(FALSE);

	if ( n == 0 ) Buffer[0] = 'X';
	   else Buffer[0]=VTFrame[Page].SubPage[SubPage].Frame[row][n-1];	

    Buffer[1]=VTFrame[Page].SubPage[SubPage].Frame[row][n];	
    Buffer[2]=VTFrame[Page].SubPage[SubPage].Frame[row][n+1];	
    Buffer[3]=VTFrame[Page].SubPage[SubPage].Frame[row][n+2];	

	if ( n == 37 ) Buffer[4] = 'X';
	   else Buffer[4]=VTFrame[Page].SubPage[SubPage].Frame[row][n+3];	
       
    if ( (Is_Val(Buffer[0]) == FALSE) &&  
         (Is_Val(Buffer[1]) == TRUE) &&  
		 (Is_Val(Buffer[2]) == TRUE) &&  
		 (Is_Val(Buffer[3]) == TRUE) &&  
		 (Is_Val(Buffer[4]) == FALSE)) {
         Zahl[0]=Buffer[1];
		 Zahl[1]=Buffer[2];
         Zahl[2]=Buffer[3];
         Zahl[3]=0x00;
         PageNr=atoi(Zahl);
         if (( PageNr < 100 ) || ( PageNr > 899 )) return(FALSE);
		 if ( VTFrame[PageNr-100].SubCount == 0 ) return(FALSE);   
		 *LinkPage=PageNr;
		 return(TRUE);
	} 
	
	return(FALSE);
};


BOOL SetModeBlink(BOOL Blink, unsigned char *ZeilenPuffer)
{
	char SetMode[30];
	if ( Blink == Old_Htm_Blink) return(TRUE);
    Old_Htm_Graph=Blink;
		if ( Blink == TRUE ) strcpy(SetMode,"<blink>");
		  else if ( Blink == FALSE ) strcpy(SetMode,"</blink>");
	      strcat(ZeilenPuffer,SetMode);
return(TRUE);
};


BOOL SetModeDouble(BOOL bDouble, int FontNr,unsigned char *ZeilenPuffer)
{
	char SetMode[30];
	unsigned short GraphMode;
	GraphMode = (unsigned short)bDouble;
	if ( GraphMode == Old_Htm_Graph ) return(TRUE);
    Old_Htm_Graph=GraphMode;

	if ( bDouble == FALSE ) sprintf(SetMode,"<font face=MD-F-%d>",FontNr);
	 else if ( bDouble == TRUE ) sprintf(SetMode,"<font face=MD-F-%dD>",FontNr);
	 strcat(ZeilenPuffer,SetMode);	
return(TRUE);
};



BOOL SetFGColor(unsigned char Color, unsigned char *ZeilenPuffer)
{
 	char SetColor[30];
 	if (( Color == Old_Htm_FG_Color ) || ( Color > 7 )) return(TRUE);
     Old_Htm_FG_Color=Color;
	if ( Color == 0 ) strcpy(SetColor,"<font color=#000000>");
    else if ( Color == 1 ) strcpy(SetColor,"<font color=#FF0000>");
    else if ( Color == 2 ) strcpy(SetColor,"<font color=#00FF00>");
    else if ( Color == 3 ) strcpy(SetColor,"<font color=#FFFF00>");
    else if ( Color == 4 ) strcpy(SetColor,"<font color=#0000FF>");
    else if ( Color == 5 ) strcpy(SetColor,"<font color=#00FF00>");
    else if ( Color == 6 ) strcpy(SetColor,"<font color=#FF00FF>");
    else if ( Color == 7 ) strcpy(SetColor,"<font color=#FFFFFF>");
	strcat(ZeilenPuffer,SetColor);
	return(TRUE);
}

 BOOL Is_Val(unsigned char Byte)
{
	if (( Byte >= '0' ) && ( Byte <= '9' )) return(TRUE);
	return(FALSE);
}



BOOL Make_Txt_Page(FILE *fp , int Page, int SubPage)
{
	BOOL bGraph,bHoldGraph,bSepGraph,bBox,bFlash,bDouble,bConceal,bHasDouble;
	BYTE nLastGraph;
	BYTE c,ch;
	int n,row;
  	char tmp[41];
      char ZeilenPuffer[41];

 	bHasDouble=FALSE;
 	ZeilenPuffer[40]=0x00;

 	for (row=0; row<25 ; row++) {

  		ZeilenPuffer[0]=0x00;
  		if(bHasDouble) { bHasDouble=FALSE; continue; }
  
		bGraph=bHoldGraph=bSepGraph=bBox=bFlash=bDouble=bConceal=bHasDouble=FALSE;
 		nLastGraph=' ';

 		for (n=0; n<40; n++) {
  			tmp[n]=VTFrame[Page].SubPage[SubPage].Frame[row][n];
  			if(tmp[n]==0x0d) bHasDouble=TRUE;
  		}

 		if ( row == 0 ) memcpy(tmp, "\x03MD-VT  ", 8);

 		for (n=0; n<40; n++) {
  			c=tmp[n];
  			ch=c;
 			if (c<' ') {
  				if(c<8) bGraph=FALSE;
  				if(c>=0x10 && c<=0x17) bGraph=TRUE;
  				if(c==0x19) bSepGraph=FALSE;
  				if(c==0x1a) bSepGraph=TRUE;
  				if(c==0x1e) bHoldGraph=TRUE;
  				if(c==0x1f) bHoldGraph=FALSE;
 				ch=bHoldGraph?nLastGraph:' ';
  			}

 			if(bGraph && (ch&0x20)) {
  				nLastGraph=ch;
  				ch+=128;
  				if(bSepGraph) ch+=32;
  			}


 			if ( ch > ' ' ) {
 				ZeilenPuffer[n] = ch;
 			} else {
 				ZeilenPuffer[n] = ' ';
 			}
 		}
 		if (fprintf(fp,"%s\n",ZeilenPuffer) < 0 ) return(FALSE);
  	}

 	return(TRUE);
 }


  BOOL Make_Page_Control( FILE *fp ,int Page, int SubPage, int FontNr, LPSTR LinkRoot)
  {
 	char ZeilenPuffer[1024];
 	char LinkPuffer[512];
 	int NewPageNr;
 	int LinkPageNr;

 	ZeilenPuffer[0]=0x00;
 	sprintf(ZeilenPuffer,"<tr><td bgcolor=#C0C0C0> <font color=#FFFFFF face=MD-F-%d>",FontNr);

 	NewPageNr=Page-1;
 	while ((NewPageNr >= 0 ) && ( VTFrame[NewPageNr].SubCount==0)) NewPageNr--;

 	if ( NewPageNr >= 0  ) {
 		sprintf(LinkPuffer,"&nbsp;<a href=\"%s%d.htm\">&lt;&lt;</a>&nbsp;",LinkRoot,NewPageNr+100);
 	} else {
 		strcpy(LinkPuffer,"&nbsp;&nbsp;&nbsp;&nbsp;");
 	}
 	strcat(ZeilenPuffer,LinkPuffer);

 	if (( SubPage > 0 ) && ( VTFrame[Page].SubPage[SubPage-1].Fill == TRUE )) {
 		if ( SubPage-1 == 0 ) {
 			sprintf(LinkPuffer,"<a href=\"%s%d.htm\">&nbsp;%c&nbsp;</a>&nbsp;",LinkRoot,Page+100,(unsigned char)130);
 		} else {
 			sprintf(LinkPuffer,"&nbsp;<a href=\"%s%d-%d.htm\">%c</a>&nbsp;",LinkRoot,Page+100,SubPage-1,(unsigned char)130);
 		}
 	} else {
 		strcpy(LinkPuffer,"&nbsp;&nbsp;&nbsp;");
 	}
 	strcat(ZeilenPuffer,LinkPuffer);

 	if (( SubPage+1 < VTFrame[Page].SubCount ) && ( VTFrame[Page].SubPage[SubPage+1].Fill == TRUE )) {
 		sprintf(LinkPuffer,"&nbsp;<a href=\"%s%d-%d.htm\">%c</a>&nbsp;",LinkRoot,Page+100,SubPage+1,(unsigned char)129);
 	} else {
 		strcpy(LinkPuffer,"&nbsp;&nbsp;&nbsp;");
 	}
 	strcat(ZeilenPuffer,LinkPuffer);

 	NewPageNr=Page+1;
 	if ( NewPageNr > 799 ) NewPageNr=0;
 	while ((NewPageNr <=799 ) && ( VTFrame[NewPageNr].SubCount==0 )) {
 		NewPageNr++;
 	}

 	if ( NewPageNr <= 799 ) {
 		sprintf(LinkPuffer,"&nbsp;<a href=\"%s%d.htm\">&gt;&gt;</a>&nbsp;",LinkRoot,NewPageNr+100);
 	} else {
 		strcpy(LinkPuffer,"&nbsp;&nbsp;&nbsp;&nbsp;");
 	}
 	strcat(ZeilenPuffer,LinkPuffer);

 	strcat(ZeilenPuffer,"&nbsp;&nbsp;");

 	if (( Page != 0 ) && ( VTFrame[0].SubCount > 0 )) {
 		sprintf(LinkPuffer,"&nbsp;<a href=\"%s%d.htm\">100</a>&nbsp;",LinkRoot,100);
 	} else {
 		strcpy(LinkPuffer,"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
 	}
 	strcat(ZeilenPuffer,LinkPuffer);
 	LinkPageNr=(Page/100)-1;
 	LinkPageNr=LinkPageNr*100;
 	if (( LinkPageNr > 0 ) && ( VTFrame[LinkPageNr].SubCount > 0 )) {
 		sprintf(LinkPuffer,"&nbsp;<a href=\"%s%d.htm\">%d</a>&nbsp;",LinkRoot,LinkPageNr+100,LinkPageNr+100);
 	} else {
 		strcpy(LinkPuffer,"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
 	}
 	strcat(ZeilenPuffer,LinkPuffer);

 	LinkPageNr=((Page+8)/10)-1;
 	LinkPageNr=LinkPageNr*10;
 	if (( LinkPageNr > 0 ) && ( VTFrame[LinkPageNr].SubCount > 0 )) {
 		sprintf(LinkPuffer,"&nbsp;<a href=\"%s%d.htm\">%d</a>&nbsp;",LinkRoot,LinkPageNr+100,LinkPageNr+100);
 	} else {
 		strcpy(LinkPuffer,"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
 	}
 	strcat(ZeilenPuffer,LinkPuffer);

 	LinkPageNr=((Page+1)/10)+1;
 	LinkPageNr=LinkPageNr*10;
 	if (( LinkPageNr < 799 ) && ( VTFrame[LinkPageNr].SubCount > 0 )) {
 		sprintf(LinkPuffer,"&nbsp<a href=\"%s%d.htm\">%d</a>&nbsp",LinkRoot,LinkPageNr+100,LinkPageNr+100);
 	} else {
 		strcpy(LinkPuffer,"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
 	}
 	strcat(ZeilenPuffer,LinkPuffer);

 	LinkPageNr=(Page/100)+1;
 	LinkPageNr=LinkPageNr*100;
 	if (( LinkPageNr < 799 ) && ( VTFrame[LinkPageNr].SubCount > 0 )) {
 		sprintf(LinkPuffer,"&nbsp;<a href=\"%s%d.htm\">%d</a>&nbsp;",LinkRoot,LinkPageNr+100,LinkPageNr+100);
 	} else {
 		strcpy(LinkPuffer,"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
 	}
 	strcat(ZeilenPuffer,LinkPuffer);

 	if ( VTFrame[Page].SubCount > 1 ) {
 		sprintf(LinkPuffer,"&nbsp;(%d/%d)",SubPage+1,VTFrame[Page].SubCount);
 		strcat(ZeilenPuffer,LinkPuffer);
 	}

 	if (fprintf(fp,"%s\n",ZeilenPuffer) < 0 ) return(FALSE);
 	return(TRUE);
  }


BOOL APIENTRY VTInfoProc
(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{

	switch (message) {
        case WM_INITDIALOG:
             ShowVTInfo=hDlg;
			 SetTimer(hDlg,0,1000,NULL);
        case WM_TIMER:
			 if ( UTPages[0] !=  0 ) SetDlgItemInt(hDlg,TEXT8,UTPages[0],FALSE);
			 if ( UTPages[1] !=  0 ) SetDlgItemInt(hDlg,TEXT9,UTPages[1],FALSE);
			 if ( UTPages[2] !=  0 ) SetDlgItemInt(hDlg,TEXT10,UTPages[2],FALSE);
			 if ( UTPages[3] !=  0 ) SetDlgItemInt(hDlg,TEXT11,UTPages[3],FALSE);
			 if ( UTPages[4] !=  0 ) SetDlgItemInt(hDlg,TEXT12,UTPages[4],FALSE);
			 if ( UTPages[5] !=  0 ) SetDlgItemInt(hDlg,TEXT13,UTPages[5],FALSE);
			 if ( UTPages[6] !=  0 ) SetDlgItemInt(hDlg,TEXT14,UTPages[6],FALSE);
			 if ( UTPages[7] !=  0 ) SetDlgItemInt(hDlg,TEXT15,UTPages[7],FALSE);
			 if ( UTPages[8] !=  0 ) SetDlgItemInt(hDlg,TEXT16,UTPages[8],FALSE);
			 if ( UTPages[9] !=  0 ) SetDlgItemInt(hDlg,TEXT17,UTPages[9],FALSE);
			 if ( UTPages[10] !=  0 ) SetDlgItemInt(hDlg,TEXT18,UTPages[10],FALSE);
			 if ( UTPages[11] !=  0 ) SetDlgItemInt(hDlg,TEXT19,UTPages[11],FALSE);
			 SetDlgItemInt(hDlg,IDT_VBI_FPS,VBI_FPS,FALSE);
			 SetDlgItemInt(hDlg,TEXT1,VT_Cache,FALSE);
			 break;

        case WM_COMMAND:
			if ((LOWORD(wParam) == IDOK ) || (LOWORD(wParam) == IDCANCEL )) {
        			KillTimer(hDlg,0);
					ShowVTInfo=NULL;
					EndDialog(hDlg,TRUE);
			}
break;
    }

return(FALSE);
}

BOOL APIENTRY VTSettingProc
(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
	int i,j;

	switch (message) {
        case WM_INITDIALOG:
        	SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_RESETCONTENT, 0, 0);
    		SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_INSERTSTRING, 0, (LPARAM)(LPSTR)t("Automatisch"));
    		SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_INSERTSTRING, 1, (LPARAM)(LPSTR)t("Englisch"));
    		SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_INSERTSTRING, 2, (LPARAM)(LPSTR)t("Deutsch"));
    		SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_INSERTSTRING, 3, (LPARAM)(LPSTR)t("Italienisch"));
    		SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_INSERTSTRING, 4, (LPARAM)(LPSTR)t("Französisch"));
    		SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_INSERTSTRING, 5, (LPARAM)(LPSTR)t("Spanisch"));
            SendMessage(GetDlgItem(hDlg,IDC_COMBO1),CB_SETCURSEL,VT_Lang,0);

			if ( Initial_FontNr == 1 ) CheckDlgButton(hDlg, IDC_RADIO1, TRUE);
				else if ( Initial_FontNr == 2 ) CheckDlgButton(hDlg, IDC_RADIO2, TRUE);
				else if ( Initial_FontNr == 3 ) CheckDlgButton(hDlg, IDC_RADIO3, TRUE);
				SetDlgItemText(hDlg,IDC_EDIT1,VT_BASE_DIR);
				SetDlgItemText(hDlg,IDC_EDIT2,VT_EXPORT_WWW_ROOT);
				CheckDlgButton(hDlg, IDC_CHECK1, VT_ALWAYS_EXPORT);
			    CheckDlgButton(hDlg, IDC_CHECK2, VT_EXPORT_BACKGROUND);
			    CheckDlgButton(hDlg, IDC_CHECK3, VT_EXPORT_PAGE_LINK);
			    CheckDlgButton(hDlg, IDC_CHECK4, VT_EXPORT_PAGE_CONTROL);
			if ( Initial_Export_Font == 1 ) CheckDlgButton(hDlg, IDC_RADIO4, TRUE);
				else if ( Initial_Export_Font == 2 ) CheckDlgButton(hDlg, IDC_RADIO5, TRUE);
				else if ( Initial_Export_Font == 3 ) CheckDlgButton(hDlg, IDC_RADIO6, TRUE);
			if ( VT_EXPORT_TYP == 0 ) CheckDlgButton(hDlg, IDC_RADIO7, TRUE);
				else if ( VT_EXPORT_TYP == 1 ) CheckDlgButton(hDlg, IDC_RADIO8, TRUE);
				else if ( VT_EXPORT_TYP == 2 ) CheckDlgButton(hDlg, IDC_RADIO9, TRUE);
				else if ( VT_EXPORT_TYP == 3 ) CheckDlgButton(hDlg, IDC_RADIO10, TRUE);

				break;


        case WM_COMMAND:
			if (LOWORD(wParam) == IDOK ) {
                VT_Lang=SendMessage(GetDlgItem(hDlg,IDC_COMBO1),CB_GETCURSEL,0,0);

                GetDlgItemText(hDlg,IDC_EDIT1,VT_BASE_DIR,sizeof(VT_BASE_DIR));
                GetDlgItemText(hDlg,IDC_EDIT2,VT_EXPORT_WWW_ROOT,sizeof(VT_EXPORT_WWW_ROOT));
				i=(int)CreateDirectory(VT_BASE_DIR,NULL);
			    if ( IsDlgButtonChecked(hDlg, IDC_RADIO1) == TRUE ) Initial_FontNr=1;
					else if ( IsDlgButtonChecked(hDlg, IDC_RADIO2) == TRUE ) Initial_FontNr=2;
					else if ( IsDlgButtonChecked(hDlg, IDC_RADIO3) == TRUE ) Initial_FontNr=3;
			    VT_ALWAYS_EXPORT=IsDlgButtonChecked(hDlg, IDC_CHECK1);
			    VT_EXPORT_BACKGROUND=IsDlgButtonChecked(hDlg, IDC_CHECK2);
			    VT_EXPORT_PAGE_LINK=IsDlgButtonChecked(hDlg, IDC_CHECK3);
			    VT_EXPORT_PAGE_CONTROL=IsDlgButtonChecked(hDlg, IDC_CHECK4);

			    if ( IsDlgButtonChecked(hDlg, IDC_RADIO4) == TRUE ) Initial_Export_Font=1;
					else if ( IsDlgButtonChecked(hDlg, IDC_RADIO5) == TRUE ) Initial_Export_Font=2;
					else if ( IsDlgButtonChecked(hDlg, IDC_RADIO6) == TRUE ) Initial_Export_Font=3;

				if ( IsDlgButtonChecked(hDlg, IDC_RADIO7) == TRUE ) VT_EXPORT_TYP=0;
					else if ( IsDlgButtonChecked(hDlg, IDC_RADIO8) == TRUE ) VT_EXPORT_TYP=1;
					else if ( IsDlgButtonChecked(hDlg, IDC_RADIO9) == TRUE ) VT_EXPORT_TYP=2;
					else if ( IsDlgButtonChecked(hDlg, IDC_RADIO10) == TRUE ) VT_EXPORT_TYP=3;

					if ( VT_ALWAYS_EXPORT == TRUE ) {
						for ( i=0; i< 800; i++ ) {
							for ( j = 0; j < VTFrame[i].SubCount; j++ ) {
								 VTFrame[i].SubPage[j].CRC=0;
							};
						};
					};
 
                    

					EndDialog(hDlg,TRUE);
			}
			
				if (LOWORD(wParam) == IDCANCEL ) {
					EndDialog(hDlg,TRUE);
			}
break;
    }

return(FALSE);
}

