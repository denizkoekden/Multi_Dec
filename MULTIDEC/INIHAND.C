#define adjust(); \
	j=0;\
	buffer[j]=0x00;\
	while ( Pos < SemmelPos) {\
	if ( *Pos != ' ' ) {\
	buffer[j]=*Pos;\
	j++;\
	buffer[j]=0x00;\
	}\
	Pos++;\
	}

#define CHECKFOR(AA)  if (((Pos=strstr(zeile,AA)) != 0)  && ( SemmelPos > Pos ))
#define CHECKTRUE()   if (( *Pos == 'T' ) ||  ( *Pos == 't' ))


void Set_Default_Values(void) {

    char Text[256];
    int i;

	// Default Inits //
	
	ShutdownZeit.H = 0xff;
	ShutdownZeit.M = 0xff;

#ifdef USE_BT8X8
	Current_Mode=0;
#endif
	CardType=0;
	Capture_Video=TRUE;
	Capture_VBI=TRUE;
	Decode_AutoDetect=FALSE;
	ColourFormat=3;
	VideoSource=1;
	AudioSource=4;
	TunerType=0;
	OPKEY[0].Name[0]=0x00;
	OPKEY[1].Name[0]=0x00;
	OPKEY[2].Name[0]=0x00;
	OPKEY[3].Name[0]=0x00;
	strcpy(OPKEY[0].Name,"KEY.TXT");
	strcpy(OPKEY[0].Extension,"idx");
	ActiveKey=0;
	strcpy(OPKEY[1].Name,"KEY_C.TXT");
	strcpy(OPKEY[1].Extension,"ic+");
	
	for ( i=0; i < MAXFAVORITEN; i++ ) { 
		Favoriten[i].ts_id = 0;
		Favoriten[i].SID_Id = 0;
	};
	
	
	OSD_Colors[0].R=0;OSD_Colors[0].G=0;OSD_Colors[0].B=100;OSD_Colors[0].Blend=255;
	OSD_Colors[1].R=50;OSD_Colors[1].G=50;OSD_Colors[1].B=200;OSD_Colors[1].Blend=255;
	OSD_Colors[2].R=255;OSD_Colors[2].G=255;OSD_Colors[2].B=0;OSD_Colors[2].Blend=255;
	OSD_Colors[3].R=255;OSD_Colors[3].G=255;OSD_Colors[3].B=255;OSD_Colors[3].Blend=255;
	
	ButtonList[0].FeldId=0; ButtonList[0].x=25;
	ButtonList[1].FeldId=1; ButtonList[1].x=30;
	ButtonList[2].FeldId=2; ButtonList[2].x=200;
	ButtonList[3].FeldId=3; ButtonList[3].x=100;
	ButtonList[4].FeldId=4; ButtonList[4].x=30;
	ButtonList[5].FeldId=5; ButtonList[5].x=80;
	ButtonList[6].FeldId=6; ButtonList[6].x=80;
	ButtonList[7].FeldId=8, ButtonList[7].x=80;
	ButtonList[8].FeldId=14;ButtonList[8].x=50;
	ButtonList[9].FeldId=-1;ButtonList[9].x=0;
	ButtonList[10].FeldId=-1;ButtonList[10].x=0;
	ButtonList[11].FeldId=-1;ButtonList[11].x=0;
	ButtonList[12].FeldId=-1;ButtonList[12].x=0;
	ButtonList[13].FeldId=-1;ButtonList[13].x=0;
	ButtonList[14].FeldId=-1;ButtonList[14].x=0;
	
	
	TVTYPE=0;
	Flip=1;
	OutDib=TRUE;
	WindowStyle=0;
	InitialHue=0x00;
	InitialBrightness=0x00;
	InitialContrast=0xd8;
	InitialSaturationU=0xfe;
	InitialSaturationV=0xb4;
	InitialVolume=1000;
	InitialSpecial=0;
	InitialLoudness=0;
	InitialBass=0;
	InitialTreble=0;
	MasterTestzeilen=16;
	ManuellAudio[0]=0;ManuellAudio[1]=0;ManuellAudio[2]=0;ManuellAudio[3]=0;
	ManuellAudio[4]=0;ManuellAudio[5]=0;ManuellAudio[6]=0;ManuellAudio[7]=0;
	
	
	LNB[0].Use=TRUE;
	LNB[0].MinFreq=10700;
	LNB[0].MaxFreq=12750;
	LNB[0].LofLow=9750;
	LNB[0].LofHigh=10600;
	LNB[0].SwitchFreq=11700;
	LNB[0].PowerReset=FALSE;
	LNB[0].Power=TRUE;
	LNB[0].Switch22khz=TRUE;
	LNB[0].BurstVal=-1;
	LNB[0].PosVal='A';
	LNB[0].OptVal='A';
	LNB[0].MiniDiseqc=0x00;
	LNB[0].orbit=130;
	LNB[0].west_east='E';
	
	LNB[1].Use=FALSE;
	LNB[1].MinFreq=10700;
	LNB[1].MaxFreq=12750;
	LNB[1].LofLow=9750;
	LNB[1].LofHigh=10600;
	LNB[1].SwitchFreq=11700;
	LNB[1].Power=TRUE;
	LNB[1].PowerReset=FALSE;
	LNB[1].Switch22khz=TRUE;
	LNB[1].BurstVal=-1;
	LNB[1].PosVal='B';
	LNB[1].OptVal='B';
	LNB[1].MiniDiseqc=0x00;
	LNB[1].orbit=192;
	LNB[1].west_east='E';
	
	LNB[2].Use=FALSE;
	LNB[2].MinFreq=10700;
	LNB[2].MaxFreq=12750;
	LNB[2].LofLow=9750;
	LNB[2].LofHigh=10600;
	LNB[2].SwitchFreq=11700;
	LNB[2].PowerReset=FALSE;
	LNB[2].Power=TRUE;
	LNB[2].Switch22khz=TRUE;
	LNB[2].BurstVal=-1;
	LNB[2].PosVal='A';
	LNB[2].OptVal='B';
	LNB[2].MiniDiseqc=0x00;
	LNB[2].orbit=0;
	LNB[2].west_east='.';
	
	LNB[3].Use=FALSE;
	LNB[3].MinFreq=10700;
	LNB[3].MaxFreq=12750;
	LNB[3].LofLow=9750;
	LNB[3].LofHigh=10600;
	LNB[3].PowerReset=FALSE;
	LNB[3].SwitchFreq=11700;
	LNB[3].Power=TRUE;
	LNB[3].Switch22khz=TRUE;
	LNB[3].BurstVal=-1;
	LNB[3].PosVal='B';
	LNB[3].OptVal='A';
	LNB[3].MiniDiseqc=0x00;
	LNB[3].orbit=0;
	LNB[3].west_east='.';
	
	Timing.WriteRegDelay=0xffffffff;
	Timing.I2C_Bus_Timeout=200;
	Timing.I2C_Bus_Reset_Delay=10;
	Timing.DVB_Reset_Wait=5;
	Timing.DVB_Reset_Scan_Wait=15;
	Timing.SendDiSEqCDelay=100;
	Timing.Pmt_TimeOut=2500;
	Timing.Sdt_TimeOut=2500;
	Timing.Pat_TimeOut=2500;
	Timing.Cat_TimeOut=2500;
	Timing.Nit_TimeOut=5000;
	Timing.Debi_Cmd_Time_Out=15000;
	Timing.OSD_Text_Time_Out=60000;
	Timing.Debi_done_MC2=2000;
	Timing.Debi_done_PSR=10000;

	for ( i=0; i<TIMERANZAHL; i++ ) {
		TimerRec[i].StartZeit_h=0xff;
		TimerRec[i].Started=FALSE;
	};
	sprintf(Text,">>>>>> (VD-RAW.Dat)");
	Write_Log(Text);

	strcpy(VDat.RawName,"VD-RAW.Dat");
	
	sprintf(Text,">>>>>> (VD-RAW.Dat) <<<<<<");
	Write_Log(Text);
	
	
	TZ[0]=16;
	TZ[1]=24;
	TZ[2]=32;
	TZ[3]=40;
	TZ[4]=48;

    plugins_count = 0;
    Ext_Dll = NULL;

	for ( i = 0 ; i < 64 ; i++ ) MixerLoad[i].MixerAccess.SoundSystem = -1;
	
	VTColourTable[0]=RGB(0,0,0);     //Schwarz
	VTColourTable[1]=RGB(255,0,0); //Rot
	VTColourTable[2]=RGB(0,255,0);     //Grun
	VTColourTable[3]=RGB(255,255,0);  //Gelb
	VTColourTable[4]=RGB(0,0,255);     //Blau
	VTColourTable[5]=RGB(192,0,192);     // Unsichtbar
	VTColourTable[6]=RGB(92,164,207);       // HellBlau
	VTColourTable[7]=RGB(255,255,255);	// Weis 
	VTColourTable[8]=RGB(254,254,254);       // Eigentich Transparent
	VTColourTable[9]=0;       // Eigentich Transparent
	VTColourTable[10]=0;       // Eigentich Transparent
	VTColourTable[11]=0;       // Eigentich Transparent
	VTColourTable[12]=0;       // Eigentich Transparent
	VTColourTable[13]=0;       // Eigentich Transparent
	VTColourTable[14]=0;       // Eigentich Transparent
	VTColourTable[15]=0;       // Eigentich Transparent
	RecordBaseDir[1][0]=0x00;
	RecordBaseDir[2][0]=0x00;
	RecordBaseDir[3][0]=0x00;
	RecordBaseDir[4][0]=0x00;
	RecordBaseDir[5][0]=0x00;
	RecordBaseDir[6][0]=0x00;
	RecordBaseDir[7][0]=0x00;

    sprintf(Text,">>>>>> GetCurrentDirectory()");
	Write_Log(Text);
	GetCurrentDirectory(255,CurrentDir);
	sprintf(RecordBaseDir[0],"%s\\Recorder",CurrentDir);
	sprintf(FIRM_DIR,"%s\\Firmware\\Linux_9_4_05_03",CurrentDir);
	sprintf(WINIBIS_DIR,"%s\\Winbis",CurrentDir);
	sprintf(VD_DIR,"%s\\VideoDat",CurrentDir);
	sprintf(VT_BASE_DIR,"%s\\VideoTxt",CurrentDir);
	sprintf(VT_EXPORT_WWW_ROOT,"file://%s",VT_BASE_DIR);
	sprintf(Text,">>>>>> GetCurrentDirectory() <<<<<<");
	Write_Log(Text);

}


BOOL Load_INI(LPSTR Name)
{
	
	FILE* iniFile;
	char zeile[256];
	char buffer[64];
	char *Pos;
	char *SemmelPos;
	unsigned int j;
	unsigned int i;
	int B=0;
	unsigned int F1,F2,F3,F4,F5,F6,F7,F8,F9; 
	unsigned char Color_R,Color_G,Color_B;
	
	if ((iniFile=fopen(Name,"r")) == NULL )  {
            WRITE_LOG = FALSE;
		return(FALSE);
	}
	
	VBI_Flags=0;
	
	while ( fgets(zeile,sizeof(zeile),iniFile) != NULL ) 
	{
		
		SemmelPos=strstr(zeile,";");
		if ( SemmelPos==NULL )	 SemmelPos=strstr(zeile,"\n");
		
		CHECKFOR("WINDOWSTARTX="){
			Pos=Pos+13;
			adjust();
			emstartx=atoi(buffer);
			continue;
		} CHECKFOR("WINDOWSTARTY="){
			Pos=Pos+13;
			adjust();
			emstarty=atoi(buffer);
			continue;
		} CHECKFOR("WINDOWSIZEX="){
			Pos=Pos+12;
			adjust();
			emsizex=atoi(buffer);
			continue;
		} CHECKFOR("WINDOWSIZEY="){
			Pos=Pos+12;
			adjust();
			emsizey=atoi(buffer);
			continue;
		} CHECKFOR("BR_TOPMOST="){			// BigReaper-TopMost
			Pos=Pos+11;
			adjust();
			topmost=atoi(buffer);
			continue;
		} CHECKFOR("CURSOR_STATUS="){		// BigReaper-CursorStatus
			Pos=Pos+14;
			adjust();
			CShowCursor=atoi(buffer);
			continue;
		} CHECKFOR("DVBOUT="){				// Moto - DVB out only
			Pos=Pos+7;
			adjust();
			dvbout=atoi(buffer);
			continue;
		} CHECKFOR("LANGUAGE="){				// R One
			Pos=Pos+9;
			adjust();
			language=atoi(buffer);
			continue;

		} CHECKFOR("WINDOWSTYLE="){
			Pos=Pos+12;
			adjust();
			WindowStyle=atoi(buffer);
			continue;
		} CHECKFOR("RESX="){
			Pos=Pos+5;
			adjust();
			Res_X=atoi(buffer);
			continue;
		} CHECKFOR("RESY="){
			Pos=Pos+5;
			adjust();
			Res_Y=atoi(buffer);
			continue;
		} CHECKFOR("TOPMOST="){
			Pos=Pos+8;
			ALLWAYS_TOP=FALSE;
			CHECKTRUE() {
				ALLWAYS_TOP=TRUE;
			}
			continue;
		} CHECKFOR("USE_TV_16_9="){
			Pos=Pos+12;
			USE_TV_16_9=FALSE;
			CHECKTRUE() {
				USE_TV_16_9=TRUE;
			}
			continue;
		} CHECKFOR("SHUTDOWN_OPTION="){
			Pos=Pos+16;
			adjust();
			ShutDownValue=atoi(buffer);
			continue;
		} CHECKFOR("WRITE_LOG="){
			Pos=Pos+10;
			WRITE_LOG=FALSE;
			CHECKTRUE() {
				WRITE_LOG=TRUE;
			}
			continue;
		} CHECKFOR("KEEP_DRIVER_LOADED="){
			Pos=Pos+19;
			Keep_Driver_Loaded=FALSE;
			CHECKTRUE() {
				Keep_Driver_Loaded=TRUE;
			}
			continue;
		} CHECKFOR("EVENTSTARTX="){
			Pos=Pos+12;
			adjust();
			evstartx=atoi(buffer);
			continue;
		} CHECKFOR("EVENTSTARTY="){
			Pos=Pos+12;
			adjust();
			evstarty=atoi(buffer);
		} CHECKFOR("EVENTSIZEX="){
			Pos=Pos+11;
			adjust();
			evsizex=atoi(buffer);
			continue;
		} CHECKFOR("EVENTSIZEY="){
			Pos=Pos+11;
			adjust();
			evsizey=atoi(buffer);
			continue;
		} CHECKFOR("EIT_SORT="){
			Pos=Pos+9;
			adjust();
			EIT_Sort=atoi(buffer);
			continue;
		} CHECKFOR("EIT_DATUM="){
			Pos=Pos+10;
			adjust();
			EIT_Datum=atoi(buffer);
			continue;
		} CHECKFOR("EIT_KANAL="){
			Pos=Pos+10;
			adjust();
			EIT_Kanal=atoi(buffer);
			continue;
		} CHECKFOR("FAVORIT="){
			Pos=Pos+8;
			j=sscanf(Pos,"%3d %6d %6d",&F1,&F2,&F3);
			if ( j == 3 ) {
				Favoriten[F1].SID_Id=F2;
				Favoriten[F1].ts_id=F3;
			};
			continue;
		} CHECKFOR("TIMERREC="){
			Pos=Pos+9;
			j=sscanf(Pos,"%02d",&F1);
			F1--;
			if (( j == 1 ) && ( F1 >= 0 ) && ( F1 < TIMERANZAHL )) {
				i=F1;
				j=sscanf(Pos,"%02d %02d %02d %02d %02d %6d %6d %1d %1d",&F1,&F2,&F3,&F4,&F5,&F6,&F7,&F8,&F9);
				if ( j == 8 ) {
					TimerRec[i].StartZeit_h=F2;
					TimerRec[i].StartZeit_m=F3;
					TimerRec[i].EndZeit_h=F4;
					TimerRec[i].EndZeit_m=F5;
					TimerRec[i].Date_Id=F6;
					TimerRec[i].Programm=F7;
					TimerRec[i].RecorderArt=F8;
					TimerRec[i].Shutdown = FALSE;
				};
				if ( j == 9 ) {
					TimerRec[i].StartZeit_h=F2;
					TimerRec[i].StartZeit_m=F3;
					TimerRec[i].EndZeit_h=F4;
					TimerRec[i].EndZeit_m=F5;
					TimerRec[i].Date_Id=F6;
					TimerRec[i].Programm=F7;
					TimerRec[i].RecorderArt=F8;
					TimerRec[i].Shutdown = F9;
				};
				
			};
			continue;
		} 
		
		CHECKFOR("OSD_HINTERGRUND1="){
			Pos=Pos+17;
			j=sscanf(Pos,"%3d %3d %3d %3d",&F1,&F2,&F3,&F4);
			if ( j == 4 ) {
				OSD_Colors[0].R=F1;
				OSD_Colors[0].G=F2;
				OSD_Colors[0].B=F3;
				OSD_Colors[0].Blend=F4;
			};
		} else CHECKFOR("OSD_HINTERGRUND2="){
			Pos=Pos+17;
			j=sscanf(Pos,"%3d %3d %3d %3d",&F1,&F2,&F3,&F4);
			if ( j == 4 ) {
				OSD_Colors[1].R=F1;
				OSD_Colors[1].G=F2;
				OSD_Colors[1].B=F3;
				OSD_Colors[1].Blend=F4;
			};
		} else CHECKFOR("OSD_FARBE1="){
			Pos=Pos+11;
			j=sscanf(Pos,"%3d %3d %3d %3d",&F1,&F2,&F3,&F4);
			if ( j == 4 ) {
				OSD_Colors[2].R=F1;
				OSD_Colors[2].G=F2;
				OSD_Colors[2].B=F3;
				OSD_Colors[2].Blend=F4;
			};
		} else CHECKFOR("OSD_FARBE2="){
			Pos=Pos+11;
			j=sscanf(Pos,"%3d %3d %3d %3d",&F1,&F2,&F3,&F4);
			if ( j == 4 ) {
				OSD_Colors[3].R=F1;
				OSD_Colors[3].G=F2;
				OSD_Colors[3].B=F3;
				OSD_Colors[3].Blend=F4;
			};
		}
		/************************/
		else CHECKFOR("LISTSTARTX="){
			Pos=Pos+11;
			adjust();
			pgstartx=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTSTARTY="){
			Pos=Pos+11;
			adjust();
			pgstarty=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTSIZEX="){
			Pos=Pos+10;
			adjust();
			pgsizex=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTSIZEY="){
			Pos=Pos+10;
			adjust();
			pgsizey=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_1_ID="){
			Pos=Pos+14;
			adjust();
			ButtonList[0].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_1_LAENGE="){
			Pos=Pos+18;
			adjust();
			ButtonList[0].x=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_2_ID="){
			Pos=Pos+14;
			adjust();
			ButtonList[1].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_2_LAENGE="){
			Pos=Pos+18;
			adjust();
			ButtonList[1].x=atoi(buffer);
			continue;
		}
		
		CHECKFOR("LISTFELD_3_ID="){
			Pos=Pos+14;
			adjust();
			ButtonList[2].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_3_LAENGE="){
			Pos=Pos+18;
			adjust();
			ButtonList[2].x=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_4_ID="){
			Pos=Pos+14;
			adjust();
			ButtonList[3].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_4_LAENGE="){
			Pos=Pos+18;
			adjust();
			ButtonList[3].x=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_5_ID="){
			Pos=Pos+14;
			adjust();
			ButtonList[4].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_5_LAENGE="){
			Pos=Pos+18;
			adjust();
			ButtonList[4].x=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_6_ID="){
			Pos=Pos+14;
			adjust();
			ButtonList[5].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_6_LAENGE="){
			Pos=Pos+18;
			adjust();
			ButtonList[5].x=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_7_ID="){
			Pos=Pos+14;
			adjust();
			ButtonList[6].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_7_LAENGE="){
			Pos=Pos+18;
			adjust();
			ButtonList[6].x=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_8_ID="){
			Pos=Pos+14;
			adjust();
			ButtonList[7].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_8_LAENGE="){
			Pos=Pos+18;
			adjust();
			ButtonList[7].x=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_9_ID="){
			Pos=Pos+14;
			adjust();
			ButtonList[8].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_9_LAENGE="){
			Pos=Pos+18;
			adjust();
			ButtonList[8].x=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_10_ID="){
			Pos=Pos+15;
			adjust();
			ButtonList[9].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_10_LAENGE="){
			Pos=Pos+19;
			adjust();
			ButtonList[9].x=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_11_ID="){
			Pos=Pos+15;
			adjust();
			ButtonList[10].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_11_LAENGE="){
			Pos=Pos+19;
			adjust();
			ButtonList[10].x=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_12_ID="){
			Pos=Pos+15;
			adjust();
			ButtonList[11].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_12_LAENGE="){
			Pos=Pos+19;
			adjust();
			ButtonList[11].x=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_13_ID="){
			Pos=Pos+15;
			adjust();
			ButtonList[12].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_13_LAENGE="){
			Pos=Pos+19;
			adjust();
			ButtonList[12].x=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_14_ID="){
			Pos=Pos+15;
			adjust();
			ButtonList[13].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_14_LAENGE="){
			Pos=Pos+19;
			adjust();
			ButtonList[13].x=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_15_ID="){
			Pos=Pos+15;
			adjust();
			ButtonList[14].FeldId=atoi(buffer);
			continue;
		}
		CHECKFOR("LISTFELD_15_LAENGE="){
			Pos=Pos+19;
			adjust();
			ButtonList[14].x=atoi(buffer);
			continue;
		}
		
		/*************************/	
		CHECKFOR("PROCESSPRIORITY="){
			Pos=Pos+16;	   
			adjust();
			PriorClassId=atoi(buffer);
			continue;
		}						   
		CHECKFOR("THREADPRIORITY="){
			Pos=Pos+15;
			adjust();
			ThreadClassId=atoi(buffer);
			continue;
		}
		CHECKFOR("WINDOWPROZESSOR="){
			Pos=Pos+16;
			adjust();
			MainProzessor=atoi(buffer);
			continue;
		}
		CHECKFOR("IRQPROZESSOR="){
			Pos=Pos+13;
			adjust();
			IrqProzessor=atoi(buffer);
			continue;
		}
		CHECKFOR("VBIPROZESSOR="){
			Pos=Pos+13;
			adjust();
			VBIProzessor=atoi(buffer);
			continue;
		}
		CHECKFOR("DECODEPROZESSOR="){
			Pos=Pos+16;
			adjust();
			AusgabeProzessor=atoi(buffer);
			continue;
		}
		CHECKFOR("STATUSBAR="){
			Pos=Pos+10;
			Display_Status_Bar=FALSE;
			CHECKTRUE() {
				Display_Status_Bar=TRUE;
			}
			
			continue;
		}
		CHECKFOR("DIBMODE="){
			Pos=Pos+8;
			OutDib=FALSE;
			CHECKTRUE() {
				OutDib=TRUE;
			}
			continue;				
		}
		
		CHECKFOR("OVERLAYMODE="){
			Pos=Pos+12;
			ModeOverlay=FALSE;
			CHECKTRUE() {
				ModeOverlay=TRUE;
			}
			continue;
			
		}
		CHECKFOR("RAWSURFACE="){
			Pos=Pos+11;
			ModeRAWSurface=FALSE;
			CHECKTRUE() {
				ModeRAWSurface=TRUE;
			}
			continue;
			
		}
		CHECKFOR("RAWOVERLAY="){
			Pos=Pos+11;
			ModeRAWOverlay=FALSE;
			CHECKTRUE() {
				ModeRAWOverlay=TRUE;
			}
			continue;
			
		}
		CHECKFOR("SYNC_IRQ="){
			Pos=Pos+9;
			Sync_Irq=FALSE;
			CHECKTRUE() {
				Sync_Irq=TRUE;
			}
			continue;
			
		}
		CHECKFOR("TVSCREEN="){
			Pos=Pos+9;
			Show_Menu=FALSE;
			CHECKTRUE() {
				Show_Menu=TRUE;
			}
			continue;
			
		}
		CHECKFOR("TVSCREENNOFRAME="){
			Pos=Pos+16;
			Toggle_WithOut_Frame=FALSE;
			CHECKTRUE() {
				Toggle_WithOut_Frame=TRUE;
			}
			continue;
			
		}
		
		CHECKFOR("HALBBILDMODUS="){
			Pos=Pos+14;
			HalfTVMode=FALSE;
			CHECKTRUE() {
				HalfTVMode=TRUE;
			}
			continue;
			
		}
		CHECKFOR("COLORMIX="){
			Pos=Pos+9;
			ColorMix=FALSE;
			CHECKTRUE() {
				ColorMix=TRUE;
			}
			continue;
			
		}
		
		
		CHECKFOR("MOVIESCALING="){
			Pos=Pos+13;
			adjust();
			MovieScale=atoi(buffer);
			continue;
			
		}

#ifdef USE_BT8X8
		CHECKFOR("NAGRATESTZEILEN="){
			Pos=Pos+16;
			adjust();
			MasterTestzeilen=atoi(buffer);
			continue;
		}
		CHECKFOR("NAGRATESTZEILEN1="){
			Pos=Pos+17;
			adjust();
			TZ[0]=atoi(buffer);
			continue;
		} 
		CHECKFOR("NAGRATESTZEILEN2="){
			Pos=Pos+17;
			adjust();
			TZ[1]=atoi(buffer);
			continue;
		}                
		CHECKFOR("NAGRATESTZEILEN3="){
			Pos=Pos+17;
			adjust();
			TZ[2]=atoi(buffer);
			continue;
		}                
		CHECKFOR("NAGRATESTZEILEN4="){
			Pos=Pos+17;
			adjust();
			TZ[3]=atoi(buffer);
			continue;
		}                
		CHECKFOR("NAGRATESTZEILEN5="){
			Pos=Pos+17;
			adjust();
			TZ[4]=atoi(buffer);
			continue;
		}                
		CHECKFOR("NAGRAFULL="){
			Pos=Pos+10;
			NagraFullField=FALSE;
			CHECKTRUE() {
				NagraFullField=TRUE;
			}
			continue;
			
		}
		CHECKFOR("AUTO-DETECT="){
			Pos=Pos+12;
			Decode_AutoDetect=FALSE;
			CHECKTRUE() {
				Decode_AutoDetect=TRUE;
			}
			continue;
			
		}  
		CHECKFOR("KEYNAME0="){
			Pos=Pos+9;
			j=0;
			OPKEY[0].Name[j]=0x00;
			while ( Pos < SemmelPos) {
				OPKEY[0].Name[j]=*Pos;
				j++;
				OPKEY[0].Name[j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("KEYNAME1="){
			Pos=Pos+9;
			j=0;
			OPKEY[1].Name[j]=0x00;
			while ( Pos < SemmelPos) {
				OPKEY[1].Name[j]=*Pos;
				j++;
				OPKEY[1].Name[j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("KEYNAME2="){
			Pos=Pos+9;
			j=0;
			OPKEY[2].Name[j]=0x00;
			while ( Pos < SemmelPos) {
				OPKEY[2].Name[j]=*Pos;
				j++;
				OPKEY[2].Name[j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("KEYNAME3="){
			Pos=Pos+9;
			j=0;
			OPKEY[3].Name[j]=0x00;
			while ( Pos < SemmelPos) {
				OPKEY[3].Name[j]=*Pos;
				j++;
				OPKEY[3].Name[j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("KEYEXTENSION0="){
			Pos=Pos+14;
			j=0;
			OPKEY[0].Extension[j]=0x00;
			while ( Pos < SemmelPos) {
				OPKEY[0].Extension[j]=*Pos;
				j++;
				OPKEY[0].Extension[j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("KEYEXTENSION1="){
			Pos=Pos+14;
			j=0;
			OPKEY[1].Extension[j]=0x00;
			while ( Pos < SemmelPos) {
				OPKEY[1].Extension[j]=*Pos;
				j++;
				OPKEY[1].Extension[j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("KEYEXTENSION2="){
			Pos=Pos+14;
			j=0;
			OPKEY[2].Extension[j]=0x00;
			while ( Pos < SemmelPos) {
				OPKEY[2].Extension[j]=*Pos;
				j++;
				OPKEY[2].Extension[j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("KEYEXTENSION3="){
			Pos=Pos+14;
			j=0;
			OPKEY[3].Extension[j]=0x00;
			while ( Pos < SemmelPos) {
				OPKEY[3].Extension[j]=*Pos;
				j++;
				OPKEY[3].Extension[j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("AKTIVERKEY="){
			Pos=Pos+11;
			adjust();
			ActiveKey=atoi(buffer);
			continue;
		}                
		CHECKFOR("AUTO-DETECT-NAGRA="){
			Pos=Pos+18;
			AutoDetect_Nagra=FALSE;
			CHECKTRUE() {
				AutoDetect_Nagra=TRUE;
			}
			continue;
			
		}  CHECKFOR("AUTO-DETECT-VC="){
			Pos=Pos+15;
			AutoDetect_VC=FALSE;
			CHECKTRUE() {
				AutoDetect_VC=TRUE;
			}
			continue;
        }
#endif
			
		CHECKFOR("VBI_VT="){
			Pos=Pos+7;
			CHECKTRUE() {
                VBI_Flags+=VBI_VT;
			}
			continue;
		}  CHECKFOR("VBI_VC="){
			Pos=Pos+7;
			CHECKTRUE() {
                VBI_Flags+=VBI_VC;
			}
			continue;
		}                            
		CHECKFOR("VBI_WINBIS="){
			Pos=Pos+11;
			CHECKTRUE() {
                VBI_Flags+=VBI_WINBIS;
			}
			continue;
		}
		CHECKFOR("WINBIS_BASE_DIR="){
			Pos=Pos+16;
			j=0;
			WINIBIS_DIR[j]=0x00;
			while ( Pos < SemmelPos) {
				WINIBIS_DIR[j]=*Pos;
				j++;
				WINIBIS_DIR[j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("VD_BASE_DIR="){
			Pos=Pos+12;
			j=0;
			VD_DIR[j]=0x00;
			while ( Pos < SemmelPos) {
				VD_DIR[j]=*Pos;
				j++;
				VD_DIR[j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("VD_FILENAME="){
			Pos=Pos+12;
			j=0;
			VDat.RawName[j]=0x00;
			while ( Pos < SemmelPos) {
				VDat.RawName[j]=*Pos;
				j++;
				VDat.RawName[j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("VD_RAWFILE="){
			Pos=Pos+11;
			VD_RAW=FALSE;
			CHECKTRUE() {
				VD_RAW=TRUE;
			}
			continue;
		}                
		CHECKFOR("VBI_VPS="){
			Pos=Pos+8;
			CHECKTRUE() {
                VBI_Flags+=VBI_VPS;
			}
			continue;
		}                            
		CHECKFOR("AUTO-MOVIEBOOST="){
			Pos=Pos+16;
			Auto_MovieBoost=FALSE;
			CHECKTRUE() {
				Auto_MovieBoost=TRUE;
			}
			continue;
		}            

#ifdef USE_BT8X8
		CHECKFOR("SCHWARZ-SCHWELLE="){
			Pos=Pos+17;
			adjust();
			InitialLow=atoi(buffer);
			continue;
		}
		CHECKFOR("VCCHROMAKEY="){
			Pos=Pos+12;
			Chroma_Key=FALSE;
			CHECKTRUE() {
				Chroma_Key=TRUE;
			}
			continue;
		}
#endif

		CHECKFOR("NTDRIVERNAME="){
			Pos=Pos+13;
			j=0;
			OrgDriverName[j]=0x00;
			while ( Pos < SemmelPos) {
				OrgDriverName[j]=*Pos;
				j++;
				OrgDriverName[j]=0x00;
				Pos++;
			}
			continue;
		}   CHECKFOR("NTDVBDRIVERNAME="){
			Pos=Pos+16;
			j=0;
			OrgDVBDriverName[j]=0x00;
			while ( Pos < SemmelPos) {
				OrgDVBDriverName[j]=*Pos;
				j++;
				OrgDVBDriverName[j]=0x00;
				Pos++;
			}
			continue;
		} CHECKFOR("BT-Treibererweiterung="){
			Pos=Pos+22;
			LoadExternBTDriver=FALSE;
			if (( *Pos == 'T' ) ||  ( *Pos == 't' )) {
				LoadExternBTDriver=TRUE;
			}
			continue;
		} CHECKFOR("BT_IRQ_SHARING="){
			Pos=Pos+15;
			BT_IRQ_SHARING=FALSE;
			if (( *Pos == 'T' ) ||  ( *Pos == 't' )) {
				BT_IRQ_SHARING=TRUE;
			}
			continue;
		} CHECKFOR("DVB_IRQ_SHARING="){
			Pos=Pos+16;
			DVB_IRQ_SHARING=FALSE;
			if (( *Pos == 'T' ) ||  ( *Pos == 't' )) {
				DVB_IRQ_SHARING=TRUE;
			}
			continue;
		}   CHECKFOR("VIDEOKARTE="){
			Pos=Pos+11;
			adjust();
			CardType=atoi(buffer);
			continue;
		}                            
		CHECKFOR("VIDEOSOURCE="){
			Pos=Pos+12;
			adjust();
			VideoSource=atoi(buffer);
			continue;
		}
		CHECKFOR("AUDIOSOURCE="){
			Pos=Pos+12;
			adjust();
			AudioSource=atoi(buffer);
			continue;
		}
		CHECKFOR("TUNERTYP="){
			Pos=Pos+9;
			adjust();
			TunerType=atoi(buffer);
			continue;
		}
		CHECKFOR("TVTYP="){
			Pos=Pos+6;
			adjust();
			TVTYPE=atoi(buffer);
			if ( TVTYPE == 5 ) TVTYPE=0;
			continue;
		}
		CHECKFOR("FREQUENZ="){
			Pos=Pos+9;
			adjust();
			InitialFreq=atol(buffer);
			continue;
		}
#ifdef USE_BT8X8
		CHECKFOR("BT848-HUE-REGISTER="){
			Pos=Pos+19;
			adjust();
			InitialHue=atoi(buffer);
			continue;
		}
		CHECKFOR("BT848-CONTRAST-REGISTER="){
			Pos=Pos+24;
			adjust();
			InitialContrast=atoi(buffer);
			continue;
		}
		CHECKFOR("BT848-BRIGHTNESS-REGISTER="){
			Pos=Pos+26;
			adjust();
			InitialBrightness=atoi(buffer);
			continue;
		}
		CHECKFOR("BT848-SATURATION_U-REGISTER="){
			Pos=Pos+28;
			adjust();
			InitialSaturationU=atoi(buffer);
			continue;
		}
		CHECKFOR("BT848-SATURATION_V-REGISTER="){
			Pos=Pos+28;
			adjust();
			InitialSaturationV=atoi(buffer);
			continue;
		}
		CHECKFOR("BT848-IFORM-REGISTER="){
			InitialIFORM=0;
			Pos=Pos+21;
			adjust();
			InitialIFORM=atoi(buffer);
			continue;
		}
		
		CHECKFOR("TUNER_THRESH1="){
			Pos=Pos+14;
			adjust();
			Tuners[8].thresh1=atoi(buffer);
			continue;
		}
		CHECKFOR("TUNER_THRESH2="){
			Pos=Pos+14;
			adjust();
			Tuners[8].thresh2=atoi(buffer);
			continue;
		}
		
		CHECKFOR("TUNER_VHF_L="){
			Pos=Pos+12;
			adjust();
			Tuners[8].VHF_L=atoi(buffer);
			continue;
		}
		
		CHECKFOR("TUNER_VHF_H="){
			Pos=Pos+12;
			adjust();
			Tuners[8].VHF_H=atoi(buffer);
			continue;
		}
		
		CHECKFOR("TUNER_UHF="){
			Pos=Pos+10;
			adjust();
			Tuners[8].UHF=atoi(buffer);
			continue;
		}
		CHECKFOR("TUNER_CONFIG="){
			Pos=Pos+13;
			adjust();
			Tuners[8].config=atoi(buffer);
			continue;
		}
		CHECKFOR("TUNER_I2C="){
			Pos=Pos+10;
			adjust();
			Tuners[8].I2C=atoi(buffer);
			continue;
		}
		CHECKFOR("TUNER_IFPCOFF="){
			Pos=Pos+14;
			adjust();
			Tuners[8].IFPCoff=atoi(buffer);
			continue;
		}

		CHECKFOR("BT848_GPIO_OUT_EN="){
			Pos=Pos+18;
			adjust();
			ManuellAudio[0]=atoi(buffer);
			continue;
		}                            
		CHECKFOR("BT848_GPIO_REG_INP="){
			Pos=Pos+19;
			adjust();
			ManuellAudio[7]=atoi(buffer);
			continue;
		}
		CHECKFOR("BT848_GPIO_DATA_TUNER="){
			Pos=Pos+22;
			adjust();
			ManuellAudio[1]=atoi(buffer);
			continue;
		}
		CHECKFOR("BT848_GPIO_DATA_RADIO="){
			Pos=Pos+22;
			adjust();
			ManuellAudio[2]=atoi(buffer);
			continue;
		}
		CHECKFOR("BT848_GPIO_DATA_EXTERN="){
			Pos=Pos+23;
			adjust();
			ManuellAudio[3]=atoi(buffer);
			continue;
		}
		CHECKFOR("BT848_GPIO_DATA_INTERN="){
			Pos=Pos+23;
			adjust();
			ManuellAudio[4]=atoi(buffer);
			continue;
		}
		CHECKFOR("BT848_GPIO_DATA_AUS="){
			Pos=Pos+20;
			adjust();
			ManuellAudio[5]=atoi(buffer);
			continue;
		}
		CHECKFOR("BT848_GPIO_DATA_AN="){
			Pos=Pos+19;
			adjust();
			ManuellAudio[6]=atoi(buffer);
			continue;
		}
		CHECKFOR("BT848-PLL="){
			Pos=Pos+10;
			INIT_PLL=0;
			j=0;
			buffer[j]=0x00;
			CHECKTRUE() {
				INIT_PLL=1;
			}
			if (( *Pos == 'F' ) ||  ( *Pos == 'f' )) {
				INIT_PLL=0;
			}  {
				buffer[j]=0x00;
				while ( Pos < SemmelPos) {
					if ( *Pos != 0x20 ) {
						buffer[j]=*Pos;
						j++;
						buffer[j]=0x00;
					}
					Pos++;
				}
				INIT_PLL=atoi(buffer);
			};
		}
#endif

		CHECKFOR("USE_OVERLAY_STRECHING="){
			Pos=Pos+22;
			USE_DX_OV_STRECH=TRUE;
			if (( *Pos == 'F' ) ||  ( *Pos == 'f' )) {
				USE_DX_OV_STRECH=FALSE;
			}
			continue;
		}
		CHECKFOR("USE_DX_LOCK="){
			Pos=Pos+12;
			USE_DX_LOCK=FALSE;
			CHECKTRUE() {
				USE_DX_LOCK=TRUE;
			}
			continue;
		}
		CHECKFOR("WAIT_DX_VBI="){
			Pos=Pos+12;
			WAIT_DX_VBI=FALSE;
			CHECKTRUE() {
				WAIT_DX_VBI=TRUE;
			}
			continue;
		}
		CHECKFOR("TUNER_USE="){
			Pos=Pos+10;
			USETUNER=TRUE;
			if (( *Pos == 'F' ) ||  ( *Pos == 'f' )) {
				USETUNER=FALSE;
			}
			continue;
		}
		CHECKFOR("CARD_USE="){
			Pos=Pos+9;
			USECARD=TRUE;
			if (( *Pos == 'F' ) ||  ( *Pos == 'f' )) {
				USECARD=FALSE;
			}
			continue;
		}
		CHECKFOR("CAPTUREVIDEO="){
			Pos=Pos+13;
			Capture_Video=FALSE;
			CHECKTRUE() {
				Capture_Video=TRUE;
			}
			continue;
			
		}
		CHECKFOR("CAPTUREVBI="){
			Pos=Pos+11;
			Capture_VBI=FALSE;
			CHECKTRUE() {
				Capture_VBI=TRUE;
			}
			continue;
			
		}
		CHECKFOR("LASTMODE="){
			Pos=Pos+9;
#ifdef USE_BT8X8
            Current_Mode = (( *Pos == 'D' ) ||  ( *Pos == 'd' )) ? 1 : 0;
#endif
			continue;
			
		}
		CHECKFOR("LETZTES_PROGRAMM="){
			Pos=Pos+17;
			adjust();
			InitialProg=atoi(buffer);
			continue;
		} CHECKFOR("DVB-DECODER="){
			Pos=Pos+12;
			adjust();
			DVBDecoderType=atoi(buffer);
			continue;
		}
		CHECKFOR("DVB-TUNER-TYPE="){
			Pos=Pos+15;
			adjust();
			DVBTunerType=atoi(buffer);
			continue;
		} CHECKFOR("DVB-AUDIO="){
			Pos=Pos+10;
			adjust();
			DVB_AUDIO=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-WriteRegDelay="){
			Pos=Pos+21;
			adjust();
			Timing.WriteRegDelay=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-I2C_Bus_Timeout="){
			Pos=Pos+23;
			adjust();
			Timing.I2C_Bus_Timeout=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-I2C_Bus_Reset_Delay="){
			Pos=Pos+27;
			adjust();
			Timing.I2C_Bus_Reset_Delay=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-DVB_Reset_Wait="){
			Pos=Pos+22;
			adjust();
			Timing.DVB_Reset_Wait=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-DVB_Reset_Scan_Wait="){
			Pos=Pos+27;
			adjust();
			Timing.DVB_Reset_Scan_Wait=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-SendDiSEqCDelay="){
			Pos=Pos+23;
			adjust();
			Timing.SendDiSEqCDelay=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-Pmt_TimeOut="){
			Pos=Pos+19;
			adjust();
			Timing.Pmt_TimeOut=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-Sdt_TimeOut="){
			Pos=Pos+19;
			adjust();
			Timing.Sdt_TimeOut=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-Pat_TimeOut="){
			Pos=Pos+19;
			adjust();
			Timing.Pat_TimeOut=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-Cat_TimeOut="){
			Pos=Pos+19;
			adjust();
			Timing.Cat_TimeOut=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-Nit_TimeOut="){
			Pos=Pos+19;
			adjust();
			Timing.Nit_TimeOut=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-Debi_Cmd_Time_Out="){
			Pos=Pos+25;
			adjust();
			Timing.Debi_Cmd_Time_Out=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-OSD_Text_Time_Out="){
			Pos=Pos+25;
			adjust();
			Timing.OSD_Text_Time_Out=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-Debi_done_MC2="){
			Pos=Pos+21;
			adjust();
			Timing.Debi_done_MC2=atoi(buffer);
			continue;
		} CHECKFOR("TIMING-Debi_done_PSR="){
			Pos=Pos+21;
			adjust();
			Timing.Debi_done_PSR=atoi(buffer);
			continue;
		}  CHECKFOR("FASTSWITCH="){
			Pos=Pos+11;
			FastSwitch=FALSE;
			CHECKTRUE() {
				FastSwitch=TRUE;
			}
			continue;
		}  CHECKFOR("EIT_EVENT="){
			Pos=Pos+10;
			Capture_Event_EIT=FALSE;
			CHECKTRUE() {
				Capture_Event_EIT=TRUE;
			}
			continue;
		}  CHECKFOR("TID_EVENT="){
			Pos=Pos+10;
			Capture_TID=FALSE;
			CHECKTRUE() {
				Capture_TID=TRUE;
			}
			continue;
		}  CHECKFOR("DIGI_TEXT_HARWARE="){
			Pos=Pos+18;
			Capture_Digi_Text=FALSE;
			CHECKTRUE() {
				Capture_Digi_Text=TRUE;
			}
			continue;
		}  CHECKFOR("STREAM_USE_VT="){
			Pos=Pos+14;
			DVB_VT=FALSE;
			CHECKTRUE() {
				DVB_VT=TRUE;
			}
			continue;
		}  CHECKFOR("STREAM_USE_WINBIS="){
			Pos=Pos+18;
			DVB_WINBIS=FALSE;
			CHECKTRUE() {
				DVB_WINBIS=TRUE;
			}
			continue;
		}  CHECKFOR("VIEW_ANALOG="){
			Pos=Pos+12;
			VIEW_ANALOG=FALSE;
			CHECKTRUE() {
				VIEW_ANALOG=TRUE;
			}
			continue;
		}  CHECKFOR("VIEW_DIGITV="){
			Pos=Pos+12;
			VIEW_DIGITV=FALSE;
			CHECKTRUE() {
				VIEW_DIGITV=TRUE;
			}
			continue;
		}  CHECKFOR("VIEW_DIGIRADIO="){
			Pos=Pos+15;
			VIEW_DIGIRADIO=FALSE;
			CHECKTRUE() {
				VIEW_DIGIRADIO=TRUE;
			}
			continue;
		}  CHECKFOR("VIEW_DIGI_REST="){
			Pos=Pos+15;
			VIEW_DIGI_REST=FALSE;
			CHECKTRUE() {
				VIEW_DIGI_REST=TRUE;
			}
			continue;
		}  CHECKFOR("VIEW_TEMP_CHANNEL="){
			Pos=Pos+18;
			VIEW_TEMP_CHANNEL=FALSE;
			CHECKTRUE() {
				VIEW_TEMP_CHANNEL=TRUE;
			}
			continue;
		}  CHECKFOR("VIEW_LINK_CHANNEL="){
			Pos=Pos+18;
			VIEW_LINK_CHANNEL=FALSE;
			CHECKTRUE() {
				VIEW_LINK_CHANNEL=TRUE;
			}
			continue;
		}  CHECKFOR("VIEW_FREE="){
			Pos=Pos+10;
			VIEW_FREE=FALSE;
			CHECKTRUE() {
				VIEW_FREE=TRUE;
			}
			continue;
		}  CHECKFOR("FAST_BOOT="){
			Pos=Pos+10;
			FastBoot=FALSE;
			CHECKTRUE() {
				FastBoot=TRUE;
			} 
			continue;
		}  CHECKFOR("VES1820_INVERSION="){
			Pos=Pos+18;
			adjust();
			VES1820_Inversion=atoi(buffer);
			continue;
		}  CHECKFOR("FIRM_DIR="){
			Pos=Pos+9;
			j=0;
			FIRM_DIR[j]=0x00;
			while ( Pos < SemmelPos) {
				FIRM_DIR[j]=*Pos;
				j++;
				FIRM_DIR[j]=0x00;
				Pos++;
			}
			continue;
		}  CHECKFOR("FIRMVERSION="){
			Pos=Pos+12;
			j=0;
			FirmID[j]=0x00;
			while ( Pos < SemmelPos) {
				FirmID[j]=*Pos;
				j++;
				FirmID[j]=0x00;
				Pos++;
			}
			continue;
		}  CHECKFOR("SAVE_DVB_BOOT="){
			Pos=Pos+14;
			SAVE_BOOT=FALSE;
			CHECKTRUE() {
				SAVE_BOOT=TRUE;
			}
			continue;
		}  CHECKFOR("TIMEZONE="){
			Pos=Pos+9;
			adjust();
			TIMEZONE=atoi(buffer);
			continue;
		};
		for (i=0; i<16; i++) {
			char buf[16];
			sprintf(buf, "SYMBOLRATE%d=", i);
			CHECKFOR(buf){
				Pos=Pos+12;
				adjust();
				SRates[i]=atoi(buffer);
				break;
			}
		}
		if (i<16) continue;
		
		CHECKFOR("RECORD_FORMAT="){
			Pos=Pos+14;
			adjust();
			RecorderFormat=atoi(buffer);
			continue;
		} CHECKFOR("RECORD_TYP="){
			Pos=Pos+11;
			adjust();
			RecorderTyp=atoi(buffer);
			continue;
		}  CHECKFOR("RECORD_MAX_FILE_SIZE="){
			Pos=Pos+21;
			adjust();
			MAXMegaBytes=atoi(buffer);
			continue;
		} CHECKFOR("RECORDERBASEDIR1="){
			Pos=Pos+17;
			j=0;
			RecordBaseDir[0][j]=0x00;
			while ( Pos < SemmelPos) {
				RecordBaseDir[0][j]=*Pos;
				j++;
				RecordBaseDir[0][j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("RECORDERBASEDIR2="){
			Pos=Pos+17;
			j=0;
			RecordBaseDir[1][j]=0x00;
			while ( Pos < SemmelPos) {
				RecordBaseDir[1][j]=*Pos;
				j++;
				RecordBaseDir[1][j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("RECORDERBASEDIR3="){
			Pos=Pos+17;
			j=0;
			RecordBaseDir[2][j]=0x00;
			while ( Pos < SemmelPos) {
				RecordBaseDir[2][j]=*Pos;
				j++;
				RecordBaseDir[2][j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("RECORDERBASEDIR4="){
			Pos=Pos+17;
			j=0;
			RecordBaseDir[3][j]=0x00;
			while ( Pos < SemmelPos) {
				RecordBaseDir[3][j]=*Pos;
				j++;
				RecordBaseDir[3][j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("RECORDERBASEDIR5="){
			Pos=Pos+17;
			j=0;
			RecordBaseDir[4][j]=0x00;
			while ( Pos < SemmelPos) {
				RecordBaseDir[4][j]=*Pos;
				j++;
				RecordBaseDir[4][j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("RECORDERBASEDIR6="){
			Pos=Pos+17;
			j=0;
			RecordBaseDir[5][j]=0x00;
			while ( Pos < SemmelPos) {
				RecordBaseDir[5][j]=*Pos;
				j++;
				RecordBaseDir[5][j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("RECORDERBASEDIR7="){
			Pos=Pos+17;
			j=0;
			RecordBaseDir[6][j]=0x00;
			while ( Pos < SemmelPos) {
				RecordBaseDir[6][j]=*Pos;
				j++;
				RecordBaseDir[6][j]=0x00;
				Pos++;
			}
			continue;
		}                
		CHECKFOR("RECORDERBASEDIR8="){
			Pos=Pos+17;
			j=0;
			RecordBaseDir[7][j]=0x00;
			while ( Pos < SemmelPos) {
				RecordBaseDir[7][j]=*Pos;
				j++;
				RecordBaseDir[7][j]=0x00;
				Pos++;
			}
			continue;
		}  CHECKFOR("LNB1_MIN_FREQ="){
			Pos=Pos+14;
			adjust();
			LNB[0].MinFreq=atoi(buffer);
			continue;
		}  CHECKFOR("LNB1_MAX_FREQ="){
			Pos=Pos+14;
			adjust();
			LNB[0].MaxFreq=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB1_LOF_LOW="){
			Pos=Pos+13;
			adjust();
			LNB[0].LofLow=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB1_LOF_HIGH="){
			Pos=Pos+14;
			adjust();
			LNB[0].LofHigh=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB1_SWITCHFREQ="){
			Pos=Pos+16;
			adjust();
			LNB[0].SwitchFreq=atoi(buffer);
			continue;
		}
		CHECKFOR("LNB1_POWER="){
			Pos=Pos+11;
			LNB[0].Power=FALSE;
			CHECKTRUE() {
				LNB[0].Power=TRUE;
			}
			continue;
		} CHECKFOR("LNB1_POWERRESET="){
			Pos=Pos+16;
			LNB[0].PowerReset=FALSE;
			CHECKTRUE() {
				LNB[0].PowerReset=TRUE;
			}
			continue;
		}  CHECKFOR("LNB1_SWITCH_22KHZ="){
			Pos=Pos+18;
			LNB[0].Switch22khz=FALSE;
			CHECKTRUE() {
				LNB[0].Switch22khz=TRUE;
			}
			continue;
		}	 CHECKFOR("LNB1_MINIDISEQC="){
			Pos=Pos+16;
			LNB[0].MiniDiseqc=0x00;
			CHECKTRUE() {
				LNB[0].MiniDiseqc=0x01;
			}
			continue;
		}	 CHECKFOR("LNB1_TONEBURST="){
			Pos=Pos+15;
			adjust();
			LNB[0].BurstVal=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB1_POS="){
			Pos=Pos+9;
			adjust();
			LNB[0].PosVal=buffer[0];
			continue;
		}	 CHECKFOR("LNB1_OPT="){
			Pos=Pos+9;
			adjust();
			LNB[0].OptVal=buffer[0];
			continue;
        }  CHECKFOR("LNB1_ORBIT="){
			Pos=Pos+11;
			adjust();
			LNB[0].orbit=atoi(buffer);
			continue;
        }  CHECKFOR("LNB1_WEST_EAST="){
			Pos=Pos+15;
			adjust();
			LNB[0].west_east=buffer[0];
			continue;
		}  CHECKFOR("LNB2_MIN_FREQ="){
			Pos=Pos+14;
			adjust();
			LNB[1].MinFreq=atoi(buffer);
			continue;
		}  CHECKFOR("LNB2_MAX_FREQ="){
			Pos=Pos+14;
			adjust();
			LNB[1].MaxFreq=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB2_LOF_LOW="){
			Pos=Pos+13;
			adjust();
			LNB[1].LofLow=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB2_LOF_HIGH="){
			Pos=Pos+14;
			adjust();
			LNB[1].LofHigh=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB2_SWITCHFREQ="){
			Pos=Pos+16;
			adjust();
			LNB[1].SwitchFreq=atoi(buffer);
			continue;
		}
		CHECKFOR("LNB2_POWER="){
			Pos=Pos+11;
			LNB[1].Power=FALSE;
			CHECKTRUE() {
				LNB[1].Power=TRUE;
			}
			continue;
		} CHECKFOR("LNB2_POWERRESET="){
			Pos=Pos+16;
			LNB[1].PowerReset=FALSE;
			CHECKTRUE() {
				LNB[1].PowerReset=TRUE;
			}
			continue;
		}  CHECKFOR("LNB2_SWITCH_22KHZ="){
			Pos=Pos+18;
			LNB[1].Switch22khz=FALSE;
			CHECKTRUE() {
				LNB[1].Switch22khz=TRUE;
			}
			continue;
		}	 CHECKFOR("LNB2_MINIDISEQC="){
			Pos=Pos+16;
			LNB[1].MiniDiseqc=0x00;
			CHECKTRUE() {
				LNB[1].MiniDiseqc=0x01;
			}
			continue;
		}	 CHECKFOR("LNB2_TONEBURST="){
			Pos=Pos+15;
			adjust();
			LNB[1].BurstVal=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB2_POS="){
			Pos=Pos+9;
			adjust();
			LNB[1].PosVal=buffer[0];
			continue;
		}	 CHECKFOR("LNB2_OPT="){
			Pos=Pos+9;
			adjust();
			LNB[1].OptVal=buffer[0];
			continue;
        }  CHECKFOR("LNB2_ORBIT="){
			Pos=Pos+11;
			adjust();
			LNB[1].orbit=atoi(buffer);
			continue;
        }  CHECKFOR("LNB2_WEST_EAST="){
			Pos=Pos+15;
			adjust();
			LNB[1].west_east=buffer[0];
			continue;
		}  CHECKFOR("LNB2_USE="){
			Pos=Pos+9;
			LNB[1].Use=FALSE;
			CHECKTRUE() {
				LNB[1].Use=TRUE;
			}
			continue;
		}  CHECKFOR("LNB3_MIN_FREQ="){
			Pos=Pos+14;
			adjust();
			LNB[2].MinFreq=atoi(buffer);
			continue;
		}  CHECKFOR("LNB3_MAX_FREQ="){
			Pos=Pos+14;
			adjust();
			LNB[2].MaxFreq=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB3_LOF_LOW="){
			Pos=Pos+13;
			adjust();
			LNB[2].LofLow=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB3_LOF_HIGH="){
			Pos=Pos+14;
			adjust();
			LNB[2].LofHigh=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB3_SWITCHFREQ="){
			Pos=Pos+16;
			adjust();
			LNB[2].SwitchFreq=atoi(buffer);
			continue;
		} CHECKFOR("LNB3_POWERRESET="){
			Pos=Pos+16;
			LNB[2].PowerReset=FALSE;
			CHECKTRUE() {
				LNB[2].PowerReset=TRUE;
			}
			continue;
		} CHECKFOR("LNB3_POWER="){
			Pos=Pos+11;
			LNB[2].Power=FALSE;
			CHECKTRUE() {
				LNB[2].Power=TRUE;
			}
			continue;
		}  CHECKFOR("LNB3_SWITCH_22KHZ="){
			Pos=Pos+18;
			LNB[2].Switch22khz=FALSE;
			CHECKTRUE() {
				LNB[2].Switch22khz=TRUE;
			}
			continue;
		}	 CHECKFOR("LNB3_MINIDISEQC="){
			Pos=Pos+16;
			LNB[2].MiniDiseqc=0x00;
			CHECKTRUE() {
				LNB[2].MiniDiseqc=0x01;
			}
			continue;
		}	 CHECKFOR("LNB3_TONEBURST="){
			Pos=Pos+15;
			adjust();
			LNB[2].BurstVal=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB3_POS="){
			Pos=Pos+9;
			adjust();
			LNB[2].PosVal=buffer[0];
			continue;
		}	 CHECKFOR("LNB3_OPT="){
			Pos=Pos+9;
			adjust();
			LNB[2].OptVal=buffer[0];
			continue;
        }  CHECKFOR("LNB3_ORBIT="){
			Pos=Pos+11;
			adjust();
			LNB[2].orbit=atoi(buffer);
			continue;
        }  CHECKFOR("LNB3_WEST_EAST="){
			Pos=Pos+15;
			adjust();
			LNB[3].west_east=buffer[0];
			continue;
		}  CHECKFOR("LNB3_USE="){
			Pos=Pos+9;
			LNB[2].Use=FALSE;
			CHECKTRUE() {
				LNB[2].Use=TRUE;
			}
			continue;
		}  CHECKFOR("LNB4_MIN_FREQ="){
			Pos=Pos+14;
			adjust();
			LNB[3].MinFreq=atoi(buffer);
			continue;
		}  CHECKFOR("LNB4_MAX_FREQ="){
			Pos=Pos+14;
			adjust();
			LNB[3].MaxFreq=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB4_LOF_LOW="){
			Pos=Pos+13;
			adjust();
			LNB[3].LofLow=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB4_LOF_HIGH="){
			Pos=Pos+14;
			adjust();
			LNB[3].LofHigh=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB4_SWITCHFREQ="){
			Pos=Pos+16;
			adjust();
			LNB[3].SwitchFreq=atoi(buffer);
			continue;
		} CHECKFOR("LNB4_POWERRESET="){
			Pos=Pos+16;
			LNB[3].PowerReset=FALSE;
			CHECKTRUE() {
				LNB[3].PowerReset=TRUE;
			}
			continue;
		} CHECKFOR("LNB4_POWER="){
			Pos=Pos+11;
			LNB[3].Power=FALSE;
			CHECKTRUE() {
				LNB[3].Power=TRUE;
			}
			continue;
		}  CHECKFOR("LNB4_SWITCH_22KHZ="){
			Pos=Pos+18;
			LNB[3].Switch22khz=FALSE;
			CHECKTRUE() {
				LNB[3].Switch22khz=TRUE;
			}
			continue;
		}	 CHECKFOR("LNB4_MINIDISEQC="){
			Pos=Pos+16;
			LNB[3].MiniDiseqc=0x00;
			CHECKTRUE() {
				LNB[3].MiniDiseqc=0x01;
			}
			continue;
		}	 CHECKFOR("LNB4_TONEBURST="){
			Pos=Pos+15;
			adjust();
			LNB[3].BurstVal=atoi(buffer);
			continue;
		}	 CHECKFOR("LNB4_POS="){
			Pos=Pos+9;
			adjust();
			LNB[3].PosVal=buffer[0];
			continue;
		}	 CHECKFOR("LNB4_OPT="){
			Pos=Pos+9;
			adjust();
			LNB[3].OptVal=buffer[0];
			continue;
        }  CHECKFOR("LNB4_ORBIT="){
			Pos=Pos+11;
			adjust();
			LNB[3].orbit=atoi(buffer);
			continue;
        }  CHECKFOR("LNB4_WEST_EAST="){
			Pos=Pos+15;
			adjust();
			LNB[3].west_east=buffer[0];
			continue;
		}  CHECKFOR("LNB4_USE="){
			Pos=Pos+9;
			LNB[3].Use=FALSE;
			CHECKTRUE() {
				LNB[3].Use=TRUE;
			}
			continue;
		}  CHECKFOR("DVB-TUNER-CABLE="){
			Pos=Pos+16;
			DVBTuners[5].CableTuner=FALSE;
			CHECKTRUE() {
				DVBTuners[5].CableTuner=TRUE;
			}
			continue;
		}  CHECKFOR("DVB-TUNER-MIN="){
			Pos=Pos+14;
			adjust();
			DVBTuners[5].min=atoi(buffer)*1000;
			continue;
		}
		CHECKFOR("DVB-TUNER-MAX="){
			Pos=Pos+14;
			adjust();
			DVBTuners[5].max=atoi(buffer)*1000;
			continue;
		}
		CHECKFOR("DVB-TUNER-RES="){
			Pos=Pos+14;
			adjust();
			DVBTuners[5].res=atoi(buffer);
			continue;
		}
		CHECKFOR("DVB-TUNER-STEP="){
			Pos=Pos+15;
			adjust();
			DVBTuners[5].step=atoi(buffer);
			continue;
		}
		CHECKFOR("DVB-TUNER-THRESH1="){
			Pos=Pos+18;
			adjust();
			DVBTuners[5].thresh1=atoi(buffer)*1000;
			continue;
		}
		CHECKFOR("DVB-TUNER-THRESH2="){
			Pos=Pos+18;
			adjust();
			DVBTuners[5].thresh2=atoi(buffer)*1000;
			continue;
		}
		CHECKFOR("DVB-TUNER-VHF_L="){
			Pos=Pos+16;
			adjust();
			DVBTuners[5].VHF_L=atoi(buffer);
			continue;
		}
		CHECKFOR("DVB-TUNER-VHF_H="){
			Pos=Pos+16;
			adjust();
			DVBTuners[5].VHF_H=atoi(buffer);
			continue;
		}
		CHECKFOR("DVB-TUNER-UHF="){
			Pos=Pos+14;
			adjust();
			DVBTuners[5].UHF=atoi(buffer);
			continue;
		}
		CHECKFOR("DVB-TUNER-CONFIG="){
			Pos=Pos+17;
			adjust();
			DVBTuners[5].config=atoi(buffer);
			continue;
		}
		CHECKFOR("DVB-TUNER-I2C="){
			Pos=Pos+14;
			adjust();
			DVBTuners[5].I2C=atoi(buffer);
			continue;
		}
		CHECKFOR("DVB-TUNER-IFPCOFF="){
			Pos=Pos+18;
			adjust();
			DVBTuners[5].IFPCoff=atoi(buffer)*1000;
			continue;
		}
		CHECKFOR("DVB-CONTRAST="){
			Pos=Pos+13;
			adjust();
			InitialDVBContrast=atoi(buffer);
			continue;
		}
		CHECKFOR("DVB-BRIGHTNESS="){
			Pos=Pos+15;
			adjust();
			InitialDVBBrightness=atoi(buffer);
			continue;
		}
		CHECKFOR("DVB-COLOR="){
			Pos=Pos+10;
			adjust();
			InitialDVBColor=atoi(buffer);
			continue;
		}
		CHECKFOR("DVB-VOLUME-LEFT="){
			Pos=Pos+16;
			adjust();
			InitialDVBVolumeLinks=atoi(buffer);
			continue;
		}
		CHECKFOR("DVB-VOLUME-RIGHT="){
			Pos=Pos+17;
			adjust();
			InitialDVBVolumeRechts=atoi(buffer);
			continue;
		}
		CHECKFOR("SAVESETTING="){
			Pos=Pos+12;
			SaveSettings=FALSE;
			CHECKTRUE() {
				SaveSettings=TRUE;
			}
			continue;
			
		}
		
		CHECKFOR("LAENDERCODE="){
			Pos=Pos+12;
			adjust();
			CountryCode=atoi(buffer);
			continue;
		}
		CHECKFOR("COLORFORMAT="){
			Pos=Pos+12;
			adjust();
			ColourFormat=atoi(buffer);
			continue;
		}
		CHECKFOR("INVERT-ROT="){
			Pos=Pos+11;
			InvertRot=FALSE;
			CHECKTRUE() {
				InvertRot=TRUE;
			}
			continue;
			
		} CHECKFOR("INVERT-GRUEN="){
			Pos=Pos+13;
			InvertGruen=FALSE;
			CHECKTRUE() {
				InvertGruen=TRUE;
			}
			continue;
		} CHECKFOR("INVERT-BLAU="){
			Pos=Pos+12;
			InvertBlau=FALSE;
			CHECKTRUE() {
				InvertBlau=TRUE;
			}
			continue;
		} CHECKFOR("OFF-ROT="){
			Pos=Pos+8;
			OffRot=FALSE;
			CHECKTRUE() {
				OffRot=TRUE;
			}
			continue;
		} CHECKFOR("OFF-GRUEN="){
			Pos=Pos+10;
			OffGruen=FALSE;
			CHECKTRUE() {
				OffGruen=TRUE;
			}
			continue;
		} CHECKFOR("OFF-BLAU="){
			Pos=Pos+9;
			OffBlau=FALSE;
			CHECKTRUE() {
				OffBlau=TRUE;
			}
			continue;
		} CHECKFOR("COLORMIXMODE1="){
			Pos=Pos+14;
			HCMode1=FALSE;
			CHECKTRUE() {
				HCMode1=TRUE;
			}
			continue;
		} CHECKFOR("MIXSCHWELLE="){
			Pos=Pos+12;
			adjust();
			HCSchwelle=atoi(buffer);
			continue;
		} CHECKFOR("MIXROT="){
			Pos=Pos+7;
			adjust();
			HCRotFaktor=atoi(buffer);
			continue;
		} CHECKFOR("MIXGRUEN="){
			Pos=Pos+9;
			adjust();
			HCGruenFaktor=atoi(buffer);
			continue;
		} CHECKFOR("MIXBLAU="){
			Pos=Pos+8;
			adjust();
			HCBlauFaktor=atoi(buffer);
			continue;
		} CHECKFOR("COLORMIXMODE2="){
			Pos=Pos+14;
			HCMode2=FALSE;
			CHECKTRUE() {
				HCMode2=TRUE;
			}
			continue;
		} CHECKFOR("MIXCUTSCHWELLE="){
			Pos=Pos+15;
			adjust();
			HCCutFaktor=atoi(buffer);
			continue;
		} CHECKFOR("MIXCUTLOROT="){
			Pos=Pos+12;
			adjust();
			HCCutLoRotFaktor=atoi(buffer);
			continue;
		} CHECKFOR("MIXCUTLOGRUEN="){
			Pos=Pos+14;
			adjust();
			HCCutLoGruenFaktor=atoi(buffer);
			continue;
		} CHECKFOR("MIXCUTLOBLAU="){
			Pos=Pos+13;
			adjust();
			HCCutLoBlauFaktor=atoi(buffer);
			continue;
		} CHECKFOR("MIXCUTHIROT="){
			Pos=Pos+12;
			adjust();
			HCCutHiRotFaktor=atoi(buffer);
			continue;
		} CHECKFOR("MIXCUTHIGRUEN="){
			Pos=Pos+14;
			adjust();
			HCCutHiGruenFaktor=atoi(buffer);
			continue;
		} CHECKFOR("MIXCUTHIBLAU="){
			Pos=Pos+13;
			adjust();
			HCCutHiBlauFaktor=atoi(buffer);
			continue;
		} CHECKFOR("VT-SPRACHE="){
			Pos=Pos+11;
			adjust();
			VT_Lang=atoi(buffer);
			continue;
		} CHECKFOR("VT-SCREENFONT="){
			Pos=Pos+14;
			adjust();
			Initial_FontNr=atoi(buffer);
			continue;
		} CHECKFOR("VT-EXPORT-TYP="){
			Pos=Pos+14;
			adjust();
			VT_EXPORT_TYP=atoi(buffer);
			continue;
		} CHECKFOR("VT-BASE-DIR="){
			Pos=Pos+12;
			j=0;
			VT_BASE_DIR[j]=0x00;
			while ( Pos < SemmelPos) {
				VT_BASE_DIR[j]=*Pos;
				j++;
				VT_BASE_DIR[j]=0x00;
				Pos++;
			}
			continue;
		} CHECKFOR("VT-ALWAYS-EXPORT="){
			Pos=Pos+17;
			VT_ALWAYS_EXPORT=FALSE;
			CHECKTRUE() {
				VT_ALWAYS_EXPORT=TRUE;
			}
			continue;
		} CHECKFOR("VT-EXPORT-BACKGROUND="){
			Pos=Pos+21;
			VT_EXPORT_BACKGROUND=FALSE;
			CHECKTRUE() {
				VT_EXPORT_BACKGROUND=TRUE;
			}
			continue;
		} CHECKFOR("VT-EXPORT-LINKS="){
			Pos=Pos+16;
			VT_EXPORT_PAGE_LINK=FALSE;
			CHECKTRUE() {
				VT_EXPORT_PAGE_LINK=TRUE;
			}
			continue;
		} CHECKFOR("VT-EXPORT-PAGE-CONTROL="){
			Pos=Pos+23;
			VT_EXPORT_PAGE_CONTROL=FALSE;
			CHECKTRUE() {
				VT_EXPORT_PAGE_CONTROL=TRUE;
			}
			continue;
		} CHECKFOR("VT-EXPORT-FONT="){
			Pos=Pos+15;
			adjust();
			Initial_Export_Font=atoi(buffer);
			continue;
		} CHECKFOR("VT-COLOR-SCHWARZ="){
			Pos=Pos+17;
			j=sscanf(Pos,"%d %d %d",&Color_R,&Color_G,&Color_B);
			if ( j == 3 ) {
				VTColourTable[0]=RGB(Color_R,Color_G,Color_B);     
			};
			continue;
		} CHECKFOR("VT-COLOR-ROT="){
			Pos=Pos+13;
			j=sscanf(Pos,"%d %d %d",&Color_R,&Color_G,&Color_B);
			if ( j == 3 ) {
				VTColourTable[1]=RGB(Color_R,Color_G,Color_B);     
			};
			continue;
		} CHECKFOR("VT-COLOR-GRUEN="){
			Pos=Pos+15;
			j=sscanf(Pos,"%d %d %d",&Color_R,&Color_G,&Color_B);
			if ( j == 3 ) {
				VTColourTable[2]=RGB(Color_R,Color_G,Color_B);     
			};
			continue;
		} CHECKFOR("VT-COLOR-GELB="){
			Pos=Pos+14;
			j=sscanf(Pos,"%d %d %d",&Color_R,&Color_G,&Color_B);
			if ( j == 3 ) {
				VTColourTable[3]=RGB(Color_R,Color_G,Color_B);     
			};
			continue;
		} CHECKFOR("VT-COLOR-BLAU="){
			Pos=Pos+14;
			j=sscanf(Pos,"%d %d %d",&Color_R,&Color_G,&Color_B);
			if ( j == 3 ) {
				VTColourTable[4]=RGB(Color_R,Color_G,Color_B);     
			};
			continue;
		} CHECKFOR("VT-COLOR-UNSICHTBAR="){
			Pos=Pos+20;
			j=sscanf(Pos,"%d %d %d",&Color_R,&Color_G,&Color_B);
			if ( j == 3 ) {
				VTColourTable[5]=RGB(Color_R,Color_G,Color_B);     
			};
			continue;
		} CHECKFOR("VT-COLOR-HELLBLAU="){
			Pos=Pos+18;
			j=sscanf(Pos,"%d %d %d",&Color_R,&Color_G,&Color_B);
			if ( j == 3 ) {
				VTColourTable[6]=RGB(Color_R,Color_G,Color_B);     
			};
			continue;
		} CHECKFOR("VT-COLOR-WEIS="){
			Pos=Pos+14;
			j=sscanf(Pos,"%d %d %d",&Color_R,&Color_G,&Color_B);
			if ( j == 3 ) {
				VTColourTable[7]=RGB(Color_R,Color_G,Color_B);     
			};
			continue;
		} CHECKFOR("VT-COLOR-TRANSPARENT="){
			Pos=Pos+21;
			j=sscanf(Pos,"%d %d %d",&Color_R,&Color_G,&Color_B);
			if ( j == 3 ) {
				VTColourTable[8]=RGB(Color_R,Color_G,Color_B);     
			};
			continue;
		}

#ifdef USE_BT8X8
		CHECKFOR("MSPMODE="){
			Pos=Pos+8;
			adjust();
			MSPMode=atoi(buffer);
			continue;
		} CHECKFOR("MSPMAJOR="){
			Pos=Pos+9;
			adjust();
			MSPMajorMode=atoi(buffer);
			continue;
		} CHECKFOR("MSPMINOR="){
			Pos=Pos+9;
			adjust();
			MSPMinorMode=atoi(buffer);
			continue;
		} CHECKFOR("MSPSTEREO="){
			Pos=Pos+10;
			adjust();
			MSPStereo=atoi(buffer);
			continue;
		} CHECKFOR("MSP-STEREO="){
			Pos=Pos+11;
			AutoStereoSelect=TRUE;
			if (( *Pos == 'F' ) ||  ( *Pos == 'f' )) {
				AutoStereoSelect=FALSE;
			}
			continue;
		} CHECKFOR("IC2_VOLUME="){
			Pos=Pos+11;
			adjust();
			InitialVolume=atoi(buffer);
			continue;
		} CHECKFOR("IC2_SPATIAL="){
			Pos=Pos+12;
			adjust();
			InitialSpecial=atoi(buffer);
			continue;
		} CHECKFOR("IC2_LOUDNESS="){
			Pos=Pos+13;
			adjust();
			InitialLoudness=atoi(buffer);
			continue;
		} CHECKFOR("IC2_BASS="){
			Pos=Pos+9;
			adjust();
			InitialBass=atoi(buffer);
			continue;
		} CHECKFOR("IC2_TREBLE="){
			Pos=Pos+11;
			adjust();
			InitialTreble=atoi(buffer);
			continue;
		} CHECKFOR("IC2_BALANCE="){
			Pos=Pos+12;
			adjust();
			InitialBalance=atoi(buffer);
			continue;
		} CHECKFOR("I2C_SUPERBASS="){
			Pos=Pos+14;           
			InitialSuperBass=FALSE;
			CHECKTRUE() {
				InitialSuperBass=TRUE;
			}
			continue;
		} CHECKFOR("IC2_EQUALIZER1="){
			Pos=Pos+15;
			adjust();
			InitialEqualizer1=atoi(buffer);
			continue;
		} CHECKFOR("IC2_EQUALIZER2="){
			Pos=Pos+15;
			adjust();
			InitialEqualizer2=atoi(buffer);
			continue;
		} CHECKFOR("IC2_EQUALIZER3="){
			Pos=Pos+15;
			adjust();
			InitialEqualizer3=atoi(buffer);
			continue;
		} CHECKFOR("IC2_EQUALIZER4="){
			Pos=Pos+15;
			adjust();
			InitialEqualizer4=atoi(buffer);
			continue;
		} CHECKFOR("IC2_EQUALIZER5="){
			Pos=Pos+15;
			adjust();
			InitialEqualizer5=atoi(buffer);
			continue;
		}
#endif

		CHECKFOR("USE_MIXER="){
			Pos=Pos+10;           
			USE_MIXER=FALSE;
			CHECKTRUE() {
				USE_MIXER=TRUE;
			}
			continue;
		} CHECKFOR("MIXER_VOLUME_LINKER_KANAL="){
			Pos=Pos+26;
			adjust();
			MIXER_LINKER_KANAL=atoi(buffer);
			continue;
		} CHECKFOR("MIXER_VOLUME_RECHTER_KANAL="){
			Pos=Pos+27;
			adjust();
			MIXER_RECHTER_KANAL=atoi(buffer);
			continue;
		} CHECKFOR("MUTE_STATUS="){
			Pos=Pos+12;
			adjust();
			System_In_Mute = atoi(buffer);
			continue;
		} CHECKFOR("MIXER_VOLUME_CONTROL="){
			Pos=Pos+21;
			Volume.SoundSystem=-1;	 
			Volume.Destination=0;	 
			Volume.Connection=0;	 
			Volume.Control=0;	 
			j=sscanf(Pos,"%03d %03d %03d %03d",&Volume.SoundSystem,&Volume.Destination,&Volume.Connection,&Volume.Control );
			if ( j != 4 ) {
				Volume.SoundSystem=-1;	 
				Volume.Destination=0;	 
				Volume.Connection=0;	 
				Volume.Control=0;	 
			};
			continue;
		} CHECKFOR("MIXER_MUTE_CONTROL="){
			Pos=Pos+19;
			Mute.SoundSystem=-1;	 
			Mute.Destination=0;	 
			Mute.Connection=0;	 
			Mute.Control=0;	 
			j=sscanf(Pos,"%03d %03d %03d %03d",&Mute.SoundSystem,&Mute.Destination,&Mute.Connection,&Mute.Control );
			if ( j != 4 ) {
				Mute.SoundSystem=-1;	 
				Mute.Destination=0;	 
				Mute.Connection=0;	 
				Mute.Control=0;	 
			};
			continue;
		}

#ifdef USE_BT8X8
			// Fantasion
		CHECKFOR("FILTREBLEU="){
			Pos=Pos+11;
			bFilterBlueLines=FALSE;
			CHECKTRUE() {
				bFilterBlueLines=TRUE;
			}
			continue;
		} CHECKFOR("SEUILBLEU="){
			Pos=Pos+10;
			adjust();
			iUFilterThresholdPCent=atoi(buffer);
			continue;
		} CHECKFOR("SEUILROUGE="){
			Pos=Pos+11;
			adjust();
			iVFilterThresholdPCent=atoi(buffer);
			continue;
		}
#endif
		CHECKFOR("MIXER_BOOT_UP="){
			Pos=Pos+14;
			MixerLoad[B].MixerAccess.SoundSystem=-1;
			MixerLoad[B].MixerAccess.Destination=0;
			MixerLoad[B].MixerAccess.Connection=0;
			MixerLoad[B].MixerAccess.Control=0;
			MixerLoad[B].MixerValues.Kanal1=0;
			MixerLoad[B].MixerValues.Kanal2=0;
			MixerLoad[B].MixerValues.Kanal3=0;
			MixerLoad[B].MixerValues.Kanal4=0;
			j=sscanf(Pos,"%03d %03d %03d %03d %d %d %d %d",&MixerLoad[B].MixerAccess.SoundSystem,&MixerLoad[B].MixerAccess.Destination,&MixerLoad[B].MixerAccess.Connection,&MixerLoad[B].MixerAccess.Control,&MixerLoad[B].MixerValues.Kanal1,&MixerLoad[B].MixerValues.Kanal2,&MixerLoad[B].MixerValues.Kanal3,&MixerLoad[B].MixerValues.Kanal4);
			B++;
			if ( j != 8 ) {
				MixerLoad[B].MixerAccess.SoundSystem=-1;
				MixerLoad[B].MixerAccess.Destination=0;
				MixerLoad[B].MixerAccess.Connection=0;
				MixerLoad[B].MixerAccess.Control=0;
				MixerLoad[B].MixerValues.Kanal1=0;
				MixerLoad[B].MixerValues.Kanal2=0;
				MixerLoad[B].MixerValues.Kanal3=0;
				MixerLoad[B].MixerValues.Kanal4=0;
				B--;
			};
			
		} 
		
	}
	
	fclose(iniFile);
	
	return(TRUE);
}


BOOL Write_INI(LPSTR Name)
{
	
	FILE* iniFile;
	RECT nowRect; 
	WINDOWPLACEMENT nowPlc;

	int i,j;
	
	if ((iniFile=fopen(Name,"w")) == NULL )  {
		return(FALSE);
	}
	
	if (!topmost)
	{
	GetWindowRect(hWnd,&nowRect);
	}
	else
	{
		nowRect.left   = restoreX;
		nowRect.top    = restoreY;
		nowRect.right  = restoreX + restoreW;
		nowRect.bottom = restoreY + restoreH;
	}
	
	GetWindowPlacement (hWnd, &nowPlc);
	
	fprintf(iniFile,"; INI-Datei MultiDecoder Version 8.5\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; Benutzerdefinierbare Einstellungen \n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; WINDOWSTARTX,WINDOWSTARTY               Position des Windows beim Starten (linke obere Ecke)\n");
	fprintf(iniFile,"WINDOWSTARTX=%-8d;						     X-Coordinate\n",nowRect.left);
	fprintf(iniFile,"WINDOWSTARTY=%-8d;						     Y-Coordinate\n",nowRect.top);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; WINDOWSIZEX,WINDOWSIZEY                 Länge und Höhe des Windows beim Starten \n");
	fprintf(iniFile,"WINDOWSIZEX=%-8d;						     X-Länge\n",nowRect.right-nowRect.left);
	fprintf(iniFile,"WINDOWSIZEY=%-8d;						     Y-Länge\n",nowRect.bottom-nowRect.top);
	fprintf(iniFile,"; \n");
	// BigReaper - Flags
	fprintf(iniFile,"; BR_TOPMOST                              Maximierte Darstellung (Top Most) oder normale \n");
	fprintf(iniFile,"BR_TOPMOST=%-8d;                             0 = normal, 1 = maximiert\n", topmost);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; CURSOR_STATUS=                          Schalter für den Cursor Status\n");
	fprintf(iniFile,";                                              1 = Cursor an\n");
	fprintf(iniFile,";                                              0 = Cursor aus\n");
	fprintf(iniFile,"CURSOR_STATUS=%d;\n", CShowCursor);
	fprintf(iniFile,"; \n");

	// Moto - Flags
	fprintf(iniFile,"; DVBOUT=                                 Schalter für DVB-out only\n");
	fprintf(iniFile,";                                              1 = DVB-out only an\n");
	fprintf(iniFile,";                                              0 = DVB-out only aus\n");
	fprintf(iniFile,"DVBOUT=%d;\n", dvbout);
	fprintf(iniFile,"; \n");

	// R One
	fprintf(iniFile,"; LANGUAGE=                               Interface language\n");
	fprintf(iniFile,";                                              0 = german\n");
	fprintf(iniFile,";                                              1 = french\n");
	fprintf(iniFile,";                                              2 = english\n");
	fprintf(iniFile,";                                              3 = spanish\n");
	fprintf(iniFile,";                                              4 = italian\n");
	fprintf(iniFile,";                                              5 = portuguese\n");
	fprintf(iniFile,"LANGUAGE=%d;\n", language);
	fprintf(iniFile,"; \n");

	fprintf(iniFile,"; WINDOWSTYLE                             Art der Ausgabe\n");
	fprintf(iniFile,";                                               0 = Fenster\n");
	fprintf(iniFile,";                                               1 = 640*480\n");
	fprintf(iniFile,";                                               2 = 800*600\n");
	fprintf(iniFile,";                                               3 = RESX*RESY\n");
	fprintf(iniFile,"WINDOWSTYLE=%d;\n",WindowStyle);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; Bei WINDOWSTYLE=3 kann eine eigene Vollbildauflösung hinterlegt werden z.B. 720 * 576\n");
	fprintf(iniFile,"RESX=%d;\n",Res_X);
	fprintf(iniFile,"RESY=%d;\n",Res_Y);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"SHUTDOWN_OPTION=%-8d;                 Benutzer-Optionen für Shutdown-Verhalten\n",ShutDownValue);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; TOPMOST=                                Schalter für Ausgabe\n");
	fprintf(iniFile,";                                              TRUE  : Ausgabe immer im Vordergrund\n");
	fprintf(iniFile,";                                              FALSE : Ausgabe als normales Window\n");
	if ( ALLWAYS_TOP == TRUE ) fprintf(iniFile,"TOPMOST=TRUE;\n");
	else fprintf(iniFile,"TOPMOST=FALSE;\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; WRITE_LOG=                              Schalter für Log-Datei MD-BOOT.txt erzeugen\n");
	fprintf(iniFile,";                                              TRUE  : Log-Datei erzeugen \n");
	fprintf(iniFile,";                                              FALSE : Log-Datei nicht erzeugen\n");
	if ( WRITE_LOG == TRUE ) fprintf(iniFile,"WRITE_LOG=TRUE;\n");
	else fprintf(iniFile,"WRITE_LOG=FALSE;\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; KEEP_DRIVER_LOADED=                     Damit wird der Treiber beim Systemstart geladen\n");
	if ( Keep_Driver_Loaded == TRUE ) fprintf(iniFile,"KEEP_DRIVER_LOADED=TRUE;\n");
	else fprintf(iniFile,"KEEP_DRIVER_LOADED=FALSE;\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"PROCESSPRIORITY=%-8d;                 Process-Priorität\n",PriorClassId);
	fprintf(iniFile,";                                               0 = NORMAL_PRIORITY_CLASS\n");
	fprintf(iniFile,";                                               1 = HIGH_PRIORITY_CLASS\n");
	fprintf(iniFile,";                                               2 = REALTIME_PRIORITY_CLASS\n");
	fprintf(iniFile,"THREADPRIORITY=%-8d;                  Thread-Prorität Decoding und Ausgabe\n",ThreadClassId);
	fprintf(iniFile,";                                               0 = THREAD_PRIORITY_BELOW_NORMAL\n");
	fprintf(iniFile,";                                               1 = THREAD_PRIORITY_NORMAL\n");
	fprintf(iniFile,";                                               2 = THREAD_PRIORITY_ABOVE_NORMAL \n");
	fprintf(iniFile,";                                               3 = THREAD_PRIORITY_HIGHEST\n");
	fprintf(iniFile,";                                               4 = THREAD_PRIORITY_TIME_CRITICAL \n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,";Bei Mehrprozessor-Systemen  koennen die Threads verteilt werden\n");
	fprintf(iniFile,";2 Prozessorsystem vermutlich WINDOWPROZESSOR=0; IRQPROZESSOR=0; VBIPROZESSOR=0; DECODEPROZESSOR=1;\n");
	fprintf(iniFile,";3 Prozessorsystem vermutlich WINDOWPROZESSOR=0; IRQPROZESSOR=0; VBIPROZESSOR=1; DECODEPROZESSOR=2;\n");
	fprintf(iniFile,";>4 Prozessorsystem vermutlich WINDOWPROZESSOR=0; IRQPROZESSOR=1; VBIPROZESSOR=2; DECODEPROZESSOR=3;\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,";WINDOWPROZESSOR                          Message und Menühandling (Wenig Last)\n");
	fprintf(iniFile,"WINDOWPROZESSOR=%d;                       Nummer der CPU (0 bis Anzahl Prozessoren)\n",MainProzessor);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,";IRQPROZESSOR                             Wandelt den IRQ in ein Event (Wenig Last)\n");
	fprintf(iniFile,"IRQPROZESSOR=%d;                          Nummer der CPU (0 bis Anzahl Prozessoren)\n",IrqProzessor);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,";VBIPROZESSOR                             VBI-Decording (Mittlere Last)\n");
	fprintf(iniFile,"VBIPROZESSOR=%d;                          Nummer der CPU (0 bis Anzahl Prozessoren)\n",VBIProzessor);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,";DECODEPROZESSOR                          Decording/Ausgabe (Maximale Last)\n");
	fprintf(iniFile,"DECODEPROZESSOR=%d;                       Nummer der CPU (0 bis Anzahl Prozessoren)\n",AusgabeProzessor);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; SYNC_IRQ                                Ausgabe über Karten-Interrupt syncronisieren\n");
	if ( Sync_Irq == TRUE ) fprintf(iniFile,"SYNC_IRQ=TRUE;\n");
	else fprintf(iniFile,"SYNC_IRQ=FALSE;\n");
	
	fprintf(iniFile,"; DIBMODE                                 Ausgabe über ein DIB \n");
	if ( OutDib == TRUE ) fprintf(iniFile,"DIBMODE=TRUE;\n");
	else fprintf(iniFile,"DIBMODE=FALSE;\n");
	fprintf(iniFile,"; OVERLAYMODE                             Ausgabe über OVERLAY\n");
	if ( ModeOverlay == TRUE ) fprintf(iniFile,"OVERLAYMODE=TRUE;\n");
	else fprintf(iniFile,"OVERLAYMODE=FALSE;\n");
	fprintf(iniFile,"; SURFACEMODE                             Ausgabe über SURFACE\n");
	if (( ModeOverlay == FALSE ) && ( OutDib == FALSE )) fprintf(iniFile,"SURFACEMODE=TRUE;\n");
	else fprintf(iniFile,"SURFACEMODE=FALSE;\n");
	fprintf(iniFile,"; RAWSURFACE                              Ausgabe in die Surface-RAW\n");
	if ( ModeRAWSurface == TRUE ) fprintf(iniFile,"RAWSURFACE=TRUE;\n");
	else fprintf(iniFile,"RAWSURFACE=FALSE;\n");
	fprintf(iniFile,"; RAWSURFACE                              Ausgabe in die Overlay-RAW\n");
	if ( ModeRAWOverlay == TRUE ) fprintf(iniFile,"RAWOVERLAY=TRUE;\n");
	else fprintf(iniFile,"RAWOVERLAY=FALSE;\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; TVSCREEN                                Bildschirm mit Rahmen, Menu, etc\n");
	if ( Show_Menu == TRUE ) fprintf(iniFile,"TVSCREEN=TRUE;\n");
	else fprintf(iniFile,"TVSCREEN=FALSE;\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; TVSCREENNOFRAME                         Bildschirm im TV-Modus ohne Rahmen\n");
	if ( Toggle_WithOut_Frame == TRUE ) fprintf(iniFile,"TVSCREENNOFRAME=TRUE;\n");
	else fprintf(iniFile,"TVSCREENNOFRAME=FALSE;\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; STATUSBAR                               Status-Bar ausgeben\n");
	if ( Display_Status_Bar == TRUE ) fprintf(iniFile,"STATUSBAR=TRUE;\n");
	else fprintf(iniFile,"STATUSBAR=FALSE;\n");
	fprintf(iniFile,"; HALBBILDMODUS                           Es wird nur ein Halbbild ausgegeben\n");
	if ( HalfTVMode == TRUE ) fprintf(iniFile,"HALBBILDMODUS=TRUE;\n");
	else fprintf(iniFile,"HALBBILDMODUS=FALSE;\n");
	fprintf(iniFile,"; COLORMIX                                Farbmischer aktiviert\n");
	if ( ColorMix == TRUE ) fprintf(iniFile,"COLORMIX=TRUE;\n");
	else fprintf(iniFile,"COLORMIX=FALSE;\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; MOVIESCALING=                           Anzahl der Zeilen \n");
	fprintf(iniFile,"MOVIESCALING=%d\n",MovieScale);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; ****************** Programm Listen Setup ****************************\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; LISTSTARTX,LISTSTARTY                   Position des Windows beim Starten (linke obere Ecke)\n");
	fprintf(iniFile,"LISTSTARTX=%d;                            X-Coordinate\n",pgstartx);
	fprintf(iniFile,"LISTSTARTY=%d;                            Y-Coordinate\n",pgstarty);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; LISTSIZEX,LISTSIZEY                     Länge und Höhe des Windows beim Starten \n");
	fprintf(iniFile,"LISTSIZEX=%d;                             X-Länge\n",pgsizex);
	fprintf(iniFile,"LISTSIZEY=%d; Y-Länge\n",pgsizey);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; Einstellung der ListFelder \n");
	for ( i=0; i < 15; i++ ) {
		fprintf(iniFile,"LISTFELD_%d_ID=%d;                        AnzeigeID Feld %d\n",i+1,ButtonList[i].FeldId,i+1);
		fprintf(iniFile,"LISTFELD_%d_LAENGE=%d;                    Laenge Feld %d\n",i+1,ButtonList[i].x,i+1);
	}
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; ************************ Kanal-Liste ***************************\n");
	fprintf(iniFile,"; \n");
	if ( VIEW_ANALOG == TRUE ) {
		fprintf(iniFile,"VIEW_ANALOG=TRUE;                         Analoge Programme anzeigen\n");
	} else {
		fprintf(iniFile,"VIEW_ANALOG=FALSE;                        Analoge Programme nicht anzeigen\n");
	}
	if ( VIEW_DIGITV == TRUE ) {
		fprintf(iniFile,"VIEW_DIGITV=TRUE;                         Digitale TV-Programme anzeigen\n");
	} else {
		fprintf(iniFile,"VIEW_DIGITV=FALSE;                        Digitale TV-Programme nicht anzeigen\n");
	}
	if ( VIEW_DIGIRADIO == TRUE ) {
		fprintf(iniFile,"VIEW_DIGIRADIO=TRUE;                      Digitale Radio-Programme anzeigen\n");
	} else {
		fprintf(iniFile,"VIEW_DIGIRADIO=FALSE;                     Digitale Radio-Programme nicht anzeigen\n");
	}
	if ( VIEW_DIGI_REST == TRUE ) {
		fprintf(iniFile,"VIEW_DIGI_REST=TRUE;                      Digitale Daten-Programme anzeigen\n");
	} else {
		fprintf(iniFile,"VIEW_DIGI_REST=FALSE;                     Digitale Daten-Programme nicht anzeigen\n");
	}
	if ( VIEW_FREE == TRUE ) {
		fprintf(iniFile,"VIEW_FREE=TRUE;                           Nur Freie Programme anzeigen\n");
	} else {
		fprintf(iniFile,"VIEW_FREE=FALSE;                          Auch verschluesselte Programme anzeigen\n");
	}
	if ( VIEW_TEMP_CHANNEL == TRUE ) {
		fprintf(iniFile,"VIEW_TEMP_CHANNEL=TRUE;                   Temoräre Digitale Programme anzeigen (Perspektiven....)\n");
	} else {
		fprintf(iniFile,"VIEW_TEMP_CHANNEL=FALSE;                  Temoräre Digitale nicht Programme anzeigen (Perspektiven....)\n");
	}
	if ( VIEW_LINK_CHANNEL == TRUE ) {
		fprintf(iniFile,"VIEW_LINK_CHANNEL=TRUE;                   Digitale Time-Shift-Client Programme anzeigen (Cdom Einzelkanäle)\n");
	} else {
		fprintf(iniFile,"VIEW_LINK_CHANNEL=FALSE;                  Digitale Time-Shift-Client Programme nicht anzeigen(Cdom Einzelkanäle)\n");
	}
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; ****************** Event Listen Setup ****************************\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; EVENTSTARTX,EVENTSTARTY                 Position des Windows beim Starten (linke obere Ecke)\n");
	fprintf(iniFile,"EVENTSTARTX=%d;                           X-Coordinate\n",evstartx);
	fprintf(iniFile,"EVENTSTARTY=%d;                           Y-Coordinate\n",evstarty);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; EVENTSIZEX,EVENTSIZEY                   Länge und Höhe des Windows beim Starten \n");
	fprintf(iniFile,"EVENTSIZEX=%d;						       X-Länge\n",evsizex);
	fprintf(iniFile,"EVENTSIZEY=%d;							   Y-Länge\n",evsizey);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"EIT_SORT=%d;                              Sortierung Event-Liste\n",EIT_Sort);
	fprintf(iniFile,"EIT_DATUM=%d;							   Datumsauswahl Event-Liste\n",EIT_Datum);
	fprintf(iniFile,"EIT_KANAL=%d;							   KanalAuswal Event-Liste\n",EIT_Kanal);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,";Favoriten-Einstellungen für Event-Liste\n");
	fprintf(iniFile,";FAVORIT= <Nr>  <SID>  <TSID>\n");
	j=0;
	for ( i=0; i < MAXFAVORITEN; i++ ) { 
		if (( Favoriten[i].ts_id != 0) || ( Favoriten[i].SID_Id != 0)) {
			fprintf(iniFile,"FAVORIT=%3d %6d %6d\n",j,Favoriten[i].SID_Id,Favoriten[i].ts_id);
			j++;
		};
	};   
	fprintf(iniFile,"; \n");
#ifdef USE_BT8X8
	fprintf(iniFile,"; ****************** NagraVision Setup ****************************\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; NAGRATESTZEILEN                         Anzahl der Testzeilen für die Nagra-Dekodierung\n");
	fprintf(iniFile,"NAGRATESTZEILEN=%d;\n",MasterTestzeilen);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; NAGRATESTZEILEN                         Testzeilen für die Änderung im Dekoderbetrieb(Die 1. sollte die Kleinste sein\n");
	fprintf(iniFile,"NAGRATESTZEILEN1=%d;\n",TZ[0]);
	fprintf(iniFile,"NAGRATESTZEILEN2=%d;\n",TZ[1]);
	fprintf(iniFile,"NAGRATESTZEILEN3=%d;\n",TZ[2]);
	fprintf(iniFile,"NAGRATESTZEILEN4=%d;\n",TZ[3]);
	fprintf(iniFile,"NAGRATESTZEILEN5=%d;\n",TZ[4]);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; NAGRAFULL                               Beide Halbbilder für die Nagra-Dekodierung\n");
	if ( NagraFullField == TRUE ) fprintf(iniFile,"NAGRAFULL=TRUE;\n");
	else fprintf(iniFile,"NAGRAFULL=FALSE;\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; AUTO-MOVIEBOOST                         Anzahl der Schwarzen Zeilen werden automatisch erkannt\n");
	if ( Auto_MovieBoost == TRUE ) fprintf(iniFile,"AUTO-MOVIEBOOST=TRUE;\n");
	else fprintf(iniFile,"AUTO-MOVIEBOOST=FALSE;\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; SCHWARZ-SCHWELLE                        Schwellwert für die Erkennung der schwarzen Zeilen\n");
	fprintf(iniFile,"SCHWARZ-SCHWELLE=%d;\n",InitialLow);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; Diverse KeyFiles abhandeln\n");
	for ( i=0; i < 4 ; i++ ) {
		fprintf(iniFile,"KEYNAME%d=%s;\n",i,OPKEY[i].Name);
		fprintf(iniFile,"KEYEXTENSION%d=%s;\n",i,OPKEY[i].Extension);
	};
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"AKTIVERKEY=%d;\n",ActiveKey);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; ****************** VideoCrypt Setup ****************************\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; VCCHROMAKEY                             FineShifting über Farbdekodierung\n");
	if ( Chroma_Key == TRUE ) fprintf(iniFile,"VCCHROMAKEY=TRUE;\n");
	else fprintf(iniFile,"VCCHROMAKEY=FALSE;\n");
	fprintf(iniFile,"; \n");
#endif
	fprintf(iniFile,"; ****************** Direct X Setup ****************************\n");
	fprintf(iniFile,"; \n");
	if ( WAIT_DX_VBI == TRUE ) {
		fprintf(iniFile,"WAIT_DX_VBI=TRUE;                         Warten auf Graka vertikalte Austastlücke \n");
	} else {
		fprintf(iniFile,"WAIT_DX_VBI=FALSE;                        Nicht warten auf Graka vertikalte Austastlücke  \n");
	}
	if ( USE_DX_LOCK == TRUE ) {
		fprintf(iniFile,"USE_DX_LOCK=TRUE;                         Direct X Locking verwenden\n");
	} else {
		fprintf(iniFile,"USE_DX_LOCK=FALSE;                        Direct X Locking nicht verwenden\n");
	}
	if ( USE_DX_OV_STRECH == TRUE ) {
		fprintf(iniFile,"USE_OVERLAY_STRECHING=TRUE;               Overlay-Hardware-Streching verwenden\n");
	} else {
		fprintf(iniFile,"USE_OVERLAY_STRECHING=FALSE;              Overlay-Hardware-Streching nicht verwenden\n");
	}
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; ****************** LASTMODE ****************************\n");
	fprintf(iniFile,"; \n");
	if ( Current_Mode == 0 ) {
		fprintf(iniFile,"LASTMODE=ANALOG;                          War in Analog\n");
	} else {
		fprintf(iniFile,"LASTMODE=DIGITAL;                         War in Digital\n");
	}
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; LETZTES_PROGRAMM                        Aktuellese Programm\n");
	fprintf(iniFile,"LETZTES_PROGRAMM=%d;\n",LastRealProg);
	fprintf(iniFile,"; \n");
	if ( SaveSettings == TRUE ) {
		fprintf(iniFile,"SAVESETTING=TRUE;                         BT_Einstellungen für Diagnose in Setting.BT sichern \n");
	} else {
		fprintf(iniFile,"SAVESETTING=FALSE;                        BT_Einstellungen für Diagnose in Setting.BT sichern \n");
	}
	fprintf(iniFile,";                                         und auch die SAA7146 Settings in Setting.SAA\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; NTDRIVERNAME=                           Diese Variable muß bei NT gesetzt werden, wenn die Org. TV-Software installiert ist.\n");
	fprintf(iniFile,";                                         Name des Gerätes der org. Software. ( Bei meiner Hauppage ist es hcw848nt )\n");
	fprintf(iniFile,";                                         Ist abzulesen in der Registry unter HKLM\\SYSTEM\\CurrentControlSet\\Services\\XXXXX \n");
	fprintf(iniFile,";                                         Die Bezeichnung XXXXX ist einzutragen\n");
	fprintf(iniFile,"NTDRIVERNAME=%s; \n",OrgDriverName);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; NTDVBDRIVERNAME=                        Diese Variable muß bei NT gesetzt werden, wenn die Org. TV-Software installiert ist.\n");
	fprintf(iniFile,";                                         Name des Gerätes der org. Software. \n");
	fprintf(iniFile,";                                         Ist abzulesen in der Registry unter HKLM\\SYSTEM\\CurrentControlSet\\Services\\XXXXX \n");
	fprintf(iniFile,";                                         Die Bezeichnung XXXXX ist einzutragen\n");
	fprintf(iniFile,"NTDVBDRIVERNAME=%s; \n",OrgDVBDriverName);
	fprintf(iniFile,"; \n");
	
	fprintf(iniFile,"; ****************** SAA7146 Setup *************************\n");
	fprintf(iniFile,"; \n");
	if ( Has_DVB == TRUE ) {
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; SAVE_DVB_BOOT                           Manchmal kommt es beim Booten der Karte zum Hängenbleiben\n");
		fprintf(iniFile,";                                         des Rechners!! Ist ein HW-Bug der Rev 1.3 !!!!\n");
		fprintf(iniFile,";                                         Mit SAVE_DVB_BOOT=TRUE kann ggf. das vermieden werden ( ist bei mir so )\n");
		if ( SAVE_BOOT == TRUE ) {
			fprintf(iniFile,"SAVE_DVB_BOOT=TRUE;\n");
		} else {
			fprintf(iniFile,"SAVE_DVB_BOOT=FALSE;\n");
		}
		fprintf(iniFile,"; DVB_IRQ_SHARING                         Die DVB-Karte läuft im IRQ-Sharing oder auch nicht\n");
		fprintf(iniFile,";                                         TRUE  : Bootmeldung nicht ausgeben und Treiber informieren\n");
		fprintf(iniFile,";                                         FALSE : So soll es sein\n");
		fprintf(iniFile,";                                         Das wird unabhängig von diesem Parameter bei jedem Start geprüft !!\n");
		fprintf(iniFile,";                                         FALSE zu setzten obwohl Sharing bringt nix ( ausser der Bootmeldung)\n");
		fprintf(iniFile,";                                         TRUE zu setzten obwohl kein Sharing veranlasst den Treiber jeden IRQ (der DVB-Karte) zu beantworten (Alter Mode Version kleiner 7.5)\n");
		if ( DVB_IRQ_SHARING == TRUE ) {
			fprintf(iniFile,"DVB_IRQ_SHARING=TRUE;\n");
		} else {
			fprintf(iniFile,"DVB_IRQ_SHARING=FALSE;\n");
		}
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"DVB-DECODER=%d;                           Chip auf der DVB-Hardware\n",DVBDecoderType);				 
		fprintf(iniFile,";                                             0= VES 1820 ( für DVB-C )\n");
		fprintf(iniFile,";                                             1= VES 1893 ( für DVB-S Rev 1.2 / 1.5)\n");
		fprintf(iniFile,";                                             2= STV 0299 ( für DVB-S Rev 2.1)\n");
		fprintf(iniFile,";                                             3= TDA 8083 ( für DVB-S Rev 1.6)\n");
		fprintf(iniFile,";											\n");
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"DVB-TUNER-TYPE=%d;                            Art des Tuners der DVB-Hardware\n",DVBTunerType);
		fprintf(iniFile,";                                             0= SPXXXX DVB-C \n");
		fprintf(iniFile,";                                             1= SP5659 DVB-S ( mit VES 1893 ) (Rev 1.3/1.5 )\n");
		fprintf(iniFile,";                                             2= TSA5059 Philips DVB-S ( mit STV 0299 ) (Rev 2.1 )\n");
		fprintf(iniFile,";                                             3= SP8060 Sat Grundig DVB-S ( mit TDA 8083 ) (Rev 1.6 )\n");
		fprintf(iniFile,";                                             4= SP5659C DVB-C (Rev 2.1 )\n");
		fprintf(iniFile,";                                             5= Eigene Definition\n");
		fprintf(iniFile,"; \n");
		fprintf(iniFile,";DVB-AUDIO=                               Audio-Chip auf der DVB-Hardware\n");				 
		fprintf(iniFile,";                                             0= TI-Audio-DAC\n");
		fprintf(iniFile,";                                             1= Crystal-Audio-DAC\n");
		fprintf(iniFile,";											\n");
		fprintf(iniFile,"DVB-AUDIO=%d;\n",DVB_AUDIO);				 
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; COLORFORMAT                             Farbformate DVB\n");
		fprintf(iniFile,";                                             2=RGB16\n");
		fprintf(iniFile,";                                             3=RGB15\n");
		fprintf(iniFile,";                                             4=YUV2 (422)\n");
		fprintf(iniFile,";\n");
		fprintf(iniFile,"; VES1820_INVERSION                       Ermöglicht das Schaltung der I/Q-Signalinversion im VES1820-Decoder\n");
		fprintf(iniFile,";                                         Einige Kabelnetze erfordern Einstellungen (Problem nur 2 grüne Lämpchen)\n");
		fprintf(iniFile,";                                         Mögliche Werte\n");
		fprintf(iniFile,";                                            0  : Automatisch \n");
		fprintf(iniFile,";                                            1  : Eingeschaltet\n");
		fprintf(iniFile,";                                            2  : Ausgeschaltet\n");
		fprintf(iniFile,";                                            2  : Ausgeschaltet\n");
		fprintf(iniFile,"VES1820_INVERSION =%d;\n",VES1820_Inversion);
		fprintf(iniFile,";\n");
		fprintf(iniFile,"COLORFORMAT=%d;             \n",ColourFormat);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; USE_TV_16_9                             TV mit 16:9 Umschaltung verwenden\n");
		if ( USE_TV_16_9 == TRUE ) {
			fprintf(iniFile,"USE_TV_16_9=TRUE;\n");
		} else {
			fprintf(iniFile,"USE_TV_16_9=FALSE;\n");
		}
		fprintf(iniFile,"; FASTSWITCH                              Schalter für das Umschaltverhalten\n");
		if ( FastSwitch == TRUE ) {
			fprintf(iniFile,"FASTSWITCH=TRUE;                          Optimiertes Umschalten\n");
		} else {
			fprintf(iniFile,"FASTSWITCH=FALSE;                         Alle Kommandos senden \n");
		}
		fprintf(iniFile,"; EIT_EVENT                               Schalter für Event-Informationen einlesen\n");
		if ( Capture_Event_EIT == TRUE ) {
			fprintf(iniFile,"EIT_EVENT=TRUE;                           Event-Informationen einlesen\n");
		} else {
			fprintf(iniFile,"EIT_EVENT=FALSE;                          Event-Informationen nicht einlesen\n");
		}
		fprintf(iniFile,"; TID_EVENT                               Schalter für DVB-Zeit-Synchronisation einlesen\n");
		if ( Capture_TID == TRUE ) {
			fprintf(iniFile,"TID_EVENT=TRUE;                           DVB-Zeit-Synchronisation einlesen\n");
		} else {
			fprintf(iniFile,"TID_EVENT=FALSE;                          DVB-Zeit-Synchronisation nicht einlesen\n");
		}
		
		fprintf(iniFile,";\n");
		fprintf(iniFile,"; DIGI_TEXT_HARWARE                       Digitales VideoTextsignal über Hardware einlesen\n");
		if ( Capture_Digi_Text == TRUE ) {
			fprintf(iniFile,"DIGI_TEXT_HARWARE=TRUE;                   Digitales VideoTextsignal  einlesen\n");
		} else {
			fprintf(iniFile,"DIGI_TEXT_HARWARE=FALSE;                  Digitales VideoTextsignal  nicht einlesen\n");
		}
		fprintf(iniFile,";\n");
		fprintf(iniFile,"; STREAM_USE_VT                           VideoText über den Stream und Softdekoder auswerten\n");
		if ( DVB_VT == TRUE ) {
			fprintf(iniFile,"STREAM_USE_VT=TRUE;                       VideoTextdaten aus Stream popeln\n");
		} else {
			fprintf(iniFile,"STREAM_USE_VT=FALSE;                      VideoTextdaten nicht aus Stream popeln\n");
		}
		fprintf(iniFile,";\n");
		fprintf(iniFile,"; STREAM_USE_WINBIS                       WinBis Daten über den Stream und Softdekoder auswerten\n");
		if ( DVB_WINBIS == TRUE ) {
			fprintf(iniFile,"STREAM_USE_WINBIS=TRUE;                   WinBis - Daten aus Stream popeln\n");
		} else {
			fprintf(iniFile,"STREAM_USE_WINBIS=FALSE;                  WinBis - Daten nicht aus Stream popeln\n");
		}

		fprintf(iniFile,";\n");
		fprintf(iniFile,"; FAST_BOOT                               Überprüft beim Starten die DVB-Karte auf ihren Zustand\n");
		fprintf(iniFile,";                                         und überspringt ggf. die Initialisierungssequenz\n");
		fprintf(iniFile,";                                         Mögliche Werte\n");
		fprintf(iniFile,";                                            TRUE  : Fastboot eingeschaltet\n");
		fprintf(iniFile,";                                            FALSE : Fastboot ausgeschaltet (immer vollständige DVB-Initialisierung)\n");
		if ( FastBoot == TRUE ) {
			fprintf(iniFile,"FAST_BOOT=TRUE\n");
		} else {
			fprintf(iniFile,"FAST_BOOT=FALSE\n");
			
		};
		fprintf(iniFile,";\n");
		fprintf(iniFile,"; FIRM_DIR                                Directory in der die zu benutzender Firmware der DVB-Karte steht\n");
		fprintf(iniFile,";                                         Dieses Verzeichniss muß die Dateien dpram, Boot_up.axf und root enthalten\n");
		fprintf(iniFile,"FIRM_DIR=%s;\n",FIRM_DIR);
		fprintf(iniFile,";\n");
		fprintf(iniFile,"; FIRMVERSION                             Wird für den Vergleich mit FAST_BOOT benötigt\n");
		fprintf(iniFile,";                                         Die zuletzt von Multidec benutzte Firmware-Version\n");
		fprintf(iniFile,"FIRMVERSION=%s;\n",FirmID);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; ************************ DVB-Timing ***************************\n");
		fprintf(iniFile,"; *                                                             *\n");
		fprintf(iniFile,"; *         Diese Funktionen sind nur in der INI zu ändern.     *\n");
		fprintf(iniFile,"; *                                                             *\n");
		fprintf(iniFile,"; * Die Timer können die Umschaltzeiten etc. verkürzen, aber    *\n");
		fprintf(iniFile,"; * auch den ARM - Prozessor der DVB-Karte aufhängen.           *\n");
		fprintf(iniFile,"; * Wer mit diesen Werten rumspielt, sollte mit dem Schalter    *\n");
		fprintf(iniFile,"; * WRITE_LOG eine Protokoll-Datei erzeugen, um die Ergebnisse  *\n");
		fprintf(iniFile,"; * zu uberprüfen bzw. die Funktionalität beobachten!           *\n");
		fprintf(iniFile,"; *                                                             *\n");
		fprintf(iniFile,"; ***************************************************************\n");
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMING-WriteRegDelay                    Nach jedem Senden über den I2C-Bus wird XX Millisekunden gewartet.\n");
		fprintf(iniFile,";                                         Hat für die Geschwindigkeit / den Zugriff auf den I2C-Bus immense \n");
		fprintf(iniFile,";                                         Bedeutung\n");
		fprintf(iniFile,";                                         Default für DVB-S = 0 \n");
		fprintf(iniFile,";                                         Default für DVB-C = 2 \n");
		fprintf(iniFile,"TIMING-WriteRegDelay=%d;\n",Timing.WriteRegDelay);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMING-I2C_Bus_Timeout                  Nach jedem Senden über den I2C-Bus wird maximal XX Millisekunden gewartet,\n");
		fprintf(iniFile,";                                         bis das Kommando bestätigt wird. Wird es innerhalt dieser Zeit nicht quittiert,\n");
		fprintf(iniFile,";                                         wird der Befehl wiederholt.\n");
		fprintf(iniFile,";                                         Fehlerhafte Werte erzeugen in der Log-Datei Meldungen mit : \n");
		fprintf(iniFile,";                                                     SAA7146 Master_Xfer  XXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		fprintf(iniFile,";                                         ( Die Wiederholung des Befehles dauert länger als das was man einspart !!\n");
		fprintf(iniFile,";                                         Wird das vorher quittiert ist der Sleep auch vorher beendet !!\n");
		fprintf(iniFile,";                                         Sollte bei häufigen SAA7146 Master_Xfer Fehlern hochgesetzt werden\n");
		fprintf(iniFile,";                                         Default 200 ms (maximale Wartezeit) \n"); 
		fprintf(iniFile,"TIMING-I2C_Bus_Timeout=%d;\n",Timing.I2C_Bus_Timeout);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMING-I2C_Bus_Reset_Delay              Reset-Verzögerung bei I2C-Bus-Fehler\n");
		fprintf(iniFile,";                                         Eigentlich soll der I2C-Bus-Fehler vermieden werden ( Bis auf bei der \n");
		fprintf(iniFile,";                                         automatischen Hardwareerkennung)\n"); 
		fprintf(iniFile,";                                         Zum Resetten braucht die Karte etwas Zeit \n");
		fprintf(iniFile,";                                         Default 10 ms \n");
		fprintf(iniFile,"TIMING-I2C_Bus_Reset_Delay=%d;\n",Timing.I2C_Bus_Reset_Delay);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMING-DVB_Reset_Wait                   Nach dem Umschalten bzw beim Scannen kann es abhängig von der Karte \n");
		fprintf(iniFile,";                                         bzw SAT-Anlage oder Kabel etwas dauern bis der LOCK anspringt.\n");
		fprintf(iniFile,";                                         Dieser Parameter hat Einfluss auf die Scan bzw. Umschaltzeit. \n");
		fprintf(iniFile,";                                         Werden beim Scannen nicht alle Transponder / Frequenzen erkannt, sollte der \n");
		fprintf(iniFile,";                                         Wert mal etwas Hochgesetzt werden. Bei meiner Karte/Anlage fahre ich mit 0 !!\n"); 
		fprintf(iniFile,";                                         Default 5 ms ???\n");
		fprintf(iniFile,"TIMING-DVB_Reset_Wait=%d;\n",Timing.DVB_Reset_Wait);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMING-DVB_Reset_Scan_Wait              Der DVB_Reset_Wait wird beim Umschalten mit angezogen. Da beim Scannen wohl\n");
		fprintf(iniFile,";                                         größere Werte notwendig sind ( DVB-c ?? ) habe ich diesen Parameter nun geteilt.\n");
		fprintf(iniFile,";                                            TIMING-DVB_Reset_Wait gilt beim Umschalten.\n");
		fprintf(iniFile,";                                            TIMING-DVB_Reset_Scan_Wait gilt beim Scannen.\n");
		fprintf(iniFile,";\n");                                         
		fprintf(iniFile,";                                         Beschreibung siehe DVB_Reset_Wait !!\n");
		fprintf(iniFile,";                                         Default 15 ms ???\n");
		fprintf(iniFile,"TIMING-DVB_Reset_Scan_Wait=%d;\n",Timing.DVB_Reset_Scan_Wait);
		fprintf(iniFile,";\n");
		fprintf(iniFile,"; TIMING-SendDiSEqCDelay                  Nach dem Umschalten bzw beim Scannen kann es abhängig von der Karte \n");
		fprintf(iniFile,";                                         bzw SAT-Anlage notwendig sein, zwischen den DiSEq-Kommandos Pausen einzulegen\n");
		fprintf(iniFile,";                                         Dieser Parameter hat Einfluss auf die Scan bzw. Umschaltzeit. \n");
		fprintf(iniFile,";                                         Werden beim Scannen /Umschalten von einer LNB auf die nächste hat, sollte mal an\n");
		fprintf(iniFile,";                                         diesem Parameter schrauben. <Ich habe leider nur ASTRA>\n");
		fprintf(iniFile,";                                         Default 100 ms ???\n");
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"TIMING-SendDiSEqCDelay=%d;\n",Timing.SendDiSEqCDelay);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMING-Pmt_TimeOut                      Maximale Wartezeit bis behauptet wird ein Transponder / eine Frequenz hat keinen PMT \n");
		fprintf(iniFile,";                                         Dieser Parameter hat Einfluss auf die Scan-Zeit \n");
		fprintf(iniFile,";                                         Default 2500 ms \n");
		fprintf(iniFile,"TIMING-Pmt_TimeOut=%d;\n",Timing.Pmt_TimeOut);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMING-Sdt_TimeOut                      Maximale Wartezeit bis behauptet wird ein Transponder / eine Frequenz hat keinen SDT\n");
		fprintf(iniFile,";                                         Dieser Parameter hat Einfluss auf die Scan-Zeit \n");
		fprintf(iniFile,";                                         Default 2500 ms \n");
		fprintf(iniFile,"TIMING-Sdt_TimeOut=%d;\n",Timing.Sdt_TimeOut);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMING-Pat_TimeOut                      Maximale Wartezeit bis behauptet wird ein Transponder / eine Frequenz hat keinen PAT\n");
		fprintf(iniFile,";                                         Dieser Parameter hat Einfluss auf die Scan-Zeit \n");
		fprintf(iniFile,";                                         Default 2500 ms \n");
		fprintf(iniFile,"TIMING-Pat_TimeOut=%d;\n",Timing.Pat_TimeOut);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMING-Cat_TimeOut                      Maximale Wartezeit bis behauptet wird ein Transponder / eine Frequenz hat keinen CAT\n");
		fprintf(iniFile,";                                         Dieser Parameter hat Einfluss auf die Scan-Zeit \n");
		fprintf(iniFile,";                                         Default 2500 ms \n");
		fprintf(iniFile,"TIMING-Cat_TimeOut=%d;\n",Timing.Cat_TimeOut);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMING-Nit_TimeOut                      Maximale Wartezeit bis behauptet wird ein Transponder / eine Frequenz hat keinen NIT\n");
		fprintf(iniFile,";                                         Dieser Parameter hat Einfluss auf die Scan-Zeit \n");
		fprintf(iniFile,";                                         Default 2500 ms \n");
		fprintf(iniFile,"TIMING-Nit_TimeOut=%d;\n",Timing.Nit_TimeOut);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMING-Debi_Cmd_Time_Out                KERNEL-PARAMETER (Liegt im Treiber MDSAA)\n");
		fprintf(iniFile,";                                         Maximale Wartezeit bis ein Debi-Kommando ausgeführt werden sollte\n");
		fprintf(iniFile,";                                         Innerhalb dieser Warteschleife wird kein weiterer IRQ (der DVB-Karte ) ausgeführt\n");
		fprintf(iniFile,";                                         Wird das Kommando ausgeführt, ist die Schleife vorher beendet und alles wird freigegeben.\n");
		fprintf(iniFile,";                                         Ist die Wartezeit zu klein, läuft der ARM-Prozessor in der Befehlsausführung weiter, \n");
		fprintf(iniFile,";                                         aber das nächste Kommando oder ein IRQ kann u.U. den ARM zerbröseln. ( War eines meiner Probleme )\n");
		fprintf(iniFile,";                                         Der richtige Wert hängt vom Rechner(auslastung)/Empfangsvolumen(Bytes/s)/DVB-Karte ab.\n");
		fprintf(iniFile,";                                         Hinweis in der Log_Datei, das diese Warteschleife mal zu klein war :\n");
		fprintf(iniFile,";                                                 SAA7146 Sync_KP_Call Debi_Command nicht innerhalb vom TimeOut ausgeführt\n");
		fprintf(iniFile,";                                         Sollte besser nicht zu oft vorkommen\n");
		fprintf(iniFile,";                                         Default 15000 (Schleife im Treiber)\n");
		fprintf(iniFile,"TIMING-Debi_Cmd_Time_Out=%d;\n",Timing.Debi_Cmd_Time_Out);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMING-OSD_Text_Time_Out                KERNEL-PARAMETER (Liegt im Treiber MDSAA)\n");
		fprintf(iniFile,";                                         Maximale Wartezeit bis ein Debi-Kommando mit Ausgabe von OSD-Text ausgeführt werden sollte!\n");
		fprintf(iniFile,";                                         Innerhalb dieser Warteschleife wird kein weiterer IRQ (der DVB-Karte ) ausgeführt\n");
		fprintf(iniFile,";                                         Wird das Kommando ausgeführt, ist die Schleife vorher beendet und alles wird freigegeben.\n");
		fprintf(iniFile,";                                         Ist die Wartezeit zu klein, läuft der ARM-Prozessor in der Befehlsausführung weiter, \n");
		fprintf(iniFile,";                                         aber das nächste Kommando oder ein IRQ kann u.U. den ARM zerbröseln. ( War eines meiner Probleme )\n");
		fprintf(iniFile,";                                         Der richtige Wert hängt vom Rechner(auslastung)/Empfangsvolumen(Bytes/s)/DVB-Karte ab.\n");
		fprintf(iniFile,";                                         Hinweis in der Log_Datei, das diese Warteschleife mal zu klein war :\n");
		fprintf(iniFile,";                                                 SAA7146 Sync_KP_Call TextPuffer nicht innerhalb vom TimeOut geleehrt \n");
		fprintf(iniFile,";                                                 SAA7146 Sync_KP_Call Debi_Command und TextPuffer nicht innerhalb vom TimeOut geleehrt\n");
		fprintf(iniFile,";                                         Sollte besser nicht zu oft vorkommen\n");
		fprintf(iniFile,";                                         Default 60000 (Schleife im Treiber)\n");
		fprintf(iniFile,"TIMING-OSD_Text_Time_Out=%d;\n",Timing.OSD_Text_Time_Out);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMING-Debi_done_MC2                    KERNEL-PARAMETER (Liegt im Treiber MDSAA)\n");
		fprintf(iniFile,";                                         Maximale Wartezeit bis ein Debi-Datentransfer eingeleitet wird !!\n");
		fprintf(iniFile,";                                         Innerhalb dieser Warteschleife wird kein weiterer IRQ (der DVB-Karte ) ausgeführt\n");
		fprintf(iniFile,";                                         Wird der Transfer vom ARM eingeleutet. ist die Schleife vorher beendet und alles wird freigegeben.\n");
		fprintf(iniFile,";                                         Ist die Wartezeit zu klein, läuft der ARM-Prozessor in der Befehlsausführung weiter, \n");
		fprintf(iniFile,";                                         aber das nächste Kommando oder ein IRQ kann u.U. den ARM zerbröseln. ( War eines meiner Probleme )\n");
		fprintf(iniFile,";                                         Der richtige Wert hängt vom Rechner(auslastung)/Empfangsvolumen(Bytes/s)/DVB-Karte ab.\n");
		fprintf(iniFile,";                                         Hinweis in der Log_Datei, das diese Warteschleife mal zu klein war :\n");
		fprintf(iniFile,";                                                 SAA7146 Debi_done_MC2 TimeOut \n");
		fprintf(iniFile,";                                         Sollte besser nicht zu oft vorkommen\n");
		fprintf(iniFile,";                                         Default 2000 (Schleife im Treiber)\n");
		fprintf(iniFile,"TIMING-Debi_done_MC2=%d;\n",Timing.Debi_done_MC2);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMING-Debi_done_PSR                    KERNEL-PARAMETER (Liegt im Treiber MDSAA)\n");
		fprintf(iniFile,";                                         Maximale Wartezeit bis ein Debi-Datentransfer eingeleitet wird !!\n");
		fprintf(iniFile,";                                         Innerhalb dieser Warteschleife wird kein weiterer IRQ (der DVB-Karte ) ausgeführt\n");
		fprintf(iniFile,";                                         Wird der Transfer vom ARM eingeleutet. ist die Schleife vorher beendet und alles wird freigegeben.\n");
		fprintf(iniFile,";                                         Ist die Wartezeit zu klein, läuft der ARM-Prozessor in der Befehlsausführung weiter, \n");
		fprintf(iniFile,";                                         aber das nächste Kommando oder ein IRQ kann u.U. den ARM zerbröseln. ( War eines meiner Probleme )\n");
		fprintf(iniFile,";                                         Der richtige Wert hängt vom Rechner(auslastung)/Empfangsvolumen(Bytes/s)/DVB-Karte ab.\n");
		fprintf(iniFile,";                                         Hinweis in der Log_Datei, das diese Warteschleife mal zu klein war :\n");
		fprintf(iniFile,";                                                 SAA7146 Debi_done_MC2 TimeOut \n");
		fprintf(iniFile,";                                         Sollte besser nicht zu oft vorkommen\n");
		fprintf(iniFile,";                                         Default 10000 (Schleife im Treiber)\n");
		fprintf(iniFile,"TIMING-Debi_done_PSR=%d;\n",Timing.Debi_done_PSR);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; ************************ OSD-Farben ***************************\n");
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; OSD_HINTERGRUND1=Rot Grün  Blau  Blend   Farbeinstellungen Hintergrund 1\n");
		fprintf(iniFile,"OSD_HINTERGRUND1=%3d %3d %3d %3d;\n",OSD_Colors[0].R,OSD_Colors[0].G,OSD_Colors[0].B,OSD_Colors[0].Blend);
		fprintf(iniFile,"; OSD_HINTERGRUND2=Rot Grün  Blau  Blend   Farbeinstellungen Hintergrund 2\n");
		fprintf(iniFile,"OSD_HINTERGRUND2=%3d %3d %3d %3d;\n",OSD_Colors[1].R,OSD_Colors[1].G,OSD_Colors[1].B,OSD_Colors[1].Blend);
		fprintf(iniFile,"; OSD_FARBE1=Rot Grün  Blau  Blend   Farbeinstellungen TextFarbe 1\n");
		fprintf(iniFile,"OSD_FARBE1=%3d %3d %3d %3d;\n",OSD_Colors[2].R,OSD_Colors[2].G,OSD_Colors[2].B,OSD_Colors[2].Blend);
		fprintf(iniFile,"; OSD_FARBE2=Rot Grün  Blau  Blend   Farbeinstellungen TextFarbe 2\n");
		fprintf(iniFile,"OSD_FARBE2=%3d %3d %3d %3d;\n",OSD_Colors[3].R,OSD_Colors[3].G,OSD_Colors[3].B,OSD_Colors[3].Blend);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; ************************ TimeZone ***************************\n");
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"TIMEZONE=%d;             \n",TIMEZONE);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; ************************ Recorder Setup **********************\n");
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; RECORD_FORMAT=                          Format der Aufnahme\n");
		fprintf(iniFile,";                                               0 = Audio\n");
		fprintf(iniFile,";                                               1 = Video\n");
		fprintf(iniFile,";                                               2 = Audio+Video\n");
		fprintf(iniFile,"RECORD_FORMAT=%d;\n",RecorderFormat);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; RECORD_TYP=                             Datei-Tyo der Aufnahme\n");
		fprintf(iniFile,";                                               0 = mpg (mpg2)\n");
		fprintf(iniFile,";                                               1 = PAV\n");
		fprintf(iniFile,"RECORD_TYP=%d;\n",RecorderTyp);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; RECORD_MAX_FILE_SIZE=                   Maximale Dateigröße/pro Datei für DVB-Aufnahme\n");
		fprintf(iniFile,";                                         Ist diese Größe erreicht wird automatisch eine neu Datei erzeugt\n");
		fprintf(iniFile,";                                         0 bedeutet : Keine Begrenzung\n");
		fprintf(iniFile,"RECORD_MAX_FILE_SIZE=%d;\n",MAXMegaBytes);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; RECORDERBASEDIR1=                       Pfad 1 für Aufnahme\n");
		fprintf(iniFile,"RECORDERBASEDIR1=%s;\n",RecordBaseDir[0]);
		fprintf(iniFile,"; RECORDERBASEDIR2=                       Pfad 2 für Aufnahme\n");
		fprintf(iniFile,"RECORDERBASEDIR2=%s;\n",RecordBaseDir[1]);
		fprintf(iniFile,"; RECORDERBASEDIR3=                       Pfad 3 für Aufnahme\n");
		fprintf(iniFile,"RECORDERBASEDIR3=%s;\n",RecordBaseDir[2]);
		fprintf(iniFile,"; RECORDERBASEDIR4=                       Pfad 4 für Aufnahme\n");
		fprintf(iniFile,"RECORDERBASEDIR4=%s;\n",RecordBaseDir[3]);
		fprintf(iniFile,"; RECORDERBASEDIR5=                       Pfad 5 für Aufnahme\n");
		fprintf(iniFile,"RECORDERBASEDIR5=%s;\n",RecordBaseDir[4]);
		fprintf(iniFile,"; RECORDERBASEDIR6=                       Pfad 6 für Aufnahme\n");
		fprintf(iniFile,"RECORDERBASEDIR6=%s;\n",RecordBaseDir[5]);
		fprintf(iniFile,"; RECORDERBASEDIR7=                       Pfad 7 für Aufnahme\n");
		fprintf(iniFile,"RECORDERBASEDIR7=%s;\n",RecordBaseDir[6]);
		fprintf(iniFile,"; RECORDERBASEDIR8=                       Pfad 8 für Aufnahme\n");
		fprintf(iniFile,"RECORDERBASEDIR8=%s;\n",RecordBaseDir[7]);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; ************************ Recorder ***************************\n");
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; TIMERREC=<SpeicherPlatz> \n");
		fprintf(iniFile,";    	   	       <Start_Zeit_Stunde>\n");
		fprintf(iniFile,";    	   	             <Start_Zeit_Minuten>\n");
		fprintf(iniFile,";    	   					   <Ende_Zeit_Stunde>\n");
		fprintf(iniFile,";    	   	                        <Ende_Zeit_Minuten>\n");
		fprintf(iniFile,";    	   	                            <Datums_Id>\n");
		fprintf(iniFile,";    	   	                                 <ProgrammNummer>\n");
		fprintf(iniFile,";    	   	                                     <AufnahmeFormat>\n");
		fprintf(iniFile,";    	   	                                         <ShutdownFlag>\n");
		
		j=1;
		for ( i=0; i < TIMERANZAHL; i++ ) {
			if (( TimerRec[i].StartZeit_h!=0xff) && ( TimerRec[i].EndZeit_h !=0xff)) {
				fprintf(iniFile,"TIMERREC=%02d %02d %02d %02d %02d %6d %6d %1d %1d\n",j,TimerRec[i].StartZeit_h,TimerRec[i].StartZeit_m,TimerRec[i].EndZeit_h,TimerRec[i].EndZeit_m,TimerRec[i].Date_Id,TimerRec[i].Programm,TimerRec[i].RecorderArt,TimerRec[i].Shutdown);
				j++;      
			};
		};
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; ************* zu berücksichtigende Symbolraten **************\n");
		fprintf(iniFile,"; \n");
		for (i=0; i<16; i++) {
			fprintf(iniFile,"SYMBOLRATE%d=%d;             \n",i,SRates[i]);
		}
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; ************************** LNB´s ****************************\n");
		fprintf(iniFile,"; \n");
		fprintf(iniFile,";                                             Einstellungen LNB 1\n");
		fprintf(iniFile,"LNB1_MIN_FREQ=%d;                             LNB 1 Minimale Frequenz\n",LNB[0].MinFreq);
		fprintf(iniFile,"LNB1_MAX_FREQ=%d;                             LNB 1 Maximale Frequenz\n",LNB[0].MaxFreq);
		fprintf(iniFile,"LNB1_LOF_LOW=%d;                              LNB 1 Lof LowBand\n",LNB[0].LofLow);
		fprintf(iniFile,"LNB1_LOF_HIGH=%d;                             LNB 1 Lof HighBand \n",LNB[0].LofHigh);
		fprintf(iniFile,"LNB1_SWITCHFREQ=%d;                           LNB 1 LowBand / HighBand Switchfrequens\n",LNB[0].SwitchFreq);
		if ( LNB[0].Power == TRUE ) {
			fprintf(iniFile,"LNB1_POWER=TRUE;                              LNB 1 Spannungsversorgung AN\n");
		} else {
			fprintf(iniFile,"LNB1_POWER=FALSE;                             LNB 1 Spannungsversorgung AUS\n");
		}
		if ( LNB[0].Switch22khz == TRUE ) {
			fprintf(iniFile,"LNB1_SWITCH_22KHZ=TRUE;                       LNB 1 Umschalten Band mit 22 KHZ AN\n");
		} else {
			fprintf(iniFile,"LNB1_SWITCH_22KHZ=FALSE;                      LNB 1 Umschalten Band mit 22 KHZ AUS\n");
		}
		if ( LNB[0].PowerReset == TRUE ) {
			fprintf(iniFile,"LNB1_POWERRESET=TRUE;                         LNB 1 SpannungsReset bei DiseqC-Kommando An\n");
		} else {
			fprintf(iniFile,"LNB1_POWERRESET=FALSE;                        LNB 1 SpannungsReset bei DiseqC-Kommando Aus\n");
		}
		fprintf(iniFile,";LNB1_MINIDISEQC=                             LNB 1 Mini-DiSEq ( Nur Toneburst ) verwenden \n");
		if ( LNB[0].MiniDiseqc == 0x01 ) {
			fprintf(iniFile,"LNB1_MINIDISEQC=TRUE;                         LNB 1 Mini-DiSEq An\n");
		} else {
			fprintf(iniFile,"LNB1_MINIDISEQC=FALSE;                        LNB 1 Mini-DiSEq Aus\n");
		}
		fprintf(iniFile,"LNB1_TONEBURST=%d;                           LNB 1 Burstzustand\n",LNB[0].BurstVal);
		fprintf(iniFile,";                                                  -1 = Nicht verwenden \n");
		fprintf(iniFile,";                                                   0 = Aus\n");
		fprintf(iniFile,";                                                   1 = An\n");
		fprintf(iniFile,"LNB1_POS=%c;                                   LNB 1 POS-Schalter\n",LNB[0].PosVal);
		fprintf(iniFile,";                                                   A = Position A\n");
		fprintf(iniFile,";                                                   B = Position B\n");
		fprintf(iniFile,"LNB1_OPT=%c;                                   LNB 1 OPT-Schalter\n",LNB[0].OptVal);
		fprintf(iniFile,";                                                   A = Option A\n");
		fprintf(iniFile,";                                                   B = Option B\n");
		fprintf(iniFile,"LNB1_ORBIT=%d;                                 LNB 1 Orbit position (0,1°)\n",LNB[0].orbit);
		fprintf(iniFile,"LNB1_WEST_EAST=%c;                             LNB 1 West (W) East (E) flag\n",LNB[0].west_east);
		
		fprintf(iniFile,"; \n");
		fprintf(iniFile,";                                             Einstellungen LNB 2\n");
		if ( LNB[1].Use == TRUE ) {
			fprintf(iniFile,"LNB2_USE=TRUE;                                LNB 2 verwenden\n");
			fprintf(iniFile,"LNB2_MIN_FREQ=%d;                             LNB 2 Minimale Frequenz\n",LNB[1].MinFreq);
			fprintf(iniFile,"LNB2_MAX_FREQ=%d;                             LNB 2 Maximale Frequenz\n",LNB[1].MaxFreq);
			fprintf(iniFile,"LNB2_LOF_LOW=%d;                              LNB 2 Lof LowBand\n",LNB[1].LofLow);
			fprintf(iniFile,"LNB2_LOF_HIGH=%d;                             LNB 2 Lof HighBand \n",LNB[1].LofHigh);
			fprintf(iniFile,"LNB2_SWITCHFREQ=%d;                           LNB 2 LowBand / HighBand Switchfrequens\n",LNB[1].SwitchFreq);
			if ( LNB[1].Power == TRUE ) {
				fprintf(iniFile,"LNB2_POWER=TRUE;                              LNB 2 Spannungsversorgung AN\n");
			} else {
				fprintf(iniFile,"LNB2_POWER=FALSE;                             LNB 2 Spannungsversorgung AUS\n");
			}
			
			if ( LNB[1].Switch22khz == TRUE ) {
				fprintf(iniFile,"LNB2_SWITCH_22KHZ=TRUE;                       LNB 2 Umschalten Band mit 22 KHZ AN\n");
			} else {
				fprintf(iniFile,"LNB2_SWITCH_22KHZ=FALSE;                      LNB 2 Umschalten Band mit 22 KHZ AUS\n");
			}
			if ( LNB[1].PowerReset == TRUE ) {
				fprintf(iniFile,"LNB2_POWERRESET=TRUE;                         LNB 2 SpannungsReset bei DiseqC-Kommando An\n");
			} else {
				fprintf(iniFile,"LNB2_POWERRESET=FALSE;                        LNB 2 SpannungsReset bei DiseqC-Kommando Aus\n");
			}
			fprintf(iniFile,";LNB2_MINIDISEQC=                             LNB 2 Mini-DiSEq ( Nur Toneburst ) verwenden \n");
			if ( LNB[1].MiniDiseqc == 0x01 ) {
				fprintf(iniFile,"LNB2_MINIDISEQC=TRUE;                         LNB 2 Mini-DiSEq An\n");
			} else {
				fprintf(iniFile,"LNB2_MINIDISEQC=FALSE;                        LNB 2 Mini-DiSEq Aus\n");
			}
			
			fprintf(iniFile,"LNB2_TONEBURST=%d;                            LNB 2 Burstzustand\n",LNB[1].BurstVal);
			fprintf(iniFile,";                                                  -1 = Nicht verwenden \n");
			fprintf(iniFile,";                                                   0 = Aus\n");
			fprintf(iniFile,";                                                   1 = An\n");
			fprintf(iniFile,"LNB2_POS=%c;                                   LNB 2 POS-Schalter\n",LNB[1].PosVal);
			fprintf(iniFile,";                                                   A = Position A\n");
			fprintf(iniFile,";                                                   B = Position B\n");
			fprintf(iniFile,"LNB2_OPT=%c;                                   LNB 2 OPT-Schalter\n",LNB[1].OptVal);
			fprintf(iniFile,";                                                   A = Option A\n");
			fprintf(iniFile,";                                                   B = Option B\n");
	    	fprintf(iniFile,"LNB2_ORBIT=%d;                                 LNB 2 Orbit position (0,1°)\n",LNB[1].orbit);
    		fprintf(iniFile,"LNB2_WEST_EAST=%c;                             LNB 2 West (W) East (E) flag\n",LNB[1].west_east);
		} else {
			fprintf(iniFile,"LNB2_USE=FALSE                                LNB 2 nicht verwendet\n");
		}
		fprintf(iniFile,"; \n");
		
		fprintf(iniFile,";                                             Einstellungen LNB 3\n");
		if ( LNB[2].Use == TRUE ) {
			fprintf(iniFile,"LNB3_USE=TRUE;                                LNB 3 verwenden\n");
			fprintf(iniFile,"LNB3_MIN_FREQ=%d;                             LNB 3 Minimale Frequenz\n",LNB[2].MinFreq);
			fprintf(iniFile,"LNB3_MAX_FREQ=%d;                             LNB 3 Maximale Frequenz\n",LNB[2].MaxFreq);
			fprintf(iniFile,"LNB3_LOF_LOW=%d;                              LNB 3 Lof LowBand\n",LNB[2].LofLow);
			fprintf(iniFile,"LNB3_LOF_HIGH=%d;                             LNB 3 Lof HighBand \n",LNB[2].LofHigh);
			fprintf(iniFile,"LNB3_SWITCHFREQ=%d;                           LNB 3 LowBand / HighBand Switchfrequens\n",LNB[2].SwitchFreq);
			if ( LNB[2].Power == TRUE ) {
				fprintf(iniFile,"LNB3_POWER=TRUE;                              LNB 3 Spannungsversorgung AN\n");
			} else {
				fprintf(iniFile,"LNB3_POWER=FALSE;                             LNB 3 Spannungsversorgung AUS\n");
			}
			if ( LNB[2].Switch22khz == TRUE ) {
				fprintf(iniFile,"LNB3_SWITCH_22KHZ=TRUE;                       LNB 3 Umschalten Band mit 22 KHZ AN\n");
			} else {
				fprintf(iniFile,"LNB3_SWITCH_22KHZ=FALSE;                      LNB 3 Umschalten Band mit 22 KHZ AUS\n");
			}
			if ( LNB[2].PowerReset == TRUE ) {
				fprintf(iniFile,"LNB3_POWERRESET=TRUE;                         LNB 3 SpannungsReset bei DiseqC-Kommando An\n");
			} else {
				fprintf(iniFile,"LNB3_POWERRESET=FALSE;                        LNB 3 SpannungsReset bei DiseqC-Kommando Aus\n");
			}
			fprintf(iniFile,";LNB3_MINIDISEQC=                             LNB 3 Mini-DiSEq ( Nur Toneburst ) verwenden \n");
			if ( LNB[2].MiniDiseqc == 0x01 ) {
				fprintf(iniFile,"LNB3_MINIDISEQC=TRUE;                         LNB 3 Mini-DiSEq An\n");
			} else {
				fprintf(iniFile,"LNB3_MINIDISEQC=FALSE;                        LNB 3 Mini-DiSEq Aus\n");
			}
			fprintf(iniFile,"LNB3_TONEBURST=%d;                           LNB 3 Burstzustand\n",LNB[2].BurstVal);
			fprintf(iniFile,";                                                  -1 = Nicht verwenden \n");
			fprintf(iniFile,";                                                   0 = Aus\n");
			fprintf(iniFile,";                                                   1 = An\n");
			fprintf(iniFile,"LNB3_POS=%c;                                   LNB 3 POS-Schalter\n",LNB[2].PosVal);
			fprintf(iniFile,";                                                   A = Position A\n");
			fprintf(iniFile,";                                                   B = Position B\n");
			fprintf(iniFile,"LNB3_OPT=%c;                                   LNB 3 OPT-Schalter\n",LNB[2].OptVal);
			fprintf(iniFile,";                                                   A = Option A\n");
			fprintf(iniFile,";                                                   B = Option B\n");
	    	fprintf(iniFile,"LNB3_ORBIT=%d;                                 LNB 3 Orbit position (0,1°)\n",LNB[2].orbit);
    		fprintf(iniFile,"LNB3_WEST_EAST=%c;                             LNB 3 West (W) East (E) flag\n",LNB[2].west_east);
		} else {
			fprintf(iniFile,"LNB3_USE=FALSE                                LNB 3 nicht verwendet\n");
		}
		fprintf(iniFile,"; \n");
		fprintf(iniFile,";                                             Einstellungen LNB 4\n");
		if ( LNB[3].Use == TRUE ) {
			fprintf(iniFile,"LNB4_USE=TRUE;                                LNB 4 verwenden\n");
			fprintf(iniFile,"LNB4_MIN_FREQ=%d;                             LNB 4 Minimale Frequenz\n",LNB[3].MinFreq);
			fprintf(iniFile,"LNB4_MAX_FREQ=%d;                             LNB 4 Maximale Frequenz\n",LNB[3].MaxFreq);
			fprintf(iniFile,"LNB4_LOF_LOW=%d;                              LNB 4 Lof LowBand\n",LNB[3].LofLow);
			fprintf(iniFile,"LNB4_LOF_HIGH=%d;                             LNB 4 Lof HighBand \n",LNB[3].LofHigh);
			fprintf(iniFile,"LNB4_SWITCHFREQ=%d;                           LNB 4 LowBand / HighBand Switchfrequens\n",LNB[3].SwitchFreq);
			if ( LNB[3].Power == TRUE ) {
				fprintf(iniFile,"LNB4_POWER=TRUE;                              LNB 4 Spannungsversorgung AN\n");
			} else {
				fprintf(iniFile,"LNB4_POWER=FALSE;                             LNB 4 Spannungsversorgung AUS\n");
			}
			if ( LNB[3].Switch22khz == TRUE ) {
				fprintf(iniFile,"LNB4_SWITCH_22KHZ=TRUE;                       LNB 4 Umschalten Band mit 22 KHZ AN\n");
			} else {
				fprintf(iniFile,"LNB4_SWITCH_22KHZ=FALSE;                      LNB 4 Umschalten Band mit 22 KHZ AUS\n");
			}
			if ( LNB[3].PowerReset == TRUE ) {
				fprintf(iniFile,"LNB4_POWERRESET=TRUE;                         LNB 4 SpannungsReset bei DiseqC-Kommando An\n");
			} else {
				fprintf(iniFile,"LNB4_POWERRESET=FALSE;                        LNB 4 SpannungsReset bei DiseqC-Kommando Aus\n");
			}
			fprintf(iniFile,";LNB4_MINIDISEQC=                             LNB 4 Mini-DiSEq ( Nur Toneburst ) verwenden \n");
			if ( LNB[3].MiniDiseqc == 0x01 ) {
				fprintf(iniFile,"LNB4_MINIDISEQC=TRUE;                         LNB 4 Mini-DiSEq An\n");
			} else {
				fprintf(iniFile,"LNB4_MINIDISEQC=FALSE;                        LNB 4 Mini-DiSEq Aus\n");
			}
			fprintf(iniFile,"LNB4_TONEBURST=%d;                            LNB 4 Burstzustand\n",LNB[3].BurstVal);
			fprintf(iniFile,";                                                  -1 = Nicht verwenden \n");
			fprintf(iniFile,";                                                   0 = Aus\n");
			fprintf(iniFile,";                                                   1 = An\n");
			fprintf(iniFile,"LNB4_POS=%c;                                  LNB 4 POS-Schalter\n",LNB[3].PosVal);
			fprintf(iniFile,";                                                   A = Position A\n");
			fprintf(iniFile,";                                                   B = Position B\n");
			fprintf(iniFile,"LNB4_OPT=%c;                                  LNB 4 OPT-Schalter\n",LNB[3].OptVal);
			fprintf(iniFile,";                                                   A = Option A\n");
			fprintf(iniFile,";                                                   B = Option B\n");
	    	fprintf(iniFile,"LNB4_ORBIT=%d;                                 LNB 4 Orbit position (0,1°)\n",LNB[3].orbit);
    		fprintf(iniFile,"LNB4_WEST_EAST=%c;                             LNB 4 West (W) East (E) flag\n",LNB[3].west_east);
		} else {
			fprintf(iniFile,"LNB4_USE=FALSE                                LNB 4 nicht verwendet\n");
		}
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; ****************** eigener DVBTuner *************************\n");
		fprintf(iniFile,"; \n");
		if ( DVBTuners[5].CableTuner  == TRUE ) {
			fprintf(iniFile,"DVB-TUNER-CABLE=TRUE;                         Als KabelTuner verwenden\n");
		} else {
			fprintf(iniFile,"DVB-TUNER-CABLE=FALSE;                        Als SAT-Tuner verwenden\n");
		}
		
		fprintf(iniFile,"DVB-TUNER-MIN=%d;                             Minimale Frequenz \n",DVBTuners[5].min/1000);
		fprintf(iniFile,"DVB-TUNER-MAX=%d;                             Maximale Frequenz \n",DVBTuners[5].max/1000);
		fprintf(iniFile,"DVB-TUNER-RES=%d;                             Frequenzauflösung\n",DVBTuners[5].res);
		fprintf(iniFile,"DVB-TUNER-STEP=%d;                            Frequensschritte\n",DVBTuners[5].step);
		fprintf(iniFile,"DVB-TUNER-THRESH1=%d;                         Schwellwert VHF_Low\n",DVBTuners[5].thresh1/1000);
		fprintf(iniFile,"DVB-TUNER-THRESH2=%d;                         Schwellwert VHF_High\n",DVBTuners[5].thresh2/1000);
		fprintf(iniFile,"DVB-TUNER-VHF_L=%d;                           Schaltwert  VHF_Low\n",DVBTuners[5].VHF_L);
		fprintf(iniFile,"DVB-TUNER-VHF_H=%d;                           Schaltwert  VHF_High\n",DVBTuners[5].VHF_H);
		fprintf(iniFile,"DVB-TUNER-UHF=%d;                             Schaltwert  UHF\n",DVBTuners[5].UHF);
		fprintf(iniFile,"DVB-TUNER-CONFIG=%d;                          Konfigurationsparameter Tuner\n",DVBTuners[5].config);
		fprintf(iniFile,"DVB-TUNER-I2C=%d;                             I2C - Bus - ID\n",DVBTuners[5].I2C);
		fprintf(iniFile,"DVB-TUNER-IFPCOFF=%d;                         Frequenz-Offset\n",DVBTuners[5].IFPCoff/1000);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; ****************** DVB Farbeinstellungen  *************************\n");
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; DVB-CONTRAST                              Letzter Wert für Kontrast\n");
		fprintf(iniFile,"DVB-CONTRAST=%d;               \n",InitialDVBContrast);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; DVB-BRIGHTNESS                            Letzter Wert für Helligkeit\n");
		fprintf(iniFile,"DVB-BRIGHTNESS=%d;          \n",InitialDVBBrightness);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; DVB-COLOR                                 Letzter Wert für Farbe \n");
		fprintf(iniFile,"DVB-COLOR=%d;        \n",InitialDVBColor);
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; ****************** DVB Audioeinstellungen  *************************\n");
		fprintf(iniFile,"; \n");
		fprintf(iniFile,"; DVB-VOLUME-LEFT                           Lautstärke linker Kanal\n");
		fprintf(iniFile,"DVB-VOLUME-LEFT=%d;        \n",InitialDVBVolumeLinks);
		fprintf(iniFile,"; DVB-VOLUME-RIGHT                          Lautstärke rechter Kanal\n");
		fprintf(iniFile,"DVB-VOLUME-RIGHT=%d;        \n",InitialDVBVolumeRechts);
} else {
	fprintf(iniFile,";                  ********************************\n");
	fprintf(iniFile,";                  **      KEINE DVB-Hardware   ***\n");
	fprintf(iniFile,";                  **            gefunden       ***\n");
	fprintf(iniFile,";                  ********************************\n");
}
fprintf(iniFile,"; \n");

#ifdef USE_BT8X8

fprintf(iniFile,"; ****************** BT8X8 Setup ****************************\n");
fprintf(iniFile,"; \n");
if ( Has_BT == TRUE ) {
	fprintf(iniFile,"; BT-Treibererweiterung=                      Erweiterten oder generic BT-Treiber verwenden\n");
	fprintf(iniFile,";                                             TRUE Treibererweiterungen verwenden\n");
	fprintf(iniFile,";                                             FALSE Generic Treiber verwenden\n");
	if ( LoadExternBTDriver == TRUE ) {
		fprintf(iniFile,"BT-Treibererweiterung=TRUE;\n");
	} else {
		fprintf(iniFile,"BT-Treibererweiterung=FALSE;\n");
	};
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; BT_IRQ_SHARING                              Die BT-Karte läuft im IRQ-Sharing oder auch nicht\n");
	fprintf(iniFile,";                                             Der Schalter ist ein Merker für die Start-Meldung!\n");
	fprintf(iniFile,";                                             Er hat keinen Einfluss aus das Sharing - Verhalten \n");
	if ( BT_IRQ_SHARING == TRUE ) {
		fprintf(iniFile,"BT_IRQ_SHARING=TRUE;\n");
	} else {
		fprintf(iniFile,"BT_IRQ_SHARING=FALSE;\n");
	}
	fprintf(iniFile,"; VIDEOKARTE                           Typ der Videokarte\n");
	fprintf(iniFile,";                                             0=Miro \n");
	fprintf(iniFile,";                                             1=Hauppauge \n");
	fprintf(iniFile,";                                             2=STB \n");
	fprintf(iniFile,";                                             3=Intel\n");
	fprintf(iniFile,";                                             4=Diamond DTV2000\n");
	fprintf(iniFile,";                                             5=AVerMedia TVPhone \n");
	fprintf(iniFile,";                                             6=Eigener Kartentyp\n");
	fprintf(iniFile,"VIDEOKARTE=%d;             \n",CardType);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; VIDEOSOURCE                          Video-Eingang/Source\n");
	fprintf(iniFile,";                                             0= Video_Tuner \n");
	fprintf(iniFile,";                                             1= Video_Ext1 \n");
	fprintf(iniFile,";                                             2= Video_Ext2 \n");
	fprintf(iniFile,";                                             3= Video_Ext3 \n");
	fprintf(iniFile,"VIDEOSOURCE=%d;             \n",VideoSource);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; AUDIOSOURCE                          Audio-Eingang/Source\n");
	fprintf(iniFile,";                                             0= Tuner \n");
	fprintf(iniFile,";                                             1= MSP-Baustein/Radio \n");
	fprintf(iniFile,";                                             2= External \n");
	fprintf(iniFile,";                                             3= Internal \n");
	fprintf(iniFile,";                                             4= Aus \n");
	fprintf(iniFile,"AUDIOSOURCE=%d;             \n",AudioSource);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; TUNERTYP                             Art des VideoTuners der Karte\n");
	fprintf(iniFile,";                                             0= Temic Pal \n");
	fprintf(iniFile,";                                             1= Philips Pal I \n");
	fprintf(iniFile,";                                             2= Philips Ntsc \n");
	fprintf(iniFile,";                                             3= Philips Secam \n");
	fprintf(iniFile,";                                             4= Unbekannt \n");
	fprintf(iniFile,";                                             5= Philips Pal \n");
	fprintf(iniFile,";                                             6= Temic Ntsc \n");
	fprintf(iniFile,";                                             7= Temic Pal I \n");
	fprintf(iniFile,";                                             8= Tuner mit manuellen Parametern\n");
	fprintf(iniFile,"TUNERTYP=%d;             \n",TunerType);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; TVTYP                                Art des TV-Signals\n");
	fprintf(iniFile,";                                      ( Die Spezialformate werden nicht abgespeichert)\n");
	fprintf(iniFile,";                                             0= PAL BDGHI \n");
	fprintf(iniFile,";                                             1= NTSC \n");
	fprintf(iniFile,";                                             2= SECAM \n");
	fprintf(iniFile,";                                             3= PAL M\n");
	fprintf(iniFile,";                                             4= PAL N\n");
	if ( TVTYPE > 4 ) TVTYPE=0;
	fprintf(iniFile,"TVTYP=%d;             \n",TVTYPE);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; COLORFORMAT                            Farbformat\n");
	fprintf(iniFile,";                                           0=RGB32           ( nicht unterstützt )\n");
	fprintf(iniFile,";                                           1=RGB24           ( nicht unterstützt )\n");
	fprintf(iniFile,";                                           2=RGB16        \n");
	fprintf(iniFile,";                                           3=RGB15        \n");
	fprintf(iniFile,";                                           4=YUV2  (422)   \n");
	fprintf(iniFile,";                                           5=BtYUV (411)     ( nicht unterstützt )\n");
	fprintf(iniFile,";                                           6=Y8              ( nicht unterstützt )\n");
	fprintf(iniFile,";                                           7=RGB8 (Dithered)  ( nicht unterstützt )\n");
	fprintf(iniFile,";                                           8=YCrCb 422 Planar ( nicht unterstützt )\n");
	fprintf(iniFile,";                                           9=YCrCb 411 Planar ( nicht unterstützt )\n");
	fprintf(iniFile,"COLORFORMAT=%d;             \n",ColourFormat);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; BT848-PLL                      PLL Initialisieren \n");
	fprintf(iniFile,";                                             0= Nicht verwenden \n");
	fprintf(iniFile,";                                             1= PLL mit 28.63636 MHz Quarz\n");
	fprintf(iniFile,";                                             2= PLL mit 35.46895 MHz Quarz\n");
	fprintf(iniFile,"BT848-PLL=%d;                     \n",INIT_PLL);
	fprintf(iniFile,"; \n");
#endif
	fprintf(iniFile,"; ****************** VBI-Daten Auswertung ****************************\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,";          Die Videodat-Einstellung wird nicht abgespeichert\n");
	fprintf(iniFile,";          Muß bei der entsprechenden Sendung eingeschaltet werden\n");
	fprintf(iniFile,"; \n");
#ifdef USE_BT8X8
	fprintf(iniFile,"; AUTO-DETECT                          Codierung automatisch erkennen\n");
	if ( Decode_AutoDetect == TRUE ) fprintf(iniFile,"AUTO-DETECT=TRUE;                   \n");
	else fprintf(iniFile,"AUTO-DETECT=FALSE;                   \n");
	fprintf(iniFile,";                                      MTV machte es notwendig\n");
	fprintf(iniFile,"; AUTO-DETECT-NAGRA                    Automatische Umschalten zur Systerdekodierung erlaubt\n");
	if ( AutoDetect_Nagra == TRUE ) fprintf(iniFile,"AUTO-DETECT-NAGRA=TRUE;                   \n");
	else fprintf(iniFile,"AUTO-DETECT-NAGRA=FALSE;                   \n");
	fprintf(iniFile,"; AUTO-DETECT-VC                       Automatische Umschalten zur Videocryptdekodierung erlaubt\n");
	if ( AutoDetect_VC == TRUE ) fprintf(iniFile,"AUTO-DETECT-VC=TRUE;                   \n");
	else fprintf(iniFile,"AUTO-DETECT-VC=FALSE;                   \n");
#endif
	fprintf(iniFile,"; VBI_VT                                   VideoText benutzen\n");
	if (VBI_Flags&VBI_VT) fprintf(iniFile,"VBI_VT=TRUE;                   \n");
	else fprintf(iniFile,"VBI_VT=FALSE;                   \n");
	fprintf(iniFile,"; VBI_VPS                                  VPS-Daten auswerten\n");
	if (VBI_Flags&VBI_VPS) fprintf(iniFile,"VBI_VPS=TRUE;                   \n");
	else fprintf(iniFile,"VBI_VPS=FALSE;                   \n");
#ifdef USE_BT8X8
	fprintf(iniFile,"; VBI_VC                                   Nagra/Videocrypt I/II auswerten\n");
	if (VBI_Flags&VBI_VC) fprintf(iniFile,"VBI_VC=TRUE;                   \n");
	else fprintf(iniFile,"VBI_VC=FALSE;                   \n");
#endif
	fprintf(iniFile,"; VBI_WINBIS                               WinBis-Daten auswerten\n");
	if (VBI_Flags&VBI_WINBIS) fprintf(iniFile,"VBI_WINBIS=TRUE;                   \n");
	else fprintf(iniFile,"VBI_WINBIS=FALSE;                   \n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; WINBIS_BASE_DIR                              Pfad für Datenablage WinBis\n");
	fprintf(iniFile,"WINBIS_BASE_DIR=%s;                      \n",WINIBIS_DIR);
	fprintf(iniFile,"; VD_BASE_DIR                              Pfad für Datenablage VideoDat\n");
	fprintf(iniFile,"VD_BASE_DIR=%s;                      \n",VD_DIR);
	fprintf(iniFile,"; VD_FILENAME                              Dateiname für Datenablage VideoDat (Raw)\n");
	fprintf(iniFile,"VD_FILENAME=%s;                      \n",VDat.RawName);
	fprintf(iniFile,"; VD_RAWFILE                               Videodat als RAW-File ablegen\n");
	if (VD_RAW == TRUE ) fprintf(iniFile,"VD_RAWFILE=TRUE;                   \n");
	else fprintf(iniFile,"VD_RAWFILE=FALSE;                   \n");
	
	fprintf(iniFile,"; \n");
#ifdef USE_BT8X8
	fprintf(iniFile,"; ****************** Special Registers Video ****************************\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; BT848-HUE-REGISTER                   Letzter Wert im Hue-Register\n");
	fprintf(iniFile,"BT848-HUE-REGISTER=%d;             \n",InitialHue);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; BT848-CONTRAST-REGISTER              Letzter Wert im Kontrast-Register\n");
	fprintf(iniFile,"BT848-CONTRAST-REGISTER=%d;             \n",InitialContrast);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; BT848-BRIGHTNESS-REGISTER            Letzter Wert im Helligkeit-Register\n");
	fprintf(iniFile,"BT848-BRIGHTNESS-REGISTER=%d;          \n",InitialBrightness);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; BT848-SATURATION_U-REGISTER          Letzter Wert im Farbe U -Register\n");
	fprintf(iniFile,"BT848-SATURATION_U-REGISTER=%d;        \n",InitialSaturationU);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; BT848-SATURATION_V-REGISTER          Letzter Wert im Farbe V-Register\n");
	fprintf(iniFile,"BT848-SATURATION_V-REGISTER=%d;        \n",InitialSaturationV);
	fprintf(iniFile,"; \n");
	if ( InitialIFORM != 0 ) { 
		fprintf(iniFile,"; BT848-IFORM-REGISTER          IFORM - Register wenn nicht default\n");
		fprintf(iniFile,"BT848-IFORM-REGISTER=%d;        \n",InitialIFORM);
		fprintf(iniFile,"; \n");
	}
	fprintf(iniFile,"; ****************** Eigener KartenType      ****************************\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"BT848_GPIO_OUT_EN=%d;             \n",ManuellAudio[0]);
	fprintf(iniFile,"BT848_GPIO_REG_INP=%d;             \n",ManuellAudio[7]);
	fprintf(iniFile,"BT848_GPIO_DATA_TUNER=%d;             \n",ManuellAudio[1]);
	fprintf(iniFile,"BT848_GPIO_DATA_RADIO=%d;             \n",ManuellAudio[2]);
	fprintf(iniFile,"BT848_GPIO_DATA_EXTERN=%d;             \n",ManuellAudio[3]);
	fprintf(iniFile,"BT848_GPIO_DATA_INTERN=%d;             \n",ManuellAudio[4]);
	fprintf(iniFile,"BT848_GPIO_DATA_AUS=%d;             \n",ManuellAudio[5]);
	fprintf(iniFile,"BT848_GPIO_DATA_AN=%d;             \n",ManuellAudio[6]);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; ****************** Special Registers Audio ****************************\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; MSPMODE                           MSP-Initialisierung\n");
	fprintf(iniFile,";                                      0 = AM (for carrier detect / msp3400\n");
	fprintf(iniFile,";                                      1 = AM (for carrier detect / msp3410\n");
	fprintf(iniFile,";                                      2 = FM Radio\n");
	fprintf(iniFile,";                                      3 = Terrestial FM-mono + FM-stereo\n");
	fprintf(iniFile,";                                      4 = Sat FM-mono\n");
	fprintf(iniFile,";                                      5 = NICAM B/G, D/K\n");
	fprintf(iniFile,";                                      6 = NICAM I\n");
	fprintf(iniFile,"MSPMODE=%d\n",MSPMode);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; MSPMAJOR                           MSP-Initialisierung Gruppe 0\n");
	fprintf(iniFile,";                                      0 = 4.5   NTSC\n");
	fprintf(iniFile,";                                      1 = 5.5   PAL B/G\n");
	fprintf(iniFile,";                                      2 = 6.0   PAL I\n");
	fprintf(iniFile,";                                      3 = 5.5   PAL D/K + SAT + SECAM\n");
	fprintf(iniFile,"MSPMAJOR=%d\n",MSPMajorMode);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; MSPMINOR                           MSP-Initialisierung Gruppe 1\n");
	fprintf(iniFile,";                                      0 = 5.742 PAL B/G FM-stereo\n");
	fprintf(iniFile,";                                      1 = 5.85  PAL B/G NICAM\n");
	fprintf(iniFile,";                                      2 = 5.85  PAL D/K NICAM\n");
	fprintf(iniFile,";                                      3 = 6.25  PAL D/K1 FM-stereo\n");
	fprintf(iniFile,";                                      4 = 6.74  PAL D/K2 FM-stereo\n");
	fprintf(iniFile,";                                      5 = 7.02  PAL SAT FM-stereo s/b\n");
	fprintf(iniFile,";                                      6 = 7.20  PAL SAT FM-stereo s\n");
	fprintf(iniFile,";                                      7 = 7.38  PAL SAT FM-stereo b\n");
	fprintf(iniFile,"MSPMINOR=%d\n",MSPMinorMode);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; MSPSTEREO                          MSP-Initialisierung StereoMode\n");
	fprintf(iniFile,";                                      1 = Mono\n");
	fprintf(iniFile,";                                      2 = Stereo\n");
	fprintf(iniFile,";                                      3 = ZweikanalTon Kanal 1\n");
	fprintf(iniFile,";                                      4 = ZweikanalTon Kanal 2\n");
	fprintf(iniFile,"MSPSTEREO=%d\n",MSPStereo);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; MSP-STEREO                         Stereomode automatisch anpassen\n");
	if ( AutoStereoSelect == TRUE ) fprintf(iniFile,"MSP-STEREO=TRUE;                   \n");
	else fprintf(iniFile,"MSP-STEREO=FALSE;                   \n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; IC2_VOLUME                            Letzte benutzte Lautstärke\n");
	fprintf(iniFile,"IC2_VOLUME=%d;             \n",InitialVolume);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; IC2_SPATIAL                          Letzter Wert im Spatial-Register\n");
	fprintf(iniFile,"IC2_SPATIAL=%d;             \n",InitialSpecial);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; IC2_LOUDNESS                         Letzter Wert im Loudness-Register\n");
	fprintf(iniFile,"IC2_LOUDNESS=%d;             \n",InitialLoudness);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; IC2_BASS                             Letzter Wert im Bass-Register\n");
	fprintf(iniFile,"IC2_BASS=%d;          \n",InitialBass);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; IC2_TREBLE                           Letzter Wert Treble-Register\n");
	fprintf(iniFile,"IC2_TREBLE=%d;        \n",InitialTreble);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; IC2_BALANCE                          Letzter Wert im Balance-Register\n");
	fprintf(iniFile,"IC2_BALANCE=%d;        \n",InitialBalance);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; I2C_SUPERBASS                        Superbass aktivieren\n");
	if ( InitialSuperBass == TRUE ) fprintf(iniFile,"I2C_SUPERBASS=TRUE;                   \n");
	else fprintf(iniFile,"I2C_SUPERBASS=FALSE;                   \n");
	fprintf(iniFile,"; IC2_EQUALIZER1                       Letzter Wert Equalizer1-Register\n");
	fprintf(iniFile,"IC2_EQUALIZER1=%d;        \n",InitialEqualizer1);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; IC2_EQUALIZER2                       Letzter Wert Equalizer2-Register\n");
	fprintf(iniFile,"IC2_EQUALIZER2=%d;        \n",InitialEqualizer2);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; IC2_EQUALIZER3                       Letzter Wert Equalizer3-Register\n");
	fprintf(iniFile,"IC2_EQUALIZER3=%d;        \n",InitialEqualizer3);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; IC2_EQUALIZER4                       Letzter Wert Equalizer4-Register\n");
	fprintf(iniFile,"IC2_EQUALIZER4=%d;        \n",InitialEqualizer4);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; IC2_EQUALIZER5                       Letzter Wert Equalizer5-Register\n");
	fprintf(iniFile,"IC2_EQUALIZER5=%d;        \n",InitialEqualizer5);
	fprintf(iniFile,"; \n");
#endif
	fprintf(iniFile,"; *********************** Risc - Steuerung  *****************************\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; CAPTUREVIDEO                            Capture Video \n");
	if ( Capture_Video == TRUE ) fprintf(iniFile,"CAPTUREVIDEO=TRUE;                   \n");
	else fprintf(iniFile,"CAPTUREVIDEO=FALSE;                   \n");
	fprintf(iniFile,"; CAPTURETEXT                            Capture VBI-Daten \n");
	if ( Capture_VBI == TRUE ) fprintf(iniFile,"CAPTUREVBI=TRUE;                   \n");
	else fprintf(iniFile,"CAPTUREVBI=FALSE;                   \n");
	fprintf(iniFile,"; LAENDERCODE                            Laendereinstellung\n");
	fprintf(iniFile,"LAENDERCODE=%d;        \n",CountryCode);
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; ********************* Tuner Verwenden*************************\n");
	fprintf(iniFile,"; \n");
	if ( USETUNER == TRUE ) fprintf(iniFile,"TUNER_USE=TRUE;               ;Tuner wird initialisiert  \n");
	else fprintf(iniFile,"TUNER_USE=FALSE;                  Tuner wird nicht initialisiert \n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"; ********************* Karte Initialisieren *************************\n");
	fprintf(iniFile,"; \n");
	if ( USECARD == TRUE ) fprintf(iniFile,"CARD_USE=TRUE;               ;Karte wird initialisiert  \n");
	else fprintf(iniFile,"CARD_USE=FALSE;                  Karte wird nicht initialisiert \n");
	fprintf(iniFile,"; \n");
#ifdef USE_BT8X8
	fprintf(iniFile,"; ********************* Manuelle Tunerparamter  *************************\n");
	fprintf(iniFile,"; \n");
	fprintf(iniFile,"TUNER_THRESH1=%d;                         Schwellwert VHF_Low\n",Tuners[8].thresh1);
	fprintf(iniFile,"TUNER_THRESH2=%d;                         Schwellwert VHF_High\n",Tuners[8].thresh2);
	fprintf(iniFile,"TUNER_VHF_L=%d;                           Schaltwert  VHF_Low\n",Tuners[8].VHF_L);
	fprintf(iniFile,"TUNER_VHF_H=%d;                           Schaltwert  VHF_High\n",Tuners[8].VHF_H);
	fprintf(iniFile,"TUNER_UHF=%d;                             Schaltwert  UHF\n",Tuners[8].UHF);
	fprintf(iniFile,"TUNER_CONFIG=%d;                          Konfigurationsparameter Tuner\n",Tuners[8].config);
	fprintf(iniFile,"TUNER_I2C=%d;                             I2C - Bus - ID (automatisch!! <ändern keinen sinn, speichern eigentlich auch nicht>)\n",Tuners[8].I2C);
	fprintf(iniFile,"TUNER_IFPCOFF=%d;                         Frequenz-Offset\n",Tuners[8].IFPCoff);
} else {
	fprintf(iniFile,";                  ********************************\n");
	fprintf(iniFile,";                  **       KEINE BT-Hardware   ***\n");
	fprintf(iniFile,";                  **           gefunden        ***\n");
	fprintf(iniFile,";                  ********************************\n");
}
#endif
fprintf(iniFile,"; \n");
fprintf(iniFile,"; *************************** FarbMischer *********************************\n");
fprintf(iniFile,"; \n");
if ( InvertRot == TRUE ) fprintf(iniFile,"INVERT-ROT=TRUE;               ;Rot invertieren \n");
else fprintf(iniFile,"INVERT-ROT=FALSE;                ; Rot invertieren \n");
if ( InvertGruen == TRUE ) fprintf(iniFile,"INVERT-GRUEN=TRUE;               ;Gruen invertieren \n");
else fprintf(iniFile,"INVERT-GRUEN=FALSE;                ; Gruen invertieren \n");
if ( InvertBlau == TRUE ) fprintf(iniFile,"INVERT-BLAU=TRUE;               ;Blau invertieren \n");
else fprintf(iniFile,"INVERT-BLAU=FALSE;                ; Blau invertieren \n");
if ( OffRot == TRUE ) fprintf(iniFile,"OFF-ROT=TRUE;               ;Rot aus \n");
else fprintf(iniFile,"OFF-ROT=FALSE;                ; Rot aus \n");
if ( OffGruen == TRUE ) fprintf(iniFile,"OFF-GRUEN=TRUE;               ;Gruen aus \n");
else fprintf(iniFile,"OFF-GRUEN=FALSE;                ; Gruen aus \n");
if ( OffBlau == TRUE ) fprintf(iniFile,"OFF-BLAU=TRUE;               ;Blau aus \n");
else fprintf(iniFile,"OFF-BLAU=FALSE;                ; Blau aus \n");
fprintf(iniFile,"; \n");
if ( HCMode1 == TRUE ) fprintf(iniFile,"COLORMIXMODE1=TRUE;               ;Farbmischer Typ 1  \n");
else fprintf(iniFile,"COLORMIXMODE1=FALSE;                ; Farbmischer Typ 1\n");
fprintf(iniFile,"MIXSCHWELLE=%d;                         Schwellwert Typ1 \n",HCSchwelle);
fprintf(iniFile,"MIXROT=%d;                              Typ 1 Rot erweiterung\n",HCRotFaktor);
fprintf(iniFile,"MIXGRUEN=%d;                              Typ 1 Gruen erweiterung\n",HCGruenFaktor);
fprintf(iniFile,"MIXBLAU=%d;                              Typ 1 Blau erweiterung\n",HCBlauFaktor);
fprintf(iniFile,"; \n");
if ( HCMode2 == TRUE ) fprintf(iniFile,"COLORMIXMODE2=TRUE;               ;Farbmischer Typ 2 \n");
else fprintf(iniFile,"COLORMIXMODE2=FALSE;                ; Farbmischer Typ 2\n");
fprintf(iniFile,"MIXCUTSCHWELLE=%d;                        Schwellwert Typ 2\n",HCCutFaktor);
fprintf(iniFile,"MIXCUTLOROT=%d;                           Typ 2 Rot unterhalb Schwelle\n",HCCutLoRotFaktor);
fprintf(iniFile,"MIXCUTLOGRUEN=%d;                           Typ 2 Gruen unterhalb Schwelle\n",HCCutLoGruenFaktor);
fprintf(iniFile,"MIXCUTLOBLAU=%d;                           Typ 2 Blau unterhalb Schwelle\n",HCCutLoBlauFaktor);
fprintf(iniFile,"MIXCUTHIROT=%d;                           Typ 2 Rot oberhalb Schwelle\n",HCCutHiRotFaktor);
fprintf(iniFile,"MIXCUTHIGRUEN=%d;                           Typ 2 Gruen oberhalb Schwelle\n",HCCutHiGruenFaktor);
fprintf(iniFile,"MIXCUTHIBLAU=%d;                           Typ 2 Blau oberhalb Schwelle\n",HCCutHiBlauFaktor);
fprintf(iniFile,"; \n");
#ifdef USE_BT8X8
fprintf(iniFile,"; ******************************************\n");
fprintf(iniFile,"; ** FILTRE BLEU/ROUGE SECAM              **\n");
fprintf(iniFile,"; ******************************************\n");
fprintf(iniFile,"; \n");
fprintf(iniFile,"; FILTREBLEU : TRUE = activer FALSE = désactiver\n");
if ( bFilterBlueLines == TRUE ) fprintf(iniFile,"FILTREBLEU=TRUE;                   \n");
else fprintf(iniFile,"FILTREBLEU=FALSE;                   \n");
fprintf(iniFile,"; SEUILBLEU                                : seuil de détection de ligne bleue ,valeur en % de 1 à 100\n");
fprintf(iniFile,"; SEUILBLEU                                : Secam-Threshold U (Blau) in  % von ( 1 bis 100 )\n");
fprintf(iniFile,"SEUILBLEU=%3d;\n",iUFilterThresholdPCent);
fprintf(iniFile,"; SEUILROUGE : seuil de détection de ligne rouge ,valeur en % de 1 à 100%\n");
fprintf(iniFile,"; SEUILROUGE                                : Secam-Threshold V (Rot) in  % von (	1 bis 100 )\n");
fprintf(iniFile,"SEUILROUGE=%3d;\n",iVFilterThresholdPCent);
fprintf(iniFile,"; \n");
#endif
fprintf(iniFile,"; *************************** VideoText *********************************\n");
fprintf(iniFile,"; \n");
fprintf(iniFile,"; VT-SPRACHE                               Default-Language-Einstellung beim VT\n");
fprintf(iniFile,";                                          0 Automatsch\n");
fprintf(iniFile,";                                          1 Englisch\n");
fprintf(iniFile,";                                          2 Deutsch\n");
fprintf(iniFile,";                                          3 Italienisch\n");
fprintf(iniFile,";                                          4 Französisch\n");
fprintf(iniFile,";                                          5 Spanisch\n");
fprintf(iniFile,"VT-SPRACHE=%d;                             \n",VT_Lang);
fprintf(iniFile,"; VT-SCREENFONT                            Default-Fenstergroesse beim starten vom VT-Fenster\n");
fprintf(iniFile,"VT-SCREENFONT=%d;                          \n",Initial_FontNr);
fprintf(iniFile,"; VT-BASE-DIR                              Pfad für Datenablage exportierter VT-Dateien\n");
fprintf(iniFile,"VT-BASE-DIR=%s;                 \n",VT_BASE_DIR);
fprintf(iniFile,"; VT-EXPORT-WWWROOT                        Für direktes Puplishing\n");
fprintf(iniFile,"VT-EXPORT-WWWROOT=%s;                 \n",VT_EXPORT_WWW_ROOT);
fprintf(iniFile,"; \n");
if ( VT_ALWAYS_EXPORT == TRUE ) fprintf(iniFile,"VT-ALWAYS-EXPORT=TRUE;                  Immer alles exportieren\n");
else fprintf(iniFile,"VT-ALWAYS-EXPORT=FALSE;                   Nur auf Anfrage exportieren\n");

fprintf(iniFile,"; VT-EXPORT-TYP                                Wie soll exportiert werden \n");
fprintf(iniFile,"VT-EXPORT-TYP=%d;                 \n",VT_EXPORT_TYP);
fprintf(iniFile,";                                               0 : Als BMP-Datei   \n");
fprintf(iniFile,";                                               1 : Als TXT-Datei   \n");
fprintf(iniFile,";                                               2 : Als HTM-Datei   \n");
fprintf(iniFile,";                                               3 : RAW-Datei  \n");
fprintf(iniFile,"; \n");
if ( VT_EXPORT_BACKGROUND == TRUE ) fprintf(iniFile,"VT-EXPORT-BACKGROUND=TRUE;                  Hintergrund-Htm-exportieren\n");
else fprintf(iniFile,"VT-EXPORT-BACKGROUND=FALSE;                   Hintergrund schwarz\n");
if ( VT_EXPORT_PAGE_LINK == TRUE ) fprintf(iniFile,"VT-EXPORT-LINKS=TRUE;                  3-stellige Zahlen als Links exportieren\n");
else fprintf(iniFile,"VT-EXPORT-LINKS=FALSE;                    3 stellige Zahlen sind keine Links\n");
if ( VT_EXPORT_PAGE_CONTROL == TRUE ) fprintf(iniFile,"VT-EXPORT-PAGE-CONTROL=TRUE;                  Seitensteuerung einfügen\n");
else fprintf(iniFile,"VT_EXPORT_PAGE_CONTROL=FALSE;                    Keine Seitensteuerung einfügen\n");
fprintf(iniFile,"; VT-EXPORT-FONT                            Default-Schriftart beim html-export\n");
fprintf(iniFile,"VT-EXPORT-FONT=%d;                          \n",Initial_Export_Font);
fprintf(iniFile,"; \n");
fprintf(iniFile,"; Farben werdem im Format Color_R,Color_G,Color_B abgelegt\n");
fprintf(iniFile,"; \n");
fprintf(iniFile,"VT-COLOR-SCHWARZ=%d,%d,%d\n",GetRValue(VTColourTable[0]),GetGValue(VTColourTable[0]),GetBValue(VTColourTable[0]));
fprintf(iniFile,"VT-COLOR-ROT=%d,%d,%d\n",GetRValue(VTColourTable[1]),GetGValue(VTColourTable[1]),GetBValue(VTColourTable[1]));
fprintf(iniFile,"VT-COLOR-GRUEN=%d,%d,%d\n",GetRValue(VTColourTable[2]),GetGValue(VTColourTable[2]),GetBValue(VTColourTable[2]));
fprintf(iniFile,"VT-COLOR-GELB=%d,%d,%d\n",GetRValue(VTColourTable[3]),GetGValue(VTColourTable[3]),GetBValue(VTColourTable[3]));
fprintf(iniFile,"VT-COLOR-BLAU=%d,%d,%d\n",GetRValue(VTColourTable[4]),GetGValue(VTColourTable[4]),GetBValue(VTColourTable[4]));
fprintf(iniFile,"VT-COLOR-HELLBLAU=%d,%d,%d\n",GetRValue(VTColourTable[6]),GetGValue(VTColourTable[6]),GetBValue(VTColourTable[6]));
fprintf(iniFile,"VT-COLOR-WEIS=%d,%d,%d\n",GetRValue(VTColourTable[7]),GetGValue(VTColourTable[7]),GetBValue(VTColourTable[7]));
fprintf(iniFile,"VT-COLOR-UNSICHTBAR=%d,%d,%d\n",GetRValue(VTColourTable[5]),GetGValue(VTColourTable[5]),GetBValue(VTColourTable[5]));
fprintf(iniFile,"VT-COLOR-TRANSPARENT=%d,%d,%d\n",GetRValue(VTColourTable[8]),GetGValue(VTColourTable[8]),GetBValue(VTColourTable[8]));
fprintf(iniFile,"; \n");
fprintf(iniFile,"; *************************** Mixer-Device  *********************************\n");
fprintf(iniFile,"; \n");
if ( USE_MIXER == TRUE ) fprintf(iniFile,"USE_MIXER=TRUE;                  ;Mixersteuerung übernehmen\n");
else fprintf(iniFile,"USE_MIXER=FALSE;                 ;Mixersteuerung nicht übernehmen\n");
fprintf(iniFile,"; \n");
fprintf(iniFile,"MIXER_VOLUME_LINKER_KANAL=%d;	;Lautstärke linker AudioKanal\n",MIXER_LINKER_KANAL);
fprintf(iniFile,"MIXER_VOLUME_RECHTER_KANAL=%d;	;Lautstärke rechter AudioKanal\n",MIXER_RECHTER_KANAL);
// BigReaper
fprintf(iniFile, "MUTE_STATUS=%d;\n", System_In_Mute);
fprintf(iniFile,"; \n");
fprintf(iniFile,"; ******************************************\n");
fprintf(iniFile,"; **         Die folgenden Einträge       **\n");
fprintf(iniFile,"; **         sollten nicht von Hand       **\n");
fprintf(iniFile,"; **            editiert werden           **\n");
fprintf(iniFile,"; **                                      **\n");
fprintf(iniFile,"; ** Beispiel:                            **\n");
fprintf(iniFile,"; ** MIXER_VOLUME_CONTROL=AAA BBB CCC DDD **\n");
fprintf(iniFile,"; **                                      **\n");
fprintf(iniFile,"; ** AAA identifiziert das Soundsubsystem **\n");
fprintf(iniFile,"; **     das auszusprechen ist.           **\n");
fprintf(iniFile,"; **                                      **\n");
fprintf(iniFile,"; ** BBB identifiziert den Ausgang, der   **\n");
fprintf(iniFile,"; **     auszusprechen ist.               **\n");
fprintf(iniFile,"; **                                      **\n");
fprintf(iniFile,"; ** CCC identifiziert den dazugehörigen  **\n");
fprintf(iniFile,"; **     Eingang                          **\n");
fprintf(iniFile,"; **                                      **\n");
fprintf(iniFile,"; ** DDD steht für das Control            **\n");
fprintf(iniFile,"; **                                      **\n");
fprintf(iniFile,"; ** Wenn iht in den Dialog unter Ausgabe **\n");
fprintf(iniFile,"; **  -> Audio-Mixer Einstellungen geht   **\n");
fprintf(iniFile,"; ** und dort auf zuweisen klickt, so     **\n");
fprintf(iniFile,"; ** wird euch für jedes Control die      **\n");
fprintf(iniFile,"; ** Interne ID angegeben.                **\n");
fprintf(iniFile,"; **                                      **\n");
fprintf(iniFile,"; ** Bei den Settings in MIXER_BOOT_UP    **\n");
fprintf(iniFile,"; **          (Wenn vorhanden)            **\n");
fprintf(iniFile,"; ** sind 4 weitere Werte enthalten,      **\n");
fprintf(iniFile,"; ** Diese Werte werden in die maximal 4  **\n");
fprintf(iniFile,"; ** Kanäle zum Startzeitpunkt eingestellt**\n");
fprintf(iniFile,"; **                                      **\n");
fprintf(iniFile,"; ******************************************\n");
fprintf(iniFile,";\n");
fprintf(iniFile,"MIXER_VOLUME_CONTROL=%03d %03d %03d %03d ;  Control-Einstellungen für Volume \n",Volume.SoundSystem,Volume.Destination,Volume.Connection,Volume.Control );
fprintf(iniFile,"MIXER_MUTE_CONTROL=%03d %03d %03d %03d ;  Control-Einstellungen für Mute \n",Mute.SoundSystem,Mute.Destination,Mute.Connection,Mute.Control );
fprintf(iniFile,";Startup-Defaults\n");
for ( i=0; i < 64; i++ ) {
	if ( MixerLoad[i].MixerAccess.SoundSystem >= 0 ) {
        fprintf(iniFile,"MIXER_BOOT_UP=%03d %03d %03d %03d %d %d %d %d\n",MixerLoad[i].MixerAccess.SoundSystem,MixerLoad[i].MixerAccess.Destination,MixerLoad[i].MixerAccess.Connection,MixerLoad[i].MixerAccess.Control,MixerLoad[i].MixerValues.Kanal1,MixerLoad[i].MixerValues.Kanal2,MixerLoad[i].MixerValues.Kanal3,MixerLoad[i].MixerValues.Kanal4);
	};
}


fclose(iniFile);
return(TRUE);
}
