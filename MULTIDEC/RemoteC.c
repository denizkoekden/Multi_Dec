/*********************************************************************************
 *                                                                               *
 * RemoteC.c: Tools für die Fernbedienung 										 *
 *                                                                               *
 * Copyright (C) 2000/2001 Espresso                                              *
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
#include <winable.h> 
#include <stdio.h>
#include "resource.h"
#include <stdlib.h>        /* atoi                                  */
#include <memory.h>         

#include "RemoteC.h"

extern void Write_Log(const char *Text);

void Write_Key_Section(LPSTR SectionName);
BOOL Load_Key_Section(LPSTR SectionName);
int Load_All_Sections( void );
void SendKeyString(LPSTR Text);
void SendKeyBoard(int Anzahl,INPUT *MyInput);
void Repeat_Thread( void );
void Create_Default_RC( void );
int PrepareKeyString(LPSTR Text,INPUT *MyInput);


void SetButton(HDC hMemDC, int x,int y,unsigned int CVal )
{
	int i,j;
	unsigned int a;


    for ( i=x; i< x+28; i++ ) {
       for ( j=y; j< y+12; j++ ) {
           a=GetPixel(hMemDC,i,j);
           if (( a == 0x007b0000 ) ||( a == 0x007f0000 ) ||( a == 0x000000ff ) || ( a == 0x0000ff00 ) || ( a == 0x00848284 ) || ( a == 0x00888488 )) {
			    SetPixel(hMemDC,i,j,CVal);
		   };
 	   };
	};
};


void Init_Key_Table( void )
{
	int i;
   FILE* KeyFile;

   if ((KeyFile=fopen("Remote.ctl","r")) == NULL )  Create_Default_RC( );
     else fclose(KeyFile);


   AllSectionCount=Load_All_Sections();

	for ( i=0; i< 256; i++ ) KeyTable[i].Name[0]=0x00;

strcpy(KeyTable[0x01].Name,"VK_LBUTTON");  
strcpy(KeyTable[0x02].Name,"VK_RBUTTON");  
strcpy(KeyTable[0x03].Name,"VK_CANCEL");   
strcpy(KeyTable[0x04].Name,"VK_MBUTTON");

strcpy(KeyTable[0x08].Name,"VK_BACK");           
strcpy(KeyTable[0x09].Name,"VK_TAB");            

strcpy(KeyTable[0x0C].Name,"VK_CLEAR");          
strcpy(KeyTable[0x0D].Name,"VK_RETURN");         

strcpy(KeyTable[0x13].Name,"VK_PAUSE");          
strcpy(KeyTable[0x14].Name,"VK_CAPITAL");        

strcpy(KeyTable[0x1B].Name,"VK_ESCAPE");
strcpy(KeyTable[0x1C].Name,"VK_CONVERT");   
strcpy(KeyTable[0x1D].Name,"VK_NONCONVERT");
strcpy(KeyTable[0x1E].Name,"VK_ACCEPT");    
strcpy(KeyTable[0x1F].Name,"VK_MODECHANGE");

strcpy(KeyTable[0x20].Name,"VK_SPACE");       
strcpy(KeyTable[0x21].Name,"VK_PRIOR");       
strcpy(KeyTable[0x22].Name,"VK_NEXT");        
strcpy(KeyTable[0x23].Name,"VK_END");       
strcpy(KeyTable[0x24].Name,"VK_HOME");         
strcpy(KeyTable[0x25].Name,"VK_LEFT");         
strcpy(KeyTable[0x26].Name,"VK_UP");           
strcpy(KeyTable[0x27].Name,"VK_RIGHT");        
strcpy(KeyTable[0x28].Name,"VK_DOWN");         
strcpy(KeyTable[0x29].Name,"VK_SELECT");       
strcpy(KeyTable[0x2A].Name,"VK_PRINT");        
strcpy(KeyTable[0x2B].Name,"VK_EXECUTE");      
strcpy(KeyTable[0x2C].Name,"VK_SNAPSHOT");     
strcpy(KeyTable[0x2D].Name,"VK_INSERT");       
strcpy(KeyTable[0x2E].Name,"VK_DELETE");       
strcpy(KeyTable[0x2F].Name,"VK_HELP");         
strcpy(KeyTable[0x5B].Name,"VK_LWIN");       
strcpy(KeyTable[0x5C].Name,"VK_RWIN");       
strcpy(KeyTable[0x5D].Name,"VK_APPS");         
strcpy(KeyTable[0x60].Name,"VK_NUMPAD0");       
strcpy(KeyTable[0x61].Name,"VK_NUMPAD1");       
strcpy(KeyTable[0x62].Name,"VK_NUMPAD2");        
strcpy(KeyTable[0x63].Name,"VK_NUMPAD3");       
strcpy(KeyTable[0x64].Name,"VK_NUMPAD4");         
strcpy(KeyTable[0x65].Name,"VK_NUMPAD5");         
strcpy(KeyTable[0x66].Name,"VK_NUMPAD6");           
strcpy(KeyTable[0x67].Name,"VK_NUMPAD7");        
strcpy(KeyTable[0x68].Name,"VK_NUMPAD8");         
strcpy(KeyTable[0x69].Name,"VK_NUMPAD9");       
strcpy(KeyTable[0x6A].Name,"VK_MULTIPLY");        
strcpy(KeyTable[0x6B].Name,"VK_ADD");      
strcpy(KeyTable[0x6C].Name,"VK_SEPARATOR");     
strcpy(KeyTable[0x6D].Name,"VK_SUBTRACT");       
strcpy(KeyTable[0x6E].Name,"VK_DECIMAL");       
strcpy(KeyTable[0x6F].Name,"VK_DIVIDE");         
strcpy(KeyTable[0x70].Name,"VK_F1");   
strcpy(KeyTable[0x71].Name,"VK_F2");       
strcpy(KeyTable[0x72].Name,"VK_F3");        
strcpy(KeyTable[0x73].Name,"VK_F4");       
strcpy(KeyTable[0x74].Name,"VK_F5");         
strcpy(KeyTable[0x75].Name,"VK_F6");         
strcpy(KeyTable[0x76].Name,"VK_F7");           
strcpy(KeyTable[0x77].Name,"VK_F8");        
strcpy(KeyTable[0x78].Name,"VK_F9");         
strcpy(KeyTable[0x79].Name,"VK_F10");       
strcpy(KeyTable[0x7A].Name,"VK_F11");        
strcpy(KeyTable[0x7B].Name,"VK_F12");      
strcpy(KeyTable[0x7C].Name,"VK_F13");     
strcpy(KeyTable[0x7D].Name,"VK_F14");       
strcpy(KeyTable[0x7E].Name,"VK_F15");       
strcpy(KeyTable[0x7F].Name,"VK_F16");         
strcpy(KeyTable[0x80].Name,"VK_F17");   
strcpy(KeyTable[0x81].Name,"VK_F18");    
strcpy(KeyTable[0x82].Name,"VK_F19");      
strcpy(KeyTable[0x83].Name,"VK_F20");       
strcpy(KeyTable[0x84].Name,"VK_F21");         
strcpy(KeyTable[0x85].Name,"VK_F22");         
strcpy(KeyTable[0x86].Name,"VK_F23");           
strcpy(KeyTable[0x87].Name,"VK_F24");        
strcpy(KeyTable[0x90].Name,"VK_NUMLOCK");           
strcpy(KeyTable[0x91].Name,"VK_SCROLL");        
strcpy(KeyTable[0xF6].Name,"VK_ATTN");           
strcpy(KeyTable[0xF7].Name,"VK_CRSEL");           
strcpy(KeyTable[0xF8].Name,"VK_EXSEL");           
strcpy(KeyTable[0xF9].Name,"VK_EREOF");           
strcpy(KeyTable[0xFA].Name,"VK_PLAY");           
strcpy(KeyTable[0xFB].Name,"VK_ZOOM");           
strcpy(KeyTable[0xFC].Name,"VK_NONAME");           
strcpy(KeyTable[0xFD].Name,"VK_PA1");           
strcpy(KeyTable[0xFE].Name,"VK_OEM_CLEAR");           
};


LONG WINAPI KeyWndProc (HWND hwnd, UINT message, DWORD wParam, LONG lParam)
{

	char Text[128];
	char Text1[128];
    char More[128];
    int Shift;
    int Control;
    int Alt;
    int k;

	switch(message) {
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
			  Shift=(GetKeyState(VK_SHIFT)&0xff);
			  Control=(GetKeyState(VK_CONTROL)&0xff);
			  Alt=(GetKeyState(VK_MENU)&0xff);
              Text[0]=0x00;
              k=(wParam&0xff);
			  if ( KeyTable[k].Name[0] != 0x00 ) {
				       GetWindowText(KeyEditWnd,Text1,127);
                       sprintf(More,"{\"");
					   if ( Shift > 1 ) strcat(More,"Shift+");
					   if ( Control > 1 ) strcat(More,"Control+");
					   if ( Alt > 1 ) strcat(More,"Alt+");
					   sprintf(Text,"%s\"}",KeyTable[k].Name);
					   strcat(Text1,More);
					   strcat(Text1,Text);
					   SetWindowText(KeyEditWnd,Text1);
                       return(TRUE);
			  } else if ((k >= 0x20 ) && (k < 0x7f ) && (( Control>1) || (Alt>1))) {
				       GetWindowText(KeyEditWnd,Text1,127);
                       sprintf(More,"{\"");
					   if ( Control > 1 ) strcat(More,"Control+");
					   if ( Alt > 1 ) strcat(More,"Alt+");
					   if ( Shift > 1 ) {
					    sprintf(Text,"%c\"}",k);
					   } else {
						   if ((k >= 0x41 ) && (k <= 0x5a )) k+=0x20;
						   sprintf(Text,"%c\"}",k);
					   };
					   strcat(Text1,More);
					   strcat(Text1,Text);
					   SetWindowText(KeyEditWnd,Text1);
                       return(TRUE);
			  } else break;
	
	
	case WM_CHAR:  
			  break;
		
	};

	
	return(CallWindowProc((WNDPROC)OldEditProc,hwnd, message, wParam, lParam));
};


BOOL APIENTRY RemoteControlCodeScanProc
(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
	switch (message) {
        case WM_INITDIALOG:
          KeyEditWnd=GetDlgItem(hDlg,IDC_EDIT1);
          OldEditProc=(FARPROC)SetWindowLong(KeyEditWnd,GWL_WNDPROC,(LONG)KeyWndProc);
          SetFocus(KeyEditWnd);
		  break;

        case WM_COMMAND:
			if (LOWORD(wParam) == IDOK ) {
				   GetDlgItemText(hDlg,IDC_EDIT1,CodeString,127);
				   EndDialog(hDlg,TRUE);
			}

			if (LOWORD(wParam) == IDCANCEL ) {
				CodeString[0]=0x00;
				EndDialog(hDlg,FALSE);
			}

	};
return(FALSE);
}
		  

BOOL APIENTRY RemoteControlSettingProc
(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
    int key;
    static HBITMAP BitmapControl;
	struct TButton {
		   int x;
		   int y;
		   unsigned int ColorVal;
	};

    char Text[128];

	static struct TButton Button[128];


	int i;
	HDC wDC;
    PAINTSTRUCT wps;                  
	BITMAP bm;
    HBITMAP         hOldBm;
    HDC             hMemDC;
    static int CurrentWorkingKey=-1;

	static int AltKey=0xffffffff;
	static RECT            rect;

	switch (message) {
        case WM_INITDIALOG:
			if (RemoteKey == NULL) return FALSE;
			SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_RESETCONTENT, 0, 0);
                	for (i=0; i<AllSectionCount; i++) {
		               if ( AllSections[i][0] != 0x00 ) SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_INSERTSTRING, i, (LPARAM)(LPSTR)AllSections[i]);
			};
			      strcpy(CurrentSectionName,Running_Remote_Control_Section);
                  i=SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_FINDSTRING, 0, (LPARAM)CurrentSectionName); 
				  if ( i >= 0 ) {
				  SendMessage(GetDlgItem(hDlg,IDC_COMBO1),CB_SETCURSEL,i,0);
				  Load_Key_Section(CurrentSectionName);
				  } else {	  
				  SendMessage(GetDlgItem(hDlg,IDC_COMBO1),CB_SETCURSEL,0,0);
         		  SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_GETLBTEXT, 0, (LPARAM)&CurrentSectionName);
				  Load_Key_Section(CurrentSectionName);
                  };
				  BitmapControl= LoadBitmap(hInst,"REMOTECONTROL");

			 SendMessage(hDlg,WM_USER,2,0);
             rect.top=76;
			 rect.bottom=246;
			 rect.left=10;
			 rect.right=200;
			 REMOTE_CONFIG_WND=hDlg;

			 break;


		case WM_USER:

            if ( wParam == 2 ) {

				  
			 for ( i=0; i < 128; i++ ) {
				 			 Button[i].x=0;
			                 Button[i].y=0;
			 };
// 1. Reihe
			 i=15;
			 Button[i].x=6;
			 Button[i].y=14;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
			 i=32;
			 Button[i].x=39;
			 Button[i].y=14;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
			 i=12;
			 Button[i].x=72;
			 Button[i].y=14;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
// 2. Reihe
			 i=17;
			 Button[i].x=6;
			 Button[i].y=37;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
			 i=46;
			 Button[i].x=39;
			 Button[i].y=37;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
			 i=16;
			 Button[i].x=72;
			 Button[i].y=37;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
// 3. Reihe
             i=13;
			 Button[i].x=6;
			 Button[i].y=60;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
             i=33;
			 Button[i].x=39;
			 Button[i].y=60;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
             i=34;
			 Button[i].x=72;
			 Button[i].y=60;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;

// 4. Reihe
			 i=1;
			 Button[i].x=6;
			 Button[i].y=83;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
 			 i=2;
			 Button[i].x=39;
			 Button[i].y=83;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
			 i=3;
			 Button[i].x=72;
			 Button[i].y=83;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
// 5. Reihe
			 i=4;
			 Button[i].x=6;
			 Button[i].y=106;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
 			 i=5;
			 Button[i].x=39;
			 Button[i].y=106;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
			 i=6;
			 Button[i].x=72;
			 Button[i].y=106;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
// 6. Reihe
			 i=7;
			 Button[i].x=6;
			 Button[i].y=129;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
 			 i=8;
			 Button[i].x=39;
			 Button[i].y=129;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
			 i=9;
			 Button[i].x=72;
			 Button[i].y=129;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
// 7. Reihe
			 i=30;
			 Button[i].x=6;
			 Button[i].y=152;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
			 i=0;
			 Button[i].x=38;
			 Button[i].y=152;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;
			 i=38;
			 Button[i].x=72;
			 Button[i].y=152;
		 	 if (RemoteKey[i].SendKey[0][0] != 0x00 ) Button[i].ColorVal=0x007f0000;
                  else Button[i].ColorVal=0x00848284;

			};
			
			
			
			if ( wParam == 1 ) {
                if ( CurrentWorkingKey != -1 ) {
                      GetDlgItemText(hDlg,IDC_EDIT1,Text,127);
                      RemoteKey[CurrentWorkingKey].TimeOutRate = atoi(Text);
                      GetDlgItemText(hDlg,IDC_EDIT2,Text,127);
                      RemoteKey[CurrentWorkingKey].RepeatRate = atoi(Text);
                      GetDlgItemText(hDlg,IDC_EDIT3,RemoteKey[CurrentWorkingKey].SendKey[0],127);
                      GetDlgItemText(hDlg,IDC_EDIT4,RemoteKey[CurrentWorkingKey].SendKey[1],127);
                      GetDlgItemText(hDlg,IDC_EDIT5,RemoteKey[CurrentWorkingKey].SendKey[2],127);
                      GetDlgItemText(hDlg,IDC_EDIT6,RemoteKey[CurrentWorkingKey].SendKey[3],127);
                      GetDlgItemText(hDlg,IDC_EDIT7,RemoteKey[CurrentWorkingKey].SendKey[4],127);
					  CurrentWorkingKey=-1;
			};

				key=lParam;
                sprintf(Text,"%08X",key);
				SetDlgItemText(hDlg,IDC_KEYNAME,Text);
				if ( key < 128 ) {      
			        if ( key == AltKey ) return(TRUE);
					Button[key].ColorVal =0x0000ff00;
                    if ( AltKey >=0 ) {
					 	 if (RemoteKey[AltKey].SendKey[0][0] != 0x00 ) Button[AltKey].ColorVal=0x007f0000;
                               else Button[AltKey].ColorVal=0x00848284;
					}
					AltKey=key;
					InvalidateRect(hDlg,&rect,FALSE);
				} else if ( key-0x8000 < 128 ) {
			        if ( key == AltKey ) return(TRUE);
					key=key-0x8000;
					Button[key].ColorVal =0x000000ff;
                    if (( AltKey >=0 ) && ( AltKey != key )) {
					 	 if (RemoteKey[AltKey].SendKey[0][0] != 0x00 ) Button[AltKey].ColorVal=0x007f0000;
                               else Button[AltKey].ColorVal=0x00848284;
					};
					AltKey=key+0x8000;
					InvalidateRect(hDlg,&rect,FALSE);
				};
				if (( key >= 0 ) && ( key < 128 )) {
					  CurrentWorkingKey=key;
                      sprintf(Text,"%d",RemoteKey[key].TimeOutRate);
                      SetDlgItemText(hDlg,IDC_EDIT1,Text);
                      sprintf(Text,"%d",RemoteKey[key].RepeatRate);
                      SetDlgItemText(hDlg,IDC_EDIT2,Text);
                      SetDlgItemText(hDlg,IDC_EDIT3,RemoteKey[key].SendKey[0]);
                      SetDlgItemText(hDlg,IDC_EDIT4,RemoteKey[key].SendKey[1]);
                      SetDlgItemText(hDlg,IDC_EDIT5,RemoteKey[key].SendKey[2]);
                      SetDlgItemText(hDlg,IDC_EDIT6,RemoteKey[key].SendKey[3]);
                      SetDlgItemText(hDlg,IDC_EDIT7,RemoteKey[key].SendKey[4]);

				};


			};
             break;


		case WM_PAINT:
	        wDC=BeginPaint(hDlg,&wps);
            GetClientRect(hDlg, &rect);
            hMemDC = CreateCompatibleDC(wDC);
			hOldBm = SelectObject(hMemDC, BitmapControl);
            for ( i=0; i < 128; i++ ) {
				if ( Button[i].x+Button[i].y != 0) SetButton(hMemDC, Button[i].x,Button[i].y,Button[i].ColorVal);
			};

			GetObject(BitmapControl, sizeof(BITMAP), (LPSTR)&bm);

			BitBlt(wDC, 10, 76, bm.bmWidth, bm.bmHeight,hMemDC, 0, 0, SRCCOPY);
			SelectObject(hMemDC,hOldBm);
            DeleteDC(hMemDC);
			DeleteDC(wDC);
            EndPaint(hDlg,&wps);
            return(TRUE);


        case WM_COMMAND:

  				if  ((HWND)lParam == GetDlgItem(hDlg,IDC_COMBO1)) {
    			 if (HIWORD(wParam)==CBN_SELCHANGE){
				  i=SendMessage(GetDlgItem(hDlg,IDC_COMBO1),CB_GETCURSEL,0,0);

				  if ( CurrentWorkingKey != -1 ) {
                      GetDlgItemText(hDlg,IDC_EDIT1,Text,127);
                      RemoteKey[CurrentWorkingKey].TimeOutRate = atoi(Text);
                      GetDlgItemText(hDlg,IDC_EDIT2,Text,127);
                      RemoteKey[CurrentWorkingKey].RepeatRate = atoi(Text);
                      GetDlgItemText(hDlg,IDC_EDIT3,RemoteKey[CurrentWorkingKey].SendKey[0],127);
                      GetDlgItemText(hDlg,IDC_EDIT4,RemoteKey[CurrentWorkingKey].SendKey[1],127);
                      GetDlgItemText(hDlg,IDC_EDIT5,RemoteKey[CurrentWorkingKey].SendKey[2],127);
                      GetDlgItemText(hDlg,IDC_EDIT6,RemoteKey[CurrentWorkingKey].SendKey[3],127);
                      GetDlgItemText(hDlg,IDC_EDIT7,RemoteKey[CurrentWorkingKey].SendKey[4],127);
					  CurrentWorkingKey=-1;
			};
                      SetDlgItemText(hDlg,IDC_EDIT1,"");
                      SetDlgItemText(hDlg,IDC_EDIT2,"");
                      SetDlgItemText(hDlg,IDC_EDIT3,"");
                      SetDlgItemText(hDlg,IDC_EDIT4,"");
                      SetDlgItemText(hDlg,IDC_EDIT5,"");
                      SetDlgItemText(hDlg,IDC_EDIT6,"");
                      SetDlgItemText(hDlg,IDC_EDIT7,"");

				    Write_Key_Section(CurrentSectionName);
         			SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_GETLBTEXT, i, (LPARAM)&CurrentSectionName);
					Load_Key_Section(CurrentSectionName);
					SendMessage(hDlg,WM_USER,2,0);
					InvalidateRect(hDlg,&rect,FALSE);
				 };
			};

				if (LOWORD(wParam) == IDCODESCAN1 ) {
					if ( DialogBox(hInst,"REMOTECONTROLCODESCAN",hDlg,RemoteControlCodeScanProc) == TRUE ) {
						SetDlgItemText(hDlg,IDC_EDIT3,CodeString);
				};
				};
				if (LOWORD(wParam) == IDCODESCAN2 ) {
					if ( DialogBox(hInst,"REMOTECONTROLCODESCAN",hDlg,RemoteControlCodeScanProc) == TRUE ) {
						SetDlgItemText(hDlg,IDC_EDIT4,CodeString);
				};
				};
				if (LOWORD(wParam) == IDCODESCAN3 ) {
					if ( DialogBox(hInst,"REMOTECONTROLCODESCAN",hDlg,RemoteControlCodeScanProc) == TRUE ) {
						SetDlgItemText(hDlg,IDC_EDIT5,CodeString);
				};
				};
				if (LOWORD(wParam) == IDCODESCAN4 ) {
					if ( DialogBox(hInst,"REMOTECONTROLCODESCAN",hDlg,RemoteControlCodeScanProc) == TRUE ) {
						SetDlgItemText(hDlg,IDC_EDIT6,CodeString);
				};
				};
				if (LOWORD(wParam) == IDCODESCAN5 ) {
					if ( DialogBox(hInst,"REMOTECONTROLCODESCAN",hDlg,RemoteControlCodeScanProc) == TRUE ) {
						SetDlgItemText(hDlg,IDC_EDIT7,CodeString);
				};
				};

			if (LOWORD(wParam) == IDOK ) {
                if ( CurrentWorkingKey != -1 ) {
                      GetDlgItemText(hDlg,IDC_EDIT1,Text,127);
                      RemoteKey[CurrentWorkingKey].TimeOutRate = atoi(Text);
                      GetDlgItemText(hDlg,IDC_EDIT2,Text,127);
                      RemoteKey[CurrentWorkingKey].RepeatRate = atoi(Text);
                      GetDlgItemText(hDlg,IDC_EDIT3,RemoteKey[CurrentWorkingKey].SendKey[0],127);
                      GetDlgItemText(hDlg,IDC_EDIT4,RemoteKey[CurrentWorkingKey].SendKey[1],127);
                      GetDlgItemText(hDlg,IDC_EDIT5,RemoteKey[CurrentWorkingKey].SendKey[2],127);
                      GetDlgItemText(hDlg,IDC_EDIT6,RemoteKey[CurrentWorkingKey].SendKey[3],127);
                      GetDlgItemText(hDlg,IDC_EDIT7,RemoteKey[CurrentWorkingKey].SendKey[4],127);
					  CurrentWorkingKey=-1;
			};
				   Write_Key_Section(CurrentSectionName);
				   strcpy(Last_Running_Remote_Control_Section,"Force_Reload_HiHi");
				   REMOTE_CONFIG_WND=NULL;
				   EndDialog(hDlg,TRUE);
			}

			if (LOWORD(wParam) == IDCANCEL ) {
			    strcpy(Last_Running_Remote_Control_Section,"Force_Reload_HiHi");
    			REMOTE_CONFIG_WND=NULL;
				EndDialog(hDlg,TRUE);
			}

	};
return(FALSE);
}

void Create_Default_RC( void )
{
 FILE* KeyFile;

 if ((KeyFile=fopen("Remote.ctl","w")) == NULL )  return;
fprintf(KeyFile,"*[MultiDec OSD-Full]*\n");
fprintf(KeyFile,"KEY 000 0500 0002 0 0\n");
fprintf(KeyFile,"KEY 001 0500 0000 0  \n");
fprintf(KeyFile,"KEY 001 0500 0000 1 1\n");
fprintf(KeyFile,"KEY 002 0500 0000 0 A\n");
fprintf(KeyFile,"KEY 002 0500 0000 1 B\n");
fprintf(KeyFile,"KEY 002 0500 0000 2 C\n");
fprintf(KeyFile,"KEY 002 0500 0000 3 2\n");
fprintf(KeyFile,"KEY 003 0500 0000 0 D\n");
fprintf(KeyFile,"KEY 003 0500 0000 1 E\n");
fprintf(KeyFile,"KEY 003 0500 0000 2 E\n");
fprintf(KeyFile,"KEY 003 0500 0000 3 3\n");
fprintf(KeyFile,"KEY 004 0500 0000 0 G\n");
fprintf(KeyFile,"KEY 004 0500 0000 1 H\n");
fprintf(KeyFile,"KEY 004 0500 0000 2 I\n");
fprintf(KeyFile,"KEY 004 0500 0000 3 4\n");
fprintf(KeyFile,"KEY 005 0500 0000 0 J\n");
fprintf(KeyFile,"KEY 005 0500 0000 1 K\n");
fprintf(KeyFile,"KEY 005 0500 0000 2 L\n");
fprintf(KeyFile,"KEY 005 0500 0000 3 5\n");
fprintf(KeyFile,"KEY 006 0500 0000 0 M\n");
fprintf(KeyFile,"KEY 006 0500 0000 1 N\n");
fprintf(KeyFile,"KEY 006 0500 0000 2 O\n");
fprintf(KeyFile,"KEY 006 0500 0000 3 6\n");
fprintf(KeyFile,"KEY 007 0500 0000 0 P\n");
fprintf(KeyFile,"KEY 007 0500 0000 1 Q\n");
fprintf(KeyFile,"KEY 007 0500 0000 2 R\n");
fprintf(KeyFile,"KEY 007 0500 0000 3 S\n");
fprintf(KeyFile,"KEY 007 0500 0000 4 7\n");
fprintf(KeyFile,"KEY 008 0500 0000 0 T\n");
fprintf(KeyFile,"KEY 008 0500 0000 1 U\n");
fprintf(KeyFile,"KEY 008 0500 0000 2 V\n");
fprintf(KeyFile,"KEY 008 0500 0000 3 8\n");
fprintf(KeyFile,"KEY 009 0500 0000 0 W\n");
fprintf(KeyFile,"KEY 009 0500 0000 1 X\n");
fprintf(KeyFile,"KEY 009 0500 0000 2 Y\n");
fprintf(KeyFile,"KEY 009 0500 0000 3 Z\n");
fprintf(KeyFile,"KEY 009 0500 0000 4 0\n");
fprintf(KeyFile,"KEY 013 0000 0000 0 {\"VK_BACK\"}\n");
fprintf(KeyFile,"KEY 016 0000 0000 0 {\"VK_RIGHT\"}\n");
fprintf(KeyFile,"KEY 017 0000 0000 0 {\"VK_LEFT\"}\n");
fprintf(KeyFile,"KEY 034 0000 0000 0 {\"VK_ESCAPE\"}\n");
fprintf(KeyFile,"KEY 046 0000 0000 0 {\"VK_RETURN\"}\n");
fprintf(KeyFile,"*[MultiDec OSD-Normal]*\n");
fprintf(KeyFile,"KEY 000 0000 0000 0 0\n");
fprintf(KeyFile,"KEY 001 0000 0000 0 1\n");
fprintf(KeyFile,"KEY 002 0000 0000 0 2\n");
fprintf(KeyFile,"KEY 003 0000 0000 0 3\n");
fprintf(KeyFile,"KEY 004 0000 0000 0 4\n");
fprintf(KeyFile,"KEY 005 0000 0000 0 5\n");
fprintf(KeyFile,"KEY 006 0000 0000 0 6\n");
fprintf(KeyFile,"KEY 007 0000 0000 0 7\n");
fprintf(KeyFile,"KEY 008 0000 0000 0 8\n");
fprintf(KeyFile,"KEY 009 0000 0000 0 9\n");
fprintf(KeyFile,"KEY 012 0000 0010 0 9\n");
fprintf(KeyFile,"KEY 013 0000 0010 0 1\n");
fprintf(KeyFile,"KEY 015 0000 0010 0 7\n");
fprintf(KeyFile,"KEY 016 0000 0000 0 6\n");
fprintf(KeyFile,"KEY 017 0000 0000 0 4\n");
fprintf(KeyFile,"KEY 030 0000 0000 0 0\n");
fprintf(KeyFile,"KEY 032 0000 0010 0 8\n");
fprintf(KeyFile,"KEY 033 0000 0010 0 2\n");
fprintf(KeyFile,"KEY 034 0000 0010 0 3\n");
fprintf(KeyFile,"KEY 038 0000 0000 0 0\n");
fprintf(KeyFile,"KEY 046 0000 0000 0 5\n");
fprintf(KeyFile,"*[MultiDec Digital]*\n");
fprintf(KeyFile,"KEY 000 0000 0000 0 0\n");
fprintf(KeyFile,"KEY 001 0000 0000 0 1\n");
fprintf(KeyFile,"KEY 002 0000 0000 0 2\n");
fprintf(KeyFile,"KEY 003 0000 0000 0 3\n");
fprintf(KeyFile,"KEY 004 0000 0000 0 4\n");
fprintf(KeyFile,"KEY 005 0000 0000 0 5\n");
fprintf(KeyFile,"KEY 006 0000 0000 0 6\n");
fprintf(KeyFile,"KEY 007 0000 0000 0 7\n");
fprintf(KeyFile,"KEY 008 0000 0000 0 8\n");
fprintf(KeyFile,"KEY 009 0000 0000 0 9\n");
fprintf(KeyFile,"KEY 012 0000 0000 0 l\n");
fprintf(KeyFile,"KEY 013 0000 0000 0 m\n");
fprintf(KeyFile,"KEY 015 0000 0000 0 P\n");
fprintf(KeyFile,"KEY 016 0000 0010 0 V\n");
fprintf(KeyFile,"KEY 017 0000 0010 0 v\n");
fprintf(KeyFile,"KEY 030 0000 0000 0  \n");
fprintf(KeyFile,"KEY 032 0000 0000 0 {\"VK_F1\"}\n");
fprintf(KeyFile,"KEY 033 0000 0000 0 {\"VK_F2\"}\n");
fprintf(KeyFile,"KEY 034 0000 0000 0 a\n");
fprintf(KeyFile,"KEY 038 0000 0000 0 D\n");
fprintf(KeyFile,"KEY 046 0000 0000 0 {\"Alt+VK_F1\"}\n");
fprintf(KeyFile,"KEY 046 0000 0000 1 {\"Alt+VK_F2\"}\n");
fprintf(KeyFile,"KEY 046 0000 0000 2 {\"Alt+VK_F3\"}\n");
fprintf(KeyFile,"*[MultiDec Videotext]*\n");
fprintf(KeyFile,"KEY 000 0000 0000 0 0\n");
fprintf(KeyFile,"KEY 001 0000 0000 0 1\n");
fprintf(KeyFile,"KEY 002 0000 0000 0 2\n");
fprintf(KeyFile,"KEY 003 0000 0000 0 3\n");
fprintf(KeyFile,"KEY 004 0000 0000 0 4\n");
fprintf(KeyFile,"KEY 005 0000 0000 0 5\n");
fprintf(KeyFile,"KEY 006 0000 0000 0 6\n");
fprintf(KeyFile,"KEY 007 0000 0000 0 7\n");
fprintf(KeyFile,"KEY 008 0000 0000 0 8\n");
fprintf(KeyFile,"KEY 009 0000 0000 0 9\n");
fprintf(KeyFile,"KEY 016 0000 0000 0 {\"VK_RIGHT\"}\n");
fprintf(KeyFile,"KEY 017 0000 0000 0 {\"VK_LEFT\"}\n");
fprintf(KeyFile,"KEY 030 0000 0000 0 100\n");
fprintf(KeyFile,"KEY 032 0000 0000 0 {\"VK_UP\"}\n");
fprintf(KeyFile,"KEY 033 0000 0000 0 {\"VK_DOWN\"}\n");
fprintf(KeyFile,"KEY 038 0000 0000 0 {\"Control+f\"}\n");
fprintf(KeyFile,"*[MultiDec Analog]*\n");
fprintf(KeyFile,"KEY 000 0000 0000 0 0\n");
fprintf(KeyFile,"KEY 001 0000 0000 0 1\n");
fprintf(KeyFile,"KEY 002 0000 0000 0 2\n");
fprintf(KeyFile,"KEY 003 0000 0000 0 3\n");
fprintf(KeyFile,"KEY 004 0000 0000 0 4\n");
fprintf(KeyFile,"KEY 005 0000 0000 0 5\n");
fprintf(KeyFile,"KEY 006 0000 0000 0 6\n");
fprintf(KeyFile,"KEY 007 0000 0000 0 7\n");
fprintf(KeyFile,"KEY 008 0000 0000 0 8\n");
fprintf(KeyFile,"KEY 009 0000 0000 0 9\n");
fprintf(KeyFile,"KEY 013 0000 0000 0 m\n");
fprintf(KeyFile,"KEY 016 0000 0010 0 V\n");
fprintf(KeyFile,"KEY 017 0000 0010 0 v\n");
fprintf(KeyFile,"KEY 032 0000 0000 0 {\"VK_F1\"}\n");
fprintf(KeyFile,"KEY 033 0000 0000 0 {\"VK_F2\"}\n");
fprintf(KeyFile,"KEY 034 0000 0000 0 {\"Control+7\"}\n");
fprintf(KeyFile,"KEY 034 0000 0000 1 {\"Control+8\"}\n");
fprintf(KeyFile,"KEY 034 0000 0000 2 {\"Control+9\"}\n");
fprintf(KeyFile,"KEY 046 0000 0000 0 {\"Alt+VK_F1\"}\n");
fprintf(KeyFile,"KEY 046 0000 0000 1 {\"Alt+VK_F2\"}\n");
fprintf(KeyFile,"KEY 046 0000 0000 2 {\"Alt+VK_F3\"}\n");

 fclose(KeyFile);
}



int Load_All_Sections(void) 
{ 
 FILE* KeyFile;
 int j=0;
 char Zeile[128];
 int i;

 for (i=0; i < 128; i++ ) AllSections[i][0]=0x00;

 if ((KeyFile=fopen("Remote.ctl","r")) == NULL )  return(0);
  i=0;
  while ( fgets(Zeile,sizeof(Zeile),KeyFile) != NULL ) {

	 if (( strstr(Zeile,"*[") != NULL ) && ( strstr(Zeile,"]*") != NULL ))  {
       	strcpy(&AllSections[i][0],&Zeile[2]);
		j=strlen(AllSections[i]);
		j-=3;
		if ( j>= 0 ) AllSections[i][j]=0x00;
		i++;
	 };
  };
fclose(KeyFile);
return(i);
};


void Load_Line(LPSTR Line)
{

    int KeyNr;
	int TimeOutRate;
    int RepeatRate;
	int loop;
	char Rest[256];
//	KEY 000 0000 0000 0 XXXXXXX\n
	int j;

	j=sscanf(Line,"KEY %03d %04d %04d %1d %s\n",&KeyNr,&TimeOutRate,&RepeatRate,&loop,&Rest[0]);

	if ( (KeyNr<0) && (KeyNr >127)) return;
	if ( (loop <0) && (loop  >4 )) return;
	if (RemoteKey == NULL) return;
	if (( j == 4 ) && ( Line[0x14] == 0x20 ) && ( Line[0x15] == 0x0a )) {
        RemoteKey[KeyNr].SendKey[loop][0]=0x20;
        RemoteKey[KeyNr].SendKey[loop][1]=0x00;
    	RemoteKey[KeyNr].TimeOutRate=TimeOutRate;
	    RemoteKey[KeyNr].RepeatRate=RepeatRate;
        return; 
	};

    if ( j != 5 ) return;
	RemoteKey[KeyNr].TimeOutRate=TimeOutRate;
	RemoteKey[KeyNr].RepeatRate=RepeatRate;
    strncpy(&RemoteKey[KeyNr].SendKey[loop][0],&Rest[0],127);

};


BOOL Load_Key_Section(LPSTR SectionName)
{
 FILE* KeyFile;
 char SectionKey[128];

 char Zeile[256];
 BOOL Load=FALSE;

 if (RemoteKey == NULL) {
	RemoteKey = (struct TRemoteKey *) malloc(128*sizeof(struct TRemoteKey));
	sprintf(Zeile, "Allocating %d bytes for RemoteKey --> 0x%08x", 128*sizeof(struct TRemoteKey), RemoteKey);
	Write_Log(Zeile);
	if (RemoteKey == NULL) {
		return FALSE;
	}
 }
 memset(RemoteKey, 0, 128*sizeof(struct TRemoteKey));

 sprintf(SectionKey,"*[%s]*",SectionName);

 if ((KeyFile=fopen("Remote.ctl","r")) == NULL )  {
				 return(FALSE);
			   };


 while ( fgets(Zeile,sizeof(Zeile),KeyFile) != NULL ) {

	 if ( strstr(Zeile,SectionKey) != NULL ) {
        // Section gefunden
		 Load=TRUE;
         while ( fgets(Zeile,sizeof(Zeile),KeyFile) != NULL ) {
             if ( strstr(Zeile,"KEY") != NULL ) {
				 Load_Line(Zeile);
				 
			 } else {
				 fclose(KeyFile);
				 return(Load);
			 };
		 };


	 }


  };
fclose(KeyFile);
return(Load);
}


void Write_Key_Section(LPSTR SectionName)
{
 FILE* KeyFileIn;
 FILE* KeyFileOut;

 char Key[256];
 char LoadLine[256];
 int i;
 int j;

 if (RemoteKey == NULL) return;
 sprintf(Key,"*[%s]*",SectionName);

 CopyFile("Remote.ctl","Remote.bak",FALSE);
 remove("Remote.ctl");

 if ((KeyFileIn=fopen("Remote.bak","r")) == NULL )  {
 if ((KeyFileOut=fopen("Remote.ctl","w")) == NULL )  return;
 fprintf(KeyFileOut,"[%s]\n",SectionName);
 for ( i =0; i < 128; i++ ) {
	 for ( j=0; j<5; j++ ) {
	 if ( RemoteKey[i].SendKey[j][0] != 0x00 ) {
		 fprintf(KeyFileOut,"KEY %03d %04d %04d %1d %s\n",i,RemoteKey[i].TimeOutRate,RemoteKey[i].RepeatRate,j,RemoteKey[i].SendKey[j]);
			};
	 };
	 };
fclose(KeyFileOut);
return;
 };

 if ((KeyFileOut=fopen("Remote.ctl","w")) == NULL )  return;
 j=0;
 while ( fgets(LoadLine,sizeof(LoadLine),KeyFileIn) != NULL ) {
	 if ( strstr(LoadLine,Key) != NULL ) {
        // Section gefunden
		 j=1;
         while ((j==1) && ( fgets(LoadLine,sizeof(LoadLine),KeyFileIn) != NULL )) {
             if ( strstr(LoadLine,"KEY") == NULL ) j=0;
		 };

	 };
	 if ( j == 0 ) fprintf(KeyFileOut,"%s",LoadLine);
 };

fclose(KeyFileIn);
remove("Remote.bak");

 fprintf(KeyFileOut,"*[%s]*\n",SectionName);
 for ( i =0; i < 128; i++ ) {
	 for ( j=0; j < 5 ; j++ ) {
	 if ( RemoteKey[i].SendKey[j][0] != 0x00 ) {
    	   fprintf(KeyFileOut,"KEY %03d %04d %04d %1d %s\n",i,RemoteKey[i].TimeOutRate,RemoteKey[i].RepeatRate,j,RemoteKey[i].SendKey[j]);
			};
	    };
	 };
fclose(KeyFileOut);
return;
}

void Repeat_Thread( void ) 
{
	int i;
	int j=3;
    for (;;) {
		if ( RepeatStatus == 0 ) return;
		if ( SendCount == 0 ) return;
		SendKeyBoard(SendCount,&InputToSend[0]);

		for ( i=0; i < RepeatDelay ; i++ ) {
            Sleep(j);
			if ( RepeatStatus == 0 ) return;
		};
		j=1;
	};
};


void TimerSendKey( void )
{
	int i;
	       i=SendCount;
		   SendCount=0;
           KillTimer(hWnd, 443);
		   RemoteKey[TimerKey].SwitchPos=0;
		   TimerSend=FALSE;
	       if ( i== 0 ) return;
		   SendKeyBoard(i,&InputToSend[0]);
};

void Irq_Ir_Control( unsigned int IR_Key )
{
int Key;
BOOL Up=FALSE;
BOOL Down=FALSE;
int LoopCount;
HANDLE RepeatThread;
DWORD LinkThreadID;
HWND Focus;
char FocusName[128];
char Zeile[255];

RepeatStatus=0;

if ( IR_Key >= 0x8000 ) {
	        Key=IR_Key-0x8000;
			Down=TRUE;
			Up=FALSE;
} else {
	        Key=IR_Key;
			Down=FALSE;
			Up=TRUE;
};

if (( TimerSend == TRUE ) &&
	( Key != TimerKey )) TimerSendKey();
if ( Key > 127 ) return;

if ( REMOTE_CONFIG_WND != NULL ) {
	 SendMessage(REMOTE_CONFIG_WND,WM_USER,1,IR_Key);
	 return;
};


Running_Remote_Control_Section[0]=0x00;

if ( Down == TRUE ) {
Focus=GetForegroundWindow();
if ( Focus != NULL ) {
	GetWindowText(Focus,FocusName,128);

	if (strncmp(FocusName,"MultiDec Videotext",18) == 0 ) strcpy(Running_Remote_Control_Section,"MultiDec Videotext");
	else if (( strcmp(FocusName,"Multi-Decoder") == 0 ) && (strlen(FocusName)==13)) {
		if ( Current_Mode==0) strcpy(Running_Remote_Control_Section,"MultiDec Analog");
		else {
               strcpy(Running_Remote_Control_Section,"MultiDec Digital");
			   if (PROGRAMM_OSD == TRUE ) strcpy(Running_Remote_Control_Section,"MultiDec OSD-Normal");
			   if ( OSD_FULL_INPUT == TRUE ) strcpy(Running_Remote_Control_Section,"MultiDec OSD-Full");

		}
	};

		if ( Running_Remote_Control_Section[0]==0x00 ) strcpy(Running_Remote_Control_Section,FocusName);
}



if (strcmp(Last_Running_Remote_Control_Section,Running_Remote_Control_Section) != 0 ) {
strcpy(Last_Running_Remote_Control_Section,Running_Remote_Control_Section);
	if ( Load_Key_Section(Running_Remote_Control_Section) == FALSE ) {

	  sprintf(Zeile,"Keine Section *[%s]* in der Datei Remote.ctl gefunden !!!!\n\nKeine Fernbedienung möglich",Running_Remote_Control_Section);
      MessageBox(Focus,Zeile,"MultiDec",MB_ICONSTOP);
      return;
	}
}
// Was nun ??  //Erste Verarbeitung
if (RemoteKey == NULL) return;
if ( RemoteKey[Key].SendKey[RemoteKey[Key].SwitchPos][0]==0x00 ) return;
	   Alter_IR_Key=Key;
	   SendCount=0;  
	   if ( RemoteKey[Key].TimeOutRate == 0 ) {
		   LoopCount=RemoteKey[Key].SwitchPos;
		   RemoteKey[Key].SwitchPos++;
		   if ( RemoteKey[Key].SwitchPos > 4 ) RemoteKey[Key].SwitchPos=0;
		   while ( RemoteKey[Key].SendKey[RemoteKey[Key].SwitchPos][0] == 0x00 ) {
            RemoteKey[Key].SwitchPos++;
 		    if ( RemoteKey[Key].SwitchPos > 4 ) RemoteKey[Key].SwitchPos=0;
		   };
		   SendCount=PrepareKeyString(RemoteKey[Key].SendKey[LoopCount],&InputToSend[0]);
	   };
	   if ( RemoteKey[Key].TimeOutRate > 0 ) {
		   KillTimer(hWnd,443);
		   LoopCount=RemoteKey[Key].SwitchPos;
		   RemoteKey[Key].SwitchPos++;
		   if ( RemoteKey[Key].SwitchPos > 4 ) RemoteKey[Key].SwitchPos=0;
		   while ( RemoteKey[Key].SendKey[RemoteKey[Key].SwitchPos][0] == 0x00 ) {
            RemoteKey[Key].SwitchPos++;
 		    if ( RemoteKey[Key].SwitchPos > 4 ) RemoteKey[Key].SwitchPos=0;
		   };
		   SendCount=PrepareKeyString(RemoteKey[Key].SendKey[LoopCount],&InputToSend[0]);
           TimerSend=TRUE;
		   TimerKey=Key;
		   SetTimer(hWnd,443,RemoteKey[Key].TimeOutRate,(TIMERPROC)TimerSendKey);

	   }
};




if (( RemoteKey[Key].TimeOutRate == 0x00 ) && ( RemoteKey[Key].RepeatRate == 0x00 ) && (Up == TRUE )) {
           SendKeyBoard(SendCount,&InputToSend[0]);
		   SendCount=0;	
		   return;
};



if (( RemoteKey[Key].RepeatRate > 0 ) && (Down == TRUE )) {
	// RepeatThread starten 
	RepeatDelay=RemoteKey[Key].RepeatRate;
    RepeatStatus=1;
	RepeatThread=CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         			  (DWORD)0,                          // Same stack size.
                         			  (LPTHREAD_START_ROUTINE)Repeat_Thread,// Thread procedure.
                         			  NULL,                         // Parameter.
                         			  (DWORD)0,                          // Start immediatly.
                         			  (LPDWORD)&LinkThreadID);  
	CloseHandle(RepeatThread);


	};
if (( RemoteKey[Key].RepeatRate > 0 ) && (Up == TRUE )) {
    RepeatStatus=0;
    SendCount=0;	
};
}


void SendKeyBoard(int Anzahl,INPUT *MyInput)
{
	int ret;
    
	if ( Anzahl == 0 ) return;

	ret=SendInput(Anzahl,MyInput,sizeof(INPUT));
};


int PrepareKeyString(LPSTR Text,INPUT *MyInput)
{
   int i,j;

   unsigned char Cmd[128];
   unsigned char CmdLast[128];
   int ShiftState=0;
   int AltState=0;
   int ControlState=0;

   unsigned short VirtualCode;
   unsigned short ScanCode;

   int Offset;
   int InputCount=0;

   for ( i=0; (unsigned int)i < strlen(Text); i++ ) {

       ShiftState=0;
       AltState=0;
       ControlState=0;
	   Cmd[0]=0x00;
	   if (( Text[i] == '{' ) && ( Text[i+1] == '"')) {
        j=0;
		i+=2;
		while ((( Text[i] != '"' ) || ( Text[i+1] != '}' )) &&  ( (unsigned int)i < strlen(Text))) {
			Cmd[j++]=Text[i];
            Cmd[j] =0x00;
			i++;
	   };
	   };
		if ( strlen(Cmd) > 0 ) {

		Offset=0;

		if ( strstr(Cmd,"Alt") != NULL ) {
			AltState=1;
			Offset+=4;
		} else AltState=0;
		if ( strstr(Cmd,"Control") != NULL ) {
		    Offset+=8;
			ControlState=1;
		} else ControlState=0;
		if ( strstr(Cmd,"Shift") != NULL ) {
		    Offset+=6;
			ShiftState=1;
		} else ShiftState=0;

        j=0;
		CmdLast[j]=0x00;
		while ( (unsigned int)Offset	< strlen(Cmd)) {
			  CmdLast[j++]=Cmd[Offset++];
              CmdLast[j]=0x00;
		};

		VirtualCode=0;
		i=0;
		while (( i < 256 ) && (VirtualCode==0 ))
		{
			if (strcmp(KeyTable[i].Name,CmdLast) == 0 ) VirtualCode=i;
         i++;
		};

		if ( VirtualCode ==	0 ) {
			   VirtualCode=VkKeyScan(CmdLast[0]);
		};
               ScanCode=(VirtualCode&0xff);
		} else {
			   VirtualCode=VkKeyScan(Text[i]);
               ScanCode=(VirtualCode&0xff);
		};

		if ( ControlState == 1 ) {
			   MyInput[InputCount].type=INPUT_KEYBOARD;
			   MyInput[InputCount].ki.dwExtraInfo=(unsigned long)NULL;
               MyInput[InputCount].ki.time=0;;
			   MyInput[InputCount].ki.dwFlags=0;
			   MyInput[InputCount].ki.wVk=0x0011;
	           MyInput[InputCount].ki.wScan=0x001d;
 			   InputCount++;
               };

		if ( AltState == 1 ) {
			   MyInput[InputCount].type=INPUT_KEYBOARD;
			   MyInput[InputCount].ki.dwExtraInfo=(unsigned long)NULL;
               MyInput[InputCount].ki.time=0;;
			   MyInput[InputCount].ki.dwFlags=0;
			   MyInput[InputCount].ki.wVk=0x0012;
	           MyInput[InputCount].ki.wScan=0x0038;
 			   InputCount++;
               };
		
		      if (((VirtualCode>>8) > 0 ) || ( ShiftState==1)) {
			   // Shift in
			   MyInput[InputCount].type=INPUT_KEYBOARD;
			   MyInput[InputCount].ki.dwExtraInfo=(unsigned long)NULL;
               MyInput[InputCount].ki.time=0;;
			   MyInput[InputCount].ki.dwFlags=0;
			   MyInput[InputCount].ki.wVk=0x0010;
	           MyInput[InputCount].ki.wScan=0x002A;
 			   InputCount++;
               };

   			   MyInput[InputCount].type=INPUT_KEYBOARD;
			   MyInput[InputCount].ki.dwExtraInfo=(unsigned long)NULL;
               MyInput[InputCount].ki.time=0;;
			   MyInput[InputCount].ki.dwFlags=0;
			   MyInput[InputCount].ki.wVk=VirtualCode;
	           MyInput[InputCount].ki.wScan=ScanCode;
 			   InputCount++;

   			   MyInput[InputCount].type=INPUT_KEYBOARD;
			   MyInput[InputCount].ki.dwExtraInfo=0;
               MyInput[InputCount].ki.time=0;;
			   MyInput[InputCount].ki.dwFlags=KEYEVENTF_KEYUP;
			   MyInput[InputCount].ki.wVk=VirtualCode;
	           MyInput[InputCount].ki.wScan=ScanCode;
 			   InputCount++;

			   if (((VirtualCode>>8) > 0 ) || ( ShiftState==1)) {
			   // Shift out
			   MyInput[InputCount].type=INPUT_KEYBOARD;
			   MyInput[InputCount].ki.dwExtraInfo=0;
               MyInput[InputCount].ki.time=0;;
			   MyInput[InputCount].ki.dwFlags=KEYEVENTF_KEYUP;
			   MyInput[InputCount].ki.wVk=0x0010;
	           MyInput[InputCount].ki.wScan=0x002A;
 			   InputCount++;
               };

			   if ( AltState == 1 ) {
			   MyInput[InputCount].type=INPUT_KEYBOARD;
			   MyInput[InputCount].ki.dwExtraInfo=(unsigned long)NULL;
               MyInput[InputCount].ki.time=0;;
			   MyInput[InputCount].ki.dwFlags=KEYEVENTF_KEYUP;
			   MyInput[InputCount].ki.wVk=0x0012;
	           MyInput[InputCount].ki.wScan=0x0038;
 			   InputCount++;
               };
			   if ( ControlState == 1 ) {
			   MyInput[InputCount].type=INPUT_KEYBOARD;
			   MyInput[InputCount].ki.dwExtraInfo=(unsigned long)NULL;
               MyInput[InputCount].ki.time=0;;
			   MyInput[InputCount].ki.dwFlags=KEYEVENTF_KEYUP;
			   MyInput[InputCount].ki.wVk=0x0011;
	           MyInput[InputCount].ki.wScan=0x001d;
 			   InputCount++;
               };

               };


return(InputCount);
};


