# Microsoft Developer Studio Project File - Name="MultiDec" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MultiDec - Win32 (80x86) Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MultiDec.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MultiDec.mak" CFG="MultiDec - Win32 (80x86) Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MultiDec - Win32 (80x86) Release" (based on "Win32 (x86) Application")
!MESSAGE "MultiDec - Win32 (80x86) Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MultiDec - Win32 (80x86) Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\WinRel"
# PROP Intermediate_Dir ".\WinRel"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /G6 /MT /W3 /Gi /O2 /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "DONT_USE_BT8X8" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /D "NDEBUG" /mktyplib203 /win32
# SUBTRACT MTL /nologo
# ADD BASE RSC /l 0x1 /d "NDEBUG"
# ADD RSC /l 0x420 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib mfc30.lib mfco30.lib mfcd30.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ddraw.lib dxguid.lib kernel32.lib user32.lib gdi32.lib advapi32.lib winmm.lib comdlg32.lib comctl32.lib /nologo /subsystem:windows /map:"../MultiDec.map" /debug /machine:I386 /out:"..\MultiDec.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MultiDec - Win32 (80x86) Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\WinDebug"
# PROP Intermediate_Dir ".\WinDebug"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /G6 /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "DONT_USE_BT8X8" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /D "_DEBUG" /mktyplib203 /win32
# SUBTRACT MTL /nologo
# ADD BASE RSC /l 0x1 /d "_DEBUG"
# ADD RSC /l 0x1 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib mfc30d.lib mfco30d.lib mfcd30d.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ddraw.lib dxguid.lib kernel32.lib user32.lib gdi32.lib advapi32.lib winmm.lib comdlg32.lib /nologo /subsystem:windows /profile /debug /machine:I386
# SUBTRACT LINK32 /map /nodefaultlib

!ENDIF 

# Begin Target

# Name "MultiDec - Win32 (80x86) Release"
# Name "MultiDec - Win32 (80x86) Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Bt848.c
# End Source File
# Begin Source File

SOURCE=.\DVB.C
# End Source File
# Begin Source File

SOURCE=.\Dvr.c
# End Source File
# Begin Source File

SOURCE=.\i18n.c
# End Source File
# Begin Source File

SOURCE=.\MixerDev.c
# End Source File
# Begin Source File

SOURCE=.\MultiDec.C
# End Source File
# Begin Source File

SOURCE=.\MultiDec.RC
# End Source File
# Begin Source File

SOURCE=.\Nagra.c
# End Source File
# Begin Source File

SOURCE=.\RemoteC.c
# End Source File
# Begin Source File

SOURCE=.\Service.c
# End Source File
# Begin Source File

SOURCE=.\Sounddec.c
# End Source File
# Begin Source File

SOURCE=.\STATUS.C
# End Source File
# Begin Source File

SOURCE=.\vcrypt.c
# End Source File
# Begin Source File

SOURCE=.\vt.c
# End Source File
# Begin Source File

SOURCE=.\MdHalReg.lib
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\bt848.h
# End Source File
# Begin Source File

SOURCE=.\DVB.H
# End Source File
# Begin Source File

SOURCE=.\Dvr.h
# End Source File
# Begin Source File

SOURCE=.\Globals.h
# End Source File
# Begin Source File

SOURCE=.\MixerDev.h
# End Source File
# Begin Source File

SOURCE=.\MultiDec.H
# End Source File
# Begin Source File

SOURCE=.\Nagra.h
# End Source File
# Begin Source File

SOURCE=.\RemoteC.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Service.h
# End Source File
# Begin Source File

SOURCE=.\Sounddec.h
# End Source File
# Begin Source File

SOURCE=.\vcrypt.h
# End Source File
# Begin Source File

SOURCE=.\vt.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RES\BirneGR.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\BirneROT.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\RES\dank.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\Dlg_mark.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Dlg_play.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\DLG_Rec.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\Dlg_Time.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Espresso.ico
# End Source File
# Begin Source File

SOURCE=.\RES\FreeShop.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\NO_Kabel.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\NO_Sat.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\OSD_Mark.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\OSD_Play.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\OSD_REc.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\OSD_Time.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\OSDMute.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\OsdRec.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\OSDRecP.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\recordpa.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\SatShop.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\satshop1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Sizex.cur
# End Source File
# Begin Source File

SOURCE=.\RES\SliderH.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\SliderV.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\STARTUP.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\STARTUP1.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\startup2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\startup3.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\startup4.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\startup5.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Vtback.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Vthome.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Vtnext.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Vtupdown.ico
# End Source File
# Begin Source File

SOURCE=.\RES\Werbe.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=..\Multidec.ini
# End Source File
# Begin Source File

SOURCE=..\Readme.txt
# End Source File
# End Target
# End Project
