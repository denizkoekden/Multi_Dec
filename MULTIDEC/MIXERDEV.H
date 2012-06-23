/*********************************************************************************
 *                                                                               *
 * Mixerdev.h: Ansteuerung MixerDevice                                           *
 *                                                                               *
 * Copyright (C) 1999/2000 Espresso                                              *
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


#define MAXKANAELE 4

unsigned int LastOpenMixer=0xffffffff;

struct TMixerAccess {
    int SoundSystem;
    int Destination;
    int Connection;
    int Control;
};

struct TMixerValues {
    int Kanal1;
    int Kanal2;
    int Kanal3;
    int Kanal4;
};


extern struct TMixerAccess Volume;
extern struct TMixerAccess Mute;

struct TMixerLoad {
        struct TMixerAccess MixerAccess;
        struct TMixerValues MixerValues;
};
extern struct TMixerLoad MixerLoad[64];

extern HMIXER hMixer=NULL;
 
extern HANDLE hInst;


struct TMixerControls {
             int AnzahlControls;
	         MIXERCONTROL          *MixerControl;
             MIXERCONTROLDETAILS   *MixerDetail;
};

struct TMixerConnections {
	        int AnzahlConnections;
			MIXERLINE *MixerConnections;
			struct TMixerControls *To_Control;
};


struct TMixerLines {
	        int AnzahlLines;
			MIXERLINE *MixerLine;
			struct TMixerConnections *To_Connection;
};



struct TSoundSystem {
	    int DeviceAnzahl;
        MIXERCAPS *MixerDev;
		struct TMixerLines *To_Lines;
};


struct TSoundSystem SoundSystem;

extern BOOL System_In_Mute;
extern BOOL USE_MIXER;
extern int  MIXER_LINKER_KANAL;
extern int  MIXER_RECHTER_KANAL;
int MIXER_MITTE;
extern int MixerVolumeStep;
extern int MixerVolumeMax;

