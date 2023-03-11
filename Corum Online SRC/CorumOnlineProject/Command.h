#pragma once


enum COMMAND_MSG_TYPE
{
	MSG_Type_Empty = 0, 
	MSG_Type_General,
	MSG_Type_GM,
};


struct COMMAND_TOKEN_ENTRY
{
	int		iMsgType;			// � �޽����� (COMMAND_MSG_TYPE ����)
	int 	iCommandID;			// � ����̳�
	char *	pszToken;
};


#define DECLARE_COMMAND_TOKEN_ENTRY() \
private:\
		static const COMMAND_TOKEN_ENTRY	_TokenCommandEntries[];

#define BEGIN_COMMAND_TOKEN_ENTRY(theClass) \
	const COMMAND_TOKEN_ENTRY	theClass::_TokenCommandEntries[] = \
	{ \

#define END_COMMAND_TOKEN_ENTRY()		\
		{MSG_Type_Empty, 0, "" }		\
		};

#define ON_GENERAL_COMMAND(command_id, Token)	\
{ MSG_Type_General, command_id, Token },		


#define ON_GM_COMMAND(command_id, Token)	\
{ MSG_Type_GM, command_id, Token },		
