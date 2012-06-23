/*********************************************************************************
 *                                                                               *
 * RemoteC.h: Tools für die Fernbedienung                                        *
 *                                                                               *
 * Copyright (C) 2000/2001 Espresso                                              *
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

struct TKeyTable {
		unsigned char Name[16];
};

struct TKeyTable KeyTable[256];


struct TRemoteKey {
	    unsigned short RepeatRate;
		unsigned short TimeOutRate;
        unsigned char SwitchPos;
        DWORD LastTickCount;
		unsigned char SendKey[5][128]; 

};

struct TRemoteKey *RemoteKey = NULL;

int  Alter_IR_Key=0;
HWND   REMOTE_CONFIG_WND;
char CurrentSectionName[128];

char AllSections[128][128];
int AllSectionCount;

char Running_Remote_Control_Section[128];
char Last_Running_Remote_Control_Section[128];

char CodeString[128];

FARPROC OldEditProc;
HWND    KeyEditWnd;

INPUT InputToSend[256];
int SendCount;
int RepeatDelay;
int RepeatStatus=0;

extern int Current_Mode;
extern BOOL PROGRAMM_OSD;
extern BOOL OSD_FULL_INPUT;
extern HWND hWnd;


BOOL  TimerSend=FALSE;
int   TimerKey=-1;