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
 *********************************************************************************
 * Change: new Winbis-dump by Elms                                               *
 *********************************************************************************/ 

#include <windows.h>       /* required for all Windows applications */
#include <stdio.h>
#include "resource.h"
#include <ddraw.h>
#include <math.h>
#include <fcntl.h>
#include <imagehlp.h>
#include "../DrvLib/windrvr.h"
#ifdef USE_BT8X8
#include "../Bt8x8.Dev/BTIOcom.h"
#endif
#include "bt848.h"

extern void Start_Thread();
extern void Write_Log(const char *Text);
void Stop_Thread();
BOOL APIENTRY ChipSettingProc(HWND hDlg,UINT message,UINT wParam,LONG lParam);

int VBI_VDatScan(BYTE *VBI_Buffer, unsigned int step, int BytePos); 
void Work_VideoDat(unsigned char *Buffer);
void VBI_VDat_Blockaustausch ( void );
int VBI_VDAT_DecodeBlockz ( void );
void VBI_VDat_Filename(struct TVDatBlockz Block, char *fNames);
void VBI_VDAT_SOTInfo(struct TVDatBlockz Block,struct SOTREC *Info);
BOOL VBI_VDat_WriteData(struct TVDatBlockz  Block);
unsigned int VBI_VDat_FileSize(struct TVDatBlockz Block);
void VideoDat_Flush ( void );



void DoVBI();
int Get_BT_Card_Interrupt( void );
void VBI_decode_vps(unsigned char *data);
unsigned char unham(unsigned char *d);
unsigned char unham2(unsigned char *d);
void deileave(unsigned char *buf);
void vcdump(unsigned char *buf);
void vc2dump(unsigned char *buf);
unsigned char VBI_Scan(BYTE *Buffer,unsigned int step); 

void VBI_AGC(BYTE *Buffer,int start, int stop, int step);
void VBI_decode_vt(unsigned char *dat);
void VBI_decode_line(unsigned char *d, int line); 
BOOL Is_Nagra_Coded(BYTE *cVBI);
void Get_VC_Crypt_Mode(unsigned char *d, int line); 

#ifdef USE_BT8X8
void BT8X8_KP_GET_INIT(HANDLE hWD, DWORD hKernelPlugIn, BT8X8_ISR *btisr);
void BT8X8_KP_SET_VCIRQCOUNT(HANDLE hWD, DWORD hKernelPlugIn);
void BT8X8_KP_SET_CURRENTIRQ(HANDLE hWD, DWORD hKernelPlugIn,DWORD wert);
void BT8X8_KP_GET_DRIVER_VERSION(HANDLE hWD, DWORD hKernelPlugIn, char *buf);
#endif
void SAA7146_KP_GET_DRIVER_VERSION(HANDLE hWD, DWORD hKernelPlugIn, char *buf);
void Reset_VCIRQCount();
void Set_CurrentIrq(DWORD Wert);


void ExitTV(void);
BOOL InitTV( HWND hwnd , BOOL Fullscreen, int x , int y );
BOOL OverlayUpdate(int x,int y,LPRECT pDestRect,DWORD  dwFlags,BOOL ColorKey);
void  Destroy_Overlay();
BOOL CreateOverlay(int x,int y);


BOOL Alloc_DMA(DWORD dwSize, WD_DMA *dma,int Option );
void Free_DMA( WD_DMA *dma );

#ifdef USE_BT8X8
BOOL Alloc_Display_DMA(DWORD dwSize, int NR);
BOOL Alloc_Surface_DMA();
BOOL Init_BT_HardWare(HWND hWnd);
void BT8X8_IntHandlerRoutine(BT8X8_HANDLE hBT8X8, BT8X8_INT_RESULT *intResult);
PHYS RiscLogToPhys(DWORD* pLog);
void MakeVBITable(int VBI_Lines);
void MakeVBITable2(int VBI_Lines);
void MakeVBITable3(int VBI_Lines);
void MakeVBITable4(int VBI_Lines);
void MakeVBITable5(int VBI_Lines);
void Set_PLL( int PLL );
void FillRiscJumps(int nFlags);
void SetRiscJumpsDecode(int nFlags);
BOOL SetGeoSize(int wWidth, int wHeight);
BOOL SetBrightness(unsigned char bBrightness);
BOOL SetHue(char bHue);
BOOL SetContrast(int wContrast);
BOOL SetSaturationU(int wData);
BOOL SetSaturationV(int wData);
BOOL SetVideoSource(int nInput);
BOOL SetColourFormat(int nColourFormat);
void SetDMA(BOOL bState);
void MakeVideoTableForDisplay();
void MakeVideoTableForDisplayHalf();
void MakeVideoTableForDisplayNarga();
void MakeVideoTableForDisplayNargaHalf();
void MakeVideoTableForDisplayVCrypt();
void MakeVideoTableForDirectDraw(WD_DMA *dma,int Pitch,int PosOffset);   
#endif

void Free_Surface_DMA( void );
BOOL Alloc_Overlay_DMA(int x, int y);
void Free_Overlay_DMA( void );
BOOL LockMemory(LPVOID pLinear, DWORD dwSize);
BOOL UnlockMemory(LPVOID pLinear, DWORD dwSize);
BOOL Init_TV_Karte(HWND hWnd);
BOOL Init_Tuner(int TunerNr );		  



BOOL SetGeometry(int width, int height, int Type,int ColourFormat);
BOOL SetAudioSource(int nChannel);
void Set_Capture(int nState);

BOOL Audio_WriteMSP(BYTE bSubAddr, int wAddr, int wData);
BOOL Audio_SetVolume(int nVolume);
BOOL Audio_SetBalance(char nBalance);
BOOL Audio_SetBass(char nBass);
BOOL Audio_SetTreble(char nTreble);
BOOL Audio_SetLoudness(BYTE nLoudness);
BOOL Audio_SetSpatial(char nSpatial);
BOOL Audio_SetSuperBass(BOOL bSuperBass);
void Audio_SetToneControl(BOOL  nMode);
BOOL Audio_SetEqualizer1(char nLevel);
BOOL Audio_SetEqualizer2(char nLevel);
BOOL Audio_SetEqualizer3(char nLevel);
BOOL Audio_SetEqualizer4(char nLevel);
BOOL Audio_SetEqualizer5(char nLevel);

int Audio_GetStereoMode(BOOL bHardwareMode);
void Load_Country_Settings();
void Load_Country_Specific_Settings(int LPos);
BOOL VideoPresent();

void WinBis_daten_ausgabe(void);
void WinBis_push_data(unsigned char *dat);
void winbis_decoder(unsigned char *dat);  


extern void Work_Text(unsigned char *TmpBuffer);
extern BOOL APIENTRY MDBootOptionProc(HWND hDlg,UINT message,UINT wParam,LONG lParam);




//Bus
BOOL I2CBus_AddDevice(BYTE I2C_Port);
BOOL I2CBus_Lock();
BOOL I2CBus_Unlock();
void I2CBus_Start();
void I2CBus_Stop();
void I2CBus_One();
void I2CBus_Zero();
BOOL I2CBus_Ack();
BOOL I2CBus_SendByte(BYTE nData, int nWaitForAck);
BYTE I2CBus_ReadByte(BOOL bLast);
BYTE I2CBus_Read(BYTE nAddr);
BOOL I2CBus_Write(BYTE nAddr, BYTE nData1, BYTE nData2, BOOL bSendBoth);
void I2CBus_wait(int us);
void I2C_SetLine(BOOL bCtrl, BOOL bData);
BOOL I2C_GetLine();
BYTE I2C_Read(BYTE nAddr);
BOOL I2C_Write(BYTE nAddr, BYTE nData1, BYTE nData2, BOOL bSendBoth);
BOOL Tuner_SetFrequency(int TunerTyp, int wFrequency);
BOOL Tuner_SetChannel(int nChannel, BOOL bNotify);
BOOL Tuner_Scan();

BOOL Init_Audio(BYTE DRead,BYTE DWrite );
void Audio_Autodetect();


BOOL MSP_Reset();
BOOL MSP_Version();
void MSP_Set_MajorMinor_Mode(int MajorMode,int MinorMode  );
void MSP_SetMode(int type);
void MSP_SetStereo(int MajorMode, int MinorMode, int mode);

extern void SAA7146_SetColourFormat(int nColourFormat);
extern void video_setmode( int v);
extern void set_up_grabbing();
extern void SAA7146_Close(BT8X8_HANDLE hSAA7146);

int PCI_Scan( void );

extern BOOL Get_Version_Informations(LPSTR Text);
extern void Get_DVB_MacAdr( unsigned char *MAC ); 

#ifdef USE_BT8X8

void MaskDataByte(int Offset,BYTE d, BYTE m)
{
BYTE a;
BYTE b;
a=BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,Offset);
b=(a&~(m))|((d)&(m));
BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,Offset,b);
};





void MaskDataWord(int Offset,WORD d, WORD m)
{
WORD a;
WORD b;
a=BT8X8_ReadWord(hBT8X8, BT8X8_AD_BAR0,Offset);
b=(a&~(m))|((d)&(m));
BT8X8_WriteWord(hBT8X8, BT8X8_AD_BAR0,Offset,b);
};


void AndDataByte(int Offset,BYTE d)
{
BYTE a;
BYTE b;
a=BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,Offset);
b=a&d;
BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,Offset,b);
};


void AndDataWord(int Offset,short d)
{
WORD a;
WORD b;
a=BT8X8_ReadWord(hBT8X8, BT8X8_AD_BAR0,Offset);
b=a&d;
BT8X8_WriteWord(hBT8X8, BT8X8_AD_BAR0,Offset,b);
};



void OrDataByte(int Offset,BYTE d)
{
BYTE a;
BYTE b;
a=BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,Offset);
b=a|d;
BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,Offset,b);
};

void OrDataWord(int Offset,unsigned short d)
{
WORD a;
WORD b;
a=BT8X8_ReadWord(hBT8X8, BT8X8_AD_BAR0,Offset);
b=a|d;
BT8X8_WriteWord(hBT8X8, BT8X8_AD_BAR0,Offset,b);
};


BOOL Init_TV_Karte(HWND hWnd)
{

FILE* SettingFile;

char VersionString[255];
char Path[255];
int ret;
unsigned int i;

	strcpy(BTVendorID,"0x109e");
	strcpy(BTDeviceID,"0x036e");

    ret=BT8X8_Open (&hBT8X8, 0x109e, 0x36e,  TRUE, FALSE );
    if ( ret == 0 ) {
            		   strcpy(BTTyp,"BT878");
		               strcpy(VersionString,"BT878 gefunden");
	} else if ( ret == 1 ) {
			MessageBox(hWnd,"Windows NT/95/98 Device-Treiber MDHALDR kann nicht angesprochen werden","Multi-Decode",MB_ICONSTOP|MB_OK);
 			return(FALSE);
	} else if ( ret == 3 ) {
		    MessageBox(hWnd,"PCI-Karte mit Bt878 kann nicht gelockt werden !\nDer Interrupt wird mehrfach verwendet!\n\nKein Zugriff auf die Karte möglich","Multi-Decode",MB_ICONSTOP|MB_OK);
			if ( NT == TRUE ) {
						  if ( OrgDriverName[0] != 0x00 ) LoadDeviceDriver( (const char *)OrgDriverName, Path, &Device_Handle,FALSE  );
			}
 			return(FALSE);
	} else {
          strcpy(BTVendorID,"0x109e");
 	      strcpy(BTDeviceID,"0x0350");
	  	 ret=BT8X8_Open (&hBT8X8, 0x109e, 0x350,  TRUE,FALSE);
    if ( ret == 0 ) {
					strcpy(BTTyp,"BT848");
            		strcpy(VersionString,"BT848 gefunden");
	} else if ( ret == 1 ) {
			MessageBox(hWnd,"Windows NT/95/98 Device-Treiber MDHALDR kann nicht angesprochen werden","Multi-Decode",MB_ICONSTOP|MB_OK);
			if ( NT == TRUE ) {
						  if ( OrgDriverName[0] != 0x00 ) LoadDeviceDriver( (const char *)OrgDriverName, Path, &Device_Handle,FALSE  );
			}
 			return(FALSE);
	} else if ( ret == 3 ) {
		    MessageBox(hWnd,"PCI-Karte mit Bt848 kann nicht gelockt werden !\nDer Interrupt wird mehrfach verwendet!\n\nKein Zugriff auf die Karte möglich","Multi-Decode",MB_ICONSTOP|MB_OK);
			if ( NT == TRUE ) {
						  if ( OrgDriverName[0] != 0x00 ) LoadDeviceDriver( (const char *)OrgDriverName, Path, &Device_Handle,FALSE  );
			}
 			return(FALSE);
	} else {
          strcpy(BTVendorID,"0x109e");
 	      strcpy(BTDeviceID,"0x0351");
	  	 ret=BT8X8_Open (&hBT8X8, 0x109e, 0x351,  TRUE,FALSE);
    if ( ret == 0 ) {
					strcpy(BTTyp,"BT849");
            		strcpy(VersionString,"BT849 gefunden");
	} else if ( ret == 1 ) {
			MessageBox(hWnd,"Windows NT/95/98 Device-Treiber MDHALDR kann nicht angesprochen werden","Multi-Decode",MB_ICONSTOP|MB_OK);
			if ( NT == TRUE ) {
						  if ( OrgDriverName[0] != 0x00 ) LoadDeviceDriver( (const char *)OrgDriverName, Path, &Device_Handle,FALSE  );
			}
 			return(FALSE);
	} else if ( ret == 3 ) {
		    MessageBox(hWnd,"PCI-Karte mit Bt849 kann nicht gelockt werden !\nDer Interrupt wird mehrfach verwendet!\n\nKein Zugriff auf die Karte möglich","Multi-Decode",MB_ICONSTOP|MB_OK);
			if ( NT == TRUE ) {
						  if ( OrgDriverName[0] != 0x00 ) LoadDeviceDriver( (const char *)OrgDriverName, Path, &Device_Handle,FALSE  );
			}
 			return(FALSE);

	} else {
          strcpy(BTVendorID,"0x109e");
 	      strcpy(BTDeviceID,"0x036F");
	  	 ret=BT8X8_Open (&hBT8X8, 0x109e, 0x36F,  TRUE,FALSE);
    if ( ret == 0 ) {
					strcpy(BTTyp,"BT878");
            		strcpy(VersionString,"Anubis BT878");
	} else if ( ret == 1 ) {
			MessageBox(hWnd,"Windows NT/95/98 Device-Treiber MDHALDR kann nicht angesprochen werden","Multi-Decode",MB_ICONSTOP|MB_OK);
			if ( NT == TRUE ) {
						  if ( OrgDriverName[0] != 0x00 ) LoadDeviceDriver( (const char *)OrgDriverName, Path, &Device_Handle,FALSE  );
			}
 			return(FALSE);
	} else if ( ret == 3 ) {
		    MessageBox(hWnd,"PCI-Karte mit Bt849 kann nicht gelockt werden !\nDer Interrupt wird mehrfach verwendet!\n\nKein Zugriff auf die Karte möglich","Multi-Decode",MB_ICONSTOP|MB_OK);
			if ( NT == TRUE ) {
						  if ( OrgDriverName[0] != 0x00 ) LoadDeviceDriver( (const char *)OrgDriverName, Path, &Device_Handle,FALSE  );
			}
 			return(FALSE);
	}
	
	
	
	} 
	}
	}


	if ( ret != 0 ) {
         				strcpy(VersionString,"Kein BT8x8 gefunden");
                    	SetDlgItemText(SplashWnd,IDC_TEXT2,VersionString);
                        return(FALSE);
	}						

    if ( SaveSettings == TRUE ) {
        if ((SettingFile =fopen("Setting.BT","w")) != NULL )  {
           fprintf(SettingFile,"BT848_COLOR_CTL %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_COLOR_CTL));
           fprintf(SettingFile,"BT848_CAP_CTL %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_CAP_CTL));
           fprintf(SettingFile,"BT848_VBI_PACK_SIZE %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_VBI_PACK_SIZE));
           fprintf(SettingFile,"BT848_VBI_PACK_DEL %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_VBI_PACK_DEL));
           fprintf(SettingFile,"BT848_GPIO_DMA_CTL %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_DMA_CTL));
           fprintf(SettingFile,"BT848_IFORM %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_IFORM));

           
		   fprintf(SettingFile,"BT848_E_SCLOOP %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_SCLOOP));
           fprintf(SettingFile,"BT848_O_SCLOOP %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_SCLOOP));
           fprintf(SettingFile,"BT848_ADELAY %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_ADELAY));
           fprintf(SettingFile,"BT848_BDELAY %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_BDELAY));

		   fprintf(SettingFile,"BT848_E_HSCALE_HI %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_HSCALE_HI));
           fprintf(SettingFile,"BT848_E_HSCALE_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_HSCALE_LO));
           fprintf(SettingFile,"BT848_E_VSCALE_HI %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VSCALE_HI));
           fprintf(SettingFile,"BT848_E_VSCALE_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VSCALE_LO));
           fprintf(SettingFile,"BT848_E_HACTIVE_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_HACTIVE_LO));
           fprintf(SettingFile,"BT848_E_HDELAY_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_HDELAY_LO));
           fprintf(SettingFile,"BT848_E_VACTIVE_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VACTIVE_LO));
           fprintf(SettingFile,"BT848_E_VDELAY_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VDELAY_LO));
           fprintf(SettingFile,"BT848_E_CROP %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_CROP));
		   
		   fprintf(SettingFile,"BT848_O_HSCALE_HI %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_HSCALE_HI));
           fprintf(SettingFile,"BT848_O_HSCALE_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_HSCALE_LO));
           fprintf(SettingFile,"BT848_O_VSCALE_HI %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_VSCALE_HI));
           fprintf(SettingFile,"BT848_O_VSCALE_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VSCALE_LO));
           fprintf(SettingFile,"BT848_O_HACTIVE_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_HACTIVE_LO));
           fprintf(SettingFile,"BT848_O_HDELAY_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_HDELAY_LO));
           fprintf(SettingFile,"BT848_O_VACTIVE_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_VACTIVE_LO));
           fprintf(SettingFile,"BT848_O_VDELAY_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_VDELAY_LO));
           fprintf(SettingFile,"BT848_O_CROP %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_CROP));

		   fprintf(SettingFile,"BT848_PLL_F_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_PLL_F_LO));
           fprintf(SettingFile,"BT848_PLL_F_HI %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_PLL_F_HI));
		   fprintf(SettingFile,"BT848_PLL_XCI %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_PLL_XCI));

           fprintf(SettingFile,"BT848_BRIGHT %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_BRIGHT));
           fprintf(SettingFile,"BT848_CONTRAST_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_CONTRAST_LO));
           fprintf(SettingFile,"BT848_SAT_V_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_SAT_V_LO));
           fprintf(SettingFile,"BT848_SAT_U_LO %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_SAT_U_LO));
           fprintf(SettingFile,"BT848_GPIO_OUT_EN %04x\n",BT8X8_ReadWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_OUT_EN));
           fprintf(SettingFile,"BT848_GPIO_OUT_EN_HIBYTE %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_OUT_EN_HIBYTE));

           fprintf(SettingFile,"BT848_GPIO_REG_INP %04x\n",BT8X8_ReadWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_REG_INP));
           fprintf(SettingFile,"BT848_GPIO_REG_INP_HIBYTE %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_REG_INP_HIBYTE));

           fprintf(SettingFile,"BT848_GPIO_DATA %04x\n",BT8X8_ReadWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_DATA));
           fprintf(SettingFile,"BT848_GPIO_DATA_HIBYTE %02x\n",BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_DATA_HIBYTE));
           i=( (BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_OUT_EN_HIBYTE)) << 16 ) + BT8X8_ReadWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_OUT_EN);
           fprintf(SettingFile,"*********************************************\n");
		   fprintf(SettingFile,"Ausgelesene Einträge für Eigenen KartenTyp\n");
           fprintf(SettingFile,"Eintrag für BT848_GPIO_OUT_EN  %9d     ( Schaltwert )\n",i);
           i=( (BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_REG_INP_HIBYTE)) << 16 ) + BT8X8_ReadWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_REG_INP);
           fprintf(SettingFile,"Eintrag für BT848_GPIO_REG_INP %9d     ( Input-Control )\n",i);
           i=((BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_DATA_HIBYTE)) << 16 ) + BT8X8_ReadWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_DATA);
           fprintf(SettingFile,"Eintrag für BT848_GPIO_DATA    %9d     ( Eingangswunsch) \n",i);
           fprintf(SettingFile,"*********************************************\n");
		   fclose(SettingFile);
		}
	}


i=Get_BT_Card_Interrupt();
if (( i >= 0 ) && ( i < 256 )) {
if ( Used_IRQ_List[i] == 1 ) {
	sprintf(Path,"BT8X8 benutzt IRQ 0x%02X <Kein IRQ-Sharing>",i);
    Write_Log(Path);
} else {
sprintf(Path,"***************************************************");
Write_Log(Path);
sprintf(Path,"*                                                 *");
Write_Log(Path);
sprintf(Path,"*        W I C H T I G E R   H I N W E I S        *");
Write_Log(Path);
sprintf(Path,"*  BT8X8 Karte mit IRQ 0x%02X läuft im IRQ-Sharing  *",i);
Write_Log(Path);
sprintf(Path,"* Der IRQ wird insgesamt %2d mal im System genutzt *",Used_IRQ_List[i]);
Write_Log(Path);
sprintf(Path,"*           Das kann Problematisch werden         *");
Write_Log(Path);
sprintf(Path,"*                                                 *");
Write_Log(Path);
sprintf(Path,"***************************************************");
Write_Log(Path);
if ( BT_IRQ_SHARING == FALSE ) {
MessageBox(hWnd,"IRQ-Sharing der BT-Karte erkannt !!\nDas kann Problematisch werden\n\nDiese Meldung wird nicht mehr angezeigt bis der Schalter BT_IRQ_SHARING auf FALSE in der MultiDec.ini gesetzt wird","Multi-Decoder",MB_ICONSTOP|MB_OK); 
BT_IRQ_SHARING=TRUE;
};

}
};

Has_BT=TRUE;

    SetDlgItemText(SplashWnd,IDC_TEXT2,VersionString);
    if (!BT8X8_IntEnable(hBT8X8, NULL)) {
         				strcpy(VersionString,"Interrupt Fail");
                    	SetDlgItemText(SplashWnd,IDC_TEXT3,VersionString);
						return(TRUE);
};

strcpy(VersionString,"Interrupt OK");
SetDlgItemText(SplashWnd,IDC_TEXT3,VersionString);
return(TRUE);
};

int Get_BT_Card_Interrupt( void ) 
{
    int i=0;
	while ( (unsigned)i < hBT8X8->cardReg.Card.dwItems ) {
				if ( hBT8X8->cardReg.Card.Item[i].item == 1 ) {
                   return(hBT8X8->cardReg.Card.Item[i].I.Int.dwInterrupt);
				};
		    i++;
			}
return(-1);
};


BOOL Init_BT_Kernel_Memory( void )
{
char Zeile[255];

	if(!Alloc_DMA(98304,&Risc_dma,DMA_KERNEL_BUFFER_ALLOC))
	{
        Write_Log("BT8X8 Kernel-Memory nicht bekommen");
		MessageBox(hWnd,t("Risc Memory nicht (98 KB physikalisch Linear) nicht verfügbar"),t("Multi-Decode"),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	sprintf(Zeile,"BT8X8 Kernel-Memory OK Log-Mem 0x%08x Phys-Mem 0x%08x",Risc_dma.pUserAddr,Risc_dma.Page[0].pPhysicalAddr);
    Write_Log(Zeile);
	RiscBaseLinear=Risc_dma.pUserAddr;
	RiscBasePhysical=(PHYS) Risc_dma.Page[0].pPhysicalAddr;
	return(TRUE);
};


BOOL Init_Memory(void)
{

	if(!Alloc_DMA(VBI_DATA_SIZE,&Vbi_dma[0],0))
	{
        Write_Log("BT8X8 VideoText Memory Buffer 0 nicht bekommen");
		MessageBox(hWnd,"VideoText Memory ( 77 KB ) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
        Write_Log("BT8X8 VideoText Memory Buffer 0 OK");
	if(!Alloc_DMA(VBI_DATA_SIZE,&Vbi_dma[1],0))
	{
        Write_Log("BT8X8 VideoText Memory Buffer 1 nicht bekommen");
		MessageBox(hWnd,"VideoText Memory ( 77 KB ) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
        Write_Log("BT8X8 VideoText Memory Buffer 1 OK");

	if(!Alloc_DMA(VBI_DATA_SIZE,&Vbi_dma[2],0))
	{
		Write_Log("BT8X8 VideoText Memory Buffer 2 nicht bekommen");
		MessageBox(hWnd,"VideoText Memory ( 77 KB ) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
        Write_Log("BT8X8 VideoText Memory Buffer 2 OK");

		if(!Alloc_DMA(VBI_DATA_SIZE,&Vbi_dma[3],0))
	{
		Write_Log("BT8X8 VideoText Memory Buffer 3 nicht bekommen");
		MessageBox(hWnd,"VideoText Memory ( 77 KB ) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
       Write_Log("BT8X8 VideoText Memory Buffer 3 OK");
 if(!Alloc_DMA(VBI_DATA_SIZE,&Vbi_dma[4],0))
	{
		Write_Log("BT8X8 VideoText Memory Buffer 4 nicht bekommen");
		MessageBox(hWnd,"VideoText Memory ( 77 KB ) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
       Write_Log("BT8X8 VideoText Memory Buffer 4 OK");
		
    if (!Alloc_Display_DMA(1064960,0)) {
		Write_Log("BT8X8 Display Memory 0 nicht bekommen");
		MessageBox(hWnd,"Display Memory (Bild 1 mit 1 MB) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
      }
       Write_Log("BT8X8 Display Memory Buffer 0 OK");
    if (!Alloc_Display_DMA(1064960,1)) {
		Write_Log("BT8X8 Display Memory 1 nicht bekommen");
		MessageBox(hWnd,"Display Memory (Bild 2 mit 1 MB) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
      }
       Write_Log("BT8X8 Display Memory Buffer 1 OK");
    if (!Alloc_Display_DMA(1064960,2)) {
		Write_Log("BT8X8 Display Memory 2 nicht bekommen");
		MessageBox(hWnd,"Display Memory (Bild 3 mit 1 MB) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
      }
       Write_Log("BT8X8 Display Memory Buffer 2 OK");

    if (!Alloc_Display_DMA(1064960,3)) {
		Write_Log("BT8X8 Display Memory 3 nicht bekommen");
		MessageBox(hWnd,"Display Memory (Bild 4 mit 1 MB) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
      }
       Write_Log("BT8X8 Display Memory Buffer 3 OK");
    if (!Alloc_Display_DMA(1064960,4)) {
		Write_Log("BT8X8 Display Memory 4 nicht bekommen");
		MessageBox(hWnd,"Display Memory (Bild 5 mit 1 MB) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
      }
       Write_Log("BT8X8 Display Memory Buffer 4 OK");

	if (!Alloc_DMA(256,&Burst_dma[0],0)) {
        Write_Log("BT8X8 Burst Memory Buffer 0 nicht bekommen");
		MessageBox(hWnd,"Burst Memory (Bild 1 mit 256 Bytes) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
      }
    pBurstLine[0]=Burst_dma[0].pUserAddr;
    Write_Log("BT8X8 Burst Memory Buffer 0 OK");

       if (!Alloc_DMA(256,&Burst_dma[1],0)) {
        Write_Log("BT8X8 Burst Memory Buffer 1 nicht bekommen");
		MessageBox(hWnd,"Burst Memory (Bild 2 mit 256 Bytes) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
      }
    pBurstLine[1]=Burst_dma[1].pUserAddr;
	        Write_Log("BT8X8 Burst Memory Buffer 1 OK");

	if (!Alloc_DMA(256,&Burst_dma[2],0)) {
        Write_Log("BT8X8 Burst Memory Buffer 2 nicht bekommen");
        MessageBox(hWnd,"Burst Memory (Bild 3 mit 256 Bytes) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
            pBurstLine[2]=Burst_dma[2].pUserAddr;
	        Write_Log("BT8X8 Burst Memory Buffer 2 OK");

	if (!Alloc_DMA(256,&Burst_dma[3],0)) {
        Write_Log("BT8X8 Burst Memory Buffer 3 nicht bekommen");
        MessageBox(hWnd,"Burst Memory (Bild 4 mit 256 Bytes) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	        Write_Log("BT8X8 Burst Memory Buffer 3 OK");
            pBurstLine[3]=Burst_dma[3].pUserAddr;

		if (!Alloc_DMA(256,&Burst_dma[4],0)) {
        Write_Log("BT8X8 Burst Memory Buffer 4 nicht bekommen");
        MessageBox(hWnd,"Burst Memory (Bild 5 mit 256 Bytes) können für DMA nicht gelockt werden","Multi-Decode",MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	        Write_Log("BT8X8 Burst Memory Buffer 4 OK");

            pBurstLine[4]=Burst_dma[4].pUserAddr;

return(TRUE);
}


BOOL Init_BT_HardWare(HWND hWnd)
{
int i;

i=0;

    Write_Log("BT8X8 BOOTE-RISC-Programm");
    
	if ( VBI_Event == NULL ) VBI_Event=CreateEvent(NULL,FALSE,FALSE,NULL);
    ResetEvent(VBI_Event);

	m_pRiscJump=RiscBaseLinear;
	m_pRiscVBIOdd=RiscBaseLinear+64;
	m_pRiscVBIEven=m_pRiscVBIOdd+176;
	m_pRiscFrameOdd=m_pRiscVBIEven+176;
	m_pRiscFrameEven=m_pRiscFrameOdd+1664;

	m_pRiscVBIOdd2=m_pRiscFrameEven+1664;
	m_pRiscVBIEven2=m_pRiscVBIOdd2+176;
	m_pRiscFrameOdd2=m_pRiscVBIEven2+176;
	m_pRiscFrameEven2=m_pRiscFrameOdd2+1664;

	m_pRiscVBIOdd3=m_pRiscFrameEven2+1664;
	m_pRiscVBIEven3=m_pRiscVBIOdd3+176;
	m_pRiscFrameOdd3=m_pRiscVBIEven3+176;
	m_pRiscFrameEven3=m_pRiscFrameOdd3+1664;


	m_pRiscVBIOdd4=m_pRiscFrameEven3+1664;
	m_pRiscVBIEven4=m_pRiscVBIOdd4+176;
	m_pRiscFrameOdd4=m_pRiscVBIEven4+176;
	m_pRiscFrameEven4=m_pRiscFrameOdd4+1664;

	m_pRiscVBIOdd5=m_pRiscFrameEven4+1664;
	m_pRiscVBIEven5=m_pRiscVBIOdd5+176;
	m_pRiscFrameOdd5=m_pRiscVBIEven5+176;
	m_pRiscFrameEven5=m_pRiscFrameOdd5+1664;

    
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0, BT848_SRESET,0);
	BT8X8_WriteDword(hBT8X8, BT8X8_AD_BAR0, BT848_RISC_STRT_ADD,RiscLogToPhys(m_pRiscJump+2));
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_COLOR_CTL,0x10);
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_CAP_CTL,0x00);
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_VBI_PACK_SIZE,(VBI_SPL/4)&0xff);
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_VBI_PACK_DEL,(VBI_SPL/4)>>8);
	BT8X8_WriteWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_DMA_CTL,0xfc);
    if ( InitialIFORM != 0 ) {
        BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_IFORM,InitialIFORM);
	} else {
        BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_IFORM,BT848_IFORM_MUX1 | BT848_IFORM_XTAUTO | BT848_IFORM_PAL_BDGHI);
	}
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_ADC,BT848_ADC_RESERVED|BT848_ADC_CRUSH);
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_CONTROL,BT848_CONTROL_LDEC);
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_CONTROL,BT848_CONTROL_LDEC);

    BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_SCLOOP,    0x40);
    BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_SCLOOP,    0x40);
    BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VSCALE_HI, 0x20);
    BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_VSCALE_HI, 0x20);
    BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_VSCALE_LO, 0x00);
    BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VSCALE_LO, 0x00);


    BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_CONTROL,   0x03);
    BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_CONTROL,   0x03);

    BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_TDEC,0x00);            

	BT8X8_WriteDword(hBT8X8, BT8X8_AD_BAR0,BT848_INT_STAT,(DWORD)0x0fffffff);
	BT8X8_WriteDword(hBT8X8, BT8X8_AD_BAR0,BT848_INT_MASK,(1<<23)|BT848_INT_RISCI);

    Set_PLL(INIT_PLL);

	MakeVBITable(VBI_lpf);
	MakeVBITable2(VBI_lpf);
	MakeVBITable3(VBI_lpf);
	MakeVBITable4(VBI_lpf);
	MakeVBITable5(VBI_lpf);
	
	SetBrightness(InitialBrightness);
	SetContrast(InitialContrast);
	SetHue(InitialHue);
	SetSaturationU(InitialSaturationU);  
	SetSaturationV(InitialSaturationV);
	SetVideoSource(VideoSource);
    SetColourFormat(ColourFormat);
	BTOK=TRUE;
    CaptureFlags=0;

    if ( Capture_Video==TRUE ) {
//    	CaptureFlags|=BT848_CAP_CTL_CAPTURE_EVEN|BT848_CAP_CTL_CAPTURE_ODD;
    	CheckMenuItem( GetMenu(hWnd), IDM_VIDEO, MF_CHECKED );
	}
	
	

	if ( Capture_VBI ==TRUE ) {
		Start_VBI();
//        CaptureFlags|=BT848_CAP_CTL_CAPTURE_VBI_EVEN|BT848_CAP_CTL_CAPTURE_VBI_ODD;
		CheckMenuItem( GetMenu(hWnd), IDM_VBI, MF_CHECKED );
        EnableMenuItem(GetMenu(hWnd),IDM_VBI_VT,MF_ENABLED);
		EnableMenuItem(GetMenu(hWnd),IDM_VBI_VC,MF_ENABLED);
		EnableMenuItem(GetMenu(hWnd),IDM_VBI_WINBIS,MF_ENABLED);
		EnableMenuItem(GetMenu(hWnd),IDM_VBI_VD,MF_ENABLED);
		EnableMenuItem(GetMenu(hWnd),IDM_VBI_VPS,MF_ENABLED);
		EnableMenuItem(GetMenu(hWnd),IDM_AUTODETECT,MF_ENABLED);
	};

	return(TRUE);
};

void Reset_BT_HardWare()
{
unsigned char nFlags;
nFlags=0x00;

    MaskDataByte(BT848_CAP_CTL,0,0x0f);
	MaskDataByte(BT848_CAP_CTL,(BYTE)nFlags,(BYTE)0x0f);
	if (nFlags&0x0f) SetDMA(TRUE);
	else SetDMA(FALSE);
	BT8X8_WriteDword(hBT8X8, BT8X8_AD_BAR0,BT848_INT_STAT,(DWORD)0x0fffffff);
    nFlags=CaptureFlags;
    SetRiscJumpsDecode(nFlags);

	MaskDataByte(BT848_CAP_CTL,(BYTE)nFlags,(BYTE)0x0f);
	if (nFlags&0x0f) SetDMA(TRUE);
	else SetDMA(FALSE);
	return;
};
#endif


void Get_Thread_Status()
{
	if ( NextFrame < 0 ) {
              SetEvent(Decode_Event_Stop);
			  ExitThread(0);
			  return;
	};

	if ( Sync_Irq == TRUE ) {
    switch(WaitForSingleObject(Decode_Event, INFINITE)) {
		case  WAIT_OBJECT_0:  ResetEvent(Decode_Event);
	                          return;
		default :             SetEvent(Decode_Event_Stop);
			                  ExitThread(0);
							  return;
	};
	} else {
		Sleep(0);
    switch(WaitForSingleObject(Decode_Event, 12 )) {
	    
	    case  WAIT_TIMEOUT: 
		case  WAIT_OBJECT_0:  ResetEvent(Decode_Event);
	                          return;
        default :             SetEvent(Decode_Event_Stop);
			                  ExitThread(0);
							  return;
  }
	}
}



void Wait_DStatus()
{
	if ( NextFrame < 0 ) {
              SetEvent(Decode_Event_Stop);
			  ExitThread(0);
			  return;
	};

    switch(WaitForSingleObject(Decode_Event, INFINITE)) {
		case  WAIT_OBJECT_0:  ResetEvent(Decode_Event);
	                          return;
		default :             SetEvent(Decode_Event_Stop);
			                  ExitThread(0);
							  return;

	}
};

#ifdef USE_BT8X8

BOOL Alloc_DMA(DWORD dwSize, WD_DMA *dma,int Option )
{
    BZERO(*dma);
	if ( Option == DMA_KERNEL_BUFFER_ALLOC ) dma->pUserAddr=NULL;
	  else dma->pUserAddr=malloc(dwSize);
	dma->dwBytes=dwSize;
    dma->dwOptions=Option;
    WD_DMALock(hBT8X8->hWD,dma);
	if ( dma->hDma == 0 ) {
	                if ( dma->dwOptions != DMA_KERNEL_BUFFER_ALLOC)  free(dma->pUserAddr);
                    dma->pUserAddr=NULL;
					return(FALSE);
	}
	return TRUE;
}

BOOL Alloc_Display_DMA(DWORD dwSize, int NR)
{
	int i;

	i=dwSize/4096;
	i=i+32;
    Display_dma[NR]=calloc(sizeof(WD_DMA)+sizeof(WD_DMA_PAGE)*i,1);
    if (Display_dma[NR]==NULL) return(FALSE);
  	
	Display_dma[NR]->pUserAddr=malloc(dwSize);
	Display_dma[NR]->dwBytes=dwSize;
	Display_dma[NR]->dwPages=i;
    Display_dma[NR]->dwOptions=DMA_LARGE_BUFFER;
    WD_DMALock(hBT8X8->hWD,Display_dma[NR]);
	if ( Display_dma[NR]->hDma == 0 ) {
	                free(Display_dma[NR]->pUserAddr);
                    Display_dma[NR]->pUserAddr=NULL;
					free(Display_dma[NR]);
					Display_dma[NR]=NULL;
					return(FALSE);
	}
	
	pDisplay[NR]=Display_dma[NR]->pUserAddr;

	return TRUE;
}
#endif

BOOL Alloc_Surface_DMA( void)
{

	int i;
    HRESULT      ddrval;
    DDSURFACEDESC  ddsd;
    int dwSize;

	memset(&ddsd,0x00, sizeof(ddsd));
    ddsd.dwSize         = sizeof( ddsd );
 	ddrval = IDirectDraw_GetDisplayMode( lpDD,&ddsd);
    
	dwSize=ddsd.dwHeight*ddsd.dwWidth*2;
	i=dwSize/4096;
	i=i+32;
    Surface_dma=calloc(sizeof(WD_DMA)+sizeof(WD_DMA_PAGE)*i,1);
    if (Surface_dma==NULL) return(FALSE);
	
	Surface_dma->dwBytes=dwSize;
	Surface_dma->dwPages=i;
	Surface_dma->pUserAddr=lpSurface;
    Surface_dma->dwOptions=DMA_LARGE_BUFFER;

	if ( hBT8X8 != NULL ) WD_DMALock(hBT8X8->hWD,Surface_dma);
      if ( hSAA7146 != NULL ) WD_DMALock(hSAA7146->hWD,Surface_dma);

	if ( Surface_dma->hDma == 0 ) {
                    Surface_dma->pUserAddr=NULL;
					free(Surface_dma);
					Surface_dma=NULL;
					return(FALSE);
	}
	return TRUE;
}

BOOL Alloc_Overlay_DMA(int x, int y)
{

	int i;
    int dwSize;
    

    dwSize=x*y*2;
	dwSize+=32768;
	i=dwSize/4096;
	i+=64;
    Overlay_dma=calloc(sizeof(WD_DMA)+sizeof(WD_DMA_PAGE)*i,1);
    if (Overlay_dma==NULL) return(FALSE);
	Overlay_dma->dwBytes=dwSize;
	Overlay_dma->dwPages=i;
	Overlay_dma->pUserAddr=lpOverlay;
    Overlay_dma->dwOptions=DMA_LARGE_BUFFER;
 if ( hBT8X8 != NULL ) WD_DMALock(hBT8X8->hWD,Overlay_dma);
 if ( hSAA7146 != NULL ) WD_DMALock(hSAA7146->hWD,Overlay_dma);

	if ( Overlay_dma->hDma == 0 ) {
                    Overlay_dma->pUserAddr=NULL;
					free(Overlay_dma);
					Overlay_dma=NULL;
					return(FALSE);
	}
	return TRUE;
}


void Free_DMA( WD_DMA *dma )
{
	LPVOID *MemPtr=NULL;
	if ( dma == NULL ) return;
	if ( dma->hDma != 0 ) {
			                if ( dma->dwOptions != DMA_KERNEL_BUFFER_ALLOC)  MemPtr=dma->pUserAddr;
		                    WD_DMAUnlock(hBT8X8->hWD,dma);
                            if ( MemPtr != NULL ) free(MemPtr);
							dma->pUserAddr=NULL;

	}
}

void Free_Display_DMA( int NR )
{
	LPVOID *MemPtr=NULL;
	if ( Display_dma[NR] == NULL ) return;
	if ( Display_dma[NR]->hDma != 0 ) {
			                MemPtr=Display_dma[NR]->pUserAddr;
		                    WD_DMAUnlock(hBT8X8->hWD,Display_dma[NR]);
                            if ( MemPtr != NULL ) free(MemPtr);
							Display_dma[NR]->pUserAddr=NULL;
							free(Display_dma[NR]);
							Display_dma[NR]=NULL;
	}
}

void Free_Surface_DMA( void )
{
	LPVOID *MemPtr=NULL;

	if ( Surface_dma == NULL ) return;
	if ( Surface_dma->hDma != 0 ) {
			                MemPtr=Surface_dma->pUserAddr;
                        	if ( hBT8X8 != NULL ) WD_DMAUnlock(hBT8X8->hWD,Surface_dma);
                                  else if ( hSAA7146 != NULL ) WD_DMAUnlock(hSAA7146->hWD,Surface_dma);
							free(Surface_dma);
							Surface_dma=NULL;
	}
}

void Free_Overlay_DMA( void )
{
	LPVOID *MemPtr=NULL;
	if ( Overlay_dma == NULL ) return;
	if ( Overlay_dma->hDma != 0 ) {
		                    MemPtr=Overlay_dma->pUserAddr;
                        	if ( hBT8X8 != NULL ) WD_DMAUnlock(hBT8X8->hWD,Overlay_dma);
                                  else if ( hSAA7146 != NULL ) WD_DMAUnlock(hSAA7146->hWD,Overlay_dma);
							free(Overlay_dma);
							Overlay_dma=NULL;
	}
}

PHYS GetPhysicalAddress(WD_DMA *dma, LPBYTE pLinear, DWORD dwSizeWanted, DWORD *pdwSizeAvailable)
{
    
	long Offset;
    int i; 
    long sum;
     
	PHYS a;
	Offset=pLinear-(LPBYTE)dma->pUserAddr;
    sum=0; 
	i=0;
	while ( (unsigned)i < dma->dwPages ) {
		if ( sum+dma->Page[i].dwBytes > (unsigned)Offset ) {
            Offset-=sum;
		    a=(PHYS)((LPBYTE)dma->Page[i].pPhysicalAddr+Offset);	
            if ( pdwSizeAvailable != NULL ) *pdwSizeAvailable = dma->Page[i].dwBytes - Offset;
			return(a);
		};
    sum+=dma->Page[i].dwBytes; 
    i++;

	};
	return(0);	
}


#ifdef USE_BT8X8

PHYS RiscLogToPhys(DWORD *pLog)
 { 
return(RiscBasePhysical+(pLog-RiscBaseLinear)*4); 
};



void MakeVBITable(int VBI_Lines)
{
	int i;
	DWORD* po=m_pRiscVBIOdd;
	DWORD* pe=m_pRiscVBIEven;
	PHYS pPhysVBI;
    LPBYTE pVBI = (LPBYTE)Vbi_dma[0].pUserAddr;

    int GotBytesPerLine;
	*(pe++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(pe++)=0;
	for (i=0; i< VBI_Lines; i++) 
	{
        pPhysVBI=GetPhysicalAddress(&Vbi_dma[0],pVBI,VBI_SPL,&GotBytesPerLine);
		if(GotBytesPerLine<VBI_SPL)
		{
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(pe++)=pPhysVBI;
			// Assumes lines aren't >8K long!
			pPhysVBI=GetPhysicalAddress(&Vbi_dma[0],pVBI+GotBytesPerLine,0,0);
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_EOL|(VBI_SPL-GotBytesPerLine); 
			*(pe++)=pPhysVBI;			
		}
		else
		{
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|VBI_SPL; 
			*(pe++)=pPhysVBI;
		}
		pVBI+=2048;
	}
if ( Capture_Video == FALSE ) *(pe++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf5 <<16);
else *(pe++)=BT848_RISC_JUMP;

	*(pe++)=RiscLogToPhys(m_pRiscJump+4);

	*(po++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(po++)=0;
	for (i=VBI_Lines; i<VBI_Lines*2; i++) 
	{
        pPhysVBI=GetPhysicalAddress(&Vbi_dma[0],pVBI,VBI_SPL,&GotBytesPerLine);
		if(GotBytesPerLine<VBI_SPL)
		{
			*(po++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(po++)=pPhysVBI;
			// Assumes lines aren't >8K long!
			pPhysVBI=GetPhysicalAddress(&Vbi_dma[0],pVBI+GotBytesPerLine,0,0);
			*(po++)=BT848_RISC_WRITE|BT848_RISC_EOL|(VBI_SPL-GotBytesPerLine); 
			*(po++)=pPhysVBI;			
		}
		else
		{
			*(po++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|VBI_SPL; 
			*(po++)=pPhysVBI;
		}
		pVBI+=2048;
	}
  	*(po++)=BT848_RISC_JUMP;
	*(po++)=RiscLogToPhys(m_pRiscJump+10);
}


void MakeVBITable2(int VBI_Lines)
{
	int i;
	DWORD* po=m_pRiscVBIOdd2;
	DWORD* pe=m_pRiscVBIEven2;
	PHYS pPhysVBI;
    LPBYTE pVBI = (LPBYTE)Vbi_dma[1].pUserAddr;
    int GotBytesPerLine;
	*(pe++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(pe++)=0;
	for (i=0; i<VBI_Lines; i++) 
	{
        pPhysVBI=GetPhysicalAddress(&Vbi_dma[1],pVBI,VBI_SPL,&GotBytesPerLine);
		if(GotBytesPerLine<VBI_SPL)
		{
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(pe++)=pPhysVBI;
			// Assumes lines aren't >8K long!
			pPhysVBI=GetPhysicalAddress(&Vbi_dma[1],pVBI+GotBytesPerLine,0,0);
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_EOL|(VBI_SPL-GotBytesPerLine); 
			*(pe++)=pPhysVBI;			
		}
		else
		{
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|VBI_SPL; 
			*(pe++)=pPhysVBI;
		}
		pVBI+=2048;
	}
if ( Capture_Video == FALSE ) *(pe++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf1 <<16);
else *(pe++)=BT848_RISC_JUMP;

	*(pe++)=RiscLogToPhys(m_pRiscJump+16);

	*(po++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(po++)=0;
	for (i=VBI_Lines; i<VBI_Lines*2; i++) 
	{
        pPhysVBI=GetPhysicalAddress(&Vbi_dma[1],pVBI,VBI_SPL,&GotBytesPerLine);
		if(GotBytesPerLine<VBI_SPL)
		{
			*(po++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(po++)=pPhysVBI;
			// Assumes lines aren't >8K long!
			pPhysVBI=GetPhysicalAddress(&Vbi_dma[1],pVBI+GotBytesPerLine,0,0);
			*(po++)=BT848_RISC_WRITE|BT848_RISC_EOL|(VBI_SPL-GotBytesPerLine); 
			*(po++)=pPhysVBI;			
		}
		else
		{
			*(po++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|VBI_SPL; 
			*(po++)=pPhysVBI;
		}
		pVBI+=2048;
	}
  	*(po++)=BT848_RISC_JUMP;
	*(po++)=RiscLogToPhys(m_pRiscJump+22);
}

void MakeVBITable3(int VBI_Lines)
{
	int i;
	DWORD* po=m_pRiscVBIOdd3;
	DWORD* pe=m_pRiscVBIEven3;
	PHYS pPhysVBI;
    LPBYTE pVBI = (LPBYTE)Vbi_dma[2].pUserAddr;
    int GotBytesPerLine;
	*(pe++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(pe++)=0;
	for (i=0; i<VBI_Lines; i++) 
	{
        pPhysVBI=GetPhysicalAddress(&Vbi_dma[2],pVBI,VBI_SPL,&GotBytesPerLine);
		if(GotBytesPerLine<VBI_SPL)
		{
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(pe++)=pPhysVBI;
			// Assumes lines aren't >8K long!
			pPhysVBI=GetPhysicalAddress(&Vbi_dma[2],pVBI+GotBytesPerLine,0,0);
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_EOL|(VBI_SPL-GotBytesPerLine); 
			*(pe++)=pPhysVBI;			
		}
		else
		{
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|VBI_SPL; 
			*(pe++)=pPhysVBI;
		}
		pVBI+=2048;
	}
     if ( Capture_Video == FALSE ) *(pe++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf2 <<16);
     else *(pe++)=BT848_RISC_JUMP;
	*(pe++)=RiscLogToPhys(m_pRiscJump+28);

	*(po++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(po++)=0;
	for (i=VBI_Lines; i<VBI_Lines*2; i++) 
	{
        pPhysVBI=GetPhysicalAddress(&Vbi_dma[2],pVBI,VBI_SPL,&GotBytesPerLine);
		if(GotBytesPerLine<VBI_SPL)
		{
			*(po++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(po++)=pPhysVBI;
			// Assumes lines aren't >8K long!
			pPhysVBI=GetPhysicalAddress(&Vbi_dma[2],pVBI+GotBytesPerLine,0,0);
			*(po++)=BT848_RISC_WRITE|BT848_RISC_EOL|(VBI_SPL-GotBytesPerLine); 
			*(po++)=pPhysVBI;			
		}
		else
		{
			*(po++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|VBI_SPL; 
			*(po++)=pPhysVBI;
		}
		pVBI+=2048;
	}
  	*(po++)=BT848_RISC_JUMP;
	*(po++)=RiscLogToPhys(m_pRiscJump+34);
}


void Run_BT_Irq(BOOL State)
{

	if ( Has_BT == TRUE ) {
		if ( State == FALSE ) {
        	Write_Log("BT8X8 Run_BT_Irq(FALSE)");
        	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_CAP_CTL,0x00);
		} else {
        	Write_Log("BT8X8 Run_BT_Irq(TRUE)");
			CaptureFlags=0;
            if ( Capture_Video==TRUE ) CaptureFlags|=BT848_CAP_CTL_CAPTURE_EVEN|BT848_CAP_CTL_CAPTURE_ODD;
	        if ( Capture_VBI ==TRUE ) CaptureFlags|=BT848_CAP_CTL_CAPTURE_VBI_EVEN|BT848_CAP_CTL_CAPTURE_VBI_ODD;
		}; 

	}
};

void MakeVBITable4( int VBI_Lines)
{
	int i;
	DWORD* po=m_pRiscVBIOdd4;
	DWORD* pe=m_pRiscVBIEven4;
	PHYS pPhysVBI;
    LPBYTE pVBI = (LPBYTE)Vbi_dma[3].pUserAddr;
    int GotBytesPerLine;
	*(pe++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(pe++)=0;
	for (i=0; i<VBI_Lines; i++) 
	{
        pPhysVBI=GetPhysicalAddress(&Vbi_dma[3],pVBI,VBI_SPL,&GotBytesPerLine);
		if(GotBytesPerLine<VBI_SPL)
		{
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(pe++)=pPhysVBI;
			// Assumes lines aren't >8K long!
			pPhysVBI=GetPhysicalAddress(&Vbi_dma[3],pVBI+GotBytesPerLine,0,0);
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_EOL|(VBI_SPL-GotBytesPerLine); 
			*(pe++)=pPhysVBI;			
		}
		else
		{
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|VBI_SPL; 
			*(pe++)=pPhysVBI;
		}
		pVBI+=2048;
	}
     if ( Capture_Video == FALSE ) *(pe++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf3 <<16);
     else *(pe++)=BT848_RISC_JUMP;
	*(pe++)=RiscLogToPhys(m_pRiscJump+40);

	*(po++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(po++)=0;
	for (i=VBI_Lines; i<VBI_Lines*2; i++) 
	{
        pPhysVBI=GetPhysicalAddress(&Vbi_dma[3],pVBI,VBI_SPL,&GotBytesPerLine);
		if(GotBytesPerLine<VBI_SPL)
		{
			*(po++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(po++)=pPhysVBI;
			// Assumes lines aren't >8K long!
			pPhysVBI=GetPhysicalAddress(&Vbi_dma[3],pVBI+GotBytesPerLine,0,0);
			*(po++)=BT848_RISC_WRITE|BT848_RISC_EOL|(VBI_SPL-GotBytesPerLine); 
			*(po++)=pPhysVBI;			
		}
		else
		{
			*(po++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|VBI_SPL; 
			*(po++)=pPhysVBI;
		}
		pVBI+=2048;
	}
  	*(po++)=BT848_RISC_JUMP;
	*(po++)=RiscLogToPhys(m_pRiscJump+46);
}

void MakeVBITable5(int VBI_Lines)
{
	int i;
	DWORD* po=m_pRiscVBIOdd5;
	DWORD* pe=m_pRiscVBIEven5;
	PHYS pPhysVBI;
    LPBYTE pVBI = (LPBYTE)Vbi_dma[4].pUserAddr;
    int GotBytesPerLine;
	*(pe++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(pe++)=0;
	for (i=0; i<VBI_Lines; i++) 
	{
        pPhysVBI=GetPhysicalAddress(&Vbi_dma[4],pVBI,VBI_SPL,&GotBytesPerLine);
		if(GotBytesPerLine<VBI_SPL)
		{
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(pe++)=pPhysVBI;
			// Assumes lines aren't >8K long!
			pPhysVBI=GetPhysicalAddress(&Vbi_dma[4],pVBI+GotBytesPerLine,0,0);
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_EOL|(VBI_SPL-GotBytesPerLine); 
			*(pe++)=pPhysVBI;			
		}
		else
		{
			*(pe++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|VBI_SPL; 
			*(pe++)=pPhysVBI;
		}
		pVBI+=2048;
	}
     if ( Capture_Video == FALSE ) *(pe++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf4 <<16);
     else *(pe++)=BT848_RISC_JUMP;
	*(pe++)=RiscLogToPhys(m_pRiscJump+52);

	*(po++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(po++)=0;
	for (i=VBI_Lines; i<VBI_Lines*2; i++) 
	{
        pPhysVBI=GetPhysicalAddress(&Vbi_dma[4],pVBI,VBI_SPL,&GotBytesPerLine);
		if(GotBytesPerLine<VBI_SPL)
		{
			*(po++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(po++)=pPhysVBI;
			// Assumes lines aren't >8K long!
			pPhysVBI=GetPhysicalAddress(&Vbi_dma[4],pVBI+GotBytesPerLine,0,0);
			*(po++)=BT848_RISC_WRITE|BT848_RISC_EOL|(VBI_SPL-GotBytesPerLine); 
			*(po++)=pPhysVBI;			
		}
		else
		{
			*(po++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|VBI_SPL; 
			*(po++)=pPhysVBI;
		}
		pVBI+=2048;
	}
  	*(po++)=BT848_RISC_JUMP;
	*(po++)=RiscLogToPhys(m_pRiscJump+58);
}


void Set_PLL( int PLL )
{
int i;
	if (( PLL != 1 ) && ( PLL != 2 )) {
                BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_TGCTRL,BT848_TGCTRL_TGCKI_NOPLL);
                BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_PLL_XCI,0x00);
		return;
	} else if ( PLL == 1 ) {
         BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_PLL_F_LO,0xf9);
         BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_PLL_F_HI,0xdc);
         BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_PLL_XCI,0x8E);

	} else if ( PLL == 2 ) {
         BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_PLL_F_LO,0x39);
         BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_PLL_F_HI,0xB0);
         BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_PLL_XCI,0x89);
	};

       for(i=0; i<100; i++)
      {

		   if (BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_DSTATUS)&BT848_DSTATUS_CSEL) {
               BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_DSTATUS,0x00);
         }
         else
         {
    	    BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_TGCTRL,BT848_TGCTRL_TGCKI_PLL);
            break;
         };
         Sleep(10);
      };

      BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_WC_UP,0xcf);
      BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_VTOTAL_LO,0x00);
      BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_VTOTAL_HI,0x00);
 	  BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_DVSIF,0x00);
}



void SetDMA(BOOL bState)
{
unsigned short x,y;

x=BT8X8_ReadWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_DMA_CTL);

	if(bState)
		OrDataWord(BT848_GPIO_DMA_CTL,3);
	else
		AndDataWord(BT848_GPIO_DMA_CTL,~3);

y=BT8X8_ReadWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_DMA_CTL);
}
#endif




BOOL SetColourFormat(int nColourFormat)
{
	if(nColourFormat>10) return FALSE;

	if ( Has_DVB == TRUE ) {
		SAA7146_SetColourFormat(nColourFormat);
			ColourFormat=nColourFormat;
	};
#ifdef USE_BT8X8
	if ( Has_BT == TRUE ) {

	if (nColourFormat==BT848_COLOR_FMT_RGB8)
		AndDataByte(BT848_CAP_CTL,~0x10);
	else
		OrDataByte(BT848_CAP_CTL,0x10);
	ColourFormat=nColourFormat;
	SetGeometry(m_wWindowWidth,m_wWindowHeight,TVTYPE,(ColourFormat<<4)|ColourFormat);
	};
#endif
	return TRUE;
}

#ifdef USE_BT8X8
BOOL SetGeoSize(int wWidth, int wHeight)
{

	if(!SetGeometry(wWidth,wHeight,TVTYPE,(ColourFormat<<4)|ColourFormat))
		return FALSE;
	if ( DecodeNagra == TRUE ) {
    if ( NagraFullField ) MakeVideoTableForDisplayNarga();
	    else MakeVideoTableForDisplayNargaHalf();
	} else if (( DecodeVCrypt == TRUE ) || ( DecodeDecoderVCrypt == TRUE ))  {
		MakeVideoTableForDisplayVCrypt();
	} else if (ModeRAWSurface == TRUE )  {
       MakeVideoTableForDirectDraw(Surface_dma,SurfacePitch,Offset_for_WindowOut);
   	   SetWindowText (hwndFPSField,"Sf-Raw");
	} else if (ModeRAWOverlay == TRUE )  {
       MakeVideoTableForDirectDraw(Overlay_dma,OverlayPitch,0);
   	   SetWindowText (hwndFPSField,"Ov-Raw");
	} else {
	   if ( HalfTVMode == FALSE ) MakeVideoTableForDisplay();
	    else MakeVideoTableForDisplayHalf();
 	}


	return TRUE;
}

BOOL SetBrightness(unsigned char bBrightness)
{
	short Low;
    unsigned short *Lowptr;

	Lowptr=(short *)&CurrentLowValue;
	Low=InitialLow;  
	nLevelHigh=135;
	if ( bBrightness < 128 ) {
		Low=Low+bBrightness;
		nLevelHigh=nLevelHigh+bBrightness;
    	nLevelLow      =  45+bBrightness;

	} else {
		Low=Low-(256-bBrightness);
		nLevelHigh=nLevelHigh-(256-bBrightness);
    	nLevelLow      =  45-(256-bBrightness);
	}

	if ( Low < 2 ) Low=0x02;
    if (nLevelHigh > 225) nLevelHigh=225;
	*Lowptr=Low;
    Lowptr++;
	*Lowptr=Low;
    Lowptr++;
	*Lowptr=Low;
    Lowptr++;
	*Lowptr=Low;
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_BRIGHT, bBrightness);
	return TRUE;
}

BOOL SetHue(char bHue)
{
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0, BT848_HUE, bHue);
	return TRUE;
}

BOOL SetContrast(int wContrast)
{
	BYTE bContHi;

	bContHi=(BYTE)(wContrast>>6)&4;
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_CONTRAST_LO,(BYTE)(wContrast&0xff));
	MaskDataByte(BT848_E_CONTROL,bContHi,4);
	MaskDataByte(BT848_O_CONTROL,bContHi,4);
	return TRUE;
}



BOOL SetSaturationU(int wData)
{
	BYTE bDataHi;

	bDataHi=(BYTE)(wData>>7)&2;
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_SAT_U_LO,(BYTE)(wData&0xff));
	MaskDataByte(BT848_E_CONTROL,bDataHi,2);
	MaskDataByte(BT848_O_CONTROL,bDataHi,2);
	return TRUE;
}

BOOL SetSaturationV(int wData)
{
	BYTE bDataHi;

	bDataHi=(BYTE)(wData>>8)&1;
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_SAT_V_LO,(BYTE)(wData&0xff));
	MaskDataByte(BT848_E_CONTROL,bDataHi,1);
	MaskDataByte(BT848_O_CONTROL,bDataHi,1);
	return TRUE;
}


BOOL SetVideoSource(int nInput)
{

// 0= Video_Tuner,
// 1= Video_Ext1,
// 2= Video_Ext2,
// 3= Video_Ext3
	
if (InitialIFORM == 0 )	AndDataByte(BT848_IFORM,~(3<<5));

	switch(nInput)
	{
	case 0:
	case 1:
	case 2:
		AndDataByte(BT848_E_CONTROL,~BT848_CONTROL_COMP);
		AndDataByte(BT848_O_CONTROL,~BT848_CONTROL_COMP);
		break;
	case 3:
		OrDataByte(BT848_E_CONTROL,BT848_CONTROL_COMP);
		OrDataByte(BT848_O_CONTROL,BT848_CONTROL_COMP);
		break;
	};

	VideoSource=nInput;
if (InitialIFORM == 0 )	MaskDataByte(BT848_IFORM,(BYTE)(((nInput+2)&3)<<5), (BYTE)(3<<5));
	return TRUE;
}

void SetGeometryEvenOdd(BOOL bOdd, BYTE bVtc,
					int wHScale, int wVScale,
					int wHActive,int wVActive,
					int wHDelay, int wVDelay,
				    BYTE bCrop)
{
	int nOff=bOdd?0x80:0x00;


	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VTC+nOff,bVtc);
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_HSCALE_HI+nOff,(BYTE)(wHScale>>8));
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_HSCALE_LO+nOff,(BYTE)(wHScale&0xFF));
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VSCALE_HI+nOff,(BYTE)(wVScale>>8));
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VSCALE_LO+nOff,(BYTE)(wVScale&0xFF));
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_HACTIVE_LO+nOff,(BYTE)(wHActive&0xFF));
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_HDELAY_LO+nOff,(BYTE)(wHDelay&0xFF));
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VACTIVE_LO+nOff,(BYTE)(wVActive&0xFF));
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VDELAY_LO+nOff,(BYTE)(wVDelay&0xFF));
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_CROP+nOff,bCrop);
}

BOOL SetGeometry(int width, int height, int Type,int CF)
{
    int vscale, hscale;
	DWORD xsf, sr;
	int hdelay, vdelay;
	int hactive, vactive;
	BYTE crop, vtc;  
	
	
	if (!width || !height)
	        return FALSE;

	m_wWindowHeight=height;
	m_wWindowWidth=width;

	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_ADELAY, TVSettings[Type].bDelayA);
	BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_BDELAY, TVSettings[Type].bDelayB);
if (InitialIFORM == 0 )	MaskDataByte(BT848_IFORM, TVSettings[Type].bIForm, BT848_IFORM_NORM|BT848_IFORM_XTBOTH);

    BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_COLOR_FMT,(BYTE)CF);

	hactive=width;
	vtc = (hactive < 193) ? 2 : ((hactive < 385) ? 1 : 0); 

	xsf = (width*TVSettings[Type].dwXsfNum)/TVSettings[Type].dwXsfDen;
	hscale = (WORD)((TVSettings[Type].wTotalWidth*4096UL)/xsf-4096);
	vdelay=TVSettings[Type].wVDelay;
	hdelay=((hactive*TVSettings[Type].wHDelayx1)/TVSettings[Type].wHActivex1-TVSettings[Type].wCropOffset)&0x3fe;

	sr=(TVSettings[Type].wCropHeight*512)/height-512;
	vscale=(WORD)(0x10000UL-sr)&0x1fff;
	vactive=TVSettings[Type].wCropHeight;
	crop=((hactive>>8)&0x03)|((hdelay>>6)&0x0c)|
	  ((vactive>>4)&0x30)|((vdelay>>2)&0xc0);

	if (( DecodeDecoderVCrypt == TRUE ) || ( DecodeVCrypt == TRUE )) {
          hscale=0x440;
          hdelay = 0x05c;
          crop = 0x33;
		  };

	SetGeometryEvenOdd(FALSE, vtc, hscale, vscale, hactive, vactive,
			hdelay, vdelay, crop);
	SetGeometryEvenOdd(TRUE, vtc, hscale, vscale, hactive, vactive,
			hdelay, vdelay, crop);
	return TRUE;
	}



void Set_BT_Decrypt_Mode()
{
   BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_OFORM,       0x00);
   BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VTC,       0x00);
   BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_VTC,       0x00);
   BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_SCLOOP,    0x40);
   BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_SCLOOP,    0x40);
   BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VDELAY_LO, 0x20);
   BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_VDELAY_LO, 0x20);
   BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VSCALE_HI, 0x20);
   BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_VSCALE_HI, 0x20);
   BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_VSCALE_LO, 0x00);
   BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_VSCALE_LO, 0x00);
   BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_E_CONTROL,   0x20);
   BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_O_CONTROL,   0x20);
}


BOOL SetAudioSource(int nChannel)
{
	BYTE B3;
	unsigned short B1;
    int i;
	char Text[64];

	int SetChannel;

	sprintf(Text,"BT8X8 SetAudioSource %d",nChannel);
	Write_Log(Text);
/*		Audio_Tuner,
		Audio_Radio,
		Audio_External,
		Audio_Internal,
		Audio_Off,
		Audio_On,
		Audio_Mute = 0x80,
		Audio_UnMute = 0x81
*/	
	if ( USECARD==FALSE ) return(TRUE);
	
	if ( CardType == 6 ) {
      B1=ManuellAudio[0];
	  B3=(ManuellAudio[0] >> 16 );
	  BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_OUT_EN_HIBYTE,B3);
	  BT8X8_WriteWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_OUT_EN, B1);      
      B1=ManuellAudio[7];
	  B3=(ManuellAudio[7] >> 16 );
	  BT8X8_WriteWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_REG_INP_HIBYTE, B3);        // 3Bytes
	  BT8X8_WriteWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_REG_INP, B1);        // 3Bytes
      B1=ManuellAudio[nChannel+1];
	  B3=(ManuellAudio[nChannel+1] >> 16 );
	  BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_DATA_HIBYTE, B3);
	  BT8X8_WriteWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_DATA, B1);        // 3Bytes
     return(TRUE);
	}


	SetChannel=nChannel;

	switch (nChannel)
	{
	    case 0x80:
			CAudioSource=(int)((CAudioSource)|(int)0x80);
			nChannel=AudioChannel[CardType][4];
			break;
 		case 0x81:
			CAudioSource=(int)((CAudioSource)&~(int)0x80);
			nChannel=AudioSource;
			break;
		case 0x04:
			nChannel=AudioChannel[CardType][4];
			break;
		case 0x05:
			nChannel=AudioSource;
			break;
		default:
			if ( nChannel == 1 ) {
			 Init_Audio(0x80,0x81);
			}
			CAudioSource=(int)( CAudioSource &( int)0x80);
			CAudioSource=(int)( CAudioSource |  AudioChannel[CardType][nChannel]);
			break;
	}


    /* enable least significant GPIO output byte */
    BT8X8_WriteWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_OUT_EN,0xffff);

i=0;
while ((SetChannel != 4 ) && (i < 20 ) && (!(BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_DSTATUS)&BT848_DSTATUS_HLOC))) {
	i++;
	Sleep(50);
};
    /* if audio mute or not in H-lock, turn audio off */
	if ((nChannel != 1 ) && ((CAudioSource&0x80) || !(BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_DSTATUS)&BT848_DSTATUS_HLOC)))
		nChannel=AudioChannel[CardType][4];


    /* select direct input */
	BT8X8_WriteWord(hBT8X8, BT8X8_AD_BAR0,BT848_GPIO_REG_INP, 0x00);        // 3Bytes
	MaskDataWord(BT848_GPIO_DATA,(unsigned short)(0xfff0|nChannel), (unsigned short)0x0ffff);                // 3Bytes
	return TRUE;
}

BOOL VideoPresent()
{
	return ((BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_DSTATUS)&(BT848_DSTATUS_PRES|BT848_DSTATUS_HLOC))==(BT848_DSTATUS_PRES|BT848_DSTATUS_HLOC))?TRUE:FALSE;
}

#endif



void Set_Capture(int nState)
{
	int nFlags=CaptureFlags;


#ifdef USE_BT8X8
	
	if (( Has_BT == TRUE ) && ( Current_Mode==0 )) {

    MaskDataByte(BT848_CAP_CTL,0,0x0f);

	switch(nState)
	{
	case 0:

		nFlags=CaptureFlags|=BT848_CAP_CTL_CAPTURE_EVEN|BT848_CAP_CTL_CAPTURE_ODD;
		break;
	case 1:
		nFlags=CaptureFlags&=~(BT848_CAP_CTL_CAPTURE_EVEN|BT848_CAP_CTL_CAPTURE_ODD);
		break;
	case 2:
		nFlags=CaptureFlags|=BT848_CAP_CTL_CAPTURE_VBI_EVEN|BT848_CAP_CTL_CAPTURE_VBI_ODD;
		break;
	case 3:
		nFlags=CaptureFlags&=~(BT848_CAP_CTL_CAPTURE_VBI_EVEN|BT848_CAP_CTL_CAPTURE_VBI_ODD);
		break;
	case 4:
		nFlags=0;
		break;
	case 5:
        ResetIrq=0;
		break;
	}




CurrentCapture=nFlags;
	if ( nFlags&(BT848_CAP_CTL_CAPTURE_EVEN|BT848_CAP_CTL_CAPTURE_ODD)) {
	//	Starte den AusgabeThread 
		if ( OutThread == NULL ) Start_Thread();
    };

if ( !( nFlags&(BT848_CAP_CTL_CAPTURE_EVEN|BT848_CAP_CTL_CAPTURE_ODD))) {
	//	Stoppe den Ausgabe Thread
	    Stop_Thread();
	};
    SetRiscJumpsDecode(nFlags);

	MaskDataByte(BT848_CAP_CTL,(BYTE)nFlags,(BYTE)0x0f);
	if (nFlags&0x0f) SetDMA(TRUE);
	else SetDMA(FALSE);



	} else
#endif
    if (( Current_Mode == 1 ) && ( Has_DVB == TRUE )) {
		CurrentCapture=nState;	
		if (nState == 4 ) {
            CurrentCapture=0;
			video_setmode(0);
            Sleep(25);
			Stop_Thread();
		} else if (dvb_front.video_pid != 0x1fff && nState == 5 )  {
		    SAA7146_SetColourFormat(ColourFormat);
			set_up_grabbing();
			video_setmode(1);
			Start_Thread();
            CurrentCapture=5;
		}
	}


}


#ifdef USE_BT8X8

void SetRiscJumpsDecode(int nFlags)
{
    m_pRiscJump=Risc_dma.pUserAddr;

	m_pRiscJump[0]=( DWORD)(BT848_RISC_SYNC|BT848_RISC_RESYNC|BT848_FIFO_STATUS_VRE);
	m_pRiscJump[1]=0;

	m_pRiscJump[2]=BT848_RISC_JUMP;
	if (nFlags&BT848_CAP_CTL_CAPTURE_VBI_EVEN)
		m_pRiscJump[3]=RiscLogToPhys(m_pRiscVBIEven);
	else
		m_pRiscJump[3]=RiscLogToPhys(m_pRiscJump+4);

	m_pRiscJump[4]=BT848_RISC_JUMP;
	if (nFlags&BT848_CAP_CTL_CAPTURE_EVEN)
		m_pRiscJump[5]=RiscLogToPhys(m_pRiscFrameEven);
	else
		m_pRiscJump[5]=RiscLogToPhys(m_pRiscJump+6);

	m_pRiscJump[6]=( DWORD)(BT848_RISC_SYNC|BT848_RISC_RESYNC|BT848_FIFO_STATUS_VRO);
	m_pRiscJump[7]=0;

	m_pRiscJump[8]=BT848_RISC_JUMP;
	if(nFlags&BT848_CAP_CTL_CAPTURE_VBI_ODD)
		m_pRiscJump[9]=RiscLogToPhys(m_pRiscVBIOdd);
	else
		m_pRiscJump[9]=RiscLogToPhys(m_pRiscJump+10);

	m_pRiscJump[10]=BT848_RISC_JUMP|(0xf0 <<16);
	if (nFlags&BT848_CAP_CTL_CAPTURE_ODD)
		m_pRiscJump[11]=RiscLogToPhys(m_pRiscFrameOdd);
	else
		m_pRiscJump[11]=RiscLogToPhys(m_pRiscJump+12);

	// 2. Bild

	m_pRiscJump[12]=( DWORD)(BT848_RISC_SYNC|BT848_RISC_RESYNC|BT848_FIFO_STATUS_VRE);
	m_pRiscJump[13]=0;

	m_pRiscJump[14]=BT848_RISC_JUMP;
	if (nFlags&BT848_CAP_CTL_CAPTURE_VBI_EVEN)
		m_pRiscJump[15]=RiscLogToPhys(m_pRiscVBIEven2);
	else
		m_pRiscJump[15]=RiscLogToPhys(m_pRiscJump+16);

	m_pRiscJump[16]=BT848_RISC_JUMP;
	if (nFlags&BT848_CAP_CTL_CAPTURE_EVEN)
		m_pRiscJump[17]=RiscLogToPhys(m_pRiscFrameEven2);
	else
		m_pRiscJump[17]=RiscLogToPhys(m_pRiscJump+18);

	m_pRiscJump[18]=( DWORD)(BT848_RISC_SYNC|BT848_RISC_RESYNC|BT848_FIFO_STATUS_VRO);
	m_pRiscJump[19]=0;

	m_pRiscJump[20]=BT848_RISC_JUMP;
	if(nFlags&BT848_CAP_CTL_CAPTURE_VBI_ODD)
		m_pRiscJump[21]=RiscLogToPhys(m_pRiscVBIOdd2);
	else
		m_pRiscJump[21]=RiscLogToPhys(m_pRiscJump+22);

	m_pRiscJump[22]=BT848_RISC_JUMP|(0xf0 <<16);
	if (nFlags&BT848_CAP_CTL_CAPTURE_ODD)
		m_pRiscJump[23]=RiscLogToPhys(m_pRiscFrameOdd2);
	else
		m_pRiscJump[23]=RiscLogToPhys(m_pRiscJump+24);
	
	// 3. Bild

	m_pRiscJump[24]=( DWORD)(BT848_RISC_SYNC|BT848_RISC_RESYNC|BT848_FIFO_STATUS_VRE);
	m_pRiscJump[25]=0;

	m_pRiscJump[26]=BT848_RISC_JUMP;
	if (nFlags&BT848_CAP_CTL_CAPTURE_VBI_EVEN)
		m_pRiscJump[27]=RiscLogToPhys(m_pRiscVBIEven3);
	else
		m_pRiscJump[27]=RiscLogToPhys(m_pRiscJump+28);

	m_pRiscJump[28]=BT848_RISC_JUMP;
	if (nFlags&BT848_CAP_CTL_CAPTURE_EVEN)
		m_pRiscJump[29]=RiscLogToPhys(m_pRiscFrameEven3);
	else
		m_pRiscJump[29]=RiscLogToPhys(m_pRiscJump+30);

	m_pRiscJump[30]=( DWORD)(BT848_RISC_SYNC|BT848_RISC_RESYNC|BT848_FIFO_STATUS_VRO);
	m_pRiscJump[31]=0;

	m_pRiscJump[32]=BT848_RISC_JUMP;
	if(nFlags&BT848_CAP_CTL_CAPTURE_VBI_ODD)
		m_pRiscJump[33]=RiscLogToPhys(m_pRiscVBIOdd3);
	else
		m_pRiscJump[33]=RiscLogToPhys(m_pRiscJump+34);

	m_pRiscJump[34]=BT848_RISC_JUMP|(0xf0 <<16);
	if (nFlags&BT848_CAP_CTL_CAPTURE_ODD)
		m_pRiscJump[35]=RiscLogToPhys(m_pRiscFrameOdd3);
	else
		m_pRiscJump[35]=RiscLogToPhys(m_pRiscJump);
	
	// 4. Bild

	m_pRiscJump[36]=( DWORD)(BT848_RISC_SYNC|BT848_RISC_RESYNC|BT848_FIFO_STATUS_VRE);
	m_pRiscJump[37]=0;

	m_pRiscJump[38]=BT848_RISC_JUMP;
	if (nFlags&BT848_CAP_CTL_CAPTURE_VBI_EVEN)
		m_pRiscJump[39]=RiscLogToPhys(m_pRiscVBIEven4);
	else
		m_pRiscJump[39]=RiscLogToPhys(m_pRiscJump+40);

	m_pRiscJump[40]=BT848_RISC_JUMP;
	if (nFlags&BT848_CAP_CTL_CAPTURE_EVEN)
		m_pRiscJump[41]=RiscLogToPhys(m_pRiscFrameEven4);
	else
		m_pRiscJump[41]=RiscLogToPhys(m_pRiscJump+42);

	m_pRiscJump[42]=( DWORD)(BT848_RISC_SYNC|BT848_RISC_RESYNC|BT848_FIFO_STATUS_VRO);
	m_pRiscJump[43]=0;

	m_pRiscJump[44]=BT848_RISC_JUMP;
	if(nFlags&BT848_CAP_CTL_CAPTURE_VBI_ODD)
		m_pRiscJump[45]=RiscLogToPhys(m_pRiscVBIOdd4);
	else
		m_pRiscJump[45]=RiscLogToPhys(m_pRiscJump+46);

	m_pRiscJump[46]=BT848_RISC_JUMP|(0xf0 <<16);
	if (nFlags&BT848_CAP_CTL_CAPTURE_ODD)
		m_pRiscJump[47]=RiscLogToPhys(m_pRiscFrameOdd4);
	else
		m_pRiscJump[47]=RiscLogToPhys(m_pRiscJump);

	// 5. Bild

	m_pRiscJump[48]=( DWORD)(BT848_RISC_SYNC|BT848_RISC_RESYNC|BT848_FIFO_STATUS_VRE);
	m_pRiscJump[49]=0;

	m_pRiscJump[50]=BT848_RISC_JUMP;
	if (nFlags&BT848_CAP_CTL_CAPTURE_VBI_EVEN)
		m_pRiscJump[51]=RiscLogToPhys(m_pRiscVBIEven5);
	else
		m_pRiscJump[51]=RiscLogToPhys(m_pRiscJump+52);

	m_pRiscJump[52]=BT848_RISC_JUMP;
	if (nFlags&BT848_CAP_CTL_CAPTURE_EVEN)
		m_pRiscJump[53]=RiscLogToPhys(m_pRiscFrameEven5);
	else
		m_pRiscJump[53]=RiscLogToPhys(m_pRiscJump+54);

	m_pRiscJump[54]=( DWORD)(BT848_RISC_SYNC|BT848_RISC_RESYNC|BT848_FIFO_STATUS_VRO);
	m_pRiscJump[55]=0;

	m_pRiscJump[56]=BT848_RISC_JUMP;
	if(nFlags&BT848_CAP_CTL_CAPTURE_VBI_ODD)
		m_pRiscJump[57]=RiscLogToPhys(m_pRiscVBIOdd5);
	else
		m_pRiscJump[57]=RiscLogToPhys(m_pRiscJump+58);

	m_pRiscJump[58]=BT848_RISC_JUMP|(0xf0 <<16);
	if (nFlags&BT848_CAP_CTL_CAPTURE_ODD)
		m_pRiscJump[59]=RiscLogToPhys(m_pRiscFrameOdd5);
	else
		m_pRiscJump[59]=RiscLogToPhys(m_pRiscJump);

	BT8X8_WriteDword(hBT8X8, BT8X8_AD_BAR0, BT848_RISC_STRT_ADD,RiscLogToPhys(m_pRiscJump+2));

}




void MakeVideoTableForDirectDraw(WD_DMA *dma,int Pitch,int PosOffset)   
{
	DWORD* ro = m_pRiscFrameOdd;
	DWORD* re = m_pRiscFrameEven;
	DWORD** rp;
	LPBYTE pLinDisplay;

	PHYS pPhysDisplay;
	DWORD GotBytesPerLine;
	DWORD Boffset=BurstOffset*2;
	DWORD BytesPerLine=CurrentX*2;   // Untested
	long Offset=Pitch;
    int i;

//1. Bild

re = m_pRiscFrameEven;
ro = m_pRiscFrameOdd;
Offset=Pitch;
pLinDisplay = dma->pUserAddr;
pLinDisplay+=PosOffset;
  if(Flip)
	{
		pLinDisplay+=Offset*(CurrentY-1);
		Offset=-Offset;
	}

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;

		pPhysDisplay=GetPhysicalAddress(dma,pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(dma,pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+6);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf1 <<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+12);
//2. Bild
re = m_pRiscFrameEven2;
ro = m_pRiscFrameOdd2;
Offset=Pitch;
pLinDisplay = dma->pUserAddr;
pLinDisplay+=PosOffset;
  if(Flip)
	{
		pLinDisplay+=Offset*(CurrentY-1);
		Offset=-Offset;
	}
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;

		pPhysDisplay=GetPhysicalAddress(dma,pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(dma,pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
}

	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+18);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf2 <<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+24); // m_pRiscJump+24
//3. Bild
re = m_pRiscFrameEven3;
ro = m_pRiscFrameOdd3;
Offset=Pitch;
pLinDisplay = dma->pUserAddr;
pLinDisplay+=PosOffset;
  if(Flip)
	{
		pLinDisplay+=Offset*(CurrentY-1);
		Offset=-Offset;
	}
	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		pPhysDisplay=GetPhysicalAddress(dma,pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(dma,pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+30);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf3<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+36);

//4. Bild

re = m_pRiscFrameEven4;
ro = m_pRiscFrameOdd4;
Offset=Pitch;
pLinDisplay = dma->pUserAddr;
pLinDisplay+=PosOffset;
  if(Flip)
	{
		pLinDisplay+=Offset*(CurrentY-1);
		Offset=-Offset;
	}
	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		pPhysDisplay=GetPhysicalAddress(dma,pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(dma,pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+42);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf4<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+48);

//5. Bild

re = m_pRiscFrameEven5;
ro = m_pRiscFrameOdd5;
Offset=Pitch;
pLinDisplay = dma->pUserAddr;
pLinDisplay+=PosOffset;
  if(Flip)
	{
		pLinDisplay+=Offset*(CurrentY-1);
		Offset=-Offset;
	}
	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		pPhysDisplay=GetPhysicalAddress(dma,pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(dma,pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+54);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf5<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump);


}


void MakeVideoTableForDisplay()   
{
	DWORD* ro = m_pRiscFrameOdd;
	DWORD* re = m_pRiscFrameEven;
	DWORD** rp;
	LPBYTE pLinDisplay;

	PHYS pPhysDisplay;
	DWORD GotBytesPerLine;
	DWORD Boffset=BurstOffset*2;
	DWORD BytesPerLine=CurrentX*2;   // Untested
	long Offset=DibPitch;
    int i;

//1. Bild

re = m_pRiscFrameEven;
ro = m_pRiscFrameOdd;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[0];
  if(Flip)
	{
		pLinDisplay+=Offset*(CurrentY-1);
		Offset=-Offset;
	}

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;

		pPhysDisplay=GetPhysicalAddress(Display_dma[0],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[0],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+6);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf1 <<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+12);



//2. Bild

re = m_pRiscFrameEven2;
ro = m_pRiscFrameOdd2;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[1];
  if(Flip)
	{
		pLinDisplay+=Offset*(CurrentY-1);
		Offset=-Offset;
	}

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		
		pPhysDisplay=GetPhysicalAddress(Display_dma[1],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[1],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+18);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf2 <<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+24); // m_pRiscJump+24
 

//3. Bild

re = m_pRiscFrameEven3;
ro = m_pRiscFrameOdd3;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[2];
  if(Flip)
	{
		pLinDisplay+=Offset*(CurrentY-1);
		Offset=-Offset;
	}

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		pPhysDisplay=GetPhysicalAddress(Display_dma[2],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[2],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+30);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf3<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+36);

//4. Bild

re = m_pRiscFrameEven4;
ro = m_pRiscFrameOdd4;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[3];
  if(Flip)
	{
		pLinDisplay+=Offset*(CurrentY-1);
		Offset=-Offset;
	}

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		pPhysDisplay=GetPhysicalAddress(Display_dma[3],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[3],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+42);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf4<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+48);

//5. Bild

re = m_pRiscFrameEven5;
ro = m_pRiscFrameOdd5;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[4];
  if(Flip)
	{
		pLinDisplay+=Offset*(CurrentY-1);
		Offset=-Offset;
	}

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		pPhysDisplay=GetPhysicalAddress(Display_dma[4],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[4],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+54);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf5<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump);


}


void MakeVideoTableForDisplayHalf()   
{
	DWORD* ro = m_pRiscFrameOdd;
	DWORD* re = m_pRiscFrameEven;
	DWORD** rp;
	LPBYTE pLinDisplay;

	PHYS pPhysDisplay;
	DWORD GotBytesPerLine;
	DWORD Boffset=BurstOffset*2;
	DWORD BytesPerLine=CurrentX*2;   // Untested
	long Offset=DibPitch;
    int i;

//1. Bild

re = m_pRiscFrameEven;
ro = m_pRiscFrameOdd;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[0];
  if(Flip)
	{
		pLinDisplay+=Offset*((CurrentY/2)-1);
		Offset=-Offset;
	}

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
  
	for (i=0; i < CurrentY ; i++)
	{
		
		rp=(i&1)?&ro:&re;
        if ( i&1 ) {

		pPhysDisplay=GetPhysicalAddress(Display_dma[0],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[0],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
		} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|BytesPerLine;
		}
		}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+6);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf1 <<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+12);



//2. Bild

re = m_pRiscFrameEven2;
ro = m_pRiscFrameOdd2;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[1];
  if(Flip)
	{
		pLinDisplay+=Offset*((CurrentY/2)-1);
		Offset=-Offset;
	}

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
        if ( i&1 ) {
		
		pPhysDisplay=GetPhysicalAddress(Display_dma[1],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[1],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
		} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|BytesPerLine;
		}
		}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+18);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf2 <<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+24); // m_pRiscJump+24
 

//3. Bild

re = m_pRiscFrameEven3;
ro = m_pRiscFrameOdd3;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[2];
  if(Flip)
	{
		pLinDisplay+=Offset*((CurrentY/2)-1);
		Offset=-Offset;
	}

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
        if ( i&1 ) {

		pPhysDisplay=GetPhysicalAddress(Display_dma[2],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[2],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
		} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|BytesPerLine;
		}
		}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+30);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf3<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+36);

//4. Bild

re = m_pRiscFrameEven4;
ro = m_pRiscFrameOdd4;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[3];
  if(Flip)
	{
		pLinDisplay+=Offset*((CurrentY/2)-1);
		Offset=-Offset;
	}

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
        if ( i&1 ) {

		pPhysDisplay=GetPhysicalAddress(Display_dma[3],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[3],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
		} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|BytesPerLine;
		}
		}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+42);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf4<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+48);

//5. Bild

re = m_pRiscFrameEven5;
ro = m_pRiscFrameOdd5;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[4];
  if(Flip)
	{
		pLinDisplay+=Offset*((CurrentY/2)-1);
		Offset=-Offset;
	}

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
        if ( i&1 ) {

		pPhysDisplay=GetPhysicalAddress(Display_dma[4],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[4],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
		} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|BytesPerLine;
		}
		}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+54);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf5<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump);


}




void MakeVideoTableForDisplayNarga()   
{
	DWORD* ro = m_pRiscFrameOdd;
	DWORD* re = m_pRiscFrameEven;
	DWORD** rp;
	LPBYTE pLinDisplay;
	LPBYTE pLinDisplayNext;

	PHYS pPhysDisplay;
	PHYS pPhysBurst;
	DWORD GotBytesPerLine;
	DWORD Boffset=BurstOffset*2;
	DWORD BytesPerLine=CurrentX*2;   // Untested
	long Offset=DibPitch;
    int i;
    BOOL ROLL = FALSE;

re = m_pRiscFrameEven;
ro = m_pRiscFrameOdd;
ROLL = FALSE;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[0];
pLinDisplayNext = (LPBYTE)pDisplay[1];
pLinDisplay=pLinDisplay+(64*Offset);


	// Because the source of the display data can be anything, we
	// can't assume it's in contiguous linear pages.  Therefore
	// we have to lookup the physical address of each line, and
	// handle breaks where the line crosses an 8K boundary.

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		if ( i == 510 ) {
			pLinDisplay=(LPBYTE)pDisplay[0];
            ROLL=TRUE; 
			pLinDisplayNext=(LPBYTE)pDisplay[1];
            pLinDisplay+=Offset;
		};

		if (( i & 1 ) && ( ROLL == TRUE )) {
    	if ( Boffset > 0 ) *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
		pPhysDisplay=GetPhysicalAddress(Display_dma[1],pLinDisplayNext,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[1],pLinDisplayNext+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		}
		else
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
   		pLinDisplayNext+=(Offset*2);
        pLinDisplay+=Offset;
        } else { 

			if ( Boffset > 0 ) {
			if ( i == 2 ) {
     		pPhysBurst=GetPhysicalAddress(&Burst_dma[0],pBurstLine[0],0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			*((*rp)++)=pPhysBurst;
			} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset;
			}
			}

		pPhysDisplay=GetPhysicalAddress(Display_dma[0],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[0],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
    		pLinDisplay+=Offset;
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
    		pLinDisplay+=Offset;
		}
	};
}
		
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+6);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf1 <<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+12);



//2. Bild

re = m_pRiscFrameEven2;
ro = m_pRiscFrameOdd2;
ROLL = FALSE;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[1];

pLinDisplay=pLinDisplay+(64*Offset);


	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		if ( i == 510 ) {
			pLinDisplay=(LPBYTE)pDisplay[1];
            ROLL=TRUE; 
			pLinDisplayNext=(LPBYTE)pDisplay[2];
            pLinDisplay+=Offset;
		};


		if (( i & 1 ) && ( ROLL == TRUE )) {
		if ( Boffset > 0 ) *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
		pPhysDisplay=GetPhysicalAddress(Display_dma[2],pLinDisplayNext,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[2],pLinDisplayNext+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		}
		else
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    	    pLinDisplay+=Offset;
    		pLinDisplayNext+=Offset*2;

        } else { 
			if ( Boffset > 0 ) {
			if ( i == 2 ) {
     		pPhysBurst=GetPhysicalAddress(&Burst_dma[1],pBurstLine[1],0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			*((*rp)++)=pPhysBurst;
			} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset;
			}
			}
		pPhysDisplay=GetPhysicalAddress(Display_dma[1],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[1],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;

		};
}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+18);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf2 <<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+24); // m_pRiscJump+24
 

//3. Bild

re = m_pRiscFrameEven3;
ro = m_pRiscFrameOdd3;
ROLL = FALSE;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[2];
pLinDisplay=pLinDisplay+(64*Offset);

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		if ( i == 510 ) {
			pLinDisplay=(LPBYTE)pDisplay[2];
            ROLL=TRUE; 
			pLinDisplayNext=(LPBYTE)pDisplay[3];
			pLinDisplay+=Offset;
		};

		if (( i & 1 ) && ( ROLL == TRUE )) {
		if ( Boffset > 0 ) *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
		 pPhysDisplay=GetPhysicalAddress(Display_dma[3],pLinDisplayNext,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
		     pPhysDisplay=GetPhysicalAddress(Display_dma[3],pLinDisplayNext+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		}
		else
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
    		pLinDisplayNext+=(Offset*2);

        } else { 

			if ( Boffset > 0 ) {
			if ( i == 2 ) {
     		pPhysBurst=GetPhysicalAddress(&Burst_dma[2],pBurstLine[2],0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			*((*rp)++)=pPhysBurst;
			} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset;
			}
			}

		pPhysDisplay=GetPhysicalAddress(Display_dma[2],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[2],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
		};
}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+30);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf3<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+36);

//4. Bild

re = m_pRiscFrameEven4;
ro = m_pRiscFrameOdd4;
ROLL = FALSE;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[3];
pLinDisplay=pLinDisplay+(64*Offset);

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		if ( i == 510 ) {
			pLinDisplay=(LPBYTE)pDisplay[3];
            ROLL=TRUE; 
			pLinDisplayNext=(LPBYTE)pDisplay[4];
            pLinDisplay+=Offset;
		};

		if (( i & 1 ) && ( ROLL == TRUE )) {
		if ( Boffset > 0 ) *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
		pPhysDisplay=GetPhysicalAddress(Display_dma[4],pLinDisplayNext,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[4],pLinDisplayNext+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		}
		else
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
    		pLinDisplayNext+=(Offset*2);

        } else { 

			if ( Boffset > 0 ) {
			if ( i == 2 ) {
     		pPhysBurst=GetPhysicalAddress(&Burst_dma[3],pBurstLine[3],0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			*((*rp)++)=pPhysBurst;
			} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset;
			}
			}

		pPhysDisplay=GetPhysicalAddress(Display_dma[3],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[3],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
		};
}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+42);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf4<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+48);

//5. Bild

re = m_pRiscFrameEven5;
ro = m_pRiscFrameOdd5;
ROLL = FALSE;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[4];
pLinDisplay=pLinDisplay+(64*Offset);

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		if ( i == 510 ) {
			pLinDisplay=(LPBYTE)pDisplay[4];
            ROLL=TRUE; 
			pLinDisplayNext=(LPBYTE)pDisplay[0];
            pLinDisplay+=Offset;
		};

		if (( i & 1 ) && ( ROLL == TRUE )) {
		if ( Boffset > 0 ) *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
		pPhysDisplay=GetPhysicalAddress(Display_dma[0],pLinDisplayNext,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[0],pLinDisplayNext+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		}
		else
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
    		pLinDisplayNext+=(Offset*2);

        } else { 

			if ( Boffset > 0 ) {
			if ( i == 2 ) {
     		pPhysBurst=GetPhysicalAddress(&Burst_dma[4],pBurstLine[4],0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			*((*rp)++)=pPhysBurst;
			} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset;
			}
			}

		pPhysDisplay=GetPhysicalAddress(Display_dma[4],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[4],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
		};
}
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+54);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf5<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump);

}

void MakeVideoTableForDisplayNargaHalf()   
{
	DWORD* ro = m_pRiscFrameOdd;
	DWORD* re = m_pRiscFrameEven;
	DWORD** rp;
	LPBYTE pLinDisplay;
	LPBYTE pLinDisplayNext;

	PHYS pPhysDisplay;
	PHYS pPhysBurst;
	DWORD GotBytesPerLine;
	DWORD Boffset=BurstOffset*2;
	DWORD BytesPerLine=CurrentX*2;   // Untested
	long Offset=DibPitch;
    int i;
    BOOL ROLL = FALSE;

re = m_pRiscFrameEven;
ro = m_pRiscFrameOdd;
ROLL = FALSE;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[0];
pLinDisplayNext = (LPBYTE)pDisplay[1];
pLinDisplay=pLinDisplay+(32*Offset);


	// Because the source of the display data can be anything, we
	// can't assume it's in contiguous linear pages.  Therefore
	// we have to lookup the physical address of each line, and
	// handle breaks where the line crosses an 8K boundary.

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		if ( i == 511 ) {
			pLinDisplay=(LPBYTE)pDisplay[0];
            ROLL=TRUE; 
			pLinDisplayNext=(LPBYTE)pDisplay[1];
		};

		if (( i & 1 ) && ( ROLL == TRUE )) {
    	if ( Boffset > 0 ) *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
		pPhysDisplay=GetPhysicalAddress(Display_dma[1],pLinDisplayNext,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[1],pLinDisplayNext+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		}
		else
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
   		pLinDisplayNext+=Offset;
        } else if (!( i & 1 ) && ( ROLL == FALSE )) {
			if ( Boffset > 0 ) {
			if ( i == 2 ) {
     		pPhysBurst=GetPhysicalAddress(&Burst_dma[0],pBurstLine[0],0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			*((*rp)++)=pPhysBurst;
			} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset;
			}
			}

		pPhysDisplay=GetPhysicalAddress(Display_dma[0],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[0],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
		} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|Boffset+BytesPerLine;
		};

	}

	
	*(re++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf1 <<16);
	*(re++)=RiscLogToPhys(m_pRiscJump+6);
	*(ro++)=BT848_RISC_JUMP;
	*(ro++)=RiscLogToPhys(m_pRiscJump+12);



//2. Bild

re = m_pRiscFrameEven2;
ro = m_pRiscFrameOdd2;
ROLL = FALSE;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[1];
pLinDisplay=pLinDisplay+(32*Offset);


	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		if ( i == 511 ) {
			pLinDisplay=(LPBYTE)pDisplay[1];
            ROLL=TRUE; 
			pLinDisplayNext=(LPBYTE)pDisplay[2];
		};


		if (( i & 1 ) && ( ROLL == TRUE )) {
		if ( Boffset > 0 ) *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
		pPhysDisplay=GetPhysicalAddress(Display_dma[2],pLinDisplayNext,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[2],pLinDisplayNext+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		}
		else
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplayNext+=Offset;

        } else if (!( i & 1 ) && ( ROLL == FALSE )) {
			if ( Boffset > 0 ) {
			if ( i == 2 ) {
     		pPhysBurst=GetPhysicalAddress(&Burst_dma[1],pBurstLine[1],0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			*((*rp)++)=pPhysBurst;
			} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset;
			}
			}
		pPhysDisplay=GetPhysicalAddress(Display_dma[1],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[1],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;
		} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|Boffset+BytesPerLine;
		};


	}
	*(re++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf2 <<16);
	*(re++)=RiscLogToPhys(m_pRiscJump+18);
	*(ro++)=BT848_RISC_JUMP;
	*(ro++)=RiscLogToPhys(m_pRiscJump+24); // m_pRiscJump+24
 

//3. Bild

re = m_pRiscFrameEven3;
ro = m_pRiscFrameOdd3;
ROLL = FALSE;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[2];
pLinDisplay=pLinDisplay+(32*Offset);

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		if ( i == 510 ) {
			pLinDisplay=(LPBYTE)pDisplay[2];
            ROLL=TRUE; 
			pLinDisplayNext=(LPBYTE)pDisplay[3];
		};

		if (( i & 1 ) && ( ROLL == TRUE )) {
		if ( Boffset > 0 ) *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
		 pPhysDisplay=GetPhysicalAddress(Display_dma[3],pLinDisplayNext,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
		     pPhysDisplay=GetPhysicalAddress(Display_dma[3],pLinDisplayNext+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		}
		else
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplayNext+=Offset;

        } else if (!( i & 1 ) && ( ROLL == FALSE )) {
			if ( Boffset > 0 ) {
			if ( i == 2 ) {
     		pPhysBurst=GetPhysicalAddress(&Burst_dma[2],pBurstLine[2],0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			*((*rp)++)=pPhysBurst;
			} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset;
			}
			}

		pPhysDisplay=GetPhysicalAddress(Display_dma[2],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[2],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;

		} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|Boffset+BytesPerLine;
		};
	}
	*(re++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf3<<16);
	*(re++)=RiscLogToPhys(m_pRiscJump+30);
	*(ro++)=BT848_RISC_JUMP;
	*(ro++)=RiscLogToPhys(m_pRiscJump+36);

//4. Bild

re = m_pRiscFrameEven4;
ro = m_pRiscFrameOdd4;
ROLL = FALSE;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[3];
pLinDisplay=pLinDisplay+(32*Offset);

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		if ( i == 511 ) {
			pLinDisplay=(LPBYTE)pDisplay[3];
            ROLL=TRUE; 
			pLinDisplayNext=(LPBYTE)pDisplay[4];
		};

		if (( i & 1 ) && ( ROLL == TRUE )) {
		if ( Boffset > 0 ) *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
		pPhysDisplay=GetPhysicalAddress(Display_dma[4],pLinDisplayNext,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[4],pLinDisplayNext+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		}
		else
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplayNext+=Offset;
        } else if (!( i & 1 ) && ( ROLL == FALSE )) {
			if ( Boffset > 0 ) {
			if ( i == 2 ) {
     		pPhysBurst=GetPhysicalAddress(&Burst_dma[3],pBurstLine[3],0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			*((*rp)++)=pPhysBurst;
			} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset;
			}
			}

		pPhysDisplay=GetPhysicalAddress(Display_dma[3],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[3],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;

		} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|Boffset+BytesPerLine;
		};
	}
	*(re++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf4<<16);
	*(re++)=RiscLogToPhys(m_pRiscJump+42);
	*(ro++)=BT848_RISC_JUMP;
	*(ro++)=RiscLogToPhys(m_pRiscJump+48);

//5. Bild

re = m_pRiscFrameEven5;
ro = m_pRiscFrameOdd5;
ROLL = FALSE;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[4];
pLinDisplay=pLinDisplay+(32*Offset);

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < CurrentY ; i++)
	{
		rp=(i&1)?&ro:&re;
		if ( i == 511 ) {
			pLinDisplay=(LPBYTE)pDisplay[4];
            ROLL=TRUE; 
			pLinDisplayNext=(LPBYTE)pDisplay[0];
		};

		if (( i & 1 ) && ( ROLL == TRUE )) {
		if ( Boffset > 0 ) *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
		pPhysDisplay=GetPhysicalAddress(Display_dma[0],pLinDisplayNext,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[0],pLinDisplayNext+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		}
		else
		{
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplayNext+=Offset;
        } else if (!( i & 1 ) && ( ROLL == FALSE )) {
			if ( Boffset > 0 ) {
			if ( i == 2 ) {
     		pPhysBurst=GetPhysicalAddress(&Burst_dma[4],pBurstLine[4],0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			*((*rp)++)=pPhysBurst;
			} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset;
			}
			}
		pPhysDisplay=GetPhysicalAddress(Display_dma[4],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*((*rp)++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[4],pLinDisplay+GotBytesPerLine,0,0);
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*((*rp)++)=pPhysDisplay;			
		} else {
			*((*rp)++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*((*rp)++)=pPhysDisplay;
		}
    		pLinDisplay+=Offset;

		} else {
			 *((*rp)++)=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|Boffset+BytesPerLine;
		};
	}
	*(re++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf5<<16);
	*(re++)=RiscLogToPhys(m_pRiscJump+54);
	*(ro++)=BT848_RISC_JUMP;
	*(ro++)=RiscLogToPhys(m_pRiscJump);

}






void MakeVideoTableForDisplayVCrypt()   
{
	DWORD* ro = m_pRiscFrameOdd;
	DWORD* re = m_pRiscFrameEven;
	LPBYTE pLinDisplay;
	PHYS pPhysDisplay;
	DWORD GotBytesPerLine;
	DWORD BytesPerLine=CurrentX*2;   // Untested
	long Offset=DibPitch;
    int i;
	PHYS pPhysBurst;
	DWORD Boffset=BurstOffset*2;

re = m_pRiscFrameEven;
ro = m_pRiscFrameOdd;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[0];


	// Because the source of the display data can be anything, we
	// can't assume it's in contiguous linear pages.  Therefore
	// we have to lookup the physical address of each line, and
	// handle breaks where the line crosses an 8K boundary.

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < 576 ; i++)
	{
        if ( i&1 ) { 

            if ( Boffset > 0 ) {
				if ( i == 3 ) {
          		pPhysBurst=GetPhysicalAddress(&Burst_dma[0],pBurstLine[0],0,0);
	         		*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			        *(re++)=pPhysBurst;

				} else *(re++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
				}
			
		pPhysDisplay=GetPhysicalAddress(Display_dma[0],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(re++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[0],pLinDisplay+GotBytesPerLine,0,0);
			*(re++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*(re++)=pPhysDisplay;			
    		pLinDisplay+=Offset;
		} else {
			*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*(re++)=pPhysDisplay;
    		pLinDisplay+=Offset;
		}
		} else {
			if ( Boffset > 0 ) *ro++=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine+Boffset;
			 else *ro++=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine;
		}
}
		
	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+6);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf1 <<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+12);



//2. Bild

re = m_pRiscFrameEven2;
ro = m_pRiscFrameOdd2;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[1];

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
  
	for (i=0; i < 576 ; i++)
	{
        if ( i&1 ) { 

            if ( Boffset > 0 ) {
				if ( i == 3 ) {
          		pPhysBurst=GetPhysicalAddress(&Burst_dma[1],pBurstLine[1],0,0);
	         		*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			        *(re++)=pPhysBurst;

				} else *(re++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
				}

			
		pPhysDisplay=GetPhysicalAddress(Display_dma[1],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(re++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[1],pLinDisplay+GotBytesPerLine,0,0);
			*(re++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*(re++)=pPhysDisplay;			
    		pLinDisplay+=Offset;
		} else {
			*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*(re++)=pPhysDisplay;
    		pLinDisplay+=Offset;
		}
		} else {
			if ( Boffset > 0 ) *ro++=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine+Boffset;
			 else *ro++=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine;
		}
}

	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+18);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf2 <<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+24); // m_pRiscJump+24
 

//3. Bild

re = m_pRiscFrameEven3;
ro = m_pRiscFrameOdd3;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[2];

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < 576 ; i++)
	{
        if ( i&1 ) { 

           if ( Boffset > 0 ) {
				if ( i == 3 ) {
          		pPhysBurst=GetPhysicalAddress(&Burst_dma[2],pBurstLine[2],0,0);
	         		*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			        *(re++)=pPhysBurst;

				} else *(re++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
				}

			
		pPhysDisplay=GetPhysicalAddress(Display_dma[2],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(re++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[2],pLinDisplay+GotBytesPerLine,0,0);
			*(re++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*(re++)=pPhysDisplay;			
    		pLinDisplay+=Offset;
		} else {
			*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*(re++)=pPhysDisplay;
    		pLinDisplay+=Offset;
		}
		} else {
			if ( Boffset > 0 ) *ro++=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine+Boffset;
			 else *ro++=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine;
		}
}

	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+30);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf3<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+36);

//4. Bild

re = m_pRiscFrameEven4;
ro = m_pRiscFrameOdd4;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[3];

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < 576 ; i++)
	{
        if ( i&1 ) { 

           if ( Boffset > 0 ) {
				if ( i == 3 ) {
          		pPhysBurst=GetPhysicalAddress(&Burst_dma[3],pBurstLine[3],0,0);
	         		*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			        *(re++)=pPhysBurst;

				} else *(re++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
				}
			
			
			
		pPhysDisplay=GetPhysicalAddress(Display_dma[3],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(re++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[3],pLinDisplay+GotBytesPerLine,0,0);
			*(re++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*(re++)=pPhysDisplay;			
    		pLinDisplay+=Offset;
		} else {
			*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*(re++)=pPhysDisplay;
    		pLinDisplay+=Offset;
		}
		} else {
			if ( Boffset > 0 ) *ro++=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine+Boffset;
			 else *ro++=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine;
		}
}

	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+42);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf4<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump+48);

//5. Bild

re = m_pRiscFrameEven5;
ro = m_pRiscFrameOdd5;
Offset=DibPitch;
pLinDisplay = (LPBYTE)pDisplay[4];

	*(re++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);
	*(re++)=0;
	*(ro++)=(BT848_RISC_SYNC|BT848_FIFO_STATUS_FM1);//|(14<<20));
	*(ro++)=0;
  
	for (i=0; i < 576 ; i++)
	{
        if ( i&1 ) { 
           if ( Boffset > 0 ) {
				if ( i == 3 ) {
          		pPhysBurst=GetPhysicalAddress(&Burst_dma[4],pBurstLine[4],0,0);
	         		*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|Boffset; 
			        *(re++)=pPhysBurst;

				} else *(re++)=BT848_RISC_SKIP|BT848_RISC_SOL|Boffset; 
				}


		pPhysDisplay=GetPhysicalAddress(Display_dma[4],pLinDisplay,BytesPerLine,&GotBytesPerLine);
		if(GotBytesPerLine<BytesPerLine)
		{
 			*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|GotBytesPerLine; 
			*(re++)=pPhysDisplay;
			// Assumes lines aren't >8K long!
			pPhysDisplay=GetPhysicalAddress(Display_dma[4],pLinDisplay+GotBytesPerLine,0,0);
			*(re++)=BT848_RISC_WRITE|BT848_RISC_EOL|(BytesPerLine-GotBytesPerLine); 
			*(re++)=pPhysDisplay;			
    		pLinDisplay+=Offset;
		} else {
			*(re++)=BT848_RISC_WRITE|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine; 
			*(re++)=pPhysDisplay;
    		pLinDisplay+=Offset;
		}
		} else {
			if ( Boffset > 0 ) *ro++=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine+Boffset;
			 else *ro++=BT848_RISC_SKIP|BT848_RISC_SOL|BT848_RISC_EOL|BytesPerLine;
		}
}

	*(re++)=BT848_RISC_JUMP;
	*(re++)=RiscLogToPhys(m_pRiscJump+54);
	*(ro++)=BT848_RISC_JUMP|BT848_RISC_IRQ|(0xf5<<16);
	*(ro++)=RiscLogToPhys(m_pRiscJump);


}

#endif

void ExitTV(void){

	if( lpDD != NULL ){

			if ( lpDDOverlay != NULL ) {
 		        OverlayUpdate(0,0,NULL, DDOVER_HIDE,FALSE);
				IDirectDrawSurface_Release(lpDDOverlay);
			}
            lpDDOverlay=NULL;
            if ( lpDDSurface != NULL ) IDirectDrawSurface_Release(lpDDSurface);
            lpDDSurface=NULL;
			IDirectDraw_Release( lpDD );
            lpDD = NULL;
    }
CloseHandle(VBI_Event);

if (hSAA7146 != NULL ) SAA7146_Close(hSAA7146);
#ifdef USE_BT8X8
if (hBT8X8 != NULL ) BT8X8_Close(hBT8X8);
#endif

Free_Surface_DMA();
Free_Overlay_DMA();

if (hBT8X8 != NULL ) {
WD_Close(hBT8X8->hWD);
free (hBT8X8);
hBT8X8=NULL;
Write_Log("BT8X8 WD Close");
};

if (hSAA7146 != NULL ) {
WD_Close(hSAA7146->hWD);
free (hSAA7146);
hBT8X8=NULL;
Write_Log("SAA7146 WD Close");
};


			if ( NT == TRUE ) {
		Write_Log(">>>>>>>>>> NT UnloadDeviceDriver <<<<<<<<<");
						  UnloadDeviceDriver("MultiDec BT-Treiber",TRUE);
						  UnloadDeviceDriver("MultiDec SAA-Treiber",TRUE);
				          UnloadDeviceDriver("MultiDec HAL-Treiber",TRUE);
						  if ( OrgDriverName[0] != 0x00 ) LoadDeviceDriver( (const char *)OrgDriverName, "", &Device_Handle,FALSE  );
						  if ( OrgDVBDriverName[0] != 0x00 ) LoadDeviceDriver( (const char *)OrgDVBDriverName, "", &Device_Handle,FALSE  );
			} else {
		Write_Log(">>>>>>>>>> 9x UnloadDeviceDriver <<<<<<<<<");
                if ( BT_Plugin_Handle != NULL ) CloseHandle(BT_Plugin_Handle);
                if ( DVB_Plugin_Handle != NULL ) CloseHandle(DVB_Plugin_Handle);
			}

};
    

void  Black_Overlay()
{
   int          nPixel;
   int          nZeile;
   HRESULT      ddrval;
   DDSURFACEDESC  ddsd;

   if ( (lpDD == NULL ) || (lpDDSurface == NULL) || (lpDDOverlay == NULL )) return;

        memset(&ddsd,0x00, sizeof(ddsd));
        ddsd.dwSize         = sizeof( ddsd );

		ddrval = IDirectDrawSurface_Lock(lpDDOverlay, NULL, &ddsd, DDLOCK_WAIT, NULL);

		if (( Current_Mode == 1 ) && ( ModeRAWOverlay == TRUE )) {
		for(nZeile=0; nZeile<(signed)ddsd.dwHeight; nZeile++)
      for(nPixel=0; nPixel<(signed)ddsd.dwWidth*2 ; nPixel+=4)
         *((int*)ddsd.lpSurface+(nZeile*ddsd.lPitch+nPixel)/4) = 0x00800080;
      } else {
		for(nZeile=0; nZeile<(signed)ddsd.dwHeight; nZeile++)
      for(nPixel=0; nPixel<(signed)ddsd.dwWidth*2 ; nPixel+=4)
         *((int*)ddsd.lpSurface+(nZeile*ddsd.lPitch+nPixel)/4) = 0x80008000;
	  }

   ddrval = IDirectDrawSurface_Unlock(lpDDOverlay,  ddsd.lpSurface);
}


BOOL OverlayUpdate(int x,int y,LPRECT pDestRect,DWORD  dwFlags,BOOL ColorKey)
{
   RECT    sSourceRect;
   HRESULT ddrval;
   DDOVERLAYFX DDOverlayFX;
   int i=0;
   if ( (lpDD == NULL ) || (lpDDSurface == NULL) || (lpDDOverlay == NULL )) return(FALSE);
   Free_Overlay_DMA();

   memset(&DDOverlayFX,0x00, sizeof(DDOverlayFX));
   DDOverlayFX.dwSize = sizeof(DDOverlayFX);

   sSourceRect.left   = 0;
   sSourceRect.top    = 0;
   sSourceRect.right  = x-5;			// Changed by BigReaper
   sSourceRect.bottom = y-5;

   if (( x == 0 ) && ( y == 0 )) {
	   ddrval = IDirectDrawSurface_UpdateOverlay(lpDDOverlay,NULL, lpDDSurface, NULL, dwFlags, &DDOverlayFX);
   return(TRUE);
   }

    i=dwFlags;

   if ( (ColorKey ) && (Can_ColorKey)) {
	   dwFlags |=DDOVER_DDFX | DDCKEYCAPS_DESTOVERLAY ;
	   DDOverlayFX.dckDestColorkey.dwColorSpaceHighValue=0;
       DDOverlayFX.dckDestColorkey.dwColorSpaceLowValue=0;
   };


   ddrval = IDirectDrawSurface_UpdateOverlay(lpDDOverlay,&sSourceRect, lpDDSurface, pDestRect, dwFlags, &DDOverlayFX);
   if ( ddrval != DD_OK ) {
   dwFlags=i;
   memset(&DDOverlayFX,0x00, sizeof(DDOverlayFX));
   DDOverlayFX.dwSize = sizeof(DDOverlayFX);
 
   ddrval = IDirectDrawSurface_UpdateOverlay(lpDDOverlay,&sSourceRect, lpDDSurface, pDestRect, dwFlags, &DDOverlayFX);
   if ( ddrval != DD_OK ) {
   Destroy_Overlay();
   CreateOverlay(x,y);
   ddrval = IDirectDrawSurface_UpdateOverlay(lpDDOverlay,&sSourceRect, lpDDSurface, pDestRect, dwFlags, &DDOverlayFX);

   if ( ddrval != DD_OK ) {
			return(FALSE);
   }
   }
   }

   Alloc_Overlay_DMA(x,y);

   return TRUE;
}

BOOL CreateOverlay(int x,int y)
{
    DDSURFACEDESC	ddsd;
    DDPIXELFORMAT PixelFormat;
    HRESULT		ddrval;


      Free_Overlay_DMA();
	  memset(&PixelFormat,0x00, sizeof(PixelFormat));
      PixelFormat.dwSize = sizeof(DDPIXELFORMAT);
      PixelFormat.dwFlags = DDPF_FOURCC;
	  if (( Current_Mode == 1 ) && ( ModeRAWOverlay == TRUE )) {
		  PixelFormat.dwFourCC = MAKEFOURCC('U','Y','V','Y');
      } else {
		  PixelFormat.dwFourCC = MAKEFOURCC('Y','U','Y','2');
	  };
      PixelFormat.dwYUVBitCount = 16;

      memset(&ddsd,0x00, sizeof(ddsd));
      ddsd.dwSize         = sizeof( ddsd );
	  ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
      ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY | DDSCAPS_VIDEOMEMORY | DDSCAPS_LIVEVIDEO;
      	  
	  ddsd.dwWidth = x;
      ddsd.dwHeight = y;
      
	  lpOverlay=NULL;

	  ddsd.ddpfPixelFormat = PixelFormat;
	  if ( IDirectDraw_CreateSurface( lpDD, &ddsd, &lpDDOverlay, NULL ) !=DD_OK ) {
              SetWindowText(hwndTextField,"Erzeugen vom Overlay fehlerhaft");
			  lpDDOverlay=NULL;
              return(FALSE);
		}

	  memset(&ddsd,0x00, sizeof(ddsd));
      ddsd.dwSize         = sizeof( ddsd );

	 ddrval = IDirectDrawSurface_Lock(lpDDOverlay, NULL, &ddsd, DDLOCK_WAIT, NULL);
     OverlayPitch=ddsd.lPitch;
     lpOverlay=ddsd.lpSurface;
	 ddrval = IDirectDrawSurface_Unlock(lpDDOverlay,  ddsd.lpSurface);
     Alloc_Overlay_DMA(x,y);
	  
  return(TRUE);
 };

void Destroy_Overlay()
{
	Free_Overlay_DMA();

	if ( lpDDOverlay != NULL ) {
 		        OverlayUpdate(0,0,NULL, DDOVER_HIDE,FALSE);
				IDirectDrawSurface_Release(lpDDOverlay);
	}
            lpDDOverlay=NULL;

};


BOOL InitTV( HWND hwnd , BOOL Fullscreen, int x , int y )
{
    HRESULT		ddrval;
    LPDIRECTDRAW	lpdd;
    DDSURFACEDESC	ddsd;
    DDCAPS DriverCaps;


     Destroy_Overlay();


	 if( lpDD != NULL )
        {
		    if ( lpDDSurface != NULL ) IDirectDrawSurface_Release(lpDDSurface);
			lpDDSurface=NULL;
	        IDirectDraw_Release( lpDD );
            lpDD = NULL;
    }


	if (lpDD == NULL) 
    {


		if ( DirectDrawCreate(NULL,&lpdd,NULL) != DD_OK ) {
              MessageBox(hWnd,"Erzeugen von DirectDraw fehlerhaft","Multi-Decode",MB_ICONSTOP|MB_OK);
      		  Write_Log("DirectDrawCreate Fehler");
              return(FALSE);
		};
		Write_Log("DirectDrawCreate OK");
		lpDD=lpdd;

		// Kann das Teil Overlay ??
		memset(&DriverCaps,0x00,sizeof(DriverCaps));
	    DriverCaps.dwSize = sizeof(DriverCaps);
		ddrval = IDirectDraw_GetCaps(lpDD,&DriverCaps,NULL);
		Write_Log("DirectDrawCreate Check Overlay");

		if ( ddrval == DD_OK) {
			if (DriverCaps.dwCaps&DDCAPS_OVERLAY ) { 
                	  Write_Log("DirectDrawCreate Overlay gefunden");
					  Can_Overlay=TRUE;   // Juchhu 
					  if (DriverCaps.dwCaps&DDCAPS_OVERLAYSTRETCH) Can_Overlay_Strech=TRUE;
			          if (DriverCaps.dwCKeyCaps&DDCKEYCAPS_DESTOVERLAY ) Can_ColorKey=TRUE;
			} else {
                	  Write_Log("DirectDrawCreate KEIN Overlay gefunden");
			}
			}

	} else { 
		lpdd=lpDD;
	};
	
 

if (Fullscreen)
        {
	  ddrval = IDirectDraw_SetCooperativeLevel( lpdd, hwnd,
                            DDSCL_ALLOWMODEX |
                            DDSCL_EXCLUSIVE |
						    DDSCL_FULLSCREEN );
	if( ddrval != DD_OK ) {
              MessageBox(hWnd,"Erzeugen von SetCooperativeLevel fehlerhaft","Multi-Decode",MB_ICONSTOP|MB_OK);
              return(FALSE);
		}

            ddrval = IDirectDraw_SetDisplayMode( lpdd, x, y, 16);
      
	if( ddrval != DD_OK ) {
              MessageBox(hWnd,"Erzeugen von Fullscreen fehlerhaft","Multi-Decode",MB_ICONSTOP|MB_OK);
              return(FALSE);
		}
 } else {
 
  ddrval = IDirectDraw_SetCooperativeLevel( lpdd, hwnd,DDSCL_NORMAL );

	if( ddrval != DD_OK ) {
              MessageBox(hWnd,"Erzeugen von SetCooperativeLevel fehlerhaft","Multi-Decode",MB_ICONSTOP|MB_OK);
              return(FALSE);
		}
 };
        memset(&ddsd,0x00, sizeof(ddsd));
        ddsd.dwSize         = sizeof( ddsd );
	    ddsd.dwFlags        = DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		if ( IDirectDraw_CreateSurface( lpdd, &ddsd, &lpDDSurface, NULL ) !=DD_OK) {
              MessageBox(hWnd,"Erzeugen der Surface fehlerhaft","Multi-Decode",MB_ICONSTOP|MB_OK);
			  return(FALSE);
			  }

		
     memset(&ddsd,0x00, sizeof(ddsd));
     ddsd.dwSize         = sizeof( ddsd );

	 ddrval = IDirectDrawSurface_Lock(lpDDSurface, NULL, &ddsd, DDLOCK_WAIT, NULL);
	 ddrval = IDirectDrawSurface_Unlock(lpDDSurface,  ddsd.lpSurface);

     SurfacePitch=ddsd.lPitch;
     lpSurface=ddsd.lpSurface;

      memset(&ddsd,0x00, sizeof(ddsd));
      ddsd.dwSize         = sizeof( ddsd );
 	  ddrval = IDirectDraw_GetDisplayMode( lpdd,&ddsd);

	  WTVBPP=0;

	  if(ddsd.ddpfPixelFormat.dwRGBBitCount == 16 ) {
		  if(ddsd.ddpfPixelFormat.dwGBitMask == 0x03e0) WTVBPP=15; 
            else WTVBPP=16;
	  } ;


       if ( WTVBPP != 0) EnableMenuItem(GetMenu(hWnd),IDM_INTERN_SURFACE,MF_ENABLED);
	   else EnableMenuItem(GetMenu(hWnd),IDM_INTERN_SURFACE,MF_GRAYED);


       if ( Can_Overlay) {
		   EnableMenuItem(GetMenu(hWnd),IDM_OVERLAY,MF_ENABLED);
		   if ( Can_Overlay_Strech == FALSE ) {
			   EnableMenuItem(GetMenu(hWnd),IDM_DX_OV_STRECH,MF_GRAYED);
		   }	else {
			   EnableMenuItem(GetMenu(hWnd),IDM_DX_OV_STRECH,MF_ENABLED);
		   }
		   } else {
				EnableMenuItem(GetMenu(hWnd),IDM_OVERLAY,MF_GRAYED);
				EnableMenuItem(GetMenu(hWnd),IDM_DX_OV_STRECH,MF_GRAYED);
			}


		
		
		return TRUE;

} /* InitTV */



#ifdef USE_BT8X8

BOOL Init_Tuner(int TunerNr )
{
unsigned char j;

if ( USETUNER == FALSE ) return(TRUE);

InitializeCriticalSection(&m_cCrit);

j=0xc0;
TunerDeviceRead=Tuners[TunerNr].I2C=j;
TunerDeviceWrite=Tuners[TunerNr].I2C=j;

while ( ( j <= 0xce ) && (I2CBus_AddDevice((BYTE)j) == FALSE ) ) {
j++;
TunerDeviceRead=Tuners[TunerNr].I2C=j;
TunerDeviceWrite=Tuners[TunerNr].I2C=j;
};

if ( j > 0xce ) {
		return(FALSE);
	}
sprintf(TunerStatus,"Tuner I2C-Bus I/O 0x%02x",j);
return(TRUE);
}


void I2C_SetLine(BOOL bCtrl, BOOL bData)
{
	BT8X8_WriteDword(hBT8X8, BT8X8_AD_BAR0,BT848_I2C,(bCtrl<<1)|bData);
    I2CBus_wait(I2C_DELAY);
}

BOOL I2C_GetLine()
{
    return BT8X8_ReadDword(hBT8X8, BT8X8_AD_BAR0,BT848_I2C)&1;
}

BYTE I2C_Read(BYTE nAddr)
{
    DWORD i;
    volatile DWORD stat;
  
 
    BT8X8_WriteDword(hBT8X8, BT8X8_AD_BAR0,BT848_INT_STAT,BT848_INT_I2CDONE);
    BT8X8_WriteDword(hBT8X8, BT8X8_AD_BAR0,BT848_I2C,(nAddr<<24)|I2C_COMMAND);
 
	for (i=0x7fffffff; i; i--)
    {
         stat=BT8X8_ReadDword(hBT8X8, BT8X8_AD_BAR0,BT848_INT_STAT);
         if (stat & BT848_INT_I2CDONE) break;
    }
  
    if(!i) return (BYTE)-1;
    if(!(stat & BT848_INT_RACK)) return (BYTE)-2;

    return (BYTE)((BT8X8_ReadDword(hBT8X8, BT8X8_AD_BAR0,BT848_I2C)>>8)&0xFF);
}

BOOL I2C_Write(BYTE nAddr, BYTE nData1, BYTE nData2, BOOL bSendBoth)
{
   DWORD i;
   DWORD data;
   DWORD stat;
  
    BT8X8_WriteDword(hBT8X8, BT8X8_AD_BAR0,BT848_INT_STAT,BT848_INT_I2CDONE);
  
    data=(nAddr<<24) | (nData1<<16) | I2C_COMMAND;
    if(bSendBoth) data|=(nData2<<8)|BT848_I2C_W3B;
    BT8X8_WriteDword(hBT8X8, BT8X8_AD_BAR0,BT848_I2C,data);  

    for (i=0x7fffffff; i; i--)
    {
         stat=BT8X8_ReadDword(hBT8X8, BT8X8_AD_BAR0,BT848_INT_STAT);
         if (stat & BT848_INT_I2CDONE) break;
    }
  
    if(!i) return FALSE;
    if(!(stat & BT848_INT_RACK)) return FALSE;
 
    return TRUE;
}

//----------------------------------------------------------------

BOOL I2CBus_AddDevice(BYTE I2C_Port)
{
    BOOL bAck;

    // Test whether device exists
	I2CBus_Lock();
	I2CBus_Start();
    bAck=I2CBus_SendByte(I2C_Port,0);
	I2CBus_Stop();
	I2CBus_Unlock();
    if(bAck) return TRUE;
      else return FALSE;
}

BOOL I2CBus_Lock()
{
	EnterCriticalSection(&m_cCrit);
	return TRUE;
}

BOOL I2CBus_Unlock()
{
	LeaveCriticalSection(&m_cCrit);
	return TRUE;
}

void I2CBus_Start()
{
    I2C_SetLine(0,1);
    I2C_SetLine(1,1);
    I2C_SetLine(1,0);
    I2C_SetLine(0,0);
}

void I2CBus_Stop()
{
    I2C_SetLine(0,0);
    I2C_SetLine(1,0);
    I2C_SetLine(1,1);
}

void I2CBus_One()
{
    I2C_SetLine(0,1);
    I2C_SetLine(1,1);
    I2C_SetLine(0,1);
}

void I2CBus_Zero()
{
    I2C_SetLine(0,0);
    I2C_SetLine(1,0);
    I2C_SetLine(0,0);
}

BOOL I2CBus_Ack()
{
    BOOL bAck;
    
    I2C_SetLine(0,1);
    I2C_SetLine(1,1);
    bAck = !I2C_GetLine();
    I2C_SetLine(0,1);
    return bAck;
}

BOOL I2CBus_SendByte(BYTE nData, int nWaitForAck)
{
    I2C_SetLine(0,0);
    nData&0x80?I2CBus_One():I2CBus_Zero();
    nData&0x40?I2CBus_One():I2CBus_Zero();
    nData&0x20?I2CBus_One():I2CBus_Zero();
    nData&0x10?I2CBus_One():I2CBus_Zero();
    nData&0x08?I2CBus_One():I2CBus_Zero();
    nData&0x04?I2CBus_One():I2CBus_Zero();
    nData&0x02?I2CBus_One():I2CBus_Zero();
    nData&0x01?I2CBus_One():I2CBus_Zero();
    if (nWaitForAck) I2CBus_wait(nWaitForAck);
    return I2CBus_Ack();
}

BYTE I2CBus_ReadByte(BOOL bLast)
{
    int i;
    BYTE bData=0;
    
    I2C_SetLine(0,1);
    for (i=7; i>=0; i--)
    {
        I2C_SetLine(1,1);
        if (I2C_GetLine())
            bData |= (1<<i);
        I2C_SetLine(0,1);
    }

    bLast ? I2CBus_One() : I2CBus_Zero();
    return bData;
}


BYTE I2CBus_Read(BYTE nAddr)
{
    BYTE bData;
    
    I2CBus_Start();
    I2CBus_SendByte(nAddr,0);
    bData = I2CBus_ReadByte(TRUE);
    I2CBus_Stop();
    return bData;
}

BOOL I2CBus_Write(BYTE nAddr, BYTE nData1, BYTE nData2, BOOL bSendBoth)
{
    BOOL bAck;

    I2CBus_Start();
    I2CBus_SendByte(nAddr,0);
    bAck=I2CBus_SendByte(nData1,0);
    if(bSendBoth) bAck=I2CBus_SendByte(nData2,0);
    I2CBus_Stop();
    return bAck;
}


void I2CBus_wait(int us)
{
	if ( us > 0 ) { 
		 Sleep(us);
		 return;
	};
Sleep(0);
Sleep(0);
Sleep(0);
Sleep(0);
Sleep(0);
}


/*
 *	Set TSA5522 synthesizer frequency 
 */
BOOL Tuner_SetFrequency(int TunerTyp, int wFrequency)
{
	BYTE config;
	WORD div;
	BOOL bAck;

	if ( USETUNER == FALSE ) return(TRUE);

	if (wFrequency < Tuners[TunerTyp].thresh1) 
		config = Tuners[TunerTyp].VHF_L;
	else if (wFrequency < Tuners[TunerTyp].thresh2) 
		config = Tuners[TunerTyp].VHF_H;
	else
		config = Tuners[TunerTyp].UHF;

	div=wFrequency+Tuners[TunerTyp].IFPCoff;

	div&=0x7fff;
	I2CBus_Lock(); // Lock/wait
	if(!I2CBus_Write((BYTE)TunerDeviceWrite,(BYTE)((div>>8)&0x7f),(BYTE)(div&0xff),TRUE))
	{
		Sleep(1);
		if(!I2CBus_Write((BYTE)TunerDeviceWrite,(BYTE)((div>>8)&0x7f),(BYTE)(div&0xff),TRUE)) {
 		   Sleep(1);
		      if(!I2CBus_Write((BYTE)TunerDeviceWrite,(BYTE)((div>>8)&0x7f),(BYTE)(div&0xff),TRUE)) {
       		    MessageBox(hWnd,"Tuner Device : Fehler beim Schreiben (1)","Multi-Decode",MB_ICONSTOP|MB_OK);
              	I2CBus_Unlock(); // Unlock
                return(FALSE);
			  }
		}
	}
	if(!(bAck=I2CBus_Write(TunerDeviceWrite,Tuners[TunerTyp].config,config,TRUE)))
	{
		Sleep(1);
	if(!(bAck=I2CBus_Write(TunerDeviceWrite,Tuners[TunerTyp].config,config,TRUE)))
	{
		Sleep(1);
	if(!(bAck=I2CBus_Write(TunerDeviceWrite,Tuners[TunerTyp].config,config,TRUE)))
	{
		MessageBox(hWnd,"Tuner Device : Fehler beim Schreiben (2)","Multi-Decode",MB_ICONSTOP|MB_OK);
    }
	}
	}
	I2CBus_Unlock(); // Unlock
	if(!bAck) return FALSE;
	return TRUE;
}

void Load_Country_Settings()
{
 FILE* iniFile;
 char zeile[128];
 char *Pos;
 char *Pos1;
 char *SemmelPos;
 unsigned int i;

  for ( i=0; i< 32; i++ ) {
    Countries[i].Name[0]=0x00;
 };

 
 if ((iniFile=fopen("Channel.lst","r")) == NULL )  {
		MessageBox(hWnd,"Datei Channel.lst nicht gefunden","Multi-Decode",MB_ICONSTOP|MB_OK);
        return;
	 }
 i=0;

 while ( fgets(zeile,sizeof(zeile),iniFile) != NULL ) 
   {
	 if ( i >= 35 ) {
		MessageBox(hWnd,"Datei Channel.lst hat mehr als 35 Ländereinstellungen!\nDer Rest wird ignoriert.","Multi-Decode",MB_ICONSTOP|MB_OK);
	    fclose(iniFile);
		return;
	 }
    SemmelPos=strstr(zeile,";");
	if ( SemmelPos==NULL )	 SemmelPos=strstr(zeile,"\n");
    

    if (((Pos=strstr(zeile,"[")) != 0) && ( SemmelPos > Pos ) && ((Pos1=strstr(zeile,"]")) != 0) ) {

     Pos++;
	 Pos1=&Countries[i].Name[0];
     i++;
     while ( *Pos != ']' ) {
         *(Pos1++)=*(Pos++);
	      *Pos1=0x00;
	 };
}

};

fclose(iniFile);


};

void Load_Country_Specific_Settings(int LPos)
{
int i,j,k;
 FILE* iniFile;
 char zeile[128];
 char txt[128];
 char *Pos;
 char *Pos1;
 char *SemmelPos;
 

 if ((iniFile=fopen("Channel.lst","r")) == NULL )  {
		MessageBox(hWnd,"Datei Channel.lst nicht gefunden","Multi-Decode",MB_ICONSTOP|MB_OK);
        return;
	 }
 i=0;
 k=0;

 
 
 while ( fgets(zeile,sizeof(zeile),iniFile) != NULL ) 
   {

    SemmelPos=strstr(zeile,";");
	if ( SemmelPos==NULL )	 SemmelPos=strstr(zeile,"\n");
    
 
	sprintf(txt,"[%s]",Countries[LPos].Name);

    
	if ( strstr(zeile,txt) != 0 ) {
		 strcpy(Channels.Name,Countries[LPos].Name);
          while ( fgets(zeile,sizeof(zeile),iniFile) != NULL ) {
   SemmelPos=strstr(zeile,";");
	if ( SemmelPos==NULL )	 SemmelPos=strstr(zeile,"\n");

			  if (((Pos=strstr(zeile,"[")) != 0) && ( SemmelPos > Pos ) && ((Pos1=strstr(zeile,"]")) != 0) ) {
				  fclose(iniFile);
				  return;
			  };
 
    if (((Pos=strstr(zeile,"KanalLow=")) != 0) && ( SemmelPos > Pos )) {
			 Pos=Pos+9;
			 j=0;
			  txt[j]=0x00;
			 while ( Pos < SemmelPos) {
			 if ( *Pos != 0x20 ) {
 			txt[j]=*Pos;
			 j++;
			 txt[j]=0x00;
			 }
			 Pos++;
			 }
			 Channels.MinChannel=atoi(txt);
		 }
    else if (((Pos=strstr(zeile,"KanalHigh=")) != 0) && ( SemmelPos > Pos )) {
			 Pos=Pos+10;
			 j=0;
			  txt[j]=0x00;
			 while ( Pos < SemmelPos) {
			 if ( *Pos != 0x20 ) {
			 txt[j]=*Pos;
			 j++;
			 txt[j]=0x00;
			 }
			 Pos++;
			 }
			  Channels.MaxChannel=atoi(txt);
		  } else {
		     Pos=&zeile[0];
			 j=0;
              txt[j]=0x00;
			 while ( Pos < SemmelPos) {
			 if (( *Pos >= '0') && ( *Pos <= '9')) {
			 txt[j]=*Pos;
			 j++;
			 txt[j]=0x00;
			 }
			 Pos++;
			 }
			 if ( txt[0] != 0x00 ) {
				 Channels.freq[k]=atol(txt);
				 Channels.freq[k]=Channels.freq[k]/1000;
			      k++;
			 };
		  }



	}

	}
	}
fclose(iniFile);
return;
}

// Audio

BOOL Init_Audio(BYTE DWrite,BYTE DRead)
{
char Text[128];

AudioDeviceWrite=DWrite;
AudioDeviceRead=DRead;

InitializeCriticalSection(&m_cCrit);

Has_MSP=TRUE;

if(!I2CBus_AddDevice(DRead))
	{
	    Has_MSP=FALSE;
		return(FALSE);
	}

if(!I2CBus_AddDevice(DWrite))
	{
	    Has_MSP=FALSE;
		return(FALSE);
	}

sprintf(MSPStatus,"MSP-Device I2C-Bus I/O 0x80/0x81");
sprintf(Text,"BT8X8 %s",MSPStatus);
Write_Log(Text);

MSP_Reset();
Sleep(4);
MSP_Version();
Sleep(4);
MSP_SetMode( MSPMode );
Sleep(4);
MSP_Set_MajorMinor_Mode(MSPMajorMode,MSPMinorMode );
Sleep(4);
MSP_SetStereo(MSPMajorMode, MSPMinorMode,MSPStereo);

MSPToneControl=TRUE;
Audio_SetVolume(InitialVolume);
Audio_SetBalance(InitialBalance);
Audio_SetSuperBass(InitialSuperBass);
Audio_SetBass(InitialBass);
Audio_SetTreble(InitialTreble);
Audio_SetLoudness(InitialLoudness);
Audio_SetSpatial(InitialSpecial);
Audio_SetEqualizer1(InitialEqualizer1);
Audio_SetEqualizer2(InitialEqualizer2);
Audio_SetEqualizer3(InitialEqualizer3);
Audio_SetEqualizer4(InitialEqualizer4);
Audio_SetEqualizer5(InitialEqualizer5);
return(TRUE);
};

BOOL Audio_WriteMSP(BYTE bSubAddr, int wAddr, int wData)
{
	I2CBus_Lock();
    I2CBus_Start();
    I2CBus_SendByte(AudioDeviceWrite,0);
    I2CBus_SendByte(bSubAddr,0);
    if(bSubAddr!=MSP_CONTROL && bSubAddr!=MSP_TEST)
    {
         I2CBus_SendByte((BYTE)(wAddr>>8),0);
         I2CBus_SendByte((BYTE)(wAddr&0xFF),0);
    }
    I2CBus_SendByte((BYTE)(wData>>8),0);
    I2CBus_SendByte((BYTE)(wData&0xFF),0);
    I2CBus_Stop();
	I2CBus_Unlock();
    return TRUE;
}

WORD Audio_ReadMSP(BYTE bSubAddr, WORD wAddr)
{
     WORD wResult;
     BYTE B0,B1;
     B0=(BYTE)(wAddr>>8);
     B1=(BYTE)(wAddr&0xFF);
	 I2CBus_Lock();
     I2CBus_Start();
     I2CBus_SendByte(AudioDeviceWrite,2);
     I2CBus_SendByte(bSubAddr,0);
     I2CBus_SendByte(B0,0);
     I2CBus_SendByte(B1,0);
     I2CBus_Start();
     if (I2CBus_SendByte(AudioDeviceRead,2)) {
	 B0=I2CBus_ReadByte(0);
	 B1=I2CBus_ReadByte(1);
     wResult=B0 << 8 | B1 ;
	 } else { 
		 wResult =-1;
	 };
     I2CBus_Stop();
	 I2CBus_Unlock();
     return wResult;
};



BOOL Audio_SetVolume(int nVolume)
{
char Text[64];
    sprintf(Text,"BT8X8 Audio_SetVolume %d",nVolume);
	Write_Log(Text);
	Audio_SetToneControl(TRUE);
	if(nVolume<0 || nVolume>1000) return FALSE;
	if ( nVolume > 0 ) InitialVolume=nVolume;
	if(nVolume>0)
		nVolume=MulDiv(nVolume+400,0x7f0,1400);
	WriteDSP(0,nVolume<<4);
	WriteDSP(6,nVolume<<4);

	return TRUE;
}

BOOL Audio_SetBalance(char nBalance)
{
	Audio_SetToneControl(TRUE);
	InitialBalance=nBalance;
	WriteDSP(1,nBalance<<8);
	WriteDSP(0x30,nBalance<<8);
	return TRUE;
}

BOOL Audio_SetBass(char nBass)
{
	Audio_SetToneControl(TRUE);
	if(nBass<-96) return FALSE;
	InitialBass=nBass;
	WriteDSP(2,nBass<<8);
	WriteDSP(0x31,nBass<<8);
	return TRUE;
}

BOOL Audio_SetTreble(char nTreble)
{
	Audio_SetToneControl(TRUE);
	if(nTreble<-96) return FALSE;
	InitialTreble=nTreble;
	WriteDSP(3,nTreble<<8);
	WriteDSP(0x32,nTreble<<8);
	return TRUE;
}

BOOL Audio_SetLoudness(BYTE nLoudness)
{
	Audio_SetToneControl(TRUE);
	if(nLoudness>68) return FALSE;
	InitialLoudness=nLoudness;
	WriteDSP(4,(nLoudness<<8)+(InitialSuperBass?0x4:0));
	WriteDSP(0x33,(nLoudness<<8)+(InitialSuperBass?0x4:0));
	return TRUE;
}

BOOL Audio_SetSuperBass(BOOL bSuperBass)
{
	Audio_SetToneControl(TRUE);
	InitialSuperBass=bSuperBass;
	WriteDSP(4,(InitialLoudness<<8)+(bSuperBass?0x4:0));
	WriteDSP(0x33,(InitialLoudness<<8)+(bSuperBass?0x4:0));
	return TRUE;
}

BOOL Audio_SetSpatial(char nSpatial)
{
	Audio_SetToneControl(TRUE);
	InitialSpatial=nSpatial;
	WriteDSP(0x5,(nSpatial<<8)+0x8); // Mode A, Automatic high pass gain
	return TRUE;
}

BOOL Audio_SetEqualizer1(char nLevel)
{
	Audio_SetToneControl(FALSE);
	if(nLevel<-96 || nLevel>96) return FALSE;
	InitialEqualizer1=nLevel;
	WriteDSP(0x21,nLevel<<8);
	return TRUE;
}

BOOL Audio_SetEqualizer2(char nLevel)
{
	Audio_SetToneControl(FALSE);
	if(nLevel<-96 || nLevel>96) return FALSE;
	InitialEqualizer2=nLevel;
	WriteDSP(0x22,nLevel<<8);
	return TRUE;
}

BOOL Audio_SetEqualizer3(char nLevel)
{
	Audio_SetToneControl(FALSE);
	if(nLevel<-96 || nLevel>96) return FALSE;
	InitialEqualizer3=nLevel;
	WriteDSP(0x23,nLevel<<8);
	return TRUE;
}

BOOL Audio_SetEqualizer4(char nLevel)
{
	Audio_SetToneControl(FALSE);
	if(nLevel<-96 || nLevel>96) return FALSE;
	InitialEqualizer4=nLevel;
	WriteDSP(0x24,nLevel<<8);
	return TRUE;
}

BOOL Audio_SetEqualizer5(char nLevel)
{
	Audio_SetToneControl(FALSE);
	if(nLevel<-96 || nLevel>96) return FALSE;
	InitialEqualizer5=nLevel;
	WriteDSP(0x25,nLevel<<8);
	return TRUE;
}

void Audio_SetToneControl(BOOL  nMode)
{
if ( nMode == MSPToneControl ) return;
MSPToneControl=nMode;
if (MSPToneControl == TRUE ) {
		WriteDSP(2,InitialBass<<8); // Bass
		WriteDSP(0x31,InitialBass<<8);
		WriteDSP(3,InitialTreble<<8); // Treble
		WriteDSP(0x32,InitialTreble<<8);
		WriteDSP(0x21,0); // Eq
		WriteDSP(0x22,0);
		WriteDSP(0x23,0);
		WriteDSP(0x24,0);
		WriteDSP(0x25,0);
		WriteDSP(0x20,0); // Mode control here (need eq=0)
} else {
		WriteDSP(0x20,0xFF<<8); // Mode control here (need eq=0)
		WriteDSP(2,0); // Bass
		WriteDSP(0x31,0);
		WriteDSP(3,0); // Treble
		WriteDSP(0x32,0);
		WriteDSP(0x21,InitialEqualizer1); // Eq
		WriteDSP(0x22,InitialEqualizer2);
		WriteDSP(0x23,InitialEqualizer3);
		WriteDSP(0x24,InitialEqualizer4);
		WriteDSP(0x25,InitialEqualizer5);
	}
}

#endif

BOOL Check_WD_Version( void )
{
    HANDLE hWD;
    WD_VERSION ver;

    hWD = WD_Open();
	WD_Version(hWD,&ver);
    WD_Close (hWD);

	if ( ver.dwVer != 503 ) return(FALSE);
	return(TRUE);
}

int PCI_Scan( void )
{
    WD_PCI_SCAN_CARDS pciScan;
    WD_PCI_SLOT pciSlot;
    WD_PCI_ID   pciId;
    WD_PCI_CARD_INFO pciCardInfo;
    WD_VERSION ver;

    HANDLE hWD;
	int i;
    int j;

	char Zeile[256];
	char Zeile1[256];


    hWD = WD_Open();
    if ( hWD == NULL ) {
	strcpy(Zeile,t("Treiber MDHALL kann nicht geladen werden")); 
    Write_Log(Zeile);
	strcpy(Zeile,"so geht es nicht !!!!!"); 
    Write_Log(Zeile);
	return(-1);
	}
    if (hWD==INVALID_HANDLE_VALUE) {
	strcpy(Zeile,"Treiber MDHALL kann nicht angesprochen werden"); 
    Write_Log(Zeile);
	strcpy(Zeile,"so geht es nicht !!!!!"); 
    Write_Log(Zeile);
	return(-2);
	}

	WD_Version(hWD,&ver);
	sprintf(Zeile,"MDHALL Version %d",ver.dwVer); 
    Write_Log(Zeile);
	BZERO(pciScan);
    
	for ( i=0; i < 256; i++ ) Used_IRQ_List[i]=(unsigned char)0x00;
	pciScan.searchId.dwVendorId = 0;
    pciScan.searchId.dwDeviceId = 0;
    WD_PciScanCards (hWD,&pciScan);
	strcpy(Zeile,"********    Hardware-Scan    ********"); 
    Write_Log(Zeile);

    for (i=0; i<(int)pciScan.dwCards; i++)
    {
		Sleep(0);
		Zeile[0]=0x00;
        Write_Log(Zeile);
        pciId = pciScan.cardId[i];
        pciSlot = pciScan.cardSlot[i];
        sprintf(Zeile,"Bus %d Slot %d Function %d VendorID %04x DeviceID %04x",pciSlot.dwBus, pciSlot.dwSlot, pciSlot.dwFunction, pciId.dwVendorId, pciId.dwDeviceId);
        Write_Log(Zeile);
		Sleep(0);
       
		BZERO(pciCardInfo);
	    pciCardInfo.pciSlot = pciSlot;
        WD_PciGetCardInfo (hWD, &pciCardInfo);

		Zeile[0]=0x00;
       for (j=0; j<(int)pciCardInfo.Card.dwItems; j++)
        {
		Sleep(0);
		   Zeile1[0]=0x00;
           if ( pciCardInfo.Card.Item[j].item == ITEM_INTERRUPT ) {
	        sprintf(Zeile1," Interrupt   : 0x%02x",pciCardInfo.Card.Item[j].I.Int.dwInterrupt);
			if (( pciCardInfo.Card.Item[j].I.Int.dwInterrupt >= 0 ) && 
				( pciCardInfo.Card.Item[j].I.Int.dwInterrupt <= 255 )) Used_IRQ_List[pciCardInfo.Card.Item[j].I.Int.dwInterrupt]++;

		   } else if ( pciCardInfo.Card.Item[j].item == ITEM_MEMORY ) {
            sprintf(Zeile1," Memory-Addr : 0x%08x  - 0x%08x",pciCardInfo.Card.Item[j].I.Mem.dwPhysicalAddr,pciCardInfo.Card.Item[j].I.Mem.dwPhysicalAddr+pciCardInfo.Card.Item[j].I.Mem.dwBytes);
		   } else if ( pciCardInfo.Card.Item[j].item == ITEM_IO ) {
            sprintf(Zeile1," IO-Addr     : 0x%08x  - 0x%08x",pciCardInfo.Card.Item[j].I.IO.dwAddr,pciCardInfo.Card.Item[j].I.IO.dwAddr+pciCardInfo.Card.Item[j].I.IO.dwBytes);
		   };
	     if ( Zeile1[0] != 0x00 ) Write_Log(Zeile1);
	   }
    }
    WD_Close (hWD);

	strcpy(Zeile,"******** Hardware-Scan Ende ********"); 
    Write_Log(Zeile);
	Zeile[0]=0x00;
    Write_Log(Zeile);
return(0);
}


#ifdef USE_BT8X8

int BT8X8_Open (BT8X8_HANDLE *phBT8X8, DWORD dwVendorID, DWORD dwDeviceID,  DWORD options, BOOL Lock)
{
	int Ret = 0;
	char Path[255];
    char SystemDir[255];
    int i;

    BT8X8_HANDLE hBT8X8 = (BT8X8_HANDLE) malloc (sizeof (BT8X8_STRUCT));

    WD_PCI_SCAN_CARDS pciScan;
    WD_PCI_CARD_INFO pciCardInfo;

    BZERO(*hBT8X8);

	sprintf(SystemDir,"Suche Karte VendorID %04x DeviceID %04x ",dwVendorID,dwDeviceID);
    Write_Log(SystemDir);

    hBT8X8->cardReg.hCard = 0;
    hBT8X8->hWD = WD_Open();

    // check if handle valid & version OK
    if (hBT8X8->hWD==INVALID_HANDLE_VALUE)
    {
	    sprintf(SystemDir,"Treiber MDHALL nicht offen ");
        Write_Log(SystemDir);
        //Cannot open WinDriver device
		Ret=1;
        goto Exit;
    }


    BZERO(pciScan);
    pciScan.searchId.dwVendorId = dwVendorID;
    pciScan.searchId.dwDeviceId = dwDeviceID;
    WD_PciScanCards (hBT8X8->hWD, &pciScan);
    if (pciScan.dwCards==0) // Found at least one card
    {
    	sprintf(SystemDir,"Karte VendorID %04x DeviceID %04x nicht gefunden",dwVendorID,dwDeviceID);
        Write_Log(SystemDir);
		Ret=2;
        // error - Cannot find PCI card
        goto Exit;
    }

    BZERO(pciCardInfo);
    pciCardInfo.pciSlot = pciScan.cardSlot[0];
    WD_PciGetCardInfo (hBT8X8->hWD, &pciCardInfo);
    hBT8X8->pciSlot = pciCardInfo.pciSlot;
    hBT8X8->cardReg.Card = pciCardInfo.Card;

    hBT8X8->fUseInt = TRUE;
        // make interrupt resource sharable

    for (i=0; i<(int)hBT8X8->cardReg.Card.dwItems; i++)
        {
            WD_ITEMS *pItem = &hBT8X8->cardReg.Card.Item[i];
            if (pItem->item==ITEM_INTERRUPT) {
				pItem->fNotSharable = Lock;
			};
        }


    hBT8X8->cardReg.fCheckLockOnly = FALSE;
    WD_CardRegister (hBT8X8->hWD, &hBT8X8->cardReg);
    if (hBT8X8->cardReg.hCard==0) {
    	sprintf(SystemDir,"Card Register : Shared Lock nicht möglich ");
        Write_Log(SystemDir);
    	sprintf(SystemDir,"                Kann so nicht weiter machen !!!");
        Write_Log(SystemDir);
		Ret=3;
        goto Exit;
	} else {
    	sprintf(SystemDir,"Card Register : Shared Lock Enabled");
        Write_Log(SystemDir);
	}

    if (!BT8X8_DetectCardElements(hBT8X8))
    {
    	sprintf(SystemDir,"Card Elements : Karten-Elemente können nicht abgefragt werden ");
        Write_Log(SystemDir);
    	sprintf(SystemDir,"                Kann so nicht weiter machen !!!");
        Write_Log(SystemDir);

		Ret=4;
        goto Exit;
    }


	if ( LoadExternBTDriver == TRUE ) {   
    	sprintf(SystemDir,t("BT8X8 Treibererweiterung MDBT soll geladen werden"));
        Write_Log(SystemDir);

	if ( NT == TRUE ) {
		if ( Keep_Driver_Loaded == FALSE ) {
		 GetCurrentDirectory(  sizeof (Path),Path );
		 strcat(Path,"\\MDBT.SYS");
    	sprintf(SystemDir,t("BT8X8 Treibererweiterung : %s "),Path);
        Write_Log(SystemDir);
		 if ( ! LoadDeviceDriver( t("MultiDec BT-Treiber"), Path, &BT_Plugin_Handle,TRUE  ) )  {
    	        sprintf(SystemDir,t("BT8X8 NT-Treibererweiterung kann nicht geladen werden"));
                Write_Log(SystemDir);
		 };
		};
	} else {
		GetSystemDirectory(SystemDir,sizeof(SystemDir));
		strcat(SystemDir,"\\vmm32\\MDBT.VXD");
		if ( CopyFile("MDBT.vxd",SystemDir,FALSE) == FALSE ) {
    	        sprintf(SystemDir,"BT8X8 Windoof 9X-Treibererweiterung MDBT.vxd kann nicht kopiert werden");
		} else {
    	        sprintf(SystemDir,"BT8X8 Windoof 9X-Treibererweiterung MDBT.vxd bereit");
		}
		Write_Log(SystemDir);
	};
	} else {
    	sprintf(SystemDir,t("BT8X8 Treibererweiterung MDBT soll nicht geladen werden"));
        Write_Log(SystemDir);
	}



    // Open finished OK
    *phBT8X8 = hBT8X8;
    return Ret;

Exit:

	sprintf(SystemDir,"BT8x8_Open Karte VendorID %04x DeviceID %04x mit FehlerCode %d ",dwVendorID,dwDeviceID,Ret);
    Write_Log(SystemDir);
    
	// Error during Open
    if (hBT8X8->cardReg.hCard) 
        WD_CardUnregister(hBT8X8->hWD, &hBT8X8->cardReg);
    if (hBT8X8->hWD!=INVALID_HANDLE_VALUE)
        WD_Close(hBT8X8->hWD);
    free (hBT8X8);
    return Ret;
}


BOOL BT8X8_DetectCardElements(BT8X8_HANDLE hBT8X8)
{
    DWORD i;
    DWORD ad_sp;

    BZERO(hBT8X8->Int);
    BZERO(hBT8X8->addrDesc);

    for (i=0; i<hBT8X8->cardReg.Card.dwItems; i++)
    {
        WD_ITEMS *pItem = &hBT8X8->cardReg.Card.Item[i];

        switch (pItem->item)
        {
        case ITEM_MEMORY:
        case ITEM_IO:
            {
                DWORD dwBytes;
                DWORD dwPhysAddr;
                BOOL fIsMemory;
                if (pItem->item==ITEM_MEMORY)
                {
                    dwBytes = pItem->I.Mem.dwBytes;
                    dwPhysAddr = pItem->I.Mem.dwPhysicalAddr;
                    fIsMemory = TRUE;
                }
                else 
                {
                    dwBytes = pItem->I.IO.dwBytes;
                    dwPhysAddr = pItem->I.IO.dwAddr;
                    fIsMemory = FALSE;
                }

                for (ad_sp=0; ad_sp<BT8X8_ITEMS; ad_sp++)
                {
                    DWORD dwPCIAddr;
                    DWORD dwPCIReg;

                    if (BT8X8_IsAddrSpaceActive(hBT8X8, ad_sp)) continue;
                    if (ad_sp<BT8X8_AD_EPROM) dwPCIReg = PCI_BAR0 + 4*ad_sp;
                    else dwPCIReg = PCI_ERBAR;
                    dwPCIAddr = BT8X8_ReadPCIReg(hBT8X8, dwPCIReg);
                    if (dwPCIAddr & 1)
                    {
                        if (fIsMemory) continue;
                        dwPCIAddr &= ~(0x3);
                    }
                    else
                    {
                        if (!fIsMemory) continue;
                        dwPCIAddr &= ~(0xf);
                    }
                    if (dwPCIAddr==dwPhysAddr)
                        break;
                }
                if (ad_sp<BT8X8_ITEMS)
                {
                    DWORD j;
                    hBT8X8->addrDesc[ad_sp].fActive = TRUE;
                    hBT8X8->addrDesc[ad_sp].index = i;
                    hBT8X8->addrDesc[ad_sp].fIsMemory = fIsMemory;
                    hBT8X8->addrDesc[ad_sp].dwMask = 0;
                    for (j=1; j<dwBytes && j!=0x80000000; j *= 2)
                    {
                        hBT8X8->addrDesc[ad_sp].dwMask = 
                            (hBT8X8->addrDesc[ad_sp].dwMask << 1) | 1;
                    }
                }
            }
            break;
        case ITEM_INTERRUPT:
            if (hBT8X8->Int.Int.hInterrupt) return FALSE;
            hBT8X8->Int.Int.hInterrupt = pItem->I.Int.hInterrupt;
            break;
        }
    }

    // check that all the items needed were found
    // check if interrupt found
    if (hBT8X8->fUseInt && !hBT8X8->Int.Int.hInterrupt) 
    {
        return FALSE;
    }

    // check that at least one memory space was found
    for (i = 0; i<BT8X8_ITEMS; i++)
        if (BT8X8_IsAddrSpaceActive(hBT8X8, i)) break;
    if (i==BT8X8_ITEMS) return FALSE;

    return TRUE;
}

void Reload_BT_Driver( void )
{
	char Path[256];
	char SystemDir[256];

		 Keep_Driver_Loaded = FALSE;
 	     UnloadDeviceDriver("MultiDec BT-Treiber",TRUE);

	if ( NT == TRUE ) {
		 GetCurrentDirectory(  sizeof (Path),Path );
		 strcat(Path,"\\MDBT.SYS");
    	sprintf(SystemDir,"BT8X8 Treibererweiterung : %s ",Path);
        Write_Log(SystemDir);
		 if ( ! LoadDeviceDriver( "MultiDec BT-Treiber", Path, &BT_Plugin_Handle,TRUE  ) )  {
    	        sprintf(SystemDir,t("BT8X8 NT-Treibererweiterung kann nicht geladen werden"));
                Write_Log(SystemDir);
		 };
	} else {
		GetSystemDirectory(SystemDir,sizeof(SystemDir));
		strcat(SystemDir,"\\vmm32\\MDBT.VXD");
		if ( CopyFile("MDBT.vxd",SystemDir,FALSE) == FALSE ) {
    	        sprintf(SystemDir,"BT8X8 Windoof 9X-Treibererweiterung MDBT.vxd kann nicht kopiert werden");
		} else {
    	        sprintf(SystemDir,"BT8X8 Windoof 9X-Treibererweiterung MDBT.vxd bereit");
		}
		Write_Log(SystemDir);
	};

};


void BT_Close()
{
if (hBT8X8 != NULL ) {
Write_Log("BT8X8 BT8x8_Close gestartet");
BT8X8_Close(hBT8X8);
WD_Close(hBT8X8->hWD);
free (hBT8X8);
hBT8X8=NULL;
Write_Log("BT8X8 WD Close");
}
};


void BT8X8_Close(BT8X8_HANDLE hBT8X8)
{
SetAudioSource(4);
Audio_SetVolume(0);
	
	 BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0, BT848_SRESET,0);
	Write_Log("BT8X8 IRQ aus");
    // disable interrupts
    if (BT8X8_IntIsEnabled(hBT8X8))
        BT8X8_IntDisable(hBT8X8);

	Write_Log("BT8X8 Treiber beenden");
	if ( ExternBTDriver == TRUE ) WD_KernelPlugInClose(hBT8X8->hWD, &BTKernelPlugIn );    

	Write_Log("BT8X8 Karte freigeben");
    // unregister card
    if (hBT8X8->cardReg.hCard) 
        WD_CardUnregister(hBT8X8->hWD, &hBT8X8->cardReg);


	Write_Log("BT8X8 Kernel-Speicher freigeben");

	Free_DMA(&Risc_dma); 
	Free_DMA(&Vbi_dma[0]);
	Free_DMA(&Vbi_dma[1]);
	Free_DMA(&Vbi_dma[2]);
	Free_DMA(&Vbi_dma[3]);
	Free_DMA(&Vbi_dma[4]);

	Free_Display_DMA(0);
	Free_Display_DMA(1);
	Free_Display_DMA(2);
	Free_Display_DMA(3);
	Free_Display_DMA(4);

	Free_DMA(&Burst_dma[0]);
	Free_DMA(&Burst_dma[1]);
	Free_DMA(&Burst_dma[2]);
	Free_DMA(&Burst_dma[3]);
	Free_DMA(&Burst_dma[4]);
}

DWORD BT8X8_ReadPCIReg(BT8X8_HANDLE hBT8X8, DWORD dwReg)
{
    WD_PCI_CONFIG_DUMP pciCnf;
    DWORD dwVal;

    BZERO(pciCnf);
    pciCnf.pciSlot = hBT8X8->pciSlot;
    pciCnf.pBuffer = &dwVal;
    pciCnf.dwOffset = dwReg;
    pciCnf.dwBytes = 4;
    pciCnf.fIsRead = TRUE;
    WD_PciConfigDump(hBT8X8->hWD,&pciCnf);
    return dwVal;
}


BOOL BT8X8_IsAddrSpaceActive(BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace){
    return hBT8X8->addrDesc[addrSpace].fActive;
}

// General read/write function
void BT8X8_ReadWriteBlock(BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace, DWORD dwOffset, BOOL fRead, PVOID buf, DWORD dwBytes, BT8X8_MODE mode)
{
    WD_TRANSFER trans;
    BOOL fMem = hBT8X8->addrDesc[addrSpace].fIsMemory;
    // safty check: is the address range active
    if (!BT8X8_IsAddrSpaceActive(hBT8X8, addrSpace)) return;
    BZERO(trans);
    if (fRead)
    {
        if (mode==BT8X8_MODE_BYTE) trans.cmdTrans = fMem ? RM_SBYTE : RP_SBYTE;
        else if (mode==BT8X8_MODE_WORD) trans.cmdTrans = fMem ? RM_SWORD : RP_SWORD;
        else if (mode==BT8X8_MODE_DWORD) trans.cmdTrans = fMem ? RM_SDWORD : RP_SDWORD;
    }
    else
    {
        if (mode==BT8X8_MODE_BYTE) trans.cmdTrans = fMem ? WM_SBYTE : WP_SBYTE;
        else if (mode==BT8X8_MODE_WORD) trans.cmdTrans = fMem ? WM_SWORD : WP_SWORD;
        else if (mode==BT8X8_MODE_DWORD) trans.cmdTrans = fMem ? WM_SDWORD : WP_SDWORD;
    }
    if (fMem)
        trans.dwPort = hBT8X8->cardReg.Card.Item[hBT8X8->addrDesc[addrSpace].index].I.Mem.dwTransAddr;
    else trans.dwPort = hBT8X8->cardReg.Card.Item[hBT8X8->addrDesc[addrSpace].index].I.IO.dwAddr;
    trans.dwPort += dwOffset;

    trans.fAutoinc = TRUE;
    trans.dwBytes = dwBytes;
    trans.dwOptions = 0;
    trans.Data.pBuffer = buf;
    WD_Transfer (hBT8X8->hWD, &trans);
}

BYTE BT8X8_ReadByte (BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace, DWORD dwOffset)
{
    BYTE data;
    if (hBT8X8->addrDesc[addrSpace].fIsMemory)
    {
        PBYTE pData = (PBYTE) (hBT8X8->cardReg.Card.Item[hBT8X8->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        data = *pData; // read from the memory mapped range directly
    }
    else BT8X8_ReadWriteBlock( hBT8X8, addrSpace, dwOffset, TRUE, &data, sizeof (BYTE), BT8X8_MODE_BYTE);
    return data;
}

WORD BT8X8_ReadWord (BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace, DWORD dwOffset)
{
    WORD data;
    if (hBT8X8->addrDesc[addrSpace].fIsMemory)
    {
        PWORD pData = (PWORD) (hBT8X8->cardReg.Card.Item[hBT8X8->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        data = *pData; // read from the memory mapped range directly
    }
    else BT8X8_ReadWriteBlock( hBT8X8, addrSpace, dwOffset, TRUE, &data, sizeof (WORD), BT8X8_MODE_WORD);
    return data;
}

DWORD BT8X8_ReadDword (BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace, DWORD dwOffset)
{
    DWORD data;
    if (hBT8X8->addrDesc[addrSpace].fIsMemory)
    {
        PDWORD pData = (PDWORD) (hBT8X8->cardReg.Card.Item[hBT8X8->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        data = *pData; // read from the memory mapped range directly
    }
    else BT8X8_ReadWriteBlock( hBT8X8, addrSpace, dwOffset, TRUE, &data, sizeof (DWORD), BT8X8_MODE_DWORD);
    return data;
}

void BT8X8_WriteByte (BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace, DWORD dwOffset, BYTE data)
{
    if (hBT8X8->addrDesc[addrSpace].fIsMemory)
    {
        PBYTE pData = (PBYTE) (hBT8X8->cardReg.Card.Item[hBT8X8->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        *pData = data; // write to the memory mapped range directly
    }
    else BT8X8_ReadWriteBlock( hBT8X8, addrSpace, dwOffset, FALSE, &data, sizeof (BYTE), BT8X8_MODE_BYTE);
}

void BT8X8_WriteWord (BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace, DWORD dwOffset, WORD data)
{
    if (hBT8X8->addrDesc[addrSpace].fIsMemory)
    {
        PWORD pData = (PWORD) (hBT8X8->cardReg.Card.Item[hBT8X8->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        *pData = data; // write to the memory mapped range directly
    }
    else BT8X8_ReadWriteBlock( hBT8X8, addrSpace, dwOffset, FALSE, &data, sizeof (WORD), BT8X8_MODE_WORD);
}

void BT8X8_WriteDword (BT8X8_HANDLE hBT8X8, BT8X8_ADDR addrSpace, DWORD dwOffset, DWORD data)
{
    if (hBT8X8->addrDesc[addrSpace].fIsMemory)
    {
        PDWORD pData = (PDWORD) (hBT8X8->cardReg.Card.Item[hBT8X8->addrDesc[addrSpace].index].I.Mem.dwUserDirectAddr + dwOffset);
        *pData = data; // write to the memory mapped range directly
    }
    else BT8X8_ReadWriteBlock( hBT8X8, addrSpace, dwOffset, FALSE, &data, sizeof (DWORD), BT8X8_MODE_DWORD);
}

BOOL BT8X8_IntIsEnabled (BT8X8_HANDLE hBT8X8)
{
    if (!hBT8X8->fUseInt) return FALSE;
    if (!hBT8X8->Int.hThread) return FALSE;
    return TRUE;
}


DWORD WINAPI BT8X8_IntThread_Intern (PVOID pData)
{
    BT8X8_HANDLE hBT8X8 = (BT8X8_HANDLE) pData;
	int status;
    int RetBytes;
	int rc;
    BT8X8_INT_INTERRUPT *Int;
	HANDLE InthWD;
    NextFrame=0;
	InthWD=WD_Open();
	Int=&hBT8X8->Int;

	Write_Log("Boote BT8X8_IntThread_Intern");
    ProzessorMask=1<<(IrqProzessor);
    status=SetThreadAffinityMask (GetCurrentThread(),ProzessorMask);         

	
IRQ_LOOP_INTERN_BT:
	rc = (DWORD) DeviceIoControl(InthWD, (DWORD) IOCTL_WD_INT_WAIT, (PVOID) Int,48, NULL, 0, &RetBytes, NULL);
	ResetIrq=0;
    if (Int->Int.fStopped)
		{
			WD_Close(InthWD);
            return(0); // WD_IntDisable() was called
		};
		if ( Current_Mode == 1 ) goto IRQ_LOOP_INTERN_BT;
		status=((Int->Trans[0].Data.Dword>>28)&0x07);
        switch(status) {
        case 1:     NextFrame=1;
			        CurrentIrq++;
					if ( VCIrqCount < 512 ) VCIrqCount++;
					if (CurrentIrq > 255 ) CurrentIrq=0;
					if ( Capture_VBI ) SetEvent(VBI_Event);
					if( Decode_Event != NULL ) SetEvent(Decode_Event);
					goto IRQ_LOOP_INTERN_BT;
        case 2:     NextFrame=2;
			        CurrentIrq++;
					if ( VCIrqCount < 512 ) VCIrqCount++;
					if (CurrentIrq > 255 ) CurrentIrq=0;
					if ( Capture_VBI ) SetEvent(VBI_Event);
					if( Decode_Event != NULL ) SetEvent(Decode_Event);
					goto IRQ_LOOP_INTERN_BT;
        case 3:     NextFrame=3;
			        CurrentIrq++; 
					if ( VCIrqCount < 512 ) VCIrqCount++;
					if (CurrentIrq > 255 ) CurrentIrq=0;
					if ( Capture_VBI ) SetEvent(VBI_Event);
					if( Decode_Event != NULL ) SetEvent(Decode_Event);
					goto IRQ_LOOP_INTERN_BT;
        case 4:     NextFrame=4;
			        CurrentIrq++;
					if ( VCIrqCount < 512 ) VCIrqCount++;
					if (CurrentIrq > 255 ) CurrentIrq=0;
					if ( Capture_VBI ) SetEvent(VBI_Event);
					if( Decode_Event != NULL ) SetEvent(Decode_Event);
					goto IRQ_LOOP_INTERN_BT;
        case 5:     NextFrame=0;
			        CurrentIrq++;
					if ( VCIrqCount < 512 ) VCIrqCount++;
					if (CurrentIrq > 255 ) CurrentIrq=0;
					if ( Capture_VBI ) SetEvent(VBI_Event);
					if( Decode_Event != NULL ) SetEvent(Decode_Event);
					goto IRQ_LOOP_INTERN_BT;
		};
		goto IRQ_LOOP_INTERN_BT;

}


DWORD WINAPI BT8X8_IntThread (PVOID pData)
{
    BT8X8_HANDLE hBT8X8 = (BT8X8_HANDLE) pData;
	BT8X8_ISR btisr;
	int status;
    BT8X8_INT_INTERRUPT *Int;
	HANDLE InthWD;
    int WDSize = sizeof(WD_INTERRUPT);
    int RetBytes;
	int rc;
	InthWD=WD_Open();
	Int=&hBT8X8->Int;

    NextFrame=0;

    Write_Log("Boote BT8X8_IntThread");


    ProzessorMask=1<<(IrqProzessor);
    status=SetThreadAffinityMask (GetCurrentThread(),ProzessorMask);         
 
 IRQ_LOOP_EXTERN_BT:
	rc = (DWORD) DeviceIoControl(InthWD, (DWORD) IOCTL_WD_INT_WAIT, (PVOID) Int,(DWORD) WDSize, NULL, 0, &RetBytes, NULL);
		ResetIrq=0;
    if (Int->Int.fStopped)
		{
			WD_Close(InthWD);
            return(0); // WD_IntDisable() was called
		};
		if ( Current_Mode == 1 ) goto IRQ_LOOP_EXTERN_BT;
        BT8X8_KP_GET_INIT(hBT8X8->hWD, hBT8X8->Int.Int.kpCall.hKernelPlugIn,&btisr);
		NextFrame=btisr.PictureNummer;
		CurrentIrq=btisr.CurrentIrq;
		VCIrqCount=btisr.VCIrqCount;
        if ( Capture_VBI ) SetEvent(VBI_Event);
	if ( Decode_Event != NULL ) SetEvent(Decode_Event);
goto IRQ_LOOP_EXTERN_BT;
}
#endif

void Reset_VCIRQCount()
{
if ( ExternBTDriver == FALSE ) {
              VCIrqCount=0;
			  return;
	};
#ifdef USE_BT8X8
BT8X8_KP_SET_VCIRQCOUNT(hBT8X8->hWD, hBT8X8->Int.Int.kpCall.hKernelPlugIn);
#endif
};

void Set_CurrentIrq(DWORD Wert)
{
if ( ExternBTDriver == FALSE ) {
              CurrentIrq=Wert;
			  return;
	};

#ifdef USE_BT8X8
BT8X8_KP_SET_CURRENTIRQ(hBT8X8->hWD, hBT8X8->Int.Int.kpCall.hKernelPlugIn,Wert);
#endif
};

#ifdef USE_BT8X8
void BT8X8_KP_GET_DRIVER_VERSION(HANDLE hWD, DWORD hKernelPlugIn, char *buf)
{
    WD_KERNEL_PLUGIN_CALL kpCall;
    BT8X8_VERSION mdbtVer;

    BZERO (kpCall);
    kpCall.hKernelPlugIn = hKernelPlugIn;
    kpCall.dwMessage = BT8X8_IOCTL_VERSION;
    kpCall.pData = &mdbtVer;
    // this will call KP_Call() callback in the kernel
    WD_KernelPlugInCall(hWD, &kpCall);
    strcpy(buf,mdbtVer.cVer);
}


void BT8X8_KP_SET_VCIRQCOUNT(HANDLE hWD, DWORD hKernelPlugIn)
{
    WD_KERNEL_PLUGIN_CALL kpCall;
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hKernelPlugIn;
    kpCall.dwMessage = BT8X8_IOCTL_SET_VCIRQCOUNT;
    kpCall.pData = 0;
	WD_KernelPlugInCall(hWD, &kpCall);
};



void BT8X8_KP_SET_CURRENTIRQ(HANDLE hWD, DWORD hKernelPlugIn,DWORD wert)
{
    WD_KERNEL_PLUGIN_CALL kpCall;
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hKernelPlugIn;
    kpCall.dwMessage = BT8X8_IOCTL_SET_CURRENTIRQ;
    kpCall.pData = (PVOID)wert;
	WD_KernelPlugInCall(hWD, &kpCall);
};


void BT8X8_KP_SET_INIT(HANDLE hWD, DWORD hKernelPlugIn)
{
    WD_KERNEL_PLUGIN_CALL kpCall;
    BT8X8_ISR btisr;
	btisr.CurrentIrq=0;
	btisr.PictureNummer=0;
	btisr.Running_Irq=0;
	btisr.VCIrqCount=0;
	btisr.ComAddress=hBT8X8->cardReg.Card.Item[hBT8X8->addrDesc[BT8X8_AD_BAR0].index].I.Mem.dwTransAddr;
    BZERO (kpCall);
    kpCall.hKernelPlugIn = hKernelPlugIn;
    kpCall.dwMessage = BT8X8_IOCTL_SET_INIT;
    kpCall.pData = &btisr;
    // this will call KP_Call() callback in the kernel
    WD_KernelPlugInCall(hWD, &kpCall);
}


void BT8X8_KP_GET_INIT(HANDLE hWD, DWORD hKernelPlugIn, BT8X8_ISR *btisr)
{
    WD_KERNEL_PLUGIN_CALL kpCall;

    BZERO (kpCall);
    kpCall.hKernelPlugIn = hKernelPlugIn;
    kpCall.dwMessage = BT8X8_IOCTL_GET_INIT;
    kpCall.pData = btisr;
    // this will call KP_Call() callback in the kernel
    WD_KernelPlugInCall(hWD, &kpCall);

}

BOOL Check_BT_Plugin( void )
{
    WD_KERNEL_PLUGIN kernelPlugIn;
    kernelPlugIn.pcDriverName = "MDBT"; // this will search for MDSAA.VXD or MDSAA.SYS
    // this will call KP_Open() callback in the kernel
    WD_KernelPlugInOpen(hBT8X8->hWD, &kernelPlugIn);            
    if (kernelPlugIn.hKernelPlugIn) {
        WD_KernelPlugInClose(hBT8X8->hWD, &kernelPlugIn);    
        return(TRUE);
	};
return(FALSE);
};


BOOL BT8X8_IntEnable (BT8X8_HANDLE hBT8X8, BT8X8_INT_HANDLER funcIntHandler)
{

	char LogZeile[128];
    ULONG threadId;
    int i;
    char MustBe[128];
	char IsVersion[128];
    BT8X8_ADDR addrSpace;

	strcpy(MustBe,"BT8x8-Kernel-Driver V1.40");

    
	ExternBTDriver=FALSE;

    if (!hBT8X8->fUseInt) return FALSE;
    // check if interrupt is already enabled

    if (hBT8X8->Int.hThread) return FALSE;

	if ( LoadExternBTDriver == TRUE ) {
	i=0;
    while ( i < 2 ) {

    BZERO (BTKernelPlugIn);
    BTKernelPlugIn.pcDriverName = "MDBT"; // this will search for BT8x8.VXD or BT8x8.SYS
    // this will call KP_Open() callback in the kernel
    WD_KernelPlugInOpen(hBT8X8->hWD, &BTKernelPlugIn);            
    if (BTKernelPlugIn.hKernelPlugIn) {
    BT8X8_KP_GET_DRIVER_VERSION(hBT8X8->hWD,BTKernelPlugIn.hKernelPlugIn, &IsVersion[0]);

	if ((i==0) && ( strcmp(IsVersion,MustBe) != 0 )) {
          	  sprintf(LogZeile,"BT8X8 Treibererweiterung für BT -> Falsche Version ist %s soll %s",IsVersion,MustBe);
              Write_Log(LogZeile);
          	  sprintf(LogZeile,"BT8X8 Versuche Reload");
              Write_Log(LogZeile);
              WD_KernelPlugInClose(hBT8X8->hWD, &BTKernelPlugIn );    
			  Reload_BT_Driver();
              i++;
			  continue;
	}
	if ((i==1) && ( strcmp(IsVersion,MustBe) != 0 )) {
              WD_KernelPlugInClose(hBT8X8->hWD, &BTKernelPlugIn );    
   			   MessageBox(hWnd,"MultiDec-BT-Treiber MDBT : Falsche Version !\n\nMögliche Lösung\n\n - Als Administrator anmelden\n - \\\\HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\MultiDec-BT Treiber löschen\n - Neu Booten","Multi-Decoder",MB_ICONSTOP|MB_OK);
          	  sprintf(LogZeile,"BT8X8 Treibererweiterung für BT -> immer noch falsche Version ist %s soll %s",IsVersion,MustBe);
              Write_Log(LogZeile);
           	  sprintf(LogZeile,"BT8X8 Abbruch mit False");
              Write_Log(LogZeile);
              WD_KernelPlugInClose(hBT8X8->hWD, &BTKernelPlugIn );    
               return(FALSE);
	};
               

	sprintf(LogZeile,"BT8X8 Treibererweiterung für BT ist OK (Version %s)",IsVersion);
    BT8X8_KP_SET_INIT(hBT8X8->hWD, BTKernelPlugIn.hKernelPlugIn);
	i=2;
    hBT8X8->Int.Int.dwCmds = 0;
    hBT8X8->Int.Int.kpCall.hKernelPlugIn = BTKernelPlugIn.hKernelPlugIn;
	WD_IntEnable (hBT8X8->hWD, &hBT8X8->Int.Int);
    // check if WD_IntEnable failed
	if (hBT8X8->Int.Int.fEnableOk) {
    ExternBTDriver=TRUE;
     // create interrupt handler thread
    hBT8X8->Int.hThread = CreateThread (0, 0x1000,
        BT8X8_IntThread, hBT8X8, 0, &threadId);
	};
	} else {
    sprintf(LogZeile,"Treibererweiterung MDBT nicht vorhanden/bereit");
	Reload_BT_Driver();
    i++; 
	};
	Write_Log(LogZeile);
	};
	};

	if ( ExternBTDriver == FALSE ) {
    BZERO(hBT8X8->Int.Trans);

    addrSpace = BT8X8_AD_BAR0; // put the address space of the register here
	hBT8X8->Int.Trans[0].dwPort = hBT8X8->cardReg.Card.Item[hBT8X8->addrDesc[addrSpace].index].I.Mem.dwTransAddr+BT848_INT_STAT;
	hBT8X8->Int.Trans[0].cmdTrans = RM_DWORD;

	hBT8X8->Int.Trans[1].dwPort = hBT8X8->cardReg.Card.Item[hBT8X8->addrDesc[addrSpace].index].I.Mem.dwTransAddr+BT848_INT_STAT;
	hBT8X8->Int.Trans[1].cmdTrans = WM_DWORD;
    hBT8X8->Int.Trans[1].Data.Dword = (DWORD) 0x0fffffff; // put the data to write to the control register here

    hBT8X8->Int.Int.dwCmds = 2;
    hBT8X8->Int.Int.Cmd = hBT8X8->Int.Trans;
    hBT8X8->Int.Int.dwOptions = INTERRUPT_CMD_COPY | INTERRUPT_LEVEL_SENSITIVE;


	WD_IntEnable (hBT8X8->hWD, &hBT8X8->Int.Int);
    // check if WD_IntEnable failed
	if (!hBT8X8->Int.Int.fEnableOk)
        return FALSE;
    // create interrupt handler thread
    hBT8X8->Int.hThread = CreateThread (0, 0x1000,
        BT8X8_IntThread_Intern, hBT8X8, 0, &threadId);

	};

    return TRUE;
}

void BT8X8_IntDisable (BT8X8_HANDLE hBT8X8)
{
    if (!hBT8X8->fUseInt) return;
    if (!hBT8X8->Int.hThread) return;
    WD_IntDisable (hBT8X8->hWD, &hBT8X8->Int.Int);
    WaitForSingleObject (hBT8X8->Int.hThread, INFINITE);
    hBT8X8->Int.hThread = NULL;
}
#endif

void Start_Thread()
{
int i = 0;
DWORD LinkThreadID;
if (( ModeRAWSurface == TRUE ) || ( ModeRAWOverlay == TRUE )) {

	if ( USE_DX_LOCK == TRUE ) {
      NextFrame=1;
      if ( Decode_Event_Stop == NULL ) Decode_Event_Stop=CreateEvent(NULL,FALSE,FALSE,NULL);
      if ( Decode_Event == NULL ) Decode_Event=CreateEvent(NULL,FALSE,FALSE,NULL);
      ResetEvent(Decode_Event_Stop);
      DIB_Hdc=GetDC(hWnd);

#ifdef USE_BT8X8
	  if ( Current_Mode == 0 ) {
// Analog
	for ( i=0; i < 5 ; i++ ) {
       pDisplay[i]=Display_dma[i]->pUserAddr;
	   };

} else {
#endif
       pDisplay[0]=DVB_Display_dma.pUserAddr;
       pDisplay[1]=DVB_Display_dma.pUserAddr;
       pDisplay[2]=DVB_Display_dma.pUserAddr;
       pDisplay[3]=DVB_Display_dma.pUserAddr;
       pDisplay[4]=DVB_Display_dma.pUserAddr;
#ifdef USE_BT8X8
}
#endif
    OutThread=CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         					(DWORD)0,                          // Same stack size.
                         					(LPTHREAD_START_ROUTINE)DX_Lock_Thread,// Thread procedure.
                         					NULL,                         // Parameter.
                         					(DWORD)0,                          // Start immediatly.
                         					(LPDWORD)&LinkThreadID);  
 

}	
	return;
}
NextFrame=1;
if ( Decode_Event == NULL ) Decode_Event=CreateEvent(NULL,FALSE,FALSE,NULL);
if ( Decode_Event_Stop == NULL ) Decode_Event_Stop=CreateEvent(NULL,FALSE,FALSE,NULL);
ResetEvent(Decode_Event_Stop);
 DIB_Hdc=GetDC(hWnd);

#ifdef USE_BT8X8

if ( Current_Mode == 0 ) {
// Analog
	for ( i=0; i < 5 ; i++ ) {
       pDisplay[i]=Display_dma[i]->pUserAddr;
	}
	
	if ( DecodeNagra==TRUE ) {
	 if ( NagraFullField ) {
     OutThread=CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         					(DWORD)0,                          // Same stack size.
                         					(LPTHREAD_START_ROUTINE)NagraYUVOutThread,// Thread procedure.
                         					NULL,                         // Parameter.
                         					(DWORD)0,                          // Start immediatly.
                         					(LPDWORD)&LinkThreadID);  
   return;
	 };
     OutThread=CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         					(DWORD)0,                          // Same stack size.
                         					(LPTHREAD_START_ROUTINE)NagraYUVHalfOutThread,// Thread procedure.
                         					NULL,                         // Parameter.
                         					(DWORD)0,                          // Start immediatly.
                         					(LPDWORD)&LinkThreadID);  
   return;

 } else if ( DecodeVCrypt== TRUE ) {


   OutThread=CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         					(DWORD)0,                          // Same stack size.
                         					(LPTHREAD_START_ROUTINE)VCYUVHalfOutThread,// Thread procedure.
                         					 NULL,                         // Parameter.
                         					(DWORD)0,                          // Start immediatly.
                         					(LPDWORD)&LinkThreadID);       // Thread ID.


 
    return;
  } else if ( DecodeDecoderVCrypt== TRUE ) {
   OutThread=CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         					(DWORD)0,                          // Same stack size.
                         					(LPTHREAD_START_ROUTINE)VCDecoderYUVHalfOutThread,// Thread procedure.
                         					 NULL,                         // Parameter.
                         					(DWORD)0,                          // Start immediatly.
                         					(LPDWORD)&LinkThreadID);       // Thread ID.


 
    return;
  }
  if ( HalfTVMode == FALSE ) {
	if ( ColourFormat != 4 ) { 
 OutThread=CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         					(DWORD)0,                          // Same stack size.
                         					(LPTHREAD_START_ROUTINE)RGBOutThread,// Thread procedure.
                         					NULL,                         // Parameter.
                         					(DWORD)0,                          // Start immediatly.
                         					(LPDWORD)&LinkThreadID);       // Thread ID.
 } else {
	 
 OutThread=CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         					(DWORD)0,                          // Same stack size.
                         					(LPTHREAD_START_ROUTINE)YUVOutThread,// Thread procedure.
                         					NULL,                         // Parameter.
                         					(DWORD)0,                          // Start immediatly.
                         					(LPDWORD)&LinkThreadID);       // Thread ID.

 }
 return;
  } else {
	if ( ColourFormat != 4 ) { 
 OutThread=CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         					(DWORD)0,                          // Same stack size.
                         					(LPTHREAD_START_ROUTINE)RGBOutThreadHalf,// Thread procedure.
                         					NULL,                         // Parameter.
                         					(DWORD)0,                          // Start immediatly.
                         					(LPDWORD)&LinkThreadID);       // Thread ID.
 } else {
	 
 OutThread=CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         					(DWORD)0,                          // Same stack size.
                         					(LPTHREAD_START_ROUTINE)YUVOutThreadHalf,// Thread procedure.
                         					NULL,                         // Parameter.
                         					(DWORD)0,                          // Start immediatly.
                         					(LPDWORD)&LinkThreadID);       // Thread ID.

 }
 return;

  }
} else {
#endif
       pDisplay[0]=DVB_Display_dma.pUserAddr;
       pDisplay[1]=DVB_Display_dma.pUserAddr;
       pDisplay[2]=DVB_Display_dma.pUserAddr;
       pDisplay[3]=DVB_Display_dma.pUserAddr;
       pDisplay[4]=DVB_Display_dma.pUserAddr;
  if ( HalfTVMode == FALSE ) {
	if ( ColourFormat != 4 ) { 
 OutThread=CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         					(DWORD)0,                          // Same stack size.
                         					(LPTHREAD_START_ROUTINE)RGBOutThread,// Thread procedure.
                         					NULL,                         // Parameter.
                         					(DWORD)0,                          // Start immediatly.
                         					(LPDWORD)&LinkThreadID);       // Thread ID.
 } else {
	 
 OutThread=CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         					(DWORD)0,                          // Same stack size.
                         					(LPTHREAD_START_ROUTINE)YUVOutThread,// Thread procedure.
                         					NULL,                         // Parameter.
                         					(DWORD)0,                          // Start immediatly.
                         					(LPDWORD)&LinkThreadID);       // Thread ID.

 }
 return;
  } else {
	if ( ColourFormat != 4 ) { 
 OutThread=CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         					(DWORD)0,                          // Same stack size.
                         					(LPTHREAD_START_ROUTINE)RGBOutThreadHalf,// Thread procedure.
                         					NULL,                         // Parameter.
                         					(DWORD)0,                          // Start immediatly.
                         					(LPDWORD)&LinkThreadID);       // Thread ID.
 } else {
	 
 OutThread=CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         					(DWORD)0,                          // Same stack size.
                         					(LPTHREAD_START_ROUTINE)YUVOutThreadHalf,// Thread procedure.
                         					NULL,                         // Parameter.
                         					(DWORD)0,                          // Start immediatly.
                         					(LPDWORD)&LinkThreadID);       // Thread ID.

 }
 return;

#ifdef USE_BT8X8
  }
#endif

};

};



void Stop_Thread()
{
DWORD dwStartCount;
DWORD dwEndCount;
char Text[128];
int i;

if ( OutThread == NULL ) return;
ResetEvent(Decode_Event_Stop);
dwStartCount=GetTickCount();
for ( i=0; i < 10 ; i++ ) {
NextFrame=-1;
NextFrame=-1;
SetEvent(Decode_Event);
NextFrame=-1;
if (WaitForSingleObject(Decode_Event_Stop, 5 ) == WAIT_OBJECT_0 ) {
// Thread Down
CloseHandle(Decode_Event);
Decode_Event=NULL;
CloseHandle(OutThread);
OutThread=NULL;
ReleaseDC(hWnd,DIB_Hdc);
DIB_Hdc=NULL;
dwEndCount=GetTickCount();
sprintf(Text,"TIMEING Stop-Out-Thread Level 1 (Loop %d) nach %d ms",i,dwEndCount-dwStartCount);
Write_Log(Text);
return;
};
};
NextFrame=-1;
SetThreadPriority(OutThread,THREAD_PRIORITY_NORMAL);
if ( Decode_Event != NULL ) CloseHandle(Decode_Event);
Decode_Event=NULL;

for ( i=0; i < 15 ; i++ ) {
NextFrame=-1;
if (WaitForSingleObject(Decode_Event_Stop, 100 ) == WAIT_OBJECT_0 ) {
// Thread Down
if ( Decode_Event != NULL ) CloseHandle(Decode_Event);
Decode_Event=NULL;
if ( OutThread != NULL ) CloseHandle(OutThread);
OutThread=NULL;
ReleaseDC(hWnd,DIB_Hdc);
DIB_Hdc=NULL;
dwEndCount=GetTickCount();
sprintf(Text,"TIMEING Stop-Out-Thread Level 2 (Loop %d) nach %d ms",i,dwEndCount-dwStartCount);
Write_Log(Text);
return;
};
};

TerminateThread(OutThread,0);
CloseHandle(OutThread);
OutThread=NULL;
ReleaseDC(hWnd,DIB_Hdc);
DIB_Hdc=NULL;
dwEndCount=GetTickCount();
sprintf(Text,"TIMEING Stop-Out-Thread Terminated %d ms",dwEndCount-dwStartCount);
Write_Log(Text);
};


#ifdef USE_BT8X8

/****************************************************************************
*
*    FUNCTION: VBI_Driver_Stuff
*
****************************************************************************/

void Start_VBI()
{
DWORD  LinkThreadID;

	    StopVBI = FALSE;
		ResetEvent(VBI_Event);
        CloseHandle(CreateThread ((LPSECURITY_ATTRIBUTES)NULL,(DWORD)0,(LPTHREAD_START_ROUTINE)DoVBI,NULL,(DWORD)0,(LPDWORD)&LinkThreadID));
};

void Stop_VBI()
{
StopVBI = TRUE;
SetEvent(VBI_Event);
Sleep(20);
StopVBI = TRUE;
SetEvent(VBI_Event);
};

void DoVBI()
{
	int CurrentFrame;
	BYTE  *pVBI;
	DWORD   VBI_Tic_Count = 0;
    int vbi_frames;
	int line;
	int norm=0;
    double vtfreq;
	double f=0.0;
    double vpsfreq=5.0; // does NTSC have VPS???
    double vcfreq=0.77;
    double freq;
    double vdatfreq=2.0;
    int i;
    int LastVBI =-1;

    ProzessorMask=1<<(VBIProzessor);
    SetThreadAffinityMask (GetCurrentThread(),ProzessorMask);         

    vtfreq=norm ? 5.72725 : 6.9375;

    /* if no frequency given, use standard ones for Bt848 and PAL/NTSC */
    if (f==0.0) 
      freq=norm ? 28.636363 : 35.468950;
    else
      freq=f;

    vtstep=(int)((freq/vtfreq)*FPFAC+0.5);
    /* VPS is shift encoded, so just sample first "state" */
    vpsstep =2*(int)((freq/vpsfreq)*FPFAC+0.5); 
    vcstep  =(int)((freq/vcfreq)*FPFAC+0.5);
    vdatstep=(int)((freq/vdatfreq)*FPFAC+0.5); 


    vbi_frames=0;
    VBI_Tic_Count=GetTickCount();

    for (;;)
    {
    vbi_frames++;
    if (  WaitForSingleObject (VBI_Event, INFINITE) != WAIT_OBJECT_0 ) continue;
	ResetEvent(VBI_Event);
    if (!VideoPresent() ) continue;
    if ( StopVBI == TRUE ) ExitThread(0);	
	i=NextFrame-1;
	if ( i < 0 ) i=4;
	if ( i == LastVBI ) continue;

	do {
    LastVBI=i;
	CurrentFrame=i;
	pVBI=(LPBYTE)Vbi_dma[CurrentFrame].pUserAddr;


	if (( Decode_AutoDetect == TRUE ) || (VBI_Flags&VBI_VC ))  {
            CryptInfo.CryptMode=nagra_coded=Is_Nagra_Coded(pVBI);

	if ( Decode_AutoDetect == TRUE ) {
		if ( nagra_coded != DecodeNagra ) {
			if ( AutoDetect_Nagra == TRUE ) {
			if ( nagra_coded == TRUE ) SendMessage(hWnd,WM_COMMAND,IDM_DECODING_NAGRA1,0);
			else SendMessage(hWnd,WM_COMMAND,IDM_DECODING_OFF,0);
			};
		}
	};
		if ( nagra_coded == FALSE ) {
for ( line=0;  line< 9; line++)  Get_VC_Crypt_Mode(pVBI+line*2048, line);
for ( line=16;  line< 25; line++) Get_VC_Crypt_Mode(pVBI+line*2048, line-16);
	if ( AutoDetect_VC == TRUE ) {
	if ( Decode_AutoDetect == TRUE ) {
        if (( CryptInfo.CryptMode == 2 ) && ( DecodeDecoderVCrypt == FALSE )) {
          SendMessage(hWnd,WM_COMMAND,IDM_DECODER_MODE,0);
		}
        if (( CryptInfo.CryptMode == 3 ) && ( DecodeVCrypt == FALSE )) {
          SendMessage(hWnd,WM_COMMAND,IDM_DECODING_VIDEOCRYPT,0);
		}
        if (( CryptInfo.CryptMode == 0 ) && (( DecodeDecoderVCrypt == TRUE ) ||( DecodeVCrypt == TRUE))) {
          SendMessage(hWnd,WM_COMMAND,IDM_DECODING_OFF,0);
		}
		}
	}	
}
     CryptInfo.LastCryptMode=CryptInfo.CryptMode;
	}


    for (line=0; line<VBI_lpf*2; line++) {
      VBI_decode_line(pVBI+line*2048, line);
    }

   if(VBI_Tic_Count+960 <= GetTickCount())
      {
	   VBI_FPS=vbi_frames+1;
	   if ( VBI_FPS > 25 ) VBI_FPS = 25;
	   vbi_frames=0;
       VBI_Tic_Count = GetTickCount();
	  }

    i=NextFrame-1;
	if ( i < 0 ) i=4;
    } while ( i != CurrentFrame );

	}
}


BOOL Is_Nagra_Coded(BYTE *cVBI)
{
	int i,j,row;
	BOOL text_lines_found=FALSE;

	unsigned char vbiline[2048];
	unsigned char vbibits[2048/4];
	unsigned char vbibytes[2048/4/8];
	
		text_lines_found=FALSE;
		for(row=0; row<32; row++) {
			for (i=0;i<2048;i++) {
           	    if (*(cVBI++) < 95) vbiline[i]=0;
	                              else vbiline[i]=1;
							}
			i=0;
				while (vbiline[i]!=1) 
					if (i++ > 2048) break;
					if (i>200) continue;
					for (j=0;j<48 ;j++) vbibits[j]=vbiline[Nagra_sample_bits[j]+Nagra_offset+i];


                	for (i=0;i<48;i+=8) {
							vbibytes[i/8]=0;
							for (j=0;j<8;j++) vbibytes[i/8]|=vbibits[i+j]<<j;  // inverse order
							}
                   if (vbibytes[0]==0x55) text_lines_found=TRUE;
				   if ((vbibytes[0]==0x55) && (vbibytes[4]==0x15)) {
					   if  (vbibytes[5]&0x20) {
						      nagra_coded = TRUE;
							  break;
					   }
					       else nagra_coded = FALSE;
				   }
 	}
		if ( text_lines_found == FALSE ) nagra_coded=FALSE;
		
		return(nagra_coded);
}


#endif

void Get_VC_Crypt_Mode(unsigned char *VBI_Buffer, int line) 
{
  int i;
  int aline;
  int lp;

  aline=line % VBI_lpf;
      
      // VC is in lines 5,6,7,8, VC2 in 1,2,3,4 (add 7 to get PAL line number)
      if (aline<9) {

//	VBI_AGC(VBI_Buffer,200,500,3);

	VBI_off=0x2D ;
	// VC ?
	if (aline > 4 && aline < 9) {
	  lp=(aline-5)*5;
	  // set pointer to about the middle of the first data bit
	  VBI_spos=140<<FPSHIFT;
	  for (i=0; i<5; i++) 
	    VBI_vcbuf[i+lp]=VBI_Scan(VBI_Buffer,vcstep);
	  if (aline==8) {
	    // de-interleave 20 byte buffer
	    deileave(VBI_vcbuf+12);
	    deileave(VBI_vcbuf+6);
	    deileave(VBI_vcbuf+0);
	    // and unham it
		for (i=0; i<10; i++) {
			VBI_vcbuf[i]=unham2(VBI_vcbuf+i*2);
		};
	    vcdump(VBI_vcbuf);
	  }
	} else if (aline > 0 && aline < 5) {
	  // NO, can only be VC2 
	  lp=aline*5-5;
	  VBI_spos=135<<FPSHIFT;
	  for (i=0; i<5; i++) 
	    VBI_vc2buf[i+lp]=VBI_Scan(VBI_Buffer,vcstep);
	  if (aline==4) {
	    deileave(VBI_vc2buf+12);
	    deileave(VBI_vc2buf+6);
	    deileave(VBI_vc2buf+0);
	    for (i=0; i<10; i++) 
	      VBI_vc2buf[i]=unham2(VBI_vc2buf+i*2);
	    vc2dump(VBI_vc2buf);
	  }
   }
  }
return;
  }




void VBI_decode_line(unsigned char *VBI_Buffer, int line) 
{
  unsigned char data[45];
  int i,p;
  BOOL VDat_Load ;

  if ( line >= VBI_lpf ) line -=VBI_lpf;

  VBI_AGC(VBI_Buffer,120,450,1);

  /* all kinds of data with videotext data format: videotext, intercast, ... */
  if ((( VBI_Flags&VBI_VT) || ( VBI_Flags&VBI_WINBIS)) && ( line < 16 )) {
    // search for first 1 bit (VT always starts with 55 55 27 !!!)
    p=50;
    while ((VBI_Buffer[p]<VBI_thresh)&&(p<350)) p++;
    VBI_spos=(p<<FPSHIFT)+vtstep/2;
    
    /* ignore first bit for now */
    data[0]=VBI_Scan(VBI_Buffer,vtstep);
    //cout << HEX(2) << (int)data[0] << endl;
    if ((data[0]&0xfe)==0x54) {
      data[1]=VBI_Scan(VBI_Buffer,vtstep);
      switch (data[1]) {
      case 0xd5: /* oops, missed first 1-bit: backup 2 bits */
	VBI_spos-=2*vtstep; 
	data[1]=0x55;
      case 0x55:
	data[2]=VBI_Scan(VBI_Buffer,vtstep);
	switch(data[2]) {
	case 0xd8: /* this shows up on some channels?!?!? */
		for (i=3; i<45; i++)  {
	        data[i]=VBI_Scan(VBI_Buffer,vtstep);
		};
	  return;
	case 0x27:
		for (i=3; i<45; i++) {
			data[i]=VBI_Scan(VBI_Buffer,vtstep);
		};
		VBI_decode_vt(data);
	  return;
	default:
	  break;
	}
      default:
	break;
      }
    }
  }
  
  /* VPS information with channel name, time, VCR programming info, etc. */
  if ((VBI_Flags&VBI_VPS) && (line==9)) {
    p=150;
    while ((VBI_Buffer[p]<VBI_thresh)&&(p<260)) p++;
    p+=2;
    VBI_spos=p<<FPSHIFT;
    if ((data[0]=VBI_Scan(VBI_Buffer,vpsstep))!=0xff) return;
    if ((data[1]=VBI_Scan(VBI_Buffer,vpsstep))!=0x5d) return;
    for (i=2; i<16; i++) {
		data[i]=VBI_Scan(VBI_Buffer,vpsstep);
	}
    VBI_decode_vps(data);
  }


  /* Video_Dat_Stuff  */
  if ( (VBI_Flags&VBI_VD) && ((line==17) || (line==18))) {
    p=100;
    VBI_off=0;
    while ((VBI_Buffer[p]<100)&&(p<260)) p++;
    if (p<200) {
      VBI_spos=p<<FPSHIFT;
      VBI_spos+=vdatstep/2;
      VDat_Load=TRUE;
      for (i=0; i<10; i++) {
		  if ( VBI_VDatScan(VBI_Buffer, vdatstep, i ) != 0 ) {
		      strcpy(VDat.Error,"Kein StartBit gefunden");
			  VDat_Load=FALSE; 
			  if ( VDat.Index > 0 ) VideoDat_Flush();
			  break;
		  }
		 };  // 10 Bytes geladen

      if ( VDat_Load == TRUE ) {
          if ( VD_RAW == TRUE ) {
 		      VDat.BlocksOK++;
	          strcpy(VDat.Error,"                    ");
			  for (i=0; i<10; i++) VDat.XDATA[VDat.Index++]=vdat[i];
              if ( VDat.Index > 1000 ) VideoDat_Flush();
		  } else Work_VideoDat(vdat);
	  } else { 
			  if ( VDat.Index > 0 ) VideoDat_Flush();
			  VDat.BlocksError++;
	  };
	  
	  
           
	} else {
		      strcpy(VDat.Error,"Kein Videodat-Signal gefunden");
			  if ( VDat.Index > 0 ) VideoDat_Flush();
			  VDat.BlocksError=0;
        	  VDat.BlocksOK=0;

	};

  };
};



  void VBI_decode_vps(unsigned char *data) {

	  unsigned char *info;

	 info=data;
    if ((info[3]&0x80)) {
      VPS_chname[VPS_namep]=0;
      if (VPS_namep==8) {
		  if ( strcpy(VPS_chname,VPS_tmpName) == 0 ) memcpy(VPS_lastname,VPS_chname,9);   // VPS-Channel-Name
             strcpy(VPS_tmpName,VPS_chname);
      }
      VPS_namep=0;
    }
    VPS_chname[VPS_namep++]=info[3]&0x7f;
    if (VPS_namep>=9) VPS_namep=0;
	if ( ShowVPSInfo != NULL ) SetDlgItemText(ShowVPSInfo,TEXT1,VPS_lastname);
  }

// VIDEODAT // 
int VBI_VDatScan(BYTE *VBI_Buffer, unsigned int step, int BytePos) 
{ 
    int j;
    /* check for start bit */
    if (!((VBI_Buffer[VBI_spos>>FPSHIFT]+VBI_off)&0x80)) return -1;


	/* Resync Glaube ich nicht notwendig 
    for (j=0; j<(step>>FPSHIFT); j++)
      if (!((VBI_Buffer[(VBI_spos>>FPSHIFT)-j]+VBI_off)&0x80))
	break;
    VBI_spos=VBI_spos-(j<<FPSHIFT)+step/2;
*/
    VBI_spos+=step;
    vdat[BytePos]=0x00;

    for (j=7;  j>=0; j-- ) {
      vdat[BytePos]|=(((VBI_Buffer[VBI_spos>>FPSHIFT]+VBI_off)&0x80)>>j);
      VBI_spos+=step;
	};
    /* check for stop bit */
    if (!((VBI_Buffer[VBI_spos>>FPSHIFT]+VBI_off)&0x80)) {
		// kein StoppBit gefunden 
		// Return -1 ???? ( Als nix machen ) ???
	};

    VBI_spos+=step;
    vdat[BytePos]^=0xff;
	return 0;
  }


void Work_VideoDat(unsigned char *Buffer)
{
	int i,j;
    unsigned short CheckSum=0;

    VDat.BlocksOK++;

	for ( i=0; i < 10; i++ ) { 
		if ( VDat.Status == 0 ) {
			if ( Buffer[i] == 0x56 ) VDat.Status=1;  
			VDat.LastKillPos=0;
		}  else if ( VDat.Status == 1 ) {
			if ( Buffer[i] == 0xAE ) VDat.Status=2;
			    else VDat.Status=0;
		} else if ( VDat.Status == 2 ) {
			 VDat.ADR = Buffer[i];
			 if ( VDat.ADR != 0xff ) VDat.Status=3;
			 else VDat.Status=0;
		} else if ( VDat.Status == 3 ) {
			 VDat.SYS = Buffer[i];
			 VDat.Interleave = (Buffer[i]&0x70) / 16;
			    	//Ermittelt die Blocklaenge...

			 switch (Buffer[i]&0x0F) {
    	        	case 0: VDat.Lenght =  1; break;
     				case 1: VDat.Lenght =  2; break;
    				case 2: VDat.Lenght =  4; break;
    				case 3: VDat.Lenght =  8; break;
    				case 4: VDat.Lenght = 16; break;
    				case 5: VDat.Lenght = 32; break;
    				case 6: VDat.Lenght = 64; break;
    				case 7: VDat.Lenght =128; break;
    				case 8: VDat.Lenght =256; break;
    				case 9: VDat.Lenght =512; break;
			 }
			 VDat.Lenght-=1;
			 VDat.Status=4;
		} else if ( VDat.Status == 4 ) {
			VDat.LBN = Buffer[i];
            VDat.Status = 5;
        } else if ( VDat.Status == 5 ) {
			VDat.KEY= Buffer[i];
			VDat.Pos=0;
            memset(&VDat.XDATA[0],0x00,512);
			VDat.Status = 6;
        } else if ( VDat.Status == 6 ) {
			VDat.XDATA[VDat.Pos++] = Buffer[i];
           	//56 AE FF Folge erkennen...
			if (VDat.XDATA[VDat.Pos-1] == 0xff) {
			    	if (VDat.Pos > 2) {
          	            	if (VDat.XDATA[VDat.Pos-2] == 0xAE) {
								if (VDat.XDATA[VDat.Pos-3] == 0x56) {
                			if (VDat.LastKillPos != (VDat.Pos-1)) { 
                				//Dann muss das FF entfernt werden, aber nur wenn
                 				//nicht schon ein FF weg gemacht wurde.!!!!
                    				VDat.Pos-- ;
//									VDat.Lenght--;
                    				VDat.LastKillPos = VDat.Pos;
									}
								}
						}
					}
			}
    	if (VDat.Pos < VDat.Lenght) VDat.Status = 6; 
		  else VDat.Status = 7; 
		} else if ( VDat.Status == 7 ) { 
                 VDat.CSL = Buffer[i];
    	         VDat.Status = 8 ;
		} else if ( VDat.Status == 8 ) { 
		VDat.CSH=Buffer[i];
    	CheckSum = 0;
    	CheckSum+=VDat.ADR;
    	CheckSum+=VDat.SYS;
      	CheckSum+=VDat.LBN;
    	CheckSum+=VDat.KEY;
		for (j=0;j<VDat.Lenght;j++) CheckSum+=VDat.XDATA[j];

		VDat.BlockSoFar++;
		if ((VDat.CSL == (CheckSum&0x00FF)) && (VDat.CSH == ((CheckSum&0xFF00) >> 8))) {
			VDat.Fehler_Frei = TRUE;
		} else {
			VDat.CRCError++;
			VDat.Fehler_Frei = FALSE;
		}

		if (VDat.KEY <= 4) {  /* Seit 0.02 Weg! || (LastInter != Block.interleave)*/
      		if (VDat.Blockzaehler < 8) {
       			VDATBlockz[VDat.Blockzaehler].KEY = VDat.KEY;
       			VDATBlockz[VDat.Blockzaehler].LBN = VDat.LBN;
       			VDATBlockz[VDat.Blockzaehler].Fehler_Frei = VDat.Fehler_Frei;
       			for(j=0;j<VDat.Lenght;j++) VDATBlockz[VDat.Blockzaehler].XDATA[j] = VDat.XDATA[j];
       		    VDATBlockz[VDat.Blockzaehler].Lenght = VDat.Lenght;
				VDat.Blockzaehler++;
			} else {
       			VBI_VDat_Blockaustausch();
       			VDat.Blockzaehler = 0;
       			VDATBlockz[VDat.Blockzaehler].KEY = VDat.KEY;
       			VDATBlockz[VDat.Blockzaehler].LBN = VDat.LBN;
       			VDATBlockz[VDat.Blockzaehler].Fehler_Frei = VDat.Fehler_Frei;
       			for(j=0;j<VDat.Lenght;j++) VDATBlockz[VDat.Blockzaehler].XDATA[j] = VDat.XDATA[j];
				VDATBlockz[VDat.Blockzaehler].Lenght = VDat.Lenght;
       			VDat.Blockzaehler++;
       		}
		};

     	if (VDat.KEY == 0) VDat.eLBN = 0x00;
//    	VDat.Fehler_Frei = FALSE;
        VDat.Status = 0;
}
}
};



void VBI_VDat_Blockaustausch () {
	int i,j,k,z;

	for (i=0;i<8;i++) {
			if (VDATBlockz[i].Fehler_Frei == FALSE) {
        		for (j=0;j<8;j++) {
     				if (VDATBlockz[i].LBN == VDATBlockz[j].LBN) {
       					VDATBlockz[i].KEY = VDATBlockz[j].KEY;
       					VDATBlockz[i].LBN = VDATBlockz[j].LBN;
       					VDATBlockz[i].Fehler_Frei = VDATBlockz[j].Fehler_Frei;
						VDATBlockz[i].Lenght = VDATBlockz[j].Lenght;
						for(k=0;k<VDATBlockz[i].Lenght;k++) VDATBlockz[i].XDATA[k] = VDATBlockz[j].XDATA[k];
       				}
			}
			} 
	}
	// Nach dem Decodeing alle KEYs auf 99 setzten...
   	if (VBI_VDAT_DecodeBlockz() == 0 ) for (z=0;z<8;z++) VDATBlockz[z].KEY = 99;
}

int VBI_VDAT_DecodeBlockz () 
{
  	//Decodiert die Datenbloecke...
  	int i;
    char FileName[255];

	for (i=0;i<8;i++) {
		if (VDat.eLBN == VDATBlockz[i].LBN) {

			switch (VDATBlockz[i].KEY) {
  				case 0: //Start of Transmission
                        VBI_VDAT_SOTInfo(VDATBlockz[i], &SOTInfoRec); //Neu !!
          				VDat.eLBN++;
          			break;
  				case 1: //End of Transmission
          				VDat.eLBN++;
          			break;
  				case 2: //Start of File
          				VBI_VDat_Filename(VDATBlockz[i], &VDat.FileName[0]);
          				VDat.FileSize = VBI_VDat_FileSize(VDATBlockz[i]);
                        sprintf(FileName,"%s\\%s",Current_VD_DIR,VDat.FileName);
						VDat.FilePtr=fopen(FileName,"wb");
						if ( VDat.FilePtr == NULL ) {
							strcpy(VDat.Error,"VideoDat-Datei kann nicht zum schreiben geöffnet werden");
						}; 
          				VDat.eLBN++;
          			break;
  				case 3: //End of File
          				if ( VDat.FilePtr != NULL ) fclose(VDat.FilePtr);
          				VDat.eLBN++;
          			break;
  				case 4: //DATA
					if ( VDat.FilePtr != NULL ) {
					
						if ( VBI_VDat_WriteData(VDATBlockz[i]) == FALSE ) {
                     	    fclose(VDat.FilePtr);
							strcpy(VDat.Error,"Schreibfehler bei der VideoDat-Datei");
						};
					};
						VDat.eLBN++;
          				break;
		};	
		};
	};
  return(0);
}

 //Ermittelt den Dateinamen...
void VBI_VDat_Filename(struct TVDatBlockz Block, char *fNames) {
	char fName[13];
  	int t=0;
  	
      	while (t != 13) {
       		fName[t] = Block.XDATA[t+13];
       		t++;
            fName[t]=0x00;
       		if (Block.XDATA[t+13] == 0x00) t = 13;
      	}
   	fName[12] = 0x00;
   	strcpy (fNames, fName);
}

unsigned int VBI_VDat_FileSize(struct TVDatBlockz Block) {
	unsigned char a,b,c,d;
  	unsigned int value;
  
  	a = Block.XDATA[4];
  	b = Block.XDATA[5];
  	c = Block.XDATA[6];
  	d = Block.XDATA[7];

  	value = 0;
  	value += d << 24;      /// BOCK in eurer Software !! //Warning ignorieren, wird sowieso bald geändert!
  	value += c << 16;
  	value += b << 8;
  	value += a;
  	
  	return(value);
}
				 
BOOL VBI_VDat_WriteData(struct TVDatBlockz  Block) {
  	//Daten sichern...
  	long BytesToWrite;
  	  	  
  	fseek (VDat.FilePtr, 0L, SEEK_END);
  	BytesToWrite = ftell (VDat.FilePtr);
  	
  	if ((BytesToWrite + 255) > (signed int)VDat.FileSize)
       		BytesToWrite = VDat.FileSize - BytesToWrite;
       	else
       		BytesToWrite = 255;
       
    	if ( fwrite (Block.XDATA, 1, (size_t)BytesToWrite, VDat.FilePtr) != (unsigned int)BytesToWrite ) return(FALSE);
return(TRUE);
}


void VBI_VDAT_SOTInfo(struct TVDatBlockz Block,struct SOTREC *Info) {
  unsigned char   sizeGN;       //Länge des GeneralNames...
  unsigned char   sizeEC;       //Länge des Exec-Komandos...
  unsigned char   sizeKO;       //Länge des Kommentars...
  FILE* txtFile;
  char FName[255];

  Info->date = (unsigned int)Block.XDATA[0];        //Murks Murks... Date.
  Info->fanz = (unsigned short)Block.XDATA[4];       //Murks Murks... Anzahl der Dateien.

  sizeGN = Block.XDATA[6];
  strncpy(Info->GeneralName,&Block.XDATA[7],sizeGN);
  Info->GeneralName[sizeGN]=0x00;

  sizeEC = Block.XDATA[6+sizeGN+1];
  if (sizeEC != 0)
  {
    strncpy(Info->ExecCommand,&Block.XDATA[7+sizeGN+1],sizeEC);
	Info->ExecCommand[sizeEC]=0x00;
  }

  sizeKO = Block.XDATA[6+sizeGN+1+sizeEC+1];
  if (sizeKO != 0)
  {
    strncpy(Info->Kommentar,&Block.XDATA[7+sizeGN+1+sizeEC+1],sizeKO);
    Info->Kommentar[sizeKO]=0x00;
  }


  //Verzeichnis Anlegen, in das wir schreiben wollen...
  sprintf(Current_VD_DIR,"%s//%s",VD_DIR,Info->GeneralName);
  CreateDirectory(Current_VD_DIR,NULL);

  sprintf(FName,"%s//VideoDat.txt",Current_VD_DIR);
  if ((txtFile=fopen(FName,"w")) == NULL )  {
   strcpy(Current_VD_DIR,VD_DIR);
   sprintf(FName,"%s//VideoDat.txt",Current_VD_DIR);
   if ((txtFile=fopen(FName,"w")) == NULL )  return;
  }
 
fprintf(txtFile,"***********************************************************************\n");
fprintf(txtFile,"\n");
fprintf(txtFile,"                     MultiDecoder - VideoDat\n");
fprintf(txtFile,"\n");
fprintf(txtFile,"***********************************************************************\n");
fprintf(txtFile,"\n");
fprintf(txtFile,"Datum der Sendung             : %d \n",Info->date);
fprintf(txtFile,"Anzahl der Dateien            : %d \n",Info->fanz);
fprintf(txtFile,"Name des Verzechnisses        : %s \n",Info->GeneralName);
fprintf(txtFile,"Auszuführendes Kommando       : %s \n",Info->ExecCommand);
fprintf(txtFile,"Kommentar                     : %s \n",Info->Kommentar);
fprintf(txtFile,"\n");
fclose(txtFile);
}


void VideoDat_Flush ( void )
{
FILE *sFile;
char FileName[255];
if ( VDat.Index > 0 ) {
sprintf(FileName,"%s//%s",VD_DIR,VDat.RawName);

sFile = fopen(FileName, "ab");
     if (sFile == NULL) {
          strcpy(VDat.Error,"Datei kann zum schreiben nicht geöffnet werden");
	      VDat.Index=0;
          return;
	 }; 

if(fwrite(VDat.XDATA, 1, VDat.Index, sFile) != (unsigned int)VDat.Index) {
      	strcpy(VDat.Error,"Datei kann nicht geschrieben werden");
		 } else strcpy(VDat.Error,"");

fclose(sFile);
VDat.Index=0;
};
};




unsigned char VBI_Scan(BYTE *VBI_Buffer, unsigned int step) { 
    int j;
    unsigned char dat;
    
    for (j=7, dat=0; j>=0; j--, VBI_spos+=step)
      dat|=((VBI_Buffer[VBI_spos>>FPSHIFT]+VBI_off)&0x80)>>j;
    return dat;
  }

// unham 2 bytes into 1, report 2 bit errors but ignore them
unsigned char unham(unsigned char *d)
{
  unsigned char c1,c2;
  
  c1=unhamtab[d[0]];
  c2=unhamtab[d[1]];
  return (c2<<4)|(c1);
}


// unham, but with reversed nibble order for VC
unsigned char unham2(unsigned char *d)
{
  unsigned char c1,c2;
  
  c1=unhamtab[d[0]];
  c2=unhamtab[d[1]];
  return (c1<<4)|(c2);
}


// de-interleave 8 bytes and invert bit order of first and last
void deileave(unsigned char *buf)
{
  int i, j;
  unsigned char wb[8], mask=0x80;
  
  memset((void *)wb, 0, 8);
  for (i=7; i>=0; i--, mask>>=1) {
    for (j=0; j<8; j++)
      wb[i]|=(((mask&buf[j])!=0)<<(7-j));
  }
  wb[0]=invtab[wb[0]];
  wb[7]=invtab[wb[7]];
  memcpy(buf, wb, 8);
}

// Interpret and dump VC data

void vcdump(unsigned char *buf)
{
  /* The same data usually is transmitted 4 times and in 2 part.
     The second part has the same identifier but with switched nibbles.  */

switch (buf[0]) {

  case 0x87:
       memcpy(CryptInfo.OSD0,&buf[0],10); 
	   CryptInfo.OSD0Set=1;
	   if (( CryptInfo.OSD0[1] == 0x00 ) && (VCIrqCount < 16)) {
             CryptInfo.CryptMode=2;
 	         Reset_VCIRQCount();
			 return;
		   }
	   if (( CryptInfo.OSD0[1] != 0x00 ) && (VCIrqCount < 96 )){
             CryptInfo.CryptMode=3;
 	         Reset_VCIRQCount();
			 return;
		   }

	   break;

  case 0x78:
       memcpy(CryptInfo.OSD1,&buf[0],10); 
	   CryptInfo.OSD1Set=1;
       Reset_VCIRQCount();
	   break;

  case 0xe1:
       memcpy(CryptInfo.MSG0,&buf[0],10); 
	   CryptInfo.MSG0Set=1;
	   break;

  case 0x1e:
       memcpy(CryptInfo.MSG1,&buf[0],10); 
	   CryptInfo.MSG1Set=1;
	   break;
  default:
	   break;
  }

if ((CryptInfo.LastCryptMode == 2 ) && ( VCIrqCount < 16 )) CryptInfo.CryptMode=CryptInfo.LastCryptMode;
if ((CryptInfo.LastCryptMode == 3 ) && ( VCIrqCount < 96 )) CryptInfo.CryptMode=CryptInfo.LastCryptMode;

}

void vc2dump(unsigned char *buf)
{
  switch (buf[0]) {
  case 0x08:
       memcpy(CryptInfo.OSD0,&buf[0],10); 
	   CryptInfo.OSD0Set=1;
	break;

  case 0x80:
       memcpy(CryptInfo.OSD1,&buf[0],10); 
	   CryptInfo.OSD1Set=1;
	   break;

  case 0xe1:
       memcpy(CryptInfo.MSG0,&buf[0],10); 
	   CryptInfo.MSG0Set=1;
	   break;

  case 0x1e:
       memcpy(CryptInfo.MSG1,&buf[0],10); 
	   CryptInfo.MSG1Set=1;
	   break;
  default:
	break;
  }
}

void Winbis_Exit(void)	
{
if (winbisfp != NULL ) fclose(winbisfp);
}

void Init_Winbis(void)	
{
 char Zeile[255];
   
   sprintf(Zeile,"%s\\Winbis.txt",WINIBIS_DIR);
   
   winbisfp=fopen(Zeile, "wb");
   
}


void VideoDat_Init( void ) {
    VDat.Pos=0;
	VDat.eLBN=0x00;
	VDat.CRCError=0;
	VDat.BlockSoFar=0;
	VDat.Lenght=0;
	VDat.Interleave = 0;
    VDat.Blockzaehler = 0;
	VDat.Status = 0;
    VDat.LastKillPos = 0;
	VDat.Pos = 0;
	VDat.FileName[0]=0x00;
    VDat.FileSize=0;
	VDat.FilePtr=NULL;
	VDat.Index=0;
	VDat.BlocksError=0;
    VDat.BlocksOK=0;
	strcpy(Current_VD_DIR,VD_DIR);
  };


void VideoDat_Exit( void ) {
    VideoDat_Flush();
	VDat.BlocksError=0;
    VDat.BlocksOK=0;
  };


 
void VBI_AGC(BYTE *Buffer, int start, int stop, int step) {
    int i, min=255, max=0;
    
    for (i=start; i<stop; i+=step) {
      if (Buffer[i]<min) min=Buffer[i];
      else if (Buffer[i]>max) max=Buffer[i];
    }
    VBI_thresh=(max+min)/2;
    VBI_off=128-VBI_thresh;
  }


void VBI_decode_vt(unsigned char *dat)
{

  int i;
  unsigned char mag, pack, mpag;
  unsigned int pnum=0;

  /* dat: 55 55 27 %MPAG% */
  mpag=unham(dat+3);
  mag=mpag&7;

  pack=(mpag>>3)&0x1f;


 if (VBI_Flags&VBI_WINBIS ) {
	 if ( winbisfp == NULL ) Init_Winbis();
   if(pack==0x1f && mag==3 && unham(dat+5)==0x14) { 
                  winbis_decoder(dat);
				  return;

   };
 };

 if ( VBI_Flags&VBI_VT ) { 
	 if (( pack >= 0 ) && ( pack <=24 )) {
     if ( pack == 0 ) {
	 Work_Text(&BT_VT_Buffer[0]);
     if (((unhamtab[*(dat+8)]&0x8) == 0x08 ) || 
		 ((unhamtab[*(dat+11)]&0x2) == 0x02 )) memset(&BT_VT_Buffer[0],0x00,1010); // erase Page
	 for ( i = 0 ; i < 8; i++) BT_VT_Buffer[1000+i]=unhamtab[*(dat+5+i)];
	 BT_VT_Buffer[1000] |= mag<<4;
     };
	 memcpy(&BT_VT_Buffer[pack*40],(dat+5),40);
     return;
 } else if ( pack == 30 ) {
	  StorePacket30(dat);
 }
}
};


void StorePacket30(BYTE *p)
{
	DWORD d,b;
	BYTE h,m,s,a,CNI0,CNI1,CNI2,CNI3;
    char Text[20];
	int n;
	if(*p!=0x55) return; // Some error, the data should be here...
	p+=5;
	
	if ( ShowPDCInfo != NULL ) {

	
	}
	
	if(unhamtab[*p]==0) // TSDP
	{
		p++;
		Packet30.HomePage.nPage=unham(p);
		Packet30.HomePage.nSubcode=(((unhamtab[p[5]]&0x3)<<12)+(unhamtab[p[4]]<<8)+
					((unhamtab[p[3]]&0x7)<<4)+unhamtab[p[2]]);

		Packet30.HomePage.nMag=((unhamtab[p[5]]>>1)&0x6)+((unhamtab[p[3]]>>3)&0x1);
		p+=6;
		Packet30.NetId=(p[1]<<8)+p[0]; 
		p+=2;
		Packet30.UTC.Offset=((*p>>1)&0x1f)*(*p&0x40)?-1:1;
		p++;
		d=(((*p)-0x01)<<16)+(((*(p+1))-0x11)<<8)+((*(p+2))-0x11);
		Packet30.UTC.JulianDay=(((d&0xF0000)>>16)*10000)+
							   (((d&0x0F000)>>12)*1000)+
							   (((d&0x00F00)>>8)*100)+
							   (((d&0x000F0)>>4)*10)+
							   (d&0x0000F);
		p+=3;
		h=(*p)-0x11;
		m=(*(p+1))-0x11;
		s=(*(p+2))-0x11;
		Packet30.UTC.Hour=(h>>4)*10+(h&0x0f);
		Packet30.UTC.Min=(m>>4)*10+(m&0x0f);
		Packet30.UTC.Sec=(s>>4)*10+(s&0x0f);
		p+=3;
		for(n=0; n<4; n++) Packet30.Unknown[n]=p[n]&0x7f;
		Packet30.Unknown[n]='\0';
		p+=4;
		for(n=0; n<20; n++) Packet30.Identifier[n]=p[n]&0x7f;
		Packet30.Identifier[n]='\0';

	if ( ShowPDCInfo != NULL ) {
                   SetDlgItemText(ShowPDCInfo,TEXT1,Packet30.Identifier);
                   SetDlgItemText(ShowPDCInfo,TEXT2,Packet30.Unknown);
                   sprintf(Text,"%d",Packet30.NetId);
				   SetDlgItemText(ShowPDCInfo,TEXT3,Text);
                   sprintf(Text,"%d",Packet30.HomePage.nMag);
				   SetDlgItemText(ShowPDCInfo,TEXT4,Text);
                   sprintf(Text,"%d",Packet30.HomePage.nPage);
				   SetDlgItemText(ShowPDCInfo,TEXT5,Text);
                   sprintf(Text,"%d",Packet30.HomePage.nSubcode);
				   SetDlgItemText(ShowPDCInfo,TEXT6,Text);
                   sprintf(Text,"%02x",Packet30.UTC.Offset);
				   SetDlgItemText(ShowPDCInfo,TEXT7,Text);
                   sprintf(Text,"%d",Packet30.UTC.JulianDay);
				   SetDlgItemText(ShowPDCInfo,TEXT8,Text);
                   sprintf(Text,"%02d",Packet30.UTC.Hour);
				   SetDlgItemText(ShowPDCInfo,TEXT9,Text);
                   sprintf(Text,"%02d",Packet30.UTC.Min);
				   SetDlgItemText(ShowPDCInfo,TEXT10,Text);
                   sprintf(Text,"%02d",Packet30.UTC.Sec);
				   SetDlgItemText(ShowPDCInfo,TEXT11,Text);
	}
	}
	else if(unhamtab[*p]==2) // PDC
	{
		p++;
		Packet30.HomePage.nPage=unham(p);
		Packet30.HomePage.nSubcode=(((unhamtab[p[5]]&0x3)<<12)+(unhamtab[p[4]]<<8)+
					((unhamtab[p[3]]&0x7)<<4)+unhamtab[p[2]]);

		Packet30.HomePage.nMag=((unhamtab[p[5]]>>1)&0x6)+((unhamtab[p[3]]>>3)&0x1);
		p+=6;
		a=revham[unhamtab[p[0]]];
		Packet30.PDC.LCI=GetBit(a,2,3);
		Packet30.PDC.LUF=GetBit(a,1,1);
		Packet30.PDC.PRF=GetBit(a,0,1);
		a=revham[unhamtab[p[1]]];
		Packet30.PDC.PCS=GetBit(a,2,3);
		Packet30.PDC.MI=GetBit(a,1,1);
		CNI0=revham[unhamtab[p[2]]];
		b=(revham[unhamtab[p[3]]]<<28)+(revham[unhamtab[p[4]]]<<24)+
		  (revham[unhamtab[p[5]]]<<20)+(revham[unhamtab[p[6]]]<<16)+
		  (revham[unhamtab[p[7]]]<<12)+(revham[unhamtab[p[8]]]<<8)+
		  (revham[unhamtab[p[9]]]<<4)+(revham[unhamtab[p[10]]]);
		CNI2=GetBit(b,30,3);
		Packet30.PDC.day=GetBit(b,25,0x1f);
		Packet30.PDC.month=GetBit(b,21,0xf);
		Packet30.PDC.hour=GetBit(b,16,0x1f);
		Packet30.PDC.minute=GetBit(b,10,0x3f);
		CNI1=GetBit(b,6,0xf);
		CNI3=GetBit(b,0,0x3f);
		Packet30.PDC.PTY=(revham[unhamtab[p[11]]]<<4)+revham[unhamtab[p[12]]];;
		Packet30.PDC.CNI=(CNI0<<12)+(CNI1<<8)+(CNI2<<4)+CNI3;
		p+=13;
		for(n=0; n<20; n++) Packet30.Identifier[n]=p[n]&0x7f;
		Packet30.Identifier[n]='\0';

	if ( ShowPDCInfo != NULL ) {
                   sprintf(Text,"%d",Packet30.PDC.LCI);
				   SetDlgItemText(ShowPDCInfo,TEXT12,Text);
                   sprintf(Text,"%d",Packet30.PDC.LUF);
				   SetDlgItemText(ShowPDCInfo,TEXT13,Text);
                   sprintf(Text,"%d",Packet30.PDC.PRF);
				   SetDlgItemText(ShowPDCInfo,TEXT14,Text);
                   sprintf(Text,"%d",Packet30.PDC.PCS);
				   SetDlgItemText(ShowPDCInfo,TEXT15,Text);
                   sprintf(Text,"%d",Packet30.PDC.MI);
				   SetDlgItemText(ShowPDCInfo,TEXT16,Text);
                   sprintf(Text,"%d",Packet30.PDC.day);
				   SetDlgItemText(ShowPDCInfo,TEXT17,Text);
                   sprintf(Text,"%d",Packet30.PDC.month);
				   SetDlgItemText(ShowPDCInfo,TEXT18,Text);
                   sprintf(Text,"%d",Packet30.PDC.hour);
				   SetDlgItemText(ShowPDCInfo,TEXT19,Text);
                   sprintf(Text,"%d",Packet30.PDC.minute);
				   SetDlgItemText(ShowPDCInfo,TEXT20,Text);
                   sprintf(Text,"%d",Packet30.PDC.CNI);
				   SetDlgItemText(ShowPDCInfo,TEXT21,Text);
                   sprintf(Text,"%d",Packet30.PDC.PTY);
				   SetDlgItemText(ShowPDCInfo,TEXT22,Text);
		}	
	}
}


BOOL APIENTRY ChipSettingProc
(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
int i;
char Text[80];
char Text1[80];
HRESULT		ddrval;
DDCAPS DriverCaps;
OSVERSIONINFO osvi;
char VS[128];   
WD_VERSION ver;
unsigned char MAC[8];


	switch (message) {
        case WM_INITDIALOG:

			


		osvi.dwOSVersionInfoSize=sizeof(osvi);
		if (GetVersionEx(&osvi) == TRUE ) {
	          sprintf(Text,"Major %d Minor %d Build %d",osvi.dwMajorVersion,osvi.dwMinorVersion,osvi.dwBuildNumber);
	          SetDlgItemText(hDlg,IDC_TEXT45,Text);

		}
		i=Get_Version_Informations(&VS[0]);
		SetDlgItemText(hDlg,IDC_TEXT44,VS);

 
		if ( NT == TRUE ) sprintf(Text,"MDHALDR.SYS");
		   else sprintf(Text,"MDHALDR.VXD");
		SetDlgItemText(hDlg,IDC_TEXT42,Text);
			if ( Has_BT == TRUE ) WD_Version(hBT8X8->hWD,&ver);
			   else if ( Has_DVB == TRUE ) WD_Version(hSAA7146->hWD,&ver);

			   sprintf(Text,"Version %d",ver.dwVer); 
               SetDlgItemText(hDlg,IDC_TEXT43,Text);

#ifdef USE_BT8X8
			   if ( Has_BT == TRUE ) {
            i=0;
			while ( (unsigned)i < hBT8X8->cardReg.Card.dwItems ) {
				if ( hBT8X8->cardReg.Card.Item[i].item == 2 ) {
                   sprintf(Text,"0x%08x-0x%08x",hBT8X8->cardReg.Card.Item[i].I.Mem.dwPhysicalAddr,hBT8X8->cardReg.Card.Item[i].I.Mem.dwPhysicalAddr+hBT8X8->cardReg.Card.Item[i].I.Mem.dwBytes);
                   SetDlgItemText(hDlg,IDC_TEXT12,Text);
				};
				if ( hBT8X8->cardReg.Card.Item[i].item == 1 ) {
					Text1[0]=0x00;
                   if (( hBT8X8->cardReg.Card.Item[i].I.Int.dwInterrupt >= 0 ) && 
					   ( hBT8X8->cardReg.Card.Item[i].I.Int.dwInterrupt < 256 )) {
					     if ( Used_IRQ_List[hBT8X8->cardReg.Card.Item[i].I.Int.dwInterrupt] == 1 ) strcpy(Text1,"<Kein IRQ-Sharing>");
						  else sprintf(Text1,"<IRQ SHARED  %2d-mal>",Used_IRQ_List[hBT8X8->cardReg.Card.Item[i].I.Int.dwInterrupt]);
				   }
				   sprintf(Text,"0x%02x %s",hBT8X8->cardReg.Card.Item[i].I.Int.dwInterrupt,Text1);
                   SetDlgItemText(hDlg,IDC_TEXT11,Text);
				};
		    i++;
			}

			SetDlgItemText(hDlg,IDC_TEXT6,BTVendorID);
			SetDlgItemText(hDlg,IDC_TEXT7,BTDeviceID);

			SetDlgItemText(hDlg,IDC_TEXT1,BTTyp);
			SetDlgItemText(hDlg,IDC_TEXT13,TunerStatus);
			SetDlgItemText(hDlg,IDC_TEXT14,MSPStatus);
			SetDlgItemText(hDlg,IDC_TEXT16,MSPVersion);
			SetDlgItemInt(hDlg,IDC_TEXT8,hBT8X8->pciSlot.dwBus,FALSE);
			SetDlgItemInt(hDlg,IDC_TEXT9,hBT8X8->pciSlot.dwSlot,FALSE);
			SetDlgItemInt(hDlg,IDC_TEXT10,hBT8X8->pciSlot.dwFunction,FALSE);

		if ( hBT8X8->Int.Int.fEnableOk == 1 ) {
					SetDlgItemText(hDlg,IDC_TEXT15,t("Interrupt geladen"));
			} else {
					SetDlgItemText(hDlg,IDC_TEXT15,t("Interrupt nicht geladen"));
			}

         if ( LoadExternBTDriver == FALSE ) {
        			SetDlgItemText(hDlg,IDC_TEXT37,t("Treibererweiterung soll nicht geladen werden"));
			} else {
				if ( ExternBTDriver == FALSE ) {
        			SetDlgItemText(hDlg,IDC_TEXT37,t("Treibererweiterung konnte nicht geladen werden"));
				} else {
                 BT8X8_KP_GET_DRIVER_VERSION(hBT8X8->hWD,hBT8X8->Int.Int.kpCall.hKernelPlugIn , &Text[0]);
				 SetDlgItemText(hDlg,IDC_TEXT37,Text);
				};

			};
			} else {
							SetDlgItemText(hDlg,IDC_TEXT1,t("Keine BT-Hardware gefunden"));
			};
#endif

			if ( Has_DVB == TRUE ) {
            i=0;

            SetDlgItemText(hDlg,IDC_TEXT46,MD_API_Version);

			while ( (unsigned)i < hSAA7146->cardReg.Card.dwItems ) {
				if ( hSAA7146->cardReg.Card.Item[i].item == 2 ) {
                   sprintf(Text,"0x%08x-0x%08x",hSAA7146->cardReg.Card.Item[i].I.Mem.dwPhysicalAddr,hSAA7146->cardReg.Card.Item[i].I.Mem.dwPhysicalAddr+hSAA7146->cardReg.Card.Item[i].I.Mem.dwBytes);
                   SetDlgItemText(hDlg,IDC_TEXT29,Text);
				};
				if ( hSAA7146->cardReg.Card.Item[i].item == 1 ) {
					Text1[0]=0x00;
                   if (( hSAA7146->cardReg.Card.Item[i].I.Int.dwInterrupt >= 0 ) && 
					   ( hSAA7146->cardReg.Card.Item[i].I.Int.dwInterrupt < 256 )) {
					     if ( Used_IRQ_List[hSAA7146->cardReg.Card.Item[i].I.Int.dwInterrupt] == 1 ) strcpy(Text1,"<Kein IRQ-Sharing>");
						  else sprintf(Text1,"<IRQ SHARED %2d-mal>",Used_IRQ_List[hSAA7146->cardReg.Card.Item[i].I.Int.dwInterrupt]);
				   }

					sprintf(Text,"0x%02x  %s",hSAA7146->cardReg.Card.Item[i].I.Int.dwInterrupt,Text1);
                   SetDlgItemText(hDlg,IDC_TEXT28,Text);
				};
		    i++;
			}

			SetDlgItemText(hDlg,IDC_TEXT23,DVBVendorID);
			SetDlgItemText(hDlg,IDC_TEXT24,DVBDeviceID);

			SetDlgItemText(hDlg,IDC_TEXT22,DVBTyp);
			SetDlgItemText(hDlg,IDC_TEXT33,VESTyp);
			SetDlgItemInt(hDlg,IDC_TEXT25,hSAA7146->pciSlot.dwBus,FALSE);
			SetDlgItemInt(hDlg,IDC_TEXT26,hSAA7146->pciSlot.dwSlot,FALSE);
			SetDlgItemInt(hDlg,IDC_TEXT27,hSAA7146->pciSlot.dwFunction,FALSE);


			if ( hSAA7146->Int.Int.fEnableOk == 1 ) {
					SetDlgItemText(hDlg,IDC_TEXT32,t("Interrupt geladen"));
			} else {
					SetDlgItemText(hDlg,IDC_TEXT32,t("Interrupt nicht geladen"));
			}
			
            if ( ci_interface == 0 ) {
               SetDlgItemText(hDlg,IDC_TEXT34,t("Kein CI-Interface gefunden"));
			} else {
               SetDlgItemText(hDlg,IDC_TEXT34,t("CI-Interface gefunden"));
				   if ( ci_module[0].Present == 0 ) SetDlgItemText(hDlg,IDC_TEXT38,t("Kein CICAM"));
                     else  if ( ci_module[0].Present == 1 ) SetDlgItemText(hDlg,IDC_TEXT38,t("CICAM gefunden"));
                       else  if ( ci_module[0].Present == 2 ) SetDlgItemText(hDlg,IDC_TEXT38,"CICAM OK");
				   if ( ci_module[1].Present == 0 ) SetDlgItemText(hDlg,IDC_TEXT39,t("Kein CICAM"));
                     else  if ( ci_module[1].Present == 1 ) SetDlgItemText(hDlg,IDC_TEXT39,t("CICAM gefunden"));
                       else  if ( ci_module[1].Present == 2 ) SetDlgItemText(hDlg,IDC_TEXT39,"CICAM OK");

			   };

            sprintf(Text,"0x%04x",TunerStatusByte);
			SetDlgItemText(hDlg,IDC_TEXT31,Text);

            Get_DVB_MacAdr(&MAC[0]);
			sprintf(Text,"%02X-%02X-%02X-%02X-%02X-%02X",MAC[0],MAC[1],MAC[2],MAC[3],MAC[4],MAC[5]);
            SetDlgItemText(hDlg,IDC_TEXT40,Text);

            SAA7146_KP_GET_DRIVER_VERSION(hSAA7146->hWD,hSAA7146->Int.Int.kpCall.hKernelPlugIn , &Text[0]);
			SetDlgItemText(hDlg,IDC_TEXT36,Text);


			if ( DVB_AUDIO == 0 ) SetDlgItemText(hDlg,IDC_TEXT35,"TI-DAC");
			 else SetDlgItemText(hDlg,IDC_TEXT35,"Crystal-Audio-DAC");

			} else {
							SetDlgItemText(hDlg,IDC_TEXT22,t("Keine DVB-Hardware gefunden"));
			};

            if ( lpSurface != NULL ) {
				if ( WTVBPP == 16 ) SetDlgItemText(hDlg,IDC_TEXT17,t("Verfügbar RGB565(16-Bit)"));
				  else if ( WTVBPP == 15 ) SetDlgItemText(hDlg,IDC_TEXT17,t("Verfügbar RGB555(15-Bit)"));
       				  else  SetDlgItemText(hDlg,IDC_TEXT17,t("Verfügbar aber nicht 15-Bit/16-Bit "));
			} else SetDlgItemText(hDlg,IDC_TEXT17,t("Nicht Verfügbar"));

		   memset(&DriverCaps,0x00,sizeof(DriverCaps));
	       DriverCaps.dwSize = sizeof(DriverCaps);
	       ddrval = IDirectDraw_GetCaps(lpDD,&DriverCaps,NULL);
           if ( ddrval != DD_OK ) DriverCaps.dwCaps=0xffffffff;
 
            if ( Can_Overlay == TRUE ) {
			    SetDlgItemText(hDlg,IDC_TEXT18,t("Verfügbar YUV2"));
			    if ( Can_Overlay_Strech ) SetDlgItemText(hDlg,IDC_TEXT19,t("Verfügbar"));
				 else SetDlgItemText(hDlg,IDC_TEXT19,t("Kein Overlay-Hardware-Streching"));
		   } else {
			   SetDlgItemText(hDlg,IDC_TEXT18,t("Nicht Verfügbar"));
		       sprintf(Text,"DirectX Caps 0x%08x",DriverCaps.dwCaps);
			   SetDlgItemText(hDlg,IDC_TEXT19,Text);
		   };

			break;


		case WM_COMMAND:


			if ( (LOWORD(wParam) == IDOK ) || (LOWORD(wParam) == IDCANCEL )) {
				EndDialog(hDlg,TRUE);
			}


			 break;
    }

return(FALSE);
}

#ifdef USE_BT8X8
BOOL APIENTRY IFormSettingProc
(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
int i;
BYTE IFORM,IFORM1;

char Text[40];
   
	switch (message) {
        case WM_INITDIALOG:

			CheckDlgButton(hDlg, IDC_RADIO1, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO2, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO3, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO4, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO5, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO6, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO7, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO8, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO9, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO10, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO11, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO12, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO13, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO14, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO15, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO16, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO17, FALSE);
			CheckDlgButton(hDlg, IDC_RADIO18, FALSE);

            if ( InitialIFORM != 0 ) EnableWindow(GetDlgItem(hDlg,IDC_BUTTON2),TRUE);   			

			IFORM=BT8X8_ReadByte(hBT8X8, BT8X8_AD_BAR0,BT848_IFORM);
			SaveIFORM=IFORM;

			if ( InitialIFORM != 0 ) {
				SetDlgItemText(hDlg,IDC_TEXT1,"Fix vergeben");
				sprintf(Text,"0x%02x",InitialIFORM);
				SetDlgItemText(hDlg,IDC_TEXT2,Text);

			} else {

				SetDlgItemText(hDlg,IDC_TEXT1,t("Automatisch"));
				sprintf(Text,"0x%02x",IFORM);
				SetDlgItemText(hDlg,IDC_TEXT2,Text);

			}
			
			IFORM1=IFORM;
            i=(IFORM1 >> 7);
			if (i == 0 ) CheckDlgButton(hDlg, IDC_RADIO1, TRUE);
              else CheckDlgButton(hDlg, IDC_RADIO2, TRUE);
            
			IFORM1=(IFORM << 1);
    	    i=(IFORM1 >> 6);

			if ( i == 0 ) CheckDlgButton(hDlg, IDC_RADIO3, TRUE);
             else if ( i == 1 ) CheckDlgButton(hDlg, IDC_RADIO4, TRUE);
  			  else if ( i == 2 ) CheckDlgButton(hDlg, IDC_RADIO5, TRUE);
                else if ( i == 3 ) CheckDlgButton(hDlg, IDC_RADIO6, TRUE);

			IFORM1=(IFORM << 3);
    	    i=(IFORM1 >> 6);

			if ( i == 0 ) CheckDlgButton(hDlg, IDC_RADIO7, TRUE);
             else if ( i == 1 ) CheckDlgButton(hDlg, IDC_RADIO8, TRUE);
  			  else if ( i == 2 ) CheckDlgButton(hDlg, IDC_RADIO9, TRUE);
                else if ( i == 3 ) CheckDlgButton(hDlg, IDC_RADIO10, TRUE);

			IFORM1=(IFORM << 5);
    	    i=(IFORM1 >> 5);

			if ( i == 0 ) CheckDlgButton(hDlg, IDC_RADIO11, TRUE);
             else if ( i == 1 ) CheckDlgButton(hDlg, IDC_RADIO12, TRUE);
  			  else if ( i == 2 ) CheckDlgButton(hDlg, IDC_RADIO13, TRUE);
                else if ( i == 3 ) CheckDlgButton(hDlg, IDC_RADIO14, TRUE);

			  break;


		case WM_COMMAND:

            IFORM=0x00;
			if (IsDlgButtonChecked(hDlg, IDC_RADIO2))  IFORM=IFORM+128;

			if (IsDlgButtonChecked(hDlg, IDC_RADIO4)) IFORM=IFORM+32;
              else if (IsDlgButtonChecked(hDlg, IDC_RADIO5)) IFORM=IFORM+64;
               else if (IsDlgButtonChecked(hDlg, IDC_RADIO6)) IFORM=IFORM+96;

			if (IsDlgButtonChecked(hDlg, IDC_RADIO8)) IFORM=IFORM+8;
              else if (IsDlgButtonChecked(hDlg, IDC_RADIO9)) IFORM=IFORM+16;
               else if (IsDlgButtonChecked(hDlg, IDC_RADIO10)) IFORM=IFORM+24;


			if (IsDlgButtonChecked(hDlg, IDC_RADIO12)) IFORM=IFORM+1;
              else if (IsDlgButtonChecked(hDlg, IDC_RADIO13)) IFORM=IFORM+2;
               else if (IsDlgButtonChecked(hDlg, IDC_RADIO14)) IFORM=IFORM+3;
                 else if (IsDlgButtonChecked(hDlg, IDC_RADIO15)) IFORM=IFORM+4;
                   else if (IsDlgButtonChecked(hDlg, IDC_RADIO16)) IFORM=IFORM+5;
                     else if (IsDlgButtonChecked(hDlg, IDC_RADIO17)) IFORM=IFORM+6;
                        else if (IsDlgButtonChecked(hDlg, IDC_RADIO18)) IFORM=IFORM+7;

				sprintf(Text,"0x%02x",IFORM);
				SetDlgItemText(hDlg,IDC_TEXT2,Text);

               BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_IFORM,IFORM);


			if (LOWORD(wParam) == IDCANCEL ) {
                BT8X8_WriteByte(hBT8X8, BT8X8_AD_BAR0,BT848_IFORM,SaveIFORM);
				EndDialog(hDlg,TRUE);
			}

			if (LOWORD(wParam) == IDOK ) {
                    EndDialog(hDlg,TRUE);
}

			if (LOWORD(wParam) == IDC_BUTTON1 ) {
			
			    if (MessageBox(hDlg,"Achtung : Die Vergabe von einen fixen Iform Parameter hat folgende Nebenwirkungen !\n\n\
    1.) Die Sourcenumschaltung über das Menü Videoeingang geht nicht mehr. ( Nur über IFORM-Menü möglich )\n\
                  Der TunerTyp kann weiterhin verändert werden, wenn auf Tuner eingestellt ist\n\
    2.) Die Videoformat -> Typeinstellung geht nicht mehr.  ( Nur über IFORM-Menü möglich )\n\n\
Das Löschen dieser fixen Einstellung kann später in diesem Dialog erfolgen","Multi-Decoder",MB_ICONSTOP|MB_OKCANCEL) == IDOK ) {
					InitialIFORM=IFORM;
        			EndDialog(hDlg,TRUE);
			}
            };

			if (LOWORD(wParam) == IDC_BUTTON2 ) {
					InitialIFORM=0;
        			EndDialog(hDlg,TRUE);
			}


			 break;
    }

return(FALSE);
}

BOOL APIENTRY PLLSettingProc
(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
  
	switch (message) {
        case WM_INITDIALOG:

		 if ( INIT_PLL == 1 ) CheckDlgButton(hDlg, IDC_CHECK1, TRUE);
		   else if ( INIT_PLL == 2 ) CheckDlgButton(hDlg, IDC_CHECK2, TRUE);
		break;


		case WM_COMMAND:

            if ( LOWORD(wParam) == IDC_CHECK1 ) {
				if ( IsDlgButtonChecked(hDlg, IDC_CHECK1) == TRUE ) { 
				 CheckDlgButton(hDlg, IDC_CHECK2, FALSE);
				};
			} else if ( LOWORD(wParam) == IDC_CHECK2 ) {
				if ( IsDlgButtonChecked(hDlg, IDC_CHECK2) == TRUE ) { 
				 CheckDlgButton(hDlg, IDC_CHECK1, FALSE);
				};
			}


			if (LOWORD(wParam) == IDOK ) {
                INIT_PLL=0;
			if ( IsDlgButtonChecked(hDlg, IDC_CHECK1) == TRUE ) INIT_PLL=1;
             else if ( IsDlgButtonChecked(hDlg, IDC_CHECK2) == TRUE ) INIT_PLL=2;
             Set_PLL(INIT_PLL);
            EndDialog(hDlg,TRUE);
			break;  
}

      if (LOWORD(wParam) == IDCANCEL ) {
           EndDialog(hDlg,TRUE);
          break;  
}
 }

return(FALSE);
}


BOOL APIENTRY TunerSettingProc
(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{
int i;


char Text[128];
float ffreq;
unsigned int freq;

	switch (message) {
        case WM_INITDIALOG:
			SaveTuner=TunerType;
			SaveVideoSource=VideoSource;
            SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_RESETCONTENT , 0, 0 );
            SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING , 0, (LONG)(LPSTR)"Temic Pal");
            SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING , 0, (LONG)(LPSTR)"Philips Pal I");
            SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING , 0, (LONG)(LPSTR)"Philips NTSC");
            SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING , 0, (LONG)(LPSTR)"Philips SECAM");
            SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING , 0, (LONG)(LPSTR)"Kein Tuner");
            SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING , 0, (LONG)(LPSTR)"Philips Pal");
            SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING , 0, (LONG)(LPSTR)"Temic NTSC");
            SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING , 0, (LONG)(LPSTR)"Temic Pal I");
			SendMessage (GetDlgItem(hDlg,IDC_COMBO1), CB_SETCURSEL  , TunerType, 0); 
			sprintf(Text,"%10.3f",(float)Programm[InitialProg].freq/1000);
            SetDlgItemText(hDlg,IDC_FREQ,Text);
            PostMessage(hDlg,WM_USER,1,0);
            break;
case WM_USER:

	if ( wParam == 1 ) {
            i=SendMessage(GetDlgItem(hDlg,IDC_COMBO1),CB_GETCURSEL,0,0);
			if (( i >= 0 ) && ( i < 8 )) {
		    SetDlgItemInt(hDlg,IDT_THRESH1,Tuners[i].thresh1,FALSE);
            SetDlgItemInt(hDlg,IDT_THRESH2,Tuners[i].thresh2,FALSE);
            SetDlgItemInt(hDlg,IDT_VHF_L,Tuners[i].VHF_L,FALSE);
            SetDlgItemInt(hDlg,IDT_VHF_H,Tuners[i].VHF_H,FALSE);
            SetDlgItemInt(hDlg,IDT_UHF,Tuners[i].UHF,FALSE);
            SetDlgItemInt(hDlg,IDT_CONFIG,Tuners[i].config,FALSE);
            SetDlgItemInt(hDlg,IDT_I2C,Tuners[i].I2C,FALSE);
            SetDlgItemInt(hDlg,IDT_IFPCoff,Tuners[i].IFPCoff,FALSE);
            };
            SetDlgItemInt(hDlg,IDM_THRESH1,Tuners[8].thresh1,FALSE);
            SetDlgItemInt(hDlg,IDM_THRESH2,Tuners[8].thresh2,FALSE);
            SetDlgItemInt(hDlg,IDM_VHF_L,Tuners[8].VHF_L,FALSE);
            SetDlgItemInt(hDlg,IDM_VHF_H,Tuners[8].VHF_H,FALSE);
            SetDlgItemInt(hDlg,IDM_UHF,Tuners[8].UHF,FALSE);
            SetDlgItemInt(hDlg,IDM_CONFIG,Tuners[8].config,FALSE);
            SetDlgItemInt(hDlg,IDT_I2C2,Tuners[8].I2C,FALSE);
            SetDlgItemInt(hDlg,IDM_IFPCoff,Tuners[8].IFPCoff,FALSE);
	};


	if ( wParam == 2 ) {
		if ( TunerType != 8 ) {
			 TunerType=8;
             Init_Tuner(TunerType);
		};
		if (  VideoSource != 0 ) {
			 VideoSource = 0;   
			 SetVideoSource(VideoSource);
		};

		    Tuners[8].thresh1=GetDlgItemInt(hDlg,IDM_THRESH1,NULL,FALSE);
            Tuners[8].thresh2=GetDlgItemInt(hDlg,IDM_THRESH2,NULL,FALSE);
            Tuners[8].VHF_L=GetDlgItemInt(hDlg,IDM_VHF_L,NULL,FALSE);
            Tuners[8].VHF_H=GetDlgItemInt(hDlg,IDM_VHF_H,NULL,FALSE);
            Tuners[8].UHF=GetDlgItemInt(hDlg,IDM_UHF,NULL,FALSE);
            Tuners[8].config=GetDlgItemInt(hDlg,IDM_CONFIG,NULL,FALSE);
            Tuners[8].IFPCoff=GetDlgItemInt(hDlg,IDM_IFPCoff,NULL,FALSE);
            GetDlgItemText(hDlg,IDC_FREQ,Text,sizeof(Text));
 		    ffreq=(float)atof(Text);
            freq=(unsigned	long)(ffreq*1000000);
		    Tuner_SetFrequency(TunerType,MulDiv(freq,16,1000000));
            SendMessage(hDlg,WM_USER,1,0);
	};

            break;
		case WM_COMMAND:

			if (LOWORD(wParam) == IDC_COMBO1 ) {
				if ( HIWORD( wParam ) == CBN_SELCHANGE  ) {
                     SendMessage(hDlg,WM_USER,1,0);
				}
			}


			if (LOWORD(wParam) == IDSET ) {
              SendMessage(hDlg,WM_USER,2,0);
              SendMessage(hDlg,WM_USER,1,0);
			}
			

			if (LOWORD(wParam) == IDOK ) {
			   CheckMenuItem( GetMenu(hWnd), TunerType+1100, MF_CHECKED );
			   CheckMenuItem( GetMenu(hWnd), SaveTuner+1100, MF_UNCHECKED );
			   CheckMenuItem( GetMenu(hWnd), IDM_EXTERN1, MF_UNCHECKED );
			   CheckMenuItem( GetMenu(hWnd), IDM_EXTERN2, MF_UNCHECKED );
			   CheckMenuItem( GetMenu(hWnd), IDM_EXTERN3, MF_UNCHECKED );
               GetDlgItemText(hDlg,IDC_FREQ,Text,sizeof(Text));
    		   ffreq=(float)atof(Text);
    		   EndDialog(hDlg,TRUE);
			}
			
			if (LOWORD(wParam) == IDCANCEL ) {
              VideoSource=SaveVideoSource;
              TunerType=SaveTuner;
              Init_Tuner(TunerType);
			  SetVideoSource(VideoSource);
              Tuner_SetFrequency(TunerType,MulDiv(Programm[InitialProg].freq*1000,16,1000000));
              EndDialog(hDlg,TRUE);
			}
			 break;
    }

return(FALSE);
}

BOOL APIENTRY CardSettingProc
(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{

	switch (message) {
        case WM_INITDIALOG:
			SaveTuner=CardType;
			SaveVideoSource=AudioSource;
            PostMessage(hDlg,WM_USER,1,0);
            break;

		
case WM_USER:

	if ( wParam == 1 ) {
		    SetDlgItemInt(hDlg,IDC_EDIT1,ManuellAudio[0],FALSE);
            SetDlgItemInt(hDlg,IDC_EDIT2,ManuellAudio[1],FALSE);
            SetDlgItemInt(hDlg,IDC_EDIT3,ManuellAudio[2],FALSE);
            SetDlgItemInt(hDlg,IDC_EDIT4,ManuellAudio[3],FALSE);
            SetDlgItemInt(hDlg,IDC_EDIT5,ManuellAudio[4],FALSE);
            SetDlgItemInt(hDlg,IDC_EDIT6,ManuellAudio[5],FALSE);
            SetDlgItemInt(hDlg,IDC_EDIT7,ManuellAudio[6],FALSE);
            SetDlgItemInt(hDlg,IDC_EDIT8,ManuellAudio[7],FALSE);

	};


            break;
		case WM_COMMAND:


			if (LOWORD(wParam) == IDSET ) {
            CardType=6;
			AudioSource=0;
            if (IsDlgButtonChecked(hDlg,IDC_RADIO1)) AudioSource=0;
            if (IsDlgButtonChecked(hDlg,IDC_RADIO2)) AudioSource=1;
            if (IsDlgButtonChecked(hDlg,IDC_RADIO3)) AudioSource=2;
            if (IsDlgButtonChecked(hDlg,IDC_RADIO4)) AudioSource=3;
            if (IsDlgButtonChecked(hDlg,IDC_RADIO5)) AudioSource=4;
            if (IsDlgButtonChecked(hDlg,IDC_RADIO6)) AudioSource=5;
			ManuellAudio[0]=GetDlgItemInt(hDlg,IDC_EDIT1,NULL,FALSE);
		    ManuellAudio[1]=GetDlgItemInt(hDlg,IDC_EDIT2,NULL,FALSE);
		    ManuellAudio[2]=GetDlgItemInt(hDlg,IDC_EDIT3,NULL,FALSE);
		    ManuellAudio[3]=GetDlgItemInt(hDlg,IDC_EDIT4,NULL,FALSE);
		    ManuellAudio[4]=GetDlgItemInt(hDlg,IDC_EDIT5,NULL,FALSE);
		    ManuellAudio[5]=GetDlgItemInt(hDlg,IDC_EDIT6,NULL,FALSE);
		    ManuellAudio[6]=GetDlgItemInt(hDlg,IDC_EDIT7,NULL,FALSE);
		    ManuellAudio[7]=GetDlgItemInt(hDlg,IDC_EDIT8,NULL,FALSE);
            SetAudioSource(AudioSource);            
			}
			

			if (LOWORD(wParam) == IDOK ) {
			ManuellAudio[0]=GetDlgItemInt(hDlg,IDC_EDIT1,NULL,FALSE);
		    ManuellAudio[1]=GetDlgItemInt(hDlg,IDC_EDIT2,NULL,FALSE);
		    ManuellAudio[2]=GetDlgItemInt(hDlg,IDC_EDIT3,NULL,FALSE);
		    ManuellAudio[3]=GetDlgItemInt(hDlg,IDC_EDIT4,NULL,FALSE);
		    ManuellAudio[4]=GetDlgItemInt(hDlg,IDC_EDIT5,NULL,FALSE);
		    ManuellAudio[5]=GetDlgItemInt(hDlg,IDC_EDIT6,NULL,FALSE);
		    ManuellAudio[6]=GetDlgItemInt(hDlg,IDC_EDIT7,NULL,FALSE);
     	    ManuellAudio[7]=GetDlgItemInt(hDlg,IDC_EDIT8,NULL,FALSE);

    		   CheckMenuItem( GetMenu(hWnd), SaveVideoSource+1110, MF_UNCHECKED );
			   CheckMenuItem( GetMenu(hWnd), AudioSource+1110, MF_CHECKED );
			   CheckMenuItem( GetMenu(hWnd), SaveTuner+1080, MF_UNCHECKED );
			   CheckMenuItem( GetMenu(hWnd), CardType+1080, MF_CHECKED );
				EndDialog(hDlg,TRUE);
			}
			
			if (LOWORD(wParam) == IDCANCEL ) {
			  CardType=SaveTuner;
 			  AudioSource=SaveVideoSource;
              SetAudioSource(AudioSource);            
              EndDialog(hDlg,TRUE);
			}
			 break;
    }

return(FALSE);
}

BOOL APIENTRY LowSettingProc
(
    HWND hDlg,
    UINT message,
    UINT wParam,
    LONG lParam)
{

	switch (message) {
        case WM_INITDIALOG:
		    SetDlgItemInt(hDlg,IDC_LOW,InitialLow,FALSE);
			SaveTuner=InitialLow;
            break;
		case WM_COMMAND:
			
			if (LOWORD(wParam) == IDOK ) {
                InitialLow=GetDlgItemInt(hDlg,IDC_LOW,NULL,FALSE);
				SetBrightness(InitialBrightness);
				EndDialog(hDlg,TRUE);
			}

			if (LOWORD(wParam) == IDDEFAULT ) {
				InitialLow=45;
				SetDlgItemInt(hDlg,IDC_LOW,InitialLow,FALSE);
				SetBrightness(InitialBrightness);
			}

			if (LOWORD(wParam) == IDCANCEL ) {
			  InitialLow=SaveTuner;
              EndDialog(hDlg,TRUE);
			}
			 break;
    }

return(FALSE);
}

BOOL MSP_Reset()
{
	BOOL ret = TRUE;
	I2CBus_Lock();
    I2CBus_Start();
    I2CBus_SendByte(AudioDeviceWrite,5);
	I2CBus_SendByte(0x00,0);
	I2CBus_SendByte(0x80,0);
	I2CBus_SendByte(0x00,0);
    I2CBus_Stop();
    I2CBus_Start();
    if (( I2CBus_SendByte(AudioDeviceWrite,5) == FALSE ) ||
		( I2CBus_SendByte(0x00,0) == FALSE ) ||
		( I2CBus_SendByte(0x00,0) == FALSE ) ||
		( I2CBus_SendByte(0x00,0) == FALSE )) ret=FALSE;

    I2CBus_Stop();
	I2CBus_Unlock();
	return ret;
}

void MSP_SetCarrier(int cdo1, int cdo2)
{
	WriteDem(0x93, cdo1 & 0xfff);
	WriteDem(0x9b, cdo1 >> 12);
	WriteDem(0xa3, cdo2 & 0xfff);
	WriteDem(0xab, cdo2 >> 12);
}

void MSP_SetMode(int type)
{
	int i;

	CheckMenuItem( GetMenu(hWnd), 1600+MSPMode, MF_UNCHECKED );
	MSPMode=type;
    CheckMenuItem( GetMenu(hWnd), 1600+MSPMode, MF_CHECKED );


	WriteDem(0xbb,msp_init_data[type].ad_cv);
    
	for (i = 5; i >= 0; i--) WriteDem( 0x01,msp_init_data[type].fir1[i]);
    
	WriteDem( 0x05, 0x0004); /* fir 2 */
	WriteDem( 0x05, 0x0040);
	WriteDem( 0x05, 0x0000);

	for (i = 5; i >= 0; i--) WriteDem( 0x05,msp_init_data[type].fir2[i]);
    
	WriteDem( 0x83,msp_init_data[type].mode_reg);
    
	MSP_SetCarrier( msp_init_data[type].cdo1,msp_init_data[type].cdo2);
    
	WriteDem( 0x60, 0); /* LOAD_REG_1/2 */
	WriteDSP( 0x08,msp_init_data[type].dfp_src);
	WriteDSP( 0x09,msp_init_data[type].dfp_src);
	WriteDSP( 0x0a,msp_init_data[type].dfp_src);
	WriteDSP( 0x0e,msp_init_data[type].dfp_matrix);

// msp3410 needs some more initialization 
if ( MSPNicam ) WriteDSP(0x10, 0x3000);

}

void MSP_SetStereo(int MajorMode, int MinorMode, int mode)
{
     int nicam=0; 

     CheckMenuItem( GetMenu(hWnd), 1630+MSPStereo, MF_UNCHECKED );
	 MSPStereo=mode;
     CheckMenuItem( GetMenu(hWnd), 1630+MSPStereo, MF_CHECKED );

	// switch demodulator 
	switch (MSPMode) {
	case MSP_MODE_FM_TERRA:
 		MSP_SetCarrier(carrier_detect[MinorMode], carrier_detect_main[MajorMode]);
		switch (MSPStereo) {
		case VIDEO_SOUND_STEREO:
				WriteDSP(0x0e, 0x3001);
			break;
		case VIDEO_SOUND_MONO:
		case VIDEO_SOUND_LANG1:
		case VIDEO_SOUND_LANG2:
				WriteDSP( 0x0e, 0x3000);
			break;
		}
		break;
	case MSP_MODE_FM_SAT:
		switch (MSPStereo) {
		case VIDEO_SOUND_MONO:
            MSP_SetCarrier( MSP_CARRIER(6.5), MSP_CARRIER(6.5));
			break;
		case VIDEO_SOUND_STEREO:
			MSP_SetCarrier(  MSP_CARRIER(7.2), MSP_CARRIER(7.02));
			break;
		case VIDEO_SOUND_LANG1:
			MSP_SetCarrier( MSP_CARRIER(7.38), MSP_CARRIER(7.02));
			break;
		case VIDEO_SOUND_LANG2:
			MSP_SetCarrier(  MSP_CARRIER(7.38), MSP_CARRIER(7.02));
			break;
		}
		break;
	case MSP_MODE_FM_NICAM1:
	case MSP_MODE_FM_NICAM2:
 		MSP_SetCarrier(carrier_detect[MinorMode], carrier_detect_main[MajorMode]);
		nicam=0x0100;
		break;
	default:
		// can't do stereo - abort here 
		return;
	}

	// switch audio 
	switch (MSPStereo) {
	case VIDEO_SOUND_STEREO:
		WriteDSP( 0x08,0x0020|nicam);
		WriteDSP( 0x09,0x0020|nicam);
		WriteDSP( 0x0a,0x0020|nicam);
		WriteDSP( 0x05,0x4000);
		break;
	case VIDEO_SOUND_MONO:
	case VIDEO_SOUND_LANG1:
		WriteDSP( 0x08,0x0000|nicam);
		WriteDSP( 0x09,0x0000|nicam);
		WriteDSP( 0x0a,0x0000|nicam);
		break;
	case VIDEO_SOUND_LANG2:
		WriteDSP( 0x08,0x0010|nicam);
		WriteDSP( 0x09,0x0010|nicam);
		WriteDSP( 0x0a,0x0010|nicam);
		break;
	}
}


BOOL MSP_Version()
{
	int              rev1,rev2;
    
	MSPNicam=FALSE;

	rev1 = ReadDSP( 0x1e);
	rev2 = ReadDSP( 0x1f);
	if (0 == rev1 && 0 == rev2) {
		return (FALSE);
	}
    MSPAutoDetectValue=3;

	sprintf(MSPVersion,"MSP34%02d%c-%c%d",(rev2>>8)&0xff, (rev1&0xff)+'@', ((rev1>>8)&0xff)+'@', rev2&0x1f);
	MSPNicam = (((rev2>>8)&0xff) != 00) ? 1 : 0;
	if ( MSPNicam == TRUE ) {
    MSPAutoDetectValue=5;

	}
    return(TRUE);
}


void MSP_Set_MajorMinor_Mode(int MajorMode,int MinorMode  )
{
     CheckMenuItem( GetMenu(hWnd), 1610+MSPMajorMode, MF_UNCHECKED );
	 MSPMajorMode=MajorMode;
     CheckMenuItem( GetMenu(hWnd), 1610+MSPMajorMode, MF_CHECKED );

     CheckMenuItem( GetMenu(hWnd), 1620+MSPMinorMode, MF_UNCHECKED );
	 MSPMinorMode=MinorMode;
     CheckMenuItem( GetMenu(hWnd), 1620+MSPMinorMode, MF_CHECKED );
        

		switch (MajorMode) {
		case 1: // 5.5 
			if (MinorMode == 0) {
				// B/G FM-stereo 
//				MSP_SetMode(MSP_MODE_FM_TERRA);
				MSP_SetStereo(MajorMode,MinorMode,VIDEO_SOUND_MONO);
			} else if (MinorMode == 1 && MSPNicam) {
				// B/G NICAM 
//				MSP_SetMode(MSP_MODE_FM_NICAM1);
				MSP_SetCarrier(carrier_detect[MinorMode], carrier_detect_main[MajorMode]);
			} else {
//			MSP_SetMode(MSP_MODE_FM_TERRA);
			MSP_SetCarrier(carrier_detect[MinorMode], carrier_detect_main[MajorMode]);
			}
			break;
		case 2: // 6.0 
			// PAL I NICAM 
//			MSP_SetMode(MSP_MODE_FM_NICAM2);
			MSP_SetCarrier(MSP_CARRIER(6.552), carrier_detect_main[MajorMode]);
			break;
		case 3: // 6.5 
			if (MinorMode == 1 || MinorMode == 2) {
				// D/K FM-stereo 
//				MSP_SetMode( MSP_MODE_FM_TERRA);
				MSP_SetStereo(MajorMode,MinorMode,VIDEO_SOUND_MONO);
			} else if (MinorMode == 0 && MSPNicam) {
				// D/K NICAM 
//				MSP_SetMode(MSP_MODE_FM_NICAM1);
				MSP_SetCarrier(carrier_detect[MinorMode], carrier_detect_main[MajorMode]);
			} else {
//			MSP_SetMode(MSP_MODE_FM_TERRA);
			MSP_SetCarrier(carrier_detect[MinorMode], carrier_detect_main[MajorMode]);
			}
				break;
		case 0: // 4.5 
		default:
//			MSP_SetMode(MSP_MODE_FM_TERRA);
			MSP_SetCarrier(carrier_detect[MinorMode], carrier_detect_main[MajorMode]);
			break;
		}

}
void MSP_Print_Mode()
{
	char Text[128];
   

    if ( Has_MSP == FALSE ) strcpy(Text,"Kein MSP-Audio-Device");
	else {

    switch(MSPMode) {
	   case 0 : strcpy(Text,"AM (msp3400)+"); break;
	   case 1 : strcpy(Text,"AM (msp3410)+"); break;
	   case 2 : strcpy(Text,"FM Radio+"); break;
	   case 3 : strcpy(Text,"TV Terrestial+"); break;
	   case 4 : strcpy(Text,"TV Sat+"); break;
	   case 5 : strcpy(Text,"NICAM B/G+"); break;
	   case 6 : strcpy(Text,"NICAM I+"); break;
	};		   


    switch(MSPMajorMode) {
	   case 0 : strcat(Text,"NTSC+"); break;
	   case 1 : strcat(Text,"PAL B/G+"); break;
	   case 2 : strcat(Text,"PAL I+"); break;
	   case 3 : strcat(Text,"PAL D/K (Sat+Secam)+"); break;
	};		   

    switch(MSPMinorMode) {
	   case 0 : strcat(Text,"FM-stereo "); break;
	   case 1 : strcat(Text,"NICAM "); break;
	   case 2 : strcat(Text,"NICAM "); break;
	   case 3 : strcat(Text,"D/K1 FM-Stereo "); break;
	   case 4 : strcat(Text,"D/K2 FM-stereo "); break;
	   case 5 : strcat(Text,"SAT FM-stereo s/b "); break;
	   case 6 : strcat(Text,"SAT FM-stereo s "); break;
	   case 7 : strcat(Text,"SAT FM-stereo b "); break;
	};		   

    switch(MSPStereo) {
	   case 1 : strcat(Text,"(Mono)"); break;
	   case 2 : strcat(Text,"(Stereo)"); break;
	   case 3 : strcat(Text,"(Kanal 1)"); break;
	   case 4 : strcat(Text,"(Kanal 2)"); break;
	};		   
}
SetWindowText(hwndAudioField,Text);
}

void MSPWatch_Mode( )
{
	int val;
    int newstereo=MSPStereo;
Sleep(2);
 	switch (MSPMode) {
			case MSP_MODE_FM_TERRA:
                
				val = ReadDSP( 0x18 );
				if (val > 4096) {
					newstereo = VIDEO_SOUND_STEREO;
				} else if (val < -4096) {
					newstereo = VIDEO_SOUND_LANG1;
				} else {
					newstereo = VIDEO_SOUND_MONO;
				}
				break;
			case MSP_MODE_FM_NICAM1:
			case MSP_MODE_FM_NICAM2:
				val = ReadDSP(0x23);
				switch ((val & 0x1e) >> 1)  {
				case 0:
				case 8:
					newstereo = VIDEO_SOUND_STEREO;
					break;
				default:
					newstereo = VIDEO_SOUND_MONO;
					break;
				}
				break;
			}

	
	if ( MSPStereo != newstereo ) {
		if ( AutoStereoSelect == TRUE ) MSP_SetStereo(MSPMajorMode,MSPMinorMode,newstereo);
		else MSPNewStereo=newstereo;
	    		
	}
};

#endif

/*******************/

void winbis_decoder(unsigned char *dat)  
{
   unsigned char ram[40], t;
   int i;

   memcpy(ram, &dat[8], 40);

   //************ Die Winbis "Mickey-Maus"-Verschlüsselung *******************

   for(i=3;i<33;i+=2)      // Bytes in zweierschritten tauschen
   {
     t=ram[i];
     ram[i]=ram[3+i];
     ram[3+i]=t;
   }

   for(i=4;i<35;i+=3)      // Jedes 3. Byte XOR mit 0xbb
   {
     ram[i] ^= 0xbb;
   }

   for(i=2;i<35;i++)
   {
     t=ram[i];
     ram[i]=(t<<3)|(t>>5); // 3 Bit nach rechts rotieren
   }

   // ******** Beschreibung der decodierten Daten ***********
   // ram[0]     = Laufende Nummer
   // ram[1]     = Beginn des ersten Datenblocks in dieser Zeile relativ zu 
   //              dieser Position (0= kein neuer Datenblock in dieser Zeile)
   // ram[2..34] = Datenbereich
   // ram[35,36] = CRC-Prüfsumme (keine Ahnung wie die berechnet wird)
   //              (könnte man mit 2 Zeilen feststellen, die nur mit 
   //               Leerzeichen gefüllt sind und sich nur durch die
   //               laufende Nummer unterscheiden)

   // Datenblöcke:
   // Datenblöcke haben einen Header der mindestens 4 Byte lang ist. Die ersten 3 Bytes scheinen die Adresse
   // in der Datenback zu sein. Byte 1 ist meist die Zeilennummer (aber nicht immer). Byte 4 scheint den Blocktyp
   // zu bestimmen. Textblöcke haben meist den Typ 0xf4. Bei einem 0xf4-Typ gibt das 5. Byte meist die Textlänge
   // an (leider stimmt das nicht immer).
   // Das Hauptproblem ist es die richtge Blocklänge zu bestimmen. Oft liegen mehrere Blöcke in einer Zeile, die
   // natürlich alle gebraucht werden. Hier komme ich nicht weiter ;-((
   // Ich habe die böse Vermutung, daß die Blocklänge in einer Tabelle steht und von der Datenbankadresse abhängig ist.
   // So spart man sich die Übertragung der Blocklänge.

   WinBis_push_data(ram);
   WinBis_daten_ausgabe();
}





void WinBis_fifo_schieben(void)
{
   if(fifo_in>0 && fifo_out>0)
   {
     fifo_in--;
     fifo_out--;
     memcpy(&fifo[0], &fifo[1], 37*(FLEN-1));
   }
}

void WinBis_push_data(unsigned char *dat)
{
   if(fifo_in<FLEN)
   {
     memcpy(fifo[fifo_in], dat, 37);
     fifo_in++;
   }

   if(fifo_in>=FLEN)
   {
    WinBis_fifo_schieben();
   }
}

unsigned int WinBis_get_byte(int adr)  // überspringt die 2 Bytes am Zeilenanfang und -ende
{
   int zeile;

   zeile=fifo_out;
   while(adr>=35)
   {
     adr-=33;
     zeile++;
   }
   return fifo[zeile][adr];
}


/************************** Testdump by ELMS **************************************************************/


#define HEADER		1
#define MYTEXT		2
#define MYVALS_A	3
#define MYVALS_B	4
#define MYVALS_C	5
#define MYVALS_D	6

 static char mymode=HEADER;
 static int bytecount=0;
static unsigned char header[8];
 static char headercount=0;


void WinBis_daten_ausgabe(void)
{
   int i;
   unsigned char c;

   if ( winbisfp == NULL ) return;

   if(fifo_in>fifo_out+10)
   {
   //fprintf(winbisfp,"%02x %02x:  ", fifo[fifo_out][0], fifo[fifo_out][1]); /* laufende paketnummer */
   for(i=2;i<35;i++) {
		c=fifo[fifo_out][i];
		if (mymode==HEADER) {
				header[headercount]=c;

				if (headercount<3) headercount++; 
					else headercount=3;
				
		 if (headercount==3) {
			 int dflag=0;

// 53 1a 00 ll      = NTV/Reuters news
// xx xx 

		if ((header[2]==0x13) && (header[3]==0xf4)) {
				fprintf(winbisfp,"\r\nWerte:");
				mymode=MYTEXT;
				dflag=1;
				bytecount=-1;
				}			 
		if ((header[2]==0x11) && (header[3]==0xf4)) {
				fprintf(winbisfp,"\r\nWerte:");
				mymode=MYTEXT;
				dflag=1;
				bytecount=-1;
				}			 

		if ((header[2]==0x12) && (header[3]==0xf4)) {
				fprintf(winbisfp,"\r\nWertpapierdaten:");
				mymode=MYTEXT;
				dflag=1;
				bytecount=-1;
				}
		 if ((header[2]==0x10) && (header[3]==0xf4)) {
				fprintf(winbisfp,"\r\nWertpapierdaten:");
				mymode=MYTEXT;
				dflag=1;
				bytecount=-1;
				}
		  if ((header[3]==0xf2) && (header[2]==0x11)) {		  
				fprintf(winbisfp,"Nachricht:\r\n");
				mymode=MYTEXT;
				dflag=1;
				bytecount=-1;
				}
		  if (   (header[0]==0x53)
			  && (header[1]==0x1a)
			  && (header[1]==0x00)
			  ) {
			  	fprintf(winbisfp,"\r\nNEWSTICKER:");
				mymode=MYTEXT;
				dflag=1;
				bytecount=-1;
				}
		  /*
		  if ((header[0]==0xb0) && (header[1]==0x1f)) {		  
				fprintf(winbisfp,"\r\nReport:");
				mymode=MYTEXT;
				dflag=1;
				bytecount=-1;
				}
		  if ((header[0]==0xcf) && (header[1]==0x20)) {		  
				fprintf(winbisfp,"\r\ndatenblatt:");
				mymode=MYTEXT;
				dflag=1;
				bytecount=-1;
				}
		  
		  if (   (header[0]==0x5d)
			  && (header[1]==0x1c)
			  && (header[2]==0x00)
			  
			  ) {
			  	fprintf(winbisfp,"\r\nNEWS_2:");
				mymode=MYTEXT;
				dflag=1;
				bytecount=-1;
				}

		  if (   (header[2]==0x6a)
			  && (header[3]==0x01)
			  
			  ) {
			  	fprintf(winbisfp,"\r\nBörse:");
				mymode=MYTEXT;
				dflag=1;
				bytecount=-1;
				}


		  if (   (header[0]==0x6f) 
			  && (header[1]==0xec)
			  && (header[2]==0x0c)
			  && (header[3]==0x55)
			  ) {		  
				fprintf(winbisfp,"\r\nPapierwert_A:");
				mymode=MYVALS_A;
				dflag=1;
				bytecount=18;
				}
		  
		  if (   (header[0]==0x6f) 
			  && (header[1]==0xec)
			  && (header[2]==0x0c)
			  && (header[3]==0x5a)
			  ) {		  
				fprintf(winbisfp,"\r\nPapierwert_B:");
				mymode=MYVALS_B;
				dflag=1;
				bytecount=9;
				}

		  if (   (header[0]==0x6f) 
			  && (header[1]==0xec)
			  && (header[2]==0x0c)
			  && (header[3]==0x48)
			  ) {		  
				fprintf(winbisfp,"\r\nPapierwert_C:");
				mymode=MYVALS_C;
				dflag=1;
				bytecount=9;
				}

		  if (   (header[2]==0x6a)
			  && (header[3]==0x12)
			  ) {		  
				fprintf(winbisfp,"\r\nPapierwert_D:");
				mymode=MYVALS_D;
				dflag=1;
				bytecount=7;
				}
*/
		  if (dflag) {		    				
				headercount=0;				
				}  else {
			  header[0]=header[1];
			  header[1]=header[2];
			  header[2]=header[3];		 
				}
			} //ende headercount==3
		} else {
			//hier sind wir nicht im Mode header
			if (bytecount>0)	{ // ist bytecount >0, dann gibt es was zu tun
				switch (mymode){

					case MYTEXT:if(c<32||c>127) c='.';
								fputc(c,winbisfp);
								break;

					case MYVALS_D:
					case MYVALS_C:
					case MYVALS_B:
					case MYVALS_A:fprintf(winbisfp,"%02x ", c);
								break;
					default:bytecount=0;
							mymode=HEADER;
							continue;
					}// ende von switch
				bytecount--;				
				if (bytecount==0) mymode=HEADER;					
			} else	{ //hier bytecount <=0
					if ((bytecount<0) && (mymode==MYTEXT))	bytecount=c;
					if ((bytecount>127) || (bytecount<-1)) {bytecount=0;mymode=HEADER;}
				}
		}//ende else headermode
   }//ende for
   fifo_out++;
   }//ende if >
}


