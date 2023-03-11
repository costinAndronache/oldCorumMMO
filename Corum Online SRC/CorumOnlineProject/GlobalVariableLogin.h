#pragma once

#include "Define.h"
#include "ObjectDefine.h"

#pragma pack(push,1)

struct NETMARBLE_LOGIN
{
	char	szIP[ MAX_IP_LENGTH ];
	DWORD	dwPort;
	BYTE	bLoging;			
	BYTE	bIsTryCorrectLogin;								// ����� ���ڰ� �Ѿ� �Գ�..
	char	szID[ MAX_ID_LENGTH ];
	char	szCode[ 40 ];
	char	szMakePassword[ 40 ];							//���������� ���յ� Password
	char	szCPCookie[ 1024*2 ];							// CP ��Ű ;; �츮 ���ӿ��� �̰͸� �ʿ�

	NETMARBLE_LOGIN()	{ memset(this, 0, sizeof(NETMARBLE_LOGIN));	}
};

#pragma pack(pop)

struct SERVER_SET_INFO
{
	char	szSetName[ MAX_SERVER_SET_NAME_LENGTH ];
	char	szIp[ MAX_IP_LENGTH ];
};

struct WORLD_CONNECT_INFO
{
	DWORD			 dwTotalSetNum;		
	DWORD			 dwCurMouseOverSet;	//���� ���콺 Over �� ��
	SERVER_SET_INFO* pServerInfo;
};

struct CHR_SELECT_INFO
{
	RS_CharacterLogInfo	ReceivedChrInfo;	//���弭���� ���� ���� ��� ĳ���� ���� ���� ����ü
	GXOBJECT_HANDLE		hHandle;			//Client�ʿ� ǥ���� Model Handle
	GXOBJECT_HANDLE		hHeadHandle;		//Client�ʿ� ǥ���� Model head Handle		
	GXOBJECT_HANDLE		hGroundHandle;		//ĳ���� ���� Ground ��� �ڵ�
	GXOBJECT_HANDLE		hSpotLightHandle;	//ĳ���� ���� �Ǿ������� SpotLight �ڵ� 
	GXOBJECT_HANDLE		hHelmetHandle;		// ĳ���� ��� �ڵ�.
	GXOBJECT_HANDLE		hRightHand;			//0228
	GXOBJECT_HANDLE		hLeftHand;			//0228
	LPObjectDesc		pDesc;				//Model Desc
	LPObjectDesc		pDescGround;		//Ground Desc
	LPObjectDesc		pDescSpotLight;		//SpotLight Desc
	BYTE				bByItemType;		//�����ϰ� �ִ� ������ ���� 
	
	LPObjectDesc		pLItemDesc;
	LPObjectDesc		pRItemDesc;
};

typedef struct GlobalVariableLogin
{
	BYTE					bIsLoginInputMode;	
	BYTE					bIntroFlag;										//0�̸� ServerSelect ���1�̸� ������ ����, 2�̸� ��������Ʈ ����������, 
	BYTE					bConnectServerIndex;							//�α����Ҷ� � ������ Connect�Ұ������� ��Ÿ���� ServerSetInfo.pServerInfo[] �� �ε��� ��ȣ 

	WORLD_CONNECT_INFO		ServerSetInfo;		
	CHR_SELECT_INFO			ChrSelectInfo[ MAX_CHAR_SELECT_NUM ];			//ĳ���� ���� ǥ�õǴ� ������
	DWORD					dwCurTotalWorldUser;

	DWORD					dwCurCharIndex;
	GXOBJECT_HANDLE			EnumCharForCreate[ MAX_KIND_OF_CLASS ];			//ĳ���� Create �Ҷ� ������ ��� ������ �� �ڵ� (1�� �ε��� ���� �������)
	GXOBJECT_HANDLE			EnumCharHeadForCreate[ 2 ][ MAX_KIND_OF_HEAD ];	//ĳ���� Create �Ҷ� ������ ��� �Ӹ��ڵ� ���ڿ� 		[����][�Ӹ���ȣ]
	BYTE					bCurClassIndexForCreate;	
	DWORD					dwCurHeadIndexforCreate;						//���ɰ��� ������ ����, ������ �Ӹ���ȣ 
	DWORD					dwFlag;
	BYTE					bIsLoging;										//���� �α��� ����� ��ٸ��� ����. 
	
	OBJECT_HANDLE_DESC*		pCharacterSelectBackGround;
	BOOL					LoadConnectionInfo();
	void					ReleaseConnectionInfo();

	DWORD					GetTotalSetNumber();
	DWORD					GetSelectedSet();
	SERVER_SET_INFO*		GetSelectedSetInfo();
	SERVER_SET_INFO*		GetSetInfo(DWORD dwIndex);
	void					SetCurSet(DWORD dwIndex);	
	
}GLOBAL_VARIABLE_LOGIN, *LPGlobalVariable_Login;