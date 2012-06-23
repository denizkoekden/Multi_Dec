#include "..\drvlib\kpstdlib.h"
#include "..\drvlib\wd_kp.h"
#include "BTIOcom.h"


BOOL __cdecl KP_Open(KP_OPEN_CALL *kpOpenCall, HANDLE hWD, PVOID pOpenData, PVOID *ppDrvContext);
void __cdecl KP_Close(PVOID pDrvContext);
void __cdecl KP_Call(PVOID pDrvContext, WD_KERNEL_PLUGIN_CALL *kpCall, BOOL fIsKernelMode);
BOOL __cdecl KP_IntEnable(PVOID pDrvContext, WD_KERNEL_PLUGIN_CALL *kpCall, PVOID *ppIntContext);
void __cdecl KP_IntDisable(PVOID pIntContext);
BOOL __cdecl KP_IntAtIrql(PVOID pIntContext, BOOL *fIsMyInterrupt);
DWORD __cdecl KP_IntAtDpc(PVOID pIntContext, DWORD dwCount);

BOOL __cdecl KP_Init(KP_INIT *kpInit)
{
    // check if the version of WD_KP.LIB is the same version as WINDRVR.H and WD_KP.H
    kpInit->funcOpen = KP_Open;
    strcpy (kpInit->cDriverName, "MDBT"); // until 8 chars

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
    
    // you can allocate here memory

    return TRUE;
}

// called when WD_KernelPlugInClose() is called
void __cdecl KP_Close(PVOID pDrvContext)
{
    // you can free here the memory allocated pDrvContext
}

// called when WD_KernelPlugInCall() is called
void __cdecl KP_Call(PVOID pDrvContext, WD_KERNEL_PLUGIN_CALL *kpCall, BOOL fIsKernelMode)
{
    kpCall->dwResult = BT8X8_OK;

    switch ( kpCall->dwMessage )
    {
    case BT8X8_IOCTL_VERSION: // GetVersion message
        { 
            BT8X8_VERSION *ver = (BT8X8_VERSION *) kpCall->pData;
            COPY_TO_USER_OR_KERNEL(ver->cVer, "BT8x8-Kernel-Driver V1.40", sizeof("BT8x8-Kernel-Driver V1.40")+1, fIsKernelMode);
            kpCall->dwResult = BT8X8_OK;
        }
        break ;

    case BT8X8_IOCTL_SET_INIT: // Set message
        { 
            BT8X8_ISR *adr = (BT8X8_ISR *) kpCall->pData;
            COPY_TO_USER_OR_KERNEL(&BT8X8_Last_ISR.ComAddress,&adr->ComAddress, sizeof(BT8X8_ISR), fIsKernelMode);
            kpCall->dwResult = BT8X8_OK;
        }
        break ;

    case BT8X8_IOCTL_GET_INIT: // Get message
        { 
            BT8X8_ISR *adr = (BT8X8_ISR *) kpCall->pData;
            COPY_TO_USER_OR_KERNEL(&adr->ComAddress,&BT8X8_Last_ISR.ComAddress, sizeof(BT8X8_ISR), fIsKernelMode);
            kpCall->dwResult = BT8X8_OK;
        }
        break ;

    case BT8X8_IOCTL_SET_VCIRQCOUNT: // Get message
        { 
            BT8X8_Last_ISR.VCIrqCount= kpCall->pData;
            kpCall->dwResult = BT8X8_OK;
        }
        break ;
    case BT8X8_IOCTL_SET_CURRENTIRQ: // Get message
        { 
            BT8X8_Last_ISR.CurrentIrq = kpCall->pData;
            kpCall->dwResult = BT8X8_OK;
        }
        break ;


        // you can implement here other messages
    default:
        kpCall->dwResult = BT8X8_WRONG_MESSAGE;
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
	DWORD isr_reset_pci;
    BOOL ret=FALSE;
	WD_TRANSFER trans;

	HANDLE hWD=(HANDLE)pIntContext;

    
   *pfIsMyInterrupt = FALSE;

    if ( BT8X8_Last_ISR.ComAddress == 0 ) return(FALSE);

    trans.cmdTrans = RM_DWORD;
    trans.dwPort   = BT8X8_Last_ISR.ComAddress + BT848_INT_STAT;
	WD_Transfer(hWD,&trans);

    isr_reset_pci=trans.Data.Dword;

	isr_status=(trans.Data.Dword>>28)&0x07;

	if (( isr_status > 0 ) && ( isr_status < 6 )) {
           *pfIsMyInterrupt = TRUE;
		   BT8X8_Last_ISR.PictureNummer=isr_status;
		   if ( BT8X8_Last_ISR.PictureNummer == 5 ) BT8X8_Last_ISR.PictureNummer=0;
		   BT8X8_Last_ISR.Running_Irq++;
		   BT8X8_Last_ISR.CurrentIrq++;
		   if ( BT8X8_Last_ISR.VCIrqCount < 512 ) BT8X8_Last_ISR.VCIrqCount++;
		   if ( BT8X8_Last_ISR.CurrentIrq > 255 ) BT8X8_Last_ISR.CurrentIrq=0;
           ret=TRUE;
	// Resetting PCI-Bus
    trans.cmdTrans = WM_DWORD;
    trans.dwPort   = BT8X8_Last_ISR.ComAddress + BT848_INT_STAT;
	trans.Data.Dword=isr_reset_pci;
	WD_Transfer(hWD,&trans);
    }; 
     return(ret);
}

// returns the number of times to notify user-mode (i.e. return from WD_IntWait)
DWORD __cdecl KP_IntAtDpc(PVOID pIntContext, DWORD dwCount)
{
		return dwCount; // return WD_IntWait as many times as KP_IntAtIrql scheduled KP_IntAtDpc()
}


