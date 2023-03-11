# Microsoft Developer Studio Project File - Name="NewSerial" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NewSerial - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NewSerial.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NewSerial.mak" CFG="NewSerial - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NewSerial - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NewSerial - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NewSerial - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Ws2_32.lib Version.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Release/CITool.exe"

!ELSEIF  "$(CFG)" == "NewSerial - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Ws2_32.lib Version.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/CITool.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "NewSerial - Win32 Release"
# Name "NewSerial - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AutoPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\Doc.cpp
# End Source File
# Begin Source File

SOURCE=.\FindItemNumDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HashTable.cpp
# End Source File
# Begin Source File

SOURCE=.\HashTable.h
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\NewSerial.cpp
# End Source File
# Begin Source File

SOURCE=.\NewSerial.rc
# End Source File
# Begin Source File

SOURCE=.\ProgressCtrlX.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UIThread.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AbyssHash.h
# End Source File
# Begin Source File

SOURCE=.\AbyssHash64.h
# End Source File
# Begin Source File

SOURCE=.\AutoPatch.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrame.h
# End Source File
# Begin Source File

SOURCE=.\Doc.h
# End Source File
# Begin Source File

SOURCE=.\Drawgdix.h
# End Source File
# Begin Source File

SOURCE=.\FindItemNumDlg.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\GuardianFrame.h
# End Source File
# Begin Source File

SOURCE=.\IOleDBThread.h
# End Source File
# Begin Source File

SOURCE=.\ItemManager.h
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrame.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\NewSerial.h
# End Source File
# Begin Source File

SOURCE=.\ProgressCtrlX.h
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\UIThread.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Character.ico
# End Source File
# Begin Source File

SOURCE=.\res\Doc.ico
# End Source File
# Begin Source File

SOURCE=.\res\My.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NewSerial.ico
# End Source File
# Begin Source File

SOURCE=.\res\NewSerial.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Server.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "View"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChildFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\GuardianFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\GuardianView.cpp
# End Source File
# Begin Source File

SOURCE=.\GuardianView.h
# End Source File
# Begin Source File

SOURCE=.\MyView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyView.h
# End Source File
# End Group
# Begin Group "Japan"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CopyItemDelete.cpp
# End Source File
# Begin Source File

SOURCE=.\CopyItemDelete.h
# End Source File
# Begin Source File

SOURCE=.\CopyItemDeleteFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CopyItemDeleteFrame.h
# End Source File
# Begin Source File

SOURCE=.\CopyItemFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CopyItemFrame.h
# End Source File
# Begin Source File

SOURCE=.\CopyItemView.cpp
# End Source File
# Begin Source File

SOURCE=.\CopyItemView.h
# End Source File
# End Group
# Begin Group "Guild"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GuildFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildFrame.h
# End Source File
# Begin Source File

SOURCE=.\GuildInitView.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildInitView.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Target
# End Project
