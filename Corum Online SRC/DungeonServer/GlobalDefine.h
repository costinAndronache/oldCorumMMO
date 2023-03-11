#pragma once


/*====================== FOR DEVELOPER DEFINITIONS	 ===========================*/
//#define DEV_MODE	
/*==============================================================================*/


/*====================== THE GAME SYSTEM DEFINITIONS ===========================*/
#define CHECK_EQUIP
#define	EQUIP_CHK
#define VER_RIVAL_GUILD_WAR
#define NEW_COMMAND
#define SUPPLY_EXCEED_USED	
#define TRAFFIC_LOG
/*==============================================================================*/


#define WSINDEX		g_pThis->GetWSConnectionIndex()


enum QUERY_TYPE_VALUE
{
	DB_UUID_DUNGEON_JOIN			= 10,
	DB_BANK_USER_INFO				= 11,
	DB_USERINFO_ID					= 12,
	DB_USER_CHARACTER				= 13,
	DB_UUID_GUARDIAN_INFO			= 14,
	DB_USERINFO_ID2					= 15,
	DB_USER_CHARACTER2				= 16,
	DB_CHANGE_DUNGEON_OWNER			= 17,
	DB_CREATE_GUARDIAN				= 18,
	DB_USER_PLAYERSHOP				= 19,
	DB_PRESENT_COUNT_CHECK			= 20,
	QUERY_TYPE_USER_UPDATE			= 21,
	QUERY_TYPE_USER_BANK_UPDATE		= 22,
	QUERY_TYPE_TRADE				= 23,
	QUERY_TYPE_USER_PLAYERSHOP		= 24,
};


#define PK_DEC_TIME						(60*60*36)		// �ð� �Ŀ� �ٿ��ش�.	
#define	PKREPEAT_DEC_TIME				(60*60*72)		// �ð� �Ŀ� �ٿ��ش�.
#define PKREPEAT_INC_TIME				(60*60*2)		// 2�ð�
#define	PKREPEAT_DECLASTUPDATE_TIME		60*1000*60*6	// �ݺ� PK ī��Ʈ ���� �ð�.
#define GODMODE_TIME					1000*5
#define MATCH_TIME						300000			// �׽�Ʈ 30�ʰ� // 5�а� (300��)
#define MATCH_START_TERM_TICK			5000			// 5��
#define MATCH_JUDGMENT_TERM_TICK		10000			// 10��
#define MATCH_END_LAYER_TERM_TICK		10000			// 10��


#ifdef CHINA_LOCALIZING
#	define __USE_STAT_LIMIT			// �߱� ������ ���, �������� ����, ���� ������ �Ǵ�.
#	define __SKILL_MASTERY_ENABLE	// ��ų �����͸��� Ȱ��ȭ�Ұ��ΰ�? �ϰ� ������ Ǯ�����.
#endif

#ifdef JAPAN_LOCALIZING				
#	undef	DEV_MODE				// ���߸�� ������ ����
#	define	__USE_STAT_LIMIT		// �Ϻ� ������ ���, �������� ����, ���� ������ �Ǵ�.
#endif


#ifdef CHINA_LOCALIZING
#	define PERIOD_SPEED_HACK_CHECK		(1000*60)		// 60 sec
#	define PERIOD_SPEED_HACK_ERROR		(500)			// 500 millisec ����
#else
#	define PERIOD_SPEED_HACK_CHECK		(1000*60)		// 1 min
#	define PERIOD_SPEED_HACK_ERROR		(500)			// 500 millisec ����
#endif


#ifdef	DEV_MODE
#	undef	PK_DEC_TIME
#	undef	PKREPEAT_DEC_TIME
#	undef	PKREPEAT_INC_TIME
#	undef	PKREPEAT_DECLASTUPDATE_TIME

#	define	PK_DEC_TIME							(60*5)			// �ð� �Ŀ� �ٿ��ش�.	
#	define	PKREPEAT_DEC_TIME					(60*6)			// �ð� �Ŀ� �ٿ��ش�.
#	define	PKREPEAT_INC_TIME					(60*2)			// 2�ð�
#	define	PKREPEAT_DECLASTUPDATE_TIME			60*1000*60*6	// �ݺ� PK ī��Ʈ ���� �ð�.
#endif






