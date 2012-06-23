
DRV_NAME  = MDSAA

!ifdef WIN95
BINDIR     = WIN95
DRV_NM     = $(DRV_NAME).vxd
CVXDFLAGS  = -Zdp -Gs -c -DIS_32 -Zl -DDEBLEVEL=1 -DDEBUG -DWIN95
AFLAGS     = -coff -DBLD_COFF -DIS_32 -W2 -Zd -c -Cx -DMASM6 -DDEBLEVEL=1 -DDEBUG
INCDIRS    = 
!else
!ifndef BASEDIR
!ERROR set BASEDIR enviroment variable to the of NTDDK path. For example: SET BASEDIR=C:\NTDDK
!endif
BINDIR     = WINNT
DRV_NM     = $(DRV_NAME).sys
CVXDFLAGS  = -nologo -DWINVER=0x030A -D_X86_=1 -Di386=1 -DSTD_CALL -DCONDITION_HANDLING=1 -DWIN32_LEAN_AND_MEAN=1 -DNT_UP=1  -DNT_INST=0 -DWIN32=100 -D_NT1X_=100 -DNT_UP=1 -Dtry=__try -Dleave=__leave -Dexcept=__except -Dfinally=__finally -D_CRTAPI1=__cdecl -D_CRTAPI2=__cdecl  -Ditoa=_itoa  -Dstrcmpi=_strcmpi  -Dstricmp=_stricmp  -Dwcsicmp=_wcsicmp  -Dwcsnicmp=_wcsnicmp  -DDBG=0 -DDEVL=1 -DFPO=1 -D_IDWBUILD /c /Zel /Zp8 /Gy /W3 /Gz /G4 /Oxs /Gf /Oy 
INCDIRS    = -I. -I$(BASEDIR)\inc -I$(BASEDIR)\inc\ddk -I$(BASEDIR)\inc\crt 
!endif
LIBDIR     = ..\Drvlib

OBJECTS= $(BINDIR)\SAA7146.obj

all: direxist $(BINDIR)\$(DRV_NM)

direxist:
        if not exist $(BINDIR)\nul    md $(BINDIR)

$(BINDIR)\SAA7146.obj: SAA7146.c
        cl $(CVXDFLAGS) $(INCDIRS) -Fo$@ %s

!ifndef WIN95
$(BINDIR)\SAA7146.res: SAA7146.rc
    rc -l 409  -r -fo $@ -D_X86_=1 -Di386=1  -DSTD_CALL -DCONDITION_HANDLING=1 -DWIN32_LEAN_AND_MEAN=1 -DNT_UP=1  -DNT_INST=0 -DWIN32=100 -D_NT1X_=100 -DNT_UP=1 -Dtry=__try -Dleave=__leave -Dexcept=__except -Dfinally=__finally -D_CRTAPI1=__cdecl -D_CRTAPI2=__cdecl  -Ditoa=_itoa  -Dstrcmpi=_strcmpi  -Dstricmp=_stricmp  -Dwcsicmp=_wcsicmp  -Dwcsnicmp=_wcsnicmp  -DDBG=0 -DDEVL=1 -DFPO=1 -D_IDWBUILD $(INCDIRS) %s
!endif

!ifdef WIN95
$(BINDIR)\$(DRV_NM): $(OBJECTS) SAA7146.def
        echo >NUL @<<$(DRV_NAME).crf
-MACHINE:i386 -DEBUG:NONE -PDB:NONE 
-DEF:SAA7146.def -OUT:$@ -MAP:$(BINDIR)\$(DRV_NAME).map
-VXD $(OBJECTS) $(LIBDIR)\kp_95w.obj $(LIBDIR)\kp_95.lib
-IGNORE:4039 -IGNORE:4078
<<
        link @$(DRV_NAME).crf
        cd $(BINDIR)
        mapsym $(DRV_NAME)
        cd ..
!else

$(BINDIR)\$(DRV_NM): $(OBJECTS) $(BINDIR)\SAA7146.res
        echo >NUL @<<$(DRV_NAME).crf
-MERGE:_PAGE=PAGE 
-MERGE:_TEXT=.text 
-SECTION:INIT,d 
-OPT:REF 
-RELEASE
-INCREMENTAL:NO
-FULLBUILD 
-FORCE:MULTIPLE 
-IGNORE:4001 -IGNORE:4037 -IGNORE:4039 -IGNORE:4065 -IGNORE:4070 -IGNORE:4078 -IGNORE:4087 -IGNORE:4089 
-PDB:NONE 
-debug:notmapped,FULL
-debugtype:coff
-version:3.51 
-osversion:3.51 
-MERGE:.rdata=.text 
-align:0x20 
-subsystem:native,3.51 
-machine:i386 
-base:0x10000 
-entry:DriverEntry@8
-driver
$(LIBDIR)\kp_nt.lib 
$(BASEDIR)\lib\i386\free\ntoskrnl.lib 
$(BINDIR)\SAA7146.res 
$(BINDIR)\SAA7146.obj 
-out:$@ 
<<
        link @$(DRV_NAME).crf
!endif

clean:
        -@del /Q $(BINDIR)\*
