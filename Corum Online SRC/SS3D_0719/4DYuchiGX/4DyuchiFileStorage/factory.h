#pragma once

#include "../4DyuchiGRX_common/stdafx.h"

/**
 * \ingroup 4DyuchiFileStorage
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 2005-03-16
 *
 * \author Àüº´·Ä
 *
 * \par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 * 
 * \todo 
 *
 * \bug 
 *
 */
class CF4DyuchiFileStorage : public IClassFactory
{
public:    
    /**
     *
     * \param refiid 
     * \param ppv 
     * \return 
     */
    STDMETHODIMP			QueryInterface(REFIID refiid, PPVOID ppv);
	/**
	 *
	 * \param ULONG 
	 * \return 
	 */
	STDMETHODIMP_(ULONG)	AddRef(void);
    /**
     *
     * \param ULONG 
     * \return 
     */
    STDMETHODIMP_(ULONG)	Release(void);
    /**
     *
     * \param IUnknown* 
     * \param REFIID 
     * \param PPVOID 
     * \return 
     */
    STDMETHODIMP			CreateInstance(IUnknown*, REFIID, PPVOID);
    /**
     *
     * \param BOOL 
     * \return 
     */
    STDMETHODIMP			LockServer(BOOL);
	/**
	 *
	 * \return 
	 */
	CF4DyuchiFileStorage();
	/**
	 *
	 * \return 
	 */
	~CF4DyuchiFileStorage();
	
private:
	DWORD					m_dwRefCount;

};
