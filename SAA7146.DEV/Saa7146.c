
#include "../DrvLib/kpstdlib.h"
#include "../DrvLib/wd_kp.h"
#include "SAAIOcom.h"

#define MASK_19           0x00080000    /* Mask value for bit 19 */
#define IER               0xDC  /* Interrupt enable register */
#define MC2               0x100  /* Main control register 2  */
#define ISR               0x10C  /* Interrupt status register */                                                             
#define PSR               0x110  /* Primary status register */

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

#define	OSDQFull	0x0010			//OSD Queue Full

#define	DPRAM_BASE	0x4000
#define IRQ_RX		    (DPRAM_BASE + 0x1FFC)
#define IRQ_TX		    (DPRAM_BASE + 0x1FFE)
#define IRQ_STATE		(DPRAM_BASE + 0x0F4)
#define IRQ_STATE_EXT	(DPRAM_BASE + 0x0F6)
#define DATA_BUFF_BASE	(DPRAM_BASE + 0x200)
#define MSGSTATE		(DPRAM_BASE + 0x0F8)
#define FILT_STATE	    (DPRAM_BASE + 0x0FA)
#define COMMAND		    (DPRAM_BASE + 0x0FC)
#define COM_BUFF		(DPRAM_BASE + 0x100)
#define BUFF1_BASE	    (DPRAM_BASE + 0x120)
#define RX_BUFF         (DPRAM_BASE + 0x1FF4)
#define TX_BUFF 	    (DPRAM_BASE + 0x1FF6)
#define HANDSHAKE_REG	(DPRAM_BASE + 0x1FF8)
#define TX_LEN          (DPRAM_BASE + 0x1FEE)
#define EXTRA_BASE  	(DPRAM_BASE + 0x1E00)

#define DEBI_CONFIG       0x7C
#define DEBI_COMMAND      0x80
#define DEBI_PAGE         0x84
#define DEBI_AD           0x88	
#define SPCI_DEBI_S     0x00080000  /* DEBI status */

#define DEBINOSWAB 0x000e0000
#define DEBISWAB   0x001e0000

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
} COMTYPE;


#define GPIO_CTRL         0xE0  /* GPIO 0-3 register */

BOOL __cdecl KP_Open(KP_OPEN_CALL *kpOpenCall, HANDLE hWD, PVOID pOpenData, PVOID *ppDrvContext);
void __cdecl KP_Close(PVOID pDrvContext);
void __cdecl KP_Call(PVOID pDrvContext, WD_KERNEL_PLUGIN_CALL *kpCall, BOOL fIsKernelMode);
BOOL __cdecl KP_IntEnable(PVOID pDrvContext, WD_KERNEL_PLUGIN_CALL *kpCall, PVOID *ppIntContext);
void __cdecl KP_IntDisable(PVOID pIntContext);
BOOL __cdecl KP_IntAtIrql(PVOID pIntContext, BOOL *fIsMyInterrupt);
DWORD __cdecl KP_IntAtDpc(PVOID pIntContext, DWORD dwCount);
BOOL gpioirq(HANDLE hWD);
BOOL debiirq(HANDLE hWD);
void ARM_ClearMailBox(HANDLE hWD);
void ARM_ClearIrq(HANDLE hWD);
void ARM_Set_IRQ_STATE_EXT(HANDLE hWD,DWORD val);
void ARM_Set_TX_LEN(HANDLE hWD,DWORD val);
void ARM_Set_TX_BUFF(HANDLE hWD,DWORD val);
void ARM_Set_RX_BUFF(HANDLE hWD,DWORD val);
int SAA7146_Wait_for_Debi_done(HANDLE hWD);
DWORD SAA7146_DebiWrite(HANDLE hWD, DWORD config, DWORD addr, DWORD val, DWORD count);
DWORD SAA7146_DebiCommandWrite(HANDLE hWD, DWORD config, DWORD addr, DWORD val, DWORD count);
DWORD SAA7146_DebiRead(HANDLE hWD, DWORD config, DWORD addr, DWORD count);
DWORD SAA7146_RecordRead(HANDLE hWD,DWORD Addresse, DWORD count);
DWORD SAA7146_RecordWrite(HANDLE hWD, DWORD Addresse, DWORD count);
DWORD SAA7146_CAWrite(HANDLE hWD, DWORD Addresse, DWORD count);
DWORD SAA7146_BitmapWrite(HANDLE hWD, DWORD Addresse, DWORD Offset, DWORD count);
DWORD SAA7146_TextWrite(HANDLE hWD, DWORD count);
DWORD SAA7146_CIRead(HANDLE hWD,DWORD Addresse, DWORD count);
DWORD SAA7146_CARead(HANDLE hWD,DWORD Addresse, DWORD count);
DWORD SAA7146_IPMPERead(HANDLE hWD, DWORD Addresse, DWORD count);
DWORD SAA7146_DebugRead(HANDLE hWD, DWORD Addresse, DWORD count);
DWORD SAA7146_IrRead(HANDLE hWD, DWORD Addresse, DWORD count);
DWORD SAA7146_WriteText(HANDLE hWD, WORD CommandLength, DWORD *CommandBuffer, WORD TextLength,DWORD *TextBuffer);
void Set_Spin_Lock( void );
void Release_Spin_Lock( HANDLE hWD );
int SAA7146_OutCommand(HANDLE hWD, DWORD *buf, WORD length);
int SAA7146_CommandRequest(HANDLE hWD, DWORD *Buff, DWORD length, WORD *buf, DWORD n);
void Debi_On(HANDLE hWD);
void Debi_Off(HANDLE hWD);
void ARM_ResetMailBox(HANDLE hWD);
void SAA7146_SetGPIO( HANDLE hWD, DWORD port, DWORD data);


SAA7146_ISR_IO SAA7146_ISR;
SAA7146_SET_IO SAA7146_SET;
SAA7146_WRITETEXT_IO SAA7146_WriteText_Buffer;
SAA7146_COMMAND_IO SAA7146_Command_Buffer;
SAA7146_COMMANDREQUEST_IO SAA7146_CommandRequest_Buffer;
SAA7146_SHORTSEND_IO SAA7146_ShortSend_Buffer;
SAA7146_TIMING_IO SAA7146_Timing;


DWORD BitmapLength=0;
DWORD BitmapOffset=0;

DWORD PlayLength=0;

unsigned char *Daten;

DWORD DebiLen=0;
DWORD DebiType=-1;

DWORD rxbuf;
DWORD txbuf;

DWORD Hold_GPIO_Irq=0;
DWORD LockStateExtern=0;
DWORD Debi_On_Set=0;



BOOL __cdecl KP_Init(KP_INIT *kpInit)
{
    // check if the version of WD_KP.LIB is the same version as WINDRVR.H and WD_KP.H

    kpInit->funcOpen = KP_Open;
    strcpy (kpInit->cDriverName, "MDSAA"); // until 8 chars
    SAA7146_SET.bitmapmem=0;
    SAA7146_SET.cimem=0;
    SAA7146_SET.ComAddress=0;
	SAA7146_SET.debimem=0;
    SAA7146_SET.ipmpemem=0;
	SAA7146_SET.recordmem=0;
	SAA7146_SET.irmem=0;
	SAA7146_SET.debugmem=0;
	SAA7146_SET.camem=0;
	SAA7146_SET.Sharing=1;
    SAA7146_Timing.Cmd_Time_Out=15000;
    SAA7146_Timing.OSD_Text_Time_Out=60000;
    SAA7146_Timing.Debi_done_MC2=2000;
    SAA7146_Timing.Debi_done_PSR=10000;
    SAA7146_ISR.Debi_Done_PSR=0;
    SAA7146_ISR.Debi_Done_MC2=0;
    return TRUE;
}

// called when WD_KernelPlugInOpen() is called. pDrvContext returned will be passed to 
// rest of the functions
BOOL __cdecl KP_Open(KP_OPEN_CALL *kpOpenCall, HANDLE hWD, PVOID pOpenData, PVOID *ppDrvContext)
{
    *ppDrvContext =(PVOID)hWD;

	kpOpenCall->funcClose = KP_Close;
    kpOpenCall->funcCall = KP_Call;
    kpOpenCall->funcIntEnable = KP_IntEnable;
    kpOpenCall->funcIntDisable = KP_IntDisable;
    kpOpenCall->funcIntAtIrql = KP_IntAtIrql;
    kpOpenCall->funcIntAtDpc = KP_IntAtDpc;
    SAA7146_SET.cimem=0;
    SAA7146_SET.ComAddress=0;
	SAA7146_SET.debimem=0;
    SAA7146_SET.ipmpemem=0;
	SAA7146_SET.recordmem=0;
	SAA7146_SET.camem=0;
    SAA7146_SET.bitmapmem=0;
	SAA7146_SET.irmem=0;
	SAA7146_SET.debugmem=0;
	SAA7146_SET.Sharing=1;
    SAA7146_Timing.Cmd_Time_Out=15000;
    SAA7146_Timing.OSD_Text_Time_Out=60000;
    SAA7146_Timing.Debi_done_MC2=2000;
    SAA7146_Timing.Debi_done_PSR=10000;
    SAA7146_ISR.Debi_Done_PSR=0;
    SAA7146_ISR.Debi_Done_MC2=0;
    // you can allocate here memory
    return TRUE;
}

// called when WD_KernelPlugInClose() is called
void __cdecl KP_Close(PVOID pDrvContext)
{
    // you can free here the memory allocated pDrvContext
    SAA7146_SET.bitmapmem=0;
    SAA7146_SET.cimem=0;
    SAA7146_SET.ComAddress=0;
	SAA7146_SET.debimem=0;
    SAA7146_SET.ipmpemem=0;
	SAA7146_SET.recordmem=0;
	SAA7146_SET.camem=0;
	SAA7146_SET.Sharing=1;
    SAA7146_ISR.Debi_Done_PSR=0;
    SAA7146_ISR.Debi_Done_MC2=0;

}

// called when WD_KernelPlugInCall() is called
void __cdecl KP_Call(PVOID pDrvContext, WD_KERNEL_PLUGIN_CALL *kpCall, BOOL fIsKernelMode)
{
	HANDLE hWD=(HANDLE)pDrvContext;
	WD_TRANSFER trans;
    kpCall->dwResult = SAA7146_OK;

    switch ( kpCall->dwMessage )
    {
    case SAA7146_IOCTL_VERSION: // GetVersion message
        { 
            SAA7146_VERSION *ver = (SAA7146_VERSION *) kpCall->pData;
            COPY_TO_USER_OR_KERNEL(ver->cVer, "SAA7146-Kernel-Driver V2.70", sizeof("SAA7146-Kernel-Driver V2.70")+1, fIsKernelMode);
            kpCall->dwResult = SAA7146_OK;
            return;
        }

    case SAA7146_IOCTL_SET_INIT: // Set message
        { 
            SAA7146_SET_IO *adr = (SAA7146_SET_IO *) kpCall->pData;
            COPY_TO_USER_OR_KERNEL(&SAA7146_SET.ComAddress,&adr->ComAddress, sizeof(SAA7146_SET_IO), fIsKernelMode);
			kpCall->dwResult = SAA7146_OK;
            return;
        }

    case SAA7146_IOCTL_GET_ISR: // Get message
        { 
            SAA7146_ISR_IO *adr = (SAA7146_ISR_IO *) kpCall->pData;
            COPY_TO_USER_OR_KERNEL(&adr->PictureSet,&SAA7146_ISR.PictureSet, sizeof(SAA7146_ISR_IO), fIsKernelMode);
			SAA7146_ISR.debistatus=0;
			SAA7146_ISR.Debi_Done_PSR=0;
			SAA7146_ISR.Debi_Done_MC2=0;
			kpCall->dwResult = SAA7146_OK;
            return;
        }
    case SAA7146_IOCTL_WRITETEXT: // Get message
        { 
            SAA7146_WRITETEXT_IO *TextInfo = (SAA7146_WRITETEXT_IO *) kpCall->pData;
            COPY_TO_USER_OR_KERNEL(&SAA7146_WriteText_Buffer.CommandLength,&TextInfo->CommandLength,sizeof(SAA7146_WRITETEXT_IO), fIsKernelMode);
            kpCall->dwResult=SAA7146_WriteText(hWD,SAA7146_WriteText_Buffer.CommandLength,SAA7146_WriteText_Buffer.CommandBuffer,SAA7146_WriteText_Buffer.TextLength,SAA7146_WriteText_Buffer.TextBuffer);
            return;
        }

    case SAA7146_IOCTL_COMMAND: // Get message
        { 
            SAA7146_COMMAND_IO *CommandInfo = (SAA7146_COMMAND_IO *) kpCall->pData;
            COPY_TO_USER_OR_KERNEL(&SAA7146_Command_Buffer.CommandLength,&CommandInfo->CommandLength,sizeof(SAA7146_COMMAND_IO), fIsKernelMode);
            kpCall->dwResult=SAA7146_OutCommand(hWD,SAA7146_Command_Buffer.CommandBuffer,SAA7146_Command_Buffer.CommandLength);
            return;
        }

    case SAA7146_IOCTL_COMMANDREQUEST: // Get message
        { 
            SAA7146_COMMANDREQUEST_IO *CommandRequest = (SAA7146_COMMANDREQUEST_IO *) kpCall->pData;
            COPY_TO_USER_OR_KERNEL(&SAA7146_CommandRequest_Buffer.OutBuf[0],&CommandRequest->OutBuf[0],sizeof(SAA7146_COMMANDREQUEST_IO), fIsKernelMode);
			kpCall->dwResult=SAA7146_CommandRequest(hWD,&SAA7146_CommandRequest_Buffer.OutBuf[0],SAA7146_CommandRequest_Buffer.OutLength,&SAA7146_CommandRequest_Buffer.InBuf[0],SAA7146_CommandRequest_Buffer.InLength);
            COPY_TO_USER_OR_KERNEL(&CommandRequest->OutBuf[0],&SAA7146_CommandRequest_Buffer.OutBuf[0],sizeof(SAA7146_COMMANDREQUEST_IO), fIsKernelMode);
            return;
        }

	case SAA7146_IOCTL_SET_TIMEING:
        { 
            SAA7146_TIMING_IO *TimerInfo = (SAA7146_TIMING_IO *) kpCall->pData;
            COPY_TO_USER_OR_KERNEL(&SAA7146_Timing.Cmd_Time_Out,&TimerInfo->Cmd_Time_Out,sizeof(SAA7146_TIMING_IO), fIsKernelMode);
            kpCall->dwResult=SAA7146_OK;
            return;
        }

    case SAA7146_IOCTL_ARM_CLEARMAILBOX: // Get message
        { 
			ARM_ClearMailBox(hWD);
			kpCall->dwResult = SAA7146_OK;
            return;
        }


    case SAA7146_IOCTL_ARM_RESETMAILBOX: // Get message
        { 
			ARM_ResetMailBox(hWD);
			kpCall->dwResult = SAA7146_OK;
            return;
        }

    case SAA7146_IOCTL_ARM_CLEARIRQ: // Get message
        { 
			ARM_ClearIrq(hWD);
			kpCall->dwResult = SAA7146_OK;
            return;
        }
 
 
    case SAA7146_IOCTL_ARM_SET_IRQ_STATE_EXT: // Get message
        { 

			SAA7146_SHORTSEND_IO *CommandInfo = (SAA7146_SHORTSEND_IO *) kpCall->pData;
            COPY_TO_USER_OR_KERNEL(&SAA7146_ShortSend_Buffer.value,&CommandInfo->value,sizeof(SAA7146_SHORTSEND_IO), fIsKernelMode);
			ARM_Set_IRQ_STATE_EXT(hWD,SAA7146_ShortSend_Buffer.value);
			kpCall->dwResult = SAA7146_OK;
            return;
        }

    case SAA7146_IOCTL_ARM_SET_SEND: // Get message
        { 

			SAA7146_SHORTSEND_IO *CommandInfo = (SAA7146_SHORTSEND_IO *) kpCall->pData;
            COPY_TO_USER_OR_KERNEL(&SAA7146_ShortSend_Buffer.value,&CommandInfo->value,sizeof(SAA7146_SHORTSEND_IO), fIsKernelMode);
			ARM_Set_IRQ_STATE_EXT(hWD,SAA7146_ShortSend_Buffer.value);
			ARM_Set_TX_LEN(hWD,SAA7146_ShortSend_Buffer.value);
			ARM_Set_TX_BUFF(hWD,SAA7146_ShortSend_Buffer.value);
			kpCall->dwResult = SAA7146_OK;
            return;
        }

    case SAA7146_IOCTL_CLEAR_RECEIVE: // Get message
        { 

			ARM_Set_RX_BUFF(hWD,0);
			ARM_ResetMailBox(hWD);
			kpCall->dwResult = SAA7146_OK;
            return;
        }


    case SAA7146_IOCTL_ARM_SET_RECEIVE: // Get message
        { 

			SAA7146_SHORTSEND_IO *CommandInfo = (SAA7146_SHORTSEND_IO *) kpCall->pData;
            COPY_TO_USER_OR_KERNEL(&SAA7146_ShortSend_Buffer.value,&CommandInfo->value,sizeof(SAA7146_SHORTSEND_IO), fIsKernelMode);
			ARM_Set_RX_BUFF(hWD,SAA7146_ShortSend_Buffer.value);
			kpCall->dwResult = SAA7146_OK;
            return;
        }
    case SAA7146_IOCTL_SEND_CA: // Get message
        { 

			SAA7146_SHORTSEND_IO *CommandInfo = (SAA7146_SHORTSEND_IO *) kpCall->pData;
           	SAA7146_ISR.debistatus=(DWORD)0;
            COPY_TO_USER_OR_KERNEL(&SAA7146_ShortSend_Buffer.value,&CommandInfo->value,sizeof(SAA7146_SHORTSEND_IO), fIsKernelMode);
			ARM_Set_TX_LEN(hWD,SAA7146_ShortSend_Buffer.value);
			ARM_Set_IRQ_STATE_EXT(hWD,SAA7146_ShortSend_Buffer.value);
			Debi_On(hWD);
			SAA7146_CAWrite(hWD, DPRAM_BASE+txbuf,SAA7146_ShortSend_Buffer.value);
			kpCall->dwResult = SAA7146_OK;
        return;
        }
    case SAA7146_IOCTL_PLAY_BUFFER: // Get message
        { 

			SAA7146_SHORTSEND_IO *CommandInfo = (SAA7146_SHORTSEND_IO *) kpCall->pData;
           	SAA7146_ISR.debistatus=(DWORD)0;
            COPY_TO_USER_OR_KERNEL(&SAA7146_ShortSend_Buffer.value,&CommandInfo->value,sizeof(SAA7146_SHORTSEND_IO), fIsKernelMode);
			ARM_Set_TX_LEN(hWD,SAA7146_ShortSend_Buffer.value);
			ARM_Set_IRQ_STATE_EXT(hWD,SAA7146_ShortSend_Buffer.value);
			Debi_On(hWD);
			SAA7146_RecordWrite(hWD, DPRAM_BASE+txbuf,SAA7146_ShortSend_Buffer.value);
			kpCall->dwResult = SAA7146_OK;
        return;
        }

    case SAA7146_IOCTL_LOAD_BITMAP: // Get message
        { 

			SAA7146_SHORTSEND_IO *CommandInfo = (SAA7146_SHORTSEND_IO *) kpCall->pData;
            COPY_TO_USER_OR_KERNEL(&SAA7146_ShortSend_Buffer.value,&CommandInfo->value,sizeof(SAA7146_SHORTSEND_IO), fIsKernelMode);
			BitmapOffset=0;
			BitmapLength=SAA7146_ShortSend_Buffer.Addresse;
			kpCall->dwResult = SAA7146_OK;
            return;
        }
   case SAA7146_IOCTL_SET_SPIN_LOCK: // Get message
        { 
			Hold_GPIO_Irq=0;
            LockStateExtern=0x00000001;
			kpCall->dwResult = SAA7146_OK;
            return;
        }


    case SAA7146_IOCTL_RELEASE_SPIN_LOCK: // Get message
        { 
            LockStateExtern=0x00000000;
			if ( Hold_GPIO_Irq == 1 ) {
//				SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, PSR,	0x8);
              trans.cmdTrans = WM_DWORD;
              trans.dwPort   = SAA7146_SET.ComAddress + PSR;
              trans.Data.Dword=0x00000008;
	          WD_Transfer(hWD,&trans);
			}
			kpCall->dwResult = SAA7146_OK;
           return;
        }

		// you can implement here other messages
    default:
        kpCall->dwResult = SAA7146_WRONG_MESSAGE;
         return;
    }
}

// called when WD_IntEnable() is called, with a kernel plugin handler specified
// the pIntContext will be passed to the rest of the functions handling interrupts.
// returns TRUE if enable is succesful
BOOL __cdecl KP_IntEnable(PVOID pDrvContext, WD_KERNEL_PLUGIN_CALL *kpCall, PVOID *ppIntContext)
{
    // you can allocate memory specific for each interrupt in *ppIntContext
 //   *ppIntContext = malloc(sizeof (DWORD)); 
 //   if (!*ppIntContext)
 //       return FALSE;
    // in this sample the information is a DWORD used to count the incomming interrupts
	*ppIntContext=pDrvContext;

    return TRUE;
}

// called when WD_IntDisable() is called
void __cdecl KP_IntDisable(PVOID pIntContext)
{

    // you can free here the interrupt specific memory in pIntContext
}

// returns TRUE if needs DPC.
// this function is called at IRQL level - at physical interrupt handler.
// most library calls are NOT allowed, for example:
// NO   WD_xxxx() calls, except WD_Transfer(), 
// NO   malloc, 
// NO   free
// YES  WD_Transfer
// YES  your functions, as long as they dont call library functions
// YES  specific kernel functions, that the Win DDK specifically allows them to be
//      called at IRQL 
BOOL __cdecl KP_IntAtIrql(PVOID pIntContext, BOOL *pfIsMyInterrupt)
{
	DWORD isr_status;
	DWORD isr_reset;

    BOOL ret1=FALSE;
    BOOL ret2=FALSE;
    BOOL ret3=FALSE;
	WD_TRANSFER trans;

	HANDLE hWD=(HANDLE)pIntContext;

    
   *pfIsMyInterrupt = FALSE;
    if ( SAA7146_SET.ComAddress == 0 ) return(FALSE);

    trans.cmdTrans = RM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + ISR;
	WD_Transfer(hWD,&trans);
	isr_status=trans.Data.Dword;
	SAA7146_ISR.PictureSet=0;
	isr_reset=0;
	if ((isr_status&0x00080000) == 0x00080000) {
		 isr_reset+=0x00080000;
         *pfIsMyInterrupt = TRUE;
		 ret1=debiirq(hWD);
	} 
	if ((isr_status&0x00000008) == 0x00000008 ) {
		 isr_reset+=0x00000008;
		*pfIsMyInterrupt = TRUE;
		 if ( LockStateExtern == 0 ) ret2=gpioirq(hWD);
		 else {
			 Hold_GPIO_Irq=1;
             Debi_On_Set=0;
		 }
		};

	if ( (isr_status&0x08000000) == 0x08000000) {
   *pfIsMyInterrupt = TRUE;
    ret3=TRUE;
    isr_reset+=0x08000000;
	SAA7146_ISR.PictureSet=(DWORD)0x01;
	};

    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + ISR;
	if ( SAA7146_SET.Sharing == 0 ) trans.Data.Dword=isr_reset;
	 else trans.Data.Dword=isr_status;
	WD_Transfer(hWD,&trans);

	if ((ret1==TRUE) || (ret2==TRUE) || (ret3==TRUE)) return(TRUE);
    return(FALSE);
}

// returns the number of times to notify user-mode (i.e. return from WD_IntWait)
DWORD __cdecl KP_IntAtDpc(PVOID pIntContext, DWORD dwCount)
{
    return dwCount; // return WD_IntWait as many times as KP_IntAtIrql scheduled KP_IntAtDpc()
}


BOOL debiirq(HANDLE hWD)
{
	DWORD type=DebiType;
	Debi_Off(hWD);
	if ( type == 0xffffffff ) {
		        Debi_On_Set=0; 
                ARM_ClearMailBox(hWD);
                ARM_ClearIrq(hWD);
                return(FALSE);
	}
	DebiType=0xffffffff;

	switch (type&0x000000ff) {
	
        case DATA_TS_RECORD:
        case DATA_PES_RECORD:
        case DATA_MPEG_RECORD:
                Debi_On_Set=0; 
			    SAA7146_ISR.debistatus=(DWORD)1;
				SAA7146_ISR.debilen=(DWORD)DebiLen;
                SAA7146_ISR.debiTyp=(DWORD)type;
				return(TRUE);

        
		case DATA_COMMON_INTERFACE:
        case DATA_FSECTION:
        case DATA_DEBUG_MESSAGE:
        case DATA_IPMPE:
        case DATA_PIPING:
        case DATA_NONE:
        case DATA_STREAMING:
        case DATA_CI_GET:
                Debi_On_Set=0; 
			    SAA7146_ISR.debistatus=(DWORD)1;
				SAA7146_ISR.debilen=(DWORD)DebiLen;
                SAA7146_ISR.debiTyp=(DWORD)type;
				return(TRUE);

        case DATA_CI_PUT:
        case DATA_MPEG_PLAY:
        case DATA_BMP_LOAD:
                Debi_On_Set=0; 
			    ARM_Set_TX_BUFF(hWD,0);
				ARM_ClearMailBox(hWD);
                return(FALSE);

        default:
                Debi_On_Set=0; 
			    ARM_Set_RX_BUFF(hWD,0);
			    ARM_Set_TX_BUFF(hWD,0);
				ARM_ClearMailBox(hWD);
                return(FALSE);
	};


};


BOOL gpioirq(HANDLE hWD)
{

	DWORD len;

	BOOL Ret=FALSE;
	Debi_On_Set=1;
	ARM_ClearIrq(hWD);
    Debi_Off(hWD);

	DebiType = SAA7146_DebiRead(hWD, DEBINOSWAB, IRQ_STATE, 2);
    DebiLen  = SAA7146_DebiRead(hWD, DEBINOSWAB, IRQ_STATE_EXT, 2);
    rxbuf= SAA7146_DebiRead(hWD, DEBINOSWAB, RX_BUFF,  2);
    txbuf= SAA7146_DebiRead(hWD, DEBINOSWAB, TX_BUFF,  2);
    len=(DebiLen+3)&(~3);


	switch (DebiType&0x000000ff) {

        case DATA_TS_PLAY:
        case DATA_PES_PLAY:
			 ARM_ClearMailBox(hWD);
             return(FALSE);

        case DATA_TS_RECORD:
        case DATA_PES_RECORD:
        case DATA_MPEG_RECORD:
				Debi_On(hWD);
				SAA7146_RecordRead(hWD,DPRAM_BASE+rxbuf, len );
				return(FALSE);

        case DATA_CI_PUT:
			    SAA7146_ISR.debistatus=(DWORD)1;
				SAA7146_ISR.debilen=(DWORD)DebiLen;
                SAA7146_ISR.debiTyp=(DWORD)DebiType;
                return(TRUE);

        case DATA_MPEG_PLAY:
			    SAA7146_ISR.debistatus=(DWORD)1;
				SAA7146_ISR.debilen=(DWORD)DebiLen;
                SAA7146_ISR.debiTyp=(DWORD)DebiType;
                return(TRUE);

        case DATA_BMP_LOAD:
			    
			if ( DebiLen == 0 ) {
				ARM_Set_IRQ_STATE_EXT(hWD,0);
   			    ARM_Set_TX_LEN(hWD,0);
			    ARM_Set_TX_BUFF(hWD,0);
				ARM_ClearMailBox(hWD);
			    SAA7146_ISR.debistatus=(DWORD)1;
				SAA7146_ISR.debilen=(DWORD)DebiLen;
                SAA7146_ISR.debiTyp=(DWORD)DebiType;
                return(TRUE);
			};
			if ( BitmapLength == 0 ) {
				ARM_Set_IRQ_STATE_EXT(hWD,0);
   			    ARM_Set_TX_LEN(hWD,0);
			    ARM_Set_TX_BUFF(hWD,0);
				ARM_ClearMailBox(hWD);
			    SAA7146_ISR.debistatus=(DWORD)1;
				SAA7146_ISR.debilen=(DWORD)0xffffffff;
                SAA7146_ISR.debiTyp=(DWORD)DebiType;
                return(TRUE);
			};

            if ( len > 2048 ) len=2048;
			if ( len > BitmapLength ) len=(WORD)BitmapLength;
      		
			ARM_Set_IRQ_STATE_EXT(hWD,len);
            ARM_Set_TX_LEN(hWD,len);
           	SAA7146_Wait_for_Debi_done(hWD);
            Debi_On(hWD);
            SAA7146_BitmapWrite(hWD,DPRAM_BASE+txbuf, BitmapOffset, len);
            BitmapOffset+=len;
			BitmapLength-=len;
			return(FALSE);
  
		case DATA_CI_GET:
                if (( len > 4096) || ( len == 0)) {
                    ARM_Set_RX_BUFF(hWD,0);
					ARM_ClearMailBox(hWD);
					return(FALSE);
				};
				Debi_On(hWD);
				SAA7146_CARead(hWD,DPRAM_BASE+rxbuf, len);
                return(FALSE);


        case DATA_COMMON_INTERFACE:
                if (( len > 4096) || ( len == 0)) {
                    ARM_Set_RX_BUFF(hWD,0);
					ARM_ClearMailBox(hWD);
					return(FALSE);
				};
				Debi_On(hWD);
				SAA7146_CIRead(hWD,DPRAM_BASE+rxbuf, len);
                return(FALSE);

        case DATA_NONE:
               ARM_ClearMailBox(hWD);
			   return(FALSE);

        case DATA_IPMPE:
                if (( len > 4096) || ( len == 0)) {
                    ARM_Set_RX_BUFF(hWD,0);
					ARM_ClearMailBox(hWD);
					return(FALSE);
				};
				Debi_On(hWD);
				SAA7146_IPMPERead(hWD,DPRAM_BASE+rxbuf, len);
                return(FALSE);

       case DATA_DEBUG_MESSAGE:
                if (( len <= 0 ) || ( len > 0xff))  {
                    ARM_Set_RX_BUFF(hWD,0);
					ARM_ClearMailBox(hWD);
					return(FALSE);
				};
				Debi_On(hWD);
				SAA7146_DebugRead(hWD, EXTRA_BASE, len);
                return(FALSE);

        case DATA_IRCOMMAND:
				SAA7146_ISR.debilen=SAA7146_IrRead(hWD,EXTRA_BASE,  4);
                SAA7146_ISR.debilen=(SAA7146_ISR.debilen>>16)|(SAA7146_ISR.debilen&0x8000);
			    SAA7146_ISR.debistatus=(DWORD)1;
                SAA7146_ISR.debiTyp=(DWORD)DebiType;
                return(TRUE);

        case DATA_FSECTION:
        case DATA_PIPING:
        case DATA_STREAMING:
                if (( len > 4096) || ( len <= 0)) {
                    ARM_Set_RX_BUFF(hWD,0);
					ARM_ClearMailBox(hWD);
					return(FALSE);
				};
				Debi_On(hWD);
				SAA7146_DebiRead(hWD,  DEBISWAB, DPRAM_BASE+rxbuf, len);
                return(FALSE);
        default:
				ARM_ClearMailBox(hWD);
				return(FALSE);

        }      
return(FALSE);
}


void ARM_ClearMailBox(HANDLE hWD)
{
    Debi_On_Set=0;
	SAA7146_DebiWrite(hWD, DEBINOSWAB, IRQ_RX, 0, 2);
}



void ARM_ResetMailBox(HANDLE hWD)
{
	SAA7146_DebiRead(hWD, DEBINOSWAB, IRQ_RX, 2);
	SAA7146_DebiWrite(hWD, DEBINOSWAB, IRQ_RX, 0, 2);
}

void ARM_ClearIrq(HANDLE hWD)
{
	SAA7146_DebiRead(hWD, DEBINOSWAB, IRQ_RX, 2);
}

void ARM_Set_IRQ_STATE_EXT(HANDLE hWD,DWORD val)
{
	SAA7146_DebiWrite( hWD,DEBINOSWAB, IRQ_STATE_EXT,  (DWORD)val,2);
};

void ARM_Set_TX_LEN(HANDLE hWD,DWORD val)
{
	SAA7146_DebiWrite( hWD,DEBINOSWAB, TX_LEN,  (DWORD)val,2);
};

void ARM_Set_TX_BUFF(HANDLE hWD,DWORD val)
{
	SAA7146_DebiWrite( hWD,DEBINOSWAB, TX_BUFF,  (DWORD)val,2);
};

void ARM_Set_RX_BUFF(HANDLE hWD,DWORD val)
{
	SAA7146_DebiWrite( hWD,DEBINOSWAB, RX_BUFF,  (DWORD)val,2);
};



DWORD SAA7146_DebiWrite(HANDLE hWD, DWORD config, DWORD addr, DWORD val, DWORD count)
{
	DWORD Length=(count << 17) | (addr & 0xffff);
    DWORD PokeReg=(2 << 16) | 2;
	WD_TRANSFER trans;
    if ( SAA7146_SET.ComAddress == 0 ) return(-1);
	if (( count > 4 ) && (SAA7146_SET.debimem == 0 )) return(-1);

	if (count > 0x1000 || count <= 0) count=0x08;
	SAA7146_Wait_for_Debi_done(hWD);

    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_CONFIG;
	trans.Data.Dword=config;
	WD_Transfer(hWD,&trans);

	if (count <= 4) {
	//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,val);
       trans.cmdTrans = WM_DWORD;
       trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	   trans.Data.Dword=val;
	   WD_Transfer(hWD,&trans);

	} else {
	   // SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,(unsigned long)debi_mem_dma.Page[0].pPhysicalAddr);
       trans.cmdTrans = WM_DWORD;
       trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	   trans.Data.Dword=SAA7146_SET.debimem;
	   WD_Transfer(hWD,&trans);
	};

	// SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_COMMAND,Length);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_COMMAND;
	trans.Data.Dword=Length;
	WD_Transfer(hWD,&trans);
    // SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,PokeReg);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + MC2;
	trans.Data.Dword=PokeReg;
	WD_Transfer(hWD,&trans);
	return 0;
}

DWORD SAA7146_DebiCommandWrite(HANDLE hWD, DWORD config, DWORD addr, DWORD val, DWORD count)
{
	DWORD Length=(count << 17) | (addr & 0xffff);
    DWORD PokeReg=(2 << 16) | 2;
	WD_TRANSFER trans;
    if ( SAA7146_SET.ComAddress == 0 ) return(-1);
	if ( count > 4 ) count=4;
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_CONFIG;
	trans.Data.Dword=config;
	WD_Transfer(hWD,&trans);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	trans.Data.Dword=val;
	WD_Transfer(hWD,&trans);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_COMMAND;
	trans.Data.Dword=Length;
	WD_Transfer(hWD,&trans);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + MC2;
	trans.Data.Dword=PokeReg;
	WD_Transfer(hWD,&trans);
	return 0;
}


DWORD SAA7146_DebiRead(HANDLE hWD, DWORD config, DWORD addr, DWORD count)
{
	DWORD result = 0;
	DWORD Length=(count << 17) | 0x10000 | (addr & 0xffff);
    DWORD PokeReg=(2 << 16) | 2;
	WD_TRANSFER trans;

	if (count > 0x1000 || count <= 0) count=0x04;
    if ( SAA7146_SET.ComAddress == 0 ) return(-1);
	if (SAA7146_SET.debimem == 0 ) return(-1);
	SAA7146_Wait_for_Debi_done(hWD);

	//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,(DWORD)debi_mem_dma.Page[0].pPhysicalAddr);
       trans.cmdTrans = WM_DWORD;
       trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	   trans.Data.Dword=SAA7146_SET.debimem;
	   WD_Transfer(hWD,&trans);


//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_COMMAND,Length);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_COMMAND;
	trans.Data.Dword=Length;
	WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_CONFIG,config );
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_CONFIG;
	trans.Data.Dword=config;
	WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,PokeReg);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + MC2;
	trans.Data.Dword=PokeReg;
	WD_Transfer(hWD,&trans);

	if (count > 4) return count;

	SAA7146_Wait_for_Debi_done(hWD);
	
//	result = SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD);
    trans.cmdTrans = RM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	WD_Transfer(hWD,&trans);
    result=trans.Data.Dword;
	
	result &= (0xffffffffUL >> ((4-count)*8));
	return result;
}

DWORD SAA7146_RecordWrite(HANDLE hWD,DWORD Addresse, DWORD count)
{
	DWORD Length=(count << 17) | (Addresse & 0xffff);
    DWORD PokeReg=(2 << 16) | 2;
	WD_TRANSFER trans;

    if ( SAA7146_SET.ComAddress == 0 ) return(0);
	if (SAA7146_SET.recordmem == 0 ) return(0);
	SAA7146_Wait_for_Debi_done(hWD);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_CONFIG,config );
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_CONFIG;
	trans.Data.Dword=DEBISWAB;
	WD_Transfer(hWD,&trans);

	// SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,(unsigned long)debi_mem_dma.Page[0].pPhysicalAddr);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	trans.Data.Dword=SAA7146_SET.recordmem ;
	WD_Transfer(hWD,&trans);

	// SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_COMMAND,Length);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_COMMAND;
	trans.Data.Dword=Length;
	WD_Transfer(hWD,&trans);
    // SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,PokeReg);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + MC2;
	trans.Data.Dword=PokeReg;
	WD_Transfer(hWD,&trans);
	return 0;
}


DWORD SAA7146_CAWrite(HANDLE hWD,DWORD Addresse, DWORD count)
{
	DWORD Length=(count << 17) | (Addresse & 0xffff);
    DWORD PokeReg=(2 << 16) | 2;
	WD_TRANSFER trans;

    if ( SAA7146_SET.ComAddress == 0 ) return(0);
	if (SAA7146_SET.camem == 0 ) return(0);
	SAA7146_Wait_for_Debi_done(hWD);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_CONFIG,config );
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_CONFIG;
	trans.Data.Dword=DEBISWAB;
	WD_Transfer(hWD,&trans);

	// SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,(unsigned long)debi_mem_dma.Page[0].pPhysicalAddr);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	trans.Data.Dword=SAA7146_SET.camem ;
	WD_Transfer(hWD,&trans);

	// SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_COMMAND,Length);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_COMMAND;
	trans.Data.Dword=Length;
	WD_Transfer(hWD,&trans);
    // SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,PokeReg);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + MC2;
	trans.Data.Dword=PokeReg;
	WD_Transfer(hWD,&trans);
	return 0;
}


DWORD SAA7146_BitmapWrite(HANDLE hWD, DWORD Addresse, DWORD Offset, DWORD count)
{
	DWORD Length=(count << 17) | (Addresse & 0xffff);
    DWORD PokeReg=(2 << 16) | 2;
	WD_TRANSFER trans;

    if ( SAA7146_SET.ComAddress == 0 ) return(0);
	if (SAA7146_SET.bitmapmem == 0 ) return(0);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_CONFIG,config );
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_CONFIG;
	trans.Data.Dword=DEBISWAB;
	WD_Transfer(hWD,&trans);

	// SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,(unsigned long)debi_mem_dma.Page[0].pPhysicalAddr);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	trans.Data.Dword=SAA7146_SET.bitmapmem +  Offset;
	WD_Transfer(hWD,&trans);

	// SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_COMMAND,Length);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_COMMAND;
	trans.Data.Dword=Length;
	WD_Transfer(hWD,&trans);
    // SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,PokeReg);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + MC2;
	trans.Data.Dword=PokeReg;
	WD_Transfer(hWD,&trans);
	return 0;
}


DWORD SAA7146_RecordRead(HANDLE hWD,  DWORD Addresse, DWORD count)
{
	DWORD result = 0;
	DWORD Length=((count << 17) | 0x10000 | Addresse & 0xffff);
    DWORD PokeReg=(2 << 16) | 2;
	WD_TRANSFER trans;

	if (count > 0x1000 || count <= 0) count=0x08;
    if ( SAA7146_SET.ComAddress == 0 ) return(0);
	if (SAA7146_SET.recordmem == 0 ) return(0);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,(DWORD)debi_mem_dma.Page[0].pPhysicalAddr);
       trans.cmdTrans = WM_DWORD;
       trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	   trans.Data.Dword=SAA7146_SET.recordmem;
	   WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_COMMAND,Length);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_COMMAND;
	trans.Data.Dword=Length;
	WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_CONFIG,config );
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_CONFIG;
	trans.Data.Dword=DEBISWAB;
	WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,PokeReg);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + MC2;
	trans.Data.Dword=PokeReg;
	WD_Transfer(hWD,&trans);
	return count;
}


DWORD SAA7146_DebugRead(HANDLE hWD, DWORD Addresse, DWORD count)
{
	DWORD result = 0;
	DWORD Length=((count << 17) | 0x10000 | Addresse & 0xffff);
    DWORD PokeReg=(2 << 16) | 2;
	WD_TRANSFER trans;

    SAA7146_Wait_for_Debi_done(hWD);
	if (count > 0x1000 || count <= 0) count=8;
    if ( SAA7146_SET.ComAddress == 0 ) return(0);
	if (SAA7146_SET.debugmem == 0 ) return(0);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,(DWORD)debi_mem_dma.Page[0].pPhysicalAddr);
       trans.cmdTrans = WM_DWORD;
       trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	   trans.Data.Dword=SAA7146_SET.debugmem;
	   WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_COMMAND,Length);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_COMMAND;
	trans.Data.Dword=Length;
	WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_CONFIG,config );
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_CONFIG;
	trans.Data.Dword=DEBISWAB;
	WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,PokeReg);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + MC2;
	trans.Data.Dword=PokeReg;
	WD_Transfer(hWD,&trans);
	return count;
}


DWORD SAA7146_IrRead(HANDLE hWD,  DWORD Addresse, DWORD count)
{
	DWORD result = 0;
	DWORD Length=((count << 17) | 0x10000 | Addresse & 0xffff);
    DWORD PokeReg=(2 << 16) | 2;
	WD_TRANSFER trans;

	if (count > 0x1000 || count <= 0) count=4;
    if ( SAA7146_SET.ComAddress == 0 ) return(0);
	if (SAA7146_SET.debimem == 0 ) return(0);
	SAA7146_Wait_for_Debi_done(hWD);

	//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,(DWORD)debi_mem_dma.Page[0].pPhysicalAddr);
       trans.cmdTrans = WM_DWORD;
       trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	   trans.Data.Dword=SAA7146_SET.irmem;
	   WD_Transfer(hWD,&trans);


//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_COMMAND,Length);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_COMMAND;
	trans.Data.Dword=Length;
	WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_CONFIG,config );
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_CONFIG;
	trans.Data.Dword=DEBINOSWAB;
	WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,PokeReg);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + MC2;
	trans.Data.Dword=PokeReg;
	WD_Transfer(hWD,&trans);
	SAA7146_Wait_for_Debi_done(hWD);
	
//	result = SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD);
    trans.cmdTrans = RM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	WD_Transfer(hWD,&trans);
    result=trans.Data.Dword;
	
	result &= (0xffffffffUL >> ((4-count)*8));
	return result;

}


DWORD SAA7146_CARead(HANDLE hWD,DWORD Addresse,  DWORD count)
{
	DWORD result = 0;
	DWORD Length=(count << 17) | 0x10000 | (Addresse & 0xffff);
    DWORD PokeReg=(2 << 16) | 2;
	WD_TRANSFER trans;

	if (count > 0x1000 || count <= 0) count=8;
    if ( SAA7146_SET.ComAddress == 0 ) return(0);
	if (SAA7146_SET.camem == 0 ) return(0);

   	SAA7146_Wait_for_Debi_done(hWD);
//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,(DWORD)debi_mem_dma.Page[0].pPhysicalAddr);
       trans.cmdTrans = WM_DWORD;
       trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	   trans.Data.Dword=SAA7146_SET.camem;
	   WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_COMMAND,Length);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_COMMAND;
	trans.Data.Dword=Length;
	WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_CONFIG,config );
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_CONFIG;
	trans.Data.Dword=DEBISWAB;
	WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,PokeReg);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + MC2;
	trans.Data.Dword=PokeReg;
	WD_Transfer(hWD,&trans);
	return count;
}

DWORD SAA7146_CIRead(HANDLE hWD, DWORD Addresse, DWORD count)
{
	DWORD result = 0;
	DWORD Length=(count << 17) | 0x10000 | (Addresse  & 0xffff);
    DWORD PokeReg=(2 << 16) | 2;
	WD_TRANSFER trans;

	if (count > 0x1000 || count <= 0) count=8;
    if ( SAA7146_SET.ComAddress == 0 ) return(0);
	if (SAA7146_SET.cimem == 0 ) return(0);
	SAA7146_Wait_for_Debi_done(hWD);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,(DWORD)debi_mem_dma.Page[0].pPhysicalAddr);
       trans.cmdTrans = WM_DWORD;
       trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	   trans.Data.Dword=SAA7146_SET.cimem;
	   WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_COMMAND,Length);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_COMMAND;
	trans.Data.Dword=Length;
	WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_CONFIG,config );
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_CONFIG;
	trans.Data.Dword=DEBISWAB;
	WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,PokeReg);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + MC2;
	trans.Data.Dword=PokeReg;
	WD_Transfer(hWD,&trans);
	return count;
}

DWORD SAA7146_IPMPERead(HANDLE hWD, DWORD Addresse, DWORD count)
{
	DWORD result = 0;
	DWORD Length=(count << 17) | 0x10000 | (Addresse & 0xffff);
    DWORD PokeReg=(2 << 16) | 2;
	WD_TRANSFER trans;
	if (count > 0x1000 || count <= 0) count=8;
    if ( SAA7146_SET.ComAddress == 0 ) return(0);
	if (SAA7146_SET.ipmpemem == 0 ) return(0);
	
	
	SAA7146_Wait_for_Debi_done(hWD);
//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_AD,(DWORD)debi_mem_dma.Page[0].pPhysicalAddr);
       trans.cmdTrans = WM_DWORD;
       trans.dwPort   = SAA7146_SET.ComAddress + DEBI_AD;
	   trans.Data.Dword=SAA7146_SET.ipmpemem;
	   WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_COMMAND,Length);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_COMMAND;
	trans.Data.Dword=Length;
	WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, DEBI_CONFIG,config );
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + DEBI_CONFIG;
	trans.Data.Dword=DEBISWAB;
	WD_Transfer(hWD,&trans);

//	SAA7146_WriteDword(hSAA7146, SAA7146_AD_BAR0, MC2,PokeReg);
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + MC2;
	trans.Data.Dword=PokeReg;
	WD_Transfer(hWD,&trans);
	return count;
}

int SAA7146_Wait_for_Debi_done(HANDLE hWD)
{
	DWORD i;
	WD_TRANSFER trans;
    DWORD result;

    if ( SAA7146_SET.ComAddress == 0 ) return(0);
	/* wait for registers to be programmed */
	for (i = 0; i < SAA7146_Timing.Debi_done_MC2; i++) {
     // if ( SAA7146_ReadDword(hSAA7146, SAA7146_AD_BAR0, MC2) & 2) break;
	 trans.cmdTrans = RM_DWORD;
     trans.dwPort   = SAA7146_SET.ComAddress + MC2;
	 WD_Transfer(hWD,&trans);
     result=trans.Data.Dword;
     if ((result&2) != 0 ) break;
	};
	if ( i >= SAA7146_Timing.Debi_done_MC2 ) SAA7146_ISR.Debi_Done_MC2=(DWORD)0x01;
	/* wait for transfer to complete */
	for (i = 0; i < SAA7146_Timing.Debi_done_PSR; i++ ) {
	 trans.cmdTrans = RM_DWORD;
     trans.dwPort   = SAA7146_SET.ComAddress + PSR;
	 WD_Transfer(hWD,&trans);
     result=trans.Data.Dword;
	 if ((result&SPCI_DEBI_S) == 0 )  break;
	 trans.cmdTrans = RM_DWORD;
     trans.dwPort   = SAA7146_SET.ComAddress + MC2;
	 WD_Transfer(hWD,&trans);
    };
	if ( i >= SAA7146_Timing.Debi_done_PSR ) SAA7146_ISR.Debi_Done_PSR=0x01;
	return 0;
}


int SAA7146_OutCommand(HANDLE hWD, DWORD *buf, WORD length)
{
        DWORD i,j;
		DWORD full,last;
		DWORD FirstLen=4;
        DWORD Ret;

		if ( LockStateExtern == 0x00000000) {
			LockStateExtern=1;
			return(0xf0);
        };
		if ( Debi_On_Set == 1 ) return(0xf1);
		if ( SAA7146_DebiRead(hWD,DEBINOSWAB, COMMAND, 2 ) != 0 ) return(0xf2);
        if ( SAA7146_DebiRead(hWD, DEBINOSWAB, MSGSTATE, 2) & OSDQFull ) return(0xf3);
        if (  SAA7146_DebiRead(hWD,  DEBINOSWAB, HANDSHAKE_REG,  2 ) ) return(0xf4);

        j=length*2;  // Bytes
		full=j/4;    // DWORD
		last=j-(full*4);
		if ( full == 0 ) {
			FirstLen=last;
            last=0;
		};

        i=1;
        for (i=1; i<full; i++) SAA7146_DebiCommandWrite(hWD, DEBINOSWAB, COMMAND + 4*i, *(buf+i), 4);
        if ( last != 0 ) SAA7146_DebiCommandWrite(hWD, DEBINOSWAB, COMMAND + 4*i, *(buf+i), 2); 
		SAA7146_DebiCommandWrite(hWD, DEBINOSWAB, COMMAND , *buf, FirstLen); 
/*
        for (i=2; i<length; i++) SAA7146_DebiWrite(hWD, DEBINOSWAB, COMMAND + 2*i, *(buf+i), 2);
        if (length) SAA7146_DebiWrite( hWD, DEBINOSWAB, COMMAND + 2, *(buf+1), 2);
        else SAA7146_DebiWrite( hWD, DEBINOSWAB, COMMAND + 2, 0, 2);
        SAA7146_DebiWrite(hWD, DEBINOSWAB, COMMAND    , *buf, 2);
*/
		i=0;
        Ret=0;
        while (( SAA7146_DebiRead( hWD, DEBINOSWAB, COMMAND, 2) != 0 ) && ( i < SAA7146_Timing.Cmd_Time_Out)) i++;
		if ( i >= SAA7146_Timing.Cmd_Time_Out ) Ret=0x01;
        SAA7146_DebiWrite(hWD, DEBINOSWAB, COMMAND    , 0, 2);
		Release_Spin_Lock(hWD);
		return(Ret);
};



DWORD SAA7146_WriteText(HANDLE hWD, WORD CommandLength, DWORD *CommandBuffer, WORD TextLength,DWORD *TextBuffer)
{
        DWORD i;
        DWORD Ret;

		if ( LockStateExtern == 0x00000000) {
			LockStateExtern=1;
			return(0xf0);
        };
		if ( Debi_On_Set == 1 ) return(0xf1);

        if ( SAA7146_DebiRead(hWD,DEBINOSWAB, BUFF1_BASE,  2 ) != 0 )  return(0xf6);
		if ( SAA7146_DebiRead(hWD,DEBINOSWAB, COMMAND, 2 ) != 0 ) return(0xf2);
        if ( SAA7146_DebiRead(hWD, DEBINOSWAB, MSGSTATE, 2) & OSDQFull ) return(0xf3);
        if (  SAA7146_DebiRead(hWD,  DEBINOSWAB, HANDSHAKE_REG,  2 ) ) return(0xf4);

        for (i=0; i<TextLength; i++) SAA7146_DebiCommandWrite( hWD,DEBINOSWAB, BUFF1_BASE + ( i*4 ),*(TextBuffer+i), 4);

        SAA7146_DebiCommandWrite(hWD, DEBINOSWAB, COMMAND + 4, *(CommandBuffer+1), 4);
        SAA7146_DebiCommandWrite(hWD, DEBINOSWAB, COMMAND + 8, *(CommandBuffer+2), 2);
        SAA7146_DebiCommandWrite(hWD, DEBINOSWAB, COMMAND    , *CommandBuffer, 4);
        
		Ret=0;
		i=0;
        while (( SAA7146_DebiRead( hWD, DEBINOSWAB, COMMAND, 2) != 0 ) && ( i < SAA7146_Timing.Cmd_Time_Out )) i++;
		if ( i >= SAA7146_Timing.Cmd_Time_Out ) Ret=0x01;
		i=0;
        while (( SAA7146_DebiRead(hWD, DEBINOSWAB, BUFF1_BASE, 2 ) != 0  ) && ( i < SAA7146_Timing.OSD_Text_Time_Out )) i++;
		if ( i >= SAA7146_Timing.OSD_Text_Time_Out ) Ret=Ret+0x02;

		SAA7146_DebiCommandWrite(hWD, DEBINOSWAB, BUFF1_BASE    , 0, 2);
        SAA7146_DebiCommandWrite(hWD, DEBINOSWAB, COMMAND    , 0, 2);
		Release_Spin_Lock(hWD);
        
        return(Ret);
}



int SAA7146_CommandRequest(HANDLE hWD, DWORD *Buff, DWORD length, WORD *buf, DWORD n)
{
        DWORD i,j;
		DWORD full,last;
		DWORD FirstLen=4;
        DWORD Ret;

		if ( LockStateExtern == 0x00000000) {
			LockStateExtern=1;
			return(0xf0);
        };
		if ( Debi_On_Set == 1 ) return(0xf1);
		if ( SAA7146_DebiRead(hWD,DEBINOSWAB, COMMAND, 2 ) != 0 ) return(0xf2);
        if ( SAA7146_DebiRead(hWD, DEBINOSWAB, MSGSTATE, 2) & OSDQFull ) return(0xf3);
        if (  SAA7146_DebiRead(hWD,  DEBINOSWAB, HANDSHAKE_REG,  2 ) ) return(0xf4);

        j=length*2;  // Bytes
		full=j/4;    // DWORD
		last=j-(full*4);
		if ( full == 0 ) {
			FirstLen=last;
            last=0;
		};

        i=1;
        for (i=1; i<full; i++) SAA7146_DebiCommandWrite(hWD, DEBINOSWAB, COMMAND + 4*i, *(Buff+i), 4);
        if ( last != 0 ) SAA7146_DebiCommandWrite(hWD, DEBINOSWAB, COMMAND + 4*i, *(Buff+i), 2); 
		SAA7146_DebiCommandWrite(hWD, DEBINOSWAB, COMMAND , *Buff, FirstLen); 

/*
		for (i=2; i<length; i++) SAA7146_DebiWrite(hWD, DEBINOSWAB, COMMAND + 2*i, *(Buff+i), 2);
        if (length) SAA7146_DebiWrite( hWD, DEBINOSWAB, COMMAND + 2, *(Buff+1), 2);
        else SAA7146_DebiWrite( hWD, DEBINOSWAB, COMMAND + 2, 0, 2);
        SAA7146_DebiWrite(hWD, DEBINOSWAB, COMMAND    , *Buff, 2);
*/
        Ret=0;
        i=0;
		while (( SAA7146_DebiRead( hWD, DEBINOSWAB, COMMAND, 2) != 0 ) && ( i < SAA7146_Timing.Cmd_Time_Out)) i++;
		if ( i >= SAA7146_Timing.Cmd_Time_Out ) Ret=0x01;

        for (i=0; i<n; i++)
                buf[i] = (WORD)SAA7146_DebiRead( hWD, DEBINOSWAB, COM_BUFF + 2*i, 2);

        SAA7146_DebiWrite(hWD, DEBINOSWAB, COMMAND , 0, 2);

		Release_Spin_Lock(hWD);

        return(Ret);
}


void Debi_On(HANDLE hWD)
{
	DWORD result;
	WD_TRANSFER trans;

    if ( SAA7146_SET.ComAddress == 0 ) return;
	 trans.cmdTrans = RM_DWORD;
     trans.dwPort   = SAA7146_SET.ComAddress + IER;
	 WD_Transfer(hWD,&trans);
     result=trans.Data.Dword;
   	 trans.cmdTrans = WM_DWORD;
     trans.dwPort   = SAA7146_SET.ComAddress + IER;
     trans.Data.Dword=(result|MASK_19);
	 WD_Transfer(hWD,&trans);
};

void Debi_Off(HANDLE hWD)
{
	DWORD result;
	WD_TRANSFER trans;

    if ( SAA7146_SET.ComAddress == 0 ) return;
	 trans.cmdTrans = RM_DWORD;
     trans.dwPort   = SAA7146_SET.ComAddress + IER;
	 WD_Transfer(hWD,&trans);
     result=trans.Data.Dword;
   	 trans.cmdTrans = WM_DWORD;
     trans.dwPort   = SAA7146_SET.ComAddress + IER;
     trans.Data.Dword=(result&~MASK_19);
	 WD_Transfer(hWD,&trans);
};


void SAA7146_SetGPIO( HANDLE hWD, DWORD port, DWORD data)
{
    DWORD val;
	WD_TRANSFER trans;

	trans.cmdTrans = RM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + GPIO_CTRL;
	WD_Transfer(hWD,&trans);
    val=trans.Data.Dword;
    val&=~(0xff << (8*(port)));
    val|=(data)<<(8*(port));
 	trans.cmdTrans = WM_DWORD;
    trans.dwPort   = SAA7146_SET.ComAddress + GPIO_CTRL;
    trans.Data.Dword=val;
	WD_Transfer(hWD,&trans);
};

void Set_Spin_Lock( void )
{
       LockStateExtern=0x00000001;
};


void Release_Spin_Lock( HANDLE hWD )
{
WD_TRANSFER trans;

		LockStateExtern=0x00000000;
			if ( Hold_GPIO_Irq == 1 ) {
              trans.cmdTrans = WM_DWORD;
              trans.dwPort   = SAA7146_SET.ComAddress + PSR;
              trans.Data.Dword=0x00000008;
	          WD_Transfer(hWD,&trans);
			}
};
