#define BT848_RISC_IRQ         (1<<24)
#define BT848_INT_STAT         0x100
#define BT848_INT_MASK         0x104

enum { 
	BT8X8_IOCTL_VERSION = 1, 
	BT8X8_IOCTL_SET_INIT = 2,
	BT8X8_IOCTL_GET_INIT = 3, 
	BT8X8_IOCTL_SET_VCIRQCOUNT = 4,
	BT8X8_IOCTL_SET_CURRENTIRQ = 5

}; // in this sample we implement a GetVersion message

typedef struct {
    CHAR  cVer[100];
} BT8X8_VERSION;

typedef struct {
   
    DWORD ComAddress;
	DWORD PictureNummer;
	DWORD Running_Irq;
	DWORD VCIrqCount;
	DWORD CurrentIrq;

} BT8X8_ISR;

BT8X8_ISR BT8X8_Last_ISR;

enum { BT8X8_WRONG_MESSAGE = 0x1000 };

enum { BT8X8_OK = 1 };
