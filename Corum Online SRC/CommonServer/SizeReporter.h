#pragma once


#include "CommonPacket.h"


#define SIZE_REPORT(__TARGET_FILE, __MAX_SIZE, __MAX_SIZE_TYPE_NAME, __SIZE_REPORT_TYPE)\
{\
	__MAX_SIZE = max( __MAX_SIZE, sizeof(__SIZE_REPORT_TYPE) );\
	fprintf(__TARGET_FILE, "[ SIZE : %5d ] %s\n", sizeof(__SIZE_REPORT_TYPE), #__SIZE_REPORT_TYPE);\
	if ( sizeof(__SIZE_REPORT_TYPE) == __MAX_SIZE )\
	{\
		wsprintf( __MAX_SIZE_TYPE_NAME, #__SIZE_REPORT_TYPE );\
	}\
}


inline void DECLSPECIFIER ShowAllPacketSize()
{
	int		nMax			= 0;
	char	szMaxName[0xff]	= {0,};

	FILE* fp = fopen( "SizeReporter.txt", "w" );

	fprintf( fp, "### Packet Size Reporting Start!!! ###");

	fprintf( fp, "\n\n### Login Agent to Client ### \n" );
	SIZE_REPORT( fp, nMax, szMaxName, ASTC_CHARACTER_SELECT_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, ASTC_CHR_MOVECHK );
	SIZE_REPORT( fp, nMax, szMaxName, ASTC_CONNECT_WORLD_SERVER );
	SIZE_REPORT( fp, nMax, szMaxName, ASTC_IS_ALIVE_USER );
	SIZE_REPORT( fp, nMax, szMaxName, ASTC_ITEMMALL_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, ASTC_MOVE_CHARACTER_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, ASTC_MOVE_CHARACTER_SUCCESS );
	SIZE_REPORT( fp, nMax, szMaxName, ASTC_SERVER_ALIVE_INFO );

	fprintf( fp, "\n@@ Max Packet is %s [%d]\n", szMaxName, nMax );
	nMax = 0; memset( szMaxName, NULL, sizeof(szMaxName) );

	fprintf( fp, "\n\n### Login Agent to World Server ### \n" );
	SIZE_REPORT( fp, nMax, szMaxName, ASTWS_ADD_NEW_USER );
	SIZE_REPORT( fp, nMax, szMaxName, ASTWS_BILLING_SMALL_TIME );
	SIZE_REPORT( fp, nMax, szMaxName, ASTWS_CHANGE_BILLING_TYPE );
	SIZE_REPORT( fp, nMax, szMaxName, ASTWS_CHINA_BILLING_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, ASTWS_CMTOOL_NOTICE );
	SIZE_REPORT( fp, nMax, szMaxName, ASTWS_GUILD_CHARACTER_DELETE );
	SIZE_REPORT( fp, nMax, szMaxName, ASTWS_KICK_USER );
	SIZE_REPORT( fp, nMax, szMaxName, ASTWS_REQUEST_SERVER_ALIVE );
	SIZE_REPORT( fp, nMax, szMaxName, ASTWS_SERVER_ATTACH_RESULT );
	SIZE_REPORT( fp, nMax, szMaxName, BILLING_INFO );

	fprintf( fp, "\n@@ Max Packet is %s [%d]\n", szMaxName, nMax );
	nMax = 0; memset( szMaxName, NULL, sizeof(szMaxName) );
	
	fprintf( fp, "\n\n### Client to Login Agent ### \n" );
	SIZE_REPORT( fp, nMax, szMaxName, CTAS_CHECK_MONITORING );
	SIZE_REPORT( fp, nMax, szMaxName, CTAS_CHR_MOVECHK );
	SIZE_REPORT( fp, nMax, szMaxName, CTAS_CMTOOL_NOTICE );
	SIZE_REPORT( fp, nMax, szMaxName, CTAS_IS_ALIVE_USER );
	SIZE_REPORT( fp, nMax, szMaxName, CTAS_KICK_USER );
	SIZE_REPORT( fp, nMax, szMaxName, CTAS_MOVE_CHARACTER_RESEND );
	SIZE_REPORT( fp, nMax, szMaxName, CTAS_REQUEST_SERVER_ALIVE );

	fprintf( fp, "\n@@ Max Packet is %s [%d]\n", szMaxName, nMax );
	nMax = 0; memset( szMaxName, NULL, sizeof(szMaxName) );
	
	fprintf( fp, "\n\n### Client to Client (TEST) ### \n" );
	SIZE_REPORT( fp, nMax, szMaxName, CTC_AIRSHIP_GETOFF );
	SIZE_REPORT( fp, nMax, szMaxName, CTC_AIRSHIP_RIDING );
	SIZE_REPORT( fp, nMax, szMaxName, CTC_MOVE_POSITION );
	SIZE_REPORT( fp, nMax, szMaxName, CTC_WORLD_MOVE );
	SIZE_REPORT( fp, nMax, szMaxName, CTC_WORLD_STOP );

	fprintf( fp, "\n@@ Max Packet is %s [%d]\n", szMaxName, nMax );
	nMax = 0; memset( szMaxName, NULL, sizeof(szMaxName) );
	
	fprintf( fp, "\n\n### Client to Dungeon Server ### \n" );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_ACCENTRANCE_ACQUISITION );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_ACCEXP_ACQUISITION );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_CHAR_LEVELUP );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_DUNGEON_ATTACK_USER_MON );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_DUNGEON_ATTACK_USER_USER );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_DUNGEON_CALLGARDIAN );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_DUNGEON_CASTING );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_DUNGEON_CHANGE_LRSKILLKIND );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_DUNGEON_CHAT );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_DUNGEON_JOIN );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_DUNGEON_MOVE );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_DUNGEON_OUT );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_DUNGEON_PKMODE );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_DUNGEON_REQUEST_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_DUNGEON_STOP );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_EMOTICON );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_ENTRANCE_EDIT );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_EVENT_CLEAR_EFFECT_FINISH );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_EVENT_USER_PREV_DUNGEON_ID );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_FIND_HACK );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_FINISH_MAPLOADING );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_GM_COMMAND );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_GM_QUICK_MOVE );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_GUARDIAN_ATTACK );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_GUARDIAN_COMMAND );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_GUARDIAN_COMMAND_ATTACK );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_GUARDIAN_COMMAND_MOVE );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_GUILD_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_GUILD_ITEM );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_GUILD_JOININFO );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_GUILD_PORTALFAIL );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_GUILD_PORTALUSED );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_GUILD_USERDATA );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_ITEMMAKING );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_ITEMUPGRADE );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_ITEM_MOVE );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_ITEM_PICKUP );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_ITEM_USED );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_ITEM_USED_NATIVE );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_MAKING_CANCEL );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_MONSTER_COMMAND );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_NEW_GM_COMMAND );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_PARTY_PORTAL );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_PARTY_PORTALFAIL );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_PARTY_PORTALUSED );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_PARTY_PORTAL_RECOGNITION );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_PLAYERSHOP_CLOSE );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_PLAYERSHOP_OPEN );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_PLAYERSHOP_SHOWALL );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_PLAYER_REST );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_PORTAL_MOVE );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_PRODUCT_ACQUISITION );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_READY_MATCH_MAN2MAN );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_REPLY_MATCH_MAN2MAN );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_REQUEST_MATCH_MAN2MAN );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_REQUEST_SECTION_INFO_FOR_PORTAL );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_SKILL );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_SKILL_LEVELUP );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_SWITCH_WEAPON );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_TICK_CHECK );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_TRADE_CALCEL );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_TRADE_OK );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_TRADE_REQUEST );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_TRADE_RESULT );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_TRADE_TRADE );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_UPGRADE_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_USER_DIE_OK );
	SIZE_REPORT( fp, nMax, szMaxName, CTDS_WORLD_MOVE );

	fprintf( fp, "\n@@ Max Packet is %s [%d]\n", szMaxName, nMax );
	nMax = 0; memset( szMaxName, NULL, sizeof(szMaxName) );
	
	fprintf( fp, "\n\n### Client to World Server ### \n" );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_CHARACTER_SEARCH );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_CHARACTER_SELECT );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_CHAT_MESSAGERQ );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_CHECK_GM );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_CHECK_MONITORING );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_COMMAND_MSG );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_CREATE_NEW_CHARACTER );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_DELETE_CHARACTER );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_DGCHAT_USER );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_DSCHAT_COMMAND );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_FRIENDUSER_DELETE );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_FRIENDUSER_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GM_COMMAND );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GM_NOTICE );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GO_TO_WORLD_SUCCESS );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILDJOIN_MESSAGE );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILDUSER_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILDWAR_CREATE );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILDWAR_ERRROR );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILDWAR_EXIT );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILDWAR_RANK );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILDWAR_REQUEST );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILDWAR_REQUESTRT );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILD_CREATE );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILD_DATA_REQUEST );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILD_DELETE );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILD_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILD_JOIN );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILD_RANK );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILD_REQUEST );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILD_USERDEL );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_GUILD_USERREQUEST_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_KICK_USER );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_LOADING_STATUS );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_LOGIN );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_MESSENGER_ANSWER );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_MESSENGER_STATUS );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_MESSENGER_USERADD );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_NETMARBLE_LOGIN );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_NEW_GM_COMMAND );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_PARTYUSER_WORLDINFO );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_PARTY_APPROVE );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_PARTY_BOARDDELETE );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_PARTY_ENTRY );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_PARTY_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_PARTY_INFOREQUEST );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_PARTY_JOIN );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_PARTY_LOGOUT );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_PARTY_LOGOUTRT );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_PARTY_SET );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_PARTY_USERREQUEST );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_PARTY_USERREQUEST_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_QUERY_EVENT_DUNGEON_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_REFRESH_RECENT_DUNGEON );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_REQUEST_DUNGEON_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_REQUEST_OWN_DUNGEON_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_SKILL_KEY );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_TIMER_ACK );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_TIMER_READY );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_TRY_TO_JOIN_DUNGEON );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_USER_GUILDMASTER );
	SIZE_REPORT( fp, nMax, szMaxName, CTWS_WORLD_LOGIN );

	fprintf( fp, "\n@@ Max Packet is %s [%d]\n", szMaxName, nMax );
	nMax = 0; memset( szMaxName, NULL, sizeof(szMaxName) );
	
	fprintf( fp, "\n\n### Dungeon Server to Client ### \n" );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ACCENTRANCE_ACQUISITION );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_APEAR_SKILL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_APPEAR );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_APPEAR_ITEM );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_APPEAR_MON );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_APPENDIX_GUARDIAN_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ARMOR_ITEM );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ATTACK_MODE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ATTACK_MON_MON );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ATTACK_MON_USER );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ATTACK_USER_MON );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ATTACK_USER_USER );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_BADACTION );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_BANK_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_CHANGE_LAYER );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_CHANGE_SECTION );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_CONGESTION_MODE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_COPY_ITEM_INSERTED );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DESTROY_CP );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DISAPPEAR );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DISAPPEAR_ITEM );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DISAPPEAR_MON );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_CASTING );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_CASTING_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_CHANGE_LRSKILLKIND );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_CHAT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_EMOTICON );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_ERROR_CODE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_JOIN_FAILED );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_MESSAGE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_NOTICE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_PKMODE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_REPLY_MATCH_MAN2MAN );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_REQUEST_MATCH_MAN2MAN );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_SIEGEINFO );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_STOPSTATUSEFFECT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_DUNGEON_USED_POTION );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_END_MATCH_MAN2MAN );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_ALIVE_MONSTER_COUNT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_ALIVE_USER_COUNT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_CHANGE_LAYER_RESULT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_CLEAR_CONDITION );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_CLEAR );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_CLEARUSER_SEND );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_CLEAR_PRESENT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_CLOSE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_CLOSE_DISPLAY );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_ENTER );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_FAILED );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_FIRST_TIMEOUT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_JOINUSER_COUNT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_LAST_LAYER );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_LAYER_CLEAR );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_OPEN_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_SURVIVAL_DISPLAY );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_TRAINING_DISPLAY );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_DUNGEON_TRAINING_ONELAYER_CLAYER );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_EVENT_EXCEPTION_RESULT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_FINISH_MAPLOADING );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_GUARDIANSUPPLIES_CANCAL_RETURN_INV );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_GUARDIAN_LEVELUP );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_GUARDIAN_STATUS );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_GUILDUSER_FLAG );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_GUILD_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_GUILD_FLAG );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_GUILD_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_GUILD_ITEM );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_GUILD_PORTALFAIL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_GUILD_RANK );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_GUILD_WAR_MESSAGE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_GURADIAN_EGG_SUMMON_STATUS );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_HELMET_ITEM );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ITEMMAKING_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ITEMMAKING_SUCCESS );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ITEMUPGRADE_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ITEMUPGRADE_FAIL_INSURANCE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ITEMUPGRADE_SUCCESS );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ITEM_MOVE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ITEM_PICKUP );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ITEM_PUT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ITEM_USED_NATIVE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_JOIN );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_KILL_MON );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_LEVELUP );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_MAKING_CANCAL_RETURN_INV );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_MAKING_CANCEL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_MATCH_USER_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_MOVE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_MOVE_MON );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_ORIGINATIONITEMWITHEFFECTKIND );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_OVERDRIVE_CHK );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PARTY );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PARTY_ARRAY );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PARTY_BOARD );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PARTY_MAIGCARRAYFAIL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PARTY_PORTALFAIL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PARTY_PORTALMOVE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PKCOUNT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PLAYERSHOPITEM_RETURN_INV );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PLAYERSHOP_CLOSE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PLAYERSHOP_ITEM );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PLAYERSHOP_OPEN );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PLAYERSHOP_SELL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PLAYERSHOP_SHOWALL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PLAYERSHOP_SOLDOUT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PLAYER_BAD );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PLAYER_REST );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PORTAL_DISAPPEAR );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PORTAL_JOIN_USER );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PORTAL_MOVE_OUTSIDE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_PORTAL_MOVE_OUTSIDE_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_READY_MATCH_MAN2MAN );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_RESPONSE_PORTAL_JOIN_USER );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_SHOP_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_SHOP_ITEM );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_SKILL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_SKILL_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_SKILL_INIT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_SKILL_RESULT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_START_MATCH_MAN2MAN );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_STATUS_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_STATUS_INIT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_STOP );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_STOP_MON );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_SWITCH_WEAPON );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_TRADE_CANCEL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_TRADE_CANCELFAIL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_TRADE_CANCEL_RETURN_INV );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_TRADE_DATA );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_TRADE_ITEM );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_TRADE_OK );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_TRADE_REQUEST );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_TRADE_RESULT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_TRADE_RETURN );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_TRADE_TRADE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_UPDATE_GUARDIAN_ITEM );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_UPGRADE_CANCAL_RETURN_INV );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_UPGRADE_CHK );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_UPGRADE_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_USER_PORTAL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_USER_SKILL_LEVEL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_USER_SPEED );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_USER_STATUS );
	SIZE_REPORT( fp, nMax, szMaxName, DSTC_WEAPON_HAND );

	fprintf( fp, "\n@@ Max Packet is %s [%d]\n", szMaxName, nMax );
	nMax = 0; memset( szMaxName, NULL, sizeof(szMaxName) );
	
	fprintf( fp, "\n\n### Dungeon Server to World Server ### \n" );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_CHECK_DUNGEONJOIN );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_DUNGEON_ALIVE_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_DUNGEON_CHANGE_OPERATION_TYPE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_DUNGEON_GROUPEXP );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_DUNGEON_GUARDIANITEM_ATTACH );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_DUNGEON_JOIN_SUCCESS );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_DUNGEON_MAGICARRAY_ATTACH );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_DUNGEON_OWNER_CHANGE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_DUNGEON_REFRESH_OPERATIONMODE );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_EVENT_DUNGEON_CLEAR );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_EVENT_DUNGEON_USER_ENTER_BY_PORTAL );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_EVENT_DUNGEON_USER_OUT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_FRIENDUSER_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_GM_COMMAND );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_GOTOWORLD );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_GUILDUSER_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_GUILDWAR_POINT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_PARTY_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_REFRESH_ACCENTRANCE_ACQUISITION );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_REFRESH_ACCEXP_ACQUISITION );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_REFRESH_DEFENSECOUNT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_REFRESH_ENTRANCE_EDIT );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_REFRESH_PRODUCT_ACQUISITION );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_SERVER_ATTACH );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_USER_ACCEPT_ALLOWED );
	SIZE_REPORT( fp, nMax, szMaxName, DSTWS_USER_ACCEPT_FAILED );

	fprintf( fp, "\n@@ Max Packet is %s [%d]\n", szMaxName, nMax );
	nMax = 0; memset( szMaxName, NULL, sizeof(szMaxName) );
	
	fprintf( fp, "\n\n### Dungeon Server to World Server - Dungeon Server ### \n" );
	SIZE_REPORT( fp, nMax, szMaxName, DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL );
	SIZE_REPORT( fp, nMax, szMaxName, DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL );
	SIZE_REPORT( fp, nMax, szMaxName, DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL );
	SIZE_REPORT( fp, nMax, szMaxName, DSWST_CREATEPRODUCTION );

	fprintf( fp, "\n@@ Max Packet is %s [%d]\n", szMaxName, nMax );
	nMax = 0; memset( szMaxName, NULL, sizeof(szMaxName) );
	
	fprintf( fp, "\n\n### World Server to Login Agent ### \n" );
	SIZE_REPORT( fp, nMax, szMaxName, WSTAS_REQUEST_CHINA_BILLING );
	SIZE_REPORT( fp, nMax, szMaxName, WSTAS_REQUEST_CHINA_BILLINGINFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTAS_SERVER_ALIVE_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTAS_SERVER_ATTACH );
	SIZE_REPORT( fp, nMax, szMaxName, WSTAS_USER_ACCEPT_ALLOWED );
	SIZE_REPORT( fp, nMax, szMaxName, WSTAS_USER_ACCEPT_FAILED );
	SIZE_REPORT( fp, nMax, szMaxName, WSTAS_USER_WORLD_LOGIN_SUCCESS );
	SIZE_REPORT( fp, nMax, szMaxName, WSTAS_WORLD_USER_DISCONNECT );

	fprintf( fp, "\n@@ Max Packet is %s [%d]\n", szMaxName, nMax );
	nMax = 0; memset( szMaxName, NULL, sizeof(szMaxName) );
	
	fprintf( fp, "\n\n### World Server to Client ### \n" );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_BILLING_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_BLOCK_ACCOUNT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_CB_MESSAGE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_CHAT_COMMAND );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_CHAT_RQ );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_CHAT_USER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_CHAT_WHISPER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_CHINA_BILL_KICK_CODE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_CHINA_BILL_REMAIN_TIME );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_COMMAND_MSG );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_CONNECT_DUNGEON_SERVER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_CREATE_CHARACTER_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_CREATE_CHARACTER_SUCCESS );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_DELETE_CHARACTER_RESULT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_DGCHAT_USER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_DGGUILD_USER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_DGGUILD_USERINFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_DSCHAT_WHISPER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_DSF_CHAT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_DS_CHAT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_DUNGEON_JOIN_FAILED );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_ENCRYPTION_KEY );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_EVENT_DUNGEON_ENTER_FAILED );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_EVENT_DUNGEON_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_EVENT_DUNGEON_STATE_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_FRIENDUSER_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GM_COMMAND_RESULT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GM_LOGIN_SUCCESS );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GO_TO_WORLD_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILDJOIN_MESSAGE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILDUSER_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILDUSER_LEVEL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILDWAR_ERRROR );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILDWAR_EXIT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILDWAR_JOIN );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILDWAR_MESSAGE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILDWAR_POINT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILDWAR_RANK );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILDWAR_REQUEST );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_COUNT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_CREATE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_DATA_REQUEST );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_DELETE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_ERROR );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_EXP );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_JOININFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_JOINRESULT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_LEVEL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_LIST );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_OFFLINEUSER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_OFFLINEUSER_DELETE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_PORTAL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_PORTALFAIL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_PORTALUSED );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_RANK );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_REQUEST );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_USER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_USERINFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_USERINFODEL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_GUILD_USERREQUEST_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_HELP_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_INVALID_GM );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_LOGIN_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_LOGIN_SUCCESS );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_MESSENGER_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_MESSENGER_STATUS );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_MESSENGER_SUCCESS );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_MESSENGER_USER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_MESSENGER_USERADD );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_MESSENGER_USERFAIL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_MESSENGER_USERINFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_MOVE_TO_WORLD );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_ENTRY );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_HP );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_INFOREQUEST );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_JOIN );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_JOINID );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_LEADER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_LEVEL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_LOGOUT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_PORTAL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_PORTALFAIL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_PORTALMOVE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_PORTALUSED );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_SET );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_USERREQUEST );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_USERREQUEST_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_PARTY_USER_TRY_TO_CONNECT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_RANK_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_REFRESHEVENT_DEFENSECOUNT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_REFRESHEVENT_DUNGEON_ACCENTRANCE_ACQUISITION );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_REFRESHEVENT_DUNGEON_ACCEXP_ACQUISITION );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER_GUILD );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_REFRESHEVENT_DUNGEON_CREATEPRODUCTION );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_REFRESHEVENT_DUNGEON_ENDSIEGE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_REFRESHEVENT_DUNGEON_ENTRANCE_EDIT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_REFRESHEVENT_DUNGEON_GUARDIANITEM );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_REFRESHEVENT_DUNGEON_LEVELUP );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_REFRESHEVENT_DUNGEON_MAGICFIELDARRAY );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_REFRESHEVENT_DUNGEON_OPERATIONMODE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_REFRESHEVENT_DUNGEON_STARTSIEGE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_REPLY_DUNGEON_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_SEARCH );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_SKILL_KEY );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_TIMER_SYNC );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_USER_DELETE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_USER_GUILDMASTER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_USER_MASTER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_WARNING_TITLE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_WORLD_APPEAR );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_WORLD_DISAPPEAR );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_WORLD_LOGIN_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_WORLD_PARTY_USER_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_WORLD_USER_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_WSF_CHAT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTC_WS_CHAT );

	fprintf( fp, "\n@@ Max Packet is %s [%d]\n", szMaxName, nMax );
	nMax = 0; memset( szMaxName, NULL, sizeof(szMaxName) );
	
	fprintf( fp, "\n\n### World Server to Dungeon Server ### \n" );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_ADD_USER_ACCEPT_ALLOWED );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_CB_LEVELCHK );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_CHARACTER_SEARCH );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_CHECK_DUNGEONJOIN_RT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_DUNGEON_MESSAGE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_EVENT_DUNGEON_CLOSE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_EVENT_DUNGEON_CLOSE_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_EVENT_DUNGEON_FIRST_TIMEOUT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_EVENT_DUNGEON_INIT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_EVENT_DUNGEON_OPEN_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_EVENT_DUNGEON_SET );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_EVENT_JOINUSER_COUNT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_FRIENDUSER_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GM_COMMAND );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GUILDINFO_MSG );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GUILDUSER_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GUILDUSER_RANK );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GUILDWAR_DELETE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GUILDWAR_MESSAGE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GUILD_ALLPORTAL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GUILD_FAIL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GUILD_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GUILD_LEVEL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GUILD_LIST );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GUILD_MSG );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GUILD_PORTALFAIL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GUILD_PORTALUSED );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_GUILD_RANK );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_KICK_USER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_NOTICE_ACTION );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_PARTY_ALLPORTAL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_PARTY_BOARD );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_PARTY_HP );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_PARTY_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_PARTY_LEADER );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_PARTY_LEVEL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_PARTY_MSG );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_PARTY_PORTAL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_PARTY_PORTALFAIL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_PARTY_PORTALMOVE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_PARTY_PORTALUSED );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_REFRESH_DUNGEON_OWNER_GUILD );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_REMOVE_USER_ACCEPT_ALLOWED );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_REQUEST_DUNGEON_ALIVE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_SEARCH );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_SET_DOUBLE_EXP );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_SET_EXPGAIN );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_SET_MAGICITEM_DROPRATE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_SET_PARTY_BONUS_EXPGAIN );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_USER_PORTAL );
	SIZE_REPORT( fp, nMax, szMaxName, WSTDS_USER_PORTAL );

	fprintf( fp, "\n@@ Max Packet is %s [%d]\n", szMaxName, nMax );
	nMax = 0; memset( szMaxName, NULL, sizeof(szMaxName) );
	
	fprintf( fp, "\n\n### World Server Server Packet ### \n" );
	SIZE_REPORT( fp, nMax, szMaxName, WSTS_DUNGEON_INFO );
	SIZE_REPORT( fp, nMax, szMaxName, WSTS_DUNGEON_LEVELUP );
	SIZE_REPORT( fp, nMax, szMaxName, WSTS_END_SIEGE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTS_FRIENDUSER_DELETE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTS_REFRESH_ALL_DUNGEON_SERVER_DATA );
	SIZE_REPORT( fp, nMax, szMaxName, WSTS_SERVER_ATTACH_RESULT );
	SIZE_REPORT( fp, nMax, szMaxName, WSTS_SERVER_TERMINATE );
	SIZE_REPORT( fp, nMax, szMaxName, WSTS_START_SIEGE );

	fprintf( fp, "\n@@ Max Packet is %s [%d]\n", szMaxName, nMax );
	nMax = 0; memset( szMaxName, NULL, sizeof(szMaxName) );
	
	fprintf( fp, "\n\n### No packet, just Struct ### \n" );
	SIZE_REPORT( fp, nMax, szMaxName, _CP_INFO_ );
	SIZE_REPORT( fp, nMax, szMaxName, ECHO_KEEP_ALIVE );
	SIZE_REPORT( fp, nMax, szMaxName, ITEM_NODE_FOR_MSG );
	SIZE_REPORT( fp, nMax, szMaxName, ITEM_NODE_FOR_MSG_NATIVE );
	SIZE_REPORT( fp, nMax, szMaxName, PLAYERSHOPINFO_NODE );
	SIZE_REPORT( fp, nMax, szMaxName, PORTAL_BUCKET );
	SIZE_REPORT( fp, nMax, szMaxName, RS_CharacterLogInfo );
	SIZE_REPORT( fp, nMax, szMaxName, TARGETINFO );
	SIZE_REPORT( fp, nMax, szMaxName, USER_STATUS );
	SIZE_REPORT( fp, nMax, szMaxName, WORLD_PARTY_APPEAR_INFO );

	fprintf( fp, "\nMax Struct is is %s [%d]\n", szMaxName, nMax );
	nMax = 0; memset( szMaxName, NULL, sizeof(szMaxName) );

	fprintf( fp, "\nReporting is Finished!!!" );

	fclose( fp );

}