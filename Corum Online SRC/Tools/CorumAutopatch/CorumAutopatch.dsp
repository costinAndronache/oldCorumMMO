# Microsoft Developer Studio Project File - Name="CorumAutopatch" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CorumAutopatch - Win32 TESTSERVER
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CorumAutopatch.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CorumAutopatch.mak" CFG="CorumAutopatch - Win32 TESTSERVER"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CorumAutopatch - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CorumAutopatch - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "CorumAutopatch - Win32 CHINA" (based on "Win32 (x86) Application")
!MESSAGE "CorumAutopatch - Win32 TAIWAN" (based on "Win32 (x86) Application")
!MESSAGE "CorumAutopatch - Win32 JAPAN" (based on "Win32 (x86) Application")
!MESSAGE "CorumAutopatch - Win32 TESTSERVER" (based on "Win32 (x86) Application")
!MESSAGE "CorumAutopatch - Win32 TAIWAN_TEST" (based on "Win32 (x86) Application")
!MESSAGE "CorumAutopatch - Win32 TAIWAN_DOWNLOAD_TEST" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Localizing_Common/New_autopatch/CorumAutopatch", KXQAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CorumAutopatch - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 comctl32.lib wininet.lib ZipArchive.lib /nologo /subsystem:windows /machine:I386 /libpath:"./ZipArchive/Release"

!ELSEIF  "$(CFG)" == "CorumAutopatch - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D _WIN32_WINNT=0X0500 /D "_AFXDLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 comctl32.lib /nologo /debug /machine:I386 /out:"WorkDirectory/CorumAutopatch.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CorumAutopatch - Win32 CHINA"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CorumAutopatch___Win32_CHINA"
# PROP BASE Intermediate_Dir "CorumAutopatch___Win32_CHINA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CorumAutopatch___Win32_CHINA"
# PROP Intermediate_Dir "CorumAutopatch___Win32_CHINA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "CHINA_LOCALIZING" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 comctl32.lib wininet.lib ZipArchive.lib /nologo /subsystem:windows /machine:I386 /libpath:"./ZipArchive/Release"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "CorumAutopatch - Win32 TAIWAN"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CorumAutopatch___Win32_TAIWAN"
# PROP BASE Intermediate_Dir "CorumAutopatch___Win32_TAIWAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CorumAutopatch___Win32_TAIWAN"
# PROP Intermediate_Dir "CorumAutopatch___Win32_TAIWAN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "TAIWAN_LOCALIZING" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x404 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 comctl32.lib wininet.lib /nologo /subsystem:windows /machine:I386 /libpath:"./ZipArchive/Release"

!ELSEIF  "$(CFG)" == "CorumAutopatch - Win32 JAPAN"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CorumAutopatch___Win32_JAPAN"
# PROP BASE Intermediate_Dir "CorumAutopatch___Win32_JAPAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CorumAutopatch___Win32_JAPAN"
# PROP Intermediate_Dir "CorumAutopatch___Win32_JAPAN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "JAPAN_LOCALIZING" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 comctl32.lib wininet.lib ZipArchive.lib /nologo /subsystem:windows /machine:I386 /libpath:"./ZipArchive/Release"

!ELSEIF  "$(CFG)" == "CorumAutopatch - Win32 TESTSERVER"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CorumAutopatch___Win32_TESTSERVER"
# PROP BASE Intermediate_Dir "CorumAutopatch___Win32_TESTSERVER"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CorumAutopatch___Win32_TESTSERVER"
# PROP Intermediate_Dir "CorumAutopatch___Win32_TESTSERVER"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D _WIN32_WINNT=0X0500 /D "_AFXDLL" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D _WIN32_WINNT=0X0500 /D "_AFXDLL" /D "TEST_SERVER" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 comctl32.lib /nologo /debug /machine:I386 /out:"WorkDirectory/CorumAutopatch.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 comctl32.lib /nologo /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CorumAutopatch - Win32 TAIWAN_TEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CorumAutopatch___Win32_TAIWAN_TEST"
# PROP BASE Intermediate_Dir "CorumAutopatch___Win32_TAIWAN_TEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CorumAutopatch___Win32_TAIWAN_TEST"
# PROP Intermediate_Dir "CorumAutopatch___Win32_TAIWAN_TEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "TAIWAN_LOCALIZING_TEST" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 comctl32.lib wininet.lib ZipArchive.lib /nologo /subsystem:windows /machine:I386 /libpath:"./ZipArchive/Release"
# ADD LINK32 comctl32.lib wininet.lib ZipArchive.lib /nologo /subsystem:windows /machine:I386 /libpath:"./ZipArchive/Release"

!ELSEIF  "$(CFG)" == "CorumAutopatch - Win32 TAIWAN_DOWNLOAD_TEST"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CorumAutopatch___Win32_TAIWAN_DOWNLOAD_TEST"
# PROP BASE Intermediate_Dir "CorumAutopatch___Win32_TAIWAN_DOWNLOAD_TEST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CorumAutopatch___Win32_TAIWAN_DOWNLOAD_TEST"
# PROP Intermediate_Dir "CorumAutopatch___Win32_TAIWAN_DOWNLOAD_TEST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "TAIWAN_LOCALIZING" /D "_AFXDLL" /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "TAIWAN_LOCALIZING_DOWNLOAD_TEST" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x404 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 comctl32.lib wininet.lib /nologo /subsystem:windows /machine:I386 /libpath:"./ZipArchive/Release"
# ADD LINK32 comctl32.lib wininet.lib /nologo /subsystem:windows /machine:I386 /libpath:"./ZipArchive/Release"

!ENDIF 

# Begin Target

# Name "CorumAutopatch - Win32 Release"
# Name "CorumAutopatch - Win32 Debug"
# Name "CorumAutopatch - Win32 CHINA"
# Name "CorumAutopatch - Win32 TAIWAN"
# Name "CorumAutopatch - Win32 JAPAN"
# Name "CorumAutopatch - Win32 TESTSERVER"
# Name "CorumAutopatch - Win32 TAIWAN_TEST"
# Name "CorumAutopatch - Win32 TAIWAN_DOWNLOAD_TEST"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CorumAutopatch.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CorumAutopatch.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\BackGround.bmp
# End Source File
# Begin Source File

SOURCE=.\Click.bmp
# End Source File
# Begin Source File

SOURCE=.\CorumAutoPatch.rc
# End Source File
# Begin Source File

SOURCE=.\cursor_button.cur
# End Source File
# Begin Source File

SOURCE=.\cursor_normal.cur
# End Source File
# Begin Source File

SOURCE=.\Default.bmp
# End Source File
# Begin Source File

SOURCE=.\Disable.bmp
# End Source File
# Begin Source File

SOURCE=.\Light.bmp
# End Source File
# Begin Source File

SOURCE=.\Pgbar.bmp
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Interface RainFTP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Interface_RainFTP.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface_RainFTP.h
# End Source File
# Begin Source File

SOURCE=.\RainFTP_TypeDefine.h
# End Source File
# End Group
# Begin Group "Entry Point"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# End Group
# Begin Group "Corum patch"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ConstantDefine.h
# End Source File
# Begin Source File

SOURCE=.\CorumPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\CorumPatch.h
# End Source File
# End Group
# Begin Group "GDI componet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Button.cpp
# End Source File
# Begin Source File

SOURCE=.\Button.h
# End Source File
# Begin Source File

SOURCE=.\ProgressView.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressView.h
# End Source File
# Begin Source File

SOURCE=.\StaticView.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticView.h
# End Source File
# Begin Source File

SOURCE=.\TextScroll.cpp
# End Source File
# Begin Source File

SOURCE=.\TextScroll.h
# End Source File
# Begin Source File

SOURCE=.\TextScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TextScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\TextView.cpp
# End Source File
# Begin Source File

SOURCE=.\TextView.h
# End Source File
# End Group
# Begin Group "Server List"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ServerList.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerList.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ZipArchive.lib
# End Source File
# End Target
# End Project
