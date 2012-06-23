# Microsoft Developer Studio Project File - Name="SAA7146Dev" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=SAA7146Dev - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SAA7146.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SAA7146.mak" CFG="SAA7146Dev - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SAA7146Dev - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "SAA7146Dev - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "SAA7146Dev - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f SAA7146Dev.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SAA7146Dev.exe"
# PROP BASE Bsc_Name "SAA7146Dev.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "D:\SOURCE\MD\SAA7146.dev\compile.bat"
# PROP Rebuild_Opt ""
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "SAA7146Dev - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f SAA7146Dev.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SAA7146Dev.exe"
# PROP BASE Bsc_Name "SAA7146Dev.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "SAA7146.dev\compile.bat"
# PROP Rebuild_Opt ""
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "SAA7146Dev - Win32 Release"
# Name "SAA7146Dev - Win32 Debug"

!IF  "$(CFG)" == "SAA7146Dev - Win32 Release"

!ELSEIF  "$(CFG)" == "SAA7146Dev - Win32 Debug"

!ENDIF 

# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\compile.bat
# End Source File
# Begin Source File

SOURCE=.\SAA7146.c
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\SAAIOcom.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\SAA7146.rc
# End Source File
# End Group
# Begin Source File

SOURCE=.\SAA7146.mak
# End Source File
# End Target
# End Project
