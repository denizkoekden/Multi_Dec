
enum { 
	SAA7146_IOCTL_VERSION = 1, 
	SAA7146_IOCTL_SET_INIT = 2,
	SAA7146_IOCTL_GET_ISR = 3,
	SAA7146_IOCTL_WRITETEXT = 10,
	SAA7146_IOCTL_COMMAND = 11,
	SAA7146_IOCTL_COMMANDREQUEST = 12,
	SAA7146_IOCTL_ARM_CLEARMAILBOX = 13,
	SAA7146_IOCTL_ARM_CLEARIRQ = 14,
	SAA7146_IOCTL_ARM_SET_IRQ_STATE_EXT = 15,
	SAA7146_IOCTL_PLAY_BUFFER = 16,
	SAA7146_IOCTL_TEXT_DONE = 17,
	SAA7146_IOCTL_SEND_CA = 18,
	SAA7146_IOCTL_LOAD_BITMAP = 20,
	SAA7146_IOCTL_ARM_RESETMAILBOX = 21,
    SAA7146_IOCTL_SET_SPIN_LOCK = 30,
    SAA7146_IOCTL_RELEASE_SPIN_LOCK = 31,
	SAA7146_IOCTL_SET_TIMEING = 34,
	SAA7146_IOCTL_ARM_SET_SEND = 35,
	SAA7146_IOCTL_ARM_SET_RECEIVE = 36,
	SAA7146_IOCTL_CLEAR_RECEIVE = 40,

}; // in this sample we implement a GetVersion message

typedef struct {
    BYTE  cVer[100];
} SAA7146_VERSION;



typedef struct {
	DWORD PictureSet;
	DWORD debistatus;
	DWORD debiTyp;
	DWORD debilen;
    DWORD Debi_Done_MC2;
    DWORD Debi_Done_PSR;
} SAA7146_ISR_IO;

typedef struct {
    DWORD ComAddress;
    DWORD debimem;
	DWORD recordmem;
	DWORD camem;
	DWORD cimem;
	DWORD ipmpemem;
	DWORD bitmapmem;
	DWORD irmem;
	DWORD debugmem;
    DWORD Sharing;
} SAA7146_SET_IO;


typedef struct {
		   WORD  CommandLength;
		   WORD  TextLength;
		   WORD  CommandBuffer[32];
		   DWORD TextBuffer[64];
} SAA7146_WRITETEXT_IO;

typedef struct {
		   WORD CommandLength;
	       WORD CommandBuffer[128];
} SAA7146_COMMAND_IO;


typedef struct {
		   WORD  OutBuf[32];
		   DWORD OutLength;
		   WORD  InBuf[32];
		   DWORD InLength;
} SAA7146_COMMANDREQUEST_IO;

typedef struct {
		   DWORD value;
		   DWORD Addresse;
} SAA7146_SHORTSEND_IO;

typedef struct {
		   DWORD  Cmd_Time_Out;
		   DWORD  OSD_Text_Time_Out;
		   DWORD  Debi_done_MC2;
		   DWORD  Debi_done_PSR;
} SAA7146_TIMING_IO;


enum { SAA7146_WRONG_MESSAGE = 0x1000 };

enum { SAA7146_OK = 1 };
