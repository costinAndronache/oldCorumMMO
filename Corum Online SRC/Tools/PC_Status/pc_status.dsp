# Microsoft Developer Studio Project File - Name="pc_status" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=pc_status - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pc_status.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pc_status.mak" CFG="pc_status - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pc_status - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "pc_status - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pc_status - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 d3dx8dt.lib d3d8.lib d3dxof.lib ddraw.lib dxguid.lib dinput8.lib dsound.lib dxerr8.lib winmm.lib Version.lib nafxcw.lib Shell32.lib /nologo /subsystem:windows /incremental:yes /machine:I386
# SUBTRACT LINK32 /debug /nodefaultlib

!ELSEIF  "$(CFG)" == "pc_status - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3dx8dt.lib d3d8.lib dxerr8.lib d3dxof.lib dxguid.lib dsound.lib ddraw.lib winmm.lib version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "pc_status - Win32 Release"
# Name "pc_status - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ControlToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\cpu_info.cpp
# End Source File
# Begin Source File

SOURCE=.\Debug.cpp
# End Source File
# Begin Source File

SOURCE=.\GetDxVer.cpp
# End Source File
# Begin Source File

SOURCE=.\graphic_card_info.cpp
# End Source File
# Begin Source File

SOURCE=.\IconButton.cpp
# End Source File
# Begin Source File

SOURCE=.\NewLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\pc_status.cpp
# End Source File
# Begin Source File

SOURCE=.\pc_status.rc
# End Source File
# Begin Source File

SOURCE=.\pc_statusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\spec_info.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecList.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecList.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ControlToolTip.h
# End Source File
# Begin Source File

SOURCE=.\cpu_info.h
# End Source File
# Begin Source File

SOURCE=.\Debug.h
# End Source File
# Begin Source File

SOURCE=.\GetDllversion.h
# End Source File
# Begin Source File

SOURCE=.\GetDxVer.h
# End Source File
# Begin Source File

SOURCE=.\global_define.h
# End Source File
# Begin Source File

SOURCE=.\graphic_card_info.h
# End Source File
# Begin Source File

SOURCE=.\IconButton.h
# End Source File
# Begin Source File

SOURCE=.\NewLabel.h
# End Source File
# Begin Source File

SOURCE=.\pc_status.h
# End Source File
# Begin Source File

SOURCE=.\pc_statusDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\spec_info.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\Corum_logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\fit.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_dan.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_har.ico
# End Source File
# Begin Source File

SOURCE=.\pc_status.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pc_status.ico
# End Source File
# Begin Source File

SOURCE=.\res\pc_status.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\graphic_card_info.asp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section pc_status : {E7B330F1-6D5D-4385-9DD9-7603D415DDF1}
# 	2:16:CLASS: CNewLabel:CNewLabel
# 	2:19:CLASS: CTextRotator:CTextRotator
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:pc_status.h
# 	2:10:NewLabel.h:NewLabel.h
# 	2:12:NewLabel.cpp:NewLabel.cpp
# End Section
# Section pc_status : {D1FC1340-5747-4F95-BE6F-4BC3C4857369}
# 	2:18:CLASS: CIconButton:CIconButton
# 	2:12:IconButton.h:IconButton.h
# 	2:19:Application Include:pc_status.h
# 	2:14:IconButton.cpp:IconButton.cpp
# End Section
# Section pc_status : {CBA7484C-3A80-4765-86ED-985305EBE292}
# 	2:16:ControlToolTip.h:ControlToolTip.h
# 	2:18:ControlToolTip.cpp:ControlToolTip.cpp
# 	2:22:CLASS: CControlToolTip:CControlToolTip
# 	2:19:Application Include:pc_status.h
# End Section
