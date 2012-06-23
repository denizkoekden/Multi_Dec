/*********************************************************************************
 *                                                                               *
 * dvb.h: Hardware-Parts for the DVB PCI card und saa7146 device driver          *
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
 *  saa7146 device driver                                                                             *
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

#include <winioctl.h>
#include "globals.h"


extern HANDLE hInst;

extern BOOL Keep_Driver_Loaded;

#define HILO(x) (x##_hi << 8 | x##_lo)


struct descr_gen_struct {
	unsigned char descriptor_tag;
	unsigned char descriptor_length;
};
typedef struct descr_gen_struct descr_gen_t;

#define get_descr(x) (((descr_gen_t *) x)->descriptor_tag)
#define get_descr_len(x) (((descr_gen_t *) x)->descriptor_length)

extern BOOL DVB_Chip;

typedef unsigned long PHYS;

typedef struct AllocData
{
	DWORD Size;		// Size to allocate/allocated 
	LPVOID Linear;	// Linear address 
	PHYS Physical;	// Physical address 
};


extern BOOL Capture_Event_EIT;
extern BOOL Capture_TID;
extern BOOL Capture_Digi_Text;
extern BOOL WRITE_LOG;
extern BOOL SAVE_BOOT;

extern HANDLE DVBDevice;
extern BOOL DVBOK;
HANDLE DVB_Plugin_Handle=NULL;

int DVBDecoderType=-1;
int VES_TYPE=-1;
int DVB_AUDIO=-1;

unsigned char FirmID[16];;
char FIRM_DIR[256];

BOOL Was_TeleText=FALSE;

unsigned char Init_VES_1893_Tab[55] =
{
        0x01, 0xA4, 0x35, 0x81, 0x2A, 0x0d, 0x55, 0xC4,
        0x09, 0x69, 0x00, 0x86, 0x4c, 0x28, 0x7F, 0x00,
        0x00, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	
        0x80, 0x00, 0x31, 0xb0, 0x14, 0x00, 0xDC, 0x20,
        0x81, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x55, 0x00, 0x00, 0x7f, 0x00
};

unsigned char Init_VES_1820_Tab[55] =
{
        0x69, 0x6A, 0x9B, 0x0A, 0x52, 0x46, 0x26, 0x1A,
		0x43, 0x6A, 0xAA, 0xAA, 0x1E, 0x85, 0x43, 0x28,
		0xE0, 0x00, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
		0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x40
};

extern int VES1820_Inversion;


unsigned char Init_TDA8083_Tab[42] = 
{
	0x04, 0x00, 0x4a, 0x79, 0x04, 0x00, 0xff, 0xea,
	0x48, 0x42, 0x79, 0x60, 0x70, 0x52, 0x9a, 0x10,
	0x0e, 0x10, 0xf2, 0xa7, 0x93, 0x0b, 0x05, 0xc8,   
	0x9d, 0x00, 0x42, 0x80, 0x00, 0x60, 0x40, 0x00,
	0x00, 0x75, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00 };



unsigned char Init_STV_0299_Tab[152] = 
{
	0x01, 0x15,   // M: 0x14 DIRCLK: 0 K:0
	0x02, 0x00,   // P: 0  SERCLK: 0 VCO:ON  STDBY:0

	0x03, 0x00,
	0x04, 0x7d,   // F22FR, F22=384MHz/(128*136) 
	0x05, 0x05,   // SDAT:0 SCLT:0 I2CT:0
	0x06, 0x00,   // DAC mode and MSB 
	0x07, 0x00,   // DAC LSB
	0x08, 0x00,   // DiSEqC
	0x09, 0x00,
	0x0a, 0x00, 
	0x0c, 0x51,   // QPSK reverse:0  Nyquist:0 OP0 val:1 OP0 con:1 OP1 val:1 OP1 con:1 
	0x0d, 0x82,
	0x0e, 0x23,
	0x0f, 0x12,

	0x10, 0x34,   // AGC2
	0x11, 0x84,   // must be this way, or not?
	0x12, 0xb9,   // Lock detect: -64  Carrier freq detect:on
	0x13, 0xb6,   // alpha_car b:4 a:0  noise est:256ks  derot:on 
	0x14, 0x93,   // beat carc:0 d:0 e:0xf  phase detect algo: 1
	0x15, 0xc9,   // lock detector threshold

	0x16, 0xff,
	0x17, 0xff,
	0x18, 0xff,
	0x19, 0xff,

	0x1a, 0x00,

	0x1b, 0xff,
	0x1c, 0xff,
	0x1d, 0xff,
	0x1e, 0xff,

	0x22, 0x00,
	0x23, 0x00,
	0x24, 0xff,
	0x25, 0xff,
	0x26, 0xff,

	0x28, 0x00,  // out imp: normal  out type: parallel FEC mode:0 
	0x29, 0x1e,  // 1/2 threshold
	0x2a, 0x14,  // 2/3 threshold
	0x2b, 0x0f,  // 3/4 threshold
	0x2c, 0x09,  // 5/6 threshold
	0x2d, 0x05,  // 7/8 threshold

	0x31, 0x1f,  // test all FECs 
	
	0x32, 0x19,  // viterbi and synchro search
	0x33, 0xfc,  // rs control
	0x34, 0x13,  // error control
	
	0x0b, 0x00, 
	0x27, 0x00, 
	0x2e, 0x00, 
	0x2f, 0x00, 
	0x30, 0x00,
	0x35, 0x00, 
	0x36, 0x00, 
	0x37, 0x00, 
	0x38, 0x00, 
	0x39, 0x00, 
	0x3a, 0x00, 
	0x3b, 0x00, 
	0x3c, 0x00, 
	0x3d, 0x00, 
	0x3e, 0x00, 
	0x3f, 0x00,
	0x40, 0x00, 
	0x41, 0x00, 
	0x42, 0x00, 
	0x43, 0x00,
	0x44, 0x00, 
	0x45, 0x00, 
	0x46, 0x00, 
	0x47, 0x00, 
	0x48, 0x00, 
	0x49, 0x00, 
	0x4a, 0x00, 
	0x4b, 0x00, 
	0x4c, 0x00, 
	0x4d, 0x00, 
	0x4e, 0x00, 
	0x4f, 0x00 
};




#define I2C_M_TEN	0x10	/* we have a ten bit chip address	*/
#define I2C_M_RD	0x01
#define I2C_M_PROBE	0x20

extern HDC                  DIB_Hdc;


struct i2c_msg {
	unsigned short addr;	/* slave address			*/
	unsigned short flags;		
	short len;		/* msg length				*/
	char *buf;		/* pointer to msg data			*/
};




//volatile DWORD *IC2_BaseAddress;





typedef enum
{
    SAA7146_MODE_BYTE   = 0,
    SAA7146_MODE_WORD   = 1,
    SAA7146_MODE_DWORD  = 2
} SAA7146_MODE;


typedef enum
{
    SAA7146_AD_BAR0 = 0,
    SAA7146_AD_BAR1 = 1,
    SAA7146_AD_BAR2 = 2,
    SAA7146_AD_BAR3 = 3,
    SAA7146_AD_BAR4 = 4,
    SAA7146_AD_BAR5 = 5,
    SAA7146_AD_EPROM = 6,
    SAA7146_ITEMS = 7
} SAA7146_ADDR;

typedef struct SAA7146_STRUCT *SAA7146_HANDLE;


typedef struct
{
    DWORD dwCounter;   // number of interrupts received
    DWORD dwLost;      // number of interrupts not yet dealt with
    BOOL fStopped;     // was interrupt disabled during wait
} SAA7146_INT_RESULT;
typedef void (WINAPI *SAA7146_INT_HANDLER)( SAA7146_HANDLE hSAA7146, SAA7146_INT_RESULT *intResult);

typedef struct
{
    WD_INTERRUPT Int;
    HANDLE hThread;
    WD_TRANSFER Trans[6];
    SAA7146_INT_HANDLER funcIntHandler;
} SAA7146_INT_INTERRUPT;

typedef struct
{
    DWORD index;
    DWORD dwMask;
    BOOL  fIsMemory;
    BOOL  fActive;
} SAA7146_ADDR_DESC;

typedef struct SAA7146_STRUCT
{
    HANDLE hWD;
    BOOL   fUseInt;
    SAA7146_INT_INTERRUPT Int;
    WD_PCI_SLOT pciSlot;
    SAA7146_ADDR_DESC addrDesc[SAA7146_ITEMS];
    WD_CARD_REGISTER cardReg;
} SAA7146_STRUCT;

// options for SAA7146_Open
enum { SAA7146_OPEN_USE_INT =   0x1 };

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

char DVBVendorID[10];
char DVBDeviceID[10];

extern BOOL NT;
extern HWND SplashWnd;
extern char OrgDVBDriverName[128];


int SAA7146_Open (SAA7146_HANDLE *phSAA7146, DWORD dwVendorID, DWORD dwDeviceID,  DWORD options,BOOL Lock);
void SAA7146_Close(SAA7146_HANDLE hSAA7146);
DWORD SAA7146_CountCards (DWORD dwVendorID, DWORD dwDeviceID);
BOOL SAA7146_IsAddrSpaceActive(SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace);

// General read/write function
void SAA7146_ReadWriteBlock(SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset, BOOL fRead, PVOID buf, DWORD dwBytes, SAA7146_MODE mode);
BYTE SAA7146_ReadByte (SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset);
WORD SAA7146_ReadWord (SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset);
DWORD SAA7146_ReadDword (SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset);
void SAA7146_WriteByte (SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset, BYTE data);
void SAA7146_WriteWord (SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset, WORD data);
void SAA7146_WriteDword (SAA7146_HANDLE hSAA7146, SAA7146_ADDR addrSpace, DWORD dwOffset, DWORD data);
// handle interrupts
void WINAPI SAA7146_IntHandlerRoutine(SAA7146_HANDLE hSAA7146, SAA7146_INT_RESULT *intResult);
BOOL SAA7146_IntIsEnabled (SAA7146_HANDLE hSAA7146);
BOOL SAA7146_IntEnable (SAA7146_HANDLE hSAA7146, SAA7146_INT_HANDLER funcIntHandler);
void SAA7146_IntDisable (SAA7146_HANDLE hSAA7146);
// access to PCI configuration registers
void SAA7146_WritePCIReg(SAA7146_HANDLE hSAA7146, DWORD dwReg, DWORD dwData);
DWORD SAA7146_ReadPCIReg(SAA7146_HANDLE hSAA7146, DWORD dwReg);
BOOL SAA7146_DetectCardElements(SAA7146_HANDLE hSAA7146);

extern HANDLE	Device_Handle;

char OSD_PROG_Name[128];

extern unsigned int SRates[16];

extern BOOL Has_DVB;
extern int Current_Mode;
extern int CurrentProgramm;
extern int LastProgramm;
extern int LastRealProg;
extern int TunerType;

SAA7146_HANDLE hSAA7146=NULL;

extern BOOL System_In_Mute;
extern int System_in_Sync;

extern char DVBTyp[30];
char VESTyp[30];
char VersionString[255];

extern HWND hWnd;

extern BOOL SaveSettings;

extern WD_DMA DVB_Display_dma;
extern WD_DMA rps0_mem_dma;
extern WD_DMA debi_mem_dma;
extern WD_DMA *Surface_dma;
extern WD_DMA *Overlay_dma;

extern WD_DMA Record_mem_dma;
extern WD_DMA BitMap_mem_dma;
extern WD_DMA Text_mem_dma;
extern WD_DMA CA_mem_dma;
extern WD_DMA CI_mem_dma;
extern WD_DMA IPMPE_mem_dma;
extern WD_DMA Ir_mem_dma;
extern WD_DMA Debug_mem_dma;



extern BOOL ModeRAWSurface;
extern BOOL ModeRAWOverlay;

extern HWND hwndFPSField;

int debiwait_maxwait = 1000;
int debitype;
int debilen;
int debifilter;

HANDLE OSD_Thread=NULL;
BOOL OSD_VOLUME=FALSE;
BOOL OSD_CHANNELCHANGE=FALSE;

extern BOOL USE_MIXER;
extern int  MIXER_LINKER_KANAL;
extern int  MIXER_RECHTER_KANAL;
extern int MixerVolumeStep;
extern int MixerVolumeMax;


extern BOOL HalfTVMode;

#define I2C_MEM_SIZE		0x001000	/* 4096 */
#define	RPS_MEM_SIZE		0x002000	/* 8192 Bytes */
#define	DEBI_MEM_SIZE		0x001000	/* 4096 Bytes */
#define	RECORD_PLAY_MEM		0x001000	/* 4096 Bytes */

BOOL DEBICOPY=TRUE;

DWORD i2c_mem[I2C_MEM_SIZE];
unsigned char ves_addr=(0x12 >> 1);  
unsigned char ves_flags=0;
unsigned char ves_ctr;
unsigned char ves_pwm,ves_reg0,ves_con;
unsigned char ves_inversion;
unsigned char ves_fec;
unsigned char ves_bclk;

DWORD DEBINOSWAP = 0x000e0000;
DWORD DEBISWAB = 0x001e0000;

#define SECTION_MODE_MASK	0x06
#define SECTION_SINGLE      0x00
#define SECTION_CYCLE		0x02
#define SECTION_CONTINUOUS	0x04

#define DATA_TYPE_MASK      0x39

#define DATA_PIPING_FLAG	0x20	// für Data Piping Filter (TS packets)
#define DATA_STREAMING_FLAG	0x30	// für Data Streaming Filter (PES packets)
#define SECTION_ANY         0x00
#define SECTION_EIT	        0x01
#define SECTION_IPMPE		(0x08|SECTION_CONTINUOUS)	// bis zu 4k groß
#define SECTION_HIGH_SPEED	(0x10|SECTION_IPMPE)	// vergrößerter Puffer für High Speed Filter

// Größe eines Piping-Buffers (im Highbyte von flags):
#define PBUFSIZE_MASK 0xff00
#define PBUFSIZE_NONE 0x0000
#define PBUFSIZE_1P   0x0100
#define PBUFSIZE_2P   0x0200
#define PBUFSIZE_1K   0x0300
#define PBUFSIZE_2K   0x0400
#define PBUFSIZE_4K   0x0500
#define PBUFSIZE_8K   0x0600
#define PBUFSIZE_16K  0x0700
#define PBUFSIZE_32K  0x0800

typedef enum  {	WCreate,
                WDestroy,
                WMoveD,
                WMoveA,
		    	WHide,
                WTop,
                DBox,
                DLine,
		    	DText,
		    	Set_Font,
                SetColor,
		    	SetBlend,
		    	SetWBlend,
		    	SetCBlend,
		    	SetNonBlend,
                LoadBmp,
				BlitBmp,
				ReleaseBmp,
	SetWTrans, // added in 0.9.4 patched
	SetWNoTrans // added in 0.9.4 patched
              } OSDCOM;

typedef enum OSDPALTYPE
{
	NoPalet =  0,      /* No palette */
	Pal1Bit =  2,      /* 2 colors for 1 Bit Palette    */
	Pal2Bit =  4,      /* 4 colors for 2 bit palette    */
	Pal4Bit =  16,     /* 16 colors for 4 bit palette   */
	Pal8Bit =  256     /* 256 colors for 16 bit palette */
} OSDPALTYPE, *POSDPALTYPE;

typedef enum DISPTYPE
{
	BITMAP1,                  /* 1 bit bitmap */
	BITMAP2,                  /* 2 bit bitmap */
	BITMAP4,                  /* 4 bit bitmap */
	BITMAP8,                  /* 8 bit bitmap */
//	BITMAP1HR,                /* 1 Bit bitmap half resolution */
//	BITMAP2HR,                /* 2 bit bitmap half resolution */
//	BITMAP4HR,                /* 4 bit bitmap half resolution */
//	BITMAP8HR,                /* 8 bit bitmap half resolution */
//	YCRCB422,                 /* 4:2:2 YCRCB Graphic Display */
//	YCRCB444,                 /* 4:4:4 YCRCB Graphic Display */
//	YCRCB444HR,               /* 4:4:4 YCRCB graphic half resolution */
//	VIDEO_MPEG,               /* Full Size MPEG Video Display */
//	VIDEOHSIZE,               /* MPEG Video Display Half Resolution */
//	VIDEOQSIZE,               /* MPEG Video Display Quarter Resolution */
//	VIDEOESIZE,               /* MPEG Video Display Quarter Resolution */
//	CURSOR                    /* Cursor */
} DISPTYPE, *PDISPTYPE;

typedef enum  { TTX_Disable_Capture,
                TTX_Capture_Parity_Corrected,
				TTX_Capture_Hamming_8_4_Corrected,
				TTX_Capture_8Bit_Data
} TTX_Capture_Types;



typedef enum  { TTXEnable,
		    	TTXDisable,
		    	ModifyTable,
		    	ReqestP29_P830,
                NoPageMemory,
                NoPESMemory, 
                PESPacket,
                TTXPage,
                P29_P830,
                DroppedPages
              } TTXCOM;

typedef enum  { AudioState,
		    	AudioBuffState,
		    	VideoState1,
		    	VideoState2,
		    	VideoState3,
		    	CrashCounter,
				ReqVersion,
				ReqVCXO,
				ReqRegister
			  } REQCOM;

typedef enum  { SetVidMode,
			    SetTestMode,
			    LoadVidCode,
				SetMonitorType,
				SetPanScanType,
            	SetFreezeMode
			  } ENC;

typedef enum  { IncCrashCounter,
		    	dummy
			  } SYSTEM;

typedef enum  { 
	            __Record,
				__Stop,
				__Play,
				__Pause,
				__Slow,
				__FF_IP,
				__Scan_I,
				__Continue
			  } REC_PLAY;

typedef enum  { 
    COMTYPE_NOCOM,
	COMTYPE_PIDFILTER,
	COMTYPE_MPEGDECODER,
	COMTYPE_OSD,
	COMTYPE_BMP,
	COMTYPE_ENCODER,
	COMTYPE_AUDIODAC,
	COMTYPE_REQUEST,
	COMTYPE_SYSTEM,
	COMTYPE_REC_PLAY,
	COMTYPE_COMMON_IF,
	COMTYPE_PID_FILTER,
    COMTYPE_PES,
    COMTYPE_TS,
	COMTYPE_VIDEO,
	COMTYPE_AUDIO,
    COMTYPE_TELETEXT, // Missing in 0.9.4 patched
	COMTYPE_CI_LL
} COMTYPE;

/* In 0.9.4 patched :
typedef enum { 
	MultiPID,
        VideoPID,
	AudioPID,
	InitFilt,
	FiltError,
	NewVersion,
	CacheError,
	AddPIDFilter,
	DelPIDFilter,
	Scan,
	SetDescr	
} PIDCOM;
			
typedef enum {
        SelAudChannels
} MPEGCOM;
*/

typedef enum  { 
    AudioDAC,
	CabADAC,
	ON22K,
	OFF22K,
	MainSwitch,
	ADSwitch,
	SendDiSEqC,
	OFDM_Channel, // Not in 0.9.4 patched
	OFDM_Guard, // Not in 0.9.4 patched
	OFDM_MpegBer, // Not in 0.9.4 patched
	SetRegister,
	SetIrControl // Not in 0.9.4 patched
} AUDCOM;


typedef enum QAM_TYPE
{	QAM_16,
	QAM_32,
	QAM_64,
	QAM_128,
	QAM_256
} QAM_TYPE, *PQAM_TYPE;

typedef struct {
  QAM_TYPE	QAM_Mode;
  int    		NoOfSym;
  unsigned char		Reg1;
  unsigned char		Reg5;
  unsigned char		Reg8;
  unsigned char		Reg9;
}QAM_SETTING;


QAM_SETTING QAM_Values[5] = {	
        {QAM_16 ,  16, 145, 164, 162, 145},
        {QAM_32 ,  32, 150, 120, 116, 150},
        {QAM_64 ,  64, 106,  70,  67, 106},
        {QAM_128, 128, 126,  54,  52, 126},
        {QAM_256, 256, 107,  38,  35, 107}
};


extern int AnzahlProzessor;
extern int MainProzessor;
extern int IrqProzessor;
extern int VBIProzessor;
extern int AusgabeProzessor;
extern int ProzessorMask;




/* Values that need to be changed for PAL or NTSC systems */
/* Example for NTSC: insmod saa7146 mode=1 */

/* Number of vertical active lines */
#define V_ACTIVE_LINES_PAL	576
#define V_ACTIVE_LINES_NTSC	480

/* Number of lines in a field for HPS to process */
#define V_FIELD_PAL	288
#define V_FIELD_NTSC	240

/* Number of lines of vertical offset before processing */
#define V_OFFSET_PAL	0x15
#define V_OFFSET_NTSC	0x16

/* Number of horizontal pixels to process */
#define H_PIXELS_PAL	720
#define H_PIXELS_NTSC	640

/* Horizontal offset of processing window */
#define H_OFFSET_PAL	0x40
#define H_OFFSET_NTSC	0x06

/************************************************************************/
/* UNSORTED								*/
/************************************************************************/

#define ME1    0x0000000800
#define PV1    0x0000000008

/************************************************************************/
/* I2C									*/
/************************************************************************/

/* time we wait after certain i2c-operations */
#define SAA7146_I2C_DELAY 		10

#define	SAA7146_IICSTA		0x090
#define	SAA7146_I2C_ABORT	(1<<7)
#define	SAA7146_I2C_SPERR	(1<<6)
#define	SAA7146_I2C_APERR	(1<<5)
#define	SAA7146_I2C_DTERR	(1<<4)
#define	SAA7146_I2C_DRERR	(1<<3)
#define	SAA7146_I2C_AL		(1<<2)
#define	SAA7146_I2C_ERR		(1<<1)
#define	SAA7146_I2C_BUSY	(1<<0)

#define	SAA7146_IICTRF		0x08c
#define	SAA7146_I2C_START	(0x3)
#define	SAA7146_I2C_CONT	(0x2)
#define	SAA7146_I2C_STOP	(0x1)
#define	SAA7146_I2C_NOP		(0x0)

#define SAA7146_I2C_BUS_BIT_RATE_6400	(0x5<<8)
#define SAA7146_I2C_BUS_BIT_RATE_3200	(0x1<<8)
#define SAA7146_I2C_BUS_BIT_RATE_480	(0x4<<8)
#define SAA7146_I2C_BUS_BIT_RATE_320	(0x6<<8)
#define SAA7146_I2C_BUS_BIT_RATE_240	(0x7<<8)
#define SAA7146_I2C_BUS_BIT_RATE_120	(0x0<<8)
#define SAA7146_I2C_BUS_BIT_RATE_80	(0x2<<8)
#define SAA7146_I2C_BUS_BIT_RATE_60	(0x3<<8)

/************************************************************************/
/* CLIPPING								*/
/************************************************************************/

/* some defines for the various clipping-modes */
#define SAA7146_CLIPPING_RECT		0x4
#define SAA7146_CLIPPING_RECT_INVERTED	0x5
#define SAA7146_CLIPPING_MASK		0x6
#define SAA7146_CLIPPING_MASK_INVERTED	0x7

/************************************************************************/
/* RPS									*/
/************************************************************************/

#define CMD_NOP		0x00000000  /* No operation */
#define CMD_CLR_EVENT	0x00000000  /* Clear event */
#define CMD_SET_EVENT	0x10000000  /* Set signal event */
#define CMD_PAUSE	0x20000000  /* Pause */
#define CMD_CHECK_LATE	0x30000000  /* Check late */
#define CMD_UPLOAD	0x40000000  /* Upload */
#define CMD_STOP	0x50000000  /* Stop */
#define CMD_INTERRUPT	0x60000000  /* Interrupt */
#define CMD_JUMP	0x80000000  /* Jump */
#define CMD_WR_REG	0x90000000  /* Write (load) register */
#define CMD_RD_REG	0xa0000000  /* Read (store) register */
#define CMD_WR_REG_MASK	0xc0000000  /* Write register with mask */

/************************************************************************/
/* OUTPUT FORMATS 							*/
/************************************************************************/

/* output formats; each entry holds three types of information */
/* composed is used in the sense of "not-planar" */

#define RGB15_COMPOSED	0x213
/* this means: yuv2rgb-conversation-mode=2, dither=yes(=1), format-mode = 3 */
#define RGB16_COMPOSED	0x210
#define RGB24_COMPOSED	0x201
#define RGB32_COMPOSED	0x202

#define YUV411_COMPOSED		0x003
/* this means: yuv2rgb-conversation-mode=0, dither=no(=0), format-mode = 3 */
#define YUV422_COMPOSED		0x000
#define YUV411_DECOMPOSED	0x00b
#define YUV422_DECOMPOSED	0x009

/************************************************************************/
/* MISC 								*/
/************************************************************************/

/* Bit mask constants */
#define MASK_00   0x00000001    /* Mask value for bit 0 */
#define MASK_01   0x00000002    /* Mask value for bit 1 */
#define MASK_02   0x00000004    /* Mask value for bit 2 */
#define MASK_03   0x00000008    /* Mask value for bit 3 */
#define MASK_04   0x00000010    /* Mask value for bit 4 */
#define MASK_05   0x00000020    /* Mask value for bit 5 */
#define MASK_06   0x00000040    /* Mask value for bit 6 */
#define MASK_07   0x00000080    /* Mask value for bit 7 */
#define MASK_08   0x00000100    /* Mask value for bit 8 */
#define MASK_09   0x00000200    /* Mask value for bit 9 */
#define MASK_10   0x00000400    /* Mask value for bit 10 */
#define MASK_11   0x00000800    /* Mask value for bit 11 */
#define MASK_12   0x00001000    /* Mask value for bit 12 */
#define MASK_13   0x00002000    /* Mask value for bit 13 */
#define MASK_14   0x00004000    /* Mask value for bit 14 */
#define MASK_15   0x00008000    /* Mask value for bit 15 */
#define MASK_16   0x00010000    /* Mask value for bit 16 */
#define MASK_17   0x00020000    /* Mask value for bit 17 */
#define MASK_18   0x00040000    /* Mask value for bit 18 */
#define MASK_19   0x00080000    /* Mask value for bit 19 */
#define MASK_20   0x00100000    /* Mask value for bit 20 */
#define MASK_21   0x00200000    /* Mask value for bit 21 */
#define MASK_22   0x00400000    /* Mask value for bit 22 */
#define MASK_23   0x00800000    /* Mask value for bit 23 */
#define MASK_24   0x01000000    /* Mask value for bit 24 */
#define MASK_25   0x02000000    /* Mask value for bit 25 */
#define MASK_26   0x04000000    /* Mask value for bit 26 */
#define MASK_27   0x08000000    /* Mask value for bit 27 */
#define MASK_28   0x10000000    /* Mask value for bit 28 */
#define MASK_29   0x20000000    /* Mask value for bit 29 */
#define MASK_30   0x40000000    /* Mask value for bit 30 */
#define MASK_31   0x80000000    /* Mask value for bit 31 */

#define MASK_B0   0x000000ff    /* Mask value for byte 0 */
#define MASK_B1   0x0000ff00    /* Mask value for byte 1 */
#define MASK_B2   0x00ff0000    /* Mask value for byte 2 */
#define MASK_B3   0xff000000    /* Mask value for byte 3 */

#define MASK_W0   0x0000ffff    /* Mask value for word 0 */
#define MASK_W1   0xffff0000    /* Mask value for word 1 */

#define MASK_PA   0xfffffffc    /* Mask value for physical address */
#define MASK_PR   0xfffffffe 	/* Mask value for protection register */
#define MASK_ER   0xffffffff    /* Mask value for the entire register */

#define MASK_NONE 0x00000000    /* No mask */

/************************************************************************/
/* REGISTERS 								*/
/************************************************************************/

#define BASE_ODD1         0x00  /* Video DMA 1 registers  */
#define BASE_EVEN1        0x04
#define PROT_ADDR1        0x08
#define PITCH1            0x0C
#define BASE_PAGE1        0x10  /* Video DMA 1 base page */
#define NUM_LINE_BYTE1    0x14

#define BASE_ODD2         0x18  /* Video DMA 2 registers */
#define BASE_EVEN2        0x1C
#define PROT_ADDR2        0x20
#define PITCH2            0x24
#define BASE_PAGE2        0x28  /* Video DMA 2 base page */
#define NUM_LINE_BYTE2    0x2C

#define BASE_ODD3         0x30  /* Video DMA 3 registers */
#define BASE_EVEN3        0x34
#define PROT_ADDR3        0x38
#define PITCH3            0x3C         
#define BASE_PAGE3        0x40  /* Video DMA 3 base page */
#define NUM_LINE_BYTE3    0x44

#define PCI_BT_V1         0x48  /* Video/FIFO 1 */
#define PCI_BT_V2         0x49  /* Video/FIFO 2 */
#define PCI_BT_V3         0x4A  /* Video/FIFO 3 */
#define PCI_BT_DEBI       0x4B  /* DEBI */
#define PCI_BT_A          0x4C  /* Audio */

#define DD1_INIT          0x50  /* Init setting of DD1 interface */

#define DD1_STREAM_B      0x54  /* DD1 B video data stream handling */
#define DD1_STREAM_A      0x56  /* DD1 A video data stream handling */

#define BRS_CTRL          0x58  /* BRS control register */
#define HPS_CTRL          0x5C  /* HPS control register */
#define HPS_V_SCALE       0x60  /* HPS vertical scale */
#define HPS_V_GAIN        0x64  /* HPS vertical ACL and gain */
#define HPS_H_PRESCALE    0x68  /* HPS horizontal prescale   */
#define HPS_H_SCALE       0x6C  /* HPS horizontal scale */
#define BCS_CTRL          0x70  /* BCS control */
#define CHROMA_KEY_RANGE  0x74
#define CLIP_FORMAT_CTRL  0x78  /* HPS outputs formats & clipping */

#define DEBI_CONFIG       0x7C
#define DEBI_COMMAND      0x80
#define DEBI_PAGE         0x84
#define DEBI_AD           0x88	

#define I2C_TRANSFER      0x8C	
#define I2C_STATUS        0x90	

#define BASE_A1_IN        0x94	/* Audio 1 input DMA */
#define PROT_A1_IN        0x98
#define PAGE_A1_IN        0x9C
  
#define BASE_A1_OUT       0xA0  /* Audio 1 output DMA */
#define PROT_A1_OUT       0xA4
#define PAGE_A1_OUT       0xA8

#define BASE_A2_IN        0xAC  /* Audio 2 input DMA */
#define PROT_A2_IN        0xB0
#define PAGE_A2_IN        0xB4

#define BASE_A2_OUT       0xB8  /* Audio 2 output DMA */
#define PROT_A2_OUT       0xBC
#define PAGE_A2_OUT       0xC0

#define RPS_PAGE0         0xC4  /* RPS task 0 page register */
#define RPS_PAGE1         0xC8  /* RPS task 1 page register */

#define RPS_THRESH0       0xCC  /* HBI threshold for task 0 */
#define RPS_THRESH1       0xD0  /* HBI threshold for task 1 */

#define RPS_TOV0          0xD4  /* RPS timeout for task 0 */
#define RPS_TOV1          0xD8  /* RPS timeout for task 1 */

#define IER               0xDC  /* Interrupt enable register */

#define GPIO_CTRL         0xE0  /* GPIO 0-3 register */

#define EC1SSR            0xE4  /* Event cnt set 1 source select */
#define EC2SSR            0xE8  /* Event cnt set 2 source select */
#define ECT1R             0xEC  /* Event cnt set 1 thresholds */
#define ECT2R             0xF0  /* Event cnt set 2 thresholds */

#define ACON1             0xF4
#define ACON2             0xF8

#define MC1               0xFC   /* Main control register 1 */
#define MC2               0x100  /* Main control register 2  */

#define RPS_ADDR0         0x104  /* RPS task 0 address register */
#define RPS_ADDR1         0x108  /* RPS task 1 address register */

#define ISR               0x10C  /* Interrupt status register */                                                             
#define PSR               0x110  /* Primary status register */
#define SSR               0x114  /* Secondary status register */

#define EC1R              0x118  /* Event counter set 1 register */
#define EC2R              0x11C  /* Event counter set 2 register */         

#define PCI_VDP1          0x120  /* Video DMA pointer of FIFO 1 */
#define PCI_VDP2          0x124  /* Video DMA pointer of FIFO 2 */
#define PCI_VDP3          0x128  /* Video DMA pointer of FIFO 3 */
#define PCI_ADP1          0x12C  /* Audio DMA pointer of audio out 1 */
#define PCI_ADP2          0x130  /* Audio DMA pointer of audio in 1 */
#define PCI_ADP3          0x134  /* Audio DMA pointer of audio out 2 */
#define PCI_ADP4          0x138  /* Audio DMA pointer of audio in 2 */
#define PCI_DMA_DDP       0x13C  /* DEBI DMA pointer */

#define LEVEL_REP         0x140,
#define A_TIME_SLOT1      0x180,  /* from 180 - 1BC */
#define A_TIME_SLOT2      0x1C0,  /* from 1C0 - 1FC */

/************************************************************************/
/* ISR-MASKS 								*/
/************************************************************************/

#define SPCI_PPEF       0x80000000  /* PCI parity error */
#define SPCI_PABO       0x40000000  /* PCI access error (target or master abort) */
#define SPCI_PPED       0x20000000  /* PCI parity error on 'real time data' */
#define SPCI_RPS_I1     0x10000000  /* Interrupt issued by RPS1 */
#define SPCI_RPS_I0     0x08000000  /* Interrupt issued by RPS0 */
#define SPCI_RPS_LATE1  0x04000000  /* RPS task 1 is late */
#define SPCI_RPS_LATE0  0x02000000  /* RPS task 0 is late */
#define SPCI_RPS_E1     0x01000000  /* RPS error from task 1 */
#define SPCI_RPS_E0     0x00800000  /* RPS error from task 0 */
#define SPCI_RPS_TO1    0x00400000  /* RPS timeout task 1 */
#define SPCI_RPS_TO0    0x00200000  /* RPS timeout task 0 */
#define SPCI_UPLD       0x00100000  /* RPS in upload */
#define SPCI_DEBI_S     0x00080000  /* DEBI status */
#define SPCI_DEBI_E     0x00040000  /* DEBI error */
#define SPCI_IIC_S      0x00020000  /* I2C status */
#define SPCI_IIC_E      0x00010000  /* I2C error */
#define SPCI_A2_IN      0x00008000  /* Audio 2 input DMA protection / limit */
#define SPCI_A2_OUT     0x00004000  /* Audio 2 output DMA protection / limit */
#define SPCI_A1_IN      0x00002000  /* Audio 1 input DMA protection / limit */
#define SPCI_A1_OUT     0x00001000  /* Audio 1 output DMA protection / limit */
#define SPCI_AFOU       0x00000800  /* Audio FIFO over- / underflow */
#define SPCI_V_PE       0x00000400  /* Video protection address */
#define SPCI_VFOU       0x00000200  /* Video FIFO over- / underflow */
#define SPCI_FIDA       0x00000100  /* Field ID video port A */
#define SPCI_FIDB       0x00000080  /* Field ID video port B */
#define SPCI_PIN3       0x00000040  /* GPIO pin 3 */
#define SPCI_PIN2       0x00000020  /* GPIO pin 2 */
#define SPCI_PIN1       0x00000010  /* GPIO pin 1 */
#define SPCI_PIN0       0x00000008  /* GPIO pin 0 */
#define SPCI_ECS        0x00000004  /* Event counter 1, 2, 4, 5 */
#define SPCI_EC3S       0x00000002  /* Event counter 3 */
#define SPCI_EC0S       0x00000001  /* Event counter 0 */


#define	DPRAM_BASE	0x4000
#define DPRAM_SIZE	0x2000

#define BOOT_STATE	(DPRAM_BASE + 0x3F8)
#define BOOT_SIZE	(DPRAM_BASE + 0x3FA)
#define BOOT_BASE	(DPRAM_BASE + 0x3FC)
#define BOOT_BLOCK	(DPRAM_BASE + 0x400)
#define BOOT_MAX_SIZE	0xc00


#define IRQ_STATE		(DPRAM_BASE + 0x0F4)
#define IRQ_STATE_EXT	(DPRAM_BASE + 0x0F6)
#define MSGSTATE		(DPRAM_BASE + 0x0F8)
#define FILT_STATE	(DPRAM_BASE + 0x0FA)
#define COMMAND		(DPRAM_BASE + 0x0FC)
#define COM_BUFF		(DPRAM_BASE + 0x100)
#define COM_BUFF_SIZE	0x20

#define BUFF1_BASE	(DPRAM_BASE + 0x120)
#define BUFF1_SIZE	0xE0

#define DATA_BUFF_BASE	(DPRAM_BASE + 0x200)
#define DATA_BUFF_SIZE	0x1C00

#define Reserved		(DPRAM_BASE + 0x1E00)
#define Reserved_SIZE	0x1D0

#define DEBUG_WINDOW	(DPRAM_BASE + 0x1FD0)
#define DEBUG_WIN_SIZE	0x28
#define	DBG_LOOP_CNT	(DEBUG_WINDOW + 0)
#define DBG_SEC_CNT		(DEBUG_WINDOW + 2)
#define DBG_AVRP_BUFF	(DEBUG_WINDOW + 4)
#define DBG_AVRP_PEAK	(DEBUG_WINDOW + 6)
#define DBG_MSG_CNT		(DEBUG_WINDOW + 8)
#define DBG_CODE_REG	(DEBUG_WINDOW + 10)
#define DBG_TTX_Q		(DEBUG_WINDOW + 12)
#define DBG_AUD_EN		(DEBUG_WINDOW + 14)
#define DBG_WRONG_COM	(DEBUG_WINDOW + 16)
#define DBG_ARR_OVFL	(DEBUG_WINDOW + 18)
#define DBG_BUFF_OVFL	(DEBUG_WINDOW + 20)
#define DBG_OVFL_CNT	(DEBUG_WINDOW + 22)
#define DBG_SEC_OVFL	(DEBUG_WINDOW + 24)



#define RX_BUFF         (DPRAM_BASE + 0x1FF4)
#define TX_BUFF 	    (DPRAM_BASE + 0x1FF6)
#define HANDSHAKE_REG	(DPRAM_BASE + 0x1FF8)
#define TX_LEN          (DPRAM_BASE + 0x1FEE)
#define COM_IF_LOCK	(DPRAM_BASE + 0x1FFA)
#define IRQ_RX		(DPRAM_BASE + 0x1FFC)
#define IRQ_TX		(DPRAM_BASE + 0x1FFE)

#define DRAM_START_CODE		0x2e000404
#define DRAM_MAX_CODE_SIZE	0x00100000

#define RESET_LINE			2
#define DEBI_DONE_LINE		1

#define DAC_CS	0x8000
#define DAC_CDS	0x0000

typedef enum GPIO_MODE
{
	GPIO_INPUT = 0x00,
	GPIO_IRQHI = 0x10,
	GPIO_IRQLO = 0x20,
	GPIO_IRQHL = 0x30,
	GPIO_OUTLO = 0x40,
	GPIO_OUTHI = 0x50
} GPIO_MODE;


#define BOOTSTATE_BUFFER_EMPTY	 0
#define BOOTSTATE_BUFFER_FULL	 1
#define BOOTSTATE_BOOT_COMPLETE	 2



#define DVB_ERR_NOSERVER		0
#define DVB_ERR_NONE			1
#define DVB_ERR_HARDWARE		2
#define DVB_ERR_ILLEGALDATA		3
#define DVB_ERR_NODATA			4
#define DVB_ERR_TIMEOUT			5
#define DVB_ERR_DENY			6
#define DVB_ERR_OVERFLOW		7
#define DVB_ERR_NOMEMORY		8
#define DVB_ERR_LOCK			9

#define	GPMQFull	0x0001			//Main Message Queue Full
#define	GPMQOver	0x0002			//Main Message Queue Overflow
#define	HPQFull	0x0004			//High Priority Msg Queue Full
#define	HPQOver	0x0008
#define	OSDQFull	0x0010			//OSD Queue Full
#define	OSDQOver	0x0020

#define DATA_NONE                0x00 
#define DATA_FSECTION            0x01
#define DATA_IPMPE               0x02
#define DATA_MPEG_RECORD         0x03
#define DATA_DEBUG_MESSAGE       0x04
#define DATA_COMMON_INTERFACE    0x05
#define DATA_MPEG_PLAY           0x06
#define DATA_BMP_LOAD            0x07
#define DATA_IRCOMMAND           0x08
#define DATA_PIPING              0x09
#define DATA_STREAMING           0x0a
#define DATA_CI_GET              0x0b
#define DATA_CI_PUT              0x0c
#define DATA_PES_RECORD          0x10
#define DATA_PES_PLAY            0x11
#define DATA_TS_RECORD           0x12
#define DATA_TS_PLAY             0x13

//SendCIMessage Tags
#define	CI_MSG_NONE							0
#define	CI_MSG_CI_INFO					1
#define	CI_MSG_MENU							2
#define	CI_MSG_LIST							3
#define	CI_MSG_TEXT							4
#define	CI_MSG_REQUEST_INPUT		5
#define	CI_MSG_INPUT_COMPLETE		6
#define	CI_MSG_LIST_MORE				7
#define	CI_MSG_MENU_MORE				8
#define	CI_MSG_CLOSE_MMI_IMM		9
#define	CI_MSG_SECTION_REQUEST	0xA
#define	CI_MSG_CLOSE_FILTER			0xB
#define	CI_PSI_COMPLETE					0xC
#define	CI_MODULE_READY					0xD
#define	CI_SWITCH_PRG_REPLY			0xE
#define	CI_MSG_TEXT_MORE				0xF

#define	CI_HC_TUNE							0x20
#define	CI_HC_REPLACE						0x21
#define	CI_HC_CLEAR_REPLACE			0x22
#define	CI_HC_ASK_RELEASE				0x23

#define	CI_LSC_CMD							0x30
#define	CI_LSC_DESCRIPTOR				0x31
#define	CI_LSC_SEND_LAST				0x32
#define	CI_LSC_SEND_MORE				0x33

#define	CI_MSG_CA_PMT						0xE0
#define	CI_MSG_ERROR						0xF0


//command interface subcommands
#define CI_CMD_ERROR             0x00
#define CI_CMD_ACK               0x01
#define CI_CMD_SYSTEM_READY      0x02
#define CI_CMD_KEYPRESS          0x03
#define CI_CMD_ON_TUNED          0x04
#define CI_CMD_ON_SWITCH_PROGRAM 0x05
#define CI_CMD_SECTION_ARRIVED   0x06
#define CI_CMD_SECTION_TIMEOUT   0x07
#define	CI_CMD_TIME              0x08
#define	CI_CMD_ENTER_MENU        0x09
#define	CI_CMD_FAST_PSI          0x0a
#define	CI_CMD_GET_SLOT_INFO     0x0b

#define	CI_CMD_HALT_ARM						0xE0

//error codes
#define	ERR_NONE									0
#define	ERR_WRONG_FLT_INDEX				1
#define	ERR_SET_FLT								2
#define	ERR_CLOSE_FLT							3
#define	ERR_INVALID_DATA					4
#define	ERR_NO_CA_RESOURCE				5

//slot status
#define	CI_SLOT_EMPTY							0
#define	CI_SLOT_MODULE_INSERTED		1
#define	CI_SLOT_MODULE_OK					2
#define CI_SLOT_CA_OK							3
#define	CI_SLOT_DBG_MSG						4
#define	CI_SLOT_UNKNOWN_STATE			0xFF

//key code masks (see DVB document A017 page 44)
#define	CI_KEYMASK_MENU				0x1
#define	CI_KEYMASK_LIST				0x1
#define	CI_KEYMASK_ALL				0x3FFFF

//control codes contained in strings
#define	CI_CTRL_LF				0x8A


//commands for the low speed communication
#define	CI_LSC_CONNECT_ON_CHANNEL				0x1
#define	CI_LSC_DISCONNECT_ON_CHANNEL		0x2
#define	CI_LSC_SET_PARAMS								0x3
#define	CI_LSC_ENQUIRE_STATUS						0x4
#define	CI_LSC_GET_NEXT_BUFFER					0x5

//commands reply ID
#define	CI_LSC_CONNECT_ACK							0x1
#define	CI_LSC_DISCONNECT_ACK						0x2
#define	CI_LSC_SET_PARAMS_ACK						0x3
#define	CI_LSC_STATUS_REPLY							0x4
#define	CI_LSC_GET_NEXT_BUFFER_ACK			0x5
#define	CI_LSC_SEND_ACK									0x6

//commands reply return values
#define REPLY_NO_ERROR									0x0
#define	REPLY_ERROR											0xFF
#define	STATUS_REPLY_DISCONNECTED				0x0
#define	STATUS_REPLY_CONNECTED					0x1


#define	SI_TELEPHONE_DESCRIPTOR					0x1
#define	CABLE_RETURN_CHANNEL_DESCRIPTOR	0x2
#define	INTERNET_DESCRIPTOR							0x3

#define	LSC_V42BIS				0x0001
#define	LSC_V42						0x0002
#define	LSC_V21						0x0004
#define LSC_V22						0x0008
#define	LSC_V22BIS				0x0010
#define LSC_V23						0x0020
#define LSC_V32						0x0040
#define LSC_V32BIS				0x0080
#define LSC_V34						0x0100
#define LSC_V27TER				0x0200
#define LSC_V29						0x0400

#define	I2C_ADR_VES1877		0x10	// Sat-Front End (alt und neu)
#define	I2C_ADR_VES1820		0x12	// Kabel-Front End
#define	I2C_ADR_FEC92314	0x20	// DVB-T Frontend
#define	I2C_ADR_OFDM92314	0x40	// DVB-T Frontend
#define	I2C_ADR_TERTUNER	0xC0	// DVB-T-Tuner
#define	I2C_ADR_SATTUNER	0xC2    // SAT-Tuner
#define	I2C_ADR_PHILIPS_TUNER 0xC2
#define	I2C_ADR_CABTUNER	0xC4  // Kabel-Tuner


#define PID_PAT 0x00	/* Program Association Table */
#define PID_CAT 0x01	/* Conditional Access Table */
/* 0x02 - 0x0F */	/* Reserved for future use */
#define PID_NIT 0x10	/* Network Information Table */
#define PID_SDT 0x11	/* Service Description Table */
#define PID_BAT 0x11	/* Bouquet Association Table */
#define PID_EIT 0x12	/* Event Information Table */
#define PID_RST 0x13	/* Running Status Table */
#define PID_TDT 0x14	/* Time Date Table */
#define PID_TOT 0x14	/* Time Offset Table */
#define PID_ST  0x14	/* Stuffing Table */
/* 0x15 - 0x1F */	/* Reserved for future use */

/* Table Identifier */

#define TID_PAT 0x00	/* Program Association Section */
#define TID_CAT 0x01	/* Conditional Access Section */
#define TID_PMT 0x02	/* Program Map Section */
/* 0x03 - 0x3F */	/* Reserved for future use */
#define TID_NIT_ACT 0x40	/* Network Information Section - actual */
#define TID_NIT_OTH 0x41	/* Network Information Section - other */
#define TID_SDT_ACT 0x42	/* Service Description Section - actual */
/* 0x43 - 0x45 */	/* Reserved for future use */
#define TID_SDT_OTH 0x46	/* Service Description Section - other */
/* 0x47 - 0x49 */	/* Reserved for future use */
#define TID_BAT 0x4A	/* Bouquet Association Section */
/* 0x4B - 0x4D */	/* Reserved for future use */
#define TID_EIT_ACT 0x4E /* Event Information Section - actual transport stream, present/following */
#define TID_EIT_OTH 0x4F /* Event Information Section - other transport stream, present/following */
#define TID_EIT_SCH_FIRST 0x50
/* 0x50 to 0x5F */ /* Event Information Section - actual transport stream, schedule */
/* 0x60 to 0x6F */ /* Event Information Section - other transport stream, schedule */
#define TID_EIT_SCH_LAST 0x6F
#define TID_TDT 0x70	/* Time Date Section */
#define TID_RST 0x71	/* Running Status Section */
#define TID_ST  0x72	/* Stuffing Section */
#define TID_TOT 0x73	/* Time Offset Section */
/* 0x74 - 0x7F */	/* Reserved for future use */
/* 0x80 - 0xFE */	/* User defined */
/* 0xFF */		/* Reserved for future use */

  /* Descriptor Identifiers */

  /* defined by ISO/IEC 13818-1 */

/* 0x00 - 0x01 */	/* Reserved */
#define DESCR_VIDEO_STREAM           0x02
#define DESCR_AUDIO_STREAM           0x03
#define DESCR_HIERARCHY              0x04
#define DESCR_REGISTRATION           0x05
#define DESCR_DATA_STREAM_ALIGN      0x06
#define DESCR_TARGET_BACKGRID        0x07
#define DESCR_VIDEO_WINDOW           0x08
#define DESCR_CA                     0x09
#define DESCR_ISO_639_LANGUAGE       0x0A
#define DESCR_SYSTEM_CLOCK           0x0B
#define DESCR_MULTIPLEX_BUFFER_UTIL  0x0C
#define DESCR_COPYRIGHT              0x0D
#define DESCR_MAXIMUM_BITRATE        0x0E
#define DESCR_PRIVATE_DATA_IND       0x0F
#define DESCR_SMOOTHING_BUFFER       0x10
#define DESCR_STD                    0x11
#define DESCR_IBP                    0x12
/* 0x13 - 0x3F */ /* Reserved */

  /* defined by ETSI */

#define DESCR_NW_NAME		0x40
#define DESCR_SERVICE_LIST	0x41
#define DESCR_STUFFING		0x42
#define DESCR_SAT_DEL_SYS	0x43
#define DESCR_CABLE_DEL_SYS	0x44
#define DESCR_VBI_DATA        0x45
#define DESCR_VBI_TELETEXT    0x46
#define DESCR_BOUQUET_NAME	0x47
#define DESCR_SERVICE		0x48
#define DESCR_COUNTRY_AVAIL	0x49
#define DESCR_LINKAGE		0x4A
#define DESCR_NVOD_REF		0x4B
#define DESCR_TIME_SHIFTED_SERVICE	0x4C
#define DESCR_SHORT_EVENT	0x4D
#define DESCR_EXTENDED_EVENT	0x4E
#define DESCR_TIME_SHIFTED_EVENT	0x4F
#define DESCR_COMPONENT		0x50
#define DESCR_MOSAIC		0x51
#define DESCR_STREAM_ID		0x52
#define DESCR_CA_IDENT		0x53
#define DESCR_CONTENT		0x54
#define DESCR_PARENTAL_RATING	0x55
#define DESCR_TELETEXT		0x56
#define DESCR_TELEPHONE		0x57
#define DESCR_LOCAL_TIME_OFF	0x58
#define DESCR_SUBTITLING	0x59
#define DESCR_TERR_DEL_SYS	0x5A
#define DESCR_ML_NW_NAME	0x5B
#define DESCR_ML_BQ_NAME	0x5C
#define DESCR_ML_SERVICE_NAME	0x5D
#define DESCR_ML_COMPONENT	0x5E
#define DESCR_PRIV_DATA_SPEC	0x5F
#define DESCR_SERVICE_MOVE	0x60
#define DESCR_SHORT_SMOOTH_BUF	0x61
#define DESCR_FREQUENCY_LIST	0x62
#define DESCR_PARTIAL_TP_STREAM	0x63
#define DESCR_DATA_BROADCAST	0x64
#define DESCR_CA_SYSTEM		0x65
#define DESCR_DATA_BROADCAST_ID	0x66
#define DESCR_TRANSPORT_STREAM      0x67
#define DESCR_DSNG                  0x68
#define DESCR_PDC                   0x69
#define DESCR_AC3                   0x6A
#define DESCR_ANCILLARY_DATA        0x6B
#define DESCR_CELL_LIST             0x6C
#define DESCR_CELL_FREQ_LINK        0x6D
#define DESCR_ANNOUNCEMENT_SUPPORT  0x6E
/* 0x6f - 0x7f */ /* Reserved */
/* 0x80 - 0xfe */ /* User private */
/* 0xff */ /* Forbidden */

/* Elementary stream identifiers */

/* 0x00 */ /* Reserved */
#define STREAMTYPE_11172_VIDEO         0x01
#define STREAMTYPE_13818_VIDEO         0x02
#define STREAMTYPE_11172_AUDIO         0x03
#define STREAMTYPE_13818_AUDIO         0x04
#define STREAMTYPE_13818_PRIVATE       0x05
#define STREAMTYPE_13818_PES_PRIVATE   0x06
#define STREAMTYPE_13522_MHPEG         0x07
#define STREAMTYPE_13818_DSMCC         0x08
#define STREAMTYPE_ITU_222_1           0x09
#define STREAMTYPE_13818_A             0x0a
#define STREAMTYPE_13818_B             0x0b
#define STREAMTYPE_13818_C             0x0c
#define STREAMTYPE_13818_D             0x0d
#define STREAMTYPE_13818_AUX           0x0e
/* 0x0F - 0x7F */ /* Reserved */
/* 0x80 - 0xFF */ /* User private */

#define ID_CRYPT_SECA        0x01 /* Canal + */
#define ID_CRYPT_VIACCESS    0x05 /* France Telecom */
#define ID_CRYPT_IRDETO      0x06 /* Irdeto */
#define ID_CRYPT_VIDEOGUARD  0x09 /* News Datacom */
#define ID_CRYPT_CRYPTOWORKS 0x0d /* Philips */
#define ID_CRYPT_POWERVU     0x0e /* Scientific Atlanta */
#define ID_CRYPT_BETACRYPT   0x17 /* Beta Technik */
#define ID_CRYPT_NAGRAVISION 0x18


struct saa7146_modes_constants {
	unsigned char  v_offset;
	unsigned short v_field;
	unsigned short v_calc;
	
	unsigned char  h_offset;
	unsigned short h_pixels;
	unsigned short h_calc;
	
	unsigned short v_max_out;
	unsigned short h_max_out;
};


struct saa7146_modes_constants
 modes_constants[4] = {
	{ V_OFFSET_PAL,		V_FIELD_PAL,	V_ACTIVE_LINES_PAL,
	  H_OFFSET_PAL,		H_PIXELS_PAL,	H_PIXELS_PAL+1,
	  V_ACTIVE_LINES_PAL,	1024 },	/* PAL values */
	{ V_OFFSET_NTSC,	V_FIELD_NTSC,	V_ACTIVE_LINES_NTSC,
	  H_OFFSET_NTSC,	H_PIXELS_NTSC,  H_PIXELS_NTSC+1,
	  V_ACTIVE_LINES_NTSC,	1024 },	/* NTSC values */
	{ 0,0,0,0,0,0,0,0 }, /* secam values */
	{ 0,0,0,0,0,0,0,0 } /* dummy values */
};

/* -----------------------------------------------------------------------------------------
   helper functions for the calculation of the horizontal- and vertical scaling	registers,
   clip-format-register etc ...
   these functions take pointers to the (most-likely read-out original-values) and manipulate
   them according to the requested new scaling parameters.
   ----------------------------------------------------------------------------------------- */

/* hps_coeff used for CXY and CXUV; scale 1/1 -> scale 1/64 */
struct {
	unsigned short hps_coeff;
	unsigned short weight_sum;
} hps_h_coeff_tab [64] = { 
	{0x00,   2}, {0x02,   4}, {0x00,   4}, {0x06,   8}, {0x02,   8}, {0x08,   8}, {0x00,   8}, {0x1E,  16},
	{0x0E,   8}, {0x26,   8}, {0x06,   8}, {0x42,   8}, {0x02,   8}, {0x80,   8}, {0x00,   8}, {0xFE,  16},
	{0xFE,   8}, {0x7E,   8}, {0x7E,   8}, {0x3E,   8}, {0x3E,   8}, {0x1E,   8}, {0x1E,   8}, {0x0E,   8},
	{0x0E,   8}, {0x06,   8}, {0x06,   8}, {0x02,   8}, {0x02,   8}, {0x00,   8}, {0x00,   8}, {0xFE,  16},
	{0xFE,   8}, {0xFE,   8}, {0xFE,   8}, {0xFE,   8}, {0xFE,   8}, {0xFE,   8}, {0xFE,   8}, {0xFE,   8},
	{0xFE,   8}, {0xFE,   8}, {0xFE,   8}, {0xFE,   8}, {0xFE,   8}, {0xFE,   8}, {0xFE,   8}, {0xFE,   8},
	{0xFE,   8}, {0x7E,   8}, {0x7E,   8}, {0x3E,   8}, {0x3E,   8}, {0x1E,   8}, {0x1E,   8}, {0x0E,   8},
	{0x0E,   8}, {0x06,   8}, {0x06,   8}, {0x02,   8}, {0x02,   8}, {0x00,   8}, {0x00,   8}, {0xFE,  16}
};

/* table of attenuation values for horizontal scaling */
unsigned char h_attenuation[9] = { 1, 2, 4, 8, 2, 4, 8, 16, 0};

struct {
	unsigned short hps_coeff;
	unsigned short weight_sum;
} hps_v_coeff_tab [64] = { 
 {0x0100,   2},  {0x0102,   4},  {0x0300,   4},  {0x0106,   8},  {0x0502,   8},  {0x0708,   8},  {0x0F00,   8},  {0x011E,  16}, 
 {0x110E,  16},  {0x1926,  16},  {0x3906,  16},  {0x3D42,  16},  {0x7D02,  16},  {0x7F80,  16},  {0xFF00,  16},  {0x01FE,  32}, 
 {0x01FE,  32},  {0x817E,  32},  {0x817E,  32},  {0xC13E,  32},  {0xC13E,  32},  {0xE11E,  32},  {0xE11E,  32},  {0xF10E,  32}, 
 {0xF10E,  32},  {0xF906,  32},  {0xF906,  32},  {0xFD02,  32},  {0xFD02,  32},  {0xFF00,  32},  {0xFF00,  32},  {0x01FE,  64}, 
 {0x01FE,  64},  {0x01FE,  64},  {0x01FE,  64},  {0x01FE,  64},  {0x01FE,  64},  {0x01FE,  64},  {0x01FE,  64},  {0x01FE,  64}, 
 {0x01FE,  64},  {0x01FE,  64},  {0x01FE,  64},  {0x01FE,  64},  {0x01FE,  64},  {0x01FE,  64},  {0x01FE,  64},  {0x01FE,  64}, 
 {0x01FE,  64},  {0x817E,  64},  {0x817E,  64},  {0xC13E,  64},  {0xC13E,  64},  {0xE11E,  64},  {0xE11E,  64},  {0xF10E,  64}, 
 {0xF10E,  64},  {0xF906,  64},  {0xF906,  64},  {0xFD02,  64},  {0xFD02,  64},  {0xFF00,  64},  {0xFF00,  64},  {0x01FE, 128} };

/* table of attenuation values for vertical scaling */
unsigned short v_attenuation[9] = { 2, 4, 8, 16, 32, 64, 128, 256, 0};


struct frontend dvb_front; 



int saa_interlace=1;
int saa_mode=0; // test
int saa_grab_height;
int saa_grab_port;
int saa_grab_width;
int saa_grab_format;



extern int ColourFormat;
extern BOOL Flip; 
extern int emstartx;
extern int emstarty;
extern int emsizex;
extern int emsizey;
extern int CurrentX;
extern int CurrentY;
extern int SurfacePitch;
extern int Offset_for_WindowOut;
extern int OverlayPitch;



struct	saa7146_video_dma {
	unsigned int base_odd;
	unsigned int base_even;
	unsigned int prot_addr;
	int pitch;
	unsigned int base_page;
	unsigned int num_line_byte;
};



int DVBTunerType=0;
int SaveTuner;

struct DVBTunerParameter DVBTuners[6] = {
        
		{"SPXXXX (DVB-C)", 
	         40000000UL , 870000000UL  , 62500 , 1   , 36125000 ,
	         174000000UL, 454000000UL  , 0xa1  , 0x92, 0x34     , 0x8e, 0x00, 0xc2},

        {"SP5659 (DVB-S)", 
	         100000000UL , 2700000000UL, 125000, 1   , 479500000,
	         0xffffffffUL, 0xffffffffUL, 0x30  , 0x30, 0x30     , 0x95, 0x00, 0xc2},
        
		{"TSA5059 Philips (DVB-S)", 
        	 100000000UL , 2700000000UL, 125000, 1   , 0        ,
	         0xffffffffUL, 0xffffffffUL, 0x30  , 0x30, 0x30     , 0x95, 0x00, 0xc2},
/*        {"TSA5522 Philips (DVB-S)", 
	          100000000UL, 2700000000UL, 125000, 1   , 479500000,
	         0xffffffffUL, 0xffffffffUL, 0x30  , 0x30, 0x30     , 0x95, 0x00, 0xc0}, */
        {"SP8060 Sat Grundig (DVB-S)", 
	          100000000UL, 2700000000UL, 125000, 1   , 0,
	         0xffffffffUL, 0xffffffffUL, 0x00  , 0x00, 0x00     , 0x8e, 0x00, 0xc0},

        {"SP5659C (DVB-C)",                            
              47000000UL , 862000000UL , 62500 , 1   , 35875000 ,
              174000000UL, 470000000UL , 0x88  , 0x84, 0x81     , 0x85, 0x00, 0xc4},

		{"Eigene Parameter", 
	          0UL        , 0UL         , 0     , 0   , 0        ,
	          0UL        , 0UL         , 0x00  , 0x00, 0x00     , 0x00, 0x00, 0xc2},

};

BOOL Found_Tuner_I2C=0x5f;

#define TUNER_POR       0x80
#define TUNER_FL        0x40
#define TUNER_MODE      0x38
#define TUNER_AFC       0x07


struct saa7146_video_dma vdma1;
	   
extern int NextFrame;
extern HANDLE Decode_Event;
extern HANDLE Decode_Event_Stop;

int odswin=0;
extern unsigned int InitialDVBBrightness;
extern unsigned int InitialDVBContrast;
extern unsigned int InitialDVBColor;
extern unsigned int InitialDVBVolumeLinks;
extern unsigned int InitialDVBVolumeRechts;

unsigned int rps_bcs_ctrl[2] = { 0,0};
unsigned int rps_clip_format[2] = { 0,0 };

#define DISEQC_HIGH_NIBLE 0xF0
#define DISEQC_LOW_BAND  0x00
#define DISEQC_HIGH_BAND 0x01
#define DISEQC_VERTICAL  0x00
#define DISEQC_HORIZONTAL 0x02
#define DISEQC_POSITION_A 0x00
#define DISEQC_POSITION_B 0x04
#define DISEQC_OPTION_A  0x00
#define DISEQC_OPTION_B  0x08


extern struct TLNB LNB[4];

extern BOOL DOSIZE;


extern struct TTransponder Transponder;


extern char CurrentDir[256];

int FilterFile;

#define MAXFILTER 32
struct TFilter *Filter = NULL;
HANDLE FilterEvent[MAXFILTER];

struct TTPCat TPCat[32];
int TPCatAnzahl=0;
struct tPAT PAT;
struct dvb_pmt_struct PMT;
#define MAXNIT 512
struct tNIT *NIT = NULL;
#define NEUNIT 96
struct tNIT NITNeu[NEUNIT];
int NitAnzahl;
struct TDT_t TDT;
short TIMEZONE=0;

#define EIT_INDEX_SIZE 0x10000

unsigned int EIT_Loaded=0;

int Current_Eit_Index_Size=0;

struct TEIT_Index *EIT;


struct dvb_sdt_t SDT;

BOOL Has_Pat = FALSE;
BOOL Has_Sdt = FALSE;

#define CI_MODULE_PRESENT 1
#define CI_MODULE_OK      2

unsigned short CA_Len=0;

extern BOOL OSD_FULL_INPUT;

DWORD NextIrq;

struct tci_menu ci_menu[20];

BOOL ci_menu_update=FALSE;
int CI_Menu_Slot=0;
int CIHeaderLines=0;
int CILines=0;

extern BOOL USE_TV_16_9;

extern char ChannelString[10];
int PopUpCam;
struct tci ci_module[2];
int ci_interface=0;
unsigned char ci_interface_LastCmd[128];
unsigned short ci_interface_LastCmdLen;
BOOL ci_interface_Refresh=FALSE;
BOOL PROGRAMM_OSD;
BOOL PROGRAMM_OSD_RUN;
// ox -- Original: int Programm_OSD_startY=560;
// Programm-Info höher beginnen
int Programm_OSD_startY=540;

HANDLE Programm_OSD_Thread=NULL;
HANDLE Programm_OSD_Stop=NULL;
HANDLE OSD_KEY_EVENT=NULL;
HANDLE DLL_OSD_Created=NULL;

unsigned char OSD_Key_Value;
int EIT_TopLine=0;
struct OSD_Colors_t OSD_Colors[4];
BOOL OSD_COLOR=FALSE;

struct DecoderState_t DecoderStatus;
extern BOOL Fail_BOOT;

char *service_type_tbl[16] = {
	"Reserved",
	"digital television service",
	"digital radio sound service",
	"television service",
	"NVOD reference service",
	"NVOD time-shifted service",
	"mosaic service",
	"PAL coded signal",
	"D/D2-MAC",
	"FM Radio",
	"NTSC coded signal",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};


// Recorder Externals
extern ring_buf *recorder_buf;
extern struct IndexHeader_t HeaderInfo;
extern BOOL Record;
extern BOOL Play;
extern BOOL Recorder_Pause;


SAA7146_ISR_IO IRQsaa;

extern BOOL LoadExternBTDriver;
extern BOOL ExternBTDriver;


WD_KERNEL_PLUGIN DVBKernelPlugIn;
struct TStreamLog StreamLog;

extern HBITMAP             BirneRot;
extern HBITMAP             BirneGruen;


/// OSD_Bitmap;


struct TOSDBitMap OSDBitMap;

unsigned short Set_TextPid;

unsigned char SAA_VT_Buffer[1010];

extern FILE *winbisfp;

int VT_Stream_ID=-1;

extern BOOL DVB_VT;
extern BOOL DVB_WINBIS;


extern unsigned char invtab[256];
extern unsigned char unhamtab[256];

BOOL DVB_IRQ_SHARING=FALSE;

struct tMultiLink MultiLinkZeit[16];
struct tMultiLink MultiLinkView[16];
struct tPidParam PidParam[16];

extern int TP_Log_Anzahl;
extern struct T_TP_Log TP_Log[128];


extern struct tSECA_Informs Seca_Information;
extern struct tSECA Seca_Log;

extern struct tIrdeto_Informs Irdeto_Information;
extern struct tIrdeto Irdeto_Log;

extern struct TTiming Timing;

extern unsigned char Used_IRQ_List[256];


BOOL FastBoot=FALSE;
BOOL Inversion=FALSE;
// Merker für FastSwitch
BOOL FastSwitch = FALSE;

DWORD AltSymbolrate=0;
int Altfec =-1;
unsigned char AltControlVal = 0xff;
int Alt22Khz=-1;
unsigned __int64 altfreq=0;
unsigned short AltQAM_Mode=0xffff;

int AltDiseqCNr=0xffffff;
int AltBand=0xffffff;
int AltVolt=0xffffff;

int Debi_Irq=0;
BOOL Run_Debi_Watch;

unsigned char EIT_Buffer[8192];
unsigned int EIT_Buffer_Offset=0;
char LastIntText[128];
