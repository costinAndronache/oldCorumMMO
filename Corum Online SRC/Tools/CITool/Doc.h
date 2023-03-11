#if !defined(AFX_DOC_H__0AF3FA69_7F2F_4DB8_8E2C_F303FA809F4D__INCLUDED_)
#define AFX_DOC_H__0AF3FA69_7F2F_4DB8_8E2C_F303FA809F4D__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


class CDoc : public CDocument
{

protected: 
	
	CDoc();
	DECLARE_DYNCREATE(CDoc)

public:

	//{{AFX_VIRTUAL(CDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

public:
	virtual ~CDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	//{{AFX_MSG(CDoc)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:

	int		m_nDocID;
	int		m_nDocID2;
};

#endif 
