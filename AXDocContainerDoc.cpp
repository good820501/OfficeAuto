// AXDocContainerDoc.cpp : implementation of the CAXDocContainerDoc class
//
//Copyright Andrew A. Garbuzov

#include "stdafx.h"
#include "AXDocContainer.h"

#include "AXDocContainerDoc.h"
#include "AXDocContainerItem.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAXDocInfoArray CAXDocContainerDoc::m_arrayDocInfo;

/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerDoc

IMPLEMENT_DYNCREATE(CAXDocContainerDoc, COleDocument)

BEGIN_MESSAGE_MAP(CAXDocContainerDoc, COleDocument)
	//{{AFX_MSG_MAP(CAXDocContainerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, COleDocument::OnUpdatePasteMenu)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, COleDocument::OnUpdatePasteLinkMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_CONVERT, COleDocument::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_OLE_EDIT_CONVERT, COleDocument::OnEditConvert)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, COleDocument::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, COleDocument::OnEditLinks)
	ON_UPDATE_COMMAND_UI(ID_OLE_VERB_FIRST, COleDocument::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAXDocContainerDoc, COleDocument)
	//{{AFX_DISPATCH_MAP(CAXDocContainerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IAXDocContainer to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {3533A5C5-D3DF-11D1-8F0B-F54176DCF130}
static const IID IID_IAXDocContainer =
{ 0x3533a5c5, 0xd3df, 0x11d1, { 0x8f, 0xb, 0xf5, 0x41, 0x76, 0xdc, 0xf1, 0x30 } };

BEGIN_INTERFACE_MAP(CAXDocContainerDoc, COleDocument)
	INTERFACE_PART(CAXDocContainerDoc, IID_IAXDocContainer, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerDoc construction/destruction

CAXDocContainerDoc::CAXDocContainerDoc()
{
	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here
	EnableAutomation();

	AfxOleLockApp();
}

CAXDocContainerDoc::~CAXDocContainerDoc()
{
	AfxOleUnlockApp();
}

BOOL CAXDocContainerDoc::OnNewDocument()
{
	if (!COleDocument::OnNewDocument())
		return FALSE;

	// Create new item connected to this document.
	CAXDocContainerCntrItem* pItem = NULL;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	CAXDocContainerTemplate* pTmpl=(CAXDocContainerTemplate*)GetDocTemplate();
	if(pTmpl)
	{
		CWaitCursor wc;
		TRY
		{
			pItem = new CAXDocContainerCntrItem(this);
			
			// Initialize the item from the dialog data.
			if (!pItem->CreateNewItem(pTmpl->m_clsid))
				AfxThrowMemoryException();  // any exception will do
			
			ASSERT_VALID(pItem);

			return TRUE;
		}
		CATCH(CException, e)
		{
			if (pItem != NULL)
			{
				ASSERT_VALID(pItem);
				pItem->Delete();
			}
			AfxMessageBox(IDP_FAILED_TO_CREATE);
			return FALSE;
		}
		END_CATCH
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerDoc diagnostics

#ifdef _DEBUG
void CAXDocContainerDoc::AssertValid() const
{
	COleDocument::AssertValid();
}

void CAXDocContainerDoc::Dump(CDumpContext& dc) const
{
	COleDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerDoc commands

BOOL CAXDocContainerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized creation code here

	// Create new item connected to this document.
	CAXDocContainerCntrItem* pItem = NULL;

	CWaitCursor wc;
	TRY
	{
		pItem = new CAXDocContainerCntrItem(this);

		m_bEmbedded=TRUE;	//To avoid the assertion failed mesage
		
		// Initialize the item from the dialog data.
		if (!pItem->CreateFromFile(lpszPathName))
			AfxThrowMemoryException();  // any exception will do
			
		SetPathName(lpszPathName);
		ASSERT_VALID(pItem);
		return TRUE;
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			ASSERT_VALID(pItem);
			pItem->Delete();
		}

		AfxMessageBox(IDP_FAILED_TO_CREATE);
		
	}
	END_CATCH

	return FALSE;
}


CAXDocContainerCntrItem* CAXDocContainerDoc::GetDocItem()
{
	POSITION pos=GetStartPosition();
	if(pos)
	{
		CDocItem* pItem=GetNextItem(pos);
		if(pItem->IsKindOf(RUNTIME_CLASS(CAXDocContainerCntrItem)))
			return (CAXDocContainerCntrItem*)pItem;
	}

	return NULL;
}

BOOL CAXDocContainerDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	USES_CONVERSION;

	WCHAR* wcPathName=T2W(lpszPathName);
	
	IStorage* pStorage=NULL;
	BOOL bSuccess=FALSE;
	if(SUCCEEDED(StgCreateDocfile(wcPathName,STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_CREATE,0,&pStorage)))
	{
		IPersistStorage* pPersistStorage=NULL;
		if(SUCCEEDED(GetDocItem()->m_lpObject->QueryInterface(IID_IPersistStorage,(void**)&pPersistStorage)))
		{
			if(SUCCEEDED(OleSave(pPersistStorage,pStorage,FALSE)))
			{
				pPersistStorage->SaveCompleted(NULL);
				bSuccess=TRUE;
			}

			pPersistStorage->Release();
		}

		pStorage->Release();
	}

	return bSuccess;
}

