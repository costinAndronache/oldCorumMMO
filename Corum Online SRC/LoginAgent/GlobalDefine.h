#pragma once
//-------------- ���� ���� ���� ------------------
//- �� ����: �ݵ�� �Ѱ����� Define �Ǿ��־����! (��, �ݸ����� ����) -
//--------------------------------------------------
//#define ADULT_MODE					//���� ������ ��쿡 �� Ǯ����� ��.
//--------------------------------------------------


/*====================== FOR DEVELOPER DEFINITIONS =============================*/
#define USE_FUNCTION_PROFILE			//	�Լ� ����ð� üũ 
/*==============================================================================*/


/*====================== THE GAME SYSTEM DEFINITIONS ===========================*/
//#	define USE_LICENSING_SERVER
#define	RANDITEM_MODE
//#define __PACKET_ENCRYPTION
//#define BILLING_CHECK					//��� ���� Check Define
/*==============================================================================*/

enum QUERY_TYPE_VALUE
{
	QUERY_TYPE_LOGIN					= 1,
	QUERY_TYPE_ONLINE_CHECK				= 2,
	QUERY_TYPE_CHR_SELECT_INFO			= 3,
	QUERY_TYPE_CREATE_NEW_CHAR			= 4,
	QUERY_TYPE_DELETE_CHARACTER			= 5,
	QUERY_TYPE_CREATE_CHAR_INIT_BINARY	= 9,
	QUERY_TYPE_GM_CHECK					= 15,
	QUERY_TYPE_LOGOUT_ALL				= 16,
	QUERY_TYPE_CHECK_MONITOR			= 18,
	QUERY_TYPE_IS_ALIVE_USER			= 19,
	QUERY_TYPE_BILLING_CHECK			= 20,
	QUERY_TYPE_ITEMMALL_CHECK			= 21,
	QUERY_TYPE_SET_ITEMMALL_RECEIVE		= 22,
	QUERY_TYPE_BANKINFO					= 23,
	QUERY_TYPE_BANK_UPDATE				= 24,
	QUERY_TYPE_JUMIN_CHECK				= 25,
	QUERY_TYPE_BLOCK_TITLE				= 26,		
	QUERY_TYPE_WARNING_TITLE			= 27,		
	QUERY_TYPE_BANKITEM_CHECK			= 28,
	QUERY_TYPE_CHR_MOVE					= 29,
	QUERY_TYPE_CREATE_NEW_CHARMOVE		= 30,
	QUERY_TYPE_CHARMOVE_UPDATE			= 31,
	QUERY_TYPE_CHARCONVERT_DELETE		= 32,
	QUERY_TYPE_GUARDIANMOVE				= 33,
	QUERY_TYPE_GUARDIANINSERT			= 34,	
};












