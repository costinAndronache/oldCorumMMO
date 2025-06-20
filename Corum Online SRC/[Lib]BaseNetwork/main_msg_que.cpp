#include "stdafx.h"
#include "main_msg_que.h"
#include "define.h"

CMsgQue*		g_pMsgQue = NULL;
CMsgQue::CMsgQue()
{
	memset(this,0,sizeof(CMsgQue));
}
BOOL CMsgQue::Initialize(DWORD dwMaxBufferSize)
{
	BOOL		bResult = FALSE;

	m_pBuffer = new char[dwMaxBufferSize+32];
	m_dwMaxBufferSize = dwMaxBufferSize;

	if (!m_pBuffer)
		goto lb_return;

	if (!m_pNext)
	{
		m_pNext = new CMsgQue;	
		m_pNext->m_pNext = this;
		m_pNext->Initialize(dwMaxBufferSize);
	}

	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CMsgQue::PushMsg(DWORD dwIndex,DWORD dwType,WSABUF* pBuf,DWORD dwNum,DWORD dwMsgSize)
{
	DWORD			i;
	BOOL			bResult = FALSE;
	MSG_CONTAINER*	pMsg;

	pMsg = (MSG_CONTAINER*)(m_pBuffer + m_dwBufferPosition);
	
	if ((dwType & MSG_TYPE_TCP_MASK) == MSG_TYPE_TCP_MASK)
	{
		if (m_dwBufferPosition + MSG_HEADER_SIZE > m_dwMaxBufferSize)
			goto lb_return;

		pMsg->m_dwConnectionIndex = dwIndex;
		pMsg->m_dwFlag = NULL;
		pMsg->m_dwSize = 0;
		pMsg->m_dwType = dwType;
		m_dwBufferPosition += MSG_HEADER_SIZE;
		m_dwMsgNum++;
		bResult = TRUE;
		goto lb_return;
	}

	if (m_dwBufferPosition + dwNum*MSG_HEADER_SIZE + dwMsgSize > m_dwMaxBufferSize)
		goto lb_return;
	
	for (i=0; i<dwNum; i++)
	{
		pMsg->m_dwConnectionIndex = dwIndex;
		pMsg->m_dwFlag = NULL;
		pMsg->m_dwSize = pBuf[i].len;

//		printf("\n Dll ����� ���� 9 : Set MSG_CONTAINER : Len = %d\n", pBuf[i].len);


		pMsg->m_dwType = dwType;
		memcpy(pMsg->m_pBuffer,pBuf[i].buf,pBuf[i].len);
		m_dwBufferPosition += (pBuf[i].len + MSG_HEADER_SIZE);
		pMsg = (MSG_CONTAINER*)(m_pBuffer + m_dwBufferPosition);
		m_dwMsgNum++;
		bResult = TRUE;
	}
	

lb_return:
	return bResult;
}

// 080924 LUJ, ���� ������ ����� �ʵ��� ��踦 üũ��
MSG_CONTAINER* CMsgQue::DispatchMsg()
{
	// 080924 LUJ, ��Ʈ��ũ ����� ��ü�� �� ��ġ ���� ���� �����Ⱑ �ȴ�
	//				�����쿡�� �޽��� ������� ȣ��ǹǷ�, ���� �Ŀ��� ȣ��� �� �ֱ� ����
	if( m_dwMaxBufferSize < m_dwDispatchPosition )
	{
		return 0;
	}

	MSG_CONTAINER* message = ( MSG_CONTAINER* )( m_pBuffer + m_dwDispatchPosition);
	m_dwDispatchPosition += ( MSG_HEADER_SIZE + message->m_dwSize );

	return message;
}


CMsgQue::~CMsgQue()
{
	if (m_pNext)
	{
		m_pNext->m_pNext = NULL;
		delete m_pNext;
		m_pNext = NULL;
	}
	if (m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
}