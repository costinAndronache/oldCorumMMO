#ifndef __CSPINLOCK_H__f7960487_1f00_48f3_a4ac_0b356893c1b7__
#define __CSPINLOCK_H__f7960487_1f00_48f3_a4ac_0b356893c1b7__

#pragma once

#define AcquireSpinLock(x)		x.Lock()
#define ReleaseSpinLock(x)		x.Unlock()



class CSpinLock  
{
public:
	void Unlock(){LeaveCriticalSection(&m_hLock);};
	void Lock(){EnterCriticalSection(&m_hLock);};
	CSpinLock(){/*InitializeCriticalSection(&m_hLock);*/ InitializeCriticalSectionAndSpinCount(&m_hLock,4000);};
	virtual ~CSpinLock(){DeleteCriticalSection(&m_hLock);};

protected:
	CRITICAL_SECTION m_hLock;
};
/*
class CSpinLock  
{
public:
	CSpinLock()			{m_lSpinLock = 0;};
	~CSpinLock()		{};

	void	Lock()		{while(InterlockedCompareExchange(&m_lSpinLock, 1, 0) != 0);};
	void	Unlock()	{InterlockedExchange(&m_lSpinLock, 0);};

private:
	LONG		m_lSpinLock;
};
*/

#endif // __CSPINLOCK_H__f7960487_1f00_48f3_a4ac_0b356893c1b7__