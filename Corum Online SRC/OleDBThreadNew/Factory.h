class CFOleDBThread : public IClassFactory
{
public:
	CFOleDBThread();
	~CFOleDBThread();

	// IUnknown 메서드
	HRESULT __stdcall QueryInterface(REFIID riid, LPVOID* ppv);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IClassFactory 메서드
	HRESULT __stdcall CreateInstance(
						LPUNKNOWN pUnkOuter, 
						REFIID riid, 
						LPVOID* ppv);
	HRESULT __stdcall LockServer(BOOL bLock);

private:
	DWORD m_cRef;		// Reference Counter
};
