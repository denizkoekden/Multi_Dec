/*********************************************************************************
 *                                                                               *
 * Service : Service-Control-Parts for the NT                                    *
 *                                                                               *
 * Copyright (C) 2000 Espresso                                                   *
 *                                                                               *
 *                                                                               *
 *  originally based on code by:                                                 *
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
 */
 
#include <windows.h>       /* required for all Windows applications */
#include <stdio.h>
#include "../DrvLib/windrvr.h"
#include "service.h"
#include "resource.h"
extern BOOL APIENTRY MDBootOptionProc(HWND hDlg,UINT message,UINT wParam,LONG lParam);

extern void Write_Log(const char *Text);
extern const char* t(const char *s);
BOOL LoadDeviceDriver( const TCHAR * Name, const TCHAR * Path, HANDLE * lphDevice,BOOL Install);
BOOL UnloadDeviceDriver( const TCHAR * Name,BOOL DRemove);
BOOL InstallDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName, IN LPCTSTR ServiceExe );
BOOL RemoveDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName );
BOOL StartDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName );
BOOL StopDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName );


unsigned int Register_MDHal_Driver( void );

BOOL Get_Version_Informations(LPSTR Text)
{
		OSVERSIONINFO osvi;
        BOOL RetOnNT=FALSE;
		char Zeile[128];
		Zeile[0]=0x00;
		osvi.dwOSVersionInfoSize=sizeof(osvi);
		if (GetVersionEx(&osvi) == TRUE ) {
			if ( osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS ) {
             RetOnNT=FALSE;
				if  (osvi.dwMinorVersion == 0 ) {
					if ( osvi.dwBuildNumber == 67109975 ) strcpy(Zeile,"Windows 95 OSR 2");
					     else strcpy(Zeile,"Windows 95");
				} else if  (osvi.dwMinorVersion == 10 ) {
					if ( osvi.dwBuildNumber == 67766222 ) strcpy(Zeile,"Windows 98");
					     else if ( osvi.dwBuildNumber == 67766446 ) strcpy(Zeile,"Windows 98 SE");
				} else if  (osvi.dwMinorVersion == 90 ) {
					strcpy(Zeile,"Windows ME ");
					if (( strlen(osvi.szCSDVersion) > 0 ) && ( strlen(osvi.szCSDVersion) < 32 )) strcat(Zeile,osvi.szCSDVersion);
				};
			} else if ( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT ) { 
			RetOnNT=TRUE;
			    if  (osvi.dwMajorVersion == 3 ) {
					strcpy(Zeile,"Windows NT 3.XX ");
					if (( strlen(osvi.szCSDVersion) > 0 ) && ( strlen(osvi.szCSDVersion) < 32 )) strcat(Zeile,osvi.szCSDVersion);
				} else if  (osvi.dwMajorVersion == 4 ) {
					strcpy(Zeile,"Windows NT 4.0 ");
					if (( strlen(osvi.szCSDVersion) > 0 ) && ( strlen(osvi.szCSDVersion) < 32 )) strcat(Zeile,osvi.szCSDVersion);
				} else if  (osvi.dwMajorVersion == 5 ) {
					if  (osvi.dwMinorVersion == 0 ) strcpy(Zeile,"Windows 2000 ");
					   else strcpy(Zeile,"Windows XP ");
					if (( strlen(osvi.szCSDVersion) > 0 ) && ( strlen(osvi.szCSDVersion) < 32 )) strcat(Zeile,osvi.szCSDVersion);

				};

				};

		}

if ( Zeile[0] == 0x00 ) sprintf(Zeile,"Major %d Minor %d Build %d",osvi.dwMajorVersion,osvi.dwMinorVersion,osvi.dwBuildNumber);		
strcpy(Text,Zeile);
return(RetOnNT);
};

int Start_MDHal( void ) {

	    unsigned int Register;
        HANDLE hWD;
		char VersionString[255];
		char Path[255];
		HANDLE		Device_Handle;


  	 NT=Get_Version_Informations(&VersionString[0]);

     Fail_BOOT=FALSE;
	
     sprintf(Path,t("Betriebssystem : %s "),VersionString);
     Write_Log(Path);
 	 SetDlgItemText(SplashWnd,IDC_TEXT1,VersionString);
     Fail_BOOT=FALSE;

    hWD = WD_Open();


	sprintf(Path,"OrgDriverName : %s ",OrgDriverName);
	Write_Log(Path);
	sprintf(Path,"OrgDVBDriverName : %s ",OrgDVBDriverName);
	Write_Log(Path);

    if ( OrgDriverName[0] != 0x00 ) {
			  UnloadDeviceDriver((const char *)OrgDriverName,FALSE);
			  Sleep(500);
		 };
	if ( OrgDVBDriverName[0] != 0x00 ) {
			  UnloadDeviceDriver((const char *)OrgDVBDriverName,FALSE);
			  Sleep(500);
		 };

	
	if ( hWD == (void *)0xffffffff) {
        sprintf(Path,t("Starte Service-Installation"));
        Write_Log(Path);
        
        if ( NT == TRUE ) {
            Write_Log(">>>>>>>> MDHALDR.SYS For NT <<<<<<<<");
            
            GetCurrentDirectory(  sizeof (Path),Path );
            strcat(Path,"\\MDHALDR.SYS");
            Write_Log(Path);
            
            if ( ! LoadDeviceDriver( "MultiDec HAL-Treiber", Path, &Device_Handle,TRUE  ) )  {
                return(-1);
            };
        } else {
            Write_Log(">>>>>>>> MDHALDR.VXD For 9x <<<<<<<<");
            
            Device_Handle=CreateFile("\\\\.\\MDHALDR.VXD",0,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_DELETE_ON_CLOSE,0);
            if(Device_Handle==INVALID_HANDLE_VALUE){
                DWORD x = GetLastError();
                if ( x != 50 ) return(-2);
            }
        }
	} else {
    	 sprintf(Path,t("Service bereits installiert"));
         Write_Log(Path);
  		 WD_Close (hWD);
	};

// Ich benutze Windriver, und um jungo nicht weiter zu schaden, 
// liegt diese Funktion in einer DLL , die nur mit MultiDec läuft! 
// ( Andere Programme würden keine Freischaltung bekommen )
//
// Sorry Jungo, aber 3500.- DM habe ich nicht, aber ich mache ab sofort 
// ( eigentlich schon immer ) Werbung für euch 

	Register = Register_MDHal_Driver();
    if ( Register !=0xefcf1ffe ) {
    	 sprintf(Path,t("Service - Register fehlerhaft ( Return : 0x%08x)"),Register);
         Write_Log(Path);
		return(-3);
	}
    	 sprintf(Path,t("Service - OK"));
         Write_Log(Path);
	return(0);
};


/****************************************************************************
*
*    FUNCTION: LoadDeviceDriver( const TCHAR, const TCHAR, HANDLE *)
*
*    PURPOSE: Registers a driver with the system configuration manager 
*	 and then loads it.
*
****************************************************************************/
BOOL LoadDeviceDriver( const TCHAR * Name, const TCHAR * Path, HANDLE * lphDevice,BOOL Install)
{
	SC_HANDLE	schSCManager;
	BOOL		okay;
	DWORD Service_Error;
	
	char LogZeile[128];

	sprintf(LogZeile,"Service Installations");
	Write_Log(LogZeile);
	sprintf(LogZeile,"=====================");
	Write_Log(LogZeile);

	schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );

	if ( schSCManager == NULL ) {
          Service_Error  = GetLastError();
		   sprintf(LogZeile,"OpenSCManager für %s (%s) return %d ",Name,Path,Service_Error);
		  if ( Service_Error == ERROR_ACCESS_DENIED ) strcat(LogZeile,"ACCESS_DENIED");
        	 else if ( Service_Error == ERROR_DATABASE_DOES_NOT_EXIST ) strcat(LogZeile,"DATABASE_DOES_NOT_EXIST");
	         else if ( Service_Error == ERROR_INVALID_PARAMETER ) strcat(LogZeile,"INVALID_PARAMETER");
		strcat(LogZeile,")");
		Write_Log(LogZeile);
        if ( Service_Error == ERROR_ACCESS_DENIED ) MessageBox(hWnd,"Sie besitzen nicht die notwendigen Rechte für die Verwaltung von Geräten","Multi-Decode",MB_ICONSTOP|MB_OK);
	}
	// Ignore success of installation: it may already be installed.
	if ( Install == TRUE ) InstallDriver( schSCManager, Name, Path );

	// Ignore success of start: it may already be started.
	okay=StartDriver( schSCManager, Name );

 	CloseServiceHandle( schSCManager );

	return okay;
}


/****************************************************************************
*
*    FUNCTION: InstallDriver( IN SC_HANDLE, IN LPCTSTR, IN LPCTSTR)
*
*    PURPOSE: Creates a driver service.
*
****************************************************************************/
BOOL InstallDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName, IN LPCTSTR ServiceExe )
{
    SC_HANDLE  schService;
	DWORD Service_Error;
	char LogZeile[128];
    int ret;
	char DependOn[256];
    LPCTSTR dependencies=NULL;

    memset(&DependOn[0],0x00,256);

	if ( ( strcmp((char *)DriverName,"MultiDec BT-Treiber") == 0 ) ||
		( strcmp((char *)DriverName,"MultiDec SAA-Treiber") == 0 ) ) {
		strcpy(DependOn,"MultiDec HAL-Treiber");
		dependencies=&DependOn[0];
	};
	
	//
    // NOTE: This creates an entry for a standalone driver. If this
    //       is modified for use with a driver that requires a Tag,
    //       Group, and/or Dependencies, it may be necessary to
    //       query the registry for existing driver information
    //       (in order to determine a unique Tag, etc.).
    //

    schService = CreateService( SchSCManager,          // SCManager database
                                DriverName,           // name of service
                                DriverName,           // name to display
                                SERVICE_ALL_ACCESS | STANDARD_RIGHTS_REQUIRED  ,    // desired access
                                SERVICE_KERNEL_DRIVER, // service type
                                SERVICE_AUTO_START,  // start type
                                SERVICE_ERROR_NORMAL,  // error control type
                                ServiceExe,            // service's binary
                                NULL,                  // no load ordering group
                                NULL,                  // no tag identifier
                                dependencies,          // no dependencies
                                NULL,                   // LocalSystem account
                                NULL                   // no password
                                );

    if ( schService == NULL )
    {
        Service_Error = GetLastError();
   	 sprintf(LogZeile,"CreateService %s Fail ( Return %x ",DriverName,Service_Error);
	 if ( Service_Error == ERROR_ACCESS_DENIED ) strcat(LogZeile,"ACCESS_DENIED");
		else if ( Service_Error == ERROR_CIRCULAR_DEPENDENCY ) strcat(LogZeile,"CIRCULAR_DEPENDENCY");
		else if ( Service_Error == ERROR_DUP_NAME ) strcat(LogZeile,"DUP_NAME");
		else if ( Service_Error == ERROR_INVALID_HANDLE ) strcat(LogZeile,"INVALID_HANDLE");
		else if ( Service_Error == ERROR_INVALID_NAME ) strcat(LogZeile,"INVALID_NAME");
		else if ( Service_Error == ERROR_INVALID_PARAMETER ) strcat(LogZeile,"INVALID_PARAMETER");
		else if ( Service_Error == ERROR_INVALID_SERVICE_ACCOUNT ) strcat(LogZeile,"INVALID_SERVICE_ACCOUNT");
		else if ( Service_Error == ERROR_SERVICE_EXISTS ) strcat(LogZeile,"SERVICE_EXISTS");
		strcat(LogZeile,")");
		Write_Log(LogZeile);
	};



		if(Service_Error == ERROR_SERVICE_EXISTS)
        {

            schService = OpenService( SchSCManager,
                                      DriverName,
                                      SERVICE_CHANGE_CONFIG
                                      );
      
			    if ( schService == NULL ) {
                   Service_Error  = GetLastError();
	               sprintf(LogZeile,"OpenService %s Fail ( Return %x ",DriverName,Service_Error);
					if ( Service_Error == ERROR_ACCESS_DENIED ) strcat(LogZeile,"ACCESS_DENIED");
					else if ( Service_Error == ERROR_INVALID_HANDLE ) strcat(LogZeile,"INVALID_HANDLE");
					else if ( Service_Error == ERROR_INVALID_NAME ) strcat(LogZeile,"INVALID_NAME");
					else if ( Service_Error == ERROR_SERVICE_DOES_NOT_EXIST ) strcat(LogZeile,"SERVICE_DOES_NOT_EXIST");
					strcat(LogZeile,")");
					Write_Log(LogZeile);
				};



			if ( DependOn[0] == 0x00 ) {
			ret = ChangeServiceConfig(
                schService,
                SERVICE_KERNEL_DRIVER, // service type
                SERVICE_AUTO_START,  // start type
                SERVICE_ERROR_NORMAL,
                ServiceExe,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                DriverName
                );
			} else {
			ret = ChangeServiceConfig(
                schService,
                SERVICE_KERNEL_DRIVER, // service type
                SERVICE_AUTO_START,  // start type
                SERVICE_ERROR_NORMAL,
                ServiceExe,
                NULL,
                NULL,
                DependOn,
                NULL,
                NULL,
                DriverName
                );
			};
            if( ret != 0  ) {
                CloseServiceHandle( schService );
				return TRUE;
			};
            Service_Error = GetLastError();
	
   	 sprintf(LogZeile,"ChangeService %s Fail ( Return %x ",DriverName,Service_Error);
	 if ( Service_Error == ERROR_ACCESS_DENIED ) strcat(LogZeile,"ACCESS_DENIED");
		else if ( Service_Error == ERROR_CIRCULAR_DEPENDENCY ) strcat(LogZeile,"CIRCULAR_DEPENDENCY");
		else if ( Service_Error == ERROR_DUP_NAME ) strcat(LogZeile,"DUP_NAME");
		else if ( Service_Error == ERROR_INVALID_HANDLE ) strcat(LogZeile,"INVALID_HANDLE");
		else if ( Service_Error == ERROR_INVALID_PARAMETER ) strcat(LogZeile,"INVALID_PARAMETER");
		else if ( Service_Error == ERROR_INVALID_SERVICE_ACCOUNT ) strcat(LogZeile,"INVALID_SERVICE_ACCOUNT");
		else if ( Service_Error == ERROR_SERVICE_MARKED_FOR_DELETE ) strcat(LogZeile,"SERVICE_MARKED_FOR_DELETE");
		strcat(LogZeile,")");
		Write_Log(LogZeile);
        CloseServiceHandle( schService );
		return FALSE;
	};

// Hier

if ( schService != NULL ) CloseServiceHandle( schService );
return(TRUE);
};
	

/****************************************************************************
*
*    FUNCTION: StartDriver( IN SC_HANDLE, IN LPCTSTR)
*
*    PURPOSE: Starts the driver service.
*
****************************************************************************/
BOOL StartDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName )
{
    SC_HANDLE  schService;
    BOOL       ret;
	SERVICE_STATUS ServiceStatus;
	char LogZeile[128];
    DWORD Service_Error ;
	

	schService = OpenService( SchSCManager,
                              DriverName,
                              SERVICE_ALL_ACCESS
                              );
    if ( schService == NULL ) {
    Service_Error  = GetLastError();
	sprintf(LogZeile,"OpenService %s Fail ( Return %x ",DriverName,Service_Error);
	if ( Service_Error == ERROR_ACCESS_DENIED ) strcat(LogZeile,"ACCESS_DENIED");
	 else if ( Service_Error == ERROR_INVALID_HANDLE ) strcat(LogZeile,"INVALID_HANDLE");
	 else if ( Service_Error == ERROR_INVALID_NAME ) strcat(LogZeile,"INVALID_NAME");
	 else if ( Service_Error == ERROR_SERVICE_DOES_NOT_EXIST ) strcat(LogZeile,"SERVICE_DOES_NOT_EXIST");
	 strcat(LogZeile,")");
	Write_Log(LogZeile);
    return(FALSE);
	};


	if ( QueryServiceStatus(schService,&ServiceStatus ) == FALSE ) {
    Service_Error  = GetLastError();
	sprintf(LogZeile,"QueryServiceStatus %s Fail ( Return %x ",DriverName,Service_Error);
	if ( Service_Error == ERROR_ACCESS_DENIED ) strcat(LogZeile,"ACCESS_DENIED");
	 else if ( Service_Error == ERROR_INVALID_HANDLE ) strcat(LogZeile,"INVALID_HANDLE");
	 strcat(LogZeile,")");
	 Write_Log(LogZeile);
     CloseServiceHandle( schService );
	 return(FALSE);
	};

    if ( ServiceStatus.dwCurrentState == SERVICE_RUNNING ) return(TRUE);


    ret = StartService( schService, 0, NULL );

    if ( ret == FALSE )
    {
       Service_Error  = GetLastError();
        if( Service_Error   == ERROR_SERVICE_ALREADY_RUNNING)
        {
           CloseServiceHandle( schService );
		   return(TRUE);
        } else {
            
	    sprintf(LogZeile,"StartService %s Fail ( Return %x ",DriverName,Service_Error);
		if ( Service_Error == ERROR_ACCESS_DENIED ) strcat(LogZeile,"ACCESS_DENIED");
			else if ( Service_Error == ERROR_INVALID_HANDLE ) strcat(LogZeile,"INVALID_HANDLE");
			else if ( Service_Error == ERROR_PATH_NOT_FOUND ) strcat(LogZeile,"PATH_NOT_FOUND");
			else if ( Service_Error == ERROR_SERVICE_DATABASE_LOCKED ) strcat(LogZeile,"SERVICE_DATABASE_LOCKED");
			else if ( Service_Error == ERROR_SERVICE_DEPENDENCY_DELETED ) strcat(LogZeile,"DEPENDENCY_DELETED");
			else if ( Service_Error == ERROR_SERVICE_DEPENDENCY_FAIL ) strcat(LogZeile,"DEPENDENCY_FAIL");
			else if ( Service_Error == ERROR_SERVICE_DISABLED ) strcat(LogZeile,"SERVICE_DISABLED");
			else if ( Service_Error == ERROR_SERVICE_LOGON_FAILED ) strcat(LogZeile,"SERVICE_LOGON_FAILED");
			else if ( Service_Error == ERROR_SERVICE_MARKED_FOR_DELETE ) strcat(LogZeile,"SERVICE_MARKED_FOR_DELETE");
			else if ( Service_Error == ERROR_SERVICE_NO_THREAD ) strcat(LogZeile,"SERVICE_NO_THREAD");
			else if ( Service_Error == ERROR_SERVICE_REQUEST_TIMEOUT ) strcat(LogZeile,"SERVICE_REQUEST_TIMEOUT");
		strcat(LogZeile,")");
		Write_Log(LogZeile);
		RemoveDriver(SchSCManager, DriverName);
        return(FALSE);
        }
    }
 


    CloseServiceHandle( schService );

    return ret;
}

/****************************************************************************
*
*    FUNCTION: UnloadDeviceDriver( const TCHAR *)
*
*    PURPOSE: Stops the driver and has the configuration manager unload it.
*
****************************************************************************/
BOOL UnloadDeviceDriver( const TCHAR * Name,BOOL DRemove)
{
	SC_HANDLE	schSCManager;

	if (( strcmp(Name,"MultiDec BT-Treiber") == 0 ) ||
			( strcmp(Name,"MultiDec SAA-Treiber") == 0 ) ||
			( strcmp(Name,"MultiDec HAL-Treiber") == 0 )) return(TRUE); 

	if ( Keep_Driver_Loaded == TRUE ) {
		/*
		if (( strcmp(Name,"MultiDec BT-Treiber") == 0 ) ||
			( strcmp(Name,"MultiDec SAA-Treiber") == 0 ) ||
			( strcmp(Name,"MultiDec HAL-Treiber") == 0 )) 
		*/
		return(TRUE); 
	};

	schSCManager = OpenSCManager(	NULL,                 // machine (NULL == local)
                              		NULL,                 // database (NULL == default)
									SC_MANAGER_ALL_ACCESS // access required
								);

	if ( StopDriver( schSCManager, Name ) == FALSE ) {
    	CloseServiceHandle( schSCManager );
		return(TRUE);
	};

	if ( DRemove == TRUE ) RemoveDriver( schSCManager, Name );
	CloseServiceHandle( schSCManager );

	return TRUE;
}



/****************************************************************************
*
*    FUNCTION: StopDriver( IN SC_HANDLE, IN LPCTSTR)
*
*    PURPOSE: Has the configuration manager stop the driver (unload it)
*
****************************************************************************/
BOOL StopDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName )
{
    SC_HANDLE       schService;
    BOOL            ret;
    SERVICE_STATUS  serviceStatus;
    DWORD Service_Error ;
    char LogZeile[128];

    schService = OpenService( SchSCManager, DriverName, SERVICE_ALL_ACCESS );
    if ( schService == NULL )
        return FALSE;

    ret = ControlService( schService, SERVICE_CONTROL_STOP, &serviceStatus );

    if ( ret == FALSE )
    {
       Service_Error  = GetLastError();
	    sprintf(LogZeile,"StopService %s Fail ( Return 0x%X  ",DriverName,Service_Error);
		if ( Service_Error == ERROR_ACCESS_DENIED ) strcat(LogZeile,"ACCESS_DENIED");
			else if ( Service_Error == ERROR_DEPENDENT_SERVICES_RUNNING ) strcat(LogZeile,"ERROR_DEPENDENT_SERVICES_RUNNING");
			else if ( Service_Error == ERROR_INVALID_SERVICE_CONTROL ) strcat(LogZeile,"ERROR_INVALID_SERVICE_CONTROL");
			else if ( Service_Error == ERROR_SERVICE_CANNOT_ACCEPT_CTRL ) strcat(LogZeile,"ERROR_SERVICE_CANNOT_ACCEPT_CTRL");
			else if ( Service_Error == ERROR_SERVICE_NOT_ACTIVE ) strcat(LogZeile,"ERROR_SERVICE_NOT_ACTIVE");
			else if ( Service_Error == ERROR_SERVICE_REQUEST_TIMEOUT ) strcat(LogZeile,"ERROR_SERVICE_REQUEST_TIMEOUT");

		strcat(LogZeile,")");
		Write_Log(LogZeile);
    };

    CloseServiceHandle( schService );

    return ret;
}


/****************************************************************************
*
*    FUNCTION: RemoveDriver( IN SC_HANDLE, IN LPCTSTR)
*
*    PURPOSE: Deletes the driver service.
*
****************************************************************************/
BOOL RemoveDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName )
{
    SC_HANDLE  schService=NULL;
    BOOL       ret;


		if (( strcmp(DriverName,"MultiDec BT-Treiber") == 0 ) ||
			( strcmp(DriverName,"MultiDec SAA-Treiber") == 0 ) ||
			( strcmp(DriverName,"MultiDec HAL-Treiber") == 0 )) return(TRUE); 


	if ( Keep_Driver_Loaded == TRUE ) {
		/*
			if (( strcmp(DriverName,"MultiDec BT-Treiber") == 0 ) ||
			( strcmp(DriverName,"MultiDec SAA-Treiber") == 0 ) ||
			( strcmp(DriverName,"MultiDec HAL-Treiber") == 0 )) 
		*/
			return(TRUE); 
	};

    schService = OpenService( SchSCManager,
                              DriverName,
                              DELETE
                              );

    if ( schService == NULL )
        return FALSE;

    ret = DeleteService( schService );

    CloseServiceHandle( schService );

    return ret;
}






