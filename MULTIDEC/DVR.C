/*********************************************************************************
 *                                                                               *
 * dvr.h: Digital Video Recorder                                                 *
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
#include <stdio.h>
#include "resource.h"
#include <stdlib.h>        /* atoi                                  */
#include <memory.h>         
#include <io.h>         
#include <fcntl.h>
#include <commctrl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <process.h>
//#include <winsock.h>
#include "../DrvLib/windrvr.h"
#include "../SAA7146.Dev/SAAIOcom.h"
#include "DVR.h"

// MPEG Stuff
void init_av_pes(av_pes_packet *p);
void nlength_av_pes(av_pes_packet *p);
unsigned short myhtons(unsigned short val);
unsigned short myntohs(unsigned short val);
unsigned int myhtonl(unsigned int val);
unsigned int myntohl(unsigned int val);

void Init_Rec_Play_Buffer( void );
void Close_Rec_Play_Buffer( void );
void Init_PAV( void ); 
int kpes_to_av_pes(unsigned char *buf, int *StartPos, int count, p2a *p,unsigned char *Next_Pes_Buffer);
int av_pes_to_pes(unsigned char *buf,int count, a2p *p,unsigned char *MpegBuffer );

void init_a2p(a2p *p,int fd);
void kill_ps(ps_packet *p);
void setlength_ps(ps_packet *p);
void setl_ps(ps_packet *p);
int write_ps(int fd, ps_packet *p);
int w_pesh(unsigned char id,int length ,unsigned char *pts, unsigned char *obuf);
void init_p2a(p2a *p);
void init_av_pes (av_pes_packet *p);

BOOL APIENTRY RecordPlayInterrupt(HWND hDlg,UINT message,UINT wParam,LONG lParam);

unsigned int LaufZeitSekunden(unsigned int start,unsigned int now);
BOOL Resync_MPG_File(int *fd );
int Get_Button_Click(int OffsetY, int x,int y);
 

void Update_MDI_File(void);
void  Debi_Record_FastFF( void );
void  Debi_Record_SlowFF( void );
void  Debi_Record_Play_Normal( void );

void Play_Stop(void);
BOOL Debi_Record_Start(int RecordFormat, int RecordTyp );
BOOL Debi_Record_Play( void );
void Debi_Record_Stop( void );
void Debi_Record_Pause( void );
void Recorder_Thread (void *param);
BOOL Next_Record_File(int, unsigned char);
BOOL Next_Play_File(  );
BOOL Jump_Time_Play(unsigned int Seconds);

BOOL Is_Audio_Sync(unsigned char *Header);
BOOL Is_Video_Sync(unsigned char *Header);
BOOL Is_Valid_Sync(unsigned char *Header);
int Get_Length(unsigned char *Header);


void Set_Win_Buttons(int Count ,unsigned short *Bset ,HDC MemDC);


extern void Reset_Debi( void );
extern void Osd_Thread_Down( void );
extern int OSD_Edit_Ascii_Mask( unsigned char windowNr, int startx , int starty, int len, int *current, unsigned char *Buffer, unsigned short fg, unsigned short bg);
extern int DrawLine( unsigned char windownr,unsigned short x, unsigned short y, unsigned short dx, unsigned short dy, unsigned short color);
extern unsigned int RGB2YUV(unsigned short R, unsigned short G, unsigned short B);
extern int SetBlend_(unsigned char windownr,unsigned short colordepth, unsigned short index, unsigned char blending);
extern int SetColor_(unsigned char windownr, unsigned short colordepth, unsigned short index, unsigned short colorhi, unsigned short colorlo);
extern int OSDLoadBitmap4(unsigned char windownr ,BITMAPINFO *BitMap, int x,int y,int TR,int TG,int TB,int TVal ,int TimeToWait);

extern unsigned short GetDateID(unsigned short year,unsigned short month,unsigned short day);
extern unsigned short RealTime(unsigned char Stunde);
extern void Date_To_String( unsigned short Date_Id,char *DatumsString);
extern void DecodeDVBDate(unsigned short DVBDate, unsigned short *year,unsigned short *month,unsigned short *day);
extern void GetCurrentNext(unsigned short sid_id,unsigned short ts_id,int typ,unsigned char *Titel,unsigned char *StartZeit);
extern int SAA7146_OutCommand( unsigned short *buf, short length);
extern void ARM_ClearMailBox();
extern void DVB_Reset( void );
extern void SAA7146_WriteDword (SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset, DWORD data);
extern int Get_Virtual_Event(unsigned short EventId,unsigned short SID);
extern void ShutDownFilters( void );
extern int SAA7146_OutCom(int type, int com, int num, ...);
extern void SAA7146_WDebi(DWORD config, int addr, DWORD val, int count);

extern void ARM_SET_IRQ_STATE_EXT(unsigned short value);

extern int CreateOSDWindow( unsigned char windownr, unsigned short disptype, unsigned short width, unsigned short height);
extern int DestroyOSDWindow(unsigned char windownr);
extern int MoveWindowAbs( unsigned char windownr, unsigned short x, unsigned short y);
extern int SetColorBlend(unsigned char windownr);
extern void DrawBlock( unsigned char windownr,unsigned short x, unsigned short y, unsigned short dx, unsigned short dy, unsigned short color);
extern int SetFont(unsigned char  windownr, unsigned char  fontsize,unsigned short  colorfg, unsigned short  colorbg);
extern int WriteText(unsigned char win, unsigned short x, unsigned short y, unsigned char *buf);
extern void OSDSetColor(unsigned char win, unsigned short color, unsigned char  r, unsigned char  g, unsigned char  b, unsigned char  blend);
extern unsigned char *find_in_mem(const unsigned char *where, const int wherelen, const unsigned char *what, const int whatlen);
void Write_PS_Header( int fd );


extern void Save_Mode(void );
extern void Restore_Mode(void );
extern int Make_ProgNr( int Nr );
extern BOOL NT;

void OSD_Play_Stop( void );

BOOL Find_Current_EIT(unsigned short sid_id,unsigned short ts_id,int *Pos, int *Offset);

int Load_Next_Buffer(int *fd, int Typ , unsigned char *Daten);
unsigned int Get_Seconds(unsigned char *MPG_Header);
void SAA7146_AudioCom(unsigned short com);
void SAA7146_VideoCom(unsigned short com,unsigned short value);
long RealDay(unsigned char Stunde, long Day);
int Write_Buffer(unsigned char *Daten, int Laenge );
int Play_Audio_Track(int Typ);
int Play_Video_Track(int Typ);
int Write_as_PAV_File(unsigned char *MpgBuffer, int Mpg_Len );
int Load_Next_Buffer_mpg(int *fd, int Typ , unsigned char *Daten);
int Load_Next_Buffer_pav(int *fd, int Typ , unsigned char *Daten);

void LoadOSDBitmapThread(void);

ring_buf *create_ring_buf(const unsigned int size) {

  ring_buf *rb = NULL;
  char text[256];

  if (size < 2 || size > 256*1024*1024) {
     sprintf(text, "Illegal size for ring buffer : %d", size);
     Write_Log(text);
  } else {
     rb = (ring_buf *) malloc(sizeof(ring_buf)+size);
     if (rb == NULL) {
        sprintf(text, "No memory available for ring buffer of size %d", size);
        Write_Log(text);
     } else {
        rb->size = size;
        rb->buffer =
        rb->in_ptr =
        rb->out_ptr = (char*)(rb+1);
        rb->drop_count =
        rb->max_used = 0;
        InitializeCriticalSection(&rb->critical_section);
        rb->has_data_evt = CreateEvent(NULL, TRUE, FALSE, NULL);
        rb->empty_buf_evt = CreateEvent(NULL, TRUE, TRUE, NULL);
     }
  }
  return rb;
}

void drop_ring_buf(ring_buf *rb) {

    char text[256];
    if (rb == NULL) return;
    CloseHandle(rb->has_data_evt);
    CloseHandle(rb->empty_buf_evt);
    DeleteCriticalSection(&rb->critical_section);
    if (rb->drop_count > 0) {
        sprintf(text, "Ring buffer drop count = %d", rb->drop_count);
        Write_Log(text);
    }
    if (rb->max_used > 0) {
        sprintf(text, "Ring buffer max used = %d", rb->max_used);
        Write_Log(text);
    }
    free(rb);
}

void clear_ring_buf(ring_buf *rb)
{
    if (rb == NULL) return;
    EnterCriticalSection(&rb->critical_section);
    rb->in_ptr =
    rb->out_ptr = rb->buffer;
    LeaveCriticalSection(&rb->critical_section);
    ResetEvent(rb->has_data_evt);
    SetEvent(rb->empty_buf_evt);
}

unsigned int ring_buf_put(ring_buf *rb, const char *data, const unsigned int count)
{
    unsigned int ndone = 0;
    unsigned int slice, avail;

    if (rb == NULL) return 0xffffffff;
    if (count > 0) {
     EnterCriticalSection(&rb->critical_section);
     avail = (unsigned int)(rb->out_ptr > rb->in_ptr ?
                                 rb->out_ptr - rb->in_ptr - 1
                               : rb->size + rb->out_ptr - rb->in_ptr - 1);
     if (avail != 0) {
        ndone = count;
        if (ndone > avail) ndone = avail;
        slice = (unsigned int)(rb->buffer + rb->size - rb->in_ptr);
        if (ndone >= slice) { // We have to wrap
           memcpy(rb->in_ptr, data, slice); // First part
           rb->in_ptr = rb->buffer;
           if (ndone > slice) {
               memcpy(rb->in_ptr, data+slice, ndone-slice); // Second part
               rb->in_ptr += ndone-slice;
           }
        } else {
           memcpy(rb->in_ptr, data, ndone);
           rb->in_ptr += ndone;
        }
     }
     if (ndone != count) rb->drop_count += count-ndone;
     avail = (unsigned int)(rb->in_ptr < rb->out_ptr ?
                                rb->size + rb->in_ptr - rb->out_ptr
                              : rb->in_ptr - rb->out_ptr);
     if (rb->max_used < avail) rb->max_used = avail;
     LeaveCriticalSection(&rb->critical_section);
     ResetEvent(rb->empty_buf_evt);
     SetEvent(rb->has_data_evt);
    }
    return ndone;
}

unsigned int ring_buf_get(ring_buf *rb, char *data, const unsigned int count, const BOOLEAN nowait)
{
    unsigned int ndone = 0;
    unsigned int slice, avail;

    if (rb == NULL) return 0xffffffff;
    if (! nowait) WaitForSingleObject(rb->has_data_evt, INFINITE);
    if (count > 0) {
     EnterCriticalSection(&rb->critical_section);
     avail = (unsigned int)(rb->in_ptr < rb->out_ptr ?
                                 rb->size + rb->in_ptr - rb->out_ptr
                               : rb->in_ptr - rb->out_ptr);
     if (avail != 0) {
         ndone = count;
         if (ndone > avail) ndone = avail;
         slice = (unsigned int) (rb->buffer + rb->size - rb->out_ptr);
         if (ndone >= slice) { // We have to wrap
             memcpy(data, rb->out_ptr, slice); // First part
             rb->out_ptr = rb->buffer;
             if (ndone > slice) {
                 memcpy(data+slice, rb->out_ptr, ndone-slice); // Second part
                 rb->out_ptr += ndone-slice;
             }
         } else {
             memcpy(data, rb->out_ptr, ndone);
             rb->out_ptr += ndone;
         }
         if (ndone == avail) { // Buffer is now empty
             rb->in_ptr =
             rb->out_ptr = rb->buffer;
             ResetEvent(rb->has_data_evt);
             SetEvent(rb->empty_buf_evt);
         }
     }
     LeaveCriticalSection(&rb->critical_section);
  }
  return ndone;
}

unsigned int ring_buf_free(ring_buf *rb) {
    unsigned int avail;

    if (rb == NULL) return 0xffffffff;
    EnterCriticalSection(&rb->critical_section);
    avail = (unsigned int)(rb->out_ptr > rb->in_ptr ?
                                rb->out_ptr - rb->in_ptr - 1
                              : rb->size + rb->out_ptr - rb->in_ptr - 1);
    LeaveCriticalSection(&rb->critical_section);
    return avail;
}

unsigned int ring_buf_count(ring_buf *rb) {
    unsigned int avail;

    if (rb == NULL) return 0xffffffff;
    EnterCriticalSection(&rb->critical_section);
    avail = (unsigned int)(rb->in_ptr < rb->out_ptr ?
                                rb->size + rb->in_ptr - rb->out_ptr
                              : rb->in_ptr - rb->out_ptr);
    LeaveCriticalSection(&rb->critical_section);
    return avail;
}

BOOL Prozess_Header_Info(LPSTR DateiName)
{
    BOOL TypVideo = FALSE;
    BOOL TypAudio = FALSE;
    int i,j,k;
    unsigned int fl;
    char StartPfad[255];
    unsigned int TimeStampStop;
    unsigned int s1;
    unsigned __int64 FileS;
    int RecorderIndexFile;
    
    unsigned char MDIName[256];
    unsigned char Buffer[0x1000];
    
    
    Init_Rec_Play_Buffer();
    
    RecorderFile=open(DateiName, _O_RDONLY  | _O_BINARY);
    if ( RecorderFile < 0 ) return(FALSE);
    strcpy(LastRecorderFile,DateiName);
    memset(&RecorderName[0],0x00,sizeof(RecorderName));
    j=i=strlen(DateiName);
    i-=1;
    while ( DateiName[i] != '\\' ) i--;
    k=0;
    while ( k < i ) {
        StartPfad[k]=DateiName[k];
        k++;
        StartPfad[k]=0x00;
    };
    i++;
    k=0;
    while ( i < j ) {
        RecorderName[k]=DateiName[i];
        k++;
        i++;
        RecorderName[k]=0x00;
    };
    
    i=0;
    while ( i < j-4 ) {
        MDIName[i]=DateiName[i];
        i++;
        MDIName[i]=0x00;
    };
    strcat(MDIName,".mdi");
    
    
    memset(&HeaderInfo.StartZeit[0],0x00,sizeof(struct IndexHeader_t));
    for ( i=0; i < 32; i++ ) HeaderInfo.SprungMarken[i]=0xffffffff;
    
    
    RecorderIndexFile=open(MDIName, _O_RDONLY  | _O_BINARY);
    if ( RecorderIndexFile >= 0 ) {
        
        i=sizeof(struct IndexHeader_t);
        fl=_filelength(RecorderIndexFile);
        if ( (unsigned int)i < fl ) fl=i;
        i=read(RecorderIndexFile, &HeaderInfo, fl);
        close(RecorderIndexFile);
    }
    strcpy(HeaderInfo.MdiName,MDIName);
    HeaderInfo.Bytesplay=0;
    HeaderInfo.Dateigroesse=_filelengthi64(RecorderFile);
    HeaderInfo.Anzahl_Files=1;
    
    strcpy(&HeaderInfo.PlayFiles[HeaderInfo.Anzahl_Files-1][0],DateiName);
    HeaderInfo.CurrentSpielZeit=0; 
    HeaderInfo.KBTotal=(unsigned int)(HeaderInfo.Dateigroesse/1024);
    
    
    HeaderInfo.TimeStampStart=0;
    HeaderInfo.SpielZeit=0;
    
    
    i=read(RecorderFile, Buffer, 8);
    if ( i != 8 ) {
        close(RecorderFile);
        RecorderFile=-1;
        return(FALSE);
    };
    
    RecorderTyp=-1;
    if (( Buffer[0]==0x00 ) && ( Buffer[1]==0x00 ) && ( Buffer[2]==0x01 )) RecorderTyp=0;
    else if (( Buffer[0]=='A') && (Buffer[1]=='V')) RecorderTyp=1;
    
    if ( RecorderTyp == -1 ) {
        close(RecorderFile);
        RecorderFile=-1;
        return(FALSE);
    };
    
    _lseeki64(RecorderFile, (__int64)0, SEEK_SET);
    
    
    
    j=0;
    while (( j < 100 ) && (TypVideo == FALSE || TypAudio == FALSE || HeaderInfo.TimeStampStart == 0)) {
        i=Load_Next_Buffer(&RecorderFile,3,&Buffer[0]);
        if ( TypVideo == FALSE ) TypVideo=Is_Video_Sync(Buffer);
        if ( TypAudio == FALSE ) TypAudio=Is_Audio_Sync(Buffer);
        if ( HeaderInfo.TimeStampStart == 0 ) HeaderInfo.TimeStampStart=Get_Seconds(Buffer);
        j++;
    };
    sprintf(Buffer, "File %s : timestamp start = %d", HeaderInfo.PlayFiles[0], HeaderInfo.TimeStampStart);
    Write_Log(Buffer);
    
    close(RecorderFile);
    RecorderFile=-1;
    
    while ( Next_Play_File() == TRUE ) { 
        FileS=_filelengthi64(RecorderFile);
        HeaderInfo.Anzahl_Files++;
        strcpy(&HeaderInfo.PlayFiles[HeaderInfo.Anzahl_Files-1][0],LastRecorderFile);
        HeaderInfo.KBTotal=HeaderInfo.KBTotal+(unsigned int)(FileS/1024);
    }
    
    
    RecordDateiNummer=0;
    RecorderFile=open(LastRecorderFile,_O_RDONLY  | _O_BINARY);
    TimeStampStop=0;
    
    _lseeki64(RecorderFile,(__int64)-32768,SEEK_END);
    i=1;
    while ( i > 0 ) {
        i=Load_Next_Buffer(&RecorderFile, 3, Buffer);
        if (i>11) {
            s1=Get_Seconds(Buffer);
            if ( s1 != 0 )TimeStampStop=s1; 
        }
    }
    sprintf(Buffer, "File %s : timestamp stop = %d", HeaderInfo.PlayFiles[HeaderInfo.Anzahl_Files-1], TimeStampStop);
    Write_Log(Buffer);

    if ( TimeStampStop != 0 ) HeaderInfo.SpielZeit=LaufZeitSekunden(HeaderInfo.TimeStampStart,TimeStampStop);
    
    if ( RecorderFile != -1 ) {
        close(RecorderFile);
        RecorderFile=-1;
    }
    
    
    for ( i = 0; i < 32; i++ ) {
        if ( HeaderInfo.SprungMarken[i] > HeaderInfo.SpielZeit ) {
            HeaderInfo.SprungMarken[i]=0xffffffff;
        }
    }
    
    
    if (( TypVideo == FALSE ) && ( TypAudio == FALSE )) return(FALSE);
    
    if (( TypVideo == FALSE ) && ( TypAudio == TRUE )) {
        strcpy(HeaderInfo.TypeName,"Audio");
        HeaderInfo.RecordFormat=0; 
    };	 
    if (( TypVideo == TRUE ) && ( TypAudio == FALSE )) {
        strcpy(HeaderInfo.TypeName,"Video");
        HeaderInfo.RecordFormat=1; 
        
    }	 
    if (( TypVideo == TRUE ) && ( TypAudio == TRUE )) {
        strcpy(HeaderInfo.TypeName,"Audio+Video");
        HeaderInfo.RecordFormat=2; 
    }	 
    
    if ( RecorderTyp == 0 ) strcat(HeaderInfo.TypeName," MPEG"); 
    else if ( RecorderTyp == 1 ) strcat(HeaderInfo.TypeName," PVA");
    
    return(TRUE);
}

     
BOOL APIENTRY RecordPlay(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
    OPENFILENAME OpenFileName;
    CHAR         szDirName[256]   = "";
    CHAR         szFile[256]      = "\0";
    CHAR         szFileTitle[256];

    const char *szFilter;
    BOOL TypVideoPES = FALSE;
	BOOL TypAudioPES = FALSE;


    int i;
	int ret;
	int m,s;
	char Text[64];

    szFilter = t("MultiDec DVR-Formate (*.pva;*.mpg)\0*.pva;*.mpg;\0Technotrend PVA (*.pva)\0*.pva\0 MPEG-2 (*.mpg)\0*.mpg\0\0");
    switch (message) {
        case WM_INITDIALOG:
			break;

        case WM_COMMAND:

		 if (LOWORD(wParam) == IDC_BUTTON1 ) {
                    EnableWindow(GetDlgItem(hDlg,IDOK),FALSE);
                    OpenFileName.lStructSize       = sizeof(OPENFILENAME);
                    OpenFileName.hwndOwner         = hDlg;
                    OpenFileName.hInstance         = (HANDLE) hInst;
                    OpenFileName.lpstrFilter       = szFilter;
                    OpenFileName.lpstrCustomFilter = (LPSTR) NULL;
                    OpenFileName.nMaxCustFilter    = 0L;
                    OpenFileName.nFilterIndex      = 0L;
                    OpenFileName.lpstrFile         = szFile;
                    OpenFileName.nMaxFile          = sizeof(szFile);
                    OpenFileName.lpstrFileTitle    = szFileTitle;
                    OpenFileName.nMaxFileTitle     = sizeof(szFileTitle);
                    OpenFileName.lpstrInitialDir   = (LPCTSTR)RecordBaseDir[0];
                    OpenFileName.lpstrTitle        = t("DVR-Dateien abspielen");
                    OpenFileName.nFileOffset       = 0;
                    OpenFileName.nFileExtension    = 0;
                    OpenFileName.lpstrDefExt       = (LPCTSTR)"*.pva;*.mpg";
                    OpenFileName.lCustData         = 0;
                    OpenFileName.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST |
                                         OFN_HIDEREADONLY | OFN_NONETWORKBUTTON;

  
                    if (!GetOpenFileName(&OpenFileName)) return(FALSE);
 

                     if ( Prozess_Header_Info(OpenFileName.lpstrFile) == FALSE ) return(FALSE);


 sprintf(Text,"%d",HeaderInfo.KBTotal);
 SetDlgItemText(hDlg,IDC_EDIT10,Text);
 sprintf(Text,"%d",HeaderInfo.Anzahl_Files);
 SetDlgItemText(hDlg,IDC_EDIT12,Text);

 sprintf(Text,"%d",HeaderInfo.KBTotal);
 SetDlgItemText(hDlg,IDC_EDIT10,Text);
 sprintf(Text,"%d",HeaderInfo.Anzahl_Files);
 SetDlgItemText(hDlg,IDC_EDIT12,Text);
 SetDlgItemText(hDlg,IDC_EDIT9,HeaderInfo.TypeName);
 SetDlgItemText(hDlg,IDC_EDIT1,OpenFileName.lpstrFile);
 SetDlgItemText(hDlg,IDC_EDIT2,HeaderInfo.ProgrammName);
 SetDlgItemText(hDlg,IDC_EDIT3,HeaderInfo.AufnahmeDatum);
 SetDlgItemText(hDlg,IDC_EDIT4,HeaderInfo.StartZeit);

	 ret=(HeaderInfo.Dauer_h*60)+HeaderInfo.Dauer_m;
	 sprintf(Text,t("%d Minuten"),ret);
	 SetDlgItemText(hDlg,IDC_EDIT5,Text);
	 SetDlgItemText(hDlg,IDC_EDIT6,"");
	 
	 m=(HeaderInfo.SpielZeit/60);
	 s=HeaderInfo.SpielZeit-(m*60);
	 sprintf(Text,t("%3d Min %02d Sek"),m,s);
	 SetDlgItemText(hDlg,IDC_EDIT6,Text);
	 sprintf(Text,"%d",HeaderInfo.BitRate);
	 SetDlgItemText(hDlg,IDC_EDIT11,Text);
	 sprintf(Text,"%d",HeaderInfo.SampleFreq);
	 SetDlgItemText(hDlg,IDC_EDIT8,Text);
	 SetDlgItemText(hDlg,IDC_EDIT7,HeaderInfo.Infos);
	 EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);


};		 

		 
		 
		 if (LOWORD(wParam) == IDOK ) {
			strcpy(Text,HeaderInfo.PlayFiles[0]);
			i=strlen(Text);
			while ( i >0 ) {
				if ( Text[i] == '\\' ) { 
					Text[i]=0x00;
					i=0;
				};
				i--;
			};

			strcpy(RecordBaseDir[8],Text);
			SetCurrentDirectory(CurrentDir);
			 Play=TRUE;
			 // Ox --> Play
			 EndDialog(hDlg,TRUE);

		 }
		 if (LOWORD(wParam) == IDCANCEL ) {
			  SetCurrentDirectory(CurrentDir);
			 EndDialog(hDlg,TRUE);
		 }



		 break;
    }
    return (FALSE);
        UNREFERENCED_PARAMETER(lParam);
}




BOOL APIENTRY PlayInfoControl(HWND hDlg,UINT message,UINT wParam,LONG lParam)
{
char Text[128];
unsigned int m,s;

    switch (message) {
    case WM_INITDIALOG:
     SetDlgItemText(hDlg,IDC_EDIT9,HeaderInfo.TypeName);
	 SetDlgItemText(hDlg,IDC_EDIT2,HeaderInfo.ProgrammName);
	 SetDlgItemText(hDlg,IDC_EDIT3,HeaderInfo.AufnahmeDatum);
	 SetDlgItemText(hDlg,IDC_EDIT4,HeaderInfo.StartZeit);
	 m=(HeaderInfo.Dauer_h*60)+HeaderInfo.Dauer_m;
	 sprintf(Text,t("%d Minuten"),m);
	 SetDlgItemText(hDlg,IDC_EDIT5,Text);
     sprintf(Text,"%d",HeaderInfo.KBTotal);
     SetDlgItemText(hDlg,IDC_EDIT10,Text);
     sprintf(Text,"%d",HeaderInfo.Anzahl_Files);
     SetDlgItemText(hDlg,IDC_EDIT12,Text);

	 m=(HeaderInfo.SpielZeit/60);
	 s=HeaderInfo.SpielZeit-(m*60);
	 sprintf(Text,t("%3d Min %02d Sek"),m,s);
	 SetDlgItemText(hDlg,IDC_EDIT6,Text);

	 SetDlgItemText(hDlg,IDC_EDIT7,HeaderInfo.Infos);
	 EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);
     SetTimer(hDlg,987,1000,NULL);
	 if ( Record == TRUE ) {
	            ShowWindow(GetDlgItem(hDlg,IDC_EDIT8),SW_HIDE);
				ShowWindow(GetDlgItem(hDlg,TEXT3),SW_HIDE);
	 }
     
	case WM_TIMER:
		if ( wParam == 987 ) {
				 if ( Play == TRUE ) {
		    	m=(HeaderInfo.CurrentSpielZeit/60);
			    s=HeaderInfo.CurrentSpielZeit-(m*60);
			    sprintf(Text,t("%3d Min %02d Sek"),m,s);
        	    SetDlgItemText(hDlg,IDC_EDIT8,Text);
				 }
				 if ( Record == TRUE ) {
					m=(HeaderInfo.CurrentSpielZeit/60);
					s=HeaderInfo.CurrentSpielZeit-(m*60);
        		    sprintf(Text,t("%3d Min %02d Sek"),m,s);
               	    SetDlgItemText(hDlg,IDC_EDIT6,Text);
	                sprintf(Text,"%d",HeaderInfo.Anzahl_Files);
                    SetDlgItemText(hDlg,IDC_EDIT12,Text);
			        sprintf(Text,"%d",HeaderInfo.KBTotal);
					SetDlgItemText(hDlg,IDC_EDIT10,Text);
				 }
		};
		break;
		 
	case WM_COMMAND:	 
		 if (LOWORD(wParam) == IDOK ) {
			 if ( InfoRet == 1 ) PostMessage(hWnd,WM_COMMAND, IDM_RECORD_PLAY,0);
			 if ( InfoRet == 2 ) PostMessage(hWnd,WM_COMMAND, IDM_PLAY_TIME_CONTROL,0);
			 if ( InfoRet == 3 ) PostMessage(hWnd,WM_COMMAND, IDM_PLAY_MARK_CONTROL,0);
			 if ( InfoRet == 4 ) PostMessage(hWnd,WM_COMMAND, IDM_RECORD_START,0);
			 EndDialog(hDlg,TRUE);

		 }
		 if (LOWORD(wParam) == IDCANCEL ) {
			 EndDialog(hDlg,TRUE);
		 }

		 break;
    }
    return (FALSE);
        UNREFERENCED_PARAMETER(lParam);
}



void Del_Sprungmarke(int Pos)
{
    int i,j;
	BOOL Done=FALSE;
    unsigned int Save;


	HeaderInfo.SprungMarken[Pos] = 0xffffffff;
	j=31;
	while ( Done == FALSE ) {
		Done=TRUE;
		for ( i=0; i < j ; i++ ) {
			if ( HeaderInfo.SprungMarken[i] > HeaderInfo.SprungMarken[i+1] ) {
				 Done=FALSE;
				 Save=HeaderInfo.SprungMarken[i];
				 HeaderInfo.SprungMarken[i]=HeaderInfo.SprungMarken[i+1];
				 HeaderInfo.SprungMarken[i+1]=Save;
			};
        };
		j--;
	};
};





BOOL Insert_Sprungmarke(unsigned int Sec)
{
	int i,j;
    unsigned int Save;
	BOOL Done=FALSE;

    for ( i=0; i < 32 ; i++ ) {
		 if ( HeaderInfo.SprungMarken[i] == Sec ) return(TRUE);
		};

	i=0;
	while (( HeaderInfo.SprungMarken[i] != 0xffffffff ) & ( i < 32 )) i++;
    if ( i >= 32 ) return(FALSE);
    HeaderInfo.SprungMarken[i] = Sec;

	j=31;
	while ( Done == FALSE ) {
		Done=TRUE;
		for ( i=0; i < j ; i++ ) {
			if ( HeaderInfo.SprungMarken[i] > HeaderInfo.SprungMarken[i+1] ) {
				 Done=FALSE;
				 Save=HeaderInfo.SprungMarken[i];
				 HeaderInfo.SprungMarken[i]=HeaderInfo.SprungMarken[i+1];
				 HeaderInfo.SprungMarken[i+1]=Save;
			};
        };
		j--;
	};
return(TRUE);
};

void Jump_Mark(int Nummer )
{
unsigned int TimeStamp;
int i; 
int Before=-1;
int After=-1;
TimeStamp=HeaderInfo.CurrentSpielZeit;
if ( Nummer < 0 ) {
	if ( TimeStamp > 4 ) TimeStamp-=4;
} else {
TimeStamp++;
}

i=0;
while (( HeaderInfo.SprungMarken[i] != 0xffffffff ) && ( i < 32)) {
if ( HeaderInfo.SprungMarken[i] <= TimeStamp ) Before = i;
if (( HeaderInfo.SprungMarken[i] > TimeStamp ) && ( After == -1 )) {
	After = i;
	i=32;
};
i++;
};


if ( Nummer < 0 ) {
    Nummer=Nummer*-1;
	Nummer--;
	if (( Before-Nummer >= 0 ) &&
	    ( HeaderInfo.SprungMarken[Before-Nummer] != 0xffffffff )) Jump_Time_Play(HeaderInfo.SprungMarken[Before-Nummer]);
     return;
};

if ( Nummer > 0 ) {
	Nummer--;
	if (( After+Nummer >= 0 ) &&
	    ( HeaderInfo.SprungMarken[After+Nummer] != 0xffffffff )) Jump_Time_Play(HeaderInfo.SprungMarken[After+Nummer]);
     return;
};
};


BOOL Check_Mark(int Nummer )
{
unsigned int TimeStamp;
int i; 
int Before=-1;
int After=-1;

TimeStamp=HeaderInfo.CurrentSpielZeit;
if ( TimeStamp > 2 ) TimeStamp-=2;

i=0;
while (( HeaderInfo.SprungMarken[i] != 0xffffffff ) && ( i < 32)) {
if ( HeaderInfo.SprungMarken[i] <= TimeStamp ) Before = i;
if (( HeaderInfo.SprungMarken[i] > TimeStamp ) && ( After == -1 )) {
	After = i;
	i=32;
};
i++;
};

if ( Nummer < 0 ) {
    Nummer=Nummer*-1;
	Nummer--;
	if (( Before-Nummer >= 0 ) &&
	    ( HeaderInfo.SprungMarken[Before-Nummer] != 0xffffffff )) return(TRUE);
     return(FALSE);
};

if ( Nummer > 0 ) {
	Nummer--;
	if (( After+Nummer >= 0 ) &&
	    ( HeaderInfo.SprungMarken[After+Nummer] != 0xffffffff )) return(TRUE);
     return(FALSE);
};
return(FALSE);
};


  

BOOL APIENTRY PlayControlDlg(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
    PAINTSTRUCT wps;                   /* paint structure           */
    HDC hdc;
    HDC             hMemDC;
    HBITMAP         hOldBm;
	BITMAP bm;

	static HBITMAP Control;
    static BITMAPINFO          *TESTBITMAP                    = NULL;

	char Text[128];
	int m,s;
    int xPos;
	int yPos;
    int i;
    static int Timer;
    static RECT UpdateRect;

	int XSize;
    static double XStep; 
    static RECT Balken;
	HPEN PenRot,OldPen;
    HBRUSH hBrush;

switch (message) {
        case WM_INITDIALOG:
			SetTimer(hDlg,657,100,NULL);
            Control =LoadBitmap(hInst,"PLAYBITMAP");
            TESTBITMAP=(BITMAPINFO*)LockResource(Control);
            Timer=10;
			UpdateRect.top=18;
			UpdateRect.left=0;
			UpdateRect.right=586;
			UpdateRect.bottom=80;
			Balken.top=59;
			Balken.bottom=75;
			Balken.left=1;


			
        case WM_TIMER:
			if ( wParam == 657 ) {

			  if ( ButtonSetPlay[3]==11 ) {
                           ButtonSetPlay[3]=10;
						   InvalidateRect(hDlg,&UpdateRect,FALSE);
					  };

			    Timer++;
				if ( Timer >= 10 ) {
					  Timer=0;  
					if ( Play == TRUE ) {
					   i=0;
					   while (( i < 64 ) && ( HeaderInfo.Infos[i] >= 0x20 )) {
					   Text[i]=HeaderInfo.Infos[i];
					   i++;
					   Text[i]=0x00;
					   };
					   if ( i == 0 ) strcpy(Text,t("<Unbekannt>"));
                       SetDlgItemText(hDlg,TEXT2,Text);
				       m=(HeaderInfo.CurrentSpielZeit/60);
					   s=HeaderInfo.CurrentSpielZeit-(m*60);
					   sprintf(Text,"%3d:%02d",m,s);
					   SetDlgItemText(hDlg,TEXT1,Text);
   					   XStep=(double)585/(double)HeaderInfo.SpielZeit;
					   hBrush=CreateSolidBrush(RGB(255,255,0));
					   hdc = GetDC(hDlg);
					   Balken.right=(int)((double)XStep*(double)HeaderInfo.CurrentSpielZeit);
                       FillRect (hdc, &Balken, hBrush);
				       PenRot=CreatePen(PS_SOLID,2,RGB(255, 0, 0));                           /* color */
					   OldPen=SelectObject(hdc,PenRot);
					  for ( i=0; i	< 32 ; i++ ) {
						  if ( HeaderInfo.SprungMarken[i] != 0xffffffff) {
						      XSize=(int)((double)HeaderInfo.SprungMarken[i]*(double)XStep);
								MoveToEx(hdc, XSize,Balken.bottom+3, NULL );
								LineTo(hdc, XSize,Balken.top-3);
						  };
                      };
					  SelectObject(hdc,OldPen);
        			  DeleteObject(PenRot);
                      ReleaseDC(hDlg, hdc);
        			  DeleteObject(hBrush);
					
					} else {
						strcpy(Text,t("<Keine Datei geladen>"));
                        SetDlgItemText(hDlg,TEXT2,Text);
					    SetDlgItemText(hDlg,TEXT1,"");
 
				};
			};
			};
            return(TRUE);

		case WM_PAINT:
	        hdc = BeginPaint(hDlg,&wps);
			hMemDC = CreateCompatibleDC(hdc);
			hOldBm=SelectObject(hMemDC, Control );
			Set_Win_Buttons(2,&ButtonSetPlay[0],hMemDC);
			GetObject(Control, sizeof(BITMAP), (LPSTR)&bm);
			BitBlt(hdc, 0, 18, bm.bmWidth, bm.bmHeight,hMemDC, 0, 0, SRCCOPY);// Signal
			SelectObject(hMemDC,hOldBm);
			DeleteDC(hMemDC);
			DeleteDC(hdc);
            EndPaint(hDlg,&wps);
            Timer=10;
			return(FALSE);

   case WM_LBUTTONUP:
       xPos = LOWORD(lParam);  // horizontal position of cursor
       yPos = HIWORD(lParam);  // vertical position of cursor

	   i=Get_Button_Click(18,xPos,yPos);
	   if ( i == 0 ) {
			 if ( ButtonSetPlay[i] < 10 ) return(TRUE);
			 if ( Play == FALSE ) {
				          DialogBox(hInst,"RECORDPLAY",hDlg,RecordPlay);
						  if ( Play == TRUE ) Debi_Record_Play();
			} else Debi_Record_Play_Normal();
			   InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 1 ) {
			 if ( ButtonSetPlay[i] < 10 ) return(TRUE);
				 Debi_Record_Pause();
			     InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 2 ) {
			 if ( ButtonSetPlay[i] < 10 ) return(TRUE);
				 Play_Stop();
			     InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 3 ) {
   			     if ( ButtonSetPlay[i] < 10 ) return(TRUE);
				 Insert_Sprungmarke(HeaderInfo.CurrentSpielZeit);
				 Update_MDI_File();
                 ButtonSetPlay[3]=11;
			     InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 4 ) {
   			     if ( ButtonSetPlay[i] < 10 ) return(TRUE);
				 Debi_Record_SlowFF();
			     InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 5 ) {
   			     if ( ButtonSetPlay[i] < 10 ) return(TRUE);
				 Debi_Record_FastFF();
			     InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 6 ) {
   			     if ( ButtonSetPlay[i] < 10 ) return(TRUE);
        		 KillTimer(hDlg,657);
				 PostMessage(hWnd,WM_COMMAND,IDM_PLAY_TIME_CONTROL,0);
    			 EndDialog(hDlg,TRUE);
	   } else if ( i == 7 ) {
   			     if ( ButtonSetPlay[i] < 10 ) return(TRUE);
        		 KillTimer(hDlg,657);
				 PostMessage(hWnd,WM_COMMAND,IDM_PLAY_MARK_CONTROL,0);
    			 EndDialog(hDlg,TRUE);
	   } else if ( i == 8 ) {
   			     if ( ButtonSetPlay[i] < 10 ) return(TRUE);
        		 KillTimer(hDlg,657);
			     InfoRet=1; 
				 PostMessage(hWnd,WM_COMMAND,IDM_PLAY_INFO_CONTROL,0);
    			 EndDialog(hDlg,TRUE);
	   } else if ( i == 9 ) {
	     		 KillTimer(hDlg,657);
		         EndDialog(hDlg,TRUE);
	   };

	break;	
   case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL ) {
    		 KillTimer(hDlg,657);
			 EndDialog(hDlg,TRUE);
             return(TRUE);
		 }


	   }



		 return(FALSE);
};


BOOL APIENTRY PlayTimeControl(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
    PAINTSTRUCT wps;                   /* paint structure           */
    HDC hdc;
    HDC             hMemDC;
    HBITMAP         hOldBm;
	BITMAP bm;

	static HBITMAP Control;
    static BITMAPINFO          *TESTBITMAP                    = NULL;

	unsigned int TimeStamp;
	char Text[128];
	int m,s;
    int xPos;
	int yPos;
    int i;
    static int Timer;
    static RECT UpdateRect;

	static unsigned short ButtonSetTime[10] = { 1,1,1,1,1,1,10,10,10 };
	unsigned short AltButtonSetTime[10] = { 1,1,1,1,1,1,10,10,10 };

	int XSize;
    static double XStep; 
    static RECT Balken;
	HPEN PenRot,OldPen;
    HBRUSH hBrush;

switch (message) {
        case WM_INITDIALOG:
			SetDlgItemText(hDlg,IDC_EDIT3,"0");
			SetDlgItemText(hDlg,IDC_EDIT4,"0");
			SetTimer(hDlg,657,100,NULL);
            Control =LoadBitmap(hInst,"TIMEBITMAP");
            TESTBITMAP=(BITMAPINFO*)LockResource(Control);
            Timer=10;
			UpdateRect.top=48;
			UpdateRect.left=0;
			UpdateRect.right=587;
			UpdateRect.bottom=110;
			Balken.top=89;
			Balken.bottom=105;
			Balken.left=1;


			
        case WM_TIMER:
			if ( wParam == 657 ) {
			    Timer++;
				if ( Timer >= 10 ) {
					  Timer=0;  
					  
		        if ( HeaderInfo.CurrentSpielZeit >= 600 ) AltButtonSetTime[0]=10; 
				  else AltButtonSetTime[0]=1;
			    if ( HeaderInfo.CurrentSpielZeit >= 300 ) AltButtonSetTime[1]=10; 
				  else AltButtonSetTime[1]=1;
				if ( HeaderInfo.CurrentSpielZeit >= 180 ) AltButtonSetTime[2]=10; 
				  else AltButtonSetTime[2]=1;
				if ( HeaderInfo.CurrentSpielZeit+180 < HeaderInfo.SpielZeit) AltButtonSetTime[3]=10;
				  else AltButtonSetTime[3]=1;
				if ( HeaderInfo.CurrentSpielZeit+300 < HeaderInfo.SpielZeit) AltButtonSetTime[4]=10;
				  else AltButtonSetTime[4]=1;
				if ( HeaderInfo.CurrentSpielZeit+600 < HeaderInfo.SpielZeit) AltButtonSetTime[5]=10;
				  else AltButtonSetTime[5]=1;

				  if ( memcmp(&AltButtonSetTime[0],&ButtonSetTime[0],12) != 0 ) {
              			     InvalidateRect(hDlg,&UpdateRect,FALSE);
				  }
					  
					   i=0;
					   while (( i < 64 ) && ( HeaderInfo.Infos[i] >= 0x20 )) {
					   Text[i]=HeaderInfo.Infos[i];
					   i++;
					   Text[i]=0x00;
					   };
					   if ( i == 0 ) strcpy(Text,t("<Unbekannt>"));
                       SetDlgItemText(hDlg,TEXT2,Text);
				       m=(HeaderInfo.CurrentSpielZeit/60);
					   s=HeaderInfo.CurrentSpielZeit-(m*60);
					   sprintf(Text,"%3d:%02d",m,s);
					   SetDlgItemText(hDlg,TEXT1,Text);
   					   XStep=(double)585/(double)HeaderInfo.SpielZeit;
					   hBrush=CreateSolidBrush(RGB(255,255,0));
					   hdc = GetDC(hDlg);
					   Balken.right=(int)((double)XStep*(double)HeaderInfo.CurrentSpielZeit);
                       FillRect (hdc, &Balken, hBrush);
				       PenRot=CreatePen(PS_SOLID,2,RGB(255, 0, 0));                           /* color */
					   OldPen=SelectObject(hdc,PenRot);
					  for ( i=0; i	< 32 ; i++ ) {
						  if ( HeaderInfo.SprungMarken[i] != 0xffffffff) {
						      XSize=(int)((double)HeaderInfo.SprungMarken[i]*(double)XStep);
								MoveToEx(hdc, XSize,Balken.bottom+3, NULL );
								LineTo(hdc, XSize,Balken.top-3);
						  };
                      };
					  SelectObject(hdc,OldPen);
        			  DeleteObject(PenRot);
                      ReleaseDC(hDlg, hdc);
        			  DeleteObject(hBrush);
			};
			};
            return(TRUE);

		case WM_PAINT:
	        hdc = BeginPaint(hDlg,&wps);
			hMemDC = CreateCompatibleDC(hdc);
			hOldBm=SelectObject(hMemDC, Control );

		        if ( HeaderInfo.CurrentSpielZeit >= 600 ) ButtonSetTime[0]=10; 
				  else ButtonSetTime[0]=1;
			    if ( HeaderInfo.CurrentSpielZeit >= 300 ) ButtonSetTime[1]=10; 
				  else ButtonSetTime[1]=1;
				if ( HeaderInfo.CurrentSpielZeit >= 180 ) ButtonSetTime[2]=10; 
				  else ButtonSetTime[2]=1;
				if ( HeaderInfo.CurrentSpielZeit+180 < HeaderInfo.SpielZeit) ButtonSetTime[3]=10;
				  else ButtonSetTime[3]=1;
				if ( HeaderInfo.CurrentSpielZeit+300 < HeaderInfo.SpielZeit) ButtonSetTime[4]=10;
				  else ButtonSetTime[4]=1;
				if ( HeaderInfo.CurrentSpielZeit+600 < HeaderInfo.SpielZeit) ButtonSetTime[5]=10;
				  else ButtonSetTime[5]=1;
			
			
			Set_Win_Buttons(2,&ButtonSetTime[0],hMemDC);
			GetObject(Control, sizeof(BITMAP), (LPSTR)&bm);
			BitBlt(hdc, 0, 48, bm.bmWidth, bm.bmHeight,hMemDC, 0, 0, SRCCOPY);// Signal
			SelectObject(hMemDC,hOldBm);
			DeleteDC(hMemDC);
			DeleteDC(hdc);
            EndPaint(hDlg,&wps);
            Timer=10;
			return(FALSE);

   case WM_LBUTTONUP:
       xPos = LOWORD(lParam);  // horizontal position of cursor
       yPos = HIWORD(lParam);  // vertical position of cursor


	   i=Get_Button_Click(48,xPos,yPos);
	   if ( i == 0 ) {
			 if ( ButtonSetTime[i] < 10 ) return(TRUE);
			   Jump_Time_Play(HeaderInfo.CurrentSpielZeit-600);
			   InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 1 ) {
			 if ( ButtonSetTime[i] < 10 ) return(TRUE);
                Jump_Time_Play(HeaderInfo.CurrentSpielZeit-300);
				InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 2 ) {
			 if ( ButtonSetTime[i] < 10 ) return(TRUE);
              Jump_Time_Play(HeaderInfo.CurrentSpielZeit-180);
			  InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 3 ) {
   			     if ( ButtonSetTime[i] < 10 ) return(TRUE);
                 Jump_Time_Play(HeaderInfo.CurrentSpielZeit+180);
				 InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 4 ) {
   			     if ( ButtonSetTime[i] < 10 ) return(TRUE);
                 Jump_Time_Play(HeaderInfo.CurrentSpielZeit+300);
				 InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 5 ) {
   			     if ( ButtonSetTime[i] < 10 ) return(TRUE);
                 Jump_Time_Play(HeaderInfo.CurrentSpielZeit+600);
				 InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 6 ) {
   			     if ( ButtonSetTime[i] < 10 ) return(TRUE);
        		 KillTimer(hDlg,657);
				 PostMessage(hWnd,WM_COMMAND,IDM_RECORD_PLAY,0);
    			 EndDialog(hDlg,TRUE);
	   } else if ( i == 7 ) {
   			     if ( ButtonSetTime[i] < 10 ) return(TRUE);
        		 KillTimer(hDlg,657);
				 PostMessage(hWnd,WM_COMMAND,IDM_PLAY_MARK_CONTROL,0);
    			 EndDialog(hDlg,TRUE);
	   } else if ( i == 8 ) {
   			     if ( ButtonSetTime[i] < 10 ) return(TRUE);
        		 KillTimer(hDlg,657);
			     InfoRet=2; 
				 PostMessage(hWnd,WM_COMMAND,IDM_PLAY_INFO_CONTROL,0);
    			 EndDialog(hDlg,TRUE);
	   } else if ( i == 9 ) {
	     		 KillTimer(hDlg,657);
		         EndDialog(hDlg,TRUE);
	   };

	break;	
   case WM_COMMAND:

		 if (LOWORD(wParam) == IDJUMP ) {
			    m=GetDlgItemInt(hDlg,IDC_EDIT3,NULL,FALSE);
			    s=GetDlgItemInt(hDlg,IDC_EDIT4,NULL,FALSE);
			    TimeStamp=m*60+s;
				
				if ( TimeStamp < HeaderInfo.SpielZeit ) {
					  Jump_Time_Play(TimeStamp);
        				InvalidateRect(hDlg,&UpdateRect,FALSE);
				} else {
					   MessageBox(hDlg,t("Zeit größer Endzeit"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
				};
				return(TRUE);
		 }

		if (LOWORD(wParam) == IDCANCEL ) {
    		 KillTimer(hDlg,657);
			 EndDialog(hDlg,TRUE);
             return(TRUE);
		 }


	   }



		 return(FALSE);
};


BOOL APIENTRY PlayMarkControl(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
    PAINTSTRUCT wps;                   /* paint structure           */
    HDC hdc;
    HDC             hMemDC;
    HBITMAP         hOldBm;
	BITMAP bm;

	static HBITMAP Control;
    static BITMAPINFO          *TESTBITMAP                    = NULL;

	unsigned int TimeStamp;
	char Text[128];
	int m,s;
    int xPos;
	int yPos;
    int i,j;
    static int Timer;
    static RECT UpdateRect;

	static unsigned short ButtonSetMark[10] = { 1,1,1,1,1,1,10,10,10,10 };
	unsigned short AltButtonSetMark[10] = { 1,1,1,1,1,1,10,10,10,10 };

	int XSize;
    static double XStep; 
    static RECT Balken;
	HPEN PenRot,OldPen;
    HBRUSH hBrush;

switch (message) {
        case WM_INITDIALOG:
			SetDlgItemText(hDlg,IDC_EDIT1,"0");
			SetDlgItemText(hDlg,IDC_EDIT2,"0");
            SendMessage (GetDlgItem(hDlg,IDC_LIST1), LB_RESETCONTENT , 0, 0 );
			for ( i = 0 ; i < 32 ; i++ ) {
						  if ( HeaderInfo.SprungMarken[i] != 0xffffffff) {
						  m=(HeaderInfo.SprungMarken[i]/60);
						  s=HeaderInfo.SprungMarken[i]-(m*60);
						  sprintf(Text,t("%3d Min %02d Sek"),m,s);
						  SendMessage(GetDlgItem(hDlg,IDC_LIST1),LB_INSERTSTRING,i,(LPARAM)Text);
                          }; 
					}

			
			SetTimer(hDlg,657,100,NULL);
            Control =LoadBitmap(hInst,"MARKBITMAP");
            TESTBITMAP=(BITMAPINFO*)LockResource(Control);
            Timer=10;
			UpdateRect.top=90;
			UpdateRect.left=0;
			UpdateRect.right=587;
			UpdateRect.bottom=135;
			Balken.top=131;
			Balken.bottom=147;
			Balken.left=1;
			
        case WM_TIMER:
			if ( wParam == 657 ) {
			    Timer++;
			  if ( ButtonSetMark[7]==11 ) {
                           ButtonSetMark[7]=10;
						   InvalidateRect(hDlg,&UpdateRect,FALSE);
					  };
				if ( Timer >= 10 ) {
					  Timer=0;  

					 if ( HeaderInfo.SprungMarken[0] != 0xffffffff ) {
                        AltButtonSetMark[0]=10;
						AltButtonSetMark[5]=10;

						if ( Check_Mark(-1)== TRUE ) {
                           AltButtonSetMark[2]=10;
          				   if ( Check_Mark(-2) == TRUE ) AltButtonSetMark[1]=10;
		                     else AltButtonSetMark[1]=1;
						} else {
                           AltButtonSetMark[1]=1;
                           AltButtonSetMark[2]=1;
						};
						if ( Check_Mark(1) == TRUE ) {
                           AltButtonSetMark[3]=10;
          				   if ( Check_Mark(2) == TRUE ) AltButtonSetMark[4]=10;
		                     else AltButtonSetMark[4]=1;
						} else {
                           AltButtonSetMark[3]=1;
                           AltButtonSetMark[4]=1;
						}
				 } else { 
                        AltButtonSetMark[0]=1;
						AltButtonSetMark[1]=1;
                        AltButtonSetMark[2]=1;
						AltButtonSetMark[3]=1;
                        AltButtonSetMark[4]=1;
                        AltButtonSetMark[5]=1;
				 }
					  

				  if ( memcmp(&AltButtonSetMark[0],&ButtonSetMark[0],20) != 0 ) {
              			     InvalidateRect(hDlg,&UpdateRect,FALSE);
				  }
					  
					   i=0;
					   while (( i < 64 ) && ( HeaderInfo.Infos[i] >= 0x20 )) {
					   Text[i]=HeaderInfo.Infos[i];
					   i++;
					   Text[i]=0x00;
					   };
					   if ( i == 0 ) strcpy(Text,t("<Unbekannt>"));
                       SetDlgItemText(hDlg,TEXT2,Text);
				       m=(HeaderInfo.CurrentSpielZeit/60);
					   s=HeaderInfo.CurrentSpielZeit-(m*60);
					   sprintf(Text,"%3d:%02d",m,s);
					   SetDlgItemText(hDlg,TEXT1,Text);
					   sprintf(Text,t("%3d Min %02d Sek"),m,s);
					   SetDlgItemText(hDlg,IDC_EDIT9,Text);

					   XStep=(double)585/(double)HeaderInfo.SpielZeit;
					   hBrush=CreateSolidBrush(RGB(255,255,0));
					   hdc = GetDC(hDlg);
					   Balken.right=(int)((double)XStep*(double)HeaderInfo.CurrentSpielZeit);
                       FillRect (hdc, &Balken, hBrush);
				       PenRot=CreatePen(PS_SOLID,2,RGB(255, 0, 0));                           /* color */
					   OldPen=SelectObject(hdc,PenRot);
					  for ( i=0; i	< 32 ; i++ ) {
						  if ( HeaderInfo.SprungMarken[i] != 0xffffffff) {
						      XSize=(int)((double)HeaderInfo.SprungMarken[i]*(double)XStep);
								MoveToEx(hdc, XSize,Balken.bottom+3, NULL );
								LineTo(hdc, XSize,Balken.top-3);
						  };
                      };
					  SelectObject(hdc,OldPen);
        			  DeleteObject(PenRot);
                      ReleaseDC(hDlg, hdc);
        			  DeleteObject(hBrush);
			};
			};
            return(TRUE);

		case WM_PAINT:
	        hdc = BeginPaint(hDlg,&wps);
			hMemDC = CreateCompatibleDC(hdc);
			hOldBm=SelectObject(hMemDC, Control );

					 if ( HeaderInfo.SprungMarken[0] != 0xffffffff ) {
                        ButtonSetMark[0]=10;
						ButtonSetMark[5]=10;

						if ( Check_Mark(-1)== TRUE ) {
                           ButtonSetMark[2]=10;
          				   if ( Check_Mark(-2) == TRUE ) ButtonSetMark[1]=10;
		                     else ButtonSetMark[1]=1;
						} else {
                           ButtonSetMark[1]=1;
                           ButtonSetMark[2]=1;
						};
						if ( Check_Mark(1) == TRUE ) {
                           ButtonSetMark[3]=10;
          				   if ( Check_Mark(2) == TRUE ) ButtonSetMark[4]=10;
		                     else ButtonSetMark[4]=1;
						} else {
                           ButtonSetMark[3]=1;
                           ButtonSetMark[4]=1;
						}
				 } else { 
                        ButtonSetMark[0]=1;
						ButtonSetMark[1]=1;
                        ButtonSetMark[2]=1;
						ButtonSetMark[3]=1;
                        ButtonSetMark[4]=1;
                        ButtonSetMark[5]=1;
				 }
			

			
			Set_Win_Buttons(2,&ButtonSetMark[0],hMemDC);
			GetObject(Control, sizeof(BITMAP), (LPSTR)&bm);
			BitBlt(hdc, 0, 90 , bm.bmWidth, bm.bmHeight,hMemDC, 0, 0, SRCCOPY);// Signal
			SelectObject(hMemDC,hOldBm);
			DeleteDC(hMemDC);
			DeleteDC(hdc);
            EndPaint(hDlg,&wps);
            Timer=10;
			return(FALSE);

   case WM_LBUTTONUP:
       xPos = LOWORD(lParam);  // horizontal position of cursor
       yPos = HIWORD(lParam);  // vertical position of cursor


	   i=Get_Button_Click(90,xPos,yPos);
   
	   if ( i == 0 ) {
			 if ( ButtonSetMark[i] < 10 ) return(TRUE);
				 Jump_Time_Play(HeaderInfo.SprungMarken[0]);
  			     InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 1 ) {
			 if ( ButtonSetMark[i] < 10 ) return(TRUE);
                 Jump_Mark(-2);
			     InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 2 ) {
			 if ( ButtonSetMark[i] < 10 ) return(TRUE);
                 Jump_Mark(-1);
			     InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 3 ) {
   			     if ( ButtonSetMark[i] < 10 ) return(TRUE);
                 Jump_Mark(1);
				 InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 4 ) {
   			     if ( ButtonSetMark[i] < 10 ) return(TRUE);
                 Jump_Mark(2);
				 InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 5 ) {
   			     if ( ButtonSetMark[i] < 10 ) return(TRUE);
			     j=31;
                 while (( j >= 0 ) && ( HeaderInfo.SprungMarken[j] == 0xffffffff )) j--;
				 Jump_Time_Play(HeaderInfo.SprungMarken[j]);
				 InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 6 ) {
   			     if ( ButtonSetMark[i] < 10 ) return(TRUE);
        		 KillTimer(hDlg,657);
				 PostMessage(hWnd,WM_COMMAND,IDM_RECORD_PLAY,0);
    			 EndDialog(hDlg,TRUE);
	   } else if ( i == 7 ) {
   			     if ( ButtonSetMark[i] < 10 ) return(TRUE);
                 Insert_Sprungmarke(HeaderInfo.CurrentSpielZeit);
		         SendMessage (GetDlgItem(hDlg,IDC_LIST1), LB_RESETCONTENT , 0, 0 );
		      	 for ( j = 0 ; j < 32 ; j++ ) {
						  if ( HeaderInfo.SprungMarken[j] != 0xffffffff) {
						  m=(HeaderInfo.SprungMarken[j]/60);
						  s=HeaderInfo.SprungMarken[j]-(m*60);
						  sprintf(Text,t("%3d Min %02d Sek"),m,s);
						  SendMessage(GetDlgItem(hDlg,IDC_LIST1),LB_INSERTSTRING,j,(LPARAM)Text);
                          }; 
					}

				 Update_MDI_File();
				 ButtonSetMark[7]=11;
				 InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 8 ) {
   			     if ( ButtonSetMark[i] < 10 ) return(TRUE);
        		 KillTimer(hDlg,657);
			     InfoRet=3; 
				 PostMessage(hWnd,WM_COMMAND,IDM_PLAY_INFO_CONTROL,0);
    			 EndDialog(hDlg,TRUE);
	   } else if ( i == 9 ) {
	     		 KillTimer(hDlg,657);
		         EndDialog(hDlg,TRUE);
	   };

	break;	
   case WM_COMMAND:


	   		 if ((LOWORD(wParam) == IDC_LIST1 ) &&
			 (HIWORD(wParam)==LBN_DBLCLK))  {
			  i=SendMessage(GetDlgItem(hDlg,IDC_LIST1),LB_GETCURSEL,0,0);
			  if (( i >= 0 ) && ( i < 32 )) {
			  Jump_Time_Play(HeaderInfo.SprungMarken[i]);
			  };
              return(TRUE);
		 };

		 
		 if (LOWORD(wParam) == IDC_INSERT ) {
 			    m=GetDlgItemInt(hDlg,IDC_EDIT1,NULL,FALSE);
			    s=GetDlgItemInt(hDlg,IDC_EDIT2,NULL,FALSE);
			    TimeStamp=m*60+s;
    			 if (Insert_Sprungmarke(TimeStamp) == FALSE ) {
						            MessageBox(hDlg,t("Max. 32 Sprungmarken erlaubt"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
									return(TRUE);
					  };
 			  Update_MDI_File();
			  InvalidateRect(hDlg,&UpdateRect,FALSE);
			  SendMessage(GetDlgItem(hDlg,IDC_LIST1),LB_RESETCONTENT,0,(LPARAM)0);
				 for ( i = 0 ; i < 32 ; i++ ) {
						  if ( HeaderInfo.SprungMarken[i] != 0xffffffff ) {
						  m=(HeaderInfo.SprungMarken[i]/60);
						  s=HeaderInfo.SprungMarken[i]-(m*60);
						  sprintf(Text,t("%3d Min %02d Sek"),m,s);
						  SendMessage(GetDlgItem(hDlg,IDC_LIST1),LB_INSERTSTRING,i,(LPARAM)Text);
                          }; 
					}
			return(TRUE);
		};

			
			if (LOWORD(wParam) == IDC_DEL ) {
			  i=SendMessage(GetDlgItem(hDlg,IDC_LIST1),LB_GETCURSEL,0,0);
			  if (( i >= 0 ) && ( i < 32 )) {
				  m=(HeaderInfo.SprungMarken[i]/60);
				  s=HeaderInfo.SprungMarken[i]-(m*60);
				  SetDlgItemInt(hDlg,IDC_EDIT1,m,FALSE);
				  SetDlgItemInt(hDlg,IDC_EDIT2,s,FALSE);
			      Del_Sprungmarke(i);
 			      Update_MDI_File();
 			      InvalidateRect(hDlg,&UpdateRect,FALSE);
				  SendMessage(GetDlgItem(hDlg,IDC_LIST1),LB_RESETCONTENT,0,(LPARAM)0);
				  for ( i = 0 ; i < 32 ; i++ ) {
						  if ( HeaderInfo.SprungMarken[i] != 0xffffffff ) {
						  m=(HeaderInfo.SprungMarken[i]/60);
						  s=HeaderInfo.SprungMarken[i]-(m*60);
						  sprintf(Text,t("%3d Min %02d Sek"),m,s);
						  SendMessage(GetDlgItem(hDlg,IDC_LIST1),LB_INSERTSTRING,i,(LPARAM)Text);
                          }; 
					}
				  
			  };
			return(TRUE);
		};

		 if (LOWORD(wParam) == IDC_ADDNOW ) {
    			 if (Insert_Sprungmarke(HeaderInfo.CurrentSpielZeit) == FALSE ) {
						            MessageBox(hDlg,t("Max. 32 Sprungmarken erlaubt"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
									return(TRUE);
					  };

 			     Update_MDI_File();
 			     InvalidateRect(hDlg,&UpdateRect,FALSE);
				 SendMessage(GetDlgItem(hDlg,IDC_LIST1),LB_RESETCONTENT,0,(LPARAM)0);
				 for ( i = 0 ; i < 32 ; i++ ) {
						  if ( HeaderInfo.SprungMarken[i] != 0xffffffff ) {
						  m=(HeaderInfo.SprungMarken[i]/60);
						  s=HeaderInfo.SprungMarken[i]-(m*60);
						  sprintf(Text,t("%3d Min %02d Sek"),m,s);
						  SendMessage(GetDlgItem(hDlg,IDC_LIST1),LB_INSERTSTRING,i,(LPARAM)Text);
                          }; 
					}
           return(TRUE);
		 }

	   
	   
	   
	   
	   if (LOWORD(wParam) == IDCANCEL ) {
    		 KillTimer(hDlg,657);
			 EndDialog(hDlg,TRUE);
             return(TRUE);
		 }


	   }



		 return(FALSE);
};



BOOL APIENTRY RecordControlDlg(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
    PAINTSTRUCT wps;                   /* paint structure           */
    HDC hdc;
    HDC             hMemDC;
    HBITMAP         hOldBm;
	BITMAP bm;

	static HBITMAP Control;
    static BITMAPINFO          *TESTBITMAP                    = NULL;

	char Text[128];
	int m,s;
    int xPos;
	int yPos;
    int i;
    static int Timer;
    static RECT UpdateRect;

switch (message) {
        case WM_INITDIALOG:
			SetTimer(hDlg,657,100,NULL);
            Control =LoadBitmap(hInst,"RECORDBITMAP");
            TESTBITMAP=(BITMAPINFO*)LockResource(Control);
            Timer=10;
			UpdateRect.top=18;
			UpdateRect.left=0;
			UpdateRect.right=586;
			UpdateRect.bottom=63;


			
        case WM_TIMER:
			if ( wParam == 657 ) {
			  if ( ButtonSetRecord[3]==11 ) {
                           ButtonSetRecord[3]=10;
						   InvalidateRect(hDlg,&UpdateRect,FALSE);
					  };

			    Timer++;
				if ( Timer >= 10 ) {
					  Timer=0;  
					if ( Record == TRUE ) {
					   i=0;
					   
					   while (( i < 64 ) && ( HeaderInfo.Infos[i] >= 0x20 )) {
					   Text[i]=HeaderInfo.Infos[i];
					   i++;
					   Text[i]=0x00;
					   };
					   if ( i == 0 ) strcpy(Text,t("<Unbekannt>"));
                       SetDlgItemText(hDlg,TEXT2,Text);
				       m=(HeaderInfo.CurrentSpielZeit/60);
					   s=HeaderInfo.CurrentSpielZeit-(m*60);
					   sprintf(Text,"%3d:%02d",m,s);
					   SetDlgItemText(hDlg,TEXT1,Text);
					
					} else {
						strcpy(Text,t("<Keine Aufnahme gestartet>"));
                        SetDlgItemText(hDlg,TEXT2,Text);
					    SetDlgItemText(hDlg,TEXT1,"");
 
				};
			};
			};
            return(TRUE);

		case WM_PAINT:
	        hdc = BeginPaint(hDlg,&wps);
			hMemDC = CreateCompatibleDC(hdc);
			hOldBm=SelectObject(hMemDC, Control );
			Set_Win_Buttons(2,&ButtonSetRecord[0],hMemDC);
			GetObject(Control, sizeof(BITMAP), (LPSTR)&bm);
			BitBlt(hdc, 0, 18, bm.bmWidth, bm.bmHeight,hMemDC, 0, 0, SRCCOPY);// Signal
			SelectObject(hMemDC,hOldBm);
			DeleteDC(hMemDC);
			DeleteDC(hdc);
            EndPaint(hDlg,&wps);
            Timer=10;
			return(FALSE);

   case WM_LBUTTONUP:
       xPos = LOWORD(lParam);  // horizontal position of cursor
       yPos = HIWORD(lParam);  // vertical position of cursor

	   i=Get_Button_Click(18,xPos,yPos);
	   
	   if ( i == 0 ) {
			 if ( ButtonSetRecord[i] < 10 ) return(TRUE);
			 if ( Record == FALSE ) {
			        Debi_Record_Start(RecorderFormat,RecorderTyp);
			 } else if ( Recorder_Pause == TRUE ) {
				    Debi_Record_Pause();
			 };
			   InvalidateRect(hDlg,&UpdateRect,TRUE);
	   } else if ( i == 1 ) {
		    	 if ( ButtonSetRecord[i] < 10 ) return(TRUE);
				 Debi_Record_Pause();
			     InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 2 ) {
	    		 if ( ButtonSetRecord[i] < 10 ) return(TRUE);
    			 if ( Record == TRUE ) Debi_Record_Stop();
			     InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 3 ) {
   			     if ( ButtonSetRecord[i] < 10 ) return(TRUE);
				 Insert_Sprungmarke(HeaderInfo.CurrentSpielZeit);
				 Update_MDI_File();
                 ButtonSetRecord[3]=11;
			     InvalidateRect(hDlg,&UpdateRect,FALSE);
	   } else if ( i == 4 ) {
   			     if ( ButtonSetRecord[i] < 10 ) return(TRUE);
        		 KillTimer(hDlg,657);
			     InfoRet=4; 
				 PostMessage(hWnd,WM_COMMAND,IDM_PLAY_INFO_CONTROL,0);
    			 EndDialog(hDlg,TRUE);
	   } else if ( i == 5 ) {
	     		 KillTimer(hDlg,657);
		         EndDialog(hDlg,TRUE);
	   };

	break;	
   case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL ) {
    		 KillTimer(hDlg,657);
			 EndDialog(hDlg,TRUE);
             return(TRUE);
		 }


	   }



		 return(FALSE);
};


	
void Play_Stop(void)
{
    int i;

	if ( Play == TRUE ) {
      Insert_Sprungmarke(HeaderInfo.CurrentSpielZeit);
      Update_MDI_File();
      Debi_Record_Stop();
	} else if ( Record == TRUE ) {
      Debi_Record_Stop();
				for ( i=0; i < TIMERANZAHL; i++ ) {
					if (( TimerRec[i].StartZeit_h != 0xff) && (TimerRec[i].Started==TRUE)) {
									TimerRec[i].Started=FALSE;
								    TimerRec[i].StartZeit_h =0xff;
									return;
									};
								};

	}
}


void Update_MDI_File(void)
{
int UpdateMDI;
 
	 UpdateMDI=open(HeaderInfo.MdiName,_O_WRONLY  | _O_TRUNC | _O_CREAT | _O_BINARY,_S_IREAD | _S_IWRITE );
         if ( UpdateMDI >= 0 ) {
           write(UpdateMDI,&HeaderInfo.StartZeit[0],sizeof(struct IndexHeader_t));
           close(UpdateMDI);
		 };
};


void Get_File_Times(unsigned int FileNummer, unsigned int *Start, unsigned int *Stop)
{
	int CheckFile;
	unsigned char Buffer[8000];
    int i;
    unsigned int s1;
	unsigned int TimeStampStart=0;
	unsigned int TimeStampStop=0;

	CheckFile=open(HeaderInfo.PlayFiles[FileNummer],_O_RDONLY | _O_BINARY);
	if ( CheckFile < 0 ) {
		                 *Start=0;
						 *Stop=0;
						 return;
	};

 i=1;
while ((i > 0 ) && ( TimeStampStart == 0 )) {
	 i=Load_Next_Buffer(&CheckFile,3,&Buffer[0]);
	 TimeStampStart=Get_Seconds(Buffer);
  };
 _lseeki64(CheckFile,(__int64)-32768,SEEK_END);
i=1;
while (i > 0 ) {
	 i=Load_Next_Buffer(&CheckFile,3,&Buffer[0]);
	 s1=Get_Seconds(Buffer);
	 if ( s1 != 0 ) TimeStampStop=s1; 
  };
*Start=TimeStampStart;
*Stop=TimeStampStop;
close(CheckFile);
}

BOOL Jump_Time_Play(unsigned int Seconds)
{

	BOOL Found=FALSE;
	int CheckFile;
	unsigned char Buffer[0x800];
    int i;
    int filecount ;
	unsigned __int64 FilePos;
	unsigned __int64 FileStart;
	unsigned __int64 FileStop;
	unsigned __int64 LastFilePos=0;
	unsigned int CurrentTimeStamp=0;
	unsigned int TimeStampStop=0;
	unsigned int TimeStampStart=0;
    unsigned int CZeit;


    CheckFile=-1;
	 
    filecount = 0;
	while (( filecount < 32 ) && (Found == FALSE )) {
		 Get_File_Times(filecount, &TimeStampStart, &TimeStampStop);
         if (( Seconds >= LaufZeitSekunden(HeaderInfo.TimeStampStart,TimeStampStart)) &&  
			 ( Seconds <= LaufZeitSekunden(HeaderInfo.TimeStampStart,TimeStampStop))) {
			 Found = TRUE;
		 } else filecount++;

	};

	if ( filecount > 32 ) return(FALSE);
	CheckFile=open(HeaderInfo.PlayFiles[filecount],_O_RDONLY | _O_BINARY);
	if ( CheckFile < 0 ) return(FALSE);

	FileStart=0;
	FileStop=_filelengthi64(CheckFile);
	Found = FALSE;
	for (;;) {
		   FilePos=(FileStart+(FileStop-FileStart)/2);
           _lseeki64(CheckFile,FilePos,SEEK_SET);

            CurrentTimeStamp=0;
			while ( CurrentTimeStamp == 0 ) 
					{
                 	 i=Load_Next_Buffer(&CheckFile,3,&Buffer[0]);
                     CurrentTimeStamp=Get_Seconds(&Buffer[0]);
			};
           FilePos=_lseeki64(CheckFile,FilePos,SEEK_SET);
  		   CZeit=LaufZeitSekunden(HeaderInfo.TimeStampStart,CurrentTimeStamp);

            if ((  FilePos == LastFilePos ) ||
				( Seconds == CZeit+1 )      ||
				( Seconds == CZeit )) 
						{
				         HeaderInfo.CurrentSpielZeit=CZeit;
						 RecordDateiNummer=filecount;
                         strcpy(LastRecorderFile,HeaderInfo.PlayFiles[filecount]);
                         Recorder_Pause=TRUE;
						 Sleep(10);
						 if ( AudioFilePtr >=  0 ) close(AudioFilePtr);
						 if ( VideoFilePtr >=  0 ) close(VideoFilePtr);
			       			AudioFilePtr=open(LastRecorderFile,_O_RDONLY | _O_BINARY);
							VideoFilePtr=open(LastRecorderFile,_O_RDONLY | _O_BINARY);
							AudioBlocks=0;
							VideoBlocks=0;
                            _lseeki64(AudioFilePtr,FilePos,SEEK_SET);
                            _lseeki64(VideoFilePtr,FilePos,SEEK_SET);
                            Recorder_Pause=FALSE;
							SAA7146_AudioCom(0x0E); // SYNC True

							close(CheckFile);
                         return(TRUE);
					}
            LastFilePos = FilePos;
			if ( Seconds < CZeit ) FileStop=FilePos;
			else FileStart=FilePos;
	}
    close(CheckFile);
	return(FALSE);
	};



BOOL APIENTRY RecordPlayInterrupt(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
    OPENFILENAME OpenFileName;
    CHAR         szDirName[256]   = "";
    CHAR         szFile[256]      = "\0";
    CHAR         szFileTitle[256];
	char StartPfad[255];
    unsigned __int64 FileS;
    const char *szFilter;

    szFilter = t("MultiDec DVR-Formate (*.pva-??;*.mpg-??)\0*.pva-*;*.mpg-*;\0Technotrend PVA (*.pva-??)\0*.pva-*\0 MPEG-2 (*.mpg-??)\0*.mpg-*\0\0");
    switch (message) {
        case WM_INITDIALOG:
              sprintf(StartPfad,t("Folge-Datei %s-%d nirgens gefunden !!!"),RecorderName,RecordDateiNummer);
			  SetDlgItemText(hDlg,IDC_TEXT1,StartPfad);
			  SetFocus(hDlg);
			  break;

        case WM_COMMAND:

		 if (LOWORD(wParam) == IDC_BUTTON1 ) {
			                                 if ( RecorderFile >= 0 ) close(RecorderFile);
											 RecorderFile=-1;
											 EnableWindow(GetDlgItem(hDlg,IDOK),FALSE);
						               		 OpenFileName.lStructSize       = sizeof(OPENFILENAME);
											 OpenFileName.hwndOwner         = hDlg;
                                             OpenFileName.hInstance         = (HANDLE) hInst;
											 OpenFileName.lpstrFilter       = szFilter;

                                             OpenFileName.lpstrCustomFilter = (LPSTR) NULL;
                                             OpenFileName.nMaxCustFilter    = 0L;
                                             OpenFileName.nFilterIndex      = 0L;
                                             OpenFileName.lpstrFile         = szFile;
                                             OpenFileName.nMaxFile          = sizeof(szFile);
                                             OpenFileName.lpstrFileTitle    = szFileTitle;
                                             OpenFileName.nMaxFileTitle     = sizeof(szFileTitle);
                                             OpenFileName.lpstrInitialDir   = (LPCTSTR)RecordBaseDir[0];
                                             OpenFileName.lpstrTitle        = t("Folge-Dateien abspielen");
                                             OpenFileName.nFileOffset       = 0;
                                             OpenFileName.nFileExtension    = 0;
                                             OpenFileName.lpstrDefExt       = (LPCTSTR)"*.pva-*;*.mpg-*";
                                             OpenFileName.lCustData         = 0;
                                             OpenFileName.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST |
                                                                  OFN_HIDEREADONLY | OFN_NONETWORKBUTTON;


											 
											  
 										     if (!GetOpenFileName(&OpenFileName)) return(FALSE);
											 SetDlgItemText(hDlg,IDC_EDIT1, OpenFileName.lpstrFile);
											 SetCurrentDirectory(CurrentDir);
                 
										          RecorderFile=open(OpenFileName.lpstrFile,_O_RDONLY | _O_BINARY);
                                                  if ( RecorderFile >= 0 ) {
												      strcpy(LastRecorderFile,OpenFileName.lpstrFile);
													  EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);
												  }
 
		 };		 

		 
		 
		 if (LOWORD(wParam) == IDOK ) {


			FileS=_filelengthi64(RecorderFile);
			HeaderInfo.Anzahl_Files++;
            strcpy(&HeaderInfo.PlayFiles[HeaderInfo.Anzahl_Files-1][0],LastRecorderFile);
            HeaderInfo.KBTotal=HeaderInfo.KBTotal+(unsigned int)(FileS/1024);
	
			if ( AudioFilePtr >=  0 ) close(AudioFilePtr);
			if ( VideoFilePtr >=  0 ) close(VideoFilePtr);

			AudioFilePtr=open(LastRecorderFile,_O_RDONLY | _O_BINARY);
			VideoFilePtr=open(LastRecorderFile,_O_RDONLY | _O_BINARY);
			AudioBlocks=0;
		    VideoBlocks=0;
            if ( RecorderFile >= 0 ) close(RecorderFile);
			 RecorderFile=-1;

			 SetCurrentDirectory(CurrentDir);
			 Debi_Record_Pause();
			 EndDialog(hDlg,0);

		 }
		 if (LOWORD(wParam) == IDCANCEL ) {
			 SetCurrentDirectory(CurrentDir);
			 Debi_Record_Stop();
			 sprintf(ChannelString,"%d",Make_ProgNr(CurrentProgramm)+1);
             SetTimer(hWnd,99,1000,NULL);
			 EndDialog(hDlg,1);
		 }


			 break;
    }
    return (FALSE);
        UNREFERENCED_PARAMETER(lParam);
}

void Check_Recorder_Name(unsigned char *Text)
{
	int i=0;
	while (( i < 256) && ( Text[i] != 0x00 )) {

      if ( Text[i]==':' ) Text[i]='-';
      if ( Text[i]=='\\' ) Text[i]='_';
      if ( Text[i]=='/' ) Text[i]='_';
      if ( Text[i]=='<' ) Text[i]='(';
      if ( Text[i]=='>' ) Text[i]=')';
      if ( Text[i]=='*' ) Text[i]='#';
      if ( Text[i]=='"' ) Text[i]='#';
      if ( Text[i]=='|' ) Text[i]='#';
      if ( Text[i]=='?' ) Text[i]='+';				// '?' added by BigReaper
     i++;
	}

};


BOOL Recorder_Create_Files(int RecordTyp)
{

    char PESFile[255];
    char PESIndex[255];

	int Index;
	int Offset;

    int Virtual_Event_Id,Virtual_Offset_Id;

	int i,j,k;
    BOOL HeaderWritten=FALSE;
	unsigned short tag,monat,jahr;
    
	// Änderung oxident
	// Dateinamenzusatz für Recordfile
	char RecPrgName[128];
	unsigned char RecPrgStartZeit[6];
	
	int RecorderIndexFile=-1;
    HeaderInfo.RecordFormat = RecorderFormat;
    strcpy(HeaderInfo.ProgrammName,Programm[CurrentProgramm].Name);
    HeaderInfo.StartZeit[0]=0x00;
    HeaderInfo.AufnahmeDatum[0]=0x00;
    HeaderInfo.Infos[0]=0x00;
	RecordBase=0;
    RecordDateiNummer=0;



		if ( Find_Current_EIT(Programm[CurrentProgramm].SID_pid,Programm[CurrentProgramm].tp_id,&Index,&Offset) == TRUE ) {
              sprintf(HeaderInfo.StartZeit,"%02d:%02d",RealTime(EIT[Index].EIT_Infos[Offset].Start_Zeit_h),EIT[Index].EIT_Infos[Offset].Start_Zeit_m);
              Date_To_String((unsigned short)RealDay(EIT[Index].EIT_Infos[Offset].Start_Zeit_h,
                                     EIT[Index].EIT_Infos[Offset].Date_Id),HeaderInfo.AufnahmeDatum);
			  HeaderInfo.Dauer_h=EIT[Index].EIT_Infos[Offset].Dauer_h;
			  HeaderInfo.Dauer_m=EIT[Index].EIT_Infos[Offset].Dauer_m;
			  HeaderInfo.Dauer_s=EIT[Index].EIT_Infos[Offset].Dauer_s;
			    i=0;
				j=0;
			    k=0;
			
               Virtual_Event_Id=Index;
				Virtual_Offset_Id=Offset;

                if (( EIT[Index].EIT_Infos[Offset].virtual_eventid != 0xffff ) &&
					( EIT[Index].EIT_Infos[Offset].virtual_sid != 0xffff )) {
                      i=Get_Virtual_Event(EIT[Index].EIT_Infos[Offset].virtual_eventid,EIT[Index].EIT_Infos[Offset].virtual_sid);
					  if ( i >= 0 ) {
                           Virtual_Event_Id=EIT[Index].EIT_Infos[Offset].virtual_eventid;
                           Virtual_Offset_Id=i;
					  }
				}
				
				
				if ( EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].HeaderSize > 0 ) {
				while ( EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Header[i] != 0x00 ) {
					HeaderInfo.Infos[j] = EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Header[i];
                    if ( HeaderInfo.Infos[j] < 0x20 ) { 
					if ( HeaderInfo.Infos[j] == 0x0a ) {
						HeaderInfo.Infos[j]=0x0d;
						j++;
						HeaderInfo.Infos[j]=0x0a;
						k=0;
					} else {
						HeaderInfo.Infos[j]=0x00;
						j--;
                        k--;
					};
					};
						if ( k > 70 ) {
						if ( HeaderInfo.Infos[j]==0x20 ) {
							HeaderInfo.Infos[j]=0x0d;
							j++;
							HeaderInfo.Infos[j]=0x0a;
                            k=0;
					}
                    };
					k++;
					j++;
					i++;
				};
				};
				HeaderInfo.Infos[j++]=0x0d;
				HeaderInfo.Infos[j++]=0x0a;
				HeaderInfo.Infos[j]=0x00;
				if ( EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].TextSize > 0 ) {
				i=0;
                k=0;
				while ( EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Text[i] != 0x00 ) {
					HeaderInfo.Infos[j] = EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Text[i];
                    if ( HeaderInfo.Infos[j] < 0x20 ) { 
					if ( HeaderInfo.Infos[j] == 0x0a ) {
						HeaderInfo.Infos[j]=0x0d;
						j++;
						HeaderInfo.Infos[j]=0x0a;
						k=0;
					} else {
						HeaderInfo.Infos[j]=0x00;
						j--;
                        k--;
					};
					};
						if ( k > 70 ) {
						if ( HeaderInfo.Infos[j]==0x20 ) {
							HeaderInfo.Infos[j]=0x0d;
							j++;
							HeaderInfo.Infos[j]=0x0a;
                            k=0;
					}
                    };
					k++;
					j++;
					i++;
				};
				HeaderInfo.Infos[j]=0x00;
		};

	};  // EIT-Infos

    DecodeDVBDate((unsigned short)(TDT.Date_Id-15018), &jahr, &monat,&tag);
    // Änderung oxident
    // Dateiname der Aufnahme umgestellt
    // Monat, Tag, Stunde, Minute, "_", Name der aktuellen Sendung
    // Original: sprintf(RecorderName,"%02d%02d%02d%02d",TDT.Hour,TDT.Min,tag,monat);
    sprintf(RecorderName,"%02d%02d%02d%02d",monat,tag,RealTime(TDT.Hour),TDT.Min);	
    strcat(RecorderName, "_");
    GetCurrentNext(Programm[CurrentProgramm].SID_pid,Programm[CurrentProgramm].tp_id,2,(unsigned char *)&RecPrgName,(unsigned char *)&RecPrgStartZeit);
    strcat(RecorderName, RecPrgName);

    Check_Recorder_Name(&RecorderName[0]);

    // Ende
    for ( i=0; i < 32; i++ ) HeaderInfo.SprungMarken[i] = 0xffffffff;

    if ( RecordTyp == 0 ) strcat(RecorderName,".mpg");
    else if ( RecordTyp == 1 ) strcat(RecorderName,".pva");

    RecorderFile=-1;
    RecorderIndexFile=-1;
    i=0;

    while (( i < 8 ) && ( RecorderIndexFile == -1 ) && (RecordBaseDir[i][0] != 0x00 ) && ( HeaderWritten == FALSE)) {
    sprintf(PESIndex,"%s\\%s",RecordBaseDir[i],RecorderName);
    PESIndex[strlen(PESIndex)-4]=0x00;
    strcat(PESIndex,".mdi");
    HeaderInfo.CurrentSpielZeit=0;

    RecorderIndexFile=open(PESIndex,_O_WRONLY  | _O_TRUNC | _O_CREAT | _O_BINARY,_S_IREAD | _S_IWRITE );
    if ( RecorderIndexFile >= 0 ) {
               j=write(RecorderIndexFile,&HeaderInfo.StartZeit[0],sizeof(struct IndexHeader_t));
               if ( j != sizeof(struct IndexHeader_t)) {
                             close(RecorderIndexFile);
                             remove(PESIndex);
						     RecorderIndexFile=-1;
		       } else {
			       // Habe Schreiben können 
                   close(RecorderIndexFile);
			       RecordBase=i;
                   HeaderWritten=TRUE;
			       RecorderIndexFile=-1;
		       };
		       }
    i++;
    };

    if ( HeaderWritten == FALSE ) return(FALSE);


    sprintf(PESFile,"%s\\%s",RecordBaseDir[RecordBase],RecorderName);
    RecorderFile=open(PESFile,_O_WRONLY  | _O_TRUNC | _O_CREAT | _O_BINARY,_S_IREAD | _S_IWRITE );
    if ( RecorderFile < 0 ) return(FALSE);
    strcpy(LastRecorderFile,PESFile);
    return(TRUE);
}


BOOL Next_Record_File(int RecordTyp, unsigned char id)
{
    char PESFile[255];
    static unsigned char video_pack_start[] = {
        0, 0, 1, VIDEO_STREAM_S,
        0, sizeof(seq_end_code)+sizeof(program_end_code)+3, 0x80, 0, 0};

    if ( RecorderFile != -1 ) {
        if (RecordTyp == 0) {
            video_pack_start[3] = id;
            write(RecorderFile, video_pack_start, sizeof(video_pack_start));
            write(RecorderFile, seq_end_code, sizeof(seq_end_code));
            write(RecorderFile, program_end_code, sizeof(program_end_code));
        }
        close(RecorderFile);
    }
    RecorderFile=-1;
    RecordDateiNummer++;
    RecordBase++;
    if (( HeaderInfo.MegaBytes_Recorded > MAXMegaBytes ) && ( MAXMegaBytes != 0 )) RecordBase--;
    HeaderInfo.Bytesplay=0;

    if ( RecordBase > 7 ) return(FALSE);
    while (( RecordBase < 8 )  && (RecordBaseDir[RecordBase][0] != 0x00 )) {
        sprintf(PESFile,"%s\\%s-%d",RecordBaseDir[RecordBase],RecorderName,RecordDateiNummer);
        RecorderFile=open(PESFile,_O_WRONLY  | _O_TRUNC | _O_CREAT | _O_BINARY,_S_IREAD | _S_IWRITE );
        if ( RecorderFile >= 0 ) {
	        strcpy(LastRecorderFile,PESFile);
		    HeaderInfo.Anzahl_Files++;
			if (RecordTyp == 0) {
				Write_PS_Header(RecorderFile);
			}
		    return(TRUE);
        }
        RecordBase++;
    }
	
    return(FALSE);
};


BOOL Next_Play_File( )
{
    char PESFile[255];
    
    if (RecorderFile!=-1) close(RecorderFile);
    if ( AudioFilePtr >=  0 ) close(AudioFilePtr);
    if ( VideoFilePtr >=  0 ) close(VideoFilePtr);
    
    RecordDateiNummer++;
    RecorderFile=-1;
    RecordBase=0;	
    while (( RecordBase < 9 ) && (RecordBaseDir[RecordBase][0] != 0x00 )) {
        sprintf(PESFile,"%s\\%s-%d",RecordBaseDir[RecordBase],RecorderName,RecordDateiNummer);
        RecorderFile=open(PESFile,_O_RDONLY  | _O_BINARY);
        if ( RecorderFile >= 0 ) {
            strcpy(LastRecorderFile,PESFile);
            
            close(RecorderFile);
            RecorderFile=-1;
            
            AudioFilePtr=open(LastRecorderFile,_O_RDONLY | _O_BINARY);
            VideoFilePtr=open(LastRecorderFile,_O_RDONLY | _O_BINARY);
            AudioBlocks=0;
            VideoBlocks=0;
            SAA7146_AudioCom(0x0E); // SYNC True
            
            return(TRUE);
        }
        RecordBase++;
    }
    return(FALSE);
};



BOOL Debi_Record_Start(int RecordFormat, int RecordTyp ) {
    int i; 
    int s1, s2;
    DWORD RecorderThreadId;

    i=0;
    while (( i < 100 ) && ( Find_Current_EIT(Programm[CurrentProgramm].SID_pid,Programm[CurrentProgramm].tp_id,&s1,&s2 ) == FALSE )) {
        Sleep(40); // ggf wg. EIT
        i++;
    }


    HeaderInfo.Bytesplay=0;
    HeaderInfo.CurrentSpielZeit=0;
    HeaderInfo.TimeStampStart=0;
    HeaderInfo.BitRate=0;
    HeaderInfo.SampleFreq=0;
    HeaderInfo.MegaBytes_Recorded=1;
    HeaderInfo.Anzahl_Files=1;
    HeaderInfo.KBTotal=0;
    HeaderInfo.MegaBytes_Recorded=1;


    if ( RecordFormat ==	0 )  strcpy(HeaderInfo.TypeName,"Audio");
    else if ( RecordFormat ==	1 )  strcpy(HeaderInfo.TypeName,"Video");
    else if ( RecordFormat ==	2 )  strcpy(HeaderInfo.TypeName,"Audio+Video");

    if ( RecordTyp ==	0 )  strcpy(HeaderInfo.TypeName,"MPG");
    else if ( RecordTyp ==	1 )  strcpy(HeaderInfo.TypeName,"PVA");

    if ( Recorder_Create_Files(RecordTyp)== FALSE ) return(FALSE); 


    Record=TRUE;
    ShutDownFilters();
    EnableMenuItem(GetMenu(hWnd),IDM_PLAY_MARK_CONTROL,MF_GRAYED);
    EnableMenuItem(GetMenu(hWnd),IDM_PLAY_TIME_CONTROL,MF_GRAYED);
    EnableMenuItem(GetMenu(hWnd),IDM_PLAY_INFO_CONTROL,MF_ENABLED);
    EnableMenuItem(GetMenu(hWnd),IDM_RECORD_PLAY,MF_GRAYED);

    ButtonSetRecord[0]=1;
    ButtonSetRecord[1]=10;
    ButtonSetRecord[2]=10;
    ButtonSetRecord[3]=10;
    ButtonSetRecord[4]=10;
    ButtonSetRecord[5]=10;

    Recorder_Pause = FALSE;
    Recorder_Slow = FALSE;
    Recorder_FF = FALSE;

    for ( i=0; i < plugins_count; i++ ) {
        if ( Ext_Dll[i].Extern_RecPlay != NULL ) (Ext_Dll[i].Extern_RecPlay)(2);
    }


    Init_Rec_Play_Buffer();

    if ( RecordFormat == 0) CurrentRecordMode = AudioMPG;
    else if ( RecordFormat == 1) CurrentRecordMode = VideoMPG;
    else CurrentRecordMode = AV_MPG;

    if ( RecordTyp == 0 ) Write_PS_Header(RecorderFile );

    if (recorder_buf == NULL) {
        recorder_buf = create_ring_buf(RECORDER_BUFFER_SIZE);
        if (recorder_buf == NULL) {
            MessageBox(hWnd, "Can't create a ring buffer for recording", "Multi-Decoder", MB_ICONSTOP|MB_OK);
            return FALSE;
        }
    }

    SAA7146_OutCom(COMTYPE_REC_PLAY, __Record, 2, CurrentRecordMode, 0);
    RecorderThreadId = _beginthread(Recorder_Thread, 0, NULL);

    //ARM_ClearMailBox();

    return(TRUE);
}

void  Debi_Record_FastFF( void )
{
if ( Recorder_FF == FALSE ) {
    Recorder_FF = TRUE ;
    SAA7146_OutCom( COMTYPE_REC_PLAY, __Scan_I, 2, CurrentPlayMode, 0);
	ButtonSetPlay[0]=10;
	ButtonSetPlay[1]=1;
	ButtonSetPlay[2]=10;
	ButtonSetPlay[3]=1;
	ButtonSetPlay[4]=1;
	ButtonSetPlay[5]=11;
	ButtonSetPlay[6]=1;
	ButtonSetPlay[7]=1;
	ButtonSetPlay[8]=1;
	return;
}; 
Debi_Record_Play_Normal();
};



void  Debi_Record_Play_Normal( void )
{
SAA7146_OutCom(COMTYPE_REC_PLAY, __Play, 2, CurrentPlayMode, 0);
SAA7146_AudioCom(0x01); // Audio Pause
SAA7146_AudioCom(0x0E); // SYNC True

	ButtonSetPlay[0]=1;
	ButtonSetPlay[1]=10;
	ButtonSetPlay[2]=10;
	ButtonSetPlay[3]=10;
	ButtonSetPlay[4]=10;
	ButtonSetPlay[5]=10;
	ButtonSetPlay[6]=10;
	ButtonSetPlay[7]=10;
	ButtonSetPlay[8]=10;
    Recorder_Pause = FALSE;
    Recorder_Slow = FALSE;
	Recorder_FF = FALSE;

};

void  Debi_Record_SlowFF( void )
{
if ( Recorder_Slow == FALSE ) {
	
	SAA7146_AudioCom(0x0F); // SYNC False
	SAA7146_AudioCom(0x01); // Audio Pause

    SAA7146_OutCom( COMTYPE_REC_PLAY, __Slow , 2, 0,0);  
    SAA7146_VideoCom(0x22,2);
	ButtonSetPlay[0]=10;
	ButtonSetPlay[1]=1;
	ButtonSetPlay[2]=10;
	ButtonSetPlay[3]=1;
	ButtonSetPlay[4]=11;
	ButtonSetPlay[5]=1;
	ButtonSetPlay[6]=1;
	ButtonSetPlay[7]=1;
	ButtonSetPlay[8]=1;
	Recorder_Pause = FALSE;
	Recorder_Slow = TRUE;
	Recorder_FF = FALSE;
	return;
}; 
Debi_Record_Play_Normal();
};

void  Debi_Record_Pause( void )
{

if ( Recorder_Pause == FALSE ) {

    Recorder_Pause = TRUE ;
    if ( Record == TRUE ) {
		ButtonSetRecord[0]=10;
		ButtonSetRecord[1]=11;
		ButtonSetRecord[2]=10;
		ButtonSetRecord[3]=1;
		ButtonSetRecord[4]=1;
		SAA7146_OutCom( COMTYPE_REC_PLAY, __Stop, 0);
	} else {
		ButtonSetPlay[0]=10;
		ButtonSetPlay[1]=11;
		ButtonSetPlay[2]=10;
		ButtonSetPlay[3]=1;
		ButtonSetPlay[4]=1;
		ButtonSetPlay[5]=1;
		ButtonSetPlay[6]=1;
		ButtonSetPlay[7]=1;
		ButtonSetPlay[8]=1;
		SAA7146_AudioCom(0x0F);
		SAA7146_AudioCom(1);
		SAA7146_OutCom(COMTYPE_REC_PLAY, __Pause, 0, 0);
	}
	return;
}; 
    Recorder_Pause = FALSE;
    Recorder_Slow = FALSE;
	Recorder_FF = FALSE;

    
	if ( Play == TRUE ) {
        SAA7146_OutCom(COMTYPE_REC_PLAY,  __Continue, 0);
		SAA7146_AudioCom(0x12);        // Audio Continue   
		SAA7146_VideoCom(0x000d, 0);   // Video Continue
		SAA7146_AudioCom(0x0E);        // Sync
		ButtonSetPlay[0]=1;
		ButtonSetPlay[1]=10;
		ButtonSetPlay[2]=10;
		ButtonSetPlay[3]=10;
		ButtonSetPlay[4]=10;
		ButtonSetPlay[5]=10;
		ButtonSetPlay[6]=10;
		ButtonSetPlay[7]=10;
		ButtonSetPlay[8]=10;
	
	} else  if ( Record == TRUE ) {
		SAA7146_OutCom(COMTYPE_REC_PLAY, __Record, 2, CurrentRecordMode, 0);
		ButtonSetRecord[0]=1;
		ButtonSetRecord[1]=10;
		ButtonSetRecord[2]=10;
		ButtonSetRecord[3]=10;
		ButtonSetRecord[4]=10;
	} else SAA7146_OutCom( COMTYPE_REC_PLAY, __Stop, 0);
	return;
};





void  Debi_Record_Stop( void ) {
    int i;

    for ( i = 0; i < 5 ; i++ ) {
	    SAA7146_OutCom( COMTYPE_REC_PLAY, __Stop, 0);
	    ARM_ClearMailBox();
	    SAA7146_OutCom( COMTYPE_REC_PLAY, __Stop, 0);
	    ARM_ClearMailBox();
    };

    EnableMenuItem(GetMenu(hWnd),IDM_RECORD_START,MF_ENABLED);
    EnableMenuItem(GetMenu(hWnd),IDM_RECORD_PLAY,MF_ENABLED);
    EnableMenuItem(GetMenu(hWnd),IDM_PLAY_TIME_CONTROL,MF_GRAYED);
    EnableMenuItem(GetMenu(hWnd),IDM_PLAY_MARK_CONTROL,MF_GRAYED);
    EnableMenuItem(GetMenu(hWnd),IDM_PLAY_INFO_CONTROL,MF_GRAYED);

    Recorder_Pause = FALSE;
    Recorder_Slow = FALSE;
    Recorder_FF = FALSE;

    ButtonSetPlay[0]=10;
    ButtonSetPlay[1]=1;
    ButtonSetPlay[2]=1;
    ButtonSetPlay[3]=1;
    ButtonSetPlay[4]=1;
    ButtonSetPlay[5]=1;
    ButtonSetPlay[6]=1;
    ButtonSetPlay[7]=1;
    ButtonSetPlay[8]=1;

    ButtonSetRecord[0] = 10;
    ButtonSetRecord[1] = 1;
    ButtonSetRecord[2] = 1;
    ButtonSetRecord[3] = 1;
    ButtonSetRecord[4] = 1;




    for ( i=0; i < plugins_count; i++ ) {
        if ( Ext_Dll[i].Extern_RecPlay != NULL ) (Ext_Dll[i].Extern_RecPlay)(3);
    };


    if ( Record == TRUE ) {
        Record=FALSE;
	    if ( RecorderFile != -1 ) {
	    // Buffers flushen
	    if ( RecorderTyp == 0 ) {
                              Mpeg_Buffer[Mpeg_Buffer_Pos++]=0x00;
                              Mpeg_Buffer[Mpeg_Buffer_Pos++]=0x00;
                              Mpeg_Buffer[Mpeg_Buffer_Pos++]=0x01;
                              Mpeg_Buffer[Mpeg_Buffer_Pos++]=0xB9;
		    if ( Write_Buffer(&Mpeg_Buffer[0], Mpeg_Buffer_Pos ) < 0 ) return;

	    } else if ( RecorderTyp == 1 ) {
                              Mpeg_Buffer[Mpeg_Buffer_Pos++]=0x00;
                              Mpeg_Buffer[Mpeg_Buffer_Pos++]=0x00;
                              Mpeg_Buffer[Mpeg_Buffer_Pos++]=0x00;
                              Mpeg_Buffer[Mpeg_Buffer_Pos++]=0x00;
		    if ( Write_as_PAV_File(&Mpeg_Buffer[0], Mpeg_Buffer_Pos) < 0 )  return;
	    }

    };
    };


    if ( Play == TRUE ) {
        Play=FALSE;
    };

    if ( RecorderFile >=  0 ) close(RecorderFile);
    if ( AudioFilePtr >=  0 ) close(AudioFilePtr);
    if ( VideoFilePtr >=  0 ) close(VideoFilePtr);
    RecorderFile=-1;
    VideoFilePtr=-1;
    VideoFilePtr=-1;
    AudioBlocks=0;
    VideoBlocks=0;
    drop_ring_buf(recorder_buf);
    recorder_buf = NULL;
    Close_Rec_Play_Buffer();
}

BOOL Debi_Record_Play( void ) {
    char PESFile[255];
    int i=0;

    HeaderInfo.Bytesplay=0;
    RecorderFile=-1;

    while (( i < 9 ) && (RecorderFile == -1 )) {
    sprintf(PESFile,"%s\\%s",RecordBaseDir[i],RecorderName);
    RecorderFile=open(PESFile,_O_RDONLY | _O_BINARY);
    i++;
    };
    if ( RecorderFile < 0 ) return(FALSE);

    close(RecorderFile);
    RecorderFile=-1;
    if ( AudioFilePtr >=  0 ) close(AudioFilePtr);
    if ( VideoFilePtr >=  0 ) close(VideoFilePtr);
    AudioFilePtr=open(PESFile,_O_RDONLY | _O_BINARY);
    VideoFilePtr=open(PESFile,_O_RDONLY | _O_BINARY);
    AudioBlocks=0;
    VideoBlocks=0;

    EnableMenuItem(GetMenu(hWnd),IDM_RECORD_START,MF_GRAYED);
    EnableMenuItem(GetMenu(hWnd),IDM_PLAY_TIME_CONTROL,MF_ENABLED);
    EnableMenuItem(GetMenu(hWnd),IDM_PLAY_MARK_CONTROL,MF_ENABLED);
    EnableMenuItem(GetMenu(hWnd),IDM_PLAY_INFO_CONTROL,MF_ENABLED);

    ButtonSetPlay[0]=1;
    ButtonSetPlay[1]=10;
    ButtonSetPlay[2]=10;
    ButtonSetPlay[3]=10;
    ButtonSetPlay[4]=10;
    ButtonSetPlay[5]=10;
    ButtonSetPlay[6]=10;
    ButtonSetPlay[7]=10;
    ButtonSetPlay[8]=10;

    Play=TRUE;
    Recorder_Pause=FALSE;
    ShutDownFilters();
    //DVB_Reset();
    //Sleep(10);



    SAA7146_OutCom( COMTYPE_REC_PLAY, __Stop, 2,0,0);

    for ( i=0; i < plugins_count; i++ ) {
    if ( Ext_Dll[i].Extern_RecPlay != NULL ) (Ext_Dll[i].Extern_RecPlay)(1);
    };


    Init_Rec_Play_Buffer();

    if ( HeaderInfo.RecordFormat == 0 ) CurrentPlayMode=AudioMPG;
    else if ( HeaderInfo.RecordFormat == 1 ) CurrentPlayMode=VideoMPG;
    else CurrentPlayMode=AV_MPG;
    SAA7146_AudioCom(0x0E);
    SAA7146_AudioCom(0x01);
    SAA7146_OutCom( COMTYPE_REC_PLAY, __Play, 2, CurrentPlayMode, 0);
    SAA7146_AudioCom(0x0E);
    SAA7146_AudioCom(0x01);
    return(TRUE);
// Should test the following sequence :
//          CHECK(ioctl(fd_video, VIDEO_SET_BLANK, true));
//          CHECK(ioctl(fd_audio, AUDIO_SELECT_SOURCE, AUDIO_SOURCE_MEMORY));
//          CHECK(ioctl(fd_audio, AUDIO_SET_AV_SYNC, PlayMode == pmAudioVideo));
//   = SAA7146_AudioCom(PlayMode == pmAudioVideo ? 0x0F : 0x0E);
//          CHECK(ioctl(fd_video, VIDEO_SELECT_SOURCE, VIDEO_SOURCE_MEMORY));
//          CHECK(ioctl(fd_audio, AUDIO_STOP, true));
//          CHECK(ioctl(fd_video, VIDEO_STOP, true));
//   = SAA7146_OutCom( COMTYPE_REC_PLAY, __Stop, 0);
//          CHECK(ioctl(fd_video, VIDEO_CLEAR_BUFFER));
//          CHECK(ioctl(fd_audio, AUDIO_CLEAR_BUFFER));
//          CHECK(ioctl(fd_audio, AUDIO_PLAY));
//          CHECK(ioctl(fd_video, VIDEO_PLAY));
//   = SAA7146_OutCom( COMTYPE_REC_PLAY, __Play, 2, CurrentPlayMode, 0);
//   = SAA7146_AudioCom(0x01);

}


BOOL Is_Valid_Extension(LPSTR Name)
{
	int i;
	char MyExt[128];
    char Ext[5];

	i=strlen(Name);
	if ( i < 4 ) return(FALSE);
	i-=4;
	Ext[0]=Name[i];
	Ext[1]=Name[i+1];
	Ext[2]=Name[i+2];
	Ext[3]=Name[i+3];
    Ext[4]=0x00;

	strcpy(MyExt,".mpg.MPG.pva.PVA");

	if ( strstr(MyExt,Ext) != NULL ) return(TRUE);
    return(FALSE);
};


int Get_Button_Click(int OffsetY, int x,int y)
{
	int B[10];
	int i;

	B[0]=7;
	B[1]=66;
	B[2]=125;
	B[3]=184;
	B[4]=243;
	B[5]=302;
	B[6]=361;
	B[7]=420;
	B[8]=479;
	B[9]=538;


	if ( y < OffsetY ) return(-1);
	if ( y > OffsetY+25 ) return(-1);
	
    for ( i=0; i < 10; i++ ) {
		if (( x >= B[i]-2 ) && ( x <= B[i]+51)) return(i);
	};
   return(-1);
};

void Set_Win_Buttons(int OffsetY,unsigned short *Bset ,HDC MemDC)
{
	int x,y,Bpos;
	int i,j;
	unsigned int CVal;
	int a;
	int B[10];
	B[0]=7;
	B[1]=66;
	B[2]=125;
	B[3]=184;
	B[4]=243;
	B[5]=302;
	B[6]=361;
	B[7]=420;
	B[8]=479;


	for ( Bpos = 0 ; Bpos < 9 ; Bpos++ ) {
	if ( Bset[Bpos] == 10 ) CVal=0x0000ff00;
     else if ( Bset[Bpos] == 1 ) CVal = 0x00848284;
	  else if ( Bset[Bpos] == 11 ) CVal = 0x0000ffff;

	for ( j=0; j < 23 ; j++ ) {
	for ( i=0; i < 47 ; i++ ) {
	  y=(OffsetY+j);
	  x=(B[Bpos]+i);
      a=GetPixel(MemDC,x,y);
	  if (( a == 0x0000FF00 ) || ( a == 0x00848284 ) || ( a == 0x00888488 )|| ( a == 0x0000FFFF )) {
		  SetPixel(MemDC,x,y,CVal);
	  };
	};
	};
	}
};




//// OSD_RECORDER ////

void OSD_Recorder_Info( void )
{

    int ListCount=0;
    char ListText[128][128];
    int i,j,k;
    char LineText[128][128];

    int cPos;
    unsigned char	OutBuffer[24];

    double XStep;
    unsigned int XSize;
    int TopLine=0;
    int Lines;
    int MaxY;
    int Selection=0;
    int AltSelection=-1;
    int l;

    long hFile;
    struct _finddata_t play_file;
    char Maske[256];
    int Top=0;                      

    unsigned short ButtonSetMark[10] = { 1,1,1,1,1,1,10,10,10 };
    unsigned short AltButtonSetMark[10];

    unsigned short ButtonSetTime[10] = { 1,1,1,1,1,1,10,10,10 };
    unsigned short AltButtonSetTime[10];

    int s,m,s1,m1;
    unsigned int TimeStamp;

    char Zeile[60];
    BITMAPINFO          *TESTBITMAP                    = NULL;
    HGLOBAL hGlobal;
    unsigned short ch, cl;
    unsigned int yuv;
    char Titel[64];

     PROGRAMM_OSD=TRUE;

	 if ( Play == TRUE ) goto PLAY_MENU;
     if ( Record == TRUE ) goto RECORD_MENU;
	 
	 if (( Play == FALSE ) && ( Record == FALSE )) {
	 ListCount=2;
     strcpy(&ListText[0][0],t("Play"));
     strcpy(&ListText[1][0],t("Record"));

	 
	 CreateOSDWindow( 1, 1, 360, (unsigned short)(((ListCount+2)*28)+8));
     MoveWindowAbs(1,200,100 );
	 SetColorBlend(1);
	 OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
	 OSDSetColor(1,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
	 OSDSetColor(1,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
	 OSDSetColor(1,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);

	

	DrawBlock(1, 0, 0,360, 30,0);
    SetFont( 1, 1, 2, 0);
    strcpy(Zeile,t("Digitaler Video Recorder"));
	WriteText( 1, 40, 1,Zeile);
	DrawBlock(1, 0, (unsigned short)(((ListCount+1)*28)+6),360, 30,0);
    strcpy(Zeile,t("<2><8> OK <5> Ende <0>"));
	WriteText( 1, 30,(unsigned short)(((ListCount+1)*28)+6),Zeile);
	DrawBlock(1, 0, 30,360,(unsigned short)((ListCount*28)+4),1);
    SetFont( 1, 1, 3, 1);
		 for ( i=0; i < ListCount; i++ ) {
			WriteText( 1, 60,  (unsigned short)(((i+1)*28)+4)  ,ListText[i] );
		 };

		 for (;;) {

			 if (( AltSelection >= 0 ) && ( AltSelection < ListCount )) {
            		 DrawBlock(1, 0, (unsigned short)(((AltSelection+1)*28)+4),360, 28,1);
                     SetFont( 1, 1, 3, 1);
					 WriteText( 1, 60,  (unsigned short)(((AltSelection+1)*28)+5) ,ListText[AltSelection]);
			 };

			 if (( Selection >= 0 ) && ( Selection < ListCount )) {
				     AltSelection=Selection;
            		 DrawBlock(1, 5, (unsigned short)(((Selection+1)*28)+5),350, 27,3-1);
                     SetFont( 1, 1, 0, 3-1);
					 WriteText( 1, 60,  (unsigned short)(((Selection+1)*28)+5),ListText[Selection]);
			 };

			 j=0;
			while ( j == 0 ) {
			 ResetEvent(OSD_KEY_EVENT);
			i=WaitForSingleObject (OSD_KEY_EVENT, INFINITE);
			 if ( i != WAIT_OBJECT_0 ) {
                    DestroyOSDWindow(1);
                    Osd_Thread_Down();
                    return;
			 };
			 
			 if ( OSD_Key_Value == 0x00 ) {; 
                    DestroyOSDWindow(1);
                    Osd_Thread_Down();
                    return;
			 };

             if ( OSD_Key_Value == '5' ) {; 
			       DestroyOSDWindow(1);
                   if ( Selection == 0 ) goto PLAY_MENU; 
                   if ( Selection == 1 ) goto RECORD_MENU; 
				   return; 
			 };

             
			 if ( OSD_Key_Value == '0' ) {; 
                    DestroyOSDWindow(1);
                    Osd_Thread_Down();
                    return;
			 };

             if ( OSD_Key_Value == '2' ) {; 
			 if ( Selection < ListCount-1 ) {
				  Selection++;
				  j=1;
			 };
			 } else if ( OSD_Key_Value == '1' ) {
				 if ( Selection < ListCount-1 ) {
				   Selection=ListCount-1;
				   j=1;
			 };
			 } else if ( OSD_Key_Value == '7' ) { 
			 if ( Selection > 0) {
				 Selection=0;
				 j=1;
			 };
			 } else if ( OSD_Key_Value == '8' ) {; 
			 if ( Selection > 0) {
				 Selection--;
				 j=1;
			 };
			};

			}; // While ActiveSel;
        }
        return;
    }

PLAY_MENU:

    CreateOSDWindow( 2, (unsigned short)BITMAP4, 588, 32);
    MoveWindowAbs(2,20,400);

    OSDSetColor(2,1, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
    OSDSetColor(2,2, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
    OSDSetColor(2,3, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
    OSDSetColor(2,4, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);

    hGlobal=LoadResource(hInst,FindResource(hInst,"OSDPLAYBITMAP",RT_BITMAP));
    TESTBITMAP=(BITMAPINFO*)LockResource(hGlobal);

    CreateOSDWindow( 1, (unsigned short)BITMAP4, (unsigned short)TESTBITMAP->bmiHeader.biWidth, (unsigned short)TESTBITMAP->bmiHeader.biHeight);
    MoveWindowAbs(1,20,440);
    SetColorBlend(1);
    yuv=RGB2YUV(0,0,0);
    cl=(yuv&0xffff);
    ch=((yuv>>16)&0xffff);
    SetColor_(1, 16,0, ch, cl);
    SetBlend_(1,16,0, 0);
    DrawBlock(1, 0, 0,(unsigned short)TESTBITMAP->bmiHeader.biWidth, (unsigned short)TESTBITMAP->bmiHeader.biHeight,0);
    DrawBlock(2, 0, 0,(unsigned short)TESTBITMAP->bmiHeader.biWidth, 32,1);
    SetFont( 2, 1, 3, 1);
    strcpy(Titel,"  Lade Bitmap  ");
    WriteText(2,20,2,Titel);

    if ( OSDLoadBitmap4(1,TESTBITMAP,0,0,0,0,0,0,2000) < 0 ) {
	    strcpy(Titel,"* Fehler beim Bitmap laden *");
        WriteText(2,20,2,Titel);
	    DeleteObject(TESTBITMAP);
        Sleep(2000);
        UnlockResource(hGlobal);
        DestroyOSDWindow(1);
        DestroyOSDWindow(2);
        Osd_Thread_Down();
        return;
    };

OSD_SET_BUTTON_PLAY:


    for ( i=0; i < 9 ; i++ ) {
	    if ( ButtonSetPlay[i] == 1 ) {
		    yuv=RGB2YUV(OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B);
            j=OSD_Colors[0].Blend;
	    } else if ( ButtonSetPlay[i] == 10  ) {
		    yuv=RGB2YUV(0,255,0);
            j=255;
	    } else if ( ButtonSetPlay[i] == 11  ) {
	  		    yuv=RGB2YUV(255,255,0);
                j=255;
	    } 
		    
		        cl=(yuv&0xffff);
			    ch=((yuv>>16)&0xffff);
			    SetColor_(1, 16,(unsigned short)(i+1), ch, cl);
			    SetBlend_(1,16,(unsigned short)(i+1), (unsigned char)j);
    };		






    DrawBlock(2, 0, 0,(unsigned short)TESTBITMAP->bmiHeader.biWidth, 32,1);






    if ( Play == TRUE ) {
    i=0;
    while (( i < 60 ) && ( HeaderInfo.Infos[i] >= 0x20 )) {
    Titel[i]=HeaderInfo.Infos[i];
    i++;
    Titel[i]=0x00;
    };

    if ( i== 0 ) strcpy(Titel,t("<Unbekannt>"));
    } else { 
	    strcpy(Titel,t("<Keine Datei geladen>"));
    };

    WriteText(2,20,2,Titel);

				     if ( Play == TRUE ) {
						    m=(HeaderInfo.CurrentSpielZeit/60);
						    s=HeaderInfo.CurrentSpielZeit-(m*60);
						    sprintf(Zeile,"%3d:%02d",m,s);
                            DrawBlock(2,(unsigned short)(TESTBITMAP->bmiHeader.biWidth-100), 0,100, 32,1);
                            WriteText(2,(unsigned short)(TESTBITMAP->bmiHeader.biWidth-100),2,Zeile);
				     }


    if ( ButtonSetPlay[5]== 11 ) {
    Debi_Record_FastFF();
    };
				     
    if ( ButtonSetPlay[3]== 11 ) {
    Update_MDI_File();
    ButtonSetPlay[3]=10;
    goto OSD_SET_BUTTON_PLAY;
    };
    OSD_Key_Value=0x20;
    ResetEvent(OSD_KEY_EVENT);

    for (;;) {

             i=WaitForSingleObject (OSD_KEY_EVENT, 1000);
			 
			 if ( i == WAIT_TIMEOUT ) {

				 if (( Play == TRUE ) && ( Recorder_Pause == FALSE )) {
						m=(HeaderInfo.CurrentSpielZeit/60);
						s=HeaderInfo.CurrentSpielZeit-(m*60);
						sprintf(Zeile,"%3d:%02d",m,s);
                        DrawBlock(2,(unsigned short)(TESTBITMAP->bmiHeader.biWidth-100), 0,100, 32,1);
                        WriteText(2,(unsigned short)(TESTBITMAP->bmiHeader.biWidth-100),2,Zeile);
				 }
					 continue;
			 };

			 
			 if ( i != WAIT_OBJECT_0 ) {
        			DeleteObject(TESTBITMAP);
                    UnlockResource(hGlobal);
                    DestroyOSDWindow(1);
                    DestroyOSDWindow(2);
                    Osd_Thread_Down();
                    return;
			 };
             ResetEvent(OSD_KEY_EVENT);
			 
			 if ( OSD_Key_Value == 0x00 ) {; 
        			DeleteObject(TESTBITMAP);
                    UnlockResource(hGlobal);
			        DestroyOSDWindow(1);
                    DestroyOSDWindow(2);
                    Osd_Thread_Down();
                    return;
			 };


			 if ( OSD_Key_Value == '0' ) {; 
        			DeleteObject(TESTBITMAP);
                    UnlockResource(hGlobal);
			        DestroyOSDWindow(1);
                    DestroyOSDWindow(2);
                    Osd_Thread_Down();
                    return;
			 };


			 if ( OSD_Key_Value == '1' ) {; 
				 if ( ButtonSetPlay[0] < 10 ) continue;
			 if ( Play == FALSE ) {
        			DeleteObject(TESTBITMAP);
                    UnlockResource(hGlobal);
			        DestroyOSDWindow(1);
                    DestroyOSDWindow(2);
                	Selection=0;
                    Top=0;                      
                	AltSelection=-1;
					goto MENU_LOAD_FILE;
			 } else {
				 Debi_Record_Play_Normal();
			     Sleep(1);
				 goto OSD_SET_BUTTON_PLAY;
			 };
			 };
			 
			 if ( OSD_Key_Value == '2' ) {
				 if ( ButtonSetPlay[1] < 10 ) continue;
				 Debi_Record_Pause();
			     goto OSD_SET_BUTTON_PLAY;
			 };

			 if ( OSD_Key_Value == '3' ) {; 
				 if ( ButtonSetPlay[2] < 10 ) continue;
				 Play_Stop();

				 goto OSD_SET_BUTTON_PLAY;
			 };

 			 if ( OSD_Key_Value == '4' ) {
				 if ( ButtonSetPlay[3] < 10 ) continue;
				 Insert_Sprungmarke(HeaderInfo.CurrentSpielZeit);
				 Update_MDI_File();
                 ButtonSetPlay[3]=11;
				 goto OSD_SET_BUTTON_PLAY;
			 };

			 if ( OSD_Key_Value == '5' ) {
				 if ( ButtonSetPlay[4] < 10 ) continue;
				 Debi_Record_SlowFF();
                 Sleep(1);Sleep(0);Sleep(1);Sleep(0);
				 goto OSD_SET_BUTTON_PLAY;
			 };

 			 if ( OSD_Key_Value == '6' ) {
				 if ( ButtonSetPlay[5] < 10 ) continue;
				 if ( Recorder_FF == FALSE ) { 	 
				 	ButtonSetPlay[0]=10;
                	ButtonSetPlay[1]=1;
					ButtonSetPlay[2]=10;
					ButtonSetPlay[3]=1;
					ButtonSetPlay[4]=1;
					ButtonSetPlay[5]=11;
					ButtonSetPlay[6]=1;
					ButtonSetPlay[7]=1;
					ButtonSetPlay[8]=1;
				 } else Debi_Record_FastFF();

                 Sleep(1);Sleep(0);Sleep(1);Sleep(0);
                 Sleep(1);Sleep(0);Sleep(1);Sleep(0);
                 Sleep(1);Sleep(0);Sleep(1);Sleep(0);
				 goto OSD_SET_BUTTON_PLAY;
			 };

             if ( OSD_Key_Value == '7' ) { 
				 if ( ButtonSetPlay[6] < 10 ) continue;
        			DeleteObject(TESTBITMAP);
                    UnlockResource(hGlobal);
 			        DestroyOSDWindow(1);
                    DestroyOSDWindow(2);
                   goto TIME_MENU;
			 };

            if ( OSD_Key_Value == '8' ) { 
				 if ( ButtonSetPlay[7] < 10 ) continue;
        			DeleteObject(TESTBITMAP);
                    UnlockResource(hGlobal);
			        DestroyOSDWindow(1);
                    DestroyOSDWindow(2);
                   goto MARK_MENU;
			 };

			 if ( OSD_Key_Value == '9' ) { 
				 if ( ButtonSetPlay[8] < 10 ) continue;
        			DeleteObject(TESTBITMAP);
                    UnlockResource(hGlobal);
  			        DestroyOSDWindow(1);
                    DestroyOSDWindow(2);
                   goto INFO_MENU;
			 };
             


    };

MENU_LOAD_FILE:
	CreateOSDWindow( 1, 0, 500, 400);
    MoveWindowAbs(1,110,100 );
    SetColorBlend(1);
    OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
    OSDSetColor(1,1, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);

	DrawBlock(1, 0, 0,500, 28,1);
	DrawBlock(1, 0, 372,500, 28,1);
    SetFont( 1, 1, 2, 1);

	strcpy(Zeile,t("DVR - Datei auswählen"));
	WriteText( 1, 140, 0,Zeile);
	strcpy(Zeile,t("<4> Info  <5> Start <0> Zurück"));
	WriteText( 1, 80, 372,Zeile);

    SetFont( 1, 3, 1, 2);

    ListCount=0;

	sprintf(Maske,"%s\\*",RecordBaseDir[0]);
	hFile = _findfirst( Maske, &play_file );

	if ( hFile != 0 ) {
	while( _findnext( hFile, &play_file ) == 0 )
            {
		      if (Is_Valid_Extension(play_file.name) == TRUE ) {
                     strncpy(&ListText[ListCount][0],play_file.name,128);
                     ListText[ListCount][127]=0x00;
                     ListCount++;
			  };
	};
       _findclose( hFile );
	};
      AltSelection=-1;

Rebuild_Liste:

    DrawBlock(1, 5, 28,490,337,0);
    SetFont( 1, 1, 1, 0);
	i=0;
	while (( i < 12 ) && ( i+Top < ListCount )) {
	 WriteText( 1, 20,  (unsigned short)(((i+1)*28)+5) ,ListText[Top+i]);
     i++;
	};



	 for (;;) {

			 if (( AltSelection >= 0 ) && ( AltSelection < ListCount )) {
            		 DrawBlock(1, 5, (unsigned short)(((AltSelection+1)*28)+4),490, 28,0);
                     SetFont( 1, 1, 1, 0);
					 WriteText( 1, 20,  (unsigned short)(((AltSelection+1)*28)+5) ,ListText[AltSelection+Top]);
			 };

			 if (( Selection >= 0 ) && ( Selection < ListCount )) {
				     AltSelection=Selection;
            		 DrawBlock(1, 5, (unsigned short)(((Selection+1)*28)+5),490, 27,1);
                     SetFont( 1, 1, 0, 1);
					 WriteText( 1, 20,  (unsigned short)(((Selection+1)*28)+5),ListText[Selection+Top]);
			 };

			 j=0;
			while ( j == 0 ) {
			 ResetEvent(OSD_KEY_EVENT);
  			 i=WaitForSingleObject (OSD_KEY_EVENT, INFINITE);

			 if ( i != WAIT_OBJECT_0 ) {
                    DestroyOSDWindow(1);
                    Osd_Thread_Down();
                    return;
			 };
			 
			 if ( OSD_Key_Value == 0x00 ) {; 
                    DestroyOSDWindow(1);
                    Osd_Thread_Down();
                    return;
			 };

             if ( OSD_Key_Value == '4' ) { 
			       DestroyOSDWindow(1);
               	   sprintf(Maske,"%s\\%s",RecordBaseDir[0],ListText[Selection+Top]);
                   Prozess_Header_Info(Maske);
				   goto INFO_MENU; 
				   };
     

             if ( OSD_Key_Value == '5' ) {
               	   sprintf(Maske,"%s\\%s",RecordBaseDir[0],ListText[Selection+Top]);
                   if ( Prozess_Header_Info(Maske) == TRUE ) {
			       DestroyOSDWindow(1);
				   for ( i=0; i < 10 ; i++ ) {
					   Sleep(0);Sleep(0);Sleep(1);Sleep(0);
				   };
				   Debi_Record_Play();
				   for ( i=0; i < 100 ; i++ ) {
					   Sleep(0);Sleep(0);Sleep(1);Sleep(0);
				   };
					 goto PLAY_MENU; 
					 return;
				   };
                   continue;
			 };

             
			 if ( OSD_Key_Value == '0' ) {
                    DestroyOSDWindow(1);
                    goto PLAY_MENU;
                    return;
			 };

             if ( OSD_Key_Value == '2' ) { 
			 if ( Top+Selection < ListCount-1 ) {
				  Selection++;
				  if ( Selection > 11 ) {
					  Top+=12;
					  Selection=0;
					  AltSelection=-1;
					  goto Rebuild_Liste;
				  };
				  j=1;
			 };
			 } else if ( OSD_Key_Value == '1' ) {
				 if ( Selection+Top < ListCount-1 ) {
				     Top=(ListCount-1)/12;
                     Top=Top*12;
					 Selection=(ListCount-1)-Top;
    				 goto Rebuild_Liste;
			 };
			 } else if ( OSD_Key_Value == '7' ) { 
			 if ( Selection+Top > 0) {
				 Selection=0;
				 Top=0;
   				 goto Rebuild_Liste;
			 };
			 } else if ( OSD_Key_Value == '8' ) { 
			 if ( Selection > 0) {
				 Selection--;
				 j=1;
			 } else if ( Top > 0 ) { 
				 Top-=12;
                 Selection=11;
   				 goto Rebuild_Liste;
			 };
			 } else if ( OSD_Key_Value == '3' ) { 
                i=(ListCount-1)/12;
				i=i*12;
    			Selection=(ListCount-1)-Top;
				if ( Top < i ) {
					 Top+=12;
					 Selection=(ListCount-1)-Top;
				     if ( Selection > 11 ) Selection = 11;
   					 goto Rebuild_Liste;
				};
			 } else if ( OSD_Key_Value == '3' ) {; 
				  Selection=0;
			 if ( Top > 0 ) {
				  Top -=12;
   					 goto Rebuild_Liste;
				};
			 }
			}; // While ActiveSel;
    }

INFO_MENU:

	CreateOSDWindow( 1, 0, 600, 400);
    MoveWindowAbs(1,60,100 );
    SetColorBlend(1);
    OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
    OSDSetColor(1,1, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);

	DrawBlock(1, 0, 0,600, 28,1);
	DrawBlock(1, 0, 372,600, 28,1);
    SetFont( 1, 1, 2, 1);

	strcpy(Zeile,t("DVR - Datei Infos"));
	WriteText( 1, 190, 0,Zeile);
	if (( Play == FALSE ) && ( Record == FALSE )) {
	strcpy(Zeile,t("<4> Auswahl  <5> Start <0> Zurück"));
	WriteText( 1, 120, 372,Zeile);
	} else {
	strcpy(Zeile,t("<0> Zurück"));
	WriteText( 1, 220, 372,Zeile);
	}

	SetFont( 1, 1, 1, 0);
	strcpy(Zeile,t("Art"));
    WriteText(1,4,28,Zeile);
    WriteText(1,130,28,HeaderInfo.TypeName);

	strcpy(Zeile,t("Titel"));
    WriteText(1,4,56,Zeile);
	i=0;
	while (( i < 60 ) && ( HeaderInfo.Infos[i] >= 0x20 )) {
	Titel[i]=HeaderInfo.Infos[i];
	i++;
	Titel[i]=0x00;
    }; 
    if ( i== 0 ) strcpy(Titel,t("<Unbekannt>"));
	WriteText(1,130,56,Titel);


	strcpy(Zeile,t("KB/Dateien"));
    WriteText(1,4,84,Zeile);
	sprintf(Zeile,t("%d KB in %d Dateien"),HeaderInfo.KBTotal,HeaderInfo.Anzahl_Files);
    WriteText(1,130,84,Zeile);

	strcpy(Zeile,t("Aufnahme "));
    WriteText(1,4,112,Zeile);
	sprintf(Zeile,t("vom %s um %s Uhr "),HeaderInfo.AufnahmeDatum ,HeaderInfo.StartZeit);
    WriteText(1,130,112,Zeile);

	strcpy(Zeile,t("Dauer"));
    WriteText(1,4,140,Zeile);
	if (( Play == FALSE ) && ( Record == FALSE )) {
			m=(HeaderInfo.SpielZeit/60);
			s=HeaderInfo.SpielZeit-(m*60);
			sprintf(Zeile,"%d:%02d",m,s);
	} else if ( Play == TRUE ) {
			m=(HeaderInfo.SpielZeit/60);
			s=HeaderInfo.SpielZeit-(m*60);
			m1=(HeaderInfo.CurrentSpielZeit/60);
			s1=HeaderInfo.CurrentSpielZeit-(m1*60);
			sprintf(Zeile,t("%d:%02d     < %d:%02d gespielt>"),m,s,m1,s1);
	} else if ( Record == TRUE ) {
			m=(HeaderInfo.CurrentSpielZeit/60);
			s=HeaderInfo.CurrentSpielZeit-(m1*60);
			sprintf(Zeile,t("%d:%02d aufgenommen"),m,s);
	}
    WriteText(1,130,140,Zeile);
	DrawLine(1,0,168,600,0,1);


	TopLine=0;
    Lines=0;
	j=0;
	i=0;
	
	while ( HeaderInfo.Infos[i] != 0x00 ) {
		
		LineText[Lines][j] = HeaderInfo.Infos[i];


    	LineText[Lines][j+1]=0x00;
              if ( LineText[Lines][j] < 0x20 ) { 
					if ( LineText[Lines][j] == 0x0a ) {
					    LineText[Lines][j]=0x00;
                        if ( Lines < 2 ) {
						Lines++;
						j=-1;
						k=0;
						} else LineText[Lines][j]=0x20;

					} else {
						LineText[Lines][j]=0x00;
						j--;
                        k--;
					};
					};
						if ( k > 41 ) {
						if ( LineText[Lines][j]==0x20 ) {
							LineText[Lines][j]=0x00;
							Lines++;
							j=-1;
                            k=0;
					}
                    };
					k++;
					j++;
					i++;
				};
                
				LineText[Lines][j]=0x00;
				Lines++;
                LineText[Lines][0]=0x00;

NEW_LINES:
            	DrawBlock(1, 0, 170,600, 196,0);
		
				i=0;
				while (( i < 7 ) && ( TopLine+i < Lines )) {
					if ( strlen(LineText[TopLine+i]) > 0 ) {
						WriteText(1,5,(unsigned short)(170+(i*28)),LineText[TopLine+i]);
				
					}
					i++;
                };

	for (;;) {
		 ResetEvent(OSD_KEY_EVENT);
		i=WaitForSingleObject (OSD_KEY_EVENT, 1000);

			 if ( i == WAIT_TIMEOUT ) {
				 if ( Play == TRUE ) {
		      	m=(HeaderInfo.SpielZeit/60);
			    s=HeaderInfo.SpielZeit-(m*60);
		    	m1=(HeaderInfo.CurrentSpielZeit/60);
			    s1=HeaderInfo.CurrentSpielZeit-(m1*60);
			    sprintf(Zeile,t("%d:%02d     < %d:%02d gespielt>    "),m,s,m1,s1);
                WriteText(1,130,140,Zeile);
				 }
				 if ( Record == TRUE ) {
					m=(HeaderInfo.CurrentSpielZeit/60);
					s=HeaderInfo.CurrentSpielZeit-(m1*60);
					sprintf(Zeile,t("%d:%02d aufgenommen"),m,s);
                    WriteText(1,130,140,Zeile);
                 	sprintf(Zeile,t("%d KB in %d Dateien    "),HeaderInfo.KBTotal,HeaderInfo.Anzahl_Files);
                    WriteText(1,130,84,Zeile);
				 }
				 continue;
			 };
			 
			 if ( i != WAIT_OBJECT_0 ) {
                    DestroyOSDWindow(1);
                    Osd_Thread_Down();
                    return;
			 };
			 
			 if ( OSD_Key_Value == 0x00 ) {; 
                    DestroyOSDWindow(1);
                    Osd_Thread_Down();
                    return;
			 };


			 if ( OSD_Key_Value == '0' ) {
                    DestroyOSDWindow(1);
                    if ( Play == TRUE ) goto PLAY_MENU;
                    if ( Record == TRUE ) goto RECORD_MENU;
                    return;
			 };


 			 if ( OSD_Key_Value == '8' ) {
				 if ( TopLine > 0 ) {
					 TopLine--;
                     goto NEW_LINES;
				 };
				 };

 			 if ( OSD_Key_Value == '2' ) {
				 if ( TopLine+7 < Lines  ) {
					 TopLine++;
                     goto NEW_LINES;
				 };
				 };

 			 if ( OSD_Key_Value == '7' ) {
				 if ( TopLine != 0 ) {
					 TopLine=0;
                     goto NEW_LINES;
				 };
				 };

			 if ( OSD_Key_Value == '9' ) {
				 if ( TopLine > 0 ) {
					 TopLine-=7;
                     if ( TopLine < 0 ) TopLine=0;
                     goto NEW_LINES;
				 };
				 };

			 if ( OSD_Key_Value == '3' ) {
				 if ( TopLine+7 < Lines ) {
					 TopLine+=7;
                     goto NEW_LINES;
				 } else if ( TopLine+7 > Lines ) {
					 TopLine=Lines-7 ;
                     goto NEW_LINES;
				 };
				 };


 			 if ( OSD_Key_Value == '1' ) {
				 if ( TopLine < Lines-7  ) {
					 TopLine=Lines-7 ;
                     goto NEW_LINES;
				 };
				 };


             if ( OSD_Key_Value == '5' ) {; 
                   if ( (Play == TRUE ) || ( Record == TRUE ) ) continue; 
			       DestroyOSDWindow(1);
					 Play=TRUE;
				   for ( i=0; i < 10 ; i++ ) {
					   Sleep(0);Sleep(0);Sleep(1);Sleep(0);
				   };
				   Debi_Record_Play();
				   for ( i=0; i < 100 ; i++ ) {
					   Sleep(0);Sleep(0);Sleep(1);Sleep(0);
				   };
					 goto PLAY_MENU; 
			 };
             if ( OSD_Key_Value == '4' ) {; 
                   if ( (Play == TRUE ) || ( Record == TRUE ) ) continue; 
			       DestroyOSDWindow(1);
				   goto MENU_LOAD_FILE; 
			 };

			 };	
	

TIME_MENU:

    CreateOSDWindow( 2, (unsigned short)BITMAP4, 588, 32);
    MoveWindowAbs(2,20,400);
    SetColorBlend(2);

    OSDSetColor(2,1, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
    OSDSetColor(2,3, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);

    hGlobal=LoadResource(hInst,FindResource(hInst,"OSDTIMEBITMAP",RT_BITMAP));
    TESTBITMAP=(BITMAPINFO*)LockResource(hGlobal);

    CreateOSDWindow( 1, (unsigned short)BITMAP4, (unsigned short)TESTBITMAP->bmiHeader.biWidth, (unsigned short)TESTBITMAP->bmiHeader.biHeight);
    MoveWindowAbs(1,20,440);

    AltButtonSetTime[0]=0x00;

    if ( HeaderInfo.CurrentSpielZeit >= 600 ) ButtonSetTime[0]=10; 
      else ButtonSetTime[0]=1;
    if ( HeaderInfo.CurrentSpielZeit >= 300 ) ButtonSetTime[1]=10; 
      else ButtonSetTime[1]=1;
    if ( HeaderInfo.CurrentSpielZeit >= 180 ) ButtonSetTime[2]=10; 
      else ButtonSetTime[2]=1;
    if ( HeaderInfo.CurrentSpielZeit+180 < HeaderInfo.SpielZeit) ButtonSetTime[3]=10;
      else ButtonSetTime[3]=1;
    if ( HeaderInfo.CurrentSpielZeit+300 < HeaderInfo.SpielZeit) ButtonSetTime[4]=10;
      else ButtonSetTime[4]=1;
    if ( HeaderInfo.CurrentSpielZeit+600 < HeaderInfo.SpielZeit) ButtonSetTime[5]=10;
      else ButtonSetTime[5]=1;

    SetColorBlend(1);

    yuv=RGB2YUV(0,0,0);
    cl=(yuv&0xffff);
    ch=((yuv>>16)&0xffff);
    SetColor_(1, 16,0, ch, cl);
    SetBlend_(1,16,0, 0);
    DrawBlock(1, 0, 0,(unsigned short)TESTBITMAP->bmiHeader.biWidth, (unsigned short)TESTBITMAP->bmiHeader.biHeight,0);
    DrawBlock(2, 0, 0,(unsigned short)TESTBITMAP->bmiHeader.biWidth, 32,1);
    SetFont( 2, 1, 3, 1);
    strcpy(Titel,"  Lade Bitmap  ");
    WriteText(2,20,2,Titel);

    if ( OSDLoadBitmap4(1,TESTBITMAP,0,0,0,0,0,0,2000) < 0 ) {
	    strcpy(Titel,"* Fehler beim Bitmap laden *");
        WriteText(2,20,2,Titel);
	    DeleteObject(TESTBITMAP);
        Sleep(2000);
        UnlockResource(hGlobal);
        DestroyOSDWindow(1);
        DestroyOSDWindow(2);
        Osd_Thread_Down();
        return;
    };

    OSD_SET_BUTTON_TIMER:
    if ( memcmp(ButtonSetTime,AltButtonSetTime,20) != 0 ) {
    memcpy(AltButtonSetTime,ButtonSetTime,20);

    for ( i=0; i < 9 ; i++ ) {
	    if ( ButtonSetTime[i] == 1 ) {
		    yuv=RGB2YUV(OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B);
            j=OSD_Colors[0].Blend;
	    } else if ( ButtonSetTime[i] == 10  ) {
		    yuv=RGB2YUV(0,255,0);
            j=255;
	    } else if ( ButtonSetTime[i] == 11  ) {
	  		    yuv=RGB2YUV(255,255,0);
                j=255;
	    } 
		    
		        cl=(yuv&0xffff);
			    ch=((yuv>>16)&0xffff);
			    SetColor_(1, 16,(unsigned short)(i+1), ch, cl);
			    SetBlend_(1,16,(unsigned short)(i+1), (unsigned char)j);
    };		

    };

    l=100;

    DrawBlock(2, 0, 0,(unsigned short)TESTBITMAP->bmiHeader.biWidth, 32,1);

    if ( Play == TRUE ) {
    i=0;
    while (( i < 60 ) && ( HeaderInfo.Infos[i] >= 0x20 )) {
    Titel[i]=HeaderInfo.Infos[i];
    i++;
    Titel[i]=0x00;
    };

    if ( i== 0 ) strcpy(Titel,t("<Unbekannt>"));
    } ;

    WriteText(2,20,2,Titel);


    for (;;) {
            OSD_Key_Value = 0x20;
			 ResetEvent(OSD_KEY_EVENT);
             i=WaitForSingleObject (OSD_KEY_EVENT, 10);

			 if ( i == WAIT_TIMEOUT ) {
                    l++;
					if ( HeaderInfo.CurrentSpielZeit >= 600 ) ButtonSetTime[0]=10; 
					  else ButtonSetTime[0]=1;
					if ( HeaderInfo.CurrentSpielZeit >= 300 ) ButtonSetTime[1]=10; 
					  else ButtonSetTime[1]=1;
					if ( HeaderInfo.CurrentSpielZeit >= 180 ) ButtonSetTime[2]=10; 
					  else ButtonSetTime[2]=1;
					if ( HeaderInfo.CurrentSpielZeit+180 < HeaderInfo.SpielZeit) ButtonSetTime[3]=10;
					  else ButtonSetTime[3]=1;
					if ( HeaderInfo.CurrentSpielZeit+300 < HeaderInfo.SpielZeit) ButtonSetTime[4]=10;
					  else ButtonSetTime[4]=1;
					if ( HeaderInfo.CurrentSpielZeit+600 < HeaderInfo.SpielZeit) ButtonSetTime[5]=10;
					  else ButtonSetTime[5]=1;

					  if (memcmp(ButtonSetTime,AltButtonSetTime,20) != 0 ) goto OSD_SET_BUTTON_TIMER;

					  if ( l >= 100 ) {
						l=0;
					    m=(HeaderInfo.CurrentSpielZeit/60);
						s=HeaderInfo.CurrentSpielZeit-(m*60);
						sprintf(Zeile,"%3d:%02d",m,s);
                        DrawBlock(2,(unsigned short)(TESTBITMAP->bmiHeader.biWidth-100), 0,100, 32,1);
                        WriteText(2,(unsigned short)(TESTBITMAP->bmiHeader.biWidth-100),2,Zeile);
					  };
						continue;
			 };

			 
			 if ( i != WAIT_OBJECT_0 ) {
        		   DeleteObject(TESTBITMAP);
                    DestroyOSDWindow(1);
 			        DestroyOSDWindow(2);
                    Osd_Thread_Down();
                    return;
			 };

			 if ( OSD_Key_Value == 0x00 ) {; 
        		   DeleteObject(TESTBITMAP);
                    DestroyOSDWindow(1);
 			        DestroyOSDWindow(2);
                    Osd_Thread_Down();
                    return;
			 } else if ( OSD_Key_Value == '0' ) {; 
        		   DeleteObject(TESTBITMAP);
                    DestroyOSDWindow(1);
			       DestroyOSDWindow(2);
                    Osd_Thread_Down();
                    return;
			 } else if ( OSD_Key_Value == '1' ) { 
				 if ( ButtonSetTime[0] < 10 ) continue;
                   Jump_Time_Play(HeaderInfo.CurrentSpielZeit-600);
			 } else if ( OSD_Key_Value == '2' ) { 
				 if ( ButtonSetTime[1] < 10 ) continue;
                   Jump_Time_Play(HeaderInfo.CurrentSpielZeit-300);
			 } else if ( OSD_Key_Value == '3' ) { 
				 if ( ButtonSetTime[2] < 10 ) continue;
                   Jump_Time_Play(HeaderInfo.CurrentSpielZeit-180);
			 } else if ( OSD_Key_Value == '4' ) { 
				 if ( ButtonSetTime[3] < 10 ) continue;
                   Jump_Time_Play(HeaderInfo.CurrentSpielZeit+180);
			 } else if ( OSD_Key_Value == '5' ) { 
				 if ( ButtonSetTime[4] < 10 ) continue;
                   Jump_Time_Play(HeaderInfo.CurrentSpielZeit+300);
			 } else if ( OSD_Key_Value == '6' ) { 
				 if ( ButtonSetTime[5] < 10 ) continue;
                   Jump_Time_Play(HeaderInfo.CurrentSpielZeit+600);
             } else if ( OSD_Key_Value == '7' ) { 
				 if ( ButtonSetTime[6] < 10 ) continue;
        		   DeleteObject(TESTBITMAP);
			       DestroyOSDWindow(2);
			       DestroyOSDWindow(1);
                   goto PLAY_MENU;
			 } else if ( OSD_Key_Value == '8' ) { 
				 if ( ButtonSetTime[7] < 10 ) continue;
        		   DeleteObject(TESTBITMAP);
			       DestroyOSDWindow(1);
			       DestroyOSDWindow(2);
                   goto MARK_MENU;
			 } else if ( OSD_Key_Value == '9' ) { 
                    DrawBlock(2,(unsigned short)( TESTBITMAP->bmiHeader.biWidth-110),0 ,110, 32,1);
				    strcpy(OutBuffer,"___:__");
				    cPos=0;
		            i=OSD_Edit_Ascii_Mask(2,TESTBITMAP->bmiHeader.biWidth-100,0,6,&cPos,OutBuffer,3,1);
                    if ( i == 13 ) {
                       i=1;
					   sscanf(OutBuffer,"%d:%d",&m,&s);
					   TimeStamp=m*60+s;
					   if ( TimeStamp < HeaderInfo.SpielZeit ) Jump_Time_Play(TimeStamp);

					}
                    DrawBlock(2, (unsigned short)(TESTBITMAP->bmiHeader.biWidth-110),0 ,110, 32,1);
					continue;
			 };
             
    };



MARK_MENU:

    AltButtonSetMark[0]=0x00;

				 if ( HeaderInfo.SprungMarken[0] != 0xffffffff ) {
                        ButtonSetMark[0]=10;
						ButtonSetMark[5]=10;

						if ( Check_Mark(-1)== TRUE ) {
                           ButtonSetMark[2]=10;
          				   if ( Check_Mark(-2) == TRUE ) ButtonSetMark[1]=10;
		                     else ButtonSetMark[1]=1;
						} else {
                           ButtonSetMark[1]=1;
                           ButtonSetMark[2]=1;
						};
						if ( Check_Mark(1) == TRUE ) {
                           ButtonSetMark[3]=10;
          				   if ( Check_Mark(2) == TRUE ) ButtonSetMark[4]=10;
		                     else ButtonSetMark[4]=1;
						} else {
                           ButtonSetMark[3]=1;
                           ButtonSetMark[4]=1;
						}
				 } else { 
                        ButtonSetMark[0]=1;
						ButtonSetMark[1]=1;
                        ButtonSetMark[2]=1;
						ButtonSetMark[3]=1;
                        ButtonSetMark[4]=1;
                        ButtonSetMark[5]=1;
				 }



    CreateOSDWindow( 2, (unsigned short)BITMAP4, 588, 32);
    MoveWindowAbs(2,20,400);
    SetColorBlend(2);

    OSDSetColor(2,1, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
    OSDSetColor(2,2, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
    OSDSetColor(2,3, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
    OSDSetColor(2,4, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);

    CreateOSDWindow( 3, (unsigned short)BITMAP4, 588, 18);
    MoveWindowAbs(3,20,470);
    SetColorBlend(3);

    yuv=RGB2YUV(0,0,0);
    cl=(yuv&0xffff);
    ch=((yuv>>16)&0xffff);
    SetColor_(3, 16,0, ch, cl);
    SetBlend_(3,16,0, 0);

    yuv=RGB2YUV(255,255,0);
    cl=(yuv&0xffff);
    ch=((yuv>>16)&0xffff);
    SetColor_(3, 16,1, ch, cl);
    SetBlend_(3,16,1, 255);

    yuv=RGB2YUV(255,0,0);
    cl=(yuv&0xffff);
    ch=((yuv>>16)&0xffff);
    SetColor_(3, 16,2, ch, cl);
    SetBlend_(3,16,2, 255);

    yuv=RGB2YUV(255,255,255);
    cl=(yuv&0xffff);
    ch=((yuv>>16)&0xffff);
    SetColor_(3, 16,3, ch, cl);
    SetBlend_(3,16,3, 255);
    OSDSetColor(3,2, 255, 0, 0, 255);

    DrawBlock(3, 0, 0,587, 18,0);
    DrawLine(3,0,0,587,0,3);
    DrawLine(3,0,17,587,0,3);
    DrawLine(3,0,0,0,17,3);
    DrawLine(3,587,0,0,17,3);

				     
    hGlobal=LoadResource(hInst,FindResource(hInst,"OSDMARKBITMAP",RT_BITMAP));
    TESTBITMAP=(BITMAPINFO*)LockResource(hGlobal);

    CreateOSDWindow( 1, (unsigned short)BITMAP4, (unsigned short)TESTBITMAP->bmiHeader.biWidth, (unsigned short)TESTBITMAP->bmiHeader.biHeight);
    MoveWindowAbs(1,20,440);



    SetColorBlend(1);
    yuv=RGB2YUV(255,255,0);
    cl=(yuv&0xffff);
    ch=((yuv>>16)&0xffff);
    SetColor_(1, 16,11, ch, cl);
    SetBlend_(1,16,11, 255);

    yuv=RGB2YUV(0,0,0);
    cl=(yuv&0xffff);
    ch=((yuv>>16)&0xffff);
    SetColor_(1, 16,0, ch, cl);
    SetBlend_(1,16,0, 0);
    DrawBlock(1, 0, 0,(unsigned short)TESTBITMAP->bmiHeader.biWidth, (unsigned short)TESTBITMAP->bmiHeader.biHeight,0);
    DrawBlock(2, 0, 0,(unsigned short)TESTBITMAP->bmiHeader.biWidth, 32,1);
    SetFont( 2, 1, 3, 1);
    strcpy(Titel,"  Lade Bitmap  ");
    WriteText(2,20,2,Titel);

    if ( OSDLoadBitmap4(1,TESTBITMAP,0,0,0,0,0,0,2000) < 0 ) {
	    strcpy(Titel,"* Fehler beim Bitmap laden *");
        WriteText(2,20,2,Titel);
	    DeleteObject(TESTBITMAP);
        Sleep(2000);
        UnlockResource(hGlobal);
        DestroyOSDWindow(1);
        DestroyOSDWindow(2);
        Osd_Thread_Down();
        return;
    };

OSD_SET_BUTTON_MARK:
    if ( memcmp(ButtonSetMark,AltButtonSetMark,20) != 0 ) {
    memcpy(AltButtonSetMark,ButtonSetMark,20);
    for ( i=0; i < 9 ; i++ ) {
	    if ( ButtonSetMark[i] == 1 ) {
		    yuv=RGB2YUV(OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B);
            j=OSD_Colors[0].Blend;
	    } else if ( ButtonSetMark[i] == 10  ) {
		    yuv=RGB2YUV(0,255,0);
            j=255;
	    } else if ( ButtonSetMark[i] == 11  ) {
	  		    yuv=RGB2YUV(255,255,0);
                j=255;
	    } 
		    
		        cl=(yuv&0xffff);
			    ch=((yuv>>16)&0xffff);
			    SetColor_(1, 16,(unsigned short)(i+1), ch, cl);
			    SetBlend_(1,16,(unsigned short)(i+1), (unsigned char)j);
    };		

    };



    l=20;

    DrawBlock(2, 0, 0,(unsigned short)TESTBITMAP->bmiHeader.biWidth, 32,1);

    if ( Play == TRUE ) {
    i=0;
    while (( i < 60 ) && ( HeaderInfo.Infos[i] >= 0x20 )) {
    Titel[i]=HeaderInfo.Infos[i];
    i++;
    Titel[i]=0x00;
    };

    if ( i== 0 ) strcpy(Titel,t("<Unbekannt>"));
    } ;

    WriteText(2,20,2,Titel);

    if ( ButtonSetMark[7]==11 ) {
	    ButtonSetMark[7]=10;
        goto OSD_SET_BUTTON_MARK;
    };


    for (;;) {
			 ResetEvent(OSD_KEY_EVENT);
             i=WaitForSingleObject (OSD_KEY_EVENT, 500);

			 if ( i == WAIT_TIMEOUT ) {

				 if ( HeaderInfo.SprungMarken[0] != 0xffffffff ) {
                        ButtonSetMark[0]=10;
						ButtonSetMark[5]=10;

						if ( Check_Mark(-1)== TRUE ) {
                           ButtonSetMark[2]=10;
          				   if ( Check_Mark(-2) == TRUE ) ButtonSetMark[1]=10;
		                     else ButtonSetMark[1]=1;
						} else {
                           ButtonSetMark[1]=1;
                           ButtonSetMark[2]=1;
						};
						if ( Check_Mark(1) == TRUE ) {
                           ButtonSetMark[3]=10;
          				   if ( Check_Mark(2) == TRUE ) ButtonSetMark[4]=10;
		                     else ButtonSetMark[4]=1;
						} else {
                           ButtonSetMark[3]=1;
                           ButtonSetMark[4]=1;
						}
				 } else { 
                        ButtonSetMark[0]=1;
						ButtonSetMark[1]=1;
                        ButtonSetMark[2]=1;
						ButtonSetMark[3]=1;
                        ButtonSetMark[4]=1;
                        ButtonSetMark[5]=1;
				 }

				 if ( memcmp(AltButtonSetMark,ButtonSetMark,20) != 0 ) {
				        goto OSD_SET_BUTTON_MARK;
				 };
				        l++;
						if ( l >= 1 ) {
							if ( l > 20 ) {
								DrawBlock(3, 1, 1,586, 16,0);
                                DrawBlock(2, (unsigned short)(TESTBITMAP->bmiHeader.biWidth-110),0 ,110, 32,1);
							}
						l=0;
						m=(HeaderInfo.CurrentSpielZeit/60);
						s=HeaderInfo.CurrentSpielZeit-(m*60);
						sprintf(Zeile,"%3d:%02d",m,s);
                        WriteText(2,(unsigned short)(TESTBITMAP->bmiHeader.biWidth-100),3,Zeile);
						XStep=(double)585/(double)HeaderInfo.SpielZeit;
                        XSize=(int)((double)XStep*(double)HeaderInfo.CurrentSpielZeit)+1;
                        DrawBlock(3, 1, 3,(unsigned short)XSize, 12,1);
					  for ( i=0; i	< 32 ; i++ ) {
						  if ( HeaderInfo.SprungMarken[i] != 0xffffffff) {
        					      XSize=(int)((double)HeaderInfo.SprungMarken[i]*(double)XStep);
							      DrawLine(3,(unsigned short)XSize,1,0,15,2);
							      DrawLine(3,(unsigned short)(XSize+1),1,0,15,2);
								};
							};
						};
                     
					 continue;
			 };

			 
			 if ( i != WAIT_OBJECT_0 ) {
        		    DeleteObject(TESTBITMAP);
                    DestroyOSDWindow(1);
                    DestroyOSDWindow(2);
                    DestroyOSDWindow(3);
                    Osd_Thread_Down();
                    return;
			 };
			 
			 if ( OSD_Key_Value == 0x00 ) {; 
        		    DeleteObject(TESTBITMAP);
                    DestroyOSDWindow(1);
                    DestroyOSDWindow(2);
                    DestroyOSDWindow(3);
                    Osd_Thread_Down();
                    return;
			 } else if ( OSD_Key_Value == '0' ) {; 
        		    DeleteObject(TESTBITMAP);
                    DestroyOSDWindow(1);
                    DestroyOSDWindow(2);
                    DestroyOSDWindow(3);
                    Osd_Thread_Down();
                    return;
			 } else if ( OSD_Key_Value == '1' ) { 
				 if ( ButtonSetMark[0] < 10 ) continue;
				 Jump_Time_Play(HeaderInfo.SprungMarken[0]);
                 l=200;
			 } else if ( OSD_Key_Value == '2' ) { 
				 if ( ButtonSetMark[1] < 10 ) continue;
                   Jump_Mark(-2);
				   l=200;
			 } else if ( OSD_Key_Value == '3' ) { 
				 if ( ButtonSetMark[2] < 10 ) continue;
                   Jump_Mark(-1);
				   l=200;
			 } else if ( OSD_Key_Value == '4' ) { 
				 if ( ButtonSetMark[3] < 10 ) continue;
                   Jump_Mark(1);
				   l=200;
			 } else if ( OSD_Key_Value == '5' ) { 
				 if ( ButtonSetMark[4] < 10 ) continue;
                   Jump_Mark(2);
				   l=200;
			 } else if ( OSD_Key_Value == '6' ) { 
				 if ( ButtonSetMark[5] < 10 ) continue;
			     i=31;
                 while (( i >= 0 ) && ( HeaderInfo.SprungMarken[i] == 0xffffffff )) i--;
				 Jump_Time_Play(HeaderInfo.SprungMarken[i]);
                 j=200;
             } else if ( OSD_Key_Value == '7' ) { 
				 if ( ButtonSetMark[6] < 10 ) continue;
        		    DeleteObject(TESTBITMAP);
			        DestroyOSDWindow(1);
				    DestroyOSDWindow(2);
                    DestroyOSDWindow(3);
                   goto PLAY_MENU;
			 } else if ( OSD_Key_Value == '8' ) { 
				 if ( ButtonSetMark[7] < 10 ) continue;
                   Insert_Sprungmarke(HeaderInfo.CurrentSpielZeit);
				   Update_MDI_File();
				   ButtonSetMark[7]=11;
                   goto OSD_SET_BUTTON_MARK;                  
			 } else if ( OSD_Key_Value == '9' ) { 
				 if ( ButtonSetMark[8] < 10 ) continue;
        		    DeleteObject(TESTBITMAP);
			        DestroyOSDWindow(1);
				    DestroyOSDWindow(2);
                    DestroyOSDWindow(3);
				   goto OSD_MARK_LISTE;
			 };


    };



OSD_MARK_LISTE:
	 ListCount=0;
	 i=0;
	 while ( i < 32 ) {
		  if ( HeaderInfo.SprungMarken[i] != 0xffffffff) {
            		m=(HeaderInfo.SprungMarken[i] /60);
					s=HeaderInfo.SprungMarken[i] -(m*60);
					sprintf(ListText[ListCount],"%3d:%02d",m,s);
                    ListCount++;
		  };
		  i++;
	 }

     MaxY=ListCount;
	 if ( MaxY > 10 ) MaxY=10;	  
	 CreateOSDWindow( 1, 1, 200, (unsigned short)(((MaxY+4)*28)+8));
     MoveWindowAbs(1,240,60 );
	 SetColorBlend(1);
	 OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
	 OSDSetColor(1,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
	 OSDSetColor(1,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
	 OSDSetColor(1,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);

		

	DrawBlock(1, 0, 0,200, 58,0);
    SetFont( 1, 1, 2, 0);
    strcpy(Zeile,t("Sprungmarken"));
	WriteText( 1, 18, 1,Zeile);

	m=(HeaderInfo.CurrentSpielZeit/60);
	s=HeaderInfo.CurrentSpielZeit-(m*60);
	sprintf(Zeile,"%3d:%02d",m,s);
	WriteText( 1, 60, 30 ,Zeile);


	DrawBlock(1, 0, (unsigned short)(((MaxY+3)*28)+6),200, 58,0);
    strcpy(Zeile,t("<4> Del"));
	WriteText( 1, 1,(unsigned short)(((MaxY+2)*28)+6),Zeile);
    strcpy(Zeile,t("<5> Pos"));
	WriteText( 1, 90,(unsigned short)(((MaxY+2)*28)+6),Zeile);
    strcpy(Zeile,t("<6> Ins"));
	WriteText( 1, 1,(unsigned short)(((MaxY+3)*28)+6),Zeile);
    strcpy(Zeile,t("<0> Ende"));
	WriteText( 1, 90,(unsigned short)(((MaxY+3)*28)+6),Zeile);
	Top=0;


REBUILD_ZEITSPRUNG_LISTE:

	DrawBlock(1, 0, 58,200,(unsigned short)((MaxY*28)+4),1);
    SetFont( 1, 1, 3, 1);
    i=0;
	while (( i < MaxY ) && ( i+Top < ListCount )) {
			WriteText( 1, 60,  (unsigned short)(((i+2)*28)+4)  ,ListText[i+Top] );
            i++;
	};

		 for (;;) {

			 if (( AltSelection >= 0 ) && ( AltSelection < MaxY )) {
            		 DrawBlock(1, 0, (unsigned short)(((AltSelection+2)*28)+4),200, 28,1);
                     SetFont( 1, 1, 3, 1);
					 WriteText( 1, 60,  (unsigned short)(((AltSelection+2)*28)+5) ,ListText[AltSelection+Top]);
			 };

			 if (( Selection >= 0 ) && ( Selection < ListCount )) {
				     AltSelection=Selection;
            		 DrawBlock(1, 5, (unsigned short)(((Selection+2)*28)+5),190, 27,3-1);
                     SetFont( 1, 1, 0, 3-1);
					 WriteText( 1, 60,  (unsigned short)(((Selection+2)*28)+5),ListText[Selection+Top]);
			 };

			 j=0;
			while ( j == 0 ) {
			 ResetEvent(OSD_KEY_EVENT);
			 i=WaitForSingleObject (OSD_KEY_EVENT, 1000);

			 if ( i == WAIT_TIMEOUT ) {
            	DrawBlock(1, 0, 30,200, 28,0);
				m=(HeaderInfo.CurrentSpielZeit/60);
				s=HeaderInfo.CurrentSpielZeit-(m*60);
				sprintf(Zeile,"%3d:%02d",m,s);
                SetFont( 1, 1, 2, 0);
				WriteText( 1, 60, 30 ,Zeile);
                continue;
			 };

			 if ( i != WAIT_OBJECT_0 ) {
                    DestroyOSDWindow(1);
                    Osd_Thread_Down();
                    return;
			 };
			 
			 if ( OSD_Key_Value == 0x00 ) {; 
                    DestroyOSDWindow(1);
                    Osd_Thread_Down();
                    return;
			 };

			 if ( OSD_Key_Value == '0' ) {
			       DestroyOSDWindow(1);
				   goto MARK_MENU;
			 };

             if ( OSD_Key_Value == '5' ) {
			   	  Jump_Time_Play(HeaderInfo.SprungMarken[Selection+Top]);
			 };

             if ( OSD_Key_Value == '4' ) {
				 			      Del_Sprungmarke(Selection+Top);
                                  DestroyOSDWindow(1);
                                  Update_MDI_File();
								  goto OSD_MARK_LISTE;
			 };

             if ( OSD_Key_Value == '6' ) {
                			      Insert_Sprungmarke(HeaderInfo.CurrentSpielZeit);
                                  DestroyOSDWindow(1);
                                  Update_MDI_File();
                                  goto OSD_MARK_LISTE;
			 };


             if ( OSD_Key_Value == '2' ) {; 
			 if (( Selection < MaxY-1 ) && ( Selection+Top < ListCount-1 ))  {
				  Selection++;
				  j=1;
			 } else if ( Selection+Top < ListCount-1 ) {
				 Top=Top+10;
				 Selection=0;
				 goto REBUILD_ZEITSPRUNG_LISTE;
			 };

			 } else if ( OSD_Key_Value == '1' ) {
				 if ( Selection+Top < ListCount-1 ) {
				     Top=ListCount/10;
                     Top=Top*10;
					 Selection=(ListCount-1)-Top;
				 goto REBUILD_ZEITSPRUNG_LISTE;
				 };
			 } else if ( OSD_Key_Value == '7' ) { 
			 if ( Selection+Top > 0) {
				 Selection=0;
				 Top=0;
				 goto REBUILD_ZEITSPRUNG_LISTE;
			 };
			 } else if ( OSD_Key_Value == '8' ) {; 
			 if ( Selection > 0) {
				 Selection--;
				 j=1;
			 } else if ( Top > 0 ) {
                 Top-=10;
				 Selection=9;
				 goto REBUILD_ZEITSPRUNG_LISTE;
			 }
			 } else if ( OSD_Key_Value == '9' ) {; 
                if ( Top > 0 ) {
                 Top-=10;
				 Selection=9;
				 goto REBUILD_ZEITSPRUNG_LISTE;
                } ;
			 } else if ( OSD_Key_Value == '3' ) {; 
                     i=Top;
			         Top=ListCount/10;
                     Top=Top*10;
			 if ( Top != i  ) {
				 Selection=0;
				 goto REBUILD_ZEITSPRUNG_LISTE;
                } ;

			 }
			}; // While ActiveSel;
}



RECORD_MENU:

    CreateOSDWindow( 2, (unsigned short)BITMAP4, 588, 32);
    MoveWindowAbs(2,20,400);
    SetColorBlend(2);
    OSDSetColor(2,1, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
    OSDSetColor(2,3, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);

    hGlobal=LoadResource(hInst,FindResource(hInst,"OSDRECORDBITMAP",RT_BITMAP));
    TESTBITMAP=(BITMAPINFO*)LockResource(hGlobal);

    CreateOSDWindow( 1, (unsigned short)BITMAP4, (unsigned short)TESTBITMAP->bmiHeader.biWidth, (unsigned short)TESTBITMAP->bmiHeader.biHeight);
    MoveWindowAbs(1,20,440);
    SetColorBlend(1);

    yuv=RGB2YUV(0,0,0);
    cl=(yuv&0xffff);
    ch=((yuv>>16)&0xffff);
    SetColor_(1, 16,0, ch, cl);
    SetBlend_(1,16,0, 0);
    DrawBlock(1, 0, 0,(unsigned short)TESTBITMAP->bmiHeader.biWidth, (unsigned short)TESTBITMAP->bmiHeader.biHeight,0);
    DrawBlock(2, 0, 0,588, 31,1);
    SetFont( 2, 1, 3, 1);
    strcpy(Titel,"  Lade Bitmap  ");
    WriteText(2,20,2,Titel);

    if ( OSDLoadBitmap4(1,TESTBITMAP,0,0,0,0,0,0,2000) < 0 ) {
	    strcpy(Titel,"* Fehler beim Bitmap laden *");
        WriteText(2,20,2,Titel);
	    DeleteObject(TESTBITMAP);
        Sleep(2000);
        UnlockResource(hGlobal);
        DestroyOSDWindow(1);
        DestroyOSDWindow(2);
        Osd_Thread_Down();
        return;
    };

OSD_SET_BUTTON_RECORD:

    for ( i=0; i < 5 ; i++ ) {
	    if ( ButtonSetRecord[i] == 1 ) {
		    yuv=RGB2YUV(OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B);
            j=OSD_Colors[0].Blend;
	    } else if ( ButtonSetRecord[i] == 10  ) {
		    yuv=RGB2YUV(0,255,0);
            j=255;
	    } else if ( ButtonSetRecord[i] == 11  ) {
	  		    yuv=RGB2YUV(255,255,0);
                j=255;
	    } 
		    
		        cl=(yuv&0xffff);
			    ch=((yuv>>16)&0xffff);
			    SetColor_(1, 16,(unsigned short)(i+1), ch, cl);
			    SetBlend_(1,16,(unsigned short)(i+1), (unsigned char)j);
    };		


    if ( ButtonSetRecord[3] == 11 ) {
         ButtonSetRecord[3]=10;
	     goto OSD_SET_BUTTON_RECORD;
    };

    DrawBlock(2, 0, 0,588, 31,1);

    if ( Record == TRUE ) {
    i=0;
    while (( i < 60 ) && ( HeaderInfo.Infos[i] >= 0x20 )) {
    Titel[i]=HeaderInfo.Infos[i];
    i++;
    Titel[i]=0x00;
    };

    if ( i== 0 ) strcpy(Titel,t("<Unbekannt>"));
    } else { 
	    strcpy(Titel,t("< Recorder nicht gestartet>"));
    };

    WriteText(2,20,0,Titel);

				     if ( Record == TRUE ) {
						    m=(HeaderInfo.CurrentSpielZeit/60);
						    s=HeaderInfo.CurrentSpielZeit-(m*60);
						    sprintf(Zeile,"%3d:%02d",m,s);
                            WriteText(2,488,0,Zeile);
				     }


    for (;;) {
			 ResetEvent(OSD_KEY_EVENT);
             i=WaitForSingleObject (OSD_KEY_EVENT, 1000);
			 if ( i == WAIT_TIMEOUT ) {
				 if ( Record == TRUE ) {
						m=(HeaderInfo.CurrentSpielZeit/60);
						s=HeaderInfo.CurrentSpielZeit-(m*60);
						sprintf(Zeile,"%3d:%02d",m,s);
                        WriteText(2,488,0,Zeile);
				 }
					 continue;
			 };

			 
			 if ( i != WAIT_OBJECT_0 ) {
        		    DeleteObject(TESTBITMAP);
                    DestroyOSDWindow(1);
                    DestroyOSDWindow(2);
                    Osd_Thread_Down();
                    return;
			 };
			 
			 if ( OSD_Key_Value == 0x00 ) {; 
        		    DeleteObject(TESTBITMAP);
                    DestroyOSDWindow(2);
                    DestroyOSDWindow(1);
                    Osd_Thread_Down();
                    return;
			 };

             
			 if ( OSD_Key_Value == '0' ) {; 
        		    DeleteObject(TESTBITMAP);
                    DestroyOSDWindow(2);
                    DestroyOSDWindow(1);
                    Osd_Thread_Down();
                    return;
			 } else if ( OSD_Key_Value == '1' ) {; 
 				 if ( ButtonSetRecord[0] < 10 ) continue;
			 if ( Record == FALSE ) {
			        Debi_Record_Start(RecorderFormat,RecorderTyp);
					goto OSD_SET_BUTTON_RECORD;
			 } else if ( Recorder_Pause == TRUE ) {
				 Debi_Record_Pause();
                    Sleep(0); Sleep(1);Sleep(0);
					Sleep(0); Sleep(1);Sleep(0);
					goto OSD_SET_BUTTON_RECORD;
			 };
			 } else if ( OSD_Key_Value == '2' ) {
 				 if ( ButtonSetRecord[1] < 10 ) continue;
				 Debi_Record_Pause();
			     goto OSD_SET_BUTTON_RECORD;
			 } else if ( OSD_Key_Value == '3' ) {; 
 				 if ( ButtonSetRecord[2] < 10 ) continue;
			 if ( Record == TRUE ) Debi_Record_Stop();
                  goto OSD_SET_BUTTON_RECORD;
			 } else if ( OSD_Key_Value == '4' ) {
 				 if ( ButtonSetRecord[3] < 10 ) continue;
                 ButtonSetRecord[3]=11; 
				 Insert_Sprungmarke(HeaderInfo.CurrentSpielZeit);
				 Update_MDI_File();
				 goto OSD_SET_BUTTON_RECORD;
			 } else if ( OSD_Key_Value == '5' ) {
 				 if ( ButtonSetRecord[4] < 10 ) continue;
       		       DeleteObject(TESTBITMAP);
                   DestroyOSDWindow(2);
			       DestroyOSDWindow(1);
                   goto INFO_MENU;
			 }
    };

    return;
}



BOOL Resync_MPG_File(int *fd ) 
{
    int TrySync=0;
    unsigned char Buffer[8192];
    int i;
    int len;

    while ( TrySync < 3 ) { 
    len = read(*fd,Buffer,sizeof(Buffer));
    for ( i =0; i < len-4; i++ ) {
    if ( Is_Valid_Sync(Buffer+i) == TRUE ) {
	    _lseeki64(*fd,(__int64)i-len,SEEK_CUR);
	    return(TRUE);
    };
    };
    TrySync++;
    };
    return(FALSE);
};


unsigned int LaufZeitSekunden(unsigned int start,unsigned int now)
{
	return(now-start);
}

int found_seq_start(unsigned char *Daten, int Laenge) {
    unsigned char *ptr;
    int count;

    ptr = Daten;
    count = 0;
    ptr = find_in_mem(Daten, Laenge-7, seq_hdr_code, sizeof(seq_hdr_code)-1);
    while (ptr != NULL) {
	if (ptr[3] >= VIDEO_STREAM_S && ptr[3] <= VIDEO_STREAM_E) {
	    if ((ptr[6] & 0xc0) == 0x80) {	/* mpeg2 */
		count = ptr-Daten+9+ptr[8];
	    } else {
		count = ptr-Daten+12;
	    }
	    if (find_in_mem(Daten+count, Laenge-count, seq_hdr_code, sizeof(seq_hdr_code)) != NULL) {
		return ptr-Daten;
	    }
	} else {
	    count += 4;
	}
	ptr = find_in_mem(Daten+count, Laenge-count-7, seq_hdr_code, sizeof(seq_hdr_code)-1);
    }
    return -1;
}

int Irq_Record(unsigned char *Daten , int Laenge)
{
    unsigned int nstored, count;
    char text[256];

    nstored = ring_buf_put(recorder_buf, (char *)Daten, (unsigned int) Laenge);
    count = ring_buf_count(recorder_buf);
    if (nstored < (unsigned int) Laenge) {
	    sprintf(text, "ring buffer overflow, %d bytes dropped", (unsigned int) Laenge - nstored);
	    Write_Log(text);
    }
    if (count > (unsigned int) Laenge) {
	sprintf(text, "ring_buf_put %d --> %d", Laenge, count);
	Write_Log(text);
    }
    return 0;
}

static int Eval_Record_Exception ( LPEXCEPTION_POINTERS pEP )
{
	int n_except;
	char buf[512];
	
	n_except = pEP->ExceptionRecord->ExceptionCode;
	
	if (n_except == EXCEPTION_ACCESS_VIOLATION) {
		
            Record = FALSE;

            sprintf(buf, "Access violation : %s address = 0x%08x, IP=0x%08x.\nRecording stopped...",
                pEP->ExceptionRecord->ExceptionInformation[0] ? "writing" : "reading",
                pEP->ExceptionRecord->ExceptionInformation[1],
                pEP->ContextRecord->Eip);
            Write_Log(buf);
	}
	
	return EXCEPTION_CONTINUE_EXECUTION;
}

static unsigned short audio_tracks;
static unsigned short video_tracks;

void Recorder_Thread (void *param){
    int Laenge, i0;
    unsigned int CurrentTimeStamp;
    unsigned char id, *ptr, *ptr2;
    unsigned short plen, tref, len, ptype;
    unsigned int pts, first_pts, prev_pts, last_vpts;
    unsigned int frame_period;
    unsigned short vsize, hsize, ratio, fr;
    unsigned char *ratios[5] = { "invalid", "1:1", "4:3", "16:9", "2.21:1" };
    unsigned int frame_periods[9] = {1, 1126125, 1125000, 1080000, 900900, 900000, 540000, 450450, 450000};
    char text[256], text2[256];
    unsigned char seq_pics[1024];
    unsigned short i, npics;
    BOOL synced = FALSE;
    BOOL put_msg;

    audio_tracks =
    video_tracks = 0;
    frame_period = 0;
    first_pts = 0;
    memset(seq_pics, 0, sizeof(seq_pics));
    npics = 0;

    __try {
        while (WaitForSingleObject(recorder_buf->has_data_evt, 10000/*ms*/) == WAIT_OBJECT_0) {
            if (Record) {
                Laenge = ring_buf_get(recorder_buf, Mpeg_Buffer, MPEG_BUFFER_SIZE, FALSE);
                CurrentTimeStamp=Get_Seconds(Mpeg_Buffer);
                if ( CurrentTimeStamp != 0 ) {
                    if ( HeaderInfo.TimeStampStart == 0 ) HeaderInfo.TimeStampStart = CurrentTimeStamp;
                    else HeaderInfo.CurrentSpielZeit = LaufZeitSekunden(HeaderInfo.TimeStampStart,CurrentTimeStamp);
                }

                i0 = 0;
                for (ptr = find_in_mem(Mpeg_Buffer, Laenge, program_end_code, sizeof(program_end_code)-1);
                     ptr != NULL && ptr+13-Mpeg_Buffer < Laenge;
                     i0 = ptr-Mpeg_Buffer, ptr = find_in_mem(Mpeg_Buffer+i0, Laenge-i0, program_end_code, sizeof(program_end_code)-1)) {

                    put_msg = FALSE;
                    id = ptr[3];
                    if (id >= AUDIO_STREAM_S && id <= AUDIO_STREAM_E ||
                        id >= VIDEO_STREAM_S && id <= VIDEO_STREAM_E ||
                        id == PRIVATE_STREAM1) {
                        if (id >= AUDIO_STREAM_S && id <= AUDIO_STREAM_E) {
                            audio_tracks |= 1<<(id-AUDIO_STREAM_S);
                        } else if (id >= VIDEO_STREAM_S && id <= VIDEO_STREAM_E) {
                            video_tracks |= 1<<(id-VIDEO_STREAM_S);
                        }
                        plen = (ptr[4] << 8) | ptr[5];
		        if ((ptr[6] & 0xc0) == 0x80 && (ptr[7] & PTS_ONLY) != 0) {	/* mpeg2 + PTS */
		            pts = ((ptr[9] >> 1) << 29) | // Low order bit dropped
			          (ptr[10] << 21) | ((ptr[11] >> 1) << 14) |
			          (ptr[12] << 6) | (ptr[13] >> 2);
                            if (first_pts == 0) {
			        first_pts = pts;
                                sprintf(text, "First PTS : %5.3f", pts/45000.);
			        Write_Log(text);
                            } else if (labs(pts-prev_pts) > 45000*2) { /* 2 seconds */
                                sprintf(text, "PTS jump : %.3f seconds", ((int)(pts-prev_pts))/45000.);
                                Write_Log(text);
                            }
                            prev_pts = pts;
                        } else {
                            pts = 0;
                            text[0] = 0;
                        }

                        if (id >= AUDIO_STREAM_S && id <= AUDIO_STREAM_E) {

                            if (pts != 0) {
                                sprintf(text, "Audio PTS %d : %5.3f", id-AUDIO_STREAM_S, ((int)(pts-first_pts))/45000.);
                            }

                        } else if (id >= VIDEO_STREAM_S && id <= VIDEO_STREAM_E) {
                            if (pts != 0) last_vpts = pts;
		            ptr2 = find_in_mem(ptr+ptr[8], Laenge-(ptr-Mpeg_Buffer)-ptr[8], picture_start_code, sizeof(picture_start_code));
		            if (ptr2 == NULL) {
                                text[0] = 0;
                            } else {
			        tref = (ptr2[4]<<2) | (ptr2[5]>>6);
			        ptype = (ptr2[5]>>3) & 0x07;
	                        sprintf(text, "Video PTS %d : %5.3f %c@%d %c", id-VIDEO_STREAM_S, ((int)(last_vpts-first_pts))/45000.,
                                        "?IPBD???"[ptype], tref, (pts == 0) ? '*' : ' ');
                                if (ptype == 1) {
                                    if (synced && npics != 0) {
                                        for (i=0; i<npics; i++) {
                                            if (seq_pics[i] != 1) {
                                                sprintf(text2, "Picture %d/%d received %d times", i+1, npics, seq_pics[i]);
                                                Write_Log(text2);
                                                put_msg = TRUE;
                                            }
                                        }
                                    }
                                    memset(&seq_pics, 0, sizeof(seq_pics));
                                    npics = 0;
                                }
                                seq_pics[tref]++;
                                if (npics < tref+1) npics = tref+1;
		            }
                        } else {
                            if (pts != 0) {
                                sprintf(text, "Data PTS : %5.3f", ((int)(pts-first_pts))/45000.);
                            }
                        }
                        if (put_msg && text[0] != 0) Write_Log(text);
                        ptr += plen+6;

                    } else if (id == PROG_PACK_START) {
                        if ((ptr[4] & 0xc0) == 0x40) {
                            plen = 14 + (ptr[13]&7);
                        } else {
                            plen = 4;
                        }
                        ptr += plen;

                    } else if (id == SYS_HEADER_STRT ||
                               id == PROG_STREAM_MAP ||
                               id == PADDING_STREAM ||
                               id == PRIVATE_STREAM2 ||
                               id >= ECM_STREAM) {
                        plen = (ptr[4] << 8) | ptr[5];
                        ptr += plen+6;
                    } else {
                        ptr += 4;
                    }
                }

                if (! synced) {
                    i0 = found_seq_start(Mpeg_Buffer, Laenge);
                    if (i0 >= 0 && i0+17 < Laenge) {
                        synced = TRUE;
                        if (i0 > 0) {
                            Laenge -= i0;
                            memmove(Mpeg_Buffer, Mpeg_Buffer+i0, Laenge);
                        }
                        ptr = find_in_mem(Mpeg_Buffer, Laenge, seq_hdr_code, sizeof(seq_hdr_code));

                        if ((Mpeg_Buffer[6] & 0xc0) == 0x80) { // MPEG2 ?
                            plen = (Mpeg_Buffer[4] << 8) | Mpeg_Buffer[5];
                            len = 9 + Mpeg_Buffer[8];
                            i0 = ptr - Mpeg_Buffer;
                        // Modify the pack block in order to let it start at the sequence start
                            if (i0 > len) {
                                plen -= i0-len;
                                memmove(Mpeg_Buffer+len, ptr, Laenge-i0);
                                Laenge -= i0-len;
                                Mpeg_Buffer[4] = plen >> 8;
                                Mpeg_Buffer[5] = plen & 0xff;
                                ptr = Mpeg_Buffer+len;
                            }
                        }
                
                        hsize = (ptr[4]<<4) | (ptr[5]>>4);
                        vsize = ((ptr[5]&0x0f)<<8) | ptr[6];
                        ratio = ptr[7] >> 4;
                        fr = ptr[7]&0x0f;
                        sprintf(text, "%dx%d, %s, %.3f fps", hsize, vsize, ratios[ratio], 27000000./frame_periods[fr]);
                        Write_Log(text);
                    } else if (((int)(pts-first_pts)) >= 90000) { /* 2 seconds */
                        synced = TRUE;
                        Write_Log("Starting record without finding a sequence start...");
                    }
                }
                if (synced) {
                    if ( RecorderTyp == 0 ) {
                        if ( Write_Buffer(Mpeg_Buffer, Laenge ) < 0 ) break;
                    } else if ( RecorderTyp == 1 ) {
                        if ( Write_as_PAV_File(Mpeg_Buffer, Laenge) < 0 )  break;;
                    }
                }
            } else {
                break;
            }
        }  // End of loop waiting for data
    }
    __except( Eval_Record_Exception( GetExceptionInformation( ))) {
	    // No code; this block never executed.
    }
    Record = FALSE;

    if (recorder_buf) {
        drop_ring_buf(recorder_buf);
        recorder_buf = NULL;
    }
    return;
}

int Write_Buffer(unsigned char *Daten, int Laenge )
{
    static int byte_cnt = 0;
    int i;
    unsigned int BytesWritten;
    static int told_get_sync = FALSE;
    char text[256];
    unsigned char *ptr, buf[9];
    unsigned short len, plen;

    if ( Laenge == 0 ) return(0);

    HeaderInfo.Bytesplay+=Laenge;
    i=(int)(HeaderInfo.Bytesplay/1048576);
    if ( i > 0 ) {
        HeaderInfo.MegaBytes_Recorded+=i;
        HeaderInfo.Bytesplay-=(i*1048576);
    }
    HeaderInfo.KBTotal=(unsigned int)((HeaderInfo.MegaBytes_Recorded*1024)+(HeaderInfo.Bytesplay/1024));

    if (( HeaderInfo.MegaBytes_Recorded > MAXMegaBytes ) && MAXMegaBytes != 0 ) {
		if (! told_get_sync) Write_Log("Looking for sync");
		told_get_sync = TRUE;
     // Synchronize here
        byte_cnt += Laenge;
        i = found_seq_start(Daten, Laenge);
	if (i>=0) {
	    sprintf(text, "Seq start found at %d", byte_cnt);
	    Write_Log(text);
	}
        if (byte_cnt > 1024000 || i>=0) {
	    if (i>0) { // Write the remaining part before the sequence start
		BytesWritten=write(RecorderFile,Daten,i);
		memmove(Daten, Daten+i, i);
		Laenge -= i;
	    }
            if (i>=0) {
                ptr = find_in_mem(Daten, Laenge, seq_hdr_code, sizeof(seq_hdr_code));

                len = 9 + Daten[8];
                if ((Daten[6] & 0xc0) == 0x80 && Laenge >= len) { // MPEG2 ?
                    plen = (Daten[4] << 8) | Daten[5];
                    len = 9 + Daten[8];
                    i = ptr - Daten;
                // Modify the pack block in order to let it start at the sequence start
                    if (i > len) { /* End of previous sequence present */
                        buf[0] = 0;
                        buf[1] = 0;
                        buf[2] = 1;
                        buf[3] = Daten[3]; // Stream ID
                        buf[4] = (i-len+3)>>8;
                        buf[5] = (i-len+3)&0xff;
                        buf[6] = 0x80;
                        buf[7] = 0;
                        buf[8] = 0;
                        BytesWritten=write(RecorderFile, buf, 9); /* Write the end of previous sequence */
                        BytesWritten=write(RecorderFile, Daten+len, i-len);
                        plen -= i-len;
                        memmove(Daten+len, ptr, Laenge-i); /* Recreate the PES packet starting with the sequence */
                        Laenge -= i-len;
                        Daten[4] = plen >> 8;
                        Daten[5] = plen & 0xff;
                    }
                }
		byte_cnt = Laenge;
            }
	    Next_Record_File(0, Daten[3]); // Change of file
	    HeaderInfo.MegaBytes_Recorded=1;
        }
    } else {
	told_get_sync = FALSE;
    }


    BytesWritten=0xffffffff;
    while ( BytesWritten != (unsigned int )Laenge )  {
        BytesWritten=write(RecorderFile,Daten,Laenge);

        if ( BytesWritten != (unsigned int )Laenge ) { // Disk full...
	        // Write im Eimer // Nächste Platte oder Ende 
            if ( Next_Record_File(0, Daten[3]) == FALSE ) return(-1);

        }
    }

    return(Laenge);
}

void Write_PS_Header( int fd )
{
	int i;

	unsigned char ps_hdr[32] = { 
            0x00,0x00,0x01,0xBA, // Pack header
            0x44,0x00,0x04,0x00,0x04,
            0x01,0x01,0xD5,0xFF, // MPEG2, SCR1 = 0, SCR2 = 0, mux rate = 1469 kB/s
            0xF8,                // Stuffing with 0 bytes

            0x00,0x00,0x01,0xBB, // SYS_HDR_START header
            0x00,0x0C,           // len=12
            0x88,0x8B,0x81,      // rate_bound=13672 kB/s
            0x05,                // audio_bound=1, fixed=0, CSPS=1
            0xE1,                // system_audio_lock=1, system_video_lock=1, video_bound=1
            0xFF,                // packet_rate_restriction=1
             0xB8, 0xB8,0x80,    //  All audio tracks : bufsize=16384 bytes
             0xB9, 0xB9,0xE6,    //  All video tracks : bufsize=235520 bytes
    };

    i=write(fd, ps_hdr, 32);
}

void Init_PAV( void ) 
{
	int i;

	Mpeg2AV_Pes.found = 0;
	Mpeg2AV_Pes.cid = 0;
	Mpeg2AV_Pes.mpeg = 0;
	Mpeg2AV_Pes.apsv.length = 0;
	Mpeg2AV_Pes.apsv.reserved = AV_RES;
	Mpeg2AV_Pes.apsv.counter = 0;
	Mpeg2AV_Pes.apsv.flags = 0x00;
	Mpeg2AV_Pes.apsa.length = 0;
	Mpeg2AV_Pes.apsa.reserved = AV_RES;
	Mpeg2AV_Pes.apsa.counter = 0;
	Mpeg2AV_Pes.apsa.flags = 0x00;
	Mpeg2AV_Pes.apsv.stream_id = AV_VIDEO;
	Mpeg2AV_Pes.apsa.stream_id = AV_AUDIO;
	Mpeg2AV_Pes.done = 0;
	Mpeg2AV_Pes.fd1 = -1;
	Mpeg2AV_Pes.av = 0;

	AV_Pes2Mpeg.fd = -1;
	AV_Pes2Mpeg.type = PS_STREAM;
	AV_Pes2Mpeg.found = 0;
	AV_Pes2Mpeg.length = 0;
	AV_Pes2Mpeg.plength = 0;
	AV_Pes2Mpeg.cid = 0;
	AV_Pes2Mpeg.flags = 0;
	AV_Pes2Mpeg.headr = 0;
	AV_Pes2Mpeg.alength=0;
	AV_Pes2Mpeg.vlength=0;
	AV_Pes2Mpeg.pid0 = 0;
	AV_Pes2Mpeg.pid1 = 0;
	AV_Pes2Mpeg.pidv = 0;
	AV_Pes2Mpeg.pida = 0;
    AV_Pes2Mpeg.Get_Sync = FALSE;
    AV_Pes2Mpeg.LastTime = 0;

	for ( i= 0; i < 4; i++){
		AV_Pes2Mpeg.last_av_pts[i] = 0;
		AV_Pes2Mpeg.av_pts[i] = 0;
		AV_Pes2Mpeg.scr[i] = 0;
	}
}

int Write_as_PAV_File(unsigned char *MpgBuffer, int Mpg_Len )
{
    int ret;
    int Written=0;
    int Mpg_Play_Pos=0;
    while ( Mpg_Play_Pos < Mpg_Len ) {
    ret=kpes_to_av_pes(&MpgBuffer[0],&Mpg_Play_Pos,Mpg_Len,&Mpeg2AV_Pes,&PAV_Video_Buffer[0]);
    if ( ret > 0 ) Written+=Write_Buffer(&PAV_Video_Buffer[0], ret );
    } 
    return(Written);
};





BOOL Resync_AV_File(int *fd ) 
{
    int TrySync=0;
    unsigned char Buffer[8192];
    int i;
    int len;

    while ( TrySync < 3 ) { 
    len = read(*fd,&Buffer[0],8192);
    for ( i =0; i < len-2; i++ ) {
	    if (( Buffer[i]=='A' ) && ( Buffer[i+1]=='V' )) {
		    _lseeki64(*fd,(__int64)i-len,SEEK_CUR);
		    return(TRUE);
	    };
    };
    TrySync++;
    };
    return(FALSE);
};


BOOL Is_Valid_AV(unsigned char *Daten) 
{
    if (( Daten[0]=='A' ) && ( Daten[1]=='V' )) return(TRUE);
    return(FALSE);
};



int GetAvPesLength(unsigned char *Data)
  { 
    if ((Data[0] == 'A' ) && ( Data[1] == 'V')  && ( Data[4] == 'U')) {
       return ((Data[6] << 8) + Data[7] + AV_PES_HEADER_LEN);
	};
return(0);
}

int GetAvPesType(unsigned char *Data) 
{ 
	return(Data[2]); 
};





int Irq_Play( int Typ )
{
    int len=0;
    if (Typ == 1 ) {
        len=Play_Audio_Track(Typ);
    } else if (Typ == 2 ) {
        len=Play_Video_Track(Typ);
    } else if (Typ == 3 ) {
        if ( AudioBlocks < VideoBlocks ) len=Play_Audio_Track(Typ);
        else len=Play_Video_Track(Typ);
    };
    
    if ( len == 0 ) return(0);
    else if ( len < 11 || len > 2048 ) {
        return(-1) ;
    } else return(len);
};


int Play_Audio_Track(int Typ)
{
    int len;
    unsigned int CurrentTimeStamp;
    unsigned char Next_Audio_Buffer[0x800];
    
    len=Load_Next_Buffer(&AudioFilePtr, 1 , Next_Audio_Buffer);
    if ( len < 0 ) return(-1);
    
    if ( RecorderFormat == 0 ) {
        CurrentTimeStamp=Get_Seconds(Next_Audio_Buffer);
        if ( CurrentTimeStamp != 0 ) {
            HeaderInfo.CurrentSpielZeit=LaufZeitSekunden(HeaderInfo.TimeStampStart,CurrentTimeStamp);
        }
    }
    
    memcpy(Record_mem_dma.pUserAddr, Next_Audio_Buffer, len);
    return(len);
}

int Play_Video_Track(int Typ )
{
    unsigned int CurrentTimeStamp;
    int len;
    unsigned char Next_Video_Buffer[0x800];
    
    len=Load_Next_Buffer(&VideoFilePtr, 0 , Next_Video_Buffer);
    if ( len < 0 ) return(-1);
    CurrentTimeStamp=Get_Seconds(Next_Video_Buffer);
    if ( CurrentTimeStamp != 0 ) {
        HeaderInfo.CurrentSpielZeit=LaufZeitSekunden(HeaderInfo.TimeStampStart,CurrentTimeStamp);
    }
    memcpy(Record_mem_dma.pUserAddr, Next_Video_Buffer, len);
    return(len);
}



BOOL Is_Audio_Sync(unsigned char *Header)
{
	unsigned int sync;

    sync=(Header[0]<<24);
    sync|=(Header[1]<<16);
    sync|=(Header[2]<<8);
    sync|=Header[3];
    if ((sync&~0x1f)==0x000001c0) return(TRUE);
return(FALSE);
};

BOOL Is_Video_Sync(unsigned char *Header)
{
	unsigned int sync;

    sync=(Header[0]<<24);
    sync|=(Header[1]<<16);
    sync|=(Header[2]<<8);
    sync|=Header[3];
    if ((sync&~0x0f)==0x000001e0) return(TRUE);
return(FALSE);
};


BOOL Is_Valid_Sync(unsigned char *Header)
{
	unsigned int sync;

    sync=(Header[0]<<24);
    sync|=(Header[1]<<16);
    sync|=(Header[2]<<8);
    sync|=Header[3];
                
    if (((sync&~0x0f)==0x000001e0) ||
                    ((sync&~0x1f)==0x000001c0) ||
                    (sync==0x000001bd)) return(TRUE);
return(FALSE);
};

int Get_Length(unsigned char *Header)
{
	unsigned int blen;
        blen=(Header[4]<<8);
        blen|=Header[5];
        blen+=6;
return(blen);
};



BOOL APIENTRY TimerRecSettingProc
(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
	int i;
    int j; 
	unsigned short jahr,monat,tag;
    BOOL Translated;
    BOOL Is_Set;
    SYSTEMTIME SystemTime;   
    char Zeile[10];

	switch (message) {
        case WM_INITDIALOG:
			
			for ( i =0; i < TIMERANZAHL ; i++ ) {
			    
			SendMessage (GetDlgItem(hDlg,IDC_COMBO1+i), CB_RESETCONTENT, 0, 0);
			SendMessage (GetDlgItem(hDlg,IDC_COMBO1+i), CB_INSERTSTRING, 0, (LPARAM)(LPSTR)"Audio MPG");
        	SendMessage (GetDlgItem(hDlg,IDC_COMBO1+i), CB_INSERTSTRING, 1, (LPARAM)(LPSTR)"Video MPG");
        	SendMessage (GetDlgItem(hDlg,IDC_COMBO1+i), CB_INSERTSTRING, 2, (LPARAM)(LPSTR)"AV MPG");
			SendMessage (GetDlgItem(hDlg,IDC_COMBO1+i), CB_INSERTSTRING, 3, (LPARAM)(LPSTR)"Audio PVA");
        	SendMessage (GetDlgItem(hDlg,IDC_COMBO1+i), CB_INSERTSTRING, 4, (LPARAM)(LPSTR)"Video PVA");
        	SendMessage (GetDlgItem(hDlg,IDC_COMBO1+i), CB_INSERTSTRING, 5, (LPARAM)(LPSTR)"AV PVA");
           
			j=RecorderFormat+(3*RecorderTyp);
			SendMessage(GetDlgItem(hDlg,IDC_COMBO1+i),CB_SETCURSEL,j,0);
				if (TimerRec[i].StartZeit_h != 0xff ) {
					SetDlgItemInt(hDlg,IDC_SH1+i,TimerRec[i].StartZeit_h,TRUE);
					SetDlgItemInt(hDlg,IDC_SM1+i,TimerRec[i].StartZeit_m,TRUE);
		
					if ( TimerRec[i].EndZeit_h != 0xff ) SetDlgItemInt(hDlg,IDC_EH1+i,TimerRec[i].EndZeit_h,TRUE);
					   else SetDlgItemText(hDlg,IDC_EH1+i,"");
					if ( TimerRec[i].EndZeit_m != 0xff ) SetDlgItemInt(hDlg,IDC_EM1+i,TimerRec[i].EndZeit_m,TRUE);
					   else SetDlgItemText(hDlg,IDC_EM1+i,"");

					DecodeDVBDate((unsigned short)(TimerRec[i].Date_Id-15018), &jahr, &monat,&tag);
					SetDlgItemInt(hDlg,IDC_DT1+i,tag,TRUE);
					SetDlgItemInt(hDlg,IDC_DM1+i,monat,TRUE);
					SetDlgItemInt(hDlg,IDC_DJ1+i,jahr,TRUE);
					SetDlgItemInt(hDlg,IDC_P1+i,TimerRec[i].Programm+1,TRUE);
					SetDlgItemText(hDlg,IDC_PNAME1+i,Programm[TimerRec[i].Programm].Name);
					SendMessage(GetDlgItem(hDlg,IDC_COMBO1+i),CB_SETCURSEL,TimerRec[i].RecorderArt,0);
					if ( TimerRec[i].Shutdown == TRUE ) CheckDlgButton(hDlg,IDC_CHECK1+i,TRUE);
				};
            };
            SetFocus(GetDlgItem(hDlg,IDC_SH1));
			break;


  case WM_COMMAND:

	  for ( j=0; j < TIMERANZAHL; j++ ) {
	  if ( LOWORD(wParam) == IDC_P1+j ) {

		  if ( HIWORD(wParam) == 0x0300 ) {
			   GetDlgItemText(hDlg,IDC_P1+j,Zeile,8);
			   if ( strlen(Zeile) >= 3 ) SetFocus(GetDlgItem(hDlg,IDC_CHECK1+j));
		  };

		  if ( HIWORD(wParam) == 0x0200 ) {
			   i=GetDlgItemInt(hDlg,IDC_P1+j,&Translated,FALSE);
 		       i--;
			   
			  if (( Translated == TRUE ) && ( i >= 0 ) && ( i < MAXPROGS )) {
			  if ( Programm[i].freq == 0 ) {
 					MessageBox(hDlg,"Speicherplatz nicht belegt","Multi-Decode",MB_ICONSTOP|MB_OK);
					SetFocus(GetDlgItem(hDlg,IDC_P1+j));
                    return(TRUE);
			  };
			  if ( Programm[i].Typ == 'A' ) {
 					MessageBox(hDlg,"Speicherplatz enthällt ANALOGES Programm","Multi-Decode",MB_ICONSTOP|MB_OK);
					SetFocus(GetDlgItem(hDlg,IDC_P1+j));
                    return(TRUE);
			  };

			  SetDlgItemText(hDlg,IDC_PNAME1+j,Programm[i].Name);
			};
		  }
	  };

	  if ( LOWORD(wParam) == IDC_SH1+j ) {
		  if ( HIWORD(wParam) == 0x0300 ) {
			   GetDlgItemText(hDlg,IDC_SH1+j,Zeile,8);
			   if ( strlen(Zeile) >= 2 ) SetFocus(GetDlgItem(hDlg,IDC_SM1+j));
		  };
		  if ( HIWORD(wParam) == 0x0200 ) {
			   i=GetDlgItemInt(hDlg,IDC_SH1+j,&Translated,FALSE);
		  if ( Translated == TRUE ) {
			  if (( i < 0 ) ||( i > 24 )) {
 					MessageBox(hDlg,t("Stunden zwischen 0 und 23 Uhr !!!"),t("Multi-Decode"),MB_ICONSTOP|MB_OK);
					SetFocus(GetDlgItem(hDlg,IDC_SH1+j));
			        return(TRUE); 	
			  };
		  };
		  }
		 }

	  if ( LOWORD(wParam) == IDC_SM1+j ) {
		  if ( HIWORD(wParam) == 0x0300 ) {
			   GetDlgItemText(hDlg,IDC_SM1+j,Zeile,8);
			   if ( strlen(Zeile) >= 2 ) SetFocus(GetDlgItem(hDlg,IDC_EH1+j));
		  };
		  if ( HIWORD(wParam) == 0x0200 ) {
			   i=GetDlgItemInt(hDlg,IDC_SM1+j,&Translated,FALSE);
		  if ( Translated == TRUE ) {
			  if (( i < 0 ) ||( i > 59 )) {
 					MessageBox(hDlg,t("Minuten zwischen 0 und 59 !!!"),t("Multi-Decode"),MB_ICONSTOP|MB_OK);
					SetFocus(GetDlgItem(hDlg,IDC_SM1+j));
			        return(TRUE); 	
			  };
		  };
		  }
		 }

	  if ( LOWORD(wParam) == IDC_EH1+j ) {
		  if ( HIWORD(wParam) == 0x0300 ) {
			   GetDlgItemText(hDlg,IDC_EH1+j,Zeile,8);
			   if ( strlen(Zeile) >= 2 ) SetFocus(GetDlgItem(hDlg,IDC_EM1+j));
		  };

		  if ( HIWORD(wParam) == 0x0200 ) {
			   i=GetDlgItemInt(hDlg,IDC_EH1+j,&Translated,FALSE);

			if ( Translated == TRUE ) {
			  if (( i < 0 ) ||( i > 24 )) {
 					MessageBox(hDlg,t("Stunden zwischen 0 und 23 Uhr !!!"),t("Multi-Decode"),MB_ICONSTOP|MB_OK);
					SetFocus(GetDlgItem(hDlg,IDC_EH1+j));
			        return(TRUE); 	
			  };
		  };
		  }
		 }
	  
	  if ( LOWORD(wParam) == IDC_EM1+j ) {

		  if ( HIWORD(wParam) == 0x0300 ) {
			   GetDlgItemText(hDlg,IDC_EM1+j,Zeile,8);
			   if ( strlen(Zeile) >= 2 ) SetFocus(GetDlgItem(hDlg,IDC_DT1+j));
		  };
		  if ( HIWORD(wParam) == 0x0200 ) {
			   i=GetDlgItemInt(hDlg,IDC_EM1+j,&Translated,FALSE);
		  if ( Translated == TRUE ) {
			  if (( i < 0 ) ||( i > 59 )) {
 					MessageBox(hDlg,t("Minuten zwischen 0 und 59 !!!"),t("Multi-Decode"),MB_ICONSTOP|MB_OK);
					SetFocus(GetDlgItem(hDlg,IDC_EM1+j));
			        return(TRUE); 	
			  };
		  };
		  }
		 }

	  if ( LOWORD(wParam) == IDC_DT1+j ) {
		  if ( HIWORD(wParam) == 0x0100 ) {
			   i=GetDlgItemInt(hDlg,IDC_DT1+j,&Translated,FALSE);
		  if ( Translated == FALSE ) {
               GetLocalTime(&SystemTime);
			   SetDlgItemInt(hDlg,IDC_DT1+j,SystemTime.wDay,FALSE);
		  }
		  }
		  if ( HIWORD(wParam) == 0x0300 ) {
			   GetDlgItemText(hDlg,IDC_DT1+j,Zeile,8);
			   if ( strlen(Zeile) >= 2 ) SetFocus(GetDlgItem(hDlg,IDC_DM1+j));
		  };
		  if ( HIWORD(wParam) == 0x0200 ) {
			   i=GetDlgItemInt(hDlg,IDC_DT1+j,&Translated,FALSE);
		  if ( Translated == TRUE ) {
			  if (( i < 1 ) ||( i > 31 )) {
 					MessageBox(hDlg,t("Tage zwischen 1 und 31 !!!"),t("Multi-Decode"),MB_ICONSTOP|MB_OK);
					SetFocus(GetDlgItem(hDlg,IDC_DT1+j));
			        return(TRUE); 	
			  };
		  };
		  }
		 }



	  if ( LOWORD(wParam) == IDC_DM1+j ) {

 		  if ( HIWORD(wParam) == 0x0300 ) {
			   GetDlgItemText(hDlg,IDC_DM1+j,Zeile,8);
			   if ( strlen(Zeile) >= 2 ) SetFocus(GetDlgItem(hDlg,IDC_DJ1+j));
		  };

		  if ( HIWORD(wParam) == 0x0100 ) {
			   i=GetDlgItemInt(hDlg,IDC_DM1+j,&Translated,FALSE);
		  if ( Translated == FALSE ) {
               GetLocalTime(&SystemTime);
			   SetDlgItemInt(hDlg,IDC_DM1+j,SystemTime.wMonth,FALSE);
		  }
		  }
		  
		  if ( HIWORD(wParam) == 0x0200 ) {
			   i=GetDlgItemInt(hDlg,IDC_DM1+j,&Translated,FALSE);
		  if ( Translated == TRUE ) {
			  if (( i < 1 ) ||( i > 12 )) {
 					MessageBox(hDlg,t("Monat zwischen 1 und 12 !!!"),t("Multi-Decode"),MB_ICONSTOP|MB_OK);
					SetFocus(GetDlgItem(hDlg,IDC_DM1+j));
			        return(TRUE); 	
			  };
		  };
		  }
		 }

	  if ( LOWORD(wParam) == IDC_DJ1+j ) {

		  if ( HIWORD(wParam) == 0x0300 ) {
			   GetDlgItemText(hDlg,IDC_DJ1+j,Zeile,8);
			   if ( strlen(Zeile) >= 4 ) SetFocus(GetDlgItem(hDlg,IDC_P1+j));
		  };

		  if ( HIWORD(wParam) == 0x0100 ) {
			   i=GetDlgItemInt(hDlg,IDC_DJ1+j,&Translated,FALSE);
		  if ( Translated == FALSE ) {
               GetLocalTime(&SystemTime);
			   SetDlgItemInt(hDlg,IDC_DJ1+j,SystemTime.wYear,FALSE);
		  }
		  }
	  };

}
			if (LOWORD(wParam) == IDOK ) {
            	  for ( i=0; i < TIMERANZAHL; i++ ) {
					TimerRec[i].StartZeit_h = 0xff;
      			    Is_Set=TRUE;
					TimerRec[i].Programm=GetDlgItemInt(hDlg,IDC_P1+i,&Translated,FALSE)-1;
					if ( Translated == FALSE ) Is_Set=FALSE;
					TimerRec[i].StartZeit_h=GetDlgItemInt(hDlg,IDC_SH1+i,&Translated,FALSE);
					if ( Translated == FALSE ) Is_Set=FALSE;
					TimerRec[i].StartZeit_m=GetDlgItemInt(hDlg,IDC_SM1+i,&Translated,FALSE);
					if ( Translated == FALSE ) Is_Set=FALSE;
					j=TimerRec[i].EndZeit_h;
					TimerRec[i].EndZeit_h=GetDlgItemInt(hDlg,IDC_EH1+i,&Translated,FALSE);
					if ( Translated == FALSE ) {
					    if ( j == 0xff ) TimerRec[i].EndZeit_h=0xff;
						else Is_Set=FALSE;
					};
					j=TimerRec[i].EndZeit_m;
					TimerRec[i].EndZeit_m=GetDlgItemInt(hDlg,IDC_EM1+i,&Translated,FALSE);
					if ( Translated == FALSE ) {
					    if ( j == 0xff ) TimerRec[i].EndZeit_m=0xff;
						else Is_Set=FALSE;
					};
					tag=GetDlgItemInt(hDlg,IDC_DT1+i,&Translated,FALSE);
					if ( Translated == FALSE ) Is_Set=FALSE;
					monat=GetDlgItemInt(hDlg,IDC_DM1+i,&Translated,FALSE);
					if ( Translated == FALSE ) Is_Set=FALSE;
					jahr=GetDlgItemInt(hDlg,IDC_DJ1+i,&Translated,FALSE);
					if ( Translated == FALSE ) Is_Set=FALSE;
                    if ( Is_Set == TRUE ) TimerRec[i].Date_Id=GetDateID(jahr,monat,tag);
					if ( TimerRec[i].Date_Id == 0 ) Is_Set=FALSE;
					TimerRec[i].RecorderArt=SendMessage(GetDlgItem(hDlg,IDC_COMBO1+i),CB_GETCURSEL,0,0);
                    TimerRec[i].Shutdown=IsDlgButtonChecked(hDlg, IDC_CHECK1+i);
					if ( Is_Set == FALSE ) TimerRec[i].StartZeit_h = 0xff;
				  }
					
				
				EndDialog(hDlg,TRUE);
			}


			if (LOWORD(wParam) == IDCANCEL ) {

				EndDialog(hDlg,TRUE);
			}

			 break;
    }

return(FALSE);
}

BOOL APIENTRY RecorderSettingProc
(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
	int i;
    char Zeile[64];

	switch (message) {
        case WM_INITDIALOG:

			sprintf(Zeile,"%d",MAXMegaBytes);
			SetDlgItemText(hDlg,IDC_EDIT9,Zeile);
			SetDlgItemText(hDlg,IDC_EDIT1,RecordBaseDir[0]);
			SetDlgItemText(hDlg,IDC_EDIT2,RecordBaseDir[1]);
			SetDlgItemText(hDlg,IDC_EDIT3,RecordBaseDir[2]);
			SetDlgItemText(hDlg,IDC_EDIT4,RecordBaseDir[3]);
			SetDlgItemText(hDlg,IDC_EDIT5,RecordBaseDir[4]);
			SetDlgItemText(hDlg,IDC_EDIT6,RecordBaseDir[5]);
			SetDlgItemText(hDlg,IDC_EDIT7,RecordBaseDir[6]);
			SetDlgItemText(hDlg,IDC_EDIT8,RecordBaseDir[7]);

			if ( RecorderFormat == 0 ) CheckDlgButton(hDlg, IDC_RADIO1, TRUE);
             else if ( RecorderFormat == 1 ) CheckDlgButton(hDlg, IDC_RADIO2, TRUE);
             else if ( RecorderFormat == 2 ) CheckDlgButton(hDlg, IDC_RADIO3, TRUE);
			if ( RecorderTyp == 0 ) CheckDlgButton(hDlg, IDC_RADIO4, TRUE);
             else if ( RecorderTyp == 1 ) CheckDlgButton(hDlg, IDC_RADIO5, TRUE);
			 break;


  case WM_COMMAND:
			if (LOWORD(wParam) == IDOK ) {
				GetDlgItemText(hDlg,IDC_EDIT1,RecordBaseDir[0],127);
				GetDlgItemText(hDlg,IDC_EDIT2,RecordBaseDir[1],127);
				GetDlgItemText(hDlg,IDC_EDIT3,RecordBaseDir[2],127);
				GetDlgItemText(hDlg,IDC_EDIT4,RecordBaseDir[3],127);
				GetDlgItemText(hDlg,IDC_EDIT5,RecordBaseDir[4],127);
				GetDlgItemText(hDlg,IDC_EDIT6,RecordBaseDir[5],127);
				GetDlgItemText(hDlg,IDC_EDIT7,RecordBaseDir[6],127);
				GetDlgItemText(hDlg,IDC_EDIT8,RecordBaseDir[7],127);

				i=(int)CreateDirectory(RecordBaseDir[0],NULL);
			i=(int)CreateDirectory(RecordBaseDir[1],NULL);
			i=(int)CreateDirectory(RecordBaseDir[2],NULL);
			i=(int)CreateDirectory(RecordBaseDir[3],NULL);
			i=(int)CreateDirectory(RecordBaseDir[4],NULL);
			i=(int)CreateDirectory(RecordBaseDir[5],NULL);
			i=(int)CreateDirectory(RecordBaseDir[6],NULL);
			i=(int)CreateDirectory(RecordBaseDir[7],NULL);

			GetDlgItemText(hDlg,IDC_EDIT9,Zeile,64);
			MAXMegaBytes=atoi(Zeile);
			if (! NT && MAXMegaBytes >= 4095) {
				MessageBox(hDlg, t("On Win 9X, the filesize is limited to 4096 MB !"), t("Multi-Decode"), MB_OK|MB_ICONINFORMATION);
				MAXMegaBytes = 4095;
			}
				if ( IsDlgButtonChecked(hDlg, IDC_RADIO1) == TRUE ) RecorderFormat=0;
                  else if ( IsDlgButtonChecked(hDlg, IDC_RADIO2) == TRUE ) RecorderFormat=1;
                  else if ( IsDlgButtonChecked(hDlg, IDC_RADIO3) == TRUE ) RecorderFormat=2;
				if ( IsDlgButtonChecked(hDlg, IDC_RADIO4) == TRUE ) RecorderTyp=0;
                  else if ( IsDlgButtonChecked(hDlg, IDC_RADIO5) == TRUE ) RecorderTyp=1;
				EndDialog(hDlg,TRUE);
			}


			if (LOWORD(wParam) == IDCANCEL ) {
				EndDialog(hDlg,TRUE);
			}

			 break;
    }

return(FALSE);
}


void OSD_Rec( void )
{
    BITMAPINFO *TESTBITMAP = NULL;
    HGLOBAL hGlobal;
    unsigned short ch, cl;
    unsigned int yuv;
    BOOL RP;

    PROGRAMM_OSD_RUN=TRUE;
    if ( Recorder_Pause == FALSE ) hGlobal=LoadResource(hInst,FindResource(hInst,"RECORD",RT_BITMAP));
      else hGlobal=LoadResource(hInst,FindResource(hInst,"RECORDPAUSE",RT_BITMAP));

    RP=Recorder_Pause ;

    TESTBITMAP=(BITMAPINFO*)LockResource(hGlobal);

    CreateOSDWindow( 1, (unsigned short)BITMAP4, (unsigned short)TESTBITMAP->bmiHeader.biWidth, (unsigned short)TESTBITMAP->bmiHeader.biHeight);
    MoveWindowAbs(1,(unsigned short)(710-TESTBITMAP->bmiHeader.biWidth),10);
    SetColorBlend(1);
    yuv=RGB2YUV(0,0,0);
    cl=(yuv&0xffff);
    ch=((yuv>>16)&0xffff);
    SetColor_(1, 16,0, ch, cl);
    SetBlend_(1,16,0, 0);
    DrawBlock(1, 0, 0,(unsigned short)TESTBITMAP->bmiHeader.biWidth, (unsigned short)TESTBITMAP->bmiHeader.biHeight,0);

    if ( OSDLoadBitmap4(1,TESTBITMAP,0,0,0,0,0,0,1000) < 0 ) {
                DestroyOSDWindow(1);
			    DeleteObject(TESTBITMAP);
                Osd_Thread_Down();
                return;
    };
    while (( Record == TRUE ) && ( Recorder_Pause == RP )) {
    if ( WaitForSingleObject(OSD_KEY_EVENT, 100) != WAIT_TIMEOUT ) {
                        DestroyOSDWindow(1);
                        DeleteObject(TESTBITMAP);
                        Osd_Thread_Down();
                        return;
			     };
    };
    DestroyOSDWindow(1);
    DeleteObject(TESTBITMAP);
    Osd_Thread_Down();
};



unsigned int Get_Seconds(unsigned char *MPG_Header)
{
    unsigned int pti, len;
    if ( MPG_Header[0] != 0) return (0);
    if ( MPG_Header[1] != 0) return (0);
    if ( MPG_Header[2] != 1) return (0);
    if ( MPG_Header[3] < AUDIO_STREAM_S || MPG_Header[3] > VIDEO_STREAM_E) return (0);
    if ((MPG_Header[6] & 0xc0) == 0x80) { // MPEG2
	if ((MPG_Header[7] & PTS_ONLY) == 0) return (0);
        len = 9;
    } else { // MPEG1
	len = 6;
	while (MPG_Header[len] == 0xff) {
	    len++;
	    if (len > 22) return (0);
	}
	if ((MPG_Header[len] & 0xc0) == 0x40) {
	    len += 2;
	}
	if ((MPG_Header[len] & 0x20) == 0) return (0);
    }

    pti = ((MPG_Header[len] >> 1) << 29) |
	  (MPG_Header[len+1] << 21) | ((MPG_Header[len+2] >> 1) << 14) |
	  (MPG_Header[len+3] << 6) | (MPG_Header[len+4] >> 2); // Low order bit dropped
    return(pti/45000);
}


void OSD_Play_Stop( void )
{
    char Zeile[60];

    PROGRAMM_OSD=TRUE;

    CreateOSDWindow( 1, 1, 360, 64);
    MoveWindowAbs(1,200,100 );
    SetColorBlend(1);
    OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
    OSDSetColor(1,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
    OSDSetColor(1,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
    OSDSetColor(1,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);

    DrawBlock(1, 0, 0,360, 64,0);
    DrawBlock(1, 4, 4,352, 56,1);

    SetFont( 1, 1, 3, 1);
    if ( Play == TRUE ) {
	    strcpy(Zeile,t("DVR Wiedergabe ist"));
        WriteText( 1, 80, 4,Zeile);
    } else if ( Record == TRUE) {
	    strcpy(Zeile,t("DVR Aufnahme ist"));
        WriteText( 1, 80, 4,Zeile);
    } else {
	    strcpy(Zeile,t("Digitaler Video Recorder"));
        WriteText( 1, 64, 4,Zeile);
    };
    strcpy(Zeile,t("beendet"));
    WriteText( 1, 120,32,Zeile);
    ResetEvent(OSD_KEY_EVENT);
    WaitForSingleObject (OSD_KEY_EVENT, 4000);
    DestroyOSDWindow(1);
    Osd_Thread_Down();
    return;
};


int Load_Next_Buffer(int *fd, int Typ , unsigned char *Daten)
{
    if ( RecorderTyp == 0 ) {
	    return(Load_Next_Buffer_mpg(fd, Typ , Daten));
    } else if ( RecorderTyp == 1 ) {
	    return(Load_Next_Buffer_pav(fd, Typ , Daten));
    };
    return(0);
};




int Load_Next_Buffer_mpg(int *fd, int Typ , unsigned char *Daten)
{
    int i,len;
    // Mpeg_Lade_Routine 
    
    if ( *fd < 0 ) return(-1);
    while (TRUE) {
        i=read(*fd, Daten, 6);
        if ( i < 6 ) {
            if (( i == 4 ) && 
                ( Daten[0] == 0x00 ) &&
                ( Daten[1] == 0x00 ) &&
                ( Daten[2] == 0x01 ) &&
                ( Daten[3] == PROGRAM_END )) {
                return(0);
                // Reguläres EOF 
            };
            
            // Test EOF	
            return(-1);
        };
        
        if ( Is_Valid_Sync(Daten) == FALSE ) {
            Resync_MPG_File(fd);
            continue;
        };

        if (Daten[3] == SEQUENCE_END_CODE) {
            _lseeki64(*fd, (__int64)-2, 1);
            return(4);
        }

        if (Daten[3] == PROG_PACK_START) {
            i = read(*fd, Daten+6, 8);
            if (i < 8) return(-1);
            if (Daten[13] & 7) {
                i = read(*fd, Daten+14, Daten[13] & 7);
                if (i < (Daten[13]&7)) return(-1);
            }
            return (14+(Daten[13]&7));
        }

        len=Get_Length(Daten);
        
        if (( len <= 11 ) || ( len > 2048 )) {
            Resync_MPG_File(fd);
            continue;
        };
        
        i=read(*fd, Daten+6,len-6);
        if ( i != len-6 ) {
            // Interrupt
            return(-1);
        }
        
        if ( Typ == 0 ) {
            // Video 
            VideoBlocks++;
            if ( Is_Video_Sync(Daten) == TRUE ) return(len);
        };
        
        if ( Typ == 1 ) {
            // Audio 
            AudioBlocks++;
            if ( Is_Audio_Sync(Daten) == TRUE ) return(len);
        };
        
        if ( Typ == 3 ) return(len); // Any Type
    }
    
    return(-1);
};


int Load_One_PAV_Buffer(int *fd, unsigned char *Daten)
{
	int i;
    int len;

    if ( *fd < 0 ) return(-1);
Next_pav_Read:

	i=read(*fd,&Daten[0],8);
	if ( i < 8 ) {
		if (( i == 4 ) && 
			( Daten[0] == 0x00 ) &&
			( Daten[1] == 0x00 ) &&
			( Daten[2] == 0x00 ) &&
			( Daten[3] == 0x00 )) {
				       return(0);
                       // Reguläres EOF 
			 };

		// Test EOF	
		return(-1);
	};

	len=GetAvPesLength(Daten);

     if ( len==0  ) {
		 Resync_AV_File(fd);
		 goto Next_pav_Read;
	 };

   	 i=read(*fd,&Daten[8],len-8);
	 if ( i != len-8 ) {
			 // Interrupt
		 return(-1);
	 };

	 
	 return(len);
};


int Load_Next_Buffer_pav(int *fd, int Typ , unsigned char *Daten)
{
int i,len;
// pav_Lade_Routine 

   

   if ( Typ == 0 ) {
Next_Video_Check:
	   if ( PAV_Video_Buffer_Length <= PAV_Video_Buffer_Pos ) {
		   Next_Video_Search:
		   i=Load_One_PAV_Buffer(fd,&Mpeg_Buffer[0]);
		   if ( i < 0 ) return(-1);
		   if ( GetAvPesType(&Mpeg_Buffer[0]) != AV_PES_VIDEO ) {
               VideoBlocks+=(i/0x800)+1;
			   goto Next_Video_Search;
		   };
	       PAV_Video_Buffer_Pos=0;
           memset(&PAV_Video_Buffer[0],0x00,MPEG_BUFFER_SIZE);
		   PAV_Video_Buffer_Length=av_pes_to_pes(&Mpeg_Buffer[0], i, &AV_Pes2Mpeg, &PAV_Video_Buffer[0]);
	   };
	   len=Get_Length(&PAV_Video_Buffer[PAV_Video_Buffer_Pos]);
	   if (( len <= 11 ) || ( len > 2048 )) {
		 PAV_Video_Buffer_Pos+=len;
		 goto Next_Video_Check;
	 };
	   VideoBlocks++;
       memcpy(Daten,&PAV_Video_Buffer[PAV_Video_Buffer_Pos],len);
       PAV_Video_Buffer_Pos+=len;
	   return(len);
   };

   if ( Typ == 1 ) {
     Next_Audio_Check:
	   if ( PAV_Audio_Buffer_Length <= PAV_Audio_Buffer_Pos ) {
		   Next_Audio_Search:
		   i=Load_One_PAV_Buffer(fd,&Mpeg_Buffer[0]);
		   if ( i < 0 ) return(-1);
		   if ( GetAvPesType(&Mpeg_Buffer[0]) != AV_PES_AUDIO ) {
               AudioBlocks+=(i/0x800)+1;
			   goto Next_Audio_Search;
		   };
	       PAV_Audio_Buffer_Pos=0;
           memset(&PAV_Audio_Buffer[0],0x00,MPEG_BUFFER_SIZE);
		   PAV_Audio_Buffer_Length=av_pes_to_pes(&Mpeg_Buffer[0], i, &AV_Pes2Mpeg, &PAV_Audio_Buffer[0]);
	   };
	   len=Get_Length(&PAV_Audio_Buffer[PAV_Audio_Buffer_Pos]);
	   if (( len <= 11 ) || ( len > 2048 )) {
		 PAV_Audio_Buffer_Pos+=len;
		 goto Next_Audio_Check;
	 };
       memcpy(Daten,&PAV_Audio_Buffer[PAV_Audio_Buffer_Pos],len);
	   AudioBlocks++;
       PAV_Audio_Buffer_Pos+=len;
	   return(len);
   };



   if ( Typ == 3 ) {
	   if ( PAV_Video_Buffer_Length <= PAV_Video_Buffer_Pos ) {
		   i=Load_One_PAV_Buffer(fd,&Mpeg_Buffer[0]);
		   if ( i < 0 ) return(-1);
	       PAV_Video_Buffer_Pos=0;
		   PAV_Video_Buffer_Length=av_pes_to_pes(&Mpeg_Buffer[0], i, &AV_Pes2Mpeg, &PAV_Video_Buffer[0]);
	   };
	   len=Get_Length(&PAV_Video_Buffer[PAV_Video_Buffer_Pos]);
       memcpy(Daten,&PAV_Video_Buffer[PAV_Video_Buffer_Pos],len);
       PAV_Video_Buffer_Pos+=len;
	   return(len);
   };


return(-1);
};



void Init_Rec_Play_Buffer( void )
{
    if ( Mpeg_Buffer == NULL ) {
        Mpeg_Buffer=(unsigned char *)HeapAlloc(MultiDec_Heap,HEAP_ZERO_MEMORY , MPEG_BUFFER_SIZE);
    }
    
    if ( PAV_Audio_Buffer == NULL ) {
        PAV_Audio_Buffer=(unsigned char *)HeapAlloc(MultiDec_Heap,HEAP_ZERO_MEMORY , MPEG_BUFFER_SIZE);
    }
    if ( PAV_Video_Buffer == NULL ) {
        PAV_Video_Buffer=(unsigned char *)HeapAlloc(MultiDec_Heap,HEAP_ZERO_MEMORY , MPEG_BUFFER_SIZE);
    }
    Mpeg_Buffer_Pos=0;
    Mpeg_Buffer_Length=0;
    PAV_Audio_Buffer_Pos=0;
    PAV_Audio_Buffer_Length=0;
    PAV_Video_Buffer_Pos=0;
    PAV_Video_Buffer_Length=0;
    Init_PAV();
};

void Close_Rec_Play_Buffer( void )
{
    Mpeg_Buffer_Pos=0;
    Mpeg_Buffer_Length=0;
    PAV_Audio_Buffer_Pos=0;
    PAV_Audio_Buffer_Length=0;
    PAV_Video_Buffer_Pos=0;
    PAV_Video_Buffer_Length=0;
    
    if ( Mpeg_Buffer != NULL ) {
        HeapFree(MultiDec_Heap,0,Mpeg_Buffer);
        Mpeg_Buffer=NULL;
    }
    if ( PAV_Audio_Buffer != NULL ) {
        HeapFree(MultiDec_Heap,0,PAV_Audio_Buffer);
        PAV_Audio_Buffer=NULL;
    }
    if ( PAV_Video_Buffer != NULL ) {
        HeapFree(MultiDec_Heap,0,PAV_Video_Buffer);
        PAV_Video_Buffer=NULL;
    }

}



/******************/

// MPG Stuff


int cwrite_av_pes(unsigned char *buf, av_pes_packet *p, long length){
	long count;
	unsigned char sync[2] = { 0x41, 0x56} ;
	
	count = 0;
	if (length < MAX_AV) return(0);
	if (! p->length) return 0;
	nlength_av_pes(p);
	buf[0] = sync[0];
	buf[1] = sync[1];
	count = 2;
	if (p->gap){
		p->flags |= (p->gap & 0x03) << 2;
	}
	buf[count] = p->stream_id;
	count++;
	buf[count] = p->counter;
	count++;
	buf[count] = p->reserved;
	count++;
	buf[count] = p->flags;
	count++;
	buf[count] = p->llength[0];
	count++;
	buf[count] = p->llength[1];
	count++;

	if ((p->flags & PTS_FLAG) && (p->stream_id == AV_VIDEO)){
		memcpy(buf+count,p->pts,4);
		count += 4;
		memcpy(buf+count,p->payload,p->length);
		count += p->length;
	} else {
		memcpy(buf+count,p->payload,p->length);
		count += p->length;
	}

	return count;

}

void nlength_av_pes(av_pes_packet *p){
	unsigned short *ll = (short *) p->llength;
	unsigned short l = p->length;
	if ((p->flags & PTS_FLAG) && (p->stream_id == AV_VIDEO)) l+= 4;
	*ll = myhtons(l);
}


int write_av_pes(av_pes_packet *p,unsigned char *AV_PesBuffer){
	long length;
	length = cwrite_av_pes(AV_PesBuffer,p,MAX_AV+8);
    return(length);

}


int kwrite_video(av_pes_packet *apsv,unsigned char *AV_PesBuffer){
	int gap = apsv->length & 3;
    int ret;
	apsv->gap = gap;
	apsv->length -= gap;
	ret=write_av_pes(apsv,AV_PesBuffer);
	if (gap) memmove(apsv->payload, apsv->payload+apsv->length,gap);
	apsv->flags = gap;
	apsv->length = gap;
	apsv->counter++;
    return(ret);
}

int kwrite_audio(av_pes_packet *apsa,unsigned char *AV_PesBuffer){
int ret;
	ret=write_av_pes(apsa,AV_PesBuffer);
	apsa->flags = 0;
	apsa->length = 0;
	apsa->counter ++;
    return(ret);
}


unsigned short myhtons(unsigned short val)
{
    unsigned short i;
// Damit ich nicht die WinSock brauche !! 
// Da werden immer einige Leute nervös 
//	i=htons(val);
	i=(val >> 8 ) + ((val&0x00ff)<<8);
	return(i);
};

unsigned short myntohs(unsigned short val)
{
    unsigned short i;
// Damit ich nicht die WinSock brauche !! 
// Da werden immer einige Leute nervös 
//	i=ntohs(val);
	i=(val >> 8 ) + ((val&0x00ff)<<8);
	return(i);
};

unsigned int myhtonl(unsigned int val)
{
    unsigned int i;
// Damit ich nicht die WinSock brauche !! 
// Da werden immer einige Leute nervös 
//	i=htonl(val);
	i=(val >> 24) + (((val>>16)&0xff)<<8) + (((val>>8)&0xff)<<16) + ((val&0xff)<<24);
	return(i);
};

unsigned int myntohl(unsigned int val)
{
    unsigned int i;
// Damit ich nicht die WinSock brauche !! 
// Da werden immer einige Leute nervös 
//	i=ntohl(val);
	i=(val >> 24) + (((val>>16)&0xff)<<8) + (((val>>8)&0xff)<<16) + ((val&0xff)<<24);
	return(i);
};


void pts2pts(unsigned char *av_pts, unsigned char *pts)
{
	av_pts[0] = ((pts[0] & 0x06) << 5) | 
		((pts[1] & 0xFC) >> 2); 
	av_pts[1] = ((pts[1] & 0x03) << 6) |
		((pts[2] & 0xFC) >> 2); 
	av_pts[2] = ((pts[2] & 0x02) << 6) |
			((pts[3] & 0xFE) >> 1);
	av_pts[3] = ((pts[3] & 0x01) << 7) |
		((pts[4] & 0xFE) >> 1);
	
}



void init_ps(ps_packet *p)
{
	p->stuff_length=0xF8;
	p->data = NULL;
	p->sheader_length = 0;
	p->audio_bound = 0;
	p->video_bound = 0;
	p->npes = 0;
	p->mpeg = 2;
}

void inc_scr(a2p *p)
{
	unsigned int *s1,s2,*s3;
	
	s1 = (unsigned int *) p->scr;
	s3 = (unsigned int *) p->last_av_pts;
	s2 = myntohl(*s3);
	s2 -= 9000;
	*s1 = myhtonl(s2);
//	fprintf (stderr,"scr: %d\n",s2);
}

void get_pespts(unsigned char *av_pts,unsigned char *pts)
{
	
	pts[0] = 0x21 | 
		((av_pts[0] & 0xC0) >>5);
	pts[1] = ((av_pts[0] & 0x3F) << 2) |
		((av_pts[1] & 0xC0) >> 6);
	pts[2] = 0x01 | ((av_pts[1] & 0x3F) << 2) |
		((av_pts[2] & 0x80) >> 6);
	pts[3] = ((av_pts[2] & 0x7F) << 1) |
		((av_pts[3] & 0x80) >> 7);
	pts[4] = 0x01 | ((av_pts[3] & 0x7F) << 1);
}



int w_tsh(unsigned char id,int length ,unsigned char *pts, unsigned char *obuf,a2p *p, int startpes)
{
	int c = 0;
	unsigned char tsheadv[4] = { 0x47, 0x00, 0xa0, 0x10}; 
	unsigned char tsheada[4] = { 0x47, 0x00, 0x50, 0x10}; 

	if (id == 0xE0){
		if (startpes) tsheadv[1] = 0x40;
		tsheadv[3] |= (p->pidv++ & 0x0F);
		memcpy(obuf+c,tsheadv,4);
		c+=4;
		if (startpes) c += w_pesh(id,0,pts,obuf+c);
	} 

	if (id == 0xC0){
		if (startpes) tsheada[1] = 0x40;
		tsheada[3] |= (p->pida++ & 0x0F) ;
		memcpy(obuf+c,tsheada,4);
		c+=4;	
	}
	
	return c;
}

int w_pesh(unsigned char id,int length ,unsigned char *pts, unsigned char *obuf)
{
	unsigned char le[2];
	unsigned char dummy[6];
	int c;
	unsigned char headr[3] = {0x00, 0x00, 0x01};
	
	c = 0;
	memcpy(obuf+c,headr,3);
	c += 3;
	memcpy(obuf+c,&id,1);
	c++;

	le[0] = 0;
	le[1] = 0;
	if (length){
		if (pts) length+=8;
		else length+=3;
	}
	le[0] |= ((unsigned char)(length >> 8) & 0xFF); 
	le[1] |= ((unsigned char)(length) & 0xFF); 
	memcpy(obuf+c,le,2);
	c += 2;

	dummy[0] = 0x80;
	dummy[1] = 0;
	dummy[2] = 0;
	if (pts){
		dummy[1] |= PTS_ONLY;
		dummy[2] = 0x05;
	}

	memcpy(obuf+c,dummy,3);
	c += 3;

	if (pts){
		memcpy(obuf+c,pts,5);
		c += 5;
	}

	if ( id == 0xE0 ) {
        if ( *(obuf+8) == 0x00 ) {
          c+=3;
          *(obuf+8)=0x03;
          *(obuf+9)=0xff;
          *(obuf+10)=0xff;
          *(obuf+11)=0xff;
		} else if ( *(obuf+8) == 0x05 ) {
          c+=2;
		  *(obuf+8)=0x07;
          *(obuf+14)=0xff;
          *(obuf+15)=0xff;

		};

	}
	return c;
}


int av_pes_to_pes(unsigned char *buf,int count, a2p *p,unsigned char *MpegBuffer )
{
	int c=0;
	int l,j;

	unsigned char pts[5];

	int BytesWritten=0;

	unsigned char tspid0[188] = { 0x47,0x40,0x00,0x10,0x00,0x00,0xb0,0x11,0x00,0x00,0xcb,0x00,0x00,0x00,0x00,0xe0,0x10,0x00,0x01,0xe4,0x00,0x2a,0xd6,0x1a,0xfb,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

	unsigned char tspid1[188] = { 0x47,0x44,0x00,0x10,0x00,0x02,0xb0,0x1c,0x00,0x01,0xcb,0x00,0x00,0xe0,0xa0,0xf0,0x05,0x48,0x03,0x01,0x00,0x00,0x02,0xe0,0xa0,0xf0,0x00,0x03,0xe0,0x50,0xf0,0x00,0xae,0xea,0x4e,0x48,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

	BytesWritten=0;


	while (c < count && p->found < 2 ){
		if (buf[c] == 0x41 ) p->found = 1;
		c++;
		if (p->found && buf[c] == 0x56) {
			p->found = 2;
			c++;
		} else p->found = 0;
	}
	
	if ( c == count ) return(BytesWritten);
 
	if ( p->found == 2 ){
		p->cid = buf[c];
		c++;
		p->found++;
	}
	
	if ( c == count ) return(BytesWritten);

        if ( p->found > 2 ){
		while ( c < count && p->found < 5){
			c++;
			p->found++;
		}
	}
	
	if ( c == count ) return(BytesWritten);
	
	if ( p->found == 5 ){
		p->flags = buf[c];
		c++;
		p->found++;
	}

	if ( c == count ) return(BytesWritten);

	if ( p->found == 6 ){
		while ( c < count && p->found < 8){
			if ( p->found == 6 ) {
				p->length =(int)( ( buf[c] & 0xFF ) << 8);
				c++;
				p->found++;
			} else {
				p->length |= ( buf[c] & 0xFF );
				c++;
				p->found++;
			}
		}
	}

	
	if ( p->found > 7 ) {
	
		while ( p->type != TS_STREAM &&
			c < count && p->found < p->length+8){
			switch (p->cid){
			case 0x01:
				if ( p->found == 8 && (p->flags & PTS_FLAG) 
				     && p->vlength ){
					l = p->vlength - 6;
					p->vbuf[4] = 0;
					p->vbuf[5] = 0;
					p->vbuf[4] |= ((unsigned char)(l >> 8) & 0xFF); 
					p->vbuf[5] |= ((unsigned char)(l) & 0xFF); 
				    memcpy(&MpegBuffer[BytesWritten],p->vbuf,p->vlength);
					BytesWritten+=p->vlength;
					p->vlength=0;
					p->headr = 0;
				}
				
				if (!(p->flags & PTS_FLAG)&& !p->headr){
					p->vlength = w_pesh(0xE0,p->length,
							    NULL,p->vbuf);
					p->headr = 1;
				}
				
				if ( (p->flags & PTS_FLAG) && !p->headr){
					if ( p->found < 12 ){
						p->av_pts[p->found-8] = buf[c];
						c++;
						p->found++;
					}
					
					if ( p->found == 12 ){
						get_pespts(p->av_pts,pts);
					
						p->vlength = w_pesh(
							0xE0,p->length,
							pts, p->vbuf);
						
						p->headr = 1;
						memcpy(p->last_av_pts,
						       p->av_pts,4);
					}
				}

				if ( p->headr ){
					l = count - c;
					if ( l + p->found > p->length+8 )
						l = p->length - p->found+8;

					if (p->vlength+l < 0x800){
						memcpy(p->vbuf+p->vlength, 
						       buf+c, l);
						p->vlength += l;
						c += l;
						p->found += l;
					} else {
						l = 0x800 - p->vlength;
						memcpy(p->vbuf+p->vlength, 
						       buf+c, l);
						p->vlength += l;
						c += l;
						p->found += l;

						inc_scr(p);
						l = p->vlength - 6;
						p->vbuf[4] = 0;
						p->vbuf[5] = 0;
						p->vbuf[4] |= ((unsigned char)(l >> 8) 
							       & 0xFF); 
						p->vbuf[5] |= ((unsigned char)(l) 
							       & 0xFF); 
        			    memcpy(&MpegBuffer[BytesWritten],p->vbuf,p->vlength);
						BytesWritten+=p->vlength;
						p->vlength=0;
						p->headr = 0;
						p->flags = 0;
					}
				}
				break;

			case 0x02:
				l = count - c;
				if ( l + p->found > p->length+8 )
					l = p->length - p->found +8;
				
				if ( p->found == 8 && 
				     p->alength ){
					p->abuf[4] = 0;
					p->abuf[5] = 0;
					p->abuf[4] |= 
						(unsigned char)(((p->alength-6) >> 8) & 
						     0xFF); 
					p->abuf[5] |= 
						(unsigned char)((p->alength-6) & 
						     0xFF); 
       			    memcpy(&MpegBuffer[BytesWritten],p->abuf,p->alength);
					BytesWritten+=p->alength;
					if (
						(count -c >4 &&
						 buf[c] == 0x00 &&
						 buf[c+1] == 0x00 &&
						 buf[c+2] == 0x01 &&
						 buf[c+3] == 0xC0)
						){
						p->alength=0;
					} else {
						p->alength = w_pesh(
							0xC0,0,
							NULL, p->abuf);
					}
				}
				
				memcpy(p->abuf+p->alength,buf+c,l);
				p->alength += l;
				c += l;
				p->found += l;

				break;
			}
		}
	}
	
		  
	
	
	if ( p->length+8 == p->found ) {
		p->found = 0;
		p->length = 0;
		p->cid = 0;
		p->flags = 0;
	}
	j=0;
	if ( c < count ) j=av_pes_to_pes( buf+c, count-c, p,&MpegBuffer[BytesWritten] );
	if ( j < 0 ) return(-1);
        BytesWritten+=j;
	return(BytesWritten);		
}

int kpes_to_av_pes(unsigned char *buf, int *StartPos, int count, p2a *p,unsigned char *Next_Pes_Buffer)
{
    int ret=0;
	int l,max;
	int c;

	unsigned char headr[3] = { 0x00, 0x00, 0x01} ;

	c=*StartPos;

#define RETURN_IF_NO_DATA if (c == count || ret > 0) {*StartPos = c; return ret; } // No more chars, exit.

	while (c < count &&                       // stop when no more data
           p->mpeg != 1 &&                    // or mpeg_1 detected
           (!p->done || p->found < 6) &&      // or not audio/video/private and length read
           (p->mpeg == 0 || p->found < 9)) {  // or mpeg set and all flags read

		switch ( p->found ){
		case 0: case 1: // Find 2 or more 0x00 then 0x01
            if (buf[c] == 0x00) {
                p->found++;
            } else {
                p->found = 0;
            }
            c++;
			break;
		case 2:
            if (buf[c] == 0x01) {
                p->found++;
            } else if (buf[c] != 0x00) { 
				p->found = 0;
			}
			c++;
			break;
		case 3: // Stream ID
            switch (buf[c]) {
            case PROG_STREAM_MAP:
            case PRIVATE_STREAM2:
            case PROG_STREAM_DIR:
            case ECM_STREAM     :
            case EMM_STREAM     :
            case PADDING_STREAM :
            case DSM_CC_STREAM  :
            case ISO13522_STREAM:
                p->done = TRUE;
    			p->cid = 0;
                break;
            case PRIVATE_STREAM1:
            case AUDIO_STREAM_S   : case AUDIO_STREAM_S+ 1: case AUDIO_STREAM_S+ 2: case AUDIO_STREAM_S+ 3:
            case AUDIO_STREAM_S+ 4: case AUDIO_STREAM_S+ 5: case AUDIO_STREAM_S+ 6: case AUDIO_STREAM_S+ 7:
            case AUDIO_STREAM_S+ 8: case AUDIO_STREAM_S+ 9: case AUDIO_STREAM_S+10: case AUDIO_STREAM_S+11:
            case AUDIO_STREAM_S+12: case AUDIO_STREAM_S+13: case AUDIO_STREAM_S+14: case AUDIO_STREAM_S+15:
            case VIDEO_STREAM_S   : case VIDEO_STREAM_S+ 1: case VIDEO_STREAM_S+ 2: case VIDEO_STREAM_S+ 3:
            case VIDEO_STREAM_S+ 4: case VIDEO_STREAM_S+ 5: case VIDEO_STREAM_S+ 6: case VIDEO_STREAM_S+ 7:
            case VIDEO_STREAM_S+ 8: case VIDEO_STREAM_S+ 9: case VIDEO_STREAM_S+10: case VIDEO_STREAM_S+11:
            case VIDEO_STREAM_S+12: case VIDEO_STREAM_S+13: case VIDEO_STREAM_S+14: case VIDEO_STREAM_S+15:
				p->found++;
				p->cid = buf[c];
				c++;
                break;
            default:
			    p->found = 0;
    			p->cid = 0;
            }
            break;
		case 4: // Hi word of length
		    p->found++;
		    p->plen[0] = buf[c];
		    p->plength = buf[c]<<8;
            c++;
		    break;
		case 5: // Low lord of length
		    p->found++;
		    p->plen[1] = buf[c];
		    p->plength += buf[c];
		    c++;

#define MMAX_PLENGTH (4*MAX_PLENGTH)
            if (p->plength == 0) p->plength = MAX_PLENGTH - 6;

		    break;
        case 6: // First flags
			p->found++;
			p->flag1 = buf[c];
			c++;
            if ( (p->flag1 & 0xC0) == 0x80 ) {
                p->mpeg = 2;
            } else {
				// The flow is not MPEG2. May be MPEG1...
                p->hlength = 0;
				p->which = 0;
				p->mpeg = 1;
				p->flag2 = 0;
				p->check = p->flag1;
			}
            break;
        case 7: // Next flags
            if (p->mpeg == 2) {
                p->flag2 = buf[c++];
                p->found++;
            }
            break;
        case 8: // Header length
            if (p->mpeg == 2) {
                p->hlength = buf[c++];
                p->found++;
            }
            break;
        }
	}

    RETURN_IF_NO_DATA;

    if ( p->done || 
	     p->mpeg == 1 || 
         p->mpeg == 2 && p->found >= 9){

        if (p->cid >= VIDEO_STREAM_S && p->cid <= VIDEO_STREAM_E) {

            if (p->mpeg == 2) {
                if ((p->flag2 & PTS_ONLY) && p->found < 14){
                    while (c < count && p->found < 14){ // Store the PTS
					    p->pts[p->found-9] = buf[c];
					    c++;
					    p->found++;
				    }
                    RETURN_IF_NO_DATA;
				    if (p->apsv.length > (p->apsv.flags & 0x03)) {
				        ret=kwrite_video(&p->apsv,Next_Pes_Buffer);
				    }
				    pts2pts(p->apsv.pts,p->pts);
				    p->apsv.flags |= PTS_FLAG;
			    }

                while (c < count && p->found < 9+p->hlength){ // Skip the remaining header data
				    c++;
				    p->found++;
			    }
                RETURN_IF_NO_DATA;

            } else if (p->mpeg == 1 && p->which < 2000) { // Decode the mpeg 1 stream header data

                if (p->which == 0) { // Look for a non 0xFF byte
				    while (c < count && p->check == 0xFF) {
					    p->check = buf[c];
					    c++;
					    p->found++;
				    }
                    RETURN_IF_NO_DATA;
                }

				if (p->which == 0 && (p->check & 0xC0) == 0x40) {
					p->check = buf[c];
					c++;
					p->found++;
					p->which = 1;
                    RETURN_IF_NO_DATA;
                }

                if (p->which == 1) {
					p->check = buf[c];
					c++;
					p->found++;
					p->which = 2;
                    RETURN_IF_NO_DATA;
				}
			
				if ((p->check & 0x30) && p->check != 0xFF){
					p->flag2 = (p->check & 0xF0) << 2;
					p->pts[0] = p->check;
					p->which = 3;
				} 

                if (p->which >= 3) {
                    if (p->flag2 & PTS_ONLY) { // Record the PTS
					    while (c < count && p->which < 7){
						    p->pts[p->which-2] = buf[c];
						    c++;
						    p->found++;
						    p->which++;
					    }
                        RETURN_IF_NO_DATA;

                        if ((p->flag2 & PTS_DTS_FLAGS) == PTS_DTS) { // Skip over DTS
					        while (c < count && p->which < 12){
						        c++;
						        p->found++;
						        p->which++;
					        }
                            RETURN_IF_NO_DATA;
                        }

                        if (p->apsv.length > (p->apsv.flags & 0x03))
	    					ret=kwrite_video(&p->apsv,Next_Pes_Buffer);
		    			pts2pts(p->apsv.pts,p->pts);
			    		p->apsv.flags |= PTS_FLAG;
    			    	p->which = 2000;
                    }
				}
							
			}

			while (c < count && (unsigned int)p->found < p->plength+6) {
				max = MAX_AV - ((p->apsv.flags & PTS_FLAG)>>2) - p->apsv.length;
				l = count-c;
				if (p->plength && (unsigned int)l+p->found > p->plength+6)
					l = p->plength+6-p->found;
                if (l < max){ // We have only part of it
					memcpy(p->apsv.payload+p->apsv.length, buf+c, l);
					p->apsv.length += l;
					p->found += l;
					c += l;
                } else { // We have all data
					memcpy(p->apsv.payload+p->apsv.length, buf+c, max);
					p->apsv.length += max;
					p->found += max;
					c += max;
					ret=kwrite_video(&p->apsv,Next_Pes_Buffer);
					if ( ret > 0 ) {
    					*StartPos=c;
	    				return(ret);
					}
				}
			}					
				
        } else if (p->cid >= AUDIO_STREAM_S && p->cid <= AUDIO_STREAM_E) {

			if (p->mpeg == 2 && p->found == 9){
				memcpy(p->apsa.payload,headr,3);
				p->apsa.payload[3] = p->cid;
				memcpy(p->apsa.payload+4,p->plen,2);
				p->apsa.payload[6] = p->flag1;
				p->apsa.payload[7] = p->flag2;
				p->apsa.payload[8] = p->hlength;
				if ((p->flag2 & PTS_ONLY) && p->mpeg == 2)
					p->apsa.flags |= PTS_FLAG;
				p->apsa.length = 9;
			}

			if (p->mpeg == 1 && p->found == 7){
				memcpy(p->apsa.payload,headr,3);
				p->apsa.payload[3] = p->cid;
				memcpy(p->apsa.payload+4,p->plen,2);
				p->apsa.payload[6] = p->flag1;
				p->apsa.length = 7;
                RETURN_IF_NO_DATA;
			}

			while (c < count && (unsigned int)p->found < p->plength+6){
				max = MAX_AVA - p->apsa.length;
				l = count -c;
				if ((unsigned int)(l+p->found) > p->plength+6)
					l = p->plength+6-p->found;
				if (l < max){
					memcpy(p->apsa.payload+p->apsa.length,
					       buf+c,l);
					p->apsa.length += l;
					p->found += l;
					c += l;
					if ((unsigned int)p->found == p->plength+6) {
						ret=kwrite_audio(&p->apsa,Next_Pes_Buffer);
                        RETURN_IF_NO_DATA;
					}
				} else {
					memcpy(p->apsa.payload+p->apsa.length,
					       buf+c,max);
					p->apsa.length += max;
					p->found += max;
					c += max;
					ret=kwrite_audio(&p->apsa,Next_Pes_Buffer);
                    RETURN_IF_NO_DATA;
				}	
			}				
        } else if (( p->cid == PROG_STREAM_MAP ) ||
                   ( p->cid == PRIVATE_STREAM2 ) ||
                   ( p->cid == PROG_STREAM_DIR ) ||
                   ( p->cid == ECM_STREAM ) ||
                   ( p->cid == EMM_STREAM ) ||
                   ( p->cid == PADDING_STREAM ) ||
                   ( p->cid == DSM_CC_STREAM ) ||
                   ( p->cid == ISO13522_STREAM ) ||
                   ( p->cid == PRIVATE_STREAM1 )) {
			if ( (unsigned int)(p->found + count - c) < p->plength+6){
				p->found += count-c;
				c = count;
			} else {
				c += p->plength+6 - p->found;
				p->found = p->plength+6;
			}
		}


		if (p->plength && (unsigned int)p->found == p->plength+6) {
			p->found = 0;
			p->done = 0;
			p->plength = 0;

            RETURN_IF_NO_DATA;

			if (c < count) {
				ret=kpes_to_av_pes(buf,&c,count, p,Next_Pes_Buffer);
			}
		}

   }
    *StartPos=c;
	return(ret);
}


