/*********************************************************************************
 *                                                                               *
 * dvr.h: Digital Video Recorder                                                 *
 *                                                                               *
 * Copyright (C) 2000/2001 Espresso                                                   *
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

#define    AudioMPG              0x01
#define    VideoMPG              0x04
#define    AV_MPG                0x05

#define TIMERANZAHL 12


#define EIT_INDEX_SIZE 0x10000

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
    COMTYPE_TELETEXT,
	COMTYPE_CI_LL
} COMTYPE;


typedef enum  { __Record,
				__Stop,
				__Play,
				__Pause,
				__Slow,
				__FF_IP,
				__Scan_I,
				__Continue
			  } REC_PLAY;

typedef enum  { SetVidMode,
			    SetTestMode,
			    LoadVidCode,
				SetMonitorType,
				SetPanScanType,
            	SetFreezeMode
			  } ENC;


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


typedef struct SAA7146_STRUCT *SAA7146_HANDLE;


extern HANDLE hInst;

extern SAA7146_HANDLE hSAA7146;

extern struct Time_Recorder_t TimerRec[TIMERANZAHL];
extern struct TEIT_Index *EIT;

extern char CurrentDir[256];

extern unsigned int MAXMegaBytes;
extern struct OSD_Colors_t OSD_Colors[4];
extern HWND hWnd;
extern void Write_Log(const char *Text);
extern struct TDT_t TDT;
extern int CurrentProgramm;
extern char ChannelString[10];
extern BOOL PROGRAMM_OSD;
extern BOOL PROGRAMM_OSD_RUN;
extern HANDLE OSD_KEY_EVENT;
extern unsigned char OSD_Key_Value;
extern WD_DMA Record_mem_dma;


unsigned short ButtonSetPlay[10] = { 10,1,1,1,1,1,1,1,1,1};
unsigned short ButtonSetRecord[10] = { 10,1,1,1,1,10,1,1,1,1};

int RecorderFormat=2;
int RecorderTyp=0;
int CurrentRecordMode=2;
int CurrentPlayMode=2;

unsigned int MegaBytes_Recorded;

struct IndexHeader_t HeaderInfo;

char RecordBaseDir[9][128];
char LastRecorderFile[256];
int RecordBase=0;
int RecordDateiNummer=0;
char RecorderName[256];

BOOL Record=FALSE;
BOOL Play=FALSE;
BOOL Recorder_Pause=FALSE;
BOOL Recorder_FF=FALSE;
BOOL Recorder_Slow=FALSE;
int SlowCount=0;
int RecorderFile=-1;
int InfoRet;
#define RECORDER_BUFFER_SIZE 4096000
ring_buf *recorder_buf = NULL;


#define AV_PES_HEADER_LEN 8
// AV_PES block types:
#define AV_PES_VIDEO 1
#define AV_PES_AUDIO 2



// MPG Stuff

#define PICTURE_START_CODE   0x00
#define SLICE_START_CODE_S   0x01
#define SLICE_START_CODE_E   0xaf
/* reserved 0xb0 */
/* reserved 0xb1 */
#define USER_DATA_START_CODE 0xb2
#define SEQUENCE_HEADER_CODE 0xb3
#define SEQUENCE_ERROR_CODE  0xb4
#define EXTENSION_START_CODE 0xb5
/* reserved 0xb6 */
#define SEQUENCE_END_CODE    0xb7
#define GROUP_START_CODE     0xb8
#define SYSTEM_START_CODE_S  0xb9

#define PROGRAM_END          0xB9
#define PROG_PACK_START      0xBA
#define SYS_HEADER_STRT      0xBB
#define PROG_STREAM_MAP      0xBC
#define PRIVATE_STREAM1      0xBD
#define PADDING_STREAM       0xBE
#define PRIVATE_STREAM2      0xBF

#define AUDIO_STREAM_S       0xC0
#define AUDIO_STREAM_E       0xDF

#define VIDEO_STREAM_S       0xE0
#define VIDEO_STREAM_E       0xEF

#define ECM_STREAM           0xF0
#define EMM_STREAM           0xF1
#define DSM_CC_STREAM        0xF2
#define ISO13522_STREAM      0xF3
#define PROG_STREAM_DIR      0xFF

#define SYSTEM_START_CODE_E  0xff

static const unsigned char picture_start_code[] = {0, 0, 1, PICTURE_START_CODE };
static const unsigned char seq_hdr_code[] =       {0, 0, 1, SEQUENCE_HEADER_CODE };
static const unsigned char seq_end_code[] =       {0, 0, 1, SEQUENCE_END_CODE};
static const unsigned char program_end_code[] =   {0, 0, 1, PROGRAM_END};

//pts_dts flags 
#define PTS_ONLY         0x80
#define PTS_DTS          0xC0
#define PTS_FLAG         0x10
#define PTS_DTS_FLAGS    0xC0


#define AV_VIDEO 0x01
#define AV_AUDIO 0x02
#define AV_RES   0x55

#define MAX_AV      0x17f8
#define MAX_AVA     0x07f8
#define MAX_PLENGTH 0x800

#define PS_MAX 200


	enum {
		  AV_PES_STREAM, 
		  PS_STREAM, 
		  TS_STREAM, 
		  PES_STREAM
	};


	typedef struct av_pes_packet_{
		unsigned char stream_id;
		unsigned char counter;
		unsigned char reserved;
		unsigned char flags;
		unsigned char llength[2];
		unsigned char pts[4];
		short  length;
		unsigned char gap;
		unsigned char payload[MAX_AV];
	} av_pes_packet;
	

typedef struct p2astruct {
		int found;
		unsigned char cid;
		av_pes_packet apsv;
		av_pes_packet apsa;
		unsigned int plength;
		unsigned char plen[2];
		unsigned char flag1;
// flag1 value : aabbcdef :
// aa = '10'
// bb = PES_scrambling_control
// c  = PES_priority
// d  = data_alignment_indicator
// e  = copyright
// f  = original_or_copy
		unsigned char flag2;
// flag2 value : aabcdefg :
// aa = PTS_DTS_flags (00 --> none, 10 : PTS only, 11 : PTS and DTS)
// b  = ESCR_flag
// c  = ES_rate_flag
// d  = DSM_trick_mode_flag
// e  = additional_copy_info_flag
// f  = PES_CRC_flag
// g  = PES_extension_flag
		unsigned char hlength; // PES_header_data_length
		unsigned char pts[5];
		int mpeg;
		unsigned char check;
		int fd1;
		int av;
		int which;
		int done;
	} p2a;

p2a Mpeg2AV_Pes;

typedef struct a2pstruct{
		int type;
		int fd;
		int found;
		int length;
		int headr;
		int plength;
		unsigned char cid;
		unsigned char flags;
		unsigned char abuf[MAX_PLENGTH];
		int alength;
		unsigned char vbuf[MAX_PLENGTH];
		int vlength;
	    unsigned char last_av_pts[4];
		unsigned char av_pts[4];
		unsigned char scr[4];
		unsigned char pid0;
		unsigned char pid1;
		unsigned char pidv;
		unsigned char pida;
        BOOL Get_Sync;
		unsigned int LastTime;
	} a2p;

a2p AV_Pes2Mpeg;

typedef struct ps_packet_{
		unsigned char scr[6];
		unsigned char mux_rate[3];
		unsigned char stuff_length;
		unsigned char *data;
		unsigned char sheader_llength[2];
		int sheader_length;
		unsigned char rate_bound[3];
		unsigned char audio_bound;
		unsigned char video_bound;
		unsigned char reserved;
		int npes;
		int mpeg;
	} ps_packet;

unsigned char *PAV_Audio_Buffer=NULL;
int PAV_Audio_Buffer_Length;
int PAV_Audio_Buffer_Pos;

unsigned char *PAV_Video_Buffer=NULL;
int PAV_Video_Buffer_Length;
int PAV_Video_Buffer_Pos;

#define MPEG_BUFFER_SIZE 0x2000

unsigned char *Mpeg_Buffer=NULL;
int Mpeg_Buffer_Length;
int Mpeg_Buffer_Pos;

int AudioFilePtr;
int VideoFilePtr;
unsigned int AudioBlocks;
unsigned int VideoBlocks;


