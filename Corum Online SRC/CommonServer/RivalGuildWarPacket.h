#ifndef __RIVAL_GUILD_WAR_PACKET__
#define __RIVAL_GUILD_WAR_PACKET__

#include "ProtocolDefinition.h"

#include "stlsupport.h"

#pragma pack(push, 1)

typedef size_t COUNT_TYPE;
typedef DWORD FLAG_TYPE;
//const COUNT_TYPE MAX_RANDOM_TEAM_INDEX = 4;

//����� �÷��� 
const FLAG_TYPE		G_W_F_NONE 		=	0x00000000;
const FLAG_TYPE		G_W_F_OBSERVER	=	0x00000001;
const FLAG_TYPE		G_W_F_PLAYER	=	0x00000010;
const FLAG_TYPE		G_W_F_MASTER 	=	0x00000110;
const FLAG_TYPE		G_W_F_CMP_BASE 	=	0x00000FFF;
//MATCH_TYPE
const FLAG_TYPE		M_T_NONE				= 0x00000000;
const FLAG_TYPE		M_T_DEAH_MATCH			= 0x00000001;
const FLAG_TYPE		M_T_KING_OF_THE_HILL	= 0x00000002;
const FLAG_TYPE		M_T_CAPTURE_THE_FLAG	= 0x00000004;
const FLAG_TYPE		M_T_LEADER_KILL			= 0x00000008;
const FLAG_TYPE		M_T_KILLING_FIELD		= 0x00000010;
const FLAG_TYPE		M_T_ALL					= 0x000000FF;
const FLAG_TYPE		M_T_CMP_BASE			= M_T_ALL;

const COUNT_TYPE	M_T_COUNT				= 5;//M_T ���� �ɶ����� �߰� �Ұ� 

//MATCH_LEVEL
const FLAG_TYPE		M_L_NONE				= 0x00000000;
const FLAG_TYPE		M_L_HIGH				= 0x10000000;
const FLAG_TYPE		M_L_MIDDLE				= 0x20000000;
const FLAG_TYPE		M_L_LOW					= 0x40000000;
const FLAG_TYPE		M_L_CMP_BASE			= 0xF0000000;	//��ġ ������ ALL�� ����.

//TEAM_TYPE
const FLAG_TYPE		T_T_NONE				= 0x00000000;
const FLAG_TYPE		T_T_GUILD				= 0x00000001;//��尣 ���
const FLAG_TYPE		T_T_RANDOM				= 0x00000002;//���� ���
const FLAG_TYPE		T_T_S_GUILD				= 0x00000004;//�����Ϳ� ���� ��� ���
const FLAG_TYPE		T_T_MAX					= 0x0000000F;

const COUNT_TYPE 	MIN_MATCH_TEAM = 2;
const COUNT_TYPE 	MAX_MATCH_TEAM = 8;
const COUNT_TYPE	MAX_STADIUM_SPOT = MAX_MATCH_TEAM;

const int MAX_LADDER_LIST = 15;


const BYTE TMS_EMPTY	= 0;//Team Match State ��忡 ����
const BYTE TMS_PREPARE	= 1;
const BYTE TMS_RUN		= 2;

//���� ��� ��
typedef enum eMatchGuildNumber
{
	MGN_ALL		= 0,
	MGN_TWO		= 1,
	MGN_THREE	= 2,
	MGN_FOUR	= 3,
}MATCH_GUILD_NUMBER;

//����� ����
typedef enum eMatchGuildType
{
	MGT_ALL			= 0,
	MGT_REGULAR		= 1,
	MGT_IRREGULAR	= 2,
	MGT_CHALLENGE	= 3,
}MATCH_GUILD_TYPE;

typedef enum eGuildWarTime
{
	GWT_NONE		= 0,
	GWT_TEN			= 10,
	GWT_FIFTEEN		= 15,
	GWT_EIGHTEEN	= 18,
	GWT_TWENTY		= 20,
}GUILD_WAR_TIME;

typedef enum eGuildWarNumber
{
	GWN_NONE	= 0,
	GWN_SIX		= 6,
	GWN_NINE	= 9,
	GWN_TWELVE	= 12,
	GWN_FIFTEEN = 15,
	GWN_TWENTY	= 20,
}GUILD_WAR_NUMBER;


typedef enum eGuiidWarRequestSequence
{
	GWRS_NONE =			0,
	GWRS_REQUEST =		1,
	GWRS_REREQUEST =	2,
	GWRS_APPLY =		3,
	GWRS_REJECT =		4,
	GWRS_END	=		5,
}GUILD_WAR_REQUEST_SEQUENCE;

typedef struct tagTeamMatchCondition
{
	int iMatchType;
	int iMatchGuildNumber;
	int iMatchGuildType;
}TEAM_MATCH_CONDITION;

typedef struct tagGuildLadderData
{
	bool	bIsUse;										// ��밡�� �÷���
	int		iIndex;		
	int		iRank[3];									// ��ü��忡���� ��������
	int		iGuildTotalCount;							// ���� �ѿ�
	char	szGuildMaster[MAX_CHARACTER_NAME_LENGTH];	// ��� ������ �̸�
	char	szGuildName[MAX_GUILD_NAME_LENGTH];			// ��� �̸�
	int		iTotalPoint;								// ��������� ��������
	int		WeekPoint[3];								// �̹����� ����
	int		MonthPoint[3];								// �Ѵް� ����
}GUILD_LADDER_DATA;

typedef struct tagGuildWarData
{
	size_t	match_type;//��������
	size_t	team_type;//�� �з�
	size_t	match_level;//���
	size_t	player_count;//������ �����
	WORD	wDungeonID;
	BYTE	byLayerIndex;

	GUILD_LADDER_DATA aGLD[MAX_MATCH_TEAM];
}GUILD_WAR_DATA;

typedef struct tagGuildWarDataList
{	
	size_t						cur_page;//��û���� ������.
	size_t						total_page;//�� ��������
	size_t						count_per_page;//�ش� �������� ǥ�õ� ����
	
	GUILD_WAR_DATA				aGuildWarData[MAX_LADDER_LIST];//����� ����Ʈ
}GUILD_WAR_DATA_LIST;

typedef struct tagGuildWarListPage
{
	int iPageNumber;
}GUILD_WAR_LIST_PAGE;

typedef struct tagGuildWarObserve
{
	WORD		wDungeonID;
	BYTE		byLayerIndex;
	FLAG_TYPE	match_type;

	int iObserveIndex;
}GUILD_WAR_OBSERVE;

typedef struct tagGuildWarRequest
{
	char szCasterName[MAX_CHARACTER_NAME_LENGTH];//��û�� ��� �̸�
	char szTargetName[MAX_CHARACTER_NAME_LENGTH];//��û�� ��� �̸�
	GUILD_WAR_REQUEST_SEQUENCE	Sequence;//��û ����(��û, ������û, ����, ����)
	FLAG_TYPE					MatchType;//�����
	GUILD_WAR_TIME				MatchTime;//���ð�
	GUILD_WAR_NUMBER			MatchNumber;//����ο�
	GUILD_LADDER_DATA			LadderData;//�������� Ŭ���̾�Ʈ�� ������ ����
}GUILD_WAR_REQUEST;

typedef struct tagGuildWarInfo
{
	BYTE byState;
	FLAG_TYPE match_type;
	FLAG_TYPE match_level;
	GUILD_WAR_TIME match_time;
	COUNT_TYPE team_count;
	COUNT_TYPE player_count;
	
	FLAG_TYPE a_team_type[MAX_MATCH_TEAM];
	size_t	a_point[MAX_MATCH_TEAM];
	DWORD	adwGuildID[MAX_MATCH_TEAM];
	char	aszGuildName[MAX_MATCH_TEAM][MAX_GUILD_NAME_LENGTH];
	bool	a_bIsUse[MAX_MATCH_TEAM];
	
	size_t winnder_Index;
}GUILD_WAR_INFO;

typedef enum eTeamMatchPacketType
{
	CTWS_TM_INSERT						= 1,//�ڵ���Ī ��û
	CTWS_TM_REMOVE						= 2,//�ڵ���Ī ����
	CTWS_TM_LADDER_LIST					= 3,//��巩ŷ ����Ʈ
	CTWS_TM_GUILD_WAR_LIST				= 4,//����� ����Ʈ
	CTWS_TM_GUILD_WAR_REQUEST			= 5,//����� ��û
	CTWS_TM_GUILD_WAR_OBSERVE			= 6,//�������
	CTWS_TM_GUILD_WAR_OBSERVE_CANCEL	= 7,//������� ���

	WSTC_TM_GUILD_WAR_INFO				= 8,//��� ���� ���
}TEAM_MATCH_PACKET_TYPE;

struct GuildLadderInfo
{
	TCHAR	m_szGuildName[20];
	TCHAR	m_szGuildMasterName[20];
	BYTE	m_uGuildMemberCount;

    USHORT	m_uWeekRank;
	USHORT	m_uWeekPoint;
	USHORT	m_uJuniorWeekPoint;
	USHORT	m_uSeniorWeekPoint;

	USHORT	m_uQuaterRank;
	UINT	m_uQuaterPoint;
	UINT	m_uJuniorQuaterPoint;
	UINT	m_uSeniorQuaterPoint;
};

struct GuildLadderInfoPage
{
	IN	UINT	m_uCurPage;
	OUT	UINT	m_uMaxPage;
	OUT UINT	m_uUseCount;
	
	OUT TCHAR	m_szMyGuildName[20];
	OUT UINT	m_uMyGuildLadderRank;
	OUT UINT	m_uMyGuildLadderPoint;	

	OUT GuildLadderInfo	m_GuildLadderInfo[15];
};

typedef struct tagTEAM_MATCH_PACKET
	:	public WORLD_PACKET_HEADER
{
	tagTEAM_MATCH_PACKET()
	{
		bGameStatus = UPDATE_GAME_WORLD;	
		bHeader		= Protocol_World::CMD_TEAM_MATCH;		
	}
	DWORD	GetPacketSize()	
	{
		const size_t base_size = sizeof(*this) - sizeof(Body);

		switch(wType)
		{
		case CTWS_TM_INSERT:
		case CTWS_TM_REMOVE:
			{
				return base_size+sizeof(TEAM_MATCH_CONDITION);
			}break;
		case CTWS_TM_LADDER_LIST:
			{
				return base_size+sizeof(GuildLadderInfoPage);
			}break;
		case CTWS_TM_GUILD_WAR_LIST:
			{
				return base_size+sizeof(GUILD_WAR_DATA_LIST);
			}break;
		case CTWS_TM_GUILD_WAR_REQUEST:
			{
				return base_size+sizeof(GUILD_WAR_REQUEST);
			}break;
		case CTWS_TM_GUILD_WAR_OBSERVE:
			{
				return base_size+sizeof(GUILD_WAR_OBSERVE);
			}break;
		case WSTC_TM_GUILD_WAR_INFO:
			{
				return base_size+sizeof(GUILD_WAR_INFO);
			}break;
		default:
			{
				return sizeof(*this);
			}
		}
	}

	WORD	wType;//TEAM_MATCH_PACKET_TYPE ����

	union								// �츮�� ��� �ε����� Ư���Ҷ��� ���⶧����, ���� �����ε����� �������� ����
	{
		TEAM_MATCH_CONDITION		Condtion;	//��Ī ��û���� ����
//		GUILD_LADDER_DATA			LadderList[MAX_LADDER_LIST];//��ŷ ����Ʈ
		GUILD_WAR_DATA_LIST			GuildWarList;//����� ����Ʈ

		GUILD_WAR_LIST_PAGE			PageNum;	//��û ������
		GUILD_WAR_OBSERVE			ObserveIndex; //������ ����� �ε���
		GUILD_WAR_REQUEST			GuildWarRequest;//����� ��û
		GUILD_WAR_INFO				GuildWarInfo;
		GuildLadderInfoPage			m_GuildLadderInfoPage;
	}Body;

}TEAM_MATCH_PACKET;

/////////////////////////////////////////////////////////////////////////////////////////////////
// ������
typedef struct tagGUILD_MATCHBEGIN_MEMBER
{
	char	szName[MAX_CHARACTER_NAME_LENGTH];	// ��� �̸�
	BYTE	byClass;	// ��� Ŭ����
	BYTE	byCalled;	// �̹� ȣ��ǰų�, ȣ���� �����Ǵ� ����ΰ�? TRUE�̸� ȣ����.
	WORD	wLevel;		// ��� ����
} GUILD_MATCHBEGIN_MEMBER;

const size_t MAX_MEMBER_LIST = 20;

typedef struct tagGUILD_MATCHBEGIN_MEMBER_LIST
{
	WORD					m_wListSize;				// ����� 
	WORD					m_wIsEnd;
	GUILD_MATCHBEGIN_MEMBER	m_aMemberList[MAX_MEMBER_LIST];
} GUILD_MATCHBEGIN_MEMBER_LIST;

typedef struct tagGUILD_MATCHBEGIN_LADDER_INFO
{
	FLAG_TYPE	a_team_type[MAX_MATCH_TEAM];
	char	m_szGuildName[MAX_MATCH_TEAM][MAX_GUILD_NAME_LENGTH];				// ��� �̸�
	bool	a_bIsUse[MAX_MATCH_TEAM];
	DWORD	m_dwLadderGrade[MAX_MATCH_TEAM];					// ���� ���
	DWORD	m_dwLadderPoint[MAX_MATCH_TEAM];					// ���� ����
	DWORD	m_dwMaxMemberCount;					// ��� �ִ��ο�
	DWORD	m_dwEnemyWaitMemberAverageLevel;	// ����� ��ü����� ��շ���
	DWORD	m_dwEnemyWaitMemberCount;			// ����� ��ü����� ��
	
	DWORD	dwRemainBeginTime;
	DWORD	dwRemainEndTime;
} GUILD_MATCHBEGIN_LADDER_INFO;

typedef struct tagGUILD_MATCHBEGIN_USER_CALL
{
	char	szName[MAX_CHARACTER_NAME_LENGTH];
} GUILD_MATCHBEGIN_USER_CALL;

typedef enum	eGMS_TYPE
{
	GMS_TYPE_MEMBER_LIST = 0,		// �������Ʈ�� ���ƿ�
	GMS_TYPE_LADDER_INFO,			// ���ŵ� ���������� ���ƿ�
	GMS_TYPE_CANCEL,				// ��ҵǾ���
	GMS_TYPE_REQUEST_MEMBER_LIST,	// �������Ʈ�� �޶�.
    GMS_TYPE_REQUEST_LADDER_INFO,	// ���������� �޶�.
	GMS_TYPE_REQUEST_USER_CALL,		// �� ������ ȣ���ش޶�.
	GMS_TYPE_REQUEST_USER_KICK,		// �� ������ ���������
	GMS_TYPE_REQUEST_SET_READY,		// ���� ��
	GMS_TYPE_REQUEST_RESET_READY,	// ���� ����
	GMS_TYPE_REQUEST_CANCEL,		// ��⸦ ���
	GMS_TYPE_REQUEST_INVITATION		// ������� �ʴ�޼��� ����
}GMS_TYPE;

typedef struct tagGUILD_MATCHBEGIN_SETTING
			: public WORLD_PACKET_HEADER
{
	tagGUILD_MATCHBEGIN_SETTING()
	{
		bGameStatus = UPDATE_GAME_WORLD;
		bHeader = Protocol_World::CMD_GUILD_MATCHBEGIN_SETTING;
	}

	DWORD	GetPacketSize() 
	{
		return sizeof(*this);
	}

	GMS_TYPE eType;

	union
	{
        GUILD_MATCHBEGIN_MEMBER_LIST	m_MemberList;
		GUILD_MATCHBEGIN_LADDER_INFO	m_LadderInfo;
		GUILD_MATCHBEGIN_USER_CALL		m_UserCall;
	} Body;

} GUILD_MATCHBEGIN_SETTING;
// ������
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// ������
typedef struct tagGUILD_MATCH_GUILDINFO
{
	BYTE	byState;
	DWORD	dwSize;
	DWORD	dwRemainBeginTime;
	DWORD	dwRemainEndTime;
	size_t	a_point[MAX_MATCH_TEAM];
	bool	a_bIsUse[MAX_MATCH_TEAM];
} GUILD_MATCH_GUILDINFO;

typedef struct tagGUILD_MATCH_STATUS
			: public WORLD_PACKET_HEADER
{
	tagGUILD_MATCH_STATUS()
	{
		bGameStatus = UPDATE_GAME_WORLD;
		bHeader		= Protocol_World::CMD_GUILD_MATCH_STATUS;
	}

	DWORD	GetPacketSize() 
	{
		return sizeof(*this);
	}

	GUILD_MATCH_GUILDINFO	m_GuildInfo;

} GUILD_MATCH_STATUS;

typedef struct tagGUILD_MATCH_STATUS_REQUEST
			: public WORLD_PACKET_HEADER
{
	tagGUILD_MATCH_STATUS_REQUEST()
	{
		bGameStatus = UPDATE_GAME_WORLD;
		bHeader = Protocol_World::CMD_GUILD_MATCH_STATUS_REQUEST;
	}

	DWORD	GetPacketSize() const
	{
		return sizeof(*this);
	}

	size_t	match_type;
	DWORD	dwGuildWarFlag;
	WORD	wDungeonID;
	BYTE	byLayerIndex;	
} GUILD_MATCH_STATUS_REQUEST;
			
typedef struct tagGUILD_MATCH_STATUS_RESULT
			: public WORLD_PACKET_HEADER
{
	tagGUILD_MATCH_STATUS_RESULT()
	{
		bGameStatus = UPDATE_GAME_WORLD;
		bHeader		= Protocol_World::CMD_GUILD_MATCH_STATUS_RESPON;
	}

	DWORD	GetPacketSize() 
	{
		return sizeof(*this);
	}

	DWORD	dwSize;
	DWORD	dwRemainBeginTime;
	DWORD	dwRemainEndTime;
	char	szName[MAX_MATCH_TEAM][MAX_GUILD_NAME_LENGTH];	
	size_t	a_point[MAX_MATCH_TEAM];
	bool	a_bIsUse[MAX_MATCH_TEAM];
} GUILD_MATCH_STATUS_RESULT;

typedef struct tagGUILD_MATCH_CANCEL_OBSERVER : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;

	DWORD	GetPacketSize()
	{
		return sizeof(*this);
	}
	tagGUILD_MATCH_CANCEL_OBSERVER()
	{
		bGameStatus = UPDATE_GAME_WORLD;
		bHeader = Protocol_World::CMD_GUILD_MATCH_CANCEL_OBSERVER;
	}
} GUILD_MATCH_CANCEL_OBSERVER;

// ������
////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct tagTEAM_MATCH_PACKET_SERVER
{
	tagTEAM_MATCH_PACKET_SERVER()
	{
//		wHeader	= Protocol_Server::CMD_TEAM_MATCH;		
	}
	DWORD	GetPacketSize()	{	return sizeof(*this);	}	

	WORD	wHeader;
	WORD	wType;//TEAM_MATCH_PACKET_TYPE ����

	union								// �츮�� ��� �ε����� Ư���Ҷ��� ���⶧����, ���� �����ε����� �������� ����
	{
//		TEAM_MATCH_CONDITION		Condtion;	//��û �� ����
//		GUILD_LADDER_DATA			LadderList[MAX_LADDER_LIST];
//		GUILD_WAR_DATA				GuildWarList[MAX_LADDER_LIST];
	}Body;
}TEAM_MATCH_PACKET_SERVER;

class CMatchCondition
{
public:
	CMatchCondition(const FLAG_TYPE &dwCommonFlag = 0)
	{
		Clear();
		MatchType( dwCommonFlag & M_T_CMP_BASE );
		MatchLevel( dwCommonFlag & M_L_CMP_BASE );
	}

	virtual ~CMatchCondition()
	{
		Clear();
	}

public:
	FLAG_TYPE MatchType() const 
	{
		return match_type;
	}

	void MatchType(const FLAG_TYPE dwFlag) 
	{
		match_type = dwFlag;
	}

	FLAG_TYPE MatchLevel() const 
	{
		return match_level;
	}

	void MatchLevel(const FLAG_TYPE dwFalg) 
	{
		match_level = dwFalg;
	}

	FLAG_TYPE CommonFlag() const
	{
		return match_type | match_level;
	}

	void Clear()
	{
		MatchType(0);
		MatchLevel(0);
	}
public:
	bool operator < (const CMatchCondition& rRight) const
	{	
		if( CommonFlag() < rRight.CommonFlag() )
		{
			return true;
		}
		return false;
	}

	bool operator == (const CMatchCondition& rRight) const
	{	
		if( CommonFlag() == rRight.CommonFlag() )
		{
			return true;
		}
		return false;
	}

private:
	FLAG_TYPE match_type;
	FLAG_TYPE match_level;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CTeamInfo
{
public:
	CTeamInfo(const FLAG_TYPE dwFlag = 0)
	{
		Clear();
		m_dwTeamFlag = dwFlag;
	}

	virtual ~CTeamInfo()
	{
		Clear();
	}

public:
	int TeamIndex() const {return m_iTeamIndex;}
	void TeamIndex(const int iIndex) {m_iTeamIndex = iIndex;}

	const string& TeamName() const {return m_strTeamName;}
	void TeamName(const string &rstrName) {m_strTeamName = rstrName;}

	const FLAG_TYPE& TeamType() const {return m_dwTeamFlag;}
	void TeamType(const FLAG_TYPE dwFlag) {m_dwTeamFlag = dwFlag;}
	
public:
	bool operator < (const CTeamInfo& rRight) const
	{
		if( this->TeamIndex() < rRight.TeamIndex() )//������ ���ų�
		{
			return true;
		} 
		
		if( rRight.TeamIndex() < this->TeamIndex() )
		{
			return false;
		}
		
		//�� �ε��� �� ���ٸ�
		if( this->TeamType()  < rRight.TeamType() )//��Ÿ���� ����?
		{
			return true;
		}
		
		if( rRight.TeamType()  < this->TeamType() )
		{
			return false;
		}
		return false;
	}

	bool operator == (const CTeamInfo& rRight) const
	{	
		if(	this->TeamIndex() == rRight.TeamIndex()
		&&	this->TeamType()  == rRight.TeamType() )
		{
			return true;
		}
		return false;
	}

	void Clear()
	{
		TeamIndex(0);
		TeamName("");
		TeamType(0);
	}
private:
	int			m_iTeamIndex;
	string		m_strTeamName;//name���δ� ã�� ����

	FLAG_TYPE	m_dwTeamFlag;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < typename T_FIRST, typename T_SECOND >// �������� �Ϸ��� SECOND�� CCondition�� �� �־��ش�
class CCondition
{
public:
	typedef T_FIRST		FIRST;
	typedef T_SECOND	SECOND;
public:
	CCondition(){}
	~CCondition(){}

public:
	bool operator < (const CCondition& rRight) const//map ���� first�� �Ǳ����� ����
	{	
		if( this->t_First < rRight.t_First)//������ ���ų�
		{
			return true;
		} 
		
		if( rRight.t_First < this->t_First)
		{
			return false;
		}
		
		//First �� ���ٸ�
		if( this->t_Second < rRight.t_Second )//���� ���ٸ�
		{
			return true;
		}
		
		if( rRight.t_Second < this->t_Second )//���� ���ٸ�
		{
			return false;
		}
			
		return false;
	}

	bool operator == (const CCondition& rRight) const
	{	
		if( ( this->t_First == rRight.t_First)
		&& 	( this->t_Second == rRight.t_Second) )
		{
			return true;
		}
		return false;
	}



	void	First(const T_FIRST& T_rFirst) {t_First = T_rFirst;}
	void	Second(const T_SECOND& T_rSecond) {t_Second = T_rSecond;}

	T_FIRST&	First() {return t_First;}
	T_SECOND&	Second() {return t_Second;}

	void Clear()
	{
		t_First.Clear();
		t_Second.Clear();
	}
private:
	T_FIRST t_First;
	T_SECOND t_Second;
};

typedef CCondition<  CMatchCondition , CTeamInfo >	G_MC_DEFAULT;

typedef struct tagStadiumData
{
	WORD		wDungeonID;
	BYTE		byLayerIndex;
	FLAG_TYPE	match_type;
	
	FLAG_TYPE	match_level;

	COUNT_TYPE	player_count;

	BYTE		byState;
	COUNT_TYPE	team_count;

	GUILD_WAR_TIME match_time;

	DWORD		dwRemainBeginTime;
	DWORD		dwRemainEndTime;

	POINT		aPos[MAX_STADIUM_SPOT];

	bool		bIsAutoMatch;

	bool operator==(const tagStadiumData &right)
	{
		if( wDungeonID == right.wDungeonID
		&& byLayerIndex == right.byLayerIndex
		&& match_type == right.match_type)
		{
			return true;
		}
		return false;
//		bCanUse == right.;//Canuse�� �����������̱� ������ �񱳽ô� ���� �ʴ´�.
	}
}STADIUM_DATA, *LPSTADIUM_DATA;

typedef struct tagStadiumTeamPoint
{
	WORD		wDungeonID;
	BYTE		byLayerIndex;
	FLAG_TYPE	match_type;
	COUNT_TYPE	team_index;
	size_t		point;

	DWORD		dwRemainBeginTime;
	DWORD		dwRemainEndTime;
}STADIUM_TEAM_POINT;

typedef struct tagGUILD_WAR_INVITE
	:	public WORLD_PACKET_HEADER
{
	tagGUILD_WAR_INVITE()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_WAR_INVITE;	}
	DWORD	GetPacketSize()		{	return (sizeof(tagGUILD_WAR_INVITE)); }
}GUILD_WAR_INVITE_PACKET, *LPGUILD_WAR_INVITE_PACKET;

typedef struct tagGUILD_WAR_BBS
	:	public WORLD_PACKET_HEADER
{
	tagGUILD_WAR_BBS()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_WAR_BBS;	}
	DWORD	GetPacketSize()		{	return (sizeof(tagGUILD_WAR_INVITE)); }
}GUILD_WAR_BBS, *LPGUILD_WAR_BBS;


#pragma pack(pop, 1)

#endif	//	__RIVAL_GUILD_WAR_PACKET__
