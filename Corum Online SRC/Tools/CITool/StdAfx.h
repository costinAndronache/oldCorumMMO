#if !defined(AFX_STDAFX_H__65AB7D10_D4C8_4575_BB4C_8369672C8A9E__INCLUDED_)
#define AFX_STDAFX_H__65AB7D10_D4C8_4575_BB4C_8369672C8A9E__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#define VC_EXTRALEAN		


#include <afxwin.h>       
#include <afxext.h>       
#include <afxdisp.h>      
#include <afxdtctl.h>	
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>		
#endif 


#include <afxtempl.h>
#include <list>
#include <vector>
#include <algorithm>
#include <functional>



using namespace std;

#pragma comment(lib, "SS3DGFUNC.lib")

#include "itemmanager.h"
#include "NewSerial.h"
#include "IOleDBThread.h"
#include "../../4DyuchiGXGFunc/global.h"  // 
             

//#define DEVELOPMODE
#define JAPANESS_MODE
#define JJWMODE
#define	CURRENT_DB_NUM		20


extern CApp				theApp;
extern IOleDBThread*	g_pDb;
extern char				g_CORUM_DB_NAME		[CURRENT_DB_NUM][32];		
extern char				g_CORUM_DB_ACCOUNT	[CURRENT_DB_NUM][32];		
extern char				g_CORUM_DB_PASSWORD	[CURRENT_DB_NUM][32];	
extern char				g_CORUM_DB_ADDRESS	[CURRENT_DB_NUM][32];
extern char				g_CORUM_TDB_NAME	[32];		
extern char				g_CORUM_TDB_ACCOUNT	[32];		
extern char				g_CORUM_TDB_PASSWORD[32];	
extern char				g_CORUM_TDB_ADDRESS	[32];
extern BYTE				g_CORUM_GAME_DB		[CURRENT_DB_NUM];
extern BYTE				g_CORUM_TOTAL_DB;
extern char				g_szExePath			[MAX_PATH];	
extern inline void		DisplayDBMessage	(char* szMsg);
extern LPCTSTR			g_szWhere[];	 	

extern STMPOOL_HANDLE	g_pGuildOfflinePool;	
extern STMPOOL_HANDLE	g_pGuildTablePool;
extern STMPOOL_HANDLE	g_pGuildWarPool;

#include "HSDBL.h"

extern CDBConnection g_DBConn[CURRENT_DB_NUM];

#endif