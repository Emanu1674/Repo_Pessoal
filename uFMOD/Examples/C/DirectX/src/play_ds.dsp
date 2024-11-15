# Microsoft Developer Studio Project File - Name="play_ds" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=play_ds - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "play_ds.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "play_ds.mak" CFG="play_ds - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "play_ds - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /FD /c
# ADD CPP /nologo /Zp1 /W3 /Ox /Ot /Og /Oi /Gf /FD /c
# SUBTRACT CPP /Os /Gy /YX /Yc /Yu
# ADD BASE RSC /l 0x240a
# ADD RSC /l 0x240a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib /machine:IX86
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comctl32.lib comdlg32.lib shell32.lib ole32.lib /entry:"start" /subsystem:windows /pdb:none /machine:I386 /out:"..\play_ds.exe" /RELEASE /opt:nowin98 /MERGE:.rdata=.text -ignore:4078
# Begin Target

# Name "play_ds - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\dsufmod.h
# End Source File
# Begin Source File

SOURCE=.\play_ds.c
# End Source File
# Begin Source File

SOURCE=.\play_ds.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\player.ico
# End Source File
# Begin Source File

SOURCE=.\player.rc
# End Source File
# Begin Source File

SOURCE=.\tbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\dsufmod.obj
# End Source File
# End Target
# End Project
