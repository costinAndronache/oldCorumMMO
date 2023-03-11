#ifndef __USER_H__
#define __USER_H__

#pragma once

#include "../CommonServer/CommonHeader.h"
#include "Linkedlist.h"

#include "ChinaBillingPacket.h"

#ifdef __PACKET_ENCRYPTION
#	include "PacketEncrypt.H"
//#	include "DFXForm.h" // ��� ���� �������� �ȵǳ� ;; �迵�� 2004-12-30
#endif


#define MAX_INV_GUARDIAN			5				
#define MAX_BELT					8
#define MAX_INV_LARGE				21
#define MAX_INV_SMALL				42		
#define	MAX_BANK_SMALL				70
#define	MAX_BANK_LARGE				63

#define EQUIP_TYPE_RHAND1			0		//
#define EQUIP_TYPE_RHAND2			1		
#define EQUIP_TYPE_RIDE				2		//Ż��.. ������ڸ� 
#define EQUIP_TYPE_HELMET			3		//
#define EQUIP_TYPE_MANTLE			4		//����
#define EQUIP_TYPE_LHAND1			5		//
#define EQUIP_TYPE_LHAND2			6		
#define EQUIP_TYPE_RGLOVE			7			
#define EQUIP_TYPE_ARMOR			8		//����
#define EQUIP_TYPE_AMULE1			9
#define EQUIP_TYPE_AMULE2			10
#define EQUIP_TYPE_AMULE3			11
#define EQUIP_TYPE_BELT				12
#define EQUIP_TYPE_LGLOVE			13
#define EQUIP_TYPE_RRING1			14
#define EQUIP_TYPE_RRING2			15
#define EQUIP_TYPE_RRING3			16
#define EQUIP_TYPE_BOOTS			17
#define EQUIP_TYPE_LRING1			18
#define EQUIP_TYPE_LRING2			19
#define EQUIP_TYPE_LRING3			20


#define EVENT_NUM_RECOM_ADD_ITEM	1	//2003�� 10�� 28�Ϻ��� ���۵Ǵ� �̺�Ʈ ��õ�޾� ������ ĳ���Ϳ��� �⺻ ������ ��� �� �־��� 
#define EVENT_NUM_PC_ROOM_EXP_TWICE	2	//2003�� 12�� 31�Ϻ��� ���۵Ǵ� PC�� ���� ����ġ 2�� �̺�Ʈ 

#define SERVER_POS_AGENT			0

#pragma pack(push,1)
struct ITEM_MALL
{
	CItem	sBankItemLarge[MAX_BANK_LARGE];
	CItem	sBankItemSmall[MAX_BANK_SMALL];
};


struct RS_CharacterLogInfo_Without_Equip
{
	DWORD	dwCharacterIndex;								// ĳ���� Index
	char	szName[ MAX_CHARACTER_NAME_LENGTH ];			// ĳ���� �̸� 
	WORD	wChrNum;										// ĳ���� ��ȣ (1 ~ 4)
	WORD	wHead;
	WORD	wClass;											// Ŭ���� 
	WORD	wGrade;											// �׷��̵� 100�̸� GM
	DWORD	dwLevel;										// ���� 
	DWORD	dwExp;											// ����ġ 
	
	DWORD	dwEgo;											// EGO
	DWORD	dwStr;											// STR
	DWORD	dwVit;											// VIT
	DWORD	dwDex;											// DEX.
	DWORD	dwInt;											// INT.

	DWORD	dwHonor;										// ��
	DWORD	dwLuck;											// ���
	DWORD	dwRecentWorldmapID;								//�ֱ� ������ WorldMap ID
};

struct RS_CharacterLogInfo_With_Equip : public RS_CharacterLogInfo_Without_Equip
{
	CItem	Equip[ MAX_EQUIP ];
	BYTE	byCurrentHand;	
	DWORD	dwGuildId;
	BYTE	byRank;	
	BYTE	byInvItemCheck;	
	DBTIMESTAMP	sCreateTime;
};

struct ITEMMALL_BOX
{
	DWORD dwIdx;
	DWORD dwItemID;	//Item ID
	DWORD dwCount;	//Item ����
};

struct ITEMMALL_SUPPLY
{
	DWORD	dwItemMallIndex;
};
#pragma pack(pop)


class CUser
{
public:
	
	DWORD					m_dwPropID;					//Chr_Log_Info ���̺� �ִ� ���� ������ 
	DWORD					m_dwConnectionIndex;		//Server ConnectionIndex;
	DWORD					m_dwStatus;
	DWORD					m_dwCurWorldServerPos;		//������ �ִ� ���� ������ ��ġ(���� ID)
	DWORD					m_dwSerialCode;				//���弭�� ������ ���� SerialCode
	DWORD					m_dwChar_Index;				//������ ������ Character�� ������ 			
	BYTE					m_bSelectedChrIndex;		//
	BYTE					m_bClass;
	BYTE					m_bTryCreateCharSlot;		//�ֱ� ĳ���� ������ �õ��ߴ� ���� ��ȣ 
	char					m_szLoginID[ MAX_ID_LENGTH ];
	char					m_szLoginPassword[ MAX_PASSWORD_LENGTH ];
	DWORD					m_dwKickUserPropID;			//�α��� ��ø�� Kick�� ��� User�� PropID
	BYTE					m_bEventNum;				//�̺�Ʈ�� ��ȣ 
	BYTE					m_bPCRoom;					//PC�� ���� ���� 
	void*					m_pListPos;					
	DWORD					m_dwBillingStartTick;		//�ϳ��� ���ݹ���� ���۵� ������ Tick
	DWORD					m_dwLastBillingCheckTick;	//���������� Billing Check�� TickCount
	BYTE					m_bBillingType;				//���� Ÿ�� 
	DWORD					m_dwRemainTime;				//������ �����ð� 
	DBTIMESTAMP				m_BillingEday;				//���� �����½ð� 
	char					m_szIp[16];					//������ IP Address
	BYTE					m_bItemMallCount;			//�����۸����� ������ ������ ���� 
	ITEMMALL_BOX*			m_pItemMallBox;				//�����۸� �ڽ� ������
	ITEMMALL_SUPPLY*		m_pItemMallSupply;			//�����۸� �Ѱ����� ���޵� ������ ����Ʈ	//kjk(41030)
	WORD					m_wItemMallSupplyCount;		//�����۸� �� ���޵� ����					//kjk(41030)

	SCHINA_BILLING_USER_DATA	m_sChinaBillingData;

	//��� 050323 
	DBTIMESTAMP				m_ID_RegDate; //����������
	DWORD					m_dwCharCount ;


#ifdef __PACKET_ENCRYPTION
private:
	DFXform					m_xform;
#endif

public:
	BYTE					m_byItemMallType;

	BILLING_INFO			m_BillingInfo;				//���� ���� 

	DWORD	GetConnectionIndex()				{ return m_dwConnectionIndex;		}
	void	SetConnectionIndex(DWORD dwIndex)	{ m_dwConnectionIndex = dwIndex;	}
	

	DWORD	GetPropID()							{ return m_dwPropID;				}
	void	SetPropID(DWORD dwID)				{ m_dwPropID = dwID;				}
	
	DWORD	GetIP()								{ return atoi(m_szIp);				}
	LPCSTR	GetIPString()						{ return m_szIp;					}

	LPCSTR	GetUserID()							{ return m_szLoginID;				}
	void	SetUserID(LPCSTR szID)				{ lstrcpy(m_szLoginID, szID);		}

#ifdef __PACKET_ENCRYPTION
	void CUser::SetEncryptKey(LPBYTE pKey)			
	{ 
		m_xform.InitializeBlowfish(pKey, ENCRYPT_KEY_LEN);	
	}

	void CUser::SetBaseEncryptKey()					
	{ 
		m_xform.InitializeBlowfishWithBaseKey();	
	}

	void CUser::Encode(LPBYTE pSrc, WORD wSrcSize, LPBYTE pDes, LPWORD pwDesSize)	
	{
		m_xform.Encode(pSrc, wSrcSize, pDes, pwDesSize);	
	}

	void CUser::Decode(LPBYTE pSrc, WORD wSrcSize, LPBYTE pDes, LPWORD pwDesSize)
	{
		m_xform.Decode(pSrc, wSrcSize, pDes, pwDesSize);	
	}
#endif

	RS_CharacterLogInfo_With_Equip	m_EnumChar[ 4 ];
	RS_CharacterLogInfo_With_Equip*	GetSelectedCharacter()	{	return &m_EnumChar[ m_bSelectedChrIndex ];	}

	CUser*	m_pNextData;
	CUser*	m_pPrevData;

	bool	m_bIsMonitor;				// ����͸� ���ΰ�?

};

#endif 