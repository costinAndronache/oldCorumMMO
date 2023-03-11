#include "stdafx.h"
#include "Factory.h"
#include "OleDBThreadCls.h"

extern LONG g_cObjects;
extern LONG g_cLocks;

CFOleDBThread::CFOleDBThread()
{
	m_cRef = 0;
}

CFOleDBThread::~CFOleDBThread()
{

}

HRESULT __stdcall CFOleDBThread::QueryInterface(REFIID riid, LPVOID* ppv)
{
	HRESULT hr = E_NOINTERFACE;
	*ppv = NULL;

	if(riid == IID_IUnknown || riid == IID_IClassFactory)
		*ppv = this;

	if(*ppv != NULL) {
		AddRef();
		hr = NO_ERROR;
	}

	return hr;
}

ULONG __stdcall CFOleDBThread::AddRef(void)
{
	return ++m_cRef;
}

ULONG __stdcall CFOleDBThread::Release(void)
{
	if(--m_cRef == 0)
		delete this;
	return m_cRef;
}

HRESULT __stdcall CFOleDBThread::CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, LPVOID* ppv)
{
	HRESULT hr = E_FAIL;
	COleDBThreadCls* pOleDBThreadCls = NULL;
	*ppv = NULL;

	if(pUnkOuter != NULL)
		hr = CLASS_E_NOAGGREGATION;
	else
	{
		pOleDBThreadCls = new COleDBThreadCls;
		if(pOleDBThreadCls != NULL) 
		{
			hr = pOleDBThreadCls->QueryInterface(riid, ppv);
			if(FAILED(hr)) 
			{
				delete pOleDBThreadCls;
			}
		}
		else
			hr = E_OUTOFMEMORY;
	}

	return hr;
}

HRESULT __stdcall CFOleDBThread::LockServer(BOOL bLock)
{
	bLock;

	return S_OK;
}
