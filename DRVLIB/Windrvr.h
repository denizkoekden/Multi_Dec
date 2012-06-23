/* 
 * W i n D r i v e r    v 5 . 0 3
 * ==============================
 *
 * Header file for Windows 95/98/ME/NT/NTE/NT2000/CE/Linux/Solaris/VxWorks.
 * FOR DETAILS ON THE WinDriver FUNCTIONS, PLEASE SEE THE WinDriver MANUAL
 * OR INCLUDED HELP FILES.
 *
 * This file may not be distributed -- it may only be used for development
 * or evaluation purposes. (see \WinDriver\docs\license.txt for details).
 *
 * Web site: http://www.jungo.com
 * Email:    support@jungo.com
 *
 * (C) Jungo 2001
 */
#ifndef _WINDRVR_H_
#define _WINDRVR_H_

#if defined(__cplusplus)
	extern "C" {
#endif

// these lines break standard VxWorks builds. remove the -asni flag from the
// compiler flags (from the makefile or the Tornado IDE) to compile WinDriver
// related source code.
#define WD_VER      503


#if defined(VXWORKS)
	#define WD_PROD_NAME "DriverBuilder"
#else
	#define WD_PROD_NAME "WinDriver"
#endif

#if defined(VXWORKS)
	#if defined(MBX860)     
		#define WD_CPU_SPEC " PPC860"
	#elif defined(MCP750)
		#define WD_CPU_SPEC " PPC750"
	#elif defined(x86)
		#define WD_CPU_SPEC " X86"
	#else	  
		#define WD_CPU_SPEC ""
	#endif
#else
	#if defined(SPARC)     
		#define WD_CPU_SPEC " Sparc"
	#elif defined(WINCE)
		#define WD_CPU_SPEC ""
	#else  
		#define WD_CPU_SPEC " X86"
	#endif
#endif

#if defined(WINNT)
	#define WD_FILE_FORMAT " SYS"
#elif defined(WIN95)
	#define WD_FILE_FORMAT " VXD"
#else
	#define WD_FILE_FORMAT ""
#endif

#define WD_VER_STR  WD_PROD_NAME  " V5.03 Jungo (c)2001 Build Date: " __DATE__ WD_CPU_SPEC WD_FILE_FORMAT

#if !defined(UNIX) && (defined(LINUX) || defined(SOLARIS) || defined(VXWORKS))
    #define UNIX
#endif

#if !defined(SPARC) && (defined(__sparc__) || defined (__sparc) || \
		defined(sparc))
	#define SPARC
#endif

#if !defined(WIN32) && (defined(WINCE) || defined(WIN95) || defined(WINNT)) && !defined(OS2)
	#define WIN32
#endif

#if defined(_WIN32_WCE) && !defined(WINCE)
	#define WINCE
#endif

#if !defined(x86) && ( defined(LINUX) || (defined(WIN32) && !defined(WINCE) && !defined(_ALPHA_) ))
	#define x86
#endif

#if defined(_KERNEL) && !defined(__KERNEL__)
	#define __KERNEL__
#endif

#if defined( __KERNEL__) && !defined(_KERNEL)
	#define _KERNEL
#endif
	
#if !defined(WIN32) && !defined(WINCE) && !defined(UNIX) && !defined(OS2)
	#define WIN32
#endif

#if  !defined(POWERPC) && defined(VXWORKS) && !defined(x86)
	#define POWERPC
#endif
#if !defined(_BIGENDIAN) && (defined (SPARC) || defined(POWERPC))
    #define _BIGENDIAN
#endif

#if defined(UNIX)
	#if !defined(__P_TYPES__)
		#if !defined(VXWORKS)
			typedef void VOID;
			typedef unsigned char UCHAR;
			typedef unsigned short USHORT;
			typedef unsigned int UINT;
			typedef unsigned long ULONG;
			typedef ULONG BOOL;
		#endif
	    typedef void *PVOID;
	    typedef unsigned char *PBYTE;
	    typedef char CHAR;
	    typedef char *PCHAR;
	    typedef unsigned short *PWORD;
		typedef unsigned long DWORD, *PDWORD;
		typedef PVOID HANDLE;
	#endif
	
	#if !defined(__KERNEL__)
		#include <string.h>
		#include <ctype.h>
		#include <stdlib.h>
	#endif
    #define TRUE 1
    #define FALSE 0
    #define __cdecl
    #define WINAPI
	
    #if defined(__KERNEL__)
		#if defined(LINUX)
    		#include <linux/types.h>
			#include <linux/string.h>
		#endif
	#else
		#include <unistd.h> 
		#if defined(LINUX)
    			#include <sys/ioctl.h> /* for BSD ioctl() */
			#include <unistd.h>
		#else
			#include <unistd.h> /* for SVR4 ioctl()*/
		#endif
		#if defined(VXWORKS)
			#include <vxWorks.h>
			#undef SPARC /* defined in vxworks.h */
			#include <string.h>
			#include <memLib.h>
			#include <stdlib.h>
			#include <taskLib.h>
			#include <ioLib.h>
			#include <iosLib.h>
			#include <taskLib.h>
			#include <semLib.h>
			#include <timers.h>
		#endif
		#include <sys/types.h>
		#include <sys/stat.h>
		#include <fcntl.h>	
	#endif
#elif defined(OS2)
	#define INCL_DOSSEMAPHORES
    #define INCL_WINLOAD
    #define INCL_DOSPROCESS
    #define INCL_DOSDEVICES
	#define __cdecl
    #define WINAPI
   	#include <os2.h>
	#include <string.h>
	#include <ctype.h>	
	#if !defined(__P_TYPES__)
			#define __P_TYPES__
		typedef unsigned short NTSTATUS;    
		typedef unsigned short WORD;
		typedef unsigned short *PWORD;
		typedef unsigned long DWORD, *PDWORD;
		typedef PVOID HANDLE;
	#endif	
	#define TRUE 1
    #define FALSE 0
#elif defined(WINCE)
    #include <windows.h>
    #include <winioctl.h>
	typedef char CHAR;
#elif defined(WIN32)
    #if defined(__KERNEL__)
	    int sprintf(char *buffer, const char *format, ...);
    #else
        #include <windows.h>
        #include <winioctl.h>
	#endif
	#if defined(__KERNEL__) && defined(WIN32)
	    int sprintf(char *buffer, const char *format, ...);
	#endif
	#if defined(__KERNEL__) && defined(WIN95) && !defined(__KERPLUG__)
		#include "kdstdlib.h"
	#endif
#endif

static CHAR *WD_VER_MODULE = WD_VER_STR;
#ifndef OS2
	typedef unsigned char BYTE;
	typedef unsigned short int WORD;
#endif

typedef enum 
{                   
    CMD_NONE = 0,       // No command
    CMD_END = 1,        // End command

    RP_BYTE = 10,       // Read port byte
    RP_WORD = 11,       // Read port word  
    RP_DWORD = 12,      // Read port dword
    WP_BYTE = 13,       // Write port byte
    WP_WORD = 14,       // Write port word 
    WP_DWORD = 15,      // Write port dword 

    RP_SBYTE = 20,      // Read port string byte
    RP_SWORD = 21,      // Read port string word  
    RP_SDWORD = 22,     // Read port string dword
    WP_SBYTE = 23,      // Write port string byte
    WP_SWORD = 24,      // Write port string word 
    WP_SDWORD = 25,     // Write port string dword 

    RM_BYTE = 30,       // Read memory byte
    RM_WORD = 31,       // Read memory word  
    RM_DWORD = 32,      // Read memory dword
    WM_BYTE = 33,       // Write memory byte
    WM_WORD = 34,       // Write memory word 
    WM_DWORD = 35,      // Write memory dword 

    RM_SBYTE = 40,      // Read memory string byte
    RM_SWORD = 41,      // Read memory string word  
    RM_SDWORD = 42,     // Read memory string dword
    WM_SBYTE = 43,      // Write memory string byte
    WM_SWORD = 44,      // Write memory string word 
    WM_SDWORD = 45      // Write memory string dword 
} WD_TRANSFER_CMD;                                         

enum { WD_DMA_PAGES = 256 };

enum { DMA_KERNEL_BUFFER_ALLOC = 1 }; // the system allocates a contiguous buffer
                                 // the user doesnt need to supply linear_address
enum { DMA_KBUF_BELOW_16M = 2 }; // if DMA_KERNEL_BUFFER_ALLOC if used,
                                 // this will make sure it is under 16M
enum { DMA_LARGE_BUFFER   = 4 }; // if DMA_LARGE_BUFFER if used,
                                 // the maximum number of pages are dwPages, and not
                                 // WD_DMA_PAGES. if you lock a user buffer (not a kernel
                                 // allocated buffer) that is larger than 1MB, then use this
                                 // option, and allocate memory for pages.

typedef struct
{
    PVOID pPhysicalAddr;    // physical address of page
    DWORD dwBytes;          // size of page
} WD_DMA_PAGE, WD_DMA_PAGE_V30;

typedef struct 
{
    DWORD hDma;             // handle of dma buffer
    PVOID pUserAddr;        // beginning of buffer
    DWORD dwBytes;          // size of buffer
    DWORD dwOptions;        // allocation options:
                            // DMA_KERNEL_BUFFER_ALLOC, DMA_KBUF_BELOW_16M, DMA_LARGE_BUFFER
    DWORD dwPages;          // number of pages in buffer
    WD_DMA_PAGE Page[WD_DMA_PAGES];
} WD_DMA, WD_DMA_V30;

typedef struct 
{
    DWORD cmdTrans;  // Transfer command WD_TRANSFER_CMD
    DWORD dwPort;    // io port for transfer or user memory address

    // parameters used for string transfers:
    DWORD dwBytes;   // for string transfer
    DWORD fAutoinc;  // transfer from one port/address 
                     // or use incremental range of addresses
    DWORD dwOptions; // must be 0
    union
    {
        BYTE Byte;   // use for byte transfer
        WORD Word;   // use for word transfer
        DWORD Dword; // use for dword transfer
        PVOID pBuffer; // use for string transfer
    } Data;       
} WD_TRANSFER, WD_TRANSFER_V30;


enum { INTERRUPT_LEVEL_SENSITIVE = 1 };
enum { INTERRUPT_CMD_COPY = 2 };
enum { INTERRUPT_CE_INT_ID = 4 };

typedef struct
{
    DWORD hKernelPlugIn;
    DWORD dwMessage;
    PVOID pData;
    DWORD dwResult;
} WD_KERNEL_PLUGIN_CALL, WD_KERNEL_PLUGIN_CALL_V40;

typedef struct
{                                                        
    DWORD hInterrupt;    // handle of interrupt
    DWORD dwOptions;     // interrupt options: INTERRUPT_CMD_COPY
    
    WD_TRANSFER *Cmd;    // commands to do on interrupt
    DWORD dwCmds;        // number of commands

    // for WD_IntEnable()
    WD_KERNEL_PLUGIN_CALL kpCall; // kernel plugin call
    DWORD fEnableOk;     // did WD_IntEnable() succeed

    // For WD_IntWait() and WD_IntCount()
    DWORD dwCounter;     // number of interrupts received
    DWORD dwLost;        // number of interrupts not yet dealt with
    DWORD fStopped;      // was interrupt disabled during wait
} WD_INTERRUPT, WD_INTERRUPT_V40;

typedef struct
{                                                        
    DWORD dwVer;  
    CHAR cVer[100];
} WD_VERSION, WD_VERSION_V30;

enum 
{
	LICENSE_DEMO = 0x1,
	LICENSE_WD = 0x4, 
    LICENSE_IO = 0x8,   
	LICENSE_MEM = 0x10, 
	LICENSE_INT = 0x20,
    LICENSE_PCI	= 0x40,  
	LICENSE_DMA	= 0x80, 
	LICENSE_NT = 0x100,
    LICENSE_95 = 0x200, 
	LICENSE_ISAPNP = 0x400, 
	LICENSE_PCMCIA = 0x800, 
    LICENSE_PCI_DUMP = 0x1000,
    LICENSE_MSG_GEN	= 0x2000, 
	LICENSE_MSG_EDU = 0x4000, 
	LICENSE_MSG_INT	= 0x8000,
    LICENSE_KER_PLUG = 0x10000,
    LICENSE_LINUX = 0x20000,    
	LICENSE_CE = 0x80000,
	LICENSE_VXWORKS = 0x10000000,
    LICENSE_THIS_PC	= 0x100000, 
	LICENSE_WIZARD = 0x200000,
    LICENSE_KD = 0x400000,  
	LICENSE_SOLARIS	= 0x800000,
    LICENSE_CPU0 = 0x40000,     
	LICENSE_CPU1 = 0x1000000,
    LICENSE_CPU2 = 0x2000000,   
	LICENSE_CPU3 = 0x4000000,
    LICENSE_USB	= 0x8000000,    
    LICENSE_OS2	= 0x20000000,
};

enum
{
	LICENSE2_CPCI = 0x1,
	LICENSE2_REMOTE = 0x2,
};

enum 
{
    LICENSE_OS_WITH_WIZARD = LICENSE_95 | LICENSE_NT,
    LICENSE_OS_WITHOUT_WIZARD = LICENSE_LINUX | LICENSE_CE | LICENSE_VXWORKS | LICENSE_SOLARIS | LICENSE_OS2,
};

enum 
{
	LICENSE_CPU_ALL = LICENSE_CPU3 | LICENSE_CPU2 | LICENSE_CPU1 | 
		LICENSE_CPU0,
	LICENSE_X86 = LICENSE_CPU0,
	LICENSE_ALPHA = LICENSE_CPU1,
	LICENSE_SPARC = LICENSE_CPU1 | LICENSE_CPU0,
	LICENSE_PPC = LICENSE_CPU2,
};

typedef struct
{
    CHAR cLicense[100]; // buffer with license string to put
                        // if empty string then get current license setting 
                        // into dwLicense
    DWORD dwLicense;    // returns license settings: LICENSE_DEMO, LICENSE_WD etc...
    DWORD dwLicense2;    // if put license was unsuccessful (i.e. invalid license)
                        // then dwLicense will return 0.
} WD_LICENSE, WD_LICENSE_V44;

typedef struct 
{
    DWORD dwBusType;        // Bus Type: ISA, EISA, PCI, PCMCIA
    DWORD dwBusNum;         // Bus number
    DWORD dwSlotFunc;       // Slot number on Bus
} WD_BUS, WD_BUS_V30;

enum 
{ 
	WD_BUS_USB = -2,
	WD_BUS_ISA = 1, 
	WD_BUS_EISA = 2, 
	WD_BUS_PCI = 5, 
	WD_BUS_PCMCIA = 8,
};

typedef enum 
{ 
	ITEM_NONE=0, 
	ITEM_INTERRUPT=1, 
	ITEM_MEMORY=2, 
	ITEM_IO=3, 
	ITEM_BUS=5,
} ITEM_TYPE;

typedef struct 
{
    DWORD item; // ITEM_TYPE
    DWORD fNotSharable;
    union
    {
        struct 
		{ // ITEM_MEMORY
            DWORD dwPhysicalAddr;     // physical address on card
            DWORD dwBytes;            // address range
            DWORD dwTransAddr;        // returns the address to pass on to transfer commands
            DWORD dwUserDirectAddr;   // returns the address for direct user read/write
            DWORD dwCpuPhysicalAddr;  // returns the CPU physical address of card
        } Mem;
        struct
		{ // ITEM_IO
            DWORD dwAddr;         // beginning of io address
            DWORD dwBytes;        // io range
        } IO;
        struct
		{ // ITEM_INTERRUPT
            DWORD dwInterrupt;	// number of interrupt to install 
            DWORD dwOptions;    // interrupt options: INTERRUPT_LEVEL_SENSITIVE
            DWORD hInterrupt;   // returns the handle of the interrupt installed
        } Int;
        WD_BUS Bus; // ITEM_BUS
        struct
		{
            DWORD dw1;
            DWORD dw2;
            DWORD dw3;
            DWORD dw4;
            DWORD dw5;
        } Val;
    } I;
} WD_ITEMS, WD_ITEMS_V30;

enum { WD_CARD_ITEMS = 20 };

typedef struct 
{
    DWORD dwItems;
    WD_ITEMS Item[WD_CARD_ITEMS];
} WD_CARD, WD_CARD_V30;

typedef struct
{
    WD_CARD Card;           // card to register
    DWORD fCheckLockOnly;   // only check if card is lockable, return hCard=1 if OK
    DWORD hCard;            // handle of card
    DWORD dwOptions;        // should be zero
    CHAR cName[32];         // name of card
    CHAR cDescription[100]; // description
} WD_CARD_REGISTER, WD_CARD_REGISTER_V40;

enum { WD_PCI_CARDS = 30 };

typedef struct 
{
    DWORD dwBus;
    DWORD dwSlot;
    DWORD dwFunction;
} WD_PCI_SLOT;

typedef struct {
	DWORD handle;
} WD_HS_WATCH;

typedef struct 
{
    DWORD dwVendorId;
    DWORD dwDeviceId;
} WD_PCI_ID;

typedef struct
{
    WD_PCI_ID searchId;     // if dwVendorId==0 - scan all vendor IDs
                            // if dwDeviceId==0 - scan all device IDs
    DWORD dwCards;          // number of cards found
    WD_PCI_ID cardId[WD_PCI_CARDS]; // VendorID & DeviceID of cards found
    WD_PCI_SLOT cardSlot[WD_PCI_CARDS]; // pci slot info of cards found
} WD_PCI_SCAN_CARDS, WD_PCI_SCAN_CARDS_V30;

typedef struct 
{
    WD_PCI_SLOT pciSlot;    // pci slot
    WD_CARD Card;           // get card parameters for pci slot
} WD_PCI_CARD_INFO, WD_PCI_CARD_INFO_V30;

typedef enum
{ 
	PCI_ACCESS_OK = 0, 
	PCI_ACCESS_ERROR = 1, 
	PCI_BAD_BUS = 2, 
	PCI_BAD_SLOT = 3,
} PCI_ACCESS_RESULT;

typedef struct 
{
    WD_PCI_SLOT pciSlot;    // pci bus, slot and function number
    PVOID       pBuffer;    // buffer for read/write
    DWORD       dwOffset;   // offset in pci configuration space to read/write from
    DWORD       dwBytes;    // bytes to read/write from/to buffer
                            // returns the number of bytes read/wrote
    DWORD       fIsRead;    // if 1 then read pci config, 0 write pci config
    DWORD       dwResult;   // PCI_ACCESS_RESULT
} WD_PCI_CONFIG_DUMP, WD_PCI_CONFIG_DUMP_V30;

enum { WD_ISAPNP_CARDS = 16 };
enum { WD_ISAPNP_COMPATIBLE_IDS = 10 };
enum { WD_ISAPNP_COMP_ID_LENGTH = 7 }; // ISA compressed ID is 7 chars long
enum { WD_ISAPNP_ANSI_LENGTH = 32 }; // ISA ANSI ID is limited to 32 chars long
typedef CHAR WD_ISAPNP_COMP_ID[WD_ISAPNP_COMP_ID_LENGTH+1]; 
typedef CHAR WD_ISAPNP_ANSI[WD_ISAPNP_ANSI_LENGTH+1+3]; // add 3 bytes for DWORD alignment
typedef struct 
{
    WD_ISAPNP_COMP_ID cVendor; // Vendor ID
    DWORD dwSerial; // Serial number of card
} WD_ISAPNP_CARD_ID;

typedef struct 
{
    WD_ISAPNP_CARD_ID cardId;  // VendorID & serial number of cards found
    DWORD dwLogicalDevices;	   // Logical devices on the card
    BYTE bPnPVersionMajor;     // ISA PnP version Major
    BYTE bPnPVersionMinor;     // ISA PnP version Minor
    BYTE bVendorVersionMajor;  // Vendor version Major
    BYTE bVendorVersionMinor;  // Vendor version Minor
    WD_ISAPNP_ANSI cIdent;     // Device identifier
} WD_ISAPNP_CARD, WD_ISAPNP_CARD_V40;

typedef struct 
{
    WD_ISAPNP_CARD_ID searchId; // if searchId.cVendor[0]==0 - scan all vendor IDs
                                // if searchId.dwSerial==0 - scan all serial numbers
    DWORD dwCards;              // number of cards found
    WD_ISAPNP_CARD Card[WD_ISAPNP_CARDS]; // cards found
} WD_ISAPNP_SCAN_CARDS, WD_ISAPNP_SCAN_CARDS_V40;

typedef struct 
{
    WD_ISAPNP_CARD_ID cardId;   // VendorID and serial number of card
    DWORD dwLogicalDevice;      // logical device in card
    WD_ISAPNP_COMP_ID cLogicalDeviceId; // logical device ID
    DWORD dwCompatibleDevices;  // number of compatible device IDs
    WD_ISAPNP_COMP_ID CompatibleDevice[WD_ISAPNP_COMPATIBLE_IDS]; // Compatible device IDs
    WD_ISAPNP_ANSI cIdent;      // Device identifier
    WD_CARD Card;               // get card parameters for the ISA PnP card
} WD_ISAPNP_CARD_INFO, WD_ISAPNP_CARD_INFO_V40;

typedef enum 
{ 
	ISAPNP_ACCESS_OK = 0, 
	ISAPNP_ACCESS_ERROR = 1, 
	ISAPNP_BAD_ID = 2, 
} ISAPNP_ACCESS_RESULT;

typedef struct 
{
    WD_ISAPNP_CARD_ID cardId; // VendorID and serial number of card
    DWORD dwOffset;   // offset in ISA PnP configuration space to read/write from
    DWORD fIsRead;    // if 1 then read ISA PnP config, 0 write ISA PnP config
    BYTE  bData;      // result data of byte read/write
    DWORD dwResult;   // ISAPNP_ACCESS_RESULT
} WD_ISAPNP_CONFIG_DUMP, WD_ISAPNP_CONFIG_DUMP_V40;

// PCMCIA Card Services

// Extreme case - two PCMCIA slots and two multi-function (4 functions) cards
enum 
{ 
    WD_PCMCIA_CARDS = 8, 
    WD_PCMCIA_VERSION_LEN = 4, 
    WD_PCMCIA_MANUFACTURER_LEN = 48,
    WD_PCMCIA_PRODUCTNAME_LEN = 48, 
    WD_PCMCIA_MAX_SOCKET = 2,
    WD_PCMCIA_MAX_FUNCTION = 2, 
};

typedef struct 
{
    BYTE uSocket;      // Specifies the socket number (first socket is 0)
    BYTE uFunction;    // Specifies the function number (first function is 0)
    BYTE uPadding0;    // 2 bytes padding so structure will be 4 bytes aligned
    BYTE uPadding1;
} WD_PCMCIA_SLOT, WD_PCMCIA_SLOT_V41;

typedef struct 
{
    DWORD dwManufacturerId; // card manufacturer
    DWORD dwCardId;         // card type and model
} WD_PCMCIA_ID;

typedef struct 
{
    WD_PCMCIA_ID searchId;           // device ID to search for
    DWORD dwCards;                   // number of cards found
    WD_PCMCIA_ID cardId[WD_PCMCIA_CARDS]; // device IDs of cards found
    WD_PCMCIA_SLOT cardSlot[WD_PCMCIA_CARDS]; // pcmcia slot info of cards found
} WD_PCMCIA_SCAN_CARDS, WD_PCMCIA_SCAN_CARDS_V41;

typedef struct 
{
    WD_PCMCIA_SLOT pcmciaSlot; // pcmcia slot
    WD_CARD Card;              // get card parameters for pcmcia slot
    CHAR cVersion[WD_PCMCIA_VERSION_LEN];
    CHAR cManufacturer[WD_PCMCIA_MANUFACTURER_LEN];
    CHAR cProductName[WD_PCMCIA_PRODUCTNAME_LEN];
    DWORD dwManufacturerId;    // card manufacturer
    DWORD dwCardId;            // card type and model
    DWORD dwFuncId;            // card function code
} WD_PCMCIA_CARD_INFO, WD_PCMCIA_CARD_INFO_V41;

typedef struct 
{
    WD_PCMCIA_SLOT pcmciaSlot;    
    PVOID pBuffer;    // buffer for read/write
    DWORD dwOffset;   // offset in pcmcia configuration space to 
					  //	read/write from
    DWORD dwBytes;    // bytes to read/write from/to buffer
                      // 	returns the number of bytes read/wrote
    DWORD fIsRead;    // if 1 then read pci config, 0 write pci config
    DWORD dwResult;   // PCMCIA_ACCESS_RESULT
} WD_PCMCIA_CONFIG_DUMP, WD_PCMCIA_CONFIG_DUMP_V41;

enum { SLEEP_NON_BUSY = 1 };
typedef struct 
{
    DWORD dwMicroSeconds; // Sleep time in Micro Seconds (1/1,000,000 Second)
    DWORD dwOptions;      // can be: SLEEP_NON_BUSY (10000 uSec +)
} WD_SLEEP, WD_SLEEP_V40;

typedef enum 
{ 
	D_OFF 		= 0, 
	D_ERROR		= 1, 
	D_WARN 		= 2, 
	D_INFO 		= 3, 
	D_TRACE		= 4
} DEBUG_LEVEL;

typedef enum 
{ 
	S_ALL		= 0xffffffff, 
    S_IO		= 0x8, 
	S_MEM		= 0x10, 
	S_INT		= 0x20, 
	S_PCI		= 0x40,
	S_DMA		= 0x80,
	S_MISC		= 0x100,
	S_LICENSE	= 0x200,
    S_ISAPNP	= 0x400,
	S_PCMCIA	= 0x800,
	S_KER_PLUG	= 0x10000,
	S_CARD_REG	= 0x2000,
	S_KER_DRV	= 0x4000,
	S_USB		= 0x8000,
	S_CPCI      = 0x20000,
} DEBUG_SECTION;

typedef enum 
{ 
    DEBUG_STATUS = 1, 
    DEBUG_SET_FILTER = 2, 
    DEBUG_SET_BUFFER = 3, 
    DEBUG_CLEAR_BUFFER = 4,
    DEBUG_DUMP_SEC_ON = 5,
    DEBUG_DUMP_SEC_OFF = 6
} DEBUG_COMMAND;

typedef struct 
{
    DWORD dwCmd;     // DEBUG_COMMAND: DEBUG_STATUS, DEBUG_SET_FILTER, DEBUG_SET_BUFFER, DEBUG_CLEAR_BUFFER
    // used for DEBUG_SET_FILTER
    DWORD dwLevel;   // DEBUG_LEVEL: D_ERROR, D_WARN..., or D_OFF to turn debugging off
    DWORD dwSection; // DEBUG_SECTION: for all sections in driver: S_ALL
                     // for partial sections: S_IO, S_MEM...
    DWORD dwLevelMessageBox; // DEBUG_LEVEL to print in a message box
    // used for DEBUG_SET_BUFFER
    DWORD dwBufferSize; // size of buffer in kernel
} WD_DEBUG, WD_DEBUG_V40;

typedef struct 
{
    PCHAR pcBuffer;  // buffer to receive debug messages
    DWORD dwSize;    // size of buffer in bytes
} WD_DEBUG_DUMP, WD_DEBUG_DUMP_V40;

typedef struct 
{
	CHAR pcBuffer[256];
	DWORD dwLevel;   
    DWORD dwSection; 
} WD_DEBUG_ADD, WD_DEBUG_ADD_V52;

typedef struct 
{
    DWORD hKernelPlugIn;
    PCHAR pcDriverName;
    PCHAR pcDriverPath; // if NULL the driver will be searched in the windows system directory 
    PVOID pOpenData;
} WD_KERNEL_PLUGIN, WD_KERNEL_PLUGIN_V40;

typedef enum 
{
	WD_CPCI_INSERT      = 0x1,
	WD_CPCI_REMOVE      = 0x2,
	WD_CPCI_ACKNOWLEDGE = 0x4,
	WD_CPCI_REENUM      = 0x8,
} WD_CPCI_ACTION;

typedef enum
{
	EVENT_STATUS_OK = 0,
	EVENT_STATUS_FAILED_ALLOCATE = 1,
} EVENT_STATUS;

typedef struct
{
	DWORD dwAction; // WD_CPCI_ACTION
	DWORD dwStatus; // EVENT_STATUS
	WD_PCI_ID cardId;
	WD_PCI_SLOT pciSlot;
	DWORD dwEventId;
} WD_CPCI_EVENT, WD_CPCI_EVENT_V443;

typedef struct
{
	WD_CPCI_EVENT cpciEvent;
	DWORD hInterrupt;
} WD_HS_REGISTER_EVENT, WD_HS_REGISTER_EVENT_V443;

typedef enum {
    PIPE_TYPE_CONTROL     = 0,
    PIPE_TYPE_ISOCHRONOUS = 1,
    PIPE_TYPE_BULK        = 2,
    PIPE_TYPE_INTERRUPT   = 3,
} USB_PIPE_TYPE;

#define WD_USB_MAX_PIPE_NUMBER_V432 16
#define WD_USB_MAX_PIPE_NUMBER 32
#define WD_USB_MAX_ENDPOINTS WD_USB_MAX_PIPE_NUMBER
#define WD_USB_MAX_INTERFACES 30

#define WD_USB_MAX_DEVICE_NUMBER_V432 20
#define WD_USB_MAX_DEVICE_NUMBER 127

typedef struct
{
	DWORD dwVendorId;
	DWORD dwProductId;
} WD_USB_ID;

typedef enum {
    USB_DIR_IN     = 1,
    USB_DIR_OUT    = 2,
    USB_DIR_IN_OUT = 3,
} USB_DIR;

typedef struct
{
    DWORD dwNumber;        // Pipe 0 is the default pipe
	DWORD dwMaximumPacketSize;
	DWORD type;            // USB_PIPE_TYPE
    DWORD direction;       // USB_DIR
                           // Isochronous, Bulk, Interrupt are either USB_DIR_IN or USB_DIR_OUT
                           // Control are USB_DIR_IN_OUT
    DWORD dwInterval;      // interval in ms relevant to Interrupt pipes
} WD_USB_PIPE_INFO, WD_USB_PIPE_INFO_V43;

typedef struct
{
    DWORD dwNumInterfaces;
    DWORD dwValue;
    DWORD dwAttributes;
    DWORD MaxPower;
} WD_USB_CONFIG_DESC;

typedef struct
{
    DWORD dwNumber;
    DWORD dwAlternateSetting;
    DWORD dwNumEndpoints;
    DWORD dwClass;
    DWORD dwSubClass;
    DWORD dwProtocol;
    DWORD dwIndex;
} WD_USB_INTERFACE_DESC, WD_USB_INTERFACE_DESC_V43;

typedef struct
{
    DWORD dwEndpointAddress;
    DWORD dwAttributes;
    DWORD dwMaxPacketSize;
    DWORD dwInterval;
} WD_USB_ENDPOINT_DESC, WD_USB_ENDPOINT_DESC_V43;

typedef struct
{
    WD_USB_INTERFACE_DESC Interface;
    WD_USB_ENDPOINT_DESC Endpoints[WD_USB_MAX_ENDPOINTS];
} WD_USB_INTERFACE;

typedef struct 
{
    DWORD uniqueId;
    DWORD dwConfigurationIndex;
    WD_USB_CONFIG_DESC configuration;
    DWORD dwInterfaceAlternatives;
    WD_USB_INTERFACE Interface[WD_USB_MAX_INTERFACES];
} WD_USB_CONFIGURATION, WD_USB_CONFIGURATION_V43;

typedef struct
{
    DWORD   fBusPowered;
    DWORD   dwPorts;              // number of ports on this hub
    DWORD   dwCharacteristics;    // Hub Charateristics
    DWORD   dwPowerOnToPowerGood; // port power on till power good in 2ms
    DWORD   dwHubControlCurrent;  // max current in mA
} WD_USB_HUB_GENERAL_INFO, WD_USB_HUB_GENERAL_INFO_V43;

typedef struct
{
	WD_USB_ID deviceId;
	DWORD dwHubNum;
	DWORD dwPortNum;
    DWORD fHub;
    DWORD fFullSpeed;
    DWORD dwConfigurationsNum;
    DWORD deviceAddress;
    WD_USB_HUB_GENERAL_INFO hubInfo;
} WD_USB_DEVICE_GENERAL_INFO;

typedef struct
{
	DWORD dwPipes;
	WD_USB_PIPE_INFO Pipe[WD_USB_MAX_PIPE_NUMBER_V432];
} WD_USB_DEVICE_INFO_V432;

typedef struct
{
	DWORD dwPipes;
	WD_USB_PIPE_INFO Pipe[WD_USB_MAX_PIPE_NUMBER];
} WD_USB_DEVICE_INFO, WD_USB_DEVICE_INFO_V43;

// IOCTL Structures
typedef struct 
{
	WD_USB_ID searchId; // if dwVendorId==0 - scan all vendor IDs
                        // if dwProductId==0 - scan all product IDs
	DWORD dwDevices;
    DWORD uniqueId[WD_USB_MAX_DEVICE_NUMBER_V432]; // a unique id to identify the device
    WD_USB_DEVICE_GENERAL_INFO deviceGeneralInfo[WD_USB_MAX_DEVICE_NUMBER_V432];
} WD_USB_SCAN_DEVICES_V432;

typedef struct 
{
	WD_USB_ID searchId; // if dwVendorId==0 - scan all vendor IDs
                        // if dwProductId==0 - scan all product IDs
	DWORD dwDevices;
    DWORD uniqueId[WD_USB_MAX_DEVICE_NUMBER]; // a unique id to identify the device
    WD_USB_DEVICE_GENERAL_INFO deviceGeneralInfo[WD_USB_MAX_DEVICE_NUMBER];
} WD_USB_SCAN_DEVICES, WD_USB_SCAN_DEVICES_V43;

// USB TRANSFER options
enum { USB_TRANSFER_HALT = 1 };

typedef struct
{
	DWORD hDevice;      // handle of USB device to read from or write to
	DWORD dwPipe;       // pipe number on device
    DWORD fRead;
	DWORD dwOptions;    // USB_TRANSFER options:
                        //    USB_TRANSFER_HALT halts the pervious transfer
    PVOID pBuffer;      // pointer to buffer to read/write
    DWORD dwBytes;
    DWORD dwTimeout;    // timeout for the transfer in milli-seconds. 0==>no timeout.
	DWORD dwBytesTransfered;    // returns the number of bytes actually read/written
    BYTE  SetupPacket[8];       // setup packet for control pipe transfer
    DWORD fOK;          // transfer succeeded
} WD_USB_TRANSFER, WD_USB_TRANSFER_V43;


typedef struct {
    DWORD uniqueId;                 // the device unique ID
    DWORD dwConfigurationIndex;     // the index of the configuration to register
    DWORD dwInterfaceNum;           // interface to register
    DWORD dwInterfaceAlternate;
	DWORD hDevice;                  // handle of device
	WD_USB_DEVICE_INFO_V432 Device;      // description of the device
    DWORD dwOptions;                // should be zero
    CHAR  cName[32];                // name of card
    CHAR  cDescription[100];        // description
} WD_USB_DEVICE_REGISTER_V432;

typedef struct {
    DWORD uniqueId;                 // the device unique ID
    DWORD dwConfigurationIndex;     // the index of the configuration to register
    DWORD dwInterfaceNum;           // interface to register
    DWORD dwInterfaceAlternate;
	DWORD hDevice;                  // handle of device
	WD_USB_DEVICE_INFO Device;      // description of the device
    DWORD dwOptions;                // should be zero
    CHAR  cName[32];                // name of card
    CHAR  cDescription[100];        // description
} WD_USB_DEVICE_REGISTER, WD_USB_DEVICE_REGISTER_V43;

typedef struct
{
	DWORD hDevice;
	DWORD dwPipe;
} WD_USB_RESET_PIPE, WD_USB_RESET_PIPE_V43;

#define WD_KERNEL_DRIVER_PLUGIN_HANDLE 0xffff0000

static DWORD GlobalDW;

#ifndef BZERO
	#define BZERO(buf) memset(&(buf), 0, sizeof(buf))
#endif

#ifndef INVALID_HANDLE_VALUE
    #define INVALID_HANDLE_VALUE ((HANDLE)(-1))
#endif

#ifndef CTL_CODE
    #define CTL_CODE(DeviceType, Function, Method, Access) ( \
        ((DeviceType)<<16) | ((Access)<<14) | ((Function)<<2) | (Method) \
    )

    #define METHOD_BUFFERED   0
    #define METHOD_IN_DIRECT  1
    #define METHOD_OUT_DIRECT 2
    #define METHOD_NEITHER    3
    #define FILE_ANY_ACCESS   0
    #define FILE_READ_ACCESS  1    // file & pipe
    #define FILE_WRITE_ACCESS 2    // file & pipe
#endif

// Device type
#define WD_TYPE 38200
#define WD_CTL_CODE(wFuncNum) ((DWORD) CTL_CODE( WD_TYPE, wFuncNum, METHOD_NEITHER, FILE_ANY_ACCESS))

// WinDriver function IOCTL calls.  For details on the WinDriver functions,
// see the WinDriver manual or included help files.

#define IOCTL_WD_DMA_LOCK                 WD_CTL_CODE(0x901)
#define IOCTL_WD_DMA_UNLOCK               WD_CTL_CODE(0x902)
#define IOCTL_WD_TRANSFER                 WD_CTL_CODE(0x903)
#define IOCTL_WD_MULTI_TRANSFER           WD_CTL_CODE(0x904)
#define IOCTL_WD_PCI_SCAN_CARDS           WD_CTL_CODE(0x90e)
#define IOCTL_WD_PCI_GET_CARD_INFO        WD_CTL_CODE(0x90f)
#define IOCTL_WD_VERSION                  WD_CTL_CODE(0x910)
#define IOCTL_WD_PCI_CONFIG_DUMP          WD_CTL_CODE(0x91a)
#define IOCTL_WD_KERNEL_PLUGIN_OPEN       WD_CTL_CODE(0x91b)
#define IOCTL_WD_KERNEL_PLUGIN_CLOSE      WD_CTL_CODE(0x91c)
#define IOCTL_WD_KERNEL_PLUGIN_CALL       WD_CTL_CODE(0x91d)
#define IOCTL_WD_INT_ENABLE               WD_CTL_CODE(0x91e)
#define IOCTL_WD_INT_DISABLE              WD_CTL_CODE(0x91f)
#define IOCTL_WD_INT_COUNT                WD_CTL_CODE(0x920)
#define IOCTL_WD_ISAPNP_SCAN_CARDS        WD_CTL_CODE(0x924)
#define IOCTL_WD_ISAPNP_CONFIG_DUMP       WD_CTL_CODE(0x926)
#define IOCTL_WD_SLEEP                    WD_CTL_CODE(0x927)
#define IOCTL_WD_DEBUG                    WD_CTL_CODE(0x928)
#define IOCTL_WD_DEBUG_DUMP               WD_CTL_CODE(0x929)
#define IOCTL_WD_CARD_UNREGISTER          WD_CTL_CODE(0x92b)
#define IOCTL_WD_ISAPNP_GET_CARD_INFO     WD_CTL_CODE(0x92d)
#define IOCTL_WD_PCMCIA_SCAN_CARDS        WD_CTL_CODE(0x92f)
#define IOCTL_WD_PCMCIA_GET_CARD_INFO     WD_CTL_CODE(0x930)
#define IOCTL_WD_PCMCIA_CONFIG_DUMP       WD_CTL_CODE(0x931)
#define IOCTL_WD_CARD_REGISTER            WD_CTL_CODE(0x956)
#define IOCTL_WD_INT_WAIT                 WD_CTL_CODE(0x94b)
#define IOCTL_WD_WATCH_PCI                WD_CTL_CODE(0x94c)
#define IOCTL_WD_WATCH_PCI_START          WD_CTL_CODE(0x95f)
#define IOCTL_WD_WATCH_PCI_STOP           WD_CTL_CODE(0x94e)
#define IOCTL_WD_HS_EVENT_PULL            WD_CTL_CODE(0x95b)
#define IOCTL_WD_HS_EVENT_SEND            WD_CTL_CODE(0x960)
#define IOCTL_WD_HS_EVENT_UNREGISTER      WD_CTL_CODE(0x95d)
#define IOCTL_WD_HS_EVENT_REGISTER        WD_CTL_CODE(0x95e)
#define IOCTL_WD_LICENSE                  WD_CTL_CODE(0x952)
#define IOCTL_WD_USB_RESET_PIPE           WD_CTL_CODE(0x93c)
#define IOCTL_WD_USB_RESET_DEVICE         WD_CTL_CODE(0x93f)
#define IOCTL_WD_USB_SCAN_DEVICES_V432	  WD_CTL_CODE(0x940)
#define IOCTL_WD_USB_SCAN_DEVICES         WD_CTL_CODE(0x95a)
#define IOCTL_WD_USB_TRANSFER             WD_CTL_CODE(0x942)
#define IOCTL_WD_USB_DEVICE_REGISTER      WD_CTL_CODE(0x949)
#define IOCTL_WD_USB_DEVICE_UNREGISTER    WD_CTL_CODE(0x959)
#define IOCTL_WD_USB_GET_CONFIGURATION    WD_CTL_CODE(0x946)
#define IOCTL_WD_DEBUG_ADD                WD_CTL_CODE(0x964)

#if defined(UNIX)
	typedef struct
	{ 
		DWORD dwHeader; 
		PVOID pData; 
		DWORD dwSize;
	} WD_IOCTL_HEADER;
	
	enum { WD_IOCTL_HEADER_CODE = 0xa410b413 };
#endif

#if defined(__KERNEL__) || defined(OS2)
	#if defined(OS2)
		DWORD EXPENTRY wdr_open();
        NTSTATUS EXPENTRY wdr_close(HANDLE h);
        NTSTATUS EXPENTRY wdr_ioctl(HANDLE h, DWORD dwFuncNum, PVOID pParam, DWORD dwSize);
		#define WD_Open() ((HANDLE)wdr_open()) 

		#define WD_Close(h) wdr_close(h)
		#define WD_FUNCTION(wFuncNum, h, pParam, dwSize, fWait) \
		{ \
			wdr_ioctl(h, \
				wFuncNum, \
				 pParam, \
				 dwSize \
				); \
		}
	#else
		HANDLE __cdecl WD_Open();
		void __cdecl WD_Close(HANDLE hWD);
		ULONG __cdecl KP_DeviceIoControl(DWORD dwFuncNum, HANDLE h, PVOID pParam,
			DWORD dwSize);
		#define WD_FUNCTION(wFuncNum, h, pParam, dwSize, fWait) \
			KP_DeviceIoControl(\
				(DWORD) wFuncNum, h,\
				(PVOID) pParam, (DWORD) dwSize\
		        ) 
	#endif
#else
    #if defined(UNIX)
    	static inline ULONG WD_FUNCTION_LOCAL( DWORD wFuncNum, HANDLE h,
				PVOID pParam, DWORD dwSize, BOOL fWait)
    	{
		    WD_IOCTL_HEADER ioctl_hdr;
		    ioctl_hdr.dwHeader = WD_IOCTL_HEADER_CODE;
		    ioctl_hdr.pData = pParam;
		    ioctl_hdr.dwSize = dwSize;
			#if defined(VXWORKS)
            	return (ULONG) ioctl((int)(h), wFuncNum, (int)&ioctl_hdr);
			#elif defined(LINUX) || defined(SOLARIS)
            	return (ULONG) ioctl((int)(h), wFuncNum, &ioctl_hdr);
			#endif
        }

		#if defined(VXWORKS)
			#define WD_OpenLocal()\
				((HANDLE)open("/windrvr", O_RDWR, 0644))
		#else
			#define WD_OpenLocal()\
				((HANDLE)open("/dev/windrvr", O_RDWR))
		#endif
			
        #define WD_CloseLocal(h)\
            close((int) (h))
    
	#elif defined(WINCE) && defined(_WIN32_WCE_EMULATION)
		HANDLE WINAPI WCE_EMU_WD_Open();
		void WINAPI WCE_EMU_WD_Close(HANDLE hWD);
		BOOL WINAPI WCE_EMU_WD_FUNCTION(DWORD wFuncNum, HANDLE h, PVOID 
			pParam, DWORD dwSize);
		#define WD_OpenLocal() WCE_EMU_WD_Open()
		#define WD_CloseLocal(h) WCE_EMU_WD_Close(h)
		#define WD_FUNCTION_LOCAL(wFuncNum, h, pParam, dwSize, fWait) \
			WCE_EMU_WD_FUNCTION(wFuncNum, h, pParam, dwSize)
	#elif defined(WIN32) || defined(WINCE)
        #define WD_CloseLocal(h)\
            CloseHandle(h)
        #if defined(WINCE)
        	void WINAPI SetProcPermissions(DWORD dwPermissions);
            #define WD_OpenLocal()\
                (SetProcPermissions(0xFFFF), CreateFile(\
                    TEXT("WDR1:"),\
                    GENERIC_READ,\
                    FILE_SHARE_READ | FILE_SHARE_WRITE,\
                    NULL, OPEN_EXISTING, 0, NULL))
            #define WD_FUNCTION_LOCAL(wFuncNum, h, pParam, dwSize, fWait)\
            	((DWORD) DeviceIoControl(h, wFuncNum, pParam, dwSize, NULL, 0,\
            		&GlobalDW, NULL))
        #elif defined(WIN32)
            #define WD_OpenLocal()\
                CreateFile(\
                    TEXT("\\\\.\\MDHALDR"),\
                    GENERIC_READ,\
                    FILE_SHARE_READ | FILE_SHARE_WRITE,\
                    NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL)  
	    	static DWORD WD_FUNCTION_LOCAL(DWORD wFuncNum, HANDLE h,
				PVOID pParam, DWORD dwSize, BOOL fWait)
	    	{
	            HANDLE hWD = fWait ? WD_OpenLocal() : h;
	            DWORD rc;
	            if (hWD==INVALID_HANDLE_VALUE)
	                return (DWORD) -1;
	            rc = (DWORD) DeviceIoControl(hWD, (DWORD) wFuncNum, (PVOID) pParam,
	                (DWORD) dwSize, NULL, 0, &GlobalDW, NULL);
	            if (fWait)
	                WD_CloseLocal(hWD);
	            return rc;
	        }
        #endif
    #endif
	#ifndef _WINDRVR_NET_H_ 
		#define WD_FUNCTION WD_FUNCTION_LOCAL
		#define WD_Close WD_CloseLocal
		#define WD_Open WD_OpenLocal
	#endif
#endif

#define WD_Debug(h,pDebug)\
    WD_FUNCTION(IOCTL_WD_DEBUG, h, pDebug, sizeof (WD_DEBUG), FALSE)
#define WD_DebugDump(h,pDebugDump)\
    WD_FUNCTION(IOCTL_WD_DEBUG_DUMP, h, pDebugDump, sizeof (WD_DEBUG_DUMP), FALSE)
#define WD_DebugAdd(h, pDebugAdd)\
	WD_FUNCTION(IOCTL_WD_DEBUG_ADD, h, pDebugAdd, sizeof(WD_DEBUG_ADD), FALSE)
#define WD_Transfer(h,pTransfer)\
    WD_FUNCTION(IOCTL_WD_TRANSFER, h, pTransfer, sizeof (WD_TRANSFER), FALSE)
#define WD_MultiTransfer(h,pTransferArray,dwNumTransfers)\
    WD_FUNCTION(IOCTL_WD_MULTI_TRANSFER, h, pTransferArray, sizeof (WD_TRANSFER) * dwNumTransfers, FALSE)
#define WD_DMALock(h,pDma)\
    WD_FUNCTION(IOCTL_WD_DMA_LOCK, h, pDma, sizeof (WD_DMA), FALSE)
#define WD_DMAUnlock(h,pDma)\
    WD_FUNCTION(IOCTL_WD_DMA_UNLOCK, h, pDma, sizeof (WD_DMA), FALSE)
#define WD_CardRegister(h,pCard)\
    WD_FUNCTION(IOCTL_WD_CARD_REGISTER, h, pCard, sizeof (WD_CARD_REGISTER), FALSE)
#define WD_CardUnregister(h,pCard)\
    WD_FUNCTION(IOCTL_WD_CARD_UNREGISTER, h, pCard, sizeof (WD_CARD_REGISTER), FALSE)
#define WD_PciScanCards(h,pPciScan)\
    WD_FUNCTION(IOCTL_WD_PCI_SCAN_CARDS, h, pPciScan, sizeof (WD_PCI_SCAN_CARDS), FALSE)
#define WD_PciGetCardInfo(h,pPciCard)\
    WD_FUNCTION(IOCTL_WD_PCI_GET_CARD_INFO, h, pPciCard, sizeof (WD_PCI_CARD_INFO), FALSE)
#define WD_PciConfigDump(h,pPciConfigDump)\
    WD_FUNCTION(IOCTL_WD_PCI_CONFIG_DUMP, h, pPciConfigDump, sizeof (WD_PCI_CONFIG_DUMP), FALSE)
#define WD_Version(h,pVerInfo)\
    WD_FUNCTION(IOCTL_WD_VERSION, h, pVerInfo, sizeof (WD_VERSION), FALSE)
#define WD_License(h,pLicense)\
    WD_FUNCTION(IOCTL_WD_LICENSE, h, pLicense, sizeof (WD_LICENSE), FALSE)
#define WD_KernelPlugInOpen(h,pKernelPlugIn)\
    WD_FUNCTION(IOCTL_WD_KERNEL_PLUGIN_OPEN, h, pKernelPlugIn, sizeof (WD_KERNEL_PLUGIN), FALSE)
#define WD_KernelPlugInClose(h,pKernelPlugIn)\
    WD_FUNCTION(IOCTL_WD_KERNEL_PLUGIN_CLOSE, h, pKernelPlugIn, sizeof (WD_KERNEL_PLUGIN), FALSE)
#define WD_KernelPlugInCall(h,pKernelPlugInCall)\
    WD_FUNCTION(IOCTL_WD_KERNEL_PLUGIN_CALL, h, pKernelPlugInCall, sizeof (WD_KERNEL_PLUGIN_CALL), FALSE)
#define WD_IntEnable(h,pInterrupt)\
    WD_FUNCTION(IOCTL_WD_INT_ENABLE, h, pInterrupt, sizeof (WD_INTERRUPT), FALSE)
#define WD_IntDisable(h,pInterrupt)\
    WD_FUNCTION(IOCTL_WD_INT_DISABLE, h, pInterrupt, sizeof (WD_INTERRUPT), FALSE)
#define WD_IntCount(h,pInterrupt)\
    WD_FUNCTION(IOCTL_WD_INT_COUNT, h, pInterrupt, sizeof (WD_INTERRUPT), FALSE)
#define WD_IntWait(h,pInterrupt)\
    WD_FUNCTION(IOCTL_WD_INT_WAIT, h, pInterrupt, sizeof (WD_INTERRUPT), TRUE)
#define WD_IsapnpScanCards(h,pIsapnpScan)\
    WD_FUNCTION(IOCTL_WD_ISAPNP_SCAN_CARDS, h, pIsapnpScan, sizeof (WD_ISAPNP_SCAN_CARDS), FALSE)
#define WD_IsapnpGetCardInfo(h,pIsapnpCard)\
    WD_FUNCTION(IOCTL_WD_ISAPNP_GET_CARD_INFO, h, pIsapnpCard, sizeof (WD_ISAPNP_CARD_INFO), FALSE)
#define WD_IsapnpConfigDump(h,pIsapnpConfigDump)\
    WD_FUNCTION(IOCTL_WD_ISAPNP_CONFIG_DUMP, h, pIsapnpConfigDump, sizeof (WD_ISAPNP_CONFIG_DUMP), FALSE)
#define WD_PcmciaScanCards(h,pPcmciaScan)\
    WD_FUNCTION(IOCTL_WD_PCMCIA_SCAN_CARDS, h, pPcmciaScan, sizeof (WD_PCMCIA_SCAN_CARDS), FALSE)
#define WD_PcmciaGetCardInfo(h,pPcmciaCard)\
    WD_FUNCTION(IOCTL_WD_PCMCIA_GET_CARD_INFO, h, pPcmciaCard, sizeof (WD_PCMCIA_CARD_INFO), FALSE)
#define WD_PcmciaConfigDump(h,pPcmciaConfigDump)\
    WD_FUNCTION(IOCTL_WD_PCMCIA_CONFIG_DUMP, h, pPcmciaConfigDump, sizeof (WD_PCMCIA_CONFIG_DUMP), FALSE)
#define WD_Sleep(h,pSleep)\
    WD_FUNCTION(IOCTL_WD_SLEEP, h, pSleep, sizeof (WD_SLEEP), FALSE)
#define WD_WatchPciStart(h, pHsWatch)\
    WD_FUNCTION(IOCTL_WD_WATCH_PCI_START, h, pHsWatch, sizeof(WD_HS_WATCH), FALSE)
#define WD_WatchPciStop(h, pHsWatch)\
	WD_FUNCTION(IOCTL_WD_WATCH_PCI_STOP, h, pHsWatch, sizeof(WD_HS_WATCH), FALSE)
#define WD_HsEventPull(h, pHsEvent)\
    WD_FUNCTION(IOCTL_WD_HS_EVENT_PULL, h, pHsEvent, sizeof (WD_HS_REGISTER_EVENT), FALSE)
#define WD_HsEventSend(h, pHsEvent)\
    WD_FUNCTION(IOCTL_WD_HS_EVENT_SEND, h, pHsEvent, sizeof (WD_HS_REGISTER_EVENT), FALSE)
#define WD_HsEventUnregister(h, pHsEvent)\
    WD_FUNCTION(IOCTL_WD_HS_EVENT_UNREGISTER, h, pHsEvent, sizeof (WD_HS_REGISTER_EVENT), FALSE)
#define WD_HsEventRegister(h, pHsEvent)\
    WD_FUNCTION(IOCTL_WD_HS_EVENT_REGISTER, h, pHsEvent, sizeof (WD_HS_REGISTER_EVENT), FALSE)
#define WD_UsbScanDevice(h, pScan)\
    WD_FUNCTION(IOCTL_WD_USB_SCAN_DEVICES, h, pScan,sizeof(WD_USB_SCAN_DEVICES), FALSE)
#define WD_UsbScanDeviceV432(h, pScan)\
    WD_FUNCTION(IOCTL_WD_USB_SCAN_DEVICES_V432, h, pScan,sizeof(WD_USB_SCAN_DEVICES_V432), FALSE)
#define WD_UsbGetConfiguration(h, pConfig) \
	WD_FUNCTION(IOCTL_WD_USB_GET_CONFIGURATION, h, pConfig, sizeof(WD_USB_CONFIGURATION), FALSE)
#define WD_UsbDeviceRegister(h, pRegister)\
	WD_FUNCTION(IOCTL_WD_USB_DEVICE_REGISTER, h, pRegister, sizeof(WD_USB_DEVICE_REGISTER), FALSE)
#define WD_UsbTransfer(h, pTrans)\
	WD_FUNCTION(IOCTL_WD_USB_TRANSFER, h, pTrans, sizeof(WD_USB_TRANSFER), TRUE)
#define WD_UsbDeviceUnregister(h, pRegister)\
	WD_FUNCTION(IOCTL_WD_USB_DEVICE_UNREGISTER, h, pRegister, sizeof(WD_USB_DEVICE_REGISTER), FALSE)
#define WD_UsbResetPipe(h, pResetPipe)\
	WD_FUNCTION(IOCTL_WD_USB_RESET_PIPE, h, pResetPipe, sizeof(WD_USB_RESET_PIPE), FALSE)
#define WD_UsbResetDevice(h, hDevice)\
	WD_FUNCTION(IOCTL_WD_USB_RESET_DEVICE, h, &hDevice, sizeof(DWORD), FALSE)

#ifdef __cplusplus
}
#endif

#endif

