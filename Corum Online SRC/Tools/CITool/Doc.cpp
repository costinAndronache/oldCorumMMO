#include "stdafx.h"
#include "NewSerial.h"
#include "Doc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CDoc, CDocument)


BEGIN_MESSAGE_MAP(CDoc, CDocument)
	//{{AFX_MSG_MAP(CDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
CDoc::CDoc()
{
	m_nDocID	= 0;
	m_nDocID2	= 0;
}


CDoc::~CDoc()
{

}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		
	}
	else
	{
		
	}
}


#ifdef _DEBUG
void CDoc::AssertValid() const
{
	CDocument::AssertValid();
}
void CDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif 
