; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "modelview.h"
LastPage=0

ClassCount=13
Class1=CMainFrame
Class2=CModelViewApp
Class3=CAboutDlg
Class4=CModelViewDoc
Class5=CModelViewView

ResourceCount=13
Resource1=IDR_MAINFRAME
Resource2=IDD_ILLUSION_SETUP
Resource3=IDD_DIALOG_FRONT_OF_CAMERA
Class6=CSetFrameDlg
Resource4=IDD_ABOUTBOX
Class7=CLightDlg
Resource5=IDD_LIGHT_DIALOG
Resource6=IDD_DIALOG_EFFECTPALETTE
Class8=CSpecularDlg
Resource7=IDD_DIALOG_CREATE_EFFECT
Class9=CIllusionAttachDlg
Resource8=IDD_ILLUSION_ATTACH_DIALOG
Class10=CIllusionSetupDlg
Resource9=IDD_DIALOGBAR
Class11=CBackgroundColorDlg
Resource10=IDD_SPECULAR_DIALOG
Class12=CDlgEffectPalette
Resource11=IDD_SET_BACKGROUND_COLOR
Class13=CDlgCreateEffect
Resource12=IDD_FRAME_SPEED_DIALOG
Resource13=IDR_TOOLBAR

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CMainFrame
Filter=T
VirtualFilter=fWC

[CLS:CModelViewApp]
Type=0
BaseClass=CWinApp
HeaderFile=ModelView.h
ImplementationFile=ModelView.cpp
LastObject=CModelViewApp
Filter=N
VirtualFilter=AC

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ModelView.cpp
ImplementationFile=ModelView.cpp
LastObject=CAboutDlg

[CLS:CModelViewDoc]
Type=0
BaseClass=CDocument
HeaderFile=ModelViewDoc.h
ImplementationFile=ModelViewDoc.cpp

[CLS:CModelViewView]
Type=0
BaseClass=CView
HeaderFile=ModelViewView.h
ImplementationFile=ModelViewView.cpp
Filter=C
VirtualFilter=VWC
LastObject=CModelViewView

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=3
Control1=IDC_STATIC,static,1342308352
Control2=IDOK,button,1342373889
Control3=IDC_STATIC_HAHA,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_OEPN_CH_FILE
Command3=ID_EXPORT_CHR
Command4=ID_OPEN_MOD
Command5=ID_OPEN_ANM
Command6=ID_OPEN_MTL
Command7=ID_CLOSE_MOD
Command8=ID_CLOSE_ANM
Command9=ID_CLOSE_MTL
Command10=ID_OPEN_WAV
Command11=ID_CLOSE_WAV
Command12=ID_WAV_LIST_CLEAR
Command13=ID_APP_EXIT
Command14=IDC_SET_FRAME
Command15=IDC_LIGHT_DIALOG
Command16=ID_SPECULAR_SETTING
Command17=IDC_GRID_DISPLAY
Command18=IDC_BOUNDING_DRAW
Command19=IDC_BONE_BOUNDING_DRAW
Command20=IDM_WIREMODE
Command21=IDM_SET_BACKGROUND_COLOR
Command22=ID_VIEW_EFFECT_PALETTE
Command23=ID_RENDER_CAMERA_INFO
Command24=ID_APP_ABOUT
Command25=IDM_ILLUMINATION_LOAD
Command26=IDM_ILLUMINATION_UNLOAD
Command27=IDM_ILLUMINATION_SETUP
CommandCount=27

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
CommandCount=1

[TB:IDR_TOOLBAR]
Type=1
Class=?
Command1=IDC_COMBO1
Command2=IDC_COMBO2
Command3=IDC_COMBO3
Command4=IDC_COMBO4
Command5=IDC_RESET_CAMERA
Command6=IDC_SELECT_ALL_MOD
Command7=IDC_LIGHT_DIALOG
Command8=IDC_FRONT_OF_CAMERA
Command9=IDC_FRONT_OF_CAMERA_STATIC
CommandCount=9

[DLG:IDD_FRAME_SPEED_DIALOG]
Type=1
Class=CSetFrameDlg
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_FPS,edit,1350631552
Control4=IDC_FPS_OK_BUTTON,button,1342242816

[CLS:CSetFrameDlg]
Type=0
HeaderFile=SetFrameDlg.h
ImplementationFile=SetFrameDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CSetFrameDlg
VirtualFilter=dWC

[DLG:IDD_LIGHT_DIALOG]
Type=1
Class=CLightDlg
ControlCount=22
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDT_DIF_R,edit,1350631552
Control9=IDC_EDT_DIF_G,edit,1350631552
Control10=IDC_EDT_DIF_B,edit,1350631552
Control11=IDC_EDT_AMB_R,edit,1350631552
Control12=IDC_EDT_AMB_G,edit,1350631552
Control13=IDC_EDT_AMB_B,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_DIRECTION_X,edit,1350631552
Control19=IDC_DIRECTION_Z,edit,1350631552
Control20=IDC_DIRECTION_Y,edit,1350631552
Control21=IDC_LIGHT_OK_BUTTON,button,1342242816
Control22=IDC_LIGHT_CHECK,button,1342242819

[CLS:CLightDlg]
Type=0
HeaderFile=LightDlg.h
ImplementationFile=LightDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CLightDlg
VirtualFilter=dWC

[DLG:IDD_DIALOGBAR]
Type=1
Class=CMainFrame
ControlCount=7
Control1=IDC_FRAME_SCROLL,scrollbar,1342177280
Control2=IDC_FRAME_STATIC,static,1342308352
Control3=IDC_PLAY_BUTTON,button,1342242816
Control4=IDC_STOP_BUTTON,button,1342242816
Control5=IDC_PLAY_SOUND_CHECK,button,1342242819
Control6=IDC_EDIT_ALPHA,edit,1350631552
Control7=IDC_STATIC_PICK_OBJNAME,static,1342308352

[DLG:IDD_SPECULAR_DIALOG]
Type=1
Class=CSpecularDlg
ControlCount=5
Control1=IDC_STATIC,static,1342308352
Control2=IDC_SPECULAR_ENABLE,button,1342242819
Control3=IDC_SPEC_SET_OK,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_SPEC_INTENSITY,edit,1350631552

[CLS:CSpecularDlg]
Type=0
HeaderFile=SpecularDlg.h
ImplementationFile=SpecularDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_SPEC_SET_OK
VirtualFilter=dWC

[DLG:IDD_ILLUSION_ATTACH_DIALOG]
Type=1
Class=CIllusionAttachDlg
ControlCount=6
Control1=ID_ILLUMINATION_LOAD_OK,button,1342242817
Control2=ID_ILLUMINATION_DLG_CANCEL,button,1342242816
Control3=IDC_ILLUSION_MOD_LIST,listbox,1352728833
Control4=IDC_ILLUSION_OBJECT_LIST,listbox,1352728833
Control5=IDC_ILLU_DLG_TEXT_MOD,static,1342308352
Control6=IDC_ILLU_DLG_TEXT_OBJECT,static,1342308352

[CLS:CIllusionAttachDlg]
Type=0
HeaderFile=IllusionAttachDlg.h
ImplementationFile=IllusionAttachDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_ILLUMINATION_DLG_CANCEL
VirtualFilter=dWC

[DLG:IDD_ILLUSION_SETUP]
Type=1
Class=CIllusionSetupDlg
ControlCount=3
Control1=IDC_ILLUSION_SETUP_DLG_LENGTH_EDIT,edit,1350631552
Control2=IDC_ILLUSION_SETUP_SHOW_CHECK,button,1342242819
Control3=IDC_ILLUSION_SETUP_LENGTH_STATIC,static,1342308352

[CLS:CIllusionSetupDlg]
Type=0
HeaderFile=IllusionSetupDlg.h
ImplementationFile=IllusionSetupDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_ILLUSION_SETUP_SHOW_CHECK
VirtualFilter=dWC

[DLG:IDD_SET_BACKGROUND_COLOR]
Type=1
Class=CBackgroundColorDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BACKGROUNDCOLOR_R,edit,1350631552
Control4=IDC_BACKGROUNDCOLOR_G,edit,1350631552
Control5=IDC_BACKGROUNDCOLOR_B,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_SET_BACKGROUNDCOLOR_DEFAULT,button,1342242816

[CLS:CBackgroundColorDlg]
Type=0
HeaderFile=BackgroundColorDlg.h
ImplementationFile=BackgroundColorDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CBackgroundColorDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG_EFFECTPALETTE]
Type=1
Class=CDlgEffectPalette
ControlCount=12
Control1=IDC_LIST_EFFECT,listbox,1353777411
Control2=IDC_CHECK_TEXGEN_WAVE,button,1342242819
Control3=IDC_CHECK_DISABLE_SRCTEX,button,1342242819
Control4=IDC_BUTTON_EFFECT_SAVE,button,1342242816
Control5=IDC_BUTTON_EFFECT_LOAD,button,1342242816
Control6=IDC_BUTTON_EFFECT_DELETE,button,1342242816
Control7=IDC_BUTTON_EFFECT_CREATE,button,1342242816
Control8=IDC_BUTTON_UPDATE_EFFECT_PALETTE,button,1342242816
Control9=IDC_STATIC,button,1342177287
Control10=IDC_BUTTON_EFFECT_APPLY,button,1342242816
Control11=IDC_BUTTON_BROWSE_TEXFILE,button,1342242816
Control12=IDC_EDIT_TEX_NAME,edit,1350631552

[CLS:CDlgEffectPalette]
Type=0
HeaderFile=DlgEffectPalette.h
ImplementationFile=DlgEffectPalette.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_TEX_NAME
VirtualFilter=dWC

[DLG:IDD_DIALOG_CREATE_EFFECT]
Type=1
Class=CDlgCreateEffect
ControlCount=2
Control1=IDC_EDIT_EFFECT_NAME,edit,1350631552
Control2=IDC_BUTTON_EFFECT_NAME_OK,button,1342242816

[CLS:CDlgCreateEffect]
Type=0
HeaderFile=DlgCreateEffect.h
ImplementationFile=DlgCreateEffect.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgCreateEffect
VirtualFilter=dWC

[DLG:IDD_DIALOG_FRONT_OF_CAMERA]
Type=1
Class=?
ControlCount=11
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_FOC_CURRENT_OBJECT_STATIC,static,1342308352
Control4=IDC_FOC_BROWSE_BUTTON,button,1342242816
Control5=IDC_FOC_ALPHA_EDIT,edit,1350631552
Control6=IDC_FOC_ALPHA_BUTTON,button,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_FOC_REMOVE_BUTTON,button,1342242816
Control9=IDC_STATIC,static,1342308352
Control10=IDC_FOC_DISTANCE_EDIT,edit,1350631552
Control11=IDC_FOC_DISTANCE_BUTTON,button,1342242816

