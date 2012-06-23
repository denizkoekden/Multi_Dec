/********************************************************************************
*                                                                               *
* dvb.c: Hardware-Parts for the DVB PCI card und saa7146 device driver          *
*                                                                               *
* Copyright (C) 2000 Espresso                                                   *
*                                                                               *
*                                                                               *
*  originally based on code by:                                                 *
*                                                                               *
*  driver for the Siemens Fujitsu DVB PCI card                                  *
*  Copyright (C) 1998,1999 Christian Theiss <mistert@rz.fh-augsburg.de>         *
*  Copyright (C) 1999 Ralph Metzler for convergence integrated media GmbH       *
*                                                                               *
*  saa7146 device driver                                                        *
*  Copyright (C) 1998,1999 Michael Hunold <michael@mihu.de>                     *
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
#include <time.h>
#include "resource.h"
#include <stdlib.h>        /* atoi                                  */
#include <memory.h>         
#include <io.h>         
#include <fcntl.h>
#include <commctrl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../DrvLib/windrvr.h"
#include "../SAA7146.Dev/SAAIOcom.h"
#include "DVB.h"


void TDA_LNB_On( void );

extern void Init_Screen_Struct();
void DelMasterFilter(int FHandle);
BOOL APIENTRY RecordPlayInterrupt(HWND hDlg,UINT message,UINT wParam,LONG lParam);
BOOL APIENTRY SaaBootOptionProc(HWND hDlg,UINT message,UINT wParam,LONG lParam);
BOOL APIENTRY StreamFilterScanProc(HWND hDlg,UINT message,UINT wParam,LONG lParam);
BOOL APIENTRY ByteFilterScanProc(HWND hDlg,UINT message,UINT wParam,LONG lParam);
int Get_DVB_Card_Interrupt( void ); 
void down_interruptible( void );
void up_interruptible( void );

void Get_DVB_MacAdr( unsigned char *MAC ); 
void SetMode( int mode);

void SAA7146_KP_SET_INIT(HANDLE hWD, DWORD hKernelPlugIn);
void SAA7146_KP_SET_TIMEING(HANDLE hWD, DWORD hKernelPlugIn);
void SAA7146_KP_CLEAR_INIT(HANDLE hWD, DWORD hKernelPlugIn);
void SAA7146_KP_GET_IRQ(HANDLE hWD, DWORD hKernelPlugIn, SAA7146_ISR_IO *saaisr);
void Reset_Debi();
void Ir_Control( void );
void Start_Ir_Thread( void );
void Init_Crystal_Dac( void );
void I2C_Trace_Thread( void );
unsigned char Get_Sync_State( unsigned char *RealVal );
void Relink_Current_Eit(unsigned short SID,unsigned short ts_id,unsigned short To_SID,unsigned short To_ts_id);
int Find_Link_Programm(unsigned short tp_id, unsigned short sid );
void Insert_CAT(int Nr);
int Get_Virtual_Event(unsigned short EventId,unsigned short SID);
BOOL Is_Time_Link(unsigned short SID, unsigned short ts_id,BOOL Current);
int Get_MultiView_Count(unsigned short Index,unsigned short Offset);
int Get_MultiLink_Count(unsigned short v_event,unsigned short v_sid);
BOOL Find_Multi_Link(unsigned short v_event,unsigned short v_sid, int *index, int *offset );
BOOL Is_View_Link(unsigned short SID, unsigned short ts_id,BOOL Current);
BOOL Is_Pid_Param(unsigned short SID, unsigned short ts_id,BOOL Current);
BOOL Set_DVB_Channel_by_SID_TS_ID( unsigned short ts_id, unsigned short sid,char *RealName);
BOOL Set_Transponder(struct TTransponder *Transponder );
BOOL Get_PMT(int ProgrammNummer, unsigned short pid );
BOOL Update_Programm_List(int NeuWert );

int StartLogFilter( unsigned short Pid , int Irq_Proc , LPSTR Name);

void Make_Service_Name(unsigned char ServiceTyp, char *Text);
void Make_Crypt_Systems(int Nr, char *Text);
unsigned long ccitt32_checkcrc(const unsigned char *icp, const unsigned int icnt);
int Work_EIT_4F(unsigned char *Daten , int Laenge );
int Work_EIT_50(unsigned char *Daten , int Laenge );

unsigned short Swab16(unsigned char *dat);
DWORD cpu_to_be32(DWORD value);
void Insert_Pid(int ProgrammNummer,unsigned char stream_type,unsigned short pid,unsigned short info_length,char *ptr);
void SAA7146_WDebi(DWORD config, int addr, DWORD val, int count);
DWORD SAA7146_RDebi(DWORD config, int addr, DWORD val, int count);
void SAA7146_SetGPIO( int port, DWORD data);
int SAA7146_FirmVersion();
int SAA7146_RequestParameter(unsigned short tag, short *Buff, short length);
int SAA7146_OutCommand( unsigned short *buf, short length);
int SAA7146_OutCom(int type, int com, int num, ...);
int SAA7146_Wait_for_Debi_done();
int SAA7146_Wait_for_Debi_Upload_Enable();

int SAA7146_Load_Dram( unsigned char *data, int len);
int VES1820_Init( BOOL MiniMize );
void  VES1820_SetInversion(int inversion);
int VES1893_Init( BOOL MiniMize );
int STV0299_Init( BOOL MiniMize );
int TDA8083_Init( BOOL MiniMize );
int Get_Filter_by_Handle(unsigned short Handle );
int SAA7146_WaitDebi( int adr, int state);
int SAA7146_CommandRequest(unsigned short *Buff, int length, unsigned short *buf, int n);
int SendDAC( unsigned char addr, unsigned char data);
int clean_up(struct i2c_msg m[], int num);
int CreateOSDWindow( unsigned char windownr, unsigned short disptype, unsigned short width, unsigned short height);
int DestroyOSDWindow(unsigned char windownr);
int MoveWindowAbs( unsigned char windownr, unsigned short x, unsigned short y);
int SetColorBlend(unsigned char windownr);
void DrawBlock( unsigned char windownr,unsigned short x, unsigned short y, unsigned short dx, unsigned short dy, unsigned short color);
void SetFont(unsigned char  windownr, unsigned short  fontsize,unsigned short  colorfg, unsigned short  colorbg);
void WriteText(unsigned char win, unsigned short x, unsigned short y, unsigned char *buf);
void OSDSetColor(unsigned char win, unsigned short color, unsigned char  r, unsigned char  g, unsigned char  b, unsigned char  blend);
int AFC(struct frontend *front,int Nr);
int GetFront(struct frontend *front);
void DelBitFilter(int FilterNummer);
BOOL Get_Pat( void );
BOOL Get_Nit( unsigned char diseq, BOOL load_ACT);
BOOL Get_SDT( void );
int Get_New_Prog_by_SID(unsigned short SID);
int Get_New_Prog(void);
int Get_New_Prog_by_PMT(unsigned short PMT);
int parse_sdt_descr(unsigned char *ptr, u_int len, int ProgrammNummer);
int parse_nit_descr (unsigned char *ptr, int NitNummer);
void Parse_PMT_Infos( unsigned char *BasePtr, int info_len, int ProgrammNummer, unsigned char stream_type, unsigned short pid, int stream_anzahl);
void DebiThread(void); 
void CI_handle(unsigned char *data,int len); 
int parse_eit( unsigned char *ptr,struct FIX_EIT_t *tmpEit );
void Get_Signal_Infos(unsigned char *Sync , unsigned int *Quality,unsigned int *BitErrorRate );

void Insert_WinRoll_Window(unsigned char WindowNr,int MaxY ,struct WinRoll_t *Roll,int MaxRoll);
int FindActiveAudioName(int Nummer, unsigned char *Name);
void GetAudioName(int Nummer,int FilterId, unsigned char *Name);
void Set_DVB_Channel(int NUmmer);
void Set_DVB_Channel_Tom(int NUmmer);
void Set_Transponder_Pol(struct TTransponder *Transponder);

void ShutDownFilters( void );


BOOL Set_Transponder_SRate(struct TTransponder *Transponder , unsigned int SRate);

void GetCurrentNext(unsigned short sid_id,unsigned short ts_id,int typ,unsigned char *Titel,unsigned char *StartZeit);
int	MakeRealValue(LPSTR Buffer);
int writereg(unsigned char addr, unsigned char flags,int reg, int data);
unsigned char readreg(unsigned char addr, unsigned char flags,unsigned char reg);
int master_xfer(struct i2c_msg msgs[], int num);
int i2c_master_send(unsigned char addr, unsigned char flags,const char *buf ,int count);
void SAA7146_SetColourFormat(int nColourFormat);
void Set_TV_Freq( unsigned __int64 freq,int LNBNr);
void ARM_ClearMailBox();
void ARM_SET_IRQ_STATE_EXT(unsigned short value);
void ARM_SET_SEND(unsigned short value);
void ARM_SET_RX_BUFF(unsigned short value);
void ARM_CA_SEND(unsigned int value);
void ARM_ClearIrq();
void ARM_PLAY_BUFFER(unsigned int value);
void ARM_LOAD_BITMAP(unsigned short value,DWORD Addr);
void ARM_CLEAR_RECEIVE( void );

void Debi_Interrupt(SAA7146_ISR_IO *saaisr);
void ARM_ResetMailBox();


void video_setmode( int v);
void gpioirq();
void DVB_SET_FRONTEND(struct frontend *front);
BOOL Alloc_DVB_Display_DMA(DWORD dwSize);
int SAA7146_FirmRead(LPSTR FileName, unsigned char *Daten, int Bytes);

int Find_Programm(unsigned short tp_id, unsigned short sid );
int Find_ProgrammNeu(unsigned short tp_id, unsigned short sid );
int SetFilter(unsigned short pid, unsigned short section, unsigned short mode, LPSTR Name); 

int Save_EIT(struct FIX_EIT_t *tmpEit, BOOL Update_Current_Next);
BOOL Find_Current_EIT(unsigned short sid_id,unsigned short ts_id,int *Pos, int *Offset);

void Start_Eit_Filters( void );
void Stop_Eit_Filters( void );
void Start_TID_Filters( void );
void Stop_TID_Filters( void );
BOOL UseDiseqC(void);
void Run_DVB_Irq(BOOL State, BOOL Keep_Run_IR);
void ShutDownFiltersByName( LPSTR Name );

extern void SetMenu_CI();
extern void Write_Log(const char *Text);

void CI_Reset(void);
void CI_Run(int Nummer);

void Get_CI_Status( void );
void Make_Crypt_Name(unsigned char CryptId, char *Text);

void ClrBit1893( void );
void ClrBit8083( void );
void ClrBit1820( void );
void ClrBit0299( void );

unsigned short RealTime(unsigned char Stunde);
int Get_Sender(unsigned short SID_Id,unsigned short ts_id,unsigned char *Name);


void Set22K(int state);
void SetDiSEqC( int DiseqCNr, int Band, int Volt);
void VidMode(int mode);

void DVB_Reset( void );
void DVB_SetVolume(unsigned char volleft, unsigned char volright);
int tuner_getstatus(void);

int msp_writereg( unsigned char dev, unsigned short reg, unsigned short val);
int i2c_writereg(unsigned char id, unsigned char reg, unsigned char val);

extern void Irdeto_Gen_Log(unsigned short EMM_Pid, struct tIrdeto *Irdeto_Ptr );
extern void Seca_Gen_Log(struct tSECA *Seca_Ptr );

long RealDay(unsigned char Stunde, long Day);
extern void Unload_External_Dll(int Nummer);

BOOL Get_CAT( void );

void  Debi_Record_FastFF( void );
void  Debi_Record_SlowFF( void );
void  Debi_Record_Play_Normal( void );
void OSD_Play_Stop( void );
void Create_Osd_Thread( LPTHREAD_START_ROUTINE ThreadProzess);


void SAA7146_Restart_Arm( void );

void Play_Stop(void);
BOOL Debi_Record_Start(int RecordFormat, int RecordTyp );
BOOL Debi_Record_Play( void );
void Debi_Record_Stop( void );
void Debi_Record_Pause( void );
BOOL Next_Record_File();
BOOL Next_Play_File( );
BOOL Jump_Time_Play(unsigned int Seconds);

void Date_To_String( unsigned short Date_Id,char *DatumsString);
void DecodeDVBDate(unsigned short DVBDate, unsigned short *year,unsigned short *month,unsigned short *day);

void Write_CI_Log(LPSTR Text);
int OSD_Edit_Hex( int startx , int starty, int len, int *current, unsigned char *Buffer);
int OSD_Edit_Dez( int startx , int starty, int len, int *current, unsigned char *Buffer);
int OSD_Edit_Asc( int startx , int starty, int len, int *current, unsigned char *Buffer);
int OSD_Edit_Ascii_Mask( unsigned char windowNr, int startx , int starty, int len, int *current, unsigned char *Buffer, unsigned short fg, unsigned short bg);
int OSD_Edit_Ascii( unsigned char windowNr, int startx , int starty, int len, int *current, unsigned char *Buffer, unsigned short fg,unsigned short bg);
int DrawLine( unsigned char windownr,unsigned short x, unsigned short y, unsigned short dx, unsigned short dy, unsigned short color);
unsigned int RGB2YUV(unsigned short R, unsigned short G, unsigned short B);
int SetBlend_(unsigned char windownr,unsigned short colordepth, unsigned short index, unsigned char blending);
int SetColor_(unsigned char windownr, unsigned short colordepth, unsigned short index, unsigned short colorhi, unsigned short colorlo);
int OSDLoadBitmap4(unsigned char windownr ,BITMAPINFO *BitMap, int x,int y,int TR,int TG,int TB,int TVal ,int TimeToWait);
void Stop_Osd_Key_Event( void );
void Osd_Thread_Down( void );
void OSD_Volume( void );
void OSD_Color_Setting_Proc( void );
void OSD_ChannelChange( void );

extern void Work_Text(unsigned char *TmpBuffer);
extern BOOL Tuner_SetFrequency(int TunerTyp, int wFrequency);
extern void Save_Mode(void );
extern void Restore_Mode(void );
extern BOOL ValidModes(char Mode);
extern void Set_Mode_Digital( void );
extern void Set_Mode_Analog( void );
extern BOOL UnloadDeviceDriver( const TCHAR * Name,BOOL DRemove);
extern BOOL LoadDeviceDriver( const TCHAR * Name, const TCHAR * Path, HANDLE * lphDevice,BOOL Install);
extern void Write_Programm_List();
extern BOOL APIENTRY MDBootOptionProc(HWND hDlg,UINT message,UINT wParam,LONG lParam);
extern void Set_Capture(int nState);
extern int Make_ProgNr( int Nr );
extern BOOL IsValidSelect(int Nr );
extern int Get_Real_ProgNr( int Nr );
extern void VT_ChannelChange();
extern BOOL Is_Sat_Karte(void );
extern BOOL Is_Kabel_Karte(void );
extern void Irq_Ir_Control( unsigned int IR_Key );

// BT8x8.c
extern void Init_Winbis(void);
extern void winbis_decoder(unsigned char *dat);

// dvr.c
extern void OSD_Recorder_Info(void );
extern int Irq_Record(unsigned char *Daten , int Laenge );
extern int Irq_Play( int Typ );



int Irq_Filters(DWORD type ,int laenge);
void Irq_Call_TDT( int Laenge , unsigned char *Daten );
void Irq_Call_Log(int Laenge , unsigned char *Daten);
void Irq_Call_VT_Stream(int Laenge , unsigned char *Daten);
int  Start_VT_Stream( unsigned short TextPid );
void Stop_VT_Stream( void );
int Irq_Load_BMP( int Laenge );
void Irq_Call_VT_Stream(int Laenge , unsigned char *Daten);
void Stop_VT_Stream( void );
void Irq_Seca_Stream(int length, unsigned char *Data);
void Irq_Irdeto_Stream(unsigned short EMM_Pid, int length, unsigned char *Data);

BOOL APIENTRY DVBTestProc(HWND hDlg,UINT message,UINT wParam,LONG lParam);

void load_alternate_osd_font(unsigned char *DRammem, int DRamLen);

/* Default charmap for the font stored in the firmware provided by Convergence */

unsigned char iso88591_to_osdfont[256] = {
   /* 00 */ 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
   /* 08 */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
   /* 10 */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
   /* 18 */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
   /* 20 */ 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
   /* 28 */ 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
   /* 30 */ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
   /* 38 */ 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
   /* 40 */ 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
   /* 48 */ 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
   /* 50 */ 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
   /* 58 */ 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
   /* 60 */ 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
   /* 68 */ 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
   /* 70 */ 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
   /* 78 */ 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
   /* 80 */ 0x45, 0x7f, 0x2c, 0x66, 0x2c, 0x5f, 0x2b, 0x7f,
   /* 88 */ 0x5e, 0x7f, 0x53, 0x3c, 0x45, 0x7f, 0x5a, 0x7f,
   /* 90 */ 0x7f, 0x27, 0x27, 0x22, 0x22, 0x2e, 0x2d, 0x2d,
   /* 98 */ 0x7e, 0x7f, 0x73, 0x3e, 0x65, 0x7f, 0x7a, 0x59,
   /* a0 */ 0x20, 0x21, 0x63, 0x7f, 0x6f, 0x59, 0x7c, 0x7f,
   /* a8 */ 0x22, 0x63, 0x61, 0x3c, 0x7f, 0x18, 0x72, 0x7f,
   /* b0 */ 0x7f, 0x7f, 0x32, 0x33, 0x27, 0x75, 0x7f, 0x2e,
   /* b8 */ 0x7f, 0x31, 0x30, 0x3e, 0x7f, 0x7f, 0x7f, 0x7f,
   /* c0 */ 0x41, 0x41, 0x41, 0x41, 0x1a, 0x41, 0x41, 0x43,
   /* c8 */ 0x45, 0x45, 0x45, 0x45, 0x49, 0x49, 0x49, 0x49,
   /* d0 */ 0x44, 0x4e, 0x4f, 0x4f, 0x4f, 0x4f, 0x1c, 0x78,
   /* d8 */ 0x4f, 0x55, 0x55, 0x55, 0x1e, 0x59, 0x7f, 0x19,
   /* e0 */ 0x61, 0x61, 0x61, 0x61, 0x1b, 0x61, 0x61, 0x63,
   /* e8 */ 0x65, 0x65, 0x65, 0x65, 0x69, 0x69, 0x69, 0x69,
   /* f0 */ 0x7f, 0x6e, 0x6f, 0x6f, 0x6f, 0x6f, 0x1d, 0x2f,
   /* f8 */ 0x6f, 0x75, 0x75, 0x75, 0x1f, 0x79, 0x7f, 0x79,
};

#define SAA7146_BootCodeLen sizeof(SAA7146_BootCode)

static const unsigned char SAA7146_BootCode[] = {
/* 0x00 */ 0xea, 0x00, 0x00, 0x0e, 0xe1, 0xb0, 0xf0, 0x0e,
/* 0x08 */ 0xe2, 0x5e, 0xf0, 0x04, 0xe2, 0x5e, 0xf0, 0x04,
/* 0x10 */ 0xe2, 0x5e, 0xf0, 0x08, 0xe2, 0x5e, 0xf0, 0x04,
/* 0x18 */ 0xe2, 0x5e, 0xf0, 0x04, 0xe2, 0x5e, 0xf0, 0x04,
/* 0x20 */ 0x2c, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x0c,
/* 0x28 */ 0x00, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x34,
/* 0x30 */ 0x00, 0x00, 0x00, 0x00, 0xa5, 0xa5, 0x5a, 0x5a,
/* 0x38 */ 0x00, 0x1f, 0x15, 0x55, 0x00, 0x00, 0x00, 0x09,
/* 0x40 */ 0xe5, 0x9f, 0xd0, 0x5c, 0xe5, 0x9f, 0x40, 0x54,
/* 0x48 */ 0xe3, 0xa0, 0x00, 0x00, 0xe5, 0x84, 0x00, 0x00,
/* 0x50 */ 0xe5, 0x84, 0x00, 0x04, 0xe1, 0xd4, 0x10, 0xb0,
/* 0x58 */ 0xe3, 0x51, 0x00, 0x00, 0x0a, 0xff, 0xff, 0xfc,
/* 0x60 */ 0xe1, 0xa0, 0x10, 0x0d, 0xe5, 0x94, 0x30, 0x04,
/* 0x68 */ 0xe1, 0xd4, 0x20, 0xb2, 0xe2, 0x82, 0x20, 0x3f,
/* 0x70 */ 0xe1, 0xb0, 0x23, 0x22, 0x03, 0xa0, 0x00, 0x02,
/* 0x78 */ 0xe1, 0xc4, 0x00, 0xb0, 0x0a, 0xff, 0xff, 0xf4,
/* 0x80 */ 0xe8, 0xb1, 0x1f, 0xe0, 0xe8, 0xa3, 0x1f, 0xe0,
/* 0x88 */ 0xe8, 0xb1, 0x1f, 0xe0, 0xe8, 0xa3, 0x1f, 0xe0,
/* 0x90 */ 0xe2, 0x52, 0x20, 0x01, 0x1a, 0xff, 0xff, 0xf9,
/* 0x98 */ 0xe2, 0x2d, 0xdb, 0x05, 0xea, 0xff, 0xff, 0xec,
/* 0xA0 */ 0x2c, 0x00, 0x03, 0xf8, 0x2c, 0x00, 0x04, 0x00,
};

BOOL Init_DVB_Karte(HWND hWnd)
{
	FILE* SettingFile;
	
	char VersionString[255];
	char Path[255];
	char DumpLine[255];
	int ret;
	unsigned int i,j;
	unsigned char b;
	
	
	
    strcpy(DVBVendorID,"0x1131");
	strcpy(DVBDeviceID,"0x7146");
	
    ret=SAA7146_Open (&hSAA7146, 0x1131, 0x7146,  TRUE, FALSE );
	
    if ( ret == 0 ) {
		strcpy(DVBTyp,"SAA7146");
		strcpy(VersionString,t("DVB mit SAA7147 gefunden"));
	} else if ( ret == 1 ) {
		MessageBox(hWnd,t("Windows NT/95/98 Device-Treiber MDHALL kann nicht angesprochen werden"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
		if ( NT == TRUE ) {
			UnloadDeviceDriver("MultiDec HAL-Treiber",TRUE);
			if ( OrgDVBDriverName[0] != 0x00 ) LoadDeviceDriver( (const char *)OrgDVBDriverName, Path, &Device_Handle,FALSE  );
		}
		return(FALSE);
	} else if ( ret == 3 ) {
		MessageBox(hWnd,t("PCI-Karte mit SAA7147 kann nicht gelockt werden !\nDer Interrupt wird mehrfach verwendet!\n\nKein Zugriff auf die Karte möglich"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
		if ( NT == TRUE ) {
			UnloadDeviceDriver("MultiDec HAL-Treiber",TRUE);
			if ( OrgDVBDriverName[0] != 0x00 ) LoadDeviceDriver( (const char *)OrgDVBDriverName, Path, &Device_Handle,FALSE  );
		}
		return(FALSE);
	} 
	
	
	if ( ret != 0 ) {
		strcpy(VersionString,t("Keine SAA7146"));
		SetDlgItemText(SplashWnd,IDC_TEXT2,VersionString);
		Sleep(2000);
		return(FALSE);
	}						
	
    if ( SaveSettings == TRUE ) {
        if ((SettingFile =fopen("Setting.SAA","w")) != NULL )  {
			
			
			fprintf(SettingFile,"SAA7146_IER              0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, IER));
			fprintf(SettingFile,"SAA7146_ISR              0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, ISR));
			fprintf(SettingFile,"SAA7146_MC1              0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, MC1));
			fprintf(SettingFile,"SAA7146_MC2              0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, MC2));
			fprintf(SettingFile,"SAA7146_PSR              0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PSR));
			fprintf(SettingFile,"SAA7146_SSR              0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, SSR));
			
			
			fprintf(SettingFile,"SAA7146_DD1_INIT         0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, DD1_INIT));
			fprintf(SettingFile,"SAA7146_DD1_STREAM_B     0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, DD1_STREAM_B));
			fprintf(SettingFile,"SAA7146_DD1_STREAM_A     0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, DD1_STREAM_A));
			fprintf(SettingFile,"SAA7146_BRS_CTRL         0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BRS_CTRL));
			fprintf(SettingFile,"SAA7146_HPS_CTRL         0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, HPS_CTRL));
			fprintf(SettingFile,"SAA7146_HPS_V_SCALE      0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, HPS_V_SCALE));
			fprintf(SettingFile,"SAA7146_HPS_V_GAIN       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, HPS_V_GAIN));
			fprintf(SettingFile,"SAA7146_HPS_H_PRESCALE   0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, HPS_H_PRESCALE));
			fprintf(SettingFile,"SAA7146_HPS_H_SCALE      0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, HPS_H_SCALE));
			fprintf(SettingFile,"SAA7146_BCS_CTRL         0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BCS_CTRL));
			fprintf(SettingFile,"SAA7146_CHROMA_KEY_RANGE 0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, CHROMA_KEY_RANGE));
			fprintf(SettingFile,"SAA7146_CLIP_FORMAT_CTRL 0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, CLIP_FORMAT_CTRL));
			fprintf(SettingFile,"SAA7146_DEBI_CONFIG      0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, DEBI_CONFIG));
			fprintf(SettingFile,"SAA7146_DEBI_COMMAND     0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, DEBI_COMMAND));
			fprintf(SettingFile,"SAA7146_DEBI_PAGE        0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, DEBI_PAGE));
			fprintf(SettingFile,"SAA7146_DEBI_AD          0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD));
			fprintf(SettingFile,"SAA7146_I2C_TRANSFER     0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, I2C_TRANSFER));
			fprintf(SettingFile,"SAA7146_I2C_STATUS       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, I2C_STATUS));
			fprintf(SettingFile,"SAA7146_RPS_PAGE0        0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, RPS_PAGE0));
			fprintf(SettingFile,"SAA7146_RPS_PAGE1        0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, RPS_PAGE1));
			fprintf(SettingFile,"SAA7146_RPS_THRESH0      0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, RPS_THRESH0));
			fprintf(SettingFile,"SAA7146_RPS_THRESH1      0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, RPS_THRESH1));
			fprintf(SettingFile,"SAA7146_RPS_TOV0         0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, RPS_TOV0));
			fprintf(SettingFile,"SAA7146_RPS_TOV1         0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, RPS_TOV1));
			fprintf(SettingFile,"SAA7146_GPIO_CTRL        0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, GPIO_CTRL));
			fprintf(SettingFile,"SAA7146_EC1SSR           0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, EC1SSR));
			fprintf(SettingFile,"SAA7146_EC2SSR           0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, EC2SSR));
			fprintf(SettingFile,"SAA7146_ECT1R            0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, ECT1R));
			fprintf(SettingFile,"SAA7146_ECT2R            0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, ECT2R));
			fprintf(SettingFile,"SAA7146_ACON1            0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, ACON1));
			fprintf(SettingFile,"SAA7146_ACON2            0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, ACON2));
			fprintf(SettingFile,"SAA7146_EC1R             0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, EC1R));
			fprintf(SettingFile,"SAA7146_EC2R             0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, EC2R));
			fprintf(SettingFile,"SAA7146_PCI_BT_V1        0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PCI_BT_V1));
			fprintf(SettingFile,"SAA7146_PCI_BT_V2        0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PCI_BT_V2));
			fprintf(SettingFile,"SAA7146_PCI_BT_V3        0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PCI_BT_V3));
			fprintf(SettingFile,"SAA7146_PCI_BT_DEBI      0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PCI_BT_DEBI));
			fprintf(SettingFile,"SAA7146_PCI_BT_A         0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PCI_BT_A));
			fprintf(SettingFile,"SAA7146_PCI_VDP1         0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PCI_VDP1));
			fprintf(SettingFile,"SAA7146_PCI_VDP2         0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PCI_VDP2));
			fprintf(SettingFile,"SAA7146_PCI_VDP3         0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PCI_VDP3));
			fprintf(SettingFile,"SAA7146_PCI_ADP1         0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PCI_ADP1));
			fprintf(SettingFile,"SAA7146_PCI_ADP2         0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PCI_ADP2));
			fprintf(SettingFile,"SAA7146_PCI_ADP3         0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PCI_ADP3));
			fprintf(SettingFile,"SAA7146_PCI_ADP4         0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PCI_ADP4));
			fprintf(SettingFile,"SAA7146_PCI_DMA_DDP      0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PCI_DMA_DDP));
			fprintf(SettingFile,"**************  Video DMA 1  **************\n");
			fprintf(SettingFile,"SAA7146_BASE_ODD1        0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BASE_ODD1));
			fprintf(SettingFile,"SAA7146_BASE_EVEN1       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BASE_EVEN1));
			fprintf(SettingFile,"SAA7146_PROT_ADDR1       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PROT_ADDR1));
			fprintf(SettingFile,"SAA7146_PITCH1           0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PITCH1));
			fprintf(SettingFile,"SAA7146_BASE_PAGE1       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BASE_PAGE1));
			fprintf(SettingFile,"SAA7146_NUM_LINE_BYTE1   0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, NUM_LINE_BYTE1));
			fprintf(SettingFile,"**************  Video DMA 2  **************\n");
			fprintf(SettingFile,"SAA7146_BASE_ODD2        0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BASE_ODD2));
			fprintf(SettingFile,"SAA7146_BASE_EVEN2       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BASE_EVEN2));
			fprintf(SettingFile,"SAA7146_PROT_ADDR2       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PROT_ADDR2));
			fprintf(SettingFile,"SAA7146_PITCH2           0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PITCH2));
			fprintf(SettingFile,"SAA7146_BASE_PAGE2       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BASE_PAGE2));
			fprintf(SettingFile,"SAA7146_NUM_LINE_BYTE2   0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, NUM_LINE_BYTE2));
			fprintf(SettingFile,"**************  Video DMA 3  **************\n");
			fprintf(SettingFile,"SAA7146_BASE_ODD3        0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BASE_ODD3));
			fprintf(SettingFile,"SAA7146_BASE_EVEN3       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BASE_EVEN3));
			fprintf(SettingFile,"SAA7146_PROT_ADDR3       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PROT_ADDR3));
			fprintf(SettingFile,"SAA7146_PITCH3           0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PITCH3));
			fprintf(SettingFile,"SAA7146_BASE_PAGE3       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BASE_PAGE3));
			fprintf(SettingFile,"SAA7146_NUM_LINE_BYTE3   0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, NUM_LINE_BYTE3));
			fprintf(SettingFile,"**************  Audio DMA 1  **************\n");
			fprintf(SettingFile,"SAA7146_BASE_A1_IN       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BASE_A1_IN));
			fprintf(SettingFile,"SAA7146_PROT_A1_IN       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PROT_A1_IN));
			fprintf(SettingFile,"SAA7146_PAGE_A1_IN       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PAGE_A1_IN));
			fprintf(SettingFile,"SAA7146_BASE_A1_OUT      0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BASE_A1_OUT));
			fprintf(SettingFile,"SAA7146_PROT_A1_OUT      0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PROT_A1_OUT));
			fprintf(SettingFile,"SAA7146_PAGE_A1_OUT      0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PAGE_A1_OUT));
			fprintf(SettingFile,"**************  Audio DMA 2  **************\n");
			fprintf(SettingFile,"SAA7146_BASE_A2_IN       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BASE_A2_IN));
			fprintf(SettingFile,"SAA7146_PROT_A2_IN       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PROT_A2_IN));
			fprintf(SettingFile,"SAA7146_PAGE_A2_IN       0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PAGE_A2_IN));
			fprintf(SettingFile,"SAA7146_BASE_A2_OUT      0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, BASE_A2_OUT));
			fprintf(SettingFile,"SAA7146_PROT_A2_OUT      0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PROT_A2_OUT));
			fprintf(SettingFile,"SAA7146_PAGE_A2_OUT      0x%08x\n",SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PAGE_A2_OUT));
            fprintf(SettingFile,"\n");
			fprintf(SettingFile,"**************  A_TIME_SLOT1 Dump **************\n");
            i=0x180;
			j=0;
			while ( i <= 0x1bc ) {
				if ( j == 0 ) {
					fprintf(SettingFile,"0x%04x : ",i);
					strcpy(DumpLine,". . . . . . . . . . . . . . . . "); 
				};
				
				b=SAA7146_ReadByte(hSAA7146, SAA7146_AD_BAR0, i);
				if (( b >= 0x20 ) && ( b < 128 )) DumpLine[j*2]=b; 
				fprintf(SettingFile,"0x%02x ",b);
				j++;
				
				if ( j > 15 ) {
					fprintf(SettingFile,"%s\n",DumpLine);
					j=0;
				};
				i++;
			};
			
			
			if ( j > 0 ) {
				while ( j < 16 ) {
					fprintf(SettingFile,"     ");
					j++;
				}
				fprintf(SettingFile,"%s\n",DumpLine);
			};
            fprintf(SettingFile,"\n");
			fprintf(SettingFile,"**************  A_TIME_SLOT2 Dump **************\n");
            i=0x1C0;
			j=0;
			while ( i <= 0x1FC ) {
				if ( j == 0 ) {
					fprintf(SettingFile,"0x%04x : ",i);
					strcpy(DumpLine,". . . . . . . . . . . . . . . . "); 
				};
				b=SAA7146_ReadByte(hSAA7146, SAA7146_AD_BAR0, i);
				if (( b >= 0x20 ) && ( b < 128 )) DumpLine[j*2]=b; 
				fprintf(SettingFile,"0x%02x ",b);
				j++;
				
				if ( j > 15 ) {
					fprintf(SettingFile,"%s\n",DumpLine);
					j=0;
				};
				i++;
			};
			
			
			if ( j > 0 ) {
				while ( j < 16 ) {
					fprintf(SettingFile,"     ");
					j++;
				}
				fprintf(SettingFile,"%s\n",DumpLine);
			};
            fprintf(SettingFile,"\n");
			fclose(SettingFile);
		}
	}
	
	
	Has_DVB=TRUE;
	
	i=Get_DVB_Card_Interrupt();
	if (( i >= 0 ) && ( i < 256 )) {
		
		if ( Used_IRQ_List[i] == 1 ) {
			sprintf(Path,t("DVB-Karte benutzt IRQ 0x%02X <Kein IRQ-Sharing>"),i);
			Write_Log(Path);
		} else {
			sprintf(Path,"***************************************************");
			Write_Log(Path);
			sprintf(Path,"*                                                 *");
			Write_Log(Path);
			sprintf(Path,"*        W I C H T I G E R   H I N W E I S        *");
			Write_Log(Path);
			sprintf(Path,"*   DVB Karte mit IRQ 0x%02X läuft im IRQ-Sharing   *",i);
			Write_Log(Path);
			sprintf(Path,"* Der IRQ wird insgesamt %2d mal im System genutzt *",Used_IRQ_List[i]);
			Write_Log(Path);
			sprintf(Path,"*           Das kann Problematisch werden         *");
			Write_Log(Path);
			sprintf(Path,"*                                                 *");
			Write_Log(Path);
			sprintf(Path,"***************************************************");
			Write_Log(Path);
			if ( DVB_IRQ_SHARING == FALSE ) {
				MessageBox(hWnd,t("IRQ-Sharing der DVB-Karte erkannt !!\nDas kann Problematisch werden\n(muß aber nicht, obwohl auf dem Karton steht : Ein freier Interrupt !\n\nDer Treiber wird informiert alle IRQ´s zu beantworten\n\nDiese Meldung wird nicht mehr angezeigt bis der Schalter DVB_IRQ_SHARING auf FALSE in der MultiDec.ini gesetzt wird"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK); 
				DVB_IRQ_SHARING=TRUE;
			};
			
		}
	};
	
	
	return(TRUE);
};

int Get_DVB_Card_Interrupt( void ) 
{
    int i=0;
	while ( (unsigned)i < hSAA7146->cardReg.Card.dwItems ) {
		if ( hSAA7146->cardReg.Card.Item[i].item == 1 ) {
			return(hSAA7146->cardReg.Card.Item[i].I.Int.dwInterrupt);
		};
		i++;
	}
	return(-1);
};


BOOL Alloc_SAA7146_DMA(DWORD dwSize, WD_DMA *dma,int Option )
{
    BZERO(*dma);
	if ( Option == DMA_KERNEL_BUFFER_ALLOC ) dma->pUserAddr=NULL;
	else dma->pUserAddr=malloc(dwSize);
	dma->dwBytes=dwSize;
	
	dma->dwOptions=Option;
    WD_DMALock(hSAA7146->hWD,dma);
	if ( dma->hDma == 0 ) {
		if ( dma->dwOptions != DMA_KERNEL_BUFFER_ALLOC)  free(dma->pUserAddr);
		dma->pUserAddr=NULL;
		return(FALSE);
	}
	return TRUE;
}

void Free_SAA7146_DMA( WD_DMA *dma )
{
	LPVOID *MemPtr=NULL;
	if ( dma == NULL ) return;
	if ( dma->hDma != 0 ) {
		if ( dma->dwOptions != DMA_KERNEL_BUFFER_ALLOC)  MemPtr=dma->pUserAddr;
		WD_DMAUnlock(hSAA7146->hWD,dma);
		if ( MemPtr != NULL ) free(MemPtr);
		dma->pUserAddr=NULL;
		
	}
}



BOOL Init_DVB_Memory(void)
{
	
	if(!Alloc_DVB_Display_DMA(933888))  
	{
		MessageBox(hWnd,t("DVB-Display Memory (Physikalisch Linear) nicht verfügbar"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	
	
	if(!Alloc_SAA7146_DMA(DEBI_MEM_SIZE,&debi_mem_dma,DMA_KERNEL_BUFFER_ALLOC))
	{
		MessageBox(hWnd,t("Debi-Memory kann für SAA-Zugriff nicht gelockt werden"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	
	if(!Alloc_SAA7146_DMA(RECORD_PLAY_MEM,&Record_mem_dma,DMA_KERNEL_BUFFER_ALLOC))
	{
		MessageBox(hWnd,t("Record/Play-Memory kann für SAA-Zugriff nicht gelockt werden"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	if(!Alloc_SAA7146_DMA(DEBI_MEM_SIZE,&CA_mem_dma,DMA_KERNEL_BUFFER_ALLOC))
	{
		MessageBox(hWnd,t("VT-Memory kann für SAA-Zugriff nicht gelockt werden"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	
	if(!Alloc_SAA7146_DMA(DEBI_MEM_SIZE,&CI_mem_dma,DMA_KERNEL_BUFFER_ALLOC))
	{
		MessageBox(hWnd,t("CI-Memory kann für SAA-Zugriff nicht gelockt werden"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	if(!Alloc_SAA7146_DMA(DEBI_MEM_SIZE,&IPMPE_mem_dma,DMA_KERNEL_BUFFER_ALLOC))
	{
		MessageBox(hWnd,t("IPMPE-Memory kann für SAA-Zugriff nicht gelockt werden"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	
	if(!Alloc_SAA7146_DMA(32768,&BitMap_mem_dma,DMA_KERNEL_BUFFER_ALLOC))
	{
		MessageBox(hWnd,t("BitMap-Memory kann für SAA-Zugriff nicht gelockt werden"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	
	if(!Alloc_SAA7146_DMA(DEBI_MEM_SIZE,&Ir_mem_dma,DMA_KERNEL_BUFFER_ALLOC))
	{
		MessageBox(hWnd,t("IR-Memory kann für SAA-Zugriff nicht gelockt werden"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	
	if(!Alloc_SAA7146_DMA(DEBI_MEM_SIZE,&Debug_mem_dma,DMA_KERNEL_BUFFER_ALLOC))
	{
		MessageBox(hWnd,t("Debug-Memory kann für SAA-Zugriff nicht gelockt werden"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	
	if(!Alloc_SAA7146_DMA(RPS_MEM_SIZE,&rps0_mem_dma,DMA_KERNEL_BUFFER_ALLOC))
	{
		MessageBox(hWnd,t("RPS Memory kann für SAA-Zugriff nicht gelockt werden"),t("Multi-Decoder"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	
	
	return(TRUE);
}


BOOL Alloc_DVB_Display_DMA(DWORD dwSize)
{
   	DVB_Display_dma.pUserAddr=NULL;
	DVB_Display_dma.dwBytes=dwSize;
    DVB_Display_dma.dwOptions=DMA_KERNEL_BUFFER_ALLOC;
    WD_DMALock(hSAA7146->hWD,&DVB_Display_dma);
	if ( DVB_Display_dma.hDma == 0 ) {
		DVB_Display_dma.pUserAddr=NULL;
		return(FALSE);
	}
	
	return TRUE;
}

void Free_DVB_Display_DMA( void )
{
	LPVOID *MemPtr=NULL;
	
	if ( DVB_Display_dma.pUserAddr == NULL ) return;
	if ( DVB_Display_dma.hDma != 0 ) {
		WD_DMAUnlock(hSAA7146->hWD,&DVB_Display_dma);
		DVB_Display_dma.pUserAddr=NULL;
	}
	return ;
}


BOOL Init_DVB_HardWare(void)
{
	int i,j;
	
	unsigned char Buffer[4];
	BOOL Skip_VES = FALSE;
	
	unsigned char *DRammem;
	int DRamLen=0;
	
	unsigned char *DPRammem;
	int DPRamLen=0;
	FILE* SaveBootFile;
	
	char FileName[256];
	char Text[256];
	
    char Zeile[256];
    int KeepRunning;
	
    if (EIT == NULL) {
        Current_Eit_Index_Size = EIT_INDEX_SIZE;
        EIT = (struct TEIT_Index *) malloc(Current_Eit_Index_Size*sizeof(struct TEIT_Index));
        if (EIT == NULL) {
            MessageBox(hWnd, "Can't allocate memory for the EIT informations", "Multi-Dec", MB_ICONSTOP|MB_OK);
            Current_Eit_Index_Size = 0;
        }
    }

	for (i=0;i < Current_Eit_Index_Size; i++ ) {
		EIT[i].Anzahl=0;
		EIT[i].EIT_Infos=NULL;
		
	};
	
	
	StreamLog.Run=FALSE;
	StreamLog.FilterIndex=-1;
	
    if (Filter == NULL) {
        Filter = (struct TFilter *) malloc(MAXFILTER*sizeof(struct TFilter));
        if (Filter == NULL) {
            MessageBox(hWnd, "Can't allocate memory for the Filter informations", "Multi-Dec", MB_ICONSTOP|MB_OK);
            return FALSE;
        }
    }

    for ( i = 0; i < MAXFILTER; i++ ) {
		Filter[i].pid=0xffff;
		FilterEvent[i]=NULL;
		
	};
	
	
	Write_Log("SAA7146 Boote SAA7146");
	
	
	if (!SAA7146_IntEnable(hSAA7146, NULL)) {
		Write_Log("SAA7146 Interrupt SAA7146 nicht Enabled");
		return FALSE;
	};
	Write_Log("SAA7146 Interrupt Enabled");
	
	Write_Log("SAA7146 Karte Booten");
	
	
	
	/* enable i2c-port pins */
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0,MC1, (MASK_08 | MASK_24));
	
	if ( FastBoot == TRUE ) {
		Sleep(5);
		i=SAA7146_FirmVersion();
        sprintf(Text,"0x%08X",i);
        if ( strcmp(FirmID,Text) == 0 )  {
			SetDlgItemText(SplashWnd,IDC_TEXT3,t("Fast-Boot"));
			Write_Log("SAA7146 Fast-Boot");
			Skip_VES = TRUE;
			goto Load_Firmware_ENDE;
		}
	}
	
	
	
	if ( SAVE_BOOT == FALSE ) {
		
		if ((SaveBootFile=fopen("SaveBoot.Set","r")) != NULL )  {
			ShowWindow(SplashWnd,SW_HIDE);
			KillTimer(SplashWnd,2);
			SAVE_BOOT = TRUE ;
			DialogBox(hInst,"SAACARDSET",hWnd,SaaBootOptionProc);
			ShowWindow(SplashWnd,SW_SHOW);
			SetTimer(SplashWnd,2,2000,NULL);
			
			fclose(SaveBootFile);
			remove("SaveBoot.Set");
		}
	};
	
	if ( SAVE_BOOT == FALSE ) {
		SaveBootFile=fopen("SaveBoot.Set","w");
		fprintf(SaveBootFile,"Boote SAA");
		fclose(SaveBootFile);
		
		SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1,0xbfff0000);
		Sleep(100);
		//	 clear out any rps-signals pending 
		for( i = 0x0; i < 0xfc; i+=0x4 ) {
			SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, i,0x00000000);
		}
		
		for( i = 0x104; i < 0x1fc; i+=0x4 ) {
			SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, i,0x00000000);
		}
		
		//	 clear out any rps-signals pending 
		SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,0xf8000000);
		
		//	 enable video-port-pins
		SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1,(MASK_10 | MASK_26));
		
		//	 disable all interrupt-conditions, only enable RPS interrupts 
		remove("SaveBoot.Set");
	};
	
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, ISR,0xffffffff);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, IER, (MASK_27 | MASK_28));
	
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, PCI_BT_V1, 	0x1c00101f);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, BCS_CTRL, 	0x80400040);
	
	//	 set dd1 stream a & b 
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DD1_STREAM_B, 	0x00000000);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DD1_INIT, 	0x02000000);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2, 	(MASK_09 | MASK_25 | MASK_10 | MASK_26));
	
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2, 0x077c077c );
	
	
	Sleep(100);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, GPIO_CTRL, 0x500000 );
    Sleep(50); 
	// Neu beim Starten
    SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, GPIO_CTRL, 0x000000);
    /* Disable DEBI and GPIO irq */
    SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, IER, SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, IER)&~(MASK_19 | MASK_03));
    SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, ISR, (MASK_19 | MASK_03));
	
	
	SAA7146_SetGPIO(RESET_LINE, GPIO_OUTLO);
	
	/* enable DEBI */
    SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, 0x08800880);
    SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DD1_STREAM_B, 0x00000000);
    SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2, (MASK_09 | MASK_25 | MASK_10 | MASK_26));
	
	SAA7146_WDebi(0x6e0000, DPRAM_BASE, 0x76543210, 4);
	
    j=0;
	while (( i != 0x10325476 ) && ( j < 10 )) {
		Sleep(1);
		i=SAA7146_RDebi(DEBINOSWAP, DPRAM_BASE, 0, 4);
		j++;
    };
	
	
	if ( i!=0x10325476) {
		Write_Log("SAA7146 HardwareCheck mit Fehler");
		SetDlgItemText(SplashWnd,IDC_TEXT3,t("Falsche Antwort"));
		SetDlgItemText(SplashWnd,IDC_TEXT4,t("von DVB-Hardware"));
		return FALSE;
	}
	Write_Log("SAA7146 HardwareCheck OK");
	
	Sleep(30);
	SAA7146_SetGPIO( 0, GPIO_IRQLO);
	Sleep(50);
	SAA7146_WDebi(DEBISWAB, DPRAM_BASE, (DWORD)SAA7146_BootCode, SAA7146_BootCodeLen);
	Sleep(5);
	SAA7146_WDebi(DEBINOSWAP, BOOT_STATE, BOOTSTATE_BUFFER_FULL, 2);
	
	(void)SAA7146_Wait_for_Debi_done();
	Sleep(50);
	SAA7146_SetGPIO(RESET_LINE, GPIO_OUTHI);
	Sleep(50);
	
	sprintf(FileName,"%s\\Root",FIRM_DIR);
	DRamLen=SAA7146_FirmRead(FileName,NULL,0 );
	RootBytes=DRamLen;

	sprintf(Text,"SAA7146 %s mit %d Bytes",FileName,DRamLen);
	Write_Log(Text);

	if ( DRamLen <= 0) {
		Write_Log("SAA7146 Root nicht gefunden");
		//SetDlgItemText(SplashWnd,IDC_TEXT3,"Firmware");
		//SetDlgItemText(SplashWnd,IDC_TEXT4,"Root");
		//SetDlgItemText(SplashWnd,IDC_TEXT5,"nicht gefunden");
		return(FALSE);
	}
	
	DRammem=malloc(DRamLen+64);
	
	if ( DRammem == NULL ) {
		Write_Log("Kein Speicher zum Laden von root");
		SetDlgItemText(SplashWnd,IDC_TEXT3,t("MEMORY-FAULT"));
		SetDlgItemText(SplashWnd,IDC_TEXT4,t("Nicht genug Speicher"));
		SetDlgItemText(SplashWnd,IDC_TEXT5,t("für Root"));
		return(FALSE);
	}
	
	i=SAA7146_FirmRead(FileName,DRammem,DRamLen );

	if ( RootBytes == 254036 && DRammem[0] == 0xb5) {
		// Linux_9_3
		VideoPID=0;
		AudioPID=1;
		MultiPID=12;
		AddPIDFilter=13;
		DelPIDFilter=14;
	} else if ( RootBytes == 254324 && DRammem[0] == 0xb5 ) {
		// Linux_9_4_05_03
		VideoPID=0;
		AudioPID=1;
		MultiPID=12;
		AddPIDFilter=13;
		DelPIDFilter=14;
	} else if ( RootBytes == 254176 && DRammem[0] == 0xb5 ) {
		// Linux_9_4_01_04
		VideoPID=0;
		AudioPID=1;
		MultiPID=6;
		AddPIDFilter=7;
		DelPIDFilter=8;
	} else if ( RootBytes == 254444 && DRammem[0] == 0xb4 ) {
		// Linux_9_4_30_08
		MultiPID=0;
		VideoPID=1;
		AudioPID=2;
		AddPIDFilter=7;
		DelPIDFilter=8;
		RootBytes = 254176; // Until Yanksee knows about it
	} else {
		Write_Log("Unsupported Root file version");
		return FALSE;
	} 

    load_alternate_osd_font(DRammem, DRamLen);

	i=SAA7146_Load_Dram(DRammem, DRamLen);
	free(DRammem);
	if ( i < 0 ) {
		sprintf(Text,"SAA7146 %s SAA7146_Load_Dram < 0 ",FileName);
		Write_Log(Text);
		return(FALSE);
	};
	sprintf(Text,t("SAA7146 %s geladen"),FileName);
	Write_Log(Text);
	
	Sleep(100);
	SAA7146_SetGPIO(RESET_LINE, GPIO_OUTLO);
	Sleep(100);
	
	sprintf(FileName,"%s\\Dpram",FIRM_DIR);
	DPRamLen=SAA7146_FirmRead(FileName,NULL,0 );
	sprintf(Text,"SAA7146 %s mit %d Bytes",FileName,DPRamLen);
	Write_Log(Text);
	
	if ( DPRamLen <= 0 ) {
		Write_Log("SAA7146 Dpram nicht gefunden");
		SetDlgItemText(SplashWnd,IDC_TEXT4,t("DPRAM"));
		SetDlgItemText(SplashWnd,IDC_TEXT5,t("nicht gefunden"));
		return(FALSE);
	}
	
	
	
	DPRammem=malloc(DPRamLen+64);
	if ( DPRammem == NULL ) {
		Write_Log("Kein Speicher zum Laden von DPRAM");
		SetDlgItemText(SplashWnd,IDC_TEXT3,t("MEMORY-FAULT"));
		SetDlgItemText(SplashWnd,IDC_TEXT4,t("Nicht genug Speicher"));
		SetDlgItemText(SplashWnd,IDC_TEXT5,t("für DPRAM "));
		return(FALSE);
	}
	
	i=SAA7146_FirmRead(FileName,DPRammem,DPRamLen );
	if ( i < 0 ) {
		sprintf(Text,"SAA7146 %s SAA7146_FirmRead < 0 ",FileName);
		Write_Log(Text);
		return(FALSE);
	}
	sprintf(Text,t("SAA7146 %s geladen"),FileName);
	Write_Log(Text);
	
	SAA7146_WDebi(DEBISWAB, DPRAM_BASE,(DWORD)DPRammem, DPRamLen);
	Sleep(100);
	(void)SAA7146_Wait_for_Debi_done();
	free(DPRammem);
	sprintf(Text,"SAA7146 Firmware Boot done");
	Write_Log(Text);
	SAA7146_SetGPIO(RESET_LINE, GPIO_OUTHI);
	Sleep(2000);
	ARM_ResetMailBox();  //NEU
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, ISR, (MASK_19 | MASK_03));
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, IER, SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, IER) | MASK_03 );
	Reset_Debi();         
	j=0;
	i=0;
	
	while (( i< 5 ) && ( j	== 0 )) {
		Sleep(5);
		j=SAA7146_FirmVersion();
        i++;
	};
	sprintf(FirmID,"0x%08X",j);
	
Load_Firmware_ENDE:
	
	sprintf(Text,"SAA7146 Firmversion = %s",FirmID);
	Write_Log(Text);
	sprintf(Text,"Firmware %s",FirmID);
	SetDlgItemText(SplashWnd,IDC_TEXT4,Text);
	sprintf(Text,"Init Frontend");
	SetDlgItemText(SplashWnd,IDC_TEXT5,Text);
	
	if ( DVBDecoderType == 0 ) {
		VES_TYPE=0;
		if ( Timing.WriteRegDelay == 0xffffffff ) Timing.WriteRegDelay=2;
		Write_Log("SAA7146 DVBDecoderType = 0 ( VES 1820 )");
		strcpy(VESTyp,"VES1820"); 
		SetDlgItemText(SplashWnd,IDC_TEXT5,t("Decoder VES1820"));
		i=VES1820_Init(Skip_VES);
		if ( i < 0 ) DVBDecoderType=-1;
	} 
	
	if ( DVBDecoderType == 1 ) {
		VES_TYPE=1;
		if ( Timing.WriteRegDelay == 0xffffffff ) Timing.WriteRegDelay=0;
		Write_Log("SAA7146 DVBDecoderType = 1 ( VES 1893 )");
		strcpy(VESTyp,"VES1893"); 
		SetDlgItemText(SplashWnd,IDC_TEXT5,t("Decoder VES1893"));
		i=VES1893_Init(Skip_VES);
		if ( i < 0 ) DVBDecoderType=-1;
	} 
	
	if ( DVBDecoderType == 2 ) {
		VES_TYPE=2;
		if ( Timing.WriteRegDelay == 0xffffffff ) Timing.WriteRegDelay=0;
		Write_Log("SAA7146 DVBDecoderType = 2 ( STV 0299)");
		strcpy(VESTyp,"STV0299"); 
		SetDlgItemText(SplashWnd,IDC_TEXT5,t("Decoder STV0299"));
		i=STV0299_Init(Skip_VES);
		if ( i < 0 ) DVBDecoderType=-1;
	} 
	
	if ( DVBDecoderType == 3 ) {
		VES_TYPE=3;
		if ( Timing.WriteRegDelay == 0xffffffff ) Timing.WriteRegDelay=0;
		Write_Log("SAA7146 DVBDecoderType = 3 ( TDA8083 )");
		strcpy(VESTyp,"TDA8083"); 
		SetDlgItemText(SplashWnd,IDC_TEXT5,t("Decoder TDA8083"));
		i=TDA8083_Init(Skip_VES);
		if ( i < 0 ) DVBDecoderType=-1;
	} 
	
	
	if ( DVBDecoderType == -1 ) {
		Write_Log("SAA7146 DVBDecoderType Autodetect");
		SetDlgItemText(SplashWnd,IDC_TEXT5,t("Decoder-Detection"));
		//        Sleep(50);
		Timing.WriteRegDelay = 2;
		VES_TYPE=0;
		Write_Log("SAA7146 Check VES1820");
		i=VES1820_Init(FALSE);
        if ( i >= 0 ) {
			Write_Log("SAA7146 VES1820 OK");
            strcpy(VESTyp,"VES1820"); 
			SetDlgItemText(SplashWnd,IDC_TEXT5,t("VES-1820 OK"));
		} else if ( i < 0 ) {
			Timing.WriteRegDelay=0;
			VES_TYPE=1;
			Write_Log("SAA7146 Check VES1893");
			i=VES1893_Init(FALSE);
			if ( i >= 0 ) {
				SetDlgItemText(SplashWnd,IDC_TEXT5,t("VES-1893 OK"));
				Write_Log("SAA7146 VES1893 OK");
				strcpy(VESTyp,"VES1893"); 
			} else {
				VES_TYPE=2; 
				Write_Log("SAA7146 Check STV0299");
				i=STV0299_Init(FALSE);
				if ( i >= 0 ) {
					Write_Log("SAA7146 STV0299 OK");
					strcpy(VESTyp,"STV0299"); 
				} else {
					VES_TYPE=3; 
					Write_Log("SAA7146 Check TDA8083");
					i=TDA8083_Init(FALSE);
					if ( i >= 0 ) {
                        Write_Log("SAA7146 TDA8083 OK");
						strcpy(VESTyp,"TDA8083"); 
					} else {
                        VES_TYPE=-1;
					}
					
				}
				
			}
			
			
		};
	};
	
	if ( VES_TYPE < 0 ) {
		Write_Log(t("SAA7146 Unbekannter Decoder"));
		VES_TYPE=-1;
		SetDlgItemText(SplashWnd,IDC_TEXT5,t("Decoder-Init Fail"));
		return(FALSE);
	};
	
	sprintf(Text,t("Decoder %s"),VESTyp);
	SetDlgItemText(SplashWnd,IDC_TEXT5,Text);
	
	if ( DVBDecoderType == -1 ) {
		Write_Log("**********************");
		Write_Log("SAA7146 Erster Start");
		if ( VES_TYPE == 3 ) {
			Write_Log("SAA7146 DVBTunerType         = 3");
			Write_Log("SAA7146 VES_Type             = 3");
			Write_Log("SAA7146 Timing.WriteRegDelay = 0");
			DVBTunerType=3;
			Timing.WriteRegDelay=0;
			Timing.DVB_Reset_Wait=25;
		};
		if ( VES_TYPE == 2 ) {
			Write_Log("SAA7146 DVBTunerType         = 2");
			Write_Log("SAA7146 VES_Type             = 2");
			Write_Log("SAA7146 Timing.WriteRegDelay = 0");
			DVBTunerType=2;
			Timing.WriteRegDelay=0;
		};
		
		if ( VES_TYPE == 1 ) {
			Write_Log("SAA7146 DVBTunerType         = 1");
			Write_Log("SAA7146 VES_Type             = 1");
			Write_Log("SAA7146 Timing.WriteRegDelay = 0");
			DVBTunerType=1;
			Timing.WriteRegDelay=0;
			SAVE_BOOT=TRUE;
		};
		if ( VES_TYPE == 0 ) {
			Write_Log("SAA7146 VES1820 Tuner-Detection ");
			Buffer[0]=0x3b;
			Buffer[1]=0xb8;
			Buffer[2]=0x95;
			Buffer[3]=0x30;
			
			if (i2c_master_send(0x61 ,(unsigned char)0,Buffer,4) == 4 ) {
				Write_Log("SAA7146 VES1820 DVBTunerType = 0 ( SPXXXX )  ");
				DVBTunerType=0;
			} else {
				Write_Log("SAA7146 VES1820 DVBTunerType = 4 ( SP5659C )  ");
				DVBTunerType=4;
            };        
			Write_Log("SAA7146 VES_Type             = 0");
			Write_Log("SAA7146 Timing.WriteRegDelay = 2");
			Timing.WriteRegDelay=2;
			LNB[0].Use=TRUE;
			LNB[0].MinFreq=322000;
			LNB[0].MaxFreq=454000;
			LNB[0].LofLow=0;
			LNB[0].LofHigh=0;
			LNB[0].SwitchFreq=0;
			LNB[0].PowerReset=FALSE;
			LNB[0].Power=TRUE;
			LNB[0].Switch22khz=FALSE;
			LNB[0].BurstVal=-1;
			LNB[0].PosVal='A';
			LNB[0].OptVal='A';
			SRates[0]=6900;
			SRates[1]=0;
			SRates[2]=0;
			SRates[3]=0;
			SRates[4]=0;
			SRates[5]=0;
			SRates[6]=0;
			SRates[7]=0;
			SRates[8]=0;
			SRates[9]=0;
			SRates[10]=0;
			SRates[11]=0;
			SRates[12]=0;
			SRates[13]=0;
			SRates[14]=0;
			SRates[15]=0;
		};
		
		Write_Log("SAA7146 Setze Default´s");
		DVBDecoderType = VES_TYPE;
		Write_Log("**********************");
	}
	
	GetDlgItemText(SplashWnd,IDC_TEXT2,Text,80);
	SetDlgItemText(SplashWnd,IDC_TEXT1,Text);
	GetDlgItemText(SplashWnd,IDC_TEXT3,Text,80);
	SetDlgItemText(SplashWnd,IDC_TEXT2,Text);
	GetDlgItemText(SplashWnd,IDC_TEXT4,Text,80);
	SetDlgItemText(SplashWnd,IDC_TEXT3,Text);
	GetDlgItemText(SplashWnd,IDC_TEXT5,Text,80);
	SetDlgItemText(SplashWnd,IDC_TEXT4,Text);
	
    memset(MD_API_Version,0x00,32);
	sprintf(MD_API_Version,"MD-API Version 01.03 Root %d",RootBytes);

	for ( i=0; i < plugins_count; i++ ) {
		if ( Ext_Dll[i].Extern_Init != NULL ) {
			KeepRunning=1;
			(Ext_Dll[i].Extern_Init)(hInst,hWnd,WRITE_LOG,i,&Ext_Dll[i].HotKey,&MD_API_Version[0],&KeepRunning);
			if ( KeepRunning == 1 ) {
                     // Externe DLL Sagt : Ich kann / Will nicht laufen 
                   		sprintf(Zeile,"MDAPI DLL %s will wieder entladen werden",Ext_Dll[i].Name);
                   		Write_Log(Zeile);
						Unload_External_Dll(i);
			}
		}
	};

	
	
	if ( DVB_AUDIO == -1 ) { 
		Write_Log("SAA7146 DVB AUDIO Type Autodetect");
		SetDlgItemText(SplashWnd,IDC_TEXT5,t("Audio-Detection"));
		
		if (i2c_writereg( 0x20, 0x00, 0x00) == 1) DVB_AUDIO=1;
		else DVB_AUDIO=0;
	};
	
	if ( DVB_AUDIO == 0 ) {
		Write_Log("SAA7146 DVB AUDIO Type DVB-TI-DAC");
		SetDlgItemText(SplashWnd,IDC_TEXT5,t("DVB-TI-DAC"));
	} else { 
		Write_Log("SAA7146 DVB AUDIO Type DVB-CRYSTAL DAC");
		SetDlgItemText(SplashWnd,IDC_TEXT5,t("DVB-CRYSTAL DAC"));
		Init_Crystal_Dac();
	};
	
	SAA7146_OutCom( COMTYPE_ENCODER, LoadVidCode, 1, 0);
	
	Reset_Debi();
	SAA7146_SetColourFormat(ColourFormat);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0,MC1, (MASK_21));
	Get_CI_Status();
	return(TRUE);
};

unsigned char *find_in_mem(const unsigned char *where, const int wherelen, const unsigned char *what, const int whatlen) {

	unsigned char *ptr;
	int i;
	ptr = NULL;
	if (whatlen <= 0 || wherelen <= 0 || wherelen<whatlen) return NULL;
	for (ptr=(unsigned char*)where; ptr<=where+wherelen-whatlen; ptr++) {
		for (i=0; ptr+i<where+wherelen && i<whatlen; i++) {
			if (ptr[i] != what[i]) break;
		}
		if (i >= whatlen) break;
	}
	if (ptr > where+wherelen-whatlen) return NULL;
	return ptr;
	return ptr;
}

void load_alternate_osd_font(unsigned char *DRammem, int DRamLen) {
	unsigned char *buf, *ptr;
	int buflen, i;
	static const unsigned char begin_osd1[12] = {
		0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x1b};
	static const unsigned char begin_osd2[12] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x21};
	static const unsigned char begin_osd3[12] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x27};
	
	char FileName[256];
    char Text[256];

	ptr = find_in_mem(DRammem, DRamLen, begin_osd1, sizeof(begin_osd1));
      if (ptr != NULL && memcmp(ptr+13964, begin_osd2, sizeof(begin_osd2)) != 0) ptr = NULL;
      if (ptr != NULL && memcmp(ptr+31000, begin_osd3, sizeof(begin_osd3)) != 0) ptr = NULL;
	if (ptr == NULL) {
		Write_Log("Can't find the osd signature(s) in Root");
		return;
	}

	sprintf(FileName,"%s\\osd_font.dat",FIRM_DIR);
	buflen=SAA7146_FirmRead(FileName,NULL,0 );
	sprintf(Text,"OSD font file %s mit %d Bytes",FileName,buflen);
	Write_Log(Text);
	
	if ( buflen <= 0 ) return;
	if ( buflen != 56364 ) {
		Write_Log("The OSD font file has an incorrect size, should be 56364.");
		return;
	}
	buf=malloc(buflen);
	if ( buf == NULL ) {
		Write_Log("Kein Speicher zum Laden von OSD font");
		return;
	}
	
	i=SAA7146_FirmRead(FileName,buf,buflen);
	if ( i < 0 ) {
		sprintf(Text,"SAA7146 %s SAA7146_FirmRead < 0 ",FileName);
		Write_Log(Text);
		free(buf);
		return;
	}
	sprintf(Text,"SAA7146 %s chargée",FileName);
	Write_Log(Text);

	if (memcmp(buf, "OSD_FONT", 8) != 0 ||
	    memcmp(buf+264, begin_osd1, sizeof(begin_osd1)) != 0 ||
	    memcmp(buf+264+13964, begin_osd2, sizeof(begin_osd2)) != 0 ||
	    memcmp(buf+264+31000, begin_osd3, sizeof(begin_osd3)) != 0) {
		sprintf(Text,"Invalid signature(s) in file %s",FileName);
		Write_Log(Text);
		free(buf);
		return;
	}

	buf[8] = 0;
	for (i=9; i<264; i++) if (buf[i] == 0) buf[i] = 0x80;
	memcpy(iso88591_to_osdfont, buf+8, 256);
	memcpy(ptr, buf+264, 56100);

	free(buf);

}

void Init_Crystal_Dac( void ) 
{
/*
Linux
i2c_writereg( 0x20, 0x01, 0x28);
i2c_writereg( 0x20, 0x02, 0x69);
i2c_writereg( 0x20, 0x03, 0x20);
i2c_writereg( 0x20, 0x04, 0x20);
	*/
	/* Alt 
    i2c_writereg(0x20, 0x01, 0x08);
    i2c_writereg(0x20, 0x02, 0x09);
    i2c_writereg(0x20, 0x03, 0x20);
    i2c_writereg(0x20, 0x04, 0x20);
	*/
    i2c_writereg(0x20, 0x01, 0xd2);
    i2c_writereg(0x20, 0x02, 0x49);
    i2c_writereg(0x20, 0x03, 0x06);
    i2c_writereg(0x20, 0x04, 0x06);
    i2c_writereg(0x20, 0x03, 0xc6);
    i2c_writereg(0x20, 0x04, 0xc6);
    i2c_writereg(0x20, 0x03, 0x01);
    i2c_writereg(0x20, 0x04, 0x01);
    i2c_writereg(0x20, 0x03, 0x80);
    i2c_writereg(0x20, 0x04, 0x80);
    i2c_writereg(0x20, 0x03, 0xc6);
    i2c_writereg(0x20, 0x04, 0xc6);
    i2c_writereg(0x20, 0x03, 0xc6);
    i2c_writereg(0x20, 0x04, 0xc6);
    i2c_writereg(0x20, 0x03, 0xc6);
    i2c_writereg(0x20, 0x04, 0xc6);
	
};




void SAA7146_SetGPIO( int port, DWORD data){
	DWORD val;
	//          SAA7146_WriteByte(hSAA7146, SAA7146_AD_BAR0, GPIO_CTRL+port, data);
	//		  return;
		  
		  
	val=SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0,GPIO_CTRL);
	val&=~(0xff << (8*(port)));
	val|=(data)<<(8*(port));
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, GPIO_CTRL, val);
}


int SAA7146_DebiWrite(DWORD config, int addr, DWORD val, int count){
	DWORD Length=(count << 17) | (addr & 0xffff);
    DWORD PokeReg=(2 << 16) | 2;
	if (count <= 0 || count > 0x2000) return(-1);
	if (SAA7146_Wait_for_Debi_done() < 0) return(-1);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_CONFIG,config );
	if (count <= 4)	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,val);
	else SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,(unsigned long)debi_mem_dma.Page[0].pPhysicalAddr);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_COMMAND,Length);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,PokeReg);
	return 0;
}


DWORD SAA7146_DebiRead(DWORD config, int addr, int count){
	DWORD result = 0;
	DWORD Length=(count << 17) | 0x10000 | (addr & 0xffff);
    DWORD PokeReg=(2 << 16) | 2;
	
	if (count > 0x2000 || count <= 0) return 0;
	//	if (SAA7146_Wait_for_Debi_done() < 0) return 0;
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,(DWORD)debi_mem_dma.Page[0].pPhysicalAddr);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_COMMAND,Length);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_CONFIG,config );
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,PokeReg);
	if (count > 4) return count;
	SAA7146_Wait_for_Debi_done();
	result = SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD);
	result &= (0xffffffffUL >> ((4-count)*8));
	return result;
}


int SAA7146_Wait_for_Debi_done(){
	unsigned int i,l1,l2;
	unsigned int valmc2,valpsr;
	l1=0 ; 
	l2=0;
	for (i = 0; i < 100; i++) {
		valmc2=SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, MC2);
		Sleep(0);
		if ((valmc2&0x02) != 0x02 )  break;
		l1++;
		
	}; 
	for (i = 0; i < 500; i++ ) {
		valpsr=SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, PSR);
		if ((valpsr&SPCI_DEBI_S) != SPCI_DEBI_S ) break;
		valmc2=SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, MC2);
		Sleep(0);
		l2++;
	};
	return 0;
}


void SAA7146_AudioCom(unsigned short com){
	SAA7146_OutCom( 0x80, 0x03, 4, (com>>16), (com&0xffff));
}

void SAA7146_VideoCom(unsigned short com,unsigned short value){
    SAA7146_OutCom( 0x80, 0x02, 4,(com>>16), (com&0xffff), (value>>16), (value&0xffff));
}


void SAA7146_WDebi(DWORD config, int addr, DWORD val, int count){
	if (count>4 && val) memcpy(debi_mem_dma.pUserAddr, (unsigned char *)val, count);
	SAA7146_DebiWrite(config, addr, val, count);
}

DWORD SAA7146_RDebi(DWORD config, int addr, DWORD val, int count){
    DWORD res;
	res=SAA7146_DebiRead(config, addr, count);
	if (count>4 && val) memcpy((unsigned char *) val, debi_mem_dma.pUserAddr, count);
	return res;
}



int SAA7146_Open (SAA7146_HANDLE *phSAA7146, DWORD dwVendorID, DWORD dwDeviceID,  DWORD options, BOOL Lock){
	int Ret = 0;
    char Path[255];
    char SystemDir[255];
    int i;
	
    SAA7146_HANDLE hSAA7146 = (SAA7146_HANDLE) malloc (sizeof (SAA7146_STRUCT));
	
    WD_PCI_SCAN_CARDS pciScan;
    WD_PCI_CARD_INFO pciCardInfo;
	
    BZERO(*hSAA7146);
	
	sprintf(SystemDir,"Suche Karte VendorID %04x DeviceID %04x ",dwVendorID,dwDeviceID);
    Write_Log(SystemDir);
	
    hSAA7146->cardReg.hCard = 0;
    hSAA7146->hWD = WD_Open();
	
    // check if handle valid & version OK
	if (hSAA7146->hWD==INVALID_HANDLE_VALUE){
		sprintf(SystemDir,"Treiber MDHALL nicht offen ");
        Write_Log(SystemDir);
        //Cannot open WinDriver device
		Ret=1;
        goto Exit;
    }
	
    BZERO(pciScan);
    pciScan.searchId.dwVendorId = dwVendorID;
    pciScan.searchId.dwDeviceId = dwDeviceID;
    WD_PciScanCards (hSAA7146->hWD, &pciScan);
	if (pciScan.dwCards==0) {// Found at least one card
		sprintf(SystemDir,"Karte VendorID %04x DeviceID %04x nicht gefunden",dwVendorID,dwDeviceID);
        Write_Log(SystemDir);
		Ret=2;
        // error - Cannot find PCI card
        goto Exit;
    }
	
    BZERO(pciCardInfo);
    pciCardInfo.pciSlot = pciScan.cardSlot[0];
    WD_PciGetCardInfo (hSAA7146->hWD, &pciCardInfo);
    hSAA7146->pciSlot = pciCardInfo.pciSlot;
    hSAA7146->cardReg.Card = pciCardInfo.Card;
	
    hSAA7146->fUseInt = TRUE;
	
	// make interrupt resource sharable
    	for (i=0; i<(int)hSAA7146->cardReg.Card.dwItems; i++){
		WD_ITEMS *pItem = &hSAA7146->cardReg.Card.Item[i];
		if (pItem->item==ITEM_INTERRUPT)
			pItem->fNotSharable = FALSE;
	}
	
    hSAA7146->cardReg.fCheckLockOnly = FALSE;
    WD_CardRegister (hSAA7146->hWD, &hSAA7146->cardReg);
	if (hSAA7146->cardReg.hCard==0){
		sprintf(SystemDir,"Card Register : Shared Lock nicht möglich ");
        Write_Log(SystemDir);
		sprintf(SystemDir,"                Kann so nicht weiter machen !!!");
        Write_Log(SystemDir);
        Ret=3;
        goto Exit;
	}
    
	if (!SAA7146_DetectCardElements(hSAA7146)){
		sprintf(SystemDir,"Card Elements : Karten-Elemente können nicht abgefragt werden ");
        Write_Log(SystemDir);
		sprintf(SystemDir,"                Kann so nicht weiter machen !!!");
        Write_Log(SystemDir);
		Ret=4;
        goto Exit;
    }
	
	
	if ( NT == TRUE ) {
		if ( Keep_Driver_Loaded == FALSE ) {
			GetCurrentDirectory(  sizeof (Path),Path );
			strcat(Path,"\\MDSAA.SYS");
			
			if ( ! LoadDeviceDriver( t("MultiDec SAA-Treiber"), Path, &DVB_Plugin_Handle,TRUE  ) )  {
				sprintf(SystemDir,t("SAA7146 NT-Treibererweiterung kann nicht geladen werden"));
                Write_Log(SystemDir);
			};
		};
	} else {
		GetSystemDirectory(SystemDir,sizeof(SystemDir));
		strcat(SystemDir,"\\vmm32\\MDSAA.VXD");
		if ( CopyFile("MDSAA.vxd",SystemDir,FALSE) == FALSE ) {
			sprintf(SystemDir,"SAA7146 Windoof 9X-Treibererweiterung MDSAA.vxd kann nicht kopiert werden");
		} else {
			sprintf(SystemDir,"SAA7146 Windoof 9X-Treibererweiterung MDSAA.vxd bereit");
		};
		Write_Log(SystemDir);
	};
    // Open finished OK
    *phSAA7146 = hSAA7146;
    return Ret;
	
Exit:
	
	sprintf(SystemDir,"SAA7146_Open Karte VendorID %04x DeviceID %04x mit FehlerCode %d ",dwVendorID,dwDeviceID,Ret);
    Write_Log(SystemDir);
    // Error during Open
    if (hSAA7146->cardReg.hCard) 
        WD_CardUnregister(hSAA7146->hWD, &hSAA7146->cardReg);
    if (hSAA7146->hWD!=INVALID_HANDLE_VALUE)
        WD_Close(hSAA7146->hWD);
    free (hSAA7146);
    return Ret;
}



BOOL SAA7146_DetectCardElements(SAA7146_HANDLE hSAA7146){
    DWORD i;
    DWORD ad_sp;
	
    BZERO(hSAA7146->Int);
    BZERO(hSAA7146->addrDesc);
	
    for (i=0; i<hSAA7146->cardReg.Card.dwItems; i++){
        WD_ITEMS *pItem = &hSAA7146->cardReg.Card.Item[i];
		
        switch (pItem->item){
        case ITEM_MEMORY:
        case ITEM_IO: {
                DWORD dwBytes;
                DWORD dwPhysAddr;
                BOOL fIsMemory;
                if (pItem->item==ITEM_MEMORY){
                    dwBytes = pItem->I.Mem.dwBytes;
                    dwPhysAddr = pItem->I.Mem.dwPhysicalAddr;
                    fIsMemory = TRUE;
                }
                else {
                    dwBytes = pItem->I.IO.dwBytes;
                    dwPhysAddr = pItem->I.IO.dwAddr;
                    fIsMemory = FALSE;
                }
				
                for (ad_sp=0; ad_sp<SAA7146_ITEMS; ad_sp++){
                    DWORD dwPCIAddr;
                    DWORD dwPCIReg;
					
                    if (SAA7146_IsAddrSpaceActive(hSAA7146, ad_sp)) continue;
                    if (ad_sp<SAA7146_AD_EPROM) dwPCIReg = PCI_BAR0 + 4*ad_sp;
                    else dwPCIReg = PCI_ERBAR;
                    dwPCIAddr = SAA7146_ReadPCIReg(hSAA7146, dwPCIReg);
                    if (dwPCIAddr & 1){
                        if (fIsMemory) continue;
                        dwPCIAddr &= ~(0x3);
                    }
                    else{
                        if (!fIsMemory) continue;
                        dwPCIAddr &= ~(0xf);
                    }
                    if (dwPCIAddr==dwPhysAddr)
                        break;
                }
                if (ad_sp<SAA7146_ITEMS){
                    DWORD j;
                    hSAA7146->addrDesc[ad_sp].fActive = TRUE;
                    hSAA7146->addrDesc[ad_sp].index = i;
                    hSAA7146->addrDesc[ad_sp].fIsMemory = fIsMemory;
                    hSAA7146->addrDesc[ad_sp].dwMask = 0;
                    for (j=1; j<dwBytes && j!=0x80000000; j *= 2){
                        hSAA7146->addrDesc[ad_sp].dwMask = 
                            (hSAA7146->addrDesc[ad_sp].dwMask << 1) | 1;
                    }
                }
            }
            break;
        case ITEM_INTERRUPT:
            if (hSAA7146->Int.Int.hInterrupt) return FALSE;
            hSAA7146->Int.Int.hInterrupt = pItem->I.Int.hInterrupt;
            break;
        }
    }
	
    // check that all the items needed were found
    // check if interrupt found
    if (hSAA7146->fUseInt && !hSAA7146->Int.Int.hInterrupt){
        return FALSE;
    }
	
    // check that at least one memory space was found
    for (i = 0; i<SAA7146_ITEMS; i++)
        if (SAA7146_IsAddrSpaceActive(hSAA7146, i)) break;
		if (i==SAA7146_ITEMS) return FALSE;
		return TRUE;
}

void Reload_SAA_Driver( void )
{
	char Path[256];
	char SystemDir[256];
	
	Keep_Driver_Loaded = FALSE;
	UnloadDeviceDriver("MultiDec SAA-Treiber",TRUE);
	
	if ( NT == TRUE ) {
		GetCurrentDirectory(  sizeof (Path),Path );
		strcat(Path,"\\MDSAA.SYS");
		sprintf(SystemDir,"SAA7146 Treibererweiterung : %s ",Path);
        Write_Log(SystemDir);
		if ( ! LoadDeviceDriver( "MultiDec SAA-Treiber", Path, &DVB_Plugin_Handle,TRUE  ) )  {
			sprintf(SystemDir,"SAA7146 NT-Treibererweiterung kann nicht geladen werden");
			Write_Log(SystemDir);
		};
	} else {
		GetSystemDirectory(SystemDir,sizeof(SystemDir));
		strcat(SystemDir,"\\vmm32\\MDSAA.VXD");
		if ( CopyFile("MDSAA.VXD",SystemDir,FALSE) == FALSE ) {
			sprintf(SystemDir,"SAA7146 Windoof 9X-Treibererweiterung MDSAA.vxd kann nicht kopiert werden");
		} else {
			sprintf(SystemDir,"SAA7146 Windoof 9X-Treibererweiterung MDSAA.vxd bereit");
		}
		Write_Log(SystemDir);
	};
	
};


void DVB_Close(){
	if (hSAA7146 != NULL ) {
		Write_Log("SAA7146_Close gestartet");
		SAA7146_Close(hSAA7146);
		WD_Close(hSAA7146->hWD);
		free (hSAA7146);
		hSAA7146=NULL;
		Write_Log("SAA7146 WD Close");
	};
};


void SAA7146_Close(SAA7146_HANDLE hSAA7146){
	Write_Log("SAA7146 Filter zu");
	ShutDownFilters();
	Debi_Record_Stop();
	Sleep(10);
	Stop_Osd_Key_Event();
	Record=FALSE;
	Play=FALSE;
	DEBICOPY=FALSE;
	
    video_setmode(0);
	if ( VES_TYPE == 0 ) { 
		SAA7146_OutCom(COMTYPE_AUDIODAC, MainSwitch, 1, 0);
		Sleep(20);
		SAA7146_OutCom(COMTYPE_AUDIODAC, ADSwitch, 1, 0);
		Sleep(20);
		VidMode(0);
		Write_Log("SAA7146 DVB-c Audio aus");
	} else {
		DVB_SetVolume(0x00,0x00);
		Write_Log("SAA7146 DVB-s Volume 0");
	};
	
    Run_DVB_Irq(FALSE,FALSE);
	/* disable alle irqs, release irq-routine */
    Sleep(10);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, IER, 0x00000000);
    Sleep(10);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, ISR,0x00000000);
	// Und nun wieder alles schön für die TT-Software machen
    Sleep(10);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, 0x09800980);
    Sleep(10);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2, 0x077f077f);
    Sleep(10);
	
    Write_Log("SAA7146 IRQ aus");
    // disable interrupts
    if (SAA7146_IntIsEnabled(hSAA7146))
        SAA7146_IntDisable(hSAA7146);
    SAA7146_KP_CLEAR_INIT(hSAA7146->hWD, hSAA7146->Int.Int.kpCall.hKernelPlugIn);
	
	Write_Log("SAA7146 Treiber beenden");
    WD_KernelPlugInClose(hSAA7146->hWD, &DVBKernelPlugIn);    
    // unregister card
	
	Write_Log("SAA7146 Unregister Card");
    if (hSAA7146->cardReg.hCard) 
        WD_CardUnregister(hSAA7146->hWD, &hSAA7146->cardReg);
	
	Write_Log("SAA7146 Kernel-Speicher freigeben");
	
	Free_SAA7146_DMA(&debi_mem_dma);
	Free_SAA7146_DMA(&Record_mem_dma);
	Free_SAA7146_DMA(&CA_mem_dma);
	Free_SAA7146_DMA(&CI_mem_dma);
	Free_SAA7146_DMA(&IPMPE_mem_dma);
	Free_SAA7146_DMA(&BitMap_mem_dma);
	Free_SAA7146_DMA(&Ir_mem_dma);
	Free_SAA7146_DMA(&Debug_mem_dma);
	Free_DVB_Display_DMA();
	Free_SAA7146_DMA(&rps0_mem_dma);
}


DWORD SAA7146_ReadPCIReg(SAA7146_HANDLE hSAA7146, DWORD dwReg){
    WD_PCI_CONFIG_DUMP pciCnf;
    DWORD dwVal;
	
    BZERO(pciCnf);
    pciCnf.pciSlot = hSAA7146->pciSlot;
    pciCnf.pBuffer = &dwVal;
    pciCnf.dwOffset = dwReg;
    pciCnf.dwBytes = 4;
    pciCnf.fIsRead = TRUE;
    WD_PciConfigDump(hSAA7146->hWD,&pciCnf);
    return dwVal;
}


BOOL SAA7146_IsAddrSpaceActive(SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace){
    return hSAA7146->addrDesc[addrSpace].fActive;
}

// General read/write function
void SAA7146_ReadWriteBlock(SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset, BOOL fRead, PVOID buf, DWORD dwBytes, SAA7146_MODE mode){
    WD_TRANSFER trans;
    BOOL fMem = hSAA7146->addrDesc[addrSpace].fIsMemory;
    // safty check: is the address range active
    if (!SAA7146_IsAddrSpaceActive(hSAA7146, addrSpace)) return;
    BZERO(trans);
    if (fRead){
        if (mode==SAA7146_MODE_BYTE) trans.cmdTrans = fMem ? RM_SBYTE : RP_SBYTE;
        else if (mode==SAA7146_MODE_WORD) trans.cmdTrans = fMem ? RM_SWORD : RP_SWORD;
        else if (mode==SAA7146_MODE_DWORD) trans.cmdTrans = fMem ? RM_SDWORD : RP_SDWORD;
    }
    else{
        if (mode==SAA7146_MODE_BYTE) trans.cmdTrans = fMem ? WM_SBYTE : WP_SBYTE;
        else if (mode==SAA7146_MODE_WORD) trans.cmdTrans = fMem ? WM_SWORD : WP_SWORD;
        else if (mode==SAA7146_MODE_DWORD) trans.cmdTrans = fMem ? WM_SDWORD : WP_SDWORD;
    }
    if (fMem)
        trans.dwPort = hSAA7146->cardReg.Card.Item[hSAA7146->addrDesc[addrSpace].index].I.Mem.dwTransAddr;
    else trans.dwPort = hSAA7146->cardReg.Card.Item[hSAA7146->addrDesc[addrSpace].index].I.IO.dwAddr;
    trans.dwPort += dwOffset;
	
    trans.fAutoinc = TRUE;
    trans.dwBytes = dwBytes;
    trans.dwOptions = 0;
    trans.Data.pBuffer = buf;
    WD_Transfer (hSAA7146->hWD, &trans);
}

BYTE SAA7146_ReadByte (SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset){
    BYTE data;
    if (hSAA7146->addrDesc[addrSpace].fIsMemory){
        PBYTE pData = (PBYTE) (hSAA7146->cardReg.Card.Item[hSAA7146->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        data = *pData; // read from the memory mapped range directly
    }
    else SAA7146_ReadWriteBlock( hSAA7146, addrSpace, dwOffset, TRUE, &data, sizeof (BYTE), SAA7146_MODE_BYTE);
    return data;
}

WORD SAA7146_ReadWord (SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset){
    WORD data;
    if (hSAA7146->addrDesc[addrSpace].fIsMemory){
        PWORD pData = (PWORD) (hSAA7146->cardReg.Card.Item[hSAA7146->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        data = *pData; // read from the memory mapped range directly
    }
    else SAA7146_ReadWriteBlock( hSAA7146, addrSpace, dwOffset, TRUE, &data, sizeof (WORD), SAA7146_MODE_WORD);
    return data;
}

DWORD SAA7146_ReadDword (SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset){
    DWORD data;
    if (hSAA7146->addrDesc[addrSpace].fIsMemory){
        PDWORD pData = (PDWORD) (hSAA7146->cardReg.Card.Item[hSAA7146->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        data = *pData; // read from the memory mapped range directly
    }
    else SAA7146_ReadWriteBlock( hSAA7146, addrSpace, dwOffset, TRUE, &data, sizeof (DWORD), SAA7146_MODE_DWORD);
    return data;
}

void SAA7146_WriteByte (SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset, BYTE data){
    if (hSAA7146->addrDesc[addrSpace].fIsMemory){
        PBYTE pData = (PBYTE) (hSAA7146->cardReg.Card.Item[hSAA7146->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        *pData = data; // write to the memory mapped range directly
    }
    else SAA7146_ReadWriteBlock( hSAA7146, addrSpace, dwOffset, FALSE, &data, sizeof (BYTE), SAA7146_MODE_BYTE);
}

void SAA7146_WriteWord (SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset, WORD data){
    if (hSAA7146->addrDesc[addrSpace].fIsMemory){
        PWORD pData = (PWORD) (hSAA7146->cardReg.Card.Item[hSAA7146->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        *pData = data; // write to the memory mapped range directly
    }
    else SAA7146_ReadWriteBlock( hSAA7146, addrSpace, dwOffset, FALSE, &data, sizeof (WORD), SAA7146_MODE_WORD);
}

void SAA7146_WriteDword (SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset, DWORD data){
    if (hSAA7146->addrDesc[addrSpace].fIsMemory){
        PDWORD pData = (PDWORD) (hSAA7146->cardReg.Card.Item[hSAA7146->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        *pData = data; // write to the memory mapped range directly
    }
    else SAA7146_ReadWriteBlock( hSAA7146, addrSpace, dwOffset, FALSE, &data, sizeof (DWORD), SAA7146_MODE_DWORD);
}

BOOL SAA7146_IntIsEnabled (SAA7146_HANDLE hSAA7146){
    if (!hSAA7146->fUseInt) return FALSE;
    if (!hSAA7146->Int.hThread) return FALSE;
    return TRUE;
}

void Debi_Watch(){
	int i;
	unsigned int rxbuf;
	unsigned int txbuf;
	char Zeile[128];

	for (;;) {
		for (i=0; i < 1000; i++ ) {
			Sleep(1);
			if ( Run_Debi_Watch == FALSE ) return;
		};
            if ( Debi_Irq > 0 ) {
				    rxbuf= SAA7146_DebiRead(DEBINOSWAP, RX_BUFF,  2);
				    txbuf= SAA7146_DebiRead(DEBINOSWAP, TX_BUFF,  2);
                    if ( LastIntText[0] != 0 ) Write_Log(LastIntText);
					LastIntText[0]=0x00;
					sprintf(Zeile,"SAA7146 Debi_Watch Reset RX_BUFF = %d TX_BUFF = %d",rxbuf,txbuf);
                    Write_Log(Zeile);
				    Debi_Irq=0;
					Reset_Debi();
					continue;
			};
			Debi_Irq++;
	};
}


DWORD WINAPI SAA7146_IntThread (PVOID pData){
    SAA7146_HANDLE hSAA7146 = (SAA7146_HANDLE) pData;
	int status;
    int RetBytes;
	DWORD LinkThreadID;
	int rc;
    SAA7146_INT_INTERRUPT *Int;
	HANDLE InthWD;
	
	InthWD=WD_Open();
	Int=&hSAA7146->Int;
	
	
    NextFrame=1;
    ProzessorMask=1<<(IrqProzessor);
    status=SetThreadAffinityMask (GetCurrentThread(),ProzessorMask);         
	//    SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_HIGHEST);
	
	Write_Log("SAA7146 Starte SAA7146_IntThread");
	Run_Debi_Watch=TRUE;
    Debi_Irq=0;
    LastIntText[0]=0x00;

	CloseHandle(CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
		(DWORD)0,                          // Same stack size.
		(LPTHREAD_START_ROUTINE)Debi_Watch,// Thread procedure.
		NULL,                         // Parameter.
		(DWORD)0,                          // Start immediatly.
		(LPDWORD)&LinkThreadID));  

    Debi_Irq=0;

	while (1) { // IRQ_LOOP_SAA	
	Sleep(0);
	rc = (DWORD) DeviceIoControl(InthWD, (DWORD) IOCTL_WD_INT_WAIT, (PVOID) Int, 48, NULL, 0, &RetBytes, NULL);
	
	if (Int->Int.fStopped){
        Run_Debi_Watch=FALSE;
		WD_Close(InthWD);
		break; // WD_IntDisable() was called
	};
	
	
	SAA7146_KP_GET_IRQ(hSAA7146->hWD, hSAA7146->Int.Int.kpCall.hKernelPlugIn,&IRQsaa);
	
	if ( IRQsaa.PictureSet > 0 )  {
		NextFrame=1;
		if( Decode_Event != NULL ) SetEvent(Decode_Event);
	};
	if ( IRQsaa.debistatus == 1 ) {
		Debi_Irq=0;
        System_in_Sync=0;
		Debi_Interrupt(&IRQsaa);
		if ( IRQsaa.Debi_Done_MC2 == 0x01 ) Write_Log("SAA7146 Debi_done_MC2 TimeOut beim letzten Transfer");
		if ( IRQsaa.Debi_Done_PSR == 0x01 ) Write_Log("SAA7146 Debi_done_PSR TimeOut beim letzten Transfer");
	};
	
	} // end of IRQ_LOOP_SAA
	return(0);
}


void Reset_Debi(){
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, IER,SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, IER) & ~MASK_19 );
	ARM_ClearIrq();
	ARM_SET_SEND(0);
    ARM_CLEAR_RECEIVE( );
}

void CI_handle(unsigned char *data,int len) {
	int ci_slot;
	int i,j,k;
	char Zeile[48];
	
	
	if ( data == NULL ) return;
	if ( ci_interface == 0 ) {
		ci_interface=1;
		SetMenu_CI();
	};
	
	ci_slot=data[2]-1;
	ci_interface_Refresh=TRUE;
	if ((ci_slot==0 ) || ( ci_slot==1)) {
		ci_module[ci_slot].LastCmdLen=len;
		if ( ci_module[ci_slot].LastCmdLen > 128 ) ci_module[ci_slot].LastCmdLen=128;
		memcpy(ci_module[ci_slot].LastCmd,data,ci_module[ci_slot].LastCmdLen);
	};
	
	switch (data[0]) {
	case CI_MSG_CI_INFO:
		if (ci_slot!=0 && ci_slot!=1) {
			ci_interface_LastCmdLen=len;
			if ( ci_interface_LastCmdLen > 128 ) ci_interface_LastCmdLen=128;
			memcpy(ci_interface_LastCmd,data,ci_interface_LastCmdLen);
			return;
		}
		
		switch (data[1]) {
		case 0xff:
		case 0:
			ci_module[ci_slot].Present=0;
			ci_module[ci_slot].ModuleName[0]=0x00;
			SetMenu_CI();
			break;
		case 1:
			ci_module[ci_slot].Present=CI_MODULE_PRESENT;
			strcpy(ci_module[ci_slot].ModuleName,t("Unbekannt"));
			SetMenu_CI();
			break;
		case 2:
			ci_module[ci_slot].Present=CI_MODULE_OK;
			i=4;
			j=0;
			ci_module[ci_slot].ModuleName[0]=0x00;
			while (( i < len ) && ( j < 32 )) {
				if (( data[i] >= 0x20 ) && ( data[i] < 0x7f )) {
					ci_module[ci_slot].ModuleName[j]=data[i];
					j++;
					ci_module[ci_slot].ModuleName[j]=0x00;
				};
				if (( j >= 0 ) && ( data[i] == 0x00 )) {
					j=32;
				};
				
				i++;
			};
			SetMenu_CI();
			break;
		}
		break;
		
		case CI_PSI_COMPLETE:
			if (ci_slot!=0 && ci_slot!=1) break;
			ci_module[ci_slot].Reply=data[1];
			break;
			
		case CI_SWITCH_PRG_REPLY:
			if (ci_slot!=0 && ci_slot!=1) {
				ci_interface_LastCmdLen=len;
				if ( ci_interface_LastCmdLen > 128 ) ci_interface_LastCmdLen=128;
				memcpy(ci_interface_LastCmd,data,ci_interface_LastCmdLen);
				return;
			}
			ci_module[ci_slot].Reply=data[1];
			break;
			
        case CI_MSG_CLOSE_MMI_IMM:
			ci_interface_LastCmdLen=len;
			if ( ci_interface_LastCmdLen > 128 ) ci_interface_LastCmdLen=128;
			memcpy(ci_interface_LastCmd,data,ci_interface_LastCmdLen);
			CI_Menu_Slot=0;
			OSD_Key_Value = 0x00;
			SetEvent(OSD_KEY_EVENT);
			OSD_Key_Value = 0x00;
			SetEvent(OSD_KEY_EVENT);
			break;
			
        case CI_MSG_REQUEST_INPUT:
			
			ci_menu_update=TRUE;
			ci_interface_LastCmdLen=len;
			if ( ci_interface_LastCmdLen > 128 ) ci_interface_LastCmdLen=128;
			memcpy(ci_interface_LastCmd,data,ci_interface_LastCmdLen);
			
            i=0;
			while ((i < 20 ) && ( ci_menu[i].Type != 0x00 )) i++;
			ci_menu[i].Type = 'T';
			j=0;
			while (( data[j+3] != 0x00 )  && ( j < 63 ) && (j+3<len)) {
				ci_menu[i].zeile[j]=data[j+3];
				j++;
				ci_menu[i].zeile[j]=0;
			};
			i++;
			j++;
			
			ci_menu[i].Type  ='E';
			k=0;
			ci_menu[i].laenge = data[2]; 
			sprintf(Zeile,t("  ( %d Bytes gefordert )"),ci_menu[i].laenge);
			strcat(ci_menu[i-1].zeile,Zeile);
			ci_menu[i].zeile[k]=0;
			while (( data[j+3] != 0x00 )  && ( k < 63 ) && (j+3<len)) {
				ci_menu[i].zeile[k]=data[j+3];
				j++;
				k++;
				ci_menu[i].zeile[k]=0;
			};
			
			i++;
			if ( i < 16 ) {
				ci_menu[i].Type  ='T';
				strcpy(ci_menu[i].zeile,t("Eingabe von Werten 0x41 ( Hex ) oder 65 (Dez) oder \"A\" Ascii"));
				i++;
				ci_menu[i].Type  ='T';
				strcpy(ci_menu[i].zeile,t("\"6962\" oder 54 57 54 50 oder 0x36 0x39 0x36 0x32"));
				i++;
				ci_menu[i].Type  ='T';
				strcpy(ci_menu[i].zeile,t("oder 54 \"96\" 0x32"));
			}
			break;
			
        case CI_MSG_MENU:
			
			for ( i=0; i < 20 ; i++ ) { 
				ci_menu[i].Type=0x00;
			};
			
			ci_interface_LastCmdLen=len;
			ci_menu_update=TRUE;
			if ( ci_interface_LastCmdLen > 128 ) ci_interface_LastCmdLen=128;
			memcpy(ci_interface_LastCmd,data,ci_interface_LastCmdLen);
            CILines=data[2];
			CIHeaderLines=2;
			k=0;
			i=3;
			while (( k < CILines ) && ( i < len )) {
				j=0;
				while (( data[i] != 0x00 ) && ( data[i] != 0x8a ) && ( i < len ) && ( j < 63 )) {
					ci_menu[k].zeile[j]=data[i];
					if ( ci_menu[k].zeile[j] == 0x86 ) ci_menu[k].zeile[j]=0x20;
					if ( ci_menu[k].zeile[j] == 0x87 ) ci_menu[k].zeile[j]=0x20;
					i++;
					j++;
					ci_menu[k].zeile[j]=0x00;
				};
				if ( data[i] == 0x8a ) {
					if ( k < CIHeaderLines ) CIHeaderLines++;
					CILines++;
				};
                if ( j > 0 ) {
					if ( k < CIHeaderLines ) ci_menu[k].Type ='T';
					else if ( k == CIHeaderLines ) ci_menu[k].Type='B';
					else ci_menu[k].Type='S';
				};
				i++;
				k++;
			};
			break;
			
        case CI_MSG_LIST:
			for ( i=0; i < 20 ; i++ ) { 
				ci_menu[i].Type=0x00;
			};
			ci_interface_LastCmdLen=len;
			ci_menu_update=TRUE;
			if ( ci_interface_LastCmdLen > 128 ) ci_interface_LastCmdLen=128;
			memcpy(ci_interface_LastCmd,data,ci_interface_LastCmdLen);
            CILines=data[2];
			CIHeaderLines=2;
			k=0;
			i=3;
			while (( k < CILines ) && ( i < len )) {
				j=0;
				while (( data[i] != 0x00 ) && ( data[i] != 0x8a ) && ( i < len ) && ( j < 63 )) {
					ci_menu[k].zeile[j]=data[i];
					if ( ci_menu[k].zeile[j] == 0x86 ) ci_menu[k].zeile[j]=0x20;
					if ( ci_menu[k].zeile[j] == 0x87 ) ci_menu[k].zeile[j]=0x20;
					i++;
					j++;
					ci_menu[k].zeile[j]=0x00;
				};
				
				if ( data[i] == 0x8a ) {
					if ( k < CIHeaderLines ) CIHeaderLines++;
					CILines++;
				};
                if ( j > 0 ) {
					if ( k < CIHeaderLines ) ci_menu[k].Type ='T';
					else if ( k == CIHeaderLines ) ci_menu[k].Type='B';
					else ci_menu[k].Type='S';
				};
				i++;
				k++;
			};
			
			if ( CI_Menu_Slot == 0 ) {
				PopUpCam=ci_slot+1;
				PostMessage(hWnd,WM_COMMAND,IDM_CI_MESSAGEPOPUP,0);
			};
			break;
			
		default:
			ci_interface_LastCmdLen=len;
			if ( ci_interface_LastCmdLen > 128 ) ci_interface_LastCmdLen=128;
			memcpy(ci_interface_LastCmd,data,ci_interface_LastCmdLen);
			break;
        }
}



void Debi_Interrupt(SAA7146_ISR_IO *saaisr){
	short len;
	char Text[64];
	
    if ( DEBICOPY == FALSE ) {
		ARM_ClearIrq();
		ARM_ClearMailBox();
		return;
	}

	switch (saaisr->debiTyp&0xff) {
		
	case DATA_TS_RECORD:
	case DATA_PES_RECORD:
		strcpy(LastIntText,"SAA7146 Debi_Interrupt <DATA_TS_RECORD/DATA_TS_RECORD> Start");
		if ( Irq_Record(Record_mem_dma.pUserAddr,saaisr->debilen) < 0 ) {
			ARM_CLEAR_RECEIVE();
			Debi_Record_Stop();
		} else ARM_CLEAR_RECEIVE();
   		strcat(LastIntText," -> Return done");
		return;
		
		
	case DATA_MPEG_PLAY:
		strcpy(LastIntText,"SAA7146 Debi_Interrupt <DATA_MPEG_PLAY> Start");
		if (( Play == FALSE ) || ( Recorder_Pause == TRUE )) {
			ARM_SET_SEND(0);
			ARM_ClearMailBox();
    		strcat(LastIntText," -> Return(1)");
			return;
		}
		len = Irq_Play((saaisr->debiTyp>>8)&0x03);
		if ( len == 0 ) {
			ARM_SET_SEND(0);
			ARM_ClearMailBox();
			Debi_Record_Stop();
			Create_Osd_Thread((LPTHREAD_START_ROUTINE)OSD_Play_Stop);
			sprintf(ChannelString,"%d",Make_ProgNr(CurrentProgramm)+1);
			SetTimer(hWnd,99,1000,NULL);
    		strcat(LastIntText," -> Return(2)");
			return;
		} else if ( len < 0 ) {
			if ( Next_Play_File() == FALSE ) {
				ARM_SET_SEND(0);
				ARM_ClearMailBox();
				Debi_Record_Pause(); 
				PostMessage(hWnd,WM_COMMAND,IDM_INTERRUPT,0);
    		    strcat(LastIntText," -> Return(3)");
				return;
			};
			len = Irq_Play((saaisr->debiTyp>>8)&0x03);
		};
		ARM_PLAY_BUFFER(len);
   		strcat(LastIntText," -> Return done");
		return;
		
	case DATA_BMP_LOAD:
		strcpy(LastIntText,"SAA7146 Debi_Interrupt <DATA_BMP_LOAD> Start");
		if ( saaisr->debilen == 0 ) {
			SetEvent(OSDBitMap.LoadBitMapEvent);
		} else  if ( saaisr->debilen == 0xffffffff ) {
      		    sprintf(Text,"SAA7146 Debi_Interrupt DATA_BMP_LOAD FAIL ");
                Write_Log(Text);
		}
   		strcat(LastIntText," -> Return done");
		return;
		
	case DATA_IPMPE:
		// IPMPE_mem_dma.pUserAddr gefüllt ;
		strcpy(LastIntText,"SAA7146 Debi_Interrupt <DATA_IPMPE> Start");
        ARM_CLEAR_RECEIVE();
  		strcat(LastIntText," -> Return done");
		return;
		
	case DATA_NONE:
		strcpy(LastIntText,"SAA7146 Debi_Interrupt <DATA_NONE> Start");
        ARM_CLEAR_RECEIVE();
  		strcat(LastIntText," -> Return done");
		return;
		
	case DATA_CI_PUT:
		// CA_mem_dma.pUserAddr gefüllt ;
		strcpy(LastIntText,"SAA7146 Debi_Interrupt <DATA_CI_PUT> Start");
		if ( CA_Len > 0 ) {
			ARM_CA_SEND(CA_Len);
			CA_Len=0; 
		} else {
			ARM_SET_SEND(0);
			ARM_ClearMailBox();
		}
  		strcat(LastIntText," -> Return done");
		return;
		
	case DATA_CI_GET:
		strcpy(LastIntText,"SAA7146 Debi_Interrupt <DATA_CI_GET> Start");
		CA_Len=(unsigned short)saaisr->debilen;
        ARM_CLEAR_RECEIVE();
  		strcat(LastIntText," -> Return done");
		return;
		
	case DATA_COMMON_INTERFACE:
		strcpy(LastIntText,"SAA7146 Debi_Interrupt <DATA_COMMON_INTERFACE> Start");
		CI_handle(CI_mem_dma.pUserAddr,saaisr->debilen);
        ARM_CLEAR_RECEIVE();
  		strcat(LastIntText," -> Return done");
		return;
		
	case DATA_PIPING:
	case DATA_FSECTION:
		strcpy(LastIntText,"SAA7146 Debi_Interrupt <DATA_PIPING/DATA_FSECTION> Start");
		if ( Irq_Filters(saaisr->debiTyp,saaisr->debilen) == 0 ) {
        ARM_CLEAR_RECEIVE();
		};
  		strcat(LastIntText," -> Return done");
		return;
		
	case DATA_DEBUG_MESSAGE:
		strcpy(LastIntText,"SAA7146 Debi_Interrupt <DATA_DEBUG_MESSAGE> Start");
        ARM_CLEAR_RECEIVE();
  		strcat(LastIntText," -> Return done");
		return;
		
	case DATA_STREAMING:
		strcpy(LastIntText,"SAA7146 Debi_Interrupt <DATA_STREAMING> Start");
        ARM_CLEAR_RECEIVE();
  		strcat(LastIntText," -> Return done");
		return;
		
	case DATA_IRCOMMAND:
		strcpy(LastIntText,"SAA7146 Debi_Interrupt <DATA_IRCOMMAND> Start");
		Irq_Ir_Control(saaisr->debilen);
        ARM_CLEAR_RECEIVE();
  		strcat(LastIntText," -> Return done");
		return;
		
	default:
		strcpy(LastIntText,"SAA7146 Debi_Interrupt <default> Start");
		if ( WRITE_LOG == TRUE ) {
			sprintf(Text,"SAA7146 Debi_Interrupt UNKNOWM_MESSAGE %08X Laenge %d ",saaisr->debiTyp,saaisr->debilen);
			Write_Log(Text);
		};
        ARM_CLEAR_RECEIVE();
  		strcat(LastIntText," -> Return done");
		return;
        }      
}

void ARM_ResetMailBox(){
	WD_KERNEL_PLUGIN_CALL kpCall;
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_ARM_RESETMAILBOX;
    kpCall.pData = NULL;
    // this will call KP_Call() callback in the kernel
    WD_KernelPlugInCall(hSAA7146->hWD, &kpCall);
}


void ARM_ClearMailBox(){
	WD_KERNEL_PLUGIN_CALL kpCall;
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_ARM_CLEARMAILBOX;
    kpCall.pData = NULL;
    // this will call KP_Call() callback in the kernel
    WD_KernelPlugInCall(hSAA7146->hWD, &kpCall);
}



void ARM_ClearIrq(){
    WD_KERNEL_PLUGIN_CALL kpCall;
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_ARM_CLEARIRQ;
    kpCall.pData = NULL;
    // this will call KP_Call() callback in the kernel
    WD_KernelPlugInCall(hSAA7146->hWD, &kpCall);
}

void ARM_SET_IRQ_STATE_EXT(unsigned short value){
    WD_KERNEL_PLUGIN_CALL kpCall;
	SAA7146_SHORTSEND_IO IOCommand;
    int i;
	IOCommand.value=value;
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_ARM_SET_IRQ_STATE_EXT;
    kpCall.pData = &IOCommand;
    // this will call KP_Call() callback in the kernel
    i=WD_KernelPlugInCall(hSAA7146->hWD, &kpCall);
	return ;
}

void ARM_SET_SEND(unsigned short value){
    WD_KERNEL_PLUGIN_CALL kpCall;
	SAA7146_SHORTSEND_IO IOCommand;
    int i;
	IOCommand.value=value;
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_ARM_SET_SEND;
    kpCall.pData = &IOCommand;
    // this will call KP_Call() callback in the kernel
    i=WD_KernelPlugInCall(hSAA7146->hWD, &kpCall);
	return ;
}

void ARM_CLEAR_RECEIVE( void ){
    WD_KERNEL_PLUGIN_CALL kpCall;
    int i;
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_CLEAR_RECEIVE;
    kpCall.pData = NULL;
    // this will call KP_Call() callback in the kernel
    i=WD_KernelPlugInCall(hSAA7146->hWD, &kpCall);
	return ;
}


void ARM_SET_RX_BUFF(unsigned short value){
    WD_KERNEL_PLUGIN_CALL kpCall;
	SAA7146_SHORTSEND_IO IOCommand;
    int i;
	IOCommand.value=value;
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_ARM_SET_RECEIVE;
    kpCall.pData = &IOCommand;
    // this will call KP_Call() callback in the kernel
    i=WD_KernelPlugInCall(hSAA7146->hWD, &kpCall);
	return ;
}

void ARM_CA_SEND(unsigned int value){
    WD_KERNEL_PLUGIN_CALL kpCall;
	SAA7146_SHORTSEND_IO IOCommand;
    int i;
	IOCommand.value=value;
	IOCommand.Addresse = 0;
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_SEND_CA;
    kpCall.pData = &IOCommand;
    // this will call KP_Call() callback in the kernel
    i=WD_KernelPlugInCall(hSAA7146->hWD, &kpCall);
	return ;
}

void ARM_PLAY_BUFFER(unsigned int value){
    WD_KERNEL_PLUGIN_CALL kpCall;
	SAA7146_SHORTSEND_IO IOCommand;
    int i;
	IOCommand.value=value;
	IOCommand.Addresse = 0;
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_PLAY_BUFFER;
    kpCall.pData = &IOCommand;
    // this will call KP_Call() callback in the kernel
    i=WD_KernelPlugInCall(hSAA7146->hWD, &kpCall);
	return ;
}


void ARM_LOAD_BITMAP(unsigned short value, DWORD addr){
    WD_KERNEL_PLUGIN_CALL kpCall;
	SAA7146_SHORTSEND_IO IOCommand;
    int i;
	IOCommand.value=value;
	IOCommand.Addresse = addr;
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_LOAD_BITMAP;
    kpCall.pData = &IOCommand;
    // this will call KP_Call() callback in the kernel
    i=WD_KernelPlugInCall(hSAA7146->hWD, &kpCall);
	return ;
}



void SAA7146_KP_GET_DRIVER_VERSION(HANDLE hWD, DWORD hKernelPlugIn, char *buf){
    WD_KERNEL_PLUGIN_CALL kpCall;
    SAA7146_VERSION mdsaaVer;
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_VERSION;
    kpCall.pData = &mdsaaVer;
    // this will call KP_Call() callback in the kernel
    WD_KernelPlugInCall(hWD, &kpCall);
    strcpy(buf,mdsaaVer.cVer);
}


void SAA7146_KP_SET_INIT(HANDLE hWD, DWORD hKernelPlugIn){
    WD_KERNEL_PLUGIN_CALL kpCall;
    SAA7146_SET_IO Set_saa;
    char Zeile[256];
	
    Set_saa.debimem=(DWORD)debi_mem_dma.Page[0].pPhysicalAddr;
    Set_saa.recordmem=(DWORD)Record_mem_dma.Page[0].pPhysicalAddr;
    Set_saa.camem=(DWORD)CA_mem_dma.Page[0].pPhysicalAddr;
    Set_saa.cimem=(DWORD)CI_mem_dma.Page[0].pPhysicalAddr;
    Set_saa.ipmpemem=(DWORD)IPMPE_mem_dma.Page[0].pPhysicalAddr;
    Set_saa.bitmapmem=(DWORD)BitMap_mem_dma.Page[0].pPhysicalAddr;
    Set_saa.irmem=(DWORD)Ir_mem_dma.Page[0].pPhysicalAddr;
    Set_saa.debugmem=(DWORD)Debug_mem_dma.Page[0].pPhysicalAddr;
	Set_saa.ComAddress=hSAA7146->cardReg.Card.Item[hSAA7146->addrDesc[SAA7146_AD_BAR0].index].I.Mem.dwTransAddr;
	if ( DVB_IRQ_SHARING == FALSE ) Set_saa.Sharing=0;
	   else Set_saa.Sharing=1;
	   Write_Log("****     SAA7146 Treiber - IO-Memory    *****");
	   sprintf(Zeile,"DVB-Karte           = 0x%08X ",Set_saa.ComAddress);
	   Write_Log(Zeile);
	   sprintf(Zeile,"Debi-Transfer       = 0x%08X ",Set_saa.debimem);
	   Write_Log(Zeile);
	   sprintf(Zeile,"Record_Play         = 0x%08X ",Set_saa.recordmem);
	   Write_Log(Zeile);
	   sprintf(Zeile,"CA                  = 0x%08X ",Set_saa.camem);
	   Write_Log(Zeile);
	   sprintf(Zeile,"CI                  = 0x%08X ",Set_saa.cimem);
	   Write_Log(Zeile);
	   sprintf(Zeile,"IPME                = 0x%08X ",Set_saa.ipmpemem);
	   Write_Log(Zeile);
	   sprintf(Zeile,"BitMap              = 0x%08X ",Set_saa.bitmapmem);
	   Write_Log(Zeile);
	   sprintf(Zeile,"Fernbedienung       = 0x%08X ",Set_saa.irmem);
	   Write_Log(Zeile);
	   sprintf(Zeile,"Debug               = 0x%08X ",Set_saa.debugmem);
	   Write_Log(Zeile);
	   if ( DVB_IRQ_SHARING == FALSE  ) sprintf(Zeile,"IRQ-Sharing         = 0x%08X (FALSE)",Set_saa.Sharing);
	   else sprintf(Zeile,"IRQ-Sharing         = 0x%08X (TRUE)",Set_saa.Sharing);
	   Write_Log(Zeile);
	   
	   BZERO (kpCall);
	   kpCall.hKernelPlugIn = hKernelPlugIn;
	   kpCall.dwMessage = SAA7146_IOCTL_SET_INIT;
	   kpCall.pData = &Set_saa;
	   // this will call KP_Call() callback in the kernel
	   WD_KernelPlugInCall(hWD, &kpCall);
	   BZERO (kpCall);
}

void SAA7146_KP_SET_TIMEING(HANDLE hWD, DWORD hKernelPlugIn)
{
    WD_KERNEL_PLUGIN_CALL kpCall;
    SAA7146_TIMING_IO SAA_Timing;
    char Text[128];
	
	SAA_Timing.Cmd_Time_Out = Timing.Debi_Cmd_Time_Out;
	SAA_Timing.OSD_Text_Time_Out = Timing.OSD_Text_Time_Out;
	SAA_Timing.Debi_done_MC2 = Timing.Debi_done_MC2;
	SAA_Timing.Debi_done_PSR = Timing.Debi_done_PSR;
	
	Write_Log("****   SAA7146 Timeing - Einstellungen  *****");
	sprintf(Text,"WriteRegDelay       = %d ",Timing.WriteRegDelay);
	Write_Log(Text);
	sprintf(Text,"I2C_Bus_Timeout     = %d ",Timing.I2C_Bus_Timeout);
	Write_Log(Text);
	sprintf(Text,"I2C_Bus_Reset_Delay = %d ",Timing.I2C_Bus_Reset_Delay);
	Write_Log(Text);
	sprintf(Text,"DVB_Reset_Wait      = %d ",Timing.DVB_Reset_Wait);
	Write_Log(Text);
	sprintf(Text,"DVB_Reset_Scan_Wait = %d ",Timing.DVB_Reset_Scan_Wait);
	Write_Log(Text);
	sprintf(Text,"SendDiSEqCDelay     = %d ",Timing.SendDiSEqCDelay);
	Write_Log(Text);
	sprintf(Text,"Pmt_TimeOut         = %d ",Timing.Pmt_TimeOut);
	Write_Log(Text);
	sprintf(Text,"Sdt_TimeOut         = %d ",Timing.Sdt_TimeOut);
	Write_Log(Text);
	sprintf(Text,"Pat_TimeOut         = %d ",Timing.Pat_TimeOut);
	Write_Log(Text);
	sprintf(Text,"Cat_TimeOut         = %d ",Timing.Cat_TimeOut);
	Write_Log(Text);
	sprintf(Text,"Nit_TimeOut         = %d ",Timing.Nit_TimeOut);
	Write_Log(Text);
	sprintf(Text,"Debi_Cmd_Time_Out   = %d ",Timing.Debi_Cmd_Time_Out);
	Write_Log(Text);
	sprintf(Text,"OSD_Text_Time_Out   = %d ",Timing.OSD_Text_Time_Out);
	Write_Log(Text);
	sprintf(Text,"Debi_done_MC2       = %d ",Timing.Debi_done_MC2);
	Write_Log(Text);
	sprintf(Text,"Debi_done_PSR       = %d ",Timing.Debi_done_PSR);
	Write_Log(Text);
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_SET_TIMEING;
    kpCall.pData = &SAA_Timing;
    // this will call KP_Call() callback in the kernel
    WD_KernelPlugInCall(hWD, &kpCall);
    BZERO (kpCall);
}


void SAA7146_KP_CLEAR_INIT(HANDLE hWD, DWORD hKernelPlugIn){
    WD_KERNEL_PLUGIN_CALL kpCall;
    SAA7146_SET_IO Set_saa;
	
	Set_saa.ComAddress=(DWORD)0;
    Set_saa.debimem=(DWORD)0;
    Set_saa.recordmem=(DWORD)0;
    Set_saa.camem=(DWORD)0;
    Set_saa.cimem=(DWORD)0;
    Set_saa.ipmpemem=(DWORD)0;
    Set_saa.bitmapmem=(DWORD)0;
    Set_saa.irmem=(DWORD)0;
    Set_saa.debugmem=(DWORD)0;
	
	down_interruptible();
    Sleep(50);
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_SET_INIT;
    kpCall.pData = &Set_saa;
    // this will call KP_Call() callback in the kernel
    WD_KernelPlugInCall(hWD, &kpCall);
}



void SAA7146_KP_GET_IRQ(HANDLE hWD, DWORD hKernelPlugIn, SAA7146_ISR_IO *saaisr){
    WD_KERNEL_PLUGIN_CALL kpCall;
	
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_GET_ISR;
    kpCall.pData = saaisr;
    // this will call KP_Call() callback in the kernel
    WD_KernelPlugInCall(hWD, &kpCall);
	
}



BOOL SAA7146_IntEnable (SAA7146_HANDLE hSAA7146, SAA7146_INT_HANDLER funcIntHandler){
    ULONG threadId;
    BOOL DVBDriver=FALSE;
	int i;
	char LogZeile[128];
    char IsVersion[256];
    char MustBe[256];
	
    if (!hSAA7146->fUseInt) return FALSE;
    // check if interrupt is already enabled
    if (hSAA7146->Int.hThread) return FALSE;
    strcpy(MustBe,"SAA7146-Kernel-Driver V2.70");
    i=0;
    while ( i < 2 ) {
		BZERO (DVBKernelPlugIn);
		DVBKernelPlugIn.pcDriverName = "MDSAA"; // this will search for MDSAA.VXD or MDSAA.SYS
		// this will call KP_Open() callback in the kernel
		WD_KernelPlugInOpen(hSAA7146->hWD, &DVBKernelPlugIn);            
		if (DVBKernelPlugIn.hKernelPlugIn) {
			SAA7146_KP_GET_DRIVER_VERSION(hSAA7146->hWD, DVBKernelPlugIn.hKernelPlugIn, &IsVersion[0]);
			if ((i==0) && ( strcmp(IsVersion,MustBe) != 0 )) {
				sprintf(LogZeile,"SAA7146 Treibererweiterung für SAA -> Falsche Version ist %s soll %s",IsVersion,MustBe);
				Write_Log(LogZeile);
				sprintf(LogZeile,"SAA7146 Versuche Reload");
				Write_Log(LogZeile);
				WD_KernelPlugInClose(hSAA7146->hWD, &DVBKernelPlugIn );    
				Reload_SAA_Driver();
				i++;
				continue;
			}
			if ((i==1) && ( strcmp(IsVersion,MustBe) != 0 )) {
				WD_KernelPlugInClose(hSAA7146->hWD, &DVBKernelPlugIn );    
				MessageBox(hWnd,"MultiDec-SAA-Treiber MDSAA : Falsche Version !\n\nMögliche Lösung\n\n - Als Administrator anmelden\n - \\\\HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\MultiDec-SAA Treiber löschen\n - Neu Booten","Multi-Decoder",MB_ICONSTOP|MB_OK); 
				sprintf(LogZeile,"SAA7146 Treibererweiterung für SAA -> immer noch falsche Version ist %s soll %s",IsVersion,MustBe);
				Write_Log(LogZeile);
				sprintf(LogZeile,"SAA7146 Abbruch mit False");
				Write_Log(LogZeile);
				WD_KernelPlugInClose(hSAA7146->hWD, &DVBKernelPlugIn );    
				return(FALSE);
			};
			
			i=2;
			sprintf(LogZeile,"SAA7146 Treibererweiterung für DVB ist OK (Version %s)",IsVersion);
			
			SAA7146_KP_SET_TIMEING(hSAA7146->hWD, DVBKernelPlugIn.hKernelPlugIn);
			SAA7146_KP_SET_INIT(hSAA7146->hWD, DVBKernelPlugIn.hKernelPlugIn);
			
			hSAA7146->Int.Int.dwCmds = 0;
			
			hSAA7146->Int.Int.kpCall.hKernelPlugIn = DVBKernelPlugIn.hKernelPlugIn;
			WD_IntEnable (hSAA7146->hWD, &hSAA7146->Int.Int);
			// check if WD_IntEnable failed
			if (hSAA7146->Int.Int.fEnableOk) {
				DVBDriver=TRUE;   
				// create interrupt handler thread
				hSAA7146->Int.hThread = CreateThread (0, 0x1000, SAA7146_IntThread, hSAA7146, 0, &threadId);
			};
		} else {
			sprintf(LogZeile,"SAA7146 Treibererweiterung MDSAA nicht vorhanden/bereit");
			Reload_SAA_Driver();
			i++; 
		};
		Write_Log(LogZeile);
	};
	
	if ( DVBDriver == FALSE ) return(FALSE);
	return(TRUE);
}



void SAA7146_IntDisable (SAA7146_HANDLE hSAA7146){
	Run_Debi_Watch=FALSE;
    if (!hSAA7146->fUseInt) return;
    if (!hSAA7146->Int.hThread) return;
    WD_IntDisable (hSAA7146->hWD, &hSAA7146->Int.Int);
    WaitForSingleObject (hSAA7146->Int.hThread, INFINITE);
    hSAA7146->Int.hThread = NULL;
}

int SAA7146_FirmRead(LPSTR FileName, unsigned char *Daten, int Bytes){
	FILE *sFile;
    int fd;
	
	unsigned long	FileSize;
	
	fd=_open(FileName,_O_RDONLY | _O_BINARY);
	if ( fd == -1 ) return(FALSE); 
	FileSize=_filelength(fd);
    _close(fd);
    if ( Bytes == 0 ) return(FileSize);
	if ( FileSize > (unsigned)Bytes ) return(-1);
	
	sFile = fopen(FileName, "rb");
	if(sFile == NULL) {
		return (-1);
	}
	
	if ( fread(Daten, sizeof(unsigned char), FileSize, sFile) != (size_t)FileSize )  {
		fclose(sFile);
		return (-1);
	}
	fclose(sFile);
	return(FileSize);
};

DWORD cpu_to_be32(DWORD value){
	return ((value << 24)
		| ((value << 8) & 0x00FF0000)
		| ((value >> 8) & 0x0000FF00)
		| (value >> 24));
}


int SAA7146_Load_Dram( unsigned char *data, int len){
	char Text[128];
	int i;
	short blocks, rest;
	DWORD base;
	DWORD bootblock=BOOT_BLOCK;
	
	blocks=len/BOOT_MAX_SIZE;
	rest=len % BOOT_MAX_SIZE;
	base=DRAM_START_CODE;
	
	for (i=0; i<blocks; i++)
	{
		sprintf(Text,t("Bytes %d"),i*BOOT_MAX_SIZE);
		SetDlgItemText(SplashWnd,IDC_TEXT4,Text);
		if (SAA7146_WaitDebi( BOOT_STATE, BOOTSTATE_BUFFER_EMPTY) < 0) return -1;
		SAA7146_WDebi(DEBISWAB, bootblock, (DWORD)data+(i*(BOOT_MAX_SIZE)),BOOT_MAX_SIZE);
		bootblock^=0x1400;
		SAA7146_WDebi(DEBISWAB, BOOT_BASE, cpu_to_be32(base), 4);
		SAA7146_WDebi(DEBINOSWAP, BOOT_SIZE, BOOT_MAX_SIZE, 2);
		SAA7146_WDebi(DEBINOSWAP, BOOT_STATE, BOOTSTATE_BUFFER_FULL, 2);
		base+=BOOT_MAX_SIZE;
	}
	
	sprintf(Text,t("Bytes %d"),(blocks*BOOT_MAX_SIZE)+rest);
	SetDlgItemText(SplashWnd,IDC_TEXT4,Text);
	
	if (rest > 0){
		if (SAA7146_WaitDebi( BOOT_STATE, BOOTSTATE_BUFFER_EMPTY) < 0) return -1;
		if (rest>4) SAA7146_WDebi(DEBISWAB, BOOT_BLOCK,(DWORD)data+(i*(BOOT_MAX_SIZE)), rest);
		else SAA7146_WDebi(DEBISWAB, BOOT_BLOCK, (DWORD)(i*(BOOT_MAX_SIZE)-4+data), rest+4);
		
		SAA7146_WDebi(DEBISWAB, BOOT_BASE, cpu_to_be32(base), 4);
		SAA7146_WDebi(DEBINOSWAP, BOOT_SIZE, rest, 2);
		SAA7146_WDebi(DEBINOSWAP, BOOT_STATE, BOOTSTATE_BUFFER_FULL, 2);
	}
	if (SAA7146_WaitDebi(BOOT_STATE, BOOTSTATE_BUFFER_EMPTY) < 0) return -1;
	SAA7146_WDebi(DEBINOSWAP, BOOT_SIZE, 0, 2);
	SAA7146_WDebi(DEBINOSWAP, BOOT_STATE, BOOTSTATE_BUFFER_FULL, 2);
	if (SAA7146_WaitDebi( BOOT_STATE, BOOTSTATE_BOOT_COMPLETE) < 0) return -1;
	return 0;
}


int SAA7146_WaitDebi( int adr, int state){
	int i,k;
	Sleep(0);
	Sleep(1);
	Sleep(0);
	for (k=0; k<256; k++ ) {
		i=SAA7146_RDebi(DEBINOSWAP, adr, 0, 2);
		if ( i == state ) return 0;
		Sleep(1);
	};
	if ( i == 0 ) return(0);
	return -1;
}


int SAA7146_FirmVersion(){
	char Text[128];
	unsigned short Buffer[18];
	unsigned int av711X;
	unsigned int firm;
	unsigned int rtsl;
	unsigned int vid;
	unsigned int app;

	unsigned short tag = ((COMTYPE_REQUEST << 8) + ReqVersion);
	
	SAA7146_RequestParameter(tag, Buffer, 16);

	firm  =(Buffer[0] << 16) + Buffer[1];
	rtsl  =(Buffer[2] << 16) + Buffer[3];
	vid   =(Buffer[4] << 16) + Buffer[5];
	app   =(Buffer[6] << 16) + Buffer[7];
	av711X=(Buffer[8] << 16) + Buffer[9];

	sprintf(Text,"AV711%d - firm %08x, rtsl %08x, vid %08x, app %08x",av711X,firm,rtsl,vid,app);
	Write_Log(Text);
	return ((Buffer[0] << 16) + Buffer[1]);
}


int SendCICommand(unsigned char subcom, unsigned char *Params, unsigned char ParamLen){
	int i;
	unsigned short CommandBuffer[18] = { ((COMTYPE_COMMON_IF << 8) + subcom),
		16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for(i=0; (i<ParamLen)&&(i<32); i++){
		if(i%2 == 0) CommandBuffer[(i/2)+2] = (unsigned	short)(Params[i]) << 8;
		else CommandBuffer[(i/2)+2] |= Params[i];
	}
	return(SAA7146_OutCommand( CommandBuffer, 18));
}


int SAA7146_RequestParameter(unsigned short tag, short *Buff, short length){
	return SAA7146_CommandRequest(&tag, 1, Buff, length);
};


void SAA7146_Restart_Arm( void ){
	int i;
	
	Write_Log("SAA7146 SAA7146_Restart_Arm");
	
	SAA7146_SetGPIO(RESET_LINE, GPIO_OUTLO);
	
	/* Disable DEBI and GPIO irq */
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, IER, SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, IER) & ~(MASK_19 | MASK_03));
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, ISR, (MASK_19 | MASK_03));
	SAA7146_SetGPIO(RESET_LINE, GPIO_OUTHI);
	Debi_Irq=0;
	Sleep(800);
	Debi_Irq=0;
	ARM_ResetMailBox(); 
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, ISR, (MASK_19 | MASK_03));
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, IER, SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, IER) | MASK_03 );
	Debi_Irq=0;
	Sleep(100);
	if ( VES_TYPE == 0 ) i=VES1820_Init(FALSE); 
			 else if ( VES_TYPE == 1 ) i=VES1893_Init(FALSE);
			 else if ( VES_TYPE == 2 ) i=STV0299_Init(FALSE); 
			 else if ( VES_TYPE == 3 ) i=TDA8083_Init(FALSE); 
			 Write_Log("SAA7146 SAA7146_Restart_Arm Done");
	Debi_Irq=0;
};


BOOL Sync_KP_Call(WD_KERNEL_PLUGIN_CALL *kpCall,int Retry){
	int i;
	int j=0;
	int k=0;
	char Text[128];
	
	down_interruptible();
	

	for ( i=0; i < Retry ; i++ ) {
		WD_KernelPlugInCall(hSAA7146->hWD, kpCall);
		if ( kpCall->dwResult < 0xf0 ) {
            if ( kpCall->dwResult == 1 ) {
				sprintf(Text,"SAA7146 Sync_KP_Call Debi_Command nicht innerhalb vom TimeOut ausgeführt ");
				Write_Log(Text);
			} else if ( kpCall->dwResult == 2 ) {
				sprintf(Text,"SAA7146 Sync_KP_Call TextPuffer nicht innerhalb vom TimeOut geleehrt ");
				Write_Log(Text);
			} else if ( kpCall->dwResult == 3) {
				sprintf(Text,"SAA7146 Sync_KP_Call Debi_Command und TextPuffer nicht innerhalb vom TimeOut geleehrt ");
				Write_Log(Text);
			};
			up_interruptible();
			return(TRUE);
		};
		
		if ( k > 20 ) {
			if ( j < 3 ) j++;
			k=0;
		};
		k++;
		Sleep(j);
	Debi_Irq=0;
	};
	sprintf(Text,"SAA7146 Sync_KP_Call TimeOut LastReturn %d ",kpCall->dwResult );
	Write_Log(Text);
	up_interruptible();
	return(FALSE);
};

int SAA7146_CommandRequest(unsigned short *Buff, int length, unsigned short *buf, int n){
    WD_KERNEL_PLUGIN_CALL kpCall;
	SAA7146_COMMANDREQUEST_IO IOCommandRequest;
	
	IOCommandRequest.OutLength=length;
	memcpy(IOCommandRequest.OutBuf,Buff,IOCommandRequest.OutLength*2);
	IOCommandRequest.InLength=n;
	memcpy(IOCommandRequest.InBuf,buf,IOCommandRequest.InLength*2);
	
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_COMMANDREQUEST;
    kpCall.pData = &IOCommandRequest;
    // this will call KP_Call() callback in the kernel
	
    if ( Sync_KP_Call(&kpCall,100) == FALSE ) {
		// Nichts abgesetst ( Arm im Eimer ??)
        return(0);
	};
	memcpy(&buf[0],&IOCommandRequest.InBuf,n*2);
	return DVB_ERR_NONE;
}


void down_interruptible( void ){
	WD_KERNEL_PLUGIN_CALL kpCall;
	BZERO (kpCall);
	kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
	kpCall.dwMessage = SAA7146_IOCTL_SET_SPIN_LOCK;
	WD_KernelPlugInCall(hSAA7146->hWD, &kpCall);
	return;
}


void up_interruptible( void ){
	WD_KERNEL_PLUGIN_CALL kpCall;
	BZERO (kpCall);
	kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
	kpCall.dwMessage = SAA7146_IOCTL_RELEASE_SPIN_LOCK;
	WD_KernelPlugInCall(hSAA7146->hWD, &kpCall);
};


void WriteText(unsigned char win, unsigned short x, unsigned short y, unsigned char *buf){
    WD_KERNEL_PLUGIN_CALL kpCall;
	SAA7146_WRITETEXT_IO IOTextOut;
    int i;
	int length;
    unsigned char TextBuffer[256];
    unsigned short cbuf[5];
	
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"WriteText(w=%d, x=%d, y=%d, t='%s')",win, x, y, buf);
	Write_Log(Text);
#endif

	length=strlen(buf)+1;
	if ( length == 1 ) return;
	if (length > 256) length = 256;
	
	cbuf[0]=(COMTYPE_OSD<<8) + DText;
	cbuf[1]= 3;
	cbuf[2]= win+1;
	cbuf[3]= x;
	cbuf[4]= y;
	
	for (i=0; i<length-1; i++) TextBuffer[i^1]=iso88591_to_osdfont[buf[i]];
	for (; i<256; i++) TextBuffer[i^1]=0x00;

	IOTextOut.CommandLength=5; 
	memcpy(IOTextOut.CommandBuffer,cbuf,10);
	IOTextOut.TextLength=(length+3)>>2;
	memcpy(IOTextOut.TextBuffer,TextBuffer,IOTextOut.TextLength<<2);

    BZERO (kpCall);
    kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_WRITETEXT;
    kpCall.pData = &IOTextOut;
    if ( Sync_KP_Call(&kpCall,100) == FALSE ) {
		return;
	}
	return;
}

int SAA7146_OutCommand( unsigned short *buf, short length){
    WD_KERNEL_PLUGIN_CALL kpCall;
	SAA7146_COMMAND_IO IOCommand;
	IOCommand.CommandLength=length;
	memcpy(IOCommand.CommandBuffer,buf,IOCommand.CommandLength*2);
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hSAA7146->Int.Int.kpCall.hKernelPlugIn;
    kpCall.dwMessage = SAA7146_IOCTL_COMMAND;
    kpCall.pData = &IOCommand;
    // this will call KP_Call() callback in the kernel
    if ( Sync_KP_Call(&kpCall,100) == FALSE ) {
		// Nichts abgesetst ( Arm im Eimer ??)
		return(0);
	};
	return DVB_ERR_NONE;
};


int SAA7146_OutCom(int type, int com, int num, ...){
	va_list args;
	unsigned short buf[32];
	int i;
	int ret;
	
	memset(&buf[0],0x00,64);
	buf[0]=(( type << 8 ) + com); 
	buf[1]=num;
	
	if (num) {
		va_start(args, num);
		for (i=0; i<num; i++)
			buf[i+2]=va_arg(args, unsigned short);
		va_end(args);
	}
	ret=SAA7146_OutCommand(buf, (short)(num+2));
	return(ret);
}



int VES1893_Init( BOOL MiniMize ){
	unsigned char i;
	char Text[128];
	
	int RetValue0;
	int RetValue1;
	
	int Init1893WTab[55] = {
        1,1,1,1,1,1,1,1, 1,1,0,0,1,1,0,0,
			0,1,0,0,0,0,0,0, 1,0,1,1,0,0,0,1,
			1,1,1,0,0,0,0,0, 0,0,1,1,0,0,0,0,
			1,1,1,0,1,1
	};
	
	ves_addr=(0x10 >> 1);  
	ves_flags=0;
	
	if ( MiniMize == FALSE ) {
        RetValue0=i2c_master_send(ves_addr,ves_flags,NULL,0);
        RetValue1=(readreg(ves_addr,ves_flags,0x1e)&0xf0);
		sprintf(Text,"SAA7146 VES1893_Init : RetValue0 0x%02x  ( soll 0x00 ) RetValue1 0x%02x ( soll 0xd0 )",RetValue0,RetValue1);
		Write_Log(Text);
		
        if (RetValue0 != 0 ) return -1;
        if (RetValue1 != 0xd0)  return -1;
		
		sprintf(Text,"SAA7146 VES1893_Init : Module gefunden");
		Write_Log(Text);
		
		//        printk("VES1893: init chip\n");
        ves_addr=(0x10 >> 1);  
		
		for (i=0; i<54; i++) {
			if ( Init1893WTab[i] == 1 ) writereg(ves_addr,ves_flags, i, Init_VES_1893_Tab[i]);
			//			writereg(ves_addr,ves_flags, i, Init1893Tab[i]);
		};
		
	};
	
	ves_inversion=0;
	ves_fec=9;
	ves_ctr= Init_VES_1893_Tab[0x1f];
	
	SendDAC(2, 1);
	Sleep(50);
	SendDAC(2, 0);
	Sleep(50);
	SendDAC(0, 0x44);
	SendDAC(1, 0x81);
	SendDAC(2, 0xf0);
				DVB_SetVolume((unsigned char)0x00, (unsigned char)0x00);
                SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,(MASK_09 | MASK_25 | MASK_10 | MASK_26));
				
				return 0;
}



int STV0299_Init(BOOL MiniMize){
	unsigned char i;
	char Text[128];
	
	int RetValue0;
	int RetValue1;
	
	ves_addr=(0xd0 >> 1);  
	ves_flags=0;
	
	
	if ( MiniMize == FALSE ) {
        RetValue0=i2c_master_send(ves_addr,ves_flags,NULL,0);
        RetValue1=readreg(ves_addr,ves_flags,0x00);
		sprintf(Text,"SAA7146 STV0299_Init : RetValue0 0x%02x  ( soll 0x00 ) RetValue1 0x%02x ( soll 0xa1 )",RetValue0,RetValue1);
		Write_Log(Text);
		
        if (RetValue0 != 0 ) return -1;
        if (RetValue1 != 0xa1)  return -1;
		
		sprintf(Text,"SAA7146 STV0299_Init : Module gefunden");
		Write_Log(Text);
		
		
        for ( i=0; i < 154; i+=2 ) {
			writereg(ves_addr,ves_flags, Init_STV_0299_Tab[i], Init_STV_0299_Tab[i+1]);
		}
		
	};
	
	Sleep(10);
	writereg(ves_addr,ves_flags,  0x05, 0x80);
	
	ves_inversion=0;
	ves_fec=9;
	ves_ctr= 0;
	return 0;
}


int TDA8083_Init(BOOL MiniMize)
{
	char Text[128];
	
	int i;
	int RetValue0;
	int RetValue1;
	
	unsigned char Buffer[4] = { 0x2d , 0x58 ,0x8e ,0x00 };
	int Init8083WTab[42] = {
        1,1,1,1,1,1,0,1, 1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1, 1,1 };
		
        ves_addr=(0xd0 >> 1);  
        ves_flags=0;
        RetValue0=i2c_master_send(ves_addr,ves_flags,NULL,0);
        RetValue1=readreg(ves_addr,ves_flags,0x00);
	
		sprintf(Text,"SAA7146 TDA8083_Init : RetValue0 0x%02x  ( soll 0x00 ) RetValue1 0x%02x ( soll 0x05 )",RetValue0,RetValue1);
		Write_Log(Text);
        if (RetValue0 != 0 ) return -1;
        if (RetValue1 != 0x05)  return -1;
		sprintf(Text,"SAA7146 TDA8083_Init : Module gefunden");
		Write_Log(Text);
		
        if (i2c_master_send(0x60 ,(unsigned char)0,Buffer,4) == 4 ) {
			Write_Log("SAA7146 TDA8083 Tuner_Init OK");
        };
		for (i=0; i<42; i++) {
			if ( Init8083WTab[i] == 1 ) writereg(ves_addr,ves_flags, i, Init_TDA8083_Tab[i]);
		};
        Sleep(50);
		ClrBit8083();
		Sleep(50);
		writereg(ves_addr,ves_flags, 0x20, 0x11);
        Sleep(50);
		ClrBit8083();
		ves_inversion=0;
		ves_fec=9;
        ves_ctr= 0;
		return 0;
}


int VES1820_Init( BOOL MiniMize ){
	char Text[128];
	int RetValue0;
	int RetValue1;
	
	struct i2c_msg msgs[2];
	unsigned char mm1[] = {0xff};
	unsigned char mm2[] = {0x00};
	int i;
	unsigned char imsg[2];
	
	ves_addr=(0x12 >> 1);
	ves_flags=0;
	
	if ( MiniMize == FALSE ) {
        RetValue0=i2c_master_send(ves_addr,ves_flags,NULL,0);
        RetValue1=(readreg(ves_addr,ves_flags,0x1a)&0xf0);
		
		sprintf(Text,"SAA7146 VES1820_Init : RetValue0 0x%02x  ( soll 0x00 ) RetValue1 0x%02x ( soll 0x70 )",RetValue0,RetValue1);
		Write_Log(Text);
		
        if (RetValue0 != 0 ) return -1;
        if (RetValue1 != 0x70)  return -1;
		
		sprintf(Text,"SAA7146 VES1820_Init : Module gefunden");
		Write_Log(Text);
	};
	
	ves_addr=(0x12 >> 1);  
	msgs[0].flags=0;
	msgs[1].flags=1;
	msgs[0].addr=msgs[1].addr=(0x28<<1);
	mm1[0]=0xff;
	msgs[0].len=1; msgs[1].len=1;
	msgs[0].buf=mm1; msgs[1].buf=mm2;
	//        i2c_transfer( msgs, 2);
	master_xfer(msgs,2);
	ves_pwm=*mm2;
	
	sprintf(Text,"SAA7146 VES1820_Init : ves_pwm %02x ",ves_pwm);
	Write_Log(Text);
	
	if (ves_pwm == 0xff) ves_pwm=0x48;
	
	//        printk("VES1820: init chip\n");
	
	if ( MiniMize == FALSE ) {
        for (i=0; i<53; i++) writereg(ves_addr,ves_flags, i, Init_VES_1820_Tab[i]);
	}
	
	ves_inversion=0;
	ves_reg0=Init_VES_1820_Tab[0];
	
	writereg(ves_addr,ves_flags,0x34, ves_pwm);
	Sleep(50);
	
	imsg[0]=0x0f;
	imsg[1]=0x28;
	writereg(ves_addr,ves_flags, imsg[0], imsg[1]);
	Sleep(20);
	
	if ( i2c_writereg( 0x80, 0x0, 0x80) == 1 ) {
		sprintf(Text,"SAA7146 VES1820_Init DVB_C Analog-Modul erkannt ");
		Write_Log(Text);
        i2c_writereg( 0x80, 0x0, 0);
        Sleep(50);
		sprintf(Text,"SAA7146 VES1820_Init DVB_C Init MSP3400");
		Write_Log(Text);
		
        msp_writereg( 0x12, 0x0013, 0x0c00);
        msp_writereg( 0x12, 0x0000, 0x7f00); // loudspeaker + headphone
        msp_writereg( 0x12, 0x0008, 0x0220); // loudspeaker source
        msp_writereg( 0x12, 0x0004, 0x7f00); // loudspeaker volume
        msp_writereg( 0x12, 0x000a, 0x0220); // SCART 1 source
        msp_writereg( 0x12, 0x0007, 0x7f00); // SCART 1 volume
        msp_writereg( 0x12, 0x000d, 0x4800); // prescale SCART
	};
	
	SAA7146_OutCom(COMTYPE_AUDIODAC, MainSwitch, 1, 0);
	Sleep(20);
	SAA7146_OutCom(COMTYPE_AUDIODAC, ADSwitch, 1, 1);
	Sleep(20);
	
	VidMode(1);
	Sleep(10);
	SAA7146_OutCom( COMTYPE_REC_PLAY, __Stop, 0);
	Sleep(20);
	return 0;
}

int DVBVideoPresent(){
	unsigned char x;
	if (( Play == TRUE ) || ( Record == TRUE )) return(0x0f);
	return(Get_Sync_State(&x));
}


void  VES1820_SetInversion(int inversion){
unsigned char val;
	val=ves_reg0;

	switch (inversion) {
	case 2:              // Aus
		val|=0x20;
		break;
	case 1:              // An
		val&=0xdf;
		break;
	default:
	case 0:              // Auto
		val|=0x20; 
		break;
	}
	ves_reg0=val;
	writereg(ves_addr,ves_flags, 0, val);
}

void DVB_SetVolume(unsigned char volleft, unsigned char volright){
	char Text[128];
	unsigned char A,B;
	
	sprintf(Text,"SAA7146 DVB_SetVolume Typ %d : Left %d Right %d",DVB_AUDIO,volleft,volright);
	Write_Log(Text);
	
	if ( DVB_AUDIO == 0 ) {
		A=(unsigned char)(volleft/3.69565);
		B=(unsigned char)(volright/3.69565);
		
		if (A > 0x45) A =0x45;
        if (B > 0x45) B =0x45;
		SendDAC( 3, (unsigned char)(0x80+A));
		SendDAC(4, (unsigned char)B);
	} else if ( DVB_AUDIO == 1 ) {
		A=127-(volleft*127)/255;
		B=127-(volright*127)/255;
		i2c_writereg(0x20, 0x03, A);
		i2c_writereg(0x20, 0x04, B);
	}
}



int SendDAC( unsigned char addr, unsigned char data){
	return SAA7146_OutCom(COMTYPE_AUDIODAC, AudioDAC, 2, addr, data);
}



int i2c_writereg(unsigned char id, unsigned char reg, unsigned char val){
	unsigned char msg[2]={ reg, val }; 
	struct i2c_msg msgs;
	msgs.flags=0;
	msgs.addr=id/2;
	msgs.len=2;
	msgs.buf=msg;
	return(master_xfer(&msgs,1));
}

int msp_writereg( unsigned char dev, unsigned short reg, unsigned short val){
	unsigned char msg[5]={ dev, reg>>8, reg&0xff, val>>8 , val&0xff }; 
	struct i2c_msg msgs;
	
	msgs.flags=0;
	msgs.addr=0x40;
	msgs.len=5;
	msgs.buf=msg;
	return(master_xfer(&msgs,1));
}


int writereg(unsigned char addr, unsigned char flags,int reg, int data){
	int ret;
	char Text[128];
	unsigned char msg[3] = {0x00, 0x1f, 0x00};
	
	if (( VES_TYPE == 0 ) || ( VES_TYPE == 1 )) {
        msg[1]=reg; msg[2]=data;
        ret=i2c_master_send(addr,flags,msg, 3);
        if ( Timing.WriteRegDelay > 0 ) Sleep(Timing.WriteRegDelay);
		if (ret!=3) {
			sprintf(Text,"SAA7146 Writereg VES_Type %d Fail -> Addr: %02x   Flags: %02x   reg %02x   data %02x",VES_TYPE , addr,flags,reg,data);
			Write_Log(Text);
			return(-1);  
		}
        return ret;
	};
	
	if (( VES_TYPE == 2 ) || ( VES_TYPE == 3 )) {
        msg[0]=reg; msg[1]=data;
        ret=i2c_master_send(addr,flags, msg, 2);
        if ( Timing.WriteRegDelay > 0 ) Sleep(Timing.WriteRegDelay);
		if (ret!=2) {
			sprintf(Text,"SAA7146 Writereg VES_Type %d Fail -> Addr: %02x   Flags: %02x   reg %02x   data %02x",VES_TYPE, addr,flags,reg,data);
			Write_Log(Text);
			return(-1);  
		}
        return ret;
	}
	return(0);
}

unsigned char readreg(unsigned char addr, unsigned char flags,unsigned char reg){
	unsigned char mm1[2] = {0x00, 0x1e};
	unsigned char mm2[2] = {0x00, 0x00};
	
	struct i2c_msg msgs[2];
	
	if (( VES_TYPE == 0 ) || ( VES_TYPE == 1 )) {
        msgs[0].flags=0;
        msgs[1].flags=I2C_M_RD;
        msgs[0].addr=msgs[1].addr=addr;
        mm1[1]=reg;
        msgs[0].len=2; msgs[1].len=1;
        msgs[0].buf=mm1; msgs[1].buf=mm2;
        //        i2c_transfer( msgs, 2);
		master_xfer(msgs,2);
        return mm2[0];
	};
	
	if (( VES_TYPE == 2 ) || ( VES_TYPE == 3 ))  {
        mm1[0] = 0x1e;
        mm1[1] = 0x00;
        msgs[0].flags=0;
        msgs[1].flags=I2C_M_RD;
        msgs[0].addr=msgs[1].addr=addr;
        mm1[0]=reg;
        msgs[0].len=1; msgs[1].len=1;
        msgs[0].buf=mm1; msgs[1].buf=mm2;
		master_xfer(msgs,2);
        return mm2[0];
	}
	return(0);
};






/* ---------------------------------------------*/
/* i2c-helper functions				*/
/* ---------------------------------------------*/

/* this functions gets the status from the saa7146 at address 'addr'
and returns it */
DWORD i2c_status_check() {
	DWORD iicsta = 0;
	iicsta = SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, SAA7146_IICSTA );
	return iicsta;
}

/* this function should be called after an i2c-command has been written. 
if we are debugging, it checks, if the busy flags rises and falls correctly
and reports a timeout (-1) or the error-bits set like in described in the specs,
p.123, table 110 */
int i2c_busy_rise_and_fall( int timeout, int retrycount){
	int i = 0;
	DWORD status = 0;
	char Zeile[128];
	/* wait until busy-flag rises */
	for (i = 0; i < 5; i++) {
		status = i2c_status_check();
		/* check busy flag */
		if ( 0 != (status & SAA7146_I2C_BUSY)) break;
		Sleep(1);
	}
	
	for (i = timeout; i > 0; i--) {
		status = i2c_status_check();
		if ( 0 == (status & SAA7146_I2C_BUSY)) return(status);
		if ( 0 != (status & SAA7146_I2C_ERR)) {
			
			sprintf(Zeile,"SAA7146 Master_Xfer i2c_busy_rise_and_fall Error Status 0x%04x Versuch Nr %d",status,retrycount);
			Write_Log(Zeile);
			return(status);
		};
		Sleep(1);
		
	}
	if ( retrycount > 1 ) {
		if ( status == 0x101 ) sprintf(Zeile,"SAA7146 Master_Xfer i2c_busy_rise_and_fall TimeOut 0x%04x (SAA7146_I2C_BUSY) Versuch Nr %d",status,retrycount);
		else sprintf(Zeile,"SAA7146 Master_Xfer i2c_busy_rise_and_fall TimeOut 0x%04x Versuch Nr %d",status,retrycount);
		Write_Log(Zeile);
	};
	return(-1);
}

/* this functions resets the saa7146 at address 'addr'
and returns 0 if everything was fine, otherwise -1 */
int i2c_reset(){
	DWORD status = 0;
	
	//	hprintk("saa7146: ==> i2c_reset\n");
	status = i2c_status_check();
	
	/* clear data-byte for sure */
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, SAA7146_IICSTA ,00);
	
	/* check if any operation is still in progress */
	if ( 0 != ( status & SAA7146_I2C_BUSY) ) {
		
		/* Yes, kill ongoing operation */
		//		hprintk("saa7146: i2c_reset: busy_state detected\n"); 
		
		/* set ABORT-OPERATION-bit */
		SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, SAA7146_IICSTA ,( SAA7146_I2C_BUS_BIT_RATE_3200 | MASK_07));
		SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2 ,(MASK_00 | MASK_16));
		Sleep( Timing.I2C_Bus_Reset_Delay );
		
		/* clear all error-bits pending; this is needed because p.123, note 1 */
		SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, SAA7146_IICSTA ,SAA7146_I2C_BUS_BIT_RATE_3200);
		SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2, (MASK_00 | MASK_16));
		Sleep( Timing.I2C_Bus_Reset_Delay );
	}
	
	/* check if any other error is still present */
	if ( SAA7146_I2C_BUS_BIT_RATE_3200 != (status = i2c_status_check()) ) {
		
		/* yes, try to kick it */
		//		hprintk("saa7146: i2c_reset: error_state detected, status:0x%08x\n",status);
		
		/* clear all error-bits pending */
		SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, SAA7146_IICSTA, SAA7146_I2C_BUS_BIT_RATE_3200 );
		SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2, (MASK_00 | MASK_16));
		Sleep( Timing.I2C_Bus_Reset_Delay );
		/* the data sheet says it might be necessary to clear the status
		twice after an abort */
		SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, SAA7146_IICSTA, SAA7146_I2C_BUS_BIT_RATE_3200 );
		SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2, (MASK_00 | MASK_16));
	}
	
	/* if any error is still present, a fatal error has occured ... */
	if ( SAA7146_I2C_BUS_BIT_RATE_3200 != (status = i2c_status_check()) ) {
		//		hprintk("saa7146: i2c_reset: fatal error, status:0x%08x\n",status);
		return -1;
	}
	
	return 0;
}

/* this functions writes out the data-bytes at 'data' to the saa7146
at address 'addr' regarding the 'timeout' and 'retries' values;
it returns 0 if ok, -1 if the transfer failed, -2 if the transfer
failed badly (e.g. address error) */
int i2c_write_out( DWORD *data, int timeout, int retrycount){
	int status = 0;
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0,SAA7146_IICTRF, *data);
   	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, SAA7146_IICSTA, SAA7146_I2C_BUS_BIT_RATE_3200);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2, (MASK_00 | MASK_16));
	   
	status = i2c_busy_rise_and_fall(timeout,retrycount);
	
	if ( -1 == status ) return -1;
	if ( 0 != (status & SAA7146_I2C_APERR)) return -2;
	if ( 0 != ( status & 0xff)) return -1;
	/* read back data, just in case we were reading ... */
	*data = SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, SAA7146_IICTRF);
	return 0;
}


int prepare(struct i2c_msg m[], int num){
	short h1, h2;
	short i, j, addr;
	short mem = 0, op_count = 0;
	
	/* determine size of needed memory */
	for(i = 0; i < num; i++) {
		mem += m[i].len + 1;
	}
	
	/* we need one unsigned int for three bytes to be send plus
	   one byte to address the device */
	mem = 1 + ((mem-1) / 3);
	
	if ( mem > I2C_MEM_SIZE ) {
		//		hprintk("saa7146: prepare: i2c-message to big\n");
		return -1;
	}
	
	/* be careful: clear out the i2c-mem first */
	memset(i2c_mem,0,4*mem);
	
	/* loop through all messages */
	for(i = 0; i < num; i++) {
		
	/* insert the address of the i2c-slave. note: we get 7-bit-i2c-addresses,
		so we have to perform a translation */
		addr = (m[i].addr*2) + ( (0 != (m[i].flags & I2C_M_RD)) ? 1 : 0); 
		h1 = op_count/3; h2 = op_count%3;
		i2c_mem[h1] |= ((unsigned char)addr << ((3-h2)*8));
		i2c_mem[h1] |= (SAA7146_I2C_START << ((3-h2)*2));
		op_count++;
		
		/* loop through all bytes of message i */
		for(j = 0; j < m[i].len; j++) {
			/* insert the data bytes */
			h1 = op_count/3; h2 = op_count%3;
			i2c_mem[h1] |= ((unsigned char)m[i].buf[j] << ((3-h2)*8));
			i2c_mem[h1] |= (SAA7146_I2C_CONT << ((3-h2)*2));
			op_count++;
		}
		
	}			
	
	/* have a look at the last byte inserted:
	if it was: ...CONT change it to ...STOP */
	h1 = (op_count-1)/3; h2 = (op_count-1)%3;
	if ( SAA7146_I2C_CONT == (0x3 & (i2c_mem[h1] >> ((3-h2)*2))) ) {
		i2c_mem[h1] &= ~(0x2 << ((3-h2)*2));
		i2c_mem[h1] |= (SAA7146_I2C_STOP << ((3-h2)*2));
	}
	return mem;
}

int master_xfer(struct i2c_msg msgs[], int num){
	int result, count;
	int retries;
	int i = 0;
	
	/* prepare the message(s), get number of unsigned ints to transfer */
	count = prepare(msgs, num);
	
	/* loop through number of retries ... */
	for (retries = 0 ; retries < 4; retries++ ) {
		/* reset the i2c-device if necessary */
		if ( retries > 0 ) {
			result = i2c_reset();
			if ( 0 > result ) {
				Write_Log("SAA7146 : Master_Xfer I2C-Bus nimmt kein Reset an ! Sende-Versuch abgebrochen");
				return -1;
			};
		};
		/* see how many unsigned int have to be transferred; if there is only 1,
		we do not start the whole rps1-engine... */
		
		for(i = 0; i < count; i++) {
			result = i2c_write_out( &i2c_mem[i],Timing.I2C_Bus_Timeout, retries);
			if ( 0 != result) {
				/* if address-error occured, don´t retry */
				if ( -2 == result ) {
					Write_Log("SAA7146 Master_Xfer I2C-Bus-Adress Fehler");
					return(-1);
				}
				break;
			}
		}
		/* see if an error occured & the last retry failed */
		if( (0 != result) && (retries > 3 ) ) {
			Write_Log("SAA7146 Master_Xfer I2C Transfer-Fehler -> End of Retrying and Fail");
			return -1;
		}
		if( 0 == result ) break;
	}
	
	/* if any things had to be read, get the results */
	result = clean_up(msgs, num);
	if ( 0 > result ) {
		//		hprintk(KERN_ERR "saa7146_core.o: master_xfer: could not cleanup\n");
		return -1;
	}
	
	/* return the number of delivered messages */
	return num;	
}

int clean_up(struct i2c_msg m[], int num){
	unsigned short i, j;
	unsigned short op_count = 0;
	
	/* loop through all messages */
	for(i = 0; i < num; i++) {
		op_count++;
		/* loop throgh all bytes of message i */
		for(j = 0; j < m[i].len; j++) {
			/* write back all bytes that could have been read */
			m[i].buf[j] = (unsigned char )(i2c_mem[op_count/3] >> ((3-(op_count%3))*8));
			op_count++;
		}
	}
	return 0;
}


int i2c_master_send(unsigned char addr, unsigned char flags, const char *buf ,int count){
	int ret;
	struct i2c_msg msg;
	
	msg.addr   = addr;
	msg.flags =  flags & I2C_M_TEN;
	msg.len = count;
	(const char *)msg.buf = buf;
	
	ret = master_xfer(&msg,1);
	/* if everything went ok (i.e. 1 msg transmitted), return #bytes
	* transmitted, else error code.
	*/
	return (ret == 1 )? count : ret;
}

int i2c_master_recv( unsigned char addr, unsigned char flags, char *buf ,int count){
	struct i2c_msg msg;
	int ret;
	msg.addr   = addr;
	msg.flags = flags & I2C_M_TEN;
	msg.flags |= I2C_M_RD;
	msg.len = count;
	msg.buf = buf;
	
	ret = master_xfer(&msg,1);
	
	/* if everything went ok (i.e. 1 msg transmitted), return #bytes
	* transmitted, else error code.
	*/
	return (ret == 1 )? count : ret;
}

void ClrBit1893(){
	char Text[128];
	sprintf(Text,"SAA7146 ClrBit1893()");
	Write_Log(Text);
	Sleep(1);
	writereg(ves_addr,ves_flags,0, Init_VES_1893_Tab[0] & 0xfe);
	Sleep(1);
	writereg(ves_addr,ves_flags,0, Init_VES_1893_Tab[0]);
	writereg(ves_addr,ves_flags,3, 0);
	writereg(ves_addr,ves_flags,3, Init_VES_1893_Tab[3]);
	if ( Timing.DVB_Reset_Wait > 0 ) Sleep(Timing.DVB_Reset_Wait);
}

void ClrBit1820(){
	char Text[128];
	sprintf(Text,"SAA7146 ClrBit1820()");
	Write_Log(Text);
	writereg(ves_addr,ves_flags, 0, ves_reg0 & 0xfe);
	writereg(ves_addr,ves_flags, 0, ves_reg0);
	if ( Timing.DVB_Reset_Wait > 0 ) Sleep(Timing.DVB_Reset_Wait);
}

void ClrBit0299(){
	char Text[128];
	sprintf(Text,"SAA7146 ClrBit0299()");
	Write_Log(Text);
	writereg(ves_addr,ves_flags, 0x22, 0x00);
	writereg(ves_addr,ves_flags, 0x23, 0x00);
	readreg(ves_addr,ves_flags, 0x23);
	writereg(ves_addr,ves_flags, 0x12, 0xb9);
	if ( Timing.DVB_Reset_Wait > 0 ) Sleep(Timing.DVB_Reset_Wait);
}


void Read8083_FrontBits(){
	int i;
	i=readreg(ves_addr,ves_flags, 0x02);
	i=readreg(ves_addr,ves_flags, 0x0b);
	i=readreg(ves_addr,ves_flags, 0x0c);
	i=readreg(ves_addr,ves_flags, 0x0d);
	i=readreg(ves_addr,ves_flags, 0x05);
	i=readreg(ves_addr,ves_flags, 0x06);
	i=readreg(ves_addr,ves_flags, 0x07);
	i=readreg(ves_addr,ves_flags, 0x0b);
	i=readreg(ves_addr,ves_flags, 0x01);
	i=readreg(ves_addr,ves_flags, 0x08);
};


void ClrBit8083(){
	int i;
	int j;
	unsigned char x;
	char Text[128];
	sprintf(Text,"SAA7146 ClrBit8083()");
	Write_Log(Text);
	
	i=0;
	while ( i < 5 ) {
		writereg(ves_addr,ves_flags, 0x00, 0x3c);
		Sleep(0);
		writereg(ves_addr,ves_flags, 0x00, 0x04);
		if ( Timing.DVB_Reset_Wait > 0 ) Sleep(Timing.DVB_Reset_Wait);
        j=Get_Sync_State(&x);
		if ( j == 0x0f ) return;
        if ( j != 0x07 ) return;
		i++;
	}
};


void SetPWM() {
	char Text[128];
	sprintf(Text,"SAA7146 SetPWM");
	Write_Log(Text);
	writereg(ves_addr,ves_flags, 0x34, ves_pwm); 
	if ( Timing.DVB_Reset_Wait > 0 ) Sleep(Timing.DVB_Reset_Wait);
}

int VES1893_SetSymbolrate(DWORD  Symbolrate, int doclr){
	DWORD BDR;
	DWORD ratio;
	unsigned char  ADCONF, FCONF, FNR;
	DWORD BDRI;
	DWORD tmp;
	char Text[128];
	if ( Symbolrate == 0 ) return(0);
	
	if (( AltSymbolrate == Symbolrate ) && ( FastSwitch == TRUE )) {
		sprintf(Text,"SAA7146 VES1893_SetSymbolrate: %d schon eingestellt",Symbolrate);
		Write_Log(Text);
	
		return(0);
	};
	AltSymbolrate=Symbolrate;
	sprintf(Text,"SAA7146 VES1893_SetSymbolrate: %d ",Symbolrate);
	Write_Log(Text);
	
	Symbolrate=Symbolrate*1000;
	
	if ( Symbolrate == 0 ) Symbolrate=27500000;
	
	if (Symbolrate>90100000UL/2) Symbolrate=90100000UL/2;
	if (Symbolrate<500000) Symbolrate=500000;
	
#define MUL (1UL<<24)
#define FIN (90100000UL>>4)
	ratio=(Symbolrate<<4)/FIN;
	
	tmp=((Symbolrate<<4)%FIN)<<8;
	ratio=(ratio<<8)+tmp/FIN;
	
	tmp=(tmp%FIN)<<8;
	ratio=(ratio<<8)+tmp/FIN;
	
	FNR = 0xFF;
	
	if (ratio < MUL/3)           FNR = 0;
	if (ratio < (MUL*11)/50)     FNR = 1;
	if (ratio < MUL/6)           FNR = 2;
	if (ratio < MUL/9)           FNR = 3;
	if (ratio < MUL/12)          FNR = 4;
	if (ratio < (MUL*11)/200)    FNR = 5;
	if (ratio < MUL/24)          FNR = 6;
	if (ratio < (MUL*27)/1000)   FNR = 7;
	if (ratio < MUL/48)          FNR = 8;
	if (ratio < (MUL*137)/10000) FNR = 9;
	
	if (FNR == 0xFF){
		ADCONF = 0x89;		//bypass Filter
		FCONF  = 0x80;		//default
		FNR	= 0;
	}else{
		ADCONF = 0x81;
		FCONF  = 0x88 | (FNR >> 1) | ((FNR & 0x01) << 5); //default | DFN | AFS
	}
	
	BDR = ((  (ratio<<(FNR>>1))  >>2)+1)>>1;
	
	BDRI = (  ((90100000UL<<4) / ((Symbolrate << (FNR>>1))>>2)  ) +1 ) >> 1;
	
	if (BDRI > 0xFF) BDRI = 0xFF;
	
    writereg(ves_addr,ves_flags,6, 0xff&BDR);
	writereg(ves_addr,ves_flags,7, 0xff&(BDR>>8));
	writereg(ves_addr,ves_flags,8, 0x0f&(BDR>>16));
	
	writereg(ves_addr,ves_flags,9, BDRI);
	writereg(ves_addr,ves_flags,0x20, ADCONF);
	writereg(ves_addr,ves_flags,0x21, FCONF);
	
    if (Symbolrate<6000000) 
		writereg(ves_addr,ves_flags, 5, Init_VES_1893_Tab[0x05] | 0x80);
	else
		writereg(ves_addr,ves_flags, 5, Init_VES_1893_Tab[0x05] & 0x7f);
	
	
	writereg(ves_addr,ves_flags,0, 0);
	writereg(ves_addr,ves_flags,0, 1);
	
	if (doclr == 0) ClrBit1893();
	return 0;
}

int VES1820_SetSymbolrate(DWORD Symbolrate, int DoCLB){
	int BDR; 
	int BDRI;
	short SFIL=0;
	unsigned int XIN ;	
	unsigned int XFIN;
	int NDEC = 0;
	
	unsigned int tmp, ratio;
	char Text[128];
	if ( Symbolrate == 0 ) Symbolrate=6900;
	
	if (( AltSymbolrate == Symbolrate ) && ( FastSwitch == TRUE )) {
		sprintf(Text,"SAA7146 VES1820_SetSymbolrate: %d schon eingestellt",Symbolrate);
		Write_Log(Text);
		return(0);
	};
	AltSymbolrate=Symbolrate;
	
	sprintf(Text,"SAA7146 VES1820_SetSymbolrate: %d ",Symbolrate);
	Write_Log(Text);
	Symbolrate=Symbolrate*1000;
	
	XIN=57840000;
	XFIN=(XIN>>4);
	if (Symbolrate > XIN/2) Symbolrate=XIN/2;
	if (Symbolrate < 500000) Symbolrate=500000;
	
	
	if (Symbolrate < XIN/16) NDEC = 1;
	if (Symbolrate < XIN/32) NDEC = 2;
	if (Symbolrate < XIN/64) NDEC = 3;
	
	if (Symbolrate < (unsigned int)(XIN/12.3)) SFIL = 1;
	if (Symbolrate < (unsigned int)(XIN/16))	 SFIL = 0;
	if (Symbolrate < (unsigned int)(XIN/14.6)) SFIL = 1;
	if (Symbolrate < (unsigned int)(XIN/32))	 SFIL = 0;
	if (Symbolrate < (unsigned int)(XIN/49.2)) SFIL = 1;
	if (Symbolrate < (unsigned int)(XIN/64))	 SFIL = 0;
	if (Symbolrate < (unsigned int)(XIN/98.4)) SFIL = 1;
	
	Symbolrate<<=NDEC;
	ratio=(Symbolrate<<4)/XFIN;
	tmp=((Symbolrate<<4)%XFIN)<<8;
	ratio=(ratio<<8)+tmp/XFIN;
	tmp=(tmp%XFIN)<<8;
	ratio=(ratio<<8)+(tmp+XFIN/2)/XFIN;
	
	BDR= ratio;
	BDRI= (((XIN<<5) / Symbolrate)+1)/2;
	
	if (BDRI > 0xFF) BDRI = 0xFF;
	
	SFIL = (SFIL << 4) | Init_VES_1820_Tab[0x0E];
	
	NDEC = (NDEC << 6) | Init_VES_1820_Tab[0x03];	// Building the Regs
	
	writereg(ves_addr,ves_flags,0x03, NDEC);
	writereg(ves_addr,ves_flags,0x0a, BDR&0xff);
	writereg(ves_addr,ves_flags,0x0b, (BDR>>8)&0xff);
	writereg(ves_addr,ves_flags,0x0c, (BDR>>16)&0x3f);
	
	writereg(ves_addr,ves_flags,0x0d, BDRI);
	writereg(ves_addr,ves_flags,0x0e, SFIL);
	SetPWM();
	if (DoCLB==1) ClrBit1820();
	return 0;
}


int STV0299_SetSymbolrate(DWORD Symbolrate){
	unsigned int ratio;
	unsigned int tmp;
	
	unsigned int M_CLK=88000000;
	unsigned int XFIN = (M_CLK>>4);
	char Text[128];
	unsigned char  aclk=0xb4, bclk=0x51;
	
	if (( AltSymbolrate == Symbolrate ) && ( FastSwitch == TRUE )) {
		sprintf(Text,"SAA7146 STV0299_SetSymbolrate: %d schon eingestellt",Symbolrate);
		Write_Log(Text);
		return(0);
	};
	AltSymbolrate=Symbolrate;
	
	
	sprintf(Text,"SAA7146 STV0299_SetSymbolrate: %d ",Symbolrate);
	Write_Log(Text);
	
	Symbolrate=Symbolrate*1000;
	
	if (Symbolrate>M_CLK) Symbolrate=M_CLK;
	if (Symbolrate<500000) Symbolrate=500000;
	
	if (Symbolrate<30000000) { aclk=0xb6; bclk=0x53; }
	if (Symbolrate<14000000) { aclk=0xb7; bclk=0x53; }
	if (Symbolrate< 7000000) { aclk=0xb7; bclk=0x4f; }
	if (Symbolrate< 3000000) { aclk=0xb7; bclk=0x4b; }
	if (Symbolrate< 1500000) { aclk=0xb7; bclk=0x47; }
	
	
	
    tmp=Symbolrate<<4;
	ratio=tmp/XFIN;
	
	tmp=(tmp%XFIN)<<8;
	ratio=(ratio<<8)+tmp/XFIN;
	
	tmp=(tmp%XFIN)<<8;
	ratio=(ratio<<8)+tmp/XFIN;
	
	ratio&=0xfffff0;
	writereg(ves_addr,ves_flags, 0x13, aclk);
	writereg(ves_addr,ves_flags, 0x14, bclk);
	writereg(ves_addr,ves_flags, 0x1f, (ratio>>16)&0xff);
	writereg(ves_addr,ves_flags, 0x20, (ratio>> 8)&0xff);
	writereg(ves_addr,ves_flags, 0x21, (ratio    )&0xff);
	ves_bclk=bclk&0x3f;
	return 0;
}


int TDA8083_SetSymbolrate(DWORD  Symbolrate, int doclr){
	char Text[128];
	if ( Symbolrate == 0 ) return(0);
	if (( AltSymbolrate == Symbolrate ) && ( FastSwitch == TRUE )) {
		sprintf(Text,"SAA7146 TDA8083_SetSymbolrate: %d schon eingestellt",Symbolrate);
		Write_Log(Text);
		return(0);
	};
	AltSymbolrate=Symbolrate;
	sprintf(Text,"SAA7146 TDA8083_SetSymbolrate: %d ",Symbolrate);
	Write_Log(Text);
	if ( Symbolrate == 29900 ) {
		writereg(ves_addr,ves_flags, 0x05, 0x00);
		writereg(ves_addr,ves_flags, 0x02, 0x44);
		writereg(ves_addr,ves_flags, 0x03, 0x7e);
		writereg(ves_addr,ves_flags, 0x04, 0xb7);
	} else if ( Symbolrate == 28000 ) {
		writereg(ves_addr,ves_flags, 0x05, 0x00);
		writereg(ves_addr,ves_flags, 0x02, 0x49);
		writereg(ves_addr,ves_flags, 0x03, 0x24);
		writereg(ves_addr,ves_flags, 0x04, 0x92);
	} else if ( Symbolrate == 27500 ) {
		writereg(ves_addr,ves_flags, 0x05, 0x00);
		writereg(ves_addr,ves_flags, 0x02, 0x4a);
		writereg(ves_addr,ves_flags, 0x03, 0x79);
		writereg(ves_addr,ves_flags, 0x04, 0x04);
	} else if ( Symbolrate == 22000 ) {
		writereg(ves_addr,ves_flags, 0x05, 0x02);
		writereg(ves_addr,ves_flags, 0x02, 0x5d);
		writereg(ves_addr,ves_flags, 0x03, 0x17);
		writereg(ves_addr,ves_flags, 0x04, 0x45);
	} else if ( Symbolrate == 21995 ) {
		writereg(ves_addr,ves_flags, 0x05, 0x02);
		writereg(ves_addr,ves_flags, 0x02, 0x5d);
		writereg(ves_addr,ves_flags, 0x03, 0x1c);
		writereg(ves_addr,ves_flags, 0x04, 0xb0);
	} else if ( Symbolrate == 20000 ) {
		writereg(ves_addr,ves_flags, 0x05, 0x02);
		writereg(ves_addr,ves_flags, 0x02, 0x66);
		writereg(ves_addr,ves_flags, 0x03, 0x66);
		writereg(ves_addr,ves_flags, 0x04, 0x66);
	} else if ( Symbolrate == 17180 ) {
		writereg(ves_addr,ves_flags, 0x05, 0x02);
		writereg(ves_addr,ves_flags, 0x02, 0x77);
		writereg(ves_addr,ves_flags, 0x03, 0x35);
		writereg(ves_addr,ves_flags, 0x04, 0x58);
	} else if ( Symbolrate == 13846 ) {
		writereg(ves_addr,ves_flags, 0x05, 0x03);
		writereg(ves_addr,ves_flags, 0x02, 0x93);
		writereg(ves_addr,ves_flags, 0x03, 0xe9);
		writereg(ves_addr,ves_flags, 0x04, 0xaa);
	} else {
		// 27500
	};
	
	ClrBit8083();
	return 0;
}




int SetControl(unsigned char val){
	char Text[128];
    if ( VES_TYPE == 0 ) return(0);   // DVB-C mit Ves 1820
	
	if (( AltControlVal == val ) && ( FastSwitch == TRUE )) {
		sprintf(Text,"SAA7146 SetControl : VES_TYPE %d Ves_Crt 0x%02x Val 0x%02x schon eingestellt",VES_TYPE,ves_ctr,val);
		Write_Log(Text);
		return(0);
	}
	AltControlVal=val;
	sprintf(Text,"SAA7146 SetControl : VES_TYPE %d Ves_Crt 0x%02x Val 0x%02x ",VES_TYPE,ves_ctr,val);
	Write_Log(Text);
	
	if ( VES_TYPE == 1 ) {            // DVB-S mit VES1893
		writereg(ves_addr,ves_flags, 0x1f, val);
		return(0);
	};
	if ( VES_TYPE == 2 ) {			 //  DVB-S mit STV 0299
		writereg(ves_addr,ves_flags, 0x0c, val );
		return(0);
    };
	if ( VES_TYPE == 3 ) {			 //  DVB-S mit TDA 8083
		writereg(ves_addr,ves_flags, 0x20, val );
		writereg(ves_addr,ves_flags, 0x20, val );
		ClrBit8083();
		return(0);
    };
	return(0);
}



int SetFEC( int fec){
	char Text[128];
	
    if ( VES_TYPE == 2 ) {
		sprintf(Text,"SAA7146 STV0299 nur AutoFec");
		Write_Log(Text);
		return(0);
	};
	if ( VES_TYPE == 3 ) {
		sprintf(Text,"SAA7146 TDA 8083 nur AutoFec");
		// 0xea in Register 7 
		Write_Log(Text);
		return(0);
	};
	
	if (( fec == Altfec ) && ( FastSwitch == TRUE )) {
		sprintf(Text,"SAA7146 SetFEC : %d schon eingestellt ",fec);
		Write_Log(Text);
		return(0);
	};
	sprintf(Text,"SAA7146 SetFEC : %d ",fec);
	Write_Log(Text);
	
	Altfec=fec;
	
	if (fec>=8 || fec<0) 
		return(writereg(ves_addr,ves_flags,0x0d, 0x08));
	else
		return(writereg(ves_addr,ves_flags,0x0d, fec&7));
}


int SetQAM( unsigned short QAM_Mode,int DoCLB){
	char Text[128];
	
	if (( QAM_Mode == AltQAM_Mode ) && ( FastSwitch == TRUE )) {
		sprintf(Text,"SAA7146 SetQam : %d  schon eingestellt ",QAM_Mode);
		Write_Log(Text);
		return(0);
	};
    AltQAM_Mode = QAM_Mode ;
	sprintf(Text,"SAA7146 SetQam : %d ",QAM_Mode);
	Write_Log(Text);
	
	ves_reg0=(ves_reg0 & 0xe3) | (QAM_Mode << 2);
	
	writereg(ves_addr,ves_flags,0x01, QAM_Values[QAM_Mode].Reg1);
	writereg(ves_addr,ves_flags,0x05, QAM_Values[QAM_Mode].Reg5);
	writereg(ves_addr,ves_flags,0x08, QAM_Values[QAM_Mode].Reg8);
	writereg(ves_addr,ves_flags,0x09, QAM_Values[QAM_Mode].Reg9);
	writereg(ves_addr,ves_flags,0x00, ves_reg0);

	if (DoCLB==1)  ClrBit1820();
	return 0;
}


int calculate_h_scale_registers(unsigned int in_x, unsigned int out_x, int flip_lr, unsigned int *hps_ctrl, unsigned int *hps_v_gain, unsigned int *hps_h_prescale, unsigned int *hps_h_scale){
	unsigned int dcgx = 0, xpsc = 0, xacm = 0, cxy = 0, cxuv = 0;	/* horizontal prescaler */
	unsigned int xim = 0, xp = 0, xsci =0;				/* horizontal scaler */
	unsigned int pfuv = 0;						/* vertical scale & gain */
	
	unsigned int h_atten = 0, i = 0;					/* helper variables */
	
	if ( 0 == out_x ) {
		//		printk("saa7146: ==> calculate_h_scale_registers: invalid value (=0).\n");
		return(-1);
	}
	
	/* mask out vanity-bit */
	*hps_ctrl &= ~MASK_29;
	
	/* calculate prescale-(xspc)-value:	[n   .. 1/2) : 1
	[1/2 .. 1/3) : 2
	[1/3 .. 1/4) : 3
	... 			*/
	if (in_x > out_x) {
		xpsc = in_x / out_x;
	}else {
		/* zooming */
		xpsc = 1;						
	}
	
	/* if flip_lr-bit is set, number of pixels after horizontal prescaling must be < 384 */
	if ( 0 != flip_lr ) {
		
		/* set vanity bit */
		*hps_ctrl |= MASK_29;
		
		while (in_x / xpsc >= 384 )
			xpsc++;
	}
	/* if zooming is wanted, number of pixels after horizontal prescaling must be < 768 */
	else {
		while ( in_x / xpsc >= 768 )
			xpsc++;
	}
	
	/* maximum prescale is 64 (p.69) */
	if ( xpsc > 64 )
		xpsc = 64;
	
	/* keep xacm clear*/
	xacm = 0;
	
	/* set horizontal filter parameters (CXY = CXUV) */
	cxy = hps_h_coeff_tab[( (xpsc - 1) < 63 ? (xpsc - 1) : 63 )].hps_coeff;
	cxuv = cxy;
	
	/* calculate and set horizontal fine scale (xsci) */
	
	/* bypass the horizontal scaler ? */
	if ( (in_x == out_x) && ( 1 == xpsc ) )
		xsci = 0x400;
	else	
		/* philips says: xsci = ( (1024 * in_x) / (out_x * xpsc) ) + xpsc; */
		xsci = ( (1024 * in_x) / (out_x * xpsc) ) + xpsc;
	
	/* set start phase for horizontal fine scale (xp) to 0 */	
	xp = 0;
	
	/* set xim, if we bypass the horizontal scaler */
	if ( 0x400 == xsci )
		xim = 1;
	else
		xim = 0;
	
		/* if the prescaler is bypassed, enable horizontal accumulation mode (xacm)
	   and clear dcgx */
	if( 1 == xpsc ) {
		xacm = 1;
		dcgx = 0;
	}else {
		xacm = 0;
		/* get best match in the table of attenuations for horizontal scaling */
		h_atten = hps_h_coeff_tab[( (xpsc - 1) < 63 ? (xpsc - 1) : 63 )].weight_sum;
		
		for (i = 0; h_attenuation[i] != 0; i++) {
			if (h_attenuation[i] >= h_atten)
				break;
		}
		
		dcgx = i;
	}
	
	/* the horizontal scaling increment controls the UV filter to reduce the bandwith to
	   improve the display quality, so set it ... */
	if ( xsci == 0x400)
		pfuv = 0x00;
	else if ( xsci < 0x600)
		pfuv = 0x01;
	else if ( xsci < 0x680)
		pfuv = 0x11;
	else if ( xsci < 0x700)
		pfuv = 0x22;
	else
		pfuv = 0x33;
	
	
	*hps_v_gain  &= MASK_W0|MASK_B2;
	*hps_v_gain  |= (pfuv << 24);	
	
	*hps_h_scale 	&= ~(MASK_W1 | 0xf000);
	*hps_h_scale	|= (xim << 31) | (xp << 24) | (xsci << 12);
	
	*hps_h_prescale	|= (dcgx << 27) | ((xpsc-1) << 18) | (xacm << 17) | (cxy << 8) | (cxuv << 0);
	
	return 0;
}


int calculate_v_scale_registers( unsigned int in_y, unsigned int out_y, unsigned int *hps_v_scale, unsigned int *hps_v_gain){
	unsigned int yacm = 0, ysci = 0, yacl = 0, ypo = 0, ype = 0;	/* vertical scaling */
	unsigned int dcgy = 0, cya_cyb = 0;				/* vertical scale & gain */
				
	unsigned int v_atten = 0, i = 0;					/* helper variables */
	
	/* error, if vertical zooming */
	if ( in_y < out_y ) {
		//	printk("saa7146: ==> calculate_v_scale_registers: we cannot do vertical zooming.\n");
		return (-1);
	}
	
	/* linear phase interpolation may be used if scaling is between 1 and 1/2
	   or scaling is between 1/2 and 1/4 (if interlace is set; see below) */
	if( ((2*out_y) >= in_y) || (((4*out_y) >= in_y) && saa_interlace != 0)) {
		
	/* convention: if scaling is between 1/2 and 1/4 we only use
	the even lines, the odd lines get discarded (see function move_to)
		if interlace is set */
		if( saa_interlace != 0 && (out_y*4) >= in_y && (out_y*2) <= in_y) 
			out_y *= 2;
		
		yacm = 0;
		yacl = 0;
		cya_cyb = 0x00ff;
		
		/* calculate scaling increment */
		if ( in_y > out_y )
			ysci = ((1024 * in_y) / (out_y + 1)) - 1024;
		else
			ysci = 0;
		
		dcgy = 0;
		
		/* calculate ype and ypo */
		ype = ysci / 16;
		ypo = ype + (ysci / 64);
		
	}else {
		yacm = 1;	
		
		/* calculate scaling increment */
		ysci = (((10 * 1024 * (in_y - out_y - 1)) / in_y) + 9) / 10;
		
		/* calculate ype and ypo */
		ypo = ype = ((ysci + 15) / 16);
		
		/* the sequence length interval (yacl) has to be set according
		to the prescale value, e.g.	[n   .. 1/2) : 0
		[1/2 .. 1/3) : 1
		[1/3 .. 1/4) : 2
		... */
		if ( ysci < 512) {
			yacl = 0;
		}else {
			yacl = ( ysci / (1024 - ysci) );
		}
		
		/* get filter coefficients for cya, cyb from table hps_v_coeff_tab */	
		cya_cyb = hps_v_coeff_tab[ (yacl < 63 ? yacl : 63 ) ].hps_coeff;
		
		/* get best match in the table of attenuations for vertical scaling */
		v_atten = hps_v_coeff_tab[ (yacl < 63 ? yacl : 63 ) ].weight_sum;
		
		for (i = 0; v_attenuation[i] != 0; i++) {
			if (v_attenuation[i] >= v_atten)
				break;
		}
		
		dcgy = i;
	}
	
	/* ypo and ype swapped in spec ? */
	*hps_v_scale	|= (yacm << 31) | (ysci << 21) | (yacl << 15) | (ypo << 8 ) | (ype << 1);
	
	*hps_v_gain	&= ~(MASK_W0|MASK_B2);
	*hps_v_gain	|= (dcgy << 16) | (cya_cyb << 0);
	
	return 0;
}

void calculate_hxo_hyo_and_sources( int port_sel, int sync_sel, unsigned int *hps_h_scale, unsigned int *hps_ctrl){
	unsigned int hyo = 0, hxo = 0;
	
	hyo = modes_constants[saa_mode].v_offset;
	hxo = modes_constants[saa_mode].h_offset;
				
	*hps_h_scale	&= ~(MASK_B0 | 0xf00);
	*hps_ctrl	&= ~(MASK_W0 | MASK_B2 | MASK_30 | MASK_31 | MASK_28);
	
	*hps_h_scale	|= (hxo <<  0);
	*hps_ctrl	|= (hyo << 12);
	
	*hps_ctrl	|= ( port_sel == 0 ? 0x0 : MASK_30);
	*hps_ctrl	|= ( sync_sel == 0 ? 0x0 : MASK_28);
}

void calculate_output_format_register(unsigned short palette, unsigned int *clip_format){
	/* clear out the necessary bits */
	*clip_format &= 0x0000ffff;	
	/* set these bits new */
	*clip_format |=  (( ((palette&0xf00)>>8) << 30) | ((palette&0x00f) << 24) | (((palette&0x0f0)>>4) << 16));
}

void calculate_bcs_ctrl_register(unsigned int brightness, unsigned int contrast, unsigned int colour, unsigned int *bcs_ctrl){
	*bcs_ctrl = ((brightness << 24) | (contrast << 16) | (colour <<  0));
}


void SetDVBColor( int Color ){
}

int calculate_video_dma1_grab( struct saa7146_video_dma* vdma1,BOOL Flip) {
	int depth = 0;
	
	
	unsigned char *dat;
    
	
	dat=DVB_Display_dma.Page[0].pPhysicalAddr;
	if ( Flip == FALSE ) {
		vdma1->pitch		= CurrentX*2*2;
		if ( CurrentY > 288 ) { 
			vdma1->base_even	= (unsigned int)dat + (CurrentX*2);
			vdma1->base_odd		= (unsigned int)dat ;
		} else {
			vdma1->base_even	= (unsigned int)dat;
			vdma1->base_odd		= (unsigned int)dat + (CurrentX*2);
		};
		vdma1->prot_addr	= (unsigned int)dat + ((CurrentY+1)*CurrentX*2);
		vdma1->num_line_byte	= ((modes_constants[saa_mode].v_field<<16) + modes_constants[saa_mode].h_pixels);
		vdma1->base_page	= 0 ;  
	} else {
		vdma1->pitch		= (unsigned int)CurrentX*2*-2;
		vdma1->base_even	= (unsigned int)dat + ((CurrentY-1)*CurrentX*2);
		if ( CurrentY > 288 ) { 
			vdma1->base_odd		= (unsigned int)dat + ((CurrentY)*CurrentX*2);
		} else {
			vdma1->base_odd		= (unsigned int)dat + ((CurrentY-2)*CurrentX*2);
		};
		vdma1->prot_addr	= (unsigned int)dat + (CurrentX*2); ;
		vdma1->num_line_byte	= ((modes_constants[saa_mode].v_field<<16) + modes_constants[saa_mode].h_pixels);
		vdma1->base_page	= 0 ;  
	};
	
	/* convention: if scaling is between 1/2 and 1/4 we only use
	   the even lines, the odd lines get discarded (see vertical scaling) */
	
	if( saa_interlace != 0 && CurrentY*4 >= modes_constants[saa_mode].v_calc && CurrentY*2 <= modes_constants[saa_mode].v_calc) {
		vdma1->base_odd = vdma1->prot_addr;
		vdma1->pitch /= 2;
	}
	
	if ( HalfTVMode == TRUE ) {
		vdma1->base_odd  = vdma1->prot_addr;
		vdma1->pitch /= 2;
	}
	
	return 0;
}


int calculate_Direct_dma1_grab(WD_DMA *dma, int Pitch, int PosOffset ) {
	int depth = 0;
	
	unsigned char *dat;
	struct saa7146_video_dma vdma1;
	
	
	dat=dma->Page[0].pPhysicalAddr;
	dat=dat+PosOffset;
	
	vdma1.pitch		= Pitch*2;
	if ( CurrentY > 288 ) {
		vdma1.base_even	= (unsigned int)dat + Pitch ;
		vdma1.base_odd	= (unsigned int)dat ;
	} else {
		vdma1.base_even	= (unsigned int)dat;
		vdma1.base_odd	= (unsigned int)dat + Pitch;
	}
	vdma1.prot_addr	= (unsigned int)dat + (CurrentY*Pitch*2);
	vdma1.num_line_byte	= ((modes_constants[saa_mode].v_field<<16) + modes_constants[saa_mode].h_pixels);
	vdma1.base_page	= 0 ;  
	/* convention: if scaling is between 1/2 and 1/4 we only use
	   the even lines, the odd lines get discarded (see vertical scaling) */
	if( saa_interlace != 0 && CurrentY*4 >= modes_constants[saa_mode].v_calc && CurrentY*2 <= modes_constants[saa_mode].v_calc) {
		vdma1.base_odd = vdma1.prot_addr;
		vdma1.pitch /= 2;
	}
	
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, BASE_EVEN1,     vdma1.base_even);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, BASE_ODD1,      vdma1.base_odd);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, PROT_ADDR1,     vdma1.prot_addr);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, BASE_PAGE1,     vdma1.base_page);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, PITCH1,		vdma1.pitch);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, NUM_LINE_BYTE1,	vdma1.num_line_byte);
	
	/* update the video dma 1 registers */
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0,  MC2, (MASK_02 | MASK_18));
	return 0;
}


void SAA7146_SetColourFormat(int nColourFormat){
	unsigned int	clip_format;
	unsigned short  palette;
	DWORD* rps0=rps0_mem_dma.pUserAddr;
	
	if ( rps0 == NULL ) return;
	
	if ( nColourFormat == 2 ) palette = 0x210;             // RGB16
	else if ( nColourFormat == 3 ) palette = 0x213;          // RGB15
	else if ( nColourFormat == 4 ) palette = 0;          // YUV2
	
#define RGB15_COMPOSED	0x213
	/* this means: yuv2rgb-conversation-mode=2, dither=yes(=1), format-mode = 3 */
#define RGB16_COMPOSED	0x210
#define RGB24_COMPOSED	0x201
#define RGB32_COMPOSED	0x202
	
	/* this means: yuv2rgb-conversation-mode=0, dither=no(=0), format-mode = 3 */
#define YUV422_COMPOSED		0x000
#define YUV411_COMPOSED		0x003
#define YUV2_COMPOSED		0x004
#define YUV411_DECOMPOSED	0x00b
#define YUV422_DECOMPOSED	0x009
#define YUV420_DECOMPOSED	0x00a
	
	
	saa_grab_format=palette;
	ColourFormat=nColourFormat;
	
	clip_format=SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0,  CLIP_FORMAT_CTRL);
	
	//dprintk("saa7146: ==> set_output_format: pal:0x%03x\n",palette);
	
	/* call helper function */
	calculate_output_format_register(palette,&clip_format);
	//dprintk("saa7146: ==> set_output_format: 0x%08x\n",clip_format);
	
	/* update the hps registers */
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0,  CLIP_FORMAT_CTRL, clip_format);
    SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2, (MASK_05 | MASK_21));
	
	// und ins laufende rps einpoken
	if ( rps_clip_format[0] != 0 ) {
		rps0[rps_clip_format[0]]=clip_format;
	}
}

void set_picture_prop(unsigned int brightness, unsigned int contrast, unsigned int colour){
	unsigned int	bcs_ctrl = 0;
	DWORD* rps0=rps0_mem_dma.pUserAddr;
	calculate_bcs_ctrl_register( brightness, contrast, colour, &bcs_ctrl);
    SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, BCS_CTRL, bcs_ctrl);
	/* update the bcs register */
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2, (MASK_06 | MASK_22));
	
	// und ins laufende rps einpoken
	if ( rps_bcs_ctrl[0] != 0 ) {
		rps0[rps_bcs_ctrl[0]]=bcs_ctrl;
	};
}

/* ---------------------------------------------*/
/* overlay enable/disable			*/
/* ---------------------------------------------*/

/* enable(1) / disable(0) video */
void video_setmode( int v){
	//	hprintk("saa7146: ==> video_setmode; m:%d\n",v);
	
	/* disable ? */
	if(v==0) {
		/* disable video dma1 */
		SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, MASK_22);
		/* turn off rps */
		SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, MASK_28);	
		Sleep(50);
	}
	/* or enable ? */
	else {
		/* turn on rps */
		if ((ModeRAWSurface == FALSE )  &&
			(ModeRAWOverlay == FALSE )) {
			SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, (MASK_12 | MASK_28));	
		};
		/* fixme: enable video */
		//	        SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, (MASK_06 | MASK_22));
	}
};		


DWORD SAALogToPhys(WD_DMA *dma ,DWORD *pLog){ 
	PHYS    BasePhysical; 
	DWORD  *BaseLinear;
	BaseLinear=dma->pUserAddr;
	BasePhysical=(PHYS) dma->Page[0].pPhysicalAddr;
	
	return(BasePhysical+(pLog-BaseLinear)*4); 
};


int Direct_Mode(WD_DMA *dma, int Pitch, int PosOffset){
	unsigned int hps_v_scale = 0, hps_v_gain  = 0, hps_ctrl = 0, hps_h_prescale = 0, hps_h_scale = 0;
	unsigned int clip_format = 0; /* this can be 0, since we don't do clipping */
	unsigned int bcs_ctrl = 0;
	
	/* turn off rps */
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, MASK_28);	
	
	Sleep(50);
	
	calculate_Direct_dma1_grab(dma,Pitch,PosOffset);
	
	calculate_v_scale_registers(modes_constants[saa_mode].v_calc, CurrentY, &hps_v_scale, &hps_v_gain);
	calculate_h_scale_registers(modes_constants[saa_mode].h_calc, CurrentX, 0, &hps_ctrl, &hps_v_gain, &hps_h_prescale, &hps_h_scale);
	calculate_hxo_hyo_and_sources(saa_grab_port, saa_grab_port, &hps_h_scale, &hps_ctrl);
	calculate_output_format_register((unsigned short)saa_grab_format,&clip_format);
	calculate_bcs_ctrl_register(InitialDVBBrightness, InitialDVBContrast, InitialDVBColor, &bcs_ctrl);
	
	
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, HPS_V_SCALE,	hps_v_scale);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, HPS_V_GAIN,	hps_v_gain);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, HPS_CTRL,	hps_ctrl);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, HPS_H_PRESCALE,hps_h_prescale);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, HPS_H_SCALE,	hps_h_scale);
	
	/* upload shadow-ram registers */
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2, (MASK_05 | MASK_06 | MASK_21 | MASK_22) );
	
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, BCS_CTRL, bcs_ctrl);
	/* update the bcs register */
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2, (MASK_06 | MASK_22));
	
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, CLIP_FORMAT_CTRL,clip_format);
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2, (MASK_05 | MASK_21));
	
	/* disable video dma2 */
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, (MASK_21));
	
	/* fixme: enable video */
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, (MASK_06 | MASK_22));
	
	
	return 0;
}



int init_rps0() {
	
	
	DWORD* rps0=rps0_mem_dma.pUserAddr;
	
	
	struct	saa7146_video_dma	vdma1;
	unsigned int hps_v_scale = 0, hps_v_gain  = 0, hps_ctrl = 0, hps_h_prescale = 0, hps_h_scale = 0;
	unsigned int clip_format = 0; /* this can be 0, since we don't do clipping */
	unsigned int bcs_ctrl = 0;
	
	int i = 0, count = 0;
	
	
	/* turn off rps */
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, MASK_28);	
	
	Sleep(50);
	
	/* write beginning of rps-program */
	count = 0;
	rps0[count++]  = CMD_PAUSE | MASK_12 ;	/* wait for o_fid_a */
	rps0[count++]  = CMD_PAUSE | MASK_11 ;	/* wait for e_fid_a */
											/* we only re-program the i-th buffer if the user had set some values for it earlier.
	otherwise the calculation-functions may fail. */
	calculate_video_dma1_grab(&vdma1,Flip);
	if ( HalfTVMode == TRUE ) calculate_v_scale_registers(modes_constants[saa_mode].v_calc, CurrentY/2, &hps_v_scale, &hps_v_gain);
	else calculate_v_scale_registers(modes_constants[saa_mode].v_calc, CurrentY, &hps_v_scale, &hps_v_gain);
	calculate_h_scale_registers(modes_constants[saa_mode].h_calc, CurrentX, 0, &hps_ctrl, &hps_v_gain, &hps_h_prescale, &hps_h_scale);
	calculate_hxo_hyo_and_sources(saa_grab_port, saa_grab_port, &hps_h_scale, &hps_ctrl);
	calculate_output_format_register((unsigned short)saa_grab_format,&clip_format);
	calculate_bcs_ctrl_register(InitialDVBBrightness, InitialDVBContrast, InitialDVBColor, &bcs_ctrl);
	
	rps0[count++]  = CMD_WR_REG | (6 << 8) | HPS_CTRL/4;	/* upload hps-registers for next grab */
	rps0[count++]  = hps_ctrl;
	rps0[count++]  = hps_v_scale;
	rps0[count++]  = hps_v_gain;
	rps0[count++]  = hps_h_prescale;
	rps0[count++]  = hps_h_scale;
	rps_bcs_ctrl[0] = count;
	rps0[count++]  = bcs_ctrl;
	
	rps0[count++]  = CMD_WR_REG | (1 << 8) | CLIP_FORMAT_CTRL/4;/* upload hps-registers for next grab */
	rps_clip_format[0] = count;
	
	rps0[count++]  = clip_format;	
	
	rps0[count++]  = CMD_UPLOAD | MASK_05 | MASK_06;		/* upload hps1/2 */
	
	rps0[count++]  = CMD_WR_REG | (6 << 8) | BASE_ODD1/4;	/* upload video-dma1 registers for next grab */
	rps0[count++]  = vdma1.base_odd;
	rps0[count++]  = vdma1.base_even;
	rps0[count++]  = vdma1.prot_addr;
	rps0[count++]  = vdma1.pitch;
	rps0[count++] = vdma1.base_page;
	rps0[count++]  = vdma1.num_line_byte;
	
	rps0[count++]  = CMD_UPLOAD | MASK_02;		/* upload video-dma1 */
	
	rps0[count++]  = CMD_WR_REG_MASK | (MC1/4);		/* turn on video-dma1 */
	rps0[count++]  = MASK_06 | MASK_22;	    		/* => mask */
	rps0[count++]  = MASK_06 | MASK_22;			/* => values */
	
	rps0[count++]  = CMD_PAUSE | ( saa_grab_port == 0 ? MASK_12 : MASK_14);	/* wait for o_fid_a/b */
	rps0[count++]  = CMD_PAUSE | ( saa_grab_port == 0 ? MASK_11 : MASK_13);	/* wait for e_fid_a/b */
	
	rps0[count++]  = CMD_WR_REG_MASK | (MC1/4);		/* turn off video-dma1 and dma2 (clipping)*/
	rps0[count++]  = MASK_06 | MASK_22 | MASK_05 | MASK_21;	/* => mask */
	rps0[count++]  = MASK_22 | MASK_21;			/* => values */
	
	rps0[count++] = CMD_WR_REG | (1 << 8) | (MC2/4); 	/* Write MC2 */
	rps0[count++] = (1 << 27);
	rps0[count++]  = CMD_INTERRUPT;				/* generate interrupt */
	
	rps0[count++]  = CMD_JUMP;	/* jump to the beginning */
	rps0[count++] = (DWORD)SAALogToPhys(&rps0_mem_dma,&rps0[2]);
	/* write the address of the rps-program */
	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, RPS_ADDR0,(DWORD)rps0_mem_dma.Page[0].pPhysicalAddr);
	/* turn on rps again */
	//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, (MASK_12 | MASK_28));	
	
	return 0;
}




/* this funtion is called whenever a new grab is requested. if possible (that
means: if the rps is not running) it re-programs the rps, otherwise it relys on
the irq-handler to do that */
void set_up_grabbing(){
	video_setmode(0);
    Sleep(25);
	if (ModeRAWSurface == TRUE )  {
		Direct_Mode(Surface_dma,SurfacePitch,Offset_for_WindowOut);
		SetWindowText (hwndFPSField,"Sf-Raw");
	} else if (ModeRAWOverlay == TRUE )  {
		Direct_Mode(Overlay_dma,OverlayPitch,0);
		SetWindowText (hwndFPSField,"Ov-Raw");
	} else {
		init_rps0();
	}
}


void SetAudioPid(unsigned short AudioPid){
	char Text[256];
	sprintf(Text,"SAA7146 SetAudioPID : AudioPID 0x%04X",AudioPid);
	Write_Log(Text);
	SAA7146_OutCom(COMTYPE_PIDFILTER, AudioPID,1, AudioPid);
}

void SetAVPID(unsigned short vpid, unsigned short apid, unsigned short ttpid,unsigned short subpid,unsigned short pcrpid){
	char Text[256];
	sprintf(Text,"SAA7146 SetAVPID : VideoPID 0x%04X AudioPID 0x%04X PCRPID 0x%04X SubPID 0x%04X TTPID 0x%04X",vpid,apid,pcrpid,subpid,ttpid);
	Write_Log(Text);
	SAA7146_OutCom(COMTYPE_PIDFILTER, MultiPID, 5, pcrpid, vpid, apid, DVB_VT ? 0xffff : 0xffff, subpid); // TTPID here does not work...
}

int Get_Tuner_Status( void ){
	return(tuner_getstatus());
}


int CI_Input_Parser(char *Line, unsigned char *OutBytes){
	int i,j;
	int slen, cpos;
    int count;
	char Buffer[255];
	
    slen=strlen(Line);
    count = 0 ;
	cpos=0;
	while ( cpos < slen ) {
        j=0;
        Buffer[j]=0x00;
        while (( Line[cpos] == ' ' ) && (cpos < slen)) cpos++;
        while (( Line[cpos] != ' ' ) && (cpos < slen)) {
			Buffer[j]=Line[cpos];
			cpos++;
			j++;
			Buffer[j]=0x00;
		};
		
		if (( Buffer[0] == '0' ) && (( Buffer[1] == 'X' ) || ( Buffer[1] == 'x' ))) {
			if ( strlen(Buffer) > 4 ) return(-1);
            Buffer[1] = 'x';
			j=sscanf(Buffer,"0x%x",&i);
			if ( j != 1 ) return(-1);
            OutBytes[count]=(unsigned char )i;
			count++;
		} else if ( Buffer[0] == '"' )  {
			j=strlen(Buffer);
			if ( j == 1 ) return(-1);
			for ( i=1; i < (int)(strlen(Buffer)-1); i++ ) {
				OutBytes[count]=(unsigned char )Buffer[i];
				count++;
			};
        } else {
			j=atoi(Buffer);
			if ( j > 255 ) return(-1);
			if ( j < 0 )   return(-1);
            OutBytes[count]=(unsigned char )j;
			count++;
			
		}
	}
	return(count);
}


void CI_MenuKey(unsigned char Cam, unsigned char key){
	unsigned char param[10];
	param[0]=key;
	param[1]=Cam;
	param[2]=0;
	
	SendCICommand(CI_CMD_KEYPRESS, param, 2);
};



void CI_EnterKey(unsigned char Cam, unsigned char Length, unsigned char *key){
	unsigned char param[64];
	int i;
	
	for ( i = 0 ; i < Length; i++ ) {
		param[0]=key[i];
		param[1]=Cam;
		param[2]=0;
		SendCICommand(CI_CMD_KEYPRESS, param, 2);
	};
	
};


void CI_Menu(short Cam){
	unsigned char param[10];
	// Vorsichtshalber erst einmal alle zu !!
	param[0]=(unsigned char)Cam;
	SendCICommand(CI_CMD_ENTER_MENU, param, 1);
};

void CI_Close_Menu(short Cam){
	unsigned char param[10];
	// Vorsichtshalber erst einmal alle zu !!
	param[0]=(unsigned char)Cam;
	SendCICommand(CI_CMD_ENTER_MENU, param, 1);
};



void CI_Reset(void){
	unsigned char param[10];
	SendCICommand(CI_CMD_SYSTEM_READY, 0, 0);
	ci_module[0].ModuleName[0]=0x00;
	ci_module[1].ModuleName[0]=0x00;
	param[0]=0x0;
	SendCICommand(CI_CMD_FAST_PSI, param, 1);
	Sleep(100);
	Get_CI_Status();
};

void CI_Run(int Nummer){
	unsigned char CI_Pnr[2];
	char Text[255];
	sprintf(Text,"SAA7146 CI_Run : SID 0x%04X  ( Modul 0 <%s> ) ( Modul 1 <%s> )",Programm[Nummer].SID_pid,ci_module[0].ModuleName,ci_module[1].ModuleName);
	Write_Log(Text);
	if (( ci_module[0].ModuleName[0] == 0x00 ) && ( ci_module[1].ModuleName[0] == 0x00 )) {
		sprintf(Text,"SAA7146 CI_Run Kein Module ->Return");
		Write_Log(Text);
		return;
	};
	
	SendCICommand(CI_CMD_SYSTEM_READY, 0, 0);
	CI_Pnr[0]=Programm[Nummer].SID_pid>>8;
	CI_Pnr[1]=Programm[Nummer].SID_pid&0xff;
	SendCICommand(CI_CMD_FAST_PSI, CI_Pnr, 2);
};


void Get_CI_Status( void ){
	unsigned char param[10];
	Write_Log("SAA7146 Get_CI_Status");
	param[0]=1;
	SendCICommand(CI_CMD_GET_SLOT_INFO, param, 1);
	param[0]=2;
	SendCICommand( CI_CMD_GET_SLOT_INFO, param, 1);
};


void Get_VideoStatus(void){
	unsigned short tag;
	tag = ((COMTYPE_REQUEST << 8) + VideoState1);
	SAA7146_RequestParameter(tag, &DecoderStatus.ProcessingState, 16);
};

void  Get_Decoder_Status( void ){
	unsigned short tag;
	tag = ((COMTYPE_REQUEST << 8) + VideoState1);
	SAA7146_RequestParameter(tag, &DecoderStatus.ProcessingState, 16);
	
	tag = ((COMTYPE_REQUEST << 8) + VideoState2);
	SAA7146_RequestParameter(tag, &DecoderStatus.dummy4, 16);
	tag = ((COMTYPE_REQUEST << 8) + VideoState3);
	SAA7146_RequestParameter(tag, &DecoderStatus.GOPTimeCode1, 16);
	
}


void Create_Osd_Thread( LPTHREAD_START_ROUTINE ThreadProzess){
	DWORD LinkThreadID;
    Stop_Osd_Key_Event();
    if ( OSD_KEY_EVENT != NULL ) CloseHandle(	OSD_KEY_EVENT );
    OSD_KEY_EVENT=NULL;
    if ( Programm_OSD_Thread != NULL ) CloseHandle(Programm_OSD_Thread );
	if ( Programm_OSD_Stop == NULL ) Programm_OSD_Stop=CreateEvent(NULL,FALSE,FALSE,NULL);
	Programm_OSD_Thread = NULL;
	OSD_KEY_EVENT=CreateEvent(NULL,FALSE,FALSE,NULL);
	ResetEvent(Programm_OSD_Stop);
	ResetEvent(OSD_KEY_EVENT);
    OSD_Key_Value=' ';
	Programm_OSD_Thread=CreateThread (
		(LPSECURITY_ATTRIBUTES)NULL,    // No security.
		(DWORD)0,                          // Same stack size.
		ThreadProzess,// Thread procedure.
		NULL,                         // Parameter.
		(DWORD)0,                          // Start immediatly.
		(LPDWORD)&LinkThreadID);  
	
};

void OSD_Mute( void ){
	BITMAPINFO          *TESTBITMAP                    = NULL;
	HGLOBAL hGlobal;
	unsigned short ch, cl;
	unsigned int yuv;
	PROGRAMM_OSD_RUN=TRUE;
	hGlobal=LoadResource(hInst,FindResource(hInst,"MUTE",RT_BITMAP));
	TESTBITMAP=(BITMAPINFO*)LockResource(hGlobal);
	
	CreateOSDWindow( 1, BITMAP4, (unsigned short)TESTBITMAP->bmiHeader.biWidth, (unsigned short)TESTBITMAP->bmiHeader.biHeight);
	MoveWindowAbs(1,(unsigned short)(710-TESTBITMAP->bmiHeader.biWidth),10);
	SetColorBlend(1);
	yuv=RGB2YUV(0,0,0);
	cl=(yuv&0xffff);
	ch=((yuv>>16)&0xffff);
	SetColor_(1, 16,0, ch, cl);
	SetBlend_(1,16,0, 0);
	DrawBlock(1, 0, 0,(unsigned short)TESTBITMAP->bmiHeader.biWidth, (unsigned short)TESTBITMAP->bmiHeader.biHeight,0);
	if ( OSDLoadBitmap4(1,TESTBITMAP,0,0,0,0,0,0,2000) < 0 ) {
		DestroyOSDWindow(1);
		DeleteObject(TESTBITMAP);
		Osd_Thread_Down();
		return;
	};
	while (( System_In_Mute == TRUE ) && ( Current_Mode == 1 )) {
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


void OSD_No_Sig( void ){
		BITMAPINFO          *TESTBITMAP                    = NULL;
		HGLOBAL hGlobal;
		unsigned short ch, cl;
		unsigned int yuv;
	    int i;

		if (Play==TRUE ) {
		        Osd_Thread_Down();
                return;
				};

		PROGRAMM_OSD_RUN=TRUE;
		if ( Is_Sat_Karte() == TRUE ) {
			hGlobal=LoadResource(hInst,FindResource(hInst,"NO_SIG",RT_BITMAP));
		} else {
			hGlobal=LoadResource(hInst,FindResource(hInst,"NO_SIGCABLE",RT_BITMAP));
		};
		TESTBITMAP=(BITMAPINFO*)LockResource(hGlobal);
		
		CreateOSDWindow( 1, BITMAP4, (unsigned short)TESTBITMAP->bmiHeader.biWidth, (unsigned short)TESTBITMAP->bmiHeader.biHeight);
		MoveWindowAbs(1,(unsigned short)(670-TESTBITMAP->bmiHeader.biWidth),10);
		SetColorBlend(1);
		yuv=RGB2YUV(0,0,0);
		cl=(yuv&0xffff);
		ch=((yuv>>16)&0xffff);
		SetColor_(1, 16,0, ch, cl);
		SetBlend_(1,16,0, 0);
		DrawBlock(1, 0, 0,(unsigned short)TESTBITMAP->bmiHeader.biWidth, (unsigned short)TESTBITMAP->bmiHeader.biHeight,1000);
		if ( OSDLoadBitmap4(1,TESTBITMAP,0,0,0,0,0,0,1000) < 0 ) {
            DestroyOSDWindow(1);
			DeleteObject(TESTBITMAP);
            Osd_Thread_Down();
			return;
		};
		i=0;
		System_in_Sync=1; // Irq von Load_Bitmap
		while (( System_in_Sync > 0 ) && ( Current_Mode == 1 )) {
			ResetEvent(OSD_KEY_EVENT);
			if ( WaitForSingleObject(OSD_KEY_EVENT, 50) != WAIT_TIMEOUT ) {
				DestroyOSDWindow(1);
				DeleteObject(TESTBITMAP);
				Osd_Thread_Down();
				System_in_Sync=0;
				return;
			};
            i++;
			if ( i > 20 ) {
				DVBVideoPresent();
				i=0;
			};
		};
		DestroyOSDWindow(1);
		DeleteObject(TESTBITMAP);
        Osd_Thread_Down();
		System_in_Sync=0;
};


void OSD_Uhr( void ){
	char Datum[40];
	char AltDatum[40];
	char Zeit[40];
	char AltZeit[40];
	int i;
	PROGRAMM_OSD_RUN=TRUE;
	
	CreateOSDWindow( 1, BITMAP2, 170, 90);
	MoveWindowAbs(1,510,20);
	SetColorBlend(1);
	OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
	OSDSetColor(1,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
	OSDSetColor(1,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
	OSDSetColor(1,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
	DrawBlock(1, 0, 0,170, 90,0);
	DrawBlock(1, 5, 5,160, 80,1);
	AltZeit[0]='!';
	AltZeit[1]=0x00;
	AltDatum[0]='!';
	AltDatum[1]=0x00;
	
	for ( i=0; i < 6; i++ ) {
		if ( TDT.Date_Id == 0 ) strcpy(Zeit,"XX:XX:XX");
		else sprintf(Zeit,"%02d:%02d:%02d",RealTime(TDT.Hour),TDT.Min,TDT.Sec);
		
		if ( strcmp(Zeit,AltZeit) != 0 ) {
			
			DrawBlock(1, 5, 5,160, 40,1);
			strcpy(AltZeit,Zeit);                
			SetFont( 1, 3, 3, 1);
			WriteText(1, 23, 9,Zeit);
        };
		
		
		if ( TDT.Date_Id == 0 ) strcpy(Datum,"XX XX.XX.XXXX");
		else Date_To_String((unsigned short)RealDay(TDT.Hour,TDT.Date_Id),Datum);
		
		if ( strcmp(Datum,AltDatum) != 0 ) {
			DrawBlock(1, 5, 45,160, 40,1);
			SetFont( 1, 1, 2, 1);
			strcpy(AltDatum,Datum);                
			WriteText( 1, 10, 49,Datum);
        };
		ResetEvent(OSD_KEY_EVENT);
		if ( WaitForSingleObject(OSD_KEY_EVENT, 1000) != WAIT_TIMEOUT ) {
			DestroyOSDWindow(1);
			Osd_Thread_Down();
			return;
		};
		
	};
	DestroyOSDWindow((unsigned char )1);
	Osd_Thread_Down();
	return;
};

void Int_To_Mode_String(int mode,int val, LPSTR Text){
	if ( mode == 0 ) sprintf(Text,"%05d",val);
	else sprintf(Text,"%04X",val);
};

unsigned short Get_Mode_String(int mode, LPSTR Text){
	int i,j;
	unsigned short Ret;
	
	if ( mode == 0 ) {
		i=atoi(Text);
	} else if ( mode == 1 ) {
		j=sscanf(Text,"%04X",&i);
	};
	Ret=(unsigned short)i;
	return(Ret);
};        


void OSD_ChannelInfo( void ){
	int i;
	int cPos=0;
	char OutBuffer[128];
	int ELen=5;
	int mode=0;
	
	char Zeile[128];
	PROGRAMM_OSD=TRUE;
	CreateOSDWindow( 1, BITMAP2, 520 , 478);
	MoveWindowAbs(1,100,30 );
	SetColorBlend(1);
	OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
	OSDSetColor(1,1, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[3].Blend);
	OSDSetColor(1,2, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
	DrawBlock(1, 0, 0,520, 33,1);
	SetFont( 1, 0, 0, 1);
	strcpy(Zeile,t("Kanal-Informationen"));
	WriteText( 1, 130, 0,Zeile);
	DrawBlock(1, 0, 448,520, 30,1);
	SetFont( 1, 1, 0, 1);
	strcpy(Zeile,t("FunktionsNr oder <9> Hex oder <0> Ende"));
	WriteText( 1, 30,447,Zeile);
	SetFont( 1, 1, 2, 0);
	strcpy(Zeile,"1");
	WriteText( 1, 8,40,Zeile);
	strcpy(Zeile,t("Name"));
	WriteText( 1, 35,40,Zeile);
	strcpy(Zeile,"2");
	WriteText( 1, 8,68,Zeile);
	strcpy(Zeile,t("Anbieter"));
	WriteText( 1, 35,68,Zeile);
	strcpy(Zeile,t("Freqenz"));
	WriteText( 1, 35,96,Zeile);
	strcpy(Zeile,t("Symbolrate"));
	WriteText( 1, 35,124,Zeile);
	if ( Is_Sat_Karte() == TRUE ) {
		strcpy(Zeile,"Pol");
		WriteText( 1, 35,152,Zeile);
		strcpy(Zeile,"Fec");
		WriteText( 1, 35,180,Zeile);
		strcpy(Zeile,"DiSEeq");
		WriteText( 1, 35,206,Zeile);
	} else {
		strcpy(Zeile,"Quam");
		WriteText( 1, 35,152,Zeile);
	};
	strcpy(Zeile,"3");
	WriteText( 1, 8,232,Zeile);
	strcpy(Zeile,"PMT - Pid");
	WriteText( 1, 35,232,Zeile);
	strcpy(Zeile,"4");
	WriteText( 1, 8,258,Zeile);
	strcpy(Zeile,"SID - Pid");
	WriteText( 1, 35,258,Zeile);
	strcpy(Zeile,"5");
	WriteText( 1, 8,284,Zeile);
	strcpy(Zeile,"PCR - Pid");
	WriteText( 1, 35,284,Zeile);
	strcpy(Zeile,"6");
	WriteText( 1, 8,310,Zeile);
	strcpy(Zeile,"Audio - Pid");
	WriteText( 1, 35,310,Zeile);
	strcpy(Zeile,"7");
	WriteText( 1, 8,336,Zeile);
	strcpy(Zeile,"Video - Pid");
	WriteText( 1, 35,336,Zeile);
	strcpy(Zeile,"8");
	WriteText( 1, 8,362,Zeile);
	strcpy(Zeile,"Text - Pid");
	WriteText( 1, 35,362,Zeile);
	strcpy(Zeile,"Service");
	WriteText( 1, 35,388,Zeile);
	strcpy(Zeile,"Crypt");
	WriteText( 1, 35,414,Zeile);
	
	
	
	WriteText( 1, 180,40,Programm[CurrentProgramm].Name);
	WriteText( 1, 180,68,Programm[CurrentProgramm].Anbieter);
 		 sprintf(Zeile,"%10.3f",Programm[CurrentProgramm].freq/1000.);
		 WriteText( 1, 180,96,Zeile);
		 sprintf(Zeile,"%d",Programm[CurrentProgramm].srate);
		 WriteText( 1, 180,124,Zeile);
		 
		 if ( Is_Sat_Karte() == TRUE ) {
			 if ( Programm[CurrentProgramm].volt == 1 ) strcpy(Zeile,t("Horizontal "));
			 else strcpy(Zeile,t("Vertikal "));
			 WriteText( 1, 180,152,Zeile);
			 if ( Programm[CurrentProgramm].fec == 0 ) strcpy(Zeile,"1/2");
			 else if ( Programm[CurrentProgramm].fec == 1 ) strcpy(Zeile,"2/3");
			 else if ( Programm[CurrentProgramm].fec == 2 ) strcpy(Zeile,"3/4");
			 else if ( Programm[CurrentProgramm].fec == 3 ) strcpy(Zeile,"4/5");
			 else if ( Programm[CurrentProgramm].fec == 4 ) strcpy(Zeile,"5/6");
			 else if ( Programm[CurrentProgramm].fec == 5 ) strcpy(Zeile,"6/7");
			 else if ( Programm[CurrentProgramm].fec == 6 ) strcpy(Zeile,"7/8");
			 else if ( Programm[CurrentProgramm].fec == 7 ) strcpy(Zeile,"8/9");
			 else if ( Programm[CurrentProgramm].fec == 8 ) strcpy(Zeile,"Auto");
			 WriteText( 1, 180,180,Zeile);
			 sprintf(Zeile,"%d",Programm[CurrentProgramm].diseqc);
			 WriteText( 1, 180,206,Zeile);
         } else {
			 if ( Programm[CurrentProgramm].qam == 0 ) strcpy(Zeile,"16");
			 else if ( Programm[CurrentProgramm].qam == 1 ) strcpy(Zeile,"32");
			 else if ( Programm[CurrentProgramm].qam == 2 ) strcpy(Zeile,"64");
			 else if ( Programm[CurrentProgramm].qam == 3 ) strcpy(Zeile,"128");
			 else if ( Programm[CurrentProgramm].qam == 4 ) strcpy(Zeile,"256");
			 WriteText( 1, 180,152,Zeile);
         };
		 
OSD_MODE_CHANGE:
		 DrawBlock(1, 0, 448,520, 30,1);
		 SetFont( 1, 1, 0, 1);
         if ( mode == 0 ) strcpy(Zeile,t("FunktionsNr oder <9> Hex oder <0> Ende"));
		 else strcpy(Zeile,t("FunktionsNr oder <9> Dez oder <0> Ende"));
		 WriteText( 1, 30,447,Zeile);
		 
		 DrawBlock(1, 179, 232,341, 216,0);
	SetFont( 1, 1, 2, 0);
         Int_To_Mode_String(mode,Programm[CurrentProgramm].PMT_pid, Zeile);
		 WriteText( 1, 180,232,Zeile);
         Int_To_Mode_String(mode,Programm[CurrentProgramm].SID_pid, Zeile);
		 WriteText( 1, 180,258,Zeile);
         Int_To_Mode_String(mode,Programm[CurrentProgramm].PCR_pid, Zeile);
		 WriteText( 1, 180,284,Zeile);
         Int_To_Mode_String(mode,Programm[CurrentProgramm].Audio_pid, Zeile);
		 WriteText( 1, 180,310,Zeile);
         Int_To_Mode_String(mode,Programm[CurrentProgramm].Video_pid, Zeile);
		 WriteText( 1, 180,336,Zeile);
         Int_To_Mode_String(mode,Programm[CurrentProgramm].TeleText_pid, Zeile);
		 WriteText( 1, 180,362,Zeile);
         Make_Service_Name(Programm[CurrentProgramm].ServiceTyp,Zeile);
		 WriteText( 1, 180,388,Zeile);
         Make_Crypt_Systems(CurrentProgramm,Zeile);
		 WriteText( 1, 180,414,Zeile);
		 
		 for (;;) {
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
			 if ( OSD_Key_Value == '0' ) {; 
			 DestroyOSDWindow(1);
			 Osd_Thread_Down();
			 return;
			 };
			 if ( OSD_Key_Value == '9' ) {; 
			 if ( mode == 0 ) mode=1;
			 else mode=0;
			 goto OSD_MODE_CHANGE;
			 return;
			 };
			 
			 if ( mode == 0 ) ELen=5; else ELen=4;
			 if ( OSD_Key_Value == '1') {
				 strcpy(OutBuffer,Programm[CurrentProgramm].Name);
				 DrawBlock(1, 180, 40,520, 30,0 );
				 cPos=0;
				 i=OSD_Edit_Ascii(1, 180 , 40, 20, &cPos, OutBuffer,1,0);
				 DrawBlock(1, 180, 40,520, 32,0 );
				 WriteText( 1, 180,68,Programm[CurrentProgramm].Anbieter);
				 if ( i == 27 ) {
					 WriteText( 1, 180,40,Programm[CurrentProgramm].Name);
				 } else if ( i == 13 ) {
					 strcpy(Programm[CurrentProgramm].Name,OutBuffer);
					 WriteText( 1, 180,40,Programm[CurrentProgramm].Name);
					 Write_Programm_List();
				 };
				 
			 };
			 
			 
			 if ( OSD_Key_Value == '2') {
				 sprintf(OutBuffer,"%05d",Programm[CurrentProgramm].PMT_pid);
				 DrawBlock(1, 180, 68,520, 30,0 );
				 cPos=0;
				 i=OSD_Edit_Ascii(1, 180 , 68, 20, &cPos, OutBuffer,1,0);
				 DrawBlock(1, 180, 68,520, 32,0 );
				 sprintf(Zeile,"%10.3f",Programm[CurrentProgramm].freq/1000.);
				 WriteText( 1, 180,96,Zeile);
				 if ( i == 27 ) {
               	     WriteText( 1, 180,68,Programm[CurrentProgramm].Anbieter);
				 } else if ( i == 13 ) {
					 strcpy(Programm[CurrentProgramm].Anbieter,OutBuffer);
					 WriteText( 1, 180,68,Programm[CurrentProgramm].Anbieter);
					 Write_Programm_List();
				 }
			 };
			 
			 if ( OSD_Key_Value == '3') {
				 Int_To_Mode_String(mode,Programm[CurrentProgramm].PMT_pid, OutBuffer);
				 DrawBlock(1, 180, 232,520, 30,0 );
				 cPos=0;
				 i=OSD_Edit_Ascii(1, 180 , 232, ELen, &cPos, OutBuffer,1,0);
				 DrawBlock(1, 180, 232,520, 32,0 );
				 Int_To_Mode_String(mode,Programm[CurrentProgramm].SID_pid, Zeile);
				 WriteText( 1, 180,258,Zeile);
				 if ( i == 27 ) {
					 Int_To_Mode_String(mode,Programm[CurrentProgramm].PMT_pid, Zeile);
					 WriteText( 1, 180,232,Zeile);
				 } else if ( i == 13 ) {
					 Programm[CurrentProgramm].PMT_pid=(unsigned short)Get_Mode_String(mode,OutBuffer);
					 Int_To_Mode_String(mode,Programm[CurrentProgramm].PMT_pid, Zeile);
					 WriteText( 1, 180,232,Zeile);
					 Write_Programm_List();
				 }
			 };
			 
			 if ( OSD_Key_Value == '4') {
				 Int_To_Mode_String(mode,Programm[CurrentProgramm].SID_pid, OutBuffer);
				 DrawBlock(1, 180, 258,520, 30,0 );
				 cPos=0;
				 i=OSD_Edit_Ascii(1, 180 , 258, ELen, &cPos, OutBuffer,1,0);
				 DrawBlock(1, 180, 258,520, 32,0 );
				 Int_To_Mode_String(mode,Programm[CurrentProgramm].PCR_pid, Zeile);
				 WriteText( 1, 180,284,Zeile);
				 if ( i == 27 ) {
					 Int_To_Mode_String(mode,Programm[CurrentProgramm].SID_pid, Zeile);
					 WriteText( 1, 180,258,Zeile);
				 } else if ( i == 13 ) {
					 Programm[CurrentProgramm].SID_pid=(unsigned short)Get_Mode_String(mode,OutBuffer);
					 Int_To_Mode_String(mode,Programm[CurrentProgramm].SID_pid, Zeile);
					 WriteText( 1, 180,258,Zeile);
					 Write_Programm_List();
				 }
			 };
			 
			 if ( OSD_Key_Value == '5') {
				 Int_To_Mode_String(mode,Programm[CurrentProgramm].PCR_pid, OutBuffer);
				 DrawBlock(1, 180, 284,520, 30,0 );
				 cPos=0;
				 i=OSD_Edit_Ascii(1, 180 , 284, ELen, &cPos, OutBuffer,1,0);
				 DrawBlock(1, 180, 284,520, 32,0 );
				 Int_To_Mode_String(mode,Programm[CurrentProgramm].Audio_pid, Zeile);
				 WriteText( 1, 180,310,Zeile);
				 
				 if ( i == 27 ) {
					 Int_To_Mode_String(mode,Programm[CurrentProgramm].PCR_pid, Zeile);
					 WriteText( 1, 180,284,Zeile);
				 } else if ( i == 13 ) {
					 Programm[CurrentProgramm].PCR_pid=(unsigned short)Get_Mode_String(mode,OutBuffer);
					 Int_To_Mode_String(mode,Programm[CurrentProgramm].PCR_pid, Zeile);
					 WriteText( 1, 180,284,Zeile);
					 Write_Programm_List();
				 }
			 };
			 
			 if ( OSD_Key_Value == '6') {
				 Int_To_Mode_String(mode,Programm[CurrentProgramm].Audio_pid, OutBuffer);
				 DrawBlock(1, 180, 310,520, 30,0 );
				 cPos=0;
				 i=OSD_Edit_Ascii(1, 180 , 310, ELen, &cPos, OutBuffer,1,0);
				 DrawBlock(1, 180, 310,520, 32,0 );
				 Int_To_Mode_String(mode,Programm[CurrentProgramm].Video_pid, Zeile);
				 WriteText( 1, 180,336,Zeile);
				 
				 if ( i == 27 ) {
					 Int_To_Mode_String(mode,Programm[CurrentProgramm].Audio_pid, Zeile);
					 WriteText( 1, 180,310,Zeile);
				 } else if ( i == 13 ) {
					 Programm[CurrentProgramm].Audio_pid=(unsigned short)Get_Mode_String(mode,OutBuffer);
					 Int_To_Mode_String(mode,Programm[CurrentProgramm].Audio_pid, Zeile);
					 WriteText( 1, 180,310,Zeile);
					 Write_Programm_List();
				 }
			 };
			 
			 if ( OSD_Key_Value == '7') {
				 Int_To_Mode_String(mode,Programm[CurrentProgramm].Video_pid, OutBuffer);
				 DrawBlock(1, 180, 336,520, 30,0 );
				 cPos=0;
				 i=OSD_Edit_Ascii(1, 180 , 336, ELen, &cPos, OutBuffer,1,0);
				 DrawBlock(1, 180, 336,520, 32,0 );
				 Int_To_Mode_String(mode,Programm[CurrentProgramm].TeleText_pid, Zeile);
				 WriteText( 1, 180,362,Zeile);
				 
				 if ( i == 27 ) {
					 Int_To_Mode_String(mode,Programm[CurrentProgramm].Video_pid, Zeile);
					 WriteText( 1, 180,336,Zeile);
				 } else if ( i == 13 ) {
					 Programm[CurrentProgramm].Video_pid=(unsigned short)Get_Mode_String(mode,OutBuffer);
					 Int_To_Mode_String(mode,Programm[CurrentProgramm].Video_pid, Zeile);
					 WriteText( 1, 180,336,Zeile);
					 Write_Programm_List();
				 }
			 };
			 
			 if ( OSD_Key_Value == '8') {
				 Int_To_Mode_String(mode,Programm[CurrentProgramm].TeleText_pid, OutBuffer);
				 DrawBlock(1, 180, 362,520, 30,0 );
				 cPos=0;
				 i=OSD_Edit_Ascii(1, 180 , 362, ELen, &cPos, OutBuffer,1,0);
				 DrawBlock(1, 180, 362,520, 32,0 );
				 Make_Service_Name(Programm[CurrentProgramm].ServiceTyp,Zeile);
				 WriteText( 1, 180,388,Zeile);
				 
				 if ( i == 27 ) {
                     Int_To_Mode_String(mode,Programm[CurrentProgramm].TeleText_pid, Zeile);
					 WriteText( 1, 180,362,Zeile);
				 } else if ( i == 13 ) {
					 Programm[CurrentProgramm].TeleText_pid=Get_Mode_String(mode,OutBuffer);
                     Int_To_Mode_String(mode,Programm[CurrentProgramm].TeleText_pid, Zeile);
					 WriteText( 1, 180,362,Zeile);
					 Write_Programm_List();
				 }
			 };
			 
		 };
		 
		 DestroyOSDWindow(1);
		 Osd_Thread_Down();
		 
};



void Update_OSD_Volume( void ){
	if ( OSD_VOLUME == FALSE ) {
		Create_Osd_Thread((LPTHREAD_START_ROUTINE)OSD_Volume);
		OSD_VOLUME=TRUE;
	} else {
		OSD_Key_Value = ' ';
		if ( OSD_KEY_EVENT != NULL ) SetEvent(OSD_KEY_EVENT);
	};
};


void OSD_Volume( void ){
	int i;
	BOOL Init;
	
	int xleftlen;
	int xrightlen;
	int altxleftlen=-1;
	int altxrightlen=-1;
	
	PROGRAMM_OSD_RUN=TRUE;
	OSD_VOLUME=TRUE;
	Init=TRUE;
	CreateOSDWindow( 1, BITMAP2, 147, 80);
	MoveWindowAbs(1,20,20);
	SetColorBlend(1);
	OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
	OSDSetColor(1,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
	OSDSetColor(1,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
	OSDSetColor(1,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
	DrawBlock(1, 0, 0,147, 80,0);
	DrawBlock(1, 5, 5,137, 70,1);
	
	for (;;) {
		ResetEvent(OSD_KEY_EVENT);
		if ( Init == TRUE ) {
			OSD_Key_Value=' ';
			SetEvent(OSD_KEY_EVENT);
		}
		Init=FALSE;
		i=WaitForSingleObject (OSD_KEY_EVENT, 3000);
		if ( i != WAIT_OBJECT_0 ) {
			DestroyOSDWindow(1);
			OSD_VOLUME=FALSE;
			Osd_Thread_Down();
			return;
		};
		if ( OSD_Key_Value == 0x00 ) { 
			DestroyOSDWindow(1);
			OSD_VOLUME=FALSE;
			Osd_Thread_Down();
			return;
		};
		
		if ( OSD_Key_Value == '0' ) { 
			DestroyOSDWindow(1);
			OSD_VOLUME=FALSE;
			Osd_Thread_Down();
			return;
		};
		
		if ( OSD_Key_Value == ' ' ) {
			if ( USE_MIXER == TRUE ) {
				i=MixerVolumeMax/127;
				xleftlen=MIXER_LINKER_KANAL/i;
				xrightlen=MIXER_RECHTER_KANAL/i;
			} else {
				xleftlen=InitialDVBVolumeLinks/2;
				xrightlen=InitialDVBVolumeRechts/2;
			}
			if ( xleftlen > 127 ) xleftlen=127;
			if ( xrightlen > 127 ) xrightlen=127;
			
			if ( altxrightlen != xrightlen ) {
				i=10+xrightlen;
				if ( altxrightlen > xrightlen ) DrawBlock(1, (unsigned short)i, 45,(unsigned short)(altxrightlen-xrightlen), 20,1);
				else DrawBlock(1, 10, 45,(unsigned short)xrightlen, 20,2);
				altxrightlen=xrightlen;
			};
			
			if ( altxleftlen != xleftlen ) {
				i=10+xleftlen;
				if ( altxleftlen > xleftlen ) DrawBlock(1, (unsigned short)i, 15,(unsigned short)(altxleftlen-xleftlen), 20,1);
				else DrawBlock(1, 10, 15,(unsigned short)xleftlen, 20,2);
				altxleftlen=xleftlen;
			};
			
		};
		
	};
};



void Update_OSD_Channel_Change( void ){
	
	if ( OSD_CHANNELCHANGE == FALSE ) {
		Create_Osd_Thread((LPTHREAD_START_ROUTINE)OSD_ChannelChange);
		OSD_CHANNELCHANGE=TRUE;
	} else {
		OSD_Key_Value = ' ';
		if ( OSD_KEY_EVENT != NULL ) SetEvent(OSD_KEY_EVENT);
	};
};


void OSD_ChannelChange( void ){
	int i;
	BOOL Init;
	int ProgNr;
	char Zeile[128];
	char AltZeile[128];
	
	PROGRAMM_OSD_RUN=TRUE;
	OSD_CHANNELCHANGE=TRUE;
	Init=TRUE;
	CreateOSDWindow( 1, BITMAP2, 640, 40);
	MoveWindowAbs(1,40,500);
	SetColorBlend(1);
	OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
	OSDSetColor(1,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
	OSDSetColor(1,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
	OSDSetColor(1,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
	DrawBlock(1, 0, 0,640, 40,0);
	AltZeile[0]='X';
	AltZeile[1]=0x00;
	
	for (;;) {
		ResetEvent(OSD_KEY_EVENT);
		if ( Init == TRUE ) {
			OSD_Key_Value=' ';
			SetEvent(OSD_KEY_EVENT);
		}
		Init=FALSE;
		i=WaitForSingleObject (OSD_KEY_EVENT, 3000);
		if ( i != WAIT_OBJECT_0 ) {
			DestroyOSDWindow(1);
			OSD_CHANNELCHANGE=FALSE;
			Osd_Thread_Down();
			return;
		};
		if ( OSD_Key_Value == 0x00 ) { 
			DestroyOSDWindow(1);
			OSD_CHANNELCHANGE=FALSE;
			Osd_Thread_Down();
			return;
		};
		
		if ( OSD_Key_Value == ' ' ) {
			
                    if (ChannelString[0] < '0' || ChannelString[0] > '9') {
			DrawBlock(1, 0, 0,640, 40,0);
			SetFont( 1, 2, 3, 0);
			WriteText( 1, 10, 4, ChannelString);
                    } else {

			i=atoi(ChannelString);
			if ( i != 0 ) {
				ProgNr= Get_Real_ProgNr(i-1);
			} else {
				ProgNr=LastProgramm ;
			};
			
			if ( ProgNr < MAXPROGS ) sprintf(Zeile,"%d   %s",Make_ProgNr(ProgNr)+1,Programm[ProgNr].Name);
			else sprintf(Zeile,t("<Schalte zurück>"));

			if ( strcmp(Zeile,AltZeile) != 0 ) {
			    DrawBlock(1, 0, 0,640, 40,0);
			    SetFont( 1, 2, 3, 0);
			    WriteText( 1, 10, 4,Zeile);
			    strcpy(AltZeile,Zeile);
			}

                    }
		};
		
	};
};


/**********************************/

void OSD_ProgInfo(){
	char Zeit[40];
	char AltZeit[40];
	
	char DisplayLine0[128];
	char DisplayLine1[128];
	char DisplayLine2[128];
	char DisplayLine3[128];
	
	char AltDisplayLine0[128];
	char AltDisplayLine1[128];
	char AltDisplayLine2[128];
	char AltDisplayLine3[128];
	
	int LastPicture;
	
	unsigned char  AudioName[81];
	unsigned char StartZeit[6];
	int ActiveAudio;
	int starty=Programm_OSD_startY;
	
	int m1,m2,s1,s2;
	
	int WaitLoops=0;
	int Nummer=CurrentProgramm;
	

	struct WinRoll_t WinRoll[4];
	int i,j;
	
	PROGRAMM_OSD_RUN=TRUE;
	
	AltZeit[0]='!';
	AltZeit[1]=0x00;
	
	Get_VideoStatus();
	LastPicture=DecoderStatus.LastPicture;
	
	memset(DisplayLine0,0x00,128);
	memset(DisplayLine1,0x00,128);
	memset(DisplayLine2,0x00,128);
	memset(DisplayLine3,0x00,128);
	memset(AltDisplayLine0,0x00,128);
	memset(AltDisplayLine1,0x00,128);
	memset(AltDisplayLine2,0x00,128);
	memset(AltDisplayLine3,0x00,128);
	
	
	for ( i = 0; i < 4 ; i++ ) {
		WinRoll[i].Created=FALSE;
	};
	
	WinRoll[0].startx=40;
	WinRoll[0].sizey=40;
	WinRoll[1].startx=40;
	WinRoll[1].sizey=28;
	WinRoll[2].startx=40;
	WinRoll[2].sizey=28;
	WinRoll[3].startx=40;
	WinRoll[3].sizey=28;
	
	
	
	Insert_WinRoll_Window(1,starty,WinRoll,4);
	//CreateOSDWindow( 1, BITMAP2, 680, 40);
	//MoveWindowAbs(1,20,500);
	
	OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
	OSDSetColor(1,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
	OSDSetColor(1,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
	OSDSetColor(1,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
	
	
	
	for ( WaitLoops=0; WaitLoops < 12 ; WaitLoops++ ) {
		
		DisplayLine0[0]=0x00;
		DisplayLine1[0]=0x00;
		DisplayLine2[0]=0x00;
		DisplayLine3[0]=0x00;
		
		if ( Play == TRUE ) {
			if ( HeaderInfo.ProgrammName[0] != 0x00 ) sprintf(DisplayLine0,"       %s",HeaderInfo.ProgrammName);
			else sprintf(DisplayLine0,t("       Unbekannte Quelle"));
		} else {
			if ( Nummer < MAXPROGS ) sprintf(DisplayLine0,"%d   %s",Make_ProgNr(Nummer)+1,Programm[Nummer].Name);
			else sprintf(DisplayLine0,"%d   %s",Make_ProgNr(LastRealProg)+1,Programm[Nummer].Name);
			
			if (( Programm[Nummer].Video_pid != 0x1fff ) ||
				( Programm[Nummer].ServiceTyp == 0 )     || 
				( Programm[Nummer].ServiceTyp == 1 )     || 
				( Programm[Nummer].ServiceTyp == 4 )     || 
				( Programm[Nummer].ServiceTyp == 5 )) {
				strcat(DisplayLine0,"   (V");
				if ( Programm[Nummer].TeleText_pid != 0x1fff )  strcat(DisplayLine0,"/T");
			} else if (( Programm[Nummer].Audio_pid != 0x1fff ) || 
				( Programm[Nummer].ServiceTyp == 2 )) {
				strcat(DisplayLine0,"   (A");
			} else {
				strcat(DisplayLine0,"   (D");
			};
			if ( Programm[Nummer].CA_ID != 0 ) {
				Get_VideoStatus();
				if ( LastPicture==DecoderStatus.LastPicture) { 
					strcat(DisplayLine0,"/S*)"); 
				} else {
					strcat(DisplayLine0,"/S)"); 
				};
			} else strcat(DisplayLine0,"/F)"); 
		};
		
		// DisplayLine0
		
		if (strcmp(DisplayLine0,AltDisplayLine0 ) != 0 ) {
			strcpy(AltDisplayLine0,DisplayLine0);
			DrawBlock(1, 0, 0,544, 40,0);
			SetFont( 1, 2, 3, 0);
			WriteText( 1, 10, 4,DisplayLine0);
		};
		// Zeit
		
		sprintf(Zeit,"%02d:%02d",RealTime(TDT.Hour),TDT.Min);
		if ( strcmp(Zeit,AltZeit) != 0 ) {
			DrawBlock(1, 545, 0,135, 40,0);
			strcpy(AltZeit,Zeit);                
			SetFont( 1, 2, 3, 0);
			WriteText( 1, 545, 4,Zeit);
		};
		
		// DisplayLine1
		
		
		
		// Line 1 
		
		if (( Record == FALSE ) && ( Play == FALSE )) {
			AudioName[0]=0x00;
			j=0;
			for ( i = 0 ; i < Programm[Nummer].Filteranzahl; i++ ) {
				if (( Programm[Nummer].Filters[i].FilterId == 0x03 ) || 
					( Programm[Nummer].Filters[i].FilterId == 0x04 )) j++;
			};
			
			
			if ( j > 1 ) {
				ActiveAudio=FindActiveAudioName(Nummer,(unsigned char *)&AudioName);
				if ( AudioName[0] == 0x00 ) sprintf(AudioName,"Audio : 0x%04x",Programm[Nummer].Audio_pid);
				strcat(DisplayLine1,AudioName);
			};
			
			
			if (Is_Time_Link(Programm[Nummer].SID_pid, Programm[Nummer].tp_id,TRUE) == TRUE ) {
				if ( strlen(DisplayLine1) > 0 ) strcat(DisplayLine1," / ");
				strcat(DisplayLine1,t("Time-Shift"));
			};
			
			if (Is_View_Link(Programm[Nummer].SID_pid, Programm[Nummer].tp_id,TRUE) == TRUE ) {
				if ( strlen(DisplayLine1) > 0 ) strcat(DisplayLine1," / ");
				strcat(DisplayLine1,t("Extra-Kanal"));
			};
		} else if ( Record == TRUE ) {
			strcpy(DisplayLine1,t("Aufnahme"));
			if ( Recorder_Pause == TRUE ) strcpy(DisplayLine1,t("Aufnahme - Pause"));
		} else if ( Play == TRUE ) {
			strcpy(DisplayLine1,t("Wiedergabe"));
			if ( Recorder_Pause == TRUE ) strcpy(DisplayLine1,t("Wiedergabe - Pause"));
		};
		
		
		
		
		if (strcmp( DisplayLine1,AltDisplayLine1) != 0 ) {
			strcpy(AltDisplayLine1,DisplayLine1);
			
			if ( WinRoll[1].Created == FALSE ) {
				Insert_WinRoll_Window(2,starty,WinRoll,4);
				OSDSetColor(2,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
				OSDSetColor(2,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
				OSDSetColor(2,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
				OSDSetColor(2,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
			};
			DrawBlock(2, 0, 0,70, 28,0);
			DrawBlock(2, 70, 0,610, 28,1);
			SetFont( 2, 1, 3, 1);
			WriteText( 2, 85, 0,DisplayLine1);
			
		};
		
		// Line 2
		
		if ( Play == TRUE ) {
			StartZeit[0]=0x00;
			i=0;
			DisplayLine2[i]=0x00;
			while (( i < 40 ) && ( HeaderInfo.Infos[i] >=0x20 )) {
				DisplayLine2[i]=HeaderInfo.Infos[i];
				i++;
				DisplayLine2[i]=0x00;
			};
			if ( DisplayLine2[0] == 0x00 ) strcpy(DisplayLine2,t("Titel der Aufnahme ist unbekannt"));
			
		} else {
			GetCurrentNext(Programm[Nummer].SID_pid,Programm[Nummer].tp_id,2,(unsigned char *)&DisplayLine2,(unsigned char *)&StartZeit);
		};

		if ( strcmp(DisplayLine2,AltDisplayLine2) != 0 ) {
			strcpy(AltDisplayLine2,DisplayLine2);
			if ( WinRoll[2].Created == FALSE ) {
				Insert_WinRoll_Window(3,starty,WinRoll,4);
				OSDSetColor(3,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
				OSDSetColor(3,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
				OSDSetColor(3,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
				OSDSetColor(3,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
			};
			DrawBlock(3, 0, 0,70, 28,0);
			DrawBlock(3, 70, 0,610, 28,1);
			if ( StartZeit[0] != 0x00 ) {
				SetFont( 3, 1, 2, 0);
				WriteText( 3, 1, 0,StartZeit);
			};
			SetFont( 3, 1, 3, 1);
			WriteText( 3, 85, 0,DisplayLine2);
		};
		
		// Line 3 
		StartZeit[0] =0x00;
		DisplayLine3[0]=0x00;
		
		if ( Play == TRUE ) {
			m1=(HeaderInfo.SpielZeit/60);
			s1=HeaderInfo.SpielZeit-(m1*60);
			m2=(HeaderInfo.CurrentSpielZeit/60);
			s2=HeaderInfo.CurrentSpielZeit-(m2*60);
			sprintf(DisplayLine3,t("%s   %02d:%02d von %02d:%02d"),HeaderInfo.TypeName,m2,s2,m1,s1);
		} else if ( Record == TRUE ) {
			m2=(HeaderInfo.CurrentSpielZeit/60);
			s2=HeaderInfo.CurrentSpielZeit-(m2*60);
			sprintf(DisplayLine3,t("%s   %02d:%02d     ( In dieser Datei %d MB )"),HeaderInfo.TypeName,m2,s2,HeaderInfo.MegaBytes_Recorded);
		} else {
			GetCurrentNext(Programm[Nummer].SID_pid,Programm[Nummer].tp_id,1,(unsigned char *)&DisplayLine3,(unsigned char *)&StartZeit);
		};
		
		
		if ( strcmp(DisplayLine3,AltDisplayLine3) != 0 ) {
			strcpy(AltDisplayLine3,DisplayLine3);
			
			if ( WinRoll[2].Created == FALSE ) {
				Insert_WinRoll_Window(3,starty,WinRoll,4);
				OSDSetColor(3,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
				OSDSetColor(3,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
				OSDSetColor(3,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
				OSDSetColor(3,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
				DrawBlock(3, 0, 0,70, 28,0);
				DrawBlock(3, 70, 0,610, 28,1);
			};
			
			
			if ( WinRoll[3].Created == FALSE ) {
				Insert_WinRoll_Window(4,starty,WinRoll,4);
				OSDSetColor(4,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
				OSDSetColor(4,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
				OSDSetColor(4,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
				OSDSetColor(4,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
			};
			DrawBlock(4, 0, 0,70, 28,0);
			DrawBlock(4, 70, 0,610, 28,1);
			
			if ( StartZeit[0] != 0x00 ) {
				SetFont( 4, 1, 2, 0);
				WriteText( 4, 1, 0,StartZeit);
			};
			SetFont( 4, 1, 3, 1);
			WriteText( 4, 85, 0,DisplayLine3); 
		};
		
		
		ResetEvent(OSD_KEY_EVENT);
		i=WaitForSingleObject (OSD_KEY_EVENT, 500 );
		if ( i != WAIT_TIMEOUT ) {
			for ( i=0; i < 4; i++ ) {
				if ( WinRoll[i].Created == TRUE ) DestroyOSDWindow((unsigned char )(i+1));
			};
			Osd_Thread_Down();
			return;
		};
		
};

for ( i=0; i < 4; i++ ) {
	if ( WinRoll[i].Created == TRUE ) DestroyOSDWindow((unsigned char )(i+1));
};
Osd_Thread_Down();
}


void Update_OSD_Color( void ){
	if ( OSD_COLOR == FALSE ) {
		Create_Osd_Thread((LPTHREAD_START_ROUTINE)OSD_Color_Setting_Proc);
		OSD_COLOR=TRUE;
	} else {
		OSD_Key_Value = ' ';
		if ( OSD_KEY_EVENT != NULL ) SetEvent(OSD_KEY_EVENT);
	};
};


void OSD_Color_Setting_Proc( void ){
	char Zeile[255];
	struct OSD_Colors_t Save_OSD_Colors[4];
	int i;
	unsigned char a;
	
	PROGRAMM_OSD_RUN=TRUE;
	OSD_COLOR=TRUE;
	CreateOSDWindow( 1, BITMAP2, 400, 200);
	MoveWindowAbs(1,160,200);
	SetColorBlend(1);
	memcpy(&Save_OSD_Colors[0],&OSD_Colors[0],sizeof(struct OSD_Colors_t));
	memcpy(&Save_OSD_Colors[1],&OSD_Colors[1],sizeof(struct OSD_Colors_t));
	memcpy(&Save_OSD_Colors[2],&OSD_Colors[2],sizeof(struct OSD_Colors_t));
	memcpy(&Save_OSD_Colors[3],&OSD_Colors[3],sizeof(struct OSD_Colors_t));
	OSDSetColor(1,0, Save_OSD_Colors[0].R, Save_OSD_Colors[0].G, Save_OSD_Colors[0].B ,Save_OSD_Colors[0].Blend);
	OSDSetColor(1,1, Save_OSD_Colors[1].R, Save_OSD_Colors[1].G, Save_OSD_Colors[1].B, Save_OSD_Colors[1].Blend);
	OSDSetColor(1,2, Save_OSD_Colors[2].R, Save_OSD_Colors[2].G, Save_OSD_Colors[2].B, Save_OSD_Colors[2].Blend);
	OSDSetColor(1,3, Save_OSD_Colors[3].R, Save_OSD_Colors[3].G, Save_OSD_Colors[3].B, Save_OSD_Colors[3].Blend);
	DrawBlock(1, 0, 0,400, 50,0);
	SetFont( 1, 2, 2, 0);
	strcpy(Zeile,t("TextFarbe 1 HinterGrund 1"));
	WriteText( 1, 10, 9,Zeile);
	DrawBlock(1, 0, 50,400, 50,1);
	SetFont( 1, 2, 2, 1);
	strcpy(Zeile,t("TextFarbe 1 HinterGrund 2"));
	WriteText( 1, 10, 59,Zeile);
	DrawBlock(1, 0, 100,400, 50,0);
	SetFont( 1, 2, 3, 0);
	strcpy(Zeile,t("TextFarbe 2 HinterGrund 1"));
	WriteText( 1, 10, 109,Zeile);
	DrawBlock(1, 0, 150,400, 50,1);
	SetFont( 1, 2, 3, 1);
	strcpy(Zeile,t("TextFarbe 2 HinterGrund 2"));
	WriteText( 1, 10, 159,Zeile);
	
	for (;;) {
		i=WaitForSingleObject (OSD_KEY_EVENT, INFINITE);
		if ( i != WAIT_OBJECT_0 ) {
			DestroyOSDWindow(1);
			OSD_COLOR=FALSE;
			Osd_Thread_Down();
			return;
		};
		if ( OSD_Key_Value == 0x00 ) { 
			DestroyOSDWindow(1);
			OSD_COLOR=FALSE;
			Osd_Thread_Down();
			return;
		};
		
		if ( OSD_Key_Value == '0' ) { 
			DestroyOSDWindow(1);
			OSD_COLOR=FALSE;
			Osd_Thread_Down();
			return;
		};
		
		if ( OSD_Key_Value == ' ' ) {
			for ( a=0; a < 4 ; a++ ) {
				if (( Save_OSD_Colors[a].R != OSD_Colors[a].R ) || 
					( Save_OSD_Colors[a].G != OSD_Colors[a].G ) || 
					( Save_OSD_Colors[a].B != OSD_Colors[a].B ) || 
					( Save_OSD_Colors[a].Blend != OSD_Colors[a].Blend )) { 
					memcpy(&Save_OSD_Colors[a],&OSD_Colors[a],sizeof(struct OSD_Colors_t));
					OSDSetColor(1, (unsigned char)a, Save_OSD_Colors[a].R, Save_OSD_Colors[a].G, Save_OSD_Colors[a].B ,Save_OSD_Colors[a].Blend);
				};
			};
			
		};
	};
};



void OSD_Eit_Info( void ){
	int ListCount=0;
	int Nummer=CurrentProgramm;
	
	unsigned char Text[128][70];
	
	struct Sort_List_t {
		unsigned int DatumZeit;
		unsigned short Eit_Index_Id;
		unsigned short Eit_Index_Offset;
	};
    struct Sort_List_t	Save;
	struct Sort_List_t	Sort_List[EIT_INDEX_SIZE];
	
	int size = sizeof(struct Sort_List_t);
	
	BOOL Austausch;
    int i,j,k;
    int ActiveSel=-1;
	
    int Virtual_Event_Id;
	int Virtual_Offset_Id;
	
	int typ=0;
    int Lines;
	
    char StartZeit[12];
    char Dauer[12];
    char Datum[12];
	
	for ( i = 0 ; i < Current_Eit_Index_Size; i++ ) {
		if ( EIT[i].Anzahl > 0 ) {
			for ( j = 0 ; j < EIT[i].Anzahl; j++ ) {
				if (( EIT[i].EIT_Infos[j].SID_Id == Programm[Nummer].SID_pid ) &&
					( EIT[i].EIT_Infos[j].ts_id == Programm[Nummer].tp_id )) {
					Sort_List[ListCount].DatumZeit=(EIT[i].EIT_Infos[j].Date_Id << 16 ) + (EIT[i].EIT_Infos[j].Start_Zeit_h<<8)+EIT[i].EIT_Infos[j].Start_Zeit_m; 
					Sort_List[ListCount].Eit_Index_Id=i;
					Sort_List[ListCount].Eit_Index_Offset=j;
					ListCount++;
				};
			}; 
		};
	};
	
	
	
	if ( ListCount == 0 ) {
		Osd_Thread_Down();
		return;
	};
	
	PROGRAMM_OSD=TRUE;
	
	
			 j=ListCount;
			 Austausch = TRUE ;
			 
			 while ( Austausch == TRUE ) {
				 Austausch = FALSE ;
				 j--;
                 i=0;
				 while ( i < j ) {
					 if ( Sort_List[i].DatumZeit > Sort_List[i+1].DatumZeit ) {
						 Austausch = TRUE;
						 memcpy(&Save.DatumZeit, &Sort_List[i+1].DatumZeit , size );
						 memcpy(&Sort_List[i+1].DatumZeit, &Sort_List[i].DatumZeit , size );
						 memcpy(&Sort_List[i].DatumZeit,&Save.DatumZeit , size );
                     };
					 i++;
				 };
			 }
			 
			 
			 i=0;                  // ggf aktive Sendung ??
			 while ( i < ListCount ) {
				 if ( EIT[Sort_List[i].Eit_Index_Id].EIT_Infos[Sort_List[i].Eit_Index_Offset].Running_Status ==  2) ActiveSel=i;
				 i++;
			 };
			 
			 if ( ActiveSel < 0 )  ActiveSel=0;
			 
			 // Änderung: oxident
			 // Sinn und Zweck: Programm-Info (Detail) Fenster 
			 //                 für manche Fernseher falsch dimensioniert
			 // Original:
			 // CreateOSDWindow( 1, BITMAP1, 690, 372);
			 // MoveWindowAbs(1,15,10 );
			 CreateOSDWindow( 1, BITMAP1, 640, 372);
			 MoveWindowAbs(1,45,25 );
			 // ---
			 SetColorBlend(1);
			 OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
			 OSDSetColor(1,1, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
			 while (ActiveSel>= 0 ) {   // Loop über Nachrichten 
				 EIT_TopLine=0;
                 Date_To_String((unsigned short)RealDay(EIT[Sort_List[ActiveSel].Eit_Index_Id].EIT_Infos[Sort_List[ActiveSel].Eit_Index_Offset].Start_Zeit_h,
                                        EIT[Sort_List[ActiveSel].Eit_Index_Id].EIT_Infos[Sort_List[ActiveSel].Eit_Index_Offset].Date_Id),Datum);
				 
				 if ( EIT[Sort_List[ActiveSel].Eit_Index_Id].EIT_Infos[Sort_List[ActiveSel].Eit_Index_Offset].Running_Status == 2  ) strcpy(Datum,t("Jetzt"));
				 else if ( EIT[Sort_List[ActiveSel].Eit_Index_Id].EIT_Infos[Sort_List[ActiveSel].Eit_Index_Offset].Running_Status == 1  ) strcpy(Datum,t("Nächstes"));
				 
				 sprintf(StartZeit,"%02d:%02d:%02d",RealTime(EIT[Sort_List[ActiveSel].Eit_Index_Id].EIT_Infos[Sort_List[ActiveSel].Eit_Index_Offset].Start_Zeit_h),EIT[Sort_List[ActiveSel].Eit_Index_Id].EIT_Infos[Sort_List[ActiveSel].Eit_Index_Offset].Start_Zeit_m,EIT[Sort_List[ActiveSel].Eit_Index_Id].EIT_Infos[Sort_List[ActiveSel].Eit_Index_Offset].Start_Zeit_s);
				 sprintf(Dauer,"(%4d Min)",EIT[Sort_List[ActiveSel].Eit_Index_Id].EIT_Infos[Sort_List[ActiveSel].Eit_Index_Offset].Dauer_h*60+EIT[Sort_List[ActiveSel].Eit_Index_Id].EIT_Infos[Sort_List[ActiveSel].Eit_Index_Offset].Dauer_m);
				 DrawBlock(1, 0, 0,690, 28,1);
				 DrawBlock(1, 0, 28,690, 140,0);
				 SetFont( 1, 1, 2, 1);
				 WriteText( 1, 94, 0,Datum);
				 WriteText( 1, 314, 0,StartZeit);
				 WriteText( 1, 518, 0,Dauer);
				 
				 memset(&Text[0][0],0x00,128*70);
				 Lines=0;
				 
				 i=0;
				 j=0;
				 k=0;
				 
				 
				 Virtual_Event_Id=Sort_List[ActiveSel].Eit_Index_Id;
				 Virtual_Offset_Id=Sort_List[ActiveSel].Eit_Index_Offset;
				 
				 if (( EIT[Sort_List[ActiveSel].Eit_Index_Id].EIT_Infos[Sort_List[ActiveSel].Eit_Index_Offset].virtual_eventid != 0xffff ) &&
					 ( EIT[Sort_List[ActiveSel].Eit_Index_Id].EIT_Infos[Sort_List[ActiveSel].Eit_Index_Offset].virtual_sid != 0xffff)) {
					 i=Get_Virtual_Event(EIT[Sort_List[ActiveSel].Eit_Index_Id].EIT_Infos[Sort_List[ActiveSel].Eit_Index_Offset].virtual_eventid,EIT[Sort_List[ActiveSel].Eit_Index_Id].EIT_Infos[Sort_List[ActiveSel].Eit_Index_Offset].virtual_sid);
					 if ( i >= 0 ) {
						 Virtual_Event_Id=EIT[Sort_List[ActiveSel].Eit_Index_Id].EIT_Infos[Sort_List[ActiveSel].Eit_Index_Offset].virtual_eventid;
						 Virtual_Offset_Id=i;
					 }
				 }
				 
				 if ( EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].HeaderSize > 0 ) {
					 while ( EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Header[i] != 0x00 ) {
						 Text[Lines][j] = EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Header[i];
						 Text[Lines][j+1]=0x00;
						 if ( Text[Lines][j] < 0x20 ) { 
							 if ( Text[Lines][j] == 0x0a ) {
								 Text[Lines][j]=0x00;
								 Lines++;
								 j=-1;
								 k=0;
							 } else {
								 Text[Lines][j]=0x00;
								 j--;
								 k--;
							 };
						 };
						 if ( k > 45 ) {
							 if ( Text[Lines][j]==0x20 ) {
								 Text[Lines][j]=0x00;
								 Lines++;
								 j=-1;
								 k=0;
							 }
						 };
						 k++;
						 j++;
						 i++;
					 };
					 
				 };
				 Text[Lines][j]=0x00;
				 Lines++;
				 
				 if ( EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].TextSize > 0 ) {
					 i=0;
					 j=0;
					 k=0;
					 while ( EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Text[i] != 0x00 ) {
						 Text[Lines][j] = EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Text[i];
						 Text[Lines][j+1]=0x00;
						 if ( Text[Lines][j] < 0x20 ) { 
							 if ( Text[Lines][j] == 0x0a ) {
								 Text[Lines][j]=0x00;
								 Lines++;
								 j=-1;
								 k=0;
							 } else {
								 Text[Lines][j]=0x00;
								 j--;
								 k--;
							 };
						 };
						 if ( k > 45 ) {
							 if ( Text[Lines][j]==0x20 ) {
								 Text[Lines][j]=0x00;
								 Lines++;
								 j=-1;
								 k=0;
							 }
						 };
						 k++;
						 j++;
						 i++;
					 };
					 
					 
					 Text[Lines][j]=0x00;
					 Lines++;
					 
				 };
				 
				 
				 
				 
				 while ( EIT_TopLine >= 0 ) { // Blättern
					 
					 DrawBlock(1, 0, 28,690, 344,0);
					 SetFont( 1, 1, 1, 0);
					 i=0;
					 
					 j=0;
					 k=28;
					 if (( i+ EIT_TopLine ) == 0 ) {
						 SetFont( 1, 2, 1, 0);
						 k=34;
						 j=1;
					 };
					 
					 while (( i < 13 ) && ( i+ EIT_TopLine < Lines)) {
						 WriteText(1,4,(unsigned short)(k+(i*28)),&Text[i+ EIT_TopLine][0]);
						 if ( j == 1 ) {
							 SetFont( 1, 1, 1, 0);
							 j=0;
						 }
						 i++;
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
						 
						 if ( OSD_Key_Value == '0' ) {; 
						 DestroyOSDWindow(1);
						 Osd_Thread_Down();
						 return;
						 };
						 
						 if ( OSD_Key_Value == '2' ) {; 
						 if ( 13+EIT_TopLine < Lines ) {
							 EIT_TopLine++;
							 j=1;
						 };
						 } else if ( OSD_Key_Value == '1' ) {
							 j=EIT_TopLine;
							 EIT_TopLine=Lines-13;
							 if ( EIT_TopLine < 0 ) EIT_TopLine=0;
							 if ( j != EIT_TopLine ) j= 1;
							 else j=0;
						 } else if ( OSD_Key_Value == '7' ) { 
							 if ( EIT_TopLine > 0) {
								 EIT_TopLine=0;
								 j=1;
							 };
						 } else if ( OSD_Key_Value == '8' ) { 
							 if ( EIT_TopLine > 0) {
								 EIT_TopLine--;
								 j=1;
							 };
						 } else if ( OSD_Key_Value == '9' ) { 
							 if ( EIT_TopLine > 0 ) {
								 EIT_TopLine-=13;
								 if ( EIT_TopLine<0 ) EIT_TopLine=0;
								 j=1;
							 };
						 } else if ( OSD_Key_Value == '3' ) { 
							 if ( EIT_TopLine < Lines ) {
								 j=EIT_TopLine;
								 EIT_TopLine+=13;
								 if ( EIT_TopLine>Lines ) EIT_TopLine=Lines-13;
								 if ( EIT_TopLine<0 ) EIT_TopLine=0;
								 if ( j != EIT_TopLine ) j=1;
								 else j=0;
							 };
							 
						 } else if ( OSD_Key_Value == '6' ) {; 
						 if ( ActiveSel+1 < ListCount ) {
							 EIT_TopLine=-1;
							 ActiveSel++;
							 j=1;
						 };
						 } else if ( OSD_Key_Value == '4' ) {; 
						 if ( ActiveSel-1 > 0) {
							 j=1;
							 EIT_TopLine=-1;
							 ActiveSel--;
						 };
						 };
					 };
				 } // Blättern Ende
		 }; // While ActiveSel;
		 
		 
}


void OSD_Eit_Liste_Current_Programm( void ){
	
	int Nummer=CurrentProgramm;
	int ListCount=0;
	
	unsigned char Text[70];
	
	int i_id,o_id;
	
	int Virtual_Event_Id,Virtual_Offset_Id;
	
	
	struct Sort_List_t {
		unsigned int DatumZeit;
		unsigned short Eit_Index_Id;
		unsigned short Eit_Index_Offset;
	};
	
    struct Sort_List_t	Save;
	struct Sort_List_t	Sort_List[EIT_INDEX_SIZE];
	
	int size = sizeof(struct Sort_List_t);
	
	BOOL Austausch;
    int i,j,k;
    int ActiveSel=-1;
	
	int typ=0;
    int Lines;
	
    char StartZeit[12];
    char Datum[12];
	
	
	int Previous_Current_Now = FALSE;			// vorheriges "jetzt"
	int Previous_Next_Now    = FALSE;			// vorheriges "danach"
	int Merker_i = 0;
	int Merker_j = 0;

	for ( i = 0 ; i < Current_Eit_Index_Size; i++ )
	{
		if ( EIT[i].Anzahl > 0 )
		{
			for ( j = 0 ; j < EIT[i].Anzahl; j++ )
			{
				if (( EIT[i].EIT_Infos[j].SID_Id == Programm[Nummer].SID_pid ) &&
					( EIT[i].EIT_Infos[j].ts_id == Programm[Nummer].tp_id ))
				{
					Sort_List[ListCount].DatumZeit=
						(EIT[i].EIT_Infos[j].Date_Id << 16 ) + (EIT[i].EIT_Infos[j].Start_Zeit_h<<8)+EIT[i].EIT_Infos[j].Start_Zeit_m; 
					Sort_List[ListCount].Eit_Index_Id=i;
					Sort_List[ListCount].Eit_Index_Offset=j;
					ListCount++;
				};
			}; 
		};
	};
	
	
	
	
	if ( ListCount == 0 ) {
		Osd_Thread_Down();
		return;
	};
	
	
	PROGRAMM_OSD=TRUE;
	
	
			 j=ListCount;
			 Austausch = TRUE ;
			 
			 while ( Austausch == TRUE ) {
				 Austausch = FALSE ;
				 j--;
                 i=0;
				 while ( i < j ) {
					 if ( Sort_List[i].DatumZeit > Sort_List[i+1].DatumZeit ) {
						 Austausch = TRUE;
						 memcpy(&Save.DatumZeit, &Sort_List[i+1].DatumZeit , size );
						 memcpy(&Sort_List[i+1].DatumZeit, &Sort_List[i].DatumZeit , size );
						 memcpy(&Sort_List[i].DatumZeit,&Save.DatumZeit , size );
                     };
					 i++;
				 };
			 }
			 
				// BigReaper

				// fp = fopen ("D:\\EIT_LISTE.TXT", "a");
				Previous_Current_Now = FALSE;
				Previous_Next_Now    = FALSE;

				i=0;                  // ggf aktive Sendung ??
				while ( i < ListCount )
				{
					// Doppeltes "jetzt" rausfiltern:
					if (EIT[Sort_List[i].Eit_Index_Id].EIT_Infos[Sort_List[i].Eit_Index_Offset].Running_Status == 2)
					{
						if (!Previous_Current_Now)
						{
							Previous_Current_Now = TRUE;
							Merker_i = i;
						}
						else
						{
							if (Previous_Current_Now)
							{
								EIT[Sort_List[Merker_i].Eit_Index_Id].EIT_Infos[Sort_List[Merker_i].Eit_Index_Offset].Running_Status = 0;
								Merker_i = i;
							}
						}
					}

					// Doppeltes "danach" rausfiltern:
					if (EIT[Sort_List[i].Eit_Index_Id].EIT_Infos[Sort_List[i].Eit_Index_Offset].Running_Status == 1)
					{
						if (!Previous_Next_Now)
						{
							Previous_Next_Now = TRUE;
							Merker_j = i;
						}
						else
						{
							if (Previous_Next_Now)
							{
								EIT[Sort_List[Merker_j].Eit_Index_Id].EIT_Infos[Sort_List[Merker_j].Eit_Index_Offset].Running_Status = 0;
								Merker_j = i;
							}
						}
					}

					i++;
				};
			 
			 i=0;                  // ggf aktive Sendung ??
			 while (( i < ListCount ) && ( ActiveSel == -1 )) {
				 if ( EIT[Sort_List[i].Eit_Index_Id].EIT_Infos[Sort_List[i].Eit_Index_Offset].Running_Status ==  2) ActiveSel=i;
				 i++;
			 };
			 
			 if ( (EIT_TopLine + 13 ) >= ListCount )  EIT_TopLine = ListCount-13;
			 if ( EIT_TopLine < 0 )  EIT_TopLine=0;
			 
			 
			 CreateOSDWindow( 1, BITMAP1, 690, 372);
			 MoveWindowAbs(1,15,10 );
			 SetColorBlend(1);
			 OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
			 OSDSetColor(1,1, OSD_Colors[3-1].R, OSD_Colors[3-1].G, OSD_Colors[3-1].B, OSD_Colors[3-1].Blend);
			 for (;;) {
				 ActiveSel=0;
				 DrawBlock(1, 0, 0,690, 372,0);
				 while (( EIT_TopLine>= 0 ) && (EIT_TopLine+ActiveSel <ListCount) && (ActiveSel < 13))  {   // Loop über Nachrichten 
					 
					 i_id=Sort_List[EIT_TopLine+ActiveSel].Eit_Index_Id;
					 o_id=Sort_List[EIT_TopLine+ActiveSel].Eit_Index_Offset;
					 
					 i=EIT[i_id].EIT_Infos[o_id].Date_Id-TDT.Date_Id;

					 if ((i > -3) && (i < 3))			// gültiger Bereich ?
					 {
					 if ( i == -2 ) sprintf(Datum,t("vorg."));
					 else if ( i == -1 ) sprintf(Datum,t("gestern"));
					 else if ( i == 0 ) sprintf(Datum,t("heute"));
					 else if ( i == 1 ) sprintf(Datum,t("morgen"));
					 else if ( i == 2 ) sprintf(Datum,t("überm."));
					 
					 if ( EIT[i_id].EIT_Infos[o_id].Running_Status == 2  ) strcpy(Datum,t("Jetzt"));
					 else if ( EIT[i_id].EIT_Infos[o_id].Running_Status == 1  ) strcpy(Datum,t("Danach"));
					 
					 SetFont( 1, 1, 1, 0);
					 WriteText( 1, 4, (unsigned short)(ActiveSel*28),Datum);
					 sprintf(StartZeit,"%02d:%02d",RealTime(EIT[i_id].EIT_Infos[o_id].Start_Zeit_h),EIT[i_id].EIT_Infos[o_id].Start_Zeit_m);
					 WriteText( 1, 94, (unsigned short)(ActiveSel*28),StartZeit);
					 
					 
					 Lines=0;
					 
					 i=0;
					 j=0;
					 k=0;
					 
					 
					 Virtual_Event_Id=i_id;
					 Virtual_Offset_Id=o_id;
					 
					 if (( EIT[i_id].EIT_Infos[o_id].virtual_eventid != 0xffff ) &&
							 ( EIT[i_id].EIT_Infos[o_id].virtual_sid != 0xffff ))
						{
						 i=Get_Virtual_Event(EIT[i_id].EIT_Infos[o_id].virtual_eventid,EIT[i_id].EIT_Infos[o_id].virtual_sid);
							if ( i >= 0 )
							{
							 Virtual_Event_Id=EIT[i_id].EIT_Infos[o_id].virtual_eventid;
							 Virtual_Offset_Id=i;
						 }
					 }
					 
						if ( EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].HeaderSize > 0 )
						{
							while (( EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Header[i] != 0x00 ) && ( k < 50))
							{
							 Text[j] = EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Header[i];
							 Text[j+1]=0x00;
								if ( Text[j] < 0x20 )
								{ 
									if ( Text[j] == 0x0a )
									{
									 Text[j]='-';
									} else
									{
									 Text[j]=0x00;
									 j--;
									 k--;
								 };
							 };
								if ( k > 45 )
								{
									if ( Text[j]==0x20 )
									{
									 Text[j]=0x00;
									 k=99;
								 }
							 };
							 k++;
							 j++;
							 i++;
						 };
						 
					 };
					 Text[j]=0x00;
					 
					 WriteText( 1, 164, (unsigned short)(ActiveSel*28),Text);
					 
					 };	// end of gültiger Bereich
					 ActiveSel++;
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
					 
					 if ( OSD_Key_Value == '0' ) {; 
					 DestroyOSDWindow(1);
					 Osd_Thread_Down();
					 return;
					 };
					 
					 if ( OSD_Key_Value == '2' ) {; 
					 if ( 13+EIT_TopLine < ListCount ) {
						 EIT_TopLine++;
						 j=1;
					 };
					 } else if ( OSD_Key_Value == '1' ) {
						 j=EIT_TopLine;
						 EIT_TopLine=ListCount-13;
						 if ( EIT_TopLine < 0 ) EIT_TopLine=0;
						 if ( j != EIT_TopLine ) j= 1;
						 else j=0;
					 } else if ( OSD_Key_Value == '7' ) { 
						 if ( EIT_TopLine > 0) {
							 EIT_TopLine=0;
							 j=1;
						 };
					 } else if ( OSD_Key_Value == '8' ) {; 
					 if ( EIT_TopLine > 0) {
						 EIT_TopLine--;
						 j=1;
					 };
					 } else if ( OSD_Key_Value == '9' ) {; 
					 if ( EIT_TopLine > 0 ) {
						 EIT_TopLine-=13;
						 if ( EIT_TopLine<0 ) EIT_TopLine=0;
						 j=1;
					 };
					 } else if ( OSD_Key_Value == '3' ) { 
						 if ( EIT_TopLine < ListCount) {
							 j=EIT_TopLine;
							 EIT_TopLine+=13;
							 if ( EIT_TopLine>=ListCount ) EIT_TopLine=ListCount-13;
							 if ( EIT_TopLine<0 ) EIT_TopLine=0;
							 if ( j != EIT_TopLine ) j=1;
							 else j=0;
						 };
						 
					 };
					 
					 
				 };
		 }; // While ActiveSel;
		 
		 
}


void OSD_Eit_Liste_Current_Next( void ){
	
	int Nummer=CurrentProgramm;
	int ListCount=0;
	
	unsigned char Text[70];
	
	
	struct Sort_List_t {
		unsigned short ProgrammNummer;
   	    unsigned int DatumZeit;
		unsigned short Eit_Index_Id;
		unsigned short Eit_Index_Offset;
	};
    struct Sort_List_t	Save;
	struct Sort_List_t	Sort_List[EIT_INDEX_SIZE];
	
	int size = sizeof(struct Sort_List_t);
	
	int i_id,o_id;
    int Virtual_Event_Id,Virtual_Offset_Id;
	
	BOOL Austausch;
    int i,j,k;
    int ActiveSel=-1;
	
	int typ=0;
    int Lines;
	
    char StartZeit[12];
	
	for ( i = 0 ; i < Current_Eit_Index_Size; i++ ) {
		if ( EIT[i].Anzahl > 0 ) {
			for ( j = 0 ; j < EIT[i].Anzahl; j++ ) {
				if (( EIT[i].EIT_Infos[j].Running_Status > 0 )  &&
					( abs(EIT[i].EIT_Infos[j].Date_Id-TDT.Date_Id) < 3 )) {
					k=Get_Sender(EIT[i].EIT_Infos[j].SID_Id,EIT[i].EIT_Infos[j].ts_id,NULL);
					if ( IsValidSelect(k) == TRUE ) {
						Sort_List[ListCount].DatumZeit=(EIT[i].EIT_Infos[j].Date_Id << 16 ) + (EIT[i].EIT_Infos[j].Start_Zeit_h<<8)+EIT[i].EIT_Infos[j].Start_Zeit_m; 
						Sort_List[ListCount].Eit_Index_Id=i;
						Sort_List[ListCount].Eit_Index_Offset=j;
						Sort_List[ListCount].ProgrammNummer=k;
						ListCount++;
					};
				};
			}; 
		};
	};
	
	
	
	if ( ListCount == 0 ) {
        Osd_Thread_Down();
		return;
	};
	
	PROGRAMM_OSD=TRUE;
	
	
	j=ListCount;
			 Austausch = TRUE ;
			 while ( Austausch == TRUE ) {
				 Austausch = FALSE ;
				 j--;
                 i=0;
				 while ( i < j ) {
					 
					 if ( Sort_List[i].ProgrammNummer > Sort_List[i+1].ProgrammNummer ) {
						 Austausch = TRUE;
						 memcpy(&Save.ProgrammNummer, &Sort_List[i+1].ProgrammNummer , size );
						 memcpy(&Sort_List[i+1].ProgrammNummer, &Sort_List[i].ProgrammNummer , size );
						 memcpy(&Sort_List[i].ProgrammNummer,&Save.ProgrammNummer , size );
					 } else if ( Sort_List[i].ProgrammNummer == Sort_List[i+1].ProgrammNummer ) {
						 if ( Sort_List[i].DatumZeit > Sort_List[i+1].DatumZeit ) {
							 Austausch = TRUE;
							 memcpy(&Save.ProgrammNummer, &Sort_List[i+1].ProgrammNummer , size );
							 memcpy(&Sort_List[i+1].ProgrammNummer, &Sort_List[i].ProgrammNummer , size );
							 memcpy(&Sort_List[i].ProgrammNummer,&Save.ProgrammNummer , size );
						 };
					 };
					 i++;
				 };
			 }
			 
			 
			 
			 EIT_TopLine=0;
			 
			 CreateOSDWindow( 1, BITMAP1, 690, 372);
			 MoveWindowAbs(1,15,10 );
			 SetColorBlend(1);
			 OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
			 OSDSetColor(1,1, OSD_Colors[3-1].R, OSD_Colors[3-1].G, OSD_Colors[3-1].B, OSD_Colors[3-1].Blend);
			 for (;;) {
				 ActiveSel=0;
				 DrawBlock(1, 0, 0,690, 372,0);
				 
				 while (( EIT_TopLine>= 0 ) && (EIT_TopLine+ActiveSel <ListCount) && (ActiveSel < 13))  {   // Loop über Nachrichten 
					 SetFont( 1, 1, 1, 0);
					 
					 i_id=Sort_List[EIT_TopLine+ActiveSel].Eit_Index_Id;
					 o_id=Sort_List[EIT_TopLine+ActiveSel].Eit_Index_Offset;
					 
					 WriteText( 1,  4, (unsigned short)(ActiveSel*28),Programm[Sort_List[EIT_TopLine+ActiveSel].ProgrammNummer].Name);
					 DrawBlock(1, 180, (unsigned short)(ActiveSel*28),40, 28,0);
					 
					 sprintf(StartZeit,"%02d:%02d",RealTime(EIT[i_id].EIT_Infos[o_id].Start_Zeit_h),EIT[i_id].EIT_Infos[o_id].Start_Zeit_m);
					 WriteText(1, 186, (unsigned short)(ActiveSel*28),StartZeit);
					 
					 
					 Lines=0;
					 
					 i=0;
					 j=0;
					 k=0;
					 
					 Virtual_Event_Id=i_id;
					 Virtual_Offset_Id=o_id;
					 
					 if (( EIT[i_id].EIT_Infos[o_id].virtual_eventid != 0xffff ) &&
						 ( EIT[i_id].EIT_Infos[o_id].virtual_sid != 0xffff )) {
						 i=Get_Virtual_Event(EIT[i_id].EIT_Infos[o_id].virtual_eventid,EIT[i_id].EIT_Infos[o_id].virtual_sid);
						 if ( i >= 0 ) {
							 Virtual_Event_Id=EIT[i_id].EIT_Infos[o_id].virtual_eventid;
							 Virtual_Offset_Id=i;
						 }
					 }
					 
					 if ( EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].HeaderSize > 0 ) {
						 
						 while (( EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Header[i] != 0x00 ) && ( k < 45)) {
							 Text[j] = EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Header[i];
							 Text[j+1]=0x00;
							 if ( Text[j] < 0x20 ) { 
								 if ( Text[j] == 0x0a ) {
									 Text[j]='-';
								 } else {
									 Text[j]=0x00;
									 j--;
									 k--;
								 };
							 };
							 if ( k > 35 ) {
								 if ( Text[j]==0x20 ) {
									 Text[j]=0x00;
									 k=99;
								 }
							 };
							 k++;
							 j++;
							 i++;
						 };
						 
					 };
					 Text[j]=0x00;
					 
					 WriteText( 1, 248, (unsigned short)(ActiveSel*28),Text);
					 
					 ActiveSel++;
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
					 
					 if ( OSD_Key_Value == '0' ) {; 
					 DestroyOSDWindow(1);
					 Osd_Thread_Down();
					 return;
					 };
					 
					 if ( OSD_Key_Value == '2' ) {; 
					 if ( 13+EIT_TopLine < ListCount ) {
						 EIT_TopLine++;
						 j=1;
					 };
					 } else if ( OSD_Key_Value == '1' ) {
						 j=EIT_TopLine;
						 EIT_TopLine=ListCount-13;
						 if ( EIT_TopLine < 0 ) EIT_TopLine=0;
						 if ( j != EIT_TopLine ) j= 1;
						 else j=0;
					 } else if ( OSD_Key_Value == '7' ) { 
						 if ( EIT_TopLine > 0) {
							 EIT_TopLine=0;
							 j=1;
						 };
					 } else if ( OSD_Key_Value == '8' ) {; 
					 if ( EIT_TopLine > 0) {
						 EIT_TopLine--;
						 j=1;
					 };
					 } else if ( OSD_Key_Value == '9' ) {; 
					 if ( EIT_TopLine > 0 ) {
						 EIT_TopLine-=13;
						 if ( EIT_TopLine<0 ) EIT_TopLine=0;
						 j=1;
					 };
					 } else if ( OSD_Key_Value == '3' ) { 
						 if ( EIT_TopLine < ListCount) {
							 j=EIT_TopLine;
							 EIT_TopLine+=13;
							 if ( EIT_TopLine>=ListCount ) EIT_TopLine=ListCount-13;
							 if ( EIT_TopLine<0 ) EIT_TopLine=0;
							 if ( j != EIT_TopLine ) j=1;
							 else j=0;
						 };
						 
					 };
					 
					 
				 };
		 }; // While ActiveSel;
		 
		 
}



void OSD_Eit_ProgrammListe( void ){
	
	int ListCount=0;
	
	
    int i,j,k,l;
    int Selection=0;
	int AltSelection=-1;
	
	int CurAkt=CurrentProgramm;
	
	BOOL First=TRUE;
    char Zeile[40];
    char Nummer[40];
	
	int dir;
	PROGRAMM_OSD=TRUE;
	CurAkt=Make_ProgNr(LastRealProg);	
	CreateOSDWindow( 1, BITMAP2, 450, 504);
	MoveWindowAbs(1,80,30 );
	SetColorBlend(1);
	OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
	OSDSetColor(1,1, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
	OSDSetColor(1,2, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
	
	ListCount=0;
	for ( i=0; i < MAXPROGS ; i++ ) {
		if ( Programm[i].freq != 0 ) ListCount++;
	};
	
	
	
	EIT_TopLine=CurAkt-6;
	if ( EIT_TopLine+18 >= ListCount ) EIT_TopLine=ListCount-18;
	if ( EIT_TopLine < 0 ) EIT_TopLine=0;
	
	Selection=CurAkt-EIT_TopLine;
	
	for (;;) {
		
		if ( First==FALSE) { 
			if ( dir == 2 ) {
				EIT_TopLine=CurAkt;
				if ( EIT_TopLine+18 >= ListCount ) EIT_TopLine=ListCount-18;
				if ( EIT_TopLine < 0 ) EIT_TopLine=0;
				Selection=CurAkt-EIT_TopLine;
				if ( Selection > 17 ) Selection=17;
			} else if ( dir ==8 ) {
				EIT_TopLine=CurAkt-17;
				if ( EIT_TopLine < 0 ) EIT_TopLine=0;
				Selection=CurAkt-EIT_TopLine;
			}
		}
		
		AltSelection=-1;
		First=FALSE;
		DrawBlock(1, 0, 0,450, 504,0);
		SetFont( 1, 1, 2, 0);
		k=Get_Real_ProgNr(EIT_TopLine);
		j=0;
		
		
		while (( k >= 0 ) && (k < ListCount) && (j < 18))  {   // Loop über Nachrichten 
			Zeile[0]=0x00;
			if ( IsValidSelect(k) == TRUE ) {
				if ( Programm[k].Typ == 'A' ) strcpy(Zeile,"Analog");
				else {
					if ( Programm[k].Video_pid != 0x1fff ) {
						strcat(Zeile,"V");
						if ( Programm[k].TeleText_pid != 0x1fff )  strcat(Zeile,"/T");
						if ( Programm[k].CA_ID != 0 )  strcat(Zeile,"/S*"); 
						else strcat(Zeile,"/F"); 
					} else if ( Programm[k].Audio_pid != 0x1fff ) {
						strcat(Zeile,"A");
						if ( Programm[k].CA_ID != 0 )  strcat(Zeile,"/S*"); 
						else strcat(Zeile,"/F"); 
					} else {
						strcat(Zeile,"D");
						if ( Programm[k].CA_ID != 0 )  strcat(Zeile,"/S*"); 
						else strcat(Zeile,"/F");  
					};
				} // Digital
				sprintf(Nummer,"%d",Make_ProgNr(k)+1);
				WriteText( 1, 4,  (unsigned short)(j*28) ,Nummer);
				WriteText( 1, 90,  (unsigned short)(j*28) ,Zeile);
				WriteText( 1, 180,  (unsigned short)(j*28) ,Programm[k].Name);
				j++;
			};
			k++;
		};
		
		
		
		
		
		j=0;
		while ( j == 0 ) {
			j=0;
			if (( AltSelection >= 0 ) && ( AltSelection < ListCount )) {
             		 DrawBlock(1, 0, (unsigned short)(AltSelection*28),450, 28,0);
	                     	SetFont(1, 1, 2, 0);
                     Zeile[0]=0x00;
                     k=Get_Real_ProgNr(EIT_TopLine+AltSelection);
					 if ( Programm[k].Typ == 'A' ) strcpy(Zeile,"Analog");
					 else {
						 if ( Programm[k].Video_pid != 0x1fff ) {
							 strcat(Zeile,"V");
							 if ( Programm[k].TeleText_pid != 0x1fff )  strcat(Zeile,"/T");
							 if ( Programm[k].CA_ID != 0 )  strcat(Zeile,"/S*"); 
							 else strcat(Zeile,"/F"); 
						 } else if ( Programm[k].Audio_pid != 0x1fff ) {
							 strcat(Zeile,"A");
							 if ( Programm[k].CA_ID != 0 )  strcat(Zeile,"/S*"); 
							 else strcat(Zeile,"/F"); 
						 } else {
							 strcat(Zeile,"D");
							 if ( Programm[k].CA_ID != 0 )  strcat(Zeile,"/S*"); 
							 else strcat(Zeile,"/F");  
						 };
					 };
					 
					 sprintf(Nummer,"%d",Make_ProgNr(k)+1);
					 WriteText(1,4,   (unsigned short)(AltSelection*28) ,Nummer);
					 WriteText(1,90,  (unsigned short)(AltSelection*28) ,Zeile);
					 WriteText(1,180, (unsigned short)(AltSelection*28) ,Programm[k].Name);
			};
			
			if (( Selection >= 0 ) && ( Selection < ListCount )) {
				AltSelection=Selection;
             		 DrawBlock(1, 0,(unsigned short)(Selection*28),450, 28,1);
                     SetFont( 1, 1, 0, 1);
                     Zeile[0]=0x00;
                     k=Get_Real_ProgNr(EIT_TopLine+Selection);
					 if ( Programm[k].Typ == 'A' ) strcpy(Zeile,"Analog");
					 else {
						 if ( Programm[k].Video_pid != 0x1fff ) {
							 strcat(Zeile,"V");
							 if ( Programm[k].TeleText_pid != 0x1fff )  strcat(Zeile,"/T");
							 if ( Programm[k].CA_ID != 0 )  strcat(Zeile,"/S*"); 
							 else strcat(Zeile,"/F"); 
						 } else if ( Programm[k].Audio_pid != 0x1fff ) {
							 strcat(Zeile,"A");
							 if ( Programm[k].CA_ID != 0 )  strcat(Zeile,"/S*"); 
							 else strcat(Zeile,"/F"); 
						 } else {
							 strcat(Zeile,"D");
							 if ( Programm[k].CA_ID != 0 )  strcat(Zeile,"/S*"); 
							 else strcat(Zeile,"/F");  
						 };
					 };
					 
					 sprintf(Nummer,"%d",Make_ProgNr(k)+1);
					 WriteText( 1,4,   (unsigned short)(Selection*28) ,Nummer);
					 WriteText( 1,90,  (unsigned short)(Selection*28) ,Zeile);
					 WriteText( 1,180, (unsigned short)(Selection*28) ,Programm[k].Name);
			};
			
            j=0;
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
			k=EIT_TopLine+Selection;
			l=Get_Real_ProgNr(k);
			if (( Programm[l].freq != 0  ) && (ValidModes(Programm[l].Typ) == TRUE  )) {
				sprintf(ChannelString,"%d",k+1);
				SetTimer(hWnd,99,1000,NULL);
				DestroyOSDWindow(1);
				Osd_Thread_Down();
				return;
			};
			};
			
			if ( OSD_Key_Value == '0' ) {; 
			DestroyOSDWindow(1);
			Osd_Thread_Down();
			return;
			};
			
			if ( OSD_Key_Value == '2' ) {; 
			if ( Selection < 17) {
				Selection++;
			} else {
				CurAkt=Selection+EIT_TopLine+1;
				if ( CurAkt < ListCount ) {
					dir=2;
					j=1;
				};
			};
			} else if ( OSD_Key_Value == '1' ) {
				if ( Selection+EIT_TopLine  < ListCount-1 ) {
					CurAkt=ListCount-1;
					dir=2;
					j=1;
				};
			} else if ( OSD_Key_Value == '7' ) { 
				if ( Selection+EIT_TopLine > 0) {
					dir=8;
					CurAkt=0;
					j=1;
				};
			} else if ( OSD_Key_Value == '8' ) {; 
			if ( Selection > 0) Selection--;
			else {
				
				CurAkt=EIT_TopLine-1;
				if ( CurAkt > 0 ) { 
					dir=8;
					j=1;
				};
			};
			} else if ( OSD_Key_Value == '9' ) {; 
			CurAkt=EIT_TopLine-1;
			if ( CurAkt >= 0) {
				dir=8;
				j=1;
			}
			
			} else if ( OSD_Key_Value == '3' ) { 
				
				CurAkt=EIT_TopLine+18;
				if ( CurAkt < ListCount-1 ) {
					dir=2;
					j=1;
				}
				
			};
}
		 };
		 
}


void OSD_Audio_Liste( void ){
	int ListCount=0;
	int i,j;
	struct CurrentAudio_t {
		unsigned short Pid;
		unsigned char Name[30];
	};
	
	static struct CurrentAudio_t Audio[16];
	
	int Selection=0;
	int AltSelection;
	char Zeile[60];
	
	PROGRAMM_OSD=TRUE;
	(void)Is_Pid_Param(Programm[CurrentProgramm].SID_pid,Programm[CurrentProgramm].tp_id,TRUE);
	ListCount=0;
	for ( i = 0 ; i < Programm[CurrentProgramm].Filteranzahl && ListCount < 16; i++ ) {
		if (( Programm[CurrentProgramm].Filters[i].FilterId == STREAMTYPE_11172_AUDIO ) || 
			( Programm[CurrentProgramm].Filters[i].FilterId == STREAMTYPE_13818_AUDIO )) {
			Audio[ListCount].Pid=Programm[CurrentProgramm].Filters[i].PID;
			GetAudioName(CurrentProgramm,i, &Audio[ListCount].Name[0]);
			
			if ((i < 16 ) && ( PidParam[i].fill != 0x00 ) && ( PidParam[i].Key_02 == 0x03 )) strcpy(Audio[ListCount].Name,PidParam[i].Name);
			
			if ( Audio[ListCount].Name[0] == 0x00 ) strcpy(Audio[ListCount].Name,t("Unbekannt"));
			ListCount++;
		}
	};
	
	Selection=FindActiveAudioName(CurrentProgramm,(unsigned char *)&Zeile);
	if ( Selection >= 0 ) {
		i=0;
		j=-1;
		while (( i < ListCount ) && ( j == -1 )) {
			if ( Programm[CurrentProgramm].Filters[Selection].PID == Audio[i].Pid ) j=i;
			i++;
		};
		Selection=j;
	};
	
	
	CreateOSDWindow( 1, BITMAP2, 360, (unsigned short)(((ListCount+2)*28)+8));
	MoveWindowAbs(1,200,100 );
	SetColorBlend(1);
	OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
	OSDSetColor(1,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
	OSDSetColor(1,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
	OSDSetColor(1,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
	
	
	if ( ListCount == 0 ) {
		DrawBlock(1, 0, 0,360, 64,0);
		DrawBlock(1, 4, 4,352, 56,1);
		
		SetFont( 1, 1, 3, 1);
		strcpy(Zeile,t("Auswahl vom Audio-Kanal"));
		WriteText( 1, 36, 4,Zeile);
		strcpy(Zeile,t("nicht möglich"));
		WriteText( 1, 110,32,Zeile);
		
		ResetEvent(OSD_KEY_EVENT);
		i=WaitForSingleObject (OSD_KEY_EVENT, 4000);
		DestroyOSDWindow(1);
		Osd_Thread_Down();
		return;
		
	};
	
	DrawBlock(1, 0, 0,360, 30,0);
    SetFont( 1, 1, 2, 0);
	strcpy(Zeile,t("Audio-Kanal auswählen"));
	WriteText( 1, 50, 1,Zeile);
	DrawBlock(1, 0, (unsigned short)(((ListCount+1)*28)+6),360, 30,0);
    strcpy(Zeile,t("<2><8> Default <5> Ende <0>"));
	WriteText( 1, 10,(unsigned short)(((ListCount+1)*28)+6),Zeile);
	DrawBlock(1, 0, 30,360,(unsigned short)((ListCount*28)+4),1);
	j=-1;
	for ( i=0; i < ListCount; i++ ) {
		sprintf(Zeile,"0x%04x",Audio[i].Pid);
		if (i==Selection) {
			if (j!=0) { SetFont(1, 1, 0, 3-1); j = 0;}
			DrawBlock(1, 5, (unsigned short)(((Selection+1)*28)+5), 350, 27, 3-1);
		} else {
			if (j!=3) {SetFont( 1, 1, 3, 1); j = 3;}
		}
		WriteText( 1, 4,  (unsigned short)(((i+1)*28)+5) ,Zeile);
		WriteText( 1, 110,  (unsigned short)(((i+1)*28)+5)  ,Audio[i].Name);
	};
	
			AltSelection=Selection;
	for (;;) {
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
			Programm[CurrentProgramm].Temp_Audio=Audio[Selection].Pid;
			Programm[CurrentProgramm].Audio_pid=Audio[Selection].Pid;
			Write_Programm_List();
			DestroyOSDWindow(1);
			Osd_Thread_Down();
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

		if (( AltSelection >= 0 ) && ( AltSelection < ListCount )) {
			DrawBlock(1, 0, (unsigned short)(((AltSelection+1)*28)+4),360, 28,1);
			SetFont(1, 1, 3, 1);
			sprintf(Zeile,"0x%04x",Audio[AltSelection].Pid);
			WriteText( 1, 4,  (unsigned short)(((AltSelection+1)*28)+5) ,Zeile);
			WriteText( 1, 110, (unsigned short)(((AltSelection+1)*28)+5) ,Audio[AltSelection].Name);
		};
		
		if (( Selection >= 0 ) && ( Selection < ListCount )) {
			AltSelection=Selection;
			DrawBlock(1, 5, (unsigned short)(((Selection+1)*28)+5),350, 27,3-1);
			SetFont( 1, 1, 0, 3-1);
			sprintf(Zeile,"0x%04x",Audio[Selection].Pid);
			WriteText( 1, 4,  (unsigned short)(((Selection+1)*28)+5) ,Zeile);
			WriteText( 1, 110,  (unsigned short)(((Selection+1)*28)+5),Audio[Selection].Name);
			Programm[CurrentProgramm].Temp_Audio=Audio[Selection].Pid;
			SetAudioPid(Audio[Selection].Pid);
			
		};
	}
};





void OSD_Text_Liste( void ){
	
	int ListCount=0;
	int i,j;
	struct CurrentText_t {
		unsigned short Pid;
		unsigned char Name[30];
	};
	
	struct CurrentText_t TextPid[64];
	
	int Selection=0;
	int AltSelection=-1;
	
	char Zeile[60];
	
	PROGRAMM_OSD=TRUE;
	
	ListCount=0;
	for ( i = 0; i < MAXPROGS ; i++ ) {
		
		if (( Programm[CurrentProgramm].tp_id == Programm[i].tp_id ) &&
			( Programm[CurrentProgramm].diseqc == Programm[i].diseqc ) &&
			( Programm[CurrentProgramm].freq == Programm[i].freq ) &&
			( Programm[CurrentProgramm].volt== Programm[i].volt) &&
			( Programm[i].TeleText_pid != 0x1fff )) {
			TextPid[ListCount].Pid=Programm[i].TeleText_pid;
			strcpy(TextPid[ListCount].Name,Programm[i].Name);
			if ( TextPid[ListCount].Pid == Set_TextPid ) Selection=ListCount;
			ListCount++;
		};
	};
	
	CreateOSDWindow( 1, BITMAP2, 360, (unsigned short)(((ListCount+2)*28)+8));
	MoveWindowAbs(1,200,63);
	SetColorBlend(1);
	OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
	OSDSetColor(1,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
	OSDSetColor(1,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
	OSDSetColor(1,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
	
	if ( ListCount == 0 ) {
		DrawBlock(1, 0, 0,360, 64,0);
		DrawBlock(1, 4, 4,352, 56,1);
		
		SetFont( 1, 1, 3, 1);
		strcpy(Zeile,t("Auswahl von TeleText-Pids"));
		WriteText( 1, 32, 4,Zeile);
		strcpy(Zeile,t("nicht möglich"));
		WriteText( 1, 110,32,Zeile);
		ResetEvent(OSD_KEY_EVENT);
		i=WaitForSingleObject (OSD_KEY_EVENT, 4000);
		DestroyOSDWindow(1);
		Osd_Thread_Down();
		return;
	};
	
	
	DrawBlock(1, 0, 0,360, 30,0);
    SetFont( 1, 1, 2, 0);
	strcpy(Zeile,t("TeleText-Pid auswählen"));
	WriteText( 1, 46, 1,Zeile);
	DrawBlock(1, 0, (unsigned short)(((ListCount+1)*28)+6),360, 30,0);
    strcpy(Zeile,t("<2><8> OK <5> Ende <0>"));
	WriteText( 1, 30,(unsigned short)(((ListCount+1)*28)+6),Zeile);
	DrawBlock(1, 0, 30,360,(unsigned short)((ListCount*28)+4),1);
    SetFont( 1, 1, 3, 1);
	for ( i=0; i < ListCount; i++ ) {
		sprintf(Zeile,"0x%04x",TextPid[i].Pid);
		WriteText( 1, 4,  (unsigned short)(((i+1)*28)+5) ,Zeile);
		WriteText( 1, 110,  (unsigned short)(((i+1)*28)+5)  ,TextPid[i].Name);
	};
	
	
	for (;;) {
		if (( AltSelection >= 0 ) && ( AltSelection < ListCount )) {
			DrawBlock(1, 0, (unsigned short)(((AltSelection+1)*28)+4),360, 28,1);
			SetFont( 1, 1, 3, 1);
			sprintf(Zeile,"0x%04x",TextPid[AltSelection].Pid);
			WriteText( 1, 4,  (unsigned short)(((AltSelection+1)*28)+5) ,Zeile);
			WriteText( 1, 110,  (unsigned short)(((AltSelection+1)*28)+5) ,TextPid[AltSelection].Name);
		};
		
		if (( Selection >= 0 ) && ( Selection < ListCount )) {
			AltSelection=Selection;
			DrawBlock(1, 5, (unsigned short)(((Selection+1)*28)+5),350, 27,3);
			SetFont( 1, 1, 0, 3-1);
			sprintf(Zeile,"0x%04x",TextPid[Selection].Pid);
			WriteText( 1, 4,  (unsigned short)(((Selection+1)*28)+5) ,Zeile);
			WriteText( 1, 110,  (unsigned short)(((Selection+1)*28)+5),TextPid[Selection].Name);
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
			Stop_VT_Stream();
			VT_Stream_ID=Start_VT_Stream(TextPid[Selection].Pid);
			DestroyOSDWindow(1);
			Osd_Thread_Down();
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
};



void OSD_Startzeit_Liste( void ){
	int ListCount=0;
	int i,j;
	
	int Selection=0;
	int AltSelection=-1;
	
	char Zeile[60];
	
	
	i=Is_Time_Link(Programm[CurrentProgramm].SID_pid, Programm[CurrentProgramm].tp_id,TRUE);
	
	PROGRAMM_OSD=TRUE;
	ListCount=0;
	
		  for ( i = 0; i < 16; i++ ) {
			  if ( MultiLinkZeit[i].fill != 0x00 ) {
				  ListCount++;
				  if ( MultiLinkZeit[i].fill == 0x02 ) Selection=i;
			  };
		  };
		  
		  
		  CreateOSDWindow( 1, BITMAP2, 360, (unsigned short)(((ListCount+2)*28)+8));
		  MoveWindowAbs(1,200,100 );
		  SetColorBlend(1);
		  OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
		  OSDSetColor(1,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
		  OSDSetColor(1,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
		  OSDSetColor(1,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
		  
		  if ( ListCount == 0 ) {
			  DrawBlock(1, 0, 0,360, 64,0);
			  DrawBlock(1, 4, 4,352, 56,1);
			  
			  SetFont( 1, 1, 3, 1);
			  strcpy(Zeile,t("Auswahl von Startzeiten"));
			  WriteText( 1, 38, 4,Zeile);
			  strcpy(Zeile,t("nicht möglich"));
			  WriteText( 1, 110,32,Zeile);
			  
			  ResetEvent(OSD_KEY_EVENT);
			  i=WaitForSingleObject (OSD_KEY_EVENT, 4000);
			  DestroyOSDWindow(1);
			  Osd_Thread_Down();
			  return;
		  };
		  
		  
		  DrawBlock(1, 0, 0,360, 30,0);
		  SetFont( 1, 1, 2, 0);
		  strcpy(Zeile,t("TimeShift-Kanal auswählen"));
		  WriteText( 1, 40, 1,Zeile);
		  DrawBlock(1, 0, (unsigned short)(((ListCount+1)*28)+6),360, 30,0);
		  strcpy(Zeile,t("<2><8> OK <5> Ende <0>"));
		  WriteText( 1, 30,(unsigned short)(((ListCount+1)*28)+6),Zeile);
		  DrawBlock(1, 0, 30,360,(unsigned short)((ListCount*28)+4),1);
		  SetFont( 1, 1, 3, 1);
		  for ( i=0; i < ListCount; i++ ) {
			  WriteText( 1, 60,  (unsigned short)(((i+1)*28)+4)  ,MultiLinkZeit[i].ExtraText );
		  };
		  for (;;) {
			  
			  if (( AltSelection >= 0 ) && ( AltSelection < ListCount )) {
				  DrawBlock(1, 0, (unsigned short)(((AltSelection+1)*28)+4),360, 28,1);
				  SetFont( 1, 1, 3, 1);
				  WriteText( 1, 60,  (unsigned short)(((AltSelection+1)*28)+5) ,MultiLinkZeit[AltSelection].ExtraText);
			  };
			  
			  if (( Selection >= 0 ) && ( Selection < ListCount )) {
				  AltSelection=Selection;
				  DrawBlock(1, 5, (unsigned short)(((Selection+1)*28)+5),350, 27,3);
				  SetFont( 1, 1, 0, 3-1);
				  WriteText( 1, 60,  (unsigned short)(((Selection+1)*28)+5),MultiLinkZeit[Selection].ExtraText);
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
				  PROGRAMM_OSD=FALSE;
				  Osd_Thread_Down();
				  Set_DVB_Channel_by_SID_TS_ID(MultiLinkZeit[Selection].ts_id,MultiLinkZeit[Selection].SID,Programm[LastRealProg].Name);
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
};


void OSD_Extra_Kanal_Liste( void ){
	int ListCount=0;
	int i,j;
	
	int Selection=0;
	int AltSelection=-1;
	
	char Zeile[60];
	
	
	i=Is_View_Link(Programm[CurrentProgramm].SID_pid, Programm[CurrentProgramm].tp_id,TRUE);
	
	PROGRAMM_OSD=TRUE;
	ListCount=0;
	
		  for ( i = 0; i < 16; i++ ) {
			  if ( MultiLinkView[i].fill != 0x00 ) {
				  ListCount++;
				  if ( MultiLinkView[i].fill == 0x02 ) Selection=i;
			  };
		  };
		  
		  CreateOSDWindow( 1, BITMAP2, 360, (unsigned short)(((ListCount+2)*28)+8));
		  MoveWindowAbs(1,200,100 );
		  SetColorBlend(1);
		  OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
		  OSDSetColor(1,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
		  OSDSetColor(1,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
		  OSDSetColor(1,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
		  
		  
		  if ( ListCount == 0 ) {
			  DrawBlock(1, 0, 0,360, 64,0);
			  DrawBlock(1, 4, 4,352, 56,1);
			  
			  SetFont( 1, 1, 3, 1);
			  strcpy(Zeile,t("Auswahl von Extra-Kanälen"));
			  WriteText( 1, 32, 4,Zeile);
			  strcpy(Zeile,t("nicht möglich"));
			  WriteText( 1, 110,32,Zeile);
			  
			  ResetEvent(OSD_KEY_EVENT);
			  i=WaitForSingleObject (OSD_KEY_EVENT, 4000);
			  DestroyOSDWindow(1);
			  Osd_Thread_Down();
			  return;
		  };
		  
		  DrawBlock(1, 0, 0,360, 30,0);
		  SetFont( 1, 1, 2, 0);
		  strcpy(Zeile,t("Extra-Kanal auswählen"));
		  WriteText( 1, 40, 1,Zeile);
		  DrawBlock(1, 0, (unsigned short)(((ListCount+1)*28)+6),360, 30,0);
		  strcpy(Zeile,t("<2><8> OK <5> Ende <0>"));
		  WriteText( 1, 30,(unsigned short)(((ListCount+1)*28)+6),Zeile);
		  DrawBlock(1, 0, 30,360,(unsigned short)((ListCount*28)+4),1);
		  SetFont( 1, 1, 3, 1);
		  for ( i=0; i < ListCount; i++ ) {
			  if ( strcmp(MultiLinkView[i].ExtraText,Programm[CurrentProgramm].Name) == 0 ) Selection=i;
			  WriteText( 1, 60,  (unsigned short)(((i+1)*28)+4)  ,MultiLinkView[i].ExtraText );
		  };
		  
		  for (;;) {
			  
			  if (( AltSelection >= 0 ) && ( AltSelection < ListCount )) {
				  DrawBlock(1, 0, (unsigned short)(((AltSelection+1)*28)+4),360, 28,1);
				  SetFont( 1, 1, 3, 1);
				  WriteText( 1, 60,  (unsigned short)(((AltSelection+1)*28)+5) ,MultiLinkView[AltSelection].ExtraText);
			  };
			  
			  if (( Selection >= 0 ) && ( Selection < ListCount )) {
				  AltSelection=Selection;
				  DrawBlock(1, 5, (unsigned short)(((Selection+1)*28)+5),350, 27,3);
				  SetFont( 1, 1, 0, 3-1);
				  WriteText( 1, 60,  (unsigned short)(((Selection+1)*28)+5),MultiLinkView[Selection].ExtraText);
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
				  Osd_Thread_Down();
				  Relink_Current_Eit(Programm[LastRealProg].SID_pid,Programm[LastRealProg].tp_id,MultiLinkView[Selection].SID,MultiLinkView[Selection].ts_id);
				  Set_DVB_Channel_by_SID_TS_ID(MultiLinkView[Selection].ts_id,MultiLinkView[Selection].SID,MultiLinkView[Selection].ExtraText);
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
};




BOOL Wait_For_CI_Menu_Data(){
	int i;
	char Zeile[128];
	int WaitRefresh=0;
	
	DrawBlock(1, 0, 28,540, 422,0);
	SetFont( 1, 1, 1, 0);
	sprintf(Zeile,t("Warte auf Antwort"));
	WriteText( 1, 180,  156 ,Zeile);
	DrawBlock(1, 0, 450 ,540, 28,1);
	SetFont( 1, 1, 2, 1);
	strcpy(Zeile,t("Beenden mit <0>"));
	WriteText( 1, 200, 450,Zeile);
	OSD_Key_Value='9';
	while ( ci_menu_update == FALSE ) {
		ResetEvent(OSD_KEY_EVENT);
		i=WaitForSingleObject (OSD_KEY_EVENT, 100);
		WaitRefresh++;
		if ( WaitRefresh > 50 ) {
			CI_Menu((short)CI_Menu_Slot);
			WaitRefresh=0;
		};
		
		if ( i == WAIT_ABANDONED ) {
			CI_Close_Menu((short)CI_Menu_Slot);
			DestroyOSDWindow(1);
			Osd_Thread_Down();
			return(FALSE);
		};
		if ( OSD_Key_Value == 0x00 ) {; 
		CI_Close_Menu((short)CI_Menu_Slot);
		DestroyOSDWindow(1);
		Osd_Thread_Down();
		return(FALSE);
		};
		if ( OSD_Key_Value == '0' ) {; 
		CI_Close_Menu((short)CI_Menu_Slot);
		DestroyOSDWindow(1);
		Osd_Thread_Down();
		return(FALSE);
		};
		
	};
	
	return(TRUE);
};







void OSD_CAM_Menu( void ){
	int ListCount=0;
	int Selection=0;
	int AltSelection=-1;
	BOOL SELECT_MODULE = TRUE;
	unsigned char Zeile[512];
	unsigned char OutBuffer[512];
	int i,j,k;
	int cPos;
	static BOOL Wait_for_CI_Data;
	static int BaseButtonReply=-1;
	static BOOL Has_Edit;
	BOOL First_Sel;
	int OffsetY;
	int MaxObjekt=0;
	int inputmode = 0;
	BOOL Input_OK;	  
	
    PROGRAMM_OSD=TRUE;
	CreateOSDWindow( 1, BITMAP1, 540, 478);
    MoveWindowAbs(1,60,25 );
    SetColorBlend(1);
    OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
    OSDSetColor(1,1, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
	
	
	DrawBlock(1, 0, 0,540, 28,1);
    SetFont( 1, 1, 2, 1);
	
	if (( ci_module[0].Present > 0 ) &&  ( ci_module[1].Present > 0 )) {
		strcpy(Zeile,t("CAM-Menü - Bitte CAM auswählen"));
		WriteText( 1, 100, 0,Zeile);
		DrawBlock(1, 0, 450,540, 28,1);
		SetFont( 1, 1, 2, 1);
		strcpy(Zeile,t("Auswahl <2><8> OK <5> Ende <0>"));
		WriteText( 1, 30, 450,Zeile);
		
		CI_Menu_Slot=1;
		while ( SELECT_MODULE == TRUE ) {
			if ( CI_Menu_Slot == 1 ) {
				DrawBlock(1, 40, 4*28,460, 28,1);
				SetFont( 1, 1, 0, 1);
				sprintf(Zeile,"CAM Slot 1  %s",ci_module[0].ModuleName);
				WriteText( 1, 70,  4*28 ,Zeile);
				DrawBlock(1, 40, 6*28,460, 28,0);
				SetFont( 1, 1, 1, 0);
				sprintf(Zeile,"CAM Slot 2  %s",ci_module[1].ModuleName);
				WriteText( 1, 70,  6*28 ,Zeile);
			} else {
				DrawBlock(1, 40, 4*28,460, 28,0);
				SetFont( 1, 1, 1, 0);
				sprintf(Zeile,"CAM Slot 1  %s",ci_module[0].ModuleName);
				WriteText( 1, 70,  4*28 ,Zeile);
				DrawBlock(1, 40, 6*28,460, 28,1);
				SetFont( 1, 1, 0, 1);
				sprintf(Zeile,"CAM Slot 2  %s",ci_module[1].ModuleName);
				WriteText( 1, 70,  6*28 ,Zeile);
			};
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
			
			if ( OSD_Key_Value == '0' ) {; 
			DestroyOSDWindow(1);
			Osd_Thread_Down();
			return;
			};
			
			if (( OSD_Key_Value == '8' ) || ( OSD_Key_Value == '2' )) {; 
			if ( CI_Menu_Slot == 1 ) CI_Menu_Slot=2;
			else CI_Menu_Slot=1;
			};
			
			if ( OSD_Key_Value == '5' ) {; 
			SELECT_MODULE = FALSE;
			};
			
		};
	} else 	if (( ci_module[0].Present > 0 ) &&  ( ci_module[1].Present == 0 )) {
		CI_Menu_Slot=1;
	} else if (( ci_module[0].Present == 0 ) &&  ( ci_module[1].Present > 0 )) {
		CI_Menu_Slot=2;
	} else {
		strcpy(Zeile,t("CAM-Menü"));
		WriteText( 1, 220, 0,Zeile);
		DrawBlock(1, 0, 6*28,540, 28,0);
        SetFont( 1, 1, 1, 0);
        if ( ci_interface > 0 ) {
			sprintf(Zeile,t("Kein CICAM am CI-Interface gefunden"));
			WriteText( 1, 70,  6*28 ,Zeile);
		} else {
			sprintf(Zeile,t("Kein CI-Interface gefunden"));
			WriteText( 1, 120,  6*28 ,Zeile);
		};
		DrawBlock(1, 0, 450,540, 28,1);
        SetFont( 1, 1, 2, 1);
		strcpy(Zeile,t("Beenden mit <0>"));
        WriteText( 1, 200, 450,Zeile);
		ResetEvent(OSD_KEY_EVENT);
        i=WaitForSingleObject (OSD_KEY_EVENT, INFINITE);
        DestroyOSDWindow(1);
        Osd_Thread_Down();
        return;
	};
	
	
	Wait_for_CI_Data=TRUE;
	ci_menu_update=FALSE;
    CI_Menu((short)CI_Menu_Slot);
	
    DrawBlock(1, 0, 0,540, 28,1);
    SetFont( 1, 1, 2, 1);
	sprintf(Zeile,t("CAM-Menü - CICAM Slot %d"),CI_Menu_Slot);
	WriteText( 1, 130, 0,Zeile);
	DrawBlock(1, 0, 450,540, 28,1);
	
	
	for (;;)  {
		if ( Wait_For_CI_Menu_Data() == FALSE ) return;
		
		DrawBlock(1, 0, 28,480, 416,0);
		j=0;
		k=0;
		OffsetY=30;
		SetFont( 1, 1, 1, 0);
		MaxObjekt=0;
		First_Sel = FALSE;
		Has_Edit=FALSE;
		for ( i = 0 ; i < 20; i++ ) {
			if ( ci_menu[i].Type == 'E' ) {
				Has_Edit=TRUE;
				j=i;
			};
		};
		if ( Has_Edit == TRUE ) {
			memset(&OutBuffer[0],0x00,512);
			cPos=0;
			Input_OK=FALSE;
			
			DrawBlock(1, 0, 422,540, 56,1);
			SetFont( 1, 1, 0, 1);
			strcpy(Zeile,"OK <Return>");
			WriteText( 1, 220, 450,Zeile);
			strcpy(Zeile,t("<Cursortasten> <Backspace> <Tab>"));
			WriteText( 1, 100,  422,Zeile);
			SetFont( 1, 1, 1, 0);
			
			while ( Input_OK == FALSE ) {
				if ( inputmode == 0 ) { 
					DrawBlock(1, 0, 28,540, 390,0);
					strcpy(Zeile,t("EditMode : Hex (umschalten mit ESC)"));
					WriteText( 1, 5,  40,Zeile);
					WriteText( 1, 5,  80,ci_menu[j-1].zeile);
					i=OSD_Edit_Hex(5,(6*28),ci_menu[j].laenge,&cPos,OutBuffer);
					if ( i == 27 ) inputmode = 1;
					if ( i == 13 ) Input_OK = TRUE ; 
				};
				if ( inputmode == 1 ) { 
					DrawBlock(1, 0, 28,540, 390,0);
					strcpy(Zeile,t("EditMode : Dez (umschalten mit ESC)"));
					WriteText( 1, 5,  40,Zeile);
					WriteText( 1, 5,  80,ci_menu[j-1].zeile);
					i=OSD_Edit_Dez(5,(6*28),ci_menu[j].laenge,&cPos,OutBuffer);
					if ( i == 27 ) inputmode = 2;
					if ( i == 13 ) Input_OK = TRUE ; 
				};
				if ( inputmode == 2 ) { 
					DrawBlock(1, 0, 28,540, 390,0);
					strcpy(Zeile,t("EditMode : Ascii (umschalten mit ESC)"));
					WriteText( 1, 5,  40,Zeile);
					WriteText( 1, 5,  80,ci_menu[j-1].zeile);
					i=OSD_Edit_Asc(5,(6*28),ci_menu[j].laenge,&cPos,OutBuffer);
					if ( i == 27 ) inputmode = 0;
					if ( i == 13 ) Input_OK = TRUE ; 
				};
				
			};
			
			if ( Input_OK == TRUE ) {
				CI_EnterKey((unsigned char)CI_Menu_Slot,(unsigned char)ci_menu[j].laenge,&OutBuffer[0]);
				ci_menu_update=FALSE;
			}
		} else {
			for ( i = 0 ; i < 20; i++ ) {
				if ( ci_menu[i].Type == 'T' ) {
					WriteText( 1, 5,  (unsigned short)((j*28)+OffsetY) ,ci_menu[i].zeile);
					j++;
				} else if ( ci_menu[i].Type == 'S' ) { 
					MaxObjekt++;
					if ( First_Sel == FALSE ) {
						DrawLine(1,0,(unsigned short)((j*28)+OffsetY),540,0,1);
						DrawLine(1,0,(unsigned short)((j*28)+OffsetY+1),540,0,1);
						OffsetY+=4;
						First_Sel=TRUE;
						Selection=1;
						AltSelection=1;
						SetFont( 1, 1, 0, 1);
						DrawBlock(1, 40,(unsigned short)(( j*28 )+OffsetY),460, 28,1);
						WriteText( 1, 70,  (unsigned short)((j*28)+OffsetY) ,ci_menu[i].zeile);
						SetFont( 1, 1, 1, 0);
						ci_menu[i].id = ( j*28 )+OffsetY;
					} else {
						WriteText( 1, 70, (unsigned short)((j*28)+OffsetY) ,ci_menu[i].zeile);
						ci_menu[i].id = ( j*28 )+OffsetY;
					};
					j++;
				};
			};
			
			SetFont( 1, 1, 2, 1);
			strcpy(Zeile,t("Auswahl <2><8> OK <5> Zurück <1> Ende <0>"));
			WriteText( 1, 20, 450,Zeile);
		};
		
		while ( ci_menu_update == TRUE ) {
			
			ResetEvent(OSD_KEY_EVENT);
			i=WaitForSingleObject (OSD_KEY_EVENT, INFINITE);
			if ( i != WAIT_OBJECT_0 ) {
				DestroyOSDWindow(1);
				Osd_Thread_Down();
				return;
			};
			
			if ( OSD_Key_Value == 0x00 ) {; 
			CI_Close_Menu((short)CI_Menu_Slot);
			DestroyOSDWindow(1);
			Osd_Thread_Down();
			return;
			};
			if ( OSD_Key_Value == '0' ) {; 
			CI_Close_Menu((short)CI_Menu_Slot);
			DestroyOSDWindow(1);
			Osd_Thread_Down();
			return;
			};
			
			if ( OSD_Key_Value == '1' ) {; 
			ci_menu_update=FALSE;
			CI_MenuKey((unsigned char)CI_Menu_Slot,0);
			};
			
			
			if ( OSD_Key_Value == '5' ) {; 
			ci_menu_update=FALSE;
			CI_MenuKey((unsigned char)CI_Menu_Slot,(unsigned char)Selection);
			} else if ( OSD_Key_Value == '2' ) {; 
			Selection++;
			if ( Selection > MaxObjekt ) Selection=1;
			} else if ( OSD_Key_Value == '8' ) {; 
			Selection--;
			if ( Selection < 1 ) Selection=MaxObjekt;
			};
			
			if ( Selection != AltSelection ) {
				
				j=1;
				for ( i = 0 ; i < 20; i++ ) {
					if ( ci_menu[i].Type == 'S' ) { 
						if ( j == AltSelection ) {
							DrawBlock(1, 40, (unsigned short)ci_menu[i].id,460, 28,0);
							SetFont( 1, 1, 1, 0);
							WriteText( 1, 70,  (unsigned short)ci_menu[i].id  ,ci_menu[i].zeile);
						};
						if ( j == Selection ) {
							DrawBlock(1, 40, (unsigned short)ci_menu[i].id,460, 28,1);
							SetFont( 1, 1, 0, 1);
							WriteText( 1,70,  (unsigned short)ci_menu[i].id  ,ci_menu[i].zeile);
						};
						j++;
					};
				};
				AltSelection = Selection;
			}
		}
 };
};



void Run_OSD_DLL_Call( void ){
	int x,y;
	int i;
	PROGRAMM_OSD_RUN=TRUE;
	PROGRAMM_OSD=DLL_OSD_Call.Input;
	
	
	x=DLL_OSD_Call.x2-DLL_OSD_Call.x1;
	y=DLL_OSD_Call.y2-DLL_OSD_Call.y1;
	
	CreateOSDWindow( 1, (unsigned short)DLL_OSD_Call.BitTiefe ,(unsigned short) x,(unsigned short) y);
	MoveWindowAbs(1,(unsigned short)DLL_OSD_Call.x1,(unsigned short)DLL_OSD_Call.y1 );
	SetColorBlend(1);
	
	if ( DLL_OSD_Call.BitTiefe == BITMAP2 ) {
		OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
		OSDSetColor(1,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
		OSDSetColor(1,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
		OSDSetColor(1,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
	} if ( DLL_OSD_Call.BitTiefe == BITMAP1 ) {
		OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
		OSDSetColor(1,1, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
	}
	
	if ( DLL_OSD_Created != NULL ) SetEvent(DLL_OSD_Created);
	
	OSD_Key_Value=0xff;
	for (;;) {
		i=WaitForSingleObject (OSD_KEY_EVENT, INFINITE );
		ResetEvent(OSD_KEY_EVENT);
		if ( OSD_Key_Value == 0 ) {
			DestroyOSDWindow(1);
			Osd_Thread_Down();
			OSD_Key_Value=0x00;
			if ( Ext_Dll[DLL_OSD_Call.DLL_Id].Extern_OSD_Key != NULL ) (Ext_Dll[DLL_OSD_Call.DLL_Id].Extern_OSD_Key)(OSD_Key_Value);
			return;
		};
		if ( i != WAIT_OBJECT_0 ) {
			DestroyOSDWindow(1);
			Osd_Thread_Down();
			OSD_Key_Value=0x00;
			if ( Ext_Dll[DLL_OSD_Call.DLL_Id].Extern_OSD_Key != NULL ) (Ext_Dll[DLL_OSD_Call.DLL_Id].Extern_OSD_Key)(OSD_Key_Value);
			return;
		};
		
		
		if ( Ext_Dll[DLL_OSD_Call.DLL_Id].Extern_OSD_Key != NULL ) (Ext_Dll[DLL_OSD_Call.DLL_Id].Extern_OSD_Key)(OSD_Key_Value);
	};
};



void OSD_CAM_MessagePopUp( void ){
	
	unsigned char Zeile[512];
	int i;
	int j;
	
    PROGRAMM_OSD=TRUE;
	CreateOSDWindow( 1, BITMAP1, 540, 140);
    MoveWindowAbs(1,60,400 );
    SetColorBlend(1);
    OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
    OSDSetColor(1,1, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
	
	DrawBlock(1, 0, 0,540, 28,1);
    SetFont( 1, 1, 2, 1);
	
	strcpy(Zeile,t("CAM-Meldung"));
	WriteText( 1, 180, 0,Zeile);
    SetFont( 1, 1, 1, 0);
	i=0;
	j=1;
	while ( i < 8 ) {
		if ( ci_menu[i].zeile[0] != 0x00 ) {
			WriteText( 1, 5,  (unsigned short)(j*28),ci_menu[i].zeile);
			j++;
		};
		i++;
	};
	
	
    OSD_FULL_INPUT=TRUE;
	for(;;) {
		ResetEvent(OSD_KEY_EVENT);
		OSD_Key_Value=0x20;
		i=WaitForSingleObject (OSD_KEY_EVENT, INFINITE);
		if ( OSD_Key_Value == 0 ) {
		         	CI_MenuKey((unsigned char)PopUpCam,(unsigned char)0x1);
					OSD_FULL_INPUT=FALSE;
					DestroyOSDWindow(1);
                    Osd_Thread_Down();
                    return;
		};
		if ( OSD_Key_Value == 27 ) {
		         	CI_MenuKey((unsigned char)PopUpCam,(unsigned char)OSD_Key_Value );
					OSD_FULL_INPUT=FALSE;
					DestroyOSDWindow(1);
                    Osd_Thread_Down();
                    return;
		};
		
		if ( OSD_Key_Value == 13 ) {
		         	CI_MenuKey((unsigned char)PopUpCam,(unsigned char)0x1);
					OSD_FULL_INPUT=FALSE;
					DestroyOSDWindow(1);
                    Osd_Thread_Down();
                    return;
		};
	};
};


int OSD_Edit_Hex( int startx , int starty, int len, int *current, unsigned char *Buffer){
	struct epos {
		int x;
		int y;
	};
	
    struct epos dpos[128]; 
	int i,j,k;
    int x,y;
    int cpos;
    unsigned char HexBuffer[128]; 
    unsigned char Zeile[5];
	
	j=0;
	
	x=0;
	y=0;
	for ( i=0; i < 64 ; i++ ) {
		dpos[i*2].x = startx+(x*35);
		dpos[i*2].y = starty+(y*34);
		dpos[i*2+1].x = startx+(x*35)+15;
		dpos[i*2+1].y = starty+(y*34);
		x++;
		if ( x > 11 ) {
			x=0;
			y++;
		};
	};
	
	HexBuffer[0]=0x00;
    for ( i=0; i< len ; i++ ) {
		sprintf(Zeile,"%02x",Buffer[i]);
		strcat(HexBuffer,Zeile);
	};
	cpos=*current;
	cpos=cpos*2;
	
    for ( i=0; i< len ; i++ ) {
		Zeile[0]=HexBuffer[i*2];
		Zeile[1]=0x00;
		WriteText( 1, (unsigned short)dpos[i*2].x,  (unsigned short)dpos[i*2].y ,Zeile);
		Zeile[0]=HexBuffer[i*2+1];
		Zeile[1]=0x00;
		WriteText( 1, (unsigned short)dpos[i*2+1].x,  (unsigned short)dpos[i*2+1].y ,Zeile);
	};
	
    DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
    DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
    OSD_FULL_INPUT=TRUE;
	for(;;) {
		ResetEvent(OSD_KEY_EVENT);
		OSD_Key_Value=0x20;
		i=WaitForSingleObject (OSD_KEY_EVENT, INFINITE);
		if ( OSD_Key_Value == 0 ) {
            *current=cpos/2;
			OSD_FULL_INPUT=FALSE;
			return(27);
		};
		
		if ( OSD_Key_Value == 1 ) {
			DrawBlock(1, (unsigned short) dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
			cpos++;
			if ( cpos >= len*2 ) cpos=0;
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
			continue;
        };
		
		if ( OSD_Key_Value == 2 ) {
			DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
			cpos--;
			if ( cpos < 0 ) cpos=(len*2)-1;
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
			continue;
        };
		
		if ( OSD_Key_Value == 3 ) {
			if ( cpos+24 < len*2 ) {
				DrawBlock(1,  (unsigned short)dpos[cpos].x, (unsigned short)(dpos[cpos].y+30),15, 2,0);
				cpos+=24;
				if ( cpos >= len*2 ) cpos=cpos-(len*2);
				DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
				DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
            };
			continue;
        };
		
		if ( OSD_Key_Value == 4 ) {
			if ( cpos-24 >= 0 ) {
				DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
				cpos-=24;
				if ( cpos < 0 ) cpos=len*2+cpos;
				DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
				DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
            }
			continue;
        };
		
		if ( OSD_Key_Value == 8 ) {
			DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
			cpos--;
			if ( cpos < 0 ) cpos=(len*2)-1;
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
			continue;
        };
		
		if ( OSD_Key_Value == 9 ) {
			DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
			j=cpos/2;
			j++;
			cpos=j*2;
			if ( cpos >= len*2 ) cpos=0;
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
			continue;
        }
		
		if ( OSD_Key_Value == 27 ) {
            *current=cpos/2;
			OSD_FULL_INPUT=FALSE;
			return(27);
		};
		
		if ( OSD_Key_Value == 13 ) {
            *current=cpos/2;
			OSD_FULL_INPUT=FALSE;
			return(13);
		};
		
		if (((  OSD_Key_Value >= '0' ) && ( OSD_Key_Value <= '9')) ||
            ((  OSD_Key_Value >= 'a' ) && ( OSD_Key_Value <= 'f')) ||
            ((  OSD_Key_Value >= 'A' ) && ( OSD_Key_Value <= 'F'))) {
			
			HexBuffer[cpos]=OSD_Key_Value;
			DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)dpos[cpos].y,15, 32,0);
			Zeile[0]=HexBuffer[cpos];
			Zeile[1]=0x00;
			WriteText( 1, (unsigned short)dpos[cpos].x,  (unsigned short)dpos[cpos].y ,Zeile);
			j=cpos/2;
			if ((j*2) == cpos ) {
				Zeile[0]=HexBuffer[cpos];
				Zeile[1]=HexBuffer[cpos+1];
				Zeile[2]=0x00;
			} else {
				Zeile[0]=HexBuffer[cpos-1];
				Zeile[1]=HexBuffer[cpos];
				Zeile[2]=0x00;
			};
			i=sscanf(Zeile,"%02x",&k);
			if ( i == 1 ) {
				Buffer[j]=(unsigned char)k;
				cpos++;
				if ( cpos >= len*2 ) cpos=0;
            };
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
		};
	};
	
};


int OSD_Edit_Dez( int startx , int starty, int len, int *current, unsigned char *Buffer){
	struct epos {
		int x;
		int y;
	};
	
    struct epos dpos[192]; 
	int i,j,k;
    int x,y;
    int cpos;
    unsigned char DezBuffer[192]; 
    unsigned char Zeile[8];
	
	j=0;
	
	x=0;
	y=0;
	for ( i=0; i < 64 ; i++ ) {
		dpos[i*3].x = startx+(x*52);
		dpos[i*3].y = starty+(y*34);
		dpos[i*3+1].x = startx+(x*52)+15;
		dpos[i*3+1].y = starty+(y*34);
		dpos[i*3+2].x = startx+(x*52)+30;
		dpos[i*3+2].y = starty+(y*34);
		x++;
		if ( x > 7 ) {
			x=0;
			y++;
		};
	};
	
	DezBuffer[0]=0x00;
    for ( i=0; i< len ; i++ ) {
		sprintf(Zeile,"%03d",Buffer[i]);
		strcat(DezBuffer,Zeile);
	};
	cpos=*current;
	cpos=cpos*3;
	
    for ( i=0; i< len ; i++ ) {
		Zeile[0]=DezBuffer[i*3];
		Zeile[1]=0x00;
		WriteText( 1, (unsigned short)dpos[i*3].x,  (unsigned short)dpos[i*3].y ,Zeile);
		Zeile[0]=DezBuffer[i*3+1];
		Zeile[1]=0x00;
		WriteText( 1, (unsigned short)dpos[i*3+1].x,  (unsigned short)dpos[i*3+1].y ,Zeile);
		Zeile[0]=DezBuffer[i*3+2];
		Zeile[1]=0x00;
		WriteText( 1, (unsigned short)dpos[i*3+2].x,  (unsigned short)dpos[i*3+2].y ,Zeile);
		
	};
	
	DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
	DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
    OSD_FULL_INPUT=TRUE;
	for(;;) {
		ResetEvent(OSD_KEY_EVENT);
		OSD_Key_Value=0x20;
		i=WaitForSingleObject (OSD_KEY_EVENT, INFINITE);
		if ( OSD_Key_Value == 0 ) {
            *current=cpos/3;
			OSD_FULL_INPUT=FALSE;
			return(27);
		};
		
		if ( OSD_Key_Value == 1 ) {
			DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
			cpos++;
			if ( cpos >= len*3 ) cpos=0;
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
			continue;
        };
		
		if ( OSD_Key_Value == 2 ) {
			DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
			cpos--;
			if ( cpos < 0 ) cpos=(len*3)-1;
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
			continue;
        };
		
		if ( OSD_Key_Value == 3 ) {
			if ( cpos+24 < len*3 ) {
				DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
				cpos+=24;
				DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
				DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
            };
			continue;
        };
		
		if ( OSD_Key_Value == 4 ) {
			if ( cpos-24 >= 0 ) {
				DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
				cpos-=24;
				DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
				DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
            }
			continue;
        };
		
		if ( OSD_Key_Value == 8 ) {
			DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
			cpos--;
			if ( cpos < 0 ) cpos=(len*3)-1;
			DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
			continue;
        };
		
		if ( OSD_Key_Value == 9 ) {
			DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
			j=cpos/3;
			j++;
			cpos=j*3;
			if ( cpos >= len*3 ) cpos=0;
			DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
			continue;
        }
		
		if ( OSD_Key_Value == 27 ) {
            *current=cpos/3;
			OSD_FULL_INPUT=FALSE;
			return(27);
		};
		
		if ( OSD_Key_Value == 13 ) {
            *current=cpos/3;
			OSD_FULL_INPUT=FALSE;
			return(13);
		};
		
		if ((  OSD_Key_Value >= '0' ) && ( OSD_Key_Value <= '9')) {
			
			DezBuffer[cpos]=OSD_Key_Value;
			DrawBlock(1,  (unsigned short)dpos[cpos].x, (unsigned short) dpos[cpos].y,15, 32,0);
			Zeile[0]=DezBuffer[cpos];
			Zeile[1]=0x00;
			WriteText( 1, (unsigned short)dpos[cpos].x,  (unsigned short)dpos[cpos].y ,Zeile);
			j=cpos/3;
			if ((j*3) == cpos ) {
				Zeile[0]=DezBuffer[cpos];
				Zeile[1]=DezBuffer[cpos+1];
				Zeile[2]=DezBuffer[cpos+2];
				Zeile[3]=0x00;
			} else if ((j*3)+1 == cpos ) {
				Zeile[0]=DezBuffer[cpos-1];
				Zeile[1]=DezBuffer[cpos];
				Zeile[2]=DezBuffer[cpos+1];
				Zeile[3]=0x00;
			} else if ((j*3)+2 == cpos ) { 
				Zeile[0]=DezBuffer[cpos-2];
				Zeile[1]=DezBuffer[cpos-1];
				Zeile[2]=DezBuffer[cpos];
				Zeile[3]=0x00;
			}
			i=sscanf(Zeile,"%03d",&k);
			if ( i == 1 ) {
				Buffer[j]=(unsigned char)k;
				cpos++;
				if ( cpos >= len*3 ) cpos=0;
            };
			DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
		};
	};
	
};

int OSD_Edit_Asc( int startx , int starty, int len, int *current, unsigned char *Buffer){
	struct epos {
		int x;
		int y;
	};
	
    struct epos dpos[64]; 
	int i,j;
    int x,y;
    int cpos;
    unsigned char AscBuffer[64]; 
    unsigned char Zeile[5];
	
	j=0;
	
	x=0;
	y=0;
	for ( i=0; i < 64 ; i++ ) {
		dpos[i].x = startx+(x*15);
		dpos[i].y = starty+(y*34);
		x++;
		if ( x > 23 ) {
			x=0;
			y++;
		};
	};
	
	AscBuffer[0]=0x00;
    for ( i=0; i< len ; i++ ) {
		if (( Buffer[i] >= 0x20 ) && ( Buffer[i] <=127 )) sprintf(Zeile,"%c",Buffer[i]);
		else strcpy(Zeile,".");
		strcat(AscBuffer,Zeile);
	};
	cpos=*current;
	
    for ( i=0; i< len ; i++ ) {
		Zeile[0]=AscBuffer[i];
		Zeile[1]=0x00;
		WriteText( 1, (unsigned short)dpos[i].x, (unsigned short)dpos[i].y ,Zeile);
	};
	
    DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
    DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
    OSD_FULL_INPUT=TRUE;
	for(;;) {
		ResetEvent(OSD_KEY_EVENT);
		OSD_Key_Value=0x19;
		i=WaitForSingleObject (OSD_KEY_EVENT, INFINITE);
		if ( OSD_Key_Value == 0 ) {
            *current=cpos;
			OSD_FULL_INPUT=FALSE;
			return(27);
		};
		
		if ( OSD_Key_Value == 1 ) {
			DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
			cpos++;
			if ( cpos >= len ) cpos=0;
			DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
			continue;
        };
		
		if ( OSD_Key_Value == 2 ) {
			DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
			cpos--;
			if ( cpos < 0 ) cpos=len-1;
			DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
			continue;
        };
		
		if ( OSD_Key_Value == 3 ) {
			if ( cpos+24 < len ) {
				DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
				cpos+=24;
				DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
				DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
            };
			continue;
        };
		
		if ( OSD_Key_Value == 4 ) {
			if ( cpos-24 >= 0 ) {
				DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
				cpos-=24;
				DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
				DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
            }
			continue;
        };
		
		if ( OSD_Key_Value == 8 ) {
			DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
			cpos--;
			if ( cpos < 0 ) cpos=len-1;
			DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
			continue;
        };
		
		if ( OSD_Key_Value == 9 ) {
			DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,0);
			cpos++;
			if ( cpos >= len ) cpos=0;
			DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
			continue;
        }
		
		if ( OSD_Key_Value == 27 ) {
            *current=cpos;
			OSD_FULL_INPUT=FALSE;
			return(27);
		};
		
		if ( OSD_Key_Value == 13 ) {
            *current=cpos;
			OSD_FULL_INPUT=FALSE;
			return(13);
		};
		
		if ((  OSD_Key_Value >= 32 ) && ( OSD_Key_Value <= 127 )) {
			AscBuffer[cpos]=OSD_Key_Value;
			DrawBlock(1,  (unsigned short)dpos[cpos].x,  (unsigned short)dpos[cpos].y,15, 32,0);
			Zeile[0]=AscBuffer[cpos];
			Zeile[1]=0x00;
			WriteText( 1, (unsigned short)dpos[cpos].x,  (unsigned short)dpos[cpos].y ,Zeile);
			Buffer[cpos]=AscBuffer[cpos];
			cpos++;
			if ( cpos >= len ) cpos=0;
			DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,1);
            DrawLine(1,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,1);
		};
	};
	
};

int OSD_Edit_Ascii( unsigned char windowNr, int startx , int starty, int len, int *current, unsigned char *Buffer, unsigned short fg,unsigned short bg){
	struct epos {
		int x;
		int y;
	};
	
    struct epos dpos[64]; 
	int i,j;
    int x,y;
    int cpos;
    unsigned char AscBuffer[64]; 
    unsigned char Zeile[5];
	
	j=0;
	
	x=0;
	y=0;
	for ( i=0; i < len ; i++ ) {
		dpos[i].x = startx+(x*17);
		dpos[i].y = starty;
		x++;
	};
	cpos=*current;
	SetFont( 1, 1, fg, bg);
FULL_LINE_REFRESH:
	AscBuffer[0]=0x00;
    for ( i=0; i< len ; i++ ) {
		if ( (unsigned int)i >= strlen(Buffer)) {
			Buffer[i]=0x00;
			strcpy(Zeile,"_");
		} else {
			sprintf(Zeile,"%c",Buffer[i]);
		};
		strcat(AscBuffer,Zeile);
	};
	
	
    for ( i=0; i< len ; i++ ) {
		Zeile[0]=AscBuffer[i];
		Zeile[1]=0x00;
		WriteText( windowNr, (unsigned short)dpos[i].x, (unsigned short)dpos[i].y ,Zeile);
	};
	
	
    DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,fg);
    DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,fg);
    OSD_FULL_INPUT=TRUE;
	for(;;) {
		ResetEvent(OSD_KEY_EVENT);
		OSD_Key_Value=0x20;
		i=WaitForSingleObject (OSD_KEY_EVENT, INFINITE);
		if ( OSD_Key_Value == 0 ) {
            *current=cpos;
			OSD_FULL_INPUT=FALSE;
			return(27);
		};
		
		if ( OSD_Key_Value == 1 ) {
			DrawBlock(windowNr,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,bg);
			cpos++;
			if ( cpos >= len ) cpos=0;
			DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,fg);
            DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,fg);
			continue;
        };
		
		if ( OSD_Key_Value == 2 ) {
			DrawBlock(windowNr,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,bg);
			cpos--;
			if ( cpos < 0 ) cpos=len-1;
			DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,fg);
            DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,fg);
			continue;
        };
		
		if ( OSD_Key_Value == 5 ) {
			for ( j=cpos ; j < len-1; j++ ) Buffer[j]=Buffer[j+1];
			Buffer[len-1]=0x00;
			goto FULL_LINE_REFRESH;
        };
		
		
		if ( OSD_Key_Value == 8 ) {
			DrawBlock(windowNr,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,bg);
			cpos--;
			if ( cpos >= 0 ) {
				for ( j=cpos ; j < len-1; j++ ) Buffer[j]=Buffer[j+1];
				Buffer[len-1]=0x00;
			}
			if ( cpos < 0 ) cpos=len-1;
			goto FULL_LINE_REFRESH;
        };
		
		if ( OSD_Key_Value == 9 ) {
			DrawBlock(windowNr,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,bg);
			cpos++;
			if ( cpos >= len ) cpos=0;
			DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,fg);
            DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,fg);
			continue;
        }
		
		if ( OSD_Key_Value == 27 ) {
            *current=cpos;
			OSD_FULL_INPUT=FALSE;
			return(27);
		};
		
		if ( OSD_Key_Value == 13 ) {
            *current=cpos;
			OSD_FULL_INPUT=FALSE;
			j=strlen(Buffer)-1;
			while ( j >= 0 ) {
				if ( Buffer[j]=='_') Buffer[j]=0x00;
				j--;
			};
			return(13);
		};
		
		if ((  OSD_Key_Value >= 32 ) && ( OSD_Key_Value <= 127 )) {
			AscBuffer[cpos]=OSD_Key_Value;
			DrawBlock(windowNr,  (unsigned short)dpos[cpos].x,  (unsigned short)dpos[cpos].y,15, 32,bg);
			Zeile[0]=AscBuffer[cpos];
			Zeile[1]=0x00;
			WriteText( windowNr, (unsigned short)dpos[cpos].x,  (unsigned short)dpos[cpos].y ,Zeile);
			Buffer[cpos]=AscBuffer[cpos];
			cpos++;
			if ( cpos >= len ) cpos=0;
			DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,fg);
            DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,fg);
		};
	};
	
};


int OSD_Edit_Ascii_Mask( unsigned char windowNr, int startx , int starty, int len, int *current, unsigned char *Buffer, unsigned short fg,unsigned short bg){
	struct epos {
		int x;
		int y;
		BOOL Edit;
	};
	
    struct epos dpos[64]; 
	int i,j;
    int x,y;
    int cpos;
    unsigned char AscBuffer[64]; 
    unsigned char Zeile[5];
	
	j=0;
	
	x=0;
	y=0;
	for ( i=0; i < len ; i++ ) {
		dpos[i].x = startx+(x*15);
		dpos[i].y = starty;
		dpos[i].Edit=FALSE;
		x++;
	};
	
	SetFont( 1, 1, fg, bg);
	
	AscBuffer[0]=0x00;
    for ( i=0; i< len ; i++ ) {
		if ( Buffer[i] == '_' ) {
			strcpy(Zeile,"_");
			dpos[i].Edit=TRUE;
		} else sprintf(Zeile,"%c",Buffer[i]);
		strcat(AscBuffer,Zeile);
	};
	cpos=*current;
	
	
    for ( i=0; i< len ; i++ ) {
		Zeile[0]=AscBuffer[i];
		Zeile[1]=0x00;
		WriteText( windowNr, (unsigned short)dpos[i].x, (unsigned short)dpos[i].y ,Zeile);
	};
	
    while (dpos[cpos].Edit == FALSE ) {
		cpos++;
		if ( cpos >= len ) cpos=0;
	};
	
    DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,fg);
    DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,fg);
    OSD_FULL_INPUT=TRUE;
	for(;;) {
		ResetEvent(OSD_KEY_EVENT);
		OSD_Key_Value=0x20;
		i=WaitForSingleObject (OSD_KEY_EVENT, INFINITE);
		if ( OSD_Key_Value == 0 ) {
            *current=cpos;
			OSD_FULL_INPUT=FALSE;
			return(27);
		};
		
		if ( OSD_Key_Value == 1 ) {
			DrawBlock(windowNr,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,bg);
			cpos++;
			if ( cpos >= len ) cpos=0;
			
			while (dpos[cpos].Edit == FALSE ) {
				cpos++;
				if ( cpos >= len ) cpos=0;
			};
			
			DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,fg);
            DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,fg);
			continue;
        };
		
		if ( OSD_Key_Value == 2 ) {
			DrawBlock(windowNr,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,bg);
			cpos--;
			if ( cpos < 0 ) cpos=len-1;
			while (dpos[cpos].Edit == FALSE ) {
				cpos--;
				if ( cpos < 0 ) cpos=len-1;
			};
			
			DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,fg);
            DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,fg);
			continue;
        };
		
		if ( OSD_Key_Value == 8 ) {
			DrawBlock(windowNr,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,bg);
			cpos--;
			if ( cpos < 0 ) cpos=len-1;
			while (dpos[cpos].Edit == FALSE ) {
				cpos--;
				if ( cpos < 0 ) cpos=len-1;
			};
			
			DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,fg);
            DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,fg);
			continue;
        };
		
		if ( OSD_Key_Value == 9 ) {
			DrawBlock(windowNr,  (unsigned short)dpos[cpos].x,  (unsigned short)(dpos[cpos].y+30),15, 2,bg);
			cpos++;
			while (dpos[cpos].Edit == FALSE ) {
				cpos++;
				if ( cpos >= len ) cpos=0;
			};
			
			if ( cpos >= len ) cpos=0;
			DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,fg);
            DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,fg);
			continue;
        }
		
		if ( OSD_Key_Value == 27 ) {
            *current=cpos;
			OSD_FULL_INPUT=FALSE;
			return(27);
		};
		
		if ( OSD_Key_Value == 13 ) {
            *current=cpos;
			OSD_FULL_INPUT=FALSE;
			return(13);
		};
		
		if ((  OSD_Key_Value >= 32 ) && ( OSD_Key_Value <= 127 )) {
			AscBuffer[cpos]=OSD_Key_Value;
			DrawBlock(windowNr,  (unsigned short)dpos[cpos].x,  (unsigned short)dpos[cpos].y,15, 32,bg);
			Zeile[0]=AscBuffer[cpos];
			Zeile[1]=0x00;
			WriteText( windowNr, (unsigned short)dpos[cpos].x,  (unsigned short)dpos[cpos].y ,Zeile);
			Buffer[cpos]=AscBuffer[cpos];
			cpos++;
			if ( cpos >= len ) cpos=0;
			while (dpos[cpos].Edit == FALSE ) {
				cpos++;
				if ( cpos >= len ) cpos=0;
			};
			
			DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+30),14,0,fg);
            DrawLine(windowNr,(unsigned short)dpos[cpos].x,(unsigned short)(dpos[cpos].y+31),14,0,fg);
		};
	};
	
};

void OSD_MenuListe( void ){
	int ListCount=0;
	int i,j;
	int Selection=0;
	int AltSelection=-1;
	char Zeile[128];
	char ListText[32][32];
	
	ListCount=13;
	strcpy(ListText[0], t("Quick-Info")); // Programm-Info
	strcpy(ListText[1], t("aktuelle Sendung")); // EPG-Detail
	strcpy(ListText[2], t("Vorschau akt. Prg.")); // EPG-Liste aktuelles Programm
	strcpy(ListText[3], t("Vorschau alle Prg.")); // EPG-Liste Current/Next
	strcpy(ListText[4], t("Programmliste")); // Programmliste
	strcpy(ListText[5], t("Uhrzeit")); // Uhr
	strcpy(ListText[6], t("DVR-Steuerung")); // Recordersteuerung allg.
	strcpy(ListText[7], t("CAM-Menü")); // CAM-Menü
	strcpy(ListText[8], "Teletext"); // Teletext-Pid's
	strcpy(ListText[9], t("Time-Shift")); // Time-Shift
	strcpy(ListText[10], t("Extra-Kanal")); // Extra-Kanal
	strcpy(ListText[11], "Audio"); // Audio-Auswahl
	strcpy(ListText[12], t("Technische Info")); // Audio-Auswahl
	PROGRAMM_OSD=TRUE;
	CreateOSDWindow( 1, BITMAP2, 260, (unsigned short)(((ListCount+2)*28)+8));
	MoveWindowAbs(1,200,60 );
	SetColorBlend(1);
	OSDSetColor(1,0, OSD_Colors[0].R, OSD_Colors[0].G, OSD_Colors[0].B ,OSD_Colors[0].Blend);
	OSDSetColor(1,1, OSD_Colors[1].R, OSD_Colors[1].G, OSD_Colors[1].B, OSD_Colors[1].Blend);
	OSDSetColor(1,2, OSD_Colors[2].R, OSD_Colors[2].G, OSD_Colors[2].B, OSD_Colors[2].Blend);
	OSDSetColor(1,3, OSD_Colors[3].R, OSD_Colors[3].G, OSD_Colors[3].B, OSD_Colors[3].Blend);
	DrawBlock(1, 0, 0,260, 30,0);
	SetFont( 1, 1, 2, 0);
	strcpy(Zeile,t("OSD - Funktionen"));
	WriteText( 1, 40, 1,Zeile);
	DrawBlock(1, 0, (unsigned short)(((ListCount+1)*28)+6),260, 30,0);
	strcpy(Zeile,t("<2,8> OK<5> Ende<0>"));
	WriteText( 1, 5,(unsigned short)(((ListCount+1)*28)+6),Zeile);
	DrawBlock(1, 0, 30,260,(unsigned short)((ListCount*28)+4),1);
	SetFont( 1, 1, 3, 1);

	// X-Position changed from 15 to 30 by BigReaper
	for ( i=0; i < ListCount; i++ ) WriteText( 1, 30,  (unsigned short)(((i+1)*28)+5)  ,ListText[i] );
	for (;;) {
		if (( AltSelection >= 0 ) && ( AltSelection < ListCount )) {
				DrawBlock(1, 0, (unsigned short)(((AltSelection+1)*28)+4),260, 28,1);
				SetFont( 1, 1, 3, 1);
				WriteText( 1, 30,  (unsigned short)(((AltSelection+1)*28)+5) ,ListText[AltSelection]);
		};
		
		if (( Selection >= 0 ) && ( Selection < ListCount )) {
			AltSelection=Selection;
				DrawBlock(1, 5, (unsigned short)(((Selection+1)*28)+5),250, 27,3-1);
				SetFont( 1, 1, 0, 3-1);
				WriteText( 1, 30,  (unsigned short)(((Selection+1)*28)+5),ListText[Selection]);
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
			Osd_Thread_Down();
			
			switch (Selection){
			case 0: // Quick-Info
				PostMessage(hWnd,WM_COMMAND,IDM_OSD_PROGINFO,0);
				return;
			case 1: // EPG-Detail
				PostMessage(hWnd,WM_COMMAND,IDM_OSD_EIT_INFO,0);
				return;
			case 2: // EPG-Liste aktuelles Programm
				PostMessage(hWnd,WM_COMMAND,IDM_OSD_EIT_LISTE_CURRENT_PROGRAMM,0);
				return;
			case 3: // EPG-Liste Current/Next
				PostMessage(hWnd,WM_COMMAND,IDM_OSD_EIT_LISTE_CURRENT_NEXT,0);
				return;
			case 4: // Programmliste
				PostMessage(hWnd,WM_COMMAND,IDM_OSD_PROGRAMM_LISTE,0);
				return;
			case 5: // Uhr
				PostMessage(hWnd,WM_COMMAND,IDM_OSD_UHR,0);
				return;
			case 6: // Recordersteuerung
				PostMessage(hWnd,WM_COMMAND,IDM_OSD_RECORDER,0);
				return;
			case 7: // CAM-Menü
				PostMessage(hWnd,WM_COMMAND,IDM_OSD_CAM,0);
				return;
			case 8: // Teletext-Pid's
				PostMessage(hWnd,WM_COMMAND,IDM_OSD_TEXT_LISTE,0);
				return;
			case 9: // Time-Shift
				PostMessage(hWnd,WM_COMMAND,IDM_OSD_STARTZEIT_LISTE,0);
				return;
			case 10: // Extra-Kanal
				PostMessage(hWnd,WM_COMMAND,IDM_OSD_EXTRA_KANAL,0);
				return;
			case 11: // Audio-Auswahl
				PostMessage(hWnd,WM_COMMAND,IDM_OSD_AUDIO_LISTE,0);
				return;
					case 12: // Canal info
				PostMessage(hWnd,WM_COMMAND,IDM_OSD_KANAL_PARAMETER,0);
				return;
			};
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
			 };
			 
			 return;
};

void Osd_Thread_Down( void ){
	PROGRAMM_OSD_RUN=FALSE;
	PROGRAMM_OSD=FALSE;
	SetEvent(Programm_OSD_Stop);
	return;
};


void Stop_Osd_Key_Event( void ){
	DWORD dwStartCount;
	DWORD dwEndCount;
	char Text[128];
	int i;
	
	if ( Programm_OSD_Thread == NULL ) return;
	dwStartCount=GetTickCount();
	for (i=0; i < 45; i++ ) { 
		OSD_Key_Value=0x00;
		SetEvent(OSD_KEY_EVENT);
		if (WaitForSingleObject(Programm_OSD_Stop, 10 ) == WAIT_OBJECT_0 ) {
			PROGRAMM_OSD_RUN=FALSE;
			PROGRAMM_OSD=FALSE;
			if ( OSD_KEY_EVENT != NULL ) CloseHandle( OSD_KEY_EVENT );
			OSD_KEY_EVENT=NULL;
			ResetEvent(Programm_OSD_Stop);
			if ( Programm_OSD_Thread != NULL ) CloseHandle(Programm_OSD_Thread );
			Programm_OSD_Thread = NULL;
			dwEndCount=GetTickCount();
			sprintf(Text,"TIMEING Stop-OSD-Thread Level 1 (Loop %d) nach %d ms",i,dwEndCount-dwStartCount);
			Write_Log(Text);
			return;
		};
	};
	
	if ( OSD_KEY_EVENT != NULL ) CloseHandle(OSD_KEY_EVENT);
	OSD_KEY_EVENT=NULL;
	OSD_Key_Value=0x00;
	for ( i=0; i < 80  ; i++ ) {
		if (WaitForSingleObject(Programm_OSD_Stop, 100 ) == WAIT_OBJECT_0 ) {
			// Thread Down
			PROGRAMM_OSD_RUN=FALSE;
			PROGRAMM_OSD=FALSE;
			ResetEvent(Programm_OSD_Stop);
			if ( OSD_KEY_EVENT != NULL ) CloseHandle(	OSD_KEY_EVENT );
			OSD_KEY_EVENT=NULL;
			if ( Programm_OSD_Thread != NULL ) CloseHandle(Programm_OSD_Thread );
			Programm_OSD_Thread = NULL;
			dwEndCount=GetTickCount();
			sprintf(Text,"TIMEING Stop-OSD-Thread Level 2 (Loop %d) nach %d ms",i,dwEndCount-dwStartCount);
			Write_Log(Text);
			return;
		};
	};
	dwEndCount=GetTickCount();
	sprintf(Text,"TIMEING Stop-OSD-Thread will nicht beenden nach %d ms",dwEndCount-dwStartCount);
	Write_Log(Text);
};



void Insert_WinRoll_Window(unsigned char WindowNr,int starty ,struct WinRoll_t *Roll,int MaxRoll) {
	int i;
	int WinNr=WindowNr-1;
    int xSize;
	
	if  ( Roll[WinNr].Created == TRUE ) return;
    // Oben erzugte Zeilen nach oben verschieben
	
	for ( i=0; i < WinNr ; i++ ) {
		if ( Roll[i].Created == TRUE ) {
            Roll[i].posy-=Roll[WinNr].sizey;
			MoveWindowAbs((unsigned char)(i+1),(unsigned short)Roll[i].startx,(unsigned short)Roll[i].posy);
		};
	};
	
	Roll[WinNr].posy=starty-Roll[WinNr].sizey;
   	for ( i=WinNr+1; i < MaxRoll ; i++ ) {
		if ( Roll[i].Created == TRUE ) Roll[WinNr].posy-=Roll[i].sizey;
	};
	xSize=720-(2*Roll[WinNr].startx);
	CreateOSDWindow( WindowNr, BITMAP2, (unsigned short)xSize, (unsigned short)Roll[WinNr].sizey);
	MoveWindowAbs(WindowNr,(unsigned short)Roll[WinNr].startx,(unsigned short)Roll[WinNr].posy);
	SetColorBlend(WindowNr);
	Roll[WinNr].Created=TRUE;
};      


int FindActiveAudioName(int Nummer, unsigned char *Name){
	int i;
	Name[0]=0x00;
	
	for ( i = 0 ; i < Programm[Nummer].Filteranzahl; i++ ) {
		if ( (( Programm[Nummer].Filters[i].FilterId == STREAMTYPE_11172_AUDIO ) ||
			  ( Programm[Nummer].Filters[i].FilterId == STREAMTYPE_13818_AUDIO )) && 
			( Programm[Nummer].Temp_Audio == Programm[Nummer].Filters[i].PID )) {
			GetAudioName(Nummer,i,Name);
			return(i);
		}
	};
	return(-1);
}


void GetAudioName(int Nummer,int FilterId, unsigned char *Name){
	Name[0]=0x00;
	
	
	if ( FilterId < 16 ) {
		(void)Is_Pid_Param(Programm[Nummer].SID_pid,Programm[Nummer].tp_id,TRUE);
		if (( PidParam[FilterId].fill != 0x00 ) && ( PidParam[FilterId].Key_02 == 0x03 )) {
			strcpy(Name,PidParam[FilterId].Name);
			if ( strcmp(Name,"Ton") != 0 ) return;
		}
	};
	
	
	if ( strcmp(Programm[Nummer].Filters[FilterId].FilterName,"ger") == 0)  strcpy(Name,t("Deutsch"));
	else if ( strcmp(Programm[Nummer].Filters[FilterId].FilterName,"deu")== 0) strcpy(Name,t("Deutsch"));
	else if ( strcmp(Programm[Nummer].Filters[FilterId].FilterName,"fre")== 0) strcpy(Name,t("Französisch"));
	else if ( strcmp(Programm[Nummer].Filters[FilterId].FilterName,"spa")== 0) strcpy(Name,t("Spanisch"));
	else if ( strcmp(Programm[Nummer].Filters[FilterId].FilterName,"ita")== 0) strcpy(Name,t("Italienisch"));
	else if ( strcmp(Programm[Nummer].Filters[FilterId].FilterName,"eng")== 0) strcpy(Name,t("Englisch"));
	else if ( strcmp(Programm[Nummer].Filters[FilterId].FilterName,"pol")== 0) strcpy(Name,t("Polnisch"));
	else if ( strcmp(Programm[Nummer].Filters[FilterId].FilterName,"ltz")== 0) strcpy(Name,t("Luxembourg"));
	else if ( strcmp(Programm[Nummer].Filters[FilterId].FilterName,"dut")== 0) strcpy(Name,t("Holländisch"));
	else if ( strcmp(Programm[Nummer].Filters[FilterId].FilterName,"esl")== 0) strcpy(Name,t("Estland"));
	else strcpy(Name,Programm[Nummer].Filters[FilterId].FilterName);
	
}

// Modified by BigReaper

BOOL Find_Current_EIT(unsigned short sid_id,unsigned short ts_id,int *Pos, int *Offset){
    int i,j;
	unsigned short Merker_Pos    = *Pos;
	unsigned short Merker_Offset = *Offset;

	*Pos    = Current_Eit_Index_Size-1;
	*Offset = Current_Eit_Index_Size-1;

	for ( i = 0 ; i < Current_Eit_Index_Size; i++ ) {
		if ( EIT[i].Anzahl > 0 ) {
			for ( j = 0 ; j < EIT[i].Anzahl; j++ ) {
				if (( EIT[i].EIT_Infos[j].SID_Id == sid_id )   &&
					( EIT[i].EIT_Infos[j].ts_id == ts_id ) &&
					( EIT[i].EIT_Infos[j].Running_Status == 2  ))
				{
					*Pos=i;
					*Offset=j;
					// return(TRUE);
				};
			}; 
		};
	};
	if ((*Pos != (Current_Eit_Index_Size-1)) && (*Offset != (Current_Eit_Index_Size-1)))
		return(TRUE);
	else
	{
		*Pos    = Merker_Pos;
		*Offset = Merker_Offset;
	return(FALSE);
	}
};

BOOL Find_Next_EIT(unsigned short sid_id,unsigned short ts_id,int *Pos, int *Offset){
    int i,j;
	unsigned short Merker_Pos    = *Pos;
	unsigned short Merker_Offset = *Offset;

	*Pos    = Current_Eit_Index_Size-1;
	*Offset = Current_Eit_Index_Size-1;

	for ( i = 0 ; i < Current_Eit_Index_Size; i++ )
	{
		if ( EIT[i].Anzahl > 0 )
		{
			for ( j = 0 ; j < EIT[i].Anzahl; j++ )
			{
				if (( EIT[i].EIT_Infos[j].SID_Id == sid_id )   &&
					( EIT[i].EIT_Infos[j].ts_id == ts_id ) &&
					( EIT[i].EIT_Infos[j].Running_Status == 1  ))
				{
					*Pos=i;
					*Offset=j;
					// return(TRUE);
				};
			}; 
		};
	};
	if ((*Pos != (Current_Eit_Index_Size-1)) && (*Offset != (Current_Eit_Index_Size-1)))
		return(TRUE);
	else
	{
		*Pos    = Merker_Pos;
		*Offset = Merker_Offset;
	return(FALSE);
	}
};




void GetCurrentNext(unsigned short sid_id,unsigned short ts_id,int typ,unsigned char *Titel,unsigned char *StartZeit){
	BOOL Ret;
	int i,j,k;
	int i_id;
	int o_id;
	int Virtual_Event_Id;
	int Virtual_Offset_Id;
	
    
	if (Titel != NULL ) Titel[0]=0x00;
	if (StartZeit != NULL ) StartZeit[0]=0x00;
	
	if ( typ == 1 ) Ret=Find_Next_EIT(sid_id,ts_id,&i_id, &o_id);
	 else if ( typ == 2 ) Ret=Find_Current_EIT(sid_id,ts_id,&i_id, &o_id);

	if ( Ret == FALSE ) return;
	
	if ( StartZeit != NULL ) sprintf(StartZeit,"%02d:%02d",RealTime(EIT[i_id].EIT_Infos[o_id].Start_Zeit_h),EIT[i_id].EIT_Infos[o_id].Start_Zeit_m);
	
	if ( Titel != NULL ) {
						k=0;
						j=0;
						Virtual_Event_Id=i_id;
						Virtual_Offset_Id=o_id;
						
						if (( EIT[i_id].EIT_Infos[o_id].virtual_eventid != 0xffff ) &&
							( EIT[i_id].EIT_Infos[o_id].virtual_sid != 0xffff )) {
							i=Get_Virtual_Event(EIT[i_id].EIT_Infos[o_id].virtual_eventid,EIT[i_id].EIT_Infos[o_id].virtual_sid);
							if ( i >= 0 ) {
								Virtual_Event_Id=EIT[i_id].EIT_Infos[o_id].virtual_eventid;
								Virtual_Offset_Id=i;
							}
						}
						
						
						if ( EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].HeaderSize > 0 ) {
				while ((j<40)
						 && (j<EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].HeaderSize)
	     	&& (EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Header[j]!=0x00)){				
					           Titel[k] = EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Header[j];
								if ( Titel[k] < 0x20 ) { 
									if ( Titel[k] == 0x0a ) {
										Titel[k]=0x00;
										j=40;
									} else {
										k--;
									};
								};
								k++;
								Titel[k]=0x00;
								j++;
							};
						} else strcpy(&Titel[0],t("<Kein Titel gefunden>"));
					}

					return;
};



BOOL Set_DVB_Channel_by_SID_TS_ID( unsigned short ts_id, unsigned short sid,char *RealName){
	int i;
	struct TTransponder Transponder;
	
	i=Find_Programm(ts_id,sid);
	if ( i >= 0 ) { 
		memcpy(&Programm[MAXPROGS], &Programm[i], sizeof(Programm[i]));
        strncpy(Programm[MAXPROGS].Name,RealName,30);
        Programm[MAXPROGS].Name[29]=0x00;
		CurrentProgramm=MAXPROGS;
		Set_DVB_Channel(MAXPROGS);
		return(TRUE);
	};
	
	for ( i=0; i < NEUSIZE ; i++ ) {
		memset(&ProgrammNeu[i].Name[0],0x00,sizeof( struct TProgramm));
		ProgrammNeu[i].Video_pid=0x1fff;        
		ProgrammNeu[i].Audio_pid=0x1fff;
		ProgrammNeu[i].TeleText_pid=0x1fff;          /* Teletext PID */
		ProgrammNeu[i].CA_Anzahl=0;
		ProgrammNeu[i].CA_Land[0]=0;
		ProgrammNeu[i].PMT_pid=0x1fff;
		ProgrammNeu[i].PCR_pid=0x1fff;
		ProgrammNeu[i].ECM_PID=0x1fff;
		ProgrammNeu[i].SID_pid=0x1fff;
		ProgrammNeu[i].AC3_pid=0x1fff;
		ProgrammNeu[i].Merker=0x00;
		ProgrammNeu[i].Link_SID=0xffff;
		ProgrammNeu[i].Link_TP=0xffff;
		ProgrammNeu[i].Dynamisch=0x00;
	};
	
	
	Transponder.diseqc = Programm[CurrentProgramm].diseqc;
	
    for ( i=0; i < MAXNIT ; i++ ) {
		if (( NIT[i].ts_id == ts_id ) && ( NIT[i].DiseqNr == Transponder.diseqc ))  break;
	};
	
	if ( i > MAXNIT ) return(FALSE);
	
    Transponder.srate=NIT[i].srate;
	Transponder.freq=NIT[i].freq;
	Transponder.power=LNB[Transponder.diseqc].Power;
	Transponder.volt=NIT[i].polar;
	Transponder.fec=NIT[i].fec;
	Transponder.qam=NIT[i].qam;
    Set_Transponder(&Transponder);
	
	
    if ( Get_Pat( ) == TRUE ) {
		for ( i=0; i < NEUSIZE ; i++ ) {
			if ( ProgrammNeu[i].SID_pid == sid ) break;
		};
		
		if ( i >= NEUSIZE )  return(FALSE);
		
		if ( Get_PMT(i,ProgrammNeu[i].PMT_pid) == TRUE ) {
					       ProgrammNeu[i].afc=FALSE;
						   ProgrammNeu[i].fec=Transponder.fec;
						   ProgrammNeu[i].freq=Transponder.freq;
						   ProgrammNeu[i].diseqc=Transponder.diseqc;
						   ProgrammNeu[i].qam=Transponder.qam;
						   ProgrammNeu[i].srate=Transponder.srate;
						   ProgrammNeu[i].volt=Transponder.volt;
						   
						   Insert_CAT(i);
						   strcpy(&ProgrammNeu[i].Name[0],RealName);
						   if ( Programm[LastRealProg].ServiceTyp == 0x04 ) {
							   ProgrammNeu[i].ServiceTyp = 0x05;
							   ProgrammNeu[i].Link_SID = Programm[LastRealProg].SID_pid;
							   ProgrammNeu[i].Link_TP = Programm[LastRealProg].tp_id;
						   };
						   
						   ProgrammNeu[i].Dynamisch = 0x01;
						   
						   Update_Programm_List(i);
						   Write_Programm_List();
						   
						   memcpy(&Programm[MAXPROGS], &ProgrammNeu[i], sizeof(ProgrammNeu[i]));
						   Programm[MAXPROGS].Typ='D';
						   CurrentProgramm=MAXPROGS;
						   Set_DVB_Channel(MAXPROGS);
						   return(TRUE);
		};
	}
	return(FALSE);
}

BOOL Reload_DVB_Channel_by_SID_TS_ID( unsigned short ts_id, unsigned short sid){
	int i;
	
	struct TTransponder Transponder;
	for ( i=0; i < NEUSIZE ; i++ ) {
		memset(&ProgrammNeu[i].Name[0],0x00,sizeof( struct TProgramm));
		ProgrammNeu[i].Video_pid=0x1fff;        
		ProgrammNeu[i].Audio_pid=0x1fff;
		ProgrammNeu[i].TeleText_pid=0x1fff;          /* Teletext PID */
		ProgrammNeu[i].CA_Anzahl=0;
		ProgrammNeu[i].CA_Land[0]=0;
		ProgrammNeu[i].PMT_pid=0x1fff;
		ProgrammNeu[i].PCR_pid=0x1fff;
		ProgrammNeu[i].ECM_PID=0x1fff;
		ProgrammNeu[i].SID_pid=0x1fff;
		ProgrammNeu[i].AC3_pid=0x1fff;
		ProgrammNeu[i].Merker=0x00;
		ProgrammNeu[i].Link_SID=0xffff;
		ProgrammNeu[i].Link_TP=0xffff;
		ProgrammNeu[i].Dynamisch=0x00;
	};
	
	
	Transponder.diseqc = Programm[CurrentProgramm].diseqc;
	
    for ( i=0; i < MAXNIT ; i++ ) {
		if (( NIT[i].ts_id == ts_id ) && ( NIT[i].DiseqNr == Transponder.diseqc ))  break;
	};
	
	if ( i > MAXNIT ) return(FALSE);
	
    Transponder.srate=NIT[i].srate;
	Transponder.freq=NIT[i].freq;
	Transponder.power=LNB[Transponder.diseqc].Power;
	Transponder.volt=NIT[i].polar;
	Transponder.fec=NIT[i].fec;
	Transponder.qam=NIT[i].qam;
    Set_Transponder(&Transponder);
	
	
    if ( Get_Pat( ) == TRUE ) {
		for ( i=0; i < NEUSIZE ; i++ ) {
			if ( ProgrammNeu[i].SID_pid == sid ) break;
		};
		
		if ( i >= NEUSIZE ) return(FALSE);
		
		if ( Get_PMT(i,ProgrammNeu[i].PMT_pid) == TRUE ) {
					       ProgrammNeu[i].afc=FALSE;
						   ProgrammNeu[i].fec=Transponder.fec;
						   ProgrammNeu[i].freq=Transponder.freq;
						   ProgrammNeu[i].diseqc=Transponder.diseqc;
						   ProgrammNeu[i].qam=Transponder.qam;
						   ProgrammNeu[i].srate=Transponder.srate;
						   ProgrammNeu[i].volt=Transponder.volt;
						   
						   Insert_CAT(i);
						   ProgrammNeu[i].Dynamisch = 0x01;
						   
						   Update_Programm_List(i);
						   Write_Programm_List();
						   
						   memcpy(&Programm[MAXPROGS], &ProgrammNeu[i], sizeof(ProgrammNeu[i]));
						   Programm[MAXPROGS].Typ='D';
						   CurrentProgramm=MAXPROGS;
						   Set_DVB_Channel(MAXPROGS);
						   return(TRUE);
		} ;
	}
	return(FALSE);
}


void Set_DVB_Channel(int Nummer){
	int Index,Offset;
	int RealIndex,RealOffset;
	unsigned char Zeile[128];
	int i;
	int Band=0;
	unsigned char Sync;
	int Quality,ErrorRate;
	
	sprintf(Zeile,"SAA7146 ** Set_DVB_Channel %d **",Nummer);
	Write_Log(Zeile);
	if ( Capture_TID == TRUE ) Stop_TID_Filters();
	ShutDownFilters();
	Set_Capture(4);
	
	if ( ( Programm[Nummer].Merker&1) == 1 ) Programm[Nummer].Merker-=1;
	EnableMenuItem(GetMenu(hWnd),IDM_TIMELISTE,MF_GRAYED);
	if ( ( Programm[Nummer].Merker&2) == 2 ) Programm[Nummer].Merker-=2;
	EnableMenuItem(GetMenu(hWnd),IDM_VIEWLISTE,MF_GRAYED);
	
	if ( Nummer != MAXPROGS ) LastRealProg=Nummer;
	Stop_Osd_Key_Event();
	
	TPCatAnzahl=0;
	
	
	if ( Programm[Nummer].ServiceTyp == 4 ) {
		if (( Find_Current_EIT(Programm[Nummer].SID_pid,Programm[Nummer].tp_id,&Index,&Offset) == TRUE ) || 
			( Find_Next_EIT(Programm[Nummer].SID_pid,Programm[Nummer].tp_id,&Index,&Offset) == TRUE )) {
			RealIndex=0;
			RealOffset=0;
			if ( Find_Multi_Link(EIT[Index].EIT_Infos[Offset].Event_Id,EIT[Index].EIT_Infos[Offset].SID_Id, &RealIndex, &RealOffset) == TRUE ) {
				if ( Set_DVB_Channel_by_SID_TS_ID(EIT[RealIndex].EIT_Infos[RealOffset].ts_id,EIT[RealIndex].EIT_Infos[RealOffset].SID_Id,Programm[Nummer].Name) == TRUE ) return;
				// CurrentProgramm = MAXPROGS;
			}
		};
	} else if ( Programm[Nummer].PMT_pid == 0x1fff ) {
		Reload_DVB_Channel_by_SID_TS_ID(Programm[Nummer].tp_id,Programm[Nummer].SID_pid);
	};
	
	
	
	dvb_front.power=LNB[Programm[Nummer].diseqc].Power;
	
				if ( LNB[Programm[Nummer].diseqc].Switch22khz == TRUE ) {
					if ( Programm[Nummer].freq/1000 >= LNB[Programm[Nummer].diseqc].SwitchFreq )  dvb_front.ttk= 1;
					else dvb_front.ttk=0;
				};
				if ( Programm[Nummer].freq/1000 >= LNB[Programm[Nummer].diseqc].SwitchFreq )  Band = 1;
				
				
				
				
                dvb_front.volt=Programm[Nummer].volt;
                dvb_front.fec=Programm[Nummer].fec;
                dvb_front.diseqc=Programm[Nummer].diseqc;
                dvb_front.freq=Programm[Nummer].freq;
                dvb_front.srate=Programm[Nummer].srate;
                dvb_front.video_pid=Programm[Nummer].Video_pid;
                dvb_front.audio_pid=Programm[Nummer].Audio_pid;
                Programm[Nummer].Temp_Audio=Programm[Nummer].Audio_pid;
				dvb_front.tt_pid=Programm[Nummer].TeleText_pid;
                dvb_front.afc=Programm[Nummer].afc;
                dvb_front.qam=Programm[Nummer].qam;
				
				if ( Is_Sat_Karte() == TRUE ) {
					SetDiSEqC(dvb_front.diseqc,Band,dvb_front.volt);
					Set22K( dvb_front.ttk);
				};
				
				
				Set_TV_Freq((unsigned __int64)dvb_front.freq,Programm[Nummer].diseqc); 
				
				DVB_SET_FRONTEND(&dvb_front); 
				Programm[Nummer].Temp_Audio=dvb_front.audio_pid;
				
				
				
				if ( Programm[Nummer].ServiceTyp == 4 ) {
					i=0;
					
					sprintf(Zeile,"SAA7146 ** Set_DVB_Channel %d NVOD Service **",Nummer);
					Write_Log(Zeile);
					
					Start_Eit_Filters();
					while ( i < 80 ) {
						Sleep(10);
						if (( Find_Current_EIT(Programm[Nummer].SID_pid,Programm[Nummer].tp_id,&Index,&Offset) == TRUE ) || 
							( Find_Next_EIT(Programm[Nummer].SID_pid,Programm[Nummer].tp_id,&Index,&Offset) == TRUE )) {
							RealIndex=0;
							RealOffset=0;
							if ( Find_Multi_Link(EIT[Index].EIT_Infos[Offset].Event_Id,EIT[Index].EIT_Infos[Offset].SID_Id, &RealIndex, &RealOffset) == TRUE ) {
								sprintf(Zeile,"%d   %s",Make_ProgNr(Nummer)+1,Programm[Nummer].Name);
								i=100;
								if ( Set_DVB_Channel_by_SID_TS_ID(EIT[RealIndex].EIT_Infos[RealOffset].ts_id,EIT[RealIndex].EIT_Infos[RealOffset].SID_Id,Programm[Nummer].Name) == TRUE ) return;
							};
						}
						i++;
					};
					i=Find_Link_Programm(Programm[Nummer].tp_id, Programm[Nummer].SID_pid );
					if (	i >= 0 ) {
						if ( Set_DVB_Channel_by_SID_TS_ID(Programm[i].tp_id,Programm[i].SID_pid,Programm[i].Name) == TRUE ) return;
					}
					Stop_Eit_Filters();
				};
				
				dvb_front.volt=Programm[Nummer].volt;
                dvb_front.fec=Programm[Nummer].fec;
                dvb_front.diseqc=Programm[Nummer].diseqc;
                dvb_front.freq=Programm[Nummer].freq;
                dvb_front.srate=Programm[Nummer].srate;
                dvb_front.video_pid=Programm[Nummer].Video_pid;
                dvb_front.audio_pid=Programm[Nummer].Audio_pid;
                Programm[Nummer].Temp_Audio=Programm[Nummer].Audio_pid;
				dvb_front.tt_pid=Programm[Nummer].TeleText_pid;
                dvb_front.afc=Programm[Nummer].afc;
                dvb_front.qam=Programm[Nummer].qam;
				
				
				SetAVPID(Programm[Nummer].Video_pid,Programm[Nummer].Audio_pid, Programm[Nummer].TeleText_pid,0,Programm[Nummer].PCR_pid);
				
				CI_Run(Nummer);
				DVB_Reset();
				
				
				
				
				Get_Signal_Infos(&Sync , &Quality ,&ErrorRate);
				
				for ( i = 0 ; i < plugins_count ; i++ ) {
					if ( Ext_Dll[i].Extern_Channel_Change != NULL ) (Ext_Dll[i].Extern_Channel_Change)(Programm[Nummer]);
				};
				
				
				if ( Capture_Event_EIT == TRUE ) Start_Eit_Filters();
				if ( Capture_TID == TRUE ) Start_TID_Filters();
				VT_Stream_ID=Start_VT_Stream(dvb_front.tt_pid);
				//	SetAudioPid(dvb_front.audio_pid);
				Create_Osd_Thread((LPTHREAD_START_ROUTINE)OSD_ProgInfo);
				Set_Capture(5);
				sprintf(Zeile,"SAA7146 ** Set_DVB_Channel %d Ende **",Nummer);
				Write_Log(Zeile);
}


void Start_TID_Filters( void ){
	int i;
	i=SetFilter(PID_TOT, (unsigned short)((TID_TOT<<8)|0x00ff), SECTION_ANY|SECTION_CONTINUOUS|PBUFSIZE_NONE, "Zeit-Sync Section 0x73");
	if ( i >= 0 ) Filter[i].Irq_Proc =0x01;
	i=SetFilter(PID_TDT, (unsigned short)((TID_TDT<<8)|0x00ff), SECTION_ANY|SECTION_CONTINUOUS|PBUFSIZE_NONE, "Zeit-Sync Section 0x70" );
	if ( i >= 0 ) Filter[i].Irq_Proc =0x01;
};

void Stop_TID_Filters( void ){
	ShutDownFiltersByName("Zeit-Sync Section 0x73");
	ShutDownFiltersByName("Zeit-Sync Section 0x70");
};


void Start_Eit_Filters( void ){
	char Zeile[128];
	sprintf(Zeile,"Event Information PID 0x0012");
	EIT_Buffer_Offset=0;
	StartLogFilter((unsigned short)PID_EIT,4,Zeile);
};

void Stop_Eit_Filters( void ){
	ShutDownFiltersByName("Event Information PID 0x0012");
};


unsigned char Get_Sync_State( unsigned char *RealVal ){
	unsigned char status=0;
	int sync=0;
	
	if ( VES_TYPE == 0 ) {
        sync=readreg(ves_addr,ves_flags,0x11);
		*RealVal=sync;
		
		if ((sync&2) == 2 ) status+=1;
		if ((sync&2) == 2 ) status+=2;
		if ((sync&4) == 4 ) status+=4;
		if ((sync&4) == 4 ) status+=8;
        if ((status&0x0f)==0x0f) System_in_Sync=0;
		return(status);
	} else if ( VES_TYPE == 1 ) {
        sync=readreg(ves_addr,ves_flags,0x0e);
		*RealVal=sync;
		if ((sync&1) == 1 ) status+=1;
		if ((sync&2) == 2 ) status+=2;
		if ((sync&4) == 4 ) status+=4;
		if ((sync&8) == 8 ) status+=8;
        if ((status&0x0f)==0x0f) System_in_Sync=0;
		return(status);
	} else if ( VES_TYPE == 2 ) {
        sync=readreg(ves_addr,ves_flags,0x1b);
		*RealVal=sync;
		if ((sync&0x80) == 0x80 )  status+=1;
		if ((sync&0x80) == 0x80 )  status+=2;
		if ((sync&0x10) == 0x10 )  status+=4;
		if ((sync&0x8) == 0x8 )  status+=8;
		if ((sync&0x90) == 0x90 ) status=0x0f;
        if ((status&0x0f)==0x0f) System_in_Sync=0;
		return(status);
	} else if ( VES_TYPE == 3 ) {
		//8083
		sync=readreg(ves_addr,ves_flags, 0x2);
		*RealVal=sync;
		if ((sync&0x01) == 1 )  status+=1;
		if ((sync&0x01) == 1 )  status+=2;
		if ((sync&0x02) == 2 )  status+=4;
		if ((sync&0x10) == 16 )  status+=8;
        if ((status&0x0f)==0x0f) System_in_Sync=0;
		// Bit 4+8 == Lock
		return(status);
	}
	return(0);
};

void Get_Signal_Infos(unsigned char *Sync , unsigned int *Quality,unsigned int *BitErrorRate ){
	unsigned int q,e;
	unsigned char vstatus;
	unsigned char x;
	unsigned int bq,be,bs;
	
	// Sync_State
	
	char Text[255];
	
	if ( VES_TYPE == 1 ) {	
		q = readreg(ves_addr,ves_flags,0x1c);
		e = readreg(ves_addr,ves_flags,0x15) | (readreg(ves_addr,ves_flags,0x16)<<8) | (readreg(ves_addr,ves_flags,0x17)<<16);
		bq=q;
		be=e;
		q=255-q;
		e=e/4110;
		if ( e > 255 ) e=255;
		e=255-e;
		*Quality=q;
		*BitErrorRate=e;
		*Sync=Get_Sync_State(&x);
//		bs=&x;	tom
		bs=x;
		
	} else if ( VES_TYPE == 2 ) {	
		e = readreg(ves_addr,ves_flags, 0x15);
		q=(readreg(ves_addr,ves_flags,0x24)<<8)|readreg(ves_addr,ves_flags,0x25);
		*Sync=Get_Sync_State(&x);
		vstatus=x;
		if ( vstatus != 0x98 ) { 
			writereg(ves_addr,ves_flags, 0x14, 0x80 | ves_bclk); 
			writereg(ves_addr,ves_flags, 0x12, 0x39 ); 
			*Sync=Get_Sync_State(&x);
			vstatus=x;
		};
		
		bs=vstatus;
		bq=255-(q/46);
		be=e;
		*Quality=bq;
		*BitErrorRate=e;
	} else if ( VES_TYPE == 3 ) {	
		q=readreg(ves_addr,ves_flags, 0x08);
		e = (readreg(ves_addr,ves_flags,0x0b)<<16)+(readreg(ves_addr,ves_flags,0x0c)<<8)+readreg(ves_addr,ves_flags,0x0d);
		bq=q;
		be=e;
		be=(be&0xffff);
		be=be/255;
		*Sync=Get_Sync_State(&x);
		bs=x;
		*Quality=bq;
		*BitErrorRate=be;
	}else if (VES_TYPE == 0 ) { 
		q = 0;
		e = readreg(ves_addr,ves_flags,0x14) | (readreg(ves_addr,ves_flags,0x15)<<8) | (readreg(ves_addr,ves_flags,0x16)<<16);
		bq=q;
		be=e;
		q=255-q;
		e=e/649;
		if ( e > 255 ) e=255;
		e=255-e;
		*Quality=q;
		*BitErrorRate=e;
		*Sync=Get_Sync_State(&x);
		bs=x;
	}
	
	sprintf(Text,"SAA7146 Get_Signal_Infos VES_Type:%d  Q:%d S:%d E:%d",VES_TYPE,bs,bq,be);
	Write_Log(Text);
	
	
}


void Run_DVB_Irq(BOOL State, BOOL Keep_Run_IR){
	
	if ( Has_DVB == TRUE ) {
		if ( State == FALSE ) {
			Write_Log("SAA7146 Run_DVB_Irq(FALSE)");
			Debi_Record_Stop();
			Record=FALSE;
			Play=FALSE;
			
			/* turn off rps */
            video_setmode(0);
			ShutDownFilters();
			Sleep(25);
			SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, MASK_28);  // Turn Off RPS
			SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, MASK_24);   // DISABLE i2c-port pins
            if ( Keep_Run_IR == FALSE ) {            
				SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, 0x08800000);// DISABLE Debi
			};
			SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, MASK_26);   // Disable video-port-pins
            if ( Keep_Run_IR == FALSE ) {            
				SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, IER, 0x00000000); // Turn Off irq
			} else {
				SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, IER, MASK_03 ); // Keep Debi Running
			}
		} else {
			Write_Log("SAA7146 Run_DVB_Irq(TRUE)");
           	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, (MASK_08 | MASK_24)); // enable i2c-port pins 
			SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1, 0x08800880);// Enable Debi
			SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC1,(MASK_10 | MASK_26)); //	 enable video-port-pins
			SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, IER , (MASK_27 | MASK_28 | MASK_03 )); // Turn On irq
			Init_Screen_Struct();
			DOSIZE=TRUE;
		}; 
	}
}


void SetMonitor_16_9( BOOL Use_16_9 ){
	int mode=0;
	if ( Use_16_9 == TRUE ) mode=1;
	SAA7146_OutCom(COMTYPE_ENCODER, SetMonitorType, 1, mode);
}

void SetMode( int mode){
	SAA7146_OutCom(COMTYPE_ENCODER, LoadVidCode, 1, mode);
}

void TestMode( int mode){
	SAA7146_OutCom(COMTYPE_ENCODER, SetTestMode, 1, mode);
}

void VidMode(int mode){
	SAA7146_OutCom( COMTYPE_ENCODER, SetVidMode, 1, mode);
}

void Set22K(int state){
	char Text[128];
	
	if (( state == Alt22Khz ) && ( FastSwitch == TRUE )) {
		sprintf(Text,"SAA7146 Set_22_KHz %s schon eingestellt",state ? "ON" : "OFF");
		Write_Log(Text);
		return;
	};
	
	Alt22Khz=state;
	sprintf(Text,"SAA7146 Set_22_KHz %s",state ? "ON" : "OFF");
	Write_Log(Text);
	
	
	SAA7146_OutCom(COMTYPE_AUDIODAC, (state ? ON22K : OFF22K),0);
}

void SetDiSEqC( int DiseqCNr, int Band, int Volt){
	unsigned short DiseqCParameter[5]; // für DiSEqC
    unsigned char Power =0x01;
    unsigned char V =0x00;
    char Text[128];
	
	if ( UseDiseqC() == FALSE ) { 
		sprintf(Text,"SAA7146 SetDiSEqC : Kein DiSEqC");
		Write_Log(Text);
		return;
    };
	
	if (( DiseqCNr == AltDiseqCNr ) && ( Band == AltBand ) && ( AltVolt == Volt ) && ( FastSwitch == TRUE )) {
		sprintf(Text,"SAA7146 SetDiSEqC : DiseqCNr %d Band %d Volt %d schon eingestellt",DiseqCNr,Band,Volt);
		Write_Log(Text);
		return;
	}
    AltDiseqCNr=DiseqCNr;
	AltBand=Band;
	AltVolt=Volt;
	
	sprintf(Text,"SAA7146 SetDiSEqC : DiseqCNr %d Band %d Volt %d ",DiseqCNr,Band,Volt);
	Write_Log(Text);
	
	if ( LNB[DiseqCNr].MiniDiseqc == 0x01 ) {
		sprintf(Text,"SAA7146 SetDiSEqC : Mini-DiSEqC-Kommando");
		Write_Log(Text);
		
		DiseqCParameter[0]=0xffff;
		if ( LNB[DiseqCNr].BurstVal != -1 ) {
			if ( LNB[DiseqCNr].BurstVal == 1 ) DiseqCParameter[0]=0x0001;
			else if ( LNB[DiseqCNr].BurstVal == 0 ) DiseqCParameter[0]=0x0000;
			else DiseqCParameter[0]=0x00ff;
		}; 
		
		sprintf(Text,"SAA7146 SetDiSEqCParameter für Mini-DiSEqC : %d ",DiseqCParameter[0]);
		
		Set22K(0); 
		Sleep(25);
		
		if ( VES_TYPE == 1 ) SetControl((unsigned char )((Power ? 0x20 : 0)|(Volt ? 0x10 : 0)));
		else if ( VES_TYPE == 2 ) SetControl((unsigned char )(1|(Power ? 0x40 : 0)|(Volt ? 0x10 : 0)));
		else if ( VES_TYPE == 3 ) SetControl((unsigned char )(Volt ? 0x11 : 0x1));
		Sleep(25);
		SAA7146_OutCom(COMTYPE_AUDIODAC, SendDiSEqC, 1, DiseqCParameter[0]); 
		if ( Timing.SendDiSEqCDelay > 0 ) Sleep(Timing.SendDiSEqCDelay);
		return;
	};
	
	sprintf(Text,"SAA7146 SetDiSEqC : Normales DiSEqC-Kommando");
	Write_Log(Text);
	
    DiseqCParameter[0]=0xffff;     // Burst
    DiseqCParameter[1]=0xe0;
    DiseqCParameter[2]=0x10;
    DiseqCParameter[3]=0x38;
    DiseqCParameter[4]=DISEQC_HIGH_NIBLE;
	
	if ( LNB[DiseqCNr].BurstVal != -1 ) {
        if ( LNB[DiseqCNr].BurstVal == 1 ) DiseqCParameter[0]=0x0001;
        else if ( LNB[DiseqCNr].BurstVal == 0 ) DiseqCParameter[0]=0x0000;
		else DiseqCParameter[0]=0x00ff;
    };
	
	
	if ( Volt == 1 ) DiseqCParameter[4]|=DISEQC_HORIZONTAL;
	else DiseqCParameter[4]|=DISEQC_VERTICAL;
	
	if ( Band == 0 ) DiseqCParameter[4]|=DISEQC_LOW_BAND;
	else DiseqCParameter[4]|=DISEQC_HIGH_BAND;
	
	if ( LNB[DiseqCNr].PosVal == 'A' ) DiseqCParameter[4] |= DISEQC_POSITION_A;
	else DiseqCParameter[4] |= DISEQC_POSITION_B;
	
	if ( LNB[DiseqCNr].OptVal == 'A' ) DiseqCParameter[4] |= DISEQC_OPTION_A;
	else DiseqCParameter[4] |= DISEQC_OPTION_B;
	
	SAA7146_OutCom(COMTYPE_AUDIODAC, SendDiSEqC, 6, 4,DiseqCParameter[0],DiseqCParameter[1], DiseqCParameter[2], DiseqCParameter[3], DiseqCParameter[4]);
    if ( Timing.SendDiSEqCDelay > 0 ) Sleep(Timing.SendDiSEqCDelay);
	DiseqCParameter[1]=0xe1;
	SAA7146_OutCom(COMTYPE_AUDIODAC, SendDiSEqC, 6, 4,DiseqCParameter[0],DiseqCParameter[1], DiseqCParameter[2], DiseqCParameter[3], DiseqCParameter[4]);
    if ( Timing.SendDiSEqCDelay > 0 ) Sleep(Timing.SendDiSEqCDelay);
	SAA7146_OutCom(COMTYPE_AUDIODAC, SendDiSEqC, 6, 4,DiseqCParameter[0],DiseqCParameter[1], DiseqCParameter[2], DiseqCParameter[3], DiseqCParameter[4]);
    if ( Timing.SendDiSEqCDelay > 0 ) Sleep(Timing.SendDiSEqCDelay);
	SAA7146_OutCom(COMTYPE_AUDIODAC, SendDiSEqC, 6, 4,DiseqCParameter[0],DiseqCParameter[1], DiseqCParameter[2], DiseqCParameter[3], DiseqCParameter[4]);
	if ( LNB[DiseqCNr].PowerReset == TRUE ) {
		if ( Timing.SendDiSEqCDelay > 0 ) Sleep(Timing.SendDiSEqCDelay);
		if ( VES_TYPE == 1 ) SetControl((unsigned char )((Power ? 0x20 : 0)|(V ? 0x10 : 0)));
		else if ( VES_TYPE == 2 ) SetControl((unsigned char )(1|(Power ? 0x40 : 0)|(V ? 0x10 : 0)));
		else if ( VES_TYPE == 3 ) SetControl((unsigned char )(V ? 0x11 : 0x01));
	}
	if ( Timing.SendDiSEqCDelay > 0 ) Sleep(Timing.SendDiSEqCDelay);
};




int GetFront(struct frontend *front){
	char Text[128];
	unsigned char x;
	
	if ( VES_TYPE == 1 ) {	
		front->afc=((int)((char)(readreg(ves_addr,ves_flags,0x0a)<<1)))/2;
		front->afc=(front->afc*(int)(front->srate/8))/16;
		front->agc=(readreg(ves_addr,ves_flags,0x0b)<<8);
		front->sync=readreg(ves_addr,ves_flags,0x0e);
		front->nest=(readreg(ves_addr,ves_flags,0x1c)<<8);
		
		front->vber = readreg(ves_addr,ves_flags,0x15);
		front->vber|=(readreg(ves_addr,ves_flags,0x16)<<8);
		front->vber|=(readreg(ves_addr,ves_flags,0x17)<<16);
		
		if ((front->fec==8) && ((front->sync&0x1f) == 0x1f))
			front->fec=(readreg(ves_addr,ves_flags,0x0d)>>4)&0x07;
	} else if ( VES_TYPE == 2 ) {	
		front->afc=0;
		front->agc=0;
		front->sync=readreg(ves_addr,ves_flags, 0x1b);
		front->nest=(readreg(ves_addr,ves_flags,0x24)<<8)|readreg(ves_addr,ves_flags,0x25);  
		front->vber = 0;
		if ( front->sync != 0x98 ) {
			writereg(ves_addr,ves_flags, 0x14, 0x80 | ves_bclk); 
			writereg(ves_addr,ves_flags, 0x12, 0x39 ); 
			front->sync=readreg(ves_addr,ves_flags, 0x1b);
			
		};
	} else if ( VES_TYPE == 3 ) {	
		//8083
		front->sync=readreg(ves_addr,ves_flags, 0x2);
		// 0x0b 0x0c 0x0d 
		front->vber = readreg(ves_addr,ves_flags,0x0b);
		front->vber|=(readreg(ves_addr,ves_flags,0x0c)<<8);
		front->vber|=(readreg(ves_addr,ves_flags,0x0d)<<16);
		// 0x0b
		front->nest=(readreg(ves_addr,ves_flags,0x08)<<8);
		// afc                // ??? 
		// front->afc = readreg(ves_addr,ves_flags,0x05);
		// front->afc|=(readreg(ves_addr,ves_flags,0x06)<<8);
		// front->afc|=(readreg(ves_addr,ves_flags,0x07)<<16);
		
		
		
	} else if ( VES_TYPE == 0 ) { 
		front->afc=((int)((char)(readreg(ves_addr,ves_flags,0x19))));
		front->afc=(front->afc*(int)(front->srate/8))/128;
		front->agc=(readreg(ves_addr,ves_flags,0x17)<<8);
		front->sync=readreg(ves_addr,ves_flags,0x11);
		front->nest=0;
		front->vber = readreg(ves_addr,ves_flags,0x14);
		front->vber|=(readreg(ves_addr,ves_flags,0x15)<<8);
		front->vber|=(readreg(ves_addr,ves_flags,0x16)<<16);
	}
	
	
	
	sprintf(Text,"SAA7146 GetFront : VES_TYPE %d front->afc %d front->agc %d front->sync %d front->nest %d front->vber %d",VES_TYPE,front->afc,front->agc,front->sync,front->nest,front->vber);
	Write_Log(Text);
	front->sync=Get_Sync_State(&x);
    return 0;
}



void DVB_Reset( void ){
	if ( VES_TYPE == 1 ) ClrBit1893();
	else if ( VES_TYPE == 0) ClrBit1820();
	else if ( VES_TYPE == 2) ClrBit0299();
	else if ( VES_TYPE == 3) ClrBit8083();
	Reset_Debi();
};


unsigned short RealTime(unsigned char Stunde){
	short x;
	x=Stunde+TIMEZONE;
    if ( x < 0 ) x=x+24;
	if ( x > 23) x=x-24;
	return(x);
};


void DVB_SET_FRONTEND(struct frontend *front){
	if ( VES_TYPE == 1 ) {
		// VES1893
		if ( ves_inversion != front->inversion ) {
			ves_inversion=front->inversion;
			writereg(ves_addr,ves_flags,  0x0c, Init_VES_1893_Tab[0x0c] ^ (ves_inversion ? 0x40 : 0x00));
			ClrBit1893();
		}
		
		SetControl((unsigned char )((front->power ? 0x20 : 0)|(front->volt ? 0x10 : 0)));
		SetFEC(front->fec);
		VES1893_SetSymbolrate(front->srate, 1);
	} else if ( VES_TYPE == 2 ) {
		// STV0299
		SetControl((unsigned char )(1|(front->power ? 0x40 : 0)|(front->volt ? 0x10 : 0)));
		STV0299_SetSymbolrate( front->srate);
	} else if ( VES_TYPE == 3 ) {
		SetControl((unsigned char )((front->volt ? 0x11 : 0x01)));
		TDA8083_SetSymbolrate(front->srate, 1);
	} else if ( VES_TYPE == 0 ) {
		VES1820_SetInversion(VES1820_Inversion);
		SetQAM( (unsigned short)front->qam, 1);
		VES1820_SetSymbolrate(front->srate, 1);
	};
}

/* All OSD commands supported by the Windows code */

unsigned short Swab16(unsigned char *dat){
	unsigned char b1;
	unsigned char b2;
	b1=dat[0];
	b2=dat[1];
	return(b1<<8|b2);
};

unsigned int RGB2YUV(unsigned short R, unsigned short G, unsigned short B){
	unsigned short y, u, v;
	unsigned short Y, Cr, Cb;
	/*
	y = (9798 * R + 19235 * G + 3736 * B) / 32768;
	u = (((-4784 * R) - (9437 * G) + 4221 * B) / 32768) + 128;	
	v = ((20218 * R - 16941 * G - 3277 * B) / 32768) + 128;
	Y = y;
	Cb= u;
	Cr= v;
	
	  
	*/
	y = R * 77 + G * 150 + B * 29;  // Luma=0.299R+0.587G+0.114B 0..65535
	u = 2048+B * 8 -(y>>5);    // Cr 0..4095
	v = 2048+R * 8 -(y>>5);    // Cb 0..4095
	
	Y=y/256;//Y = y/4788+16;
	Cb=u/16;//Cb= u/18;
	Cr=v/16;//Cr= v/18;
	
	return Cr|(Cb<<16)|(Y<<8);
}

int ResetBlend( unsigned char windownr){
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"ResetBlend(w=%d)",windownr);
	Write_Log(Text);
#endif
	windownr++;
	return(SAA7146_OutCom(COMTYPE_OSD, SetNonBlend, 1, windownr));
}

int SetColorBlend(unsigned char windownr){
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"SetColorBlend(w=%d)",windownr);
	Write_Log(Text);
#endif
	windownr++;
	return(SAA7146_OutCom( COMTYPE_OSD, SetCBlend,  1, windownr));
}

int SetWindowBlend( unsigned char windownr, unsigned char blending){
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"SetWindowBlend(w=%d, b=%d)",windownr, blending);
	Write_Log(Text);
#endif
	windownr++;
	return(SAA7146_OutCom( COMTYPE_OSD, SetWBlend, 2, windownr, blending));
}

int SetBlend_(unsigned char windownr,unsigned short colordepth, unsigned short index, unsigned char blending){
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"SetBlend_(w=%d, cd=%d, i=%d, b=%d)",windownr, colordepth, index, blending);
	Write_Log(Text);
#endif
	windownr++;
	return(SAA7146_OutCom(COMTYPE_OSD, SetBlend, 4,windownr, colordepth, index, blending));
} 

int SetColor_(unsigned char windownr, unsigned short colordepth, unsigned short index, unsigned short colorhi, unsigned short colorlo){
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"SetColor_(w=%d, cd=%d, i=%d, c=0x%08x)",windownr, colordepth, index, colorhi*65536+colorlo);
	Write_Log(Text);
#endif
	windownr++;
	return (SAA7146_OutCom(COMTYPE_OSD, SetColor, 5,windownr, colordepth, index, colorhi, colorlo));
} 

int BringToTop( unsigned char  windownr){
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"BringToTop(w=%d)",windownr);
	Write_Log(Text);
#endif
	windownr++;
	return(SAA7146_OutCom(COMTYPE_OSD, WTop, 1, windownr));
} 

void SetFont(unsigned char  windownr, unsigned short  fontsize,unsigned short  colorfg, unsigned short  colorbg){
	int ret;
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"SetFont(w=%d, f=%d, fg=%d, bg=%d)", windownr, fontsize, colorfg, colorbg);
	Write_Log(Text);
#endif
	windownr++;
	ret=SAA7146_OutCom(COMTYPE_OSD, Set_Font, 4,windownr, fontsize, colorfg, colorbg);
	return;
} 

int DrawLine( unsigned char windownr,unsigned short x, unsigned short y, unsigned short dx, unsigned short dy, unsigned short color){
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"DrawLine(w=%d, x=%d, y=%d, dx=%d, dy=%d, c=%d)", windownr, x, y, dx, dy, color);
	Write_Log(Text);
#endif
	windownr++;
	return(SAA7146_OutCom(COMTYPE_OSD, DLine, 6,windownr, x, y, dx, dy, color));
} 

void DrawBlock( unsigned char windownr,unsigned short x, unsigned short y, unsigned short dx, unsigned short dy, unsigned short color){
	int ret;
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"DrawBlock(w=%d, x=%d, y=%d, dx=%d, dy=%d, c=%d)",windownr, x, y, dx, dy, color);
	Write_Log(Text);
#endif
	windownr++;
	ret=SAA7146_OutCom(COMTYPE_OSD, DBox, 6,windownr, x, y, dx, dy, color);
	return;
} 

int HideWindow(unsigned char windownr){
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"HideWindow(w=%d)",windownr);
	Write_Log(Text);
#endif
	windownr++;
	return(SAA7146_OutCom(COMTYPE_OSD, WHide, 1, windownr));
} 

int MoveWindowRel( unsigned char windownr, unsigned short x, unsigned short y){
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"MoveWindowRel(w=%d, x=%d, y=%d)",windownr, x, y);
	Write_Log(Text);
#endif
	windownr++;
	return(SAA7146_OutCom(COMTYPE_OSD, WMoveD, 3, windownr, x, y));
} 

int MoveWindowAbs( unsigned char windownr, unsigned short x, unsigned short y){
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"MoveWindowAbs(w=%d, x=%d, y=%d)",windownr, x, y);
	Write_Log(Text);
#endif
	windownr++;
	return SAA7146_OutCom(COMTYPE_OSD, WMoveA, 3, windownr, x, y);
} 

int DestroyOSDWindow(unsigned char windownr){
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"DestroyWindow(w=%d)",windownr);
	Write_Log(Text);
#endif
	windownr++;
	return SAA7146_OutCom(COMTYPE_OSD, WDestroy, 1, windownr);
} 

int CreateOSDWindow( unsigned char windownr,unsigned short disptype, unsigned short width, unsigned short height){
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"CreateOSDWindow(w=%d, t=%d, w=%d, h=%d)",windownr, disptype, width, height);
	Write_Log(Text);
#endif
	windownr++;
	return SAA7146_OutCom( COMTYPE_OSD, WCreate, 4,windownr, disptype, width, height);
} 

void OSDSetColor(unsigned char windownr, unsigned short color, unsigned char  r, unsigned char  g, unsigned char  b, unsigned char  blend){
	unsigned short ch, cl;
	unsigned int yuv;
#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"OSDSetColor(w=%d, c=%d, r=%d, g=%d, b=%d, bl=%d)",windownr, color, r, g, b, blend);
	Write_Log(Text);
#endif
	
	yuv=RGB2YUV(r,g,b);
	cl=(yuv&0xffff);
	ch=((yuv>>16)&0xffff);
	SetColor_(windownr, 4,color, ch, cl);
	SetBlend_(windownr, 4,color, blend);
}

int OSDLoadBitmap4(unsigned char windownr ,BITMAPINFO *BitMap, int x,int y,int TR,int TG,int TB,int TVal,int TimeToWait){
	
	unsigned char *SrcPtr;
    int i;
    int Fix;
	unsigned short ch, cl;
    unsigned int yuv;
	
	int SrcLine;
    int ZielLine;
    int ScreenLine=0;
	
	struct CTable {
		BITMAPINFO bi;
		RGBQUAD    QuadVal[16];
	};
	
	struct CTable *ct;
	BOOL Used[16];

#if defined(DEBUG_OSD)
	char Text[256];
	sprintf(Text,"OSDLoadBitmap4(w=%d, @=%x, x=%d, y=%d, TR=%d, TG=%d, TB=%d, TV=%d, w=%d)",windownr, BitMap, x, y, TR, TG, TB, TVal, TimeToWait);
	Write_Log(Text);
#endif
	
	for ( i=0; i < 16 ; i++ ) Used[i]=FALSE;
	SrcPtr =(unsigned char *)BitMap;
    SrcPtr+=(sizeof(BITMAPINFOHEADER)+64);
	
	for ( i=0; i <(signed)BitMap->bmiHeader.biSizeImage ; i++ ) {
		Fix=*SrcPtr&0x0f;
		Used[Fix]=TRUE;
		Fix=*SrcPtr>>4;
		Used[Fix]=TRUE;
		SrcPtr++;
	};
	
	ct=(struct CTable *)BitMap;
	for ( i=0; i < 16 ; i++ ) {
		if ( Used[i]==TRUE ) {
			yuv=RGB2YUV(ct->QuadVal[i-1].rgbRed,ct->QuadVal[i-1].rgbGreen,ct->QuadVal[i-1].rgbBlue);
			cl=(yuv&0xffff);
			ch=((yuv>>16)&0xffff);
			SetColor_(1, 16,(unsigned short)i,(unsigned short) ch,(unsigned short) cl);
			if (( ct->QuadVal[i-1].rgbRed == TR ) &&
				( ct->QuadVal[i-1].rgbGreen == TG ) &&
				( ct->QuadVal[i-1].rgbBlue == TB )) {
				SetBlend_(1,16,(unsigned short)i, (unsigned char)TVal);
			} else {
				SetBlend_(1,16,(unsigned short)i, 255);
			};
		};
	};
	SrcLine=BitMap->bmiHeader.biHeight;
	SrcLine--;
    ZielLine=0;
	memset(OSDBitMap.DataPtr,0x00,32768+1024);
	
	OSDBitMap.bpl=(unsigned short)BitMap->bmiHeader.biWidth/2;
    OSDBitMap.Offset=0;
    OSDBitMap.size = 0;
	
	i=OSDBitMap.bpl/4;
	Fix=OSDBitMap.bpl-(i*4);
	//    Fix=4-Fix;
	
	SrcPtr =(unsigned char *)BitMap;
    SrcPtr+=(sizeof(BITMAPINFOHEADER)+64);
	
	while (((OSDBitMap.size + OSDBitMap.bpl) < 32768 ) && ( SrcLine >= 0 )) {
		memcpy(&OSDBitMap.DataPtr[1024+(ZielLine*OSDBitMap.bpl)],&SrcPtr[SrcLine*(OSDBitMap.bpl+Fix)],OSDBitMap.bpl);
		ZielLine++;
		SrcLine--;
		OSDBitMap.size+=OSDBitMap.bpl;
	};
	
	OSDBitMap.size+=1024;
	
    memcpy(BitMap_mem_dma.pUserAddr,&OSDBitMap.DataPtr[0],OSDBitMap.size);
    ARM_LOAD_BITMAP(0,OSDBitMap.size);
	OSDBitMap.LoadBitMapEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
	ResetEvent(OSDBitMap.LoadBitMapEvent);
	SAA7146_OutCom(COMTYPE_OSD, LoadBmp, 3, BITMAP4, BitMap->bmiHeader.biWidth, BitMap->bmiHeader.biHeight);
	if ( WaitForSingleObject(OSDBitMap.LoadBitMapEvent, TimeToWait ) == WAIT_TIMEOUT ) {
		OSDBitMap.size=0;
		SAA7146_OutCom( COMTYPE_OSD, ReleaseBmp, 0,0);
		ResetEvent(OSDBitMap.LoadBitMapEvent);
		CloseHandle(OSDBitMap.LoadBitMapEvent);
		OSDBitMap.LoadBitMapEvent=NULL;
		return(-1);
				};
	ResetEvent(OSDBitMap.LoadBitMapEvent);
	CloseHandle(OSDBitMap.LoadBitMapEvent);
	OSDBitMap.LoadBitMapEvent=NULL;
	SAA7146_OutCom( COMTYPE_OSD, BlitBmp, 4, windownr+1 , 0, 0, 0x00);
	SAA7146_OutCom( COMTYPE_OSD, ReleaseBmp, 0,0);
	return(0);
};




void Get_DVB_MacAdr( unsigned char *MAC ) {
	unsigned char  k0[3] = { 0x54, 0x7B, 0x9E };
	unsigned char  k1[3] = { 0xD3, 0xF1, 0x23 };
	unsigned char Buffer[6];
	unsigned char *imac=&Buffer[0];
	unsigned char  tmp1, tmp0;
	
	
	unsigned char mm1=0xd4;
	struct i2c_msg msgs[2];
	int		i;
	
	msgs[0].flags=0;
	msgs[1].flags=I2C_M_RD;
	msgs[0].addr=msgs[1].addr=0x50;
	msgs[0].len=1; msgs[1].len=6;
	msgs[0].buf=&mm1; msgs[1].buf=Buffer;
	i=master_xfer(msgs,2);
	
	for(i=0; i<3; i++){
		tmp0 = *(imac++) ^ k0[i];
		tmp1 = *(imac++) ^ k1[i];
		MAC[5-i]=(unsigned char)(( ((tmp1<<8)|tmp0) >> ((tmp1 >> 6) & 0x3)));
	}
	MAC[0]=0x00;
	MAC[1]=0xd0;
	MAC[2]=0x5c;
	MAC[6]=0x00;
	MAC[7]=0x00;
	return;
};



int tuner_getstatus(void){
	char dat;
	i2c_master_recv((unsigned char )(DVBTuners[DVBTunerType].I2C/2),(unsigned char)0, &dat, 1);
	
	return dat;
};


int tuner_islocked (void){
	return (tuner_getstatus() & TUNER_FL);
}

int tuner_afcstatus(void){
	return (tuner_getstatus() & TUNER_AFC) - 2;
}

int tuner_mode(void){
	return (tuner_getstatus() & TUNER_MODE) >> 3;
}

void Set_TV_Freq( unsigned __int64 mfreq,int LNBNr){
	
	unsigned char config;
    unsigned __int64 div2;
	unsigned short div;
    unsigned char buffer[4];
	int swap=0;
    unsigned __int64 freq;
	char Text[128];
	
	if (( mfreq == altfreq ) && ( FastSwitch == TRUE )) {
		sprintf(Text,"SAA7146 Set_TV_Freq : Freq %d LNB %d schon eingestellt",(unsigned int)(mfreq),LNBNr);
		Write_Log(Text);
		return;
	};
	
	altfreq=mfreq;
	sprintf(Text,"SAA7146 Set_TV_Freq : Freq %d LNB %d Tuner %d ",(unsigned int)(mfreq),LNBNr,DVBTunerType);
	Write_Log(Text);
	
	if ( VES_TYPE == 2 ) writereg(ves_addr,ves_flags, 0x05, 0x85);
	
    if ( DVBTunerType == 3 ) {
		if ( mfreq > 12500000 ) {
			DVBTuners[DVBTunerType].config=0xCE;
		} else {
			DVBTuners[DVBTunerType].config=0x8E;
		};
	};
	
	if (( DVBTunerType == 1 ) || (DVBTunerType == 2 ) || (DVBTunerType == 3 ) || ((DVBTunerType == 5 ) && (DVBTuners[DVBTunerType].CableTuner == FALSE ))) {
		// SAT
		if (mfreq< (LNB[LNBNr].SwitchFreq*1000)) mfreq=(mfreq - LNB[LNBNr].LofLow*1000);
		else mfreq=(mfreq - LNB[LNBNr].LofHigh*1000);
		freq=mfreq*1000;
		sprintf(Text,"SAA7146 Set_TV_Freq Sat ");
		Write_Log(Text);
    } else {
		// Cable
		sprintf(Text,"SAA7146 Set_TV_Freq Cable/Terr");
		Write_Log(Text);
		freq=mfreq; 
	};
	
	freq*=DVBTuners[DVBTunerType].step;
	
	if (freq < DVBTuners[DVBTunerType].thresh1) 
		config = DVBTuners[DVBTunerType].VHF_L;
	else if (freq < DVBTuners[DVBTunerType].thresh2) 
		config = DVBTuners[DVBTunerType].VHF_H;
	else
		config = DVBTuners[DVBTunerType].UHF;
	
	config &= ~DVBTuners[DVBTunerType].mode;
	
	
	div2=freq + DVBTuners[DVBTunerType].IFPCoff;
    div2/=DVBTuners[DVBTunerType].res;
    div=(unsigned short)div2;
	
	/* some trickery which data is sent first */
	buffer[0] = buffer[2] = DVBTuners[DVBTunerType].config;
	buffer[1] = buffer[3] = config;
	
	buffer[swap]     =  (unsigned char )((div>>8) & 0x7f);
	buffer[swap + 1] =  (unsigned char )(div & 0xff);
	
	if (4 != i2c_master_send((unsigned char)(DVBTuners[DVBTunerType].I2C/2),(unsigned char)0,buffer,4))
	{
		sprintf(Text,"SAA7146 Set_TV_Freq fail : I2C-Addr %02x Freq %02x %02x %02x %02x",DVBTuners[DVBTunerType].I2C,buffer[0],buffer[1],buffer[2],buffer[3]);
		Write_Log(Text);
		i2c_reset();
	}
}


int SetStreamFilter( unsigned short pid , LPSTR Name){
	unsigned short buf[20];
	int ret;
	unsigned short index;
	char Text[256];
	
	memset(buf+4,0x00,32);
	buf[0] = (COMTYPE_PID_FILTER << 8) + AddPIDFilter; 
	buf[1] = 16;	
	buf[2] = pid & 0x1fff;
	buf[3] = SECTION_ANY|DATA_PIPING_FLAG;  
	
	ret=SAA7146_CommandRequest( buf, 20, &index, 1);          
	
	sprintf(Text,"SAA7146 SetStreamFilter : PID 0x%04X Return-Handle 0x%04X ",pid,index);
	Write_Log(Text);
	
	if (( index >= 0 ) && ( index < 32 )) {
        if ( FilterEvent[index] != NULL ) CloseHandle(FilterEvent[index]);
		FilterEvent[index]=CreateEvent(NULL,FALSE,FALSE,NULL);
		ResetEvent(FilterEvent[index]);
		Filter[index].pid = pid;
        sprintf(Filter[index].Name,"%s Pid 0x%04X",Name,pid);
        return((int)index);
	};
	
	return(-1);
};

int  Start_VT_Stream( unsigned short TextPid ){
	unsigned short buf[20];
	int ret;
	unsigned short index;
	char Text[256];
	
	if (( DVB_VT == FALSE )  && ( DVB_WINBIS == FALSE )) return (0);
	if ( VT_Stream_ID != -1 ) return(0);
	if ( TextPid == 0x1fff ) return(0);
	Set_TextPid=TextPid;
	memset(buf+4,0x00,32); // No specific filtering
	buf[0] = (COMTYPE_PID_FILTER << 8) + AddPIDFilter; 
	buf[1] = 16;	
	buf[2] = TextPid;
	buf[3] = SECTION_ANY|DATA_STREAMING_FLAG;
	
	ret=SAA7146_CommandRequest( buf, 20, &index, 1);          
	sprintf(Text,"SAA7146 Start_VT_Stream : PID 0x%04X Return-Handle 0x%04X ",TextPid,index);
	Write_Log(Text);
	
	if (( index >= 0 ) && ( index < 32 )) {
        if ( FilterEvent[index] != NULL ) CloseHandle(FilterEvent[index]);
        FilterEvent[index]=NULL;
		Filter[index].Irq_Proc = 0x03;
        sprintf(Filter[index].Name,t("VideoText Stream Pid 0x%04X"),TextPid);   
		Filter[index].pid = TextPid;
		return((int)index);
	};
	return(-1);
};


void Stop_VT_Stream( void ){
	if ( VT_Stream_ID == -1 ) return;
	(void)DelBitFilter(VT_Stream_ID);
	VT_Stream_ID=-1;
	Set_TextPid=0x1fff;
	return;
}


int StartLogFilter( unsigned short Pid , int Irq_Proc , LPSTR Name){
	unsigned short buf[20];
	int ret;
	unsigned short index;
	char Text[256];
	
	memset(buf+4,0x00,32); // No specific filtering
	buf[0] = (COMTYPE_PID_FILTER << 8) + AddPIDFilter; 
	buf[1] = 16;	
	buf[2] = Pid;
	buf[3] = SECTION_ANY | PBUFSIZE_1K | DATA_PIPING_FLAG;	
	ret=SAA7146_CommandRequest( buf, 20, &index, 1);          
	
	if (( index >= 0 ) && ( index < 32 )) {
		sprintf(Text,"SAA7146 StartLogFilter : PID 0x%04X (%s) Irq_Proc %d Return-Handle 0x%04X ",Pid,Name,Irq_Proc,index);
		Write_Log(Text);
        if ( FilterEvent[index] != NULL ) CloseHandle(FilterEvent[index]);
        FilterEvent[index]=NULL;
   	    strcpy(Filter[index].Name,Name);
		Filter[index].Irq_Proc = Irq_Proc;
		Filter[index].pid = Pid;
		return((int)index);
	};
	sprintf(Text,"SAA7146 StartLogFilter Fail: PID 0x%04X (%s) Irq_Proc %d Return-Handle 0x%04X ",Pid,Name,Irq_Proc,index);
	Write_Log(Text);
	return(-1);
};



int StartStreamFilter( ){
	unsigned short buf[20];
	int ret;
	unsigned short int index;
	char Text[256];
	
	StreamLog.StreamFileFd =fopen(StreamLog.StreamFile,"wb");
	if ( StreamLog.StreamFileFd == NULL  ) return(-1);
	
	memcpy(buf+4,&StreamLog.ExtFilter[0],32);
	
	buf[0] = (COMTYPE_PID_FILTER << 8) + AddPIDFilter; 
	buf[1] = 16;	
	buf[2] = StreamLog.Pid & 0x1fff;
	buf[3] = StreamLog.Mode;  
	
	
	   ret=SAA7146_CommandRequest( buf, 20, &index, 1);          
	   sprintf(Text,"SAA7146 StartStreamFilter : PID 0x%04X Mode %d Return-Handle 0x%04X ",StreamLog.Pid,StreamLog.Mode,index);
       Write_Log(Text);
	   
      	if (( index >= 0 ) && ( index < 32 )) {
			if ( FilterEvent[index] != NULL ) CloseHandle(FilterEvent[index]);
			FilterEvent[index]=NULL;
			StreamLog.Run=TRUE;
			StreamLog.FilterIndex = index;
			Filter[index].Irq_Proc = 0x02;
			StreamLog.KBytesWritten=0;
			sprintf(Filter[index].Name,t("Filter Log Pid 0x%04X"),StreamLog.Pid);
			Filter[index].pid = StreamLog.Pid;
			return((int)index);
		};
		
		if ( StreamLog.StreamFileFd != NULL ) fclose(StreamLog.StreamFileFd);
		StreamLog.StreamFileFd=NULL;
		StreamLog.FilterIndex=-1;
		StreamLog.Run=FALSE;
		return(-1);
};

void StopStreamFilter( void ){
	if ( StreamLog.Run == FALSE ) return;
	(void)DelBitFilter(StreamLog.FilterIndex);
	Sleep(1);Sleep(0);Sleep(1);Sleep(0);Sleep(1);Sleep(0);
	Sleep(1);Sleep(0);Sleep(1);Sleep(0);Sleep(1);Sleep(0);
	Sleep(1);Sleep(0);Sleep(1);Sleep(0);Sleep(1);Sleep(0);
	Sleep(1);Sleep(0);Sleep(1);Sleep(0);Sleep(1);Sleep(0);
	Sleep(1);Sleep(0);Sleep(1);Sleep(0);Sleep(1);Sleep(0);
	Sleep(1);Sleep(0);Sleep(1);Sleep(0);Sleep(1);Sleep(0);
	   if ( StreamLog.StreamFileFd != NULL ) fclose(StreamLog.StreamFileFd);
       StreamLog.StreamFileFd=NULL;
	   StreamLog.Run=FALSE;
	   StreamLog.FilterIndex=-1;
	   return;
}




int SetBitFilter(struct bitfilter *filter){
	unsigned short buf[20];
	int ret;
	unsigned short index;
	
	
	memcpy(buf + 4, filter->data, 32);
	buf[0] = (COMTYPE_PID_FILTER << 8) + AddPIDFilter; 
	buf[1] = 16;	
	buf[2] = filter->pid;
	buf[3] = filter->mode;
	ret=SAA7146_CommandRequest( buf, 20, &index, 1);          
	
	if (( index >= 0 ) && ( index < 32 )) {
        if ( FilterEvent[index] != NULL ) CloseHandle(FilterEvent[index]);
        FilterEvent[index]=NULL;
		FilterEvent[index]=CreateEvent(NULL,FALSE,FALSE,NULL);
		ResetEvent(FilterEvent[index]);
		Filter[index].Section=(buf[4]>>8);
		Filter[index].pid = filter->pid;
        return((int)index);
	};
	return(-1);
};


void ShutDownFiltersByName( LPSTR Name ){
	int i;
	char Text[256];
	for ( i = 0; i < MAXFILTER; i++ ) {
		if(( Filter[i].pid != 0xffff ) &&
			( Filter[i].Name[0] != 0x00 )      &&
			( strcmp(Filter[i].Name,Name) == 0 )) {
			DelBitFilter(i);
			sprintf(Text,"SAA7146 ShutdownFilter '%s' done.\n", Name);
			Write_Log(Text);
		}
	
	};
};


void ShutDownFilters( void ){
	int i;
	if ( VT_Stream_ID != -1 ) Stop_VT_Stream();
	if ( StreamLog.FilterIndex != -1 ) StopStreamFilter();
	for ( i = 0; i < MAXFILTER; i++ ) DelBitFilter(i);
	Reset_Debi();
}


void DelBitFilter(int FilterNummer){
	unsigned short buf[3];
	unsigned short answ[2];
	int ret;
	int Dll_id,Dll_Offset;
	char Text[256];
	unsigned short PtoClose;
	
	if (Filter == NULL || FilterNummer < 0 || FilterNummer >= MAXFILTER) return;
	PtoClose=Filter[FilterNummer].pid;
	Filter[FilterNummer].pid = 0xffff;
	
	if ( Filter[FilterNummer].Irq_Proc >= 256  )  {
		Dll_id = (Filter[FilterNummer].Irq_Proc-256)>>16;
		Dll_Offset = (Filter[FilterNummer].Irq_Proc-256)&0xffff;
		Filter[FilterNummer].Irq_Proc=0;
		if (( Dll_id < plugins_count ) && 
		    ( Dll_Offset < 8 ) &&
			( Ext_Dll[Dll_id].Extern_Filter_Close != NULL )) {
			( Ext_Dll[Dll_id].Extern_Filter_Close)(Dll_Offset);
			Ext_Dll[Dll_id].Extern_Stream_Function[Dll_Offset]=NULL;
		}
	};
	
	if ( FilterEvent[FilterNummer] != NULL ) CloseHandle(FilterEvent[FilterNummer]);
	FilterEvent[FilterNummer]=NULL;
	Filter[FilterNummer].Irq_Proc = 0;
	Filter[FilterNummer].Name[0]=0x00; 
	
	if (PtoClose != 0xffff ) {
        buf[0] = (COMTYPE_PID_FILTER << 8) + DelPIDFilter; 
        buf[1] = 1;	
        buf[2] = FilterNummer;
        ret=SAA7146_CommandRequest(buf, 3, answ, 2);          
		sprintf(Text,"SAA7146 DelBitFilter : Handle 0x%04X Return-Handle 0x%04X ",FilterNummer,answ[1]);
        Write_Log(Text);
	};

	
}


void Mapi_DelBitFilter(int FilterNummer){
	unsigned short buf[3];
	unsigned short answ[2];
	int ret;
	char Text[256];
	unsigned short PtoClose;
	int Dll_id,Dll_Offset;
	
	PtoClose=Filter[FilterNummer].pid;
	Filter[FilterNummer].pid = 0xffff;

	if ( Filter[FilterNummer].Irq_Proc >= 256  )  {
		Dll_id = (Filter[FilterNummer].Irq_Proc-256)>>16;
		Dll_Offset = (Filter[FilterNummer].Irq_Proc-256)&0xffff;
		Filter[FilterNummer].Irq_Proc=0;
		if (( Dll_id < plugins_count ) && 
			( Dll_Offset < 8 )) Ext_Dll[Dll_id].Extern_Stream_Function[Dll_Offset]=NULL;
	};

	
	if ( FilterEvent[FilterNummer] != NULL ) CloseHandle(FilterEvent[FilterNummer]);
	FilterEvent[FilterNummer]=NULL;
	Filter[FilterNummer].Irq_Proc = 0;
	Filter[FilterNummer].Name[0]=0x00; 
	
	if (PtoClose != 0xffff ) {
        buf[0] = (COMTYPE_PID_FILTER << 8) + DelPIDFilter; 
        buf[1] = 1;	
        buf[2] = FilterNummer;
        ret=SAA7146_CommandRequest(buf, 3, answ, 2);          
		sprintf(Text,"SAA7146 DelBitFilter : Handle 0x%04X Return-Handle 0x%04X ",FilterNummer,answ[1]);
        Write_Log(Text);
	};
	
}



int SetFilter(unsigned short pid, unsigned short section, unsigned short mode, LPSTR Name) { 
    int i;
	char Text[255];
	struct bitfilter filt;
	if ( pid == 0x1fff ) return(0);
	filt.pid = pid;
    
	
	for ( i=0; i < 16 ; i++ ) filt.data[i]=0x0000;
	filt.data[0]= section;
	filt.mode   = mode;
	filt.handle = 0;
	filt.flags  = 0;
	
	i=SetBitFilter(&filt);
	if ( i >= 0 ) strcpy( Filter[i].Name,Name);
    sprintf(Text,"SAA7146 SetFilter : PID 0x%04X  Section 0x%04X  Mode 0x%04X  (%s) Return %d ",pid,section,mode,Name,i);
    Write_Log(Text);
	return(i);
};

void Scan_TP( HWND Ausgabe){
	char Zeile[128];
	int i, j;
	Has_Pat = FALSE;
	Has_Sdt = FALSE;
	
	for ( i=0; i < NEUSIZE ; i++ ) {
		memset(&ProgrammNeu[i].Name[0],0x00,sizeof( struct TProgramm));
		ProgrammNeu[i].Video_pid=0x1fff;        
		ProgrammNeu[i].Audio_pid=0x1fff;
		ProgrammNeu[i].TeleText_pid=0x1fff;          /* Teletext PID */
		ProgrammNeu[i].CA_Anzahl=0;
		ProgrammNeu[i].CA_Land[0]=0;
		ProgrammNeu[i].PMT_pid=0x1fff;
		ProgrammNeu[i].PCR_pid=0x1fff;
		ProgrammNeu[i].ECM_PID=0x1fff;
		ProgrammNeu[i].SID_pid=0x1fff;
		ProgrammNeu[i].AC3_pid=0x1fff;
		ProgrammNeu[i].Merker=0x00;
		ProgrammNeu[i].Link_SID=0xffff;
		ProgrammNeu[i].Link_TP=0xffff;
		ProgrammNeu[i].Dynamisch=0x00;
	};

    if ( Ausgabe != NULL ) SetWindowText(Ausgabe,t("Lade PAT"));
	
   	Has_Pat = Get_Pat();
   	if ( Has_Pat ) {
		Transponder.ts_id=PAT.ts_id;
		sprintf(Zeile,t("PAT TS-Id %d Anzahl %d   Lade NIT"),PAT.ts_id,PAT.ProgAnzahl);
		if ( Ausgabe != NULL ) SetWindowText(Ausgabe,Zeile);
		if ( Get_Nit((unsigned char)Transponder.diseqc, TRUE) == TRUE ) {
			for (j=0; j<MAXNIT; j++) {
				if (NITNeu[j].freq > 0 && NITNeu[j].ts_id == PAT.ts_id) {
					if (labs(Transponder.freq - NITNeu[j].freq) <= 3000) {
						sprintf(Zeile, "Adjusting frequency from %.3f to %.3f", Transponder.freq/1000., NITNeu[j].freq/1000.);
						Write_Log(Zeile);
						Transponder.freq = NITNeu[j].freq;
						break;
					}
				}
			}
		}
		sprintf(Zeile,t("PAT TS-Id %d Anzahl %d   Lade SDT"),PAT.ts_id,PAT.ProgAnzahl);
   	} else {
		sprintf(Zeile,t("Kein PAT  Lade SDT"));
   	}
	if ( Ausgabe != NULL ) SetWindowText(Ausgabe,Zeile);
	
   	Has_Sdt = Get_SDT();
   	if ( Has_Sdt ) {
		sprintf(Zeile,t("PAT TS-Id %d Anzahl %d   SDT Network Id %d Anzahl %d"),PAT.ts_id,PAT.ProgAnzahl,SDT.orig_nw_id,SDT.ServiceAnzahl);
   	} else if ( Has_Pat ) {
		Transponder.ts_id=PAT.ts_id;
		sprintf(Zeile,t("PAT TS-Id %d Anzahl %d   Kein SDT"),PAT.ts_id,PAT.ProgAnzahl);
	} else {
		sprintf(Zeile,t("Kein PAT  Kein SDT"));
   	}
	
   	if ( Ausgabe != NULL ) SetWindowText(Ausgabe,Zeile);
	
	for ( i=0; i < NEUSIZE ; i++ ) {
		if ( ProgrammNeu[i].SID_pid != 0x1fff ) {
			ProgrammNeu[i].afc=FALSE;
			ProgrammNeu[i].fec=Transponder.fec;
			ProgrammNeu[i].freq=Transponder.freq;
			ProgrammNeu[i].diseqc=Transponder.diseqc;
			ProgrammNeu[i].qam=Transponder.qam;
			ProgrammNeu[i].srate=Transponder.srate;
			ProgrammNeu[i].volt=Transponder.volt;
			ProgrammNeu[i].tp_id=Transponder.ts_id;
		}
		
		if ( ProgrammNeu[i].SID_pid == 0x0000 )  strcpy(ProgrammNeu[i].Name,t("NIT-Tabelle"));
	}
}

static void dump_hex(unsigned char *ptr, unsigned short len) {
    unsigned char Text[256];
    unsigned short i, j, k, l;

    if (len == 0) return;
    if (len > 4096) len = 4096;
    for (i=0; i<len; i+=16) {

      for (j=0; j<16 && i+j<len; j++) {
        Text[3*j] = ' ';
        Text[3*j+1] = (((ptr[i+j]>>4) > 9) ? '7':'0')+(ptr[i+j]>>4);
        Text[3*j+2] = (((ptr[i+j]&0x0f) > 9) ? '7':'0')+(ptr[i+j]&0x0f);
      }
      for (l=j; l<17; l++) {
        Text[3*l] = ' ';
        Text[3*l+1] = ' ';
        Text[3*l+2] = ' ';
      }
	  j = i;
      for (k=0; k<16 && j+k<len; k++) {
        Text[50+k] = (ptr[j+k] >= ' ') ? ptr[j+k] : '.' ;
      }
      Text[0] = '\t';
      Text[50+k] = 0;
      Write_Log(Text);
    }
}

int service_has_moved;
unsigned short new_orig_nw_id, new_ts_id, new_service_id;

BOOL Get_PMT(int ProgrammNummer, unsigned short pid ){
#define PMT_LEN 12
	
	struct pmt_struct {
		u_char table_id;
		u_char BitField1;
		// u_char section_length_hi		: 4;
		// u_char					: 2;
		// u_char dummy				: 1; // has to be 0
		// u_char section_syntax_indicator		: 1;
		u_char section_length_lo;
		u_char program_number_hi;
		u_char program_number_lo;
		u_char BitField2;
		// u_char current_next_indicator		: 1;
		// u_char version_number			: 5;
		// u_char					: 2;
		u_char section_number;
		u_char last_section_number;
		u_char BitField3;
		// u_char PCR_PID_hi			: 5;
		// u_char					: 3;
		u_char PCR_PID_lo;
		u_char BitField4;
		//u_char program_info_length_hi		: 4;
		// u_char					: 4;
		
		u_char program_info_length_lo;
		//descriptors
	};
	
	
#define PMT_info_LEN 5
	
	struct pmt_info_struct {
		u_char stream_type;
		u_char BitField1;
		// u_char elementary_PID_hi		: 5;
		// u_char					: 3;
		u_char elementary_PID_lo;
		u_char BitField2;
		// u_char ES_info_length_hi		: 4;
		// u_char					: 4;
		
		u_char ES_info_length_lo;
		// descriptors
	};
	
	
	
	int sec_len;
	int stream_len;
	int Loop = 0;
	int PMTFilter;
	
	unsigned char *ptr;
	struct pmt_struct *pmt_ptr;
	struct pmt_info_struct *info;
	unsigned char stream_type;
	unsigned short info_length;
	char Text[256];
	int ngets = 0;
#define MAX_PMT_GETS 100
	
	service_has_moved = FALSE;
	
	PMT.StreamAnzahl=0;
	if ( pid == 0x1fff ) return(TRUE);
	
	PMTFilter=SetFilter(pid, (unsigned short)((TID_PMT<<8)|0x00ff), SECTION_ANY|SECTION_CONTINUOUS|PBUFSIZE_NONE, "PMT Filter");
	if ( PMTFilter < 0 ) {
		MessageBox(hWnd,t("Keine HW-Filter mehr frei !!!!! "),t("MultiDec"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	
	sprintf(Text, "Waiting for PMT for program SID %d", ProgrammNeu[ProgrammNummer].SID_pid);
	Write_Log(Text);

	while ( Loop >= 0 && ngets++ <= MAX_PMT_GETS) {
		if ( Loop > 0 ) ResetEvent(FilterEvent[PMTFilter]);
		if ( WaitForSingleObject(FilterEvent[PMTFilter], Timing.Pmt_TimeOut ) == WAIT_TIMEOUT ) {
			(void)DelBitFilter(PMTFilter);
			return(FALSE);
		};
		
		pmt_ptr=(struct pmt_struct *)Filter[PMTFilter].Buffer;
		
		if (pmt_ptr->table_id != TID_PMT) {
			sprintf(Text, "Incorrect table id 0x%02x in PMT stream", pmt_ptr->table_id);
			Write_Log(Text);
			continue;
		}
		
		sec_len = ((pmt_ptr->BitField1&0x0f)<<8)+pmt_ptr->section_length_lo;
		if (sec_len < 12 || sec_len > 1024 ||
	          ccitt32_checkcrc(Filter[PMTFilter].Buffer, sec_len+3) != 0) continue;
		sec_len--; /* + 3 - 4 for CRC */

		PMT.info_length= ((pmt_ptr->BitField4&0x0f)<<8)+pmt_ptr->program_info_length_lo;
		
		if (sec_len < PMT.info_length + PMT_LEN ) {
			sprintf(Text, "Incorrect length %d in PMT stream, ending Get_PMT", sec_len);
			Write_Log(Text);
			(void)DelBitFilter(PMTFilter);
			return(FALSE);
		}
		
		sprintf(Text, "Received PMT for program SID %d, len = %d, section %d/%d, current = %d",
				  HILO(pmt_ptr->program_number), sec_len, pmt_ptr->section_number, pmt_ptr->last_section_number, pmt_ptr->BitField2 & 0x01);
		Write_Log(Text);
		if (pmt_ptr->section_number != Loop ||
		    (pmt_ptr->BitField2 & 0x01) == 0 ||
		    ProgrammNeu[ProgrammNummer].SID_pid != HILO (pmt_ptr->program_number)) continue;

		if ( Loop == pmt_ptr->last_section_number ) Loop = -1;
		else Loop++;
		
		ProgrammNeu[ProgrammNummer].PMT_pid=pid;
		ProgrammNeu[ProgrammNummer].Filteranzahl=0;
		ProgrammNeu[ProgrammNummer].CA_Anzahl=0;
		PMT.prog_nr = HILO (pmt_ptr->program_number);
		PMT.version = (pmt_ptr->BitField2 >> 1)&0x1f;
		PMT.pcr_pid=((pmt_ptr->BitField3&0x1f)<<8)+pmt_ptr->PCR_PID_lo;
		ProgrammNeu[ProgrammNummer].PCR_pid=PMT.pcr_pid;
			
		ptr=Filter[PMTFilter].Buffer + PMT_LEN;
			
		sprintf(Text, "== Descriptors for this PMT == (ptr = %d, len = %d)", ptr-Filter[PMTFilter].Buffer, PMT.info_length);
		Write_Log(Text);
		Parse_PMT_Infos(ptr, PMT.info_length, ProgrammNummer, 0, 0, -1);
			
			if ( PMT.info_length > 0 ) ptr+=PMT.info_length;
			
		stream_len = (sec_len - PMT.info_length - PMT_LEN);
				
		while (stream_len >= PMT_info_LEN) {
				info=(struct pmt_info_struct *)ptr;
			ptr+= PMT_info_LEN;
				stream_type = info->stream_type;
				pid = (( info->BitField1&0x1f)<<8)+info->elementary_PID_lo;
				info_length = (( info->BitField2&0x0f)<<8)+info->ES_info_length_lo;
			sprintf(Text, "== Descriptors for PID %d of type %d == (ptr = %d, len = %d)", pid, stream_type, ptr-Filter[PMTFilter].Buffer, info_length);
			Write_Log(Text);
			Parse_PMT_Infos(ptr, info_length, ProgrammNummer, stream_type, pid, PMT.StreamAnzahl);
			stream_len -= PMT_info_LEN+info_length;
				if ( info_length > 0 ) ptr+=info_length;
				PMT.StreamAnzahl++;
			}
		if ( ProgrammNeu[ProgrammNummer].CA_Anzahl > 0 ) ProgrammNeu[ProgrammNummer].CA_ID = 0x01;
		sprintf(Text, "== Total of %d streams, %d filters and %d CA stored. ==",
			 PMT.StreamAnzahl, ProgrammNeu[ProgrammNummer].Filteranzahl, ProgrammNeu[ProgrammNummer].CA_Anzahl);
		Write_Log(Text);
		if (stream_len > 0) {
			sprintf(Text, "(remaining len = %d)", stream_len);
			Write_Log(Text);
			dump_hex(ptr, (unsigned short)stream_len);
		}

	}

    if (ngets >= MAX_PMT_GETS) Write_Log("Max gets reached for PMT");
	(void)DelBitFilter(PMTFilter);

	if (service_has_moved) { /* Set by parse_PMT_Infos */
		sprintf(Text, "This service has moved :\n"
				  "      new original network id = %d, \n"
				  "      new TS id = %d, \n"
				  "      new service id = %d.",
			  new_orig_nw_id, new_ts_id, new_service_id);
		Write_Log(Text);
		MessageBox(hWnd, Text, "Service move", MB_ICONINFORMATION|MB_OK);
	}
	return(TRUE);
}


static void insert_new_ca_system(int ProgrammNummer, unsigned short CA_Typ, unsigned int Prov_Ident, unsigned short CA_system_ID, int stream_anzahl, int is_EMM) {
	unsigned int j;

	if (ProgrammNummer < 0) return;
    j=0;
	while ( j < ProgrammNeu[ProgrammNummer].CA_Anzahl ) {
		if (( ProgrammNeu[ProgrammNummer].CA_System[j].CA_Typ == CA_Typ ) &&
		    ( ProgrammNeu[ProgrammNummer].CA_System[j].Provider_Id == Prov_Ident )) break;
		j++;
	}

	if ( j < MAX_CA_SYSTEMS ) {
		if ( j >= ProgrammNeu[ProgrammNummer].CA_Anzahl ) { /* New value to insert */
			if (stream_anzahl >= 0) { /* if from a stream, then insert on top */
				memmove(&ProgrammNeu[ProgrammNummer].CA_System[1],
					  &ProgrammNeu[ProgrammNummer].CA_System[0],
					  ProgrammNeu[ProgrammNummer].CA_Anzahl*sizeof(ProgrammNeu[0].CA_System[0]));
				j = 0;
			} /* else insert after */
			ProgrammNeu[ProgrammNummer].CA_Anzahl++;
		} /* else value to update */
	} else { /* array is full */
		if (stream_anzahl >= 0) { /* if from a stream, then insert on top, drop oldest */
			memmove(&ProgrammNeu[ProgrammNummer].CA_System[1],
				  &ProgrammNeu[ProgrammNummer].CA_System[0],
				  (ProgrammNeu[ProgrammNummer].CA_Anzahl-1)*sizeof(ProgrammNeu[0].CA_System[0]));
			j = 0;
		} /* else drop it */
      }
	if ( j < MAX_CA_SYSTEMS ) {
		ProgrammNeu[ProgrammNummer].CA_System[j].CA_Typ = CA_Typ;
      	ProgrammNeu[ProgrammNummer].CA_System[j].Provider_Id = Prov_Ident;
		if (is_EMM) {
			ProgrammNeu[ProgrammNummer].CA_System[j].EMM = CA_system_ID;
		} else {
			ProgrammNeu[ProgrammNummer].CA_System[j].ECM = CA_system_ID;
		}
	}
}

void parse_CA_Infos(int ProgrammNummer, unsigned char *MyPtr, int stream_anzahl, int is_EMM) {

#define CA_info_LEN 6
	struct ca_info_struct {
		u_char CA_Ident_hi;
		u_char CA_Ident_lo;
		u_char ECM_hi;
		u_char ECM_lo;
	
	};
	
	int j, l;
	unsigned short CA_Typ;
	unsigned short CA_ECM;
	unsigned int   Prov_Ident;
	unsigned short other;
	unsigned char tagid, taglen;
	struct ca_info_struct *ca_info;
    char Text[256];
    static const char typ[2][4] = {"ECM", "EMM"};

    ca_info=(struct ca_info_struct *)(MyPtr+2);
	CA_Typ = HILO(ca_info->CA_Ident);
	CA_ECM = HILO(ca_info->ECM) & 0x1fff;

	Prov_Ident=0x00;

	switch (CA_Typ>>8) {

	case ID_CRYPT_SECA :
        if (is_EMM) {
            sprintf(Text, "--> Seca CA, sub = 0x%02x, EMM = %d. List of providers :", CA_Typ&0xff, CA_ECM);
    	    Write_Log(Text);
            for (j=7; j<7+4*MyPtr[6] && j<get_descr_len(MyPtr)+2; j+=4) {;
			    CA_ECM = ((MyPtr[j]&0x1f)<<8) | MyPtr[j+1];
			    Prov_Ident = (MyPtr[j+2]<<8) | MyPtr[j+3];
                sprintf(Text, "      Provider = 0x%04x, PID = %d", Prov_Ident, CA_ECM);
			    Write_Log(Text);
			    insert_new_ca_system(ProgrammNummer, CA_Typ, Prov_Ident, CA_ECM, stream_anzahl, is_EMM);
            }
            l = get_descr_len(MyPtr)+2-j;
            if (l>0) {
                Write_Log("    Remaining info :");
                dump_hex(MyPtr+j, (unsigned short) l);
            }
        } else {
    		if ( (CA_Typ&0x00ff) == 0x00 ) {
		        for (j=4; j<get_descr_len(MyPtr)+2; j+=15) {
			        CA_ECM = ((MyPtr[j]&0x1f)<<8) | MyPtr[j+1];
			        Prov_Ident = (MyPtr[j+2]<<8) | MyPtr[j+3];
                    sprintf(Text, "--> Seca CA, sub = 0x%02x, provider = 0x%04x, ECM = %d [%d]. Other info :", CA_Typ&0xff, Prov_Ident, CA_ECM, (j+11)/15);
			        Write_Log(Text);
                    l = get_descr_len(MyPtr)+2-j-4;
                    if (l>11) l=11;
			        dump_hex(MyPtr+j+4, (unsigned short) l);
			        insert_new_ca_system(ProgrammNummer, CA_Typ, Prov_Ident, CA_ECM, stream_anzahl, is_EMM);
		        }
		    } else {
			    CA_ECM = ((MyPtr[4]&0x1f)<<8) | MyPtr[5];
			    Prov_Ident = (MyPtr[6]<<8) | MyPtr[7];
                sprintf(Text, "--> Seca CA, sub = 0x%02x, provider = 0x%04x, %s = %d. Other info :", CA_Typ&0xff, Prov_Ident, typ[is_EMM?1:0], CA_ECM);
    		    Write_Log(Text);
			    if (get_descr_len(MyPtr)>6) dump_hex(MyPtr+8, (unsigned short)(get_descr_len(MyPtr)-6));
			    insert_new_ca_system(ProgrammNummer, CA_Typ, Prov_Ident, CA_ECM, stream_anzahl, is_EMM);
            }
		}
		break;

	case ID_CRYPT_VIACCESS :
		j=6;
		while ( j < get_descr_len(MyPtr) ) {
			tagid = MyPtr[j];
			taglen = MyPtr[j+1];
			switch (tagid) {
			case 0x14 :
				if (taglen == 3) { 
          			Prov_Ident = (MyPtr[j+2] << 16) | ( MyPtr[j+3]<<8) | (MyPtr[j+4]&0xf0);
					sprintf(Text, "--> Viaccess CA, sub = 0x%02x, provider = 0x%06x, %s = %d", CA_Typ&0xff, Prov_Ident, typ[is_EMM?1:0], CA_ECM);
					Write_Log(Text);
					insert_new_ca_system(ProgrammNummer, CA_Typ, Prov_Ident, CA_ECM, stream_anzahl, is_EMM);
			}
				break;
			}
			j += taglen+2;
		}
		if (get_descr_len(MyPtr)>4) dump_hex(MyPtr+6, (unsigned short)(get_descr_len(MyPtr)-4));
		break;
				
	case ID_CRYPT_IRDETO :
		sprintf(Text, "--> Irdeto CA, sub = 0x%02x, %s = %d", CA_Typ&0xff, typ[is_EMM?1:0], CA_ECM);
		Write_Log(Text);
		if (get_descr_len(MyPtr)>4) dump_hex(MyPtr+6, (unsigned short)(get_descr_len(MyPtr)-4));
		insert_new_ca_system(ProgrammNummer, CA_Typ, Prov_Ident, CA_ECM, stream_anzahl, is_EMM);
		break;

	case ID_CRYPT_VIDEOGUARD :
		sprintf(Text, "--> VideoGuard CA, sub = 0x%02x, %s = %d", CA_Typ&0xff, typ[is_EMM?1:0], CA_ECM);
		Write_Log(Text);
		if (get_descr_len(MyPtr)>4) dump_hex(MyPtr+6, (unsigned short)(get_descr_len(MyPtr)-4));
		insert_new_ca_system(ProgrammNummer, CA_Typ, Prov_Ident, CA_ECM, stream_anzahl, is_EMM);
		break;

	case ID_CRYPT_CRYPTOWORKS :
		sprintf(Text, "--> CryptoWorks CA, sub = 0x%02x, %s = %d", CA_Typ&0xff, typ[is_EMM?1:0], CA_ECM);
		Write_Log(Text);
		if (get_descr_len(MyPtr)>4) dump_hex(MyPtr+6, (unsigned short)(get_descr_len(MyPtr)-4));
		insert_new_ca_system(ProgrammNummer, CA_Typ, Prov_Ident, CA_ECM, stream_anzahl, is_EMM);
		break;
					
	case ID_CRYPT_POWERVU :
		sprintf(Text, "--> PowerVu CA, sub = 0x%02x, %s = %d", CA_Typ&0xff, typ[is_EMM?1:0], CA_ECM);
		Write_Log(Text);
		if (get_descr_len(MyPtr)>4) dump_hex(MyPtr+6, (unsigned short)(get_descr_len(MyPtr)-4));
		insert_new_ca_system(ProgrammNummer, CA_Typ, Prov_Ident, CA_ECM, stream_anzahl, is_EMM);
		break;

	case ID_CRYPT_BETACRYPT :
        if (is_EMM) {
		    sprintf(Text, "--> BetaCrypt CA, sub = 0x%02x, EMM = %d", CA_Typ&0xff, CA_ECM);
		    Write_Log(Text);
		    if (get_descr_len(MyPtr)>4) dump_hex(MyPtr+6, (unsigned short)(get_descr_len(MyPtr)-4));
        } else {
		    other = (MyPtr[6]<<8)|MyPtr[7];
		    sprintf(Text, "--> BetaCrypt CA, sub = 0x%02x, %s = %d, extra = 0x%04x", CA_Typ&0xff, typ[is_EMM?1:0], CA_ECM, other);
		    Write_Log(Text);
		    if (get_descr_len(MyPtr)>6) dump_hex(MyPtr+8, (unsigned short)(get_descr_len(MyPtr)-6));
        }
		insert_new_ca_system(ProgrammNummer, CA_Typ, Prov_Ident, CA_ECM, stream_anzahl, is_EMM);
		break;

	case ID_CRYPT_NAGRAVISION :
		sprintf(Text, "--> NagraVision CA, sub = 0x%02x, %s = %d", CA_Typ&0xff, typ[is_EMM?1:0], CA_ECM);
		Write_Log(Text);
		if (get_descr_len(MyPtr)>4) dump_hex(MyPtr+6, (unsigned short)(get_descr_len(MyPtr)-4));
		insert_new_ca_system(ProgrammNummer, CA_Typ, Prov_Ident, CA_ECM, stream_anzahl, is_EMM);
		break;

	default:
		sprintf(Text, "--> Unknown CA : Typ = 0x%04x, %s = %d. Other info :", CA_Typ, typ[is_EMM?1:0], CA_ECM);
		Write_Log(Text);
		if (get_descr_len(MyPtr)>4) dump_hex(MyPtr+6, (unsigned short)(get_descr_len(MyPtr)-4));
		insert_new_ca_system(ProgrammNummer, CA_Typ, Prov_Ident, CA_ECM, stream_anzahl, is_EMM);
	}
					
}
				
void Parse_PMT_Infos( unsigned char *BasePtr, int info_len, int ProgrammNummer, unsigned char stream_type, unsigned short pid, int stream_anzahl)
{
	char Text[256];
	int i, j;
	unsigned char *MyPtr;
	char filter_name[32] = "";
	int data_is_teletext = 0;
	int data_is_AC3 = 0;

	if ( info_len >= 2 ) {
	  i=0;
	  MyPtr=BasePtr;
      
	  while ( i+2+get_descr_len(MyPtr)  <= info_len ) {
		sprintf(Text, "Descr. 0x%02x, len = %d", get_descr(MyPtr), get_descr_len(MyPtr));
		Write_Log(Text);
		switch ( get_descr(MyPtr) ) {
		case DESCR_VIDEO_STREAM:
			sprintf(Text, "--> Video stream : Multiple rate = %d,"
					  "  Frame rate code = %d,"
					  "  MPEG1 only = %d,"
					  "  Constrained_parameter = %d,"
					  "  Still picture = %d",
					  MyPtr[2]>>7, (MyPtr[2]>>3)&0x0f, (MyPtr[2]>>2)&0x01, (MyPtr[2]>>1)&0x01, (MyPtr[2]>>1)&0x01);
			Write_Log(Text);
			if (MyPtr[2]&0x04) {
				sprintf(Text, "  Profile and level indication = 0x%02x,"
						  "  Chroma format = %d,"
						  "  Frame extension flag = %d",
						  MyPtr[3], (MyPtr[4]>>6), (MyPtr[4]>>5)&0x01);
				Write_Log(Text);
			}
			break;
		case DESCR_AUDIO_STREAM:
			sprintf(Text, "--> Audio stream : Free format = %d,"
					  "  ID = %d,"
					  "  Layer = %d,"
					  "  Variable rate = %d",
					  MyPtr[2]>>7, (MyPtr[2]>>6)&0x01, (MyPtr[2]>>4)&0x03, (MyPtr[2]>>3)&0x01);
			Write_Log(Text);
			break;
		case DESCR_HIERARCHY:
			Write_Log("--> Hierarchy descriptor info :");
			dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
			break;
		case DESCR_REGISTRATION:
			Write_Log("--> Registration descriptor info :");
			dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
			break;
		case DESCR_DATA_STREAM_ALIGN:
			Write_Log("--> Data stream align descriptor info :");
			dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
			break;
		case DESCR_TARGET_BACKGRID:
			sprintf(Text, "--> Target backgrid w=%d, h=%d, aspect ratio=%d",
				(MyPtr[2]<<6)|(MyPtr[3]>>2),
				((MyPtr[3]&0x3)<<12)|(MyPtr[4]<<4)|(MyPtr[5]>>4),
				MyPtr[5]&0x0f);
			Write_Log(Text);
			break;
		case DESCR_VIDEO_WINDOW:
			Write_Log("--> Video window descriptor info :");
			dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
			break;
		case DESCR_CA :
			parse_CA_Infos(ProgrammNummer, MyPtr, stream_anzahl, FALSE); 
			break;
		case DESCR_ISO_639_LANGUAGE :
			for (j=0; j<get_descr_len(MyPtr)-3; j+=4) {
				sprintf(Text, "--> Language : %-3.3s, type = %d.", MyPtr+2+j, MyPtr[2+j+3]);
				Write_Log(Text);

				ProgrammNeu[ProgrammNummer].CA_Land[3]=0x00;
				memcpy(ProgrammNeu[ProgrammNummer].CA_Land, MyPtr+2, 3);
				memcpy(filter_name, ProgrammNeu[ProgrammNummer].CA_Land, 4);
			}
			
			break;
		case DESCR_SYSTEM_CLOCK:
			if (MyPtr[2]&0x3f) {
				sprintf(Text, "--> System clock : external = %d, accuracy = %d*10^-%d ppm",
						 MyPtr[2]>>7, MyPtr[2]&0x3f, MyPtr[3]>>5);
			} else {
				sprintf(Text, "--> System clock : external = %d, accuracy = 30 ppm",
						 MyPtr[2]>>7);
			}
			Write_Log(Text);
			break;
		case DESCR_MULTIPLEX_BUFFER_UTIL:
			Write_Log("--> Multiplex buffer util descriptor info :");
			dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
			break;
		case DESCR_COPYRIGHT:
			Write_Log("--> Copyright descriptor info :");
			dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
			break;
		case DESCR_MAXIMUM_BITRATE:
			sprintf(Text, "--> Maximum bitrate = %u bits/s\n",
					 (((MyPtr[2]&0x3f)<<16)|(MyPtr[3]<<8)|MyPtr[4])*400);
			Write_Log(Text);
			break;
		case DESCR_PRIVATE_DATA_IND:
			sprintf(Text, "--> Private data indicator = 0x%02x%02x%02x%02x", MyPtr[2], MyPtr[3], MyPtr[4], MyPtr[5]);
			Write_Log(Text);
			break;
		case DESCR_SMOOTHING_BUFFER:
			sprintf(Text, "--> Smoothing buffer : leak rate = %u bits/s, size = %u bytes",
				  (((MyPtr[2]&0x3f)<<16)|(MyPtr[3]<<8)|MyPtr[4])*400,
				  ((MyPtr[5]&0x3f)<<16)|(MyPtr[6]<<8)|MyPtr[7]);
			Write_Log(Text);
			break;
		case DESCR_STD:
			Write_Log("--> STD descriptor info :");
			sprintf(Text, "      leak_valid = %d", MyPtr[2]&0x01);
			Write_Log(Text);
			break;
		case DESCR_IBP:
			Write_Log("--> IBP descriptor info :");
			dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
			break;
		
		case DESCR_VBI_DATA:
			Write_Log("--> VBI data descriptor info :");
			dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
			break;
		case DESCR_VBI_TELETEXT:
			Write_Log("--> VBI teletext descriptor info :");
			dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
			break;
		case DESCR_MOSAIC:
			Write_Log("--> Mosaic descriptor info :");
			dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
			break;
		case DESCR_STREAM_ID:
			sprintf(Text, "--> Stream ID descriptor : tag = %d", MyPtr[2]);
			Write_Log(Text);
			break;
		case DESCR_TELETEXT:
			Write_Log("--> Teletext descriptor info :");
			for (j=0; j<get_descr_len(MyPtr)-4; j+=5) {
				sprintf(Text, "      language = %c%c%c, type = %d, magazine = %d, page = %02x",
					MyPtr[j+2], MyPtr[j+3], MyPtr[j+4], MyPtr[j+5]>>3, MyPtr[j+5]&0x07, MyPtr[j+6]);
				Write_Log(Text);
			}
			data_is_teletext = TRUE;
			break;
		case DESCR_SUBTITLING:
			Write_Log("--> Subtitling descriptor info :");
			dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
			break;
		case DESCR_PRIV_DATA_SPEC:
			sprintf(Text, "--> Private data specifier = 0x%02x%02x%02x%02x", MyPtr[2], MyPtr[3], MyPtr[4], MyPtr[5]);
			Write_Log(Text);
			break;
		case DESCR_SERVICE_MOVE:
			new_orig_nw_id = (MyPtr[2]<<8)|MyPtr[3];
			new_ts_id = (MyPtr[4]<<8)|MyPtr[5];
			new_service_id = (MyPtr[6]<<8)|MyPtr[7];
			service_has_moved = TRUE;
			Write_Log("--> Service move :");
			sprintf(Text, "      New original network id = %d, "
						  "new TS id = %d, "
						  "new service id = %d",
				  new_orig_nw_id, new_ts_id, new_service_id);
			Write_Log(Text);
			break;
		case DESCR_CA_SYSTEM:
			Write_Log("--> CA system descriptor info :");
			dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
			break;
		case DESCR_DATA_BROADCAST_ID:
            j = get_descr_len(MyPtr)-2;
			sprintf(Text, "--> Data broadcast ID = %d, selector (len = %d) :",
                (MyPtr[2]<<8)|MyPtr[3], j);
			Write_Log(Text);
            if (j > 0) dump_hex(MyPtr+4, (unsigned short) j);
			break;
		case DESCR_AC3:
			Write_Log("--> AC3 descriptor info :");
			j = 0;
			if (MyPtr[2] & 0x80) {
				sprintf(Text, "      AC3 type = 0x%02x", MyPtr[2+j++]);
				Write_Log(Text);
			}
			if (MyPtr[2] & 0x40) {
				sprintf(Text, "      bsid = 0x%02x", MyPtr[2+j++]);
				Write_Log(Text);
			}
			if (MyPtr[2] & 0x20) {
				sprintf(Text, "      mainid = 0x%02x", MyPtr[2+j++]);
				Write_Log(Text);
			}
			if (MyPtr[2] & 0x10) {
				sprintf(Text, "      asvc = 0x%02x", MyPtr[2+j++]);
				Write_Log(Text);
			}
			if (get_descr_len(MyPtr)-j-1 > 0) {
				Write_Log("      Additional info :");
				dump_hex(MyPtr+3+j, (unsigned short)(get_descr_len(MyPtr)-j-1));
			}
			data_is_AC3 = TRUE;
			break;
		case DESCR_ANCILLARY_DATA:
			Write_Log("--> Ancillary data descriptor info :");
			dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
			break;
		case 0xc5:	/* Radio id and name (CanalSat) */
			if (MyPtr[2] != 0x00 &&
			    (stream_type == STREAMTYPE_11172_AUDIO || stream_type == STREAMTYPE_13818_AUDIO)) {
				sprintf(Text, "--> Radio id = %d, name = '%s'", MyPtr[2], MyPtr+3);
				Write_Log(Text);
				if (filter_name[0] == 0) strncpy(filter_name, MyPtr+3, 31);
			} else {
				Write_Log("--> Private descriptor info :");
				dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
			}
			break;
		default :
			if (get_descr(MyPtr) >= 0x6f && get_descr(MyPtr) <= 0xfe) {
				Write_Log("--> Private descriptor info :");
			} else {
				Write_Log("--> Unexpected descriptor info :");
			}
			dump_hex(MyPtr+2, (unsigned short)(get_descr_len(MyPtr)));
		}
	
		i+=(get_descr_len(MyPtr)+2);
		MyPtr+=(get_descr_len(MyPtr)+2);
	  }
	}

	if (stream_anzahl >= 0) {
		switch (stream_type) {
		case STREAMTYPE_11172_VIDEO: 
		case STREAMTYPE_13818_VIDEO: 
			ProgrammNeu[ProgrammNummer].Video_pid=pid;
			break;
		case STREAMTYPE_11172_AUDIO: 
		case STREAMTYPE_13818_AUDIO: 
			ProgrammNeu[ProgrammNummer].Audio_pid=pid;
			break;
		
		case STREAMTYPE_13818_PES_PRIVATE:
			if (data_is_AC3) {
				ProgrammNeu[ProgrammNummer].AC3_pid=pid;
			} else if (data_is_teletext) {
				ProgrammNeu[ProgrammNummer].TeleText_pid=pid;
			}
			break;
		}

		if ( ProgrammNeu[ProgrammNummer].Filteranzahl < MAX_PID_IDS ) {
			ProgrammNeu[ProgrammNummer].Filters[ProgrammNeu[ProgrammNummer].Filteranzahl].FilterId=stream_type;
			ProgrammNeu[ProgrammNummer].Filters[ProgrammNeu[ProgrammNummer].Filteranzahl].PID=pid;
			strncpy(ProgrammNeu[ProgrammNummer].Filters[ProgrammNeu[ProgrammNummer].Filteranzahl].FilterName, filter_name, sizeof(ProgrammNeu[0].Filters[0].FilterName)-1);
			ProgrammNeu[ProgrammNummer].Filteranzahl++;
		}
	}
}

static unsigned long crctab[256] = { /* Generated with makecrc.c from gzip. */
    0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
    0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61, 0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
    0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
    0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
    0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039, 0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
    0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
    0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
    0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1, 0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
    0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
    0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
    0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde, 0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
    0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
    0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
    0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6, 0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
    0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
    0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
    0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637, 0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
    0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
    0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
    0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff, 0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
    0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
    0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
    0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7, 0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
    0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
    0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
    0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8, 0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
    0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
    0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
    0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0, 0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
    0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
    0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
    0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668, 0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4 };

unsigned long ccitt32_checkcrc(const unsigned char *icp, const unsigned int icnt)
{

#define M1 0xffffff
#define M2 0xffffff00
    register unsigned long crc = 0xffffffffL;
    register unsigned char *cp = (unsigned char*) icp;
    register int cnt = icnt;

    while(cnt--) crc=((crc<<8)&M2)^crctab[((crc>>24)&0xff)^*(cp++)];

    return(crc);
}


BOOL IsValidSection(unsigned char *Daten){
    if ( Daten[0] != 0x00 ) return(FALSE);
    if ( Daten[1] == TID_EIT_ACT || Daten[1] == TID_EIT_OTH) return(TRUE);
    if ( Daten[1] >= TID_EIT_SCH_FIRST && Daten[1] <= TID_EIT_SCH_LAST) return(TRUE);
    /* Was 0x4e, 0x4f, 0x50, 0x51, 0x54, 0x60, 0x61 */
	return(FALSE);
};

BOOL Get_Section_Length(unsigned char *Daten,int Offset,int Max, int *SectionLength){
	
    int	StepRate=0xb8;
	int i;
	i=StepRate;
	while ( Offset+i < Max ) {
		if ( IsValidSection(&Daten[i]) == FALSE ) i+=StepRate;
		else {
			*SectionLength=i;
			return(TRUE);
		};
		
	}
	return(FALSE);
};


void Irq_Work_EIT_Stream(unsigned char *Daten , int Laenge ){
	unsigned char *DataPtr;
    int Length=0;
	unsigned int *srcPtr;
	unsigned int *destPtr;
	
	int SectionLength;
	int i,j;
	
	if (( EIT_Buffer_Offset+Laenge ) > 8192 ) {
		EIT_Buffer_Offset=0;
		return;
	};
	
    memcpy(&EIT_Buffer[EIT_Buffer_Offset],&Daten[0],Laenge);
    
	EIT_Buffer[EIT_Buffer_Offset+Laenge]=0xff;
    EIT_Buffer[EIT_Buffer_Offset+Laenge+1]=0xff;
    EIT_Buffer[EIT_Buffer_Offset+Laenge+2]=0xff;
    EIT_Buffer[EIT_Buffer_Offset+Laenge+3]=0xff;
    EIT_Buffer[EIT_Buffer_Offset+Laenge+4]=0xff;
    EIT_Buffer[EIT_Buffer_Offset+Laenge+5]=0xff;
    EIT_Buffer[EIT_Buffer_Offset+Laenge+6]=0xff;
	
	
	Laenge=EIT_Buffer_Offset+Laenge;
	EIT_Buffer_Offset=0;
	
	DataPtr=&EIT_Buffer[0];
	Length=0;
	
	while ( Length < Laenge ) {
		
		if ( IsValidSection(&EIT_Buffer[Length]) == FALSE ) {
			j=Length/0xb8;
			i=(j+1)*0xb8;
            Length=i;
            continue;
		};
		
		if ( Get_Section_Length(&EIT_Buffer[Length],Length,Laenge,&SectionLength) == FALSE ) {
			if ( Length > 0 ) {
				i= ((Laenge-Length)/4+1);
				srcPtr =(unsigned int *)&EIT_Buffer[Length];
				destPtr=(unsigned int *)&EIT_Buffer[0];
				for ( j=0; j < i ; j++ ) *(destPtr++)=*(srcPtr++);
				EIT_Buffer_Offset=(Laenge-Length);
			} else {
				EIT_Buffer_Offset=Laenge;
			};
			return;
		}
		
		
		if (( EIT_Buffer[Length+1] == TID_EIT_ACT ) ||
			( EIT_Buffer[Length+1] == TID_EIT_OTH )) {
			j=Work_EIT_4F(&EIT_Buffer[Length+1],SectionLength);
		} else if (( EIT_Buffer[Length+1] >= TID_EIT_SCH_FIRST ) &&
			( EIT_Buffer[Length+1] <= TID_EIT_SCH_LAST )) {
			j=Work_EIT_50(&EIT_Buffer[Length+1],SectionLength);
		} else {
            j=1;

		}
        Length+=SectionLength;
	};
	
};


int Work_EIT_50(unsigned char *Daten , int Laenge ){
	struct FIX_EIT_t tmpEIT;
	
	struct eit_t {
		unsigned char	table_id;
		unsigned char BitField1;
		unsigned char  section_length_lo;
		unsigned char  service_id_hi;
		unsigned char  service_id_lo;
		unsigned char BitField2;
		unsigned char	section_number;
		unsigned char	last_section_number;
		unsigned char	transport_stream_id_hi;
		unsigned char	transport_stream_id_lo;
		unsigned char	original_network_id_hi;
		unsigned char	original_network_id_lo;
		unsigned char	segment_last_section_number;
		unsigned char	segment_last_table_id;
	} ;
	
#define EIT_SIZE 14
	
	struct eit_loop_struct {
		unsigned char 	event_id_hi;
		unsigned char 	event_id_lo;
		unsigned char 	start_time_1;
		unsigned char 	start_time_2;
		unsigned char 	start_time_3;
		unsigned char 	start_time_4;
		unsigned char 	start_time_5;
		
		unsigned char 	duration_1;
		unsigned char 	duration_2;
		unsigned char 	duration_3;
		unsigned char BitField1;
		//u_char	descriptors_loop_length_hi	: 4;
		//u_char	free_CA_mode			: 1;
		//u_char	running_status			: 3;
		unsigned char 	descriptors_loop_length_lo;
		
	};
#define EIT_LOOP_SIZE 12
	
	
	int sec_len,descrlen,work_len;
	struct eit_t *eit;
	struct eit_loop_struct *eit_loop;
	unsigned char *ptr;
	int FilterNummer=-1;
	int i;
	
	eit=(struct eit_t *)Daten;
	sec_len = ((eit->BitField1&0x0f)<<8)+eit->section_length_lo;
	if ( sec_len <= EIT_SIZE ) return(0);
	if ( sec_len > Laenge ) return(-1);
      if (ccitt32_checkcrc(Daten, sec_len+3) != 0) return(-1);
	ptr=Daten; 
	tmpEIT.ts_id = HILO (eit->transport_stream_id);
	tmpEIT.SID_Id = HILO (eit->service_id);
	
	ptr += EIT_SIZE ;
	work_len=EIT_SIZE;
	while ( work_len+4 < sec_len ) {
        eit_loop=(struct eit_loop_struct *)ptr;
		tmpEIT.EIT_Extra_4A_Laenge=0;
		tmpEIT.EIT_Extra_50_Laenge=0;
		tmpEIT.EIT_Extra_54_Laenge=0;
		tmpEIT.EIT_Extra_55_Laenge=0;
		tmpEIT.EIT_Extra_5F_Laenge=0;
		tmpEIT.EIT_Extra_81_Laenge=0;
		tmpEIT.EIT_Extra_F0_Laenge=0;
		tmpEIT.EIT_Header_Laenge=0;
		tmpEIT.EIT_Text_Laenge=0;
		tmpEIT.virtual_eventid=0xffff;
		tmpEIT.virtual_sid=0xffff;
		
        tmpEIT.Event_Id=HILO (eit_loop->event_id);
		
		tmpEIT.Date_Id=(eit_loop->start_time_1 << 8) + eit_loop->start_time_2;
		tmpEIT.Start_Zeit_h=((eit_loop->start_time_3>>4)*10)+(eit_loop->start_time_3&0x0f);
		tmpEIT.Start_Zeit_m=((eit_loop->start_time_4>>4)*10)+(eit_loop->start_time_4&0x0f);
		tmpEIT.Start_Zeit_s=((eit_loop->start_time_5>>4)*10)+(eit_loop->start_time_5&0x0f);
		tmpEIT.Dauer_h=((eit_loop->duration_1>>4)*10)+(eit_loop->duration_1&0x0f);
		tmpEIT.Dauer_m=((eit_loop->duration_2>>4)*10)+(eit_loop->duration_2&0x0f);
		tmpEIT.Dauer_s=((eit_loop->duration_3>>4)*10)+(eit_loop->duration_3&0x0f);
		tmpEIT.Running_Status = 0;
		tmpEIT.Running_Status=eit_loop->BitField1>>5;
		descrlen = ((eit_loop->BitField1&0x0f)<<8)+eit_loop->descriptors_loop_length_lo;
        ptr+=EIT_LOOP_SIZE;
		work_len+=EIT_LOOP_SIZE;
		while ( descrlen > 0 ) {
			i=parse_eit(ptr,&tmpEIT);
			descrlen-=i;
			work_len+=i;
			if (( i <= 0 ) || ( descrlen < 0 )) {
				return(-1);
			}
			ptr+=i;
		}
		Save_EIT(&tmpEIT,FALSE);
	};
	return(sec_len);
};


int Work_EIT_4F(unsigned char *Daten , int Laenge ){
	struct FIX_EIT_t tmpEIT;
	
	struct eit_t {
		unsigned char	table_id;
		unsigned char BitField1;
		unsigned char  section_length_lo;
		unsigned char  service_id_hi;
		unsigned char  service_id_lo;
		unsigned char BitField2;
		unsigned char	section_number;
		unsigned char	last_section_number;
		unsigned char	transport_stream_id_hi;
		unsigned char	transport_stream_id_lo;
		unsigned char	original_network_id_hi;
		unsigned char	original_network_id_lo;
		unsigned char	segment_last_section_number;
		unsigned char	segment_last_table_id;
	} ;
	
#define EIT_SIZE 14
	
	struct eit_loop_struct {
		unsigned char 	event_id_hi;
		unsigned char 	event_id_lo;
		unsigned char 	start_time_1;
		unsigned char 	start_time_2;
		unsigned char 	start_time_3;
		unsigned char 	start_time_4;
		unsigned char 	start_time_5;
		
		unsigned char 	duration_1;
		unsigned char 	duration_2;
		unsigned char 	duration_3;
		unsigned char BitField1;
		//u_char	descriptors_loop_length_hi	: 4;
		//u_char	free_CA_mode			: 1;
		//u_char	running_status			: 3;
		unsigned char 	descriptors_loop_length_lo;
		
	};
#define EIT_LOOP_SIZE 12
	
	
	int sec_len,descrlen,work_len;
	struct eit_t *eit;
	struct eit_loop_struct *eit_loop;
	unsigned char *ptr;
	int FilterNummer=-1;
	int i;
	
	
	eit=(struct eit_t *)Daten;
	sec_len = ((eit->BitField1&0x0f)<<8)+eit->section_length_lo;
	if ( sec_len <= EIT_SIZE ) return(0);
	if ( sec_len > Laenge ) return(-1);
      if (ccitt32_checkcrc(Daten, sec_len+3) != 0) return(-1);
	
	ptr=Daten; 
	tmpEIT.ts_id = HILO (eit->transport_stream_id);
	tmpEIT.SID_Id = HILO (eit->service_id);
	tmpEIT.Current_Next = (eit->BitField1&1);
	ptr += EIT_SIZE ;
	work_len=EIT_SIZE;
	
	while ( work_len + EIT_LOOP_SIZE < sec_len ) {
		eit_loop=(struct eit_loop_struct *)ptr;
		
		tmpEIT.EIT_Extra_4A_Laenge=0;
		tmpEIT.EIT_Extra_50_Laenge=0;
		tmpEIT.EIT_Extra_54_Laenge=0;
		tmpEIT.EIT_Extra_55_Laenge=0;
		tmpEIT.EIT_Extra_5F_Laenge=0;
		tmpEIT.EIT_Extra_81_Laenge=0;
		tmpEIT.EIT_Extra_F0_Laenge=0;
		tmpEIT.EIT_Header_Laenge=0;
		tmpEIT.EIT_Text_Laenge=0;
		tmpEIT.virtual_eventid=0xffff;
		tmpEIT.virtual_sid=0xffff;
		
		tmpEIT.Event_Id=HILO (eit_loop->event_id);
		
		tmpEIT.Date_Id=(eit_loop->start_time_1 << 8) + eit_loop->start_time_2;
		tmpEIT.Start_Zeit_h=((eit_loop->start_time_3>>4)*10)+(eit_loop->start_time_3&0x0f);
		tmpEIT.Start_Zeit_m=((eit_loop->start_time_4>>4)*10)+(eit_loop->start_time_4&0x0f);
		tmpEIT.Start_Zeit_s=((eit_loop->start_time_5>>4)*10)+(eit_loop->start_time_5&0x0f);
		tmpEIT.Dauer_h=((eit_loop->duration_1>>4)*10)+(eit_loop->duration_1&0x0f);
		tmpEIT.Dauer_m=((eit_loop->duration_2>>4)*10)+(eit_loop->duration_2&0x0f);
		tmpEIT.Dauer_s=((eit_loop->duration_3>>4)*10)+(eit_loop->duration_3&0x0f);
		tmpEIT.Running_Status=0;
		tmpEIT.Running_Status=(eit_loop->BitField1>>5);
		descrlen = ((eit_loop->BitField1&0x0f)<<8)+eit_loop->descriptors_loop_length_lo;
		ptr+=EIT_LOOP_SIZE;
		work_len+=EIT_LOOP_SIZE;
		
		while ( descrlen > 0 ) {
			i=parse_eit(ptr,&tmpEIT);
			descrlen-=i;
			if (( i <= 0 ) || ( descrlen < 0 )) {
				return(-1);
			}
			ptr+=i;
			work_len+=i;
		}
		Save_EIT(&tmpEIT,TRUE);
	};
	return(sec_len);
};



int Get_Virtual_Event(unsigned short EventId,unsigned short SID){
	int i=0;
	for ( i =0; i < EIT[EventId].Anzahl; i++ ) {
		if ( EIT[EventId].EIT_Infos[i].SID_Id == SID ) return(i);
	};
	return(-1);
};


int parse_eit( unsigned char *ptr,struct FIX_EIT_t *tmpEit ){
	
	struct eit_short_event_descriptor_struct {
		unsigned char 	descriptor_tag;
		unsigned char 	descriptor_length;
		unsigned char 	language_code_1;
		unsigned char 	language_code_2;
		unsigned char 	language_code_3;
	};
	
	struct eit_short_event_descriptor_struct *eit_decr;
	
	struct eit_long_event_descriptor_struct {
		unsigned char 	descriptor_tag;
		unsigned char 	descriptor_length;
		unsigned char   block_count;
		unsigned char 	language_code_1;
		unsigned char 	language_code_2;
		unsigned char 	language_code_3;
		unsigned char   language_eot;
	};
	
	
	struct eit_long_event_descriptor_struct *eit_decr_long;
	
	struct eit_extra_event_descriptor_struct {
		unsigned char 	descriptor_tag;
		unsigned char 	descriptor_length;
		unsigned char   descriptor_Code;
		unsigned char   language_count;
		unsigned char   descriptor_Value;
		unsigned char 	language_code_1;
		unsigned char 	language_code_2;
		unsigned char 	language_code_3;
	};
	
	struct eit_extra_event_descriptor_struct *eit_decr_extra;
	
	
	struct eit_unknown_event_descriptor_struct {
		unsigned char 	descriptor_tag;
		unsigned char 	descriptor_length;
	};
	
	struct eit_unknown_event_descriptor_struct *eit_decr_unknown;
	
	
	unsigned char *posptr;
	int k;
	unsigned int RetCount;
	
	posptr=ptr;
	
	switch(*ptr) {
		
	case DESCR_SHORT_EVENT :  
		eit_decr=(struct eit_short_event_descriptor_struct *)ptr;
		posptr+=5;
		RetCount=eit_decr->descriptor_length+2; 
		if ( *posptr > 256 ) return(-1);
		memcpy(&tmpEit->HeaderLang[0],(ptr+2),3);
		memcpy(&tmpEit->Header[0],(posptr+1),*posptr);
		tmpEit->Header[*posptr]=0x00;
		tmpEit->EIT_Header_Laenge=*posptr;
		if ( tmpEit->EIT_Header_Laenge+6 < eit_decr->descriptor_length ) {
			tmpEit->Header[tmpEit->EIT_Header_Laenge]=0x0a;
			tmpEit->EIT_Header_Laenge++;
			posptr+=tmpEit->EIT_Header_Laenge;
			memcpy(&tmpEit->Header[tmpEit->EIT_Header_Laenge],(posptr+1),*posptr);
			tmpEit->EIT_Header_Laenge+=*posptr;
		}
		
		return(RetCount);

	case DESCR_EXTENDED_EVENT:
		k=0;
		RetCount=0;
		while ( *posptr == 0x4e ) {
			eit_decr_long=(struct eit_long_event_descriptor_struct *)posptr;
			RetCount+=(eit_decr_long->descriptor_length+2);
			if ( k+*(posptr+7) > 2048 )  return(-1);
			memcpy(&tmpEit->TextLang[0],(posptr+3),3);
			memcpy(&tmpEit->Text[k],(posptr+8),*(posptr+7));
			k+=*(posptr+7);
			posptr+=(eit_decr_long->descriptor_length+2);
		};
		tmpEit->EIT_Text_Laenge=k;
		return(RetCount);
		
		
	case DESCR_LINKAGE:  
		eit_decr_unknown=(struct eit_unknown_event_descriptor_struct *)ptr;
		RetCount=eit_decr_unknown->descriptor_length+2;
		if ( RetCount > 1024 ) return(-1);
		memcpy(&tmpEit->EIT_Extra_4A[tmpEit->EIT_Extra_4A_Laenge],ptr,RetCount);
		tmpEit->EIT_Extra_4A_Laenge+=RetCount;
		return(RetCount);
		
	case DESCR_TIME_SHIFTED_EVENT:
		eit_decr_unknown=(struct eit_unknown_event_descriptor_struct *)ptr;
		if ( eit_decr_unknown->descriptor_length == 4 ) {
			tmpEit->virtual_sid=(*(ptr+2)<<8)+*(ptr+3);
			tmpEit->virtual_eventid=(*(ptr+4)<<8)+*(ptr+5);
		} else {
			tmpEit->virtual_sid=0xffff;
			tmpEit->virtual_eventid=0xffff;
		};
		RetCount=eit_decr_unknown->descriptor_length+2;
		return(RetCount);
						  
	case DESCR_COMPONENT:
		eit_decr_extra=(struct eit_extra_event_descriptor_struct *)ptr;
		RetCount=eit_decr_extra->descriptor_length+2; 
		if ( RetCount > 1024 ) return(-1);
		memcpy(&tmpEit->EIT_Extra_50[tmpEit->EIT_Extra_50_Laenge],ptr,RetCount);
		tmpEit->EIT_Extra_50_Laenge+=RetCount;
		return(RetCount);

	case DESCR_CONTENT :  
		eit_decr_unknown=(struct eit_unknown_event_descriptor_struct *)ptr;
		RetCount=eit_decr_unknown->descriptor_length+2;
		if ( RetCount > 1024 ) return(-1);
		memcpy(&tmpEit->EIT_Extra_54[tmpEit->EIT_Extra_54_Laenge],ptr,RetCount);
		tmpEit->EIT_Extra_54_Laenge+=RetCount;
		return(RetCount);
		
	case DESCR_PARENTAL_RATING :  
		eit_decr_unknown=(struct eit_unknown_event_descriptor_struct *)ptr;
		RetCount=eit_decr_unknown->descriptor_length+2;
		memcpy(&tmpEit->EIT_Extra_55[tmpEit->EIT_Extra_55_Laenge],ptr,RetCount);
		if ( RetCount > 1024 ) return(-1);
		tmpEit->EIT_Extra_55_Laenge+=RetCount;
		return(RetCount);

	case DESCR_PRIV_DATA_SPEC :  
		eit_decr_unknown=(struct eit_unknown_event_descriptor_struct *)ptr;
		RetCount=eit_decr_unknown->descriptor_length+2;
		if ( RetCount > 1024 ) return(-1);
		memcpy(&tmpEit->EIT_Extra_5F[tmpEit->EIT_Extra_5F_Laenge],ptr,RetCount);
		tmpEit->EIT_Extra_5F_Laenge+=RetCount;
		return(RetCount);

	case 0xf0 :  
		eit_decr_unknown=(struct eit_unknown_event_descriptor_struct *)ptr;
		RetCount=eit_decr_unknown->descriptor_length+2;
		if ( RetCount > 1024 ) return(-1);
		memcpy(&tmpEit->EIT_Extra_F0[tmpEit->EIT_Extra_F0_Laenge],ptr,RetCount);
		tmpEit->EIT_Extra_F0_Laenge+=RetCount;
		return(RetCount);
		
	case 0x81 :  
		eit_decr_unknown=(struct eit_unknown_event_descriptor_struct *)ptr;
		RetCount=eit_decr_unknown->descriptor_length+2;
		if ( RetCount > 1024 ) return(-1);
		memcpy(&tmpEit->EIT_Extra_81[tmpEit->EIT_Extra_81_Laenge],ptr,RetCount);
		tmpEit->EIT_Extra_81_Laenge+=RetCount;
		return(RetCount);

		//Neue Items
	case DESCR_ISO_639_LANGUAGE :
		if ( *(ptr+1) == 0x04 ) return(6);
		if ( *(ptr+1) == 0x08 ) return(10);
	case 0x29 :
	case 0x30 :
	case 0x31 :
	case 0x34 :
	case 0x35 :
	case 0x36 :
	case 0x37 :
	case 0x38 :
	case 0x39 :
	case 0x8a :
		if ( *(ptr+1) == 0x55 ) return(6);
		else return(-1);
		
		
	case DESCR_PDC :  
	case 0x80 :  
	case 0x8d :  
	case 0x8e :  
	case 0xb2 :  
	case 0xbb :  
		eit_decr_unknown=(struct eit_unknown_event_descriptor_struct *)ptr;
		return(eit_decr_unknown->descriptor_length+2);
		
	default:     
		return(-1);
		
};

};



int Get_Sender(unsigned short SID_Id,unsigned short ts_id,unsigned char *Name){
	int i;
	if ( Name != NULL ) *Name=0x00;
	
	for ( i = 0 ; i < MAXPROGS; i++ ) {
		if (( Programm[i].SID_pid == SID_Id ) && ( Programm[i].tp_id == ts_id )) {
			if ( Name != NULL ) strcpy(Name,Programm[i].Name);
			return(i);
		}
	};
	return(-1);
};


void Relink_Current_Eit(unsigned short SID,unsigned short ts_id,unsigned short To_SID,unsigned short To_ts_id){
	int Index,Offset;
	
	if ( Find_Current_EIT(SID,ts_id,&Index,&Offset) == FALSE ) return;
	EIT[Index].EIT_Infos[Offset].SID_Id=To_SID;
	EIT[Index].EIT_Infos[Offset].ts_id=To_ts_id;
};



int Save_EIT(struct FIX_EIT_t *tmpEit, BOOL Update_Current_Next){ 
	int index;
	int count;
    int i;
	BOOL Is_New;
	struct T_EIT_Infos *Save;
	index=tmpEit->Event_Id;
    Is_New=FALSE;
	if (( index < 0 ) || ( index > 0xffff )) return(-1);
	if (( tmpEit->EIT_Header_Laenge	< 0 ) || ( tmpEit->EIT_Header_Laenge > 512 )) return(-1);
	if (( tmpEit->EIT_Text_Laenge	< 0 ) || ( tmpEit->EIT_Text_Laenge > 4096 )) return(-1);
	if (( tmpEit->EIT_Extra_4A_Laenge	< 0 ) || ( tmpEit->EIT_Extra_4A_Laenge > 1024 )) return(-1);
	if (( tmpEit->EIT_Extra_50_Laenge	< 0 ) || ( tmpEit->EIT_Extra_50_Laenge > 1024 )) return(-1);
	if (( tmpEit->EIT_Extra_54_Laenge	< 0 ) || ( tmpEit->EIT_Extra_54_Laenge > 1024 )) return(-1);
	if (( tmpEit->EIT_Extra_55_Laenge	< 0 ) || ( tmpEit->EIT_Extra_55_Laenge > 1024 )) return(-1);
	if (( tmpEit->EIT_Extra_5F_Laenge	< 0 ) || ( tmpEit->EIT_Extra_5F_Laenge > 1024 )) return(-1);
	if (( tmpEit->EIT_Extra_F0_Laenge	< 0 ) || ( tmpEit->EIT_Extra_F0_Laenge > 1024 )) return(-1);
	if (( tmpEit->EIT_Extra_81_Laenge	< 0 ) || ( tmpEit->EIT_Extra_81_Laenge > 1024 )) return(-1);
	
	
	
	if ( EIT[index].Anzahl == 0x00 ) {
		count=0;
        Is_New=TRUE;
		EIT[index].EIT_Infos=HeapAlloc(MultiDec_Heap,HEAP_ZERO_MEMORY , sizeof(struct T_EIT_Infos));
		if ( EIT[index].EIT_Infos == NULL ) return(-1);
		EIT[index].Anzahl=0x01;
		EIT_Loaded++;
		
	} else {
		i=0;
        while (( i < (int) EIT[index].Anzahl ) && ( EIT[index].EIT_Infos[i].SID_Id != tmpEit->SID_Id )) i++;
        if ( i == EIT[index].Anzahl ) {
			Is_New=TRUE;
			count= EIT[index].Anzahl;
			Save= EIT[index].EIT_Infos;
			EIT[index].EIT_Infos=HeapReAlloc(MultiDec_Heap,HEAP_ZERO_MEMORY ,Save, (EIT[index].Anzahl+1)*sizeof(struct T_EIT_Infos));
			if ( EIT[index].EIT_Infos == NULL ) {
				EIT[index].EIT_Infos=Save;
				return(-1);
												};
			EIT_Loaded++;
			EIT[index].Anzahl++;
		} else count=i;
	};
	
	
	
	EIT[index].EIT_Infos[count].SID_Id=tmpEit->SID_Id;
	EIT[index].EIT_Infos[count].ts_id=tmpEit->ts_id;
    EIT[index].EIT_Infos[count].Event_Id=tmpEit->Event_Id;
    EIT[index].EIT_Infos[count].Date_Id=tmpEit->Date_Id;
	EIT[index].EIT_Infos[count].Start_Zeit_h=tmpEit->Start_Zeit_h;
	EIT[index].EIT_Infos[count].Start_Zeit_m=tmpEit->Start_Zeit_m;
	EIT[index].EIT_Infos[count].Start_Zeit_s=tmpEit->Start_Zeit_s;
	EIT[index].EIT_Infos[count].Dauer_h=tmpEit->Dauer_h;
	EIT[index].EIT_Infos[count].Dauer_m=tmpEit->Dauer_m;
	EIT[index].EIT_Infos[count].Dauer_s=tmpEit->Dauer_s;
	EIT[index].EIT_Infos[count].virtual_eventid=tmpEit->virtual_eventid;
	EIT[index].EIT_Infos[count].virtual_sid=tmpEit->virtual_sid;
	
	
	
	
	if ( tmpEit->EIT_Header_Laenge	> 0 ) {
		memcpy(&EIT[index].EIT_Infos[count].HeaderLang[0],tmpEit->HeaderLang,3);
		if ( EIT[index].EIT_Infos[count].HeaderSize < tmpEit->EIT_Header_Laenge ) {
			if ( EIT[index].EIT_Infos[count].Header != NULL ) {
				if ( HeapFree(MultiDec_Heap,0,EIT[index].EIT_Infos[count].Header ) == FALSE ) return(-1);
				EIT[index].EIT_Infos[count].Header=NULL;
			};
			EIT[index].EIT_Infos[count].Header=HeapAlloc(MultiDec_Heap,HEAP_ZERO_MEMORY , tmpEit->EIT_Header_Laenge+2);
			if ( EIT[index].EIT_Infos[count].Header == NULL ) return(-1);
		};
		memcpy(EIT[index].EIT_Infos[count].Header,&tmpEit->Header[0],tmpEit->EIT_Header_Laenge);
		EIT[index].EIT_Infos[count].HeaderSize=tmpEit->EIT_Header_Laenge;
	}
	
	if ( tmpEit->EIT_Text_Laenge	> 0 ) {
		memcpy(&EIT[index].EIT_Infos[count].TextLang[0],tmpEit->TextLang,3);
		if ( EIT[index].EIT_Infos[count].TextSize < tmpEit->EIT_Text_Laenge ) {
			if ( EIT[index].EIT_Infos[count].Text != NULL ) {
				if ( HeapFree(MultiDec_Heap,0,EIT[index].EIT_Infos[count].Text ) == FALSE ) return(-1);
				EIT[index].EIT_Infos[count].Text=NULL;
			};
			EIT[index].EIT_Infos[count].Text=HeapAlloc(MultiDec_Heap,HEAP_ZERO_MEMORY , tmpEit->EIT_Text_Laenge+2);
			if ( EIT[index].EIT_Infos[count].Text == NULL ) return(-1);
			
		};
		memcpy(EIT[index].EIT_Infos[count].Text,&tmpEit->Text[0],tmpEit->EIT_Text_Laenge);
		EIT[index].EIT_Infos[count].TextSize=tmpEit->EIT_Text_Laenge;
	}
	
	if ( tmpEit->EIT_Extra_50_Laenge > 0 ) {
		if ( EIT[index].EIT_Infos[count].Extra_50Size < tmpEit->EIT_Extra_50_Laenge ) {
			if ( EIT[index].EIT_Infos[count].Extra_50 != NULL ) {
				if ( HeapFree(MultiDec_Heap,0,EIT[index].EIT_Infos[count].Extra_50 ) == FALSE ) return(-1);
				EIT[index].EIT_Infos[count].Extra_50=NULL;
			};
			EIT[index].EIT_Infos[count].Extra_50=HeapAlloc(MultiDec_Heap,HEAP_ZERO_MEMORY , tmpEit->EIT_Extra_50_Laenge+2);
			if ( EIT[index].EIT_Infos[count].Extra_50 == NULL ) return(-1);
		};
		memcpy(EIT[index].EIT_Infos[count].Extra_50,&tmpEit->EIT_Extra_50[0],tmpEit->EIT_Extra_50_Laenge);
		EIT[index].EIT_Infos[count].Extra_50Size=tmpEit->EIT_Extra_50_Laenge;
	}
	
	if ( tmpEit->EIT_Extra_54_Laenge > 0 ) {
		if ( EIT[index].EIT_Infos[count].Extra_54Size < tmpEit->EIT_Extra_54_Laenge ) {
			if ( EIT[index].EIT_Infos[count].Extra_54 != NULL ) {
				if ( HeapFree(MultiDec_Heap,0,EIT[index].EIT_Infos[count].Extra_54 ) == FALSE ) return(-1);
				EIT[index].EIT_Infos[count].Extra_54=NULL;
			};
			EIT[index].EIT_Infos[count].Extra_54=HeapAlloc(MultiDec_Heap,HEAP_ZERO_MEMORY , tmpEit->EIT_Extra_54_Laenge+2);
			if ( EIT[index].EIT_Infos[count].Extra_54 == NULL ) return(-1);
			
		};
		memcpy(EIT[index].EIT_Infos[count].Extra_54,&tmpEit->EIT_Extra_54[0],tmpEit->EIT_Extra_54_Laenge);
		EIT[index].EIT_Infos[count].Extra_54Size=tmpEit->EIT_Extra_54_Laenge;
	}
	
	if ( tmpEit->EIT_Extra_4A_Laenge > 0 ) {
		if ( EIT[index].EIT_Infos[count].Extra_4ASize < tmpEit->EIT_Extra_4A_Laenge ) {
			if ( EIT[index].EIT_Infos[count].Extra_4A != NULL ) {
				if ( HeapFree(MultiDec_Heap,0,EIT[index].EIT_Infos[count].Extra_4A ) == FALSE ) return(-1);
				EIT[index].EIT_Infos[count].Extra_4A=NULL;
			};
			EIT[index].EIT_Infos[count].Extra_4A=HeapAlloc(MultiDec_Heap,HEAP_ZERO_MEMORY , tmpEit->EIT_Extra_4A_Laenge+2);
			if ( EIT[index].EIT_Infos[count].Extra_4A == NULL ) return(-1);
		};
		memcpy(EIT[index].EIT_Infos[count].Extra_4A,&tmpEit->EIT_Extra_4A[0],tmpEit->EIT_Extra_4A_Laenge);
		EIT[index].EIT_Infos[count].Extra_4ASize=tmpEit->EIT_Extra_4A_Laenge;
	}
	
	
	if ( tmpEit->EIT_Extra_55_Laenge > 0 ) {
		if ( EIT[index].EIT_Infos[count].Extra_55Size < tmpEit->EIT_Extra_55_Laenge ) {
			if ( EIT[index].EIT_Infos[count].Extra_55 != NULL ) {
				if ( HeapFree(MultiDec_Heap,0,EIT[index].EIT_Infos[count].Extra_55 ) == FALSE ) return(-1);
				EIT[index].EIT_Infos[count].Extra_55=NULL;
			};
			EIT[index].EIT_Infos[count].Extra_55=HeapAlloc(MultiDec_Heap,HEAP_ZERO_MEMORY , tmpEit->EIT_Extra_55_Laenge+2);
			if ( EIT[index].EIT_Infos[count].Extra_55 == NULL ) return(-1);
			
		};
		memcpy(EIT[index].EIT_Infos[count].Extra_55,&tmpEit->EIT_Extra_55[0],tmpEit->EIT_Extra_55_Laenge);
		EIT[index].EIT_Infos[count].Extra_55Size=tmpEit->EIT_Extra_55_Laenge;
	}
	
	if ( tmpEit->EIT_Extra_5F_Laenge > 0 ) {
		if ( EIT[index].EIT_Infos[count].Extra_5FSize < tmpEit->EIT_Extra_5F_Laenge ) {
			if ( EIT[index].EIT_Infos[count].Extra_5F != NULL ) {
				if ( HeapFree(MultiDec_Heap,0,EIT[index].EIT_Infos[count].Extra_5F ) == FALSE ) return(-1);
				EIT[index].EIT_Infos[count].Extra_5F=NULL;
			};
			EIT[index].EIT_Infos[count].Extra_5F=HeapAlloc(MultiDec_Heap,HEAP_ZERO_MEMORY , tmpEit->EIT_Extra_5F_Laenge+2);
			if ( EIT[index].EIT_Infos[count].Extra_5F == NULL ) return(-1);
			
		};
		memcpy(EIT[index].EIT_Infos[count].Extra_5F,&tmpEit->EIT_Extra_5F[0],tmpEit->EIT_Extra_5F_Laenge);
		EIT[index].EIT_Infos[count].Extra_5FSize=tmpEit->EIT_Extra_5F_Laenge;
	}
	
	if ( tmpEit->EIT_Extra_F0_Laenge > 0 ) {
		if ( EIT[index].EIT_Infos[count].Extra_F0Size < tmpEit->EIT_Extra_F0_Laenge ) {
			if ( EIT[index].EIT_Infos[count].Extra_F0 != NULL ) {
				if ( HeapFree(MultiDec_Heap,0,EIT[index].EIT_Infos[count].Extra_F0 ) == FALSE ) return(-1);
				EIT[index].EIT_Infos[count].Extra_F0=NULL;
			};
			EIT[index].EIT_Infos[count].Extra_F0=HeapAlloc(MultiDec_Heap,HEAP_ZERO_MEMORY , tmpEit->EIT_Extra_F0_Laenge+2);
			if ( EIT[index].EIT_Infos[count].Extra_F0 == NULL ) return(-1);
			
		};
		memcpy(EIT[index].EIT_Infos[count].Extra_F0,&tmpEit->EIT_Extra_F0[0],tmpEit->EIT_Extra_F0_Laenge);
		EIT[index].EIT_Infos[count].Extra_F0Size=tmpEit->EIT_Extra_F0_Laenge;
	}
	
	if ( tmpEit->EIT_Extra_81_Laenge > 0 ) {
		if ( EIT[index].EIT_Infos[count].Extra_81Size < tmpEit->EIT_Extra_81_Laenge ) {
			if ( EIT[index].EIT_Infos[count].Extra_81 != NULL ) {
				if ( HeapFree(MultiDec_Heap,0,EIT[index].EIT_Infos[count].Extra_81 ) == FALSE ) return(-1);
				EIT[index].EIT_Infos[count].Extra_81=NULL;
			};
			EIT[index].EIT_Infos[count].Extra_81=HeapAlloc(MultiDec_Heap,HEAP_ZERO_MEMORY , tmpEit->EIT_Extra_81_Laenge+2);
			if ( EIT[index].EIT_Infos[count].Extra_81 == NULL ) return(-1);
		};
		memcpy(EIT[index].EIT_Infos[count].Extra_81,&tmpEit->EIT_Extra_81[0],tmpEit->EIT_Extra_81_Laenge);
		EIT[index].EIT_Infos[count].Extra_81Size=tmpEit->EIT_Extra_81_Laenge;
	}
	
	
	if ( Update_Current_Next == FALSE ) {
		
		if (  EIT[index].EIT_Infos[count].UpdateCount < 2 ) EIT[index].EIT_Infos[count].UpdateCount++;
		if (( Is_New == TRUE ) || ( EIT[index].EIT_Infos[count].UpdateCount> 2 )) {
			EIT[index].EIT_Infos[count].Current_Next=0;
			EIT[index].EIT_Infos[count].Running_Status=0;
								};
	} else if ( Update_Current_Next == TRUE ) {
		
		EIT[index].EIT_Infos[count].UpdateCount=0x00;
		if ( tmpEit->Running_Status == 4 )						// jetzt
		{
			if (( tmpEit->virtual_eventid != 0xffff ) &&
				( tmpEit->virtual_sid != 0xffff ) && 
				( Programm[CurrentProgramm].SID_pid == tmpEit->SID_Id) && 
				( Programm[CurrentProgramm].tp_id == tmpEit->ts_id ) && 
				( (Programm[CurrentProgramm].Merker&1) != 1	)) {
				
				if ( Get_MultiLink_Count(tmpEit->virtual_eventid,tmpEit->virtual_sid ) > 1 ) {
					Programm[CurrentProgramm].Merker+=1;
					EnableMenuItem(GetMenu(hWnd),IDM_TIMELISTE,MF_ENABLED);
				}
			};
			
			if  (( Programm[CurrentProgramm].SID_pid == tmpEit->SID_Id) && 
				( Programm[CurrentProgramm].tp_id == tmpEit->ts_id ) && 
				( (Programm[CurrentProgramm].Merker&2) != 2	)) {
				
				if ( Get_MultiView_Count((unsigned short)index,(unsigned short)count ) > 1 ) {
					Programm[CurrentProgramm].Merker+=2;
					EnableMenuItem(GetMenu(hWnd),IDM_VIEWLISTE,MF_ENABLED);
				}
			}
			
			EIT[index].EIT_Infos[count].Running_Status=0x02;
		} else													// alles andere
		{
			EIT[index].EIT_Infos[count].Running_Status=0x01;
								}
	};
	
	
	return(0);
};


BOOL Find_Multi_Link(unsigned short v_event,unsigned short v_sid, int *index, int *offset ){
	int i,j;
	for ( i=*index ; i < Current_Eit_Index_Size; i++ ) {
		for ( j=*offset ; j < EIT[i].Anzahl; j++ ) {
			if (( v_event == EIT[i].EIT_Infos[j].virtual_eventid ) &&
				( v_sid == EIT[i].EIT_Infos[j].virtual_sid ) && 
				( EIT[i].EIT_Infos[j].Running_Status == 2 )) {
				*index=i;
				*offset=j;
                return(TRUE);
			}
			*offset=0;
			
		}
	};
	*index=i;
	*offset=j;
	return(FALSE);
}

int Get_MultiLink_Count(unsigned short v_event,unsigned short v_sid){
	int StartIndex, StartOffset;
	int LinkAnzahl=0;	
	StartIndex=0;StartOffset=0;
	while ( StartIndex < Current_Eit_Index_Size ) {
		if ( Find_Multi_Link(v_event,v_sid, &StartIndex, &StartOffset ) == TRUE ) {
			LinkAnzahl++;
			StartOffset++;
		};
	};
	return(LinkAnzahl);
};


int Get_MultiView_Count(unsigned short Index,unsigned short Offset){
	int LinkAnzahl=0;	
	int laenge = 0;
	
	struct tmv {
		unsigned char TableId;
		unsigned char size;
		unsigned char 	transport_stream_id_hi;
		unsigned char 	transport_stream_id_lo;
		unsigned short Buffer;
		unsigned char 	Sid_id_hi;
		unsigned char 	Sid_id_lo;
		unsigned char Unknown;
		unsigned char Name;
	};
	
	struct tmv *mv;
	
	
	if ( EIT[Index].EIT_Infos[Offset].Extra_4ASize == 0 )  return(0);
	
	laenge=0;
	
	while ( laenge < EIT[Index].EIT_Infos[Offset].Extra_4ASize ) {
		mv=(struct tmv *)&EIT[Index].EIT_Infos[Offset].Extra_4A[laenge];
		laenge+=mv->size+2;
		LinkAnzahl++;
	};
	return(LinkAnzahl);
};





BOOL Is_Time_Link(unsigned short SID, unsigned short ts_id,BOOL Current){
	int Index,Offset;
	int i;
	int StartIndex, StartOffset;
	int LinkAnzahl=0;	
	
	unsigned char h,m;
	
	int CP;
	
	unsigned short v_event,v_sid;  
	
	
	for ( i=0; i < 16; i++ ) MultiLinkZeit[i].fill=0x00;
	
	if ( Current == TRUE ) {
		if ( Find_Current_EIT(SID,ts_id,&Index,&Offset) == FALSE ) return(FALSE);
	} else {
		Index=SID;
		Offset=ts_id;
	}
	
	v_event=EIT[Index].EIT_Infos[Offset].virtual_eventid ;
	v_sid=EIT[Index].EIT_Infos[Offset].virtual_sid ;
	
	
	if (( v_event == 0xffff ) ||
		( v_sid == 0xffff )) return(FALSE);
	
	for ( i=0; i < 16; i++ ) MultiLinkZeit[i].fill=0x00;
	StartIndex=0;StartOffset=0;
	while ( StartIndex < Current_Eit_Index_Size ) {
		if ( Find_Multi_Link(v_event,v_sid, &StartIndex, &StartOffset ) == TRUE ) {
			MultiLinkZeit[LinkAnzahl].fill=0x01;
			
			if (( StartIndex==Index) && (StartOffset==Offset)) MultiLinkZeit[LinkAnzahl].fill=0x02;
			MultiLinkZeit[LinkAnzahl].ts_id=EIT[StartIndex].EIT_Infos[StartOffset].ts_id;
			MultiLinkZeit[LinkAnzahl].SID=EIT[StartIndex].EIT_Infos[StartOffset].SID_Id;
			
			h=EIT[StartIndex].EIT_Infos[StartOffset].Start_Zeit_h+EIT[StartIndex].EIT_Infos[StartOffset].Dauer_h;
			m=EIT[StartIndex].EIT_Infos[StartOffset].Start_Zeit_m+EIT[StartIndex].EIT_Infos[StartOffset].Dauer_m;
			
			if ( m > 59 ) {
				h++;
				m-=60;
			};
			if ( h > 23 ) h=0;
			sprintf(MultiLinkZeit[LinkAnzahl].ExtraText,"%02d:%02d - %02d:%02d",RealTime(EIT[StartIndex].EIT_Infos[StartOffset].Start_Zeit_h),EIT[StartIndex].EIT_Infos[StartOffset].Start_Zeit_m,RealTime(h),m);
			CP=Find_Programm(EIT[StartIndex].EIT_Infos[StartOffset].ts_id,EIT[StartIndex].EIT_Infos[StartOffset].SID_Id);
			if ( CP >= 0 ) strncpy(MultiLinkZeit[LinkAnzahl].Name,Programm[CP].Name,30);
			else sprintf(MultiLinkZeit[LinkAnzahl].Name,"TP:% 5d SID:% 5d ",EIT[StartIndex].EIT_Infos[StartOffset].ts_id,EIT[StartIndex].EIT_Infos[StartOffset].SID_Id);
			
			LinkAnzahl++;
			StartOffset++;
		};
		
	}
	
	if ( LinkAnzahl > 1 ) return(TRUE);
	return(FALSE);
	
};



BOOL Is_View_Link(unsigned short SID, unsigned short ts_id,BOOL Current){
	int Index,Offset;
	int i,laenge;
	int LinkAnzahl=0;	
	int CP;
	
	struct tmv {
		unsigned char TableId;
		unsigned char size;
		unsigned char 	transport_stream_id_hi;
		unsigned char 	transport_stream_id_lo;
		unsigned short Buffer;
		unsigned char 	Sid_id_hi;
		unsigned char 	Sid_id_lo;
		unsigned char Unknown;
		unsigned char Name;
	};
	
	struct tmv *mv;
	
	
	
	for ( i=0; i < 16; i++ ) MultiLinkView[i].fill=0x00;
	
	if ( Current == TRUE ) {
		if ( Find_Current_EIT(SID,ts_id,&Index,&Offset) == FALSE ) return(FALSE);
	} else {
		Index=SID;
		Offset=ts_id;
	}
	
	if ( EIT[Index].EIT_Infos[Offset].Extra_4ASize == 0 )  return(FALSE);
	
	laenge=0;
	
	while ( laenge < EIT[Index].EIT_Infos[Offset].Extra_4ASize ) {
		mv=(struct tmv *)&EIT[Index].EIT_Infos[Offset].Extra_4A[laenge];
		laenge+=mv->size+2;
		MultiLinkView[LinkAnzahl].fill=0x01;
		MultiLinkView[LinkAnzahl].ts_id=HILO(mv->transport_stream_id);
		MultiLinkView[LinkAnzahl].SID=HILO(mv->Sid_id);
		if ( Current == TRUE ) {
			if (( MultiLinkView[LinkAnzahl].SID ==SID ) && (MultiLinkView[LinkAnzahl].ts_id==ts_id )) MultiLinkZeit[LinkAnzahl].fill=0x02;
		};
		memcpy(MultiLinkView[LinkAnzahl].ExtraText,&mv->Name,mv->size-7);
		MultiLinkView[LinkAnzahl].ExtraText[mv->size-7]=0x00;
		CP=Find_Programm(MultiLinkView[LinkAnzahl].ts_id,MultiLinkView[LinkAnzahl].SID);
		if ( CP >= 0 ) strncpy(MultiLinkView[LinkAnzahl].Name,Programm[CP].Name,30);
		else sprintf(MultiLinkView[LinkAnzahl].Name,"TP:% 5d SID:% 5d ",MultiLinkView[LinkAnzahl].ts_id,MultiLinkView[LinkAnzahl].SID);
		LinkAnzahl++;
	};
	
	
	if ( LinkAnzahl > 1 ) return(TRUE);
	return(FALSE);
	
};



BOOL Is_Pid_Param(unsigned short SID, unsigned short ts_id,BOOL Current){
	int Index,Offset;
	int i,laenge;
	int LinkAnzahl=0;	
	
	int Virtual_Event_Id,Virtual_Offset_Id;
	
	struct tpf {
		unsigned char TableId;
		unsigned char size;
		unsigned char 	Key_01;
		unsigned char 	Key_02;
		unsigned char  Key_03;
		unsigned char 	Language[3];
		unsigned char Name;
	};
	
	struct tpf *pf;
	
	
	
	for ( i=0; i < 16; i++ ) PidParam[i].fill=0x00;
	
	if ( Current == TRUE ) {
		if ( Find_Current_EIT(SID,ts_id,&Index,&Offset) == FALSE ) return(FALSE);
	} else {
		Index=SID;
		Offset=ts_id;
	}
	
	
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
	
	if ( EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Extra_50Size ==	0 ) return(FALSE);
	
	laenge=0;
	
	while ( laenge < EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Extra_50Size ) {
		pf=(struct tpf *)&EIT[Virtual_Event_Id].EIT_Infos[Virtual_Offset_Id].Extra_50[laenge];
		laenge+=pf->size+2;
		PidParam[LinkAnzahl].fill=0x01;
		memcpy(&PidParam[LinkAnzahl].Key_01,&pf->Key_01,6);
		PidParam[LinkAnzahl].Language[3]=0x00;
		memcpy(PidParam[LinkAnzahl].Name,&pf->Name,pf->size-6);
		PidParam[LinkAnzahl].Name[pf->size-6]=0x00;
		LinkAnzahl++;
	};
	
	
	if ( LinkAnzahl >= 1 ) return(TRUE);
	return(FALSE);
	
};



BOOL Get_SDT( void ){
#define SDT_LEN 11
#define SDT_DESCR_LEN 5
#define DESCR_GEN_LEN 2
	
	struct sdt_descr_t {
		unsigned char service_id_hi;
		u_char	service_id_lo;
		u_char  BitField1;
		// u_char	eit_present_following_flag	: 1;
		//u_char	eit_schedule_flag		: 1;
		//u_char					: 6;
		u_char  BitField2;
		// u_char	descriptors_loop_length_hi	: 4;
		// u_char	free_ca_mode			: 1;
		// u_char	running_status			: 3;
		u_char	descriptors_loop_length_lo;
	};
	
	
	struct sdt_t {
		u_char	table_id;
		u_char  BitField1;
		//u_char	section_length_hi		: 4;
		//u_char					: 3;
		//u_char	section_syntax_indicator	: 1;
		u_char	section_length_lo;
		u_char	transport_stream_id_hi;
		u_char	transport_stream_id_lo;
		u_char  BitField2;
		// u_char	current_next_indicator		: 1;
		// u_char	version_number			: 5;
		// u_char					: 2;
		u_char	section_number;
		u_char	last_section_number;
		u_char	original_network_id_hi;
		u_char	original_network_id_lo;
		u_char	buffer;
	};
	
	struct sdt_t *sdt;
	struct sdt_descr_t *sdt_descr;
	
	int j;
	unsigned short SID_Id;
	int sec_len;
	unsigned char *ptr;
	unsigned char *buf;
	int loop_len,descr_len;
	int Loop=0;
	int SDTFilter;
	char Text[256];
	int ngets = 0;
#define MAX_SDT_GETS 100
	
	SDTFilter=SetFilter(PID_SDT, (unsigned short)((TID_SDT_ACT<<8)|0x00ff), SECTION_ANY|SECTION_CONTINUOUS|PBUFSIZE_NONE, "SDT Filter");
	if ( SDTFilter < 0 ) {
		MessageBox(hWnd,t("Keine HW-Filter mehr frei !!!!! "),t("MultiDec"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	
	
	SDT.ServiceAnzahl=0;
	
	while ( Loop >= 0 && ngets++ < MAX_SDT_GETS) {
		if ( Loop > 0 ) ResetEvent(FilterEvent[SDTFilter]);
		if (WaitForSingleObject(FilterEvent[SDTFilter], Timing.Sdt_TimeOut ) == WAIT_TIMEOUT ) {
			(void)DelBitFilter(SDTFilter);
			return(FALSE);
		}
		
		sdt=(struct sdt_t *)Filter[SDTFilter].Buffer;
		
		sec_len = ((sdt->BitField1&0x0f)<<8)+sdt->section_length_lo;
		if (sec_len < 12 || sec_len > 1024 ||
	          ccitt32_checkcrc(Filter[SDTFilter].Buffer, sec_len+3) != 0) continue;
		sec_len--; /* + 3 - 4 for CRC */

		if (sdt->table_id != TID_SDT_ACT &&
			sdt->table_id != TID_SDT_OTH) {
			sprintf(Text, "Incorrect table id 0x%02x in SDT stream", sdt->table_id);
			Write_Log(Text);
			continue;
		}
		
		sprintf(Text, "Received SDT for TS id %d, orig nw id %d, len = %d, section %d/%d, current = %d, other = %d",
				  HILO(sdt->transport_stream_id), HILO(sdt->original_network_id), sec_len, sdt->section_number, sdt->last_section_number, sdt->BitField2 & 0x01, sdt->table_id != TID_SDT_ACT);
		Write_Log(Text);
		if (sdt->section_number != Loop ||
		    (sdt->BitField2 & 0x01) == 0) continue;

		if ( Loop == sdt->last_section_number ) Loop = -1;
		else Loop++;
		
		SDT.version = (sdt->BitField2 >> 1)&0x1f;
		SDT.ts_id = HILO(sdt->transport_stream_id);
		SDT.orig_nw_id = HILO(sdt->original_network_id);
		
		buf = Filter[SDTFilter].Buffer;
		ptr = buf + SDT_LEN;
		
		while ((ptr - buf) < sec_len) {
			
			sdt_descr = (struct sdt_descr_t *)ptr;
			
			SID_Id = HILO (sdt_descr->service_id);
			j=Get_New_Prog_by_SID(SID_Id);
			if ( j < 0 ) j = Get_New_Prog();
			if ( j < 0 ) break;
			ProgrammNeu[j].SID_pid=SID_Id;
			ProgrammNeu[j].CA_ID = (sdt_descr->BitField2>>4)&1;
			ProgrammNeu[j].tp_id = SDT.ts_id;
			ProgrammNeu[j].orig_nw_id = SDT.orig_nw_id;
			loop_len = ((sdt_descr->BitField2&0x0f) << 8 ) + sdt_descr->descriptors_loop_length_lo;
			ptr += SDT_DESCR_LEN;
			descr_len = 0;
			sprintf(Text, " -- SDT for program SID %d :", SID_Id);
			Write_Log(Text);
			while (descr_len < loop_len) {
				parse_sdt_descr(ptr, loop_len - descr_len, j);
				descr_len += get_descr_len(ptr) + DESCR_GEN_LEN;
				ptr += get_descr_len(ptr) + DESCR_GEN_LEN;
			}
			SDT.ServiceAnzahl+=1;
		}
		
	}

    if (ngets >= MAX_SDT_GETS) Write_Log("Max gets reached for SDT");
	(void)DelBitFilter(SDTFilter);
	
	return(TRUE);
   }
   
   int parse_sdt_descr(unsigned char *ptr, u_int len, int ProgrammNummer)
   {
	   unsigned char *buf;
	   unsigned char avail;
	   int i,j,k;
	   
	   unsigned short tp_id;
	   unsigned short orig_nw_id;
	   unsigned short sid_id;
	   char Text[512];
	   
	   buf=ptr;
	   
	   switch (get_descr(buf)) {
	   case DESCR_STUFFING:
		   break;

       case DESCR_COUNTRY_AVAIL: 
		   if (get_descr_len(buf) > len) return(0);
		   buf += DESCR_GEN_LEN;
		   avail = *buf & 0x01;
		   buf += 1;
		   i=get_descr_len(ptr);
		   sprintf(Text, "--> Country availability : %d for %-*.*s", avail, i, i, buf);
		   Write_Log(Text);
		   if ( i > 29 ) i = 29;
		   memcpy(&ProgrammNeu[ProgrammNummer].Land,buf,i);
		   ProgrammNeu[ProgrammNummer].Land[i] = 0;
		   break;
		   
	   case DESCR_SERVICE: 
		   if (get_descr_len(buf) > len) return(0);
		   buf += DESCR_GEN_LEN;
		   ProgrammNeu[ProgrammNummer].ServiceTyp=*buf;
		   buf++;
		   if (*buf>0) {
			   i=*buf;
			   k=0;
			   j=0;
			   while ( j < i ) {
				   if (( buf[j+1] != 0x86 ) && ( buf[j+1] != 0x87 ))
					ProgrammNeu[ProgrammNummer].Anbieter[k++]=buf[j+1];
				   ProgrammNeu[ProgrammNummer].Anbieter[k]=0x00;
				   if ( k >= sizeof(ProgrammNeu[ProgrammNummer].Anbieter)-1 ) break;
				   j++;
			   };
		   } else {
			   strcpy(ProgrammNeu[ProgrammNummer].Anbieter,t(">NULL length"));
		   };
		   
		   buf += *buf + 1;
		   if (*buf>0) {
			   i=*buf;
			   k=0;
			   j=0;
			   while ( j < i ) {
				   if (( buf[j+1] >= 0x20 ) && ( buf[j+1] != 0x86 ) && ( buf[j+1] != 0x87 )) ProgrammNeu[ProgrammNummer].Name[k++]=buf[j+1];
				   ProgrammNeu[ProgrammNummer].Name[k]=0x00;
				   if ( k >= sizeof(ProgrammNeu[ProgrammNummer].Name)-1 ) break;
				   j++;
			   };
		   } else {
			   strcpy(ProgrammNeu[ProgrammNummer].Name,t(">NULL length"));
		   };
		   sprintf(Text, "--> Service : type = %d, provider = %s, name = %s",
			   ProgrammNeu[ProgrammNummer].ServiceTyp, ProgrammNeu[ProgrammNummer].Anbieter, ProgrammNeu[ProgrammNummer].Name);
		   Write_Log(Text);

		   if ( ProgrammNeu[ProgrammNummer].ServiceTyp == 4 ) {
			   i=1;
		   };
		   break;
	   case DESCR_NVOD_REF:
		   buf++;
		   i=*buf;
		   buf++;
		   j=0;
		   while ( j < i ) {
			   tp_id=Swab16((unsigned char *)buf);
			   buf+=2;
			   orig_nw_id=Swab16((unsigned char *)buf);
			   buf+=2;
			   sid_id=Swab16((unsigned char *)buf);
			   sprintf(Text, "--> NVOD ref : tp_id = %d, orig nw id = %d, sid = %d", tp_id, orig_nw_id, sid_id);
			   Write_Log(Text);

			   
			   buf+=2;
			   j+=6;
			   k=Find_Programm(tp_id,sid_id);
			   if ( k < 0 ) {
				   k=Find_ProgrammNeu(tp_id,sid_id);
				   if ( k < 0 ) {
					   k=Get_New_Prog();
					   if (k<0) break;
					   // Link-Client-Programm-Anlegen
					   strcpy(ProgrammNeu[k].Name,ProgrammNeu[ProgrammNummer].Name);
					   strcpy(ProgrammNeu[k].Anbieter,ProgrammNeu[ProgrammNummer].Anbieter);
					   strcpy(ProgrammNeu[k].Land,ProgrammNeu[ProgrammNummer].Land);
					   ProgrammNeu[k].orig_nw_id = ProgrammNeu[ProgrammNummer].orig_nw_id;
					   ProgrammNeu[k].CA_ID = ProgrammNeu[ProgrammNummer].CA_ID;
					   ProgrammNeu[k].SID_pid=sid_id;
					   ProgrammNeu[k].tp_id=tp_id;
					   ProgrammNeu[k].PMT_pid=0x1fff; // Filler ?????
					   ProgrammNeu[k].ServiceTyp=0x05;
					   ProgrammNeu[k].Link_TP=ProgrammNeu[ProgrammNummer].tp_id;
					   ProgrammNeu[k].Link_SID=ProgrammNeu[ProgrammNummer].SID_pid;
					   ProgrammNeu[k].Dynamisch=0x01;
				   } else {
					   strcpy(ProgrammNeu[k].Name,ProgrammNeu[ProgrammNummer].Name);
					   strcpy(ProgrammNeu[k].Anbieter,ProgrammNeu[ProgrammNummer].Anbieter);
					   strcpy(ProgrammNeu[k].Land,ProgrammNeu[ProgrammNummer].Land);
					   ProgrammNeu[k].ServiceTyp=0x05;
					   ProgrammNeu[k].Dynamisch=0x01;
					   ProgrammNeu[k].CA_ID = ProgrammNeu[ProgrammNummer].CA_ID;
					   ProgrammNeu[k].Link_TP=ProgrammNeu[ProgrammNummer].tp_id;
					   ProgrammNeu[k].Link_SID=ProgrammNeu[ProgrammNummer].SID_pid;
				   }
			   } else {
				   
				   if (( Programm[k].Name[0] >= '0' ) && 
					   ( Programm[k].Name[0] <= '9' ) && 
					   ( Programm[k].Name[1] >= '0' ) && 
					   ( Programm[k].Name[1] <= '9' ) && 
					   ( Programm[k].Name[2] >= '0' ) && 
					   ( Programm[k].Name[2] <= '9' )) {
					   strcpy(Programm[k].Name,ProgrammNeu[ProgrammNummer].Name);
				   };
				   
				   Programm[k].ServiceTyp=0x05;
				   Programm[k].Dynamisch=0x01;
				   strcpy(Programm[k].Anbieter,ProgrammNeu[ProgrammNummer].Anbieter);
				   strcpy(Programm[k].Land,ProgrammNeu[ProgrammNummer].Land);
				   Programm[k].orig_nw_id = ProgrammNeu[ProgrammNummer].orig_nw_id;
				   Programm[k].CA_ID = ProgrammNeu[ProgrammNummer].CA_ID;
				   Programm[k].Link_TP=ProgrammNeu[ProgrammNummer].tp_id;
				   Programm[k].Link_SID=ProgrammNeu[ProgrammNummer].SID_pid;
			   }
		   }
		   break;
		   
	   case DESCR_BOUQUET_NAME:
		   buf++;
		   i=*buf;
		   buf++;
		   sprintf(Text, "--> Bouquet name : %-*.*s", i, i, buf);
		   Write_Log(Text);
		   i=0;
		   break;
		   
	   case DESCR_CA_IDENT:
		   buf++;
		   i=*buf;
		   buf++;
		   for (j=0; j<i; j+=2) {
				sprintf(Text, "--> CA identifier : 0x%04x", Swab16((unsigned char *)buf+j));
				Write_Log(Text);
		   }
		   i=0;
		   break;
	   
	   case DESCR_DATA_BROADCAST:
           sprintf(Text, "--> Data broadcast ID = %d, tag = %d", (buf[2]<<8)|buf[3], buf[4]);
           Write_Log(Text);
           i = buf[5];
           sprintf(Text, "    Selector (len = %d) :", i);
           Write_Log(Text);
           dump_hex(buf+6, (unsigned short) i);
           sprintf(Text, "    Language = %-3.3s", buf+6+i);
           Write_Log(Text);
           sprintf(Text, "    Text = %-*.*s", buf[9+i], buf[9+i], buf+10+i);
           Write_Log(Text);
           break;

	   case DESCR_LINKAGE:
	   case DESCR_TIME_SHIFTED_SERVICE:
	   case DESCR_MOSAIC:
	   case DESCR_TELEPHONE:
	   case DESCR_ML_SERVICE_NAME:
	   case DESCR_NW_NAME:
	   case DESCR_PRIV_DATA_SPEC:
			sprintf(Text, "Descriptor 0x%02x :", get_descr(buf));
			Write_Log(Text);
		    buf++;
		    i=*buf;
		    buf++;
			dump_hex(buf, (unsigned char)i);
		    i=0;

			break;
		   
	   default:
		    sprintf(Text, "SDT: invalid descriptor (0x%x)", get_descr(buf));
			Write_Log(Text);
		    buf++;
		    i=*buf;
		    buf++;
			dump_hex(buf, (unsigned char)i);
		    break;
	};
	
	return 0;
}


int Get_New_Prog(void){
	int i;
	for ( i = 0; i < NEUSIZE ; i++ ) {
		if (( ProgrammNeu[i].PMT_pid == 0x1fff ) && ( ProgrammNeu[i].SID_pid == 0x1fff)) return(i);
	};
	Write_Log("No more space available for ProgrammNeu");

	return (-1);
}

int Get_New_Prog_by_SID(unsigned short SID){
   	int i;
	for ( i = 0; i < NEUSIZE ; i++ ) {
		if (  ProgrammNeu[i].SID_pid == SID ) return(i);
	};
	return(-1);
}

int Get_New_Prog_by_PMT(unsigned short PMT){
   	int i;
	for ( i = 0; i < NEUSIZE ; i++ ) {
		if (  ProgrammNeu[i].PMT_pid == PMT ) return(i);
	};
	return(-1);
}


BOOL Get_Pat( void ){
	int i,j;
	int sec_len;
	unsigned char *ptr;
	int num_progs;
	
	struct pat_prog_t {
		unsigned char program_number_hi;
		unsigned char program_number_lo;
		unsigned char BitField1;
		// unsigned  network_pid_hi:5;
		// unsigned  dummy:3;
		unsigned char network_pid_lo;
	};
	
	struct pat_prog_t *pat_prog;
	
	
	
	struct pat_t {
		unsigned char table_id;
		unsigned char BitField1 ;
		// section_length_hi		:4;
		// unsigned dummy0					:2;
		// unsigned dummy1				:1;	// has to be 0
		// unsigned section_syntax_indicator		:1;
		unsigned char section_length_lo;
		unsigned char transport_stream_id_hi;
		unsigned char transport_stream_id_lo;
		unsigned char BitField2 ;
		// current_next_indicator		:1;
		// version_number			:5;
		// dummy3					:2;
		unsigned char section_number;
		unsigned char last_section_number;
	};
	
	struct pat_t *pat_tptr;
    unsigned short prg_num, pmt_sid;
	int Loop = 0;
    int PATFilter;
	char Text[256];
	int ngets = 0;
#define MAX_PAT_GETS 100
	
	PAT.ProgAnzahl=0;
	PATFilter=SetFilter(PID_PAT, (unsigned short)((TID_PAT<<8)|0x00ff), SECTION_ANY|SECTION_CONTINUOUS|PBUFSIZE_NONE, "PAT Filter");
	
	if ( PATFilter < 0 ) {
		MessageBox(hWnd,t("Keine HW-Filter mehr frei !!!!! "),t("MultiDec"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	
	
	
	while ( Loop >= 0 && ngets++ < MAX_PAT_GETS) {
		if ( Loop > 0 ) ResetEvent(FilterEvent[PATFilter]);
		if ( WaitForSingleObject(FilterEvent[PATFilter], Timing.Pat_TimeOut ) == WAIT_TIMEOUT ) {
			(void)DelBitFilter(PATFilter);
			return(FALSE);
		}
		
		pat_tptr = (struct pat_t *)Filter[PATFilter].Buffer;
		
        sec_len = ((pat_tptr->BitField1&0x0f)<<8)+pat_tptr->section_length_lo;
		if (sec_len < 12 || sec_len > 1024 ||
	          ccitt32_checkcrc(Filter[PATFilter].Buffer, sec_len+3) != 0) continue;
		sec_len--; /* + 3 - 4 for CRC */

	    if (pat_tptr->table_id != TID_PAT) {
			sprintf(Text, "Incorrect table id 0x%02x in PAT stream", pat_tptr->table_id);
			Write_Log(Text);
			continue;
	    }
	
		sprintf(Text, "Received PAT for TS id %d, len = %d, section %d/%d, current = %d",
				  HILO(pat_tptr->transport_stream_id), sec_len, pat_tptr->section_number, pat_tptr->last_section_number, pat_tptr->BitField2 & 0x01);
		Write_Log(Text);
		if (pat_tptr->section_number != Loop ||
		    (pat_tptr->BitField2 & 0x01) == 0) continue;

		if ( Loop == pat_tptr->last_section_number ) Loop = -1;
		else Loop++;

		PAT.ts_id = HILO (pat_tptr->transport_stream_id);
		PAT.version = ( pat_tptr->BitField2 >> 1) & 0x1f;
		
		ptr=Filter[PATFilter].Buffer;
		
		num_progs = (sec_len - 8 ) / 4;	// 4 CRC_LEN
		ptr += 8;
		PAT.ProgAnzahl=num_progs;
		
		for ( i = 0 ; i < num_progs; i++ ) {
			
			pat_prog = (struct pat_prog_t *)ptr;
            prg_num = HILO(pat_prog->program_number);
            pmt_sid = ((pat_prog->BitField1&0x1f) << 8) + pat_prog->network_pid_lo;
            sprintf(Text, " -- Program SID %d has the PMT SID %d (0x%04x)", prg_num, pmt_sid, pmt_sid);
            Write_Log(Text);
			j=Get_New_Prog();
			if ( j>= 0 ) {
				ProgrammNeu[j].tp_id = PAT.ts_id;
				ProgrammNeu[j].SID_pid = prg_num;
				ProgrammNeu[j].PMT_pid = pmt_sid;
			}
			ptr+=4;
		}

    }

    if (ngets >= MAX_PAT_GETS) Write_Log("Max gets reached for PAT");
	(void)DelBitFilter(PATFilter);
	return(TRUE);
}

void Insert_CAT(int Nr){
	int i,j;
	if ( ProgrammNeu[Nr].CA_Anzahl == 0 ) return;
	if ( TPCatAnzahl == 0xffff ) return;
	if ( TPCatAnzahl == 0 ) Get_CAT();
	if ( TPCatAnzahl == 0xffff ) return;
	
	for ( i=0; i < TPCatAnzahl; i++ ) {
		for ( j =0; j < ProgrammNeu[Nr].CA_Anzahl; j++ ) {
			if ( TPCat[i].CA_ID == ProgrammNeu[Nr].CA_System[j].CA_Typ ) {
				ProgrammNeu[Nr].CA_System[j].EMM=TPCat[i].EMM;
			};
		}
	}
	return;
};


BOOL Get_CAT( void ){
	int sec_len;
	unsigned char *ptr;
	
#define CA_LEN 8
	struct  ca_t {
		unsigned char table_id;
		unsigned char BitField1;
		//	u_char	section_length_hi		: 4;
		//	u_char	        			: 2;
		//	u_char	zero    			: 1;
		//	u_char	section_syntax_indicator	: 1;
		unsigned char section_length_lo;
		unsigned char reserved_1;
		unsigned char reserved_2;
		unsigned char BitField2;
		//	u_char current_next_indicator		: 1;
		//	u_char version_number 			: 5;
		//	u_char	        			: 2;
		unsigned char section_number;
		unsigned char last_section_number;
	};
	
	
#define CA_DESCR_LEN 6
	
	struct ca_descr_t {
		unsigned char 	descriptor_tag;
		unsigned char 	descriptor_length;
		unsigned char 	CA_system_ID_hi;
		unsigned char 	CA_system_ID_lo;
		unsigned char   BitField1;
		//u_char	CA_PID_hi                   	: 5;
		//u_char	                             	: 3;
		unsigned char   CA_PID_lo                   	: 8;
	};
	
	struct ca_descr_t *cs_desc;
	struct ca_t *ca_tptr;
	int Loop = 0;
	int len;
    int CATFilter;
    char Text[256];
	int ngets = 0;
#define MAX_CAT_GETS 100
	
	TPCatAnzahl=0xffff;
    
	
	CATFilter=SetFilter(PID_CAT, (unsigned short)((TID_CAT<<8)|0x00ff), SECTION_ANY|SECTION_CONTINUOUS|PBUFSIZE_NONE, "CAT Filter");
	if ( CATFilter < 0 ) {
		MessageBox(hWnd,t("Keine HW-Filter mehr frei !!!!! "),t("MultiDec"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	
	while ( Loop >= 0 && ngets++ < MAX_CAT_GETS) {
		if ( Loop > 0 ) ResetEvent(FilterEvent[CATFilter]);
		if ( WaitForSingleObject(FilterEvent[CATFilter], Timing.Cat_TimeOut ) == WAIT_TIMEOUT ) {
			(void)DelBitFilter(CATFilter);
			return(FALSE);
		}
		ca_tptr = (struct ca_t *)Filter[CATFilter].Buffer;
		
		sec_len = ((ca_tptr->BitField1&0x0f)<< 8) + ca_tptr->section_length_lo;
		
		if (sec_len < 9 || sec_len > 1024 ||
            ccitt32_checkcrc(Filter[CATFilter].Buffer, sec_len+3) != 0) {
            Write_Log("Received invalid CAT packet :");
            dump_hex(Filter[CATFilter].Buffer, 1024);
            continue;
        }
		sec_len--; /* + 3 - 4 for CRC */

		if (ca_tptr->table_id != TID_CAT) {
			sprintf(Text, "Incorrect table id 0x%02x in CAT stream", ca_tptr->table_id);
			Write_Log(Text);
			continue;
		}

        sprintf(Text, "Received CAT, len = %d, section %d/%d, current = %d",
				  sec_len, ca_tptr->section_number, ca_tptr->last_section_number, ca_tptr->BitField2 & 0x01);
		Write_Log(Text);

        if ( ca_tptr->section_number != Loop ||
		    (ca_tptr->BitField2 & 0x01) == 0 ) continue;
		
		if ( Loop == ca_tptr->last_section_number ) Loop = -1;
		else Loop++;
		
		TPCatAnzahl=0;
		ptr=Filter[CATFilter].Buffer;

        ptr += CA_LEN;
		len=CA_LEN;
		while ( len + CA_DESCR_LEN <= sec_len ) {
			cs_desc=(struct ca_descr_t *)ptr;
            sprintf(Text, "Descr. 0x%02x, len = %d :", cs_desc->descriptor_tag, cs_desc->descriptor_length);
            Write_Log(Text);
            switch (cs_desc->descriptor_tag) {
            case DESCR_CA:
    			parse_CA_Infos(-1, ptr, 0, TRUE); 
		        if ( TPCatAnzahl < 32 ) {
			        TPCat[TPCatAnzahl].TAG = cs_desc->descriptor_tag;
			        TPCat[TPCatAnzahl].DesLen = cs_desc->descriptor_length;
			        TPCat[TPCatAnzahl].CA_ID = HILO(cs_desc->CA_system_ID);
			        TPCat[TPCatAnzahl].EMM = ((cs_desc->BitField1&0x1f)<< 8) + cs_desc->CA_PID_lo;
			        TPCat[TPCatAnzahl].BufferLen=(cs_desc->descriptor_length +2 );
			        if ( TPCat[TPCatAnzahl].BufferLen > 64 ) TPCat[TPCatAnzahl].BufferLen=64;
			        
			        memcpy(&TPCat[TPCatAnzahl].Buffer[0],ptr,TPCat[TPCatAnzahl].BufferLen);
			        TPCatAnzahl++;
		        }
                break;
            default:
		        sprintf(Text, "CAT: invalid descriptor (0x%02x)", cs_desc->descriptor_tag);
			    Write_Log(Text);
			    dump_hex(ptr+2, cs_desc->descriptor_length);
		        break;
            }
			len += (cs_desc->descriptor_length +2);
			ptr += (cs_desc->descriptor_length +2);
		}
        if (len < sec_len) {
            Write_Log("Remaining CAT infos :");
            dump_hex(ptr, (unsigned short) (sec_len-len));
        }

	}

    if (ngets >= MAX_CAT_GETS) Write_Log("Max gets reached for CAT");
	(void)DelBitFilter(CATFilter);
	return(TRUE);
}


BOOL Get_Nit( unsigned char diseq, BOOL load_ACT){
	
	struct nit_mid_t {	// after descriptors
		u_char BitField1;
		// u_char transport_stream_loop_length_hi	:4;
		// u_char 					:4;
		u_char transport_stream_loop_length_lo;
	};
	
	struct nit_ts_t {
		u_char transport_stream_id_hi;
		u_char transport_stream_id_lo;
		u_char original_network_id_hi;
		u_char original_network_id_lo;
		u_char BitField1;
		// u_char transport_descriptors_length_hi	:4;
		// u_char 					:4;
		u_char transport_descriptors_length_lo;
		
		/* descriptors  */
	};
	
	
	
	struct nit_t {
		u_char table_id;
		u_char BitField1;
		//	u_char section_length_hi		:4;
		//	u_char 					:3;
		//	u_char section_syntax_indicator		:1;
		u_char section_length_lo;
		u_char network_id_hi;
		u_char network_id_lo;
		u_char BitField2;
		//	u_char current_next_indicator		:1;
		//	u_char version_number			:5;
		//	u_char 					:2;
		u_char section_number;
		u_char last_section_number;
		u_char BitField3;
		// u_char network_descriptor_length_hi	:4;
		// u_char 					:4;
		u_char network_descriptor_length_lo;
	};
	
	
	struct nit_t *nit_tptr;
	struct nit_mid_t *nit_mid;
	struct nit_ts_t *nit_ts;
	unsigned char *ptr;
	
	int i;
	int sec_len;
    int Loop;
    int loop_id;
    unsigned short nw_id;
	
	static int loop_len;
	static int descr_len;
	int len;
    int NITFilter;
    char Text[256];
	int ngets = 0;
#define MAX_NIT_GETS 20
	
	NitAnzahl=0;
    memset(NITNeu, 0, sizeof(struct tNIT)*NEUNIT);
	for ( i=0; i < NEUNIT; i++ ) {
		NITNeu[i].DiseqNr=diseq;
	};
	for (loop_id=0; loop_id<(load_ACT?1:2); loop_id++) {
		Loop = 0;
		// We filter NIT ACT and NIT OTH tables
		NITFilter=SetFilter(PID_NIT, (unsigned short)(((loop_id==0?TID_NIT_ACT:TID_NIT_OTH)<<8)|0x00ff), SECTION_ANY|SECTION_CONTINUOUS|PBUFSIZE_NONE, "NIT Filter");
		if ( NITFilter < 0 ) {
			MessageBox(hWnd,t("Keine HW-Filter mehr frei !!!!! "),t("MultiDec"),MB_ICONSTOP|MB_OK);
			return(FALSE);
		}
		
		while ( Loop >= 0 && ngets++ <= MAX_NIT_GETS) {
			if ( Loop > 0) ResetEvent(FilterEvent[NITFilter]);
			if ( WaitForSingleObject(FilterEvent[NITFilter], Timing.Nit_TimeOut*(2-loop_id) ) == WAIT_TIMEOUT ) {
				(void)DelBitFilter(NITFilter);
				return(FALSE);
			}
		
			nit_tptr = (struct nit_t *)Filter[NITFilter].Buffer;

			sec_len = ((nit_tptr->BitField1&0x0f)<<8)+nit_tptr->section_length_lo;
			if (sec_len < 9 || sec_len > 1024 ||
				  ccitt32_checkcrc(Filter[NITFilter].Buffer, sec_len+3) != 0) continue;
			sec_len--; /* + 3 - 4 for CRC */

			if (nit_tptr->table_id != (loop_id==0?TID_NIT_ACT:TID_NIT_OTH)) {
				 sprintf(Text, "Incorrect table id 0x%02x in NIT stream", nit_tptr->table_id);
				Write_Log(Text);
				continue;
			}
		
			sprintf(Text, "Received NIT for nw id %d, len = %d, section %d/%d, current = %d, other = %d",
					  HILO(nit_tptr->network_id), sec_len, nit_tptr->section_number, nit_tptr->last_section_number, nit_tptr->BitField2 & 0x01, nit_tptr->table_id != TID_NIT_ACT);
			Write_Log(Text);

			if (nit_tptr->section_number != Loop ||
				(nit_tptr->BitField2 & 0x01) == 0) continue;

			if ( Loop == nit_tptr->last_section_number ) Loop = -1;
			else Loop++;

			descr_len = ((nit_tptr->BitField3&0x0f)<<8)+nit_tptr->network_descriptor_length_lo;
			nw_id = HILO(nit_tptr->network_id);
			
			ptr=Filter[NITFilter].Buffer;
			ptr += 10;

			parse_nit_descr(ptr, NitAnzahl);
			ptr += descr_len;
			
			nit_mid = (struct nit_mid_t *) ptr;
			loop_len = ((nit_mid->BitField1&0x0f)<<8) + nit_mid->transport_stream_loop_length_lo;
			
			ptr+=2;
			
			while (loop_len > 0 && NitAnzahl < NEUNIT)  {
				nit_ts = (struct nit_ts_t *) ptr;

				NITNeu[NitAnzahl].ts_id = HILO (nit_ts->transport_stream_id);
				NITNeu[NitAnzahl].orig_nw_id = HILO (nit_ts->original_network_id);
				
				descr_len = ((nit_ts->BitField1&0x0f)<<8)+nit_ts->transport_descriptors_length_lo;
				ptr+= 6;
				
				sprintf(Text, " -- NIT informations for TS id %d, orig network id %d (len = %d)", NITNeu[NitAnzahl].ts_id, NITNeu[NitAnzahl].orig_nw_id, descr_len);
				Write_Log(Text);

				loop_len=loop_len-(6+descr_len);
				while (descr_len > 0) {
					len=*(ptr+1)+2;
					parse_nit_descr(ptr, NitAnzahl);
					descr_len-=len;
					ptr+=len;
				}
				NitAnzahl++;
			}

		}

		if (ngets >= MAX_NIT_GETS) Write_Log("Max gets reached for NIT");
		(void)DelBitFilter(NITFilter);
	}
	return(TRUE);
	
};


int parse_nit_descr(unsigned char *ptr, int NitNummer){
	struct descr_satellite_delivery_system_struct {
		unsigned char	frequency1;
		unsigned char	frequency2;
		unsigned char	frequency3;
		unsigned char	frequency4;
		unsigned char	orbital_position1;
		unsigned char	orbital_position2;
		
		unsigned char  BitFilter1;
		// u_char	west_east_flag		: 1;
		// u_char	polarization		: 2;
		// u_char	modulation		: 5;
		
		unsigned char	symbol_rate1;
		unsigned char	symbol_rate2;
		unsigned char	symbol_rate3;
		unsigned char  BitFilter2;
		// u_char	fec_inner		: 4;
		// u_char	symbol_rate4		: 4;
	};
	
	struct descr_cable_delivery_system_struct {
		unsigned char	frequency1;
		unsigned char	frequency2;
		unsigned char	frequency3;
		unsigned char	frequency4;
		unsigned char	reserved1;
		unsigned char  BitFilter1;
		//   u_char	fec_outer               : 4;
		//   u_char	reserved2	        : 4;
		
		unsigned char	modulation;
		unsigned char	symbol_rate1;
		unsigned char	symbol_rate2;
		unsigned char	symbol_rate3;
		unsigned char  BitFilter2;
		// u_char	fec_inner		: 4;
		//u_char	symbol_rate4		: 4;
	};
	
	struct descr_satellite_delivery_system_struct *descr;
	struct descr_cable_delivery_system_struct *descrcable;
	int i, j;
	unsigned int freq;
	unsigned int orbit;
	int rate;
    char Text[512];
	
	switch (get_descr(ptr)) {
	case DESCR_NW_NAME:
		i=get_descr_len(ptr);
		NITNeu[NitNummer].Name[0]=0x00;
		if ( i > 31 ) i=31;
		if (i < 0 ) break;
		memcpy (NITNeu[NitNummer].Name, ptr+2,i );
		NITNeu[NitNummer].Name[i]=0x00;
        sprintf(Text, "Network name : %-*.*s", i, i, ptr+2);
        Write_Log(Text);
		break;
		
  	case DESCR_STUFFING:
		i=1;
		break;
		
	case DESCR_SAT_DEL_SYS:
		descr = (struct descr_satellite_delivery_system_struct *)(ptr + 2);
		freq = 10000000 * ((descr->frequency1 & 0xF0) >> 4)
			+  1000000 * ( descr->frequency1 & 0x0F)
			+   100000 * ((descr->frequency2 & 0xF0) >> 4)
			+    10000 * ( descr->frequency2 & 0x0F)
			+     1000 * ((descr->frequency3 & 0xF0) >> 4)
			+      100 * ( descr->frequency3 & 0x0F)
			+       10 * ((descr->frequency4 & 0xF0) >> 4)
			+        1 * ( descr->frequency4 & 0x0F);
		NITNeu[NitNummer].freq=(freq/25)*250; // frequency is coded in ten kHz and stored in kHz
		
		orbit = 1000 * ((descr->orbital_position1 & 0xF0) >> 4)
			+  100 * ( descr->orbital_position1 & 0x0F)
			+   10 * ((descr->orbital_position2 & 0xF0) >> 4)
			+    1 * ( descr->orbital_position2 & 0x0F);
		NITNeu[NitNummer].orbit = orbit;
		NITNeu[NitNummer].west_east = descr->BitFilter1 >> 7; // Was BitFilter1&1
		
		NITNeu[NitNummer].polar= (((descr->BitFilter1 >> 5) & 3) == 1) ? 0 : 1;
		
		rate = 1000000 * ((descr->symbol_rate1 & 0xF0) >> 4)
			+  100000 * ( descr->symbol_rate1 & 0x0F)
			+   10000 * ((descr->symbol_rate2 & 0xF0) >> 4)
			+    1000 * ( descr->symbol_rate2 & 0x0F)
			+     100 * ((descr->symbol_rate3 & 0xF0) >> 4)
			+      10 * ( descr->symbol_rate3 & 0x0F)
			+       1 * ( descr->BitFilter2>>4);
		NITNeu[NitNummer].srate=(rate+5)/10; // + 5 for rounding
		
		NITNeu[NitNummer].fec = (descr->BitFilter2 & 0x0f) - 1;
		if ( NITNeu[NitNummer].fec == 3 ) NITNeu[NitNummer].fec = 4;

        sprintf(Text, "Satellite delivery system : orbit = %3.1f %c, freq = %5.3f MHz, pol. = %c, rate = %d, fec = %d",
            NITNeu[NitNummer].orbit/10.,
            NITNeu[NitNummer].west_east ? 'E' : 'W',
            NITNeu[NitNummer].freq/1000.,
            "HVLR"[(descr->BitFilter1>>5)&3],
            NITNeu[NitNummer].srate,
            (descr->BitFilter1 & 0x0f),
            (descr->BitFilter2 & 0x0f));
        Write_Log(Text);
		break;
		
	case DESCR_CABLE_DEL_SYS:
		i=get_descr_len(ptr);
		if ( i < 11 ) break;
		descrcable = (struct descr_cable_delivery_system_struct *)(ptr + 2);
		
		freq  = 10000000 * ((descrcable->frequency1 & 0xF0) >> 4);
		freq +=  1000000 * ( descrcable->frequency1 & 0x0F);
		freq +=   100000 * ((descrcable->frequency2 & 0xF0) >> 4);
		freq +=    10000 * ( descrcable->frequency2 & 0x0F);
		freq +=     1000 * ((descrcable->frequency3 & 0xF0) >> 4);
		freq +=      100 * ( descrcable->frequency3 & 0x0F);
		freq +=       10 * ((descrcable->frequency4 & 0xF0) >> 4);
		freq +=        1 * ( descrcable->frequency4 & 0x0F);
		NITNeu[NitNummer].freq=freq*10; // frequency is coded in hundred Hz and stored in ten Hz ?
		
		NITNeu[NitNummer].qam = descrcable->modulation;
		
		rate = 1000000 * ((descrcable->symbol_rate1 & 0xF0) >> 4);
		rate += 100000 * ( descrcable->symbol_rate1 & 0x0F);
		rate +=  10000 * ((descrcable->symbol_rate2 & 0xF0) >> 4);
		rate +=   1000 * ( descrcable->symbol_rate2 & 0x0F);
		rate +=    100 * ((descrcable->symbol_rate3 & 0xF0) >> 4);
		rate +=     10 * ( descrcable->symbol_rate3 & 0x0F);
		rate +=      1 * ( descrcable->BitFilter2>>4);
		NITNeu[NitNummer].srate=(rate+5)/10;  // + 5 for rounding
		NITNeu[NitNummer].fec = (descrcable->BitFilter2&0x0f);
        NITNeu[NitNummer].polar = 0xff;

        sprintf(Text, "Cable delivery system : freq = %3.5f MHz, QAM = %d, rate = %d, fec = %d:%d",
            NITNeu[NitNummer].freq/100000.,
            (NITNeu[NitNummer].qam > 0 && NITNeu[NitNummer].qam < 6) ? (8<<NITNeu[NitNummer].qam) : NITNeu[NitNummer].qam,
            NITNeu[NitNummer].srate,
            (descrcable->BitFilter2 & 0x0f));
        Write_Log(Text);
		break;
		
	case DESCR_SERVICE_LIST:
        Write_Log("List of services :");
		i=get_descr_len(ptr);
        for (j=0; j<i; j+=3) {
            sprintf(Text, " -- Service ID %d is of type %d", (ptr[j+2]<<8)|ptr[j+3], ptr[j+4]);
            Write_Log(Text);
        }
        break;

    case DESCR_PRIV_DATA_SPEC:
		sprintf(Text, "--> Private data specifier = 0x%02x%02x%02x%02x", ptr[2], ptr[3], ptr[4], ptr[5]);
		Write_Log(Text);
        break;

    case DESCR_LINKAGE:
	case DESCR_TERR_DEL_SYS:
	case DESCR_ML_NW_NAME:
    case DESCR_FREQUENCY_LIST:
    case DESCR_CELL_LIST:
    case DESCR_CELL_FREQ_LINK:
    case DESCR_ANNOUNCEMENT_SUPPORT:
		sprintf(Text, "Descriptor 0x%02x :", get_descr(ptr));
		Write_Log(Text);
		ptr++;
		i=*ptr;
		ptr++;
		dump_hex(ptr, (unsigned char)i);
		i=1;
		break;
		
    case 0x82:
		sprintf(Text, "Descriptor 0x%02x :", get_descr(ptr));
		Write_Log(Text);
		ptr++;
		i=*ptr;
		ptr++;
        for (j=0; j<i; j+=4) {
            sprintf(Text, "   %d --> %d", (ptr[j]<<8)|ptr[j+1], (ptr[j+2]<<8)|ptr[j+3]);
            Write_Log(Text);
        }
        break;

    case 0x91:
		sprintf(Text, "Descriptor 0x%02x :", get_descr(ptr));
		Write_Log(Text);
		ptr++;
		i=*ptr;
		ptr++;
        for (j=0; j<i; j+=3) {
            sprintf(Text, "   %d --> %d", (ptr[j]<<8)|ptr[j+1], ptr[j+2]);
            Write_Log(Text);
        }
        break;

    default:
        if (get_descr(ptr) >= 0x80 && get_descr(ptr) <= 0xfe) {
    		sprintf(Text, "NIT: private descriptor (0x%02x)", get_descr(ptr));
        } else {
	    	sprintf(Text, "NIT: invalid descriptor (0x%02x)", get_descr(ptr));
        }
		Write_Log(Text);
		ptr++;
		i=*ptr;
		ptr++;
		dump_hex(ptr, (unsigned char)i);
		i=1;
		break;
	};
	
	return 0;
}

int Find_Link_Programm(unsigned short tp_id, unsigned short sid ){
	int i;
	
	for ( i=0; i < MAXPROGS ; i++ ) {
		if (( Programm[i].Link_TP == tp_id ) && ( Programm[i].Link_SID == sid)) return(i);
	};
	return(-1);
}



int Find_Programm(unsigned short tp_id, unsigned short sid ){
	int i;
	
	for ( i=0; i < MAXPROGS ; i++ ) {
		if (( Programm[i].tp_id == tp_id ) && ( Programm[i].SID_pid == sid)) return(i);
	};
	return(-1);
}




int Find_ProgrammNeu(unsigned short tp_id, unsigned short sid ){
	int i;
	for ( i=0; i < NEUSIZE ; i++ ) {
		if (( ProgrammNeu[i].tp_id == tp_id ) && ( ProgrammNeu[i].SID_pid == sid)) return(i);
	};
	return(-1);
}



BOOL Update_Programm_List(int NeuWert ){
	int i;
	unsigned short SaveECM;
	i=0;
	if ( ProgrammNeu[NeuWert].freq != 0) {
		while ( i < MAXPROGS ) {
			if (Programm[i].freq != 0 &&
				Programm[i].tp_id == ProgrammNeu[NeuWert].tp_id &&
				Programm[i].SID_pid == ProgrammNeu[NeuWert].SID_pid) {
				if ( ProgrammNeu[NeuWert].PMT_pid != 0x1fff ) {
					SaveECM=Programm[i].ECM_PID;
					memcpy(&Programm[i], &ProgrammNeu[NeuWert], sizeof(ProgrammNeu[NeuWert]));
					Programm[i].ECM_PID=SaveECM;
					Programm[i].Typ='D';
				}
				if (( ProgrammNeu[NeuWert].PMT_pid == 0x1fff ) && ( Programm[i].PMT_pid == 0x1fff )) {
					SaveECM=Programm[i].ECM_PID;
					memcpy(&Programm[i], &ProgrammNeu[NeuWert], sizeof(ProgrammNeu[NeuWert]));
					Programm[i].ECM_PID=SaveECM;
					Programm[i].Typ='D';
				}
				
				
				
				return(TRUE);
			}
			i++;
		}
	}
	
	i=0;
	while ( i < MAXPROGS ) {
		if  (Programm[i].freq == 0 ) {
			memcpy(&Programm[i], &ProgrammNeu[NeuWert], sizeof(ProgrammNeu[NeuWert]));
			Programm[i].Typ='D';
			return(FALSE);
		}
		i++;
	}
	return(FALSE);
	// Nix gemacht weil nix Frei
}


BOOL Update_Nit_List(int NeuWert ){
	int i;
	char save[127];
	i=0;
	while ( i < MAXNIT ) {
		if (( (unsigned int)(NIT[i].freq/1000) == (unsigned int)(NITNeu[NeuWert].freq/1000) ) &&
			( NIT[i].DiseqNr == NITNeu[NeuWert].DiseqNr)) {
			save[0]=0x00;
			if ( strlen(NIT[i].Name) > strlen(NITNeu[NeuWert].Name) ) strcpy(save,NIT[i].Name);
			memcpy(&NIT[i],&NITNeu[NeuWert],sizeof( struct tNIT ));
			if ( save[0] != 0x00 ) strcpy(NIT[i].Name,save);
			return(TRUE);
		}
		i++;
	}
	
	i=0;
	while ( i < MAXNIT ) {
		if  ( NIT[i].freq == 0 ) {
			memcpy(&NIT[i],&NITNeu[NeuWert],sizeof( struct tNIT ));
			return(FALSE);
		}
		i++;
	}
	return(FALSE);
	// Nix gemacht weil nix Frei 
}



BOOL APIENTRY TunerDigiSettingProc
(
 HWND hDlg,
 UINT message,
 UINT wParam,
 LONG lParam)
{
	int i;
	
	
	
	switch (message) {
	case WM_INITDIALOG:
		SaveTuner=DVBTunerType;
		SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_RESETCONTENT , 0, 0 );
		SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING , 0, (LONG)(LPSTR)"SPXXXX (DVB-C)");
		SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING , 0, (LONG)(LPSTR)"SP5659 (DVB-S)");
		SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING , 0, (LONG)(LPSTR)"TSA5059 (DVB-S)");
		SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING , 0, (LONG)(LPSTR)"TSA5522 (DVB-S)");
		SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING , 0, (LONG)(LPSTR)"SP5659C (DVB-C)");
		SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_SETCURSEL  , DVBTunerType, 0); 
		PostMessage(hDlg,WM_USER,1,0);
		break;
	case WM_USER:
		
		if ( wParam == 1 ) {
            i=SendMessage(GetDlgItem(hDlg,IDC_COMBO1),CB_GETCURSEL,0,0);
			if (( i >= 0 ) && ( i < 5 )) {
				SetDlgItemInt(hDlg,IDT_MIN,DVBTuners[i].min/1000,FALSE);
				SetDlgItemInt(hDlg,IDT_MAX,DVBTuners[i].max/1000,FALSE);
				SetDlgItemInt(hDlg,IDT_RES,DVBTuners[i].res,FALSE);
				SetDlgItemInt(hDlg,IDT_STEP,DVBTuners[i].step,FALSE);
				SetDlgItemInt(hDlg,IDT_THRESH1,DVBTuners[i].thresh1/1000,FALSE);
				SetDlgItemInt(hDlg,IDT_THRESH2,DVBTuners[i].thresh2/1000,FALSE);
				SetDlgItemInt(hDlg,IDT_VHF_L,DVBTuners[i].VHF_L,FALSE);
				SetDlgItemInt(hDlg,IDT_VHF_H,DVBTuners[i].VHF_H,FALSE);
				SetDlgItemInt(hDlg,IDT_UHF,DVBTuners[i].UHF,FALSE);
				SetDlgItemInt(hDlg,IDT_CONFIG,DVBTuners[i].config,FALSE);
				SetDlgItemInt(hDlg,IDT_I2C,DVBTuners[i].I2C,FALSE);
				SetDlgItemInt(hDlg,IDT_IFPCoff,DVBTuners[i].IFPCoff/1000,FALSE);
            };
			
            CheckDlgButton(hDlg,IDC_CHECK1,DVBTuners[5].CableTuner);
			SetDlgItemInt(hDlg,IDM_MIN,DVBTuners[5].min/1000,FALSE);
            SetDlgItemInt(hDlg,IDM_MAX,DVBTuners[5].max/1000,FALSE);
            SetDlgItemInt(hDlg,IDM_RES,DVBTuners[5].res,FALSE);
            SetDlgItemInt(hDlg,IDM_STEP,DVBTuners[5].step,FALSE);
			
			SetDlgItemInt(hDlg,IDM_THRESH1,DVBTuners[5].thresh1/1000,FALSE);
            SetDlgItemInt(hDlg,IDM_THRESH2,DVBTuners[5].thresh2/1000,FALSE);
            SetDlgItemInt(hDlg,IDM_VHF_L,DVBTuners[5].VHF_L,FALSE);
            SetDlgItemInt(hDlg,IDM_VHF_H,DVBTuners[5].VHF_H,FALSE);
            SetDlgItemInt(hDlg,IDM_UHF,DVBTuners[5].UHF,FALSE);
            SetDlgItemInt(hDlg,IDM_CONFIG,DVBTuners[5].config,FALSE);
            SetDlgItemInt(hDlg,IDM_I2C,DVBTuners[5].I2C,FALSE);
            SetDlgItemInt(hDlg,IDM_IFPCoff,DVBTuners[5].IFPCoff/1000,FALSE);
		};
		break;
		
	case WM_COMMAND:
		
		if (LOWORD(wParam) == IDC_COMBO1 ) {
			if ( HIWORD( wParam ) == CBN_SELCHANGE  ) {
				SendMessage(hDlg,WM_USER,1,0);
			}
		}
		
		
		if (LOWORD(wParam) == IDOK ) {
			DVBTunerType=5;
			
            DVBTuners[DVBTunerType].CableTuner=IsDlgButtonChecked(hDlg,IDC_CHECK1);
			DVBTuners[DVBTunerType].min=GetDlgItemInt(hDlg,IDM_MIN,NULL,FALSE)*1000;
            DVBTuners[DVBTunerType].max=GetDlgItemInt(hDlg,IDM_MAX,NULL,FALSE)*1000;
            DVBTuners[DVBTunerType].res=GetDlgItemInt(hDlg,IDM_RES,NULL,FALSE);
            DVBTuners[DVBTunerType].step=GetDlgItemInt(hDlg,IDM_STEP,NULL,FALSE);
			DVBTuners[DVBTunerType].thresh1=GetDlgItemInt(hDlg,IDM_THRESH1,NULL,FALSE)*1000;
            DVBTuners[DVBTunerType].thresh2=GetDlgItemInt(hDlg,IDM_THRESH2,NULL,FALSE)*1000;
            DVBTuners[DVBTunerType].VHF_L=GetDlgItemInt(hDlg,IDM_VHF_L,NULL,FALSE);
            DVBTuners[DVBTunerType].VHF_H=GetDlgItemInt(hDlg,IDM_VHF_H,NULL,FALSE);
            DVBTuners[DVBTunerType].UHF=GetDlgItemInt(hDlg,IDM_UHF,NULL,FALSE);
            DVBTuners[DVBTunerType].config=GetDlgItemInt(hDlg,IDM_CONFIG,NULL,FALSE);
            DVBTuners[DVBTunerType].I2C=GetDlgItemInt(hDlg,IDM_I2C,NULL,FALSE);
			DVBTuners[DVBTunerType].IFPCoff=GetDlgItemInt(hDlg,IDM_IFPCoff,NULL,FALSE)*1000;
			CheckMenuItem( GetMenu(hWnd), SaveTuner+1100, MF_UNCHECKED );
			CheckMenuItem( GetMenu(hWnd), DVBTunerType+1100, MF_CHECKED );
			EndDialog(hDlg,TRUE);
		}
		
		if (LOWORD(wParam) == IDCANCEL ) {
			DVBTunerType=SaveTuner;
			EndDialog(hDlg,TRUE);
		}
		break;
    }
	
	return(FALSE);
}


BOOL UseDiseqC(void){ 
    if ( LNB[1].Use == TRUE ) return(TRUE);
    if ( LNB[2].Use == TRUE ) return(TRUE);
    if ( LNB[3].Use == TRUE ) return(TRUE);
	return(FALSE);
};

BOOL Set_Transponder_Trace(HWND hDlg, struct TTransponder *Transponder ){
	int Band = 0;
	char Zeile[128];
	
	SetDlgItemText(hDlg,TEXT1,"");
	SetDlgItemText(hDlg,TEXT6,"");
	SetDlgItemText(hDlg,TEXT7,"");
	SetDlgItemText(hDlg,TEXT8,"");
	SetDlgItemText(hDlg,TEXT9,"");
	SetDlgItemText(hDlg,TEXT10,"");
	SetDlgItemText(hDlg,TEXT13,"");
	SetDlgItemText(hDlg,TEXT14,"");
	SetDlgItemText(hDlg,TEXT15,"");
	SetDlgItemText(hDlg,TEXT16,"");
	SetDlgItemText(hDlg,TEXT23,"");
	SetDlgItemText(hDlg,TEXT18,"");
	SetDlgItemText(hDlg,TEXT19,"");
	SetDlgItemText(hDlg,TEXT20,"");
	
	sprintf(Zeile,t("Trace für LNB %d - DiseqC Nr. %d "),Transponder->diseqc+1,Transponder->diseqc);
	SetDlgItemText(hDlg,TEXT1,Zeile);
	
	dvb_front.power=LNB[Transponder->diseqc].Power;
	Transponder->power=dvb_front.power;
	
	if ( dvb_front.power == 0 ) SetDlgItemText(hDlg,TEXT6,t("LNB-Spannungsversorgung ist ausgeschaltet"));
	else SetDlgItemText(hDlg,TEXT6,t("LNB-Spannungsversorgung ist eingeschaltet"));
	
	dvb_front.ttk=0;
	if ( LNB[Transponder->diseqc].Switch22khz == TRUE ) {
		if ( Transponder->freq/1000 >= LNB[Transponder->diseqc].SwitchFreq )  dvb_front.ttk= 1;
	};
	
	if ( dvb_front.ttk == 0 ) SetDlgItemText(hDlg,TEXT7,t("22 Khz Signal ist aus"));
	else SetDlgItemText(hDlg,TEXT7,t("22 Khz Signal ist an"));
	
	if ( Transponder->freq/1000 >= LNB[Transponder->diseqc].SwitchFreq ) Band=1;
	
	if ( Band == 0 ) SetDlgItemText(hDlg,TEXT8,t("Low-Band angefordert "));
	else SetDlgItemText(hDlg,TEXT8,t("High-Band angefordert"));
	
	
	ShutDownFilters();
	
	dvb_front.volt=Transponder->volt;
	dvb_front.fec=Transponder->fec;
	dvb_front.diseqc=Transponder->diseqc;
	dvb_front.freq=Transponder->freq;
	dvb_front.srate=Transponder->srate;
	dvb_front.video_pid=0;
	dvb_front.audio_pid=0;
	dvb_front.tt_pid=0x1fff;
	dvb_front.afc=0;
	
	if ( Is_Sat_Karte() == TRUE ) { 
		if ( UseDiseqC() == TRUE ) {
			
			if ( LNB[Transponder->diseqc].MiniDiseqc == 0x01  )  {
				if ( LNB[Transponder->diseqc].BurstVal == -1  ) strcpy(Zeile,t("Mini-DiSEqC Tone-Burst nicht verwenden"));
				else if ( LNB[Transponder->diseqc].BurstVal == 0 ) strcpy(Zeile,t("Mini-DiSEqC Tone-Burst aus"));
				else if ( LNB[Transponder->diseqc].BurstVal == 1 ) strcpy(Zeile,t("Mini-DiSEqC Tone-Burst an"));
				SetDlgItemText(hDlg,TEXT10,Zeile);
			} else {
				
				if ( LNB[Transponder->diseqc].BurstVal == -1  ) strcpy(Zeile,t("DiSEqC Tone-Burst nicht verwenden"));
				else if ( LNB[Transponder->diseqc].BurstVal == 0 ) strcpy(Zeile,t("DiSEqC Tone-Burst aus"));
				else if ( LNB[Transponder->diseqc].BurstVal == 1 ) strcpy(Zeile,t("DiSEqC Tone-Burst an"));
				SetDlgItemText(hDlg,TEXT10,Zeile);
				
				sprintf(Zeile,"DiSEqC Position %c  Option %c",LNB[Transponder->diseqc].PosVal,LNB[Transponder->diseqc].OptVal);
				SetDlgItemText(hDlg,TEXT13,Zeile);
			};
		} else SetDlgItemText(hDlg,TEXT13,t("Kein DiSEqC Kommando"));
		
		SetDiSEqC(dvb_front.diseqc,Band,dvb_front.volt);
		if ( dvb_front.ttk == 1 ) SetDlgItemText(hDlg,TEXT15,t("Stelle 22 Khz-Signal an"));
		else SetDlgItemText(hDlg,TEXT15,t("Stelle 22 Khz-Signal aus"));
		Set22K( dvb_front.ttk);
	};
	sprintf(Zeile,t("Stelle Frequenz %10.3f Ghz ein "),dvb_front.freq/1000.);
	SetDlgItemText(hDlg,TEXT16,Zeile);
	Set_TV_Freq((unsigned __int64)dvb_front.freq,dvb_front.diseqc); 
	if (( Transponder->power == 1 ) && ( Transponder->volt == 1 )) strcpy(Zeile,t("Stelle LNB-Spannung an und horizontale Pol. ein"));
	else if (( Transponder->power == 1 ) && ( Transponder->volt == 0 )) strcpy(Zeile,t("Stelle LNB-Spannung an und vertikale Pol. ein"));
	if (( Transponder->power == 0 ) && ( Transponder->volt == 1 )) strcpy(Zeile,t("Stelle LNB-Spannung aus und horizontale Pol. ein"));
	if (( Transponder->power == 0 ) && ( Transponder->volt == 0 )) strcpy(Zeile,t("Stelle LNB-Spannung aus und vertikale Pol. ein"));
	SetDlgItemText(hDlg,TEXT23,Zeile);
	Set_Transponder_Pol(Transponder);
	if ( Transponder->fec == 0 ) sprintf(Zeile,t("Stelle FEC 1/2 ein"));
	else if ( Transponder->fec == 1 ) sprintf(Zeile,t("Stelle FEC 2/3 ein"));
	else if ( Transponder->fec == 2 ) sprintf(Zeile,t("Stelle FEC 3/4 ein"));
	else if ( Transponder->fec == 3 ) sprintf(Zeile,t("Stelle FEC 4/5 ein"));
	else if ( Transponder->fec == 4 ) sprintf(Zeile,t("Stelle FEC 5/6 ein"));
	else if ( Transponder->fec == 5 ) sprintf(Zeile,t("Stelle FEC 6/7 ein"));
	else if ( Transponder->fec == 6 ) sprintf(Zeile,t("Stelle FEC 7/8 ein"));
	else if ( Transponder->fec == 7 ) sprintf(Zeile,t("Stelle FEC 8/9 ein"));
	else if ( Transponder->fec == 8 ) sprintf(Zeile,t("Stelle FEC auf Auto"));
	SetDlgItemText(hDlg,TEXT18,Zeile);
	SetFEC(Transponder->fec);
	sprintf(Zeile,t("Stelle Symbolrate %d ein "),Transponder->srate);
	SetDlgItemText(hDlg,TEXT19,Zeile);
	Set_Transponder_SRate(Transponder, Transponder->srate );
	DVB_Reset();
	return(0);
};


BOOL Set_Transponder_Diseq_Scan(HWND hDlg, struct TTransponder *Transponder ){
	int Band = 0;
	char Zeile[128];
	unsigned short DiseqCParameter[5]; // für DiSEqC
	int i,j;
	unsigned char Sync;
	unsigned int sig,err;
	
	unsigned short BurstVal=0xffff;
	int BurstLoop;
	int OptionLoop;
	MSG msg;
	RECT rc;
	int Hit = 0;
	
	
	SetDlgItemText(hDlg,TEXT1,"");
	SetDlgItemText(hDlg,TEXT6,"");
	SetDlgItemText(hDlg,TEXT7,"");
	SetDlgItemText(hDlg,TEXT8,"");
	SetDlgItemText(hDlg,TEXT9,"");
	SetDlgItemText(hDlg,TEXT10,"");
	SetDlgItemText(hDlg,TEXT13,"");
	SetDlgItemText(hDlg,TEXT14,"");
	SetDlgItemText(hDlg,TEXT15,"");
	SetDlgItemText(hDlg,TEXT16,"");
	SetDlgItemText(hDlg,TEXT23,"");
	SetDlgItemText(hDlg,TEXT18,"");
	SetDlgItemText(hDlg,TEXT19,"");
	SetDlgItemText(hDlg,TEXT20,"");
	
	sprintf(Zeile,t("DiseqScan für LNB %d - DiseqC Nr. %d "),Transponder->diseqc+1,Transponder->diseqc);
	SetDlgItemText(hDlg,TEXT1,Zeile);
	
	ShutDownFilters();
	
	
	dvb_front.power=LNB[Transponder->diseqc].Power;
	Transponder->power=dvb_front.power;
	
	dvb_front.ttk=0;
	if ( LNB[Transponder->diseqc].Switch22khz == TRUE )
	{
		if ( Transponder->freq/1000 >= LNB[Transponder->diseqc].SwitchFreq )
			dvb_front.ttk= 1;
	}
	
	
	if ( Transponder->freq/1000 >= LNB[Transponder->diseqc].SwitchFreq )
		Band=1;
	
	dvb_front.volt=Transponder->volt;
	dvb_front.fec=Transponder->fec;
	dvb_front.diseqc=Transponder->diseqc;
	dvb_front.freq=Transponder->freq;
	dvb_front.srate=Transponder->srate;
	dvb_front.video_pid=0;
	dvb_front.audio_pid=0;
	dvb_front.tt_pid=0x1fff;
	dvb_front.afc=0;
	
	for ( BurstLoop=0; BurstLoop< 4 ; BurstLoop++ )
	{
		if ( BurstLoop == 0 )
			DiseqCParameter[0]=0xffff;
		else
			if ( BurstLoop == 1 )
				DiseqCParameter[0]=0x0000;
			else
				if ( BurstLoop == 2 )
					DiseqCParameter[0]=0x0001;
				else
					if ( BurstLoop == 3 )
						DiseqCParameter[0]=0x00FF;
					sprintf(Zeile,"Teste Mini-DiSEqC-Cmd BurstVal %04x ",DiseqCParameter[0]);
					Set22K(0); 
					Sleep(15);
					if ( VES_TYPE == 1 )
						SetControl((unsigned char )((dvb_front.power ? 0x20 : 0)|(dvb_front.volt ? 0x10 : 0)));
					else
						if ( VES_TYPE == 2 )
							SetControl((unsigned char )(1|(dvb_front.power ? 0x40 : 0)|(dvb_front.volt ? 0x10 : 0)));
						else
							if ( VES_TYPE == 3 )
								SetControl((unsigned char )(dvb_front.volt ? 0x11 : 0x01));
							Sleep(15);
							SAA7146_OutCom(COMTYPE_AUDIODAC, SendDiSEqC, 1, DiseqCParameter[0]); 
							Sleep(25); 
							
							for ( OptionLoop=-1; OptionLoop< 16 ; OptionLoop++ )
							{
								if ( OptionLoop != -1 )
								{
									if ( BurstLoop == 0 )
										DiseqCParameter[0]=0xffff;
									else
										if ( BurstLoop == 1 )
											DiseqCParameter[0]=0x0000;
										else
											if ( BurstLoop == 2 )
												DiseqCParameter[0]=0x0001;
											else
												if ( BurstLoop == 3 ) DiseqCParameter[0]=0x00FF;
												
												DiseqCParameter[1]=0xe0;
												DiseqCParameter[2]=0x10;
												DiseqCParameter[3]=0x38;
												DiseqCParameter[4]=DISEQC_HIGH_NIBLE+OptionLoop;
												
												sprintf(Zeile,"Teste DiSEqC-Cmd BurstVal %04x SwitchNibble %04x ",DiseqCParameter[0],DiseqCParameter[4]);
												
												SAA7146_OutCom(COMTYPE_AUDIODAC, SendDiSEqC, 6, 4,DiseqCParameter[0],DiseqCParameter[1], DiseqCParameter[2], DiseqCParameter[3], DiseqCParameter[4]);
												Sleep(Timing.SendDiSEqCDelay);
												DiseqCParameter[1]=0xe1;
												SAA7146_OutCom(COMTYPE_AUDIODAC, SendDiSEqC, 6, 4,DiseqCParameter[0],DiseqCParameter[1], DiseqCParameter[2], DiseqCParameter[3], DiseqCParameter[4]);
												Sleep(Timing.SendDiSEqCDelay);
												SAA7146_OutCom(COMTYPE_AUDIODAC, SendDiSEqC, 6, 4,DiseqCParameter[0],DiseqCParameter[1], DiseqCParameter[2], DiseqCParameter[3], DiseqCParameter[4]);
												Sleep(Timing.SendDiSEqCDelay);
												SAA7146_OutCom(COMTYPE_AUDIODAC, SendDiSEqC, 6, 4,DiseqCParameter[0],DiseqCParameter[1], DiseqCParameter[2], DiseqCParameter[3], DiseqCParameter[4]);
								}
								
								if ( Hit == 0 ) SetDlgItemText(hDlg,TEXT6,Zeile);
								else if ( Hit == 1 ) SetDlgItemText(hDlg,TEXT7,Zeile);
								else if ( Hit == 2 ) SetDlgItemText(hDlg,TEXT8,Zeile);
								else if ( Hit == 3 ) SetDlgItemText(hDlg,TEXT9,Zeile);
								else if ( Hit == 4 ) SetDlgItemText(hDlg,TEXT10,Zeile);
								else if ( Hit == 5 ) SetDlgItemText(hDlg,TEXT13,Zeile);
								else if ( Hit == 6 ) SetDlgItemText(hDlg,TEXT14,Zeile);
								else if ( Hit == 7 ) SetDlgItemText(hDlg,TEXT15,Zeile);
								else if ( Hit == 8 ) SetDlgItemText(hDlg,TEXT16,Zeile);
								else if ( Hit == 9 ) SetDlgItemText(hDlg,TEXT23,Zeile);
								else if ( Hit == 10 ) SetDlgItemText(hDlg,TEXT18,Zeile);
								else if ( Hit == 11 ) SetDlgItemText(hDlg,TEXT19,Zeile);
								else if ( Hit == 12 ) SetDlgItemText(hDlg,TEXT20,Zeile);
								
								
								if ( Hit == 12 ) SetDlgItemText(hDlg,TEXT6,"");
								else if ( Hit == 0 ) SetDlgItemText(hDlg,TEXT7,"");
								else if ( Hit == 1 ) SetDlgItemText(hDlg,TEXT8,"");
								else if ( Hit == 2 ) SetDlgItemText(hDlg,TEXT9,"");
								else if ( Hit == 3 ) SetDlgItemText(hDlg,TEXT10,"");
								else if ( Hit == 4 ) SetDlgItemText(hDlg,TEXT13,"");
								else if ( Hit == 5 ) SetDlgItemText(hDlg,TEXT14,"");
								else if ( Hit == 6 ) SetDlgItemText(hDlg,TEXT15,"");
								else if ( Hit == 7 ) SetDlgItemText(hDlg,TEXT16,"");
								else if ( Hit == 8 ) SetDlgItemText(hDlg,TEXT23,"");
								else if ( Hit == 9 ) SetDlgItemText(hDlg,TEXT18,"");
								else if ( Hit == 10 ) SetDlgItemText(hDlg,TEXT19,"");
								else if ( Hit == 11 ) SetDlgItemText(hDlg,TEXT20,"");
								
								
								Set_TV_Freq((unsigned __int64)dvb_front.freq,dvb_front.diseqc); 
								Set_Transponder_Pol(Transponder);
								SetFEC(Transponder->fec);
								Set_Transponder_SRate(Transponder, Transponder->srate );
								DVB_Reset();
								if ( Hit == 12 ) SetDlgItemText(hDlg,TEXT6,t("Syncing........"));
								else if ( Hit == 0 ) SetDlgItemText(hDlg,TEXT7,t("Syncing........"));
								else if ( Hit == 1 ) SetDlgItemText(hDlg,TEXT8,t("Syncing........"));
								else if ( Hit == 2 ) SetDlgItemText(hDlg,TEXT9,t("Syncing........"));
								else if ( Hit == 3 ) SetDlgItemText(hDlg,TEXT10,t("Syncing........"));
								else if ( Hit == 4 ) SetDlgItemText(hDlg,TEXT13,t("Syncing........"));
								else if ( Hit == 5 ) SetDlgItemText(hDlg,TEXT14,t("Syncing........"));
								else if ( Hit == 6 ) SetDlgItemText(hDlg,TEXT15,t("Syncing........"));
								else if ( Hit == 7 ) SetDlgItemText(hDlg,TEXT16,t("Syncing........"));
								else if ( Hit == 8 ) SetDlgItemText(hDlg,TEXT23,t("Syncing........"));
								else if ( Hit == 9 ) SetDlgItemText(hDlg,TEXT18,t("Syncing........"));
								else if ( Hit == 10 ) SetDlgItemText(hDlg,TEXT19,t("Syncing........"));
								else if ( Hit == 11 ) SetDlgItemText(hDlg,TEXT20,t("Syncing........"));
								for ( i=0; i < 10 ; i++ )
								{
									Get_Signal_Infos((unsigned char *)&Sync,&sig,&err);
									rc.top=400;
									rc.left=40;
									rc.bottom=500;
									rc.right=300;
									InvalidateRect(hDlg,&rc,FALSE);
									for ( j = 0; j < 10 ; j++ ) {
										if ( PeekMessage(&msg,hDlg,0,0xffffffff,PM_REMOVE) == TRUE )
										{
											SendMessage(msg.hwnd,msg.message,msg.wParam,msg.lParam);
										}
									}
									Sleep(0);
								}
								
								Get_Signal_Infos((unsigned char *)&Sync,&sig,&err);
								
								if ((Sync&0x0f)==0x0f)
								{
									sprintf(Zeile,t("Sync bei DiSEqC-Cmd BurstVal 0x%04x SwitchNibble 0x%04x "),DiseqCParameter[0],DiseqCParameter[4]);
									if ( Hit == 0 ) SetDlgItemText(hDlg,TEXT6,Zeile);
									else if ( Hit == 1 ) SetDlgItemText(hDlg,TEXT7,Zeile);
									else if ( Hit == 2 ) SetDlgItemText(hDlg,TEXT8,Zeile);
									else if ( Hit == 3 ) SetDlgItemText(hDlg,TEXT9,Zeile);
									else if ( Hit == 4 ) SetDlgItemText(hDlg,TEXT10,Zeile);
									else if ( Hit == 5 ) SetDlgItemText(hDlg,TEXT13,Zeile);
									else if ( Hit == 6 ) SetDlgItemText(hDlg,TEXT14,Zeile);
									else if ( Hit == 7 ) SetDlgItemText(hDlg,TEXT15,Zeile);
									else if ( Hit == 8 ) SetDlgItemText(hDlg,TEXT16,Zeile);
									else if ( Hit == 9 ) SetDlgItemText(hDlg,TEXT23,Zeile);
									else if ( Hit == 10 ) SetDlgItemText(hDlg,TEXT18,Zeile);
									else if ( Hit == 11 ) SetDlgItemText(hDlg,TEXT19,Zeile);
									else if ( Hit == 12 ) SetDlgItemText(hDlg,TEXT20,Zeile);
									if ( Hit == 12 ) SetDlgItemText(hDlg,TEXT6,"");
									else if ( Hit == 0 ) SetDlgItemText(hDlg,TEXT7,"");
									else if ( Hit == 1 ) SetDlgItemText(hDlg,TEXT8,"");
									else if ( Hit == 2 ) SetDlgItemText(hDlg,TEXT9,"");
									else if ( Hit == 3 ) SetDlgItemText(hDlg,TEXT10,"");
									else if ( Hit == 4 ) SetDlgItemText(hDlg,TEXT13,"");
									else if ( Hit == 5 ) SetDlgItemText(hDlg,TEXT14,"");
									else if ( Hit == 6 ) SetDlgItemText(hDlg,TEXT15,"");
									else if ( Hit == 7 ) SetDlgItemText(hDlg,TEXT16,"");
									else if ( Hit == 8 ) SetDlgItemText(hDlg,TEXT23,"");
									else if ( Hit == 9 ) SetDlgItemText(hDlg,TEXT18,"");
									else if ( Hit == 10 ) SetDlgItemText(hDlg,TEXT19,"");
									else if ( Hit == 11 ) SetDlgItemText(hDlg,TEXT20,"");
									Hit++;	
									if ( Hit >12 )  Hit=0;
								}
		}
	}
	return(0);
}


BOOL Set_Transponder(struct TTransponder *Transponder ){
	unsigned char msg[2] = { 0x0f, 0x28 }; 
	int Band=0;
	Write_Log("SAA7146 Set_Transponder");
	TPCatAnzahl=0;
	dvb_front.power=LNB[Transponder->diseqc].Power;
	
				if ( LNB[Transponder->diseqc].Switch22khz == TRUE ) {
					if ( Transponder->freq/1000 >= LNB[Transponder->diseqc].SwitchFreq )  dvb_front.ttk= 1;
					else dvb_front.ttk=0;
				};
                ShutDownFilters();
                if ( Transponder->freq/1000 >= LNB[Transponder->diseqc].SwitchFreq )  Band = 1;
                dvb_front.volt=Transponder->volt;
                dvb_front.fec=Transponder->fec;
                dvb_front.diseqc=Transponder->diseqc;
                dvb_front.freq=Transponder->freq;
                dvb_front.srate=Transponder->srate;
                dvb_front.qam=Transponder->qam;
                dvb_front.video_pid=0;
                dvb_front.audio_pid=0;
				dvb_front.tt_pid=0x1fff;
                dvb_front.afc=0;
				
				
				if ( Is_Sat_Karte() == TRUE ) { 
					SetDiSEqC(dvb_front.diseqc,Band,dvb_front.volt);
					Set22K( dvb_front.ttk);
				} ;
				
				Set_TV_Freq((unsigned __int64)dvb_front.freq,dvb_front.diseqc); 
				DVB_SET_FRONTEND(&dvb_front); 
				SetAVPID(0,0,0,0,0);
				DVB_Reset();
				GetFront(&dvb_front);
				
				Transponder->sync=dvb_front.sync;
				Transponder->nest=dvb_front.nest;
				Transponder->afc=dvb_front.afc;
				Transponder->fec=dvb_front.fec;
				Transponder->vber=dvb_front.vber;
				Transponder->agc=dvb_front.agc;
				
				if ((dvb_front.sync&0x0f)==0x0f) return(TRUE);
				return(FALSE);
}


void Set_Transponder_Freq(struct TTransponder *Transponder){
	int ttk;
	int Band;
	Write_Log("SAA7146 Set_Transponder_Freq");
	TPCatAnzahl=0;
	if ( LNB[Transponder->diseqc].Switch22khz == TRUE ) {
		if ( Transponder->freq/1000 >= LNB[Transponder->diseqc].SwitchFreq )  ttk= 1;
		else ttk=0;
	};
	
	Band = ( Transponder->freq/1000 >= LNB[Transponder->diseqc].SwitchFreq ) ? 1 : 0;
	
	
	if ( Is_Sat_Karte() == TRUE ) { 
		if ( ttk != dvb_front.ttk ) {
			dvb_front.ttk=ttk;
			dvb_front.freq=Transponder->freq;
			SetDiSEqC(dvb_front.diseqc, Band, dvb_front.volt);
			Set22K( dvb_front.ttk);
		}
	}
	Set_TV_Freq((unsigned __int64)Transponder->freq,Transponder->diseqc);
};

void Set_Transponder_Pol(struct TTransponder *Transponder){
	Write_Log("SAA7146 Set_Transponder_Pol");
	TPCatAnzahl=0;
	if ( VES_TYPE == 1 ) SetControl((unsigned char )((Transponder->power ? 0x20 : 0)|(Transponder->volt ? 0x10 : 0)));
	else if ( VES_TYPE == 2 ) SetControl((unsigned char )(1|(Transponder->power ? 0x40 : 0)|(Transponder->volt ? 0x10 : 0)));
	else if ( VES_TYPE == 3 ) SetControl((unsigned char )(Transponder->volt ? 0x11 : 0x01));
};


BOOL Set_Transponder_SRate(struct TTransponder *Transponder , unsigned int SRate){
	Write_Log("SAA7146 Set_Transponder_SRate");
	TPCatAnzahl=0;
	if ( SRate == 0 ) return(FALSE);
	if ( VES_TYPE == 1 ) {
		VES1893_SetSymbolrate(SRate, 1);
	} else if ( VES_TYPE == 2 ) {
		STV0299_SetSymbolrate(SRate);
	} else if ( VES_TYPE == 0 ) {
		VES1820_SetSymbolrate(SRate, 1);
	} else if ( VES_TYPE == 3 ) {
		TDA8083_SetSymbolrate(SRate, 1);
	};
	
	
	DVB_Reset();
	if ( Timing.DVB_Reset_Scan_Wait > 0 ) Sleep(Timing.DVB_Reset_Scan_Wait);
	GetFront(&dvb_front);
	Transponder->sync=dvb_front.sync;
	Transponder->nest=dvb_front.nest;
	Transponder->afc=dvb_front.afc;
	Transponder->fec=dvb_front.fec;
	Transponder->vber=dvb_front.vber;
	Transponder->agc=dvb_front.agc;
	if ((dvb_front.sync&0x0f)==0x0f) return(TRUE);
	return(FALSE);
}


BOOL APIENTRY CIStatusProc
(
 HWND hDlg,
 UINT message,
 UINT wParam,
 LONG lParam)
{
	char ZeileAscii[512];
	char ZeileHex[512];
	char Buffer[10];
	int i;
	switch (message) {
	case WM_INITDIALOG:
		ci_interface_Refresh = TRUE ;
		SetTimer(hDlg,100,1000,NULL);
	case WM_TIMER:
		
		if ( ci_interface_Refresh == TRUE ) {
			ci_interface_Refresh = FALSE;			
			ZeileAscii[0]=0x00;
			ZeileHex[0]=0x00;
			for ( i = 0; i < ci_interface_LastCmdLen; i++ ) {
				if ( ci_interface_LastCmd[i] >= 0x20 ) {
					sprintf(Buffer,"%c",ci_interface_LastCmd[i]);
				} else  {
					strcpy(Buffer,"*");
				};
				strcat(ZeileAscii,Buffer);
				sprintf(Buffer,"%02x ",ci_interface_LastCmd[i]);
				strcat(ZeileHex,Buffer);
			};
			
			SetDlgItemText(hDlg,TEXT2,ZeileAscii);
			SetDlgItemText(hDlg,TEXT1,ZeileHex);
			
			
			sprintf(Buffer,"%d",ci_module[0].Present);
			SetDlgItemText(hDlg,STATUS_SLOT1,Buffer);
			sprintf(Buffer,"%d",ci_module[0].Reply);
			SetDlgItemText(hDlg,REPLY_SLOT1,Buffer);
			SetDlgItemText(hDlg,NAME_SLOT1,ci_module[0].ModuleName);
			ZeileAscii[0]=0x00;
			ZeileHex[0]=0x00;
			for ( i = 0; i < ci_module[0].LastCmdLen; i++ ) {
				if ( ci_module[0].LastCmd[i] >= 0x20 ) {
					sprintf(Buffer,"%c",ci_module[0].LastCmd[i]);
				} else  {
					strcpy(Buffer,"*");
				};
				strcat(ZeileAscii,Buffer);
				sprintf(Buffer,"%02x ",ci_module[0].LastCmd[i]);
				strcat(ZeileHex,Buffer);
			};
			
			SetDlgItemText(hDlg,LAST_ASCII_SLOT1,ZeileAscii);
			SetDlgItemText(hDlg,LAST_HEX_SLOT1,ZeileHex);
			
			
			sprintf(Buffer,"%d",ci_module[1].Present);
			SetDlgItemText(hDlg,STATUS_SLOT2,Buffer);
			sprintf(Buffer,"%d",ci_module[1].Reply);
			SetDlgItemText(hDlg,REPLY_SLOT2,Buffer);
			SetDlgItemText(hDlg,NAME_SLOT2,ci_module[1].ModuleName);
			ZeileAscii[0]=0x00;
			ZeileHex[0]=0x00;
			for ( i = 0; i < ci_module[1].LastCmdLen; i++ ) {
				if ( ci_module[1].LastCmd[i] >= 0x20 ) {
					sprintf(Buffer,"%c",ci_module[1].LastCmd[i]);
				} else  {
					strcpy(Buffer,"*");
				};
				strcat(ZeileAscii,Buffer);
				sprintf(Buffer,"%02x ",ci_module[1].LastCmd[i]);
				strcat(ZeileHex,Buffer);
			};
			
			SetDlgItemText(hDlg,LAST_ASCII_SLOT2,ZeileAscii);
			SetDlgItemText(hDlg,LAST_HEX_SLOT2,ZeileHex);
			
		};
		
		break;
		
	case WM_COMMAND:
		if ((LOWORD(wParam) == IDOK ) || (LOWORD(wParam) == IDCANCEL )) {
			KillTimer(hDlg,100); 
			EndDialog(hDlg,TRUE);
		}
		break;
    }
	
	return(FALSE);
}


BOOL APIENTRY CIMenuProc
(
 HWND hDlg,
 UINT message,
 UINT wParam,
 LONG lParam)
{
	char Zeile[512];
	char OutBuffer[512];
	
	int i,j,k;
	static BOOL Wait_for_CI_Data;
	static int BaseButtonReply=-1;
	static BOOL Has_Edit;
	
	switch (message) {
	case WM_INITDIALOG:
		Wait_for_CI_Data=TRUE;
		ci_menu_update=FALSE;
		CI_Menu((short)CI_Menu_Slot);
		SetTimer(hDlg,123,500,NULL);
		Has_Edit=FALSE;
		break;
        
		
	case WM_TIMER:
		if ( CI_Menu_Slot == 0 ) EndDialog(hDlg,0);
		if ( Wait_for_CI_Data == TRUE ) {
			
			SetWindowText(GetDlgItem(hDlg,IDOK),t("Abbrechen"));
			if ( ci_menu_update == FALSE ) {
				sprintf(Zeile,"");
				for ( i = 0 ; i < 20; i++ ) {
					EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1+i),TRUE);
					ShowWindow(GetDlgItem(hDlg,IDC_BUTTON1+i),SW_HIDE);
					ShowWindow(GetDlgItem(hDlg,IDC_EDIT1+i),SW_HIDE);
					ShowWindow(GetDlgItem(hDlg,IDC_TEXT1+i),SW_SHOW);
					SetDlgItemText(hDlg,TEXT1+i,Zeile);
				};
				SetDlgItemText(hDlg,TEXT8,t("Warte auf Antwort"));
			} else {
				Has_Edit=FALSE; 
				SetWindowText(GetDlgItem(hDlg,IDOK),t("Ende"));
				sprintf(Zeile,"");
				for ( i = 0 ; i < 20; i++ ) {
					EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1+i),TRUE);
					ShowWindow(GetDlgItem(hDlg,IDC_BUTTON1+i),SW_HIDE);
					ShowWindow(GetDlgItem(hDlg,IDC_EDIT1+i),SW_HIDE);
					ShowWindow(GetDlgItem(hDlg,IDC_TEXT1+i),SW_SHOW);
					SetDlgItemText(hDlg,TEXT1+i,Zeile);
				};
				
				j=0;
				k=0;
				for ( i = 0 ; i < 20; i++ ) {
					if ( ci_menu[i].Type == 'T' ) {
						SetDlgItemText(hDlg,TEXT1+j,ci_menu[i].zeile);
						j++;
					} else if ( ci_menu[i].Type == 'S' ) { 
						
						if ( BaseButtonReply == -1 ) BaseButtonReply=j;
						
						ShowWindow(GetDlgItem(hDlg,IDC_BUTTON1+j),SW_SHOW);
						ShowWindow(GetDlgItem(hDlg,IDC_TEXT1+j),SW_HIDE);
						SetWindowText(GetDlgItem(hDlg,IDC_BUTTON1+j),ci_menu[i].zeile);
						ci_menu[i].id=j;
						j++;
					} else  if ( ci_menu[i].Type == 'E' ) { 
						Has_Edit= TRUE;
						ShowWindow(GetDlgItem(hDlg,IDC_BUTTON1+j),SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,IDC_TEXT1+j),SW_HIDE);
						ShowWindow(GetDlgItem(hDlg,IDC_EDIT1+j),SW_SHOW);
						SetDlgItemText(hDlg,IDC_EDIT1+j,ci_menu[i].zeile);
						ci_menu[i].id=j;
						j++;
					}
					
				};
				
				for ( i = 0 ; i < 20; i++ ) {
					if ( ci_menu[i].Type == 'B' ) {
						SetDlgItemText(hDlg,TEXT1+j,ci_menu[i].zeile);
						j++;
					};
				};
                
				if ( Has_Edit == TRUE ) {
					SetWindowText(GetDlgItem(hDlg,IDOK),t("Senden"));
					for ( i = 0 ; i < 20; i++ ) {
						EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1+i),FALSE);
					}
					
					
				};
				Wait_for_CI_Data =FALSE;
				ci_menu_update=FALSE;
			};
			
		};
		break;
		
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			
		case IDC_BUTTON1:
		case IDC_BUTTON2:
		case IDC_BUTTON3:
		case IDC_BUTTON4:
		case IDC_BUTTON5:
		case IDC_BUTTON6:
		case IDC_BUTTON7:
		case IDC_BUTTON8:
		case IDC_BUTTON9:
		case IDC_BUTTON10:
		case IDC_BUTTON11:
		case IDC_BUTTON12:
		case IDC_BUTTON13:
		case IDC_BUTTON14:
		case IDC_BUTTON15:
		case IDC_BUTTON16:
		case IDC_BUTTON17:
		case IDC_BUTTON18:
		case IDC_BUTTON19:
			i=(LOWORD(wParam)-IDC_BUTTON1)-BaseButtonReply;
			CI_MenuKey((unsigned char)CI_Menu_Slot,(unsigned char)(i+1));
			BaseButtonReply=-1;
			Wait_for_CI_Data=TRUE;
			ci_menu_update=FALSE;
			break;
			
		case IDBACK:
			BaseButtonReply=-1;
			Wait_for_CI_Data=TRUE;
			ci_menu_update=FALSE;
			CI_MenuKey((unsigned char)CI_Menu_Slot,0);
			break;
			
			
			
		case IDOK:
			if ( Has_Edit == TRUE ) {
				Has_Edit= FALSE;
				for ( i = 0 ; i < 20; i++ ) {
					if ( ci_menu[i].Type == 'E' ) {
						GetDlgItemText(hDlg,IDC_EDIT1+ci_menu[i].id,Zeile,512);
						j=CI_Input_Parser(Zeile,OutBuffer);
						if ( j == -1 ) {
							sprintf(Zeile,t("Fehlerhafte Eingabe"));
							MessageBox(hDlg,Zeile,"Multi-Decoder",MB_ICONSTOP|MB_OK);
							return(TRUE);
						};
						if ( j != ci_menu[i].laenge ) {
							sprintf(Zeile,t("Fehlerhafte Feldlaenge !!!\n%d Bytes sind gefordert\n%d Bytes aufgelösst"),ci_menu[i].laenge,j);
							MessageBox(hDlg,Zeile,"Multi-Decoder",MB_ICONSTOP|MB_OK);
							return(TRUE);
						};
						CI_EnterKey((unsigned char)CI_Menu_Slot,(unsigned char)j,&OutBuffer[0]);
					}
				};
				
				BaseButtonReply=-1;
				Wait_for_CI_Data=TRUE;
				ci_menu_update=FALSE;
				break;
			}
			CI_Close_Menu((short)CI_Menu_Slot);
			KillTimer(hDlg,123);
			EndDialog(hDlg,TRUE);
			break;
			
		case IDCANCEL:
			CI_Close_Menu((short)CI_Menu_Slot);
			KillTimer(hDlg,123);
			EndDialog(hDlg,TRUE);
			break;
			
		};
		
		
		break;
    }
	
	return(FALSE);
}




BOOL APIENTRY DecoderStatusProc
(
 HWND hDlg,
 UINT message,
 UINT wParam,
 LONG lParam)
{
	
	switch (message) {
	case WM_INITDIALOG:
		SetTimer(hDlg,0,500,NULL);
	case WM_TIMER:
		Get_Decoder_Status();
		SetDlgItemInt(hDlg,IDC_EDIT1,DecoderStatus.ProcessingState,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT2,DecoderStatus.CommandID,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT3,DecoderStatus.dummy1,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT4,DecoderStatus.dummy2,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT5,DecoderStatus.RateBuffFullness,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT6,DecoderStatus.dummy3,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT7,DecoderStatus.BytesDecoded,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT8,DecoderStatus.SkippedPictures,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT9,DecoderStatus.RepeatedPictures,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT10,DecoderStatus.MostRecentPTS,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT11,DecoderStatus.LastPicture,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT12,DecoderStatus.InitDone,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT13,DecoderStatus.FreezeIndex,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT14,DecoderStatus.FindIndex,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT15,DecoderStatus.DistanceI,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT16,DecoderStatus.ThresholdPTS,FALSE);
		
		SetDlgItemInt(hDlg,IDC_EDIT17,DecoderStatus.dummy4,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT18,DecoderStatus.dummy5,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT19,DecoderStatus.DisablePTSFilt,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT20,DecoderStatus.HSize,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT21,DecoderStatus.VSize,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT22,DecoderStatus.AspectRatio,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT23,DecoderStatus.FrameRate,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT24,DecoderStatus.BitRate,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT25,DecoderStatus.VBVBuffSize,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT26,DecoderStatus.ConstParamFlag,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT27,DecoderStatus.IntraQ,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT28,DecoderStatus.NonIntraQ,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT29,DecoderStatus.FrameInterval,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT30,DecoderStatus.HeaderBackup,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT31,DecoderStatus.RedHeaderFlag,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT32,DecoderStatus.SeqExtension,FALSE);
		
		SetDlgItemInt(hDlg,IDC_EDIT33,DecoderStatus.GOPTimeCode1,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT34,DecoderStatus.GOPTimeCode2,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT35,DecoderStatus.ClosedGOP,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT36,DecoderStatus.PICHeadTempRef,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT37,DecoderStatus.SegHeaderExt,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT38,DecoderStatus.ColorDesc,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT39,DecoderStatus.ColorPrim,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT40,DecoderStatus.TransferChar,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT41,DecoderStatus.MatrixCoeff,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT42,DecoderStatus.DisplayHSize,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT43,DecoderStatus.DisplayVSize,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT44,DecoderStatus.GOPHeader,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT45,DecoderStatus.TimeCodeW1,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT46,DecoderStatus.TimeCodeW2,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT47,DecoderStatus.dummy6,FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT48,DecoderStatus.dummy7,FALSE);
		
		break;
		
	case WM_COMMAND:
		
		if ((LOWORD(wParam) == IDOK ) || (LOWORD(wParam) == IDCANCEL )) {
			KillTimer(hDlg,0);
			EndDialog(hDlg,TRUE);
		}
		break;
    }
	
	return(FALSE);
}








int MakeRealValue (LPSTR Buffer){
	int j,i;
	
	j=sscanf(Buffer,"0x%x",&i);
    if ( j == 1 ) return(i);
	j=sscanf(Buffer,"0X%x",&i);
    if ( j == 1 ) return(i);
	j=sscanf(Buffer,"0x%X",&i);
    if ( j == 1 ) return(i);
	j=sscanf(Buffer,"0X%X",&i);
    if ( j == 1 ) return(i);
	j=sscanf(Buffer,"%d",&i);
    if ( j == 1 ) return(i);
	return(-1);
};


void DivMod(long Dividend, long Divisor, unsigned short *Result, unsigned short *Remainder){
	*Result = (unsigned short) (Dividend / Divisor);
	*Remainder = (unsigned short) (Dividend % Divisor);
}

// Datumsberechnung

long DateDelta = 693594;

static unsigned char DayTable1[] = {31, 28, 31, 30, 31, 30, 31, 31, 30,31, 30, 31};
static unsigned char DayTable2[] = {31, 29, 31, 30, 31, 30, 31, 31, 30,31, 30, 31};

int IsLeapYear(unsigned short Year){
	return (Year % 4 == 0) && ((Year % 100 != 0) || (Year % 400 == 0));
}


unsigned short GetDateID(unsigned short year,unsigned short month,unsigned short day){
	int start;
    unsigned short t,j,m;
	
	
	for (start=15018+365*(year-1900)+28*(month-1)+day; start <= 0xffff; start++ ) {
		DecodeDVBDate((unsigned short)(start-15018),&j,&m,&t);
		if ( ( j==year ) && ( month == m ) && ( day == t )) return((unsigned short)start);
	};
	return(0);
};


void DecodeDVBDate(unsigned short DVBDate, unsigned short *year,unsigned short *month,unsigned short *day){
	unsigned short D1 = 365;
	unsigned short D4 = D1 * 4 + 1;
	unsigned short D100 = D4 * 25 - 1;
	unsigned long D400 = D100 * 4 + 1;
	unsigned short Y, M, D, I;
	unsigned long T;
	T = DVBDate+DateDelta;
	if (T == 0)
	{
		*year  = 0;
		*month = 0;
		*day   = 0;
	}
	else
	{
		T--;
		Y = 1;
		while (T >= D400)
		{
			T-=D400;
			Y+=400;
		}
		DivMod(T, D100, &I, &D);
		if (I==4)
		{
			I--;
			D+=D100;
		}
		Y+=I * 100;
		DivMod((long)D, D4, &I, &D);
		Y+=I * 4;
		DivMod((long)D, D1, &I, &D);
		if (I==4)
		{
			I--;
			D+=(unsigned short)D1;
		}
		Y+=I;
		M = 1;
		while (1==1)
		{
			if (IsLeapYear(Y)==1)  I = DayTable2[M-1];
			else
				I = DayTable1[M-1];
			if (D < I) break;
			D-=I;
			M++;
		}
		*year  = Y;
		*month = M;
		*day   = D + 1;
	}
}

int DayOfWeek(long Date){
	return (Date+DateDelta) % 7 + 1;
}

long RealDay(unsigned char Stunde, long Day){
	short x;
	x=Stunde+TIMEZONE;
	if ( x < 0 ) Day--;
	if ( x > 23) Day++;
	return(Day);
};


void Date_To_String( unsigned short Date_Id,char *DatumsString){
	
	char tmpr[3];
	unsigned short year, month, day;
	static char WeekDays[14] = "";

	if (WeekDays[0] == 0) memcpy(WeekDays, t("SoMoDiMiDoFrSa"), 14);
	
	DecodeDVBDate((unsigned short)(Date_Id-15018), &year, &month,&day);
	tmpr[0]=WeekDays[(DayOfWeek(Date_Id-15018)-1)*2];
	tmpr[1]=WeekDays[(DayOfWeek(Date_Id-15018)-1)*2+1];
	tmpr[2]=0;
	sprintf(DatumsString,"%s %02d.%02d.%04d", tmpr, day, month, year);
};


unsigned short filter_mode = SECTION_ANY|SECTION_CONTINUOUS;

BOOL APIENTRY StreamFilterSetup(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
    char Text[255];
    int i,j,k;
    static unsigned int LastBytes=0;
    static unsigned short new_filter_mode;
	HWND DlgItem;
    const struct {
        int iddlgitem;
        unsigned short mask;
    	struct {
	    	char name[17];
		    unsigned short value;
	    } info[9];
    } filters[3] = {
        {   IDC_COMBO1,
            DATA_TYPE_MASK,
              "TS packets",       DATA_PIPING_FLAG  ,
		      "PES packets",      DATA_STREAMING_FLAG  ,
		      "Any section",      SECTION_ANY  ,
		      "EIT Sections",     SECTION_EIT  ,
		      "IPMPE Sections",   SECTION_IPMPE  ,
		      "High speed IPMPE", SECTION_HIGH_SPEED  ,
              "",                 0  ,
              "",                 0  ,
              "",                 0  ,
        }, {
            IDC_COMBO2,
            SECTION_MODE_MASK,
              "Single",           SECTION_SINGLE  ,
		      "Cycle",            SECTION_CYCLE  ,
		      "Continuous",       SECTION_CONTINUOUS  ,
              "",                 0  ,
              "",                 0  ,
              "",                 0  ,
              "",                 0  ,
              "",                 0  ,
              "",                 0  ,
        }, {
            IDC_COMBO3,
            PBUFSIZE_MASK,
    		  "Default (1 K)",    PBUFSIZE_NONE  ,
    		  "1 buffer",         PBUFSIZE_1P  ,
    		  "2 buffers",        PBUFSIZE_2P  ,
    		  "1 K",              PBUFSIZE_1K  ,
    		  "2 K",              PBUFSIZE_2K  ,
    		  "4 K",              PBUFSIZE_4K  ,
    		  "8 K",              PBUFSIZE_8K  ,
    		  "16 K",             PBUFSIZE_16K  ,
    		  "32 K",             PBUFSIZE_32K  ,
        }
    };

    switch (message) {
        case WM_INITDIALOG:
                new_filter_mode = filter_mode;
                if (StreamLog.Run == FALSE) {
    				GetCurrentDirectory(  sizeof (Text),Text);
	    	        strcat(Text,"\\Stream.log");
		    	    SetDlgItemText(hDlg,IDC_EDIT1,Text);
                }
                /* Populate the combox dialogs */
                for (j=0; j<3; j++) {
				    DlgItem = GetDlgItem(hDlg, filters[j].iddlgitem);
				    SendMessage(DlgItem, CB_RESETCONTENT, 0, 0);
                    k = 0;
				    for (i=0; i<9 && filters[j].info[i].name[0] != 0; i++) {
			            SendMessage(DlgItem, CB_ADDSTRING, 0, (LPARAM)filters[j].info[i].name);
                        if ((new_filter_mode&filters[j].mask) == filters[j].info[i].value) k = i;
				    }
				    SendMessage(DlgItem, CB_SETCURSEL, (WPARAM) k, 0);
                }
                wParam = lParam = 0;
                /* Fall through */
        case WM_USER:
            if (wParam != 0 || lParam != 0) break; /* Not from us */
			if ( StreamLog.Run == FALSE ) {
                SetWindowText(GetDlgItem(hDlg,IDOK),t("Start"));
                SetWindowText(GetDlgItem(hDlg,IDCANCEL),t("Ende"));
                EnableWindow(GetDlgItem(hDlg,IDC_EDIT1),TRUE);
    			EnableWindow(GetDlgItem(hDlg,IDC_EDIT2),TRUE);

                for (j=0; j<3; j++) {
				    DlgItem = GetDlgItem(hDlg, filters[j].iddlgitem);
                    if (filters[j].iddlgitem == IDC_COMBO2) { /* Mode */
                        EnableWindow(DlgItem, (new_filter_mode & SECTION_IPMPE) == SECTION_IPMPE ? FALSE : TRUE);
                    } else if (filters[j].iddlgitem == IDC_COMBO3) { /* Buffer size */
                        EnableWindow(DlgItem, ((new_filter_mode & DATA_TYPE_MASK) == DATA_PIPING_FLAG) ? TRUE : FALSE);
                    } else {
		                EnableWindow(DlgItem, TRUE);
                    }
                }
				for ( i=0; i<16; i++ ) {
					ShowWindow(GetDlgItem(hDlg,IDC_IS0+i),SW_HIDE);
				    EnableWindow(GetDlgItem(hDlg,IDC_BIT0+i),TRUE);
				    EnableWindow(GetDlgItem(hDlg,IDC_MASK0+i),TRUE);
				}
			    break;
            } else {
                EnableWindow(GetDlgItem(hDlg,IDC_EDIT1),FALSE);
			    EnableWindow(GetDlgItem(hDlg,IDC_EDIT2),FALSE);
                for (j=0; j<3; j++) {
				    DlgItem = GetDlgItem(hDlg, filters[j].iddlgitem);
			        EnableWindow(DlgItem,FALSE);
                }
			    SetDlgItemText(hDlg,IDC_EDIT1,StreamLog.StreamFile);
			    sprintf(Text,"%d (0x%04x)",StreamLog.Pid,StreamLog.Pid);
			    SetDlgItemText(hDlg,IDC_EDIT2,Text);
                SetWindowText(GetDlgItem(hDlg,IDOK),t("Stop"));
                SetWindowText(GetDlgItem(hDlg,IDCANCEL),t("Fenster schliessen"));
			    sprintf(Text,"%d",StreamLog.KBytesWritten);
                SetDlgItemText(hDlg,IDC_EDIT3,Text);

                for ( i=0 ; i < 16 ; i++ ) {
			      EnableWindow(GetDlgItem(hDlg,IDC_BIT0+i),FALSE);
				  EnableWindow(GetDlgItem(hDlg,IDC_MASK0+i),FALSE);
			      if ( StreamLog.ExtFilter[i]!=0 ) {
				      sprintf(Text,"0x%02x",StreamLog.ExtFilter[i]>>8);
                      SetDlgItemText(hDlg,IDC_BIT0+i,Text);
					      sprintf(Text,"0x%02x",StreamLog.ExtFilter[i]&0xff);
					      SetDlgItemText(hDlg,IDC_MASK0+i,Text);
				      } else {
					      SetDlgItemText(hDlg,IDC_BIT0+i,"");
					      SetDlgItemText(hDlg,IDC_MASK0+i,"");
				      }
			          ShowWindow(GetDlgItem(hDlg,IDC_IS0+i),SW_SHOW);
                  }
			}
			break;

        case WM_TIMER:
			if ( LastBytes != StreamLog.KBytesWritten ) {
				 LastBytes = StreamLog.KBytesWritten ;
				  sprintf(Text,"%d",StreamLog.KBytesWritten);
                  SetDlgItemText(hDlg,IDC_EDIT3,Text);
                  for ( i=0 ; i < 16 ; i++ ) {
	    			    sprintf(Text,"0x%02x",Filter[StreamLog.FilterIndex].Buffer[i]);
                        SetDlgItemText(hDlg,IDC_IS0+i,Text);
                  }

			}
			break;

        case WM_COMMAND:

            switch (LOWORD(wParam)) {
            case IDC_COMBO1 : /* Type de données */
	    		if ( HIWORD(wParam) == CBN_SELCHANGE  ) {

			        DlgItem = GetDlgItem(hDlg, IDC_COMBO1);
				    i=SendMessage(DlgItem, CB_GETCURSEL, 0, 0);
   			        DlgItem = GetDlgItem(hDlg, IDC_COMBO2);
                    if (filters[0].info[i].value & SECTION_CONTINUOUS) { /* Force the type */
				        SendMessage(DlgItem, CB_SETCURSEL, (WPARAM) 2, 0); /* to continuous */
                        EnableWindow(DlgItem, FALSE);
                        new_filter_mode &= ~(DATA_TYPE_MASK|SECTION_MODE_MASK);
                    } else {
                        EnableWindow(DlgItem, TRUE);
                        new_filter_mode &= ~DATA_TYPE_MASK;
                    }
   			        DlgItem = GetDlgItem(hDlg, IDC_COMBO3);
                    if ((filters[0].info[i].value & DATA_TYPE_MASK) == DATA_PIPING_FLAG) { /* Enable buffer size */
                        EnableWindow(DlgItem, TRUE);
                    } else {
                        EnableWindow(DlgItem, FALSE);
                    }
                    new_filter_mode |= filters[0].info[i].value;
                }
                break;
            case IDC_COMBO2 : /* Type de filtre */
	    		if ( HIWORD(wParam) == CBN_SELCHANGE  ) {
			        DlgItem = GetDlgItem(hDlg, IDC_COMBO2);
				    i=SendMessage(DlgItem, CB_GETCURSEL, 0, 0);
                    new_filter_mode &= ~SECTION_MODE_MASK;
                    new_filter_mode |= filters[1].info[i].value;
                }
                break;
            case IDC_COMBO3 : /* Taille buffer */
	    		if ( HIWORD(wParam) == CBN_SELCHANGE  ) {
			        DlgItem = GetDlgItem(hDlg, IDC_COMBO4);
				    i=SendMessage(DlgItem, CB_GETCURSEL, 0, 0);
                    new_filter_mode &= ~PBUFSIZE_MASK;
                    new_filter_mode |= filters[3].info[i].value;
                }
                break;
            case IDOK :
                if ( StreamLog.Run == FALSE ) { /* Start log */
                    ShutDownFilters();
                    GetDlgItemText(hDlg,IDC_EDIT1,StreamLog.StreamFile,sizeof(StreamLog.StreamFile));
                    GetDlgItemText(hDlg,IDC_EDIT2,Text,sizeof(Text));
                    filter_mode = new_filter_mode;
                    StreamLog.Mode = filter_mode;
                    
                    StreamLog.Pid=MakeRealValue(Text);
                    StreamLog.KBytesWritten=0;
                    for ( i=0 ; i < 16 ; i++ ) {
                        StreamLog.ExtFilter[i]=0x00;
                        GetDlgItemText(hDlg,IDC_BIT0+i,Text,sizeof(Text));
                        if (Text[0] != 0x00 ) {
                            j = MakeRealValue(Text);
                            GetDlgItemText(hDlg,IDC_MASK0+i,Text,sizeof(Text));
                            if (Text[0] != 0x00 ) {
                                k = MakeRealValue(Text);
                            } else {
                                k = 0xff;
                                sprintf(Text,"0x%02x",k);
                                SetDlgItemText(hDlg,IDC_MASK0+i,Text);
                            }
                            if ( j != -1 && k != -1 ) StreamLog.ExtFilter[i] = (j<<8)|k;
                        }
                        SetDlgItemText(hDlg,IDC_IS0+i,"");
                    }
                    
                    j=StartStreamFilter(&StreamLog);
                    if (j == -1) {
                        MessageBox(hDlg, "Unable to create filter or output file.", "Multi-Dec", MB_ICONSTOP|MB_OK);
                    } else {
                        PostMessage(hDlg,WM_USER,0,0); /* Update dialogbox */
                        SetTimer(hDlg,100,250,NULL);
                    }
                    
                } else if ( StreamLog.Run == TRUE ) { /* Stop log */
                    KillTimer(hDlg,100);
                    StopStreamFilter();
                    ShutDownFilters();
                    PostMessage(hDlg,WM_USER,0,0); /* Update dialogbox */
                }
                break;
            case IDCANCEL :
                EndDialog(hDlg,1); /* Close the dialog box even if running */
                break;
            } /* End switch (LOWORD(wParam)) */
            
            break;
    } /* End switch (message) */ 
    return (FALSE);
    UNREFERENCED_PARAMETER(lParam);
}

BOOL APIENTRY StreamFilterSetup_Old(
								HWND hDlg,
								UINT message,
								UINT wParam,
								LONG lParam)
{
    char Text[255];
    int i,j;
    static unsigned int LastBytes=0;
	
    switch (message) {
	case WM_INITDIALOG:
		GetCurrentDirectory(  sizeof (Text),Text);
		strcat(Text,"\\Stream.log");
		SetDlgItemText(hDlg,IDC_EDIT1,Text);
		CheckDlgButton(hDlg, IDC_RADIO3, TRUE);
		
	case WM_USER:
		if ( StreamLog.Run == FALSE ) {
			SetWindowText(GetDlgItem(hDlg,IDOK),t("Start"));
			SetWindowText(GetDlgItem(hDlg,IDCANCEL),t("Ende"));
			EnableWindow(GetDlgItem(hDlg,IDC_EDIT1),TRUE);
			EnableWindow(GetDlgItem(hDlg,IDC_EDIT2),TRUE);
			EnableWindow(GetDlgItem(hDlg,IDC_RADIO1),TRUE);
			EnableWindow(GetDlgItem(hDlg,IDC_RADIO2),TRUE);
			EnableWindow(GetDlgItem(hDlg,IDC_RADIO3),TRUE);
			for ( i=0; i<16; i++ ) {
				ShowWindow(GetDlgItem(hDlg,IDC_IS0+i),SW_HIDE);
				EnableWindow(GetDlgItem(hDlg,IDC_BIT0+i),TRUE);
			};
			break;
		};
		if ( StreamLog.Run == TRUE ) {
			EnableWindow(GetDlgItem(hDlg,IDC_EDIT1),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_EDIT2),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_RADIO1),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_RADIO2),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_RADIO3),FALSE);
			SetDlgItemText(hDlg,IDC_EDIT1,StreamLog.StreamFile);
			sprintf(Text,"%d (0x%04x)",StreamLog.Pid,StreamLog.Pid);
			SetDlgItemText(hDlg,IDC_EDIT2,Text);
			SetWindowText(GetDlgItem(hDlg,IDOK),"Stop");
			SetWindowText(GetDlgItem(hDlg,IDCANCEL),t("Fenster schliessen"));
			sprintf(Text,"%d",StreamLog.KBytesWritten);
			SetDlgItemText(hDlg,IDC_EDIT3,Text);
			
			for ( i=0 ; i < 16 ; i++ ) {
				EnableWindow(GetDlgItem(hDlg,IDC_BIT0+i),FALSE);
				if ( StreamLog.ExtFilter[i]&0x00ff!=0 ) {
					sprintf(Text,"0x%02x",StreamLog.ExtFilter[i]>>8);
					SetDlgItemText(hDlg,IDC_BIT0+i,Text);
				} else SetDlgItemText(hDlg,IDC_BIT0+i,"");
				ShowWindow(GetDlgItem(hDlg,IDC_IS0+i),SW_SHOW);
			};
		};
		break;
		
	case WM_TIMER:
		if ( LastBytes != StreamLog.KBytesWritten ) {
			LastBytes = StreamLog.KBytesWritten ;
			sprintf(Text,"%d",StreamLog.KBytesWritten);
			SetDlgItemText(hDlg,IDC_EDIT3,Text);
			for ( i=0 ; i < 16 ; i++ ) {
				sprintf(Text,"0x%02x",Filter[StreamLog.FilterIndex].Buffer[i]);
				SetDlgItemText(hDlg,IDC_IS0+i,Text);
			};
			
		}
		break;
		
		
		
	case WM_COMMAND:
		
		
		if (LOWORD(wParam) == IDOK ) {
			if ( StreamLog.Run == FALSE ) {
				GetDlgItemText(hDlg,IDC_EDIT1,StreamLog.StreamFile,sizeof(StreamLog.StreamFile));
				GetDlgItemText(hDlg,IDC_EDIT2,Text,sizeof(Text));
				if ( IsDlgButtonChecked(hDlg, IDC_RADIO1) == TRUE ) StreamLog.Mode=0x0004;
				else if ( IsDlgButtonChecked(hDlg, IDC_RADIO2) == TRUE ) StreamLog.Mode=0x0020;
				else if ( IsDlgButtonChecked(hDlg, IDC_RADIO3) == TRUE ) StreamLog.Mode=0x0030;
				
				StreamLog.Pid=MakeRealValue(Text);
				StreamLog.KBytesWritten=0;
                for ( i=0 ; i < 16 ; i++ ) {
					StreamLog.ExtFilter[i]=0x00;
					GetDlgItemText(hDlg,IDC_BIT0+i,Text,sizeof(Text));
					if (Text[0] != 0x00 ) {
						j = MakeRealValue(Text);
						if ( j != -1 ) StreamLog.ExtFilter[i]=(j<<8)+0xff;
					}
					
				};
				
                if ( StartStreamFilter( ) < 0 ) {
                    MessageBox(hWnd,t("Keine HW-Filter mehr frei !!!!! "),t("MultiDec"),MB_ICONSTOP|MB_OK);
                    return(TRUE);
				};
				
				//				StreamLog.Run=TRUE;
                PostMessage(hDlg,WM_USER,0,0); 
                SetTimer(hDlg,100,250,NULL);
				
			} else if ( StreamLog.Run == TRUE ) {
                KillTimer(hDlg,100);
				StopStreamFilter( );
				//				StreamLog.Run=FALSE;   
				PostMessage(hDlg,WM_USER,0,0); 
			}
		};
		if (LOWORD(wParam) == IDCANCEL ) {
			EndDialog(hDlg,1);
		}
		
		break;
    }
    return (FALSE);
	UNREFERENCED_PARAMETER(lParam);
}



BOOL APIENTRY SaaBootOptionProc
(
 HWND hDlg,
 UINT message,
 UINT wParam,
 LONG lParam)
{
	
	
	switch (message) {
		
	case WM_INITDIALOG:
		CheckDlgButton(hDlg,IDC_CHECK1, FastSwitch);
		CheckDlgButton(hDlg,IDC_CHECK2, SAVE_BOOT);
		CheckDlgButton(hDlg,IDC_CHECK3, FastBoot);
		SetDlgItemText(hDlg,IDC_EDIT1,FIRM_DIR);
		return(TRUE);
		
		
	case WM_COMMAND:
		
		if (LOWORD(wParam) == IDOK ) {
			FastSwitch=IsDlgButtonChecked(hDlg, IDC_CHECK1);
			SAVE_BOOT=IsDlgButtonChecked(hDlg, IDC_CHECK2);
			FastBoot=IsDlgButtonChecked(hDlg, IDC_CHECK3);
			GetDlgItemText(hDlg,IDC_EDIT1,FIRM_DIR,256);
			EndDialog(hDlg,TRUE);
		}
		
		if (LOWORD(wParam) == IDCANCEL ) {
			EndDialog(hDlg,TRUE);
		}
		
		break;
    }
	
	return(FALSE);
}


BOOL APIENTRY SwitchSettingProc(
 HWND hDlg,
 UINT message,
 UINT wParam,
					LONG lParam
				) {
	
	switch (message) {
		
	case WM_INITDIALOG:
		CheckDlgButton(hDlg,IDC_CHECK1, FastSwitch);
		return(TRUE);
		
		
	case WM_COMMAND:
		
		if (LOWORD(wParam) == IDOK ) {
			FastSwitch=IsDlgButtonChecked(hDlg, IDC_CHECK1);
			EndDialog(hDlg,TRUE);
		}
		
		if (LOWORD(wParam) == IDCANCEL ) {
			EndDialog(hDlg,TRUE);
		}
		
		break;
    }
	
	return(FALSE);
}



void Make_Service_Name(unsigned char ServiceTyp, char *Text){
	if ( Text == NULL ) return;
	if (( ServiceTyp >= 0 ) &&  ( ServiceTyp < 16 )) {
		strcpy(Text,t(service_type_tbl[ServiceTyp]));
	} else sprintf(Text,t("Unbekannt ID %d"),ServiceTyp);
};


void Make_Pid_Name(unsigned char stream_type, char *Text){
	
	int index = stream_type;
	static const char *stream_names[17] = {
	  "Reserved",
	  "11172 Video stream",
	  "13818 Video stream", 
	  "11172 Audio stream",
	  "13818 Audio stream", 
	  "Private sections", 
	  "PES packets with private data", 
	  "MHEG 13522", 
	  "DSMCC data", 
	  "H.222.1", 
	  "ISO 13818-6 type A", 
	  "ISO 13818-6 type B", 
	  "ISO 13818-6 type C", 
	  "ISO 13818-6 type D", 
	  "ISO 13818-6 aux.", 
	  "Reserved", 
	  "Private"
	}; 
	
	if ((stream_type>=0x0f ) && ( stream_type<=0x7F)) index = 15;
	else if (stream_type>=0x80) index = 16;
	strcpy(Text, t(stream_names[index]));
}


void Make_Crypt_Systems(int Nr, char *Text){
	unsigned char ID[MAX_CA_SYSTEMS];
	unsigned char T;
	int i,j,k;
	BOOL Drinnen;
	char Zeile[64];
	char Zeile1[64];
	
	for ( i=0; i< MAX_CA_SYSTEMS ; i++ ) ID[i]=0xff;
	j=0;
	i=0;
	while ( i < Programm[Nr].CA_Anzahl ) {
		T=(Programm[Nr].CA_System[i].CA_Typ>>8);
		Drinnen=FALSE;
		for ( k = 0; k < j ; k++ ) {
			if ( T == ID[k] ) Drinnen=TRUE;
		};
		if ( Drinnen == FALSE ) ID[j++]=T;
		i++;
	};
	
	Zeile[0]=0x00;
	
	if ( j == 0 ) {
		if ( Programm[Nr].CA_ID != 0 )  strcpy(Text,t("Verschlüsselt")); 
		else  strcpy(Text,t("Freies Programm"));
		return;
	};
	i=0;
	Programm[Nr].CA_ID=0x01;
	while ( i < j ) {
		if ( i != 0 ) strcat(Zeile,"/");
		Make_Crypt_Name(ID[i],Zeile1);
		strcat(Zeile,Zeile1);
		i++;
	};
	strcpy(Text,Zeile);
};



void Make_Crypt_Name(unsigned char CryptId, char *Text){
	char Zeile[64];
    char *s;

    switch (CryptId) {
    case ID_CRYPT_NAGRAVISION : s = "Nagravision"; break;
    case ID_CRYPT_BETACRYPT :   s = "Betacrypt";   break;
    case ID_CRYPT_POWERVU :     s = "PowerVU";     break;
    case ID_CRYPT_CRYPTOWORKS : s = "Cryptoworks"; break;
    case ID_CRYPT_VIDEOGUARD :  s = "Videoguard";  break;
    case ID_CRYPT_IRDETO :      s = "Irdeto";      break;
    case ID_CRYPT_VIACCESS :    s = "Viaccess";    break;
    case ID_CRYPT_SECA :        s = "Seca";        break;
    default : 
		sprintf(Zeile,"System %02x",CryptId);
        s = Zeile;
	}
    strcpy(Text, s);
}

int Make_Provider_Name(unsigned short CA_Typ, unsigned short EMM, unsigned int Prov_id, char *Name){
    int nchars;
    char Zeile[256];
    char SString[256];
    FILE* ProviderFile;
    char *SemmelPos;

    if ( Prov_id == 0xffffffff ) {
	      Name[0]=0x00;
	      return FALSE;
    }

    switch (CA_Typ>>8) {
    case ID_CRYPT_VIACCESS :
        sprintf(SString, "V %06X", Prov_id);
        nchars = 6;
        break;
    case ID_CRYPT_SECA :
        sprintf(SString, "S %04X", Prov_id);
        nchars = 4;
        break;
    case ID_CRYPT_NAGRAVISION :
        sprintf(SString, "N %04X", Prov_id);
        nchars = 4;
        break;
    case ID_CRYPT_BETACRYPT :
    case ID_CRYPT_IRDETO :
        sprintf(SString, "I %04X", EMM);
        nchars = 4;
        break;
    case ID_CRYPT_CRYPTOWORKS :
        sprintf(SString, "C %02X", Prov_id);
        nchars = 2;
        break;
    default :
        sprintf(Name, "(Unknown CA)");
        return FALSE;
	}

    ProviderFile = fopen("Multidec.prv","r");
    if (ProviderFile != NULL) {
        while ( fgets(Zeile, sizeof(Zeile), ProviderFile) != NULL ) {
            SemmelPos = strchr(Zeile, ';');
	        if ( SemmelPos == NULL ) SemmelPos = strchr(Zeile, '\n');
            if ( SemmelPos != NULL ) *SemmelPos = 0;

	        if ( strncmp(Zeile, SString, nchars+2) == 0 ) {
                fclose(ProviderFile);
                SemmelPos = Zeile+nchars+2;
                if (*SemmelPos == ' ' || *SemmelPos == '\t') {
                    SemmelPos++;
                    while (*SemmelPos != 0 &&
                           (*SemmelPos == ' ' || *SemmelPos == '\t')) SemmelPos++;
                    strcpy(Name, SemmelPos);
			        return TRUE;
                }
	        }
        }
        fclose(ProviderFile);
    }

    strcpy(Name, SString+2);
    strcat(Name, t(" Unbekannt"));
    return FALSE;
}

int Irq_Filters(DWORD type ,int laenge){ 
	unsigned char Buffer[1024];
	int Dll_id;
	int Dll_Offset;
    debifilter=(unsigned short)(type>>8);
	
	if (( laenge <= 0 ) || ( laenge >= 1024 )) return(0);
	if (( DEBICOPY==FALSE) || 
		( debifilter < 0 ) || 
		( debifilter >= MAXFILTER ) || 
		( debi_mem_dma.pUserAddr == NULL )) return(0);
	
	if ( Filter[debifilter].pid == 0xffff ) return(0);
	
	// IRQ_DirectCalls
	if ( Filter[debifilter].Irq_Proc != 0 ) {
		if ( Filter[debifilter].Irq_Proc == 0x01 ) Irq_Call_TDT(laenge,(unsigned char *)debi_mem_dma.pUserAddr);
		else if ( Filter[debifilter].Irq_Proc == 0x02 ) Irq_Call_Log(laenge,(unsigned char *)debi_mem_dma.pUserAddr);
					   else if ( Filter[debifilter].Irq_Proc == 0x03 ) Irq_Call_VT_Stream(laenge,(unsigned char *)debi_mem_dma.pUserAddr);
					   else if ( Filter[debifilter].Irq_Proc == 0x04 ) Irq_Work_EIT_Stream((unsigned char *)debi_mem_dma.pUserAddr,laenge);
					   else if ( Filter[debifilter].Irq_Proc >= 256  )  {

						   Dll_id = (Filter[debifilter].Irq_Proc-256)>>16;
						   Dll_Offset = (Filter[debifilter].Irq_Proc-256)&0xffff;
						   if ( Dll_id >= plugins_count ) return(0);
						   if ( Dll_Offset >= 8 ) return(0);
						   if ( Ext_Dll[Dll_id].Extern_Stream_Function[Dll_Offset] != NULL ) {
							   memcpy(&Buffer[0],(unsigned char *)debi_mem_dma.pUserAddr,laenge);
                               ARM_CLEAR_RECEIVE();
							   (Ext_Dll[Dll_id].Extern_Stream_Function[Dll_Offset])(Dll_Offset,laenge,&Buffer[0]);
                                return(1);
						   };
						   };
					   
					   
					   return(0);
				};
	
	// Event_Calls
				if (( FilterEvent[debifilter] != NULL ) &&
					( WaitForSingleObject(FilterEvent[debifilter], 0 ) != WAIT_OBJECT_0 )) {
					memcpy(Filter[debifilter].Buffer,debi_mem_dma.pUserAddr,laenge);
                    SetEvent(FilterEvent[debifilter]);
					return(0);
				}
				return(0);
};

int Irq_Load_BMP( int Laenge ){
	if ( Laenge > OSDBitMap.size ) Laenge = OSDBitMap.size;
	memcpy(BitMap_mem_dma.pUserAddr,&OSDBitMap.DataPtr[OSDBitMap.Offset],Laenge);
	OSDBitMap.Offset+=Laenge;
	OSDBitMap.size-=Laenge;
	return(Laenge);
};

void Irq_Call_TDT( int Laenge, unsigned char *Daten){
	struct tdt_t {
		unsigned char table_id;
		unsigned char BitField1;
        // u_char	section_length_hi		: 4;
		// u_char					: 3;
		// u_char	section_syntax_indicator	: 1;
		
		unsigned char  section_length_lo;
		unsigned char  Date_1;
		unsigned char  Date_2;
		unsigned char  Time_h;
		unsigned char  Time_m;
		unsigned char  Time_s;
	};
	struct tdt_t *tdt;
#define TDT_SIZE 8

	struct tot_t {
		struct tdt_t tdt;
		// u_char	descr_loop_len_hi		: 4;
		// u_char					: 4;
		unsigned char BitField2;
		unsigned char descr_loop_len_lo;
		unsigned char descr_tag; // should be DESCR_LOCAL_TIME_OFF = 0x58
		unsigned char descr_len; // multiple of 13
		struct local_time_offset_t {
			unsigned char country_code[3];
			// u_char local_time_offset_pol :1;
			// u_char				  :1;
			// u_char country_region_id	  :6;
			unsigned char country_region_id;
			unsigned char local_time_offset_hi;
			unsigned char local_time_offset_lo;
			unsigned char change_Date_1;
			unsigned char change_Date_2;
			unsigned char change_Time_h;
			unsigned char change_Time_m;
			unsigned char change_Time_s;
			unsigned char next_time_offset_hi;
			unsigned char next_time_offset_lo;
		} descr[1]; // Repeated until descr_len reached
		// u_char CRC_32[4]; // and followed by a CRC_32 check.
	};
	struct tot_t *tot;
#define TOT_SIZE 25

	
	int sec_len;
	
	switch (Daten[0]) {
	case TID_TDT:	// Time Date Section 
	// Funktion wird nur zur Zeitsync verwendet
	tdt=(struct tdt_t *)Daten;
		if (tdt->BitField1 & 0x80) return;
		sec_len = ((tdt->BitField1&0x0f)<<8)+tdt->section_length_lo;
		if ( sec_len < 5 || sec_len >= 1024) return;
		TDT.Date_Id=(tdt->Date_1 << 8) + tdt->Date_2;
		TDT.Hour=((tdt->Time_h>>4)*10)+(tdt->Time_h&0x0f);
		TDT.Min=((tdt->Time_m>>4)*10)+(tdt->Time_m&0x0f);
		TDT.Sec=((tdt->Time_s>>4)*10)+(tdt->Time_s&0x0f);
		TDT.RefreshHour=TDT.Hour;
		TDT.RefreshMin=TDT.Min;
		TDT.RefreshSec=TDT.Sec;
		break;
	case TID_TOT:	// Time Offset Section 
		tot=(struct tot_t *)Daten;
		tdt = &(tot->tdt);
		if (tdt->BitField1 & 0x80) return;
	sec_len = ((tdt->BitField1&0x0f)<<8)+tdt->section_length_lo;
		if ( sec_len < 5 || sec_len >= 1024) return;
	TDT.Date_Id=(tdt->Date_1 << 8) + tdt->Date_2;
	TDT.Hour=((tdt->Time_h>>4)*10)+(tdt->Time_h&0x0f);
	TDT.Min=((tdt->Time_m>>4)*10)+(tdt->Time_m&0x0f);
	TDT.Sec=((tdt->Time_s>>4)*10)+(tdt->Time_s&0x0f);
	TDT.RefreshHour=TDT.Hour;
	TDT.RefreshMin=TDT.Min;
	TDT.RefreshSec=TDT.Sec;
		// Someone can parse the time offset descriptor...
		break;
	}
	return;
};


void Irq_Call_Log(int Laenge, unsigned char *DataPtr){
	unsigned char val, mask;
	unsigned int KBWritten;
	BOOL SaveBuffer;
	int i;
	
	
	if (( StreamLog.Run == FALSE ) || ( DataPtr == NULL )) {
		(void)DelBitFilter(StreamLog.FilterIndex);
		if ( StreamLog.StreamFileFd != NULL ) fclose(StreamLog.StreamFileFd);
		StreamLog.StreamFileFd=NULL;
		StreamLog.Run=FALSE;
		return;
	}
	
	SaveBuffer=TRUE;
	if ( (StreamLog.Mode & (~PBUFSIZE_MASK)) != (SECTION_CONTINUOUS|SECTION_ANY) ) {
		i=0;
		while (( i < 16 ) && ( SaveBuffer == TRUE ) && ( i < Laenge)) {
		    mask=StreamLog.ExtFilter[i]&0xff;
			if ( mask!=0 ) {
				  val=StreamLog.ExtFilter[i]>>8;
                  if ( (DataPtr[i]&mask) != (val&mask) ) SaveBuffer=FALSE;
			}
          i++;
		}
	} else {
        if ((DataPtr[1] & 0x80) != 0) { // Section syntax == 1 ==> there is a CRC
			if (ccitt32_checkcrc(DataPtr, (unsigned int)Laenge) != 0) {
				SaveBuffer=FALSE;
			}
		}
	}
	
    if ( SaveBuffer==TRUE ) {
		StreamLog.Bytes+=Laenge;
		memcpy(Filter[StreamLog.FilterIndex].Buffer,DataPtr,16);
		if(fwrite(DataPtr, sizeof(unsigned char), Laenge, StreamLog.StreamFileFd) != (size_t)Laenge) {
			(void)DelBitFilter(StreamLog.FilterIndex);
			if ( StreamLog.StreamFileFd != NULL ) fclose(StreamLog.StreamFileFd);
			StreamLog.StreamFileFd=NULL;
			StreamLog.Run=FALSE;
			return;
		}
		KBWritten=StreamLog.Bytes/1024;
		StreamLog.Bytes-=(KBWritten*1024);
		StreamLog.KBytesWritten+=KBWritten;
	};
};


void Irq_Call_VT_Stream(int Laenge , unsigned char *Daten){
	struct TextPacket {
		unsigned char PacketId;
		unsigned char Laenge;
        unsigned char TextID1;
        unsigned char TextID2;
        unsigned char Mag;
        unsigned char Pack;
		unsigned char Daten[40];
	};
#define TEXTPACKETSIZE 46
	
	BOOL BlockDone;
	int i,Pos;
	int BlockLaenge;
	struct TextPacket *PacketPtr;
	unsigned char *DataPtr;
	unsigned char mag,pack;
	unsigned short s1,s2;
	unsigned char LinePuffer[50];
	
	DataPtr=(unsigned char *)Daten;
	Pos=0;
	while ( Pos + TEXTPACKETSIZE <= Laenge ) {
		PacketPtr=( struct TextPacket *)DataPtr;
		BlockLaenge=PacketPtr->Laenge;
		if (BlockLaenge == 0 || PacketPtr->PacketId == 0xff) {
			BlockLaenge=0x2c;
			DataPtr += BlockLaenge+2;
			Pos += BlockLaenge+2;
			continue;
		}
		
		BlockDone=FALSE;
		if ((PacketPtr->PacketId == 0x02 || PacketPtr->PacketId == 0x03) &&
			BlockLaenge == 0x2c) {
			
			s1 = unhamtab[invtab[PacketPtr->Pack]];
			s2 = unhamtab[invtab[PacketPtr->Mag]];
			mag  = s2&7;
			pack = (s1<<1) | (s2>>3);
			
			if (DVB_WINBIS && pack==0x1f && mag==3) {
				if ( winbisfp == NULL ) Init_Winbis();
				s1 = unhamtab[invtab[PacketPtr->Daten[0]]];
				s2 = unhamtab[invtab[PacketPtr->Daten[1]]];
				if (((s2<<4)|s1)==0x14) {
					for (i=0; i<40; i++)
						LinePuffer[i]=invtab[PacketPtr->Daten[i]];
					winbis_decoder(&LinePuffer[0]);
					BlockDone=TRUE;
				}
			}
			
			if (DVB_VT &&  !BlockDone) {
				if (pack >= 0 && pack <= 24) {
					if (pack == 0) {
						Work_Text(&SAA_VT_Buffer[0]);
						if ((unhamtab[invtab[PacketPtr->Daten[3]]]&0x8) == 0x08 ||
							(unhamtab[invtab[PacketPtr->Daten[6]]]&0x2) == 0x02)
							memset(SAA_VT_Buffer,0,1008);	 // erase/Update Page
						for (i=0; i<8; i++)
							SAA_VT_Buffer[1000+i]=unhamtab[invtab[PacketPtr->Daten[i]]];
						SAA_VT_Buffer[1000] |= mag<<4;
					}
					for (i=0; i<40 ;i++)
						SAA_VT_Buffer[pack*40+i]=invtab[PacketPtr->Daten[i]];
				}
			}
		}
		
		Pos += BlockLaenge+2;
		DataPtr += BlockLaenge+2;
	}
}




void Free_EIT( void ){
	
	int i,j;
	int OK=0;
	for (i=0;i < Current_Eit_Index_Size; i++ ) {
		
		if ( EIT[i].Anzahl>0) {
			
			for (j=0;j < EIT[i].Anzahl; j++ ) {
				if ( EIT[i].EIT_Infos[j].Header != NULL ) {
					if ( HeapFree(MultiDec_Heap,0,EIT[i].EIT_Infos[j].Header) == FALSE ) OK=1;
				}
				if ( EIT[i].EIT_Infos[j].Text != NULL ) {
					if ( HeapFree(MultiDec_Heap,0,EIT[i].EIT_Infos[j].Text) == FALSE ) OK=1;
				}
				if ( EIT[i].EIT_Infos[j].Extra_4A != NULL ) {
					if ( HeapFree(MultiDec_Heap,0,EIT[i].EIT_Infos[j].Extra_4A) == FALSE ) OK=1;
				}
				if ( EIT[i].EIT_Infos[j].Extra_50 != NULL ) {
					if ( HeapFree(MultiDec_Heap,0,EIT[i].EIT_Infos[j].Extra_50) == FALSE ) OK=1;
				}
				if ( EIT[i].EIT_Infos[j].Extra_54 != NULL ) {
					if ( HeapFree(MultiDec_Heap,0,EIT[i].EIT_Infos[j].Extra_54) == FALSE ) OK=1;
				}
				if ( EIT[i].EIT_Infos[j].Extra_55 != NULL ) {
					if ( HeapFree(MultiDec_Heap,0,EIT[i].EIT_Infos[j].Extra_55) == FALSE ) OK=1;
				}
				if ( EIT[i].EIT_Infos[j].Extra_5F != NULL ) {
					if ( HeapFree(MultiDec_Heap,0,EIT[i].EIT_Infos[j].Extra_5F) == FALSE ) OK=1;
				}
				if ( EIT[i].EIT_Infos[j].Extra_F0 != NULL ) {
					if ( HeapFree(MultiDec_Heap,0,EIT[i].EIT_Infos[j].Extra_F0) == FALSE ) OK=1;
				}
				if ( EIT[i].EIT_Infos[j].Extra_81 != NULL ) {
					if ( HeapFree(MultiDec_Heap,0,EIT[i].EIT_Infos[j].Extra_81) == FALSE ) OK=1;
				}
				
				EIT[i].EIT_Infos[j].Header=NULL;
				EIT[i].EIT_Infos[j].Text = NULL;
				EIT[i].EIT_Infos[j].Extra_4A=NULL;
				EIT[i].EIT_Infos[j].Extra_50=NULL;
				EIT[i].EIT_Infos[j].Extra_54=NULL;
				EIT[i].EIT_Infos[j].Extra_55=NULL;
				EIT[i].EIT_Infos[j].Extra_5F=NULL;
				EIT[i].EIT_Infos[j].Extra_F0=NULL;
				EIT[i].EIT_Infos[j].Extra_81=NULL;
				
			}
			if ( HeapFree(MultiDec_Heap,0,EIT[i].EIT_Infos) == FALSE ) OK=1;
		};
	};
};

BOOL APIENTRY ByteFilterScanProc(
								 HWND hDlg,
								 UINT message,
								 UINT wParam,
								 LONG lParam)
{
	char zeile[140];
	char Zeile[140];
	char zeile1[140];
	static BOOL STOP=FALSE;
	
	static unsigned short NextPid=0x0000;
	static unsigned short NextTid=0x0000;
	
	static unsigned short Retry[MAXFILTER];
	
	LOGFONT Mfont = {9,5,0,0,0,0,0,0,0,0,0,0,0,"Fixedsys"};   
    HFONT currFont;
    int FilterID;
    static int MaxRetry=16;
	
	
    switch (message) {
	case WM_INITDIALOG:
		
		currFont = CreateFontIndirect(&Mfont);
		SendMessage(GetDlgItem(hDlg,IDC_LIST1),WM_SETFONT,(WPARAM)currFont,TRUE);
		SendMessage (GetDlgItem(hDlg,IDC_LIST1), LB_RESETCONTENT, 0, 0);
		SetDlgItemText(hDlg,IDC_EDIT3,"4");
		return(TRUE);
		
		
	case WM_TIMER:
		FilterID = LOWORD(wParam);
		KillTimer(hDlg,FilterID);
		if ( STOP ) return(TRUE);
		Retry[FilterID]++;
		
		if ( WaitForSingleObject(FilterEvent[FilterID], 0 ) == WAIT_OBJECT_0 ) {
			Retry[FilterID]=0xff;
			ResetEvent(FilterEvent[FilterID]);
			
			if ( Filter[FilterID].pid==PID_PAT )      sprintf(zeile,"PAT       (0x%02x)",Filter[FilterID].pid);
			else if ( Filter[FilterID].pid==PID_CAT ) sprintf(zeile,"CAT       (0x%02x)",Filter[FilterID].pid);
			else if ( Filter[FilterID].pid==PID_NIT ) sprintf(zeile,"NIT       (0x%02x)",Filter[FilterID].pid);
			else if ( Filter[FilterID].pid==PID_SDT ) sprintf(zeile,"SDT/BAT   (0x%02x)",Filter[FilterID].pid);
			else if ( Filter[FilterID].pid==PID_EIT ) sprintf(zeile,"EIT       (0x%02x)",Filter[FilterID].pid);
			else if ( Filter[FilterID].pid==PID_RST ) sprintf(zeile,"RST       (0x%02x)",Filter[FilterID].pid);
			else if ( Filter[FilterID].pid==PID_TDT ) sprintf(zeile,"TDT/TOT/ST(0x%02x)",Filter[FilterID].pid);
			else								      sprintf(zeile,t("Unbekannt (0x%02x)"),Filter[FilterID].pid);
			
			
			if ( Filter[FilterID].Section == TID_PAT )          sprintf(zeile1,"TID_PAT     (0x%02x)",Filter[FilterID].Section);
			else if ( Filter[FilterID].Section == TID_CAT )     sprintf(zeile1,"TID_CAT     (0x%02x)",Filter[FilterID].Section);
			else if ( Filter[FilterID].Section == TID_PMT )     sprintf(zeile1,"TID_PMT     (0x%02x)",Filter[FilterID].Section);
			else if ( Filter[FilterID].Section == TID_NIT_ACT ) sprintf(zeile1,"TID_NIT_ACT (0x%02x)",Filter[FilterID].Section);
			else if ( Filter[FilterID].Section == TID_NIT_OTH ) sprintf(zeile1,"TID_NIT_OTH (0x%02x)",Filter[FilterID].Section);
			else if ( Filter[FilterID].Section == TID_SDT_ACT ) sprintf(zeile1,"TID_SDT_ACT (0x%02x)",Filter[FilterID].Section);
			else if ( Filter[FilterID].Section == TID_SDT_OTH ) sprintf(zeile1,"TID_SDT_OTH (0x%02x)",Filter[FilterID].Section);
			else if ( Filter[FilterID].Section == TID_BAT )     sprintf(zeile1,"TID_BAT     (0x%02x)",Filter[FilterID].Section);
			else if ( Filter[FilterID].Section == TID_EIT_ACT ) sprintf(zeile1,"TID_EIT_ACT (0x%02x)",Filter[FilterID].Section);
			else if ( Filter[FilterID].Section == TID_EIT_OTH ) sprintf(zeile1,"TID_EIT_OTH (0x%02x)",Filter[FilterID].Section);
			else if ( Filter[FilterID].Section >= TID_EIT_SCH_FIRST &&
                      Filter[FilterID].Section <= TID_EIT_SCH_LAST) sprintf(zeile1,"TID_EIT_SCH (0x%02x)",Filter[FilterID].Section);
			else if ( Filter[FilterID].Section == TID_TDT )     sprintf(zeile1,"TID_TDT     (0x%02x)",Filter[FilterID].Section);
			else if ( Filter[FilterID].Section == TID_RST )     sprintf(zeile1,"TID_RST     (0x%02x)",Filter[FilterID].Section);
			else if ( Filter[FilterID].Section == TID_ST )      sprintf(zeile1,"TID_ST      (0x%02x)",Filter[FilterID].Section);
			else if ( Filter[FilterID].Section == TID_TOT )     sprintf(zeile1,"TID_TOT     (0x%02x)",Filter[FilterID].Section);
			else  sprintf(zeile1,t("Unbekannt   (0x%02x)"),Filter[FilterID].Section);
			sprintf(Zeile,"%s %s",zeile,zeile1);
			SendMessage(GetDlgItem(hDlg,IDC_LIST1),LB_INSERTSTRING,0,(LPARAM)Zeile);
			sprintf(zeile,"SAA7146 ByteFilterScan %s",Zeile);
			Write_Log(zeile);
		};
		
		if ( Retry[FilterID] > MaxRetry ) {
			DelBitFilter(FilterID);
			if ( NextPid < 0x1fff ) {
				SetDlgItemInt(hDlg,IDC_EDIT1,NextPid,FALSE);
				SetDlgItemInt(hDlg,IDC_EDIT2,NextTid,FALSE);
				Retry[FilterID]=0;
				FilterID=SetFilter(NextPid, (unsigned short)((NextTid<<8)|0x00ff), SECTION_CONTINUOUS,"Bit-Filter Scan");             
				if ( FilterID > 0 ) {
					NextTid++;
					if ( NextTid > 0xff ) {
						NextPid++;
						NextTid=0;
					};
					SetTimer(hDlg,FilterID,250,NULL);
				};
			}; 
		} else {
			SetTimer(hDlg,FilterID,250,NULL);
		}
		break;
		
	case WM_COMMAND:
		
		
		if (LOWORD(wParam) == IDSTART ) {
			STOP = FALSE;
			MaxRetry=GetDlgItemInt(hDlg,IDC_EDIT3,NULL,FALSE);
            MaxRetry=MaxRetry*4;
			NextPid=0x00;
			NextTid=0;
            FilterID=1;
			while ( FilterID >= 0 ) {
                SetDlgItemInt(hDlg,IDC_EDIT1,NextPid,FALSE);
                SetDlgItemInt(hDlg,IDC_EDIT2,NextTid,FALSE);
				FilterID=SetFilter((unsigned short)NextPid, (unsigned short)((NextTid<<8)|0x00ff), SECTION_CONTINUOUS,"Bit-Filter Scan");
                if ( FilterID >= 0 ) {
					Retry[FilterID]=0;
					NextTid++;
					SetTimer(hDlg,FilterID,250,NULL);
				};
			};
			
		};
		
		if ( LOWORD(wParam) == IDCANCEL) {
			STOP=TRUE;
			ShutDownFiltersByName("Bit-Filter Scan");
			EndDialog(hDlg,TRUE);
		}
		
		break;
    }
    return (FALSE);
	UNREFERENCED_PARAMETER(lParam);
}

BOOL APIENTRY StreamFilterScanProc(
								   HWND hDlg,
								   UINT message,
								   UINT wParam,
								   LONG lParam)
{
	char zeile[140];
	char Zeile[140];
	static BOOL STOP=FALSE;
	
	static unsigned short NextPid=0x0000;
	
	static unsigned short Retry[MAXFILTER];
	
	LOGFONT Mfont = {9,5,0,0,0,0,0,0,0,0,0,0,0,"Fixedsys"};   
    HFONT currFont;
    int FilterID;
	
    switch (message) {
	case WM_INITDIALOG:
		
		currFont = CreateFontIndirect(&Mfont);
		SendMessage(GetDlgItem(hDlg,IDC_LIST1),WM_SETFONT,(WPARAM)currFont,TRUE);
		SendMessage (GetDlgItem(hDlg,IDC_LIST1), LB_RESETCONTENT, 0, 0);
		return(TRUE);
		
		
	case WM_TIMER:
		FilterID = LOWORD(wParam);
		KillTimer(hDlg,FilterID);
		if ( STOP ) return(TRUE);
		Retry[FilterID]++;
		
		if ( WaitForSingleObject(FilterEvent[FilterID], 0 ) == WAIT_OBJECT_0 ) {
			Retry[FilterID]=0xff;
			ResetEvent(FilterEvent[FilterID]);
			sprintf(zeile,"Pid %d   ( 0x%04x ) ",Filter[FilterID].pid,Filter[FilterID].pid);
			SendMessage(GetDlgItem(hDlg,IDC_LIST1),LB_INSERTSTRING,0,(LPARAM)zeile);
			sprintf(Zeile,"SAA7146 PIDFilterScan %s",zeile);
			Write_Log(Zeile);
		};
		
		if ( Retry[FilterID] > 10 ) {
			DelBitFilter(FilterID);
			if ( NextPid < 0x1fff ) {
				SetDlgItemInt(hDlg,IDC_EDIT1,NextPid,FALSE);
				Retry[FilterID]=0;
				FilterID=SetStreamFilter(NextPid,"Pipe/Stream Scan");    
				if ( FilterID >= 0 ) {
					NextPid++;
					SetTimer(hDlg,FilterID,500,NULL);
				};
				
			};
			
		} else {
			SetTimer(hDlg,FilterID,500,NULL);
		}
		break;
		
	case WM_COMMAND:
		
		
		if (LOWORD(wParam) == IDSTART ) {
			STOP = FALSE;
			NextPid=0x0;
			FilterID=1;
			while ( FilterID >= 0 ) {
                SetDlgItemInt(hDlg,IDC_EDIT1,NextPid,FALSE);
				FilterID=SetStreamFilter((unsigned short)NextPid,"Pipe/Stream Scan");
                if ( FilterID >= 0 ) { 
					NextPid++;
					Retry[FilterID]=0;
					SetTimer(hDlg,FilterID,500,NULL);
				};
			};
			
		};
		
		if ( LOWORD(wParam) == IDCANCEL) {
			STOP=TRUE;
			ShutDownFiltersByName("Pipe/Stream Scan");
			EndDialog(hDlg,TRUE);
		}
		
		break;
    }
    return (FALSE);
	UNREFERENCED_PARAMETER(lParam);
}
