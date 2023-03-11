//
//	Tool Information Class
//
//			2002.12.12
//			Trust Pak


#include "StdAfx.h"
#include <afxtempl.h>
#include "EngineRelation.h"
#include "Camera.h"


#ifndef _TOOL_INFORMATION_BY_TRUSTPAK_
#define _TOOL_INFORMATION_BY_TRUSTPAK_

#define	DEFAULT_BACKGROUND_COLOR_R	0x3A
#define	DEFAULT_BACKGROUND_COLOR_G	0x6E
#define	DEFAULT_BACKGROUND_COLOR_B	0xA5



/// Constants ////////////////////////////////////////////////////////////////

enum ENUM_CH_FILE_TYPE
{
	CHR_FILE = 0,
	CHX_FILE = 1,
	NOT_CH_FILE = 2
};


const char* const c_szGridInfoFileName	= "ModelView.ini";
const char* const c_szGridFileIndicator = "[GRID_FILE_NAME]";

//----------------------------------------------------------------------------
class CModelViewToolInfo
{
public:
	void	OnLButtonDown(UINT nFlags, CPoint point);

	void SetBackgroundColor( DWORD R, DWORD G, DWORD B);
	CModelViewToolInfo();
	~CModelViewToolInfo();

	void UpdateEffectIndex();
	BOOL ViewEffectPalette();
	BOOL ClearInformation(VOID);

	BOOL AddModFile(char* szFileName);
	BOOL AddAnmFile(char* szFileName);
	BOOL AddMtlFile(char* szFileName);
	BOOL AddWavFile(char* szFileName);

	BOOL RemoveModFile(int iCount);
	BOOL RemoveAnmFile(int iCount);
	BOOL RemoveMtlFile(int iCount);
	BOOL RemoveWavFile(int iCount);

	BOOL AddChrFile(char* szFileName);
	BOOL AddChxFile(char* szFileName);

	BOOL SetCurrentModFile(int iCount);
	BOOL SetCurrentAnmFile(int iCount);	
	BOOL SetCurrentMtlFile(int iCount);
	BOOL SetCurrentWavFile(int iCount);

	BOOL StopAnimation(VOID);	
	BOOL UpdateAnimationInfo(INT iIndex);		// Index starting from Zero.

	// Play sound relation.

	BOOL StopPlaySound(VOID);
	BOOL PlayWav(int iCount);

	BOOL DeleteAllAudio(VOID);

	BOOL ClearAudioInformation(VOID);

	BOOL m_bPlaySound;	

	// Above two functions is called by AddChrFile, AddChxFile routines.
	
	BOOL CreateFileNameListFromChrFile(GXOBJECT_HANDLE hGxObject, char* szChrFileName, char* szDirectory);
	BOOL CreateFileNameListFromChxFile(GXOBJECT_HANDLE hGxObject, char* szChrFileName, char* szDirectory);

	BOOL CreateChFile(char* szFileName, ENUM_CH_FILE_TYPE chType);

	// Grid relation.

	BOOL LoadGridInformation(VOID);
	BOOL ClearGridInformation(VOID);
	BOOL CreateGridInformation(VOID);		// Grid create Order : LoadGrid -> CreateGrid.
											// Becuase, When grid is loaded, IExecutive object is not created yet.

	BOOL DisplayGrid(BOOL bDisplay);

	// Bounding box relation.

	BOOL DisplayBoundingBox(BOOL bDisplay);
	BOOL DisplayBoneBoundingBox(BOOL bDisplay);


	// �ܻ� ���ÿ����� �߰��� �Լ���	2003.09.17
//	BOOL	OnLoadIllusionMesh( GXOBJECT_HANDLE	gxhTarget, I3DModel* pModel, char* szObjectName, char* szIllusionMeshName); 
	BOOL	OnLoadIllusionMesh( GXOBJECT_HANDLE	gxhTarget, char* szObjectName, char* szIllusionMeshName); 
	BOOL	OnUnloadIllusionMesh();
		// attach�� ��� gxobject, ���gxobject�� pModel, pModel���� ������Ʈ �̸�, ���� ���� ���� �ܻ� ������Ʈ.
	BOOL	ApplyIllusionMeshStatus( DWORD dwFrameCount, void* pMtlHandle, BOOL bShow);

	BOOL	CreateIllusionMesh();			// m_Illusion���� �־��� ���ڷ� ������Ʈ�� �����Ѵ�.
	BOOL	DeleteIllusionMesh();			// �ܻ� ������Ʈ�� ���� ��� ������. m_Illusion ���� �ǵ帮�� �ʴ´�.

	struct	TOOL_ILLUSION_MESH_BLOCK
	{
		BOOL			bIllusionMeshBlockSet;				// �ܻ� ������Ʈ�� set �Ǿ��°�??

		GXOBJECT_HANDLE	gxhIllusionMesh;					// �ܻ� ������Ʈ�� �ڵ�.
		char			szIllusionMeshFileName[MAX_PATH];			// �ܻ� ������Ʈ�� �����̸�, �������� �ٲٰų� �ϸ� �ı��ϰ� ���� ��������� ���� �ִ�.
		DWORD			dwIllusionLength;					// �ܻ��� ����.
		void*			phMaterial;							// �ܻ��� ���͸���.

		GXOBJECT_HANDLE	gxhIllusionAttachTarget;			// �ܻ� ������Ʈ�� ����ġ�� gxobject �ڵ�.
		char			szObjectName[MAX_PATH];				// �ܻ��� ���� model ���� Object
	}	m_Illusion;

	// ����.
	DWORD	m_dwBackGroundColorR;
	DWORD	m_dwBackGroundColorG;
	DWORD	m_dwBackGroundColorB;


	// Camera Information.

	BOOL RenderCameraInfo(VOID);

	CModelViewCamera m_Camera;

	INT m_iScrollMin;
	INT m_iScrollMax;
	INT m_iScrollPos;

	BOOL m_bNowPlayed;

	INT m_iCurrentFrame;

	INT m_iTotalModNo;
	INT m_iTotalAnmNo;
	INT m_iTotalMtlNo;
	INT m_iTotalWavNo;

	INT m_iCurrentModNo;	// -1 : Not contain.
	INT m_iCurrentAnmNo;	// -1 : Not contain.
	INT m_iCurrentMtlNo;	// -1 : Not contain.
	INT m_iCurrentWavNo;	// -1 : Not contain.

	INT m_iCurrentGxObject; // Start : zero.
	INT m_iTotalGxObject;	

	// Specular properties.

	BOOL	m_bSpecularEnable;
	FLOAT	m_fSpecularIntensity;

	// Frame per second for Animation.

	DWORD m_dwFps;

	// Light information.

	BYTE m_abyAmbient[3];
	BYTE m_abyDiffuse[3];
	BYTE m_abySpecular[3];
	FLOAT m_afDirection[3];

	BOOL m_bOnLight;
	BOOL m_bRenderCameraInfo;

	CList<CString, CString&> m_lstModFileName;
	CList<CString, CString&> m_lstAnmFileName;
	CList<CString, CString&> m_lstMtlFileName;
	CList<CString, CString&> m_lstWavFileName;

	BOOL		 m_bIsChFile;
	BOOL		 m_bIsAllSelect;

	CList<GXOBJECT_HANDLE, GXOBJECT_HANDLE> m_lstGXObject;	
	CList<GXSOUND_HANDLE, GXSOUND_HANDLE>	m_lstGXAudio;

	GXOBJECT_HANDLE		  m_gxhCurrentRendered;	
	GXSOUND_HANDLE		  m_gxLastSoundHandle;
	CString				  m_strChrFileName;


	// Grid relation.

	char				m_szGridName[1024];
	BOOL				m_bIsGrid;
	GXOBJECT_HANDLE		m_gxhGrid;
	BOOL				m_bDisplayGrid;
	BOOL				m_bDisplayBoundingBox;
	BOOL				m_bDisplayBoneBoundingBox;
	BOOL				m_bWireMode;
	BOOL				m_bViewEffectPalette;

	IDIFontObject*		m_pFont;

	char				m_szProgramPath[1024];

	
	// Camera �տ� attatch �Ǵ� Front Of Camera Object ���� �κ�.

	GXOBJECT_HANDLE		m_gxhFrontOfCamera;
	char				m_szFrontOfCameraName[_MAX_PATH + _MAX_FNAME];
	BYTE				m_byFrontOfCameraAlpah;
	float				m_fFrontOfCameraDistance;

	BOOL				CreateFrontOfCameraObject(char* pszFrontOfCameraFileName);
	BOOL				RemoveFrontOfCameraObject(VOID);

	BOOL				SetFrontOfCameraAlpha(BYTE byAlpha);
	BYTE				GetFrontOfCameraAlpha(VOID);

	BOOL				SetFrontOfCameraDistance(float fDistance);
	float				GetFrontOfCameraDistance(VOID);
};

//----------------------------------------------------------------------------


/// Function proto types /////////////////////////////////////////////////////

ENUM_CH_FILE_TYPE QueryChFileType(const char* szFileName);
BOOL QueryDirectoryString(char* pResult, char* pSource);
BOOL GetPureFileName(char* pResult, const char* pSource, INT iResultSize);
BOOL GetFileExtension(char* pResult, const char* pSource, INT iResultSize);


/// Global variables /////////////////////////////////////////////////////////

extern CModelViewToolInfo g_ToolInfo;


#endif

