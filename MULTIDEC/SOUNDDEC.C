/*********************************************************************************
 *                                                                               *
 * sounddec.c                                                                    *
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

#ifdef USE_BT8X8

#include <windows.h> 
#include <windowsx.h> 
#include <mmsystem.h> 

#include <math.h>
#include "Sounddec.h"

//-----------------------------------------------------------------------------

#define BUFFERS    5			// Splitted buffer chunks.  
							            	// Latency is approx. 1/BUFFERS of second
#define MODFREQ    12800		// Frequency of audio signal modulation
#define SOUNDFREQ  44100/*(MODFREQ * 2)*/
#define BPSAMPLE   2
#define BUFLEN    ((SOUNDFREQ * BPSAMPLE) / BUFFERS)

//-----------------------------------------------------------------------------


#define M_PI    3.141592654
#define M_2PI  (2.0 * M_PI)

SHORT cos_tab [ SOUNDFREQ ] ;

#define FIX_SHIFT 15

//-----------------------------------------------------------------------------

static WAVEHDR  waveIHeader [ BUFFERS ] ;
static WAVEHDR  waveOHeader [ BUFFERS ] ;
static BYTE     bufferI     [ BUFFERS ][ BUFLEN ] ;
static BYTE     bufferO     [ BUFFERS ][ BUFLEN ] ;
static HWAVEOUT hWaveO ;
static HWAVEIN  hWaveI ;

extern HWND hWnd;

//-----------------------------------------------------------------------------
//
// WaveStreamCallback:
//
//  Here we decode a chunk of sound
//
void CALLBACK WaveStreamCallback ( HWAVEIN hwi , UINT uMsg , 
								   DWORD dwInstance , DWORD dwParam1 , DWORD dwParam2 )
{
	LPWAVEHDR wI , wO ;
	PSHORT    src , dst ;
	int       i ;

	if ( !useSound )
		return ;

	if ( uMsg == WIM_DATA )
	{
		wI = (LPWAVEHDR) dwParam1 ;

		if ( wI -> dwBytesRecorded == wI -> dwBufferLength )
		{
			wO = (LPWAVEHDR) wI -> dwUser ;

			src = (PSHORT) wI -> lpData ;
			dst = (PSHORT) wO -> lpData ;

/*			// THIS WORKED WHEN SOUNDFREQ = 25600
			// AND src & dst WERE DEFINED AS PULONG
			for ( i = 0 ; i < BUFLEN / 4 ; i ++ )
				*dst ++ = *src ++ ^ 0x0000FFFF ;*/

			for ( i = 0 ; i < BUFLEN / 2 ; i ++ )
				*dst ++ = ((int)(*src ++) * cos_tab[i]) >> FIX_SHIFT ;
				
			   waveOutWrite( hWaveO , wO , sizeof(WAVEHDR) ) ;
			waveInAddBuffer( hWaveI , wI , sizeof(WAVEHDR) ) ;
		}
	}
}

//-----------------------------------------------------------------------------
//
// SoundCapture_Open:
//
//  Here we make the setup for the characteristics of the captured sound.
//
void SoundCapture_Open ()
{

	
	WAVEFORMATEX waveFormat ;
	int    i ;
	double freq ;

	// Set up audio capture format...
	waveFormat.wFormatTag      = WAVE_FORMAT_PCM ;
	waveFormat.nChannels       = 1 ;
    waveFormat.nSamplesPerSec  = SOUNDFREQ ;
	waveFormat.wBitsPerSample  = BPSAMPLE * 8 ;
	waveFormat.nBlockAlign     = waveFormat.nChannels * BPSAMPLE ;
    waveFormat.nAvgBytesPerSec = ((long)waveFormat.nSamplesPerSec * waveFormat.nBlockAlign) ;

	canSound = (waveOutOpen( &hWaveO , WAVE_MAPPER , &waveFormat , 
							 0 , 0 , CALLBACK_NULL ) == MMSYSERR_NOERROR) &&
			   (waveInOpen(  &hWaveI , WAVE_MAPPER , &waveFormat , 
							 (ULONG) WaveStreamCallback , 
							 0 , CALLBACK_FUNCTION ) == MMSYSERR_NOERROR) ;

	if ( canSound )
	{
		for ( i = 0 ; i < BUFFERS ; i ++ )
		{
			waveIHeader[i].dwUser		  = (ULONG) &waveOHeader[i] ;
			waveIHeader[i].lpData         = bufferI[i] ;
			waveIHeader[i].dwBufferLength = BUFLEN ;
			waveIHeader[i].dwLoops        = 0 ;
            waveIHeader[i].dwFlags        = 0 ;
			if (i==0)	waveIHeader[i].dwFlags        = WHDR_BEGINLOOP  ;
			if (i==BUFFERS)	waveIHeader[i].dwFlags        = WHDR_ENDLOOP  ;

			waveOHeader[i].lpData         = bufferO[i] ;
			waveOHeader[i].dwBufferLength = BUFLEN ;
			waveOHeader[i].dwLoops        = 1 ;
            waveOHeader[i].dwFlags        = 0 ;
			if (i==0)	waveOHeader[i].dwFlags        = WHDR_BEGINLOOP  ;
			if (i==BUFFERS)	waveOHeader[i].dwFlags    = WHDR_ENDLOOP  ;

			canSound = canSound && 
						(waveInPrepareHeader(  hWaveI , &waveIHeader[i] , sizeof(WAVEHDR) ) 
							== MMSYSERR_NOERROR) &&
						(waveInAddBuffer(      hWaveI , &waveIHeader[i] , sizeof(WAVEHDR) ) 
							== MMSYSERR_NOERROR) &&
						(waveOutPrepareHeader( hWaveO , &waveOHeader[i] , sizeof(WAVEHDR) )
							== MMSYSERR_NOERROR) ;

			if ( !canSound )
				break ;

		}
	}

	if ( canSound )
	{
		freq = ((double)MODFREQ / (double)SOUNDFREQ * M_2PI) ;
		for ( i = 0 ; i < SOUNDFREQ ; i ++ )
			cos_tab[i] = (SHORT) ( cos( freq * i ) * (double)(1 << FIX_SHIFT)) ;
			}
	else
	{
		MessageBox(hWnd,"Wave-Device kann nicht noch einmal geöffnet werden.\n MultiDec muß neu gestartet werden !","Multi-Decode",MB_ICONSTOP|MB_OK);
		SoundCapture_Close() ;
		useSound = FALSE ;		
	}
}

//-----------------------------------------------------------------------------
//
// SoundCapture_Close:
//
//  Here we shut down the sound devices
//
void SoundCapture_Close ()
{
	int i ;

	if ( !canSound )
		return ;

	SoundCapture_Stop() ;

/*	do
	{
		shit = TRUE ;
		for ( i = 0 ; i < BUFFERS ; i ++ )
			if ( waveOHeader[i].dwFlags == WHDR_INQUEUED )
			{
				shit = FALSE ;
				break ;
			}
	}
	while ( !shit ) ;*/

	for ( i = 0 ; i < BUFFERS ; i ++ )
	{
		 waveInUnprepareHeader( hWaveI , &waveIHeader[i] , sizeof(WAVEHDR) ) ;
		 waveOutUnprepareHeader( hWaveO , &waveOHeader[i] , sizeof(WAVEHDR) ) ;
	}

	 waveInClose( hWaveI ) ;
	waveOutClose( hWaveO ) ;

	canSound = FALSE ;	
}

//-----------------------------------------------------------------------------
//
// SoundCapture_Play/Stop:
//
//  Here we make the ball roll/stop
//
int SoundCapture_Play ()
{
	MMRESULT ret;

		if ( canSound ) {
		ret=waveInStart( hWaveI ) ;
        switch (ret) {
		case MMSYSERR_NOERROR :
			return(0);
		case MMSYSERR_INVALHANDLE :
			return(1);
		case MMSYSERR_NODRIVER :
			return(2);
		case MMSYSERR_NOMEM :
			return(3);
        default :
			return(4);
		};
		
		
		}
return(5);

}

void SoundCapture_Stop ()
{
	MMRESULT ret;
	if ( canSound )
	{
		  ret=waveInReset( hWaveI ) ;
		  ret=waveOutReset( hWaveO ) ;
		  ret=waveInStop( hWaveI ) ;
	}
}

#endif