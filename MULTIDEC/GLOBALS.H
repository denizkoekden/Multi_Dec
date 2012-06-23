/*********************************************************************************
 *                                                                               *
 * Struct-Definitionen für alle                                                  *
 *                                                                               *
 * Copyright (C) 2000 Espresso                                                   *
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


#define MAX_CA_SYSTEMS 32
#define MAX_PID_IDS  32

struct PIDFilters {
         char FilterName[13];
		 unsigned char FilterId;
		 unsigned short PID;
};


struct TCA_System {
		 unsigned short CA_Typ;
		 unsigned short ECM;
		 unsigned short EMM;
		 unsigned short _align;
		 unsigned int   Provider_Id;
};


typedef struct TProgramm
{
/*0x000*/    char              Name[30];
/*0x01e*/    char              Anbieter[30];
/*0x03c*/    char              Land[30];
/*0x05a*/    unsigned short    orig_nw_id;
/*0x05c*/    unsigned long     freq;
/*0x060*/    unsigned char     Typ;
/*0x061*/    unsigned char     volt;              /* 14/18V (V=0/H=1) */
/*0x062*/    unsigned char     afc;
/*0x063*/    unsigned char     diseqc;            /* Diseqc input select */
/*0x064*/    unsigned int      srate;         
/*0x068*/    unsigned char     qam;               
/*0x069*/    unsigned char     fec;   
/*0x06a*/    unsigned char     norm;
/*0x06b*/    unsigned char     _align1;
/*0x06c*/    unsigned short    tp_id;        
/*0x06e*/    unsigned short    Video_pid;        
/*0x070*/    unsigned short    Audio_pid;
/*0x072*/    unsigned short    TeleText_pid;          /* Teletext PID */
/*0x074*/    unsigned short    PMT_pid;
/*0x076*/    unsigned short    PCR_pid;
/*0x078*/    unsigned short    ECM_PID;
/*0x07a*/    unsigned short    SID_pid;
/*0x07c*/    unsigned short    AC3_pid;
/*0x07e*/    unsigned char     TVType; //  == 00 PAL ; 11 == NTSC    
/*0x07f*/    unsigned char     ServiceTyp;
/*0x080*/    unsigned char     CA_ID;
/*0x081*/    unsigned char     _align2;
/*0x082*/    unsigned short    Temp_Audio;
/*0x084*/    unsigned short    Filteranzahl;
/*0x086*/    struct PIDFilters Filters[MAX_PID_IDS];
/*0x286*/    unsigned short    CA_Anzahl;
/*0x288*/    struct TCA_System CA_System[MAX_CA_SYSTEMS];
/*0x408*/    char              CA_Land[5];
/*0x40d*/    unsigned char     Merker;
/*0x40e*/    unsigned short    Link_TP;
/*0x410*/    unsigned short    Link_SID;
/*0x412*/    unsigned char     Dynamisch;
/*0x413*/    char              Extern_Buffer[16];
/*0x423*/    unsigned char     _align3;
/*0x424*/};

struct DVBTunerParameter 
{
	char name[64];
	/* all frequencies in units of Hz */
	unsigned long min;     /* minimum frequency */  
	unsigned long max;     /* maximum frequency */
	unsigned long res;     /* frequency resolution in Hz */
	unsigned long step;    /* stepsize in Hz (units of setfrequency ioctl) */
	unsigned long IFPCoff; /* frequency offset */
	unsigned long thresh1; /* frequency Range for UHF,VHF-L, VHF_H */   
	unsigned long thresh2;  
	unsigned char VHF_L;   
	unsigned char VHF_H;
	unsigned char UHF;
	unsigned char config; 
	unsigned char mode; 
	unsigned char I2C;
    BOOL CableTuner;
};


typedef struct TLNB {
	     BOOL Use;
	     BOOL Power;
	     BOOL PowerReset;
		 BOOL Switch22khz;
         unsigned int MinFreq;
         unsigned int MaxFreq;
         unsigned int LofLow;
         unsigned int LofHigh;
		 unsigned int SwitchFreq;
		 unsigned char MiniDiseqc;
	     unsigned char west_east; // 'E' = East, 'W' = West, other = unknown sat pos.
		 short BurstVal;
		 unsigned char PosVal;
		 unsigned char OptVal;
    	 unsigned short orbit;   // orbit position in tenths of degrees
};


struct TTransponder {
		int ttk;
        int power;
		int diseqc;
        unsigned int freq;
        int volt;
        int qam;
        unsigned int srate;
        int fec;
    	int sync;              /* sync from decoder */
    	int afc;               /* frequency offset in Hz */
	    unsigned short agc;             /* gain */
	    unsigned short nest;            /* noise estimation */
        unsigned int vber;            /* viterbi bit error rate */
        unsigned short ts_id;
    	unsigned int sig;               
    	unsigned int err;               
	};


struct bitfilter {
   	    unsigned short pid;
        unsigned short data[16];
        unsigned short mode;
        unsigned short handle;
        unsigned short flags;
};

struct TFilter {
	       unsigned short pid;
           unsigned char Section;
		   unsigned char Buffer[6144];
		   unsigned char Name[128];
		   int Irq_Proc;
		   unsigned int ExternCall;
};

/***********   CAT **********/
struct TTPCat {
    unsigned char  TAG;
    unsigned char  DesLen;
	unsigned short CA_ID;
	unsigned short EMM;
	unsigned short BufferLen;
    unsigned char  Buffer[64];
};

/********** PAT **********/

struct tPAT {
	unsigned short ts_id;
	unsigned char  version;
    int ProgAnzahl;
};

/********** PMT **********/



struct dvb_pmt_struct {
	unsigned short prog_nr;
	unsigned short version;
	unsigned short pcr_pid;
	unsigned short StreamAnzahl;
    unsigned short info_length;  
};	


/********** NIT **********/
struct tNIT {
/*0x00*/	unsigned char  DiseqNr;
/*0x01*/    unsigned char  _align1;
/*0x02*/	unsigned short ts_id;			// Transport Stream ID
/*0x04*/	unsigned short orig_nw_id;		// Original Network ID
/*0x06*/	char           Name[32];
/*0x26*/    unsigned short _align2;
/*0x28*/	unsigned int   freq;
/*0x2c*/	unsigned int   orbit;
/*0x30*/	unsigned char  west_east;
/*0x31*/	unsigned char  polar;
/*0x32*/    unsigned short _align3;
/*0x34*/	unsigned int   srate;
/*0x38*/	unsigned char  fec;
/*0x39*/    unsigned char  qam;
/*0x3a*/    unsigned short _align4;
/*0x3c*/};

/********** TDT **********/
struct TDT_t {
	 unsigned short Date_Id;
	 unsigned char Hour;
	 unsigned char Min;
	 unsigned char Sec;
	 unsigned char RefreshHour;
	 unsigned char RefreshMin;
	 unsigned char RefreshSec;
};


/********** EIT **********/

struct FIX_EIT_t {
	 unsigned short SID_Id;
	 unsigned short ts_id;
	 unsigned short Event_Id;
	 unsigned short Date_Id;
     unsigned char Current_Next;
	 unsigned char Start_Zeit_h;
	 unsigned char Start_Zeit_m;
	 unsigned char Start_Zeit_s;
	 unsigned char Dauer_h;
	 unsigned char Dauer_m;
	 unsigned char Dauer_s;
	 unsigned char Running_Status;
	 int EIT_Extra_4A_Laenge;
	 unsigned char EIT_Extra_4A[1024];
	 int EIT_Extra_50_Laenge;
	 unsigned char EIT_Extra_50[1024];
	 int EIT_Extra_54_Laenge;
	 unsigned char EIT_Extra_54[1024];
	 int EIT_Extra_55_Laenge;
	 unsigned char EIT_Extra_55[1024];
	 int EIT_Extra_5F_Laenge;
	 unsigned char EIT_Extra_5F[1024];
	 int EIT_Extra_F0_Laenge;
	 unsigned char EIT_Extra_F0[1024];
	 int EIT_Extra_81_Laenge;
	 unsigned char EIT_Extra_81[1024];

     unsigned char HeaderLang[3];
     int EIT_Header_Laenge;
	 char Header[256];
     unsigned char TextLang[3];
     int EIT_Text_Laenge;
	 char Text[2048];
     unsigned short virtual_eventid;
     unsigned short virtual_sid;

};


struct T_EIT_Infos {
	 unsigned short SID_Id;
	 unsigned short ts_id;
	 unsigned short Event_Id;
	 unsigned short Date_Id;
     unsigned char Current_Next;
	 unsigned char Start_Zeit_h;
	 unsigned char Start_Zeit_m;
	 unsigned char Start_Zeit_s;
	 unsigned char Dauer_h;
	 unsigned char Dauer_m;
	 unsigned char Dauer_s;
	 unsigned char Running_Status;
     unsigned char UpdateCount;
     unsigned short virtual_eventid;
     unsigned short virtual_sid;
     unsigned char HeaderLang[3];
	 unsigned short HeaderSize;
	 unsigned char *Header;
	 unsigned short TextSize;
     unsigned char TextLang[3];
	 unsigned char *Text;
	 unsigned short Extra_4ASize;
	 unsigned char *Extra_4A;
	 unsigned short Extra_50Size;
	 unsigned char *Extra_50;
	 unsigned short Extra_54Size;
	 unsigned char *Extra_54;
	 unsigned short Extra_55Size;
	 unsigned char *Extra_55;
	 unsigned short Extra_5FSize;
	 unsigned char *Extra_5F;
	 unsigned short Extra_F0Size;
	 unsigned char *Extra_F0;
	 unsigned short Extra_81Size;
	 unsigned char *Extra_81;

};


struct TEIT_Index {
	       unsigned short Anzahl;
           struct T_EIT_Infos *EIT_Infos;
};




/********** SDT **********/

struct dvb_sdt_t {
	u_int version;
	u_int ts_id;			// Transport Stream ID
	u_int orig_nw_id;		// Original Network ID
	int ServiceAnzahl; 
} ;


struct tci {
	 unsigned char Present;
	 unsigned char ModuleName[32];
	 unsigned char Reply;
	 unsigned char LastCmd[128];
	 unsigned short LastCmdLen;
};


struct tci_menu {
             unsigned char zeile[64];
			 unsigned char Type;
			 unsigned short laenge;
			 int id;

};


struct WinRoll_t {
          BOOL Created;
          int startx;
		  int posy;
		  int sizey;
};

struct OSD_Colors_t {
	  unsigned char R;
	  unsigned char G;
	  unsigned char B;
	  unsigned char Blend;
};

// Statusanzeigen
struct DecoderState_t
{
   // VideoState1
	unsigned short	ProcessingState;		
	unsigned short	CommandID;
	unsigned short	dummy1;					
	unsigned short	dummy2;
	unsigned short	RateBuffFullness;		
	unsigned short	dummy3;
	unsigned short	BytesDecoded;			
	unsigned short	SkippedPictures; 
	unsigned short	RepeatedPictures;		
	unsigned short	MostRecentPTS;
	unsigned short	LastPicture;			
	unsigned short	InitDone;
	unsigned short	FreezeIndex;			
	unsigned short	FindIndex;
	unsigned short	DistanceI;				
	unsigned short	ThresholdPTS;

   // VideoState2
	unsigned short	dummy4;					
	unsigned short	dummy5;
	unsigned short	DisablePTSFilt;			
	unsigned short	HSize;
	unsigned short	VSize;					
	unsigned short	AspectRatio;
	unsigned short	FrameRate;				
	unsigned short	BitRate;
	unsigned short	VBVBuffSize;			
	unsigned short	ConstParamFlag;
	unsigned short	IntraQ;					
	unsigned short	NonIntraQ;
	unsigned short	FrameInterval;			
	unsigned short	HeaderBackup;
	unsigned short	RedHeaderFlag;			
	unsigned short	SeqExtension;

	// VideoState3
	unsigned short	GOPTimeCode1;			
	unsigned short	GOPTimeCode2;
	unsigned short	ClosedGOP;				
	unsigned short	PICHeadTempRef;
	unsigned short	SegHeaderExt;			
	unsigned short	ColorDesc;
	unsigned short	ColorPrim;				
	unsigned short	TransferChar;
	unsigned short	MatrixCoeff;			
	unsigned short	DisplayHSize;
	unsigned short	DisplayVSize;			
	unsigned short	GOPHeader;
	unsigned short	TimeCodeW1;				
	unsigned short	TimeCodeW2;
	unsigned short	dummy6;
	unsigned short	dummy7;

};


struct Time_Recorder_t {
	       unsigned short Date_Id;
		   unsigned char StartZeit_h;
		   unsigned char StartZeit_m;
		   unsigned char EndZeit_h;
		   unsigned char EndZeit_m;
		   int Programm;
		   int RecorderArt;
		   BOOL Started;
           BOOL Shutdown;
};



struct IndexHeader_t {
	   char StartZeit[20];
       char AufnahmeDatum[30];
       unsigned char Dauer_h;
	   unsigned char Dauer_m;
	   unsigned char Dauer_s;
	   unsigned char ProgrammName[30];
	   unsigned char Infos[1300];
	   unsigned short RecordFormat;
       unsigned int BitRate;
	   unsigned int SampleFreq;
	   unsigned int TimeStampStart;
	   unsigned int CurrentSpielZeit;
	   unsigned int SpielZeit;
	   unsigned __int64 Dateigroesse;
	   unsigned __int64 Bytesplay;
       char TypeName[30];
	   unsigned int KBTotal;
	   int Anzahl_Files;
	   unsigned char MdiName[256];
	   unsigned char PlayFiles[32][256];
       unsigned int SprungMarken[32];
       unsigned int MegaBytes_Recorded;
};


struct TStreamLog{
	        BOOL Run;
			unsigned short Pid;
			unsigned short Mode;
            char StreamFile[255];
            unsigned short ExtFilter[16];
			unsigned int KBytesWritten;
			unsigned int Bytes;
			unsigned short FilterIndex;
			FILE* StreamFileFd;
};


struct TOSDBitMap {
	       unsigned char DataPtr[32768+1024];
		   unsigned int Offset;
		   unsigned short lpb;
		   unsigned short bpl;
		   unsigned short size;
		   unsigned short bnum;
		   unsigned short brest;
           HANDLE   LoadBitMapEvent;
};

struct frontend {
	int type;              /* type of frontend (tv tuner, dvb tuner/decoder, etc. */
#define FRONT_TV   0
#define FRONT_DVBS 1
#define FRONT_DVBC 2
#define FRONT_DVBT 3

	/* Sat line control */
    int power;             /* LNB power 0=off/pass through, 1=on */
	int volt;              /* 14/18V (V=0/H=1) */
	int ttk;               /* 22KHz */
	int diseqc;            /* Diseqc input select */
	unsigned int freq;            /* offset frequency (from local oscillator) in Hz */
    int AFC;
	unsigned int srate;           /* Symbol rate in Hz */
	int qam;               /* QAM mode for cable decoder, sat is always QPSK */
	int fec;               /* Forward Error Correction */
	unsigned char inversion;
	unsigned short video_pid;        
	unsigned short audio_pid;
    unsigned short tt_pid;          /* Teletext PID */

	/* status information */
	int fsync;             /* frequency sync (from tuner) */
	int sync;              /* sync from decoder */

#define DVB_SYNC_SIGNAL        1
#define DVB_SYNC_CARRIER       2
#define DVB_SYNC_VITERBI       4
#define DVB_SYNC_FSYNC         8
#define DVB_SYNC_FRONT        16

	int afc;               /* frequency offset in Hz */
	unsigned short agc;             /* gain */
	unsigned short nest;            /* noise estimation */
    unsigned int vber;            /* viterbi bit error rate */

	int flags;
#define FRONT_TP_CHANGED  1 
#define FRONT_FREQ_CHANGED 2
#define FRONT_RATE_CHANGED 4
};


typedef struct TVTPage {
	BOOL Fill;
	BYTE Frame[25][40];
	BYTE bUpdated;
	BYTE Lang;
	unsigned int CRC;
	unsigned char crc_count;
};

typedef struct TVT {
    unsigned short SubCount;
	struct TVTPage *SubPage;
};



struct tMultiLink {
	      unsigned char fill;
	      char Name[30];
	      char ExtraText[30];
		  unsigned short ts_id;
		  unsigned short SID;
};

struct tPidParam {
	      unsigned char fill;
	      unsigned char Key_01;
	      unsigned char Key_02;
	      unsigned char Key_03;
	      unsigned char Language[4];
		  unsigned char Name[30];
};


unsigned int RootBytes;

HANDLE MultiDec_Heap;

#define MAXPROGS 4096
#define NEUSIZE   256

struct TProgramm *Programm;
struct TProgramm *ProgrammNeu;

FILE *LogFile;

struct TTiming {
	 DWORD WriteRegDelay;
     DWORD I2C_Bus_Timeout;
	 DWORD I2C_Bus_Reset_Delay;
	 DWORD DVB_Reset_Wait;
	 DWORD DVB_Reset_Scan_Wait;
	 DWORD SendDiSEqCDelay;
	 DWORD Pmt_TimeOut;
	 DWORD Sdt_TimeOut;
	 DWORD Pat_TimeOut;
	 DWORD Cat_TimeOut;
	 DWORD Nit_TimeOut;
     DWORD Debi_Cmd_Time_Out;
     DWORD OSD_Text_Time_Out;
	 DWORD Debi_done_MC2;
	 DWORD Debi_done_PSR;
};
    typedef VOID (*Extern_INIT_DLL)(HINSTANCE , HWND , BOOL , INT , unsigned char *HotKey, unsigned char *Vers, int *ReturnValue );	
	typedef VOID (*Extern_EXIT_DLL)();
	typedef VOID (*Extern_Channel_Change_DLL)(struct TProgramm TP );
	typedef VOID (*Extern_Hot_Key_DLL)();
	typedef VOID (*Extern_OSD_Key_DLL)( unsigned char Osd_Key);
	typedef VOID (*Extern_Menu_Cmd_DLL)( int MenuID );
	typedef VOID (*Extern_Stream_DLL)( int Id, int Laenge, unsigned char *Daten );
	typedef VOID (*Extern_Send_Dll_ID_Name_DLL)(char *Name );
	typedef VOID (*Extern_Filter_Close_DLL)( int FilterId );
	typedef VOID (*Extern_Extern_RecPlay_DLL)( int Command );


struct External_Stream_Dll {
	    unsigned char Name[512];
	    HINSTANCE Externe_DLL;
        Extern_INIT_DLL Extern_Init;
	    Extern_EXIT_DLL Extern_Exit;
        Extern_Channel_Change_DLL Extern_Channel_Change;
	    Extern_Hot_Key_DLL Extern_Hot_Key;
	    Extern_OSD_Key_DLL Extern_OSD_Key;
	    Extern_Menu_Cmd_DLL Extern_Menu_Cmd;
		Extern_Stream_DLL Extern_Stream_Function[8];
		Extern_Filter_Close_DLL Extern_Filter_Close;
		Extern_Extern_RecPlay_DLL Extern_RecPlay;
        Extern_Send_Dll_ID_Name_DLL  Extern_Send_Dll_ID_Name;
        unsigned char HotKey;
		HMENU Extern_Menu;
};

struct External_Stream_Dll *Ext_Dll;
int plugins_count;

#define MDAPI_GET_TRANSPONDER        0x01020000
#define MDAPI_SET_TRANSPONDER        0x01020001

#define MDAPI_GET_PROGRAMM           0x01020010
#define MDAPI_SET_PROGRAMM           0x01020011
#define MDAPI_RESCAN_PROGRAMM        0x01020012
#define MDAPI_SAVE_PROGRAMM          0x01020013
#define MDAPI_GET_PROGRAMM_NUMMER    0x01020014
#define MDAPI_SET_PROGRAMM_NUMMER    0x01020015

#define MDAPI_START_FILTER           0x01020020
#define MDAPI_STOP_FILTER            0x01020021

#define MDAPI_SCAN_CURRENT_TP        0x01020030
#define MDAPI_SCAN_CURRENT_CAT       0x01020031

#define MDAPI_START_OSD              0x01020040
#define MDAPI_OSD_DRAWBLOCK          0x01020041
#define MDAPI_OSD_SETFONT            0x01020042
#define MDAPI_OSD_TEXT               0x01020043
#define MDAPI_SEND_OSD_KEY           0x01020044
#define MDAPI_STOP_OSD               0x01020049

#define MDAPI_GET_VERSION            0x01020100

#define MDAPI_DVB_COMMAND            0x01020060
#define MDAPI_GET_SIGNAL_INFO        0x01020061

struct TOSD_START {
        int DLL_Id;
	    unsigned char BitTiefe;
	    int x1;
	    int y1;
	    int x2;
	    int y2;
		BOOL Input;
};

struct TOSD_START DLL_OSD_Call;

char MD_API_Version[32];

int VideoPID;
int	AudioPID;
int	MultiPID;
int	AddPIDFilter;
int	DelPIDFilter;

unsigned short language;
const char *t(const char *s);

typedef struct {
    unsigned int size;
    char *buffer;
    char *in_ptr;
    char *out_ptr;
    CRITICAL_SECTION critical_section;
    HANDLE has_data_evt, empty_buf_evt;
    unsigned int drop_count;
    unsigned int max_used;
} ring_buf;

