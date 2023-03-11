/*
 *	added by minjin. 2004. 11. 12.
 *	DB Connection �� �ΰ� ������ �� �ְ� ����..
 *	
 *	���� : �ι�° Ŀ�ؼ����� �������� �ϴ� ���� ���� �Լ� ��ü�� ���� �Ʒ���
 *	
 *	SECOND_DB_OPERATION_START()
 *		????????????->THExecuteSQL(".............", ....... );
 *	SECOND_DB_OPERATION_END()
 *
 *	���Ͱ��� ����Ѵ�.
 */
#ifndef __DOUBLE_DB_CONNECTION_H__
#define __DOUBLE_DB_CONNECTION_H__

/*
 *	GAME_DB1 �� GAME_DB2 �� main.cpp �� ���ǵǾ� �ְ�,
 *	stdafx.h �� extern ����Ǿ� �ִ�.
 */
#pragma once

	#ifdef DOUBLE_DB_CONNECTION

		#define	SECOND_DB_OPERATION_START()\
			GAME_DB = (BYTE)GAME_DB2;

		#define	SECOND_DB_OPERATION_END()\
			GAME_DB = (BYTE)GAME_DB1;

	#else

		#define SECOND_DB_OPERATION_START()

		#define SECOND_DB_OPERATION_END()

	#endif	//	DOUBLE_DB_CONNECTION


#endif	//	__DOUBLE_DB_CONNECTION_H__