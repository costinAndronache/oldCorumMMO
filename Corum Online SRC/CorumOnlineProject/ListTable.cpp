#include "ListTable.h"
#include "InitGame.h"

void DeleteAllChatList()
{
	if(g_pWhisperChatList)
	{
		POSITION_ pos = g_pWhisperChatList->GetHeadPosition();	
		POSITION_ posTemp;
		
		while(pos)
		{		
			posTemp = pos;

			LP_CHAT_MSG lpChatNode = (LP_CHAT_MSG)g_pWhisperChatList->GetAndAdvance(pos);
			delete lpChatNode;
			lpChatNode	= NULL;
			g_pWhisperChatList->RemoveAt(posTemp);
		}		
	}
}

void DeleteGuildChatList()
{
	if(g_pGuildChatList)
	{
		POSITION_ pos = g_pGuildChatList->GetHeadPosition();	
		POSITION_ posTemp;
		
		while(pos)
		{		
			posTemp = pos;

			LP_CHAT_MSG lpChatNode = (LP_CHAT_MSG)g_pGuildChatList->GetAndAdvance(pos);
			delete lpChatNode;
			lpChatNode	= NULL;
			g_pGuildChatList->RemoveAt(posTemp);
		}		
	}
}

void DeleteFriendChatList()
{
	if(g_pFriendChatList)
	{
		POSITION_ pos = g_pFriendChatList->GetHeadPosition();	
		POSITION_ posTemp;
		
		while(pos)
		{		
			posTemp = pos;

			LP_CHAT_MSG lpChatNode = (LP_CHAT_MSG)g_pFriendChatList->GetAndAdvance(pos);
			delete lpChatNode;
			lpChatNode	= NULL;
			g_pFriendChatList->RemoveAt(posTemp);
		}		
	}
}

void DeleteWhisperChatList()
{
	if(g_pWhisperChatList)
	{
		POSITION_ pos = g_pWhisperChatList->GetHeadPosition();	
		POSITION_ posTemp;
		
		while(pos)
		{		
			posTemp = pos;

			LP_CHAT_MSG lpChatNode = (LP_CHAT_MSG)g_pWhisperChatList->GetAndAdvance(pos);
			delete lpChatNode;
			lpChatNode	= NULL;
			g_pWhisperChatList->RemoveAt(posTemp);
		}		
	}
}

void DeletePartyChatList()
{
	if(g_pPartyChatList)
	{
		POSITION_ pos = g_pPartyChatList->GetHeadPosition();	
		POSITION_ posTemp;
		
		while(pos)
		{		
			posTemp = pos;

			LP_CHAT_MSG lpChatNode = (LP_CHAT_MSG)g_pPartyChatList->GetAndAdvance(pos);
			delete lpChatNode;
			lpChatNode	= NULL;
			g_pPartyChatList->RemoveAt(posTemp);
		}
	}
}
	

	
	

	
