/*********************************************************************************
 *                                                                               *
 * MultiDec.h                                                                    *
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

#include <winioctl.h>
#include "globals.h"

#define ID_STATUSBAR    1700
#define ID_TEXTFIELD       ID_STATUSBAR+1
#define ID_KENNUNGFFIELD   ID_STATUSBAR+2
#define ID_CODEFIELD       ID_STATUSBAR+3
#define ID_FPSFIELD        ID_STATUSBAR+4
#define ID_AUDIOFIELD      ID_STATUSBAR+5

#define INIT_HARDWARE        1800
#define RESET_LIST           1802

#define MAXXRESO              724
#define MAXYRESO              288
#define FAVORIFILE	"favori.set"

int MasterTestzeilen=16;
int TZ[5];

extern BOOL NT;

BOOL DOSIZE=FALSE;


char CurrentDir[256];

HWND hWnd;
HANDLE hInst;
HWND TransPonderDialog=NULL;
int TSLOOP=0;

BOOL OSD_FULL_INPUT=FALSE;

BOOL WRITE_LOG=FALSE;

LONG APIENTRY MainWndProc(HWND, UINT, UINT, LONG);

int topmost;			// added by BigReaper
int dvbout;				// added by Moto
int restoreX;
int restoreY;
int restoreW;
int restoreH;

int emsizex;
int emsizey;
int emstartx;
int emstarty;

int pgsizex=-1;
int pgsizey=-1;
int pgstartx=-1;
int pgstarty=-1;

int evsizex=-1;
int evsizey=-1;
int evstartx=-1;
int evstarty=-1;

extern int VES_TYPE;
extern int DVB_AUDIO;

BOOL Fail_BOOT=FALSE;
BOOL Keep_Driver_Loaded = FALSE;


BOOL USE_TV_16_9=FALSE;

BOOL SaveSettings=FALSE;

BOOL ALLWAYS_TOP=FALSE;

BOOL SplashShow;

BOOL Capture_Event_EIT=TRUE;
BOOL Capture_TID=TRUE;
BOOL Capture_Digi_Text=FALSE;
BOOL SAVE_BOOT=FALSE;


BOOL Display_Status_Bar=TRUE;
BOOL InitStatusBar (HANDLE);
BOOL CreateStatusBar (HWND, HANDLE, int);
BOOL AdjustStatusBar (HWND);
HWND AddStatusField (HANDLE, int, int, int, BOOL);
BOOL DestroyStatusBar (void);

HWND    Status_Bar;
HWND    hwndTextField, hwndPalField;
HWND    hwndKeyField,hwndFPSField;
HWND    hwndAudioField;
 
RECT rectscreen;
RECT LastRect;


#define BT848_RISC_COUNT       0x120
#define BT848_DSTATUS          0x000
#define BT848_DSTATUS_PRES     (1<<7)
#define BT848_DSTATUS_HLOC     (1<<6)
#define BT848_DSTATUS_FIELD    (1<<5)
#define BT848_DSTATUS_NUML     (1<<4)
#define BT848_DSTATUS_CSEL     (1<<3)
#define BT848_DSTATUS_LOF      (1<<1)
#define BT848_DSTATUS_COF      (1<<0)

#define BT848_FCNTR             0x0E8

#define BT848_SRESET           0x07C

#define BT848_IFORM            0x004  
#define BT848_IFORM_HACTIVE    (1<<7)
#define BT848_IFORM_MUXSEL     (3<<5)
#define BT848_IFORM_MUX0       (2<<5)
#define BT848_IFORM_MUX1       (3<<5)
#define BT848_IFORM_MUX2       (1<<5)
#define BT848_IFORM_XTSEL      (3<<3)
#define BT848_IFORM_XT0        (1<<3)
#define BT848_IFORM_XT1        (2<<3)
#define BT848_IFORM_XTAUTO     (3<<3)
#define BT848_IFORM_XTBOTH     (3<<3)
#define BT848_IFORM_NTSC       1
#define BT848_IFORM_PAL_BDGHI  3
#define BT848_IFORM_PAL_M      4
#define BT848_IFORM_PAL_N      5
#define BT848_IFORM_SECAM      6
#define BT848_IFORM_AUTO       0
#define BT848_IFORM_NORM       7

typedef unsigned long PHYS;

PHYS    RiscBasePhysical; 
PHYS    DiplayPhysical; 
DWORD  *RiscBaseLinear;
DWORD  *pDiplayLinear;

DWORD BurstOffset=154;

WD_DMA Risc_dma;
WD_DMA Vbi_dma[5];
WD_DMA *Display_dma[5];
WD_DMA *Surface_dma;
WD_DMA *Overlay_dma;
WD_DMA Burst_dma[5];

HANDLE Bt848Device=NULL;
BOOL BTOK=FALSE;

BOOL USETUNER=TRUE;
BOOL USECARD=TRUE;

#define MAXVTDIALOG 8


typedef struct TVTDialog {
	HWND Dialog;
	int  Page;
	int  SubPage;
	int  FramePos;
	int  FontNr;
	BOOL PageChange;
	unsigned char AsciiPuffer[26][40];
};

struct TVTDialog VTDialog[MAXVTDIALOG];


HANDLE OutThread;

int CountryCode=0;
int CurrentProgramm;
int LastProgramm;
int LastRealProg;
int EditProgramm;
/// Neu

#ifdef USE_BT8X8
typedef struct TCountries
{
        // Device data
        char Name[128];
};

struct TCountries Countries[35];
#endif

unsigned long freq;
char Typ;
unsigned int srate;         

extern struct TTPCat TPCat[32];
extern int TPCatAnzahl;
extern struct tPAT PAT;
extern struct dvb_pmt_struct PMT;

extern int Current_Eit_Index_Size;
extern struct TEIT_Index *EIT;


unsigned char EIT_Sort=2;
unsigned char EIT_Datum=3;
unsigned char EIT_Kanal=0;
unsigned int EIT_Message;

struct Favoriten_t {
	 unsigned short SID_Id;
	 unsigned short ts_id;
};

#define MAXFAVORITEN 128
struct Favoriten_t Favoriten[MAXFAVORITEN];
extern struct TDT_t TDT;
extern short TIMEZONE;
extern struct dvb_sdt_t SDT;
extern BOOL Has_Pat;
extern BOOL Has_Sdt;
#define MAXNIT 512
extern struct tNIT *NIT;
#define NEUNIT 96
extern struct tNIT NITNeu[NEUNIT];

#define MAXFILTER 32

extern struct TFilter *Filter;
extern HANDLE FilterEvent[MAXFILTER];


typedef enum DISPTYPE
{
	BITMAP1,                  /* 1 bit bitmap */
	BITMAP2,                  /* 2 bit bitmap */
	BITMAP4,                  /* 4 bit bitmap */
	BITMAP8,                  /* 8 bit bitmap */
} DISPTYPE, *PDISPTYPE;

extern struct frontend dvb_front; 


int m_dwDisplaySize=0;
BYTE  *pDisplay[5] =  { NULL,NULL,NULL,NULL,NULL  };
unsigned short  *pDecode=NULL;

int TVFormat;
int ColourFormat;
int S_ColourFormat;
int AudioSource;

BOOL VCIRQSync=FALSE;

BOOL Flip=1; 

int UTLoop=0;
HWND UTDialog=NULL;
int UTPage=50;

int MoveXDist=-1;
int MoveYDist=-1;

short HQTabelle[345*4*4];

/// VideoText
COLORREF VTColourTable[16];

HBITMAP             BirneRot;
HBITMAP             BirneGruen;


int WindowStyle = 0;
long WStyle;

extern char VD_DIR[256];
extern char WINIBIS_DIR[256];


#define FLEN 50

extern unsigned char fifo[FLEN][37];
extern int fifo_in;
extern int fifo_out;


BOOL Show_Menu=TRUE;
HMENU hMenu;
HANDLE          hAccel;

extern int CurrentCapture;

int InitialProg=-1;
 
unsigned long InitialFreq;
unsigned char InitialHue=0x00;
unsigned char InitialBrightness=0x00;
unsigned char InitialIFORM=0x00;
int InitialContrast=0xd8;
int LastSaturation=0x00;
int InitialSaturationU=0xfe;
int InitialSaturationV=0xb4;
unsigned char THue=0x00;
unsigned char TBrightness=0x00;
int TContrast=0xd8;
int TSaturationU=0xfe;
int TSaturationV=0xb4;
int InitialLow=45;

int InitialVolume=1000;
char InitialBalance=0x00;
char InitialSpecial=0x00;
char InitialLoudness=0x00;
char InitialBass=0x00;
char InitialTreble=0x00;
BOOL InitialSuperBass=FALSE;
char InitialEqualizer1=0x00;
char InitialEqualizer2=0x00;
char InitialEqualizer3=0x00;
char InitialEqualizer4=0x00;
char InitialEqualizer5=0x00;
BOOL MSPToneControl=FALSE;
char InitialSpatial=0x00;


char TEqualizer1;
char TEqualizer2;
char TEqualizer3;
char TEqualizer4;
char TEqualizer5;

BOOL TSuperBass;
int  TVolume=1000;
char TBalance=0x00;
char TSpecial=0x00;
char TLoudness=0x00;
char TBass=0x00;
char TTreble=0x00;

BOOL Has_MSP=FALSE;

HWND SplashWnd;


struct TPacket30
{
	// Type 0 data

	struct
	{
		unsigned nMag:3;
		unsigned nPage:8;
		unsigned:5; // Unused
		WORD nSubcode;
	} HomePage;
	WORD NetId;
	struct
	{
		char Offset;
		DWORD JulianDay;
		BYTE Hour;
		BYTE Min;
		BYTE Sec;
	} UTC;
	char Unknown[5];
	char Identifier[21];

	// Type 2 data
	
	struct
	{
		unsigned LCI:2;
		unsigned LUF:1;
		unsigned PRF:1;
		unsigned PCS:2;
		unsigned MI:1;
		unsigned day:5;
		unsigned month:4;
		unsigned hour:5;
		unsigned minute:6;
		unsigned:5;
		WORD CNI;
		WORD PTY;
	} PDC;
};

struct TPacket30 Packet30;


unsigned short Set_TextPid;

extern struct TVT VTFrame[800];


struct TTVSetting
{
	WORD wCropWidth,wCropHeight;
	WORD wTotalWidth;
	BYTE bDelayA,bDelayB,bIForm;
	DWORD dwXsfNum,dwXsfDen;
	WORD wHDelayx1,wHActivex1;
	WORD wVDelay;
	WORD wCropOffset;
};

struct TTunerType
{
  WORD thresh1; /* frequency Range for UHF,VHF-L, VHF_H */   
  WORD thresh2;  
  BYTE VHF_L;
  BYTE VHF_H;
  BYTE UHF;
  BYTE config; 
  BYTE I2C;
  WORD IFPCoff;
};

extern BOOL AutoStereoSelect;
extern int MSPMode;
extern int MSPStereo;
extern int MSPMajorMode;
extern int MSPMinorMode;


extern struct TTunerType Tuners[9];

extern unsigned int ManuellAudio[8];



extern struct DVBTunerParameter DVBTuners[6];

struct TTVSetting TVSettings[8] =
{
	/* PAL-BDGHI */
	{ 768, 576, 1135, 0x7f, 0x72, (BT848_IFORM_PAL_BDGHI|BT848_IFORM_XT1),
	    944, 768, 186, 922, 0x20, 0 },
	/* NTSC */
	{ 640, 480,  910, 0x68, 0x5d, (BT848_IFORM_NTSC|BT848_IFORM_XT0),
	    780, 640, 135, 754, 0x16, 0},
	/* SECAM */
	{ 768, 576, 1135, 0x7f, 0xb0, (BT848_IFORM_SECAM|BT848_IFORM_XT1),
	    944, 768, 186, 922, 0x20},
	/* PAL-M */
	{ 640, 480, 910, 0x68, 0x5d, (BT848_IFORM_PAL_M|BT848_IFORM_XT0),
	    780, 640, 186, 922, 0x16, 0},
	/* PAL-N */
	{ 768, 576, 1135, 0x7f, 0x72, (BT848_IFORM_PAL_N|BT848_IFORM_XT1),
	    944, 768, 186, 922, 0x20, 0},

	/* The Nagra Screen Burst*/
	{ 922, 576, 1135, 0x7f, 0x72, (BT848_IFORM_PAL_BDGHI|BT848_IFORM_XT1),
	    944, 822, 110, 998, 0x20, 0 },

	/* The 800x600 Special Screen */
	{ 800, 576, 1135, 0x7f, 0x72, (BT848_IFORM_PAL_BDGHI|BT848_IFORM_XT1),
	    944, 768, 186, 922, 0x20, 0 },

		/* PAL-BDGHI VideoDat-Receiver*/ 
	{ 768, 576, 1135, 0x7f, 0x72, (BT848_IFORM_PAL_BDGHI|BT848_IFORM_XT1),
	    944, 768, 186, 922, 0x26, 0 },

};


typedef struct TChannels
{
        char Name[128];
		int MinChannel;
		int MaxChannel;
        unsigned long freq[512];
};

extern struct TChannels Channels;


char ChannelString[10];

char BTTyp[30];
char MSPStatus[30];
char TunerStatus[30];


int CardType;
int TunerType;
int VideoSource;
int TVTYPE;
int S_TVTYPE;

int LastFrame;


BOOL Capture_Video = TRUE;
BOOL Capture_VBI=TRUE;
BOOL Capture_Pause=FALSE;
BOOL OutDib=TRUE;
BOOL ModeOverlay=FALSE;
UINT  WTVBPP=0;
BOOL WAIT_DX_VBI=FALSE;
BOOL USE_DX_LOCK=FALSE;
BOOL USE_DX_OV_STRECH=TRUE;

LPVOID                 lpSurface=NULL;
LPVOID                 lpOverlay=NULL;
LPDIRECTDRAW	lpDD;


extern BOOL Can_Overlay;
extern BOOL Can_Overlay_Strech;
extern LPDIRECTDRAWSURFACE     lpDDSurface;
extern LPDIRECTDRAWSURFACE     lpDDOverlay;


BOOL bFullscreen=FALSE;

int Offset_for_WindowOut;
int CurrentX,CurrentY;
int Res_X,Res_Y;

struct TKEY {
	    char Name[20];
		char Extension[4];
};

struct TKEY OPKEY[4];

int ActiveKey=0;

int ShowY;
int SurfacePitch;
int OverlayPitch;
int DibPitch;
int nPixelSchritt;
int nPixelStart;

unsigned char *pBurstLine[5];


// Decoding Stuff
BOOL DecodeNagra=FALSE;
BOOL NagraFullField = FALSE;
BOOL DecodeVCrypt=FALSE;
BOOL DecodeDecoderVCrypt=FALSE;

HDC  DIB_Hdc;

// Nagravision Stuff 
#define TESTPIXEL             32
#define ZEILENZAHL           287

int MovieBoost;
//int Old_MovieBoost=0;
int MovieScale=0;
int MaxY;
int MaxX;

BOOL Chroma_Key=TRUE;

extern DWORD GlobalJump;



int VCLmargin=10;

int                FORMAT_MASK           =  0x0F;
#define            FORMAT_4_3               0x08
#define            FORMAT_14_9              0x01
#define            FORMAT_16_9              0x0B
int PalFormat = 0;

__int64  CurrentLowValue    = 0x002d002d002d002d;

BOOL useSound=FALSE;
BOOL canSound;

BOOL MoreLinesLock=FALSE;
int  FirstWidthEven=TESTPIXEL;
int  FirstWidthOdd=TESTPIXEL;
int  FirstWidthEvenCount=0;
int  FirstWidthOddCount=0;

BOOL Line286BlackEven=FALSE;
BOOL Line286BlackOdd=FALSE;
BOOL Line285BlackEven=FALSE;
BOOL Line285BlackOdd=FALSE;
BOOL Line0BlackEven=FALSE;
BOOL Line0BlackOdd=FALSE;
BOOL Line1BlackEven=FALSE;
BOOL Line1BlackOdd=FALSE;
int B0e;
int B1e;
int B285e;
int B286e;
int B0o;
int B1o;
int B285o;
int B286o;
BOOL MatrixModeEven=FALSE;
BOOL MatrixModeOdd=FALSE;
int MatrixLinesEven=0;
int MatrixLinesOdd=0;
int MatrixCountEven=0;
int MatrixCountOdd=0;
int AutoLockCount=0;
int BlackEven=-1;
int BlackOdd=-1;
int UntilEven=0;
int UntilOdd=0;


char OrgDriverName[128];
char OrgDVBDriverName[128];

#ifdef USE_BT8X8
BOOL Has_BT=FALSE;
int Current_Mode=0;
#else
const BOOL Has_BT=FALSE;
const Current_Mode=1;
#endif
BOOL Has_DVB=FALSE;

extern int pSortLinesEven[ZEILENZAHL];
extern int pSortLinesOdd[ZEILENZAHL];

int  nPALplusZeile  = -10;
int  nPALplusData   =  0;
int    BLData =  0;

BOOL  BlackSet[288];


int INIT_PLL = 0; 
const __int64 nMaske    = 0x00ff00ff00ff00ff;
const __int64 One       = 0x0001000100010001;

int NextFrame=1;
int CurrentFrame=1;

// VC

extern HANDLE Decode_Event;
extern HANDLE Decode_Event_Stop;

int OldMode;

BOOL AutoDetect_Nagra=TRUE;
BOOL AutoDetect_VC=TRUE;

BOOL Decode_AutoDetect;
int msgx=0;
BOOL Decode_AutoLock;
BOOL Auto_MovieBoost=TRUE;
BOOL AutoMovieBoostEnable=FALSE;

BOOL ModeRAWSurface=FALSE;
BOOL ModeRAWOverlay=FALSE;


extern int CurrentIrq;

 __int64  Black=0x8000800080008000;
 __int64  nMinus128 = 0x8000800080008000;
 __int64  nFaktoren = 0xf2fe20491989f9bf;
 __int64  nMaskeUV = 0xFF00FF00FF00FF00;

short HQLinebufferEven[846*8];
short *HQLinebufferEvenPtr=HQLinebufferEven;
short HQLinebufferOdd[846*8];
short *HQLinebufferOddPtr=HQLinebufferOdd;

#ifdef USE_BT8X8
extern unsigned short cp_odd[256][285];
#endif
 
short VCHQTabelleBurst[710*4*2];
short VCHQTabelleNoBurst[710*4*2];


BOOL HalfTVMode=FALSE;
BOOL Toggle_WithOut_Frame=FALSE;

BOOL CleanUP=FALSE;

BOOL Sync_Irq=FALSE;
extern BOOL VD_RAW;
extern int VBI_FPS;

int VBI_Flags=3;
#define VBI_VT  1
#define VBI_VPS 2
#define VBI_VC  4
#define VBI_IC  8
#define VBI_VD  16
#define VBI_WINBIS  32

int CryptMode;
extern	char VPS_chname[9];
extern 	char VPS_lastname[9];

typedef struct TCryptInfo  {
       int CryptMode;
	   int LastCryptMode;
	   unsigned char OSD0[10];
       unsigned char OSD1[10];
       unsigned char MSG0[10];
       unsigned char MSG1[10];
   	   BYTE OSD0Set;
	   BYTE OSD1Set;
	   BYTE MSG0Set;
	   BYTE MSG1Set;

};

struct TCryptInfo CryptInfo;


HWND ShowPDCInfo=NULL;
HWND ShowVTInfo=NULL;
HWND ShowVPSInfo=NULL;
BOOL System_Shutdown=FALSE;


extern unsigned short UTPages[12];
extern unsigned short UTCount;


// HC_ColorOptionen

BOOL HCInit15=FALSE;
BOOL InvertRot=FALSE;
BOOL InvertGruen=FALSE;
BOOL InvertBlau=FALSE;
BOOL OffRot=FALSE;
BOOL OffGruen=FALSE;
BOOL OffBlau=FALSE;
BOOL HCMode1=FALSE;
BOOL HCMode2=FALSE;


short HCSchwelle=0x00;
short HCRotFaktor=0x00;
short HCGruenFaktor=0x00;
short HCBlauFaktor=0x00;
short HCCutFaktor=0x00;
short HCCutHiRotFaktor=0x00;
short HCCutHiGruenFaktor=0x00;
short HCCutHiBlauFaktor=0x00;
short HCCutLoRotFaktor=0x00;
short HCCutLoGruenFaktor=0x00;
short HCCutLoBlauFaktor=0x00;

BOOL ColorMix=FALSE;
unsigned short *HCYUV2RGBTabelle = NULL;

int HardWareFound=0;

HWND ProgList;
HWND UTList;
LOGFONT lf = {16,0,0,0,400,0,0,0,0,0,0,0,0,"MS Sans Serif"};   
HFONT currFont;

struct TUT {
	char Zeile[240];
    short Fg;
	short Bg;
};

struct TUT UT[128];

 char VTtoAscii[8][96] = {
 	" !#£$%&´()*+,-./0123456789:;<=>?" // English
 	"@ABCDEFGHIJKLMNOPQRSTUVWXYZ«½»^#"
 	"-abcdefghijklmnopqrstuvwxyz¼¦¾÷#",

 	" !##$%&´()*+,-./0123456789:;<=>?" // German
 	"§ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜ^_"
 	"-abcdefghijklmnopqrstuvwxyzäöüß#",

 	" !##¤%&´()*+,-./0123456789:;<=>?" // Swedish
 	"ÉABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÅÜ_"
 	"-abcdefghijklmnopqrstuvwxyzäöåü#",

 	" !#£$%&´()*+,-./0123456789:;<=>?" // Italian
 	"éABCDEFGHIJKLMNOPQRSTUVWXYZ°ç»^#"
 	"-abcdefghijklmnopqrstuvwxyzàòèì#",

 	" !#éï%&´()*+,-./0123456789:;<=>?" // French
 	"àABCDEFGHIJKLMNOPQRSTUVWXYZëêùî#"
 	"-abcdefghijklmnopqrstuvwxyzâôûç#",

 	" !#ç$%&´()*+,-./0123456789:;<=>?" // Spanish
 	"¡ABCDEFGHIJKLMNOPQRSTUVWXYZáéíóú"
 	"-abcdefghijklmnopqrstuvwxyzüñèà#",

 	" !#£$%&´()*+,-./0123456789:;<=>?" // Defaults to English
 	"@ABCDEFGHIJKLMNOPQRSTUVWXYZ«½»^#"
 	"-abcdefghijklmnopqrstuvwxyz¼¦¾÷#",

 	" !#£$%&´()*+,-./0123456789:;<=>?" // Defaults to English
 	"@ABCDEFGHIJKLMNOPQRSTUVWXYZ«½»^#"
 	"-abcdefghijklmnopqrstuvwxyz¼¦¾÷#",

  };


extern int VBI_lpf;
int BeforeVD=0;

// Neues MenuHandling
int MenuFlip;
BOOL MenuOutDib;
BOOL MenuModeOverlay;
BOOL MenuModeRAWSurface;
BOOL MenuModeRAWOverlay;
BOOL MenuSave=FALSE;

struct TVDat {
	    int Status;
		int LastKillPos;
        int Interleave;
		int Lenght;
		int Pos;
		int Blockzaehler;
		int CRCError;
		int BlockSoFar;
    	unsigned char eLBN;
		unsigned char SOB1;
        unsigned char SOB2;
        unsigned char ADR;
        unsigned char SYS;
        unsigned char LBN;
        unsigned char KEY;
        unsigned char XDATA[1024];   //ARRAY of Byte...
        unsigned char CSL;
        unsigned char CSH;
        int Fehler_Frei;
		char FileName[13];
		unsigned int FileSize;
		FILE *FilePtr;

    int Index;
	char RawName[128];
	unsigned int BlocksOK;
	unsigned int BlocksError;
	char Error[512];
};

extern struct TVDat VDat;

typedef struct SOTREC  {
	unsigned int  date;   //Datum der Ausstrahlung...
    unsigned short fanz;          //Anzahl der übertragenden Dateien.
 	char           GeneralName[512];  
	char           ExecCommand[512];  
    char           Kommentar[512];    
};

extern struct SOTREC SOTInfoRec;


BOOL System_In_Mute=FALSE;
int System_in_Sync=0;

extern int CI_Menu_Slot;
// Mixer
extern int DeviceAnzahl;
extern MIXERCAPS *MixerDev;
extern MIXERLINE *MixerLine;
extern char InitialMixer[MAXPNAMELEN]; 
extern char InitialMixerLine[MIXER_LONG_NAME_CHARS]; 
extern char InitialMixerConnect[MIXER_LONG_NAME_CHARS]; 
extern int CurrentLine;
extern int CurrentMixer;
extern int CurrentConnect;



struct TMixerAccess {
    int SoundSystem;
    int Destination;
    int Connection;
    int Control;
};

struct TMixerValues {
    int Kanal1;
    int Kanal2;
    int Kanal3;
    int Kanal4;
};


struct TMixerAccess Volume={-1,0,0,0};
struct TMixerAccess Mute={-1,0,0,0};

struct TMixerLoad {
    	struct TMixerAccess MixerAccess;
        struct TMixerValues MixerValues;
};
struct TMixerLoad MixerLoad[64];

struct TMixerControls {
             int AnzahlControls;
	         MIXERCONTROL          *MixerControl;
             MIXERCONTROLDETAILS   *MixerDetail;
};

struct TBL {
	        short FeldId;
			unsigned short x;
			unsigned short s;
			unsigned short r;
		};

struct TBL ButtonList[15];
unsigned short SelectButton;

char KeyValue;
int CurSel;

struct TMixerConnections {
	        int AnzahlConnections;
			MIXERLINE *MixerConnections;
			struct TMixerControls *To_Control;
};


struct TMixerLines {
	        int AnzahlLines;
			MIXERLINE *MixerLine;
			struct TMixerConnections *To_Connection;
};


struct TSoundSystem {
	    int DeviceAnzahl;
        MIXERCAPS *MixerDev;
		struct TMixerLines *To_Lines;
};


extern struct TSoundSystem SoundSystem;


BOOL USE_MIXER=FALSE;
int  MIXER_LINKER_KANAL=-1;
int  MIXER_RECHTER_KANAL=-1;
int  MIXER_LINKER_KANAL_STARTUP=-1;				// Systemlautstaerke beim starten
int  MIXER_RECHTER_KANAL_STARTUP=-1;
MIXERCONTROLDETAILS_BOOLEAN MUTE_ON_STARTUP[4];	// Mutestatus beim starten

int MixerVolumeStep=-1;
int MixerVolumeMax=-1;

// Fantasio for Secam-Extensions
short NullTable[4] = {8192, 0, 0, 8192};
int	iUFilterThresholdPCent=70;  // Should start at 70 %
int	iVFilterThresholdPCent=70;  // Should start at 70 %
int TVFilter;
int TUFilter;



// CALL-Test
short *ScreenPtr;
short *TmpPtr;
short *ColFaktor;
int LinesAnzahl;
int BytesOut;
int CurrentPitch;

int AnzahlProzessor=1;
int MainProzessor=0;
int IrqProzessor=0;
int VBIProzessor=0;
int AusgabeProzessor=0;
int ProzessorMask;

#define CLEARLINES 288

extern short ZeilenPuffer[844];
short *pLinesDstAddressEven[CLEARLINES];
short *pLinesDstAddressOdd[CLEARLINES];
short *pLinesAddressEven[5*CLEARLINES];
short *pLinesAddressOdd[5*CLEARLINES];
short LinesOutEven[CLEARLINES];
short LinesOutOdd[CLEARLINES];
short RealOffset[287];

extern int referenzes[16];

int ResetIrq=0;

int VCFirstLine;
int VCTotalLines;
BOOL bFilterBlueLines=FALSE;


// DVB
extern int DVBTunerType;
extern int DVBDecoderType;


char DVBTyp[30];
HANDLE		Device_Handle;
WD_DMA rps0_mem_dma;
WD_DMA debi_mem_dma;
WD_DMA DVB_Display_dma;
WD_DMA Record_mem_dma;
WD_DMA BitMap_mem_dma;
WD_DMA Text_mem_dma;
WD_DMA CA_mem_dma;
WD_DMA CI_mem_dma;
WD_DMA IPMPE_mem_dma;
WD_DMA Ir_mem_dma;
WD_DMA Debug_mem_dma;


unsigned int InitialDVBBrightness=0x80;
unsigned int InitialDVBContrast=0x40;
unsigned int InitialDVBColor=0x40;
unsigned int TColor;
extern unsigned int InitialDVBVolumeLinks=0xff;
extern unsigned int InitialDVBVolumeRechts=0xff;
unsigned int TVolumeLinks=0;
unsigned int TVolumeRechts=0;

extern BOOL Keep_DVB_Loaded;
extern char FirmID[16];
extern char FIRM_DIR[256];
extern BOOL FastBoot;
int VES1820_Inversion=0;

#define DISEQC_HIGH_NIBLE 0xF0
#define DISEQC_LOW_BAND  0x00
#define DISEQC_HIGH_BAND 0x01
#define DISEQC_VERTICAL  0x00
#define DISEQC_HORIZONTAL 0x02
#define DISEQC_POSITION_A 0x00
#define DISEQC_POSITION_B 0x04
#define DISEQC_OPTION_A  0x00
#define DISEQC_OPTION_B  0x08

struct TLNB LNB[4];

struct TTransponder Transponder;

unsigned int SRates[16] = { 27500,22000,29900,28000,13400,12130,9100,6161,5632,4340,4000,0,0,0,0,0 };

int Initial_FontNr=1;
int Initial_Export_Font=1;

unsigned int ShutDownValue=EWX_POWEROFF;
unsigned int CurrentShutDownFlag;
char ShutDownText[255];

int VT_Lang=0;
int VT_Cache;
BYTE VT_Header_Line[40];
int VT_EXPORT_TYP=3;
BOOL VT_ALWAYS_EXPORT=FALSE;
BOOL VT_EXPORT_BACKGROUND=FALSE;
BOOL VT_EXPORT_PAGE_LINK=FALSE;
BOOL VT_EXPORT_PAGE_CONTROL=FALSE;
char VT_BASE_DIR[256];
char VT_EXPORT_WWW_ROOT[256];
int TXTStatus;
char TXTError[256];


extern BOOL PROGRAMM_OSD;
extern BOOL PROGRAMM_OSD_RUN;
extern Programm_OSD_startY;
extern HANDLE Programm_OSD_Thread;
extern HANDLE Programm_OSD_Stop;
extern HANDLE OSD_KEY_EVENT;
extern unsigned char OSD_Key_Value;

extern struct OSD_Colors_t OSD_Colors[4];

unsigned int MAXMegaBytes=4096;

#define TIMERANZAHL 12
struct Time_Recorder_t TimerRec[TIMERANZAHL];

struct TShutdownZeit {
	   unsigned char H;
	   unsigned char M;
};

struct TShutdownZeit ShutdownZeit;


#define    AudioMPG              0x01
#define    VideoMPG              0x04
#define    AV_MPG                0x05

extern int RecorderFormat;
extern int RecorderTyp;
extern int RecorderFile;
extern char LastRecorderFile[256];
extern char RecordBaseDir[9][128];
extern BOOL Record;
extern BOOL Play;
extern BOOL Recorder_Pause;

extern int ci_interface;
extern struct tci ci_module[2];
extern int PopUpCam;

// Erweiterte Treiberumgebung

BOOL LoadExternBTDriver=FALSE;
BOOL ExternBTDriver=FALSE;

extern int VT_Stream_ID;

BOOL DVB_VT=FALSE;
BOOL DVB_WINBIS=FALSE;

DWORD TunerStatusByte;

int LastWork=-1;

BOOL VIEW_ANALOG=TRUE;
BOOL VIEW_DIGITV=TRUE;
BOOL VIEW_DIGIRADIO=TRUE;
BOOL VIEW_DIGI_REST=TRUE;
BOOL VIEW_FREE=FALSE;
VIEW_TEMP_CHANNEL;
BOOL VIEW_LINK_CHANNEL=TRUE;
BOOL VIEW_TEMP_CHANNEL=TRUE;

extern struct tMultiLink MultiLinkZeit[16];
extern struct tMultiLink MultiLinkView[16];
extern struct tPidParam PidParam[16];

extern char OSD_PROG_Name[128];


BOOL auto_scan_srate;


HWND REMOTE_CONFIG_WND=NULL;

HWND ScanWndHandle;
BOOL ContinueScan;
BOOL auto_srate;
char Nit_Orbital[64];

extern BOOL FastSwitch;


struct FavoriInfo {
	   char FolderName[15];
       unsigned char diseqc;
       unsigned short TP;
	   unsigned short SID;
};

struct FavoriTree {
		struct FavoriInfo *info;
		struct FavoriTree *pElement;
		struct FavoriTree *nElement;
		HTREEITEM item;
		HTREEITEM root;
};

struct FavoriTree *fElement = NULL;

BOOL favoriBoxOpen = FALSE;

int cableScanStep = 100;

unsigned int srate_auto_scan = 1;
unsigned int srate_auto_scan_step = 1;
unsigned int srate_min = 1;
unsigned int srate_max = 7000000;
struct TTiming Timing;

extern BOOL DVB_IRQ_SHARING;
extern BOOL BT_IRQ_SHARING;

extern HANDLE DLL_OSD_Created;

