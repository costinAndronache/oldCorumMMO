# Microsoft Developer Studio Project File - Name="PatchMaker" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PatchMaker - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PatchMaker.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PatchMaker.mak" CFG="PatchMaker - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PatchMaker - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PatchMaker - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "PatchMaker - Win32 Release_NoEncryption" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Localizing/PatchMaker", EISAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PatchMaker - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /Ot /Oa /Ow /Oi /Oy /I "./ZipArchive" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Version.lib ZipArchive.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /pdb:none /machine:I386 /libpath:".\ZipArchive\Unicode_Release"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "PatchMaker - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Version.lib ZipArchive.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"D:\Workspace\PatchMaker\ZipArchive\Unicode_Debug"

!ELSEIF  "$(CFG)" == "PatchMaker - Win32 Release_NoEncryption"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PatchMaker___Win32_Release_NoEncryption"
# PROP BASE Intermediate_Dir "PatchMaker___Win32_Release_NoEncryption"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PatchMaker___Win32_Release_NoEncryption"
# PROP Intermediate_Dir "PatchMaker___Win32_Release_NoEncryption"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Ot /Oa /Ow /Oi /Oy /I "./ZipArchive" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Ot /Oa /Ow /Oi /Oy /I "./ZipArchive" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_UNICODE" /D "NO_ENCRYPT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Version.lib ZipArchive.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /pdb:none /machine:I386 /libpath:".\ZipArchive\Unicode_Release"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 Version.lib ZipArchive.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /pdb:none /machine:I386 /libpath:".\ZipArchive\Unicode_Release"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "PatchMaker - Win32 Release"
# Name "PatchMaker - Win32 Debug"
# Name "PatchMaker - Win32 Release_NoEncryption"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddVersionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Config.cpp
# End Source File
# Begin Source File

SOURCE=.\DFXForm.cpp
# End Source File
# Begin Source File

SOURCE=.\ExceptionFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderOp.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PatchEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\PatchMaker.cpp
# End Source File
# Begin Source File

SOURCE=.\PatchMaker.rc
# End Source File
# Begin Source File

SOURCE=.\PatchMakerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressCtrlX.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VDBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\VDFFile.cpp
# End Source File
# Begin Source File

SOURCE=.\VDFNodePool.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddVersionDialog.h
# End Source File
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\DFXForm.h
# End Source File
# Begin Source File

SOURCE=.\Drawgdix.h
# End Source File
# Begin Source File

SOURCE=.\ExceptionFilter.h
# End Source File
# Begin Source File

SOURCE=.\FolderOp.h
# End Source File
# Begin Source File

SOURCE=.\LoginDialog.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\PatchEngine.h
# End Source File
# Begin Source File

SOURCE=.\PatchMaker.h
# End Source File
# Begin Source File

SOURCE=.\PatchMakerDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgressCtrlX.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VDBrowser.h
# End Source File
# Begin Source File

SOURCE=.\VDFFile.h
# End Source File
# Begin Source File

SOURCE=.\VDFNodePool.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\PatchMaker.ico
# End Source File
# Begin Source File

SOURCE=.\res\PatchMaker.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
