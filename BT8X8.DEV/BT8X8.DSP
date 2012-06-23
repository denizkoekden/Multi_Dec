# Microsoft Developer Studio Project File - Name="BT8x8Dev" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=BT8x8Dev - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BT8x8.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BT8x8.mak" CFG="BT8x8Dev - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BT8x8Dev - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "BT8x8Dev - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "BT8x8Dev - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f BT8x8Dev.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "BT8x8Dev.exe"
# PROP BASE Bsc_Name "BT8x8Dev.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "D:\SOURCE\MD\BT8x8.dev\compile.bat"
# PROP Rebuild_Opt ""
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "BT8x8Dev - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f BT8x8Dev.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "BT8x8Dev.exe"
# PROP BASE Bsc_Name "BT8x8Dev.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "BT8x8.dev\compile.bat"
# PROP Rebuild_Opt ""
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "BT8x8Dev - Win32 Release"
# Name "BT8x8Dev - Win32 Debug"

!IF  "$(CFG)" == "BT8x8Dev - Win32 Release"

!ELSEIF  "$(CFG)" == "BT8x8Dev - Win32 Debug"

!ENDIF 

# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BT8x8.c
# End Source File
# Begin Source File

SOURCE=.\compile.bat
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BTIOcom.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\BT8x8.rc
# End Source File
# End Group
# Begin Source File

SOURCE=.\BT8x8.mak
# End Source File
# End Target
# End Project
