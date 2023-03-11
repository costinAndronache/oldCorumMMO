#include "../../StdAfx.h"
#include "Pulse.h"

// constructor and destructor
CPulse::CPulse(void) : m_dwPulse(0), m_dwExtraTick(timeGetTime()), m_dwLastTick(timeGetTime()), m_dwHeavyTrafficCount(0), m_TPPOverTwoTime(false)
{
	m_PulseTimerList.clear();
	SetTicksPerPulse(DEFAULT_TPP);
//	BASEDEBUGLOG("this:0x%08x.", this);
}

CPulse::~CPulse(void)
{
//	BASEDEBUGLOG("this:0x%08x.", this);	
	if ( m_PulseTimerList.size() != 0 ) {
		PulseTimerMapItr itr;
	
		for( itr=m_PulseTimerList.begin(); itr != m_PulseTimerList.end();) {
			delete (itr->second)->m_pDelegate;
			delete itr->second;
			itr	= m_PulseTimerList.erase( itr );
		}
	}
}

void CPulse::SetTicksPerPulse(DWORD dwTicksPerPulse)
{
	m_dwTicksPerPulse = dwTicksPerPulse;
}

DWORD CPulse::GetLeftTick(void)
{
	m_dwExtraTick = timeGetTime();
	DWORD dwElapsed = m_dwExtraTick - m_dwLastTick;

	if (dwElapsed >= m_dwTicksPerPulse)
		return 0;
	return m_dwTicksPerPulse - dwElapsed;
}

DWORD CPulse::CheckSleep(void)
{
	m_dwExtraTick = timeGetTime();
	DWORD dwElapsed = m_dwExtraTick - m_dwLastTick;

	if (dwElapsed <= m_dwTicksPerPulse)  {
		// �帥�ð��� TPP���� ���� ��� Sleep
		if (dwElapsed < m_dwTicksPerPulse)
			Sleep( m_dwTicksPerPulse - dwElapsed );

		m_dwLastTick = timeGetTime();
		m_dwHeavyTrafficCount = 0;

		m_TPPOverTwoTime = false;

	} else if (((++m_dwHeavyTrafficCount) > 5) || (dwElapsed > (m_dwTicksPerPulse * 2))) {
		// �帥 �ð��� TPP�� �ι躸�� ū ��� �Ǵ� ���������� Ʈ������ �ʹ� ���� ���
		WARNLOG2("Time Overflow : �帥 �ð��� %d �� TPP �� �ι躸�� ũ�ų�, �������� Ʈ������ %d �Դϴ�.", dwElapsed, m_dwHeavyTrafficCount);
		m_dwLastTick = timeGetTime();

		m_TPPOverTwoTime = true;
	} else {
		INFOLOG1("Ʈ������ %d �Դϴ�.", m_dwHeavyTrafficCount);
		// m_dwTicksPerPulse < dwElapsed <= m_dwTicksPerPulse * 2
		m_dwLastTick = timeGetTime() - (dwElapsed - m_dwTicksPerPulse);

		m_TPPOverTwoTime = false;
	}

	return ++m_dwPulse;
}

/*bool CPulse::InstallTimer(int nOperation, DWORD dwPulse, CProcedure* pClass, Operation pOperation)
{
	LPPULSE_TIMER newPulseTimer		= new PULSE_TIMER;

//	ASSERT( newPulseTimer != NULL );
	newPulseTimer->m_nOperationID	= nOperation;
	newPulseTimer->m_dwMaxPulse		= dwPulse;
	newPulseTimer->m_dwPulse		= 0;
	newPulseTimer->m_pClass			= pClass;
	newPulseTimer->m_pOperation		= pOperation;

	std::pair< PulseTimerMapItr, bool > PulseTimerMapPr;

	PulseTimerMapPr = m_PulseTimerList.insert( PulseTimerMap::value_type(nOperation, newPulseTimer) );
	if ( PulseTimerMapPr.second == false ) {
		delete newPulseTimer;
		return false;
	}
	return true;
}*/

bool CPulse::UninstallTimer(int nOperation)
{
	PulseTimerMapItr itr;
	
	if ( m_PulseTimerList.end() == (itr = m_PulseTimerList.find( (DWORD) nOperation )) )
		return false;
//	ASSERT( lpPulseTimer != NULL );
	delete (itr->second)->m_pDelegate;
	delete itr->second;
	m_PulseTimerList.erase( itr );
	return true;
}

void CPulse::Operate(void)
{
	for( PulseTimerMapItr itr=m_PulseTimerList.begin(); itr != m_PulseTimerList.end(); ++itr ) {
		LPPULSE_TIMER lpPulseTimer = itr->second;

//		ASSERT( lpPulseTimer != NULL );

		lpPulseTimer->m_dwPulse++;
		if ( lpPulseTimer->m_dwMaxPulse <= lpPulseTimer->m_dwPulse ) {
			lpPulseTimer->m_dwPulse = 0;
			//if ( lpPulseTimer->m_pOperation ) {
				//lpPulseTimer->m_pOperation();
			//	((lpPulseTimer->m_pClass)->*(lpPulseTimer->m_pOperation))();
			//}
			(*lpPulseTimer->m_pDelegate)();
		}
	}
}
