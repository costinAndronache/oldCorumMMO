// HSIME.h: interface for the CHSIME class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSIME_H__17714882_2106_46F5_9F6E_40C38C80E4FF__INCLUDED_)
#define AFX_HSIME_H__17714882_2106_46F5_9F6E_40C38C80E4FF__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>
#include <imm.h>
#pragma comment(lib,"imm32.lib")

#pragma warning(disable:4786)

#include <string>
#include <map>
#include <hash_map>
#include <vector>

using namespace std;
using namespace stdext;

namespace HSIME
{
	typedef struct CARETDATA
	{
		__int32 iCaretPos;
		__int32 iSelectStart;
		__int32 iSelectEnd;
		__int32 iImeCursor;
		__int32 iImeDeltaStart;
		CARETDATA()
		{
			iCaretPos = 0;
			iSelectStart = 0;
			iSelectEnd = 0;
			iImeCursor = 0;
			iImeDeltaStart = 0;
		}
	}t_CaretData,*LPCARETDATA;

	class CCandidateData
	{
	public:
		CCandidateData(const int iIndex, const string &str)
		{
			m_iIndex  = iIndex;
			m_strCadidate = str;
		};
		~CCandidateData(){};

		__int32 GetIndex() const {return m_iIndex;}
		const string& GetString() const {return m_strCadidate;}
	
	private:
		__int32 m_iIndex;
		string	m_strCadidate;
	};

	class CHSIME  
	{
	public:
		enum eKeyState
		{
			KEY_STATE_DOWN	= 1,
			KEY_STATE_UP	= 2,
		};

		typedef vector<CCandidateData> CANDI_BUF;
		typedef CANDI_BUF::iterator CANDI_ITOR;
		typedef CANDI_BUF::const_iterator CANDI_CONST_ITOR;

		typedef __int32 (CALLBACK *LPKBFUNC)(const eKeyState KeyState);//Ű���尡 ���������� ȣ�� �Ǵ� �Լ�
		
		typedef hash_map<__int32, LPKBFUNC> KB_HASH;
		typedef KB_HASH::iterator KBIter;

		enum eLANGUAGE_TYPE
		{
			LT_DEFAULT = 0,
			LT_TRADITIONAL_CHINESE,	
			LT_JAPANESE,
			LT_KOREAN,
			LT_SIMPLIFIED_CHINESE
		}; 
	public:
		CHSIME();
		~CHSIME();
	
	public:
		const bool Init(const HWND hParentWnd, const HINSTANCE hInstance, const WNDPROC lpProc);

		const bool SetFocus();//EditWnd�� ��Ŀ���� ����. ��Ŀ���� ������ ���� �Է� �Ұ���.

		const bool IsShowCandidate() const {return m_bIsShowCandidate;}//�����찡 �����ִ� �⺻â�� �� �� �ְ� �� ���ΰ�?
		const bool IsSymbolMode() const {return m_bIsSymbolMode;}//�Է±� ������ �ɺ� �Է� ���
		const bool IsFullShape() const {return m_bIsFullShape;}//����, ���� ���
		const bool IsOnlyNumeric() const {return m_bIsNumeric;}//���� �Է� ����ΰ�? 
		const bool IsMultiLine() const {return m_bIsMultiLine;}//���� �Է��� �޴°�?(��Ƽ������ ���°�)
		const bool IsNowComp() const {return m_bIsComp;}//���� �������ΰ�?

	public:
		const string& GetIMEName() const { return m_strIMEName; }//���� �Է±��� �̸��� ����
		const CARETDATA& GetCaretPos();
		const eLANGUAGE_TYPE GetLanguageType() const{ return m_eLanguageType; }//���Ÿ���� ����. eLANGUAGE_TYPE�� ����

		DWORD GetLimitLength() const {return m_dwLimitLength;}
		DWORD GetMaxLineCount() const {return m_dwMaxLineCount;}
		
	public://���ڿ� ����
		const string& GetResultStr();
		const string& GetCompStr()  const { return m_strCompStr; }//������ ������� ����
		const string& GetCompReadStr() const { return m_strCompReadStr; }//�������� ������ ���ڸ� ����

//		void GetCadidateData(CANDI_BUF &vtOut){	vtOut = m_vtCandlist; }//Candidate�� ����
		
		const CANDI_BUF& GetCadidateData() const {return m_vtCandlist;}//Candidate�� ����

		LPCANDIDATELIST	GetCandiPoint();//Cadndidate ����Ʈ�� ������ m_lpCL �� ����

		void SetString(const char* szText);//������ ������ ����. IsOnlyNumeric�ε� ���ڰ� �ƴ� ���ڸ� ������ ""�� ���� ��

	public://��� ����
		void SetShowCandidate(const bool bIsShow) { m_bIsShowCandidate = bIsShow; }
		void SetEnableIME(const bool bIsEnable);
		void SetEnableIME(const bool bIsEnable, HWND hWnd, HIMC &hIMC);//�ش� �������� IME �ѱ� ����
		void SetNextIME();//�ٸ� �Է±�� ��ȯ.
		void SetNativeIME();//�ڱ��� �Է±�� ��ȯ.
		void SetEnglishIME(const bool bIsForce);//���� �Է±�� ��ȯ
		
		void SetDisableEnglish();//�����Է� ������ ���� �Ǿ��� ��� Ǯ����

		void SetShapeIME();//��/���� ��� ��ȯ
		void SetSymbolIME();//�ɺ� ���� ��ȯ
		void SetCaretPos(const bool bIsHome);//ĳ���� �Ǿ� �Ǵ� �ǵڷ� ����
		void SetCaretPos(const __int32 iStartPos, const __int32 iEndPos );//ĳ���� ���� ����, (iStartPos = 0 , iEndPos = -1 )--> ��� �������� //ĳ���� iEndPos �� ����
		void SetOnlyNumeric( const bool bIsNumeric, const bool bDoClear = true );//���� ���� ���� ����
		void SetMultiLine(const bool bIsMultiLine, const DWORD dwMaxCaretPerLine= 1, const DWORD dwMaxLineConut = 1, const bool bDoClear = true);
		
		void SetLimitLength(const DWORD dwLen, const bool bDoClear = true);//�Է¹޴� �ִ� ���� ����. 
		
	public://Ű���� �̺�Ʈ ���ε�
		bool FuncBind(const __int32 iIndex, const LPKBFUNC lpFunc);//�Լ� ���ε�
		
	private://Ű���� �̺�Ʈ �Լ� ����
		__int32 Excute(const __int32 iIndex, const eKeyState KeyState);

	public://Edit Wnd ���ν������� ���
		bool CALLBACK HandleMessage(HWND &hWnd, UINT &message, WPARAM &wParam, LPARAM &lParam);//������ ���ν��� ���� �ҷ����� �ݹ�

#ifdef STRICT
		WNDPROC	GetParentProc(){return m_pParentProc;}
#else
		FARPROC	GetParentProc(){return m_pParentProc;}
#endif

	private:
		LPCANDIDATELIST m_lpCL;	//Candidate���� ������
		CANDI_BUF m_vtCandlist;
		KB_HASH m_mapKBHash;

		bool m_bIsShowCandidate;//Candidate�����츦 ���̰� �ϴ� �÷��� 
		bool m_bIsSymbolMode;//�ɺ� ������� �˾� ���� �÷���
		bool m_bIsFullShape;//��/���� �÷���
		bool m_bIsNumeric;//�������� ��� �÷���
		bool m_bIsMultiLine;//���� �Է��� ���� ���ΰ�?
		bool m_bIsComp;//���� �������ΰ�?
		
		string m_strIMEName;//IME �� �̸�.
		
		CARETDATA	m_CaretData;//����Ʈ�� ĳ���� ��ġ
		
		eLANGUAGE_TYPE m_eLanguageType;
	
	private://���ڿ�
		string m_strCompStr;//�������� ���ڰ� �����
		string m_strCompReadStr;//������ ���ؼ� ������ �ִ� ���ڿ�
		string m_strResult;//�����. ����Ʈ �ڽ��� ��ϵǴ� ���� ��Ʈ��
		DWORD m_dwLimitLength;//�ִ� �Է� ���� �����ÿ� MAX_PATH�� ����
		DWORD m_dwMaxCaretPerLine;//��Ƽ���ν� 1�ٴ� �ִ� ĳ��
		DWORD m_dwMaxLineCount;//��Ƽ���ν� ����ī��Ʈ

	private:
		bool m_bIsForceEnglishIME;
		HWND m_hEWnd;//�Է��� �ް��ִ� IME �������� �ڵ� //Init �ÿ� ����.
		HIMC m_hImcSave;//IME Enable/Disable ������ �ִ� IME �ڵ� ����뺯��

	private:
		vector<char> m_vecCandListBuf;

#ifdef STRICT
		WNDPROC	m_pParentProc;
#else
		FARPROC	m_pParentProc;
#endif

	private:
		void Clear();
		void ClearCadndidateBuf();//���� ���۸� ����

		const HWND GetEditHWND() const {return m_hEWnd;}

		void SetIMEName(const char* szIMEName) { m_strIMEName = szIMEName; }//���ο����� ��� 
		void SetConversionStatus( const HWND &hWnd, const bool bIsOn = true, const DWORD dwNewConvMode = 0, const DWORD dwNewSentMode = 0);//�Է±��� ��� ��ȯ 
		void SetLanguageType();//�Է±��� ���Ÿ�� ����

		void AllocCandList( const DWORD dwSize );//
		void FreeCandList();

		bool SetIMECandiDateList(const HWND hWnd);//m_lpCL �� ���� ���� �Ѵ�.
//		bool SetIMEConversionList(const HWND hWnd);//m_lpCL �� ���� ���� �Ѵ�.

		bool IsCorrectString(const string& strSrc);
		bool IsCanInputAtMultiLine(const string& strSrc, const int nVirtKey);

		void GetWindowText(string &str);

		bool CorrectAlphaNumeric(string &str);
		
		LRESULT	CALLBACK OnWM_IME_SETCONTEXT(const HWND &hWnd, const UINT &message, WPARAM &wParam, LPARAM &lParam);
		LRESULT	CALLBACK OnWM_IME_COMPOSITION(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);//�������� ��.
		LRESULT	CALLBACK OnWM_IME_NOTIFY(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
		LRESULT	CALLBACK OnWM_INPUTLANGCHANGEREQUEST(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
		LRESULT	CALLBACK OnWM_INPUTLANGCHANGE(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
		LRESULT	CALLBACK OnWM_IME_STARTCOMPOSITION(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
		LRESULT	CALLBACK OnWM_IME_ENDCOMPOSITION(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
		LRESULT	CALLBACK OnWM_IME_CONTROL(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
		LRESULT	CALLBACK OnWM_CHAR(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
		LRESULT	CALLBACK OnWM_IME_SELECT(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
	};
}
//�Ұ�. �Է±� �����.
//������ �Է±��� ���ڸ���Ʈ �̱�.
//�Է¿� �̻��� ���� �ϱ�. 
#endif // !defined(AFX_HSIME_H__17714882_2106_46F5_9F6E_40C38C80E4FF__INCLUDED_)
