# Microsoft Developer Studio Project File - Name="4DyuchiGXGFunc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=4DyuchiGXGFunc - Win32 for SSE Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "4DyuchiGXGFunc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "4DyuchiGXGFunc.mak" CFG="4DyuchiGXGFunc - Win32 for SSE Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "4DyuchiGXGFunc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "4DyuchiGXGFunc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "4DyuchiGXGFunc - Win32 for SSE Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "4DyuchiGXGFunc - Win32 for SSE Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4DyuchiGX/4DyuchiGXGFunc", HQCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "4DyuchiGXGFunc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "d:\4dyuchidll"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x68000000" /subsystem:windows /dll /pdb:"\4DyuchiDLL/SS3DGFuncN.pdb" /map:"\4DyuchiDLL/SS3DGFuncN.map" /machine:I386 /out:"\4DyuchiDLL\SS3DGFuncN.dll"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "4DyuchiGXGFunc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "d:\4dyuchidll"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x68000000" /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /out:"c:\windows\system32\SS3DGFunc.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "4DyuchiGXGFunc - Win32 for SSE Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "4DyuchiGXGFunc___Win32_for_SSE_Debug"
# PROP BASE Intermediate_Dir "4DyuchiGXGFunc___Win32_for_SSE_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "d:\4dyuchidll"
# PROP Intermediate_Dir "4DyuchiGXGFunc___Win32_for_SSE_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USE_SSE" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /debugtype:both /machine:I386 /out:"c:\winnt\system32\4DyuchiGXGFunc.dll" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x68000000" /subsystem:windows /dll /map /debug /debugtype:both /machine:I386 /out:"c:\windows\system32\SS3DGFunc.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "4DyuchiGXGFunc - Win32 for SSE Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "4DyuchiGXGFunc___Win32_for_SSE_Release"
# PROP BASE Intermediate_Dir "4DyuchiGXGFunc___Win32_for_SSE_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "d:\4dyuchidll"
# PROP Intermediate_Dir "4DyuchiGXGFunc___Win32_for_SSE_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USE_SSE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"c:\winnt\system32\4DyuchiGXGFunc.dll"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x68000000" /subsystem:windows /dll /pdb:"\4DyuchiDLL/SS3DGFuncSSE.pdb" /map:"\4DyuchiDLL/SS3DGFuncSSE.map" /machine:I386 /out:"\4DyuchiDLL\SS3DGFuncSSE.dll"
# SUBTRACT LINK32 /pdb:none /debug

!ENDIF 

# Begin Target

# Name "4DyuchiGXGFunc - Win32 Release"
# Name "4DyuchiGXGFunc - Win32 Debug"
# Name "4DyuchiGXGFunc - Win32 for SSE Debug"
# Name "4DyuchiGXGFunc - Win32 for SSE Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BinSearchAE.cpp
# End Source File
# Begin Source File

SOURCE=.\create_index.cpp
# End Source File
# Begin Source File

SOURCE=.\DPCQ.cpp
# End Source File
# Begin Source File

SOURCE=.\global.cpp
# End Source File
# Begin Source File

SOURCE=.\Heap.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemIndexTable.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\look_aside_list.cpp
# End Source File
# Begin Source File

SOURCE=.\performance.cpp
# End Source File
# Begin Source File

SOURCE=.\quad_bytes_hash.cpp
# End Source File
# Begin Source File

SOURCE=.\resouce.rc
# End Source File
# Begin Source File

SOURCE=.\sort.cpp
# End Source File
# Begin Source File

SOURCE=.\SortLink.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticResourcePool.cpp
# End Source File
# Begin Source File

SOURCE=.\various_bytes_hash.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BinSearchAE.h
# End Source File
# Begin Source File

SOURCE=.\create_index.h
# End Source File
# Begin Source File

SOURCE=.\DPCQ.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\Heap.h
# End Source File
# Begin Source File

SOURCE=.\ItemIndexTable.h
# End Source File
# Begin Source File

SOURCE=.\ItemQueue.h
# End Source File
# Begin Source File

SOURCE=.\look_aside_list.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\math.inl
# End Source File
# Begin Source File

SOURCE=.\quad_bytes_hash.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StaticResourcePool.h
# End Source File
# Begin Source File

SOURCE=.\type.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\typedef.h
# End Source File
# Begin Source File

SOURCE=.\various_bytes_hash.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
