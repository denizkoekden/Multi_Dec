
/*********************************************************************************
 *                                                                               *
 * bt848.h: Hardware-Parts for the BT-Chips                                      *
 *                                                                               *
 * Copyright (C) 2000 Espresso                                                   *
 *                                                                               *
 *                                                                               *
 *  originally based on code by:                                                 *
 *                                                                               *
 *   BT-Parts                                                                    *
 *                                                                               *
 *   Copyright (C) 1996,97,98 Ralph  Metzler (rjkm@thp.uni-koeln.de)             * 
 *                          & Marcus Metzler (mocm@thp.uni-koeln.de)             *
 *   msp34XX                                                                     *
 *                                                                               *
 *   Copyright (C) 1997,1998 Gerd Knorr <kraxel@goldbach.in-berlin.de>           *
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

extern HANDLE hInst;

extern BOOL USETUNER;
extern BOOL USECARD;



extern int msgx;
extern BOOL Decode_AutoDetect;
extern BOOL MoreLinesLock;
extern BOOL Decode_AutoLock;

extern BOOL AutoDetect_Nagra;
extern BOOL AutoDetect_VC;

extern BOOL DOSIZE;
extern DWORD TunerStatusByte;


extern int ResetIrq;

extern int AnzahlProzessor;
extern int MainProzessor;
extern int IrqProzessor;
extern int VBIProzessor;
extern int AusgabeProzessor;
extern int ProzessorMask;

extern BOOL Keep_Driver_Loaded;

BOOL nagra_coded;
extern BOOL USE_DX_LOCK;

extern BOOL Flip; 
extern int emstartx;
extern int emstarty;
extern int emsizex;
extern int emsizey;
extern int CurrentX;
extern int CurrentY;
extern BOOL Capture_Video;
extern BOOL Capture_VBI;
extern BOOL Capture_Pause;
extern BOOL OutDib;
extern int ColourFormat;
extern BOOL DecodeNagra;
extern BOOL NagraFullField;
extern BOOL DecodeVCrypt;
extern BOOL DecodeDecoderVCrypt;
extern BOOL HalfTVMode;

extern char OrgDriverName[128];
extern char OrgDVBDriverName[128];

extern BOOL SaveSettings;

extern HANDLE OutThread;
extern BYTE  *pDisplay[5];
extern int Offset_for_WindowOut;
extern int CurrentX,CurrentY;
extern int SurfacePitch;
extern int DibPitch;
extern LPVOID lpSurface;

extern BOOL bFullscreen;
extern HWND hWnd;
extern LPDIRECTDRAW	lpDD;
extern UINT  WTVBPP;     
extern HWND hwndTextField;
extern DWORD BurstOffset;
extern BOOL Has_MSP;

extern int INIT_PLL;

extern int InitialProg;
extern unsigned char InitialHue;
extern unsigned char InitialBrightness;
extern int InitialContrast;
extern int InitialSaturationU;
extern int InitialSaturationV;
extern unsigned char InitialIFORM;
unsigned char SaveIFORM;
extern int InitialLow;
extern	BOOL InitialSuperBass;
extern	char InitialEqualizer1;
extern	char InitialEqualizer2;
extern	char InitialEqualizer3;
extern	char InitialEqualizer4;
extern	char InitialEqualizer5;
extern	char InitialSpatial;

HANDLE		Device_Handle;
HANDLE		BT_Plugin_Handle;
extern HANDLE		DVB_Plugin_Handle;

int SaveTuner;
int SaveVideoSource;

extern int InitialVolume;
extern char InitialSpecial;
extern char InitialBalance;
extern char InitialLoudness;
extern char InitialBass;
extern char InitialTreble;
extern char InitialBalance;

extern HWND SplashWnd;

extern unsigned char *pBurstLine[5];

extern int LastFrame;

extern int PAGE_BUFFER_COUNT;

extern BOOL VT_ALWAYS_EXPORT;

#define MAXVTDIALOG 8

extern BOOL Fail_BOOT;

BOOL VD_RAW=FALSE;

extern struct tci ci_module[2];
extern int ci_interface;

BOOL BT_IRQ_SHARING=FALSE;


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

struct TVDat VDat;

struct TVDatBlockz {
    	unsigned char SOB1;
        unsigned char SOB2;
        unsigned char ADR;
        unsigned char SYS;
        unsigned char LBN;
        unsigned char KEY;
        unsigned char XDATA[1024];   //ARRAY of Byte...
        int  pos;           //Aktuelle Position im Bytestream
        int  Lenght;        //Laenge des Datenrumpfes
        int  interleave;    //WH Zdhler...
        unsigned char CSL;
        unsigned char CSH;
        int Fehler_Frei;
};
struct TVDatBlockz VDATBlockz[8];

unsigned char vdat[10];

//Struktur für den Block SOT - Start of Transmission   !!SF
typedef struct SOTREC  {
	unsigned int  date;   //Datum der Ausstrahlung...
    unsigned short fanz;          //Anzahl der übertragenden Dateien.
 	char           GeneralName[512];  
	char           ExecCommand[512];  
    char           Kommentar[512];    
};

struct SOTREC SOTInfoRec;

//Dummy für den Block SOF - Start of File.            !!SF
//Wird in einer der kommenden Version implementiert!
typedef struct strcSOF {
  int            not_in_use;
}SOFREC;

extern HWND ShowPDCInfo;
extern HWND ShowVPSInfo;


#define VBI_DATA_SIZE 2048*39
int VBI_lpf = 16;   // lines per field
int VBI_bpl = 2048;   // bytes per line
BYTE VBI_thresh;
BYTE VBI_off;
BYTE VBI_vcbuf[25];
BYTE VBI_vc2buf[25];

unsigned char BT_VT_Buffer[1010];

char VD_DIR[256];
char Current_VD_DIR[256];

extern int VBI_Flags;
unsigned int VBI_spos;

int VBI_FPS;

unsigned int vtstep, vcstep, vpsstep, vdatstep;

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

extern struct TCryptInfo CryptInfo;



#define VBI_VT  1
#define VBI_VPS 2
#define VBI_VC  4
#define VBI_VD  16
#define VBI_WINBIS  32

char VPS_tmpName[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
char VPS_lastname[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
char VPS_chname[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
int VPS_namep=0;

#define FPSHIFT 16
#define FPFAC (1<<FPSHIFT)

BOOL Has_BT;
extern BOOL Has_DVB;
extern int Current_Mode;

unsigned char invtab[256] = {
  0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 
  0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0, 
  0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 
  0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8, 
  0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 
  0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4, 
  0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 
  0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc, 
  0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 
  0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2, 
  0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 
  0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa, 
  0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 
  0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6, 
  0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 
  0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe, 
  0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 
  0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1, 
  0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 
  0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9, 
  0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 
  0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5, 
  0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 
  0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd, 
  0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 
  0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3, 
  0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 
  0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb, 
  0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 
  0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7, 
  0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 
  0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff, 
};

unsigned char revham[16] = { 
  0x00, 0x08, 0x04, 0x0c, 0x02, 0x0a, 0x06, 0x0e,
  0x01, 0x09, 0x05, 0x0d, 0x03, 0x0b, 0x07, 0x0f };


unsigned char unhamtab[256] = {
  0x01, 0x0f, 0x01, 0x01, 0x0f, 0x00, 0x01, 0x0f, 
  0x0f, 0x02, 0x01, 0x0f, 0x0a, 0x0f, 0x0f, 0x07, 
  0x0f, 0x00, 0x01, 0x0f, 0x00, 0x00, 0x0f, 0x00, 
  0x06, 0x0f, 0x0f, 0x0b, 0x0f, 0x00, 0x03, 0x0f, 
  0x0f, 0x0c, 0x01, 0x0f, 0x04, 0x0f, 0x0f, 0x07, 
  0x06, 0x0f, 0x0f, 0x07, 0x0f, 0x07, 0x07, 0x07, 
  0x06, 0x0f, 0x0f, 0x05, 0x0f, 0x00, 0x0d, 0x0f, 
  0x06, 0x06, 0x06, 0x0f, 0x06, 0x0f, 0x0f, 0x07, 
  0x0f, 0x02, 0x01, 0x0f, 0x04, 0x0f, 0x0f, 0x09, 
  0x02, 0x02, 0x0f, 0x02, 0x0f, 0x02, 0x03, 0x0f, 
  0x08, 0x0f, 0x0f, 0x05, 0x0f, 0x00, 0x03, 0x0f, 
  0x0f, 0x02, 0x03, 0x0f, 0x03, 0x0f, 0x03, 0x03, 
  0x04, 0x0f, 0x0f, 0x05, 0x04, 0x04, 0x04, 0x0f, 
  0x0f, 0x02, 0x0f, 0x0f, 0x04, 0x0f, 0x0f, 0x07, 
  0x0f, 0x05, 0x05, 0x05, 0x04, 0x0f, 0x0f, 0x05, 
  0x06, 0x0f, 0x0f, 0x05, 0x0f, 0x0e, 0x03, 0x0f, 
  0x0f, 0x0c, 0x01, 0x0f, 0x0a, 0x0f, 0x0f, 0x09, 
  0x0a, 0x0f, 0x0f, 0x0b, 0x0a, 0x0a, 0x0a, 0x0f, 
  0x08, 0x0f, 0x0f, 0x0b, 0x0f, 0x00, 0x0d, 0x0f, 
  0x0f, 0x0b, 0x0b, 0x0b, 0x0a, 0x0f, 0x0f, 0x0b, 
  0x0c, 0x0c, 0x0f, 0x0c, 0x0f, 0x0c, 0x0d, 0x0f, 
  0x0f, 0x0c, 0x0f, 0x0f, 0x0a, 0x0f, 0x0f, 0x07, 
  0x0f, 0x0c, 0x0d, 0x0f, 0x0d, 0x0f, 0x0d, 0x0d, 
  0x06, 0x0f, 0x0f, 0x0b, 0x0f, 0x0e, 0x0d, 0x0f, 
  0x08, 0x0f, 0x0f, 0x09, 0x0f, 0x09, 0x09, 0x09, 
  0x0f, 0x02, 0x0f, 0x0f, 0x0a, 0x0f, 0x0f, 0x09, 
  0x08, 0x08, 0x08, 0x0f, 0x08, 0x0f, 0x0f, 0x09, 
  0x08, 0x0f, 0x0f, 0x0b, 0x0f, 0x0e, 0x03, 0x0f, 
  0x0f, 0x0c, 0x0f, 0x0f, 0x04, 0x0f, 0x0f, 0x09, 
  0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0e, 0x0f, 0x0f, 
  0x08, 0x0f, 0x0f, 0x05, 0x0f, 0x0e, 0x0d, 0x0f, 
  0x0f, 0x0e, 0x0f, 0x0f, 0x0e, 0x0e, 0x0f, 0x0e, 
};

#ifdef USE_BT8X8
typedef struct TCountries
{
        char Name[128];
};

extern struct TCountries Countries[35];

#endif

typedef unsigned long PHYS;

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


unsigned short UTPages[12];
unsigned short UTCount=0;

extern struct frontend dvb_front; 

typedef struct TChannels
{
        char Name[128];
		int MinChannel;
		int MaxChannel;
        unsigned long freq[512];
};

struct TChannels Channels;

extern struct TPacket30 Packet30;

CRITICAL_SECTION m_cCrit;

typedef struct AllocData
{
	DWORD Size;		// Size to allocate/allocated 
	LPVOID Linear;	// Linear address 
	PHYS Physical;	// Physical address 
};

extern PHYS    RiscBasePhysical; 
extern PHYS    DiplayPhysical; 
extern DWORD  *RiscBaseLinear;
extern DWORD  *pDiplayLinear;

extern HANDLE Bt848Device;
extern BOOL BTOK;


/* Brooktree 848 registers */

#define BT848_DSTATUS          0x000
#define BT848_DSTATUS_PRES     (1<<7)
#define BT848_DSTATUS_HLOC     (1<<6)
#define BT848_DSTATUS_FIELD    (1<<5)
#define BT848_DSTATUS_NUML     (1<<4)
#define BT848_DSTATUS_CSEL     (1<<3)
#define BT848_DSTATUS_LOF      (1<<1)
#define BT848_DSTATUS_COF      (1<<0)

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


#define BT848_FCNTR            0x0E8
#define BT848_PLL_F_LO         0x0F0
#define BT848_PLL_F_HI         0x0F4
#define BT848_PLL_XCI          0x0F8

#define BT848_TGCTRL           0x084  
#define BT848_TGCTRL_TGCKI_PLL 0x08
#define BT848_TGCTRL_TGCKI_NOPLL 0x00

#define BT848_TDEC             0x008  
#define BT848_TDEC_DEC_FIELD   (1<<7)
#define BT848_TDEC_FLDALIGN    (1<<6)
#define BT848_TDEC_DEC_RAT     (0x1f)

#define BT848_E_CROP           0x00C
#define BT848_O_CROP           0x08C

#define BT848_E_VDELAY_LO      0x010
#define BT848_O_VDELAY_LO      0x090

#define BT848_E_VACTIVE_LO     0x014
#define BT848_O_VACTIVE_LO     0x094

#define BT848_E_HDELAY_LO      0x018
#define BT848_O_HDELAY_LO      0x098

#define BT848_E_HACTIVE_LO     0x01C
#define BT848_O_HACTIVE_LO     0x09C

#define BT848_E_HSCALE_HI      0x020
#define BT848_O_HSCALE_HI      0x0A0

#define BT848_E_HSCALE_LO      0x024
#define BT848_O_HSCALE_LO      0x0A4

#define BT848_BRIGHT           0x028

#define BT848_E_CONTROL        0x02C
#define BT848_O_CONTROL        0x0AC
#define BT848_CONTROL_LNOTCH    (1<<7)
#define BT848_CONTROL_COMP      (1<<6)
#define BT848_CONTROL_LDEC      (1<<5)
#define BT848_CONTROL_CBSENSE   (1<<4)
#define BT848_CONTROL_CON_MSB   (1<<2)
#define BT848_CONTROL_SAT_U_MSB (1<<1)
#define BT848_CONTROL_SAT_V_MSB (1<<0)

#define BT848_CONTRAST_LO      0x030
#define BT848_SAT_U_LO         0x034
#define BT848_SAT_V_LO         0x038
#define BT848_HUE              0x03C

#define BT848_E_SCLOOP         0x040
#define BT848_O_SCLOOP         0x0C0
#define BT848_SCLOOP_CAGC       (1<<6)
#define BT848_SCLOOP_CKILL      (1<<5)
#define BT848_SCLOOP_HFILT_AUTO (0<<3)
#define BT848_SCLOOP_HFILT_CIF  (1<<3)
#define BT848_SCLOOP_HFILT_QCIF (2<<3)
#define BT848_SCLOOP_HFILT_ICON (3<<3)

#define BT848_WC_UP            0x044
#define BT848_VTOTAL_LO        0x0B0
#define BT848_VTOTAL_HI        0x0B4
#define BT848_DVSIF            0x0FC

#define BT848_OFORM            0x048
#define BT848_OFORM_RANGE      (1<<7)
#define BT848_OFORM_CORE0      (0<<5)
#define BT848_OFORM_CORE8      (1<<5)
#define BT848_OFORM_CORE16     (2<<5)
#define BT848_OFORM_CORE32     (3<<5)

#define BT848_E_VSCALE_HI      0x04C
#define BT848_O_VSCALE_HI      0x0CC
#define BT848_VSCALE_YCOMB     (1<<7)
#define BT848_VSCALE_COMB      (1<<6)
#define BT848_VSCALE_INT       (1<<5)
#define BT848_VSCALE_HI        15

#define BT848_E_VSCALE_LO      0x050
#define BT848_O_VSCALE_LO      0x0D0
#define BT848_TEST             0x054
#define BT848_ADELAY           0x060
#define BT848_BDELAY           0x064

#define BT848_ADC              0x068
#define BT848_ADC_RESERVED     (2<<6)
#define BT848_ADC_SYNC_T       (1<<5)
#define BT848_ADC_AGC_EN       (1<<4)
#define BT848_ADC_CLK_SLEEP    (1<<3)
#define BT848_ADC_Y_SLEEP      (1<<2)
#define BT848_ADC_C_SLEEP      (1<<1)
#define BT848_ADC_CRUSH        (1<<0)

#define BT848_E_VTC            0x06C
#define BT848_O_VTC            0x0EC
#define BT848_VTC_HSFMT        (1<<7)
#define BT848_VTC_VFILT_2TAP   0
#define BT848_VTC_VFILT_3TAP   1
#define BT848_VTC_VFILT_4TAP   2
#define BT848_VTC_VFILT_5TAP   3

#define BT848_SRESET           0x07C

#define BT848_COLOR_FMT             0x0D4
#define BT848_COLOR_FMT_O_RGB32     (0<<4)
#define BT848_COLOR_FMT_O_RGB24     (1<<4)
#define BT848_COLOR_FMT_O_RGB16     (2<<4)
#define BT848_COLOR_FMT_O_RGB15     (3<<4)
#define BT848_COLOR_FMT_O_YUY2      (4<<4)
#define BT848_COLOR_FMT_O_BtYUV     (5<<4)
#define BT848_COLOR_FMT_O_Y8        (6<<4)
#define BT848_COLOR_FMT_O_RGB8      (7<<4)
#define BT848_COLOR_FMT_O_YCrCb422  (8<<4)
#define BT848_COLOR_FMT_O_YCrCb411  (9<<4)
#define BT848_COLOR_FMT_O_RAW       (14<<4)
#define BT848_COLOR_FMT_E_RGB32     0
#define BT848_COLOR_FMT_E_RGB24     1
#define BT848_COLOR_FMT_E_RGB16     2
#define BT848_COLOR_FMT_E_RGB15     3
#define BT848_COLOR_FMT_E_YUY2      4
#define BT848_COLOR_FMT_E_BtYUV     5
#define BT848_COLOR_FMT_E_Y8        6
#define BT848_COLOR_FMT_E_RGB8      7
#define BT848_COLOR_FMT_E_YCrCb422  8
#define BT848_COLOR_FMT_E_YCrCb411  9
#define BT848_COLOR_FMT_E_RAW       14

#define BT848_COLOR_FMT_RGB32       0x00
#define BT848_COLOR_FMT_RGB24       0x11
#define BT848_COLOR_FMT_RGB16       0x22
#define BT848_COLOR_FMT_RGB15       0x33
#define BT848_COLOR_FMT_YUY2        0x44
#define BT848_COLOR_FMT_BtYUV       0x55
#define BT848_COLOR_FMT_Y8          0x66
#define BT848_COLOR_FMT_RGB8        0x77
#define BT848_COLOR_FMT_YCrCb422    0x88
#define BT848_COLOR_FMT_YCrCb411    0x99
#define BT848_COLOR_FMT_RAW         0xee

#define BT848_COLOR_CTL                0x0D8
#define BT848_COLOR_CTL_EXT_FRMRATE    (1<<7)
#define BT848_COLOR_CTL_COLOR_BARS     (1<<6)
#define BT848_COLOR_CTL_RGB_DED        (1<<5)
#define BT848_COLOR_CTL_GAMMA          (1<<4)
#define BT848_COLOR_CTL_WSWAP_ODD      (1<<3)
#define BT848_COLOR_CTL_WSWAP_EVEN     (1<<2)
#define BT848_COLOR_CTL_BSWAP_ODD      (1<<1)
#define BT848_COLOR_CTL_BSWAP_EVEN     (1<<0)

#define BT848_CAP_CTL                  0x0DC
#define BT848_CAP_CTL_DITH_FRAME       (1<<4)
#define BT848_CAP_CTL_CAPTURE_VBI_ODD  (1<<3)
#define BT848_CAP_CTL_CAPTURE_VBI_EVEN (1<<2)
#define BT848_CAP_CTL_CAPTURE_ODD      (1<<1)
#define BT848_CAP_CTL_CAPTURE_EVEN     (1<<0)

#define BT848_VBI_PACK_SIZE    0x0E0

#define BT848_VBI_PACK_DEL     0x0E4
#define BT848_VBI_PACK_DEL_VBI_HDELAY 0xfc
#define BT848_VBI_PACK_DEL_EXT_FRAME  2
#define BT848_VBI_PACK_DEL_VBI_PKT_HI 1

#define BT848_INT_STAT         0x100
#define BT848_INT_MASK         0x104

#define BT848_INT_ETBF         (1<<23)

#define BT848_INT_RISCS     (0xf<<28)
#define BT848_INT_RISC_EN   (1<<27)
#define BT848_INT_RACK      (1<<25)
#define BT848_INT_FIELD_ODD (1<<24)
#define BT848_INT_FIELD_EV  (1<<23)
#define BT848_INT_SCERR     (1<<19)
#define BT848_INT_OCERR     (1<<18)
#define BT848_INT_PABORT    (1<<17)
#define BT848_INT_RIPERR    (1<<16)
#define BT848_INT_PPERR     (1<<15)
#define BT848_INT_FDSR      (1<<14)
#define BT848_INT_FTRGT     (1<<13)
#define BT848_INT_FBUS      (1<<12)
#define BT848_INT_RISCI     (1<<11)
#define BT848_INT_GPINT     (1<<9)
#define BT848_INT_I2CDONE   (1<<8)
#define BT848_INT_VPRES     (1<<5)
#define BT848_INT_HLOCK     (1<<4)
#define BT848_INT_OFLOW     (1<<3)
#define BT848_INT_HSYNC     (1<<2)
#define BT848_INT_VSYNC     (1<<1)
#define BT848_INT_FMTCHG    (1<<0)


#define BT848_GPIO_DMA_CTL             0x10C
#define BT848_GPIO_DMA_CTL_GPINTC      (1<<15)
#define BT848_GPIO_DMA_CTL_GPINTI      (1<<14)
#define BT848_GPIO_DMA_CTL_GPWEC       (1<<13)
#define BT848_GPIO_DMA_CTL_GPIOMODE    (3<<11)
#define BT848_GPIO_DMA_CTL_GPCLKMODE   (1<<10)
#define BT848_GPIO_DMA_CTL_PLTP23_4    (0<<6)
#define BT848_GPIO_DMA_CTL_PLTP23_8    (1<<6)
#define BT848_GPIO_DMA_CTL_PLTP23_16   (2<<6)
#define BT848_GPIO_DMA_CTL_PLTP23_32   (3<<6)
#define BT848_GPIO_DMA_CTL_PLTP1_4     (0<<4)
#define BT848_GPIO_DMA_CTL_PLTP1_8     (1<<4)
#define BT848_GPIO_DMA_CTL_PLTP1_16    (2<<4)
#define BT848_GPIO_DMA_CTL_PLTP1_32    (3<<4)
#define BT848_GPIO_DMA_CTL_PKTP_4      (0<<2)
#define BT848_GPIO_DMA_CTL_PKTP_8      (1<<2)
#define BT848_GPIO_DMA_CTL_PKTP_16     (2<<2)
#define BT848_GPIO_DMA_CTL_PKTP_32     (3<<2)
#define BT848_GPIO_DMA_CTL_RISC_ENABLE (1<<1)
#define BT848_GPIO_DMA_CTL_FIFO_ENABLE (1<<0)

#define BT848_I2C              0x110
#define BT848_I2C_DIV          (0xf<<4)
#define BT848_I2C_SYNC         (1<<3)
#define BT848_I2C_W3B	       (1<<2)
#define BT848_I2C_SCL          (1<<1)
#define BT848_I2C_SDA          (1<<0)


#define BT848_RISC_STRT_ADD    0x114
#define BT848_GPIO_OUT_EN      0x118
#define BT848_GPIO_OUT_EN_HIBYTE      0x11A
#define BT848_GPIO_REG_INP     0x11C
#define BT848_GPIO_REG_INP_HIBYTE     0x11E
#define BT848_RISC_COUNT       0x120
#define BT848_GPIO_DATA        0x200
#define BT848_GPIO_DATA_HIBYTE 0x202


/* Bt848 RISC commands */

/* only for the SYNC RISC command */
#define BT848_FIFO_STATUS_FM1  0x06
#define BT848_FIFO_STATUS_FM3  0x0e
#define BT848_FIFO_STATUS_SOL  0x02
#define BT848_FIFO_STATUS_EOL4 0x01
#define BT848_FIFO_STATUS_EOL3 0x0d
#define BT848_FIFO_STATUS_EOL2 0x09
#define BT848_FIFO_STATUS_EOL1 0x05
#define BT848_FIFO_STATUS_VRE  0x04
#define BT848_FIFO_STATUS_VRO  0x0c
#define BT848_FIFO_STATUS_PXV  0x00

#define BT848_RISC_RESYNC      (1<<15)

/* WRITE and SKIP */
/* disable which bytes of each DWORD */
#define BT848_RISC_BYTE0       (1<<12)
#define BT848_RISC_BYTE1       (1<<13)
#define BT848_RISC_BYTE2       (1<<14)
#define BT848_RISC_BYTE3       (1<<15)
#define BT848_RISC_BYTE_ALL    (0x0f<<12)
#define BT848_RISC_BYTE_NONE   0
/* cause RISCI */
#define BT848_RISC_IRQ         (1<<24)
/* RISC command is last one in this line */
#define BT848_RISC_EOL         (1<<26)
/* RISC command is first one in this line */
#define BT848_RISC_SOL         (1<<27)

#define BT848_RISC_WRITE       (0x01<<28)
#define BT848_RISC_SKIP        (0x02<<28)
#define BT848_RISC_WRITEC      (0x05<<28)
#define BT848_RISC_JUMP        (0x07<<28)
#define BT848_RISC_SYNC        (0x08<<28)

#define BT848_RISC_WRITE123    (0x09<<28)
#define BT848_RISC_SKIP123     (0x0a<<28)
#define BT848_RISC_WRITE1S23   (0x0b<<28)


// MSP34x0 definitions
#define MSP_CONTROL 0x00 // Software reset
#define MSP_TEST	0x01 // Internal use
#define MSP_WR_DEM	0x10 // Write demodulator
#define MSP_RD_DEM	0x11 // Read demodulator
#define MSP_WR_DSP	0x12 // Write DSP
#define MSP_RD_DSP	0x13 // Read DSP

WORD m_wWindowWidth;
WORD m_wWindowHeight;

DWORD *m_pRiscJump;
DWORD *m_pRiscVBIOdd;
DWORD *m_pRiscVBIEven;
DWORD *m_pRiscFrameOdd;
DWORD *m_pRiscFrameEven;
DWORD *m_pRiscVBIOdd2;
DWORD *m_pRiscVBIEven2;
DWORD *m_pRiscFrameOdd2;
DWORD *m_pRiscFrameEven2;
DWORD *m_pRiscVBIOdd3;
DWORD *m_pRiscVBIEven3;
DWORD *m_pRiscFrameOdd3;
DWORD *m_pRiscFrameEven3;
DWORD *m_pRiscVBIOdd4;
DWORD *m_pRiscVBIEven4;
DWORD *m_pRiscFrameOdd4;
DWORD *m_pRiscFrameEven4;
DWORD *m_pRiscVBIOdd5;
DWORD *m_pRiscVBIEven5;
DWORD *m_pRiscFrameOdd5;
DWORD *m_pRiscFrameEven5;

extern HDC                  DIB_Hdc;

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


struct TTunerType Tuners[9] =
{
    { 2244/*16*140.25*/,7412/*16*463.25*/,0x02,0x04,0x01,0x8e,0xc2,623},
	{ 2244/*16*140.25*/,7412/*16*463.25*/,0xa0,0x90,0x30,0x8e,0xc0,623},
	{ 2516/*16*157.25*/,7220/*16*451.25*/,0xA0,0x90,0x30,0x8e,0xc0,732},
	{ 2692/*16*168.25*/,7156/*16*447.25*/,0xA7,0x97,0x37,0x8e,0xc0,623},
	{ 0        ,0        ,0x00,0x00,0x00,0x00,0x00,000},
	{ 2692/*16*168.25*/,7156/*16*447.25*/,0xA0,0x90,0x30,0x8e,0xc0,623},
	{ 2516/*16*157.25*/,7412/*16*463.25*/,0x02,0x04,0x01,0x8e,0xc2,732},
	{ 2720/*16*170.00*/,7200/*16*450.00*/,0xa0,0x90,0x30,0x8e,0xc2,623},
	{ 0        ,0        ,0x00,0x00,0x00,0x00,0x00,000},

}; 

extern int TunerType;

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


extern struct TTVSetting TVSettings[7];

extern int VideoSource;
extern int TVTYPE;

//0		TVFormat_PAL_BDGHI,
//1		TVFormat_NTSC,
//2		TVFormat_SECAM,
//3		TVFormat_PAL_M,
//4		TVFormat_PAL_N,


unsigned int ManuellAudio[7] = 	{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,0x0000,0x0000}; /* Eigener */

static const BYTE AudioChannel[][5] = 
{
	{ 0x02, 0x00, 0x00, 0x00, 0x0a}, /* MIRO */
	{ 0x00, 0x01, 0x02, 0x03, 0x04}, /* Hauppauge */
	{ 0x04, 0x00, 0x02, 0x03, 0x01}, /* STB */
	{ 0x00, 0x01, 0x02, 0x03, 0x04}, /* Intel??? */
	{ 0x00, 0x01, 0x02, 0x03, 0x04}, /* Diamond DTV2000 */
	{ 0x0c, 0x00, 0x0b, 0x0b, 0x00}, /* AVerMedia TVPhone */
};

extern int CardType;
//0		ColourFormat_RGB32,
//1		ColourFormat_RGB24,
//2		ColourFormat_RGB16,
//3		ColourFormat_RGB15,
//4		ColourFormat_YUV2,
//5		ColourFormat_BtYUV,
//6		ColourFormat_Y8,
//7		ColourFormat_RGB8,
//8		ColourFormat_YCrCb_422,
//9		ColourFormat_YCrCb_411


int AudioSource;
int CAudioSource=0;

/*
0		Audio_Tuner,
1		Audio_Radio,
2		Audio_External,
3		Audio_Internal,
4		Audio_Off,
5		Audio_On,
0x80	Audio_Mute = 0x80,
0x81	Audio_UnMute = 0x81
*/	
int CurrentCapture;
int CaptureFlags=0;
// 0  Capture_Frame_On
// 1  Capture_Frame_Off
// 2  Capture_VBI_On
// 3  Capture_VBI_Off
// 4  Capture_Pause_On
// 5  Capture_Pause_Off


// Audio



BYTE AudioDeviceWrite, AudioDeviceRead;

#define WriteDem(wAddr,wData) Audio_WriteMSP(MSP_WR_DEM,wAddr,wData) // I2C_MSP3400C_DEM
#define WriteDSP(wAddr,wData) Audio_WriteMSP(MSP_WR_DSP,wAddr,wData) // I2C_MSP3400C_DEM
#define ReadDem(wAddr) Audio_ReadMSP(MSP_RD_DEM,wAddr)  // I2C_MSP3400C_DFP
#define ReadDSP(wAddr) Audio_ReadMSP(MSP_RD_DSP,wAddr)  // I2C_MSP3400C_DFP

volatile DWORD *IC2_BaseAddress;

#define I2C_DELAY 0
#define I2C_TIMING (0x7<<4)
#define I2C_COMMAND (I2C_TIMING | BT848_I2C_SCL | BT848_I2C_SDA)

#define VBI_SPL 2044

BYTE TunerDeviceWrite, TunerDeviceRead;

extern BOOL MSPToneControl;

extern BOOL Sync_Irq;

LPDIRECTDRAWSURFACE     lpDDSurface;
// OverLay 
LPDIRECTDRAWSURFACE     lpDDOverlay;

extern LPVOID lpOverlay;
BOOL Can_Overlay=FALSE;
BOOL Can_Overlay_Strech=FALSE;
BOOL Can_ColorKey=FALSE;

extern int OverlayPitch;


typedef enum
{
    BT8X8_MODE_BYTE   = 0,
    BT8X8_MODE_WORD   = 1,
    BT8X8_MODE_DWORD  = 2
} BT8X8_MODE;


typedef enum
{
    BT8X8_AD_BAR0 = 0,
    BT8X8_AD_BAR1 = 1,
    BT8X8_AD_BAR2 = 2,
    BT8X8_AD_BAR3 = 3,
    BT8X8_AD_BAR4 = 4,
    BT8X8_AD_BAR5 = 5,
    BT8X8_AD_EPROM = 6,
} BT8X8_ADDR;

enum { BT8X8_ITEMS = 7 };

typedef struct BT8X8_STRUCT *BT8X8_HANDLE;


typedef struct
{
    DWORD dwCounter;   // number of interrupts received
    DWORD dwLost;      // number of interrupts not yet dealt with
    BOOL fStopped;     // was interrupt disabled during wait
} BT8X8_INT_RESULT;

typedef void (WINAPI *BT8X8_INT_HANDLER)( BT8X8_HANDLE hBT8X8, BT8X8_INT_RESULT *intResult);

typedef struct
{
    WD_INTERRUPT Int;
    HANDLE hThread;
    WD_TRANSFER Trans[6];
    BT8X8_INT_HANDLER funcIntHandler;
} BT8X8_INT_INTERRUPT;

typedef struct
{
    DWORD index;
    DWORD dwMask;
    BOOL  fIsMemory;
    BOOL  fActive;
} BT8X8_ADDR_DESC;

typedef struct BT8X8_STRUCT
{
    HANDLE hWD;
    BOOL   fUseInt;
    BT8X8_INT_INTERRUPT Int;
    WD_PCI_SLOT pciSlot;
    BT8X8_ADDR_DESC addrDesc[BT8X8_ITEMS];
    WD_CARD_REGISTER cardReg;
} BT8X8_STRUCT;

// options for BT8X8_Open
enum { BT8X8_OPEN_USE_INT =   0x1 };

enum {
    PCI_IDR  = 0x00,
    PCI_CR   = 0x04,
    PCI_SR   = 0x06,
    PCI_REV  = 0x08,
    PCI_CCR  = 0x09,
    PCI_LSR  = 0x0c,
    PCI_LTR  = 0x0d,
    PCI_HTR  = 0x0e,
    PCI_BISTR= 0x0f,
    PCI_BAR0 = 0x10,
    PCI_BAR1 = 0x14,
    PCI_BAR2 = 0x18,
    PCI_BAR3 = 0x1c,
    PCI_BAR4 = 0x20,
    PCI_BAR5 = 0x24,
    PCI_CIS  = 0x28,
    PCI_SVID = 0x2c,
    PCI_SID  = 0x2e,
    PCI_ERBAR= 0x30,
    PCI_ILR  = 0x3c,
    PCI_IPR  = 0x3d,
    PCI_MGR  = 0x3e,
    PCI_MLR  = 0x3f
};


extern BOOL LoadDeviceDriver( const TCHAR * Name, const TCHAR * Path, HANDLE * lphDevice,BOOL Install);
extern BOOL InstallDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName, IN LPCTSTR ServiceExe );
extern BOOL UnloadDeviceDriver( const TCHAR * Name,BOOL DRemove );
extern BOOL StartDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName );
extern BOOL StopDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName );
extern BOOL RemoveDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName );


void Start_VBI();
void Stop_VBI();
void DoVBIatEvent();

extern void RGBOutThread( );
extern void RGBOutThreadHalf( );

extern void YUVOutThread( ); 
extern void YUVOutThreadHalf( ); 

extern void NagraYUVHalfOutThread( );
extern void NagraYUVOutThread( );

extern void VCYUVHalfOutThread( ); 
extern void VCDecoderYUVHalfOutThread( ); 

extern void DX_Lock_Thread( );

extern BOOL Init_Screen_Struct( );


int BT8X8_Open (BT8X8_HANDLE *phBT8X8, DWORD dwVendorID, DWORD dwDeviceID,  DWORD options,BOOL Lock);
void BT8X8_Close(BT8X8_HANDLE hBT8X8);
DWORD BT8X8_CountCards (DWORD dwVendorID, DWORD dwDeviceID);
BOOL BT8X8_IsAddrSpaceActive(BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace);

// General read/write function
void BT8X8_ReadWriteBlock(BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace, DWORD dwOffset, BOOL fRead, PVOID buf, DWORD dwBytes, BT8X8_MODE mode);
BYTE BT8X8_ReadByte (BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace, DWORD dwOffset);
WORD BT8X8_ReadWord (BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace, DWORD dwOffset);
DWORD BT8X8_ReadDword (BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace, DWORD dwOffset);
void BT8X8_WriteByte (BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace, DWORD dwOffset, BYTE data);
void BT8X8_WriteWord (BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace, DWORD dwOffset, WORD data);
void BT8X8_WriteDword (BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace, DWORD dwOffset, DWORD data);
// handle interrupts
// handle interrupts
BOOL BT8X8_IntIsEnabled (BT8X8_HANDLE hBT8X8);
BOOL BT8X8_IntEnable (BT8X8_HANDLE hBT8X8, BT8X8_INT_HANDLER funcIntHandler);
void BT8X8_IntDisable (BT8X8_HANDLE hBT8X8);
// access to PCI configuration registers
void BT8X8_WritePCIReg(BT8X8_HANDLE hBT8X8, DWORD dwReg, DWORD dwData);
DWORD BT8X8_ReadPCIReg(BT8X8_HANDLE hBT8X8, DWORD dwReg);
BOOL BT8X8_DetectCardElements(BT8X8_HANDLE hBT8X8);

BYTE DeHam(BYTE byte);
BYTE ReverseDeHam(BYTE b);
void StorePage(BYTE mag, BYTE page);
void StorePacket30();

#define GetBit(val,bit,mask) (BYTE)(((val)>>(bit))&(mask))

BYTE current_page[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };


unsigned int Nagra_sample_bits[48]={
 		0,
		7,
		15,
		23,
		31,
		39,
		47,
		55,
		63,
		71,
		79,
		87,
		95,
		103,
		111,
		119,
		126,
		134,
		142,
		150,
		158,
		166,
		174,
		182,
		190,
		198,
		206,
		214,
		222,
		230,
		238,
		245,
		253,
		261,
		269,
		277,
		285,
		293,
		301,
		309,
		317,
		325,
		333,
		341,
		349,
		357,
		364,
		372};
int Nagra_offset=4;

// this string is set to an error message, if one occurs
HANDLE		Bt_Device_Handle;

BT8X8_HANDLE hBT8X8=NULL;
extern BT8X8_HANDLE hSAA7146;

extern char VESTyp[30];
extern char DVBTyp[30];
extern char BTTyp[30];
extern char MSPStatus[30];
extern char TunerStatus[30];
char BTVendorID[10];
char BTDeviceID[10];
extern char DVBVendorID[10];
extern char DVBDeviceID[10];
extern int DVB_AUDIO;
BOOL NT=FALSE;

extern WD_DMA Risc_dma;
extern WD_DMA Vbi_dma[5];
extern WD_DMA *Display_dma[5];
extern WD_DMA Burst_dma[5];
extern WD_DMA *Surface_dma;
extern WD_DMA *Overlay_dma;
extern WD_DMA DVB_Display_dma;


extern __int64  CurrentLowValue;
extern short nLevelLow; 
extern short nLevelHigh;

extern int TestCount;
extern BOOL DoTest;
extern BOOL ModeRAWSurface;
extern BOOL ModeRAWOverlay;


HANDLE VBI_Event=NULL;
BOOL StopVBI;
HANDLE Decode_Event=NULL;
HANDLE Decode_Event_Stop;


extern int CurrentMode;
extern int NextFrame;
static BYTE sync[24] = { 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 
                         0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 
                         0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00 };



/* This macro is allowed for *constants* only, gcc must calculate it
   at compile time.  Remember -- no floats in kernel mode */
#define MSP_CARRIER(freq) ((int)((float)(freq/18.432)*(1<<24)))

#define MSP_MODE_AM_DETECT   0
#define MSP_MODE_FM_RADIO    2
#define MSP_MODE_FM_TERRA    3
#define MSP_MODE_FM_SAT      4
#define MSP_MODE_FM_NICAM1   5
#define MSP_MODE_FM_NICAM2   6

static struct MSP_INIT_DATA_DEM {
	int fir1[6];
	int fir2[6];
	int cdo1;
	int cdo2;
	int ad_cv;
	int mode_reg;
	int dfp_src;
	int dfp_matrix;
} msp_init_data[] = {
	/* AM (for carrier detect / msp3400) */
	{ { 75, 19, 36, 35, 39, 40 }, { 75, 19, 36, 35, 39, 40 },
	  MSP_CARRIER(5.5), MSP_CARRIER(5.5),
	  0x00d0, 0x0500,   0x0020, 0x3000},

	/* AM (for carrier detect / msp3410) */
	{ { -1, -1, -8, 2, 59, 126 }, { -1, -1, -8, 2, 59, 126 },
	  MSP_CARRIER(5.5), MSP_CARRIER(5.5),
	  0x00d0, 0x0100,   0x0020, 0x3000},

	/* FM Radio */
	{ { -8, -8, 4, 6, 78, 107 }, { -8, -8, 4, 6, 78, 107 },
	  MSP_CARRIER(10.7), MSP_CARRIER(10.7),
	  0x00d0, 0x0480, 0x0020, 0x3000 },

	/* Terrestial FM-mono + FM-stereo */
	{ {  3, 18, 27, 48, 66, 72 }, {  3, 18, 27, 48, 66, 72 },
	  MSP_CARRIER(5.5), MSP_CARRIER(5.5),
	  0x00d0, 0x0480,   0x0030, 0x3000},

	/* Sat FM-mono */
	{ {  1,  9, 14, 24, 33, 37 }, {  3, 18, 27, 48, 66, 72 },
	  MSP_CARRIER(6.5), MSP_CARRIER(6.5),
	  0x00c6, 0x0480,   0x0000, 0x3000},

	/* NICAM B/G, D/K */
	{ { -2, -8, -10, 10, 50, 86 }, {  3, 18, 27, 48, 66, 72 },
	  MSP_CARRIER(5.5), MSP_CARRIER(5.5),
	  0x00d0, 0x0040,   0x0120, 0x3000},

	/* NICAM I */
	{ {  2, 4, -6, -4, 40, 94 }, {  3, 18, 27, 48, 66, 72 },
	  MSP_CARRIER(6.0), MSP_CARRIER(6.0),
	  0x00d0, 0x0040,   0x0120, 0x3000},
};


int carrier_detect_main[4] = {
	/* main carrier */
	 MSP_CARRIER(4.5),   // 4.5   NTSC                 
	 MSP_CARRIER(5.5),   // 5.5   PAL B/G              
	 MSP_CARRIER(6.0),   // 6.0   PAL I                
	 MSP_CARRIER(6.5),   // 6.5   PAL D/K + SAT + SECAM
};

int carrier_detect[8] = {
	/* PAL B/G */
	 MSP_CARRIER(5.7421875), // 5.742 PAL B/G FM-stereo
	 MSP_CARRIER(5.85),      // 5.85  PAL B/G NICAM
	/* PAL SAT / SECAM */
	 MSP_CARRIER(5.85),      // 5.85  PAL D/K NICAM
	 MSP_CARRIER(6.2578125), //6.25  PAL D/K1 FM-stereo
	 MSP_CARRIER(6.7421875), //6.74  PAL D/K2 FM-stereo
	 MSP_CARRIER(7.02),      //7.02  PAL SAT FM-stereo s/b
	 MSP_CARRIER(7.20),      //7.20  PAL SAT FM-stereo s
	 MSP_CARRIER(7.38),      //7.38  PAL SAT FM-stereo b
};

#define VIDEO_SOUND_MONO	1
#define VIDEO_SOUND_STEREO	2
#define VIDEO_SOUND_LANG1	3
#define VIDEO_SOUND_LANG2	4

extern HWND hwndAudioField;
extern HWND hwndFPSField;

int MSPMode;
int MSPStereo;
int MSPNewStereo;
int MSPAutoDetectValue;
BOOL MSPNicam;
char MSPVersion[16];
int MSPMajorMode;
int MSPMinorMode;
BOOL AutoStereoSelect=TRUE;

// 0		ToneControl_BassTreble,
// 1		ToneControl_Equalizer

int CurrentIrq=0;
int VCIrqCount=0;

unsigned char amag=0x00;

#define FLEN 50

unsigned char fifo[FLEN][37];
int fifo_in=0;
int fifo_out=0;

FILE *winbisfp=NULL;
char WINIBIS_DIR[256];

// Erweiterte Treiberumgebung

extern BOOL LoadExternBTDriver;
extern BOOL ExternBTDriver;

WD_KERNEL_PLUGIN BTKernelPlugIn;

unsigned char Used_IRQ_List[256];
