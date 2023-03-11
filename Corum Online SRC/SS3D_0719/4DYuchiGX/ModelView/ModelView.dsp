# Microsoft Developer Studio Project File - Name="ModelView" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ModelView - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ModelView.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ModelView.mak" CFG="ModelView - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ModelView - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ModelView - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4DyuchiGX/ModelView", NHCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ModelView - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ModelView - Win32 Debug"

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
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../../4DyuchiGX/4DyuchiGXTools/ModelView.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ModelView - Win32 Release"
# Name "ModelView - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BackgroundColorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEffectPalette.cpp
# End Source File
# Begin Source File

SOURCE=.\EngineRelation.cpp
# End Source File
# Begin Source File

SOURCE=.\FrontOfCameraDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IllusionAttachDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IllusionSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LightDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ListBoxContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelView.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelView.rc
# End Source File
# Begin Source File

SOURCE=.\ModelViewDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelViewView.cpp
# End Source File
# Begin Source File

SOURCE=.\SetFrameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecularDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolInfo.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BackgroundColorDlg.h
# End Source File
# Begin Source File

SOURCE=.\Camera.h
# End Source File
# Begin Source File

SOURCE=.\DialogBar.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateEffect.h
# End Source File
# Begin Source File

SOURCE=.\DlgEffectPalette.h
# End Source File
# Begin Source File

SOURCE=.\EngineRelation.h
# End Source File
# Begin Source File

SOURCE=.\FrontOfCameraDlg.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\GXSCHEDULE_PROC_MSG.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IAudio.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IAudio_GUID.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IExecutive.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IExecutive_GUID.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IGeometry.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IGeometry_GUID.h
# End Source File
# Begin Source File

SOURCE=.\IllusionAttachDlg.h
# End Source File
# Begin Source File

SOURCE=.\IllusionSetupDlg.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IRenderer.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IRenderer_GUID.h
# End Source File
# Begin Source File

SOURCE=.\LightDlg.h
# End Source File
# Begin Source File

SOURCE=.\ListBoxContainer.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\math.inl
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\mesh_flag.h
# End Source File
# Begin Source File

SOURCE=.\ModelView.h
# End Source File
# Begin Source File

SOURCE=.\ModelViewDoc.h
# End Source File
# Begin Source File

SOURCE=.\ModelViewView.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\motion_flag.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SetFrameDlg.h
# End Source File
# Begin Source File

SOURCE=.\SpecularDlg.h
# End Source File
# Begin Source File

SOURCE=D:\4dyuchigx\4DyuchiGRX_common\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolBar.h
# End Source File
# Begin Source File

SOURCE=.\ToolInfo.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\typedef.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ModelView.ico
# End Source File
# Begin Source File

SOURCE=.\res\ModelView.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ModelViewDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Group "EngineHeader"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\DialogBar.asp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
