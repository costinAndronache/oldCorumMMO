//======================================================//
// Code by Jang.							2003.07.21	// 
// Obejct : Group Window.								//
//======================================================//
#ifndef		__GROUP_INC__
#define		__GROUP_INC__

#include	"Menu.h"

#define SPR_OBJ_GROUPWND				0
#define SPR_OBJ_GROUPWND_CLOSE1			1	
#define SPR_OBJ_GROUPWND_CLOSE2			2
#define SPR_OBJ_GROUPWND_PARTY1			3
#define SPR_OBJ_GROUPWND_PARTY2			4
#define SPR_OBJ_GROUPWND_GUILD1			5
#define SPR_OBJ_GROUPWND_GUILD2			6
#define SPR_OBJ_GROUPWND_FRIEND1		7
#define SPR_OBJ_GROUPWND_FRIEND2		8
#define SPR_OBJ_GROUPWND_GUILDWAR1		9
#define SPR_OBJ_GROUPWND_GUILDWAR2		10
#define SPR_OBJ_GROUPWND_ADD1			11
#define SPR_OBJ_GROUPWND_ADD2			12
#define SPR_OBJ_GROUPWND_ADD3			13
#define SPR_OBJ_GROUPWND_DELETE1		14
#define SPR_OBJ_GROUPWND_DELETE2		15
#define SPR_OBJ_GROUPWND_DELETE3		16
#define SPR_OBJ_GROUPWND_INFO1			17
#define SPR_OBJ_GROUPWND_INFO2			18
#define SPR_OBJ_GROUPWND_INFO3			19
#define SPR_OBJ_GROUPWND_OUT1			20
#define SPR_OBJ_GROUPWND_OUT2			21
#define SPR_OBJ_GROUPWND_OUT3			22
#define SPR_OBJ_GROUPWND_MSG1			23
#define SPR_OBJ_GROUPWND_MSG2			24
#define SPR_OBJ_GROUPWND_MSG3			25
#define SPR_OBJ_GROUPWND_SELECT			26
#define SPR_OBJ_GROUPWND_GUILDWARADD1	27
#define SPR_OBJ_GROUPWND_GUILDWARADD2	28
#define SPR_OBJ_GROUPWND_GUILDWARADD3	29
#define SPR_OBJ_GROUPWND_GUILDWAREXIT1	30
#define SPR_OBJ_GROUPWND_GUILDWAREXIT2	31
#define SPR_OBJ_GROUPWND_GUILDWAREXIT3	32
#define SPR_OBJ_GUILD_PAGE				33
#define SPR_OBJ_GUILD_PAGEPREV1			34
#define SPR_OBJ_GUILD_PAGEPREV2			35
#define SPR_OBJ_GUILD_PAGEPREV3			36
#define SPR_OBJ_GUILD_PAGENEXT1			37
#define SPR_OBJ_GUILD_PAGENEXT2			38
#define SPR_OBJ_GUILD_PAGENEXT3			39
#define SPR_OBJ_GUILD_ONOFF1			40
#define SPR_OBJ_GUILD_ONOFF2			41
#define SPR_OBJ_GUILD_ONOFF3			42
#define SPR_OBJ_GUILD_INFO1				43
#define SPR_OBJ_GUILD_INFO2				44
#define SPR_OBJ_GUILD_INFO3				45
#define SPR_OBJ_GUILD_LEVEL1			46
#define SPR_OBJ_GUILD_LEVEL2			47
#define SPR_OBJ_GUILD_LEVEL3			48
#define SPR_OBJ_GUILD_LEVEL4			49
#define SPR_OBJ_GUILD_MARK				50
#define SPR_OBJ_GROUPWND_STATUS			51
#define SPR_OBJ_GROUP_MASTER1			52
#define SPR_OBJ_GROUP_MASTER2			53
#define SPR_OBJ_GROUP_MASTER3			54
#define SPR_OBJ_GUILDWAR_GUILD			55
#define SPR_OBJ_GROUPWND_PARTYBAR1		56
#define SPR_OBJ_GROUPWND_PARTYBAR2		57
#define SPR_OBJ_GROUPWND_PARTYBAR3		58
#define SPR_OBJ_GROUPWND_PARTYBAR4		59
#define SPR_OBJ_GROUPWND_PARTYBAR5		60
#define SPR_OBJ_GROUPWND_PARTYBAR6		61
#define SPR_OBJ_GROUPWND_PARTYBAR7		62
#define SPR_OBJ_GROUPWND_PARTYGAGE1		63
#define SPR_OBJ_GROUPWND_PARTYGAGE2		64
#define SPR_OBJ_GROUPWND_PARTYGAGE3		65
#define SPR_OBJ_GROUPWND_PARTYGAGE4		66
#define SPR_OBJ_GROUPWND_PARTYGAGE5		67
#define SPR_OBJ_GROUPWND_PARTYGAGE6		68
#define SPR_OBJ_GROUPWND_PARTYGAGE7		69
#define SPR_OBJ_GROUPWND_PBCREATE1		70
#define SPR_OBJ_GROUPWND_PBCREATE2		71
#define SPR_OBJ_GROUPWND_PBCREATE3		72
#define SPR_OBJ_GROUPWND_PBDESTROY1		73
#define SPR_OBJ_GROUPWND_PBDESTROY2		74
#define SPR_OBJ_GROUPWND_PBDESTROY3		75

//��Ƽ ��ȭ �ý��� (����â 2005.01.25)
#define SPR_OBJ_GROUPWND_PARTYUSERINFO1					76      //  ��Ƽâ 6��° ��ư ��Ȱ��ȭ (������) 
#define SPR_OBJ_GROUPWND_PARTYUSERINFO2					77		//	��Ƽâ 6��° ��ư Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_PARTYUSERINFO3					78		//	��Ƽâ 6��° ��ư ��ư Ŭ���� 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITION1				79		//  ��Ƽâ 7��° ��ư ��Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITION2				80		//	��Ƽâ 7��° ��ư Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITION3				81		//	��Ƽâ 7��° ��ư ��ư Ŭ���� 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION1				82		//	��Ƽâ 8��° ��ư ��Ȱ��ȭ (������)
#define SPR_OBJ_GROUPWND_PMATCHCONDITION2				83		//	��Ƽâ 8��° ��ư Ȱ��ȭ	
#define SPR_OBJ_GROUPWND_PMATCHCONDITION3				84		//	��Ƽâ 8��° ��ư ��ư Ŭ���� 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF			85		//	��Ī�ý��ۼ��� �� Ȯ�� ��ư (��Ȱ��ȭ)
#define SPR_OBJ_GROUPWND_PARTYFRIENDADD					86		//	��Ƽ �� ģ���߰� ��ư 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1				87		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ������
#define SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2				88		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��� 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3				89		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4				90		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư Ŭ�� 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5				91		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư ��Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6				92		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ���� 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1			93		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ������
#define SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2			94		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��� 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3			95		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4			96		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư Ŭ�� 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5			97		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư ��Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6			98		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ���� 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1             99      //  Ȯ�� ��ư ��Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK2				100     //  Ȯ�� ��ư Ȱ��ȭ  
#define SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK3				101     //  Ȯ�� ��ư �������� 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1			102     //  ��� ��ư ��Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL2			103		//  ��� ��ư Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL3			104     //  ��� ��ư �������� 
//#define SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1           105     //  �˻��� ��Ƽ�� ���� ���� Ȯ�� ��ư ��Ȱ��ȭ 
//#define SPR_OBJ_GROUPWND_PMATCHCONDITION_OK2			106     //  �˻��� ��Ƽ�� ���� ����Ȯ�� ��ư Ȱ��ȭ  
//#define SPR_OBJ_GROUPWND_PMATCHCONDITION_OK3			107     //  �˻��� ��Ƽ�� ���� ����Ȯ�� ��ư �������� 
//#define SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1		108     //  �˻��� ��Ƽ�� ���� ������� ��ư ��Ȱ��ȭ 
//#define SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL2		109		//  �˻��� ��Ƽ�� ���� ������� ��ư Ȱ��ȭ 
//#define SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL3		110     //  �˻��� ��Ƽ�� ���� ������� ��ư �������� 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON			111		//	��Ī�ý��ۼ��� �� Ȯ�� ��ư (Ȱ��ȭ)
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1				112		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ������
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2				113		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��� 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3				114		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4				115		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư Ŭ�� 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5				116		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư ��Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6				117		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ���� 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1				118		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ������
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2				119		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��� 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3				120		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4				121		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư Ŭ�� 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5				122		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư ��Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6				123		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ���� 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1				124		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ������
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2				125		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��� 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3				126		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4				127		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư Ŭ�� 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5				128		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư ��Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6				129		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ���� 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1			130		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ������
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2			131		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��� 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3			132		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4			133		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư Ŭ�� 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5			134		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ��ư ��Ȱ��ȭ 
#define SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6			135		//	��Ī�ý��ۼ��� ����Ʈ �ڽ� ���� 

//����� �޴���ư 4�� *3 (Ȱ��, ��������, ��Ȱ��)
#define SPR_OBJ_GUILDWAR_RANK1					136
#define SPR_OBJ_GUILDWAR_RANK2					137
#define SPR_OBJ_GUILDWAR_RANK3					138
#define SPR_OBJ_GUILDWAR_LIST1					139
#define SPR_OBJ_GUILDWAR_LIST2					140
#define SPR_OBJ_GUILDWAR_LIST3					141
#define SPR_OBJ_GUILDWAR_MATCH1					142
#define SPR_OBJ_GUILDWAR_MATCH2					143
#define SPR_OBJ_GUILDWAR_MATCH3					144
#define SPR_OBJ_GUILDWAR_CHALLENGE1				145
#define SPR_OBJ_GUILDWAR_CHALLENGE2				146
#define SPR_OBJ_GUILDWAR_CHALLENGE3				147
//����� ��û ��ư
#define SPR_OBJ_GUILDWAR_APPLY1					148
#define SPR_OBJ_GUILDWAR_APPLY2					149
#define SPR_OBJ_GUILDWAR_APPLY3					150
//���� ��ư
#define SPR_OBJ_GUILDWAR_OBSERVE1				151
#define SPR_OBJ_GUILDWAR_OBSERVE2				152
#define SPR_OBJ_GUILDWAR_OBSERVE3				153
//��Ӵٿ� ���ҽ� 3��
#define DROPDOWN_OBJ_GUILDWAR_FORM_LEFT			154
#define DROPDOWN_OBJ_GUILDWAR_FORM_MIDDLE		155
#define DROPDOWN_OBJ_GUILDWAR_FORM_RIGHT1		156
#define DROPDOWN_OBJ_GUILDWAR_FORM_RIGHT2		157
#define DROPDOWN_OBJ_GUILDWAR_FORM_RIGHT3		158
#define DROPDOWN_OBJ_GUILDWAR_FORM_BOARD		159

#define DROPDOWN_OBJ_GUILDWAR_NUMBER_LEFT		160
#define DROPDOWN_OBJ_GUILDWAR_NUMBER_MIDDLE		161
#define DROPDOWN_OBJ_GUILDWAR_NUMBER_RIGHT1		162
#define DROPDOWN_OBJ_GUILDWAR_NUMBER_RIGHT2		163
#define DROPDOWN_OBJ_GUILDWAR_NUMBER_RIGHT3		164
#define DROPDOWN_OBJ_GUILDWAR_NUMBER_BOARD		165

#define DROPDOWN_OBJ_GUILDWAR_TYPE_LEFT			166
#define DROPDOWN_OBJ_GUILDWAR_TYPE_MIDDLE		167
#define DROPDOWN_OBJ_GUILDWAR_TYPE_RIGHT1		168
#define DROPDOWN_OBJ_GUILDWAR_TYPE_RIGHT2		169
#define DROPDOWN_OBJ_GUILDWAR_TYPE_RIGHT3		170
#define DROPDOWN_OBJ_GUILDWAR_TYPE_BOARD		171
//�����Ī ��ư
#define SPR_OBJ_GUILDWAR_AUTOMATCHING1			172
#define SPR_OBJ_GUILDWAR_AUTOMATCHING2			173
#define SPR_OBJ_GUILDWAR_AUTOMATCH_V			174
//�¿� ��ġ�� �ѱ��(����:10)
#define SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE1		175
#define SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE2		176
#define SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE3		177
#define SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE1		178
#define SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE2		179
#define SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE3		180
//��ŷ, ����Ʈ, ��Ī, ���� Ÿ��Ʋ��
#define SPR_GUILDWAR_TITLEBAR_RANK				181
#define SPR_GUILDWAR_TITLEBAR_LIST				182
#define SPR_GUILDWAR_TITLEBAR_MATCH				183
//��Ƽ �ý��� ģ�� �߰� (����â 2005.02.22)
#define SPR_OBJ_GROUPWND_FRIENDADD_OFF1			184
#define SPR_OBJ_GROUPWND_FRIENDADD_OFF2			185
#define SPR_OBJ_GROUPWND_FRIENDADD_OFF3			186
#define SPR_OBJ_GROUPWND_FRIENDADD_OFF4			187
#define SPR_OBJ_GROUPWND_FRIENDADD_OFF5			188
#define SPR_OBJ_GROUPWND_FRIENDADD_OFF6			189
#define SPR_OBJ_GROUPWND_FRIENDADD_OFF7			190
#define SPR_OBJ_GROUPWND_FRIENDADD_ON1			191
#define SPR_OBJ_GROUPWND_FRIENDADD_ON2			192
#define SPR_OBJ_GROUPWND_FRIENDADD_ON3			193
#define SPR_OBJ_GROUPWND_FRIENDADD_ON4			194
#define SPR_OBJ_GROUPWND_FRIENDADD_ON5			195
#define SPR_OBJ_GROUPWND_FRIENDADD_ON6			196
#define SPR_OBJ_GROUPWND_FRIENDADD_ON7			197


#define PARTY_FRIEND_MAX_PAGE					3    //��Ƽ��Ȳ (����â 2005. 03.04) �ֱ� ��Ƽ ��Ȳ MAX ������ 


//�� �������� ������ ����Ʈ ����(�ؽ�Ʈ)
#define MAXLINE_PER_PAGE						15
//��û, ������ư
#define GUILDWAR_CHECK_RETURN_APPLY				56
//�ڵ���Ī, 55�� �ٲ�鼭 �Ⱦ���.
#define GUILDWAR_CHECK_RETURN_MATCH				57
//��Ӵٿ� ��ư�� ���� ���ϰ�
#define GUILDWAR_CHECK_DROPDOWN_BUTTON_FORM		58
#define GUILDWAR_CHECK_DROPDOWN_BUTTON_NUMBER	59
#define GUILDWAR_CHECK_DROPDOWN_BUTTON_TYPE		60
//��Ӵٿ� ����Ʈ üũ ���� ��(����Ʈ�� �߰��Ǹ� ��ȣ�� �� �з���� �Ѵ�.)
#define GUILDWAR_CHECK_DROPDOWN_FORM			61
#define GUILDWAR_CHECK_DROPDOWN_NUMBER			69
#define GUILDWAR_CHECK_DROPDOWN_TYPE			77
//���� �����ִ� �κ� üũ(15��)
#define GUILDWAR_CHECK_TEXT_LINE_NUMBER			85
//������ �ѱ�� ����(10)
#define GUILDWAR_CHAGE_DOUBLE_PAGEPREV			100
#define GUILDWAR_CHAGE_DOUBLE_PAGENEXT			101


#define GROUP_RENDER_TEXT__GUILDWAR_RANK		41
#define GROUP_RENDER_TEXT__GUILDWAR_LIST		42
#define GROUP_RENDER_TEXT__GUILDWAR_MATCH		43
#define GROUP_RENDER_TEXT__GUILDWAR_CHALLENGE	44


class CGroupWnd : public CMenu
{
//��Ƽ ��ȭ �ý���(����â 2005.01.28)
public:
	PARTY_CONFIG_EXP	m_nExpDistrib;				//��Ƽ �� �޴� 7��° ��ư���� ù��° ��Ӵٿ� 
	PARTY_CONFIG_ITEM	m_nItemRouting;				//��Ƽ �� �޴� 7��° ��ư���� �ι�° ��Ӵٿ� 

	BYTE				m_bLevelMatch;				//��Ƽ �� �޴� 8��° ��ư���� ù��° ��Ӵٿ� 
	BYTE				m_bExpMatch;				//��Ƽ �� �޴� 8��° ��ư���� �ι�° ��Ӵٿ� 
	BYTE				m_bItemMatch;				//��Ƽ �� �޴� 8��° ��ư���� ����° ��Ӵٿ� 
	BYTE				m_bMasterMatch;				//��Ƽ �� �޴� 8��° ��ư���� �׹�° ��Ӵٿ� 
//��Ƽ ��ȭ �ý���(����â 2005.02.01)	
	BYTE				m_byBackBoard;				//���� �ʱⰪ:0
	BYTE		    	m_byPMatchStatus;           //��ġ ���� �ʱⰪ:0     ��Ī ������ ��� 0, ��Ī ���°� �ƴ� ��� 1
	int					m_nPMatchResult;            //����� ��� ��� �޾� ���� 0:���� �ƴϸ� �����ڵ� 
	int                 m_nPMatchEscapeResult;	    //����� Ż�� ��� �޾� ���� 0:���� �ƴϸ� �����ڵ� 
	BYTE                m_bMasterMatchStatus;       //��Ƽ��,��Ƽ�� �Ǵ� 
	PARTY_FRIEND		m_nPartyFriend[20];				//��Ƽ��Ȳ(����â 2005.03.03)
	DWORD				m_dwPartyCount;				//��Ƽ��Ȳ(����â 2005. 03.03)
	
 	// sung-han 2005-01-29 �������ŷ ����Ʈ
	//GUILD_LADDER_DATA m_LadderList[MAX_LADDER_LIST];//�������ŷ ����Ʈ�� ����� ����Ʈ
	GuildLadderInfoPage m_pGuildWarRankListInfoPage;//�������ŷ ����Ʈ�� ����� ����Ʈ
	int 		m_nGuildWarRankListPage;//�������ŷ ����Ʈ�� ������ (�ʱⰪ:1)
	int			m_nGuildWarRankListSelect;//�������ŷ ����Ʈ���� ������ ���� (�ʱⰪ:-1)
	int			m_nGuildWarRankListTotalCount;//�������ŷ ����Ʈ�� ���� (�ʱⰪ:0)
	int			m_nGuildWarRankListMaxPage;//�������ŷ ����Ʈ�� ������ �ִ밪 (�ʱⰪ:0)
	BYTE		m_byBtnChallange;//������ư ����(�ʱⰪ:0)
	BYTE		m_byBtnLeftDouble;//������ ���� �ΰ���ư(�ʱⰪ:0)
	BYTE		m_byBtnRightDouble;//������ ������ �ΰ���ư(�ʱⰪ:0)

	// sung-han 2005-01-29 ����� ����Ʈ
	GUILD_WAR_DATA m_GuildWarList[MAX_LADDER_LIST];//����� ����Ʈ�� ����� ����Ʈ
	int 		m_nGuildWarListPage;//����� ����Ʈ�� ������ (�ʱⰪ:1)
	int			m_nGuildWarListSelect;//����� ����Ʈ���� ������ ���� (�ʱⰪ:-1)
	int			m_nGuildWarListTotalCount;//����� ����Ʈ�� ���� (�ʱⰪ:0)
	int			m_nGuildWarListMaxPage;//����� ����Ʈ�� ������ �ִ밪 (�ʱⰪ:0)
	BYTE		m_byBtnOvserve;//������ư ����(�ʱⰪ:0)

	// sung-han 2005-01-29 ����� ��Ī �ɼ�
	BYTE		m_byGuildWarDropDownEnableNum;//���� �����ִ� ��Ӵٿ� �ڽ�(�ʱⰪ:0)
	BYTE		m_bynGuildWarForm;//�����(�ʱⰪ:0, ù��°���� ���õ� ����)
	BYTE		m_byGuildWarNumber;//������� ��(�ʱⰪ:0, ù��°���� ���õ� ����)
	BYTE		m_byGuildWarType;//���Ÿ��(�ʱⰪ:0, ù��°���� ���õ� ����)
	BOOL		m_bGuildWarMatchEnable;//��Ī �۵����ΰ�(�ʱⰪ:FALSE)
	MOUSECHECK 	m_byGuildWarDropDownButtonStatus;//��Ӵٿ� ��ư�� ����, ������ ��Ӵٿ�� �������� ���(�ʱⰪ:0)
	char		m_szGuildWarMatch[4][TEXT_RENDER_BUFFER];
	char		m_szGuildWarNumber[4][TEXT_RENDER_BUFFER];
	char		m_szGuildWarType[4][TEXT_RENDER_BUFFER];

	// sung-han 2005-01-29 ����� ����
	BOOL		m_bGuildWarChallenge;//����� ���� ��ư�� Ŭ���� �����ΰ�(�ʱⰪ:FALSE)

	// sung-han 2005-01-30 ����� ��Ŷ ������ üũ
	BOOL		m_bGuildWarPacketSendEnable;//�ʱⰪ:TRUE

public:

	BOOL		m_bMoveChk;
	
	BYTE		m_byRenderType;
	BYTE		m_byOrder;
	BYTE		m_byGroupTab;
	BYTE		m_byStatus;	
	
	BYTE		m_byColor; //��Ƽ ��ȭ �ý��� (����â 2005.01.26) ��ӹڽ� �ؽ�Ʈ ���� 

	char		m_szInfo[2][TEXT_RENDER_BUFFER];
	RECT		m_rtPos;

	BYTE		m_byMsgCnt;
	
	BOOL		m_bPartyMove;
	BOOL		m_bGuildMove;
	BOOL		m_bMessengerMove;
	BOOL		m_bGuildWarMove;

	int			m_nPartyUserIndex;
	
	BYTE		m_byLayerIndex;
	BOOL		m_bInfo;
	DWORD		m_dwPrevTime;
	DWORD		m_dwCurTime;
	WORD		m_wDungeonID;

	char		m_szName[MAX_CHARACTER_NAME_LENGTH];
	
	BYTE		m_byCurPage;
	BYTE		m_byMaxPage;

	BYTE		m_byUserType;
	BYTE		m_byMessengerType;

	BOOL		m_bPartyBtn[6];
	BOOL		m_bGuildBtn[7];
	BOOL		m_bFriednBtn[4];
	BOOL		m_bGuildWarBtn;
	BOOL		m_bPartyMsg;
	BOOL		m_bGuildMsg;	

	BYTE		m_byGuildLevel;
	
	ListNode<GUILD_USER>*			m_pGuildUserPos;	
	ListNode<GUILD_OFFLINE_USER>*	m_pGuildOfflineUserPos;
	
	ListNode<MESSENGER_USER>*		m_pMessengerOnlinePos;	
	ListNode<MESSENGER_USER>*		m_pMessengerOfflinePos;	
	
	IDISpriteObject*				m_pSprStatus[4];




public:
	
private:
	static CGroupWnd* c_pThis;

public:
	static CGroupWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGroupWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// �������� ���Ǵ� �͵��� ������ �Ѵ� //
	BOOL	GetClick();	
	BOOL	Init();
		
	void	RenderText();	
	void	Remove();
	void	SetOrder();		
	void	SetActive(BOOL bActive = TRUE);
		
	int		CheckInterface();

	// üũ ���� �Լ� //			
	void	OpenTab();
	void	SetStatus(BYTE byStatus);

	void	SetPartyUI();
	void	SetPosGroup();

	DWORD	GetBarResource(BYTE byIndex);
	DWORD	GetGageResource(BYTE byIndex);

	void	SetPrevNext(BYTE byType);
	
	void	RenderGuildInfo();
	void	RenderPartyInfo();
	void	RenderFriendInfo();	
	void	RenderGuildWarInfo();
	void	RenderGuildWarInfoNew();

	//sung-han 2005-01-29 ���̹� �����
	void	RenderTextGuildWarRankList();
	void	RenderTextGuildWarList();
	void	SetGuildWarRenderStatus();
	void	ReSetGuildWarRenderStatus();
	void	TempSetRandomData();
	void	InitGuildWarMatching();

	CGroupWnd();
	~CGroupWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//

