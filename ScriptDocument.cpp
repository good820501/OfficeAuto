// ScriptDocument.cpp : implementation file
//
//Copyright Andrew A. Garbuzov

#include "stdafx.h"
#include "AXDocContainer.h"
#include "ScriptDocument.h"

#include "AXDocContainerDoc.h"
#include "AXDocContainerItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptDocument

IMPLEMENT_DYNCREATE(CScriptDocument, CDocument)

CScriptDocument::CScriptDocument()
{
	EnableAutomation();
}

BOOL CScriptDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CScriptDocument::~CScriptDocument()
{
}

void CScriptDocument::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDocument::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CScriptDocument, CDocument)
	//{{AFX_MSG_MAP(CScriptDocument)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CScriptDocument, CDocument)
	//{{AFX_DISPATCH_MAP(CScriptDocument)
	DISP_FUNCTION(CScriptDocument, "OpenDocumentFile", OleOpenDocumentFile, VT_DISPATCH, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IScriptDocument to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {18990D61-3474-11D2-8F0B-F54176DCF130}
static const IID IID_IScriptDocument =
{ 0x18990d61, 0x3474, 0x11d2, { 0x8f, 0xb, 0xf5, 0x41, 0x76, 0xdc, 0xf1, 0x30 } };

BEGIN_INTERFACE_MAP(CScriptDocument, CDocument)
	INTERFACE_PART(CScriptDocument, IID_IScriptDocument, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptDocument diagnostics

#ifdef _DEBUG
void CScriptDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CScriptDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScriptDocument serialization

void CScriptDocument::Serialize(CArchive& ar)
{
	POSITION pos=GetFirstViewPosition();
	((CEditView*)GetNextView(pos))->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CScriptDocument commands

LPDISPATCH CScriptDocument::OleOpenDocumentFile(LPCTSTR strFileName) 
{
	// TODO: Add your dispatch handler code here
	CDocument* pDoc=AfxGetApp()->OpenDocumentFile(strFileName);
	if(pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CAXDocContainerDoc)))
	{
		CAXDocContainerDoc* pContDoc=(CAXDocContainerDoc*)pDoc;
		CAXDocContainerCntrItem* pItem=pContDoc->GetDocItem();

		if(pItem)
		{
			return pItem->GetIDispatch();
		}
	}

	return NULL;
}
