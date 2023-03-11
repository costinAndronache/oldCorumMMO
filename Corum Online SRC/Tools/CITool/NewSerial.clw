; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCopyItemDelete
LastTemplate=CFormView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "NewSerial.h"
LastPage=0

ClassCount=20
Class1=CApp
Class2=CDoc
Class3=CNewSerialView
Class4=CMainFrame
Class7=CMyView

ResourceCount=11
Resource1=IDR_NEWSERTYPE
Resource2=IDD_MYFORM
Resource3=IDD_ABOUTBOX
Class5=CChildFrame
Class6=CAboutDlg
Class8=CUIThread
Resource4=IDR_MAINFRAME
Class9=CFindItemNumDlg
Class10=CGuardianFrame
Resource5=IDD_AUTOPATCH
Class11=CGuardianView
Resource6=IDD_DELETECOPY
Class12=CLoginDlg
Resource7=IDD_COPYITEM
Class13=CCopyItemView
Class14=CCopyItemFrame
Resource8=IDD_ITEM
Class15=CAutoPatch
Resource9=IDD_LOGIN
Class16=CCopyItemDelete
Class17=CCopyItemDeleteFrame
Resource10=IDD_GUARDIAN
Class18=CGuildView
Class19=CGuildFrame
Class20=CGuildInitView
Resource11=IDD_GUILD

[CLS:CApp]
Type=0
HeaderFile=NewSerial.h
ImplementationFile=NewSerial.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CApp

[CLS:CDoc]
Type=0
HeaderFile=Doc.h
ImplementationFile=Doc.cpp
Filter=N

[CLS:CNewSerialView]
Type=0
HeaderFile=NewSerialView.h
ImplementationFile=NewSerialView.cpp
Filter=C


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrame.h
ImplementationFile=MainFrame.cpp
Filter=T
BaseClass=CMDIFrameWnd
VirtualFilter=fWC
LastObject=ID_GUILD_INIT


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrame.h
ImplementationFile=ChildFrame.cpp
Filter=M
BaseClass=CMDIChildWnd
VirtualFilter=mfWC
LastObject=CChildFrame


[CLS:CAboutDlg]
Type=0
HeaderFile=NewSerial.cpp
ImplementationFile=NewSerial.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_NEW_SERIAL
CommandCount=1

[MNU:IDR_NEWSERTYPE]
Type=1
Class=CNewSerialView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_APP_ABOUT
CommandCount=21

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_MYFORM]
Type=1
Class=CMyView
ControlCount=5
Control1=IDC_STATIC,button,1342210055
Control2=IDC_COMBO,combobox,1344274691
Control3=IDC_BUTTON,button,1342177280
Control4=IDC_STATIC,button,1342210055
Control5=IDC_CHECK_NEW_SERIAL,button,1342242819

[CLS:CMyView]
Type=0
HeaderFile=MyView.h
ImplementationFile=MyView.cpp
BaseClass=CFormView
Filter=D
VirtualFilter=VWC
LastObject=CMyView

[CLS:CUIThread]
Type=0
HeaderFile=UIThread.h
ImplementationFile=UIThread.cpp
BaseClass=CWinThread
Filter=N
LastObject=CUIThread
VirtualFilter=TC

[DLG:IDD_ITEM]
Type=1
Class=CFindItemNumDlg
ControlCount=8
Control1=IDC_LIST,SysListView32,1350631429
Control2=IDC_STATIC,button,1342210055
Control3=IDC_EDIT,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_BUTTON,button,1342242816
Control6=IDC_LIST_SELECT,SysListView32,1350631429
Control7=IDC_RIGHT,button,1342242816
Control8=IDC_LEFT,button,1342242816

[CLS:CFindItemNumDlg]
Type=0
HeaderFile=FindItemNumDlg.h
ImplementationFile=FindItemNumDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_RIGHT
VirtualFilter=dWC

[CLS:CGuardianFrame]
Type=0
HeaderFile=GuardianFrame.h
ImplementationFile=GuardianFrame.cpp
BaseClass=CMDIChildWnd
Filter=M
VirtualFilter=mfWC
LastObject=CGuardianFrame

[DLG:IDD_GUARDIAN]
Type=1
Class=CGuardianView
ControlCount=3
Control1=IDC_STATIC,button,1342210055
Control2=IDC_COMBO_GUARDIAN,combobox,1344340227
Control3=IDC_BUTTON_GUARDIAN,button,1342242816

[CLS:CGuardianView]
Type=0
HeaderFile=GuardianView.h
ImplementationFile=GuardianView.cpp
BaseClass=CFormView
Filter=D
VirtualFilter=VWC
LastObject=CGuardianView

[DLG:IDD_LOGIN]
Type=1
Class=CLoginDlg
ControlCount=5
Control1=IDC_STATIC,button,1342210055
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_ID,edit,1350631552
Control5=IDC_EDIT_PW,edit,1350631584

[CLS:CLoginDlg]
Type=0
HeaderFile=LoginDlg.h
ImplementationFile=LoginDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CLoginDlg

[DLG:IDD_COPYITEM]
Type=1
Class=CCopyItemView
ControlCount=3
Control1=IDC_STATIC,button,1342210055
Control2=IDC_COMBO,combobox,1344274691
Control3=IDC_BUTTON,button,1342177280

[CLS:CCopyItemView]
Type=0
HeaderFile=CopyItemView.h
ImplementationFile=CopyItemView.cpp
BaseClass=CFormView
Filter=D
VirtualFilter=VWC
LastObject=IDC_BUTTON

[CLS:CCopyItemFrame]
Type=0
HeaderFile=CopyItemFrame.h
ImplementationFile=CopyItemFrame.cpp
BaseClass=CMDIChildWnd
Filter=M
VirtualFilter=mfWC
LastObject=CCopyItemFrame

[DLG:IDD_AUTOPATCH]
Type=1
Class=CAutoPatch
ControlCount=2
Control1=IDC_PROGRESS,msctls_progress32,1350565888
Control2=IDC_STATIC,static,1342308353

[CLS:CAutoPatch]
Type=0
HeaderFile=AutoPatch.h
ImplementationFile=AutoPatch.cpp
BaseClass=CDialog
Filter=D
LastObject=CAutoPatch
VirtualFilter=dWC

[DLG:IDD_DELETECOPY]
Type=1
Class=CCopyItemDelete
ControlCount=10
Control1=IDC_STATIC,button,1342210055
Control2=IDC_COMBO,combobox,1344274691
Control3=IDC_BUTTON,button,1342177280
Control4=IDC_COMBO_BACKUP,combobox,1210056963
Control5=IDC_STATIC,button,1207992327
Control6=IDC_CHECK,button,1208025091
Control7=IDC_EDIT_CHARNAME,edit,1216413824
Control8=IDC_BUTTON_SKILLINIT,button,1208025088
Control9=IDC_BUTTON_STATINIT,button,1208025088
Control10=IDC_STATIC,button,1207992327

[CLS:CCopyItemDelete]
Type=0
HeaderFile=CopyItemDelete.h
ImplementationFile=CopyItemDelete.cpp
BaseClass=CFormView
Filter=D
VirtualFilter=VWC
LastObject=CCopyItemDelete

[CLS:CCopyItemDeleteFrame]
Type=0
HeaderFile=CopyItemDeleteFrame.h
ImplementationFile=CopyItemDeleteFrame.cpp
BaseClass=CMDIChildWnd
Filter=M
VirtualFilter=mfWC
LastObject=CCopyItemDeleteFrame

[CLS:CGuildView]
Type=0
HeaderFile=GuildView.h
ImplementationFile=GuildView.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CGuildView

[CLS:CGuildFrame]
Type=0
HeaderFile=GuildFrame.h
ImplementationFile=GuildFrame.cpp
BaseClass=CMDIChildWnd
Filter=M
VirtualFilter=mfWC
LastObject=CGuildFrame

[DLG:IDD_GUILD]
Type=1
Class=CGuildInitView
ControlCount=5
Control1=IDC_STATIC,button,1342210055
Control2=IDC_COMBO,combobox,1344274691
Control3=IDC_BUTTON,button,1342177280
Control4=IDC_COMBO_BACKUP,combobox,1344274691
Control5=IDC_STATIC,button,1342210055

[CLS:CGuildInitView]
Type=0
HeaderFile=GuildInitView.h
ImplementationFile=GuildInitView.cpp
BaseClass=CFormView
Filter=D
VirtualFilter=VWC
LastObject=IDC_BUTTON

