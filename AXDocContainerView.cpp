// AXDocContainerView.cpp : implementation of the CAXDocContainerView class
//
//Copyright Andrew A. Garbuzov

#include "stdafx.h"
#include "AXDocContainer.h"

#include "AXDocContainerDoc.h"
#include "AXDocContainerItem.h"
#include "AXDocContainerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerView

IMPLEMENT_DYNCREATE(CAXDocContainerView, CView)

BEGIN_MESSAGE_MAP(CAXDocContainerView, CView)
	//{{AFX_MSG_MAP(CAXDocContainerView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerView construction/destruction

CAXDocContainerView::CAXDocContainerView()
{
	// TODO: add construction code here
}

CAXDocContainerView::~CAXDocContainerView()
{
}


/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerView drawing

void CAXDocContainerView::OnDraw(CDC* pDC)
{
	CAXDocContainerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

void CAXDocContainerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: remove this code when final selection model code is written
	CAXDocContainerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(GetDocItem())
	{
		GetDocItem()->DoVerb(0,this);
		
		CRect rect;
		GetClientRect(rect);
		GetDocItem()->SetDocViewRect(rect);
	}
}

void CAXDocContainerView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
   CView::OnDestroy();
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
}


/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

BOOL CAXDocContainerView::IsSelected(const CObject* pDocItem) const
{
	// The implementation below is adequate if your selection consists of
	//  only CAXDocContainerCntrItem objects.  To handle different selection
	//  mechanisms, the implementation here should be replaced.

	// TODO: implement this function that tests for a selected OLE client item
	CObject* pFirst=(CObject*)pDocItem;
	CObject* pSecond=(CObject*)((CAXDocContainerView*)this)->GetDocItem();
	return pFirst == pSecond;
}


// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place.
void CAXDocContainerView::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// need to set focus to this item if it is in the same view
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();   // don't call the base class
			return;
		}
	}

	CView::OnSetFocus(pOldWnd);
}

void CAXDocContainerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CAXDocContainerCntrItem* pActiveItem = GetDocItem();
	if (pActiveItem != NULL)
	{
		CRect rect(CPoint(0,0),CSize(cx,cy));
		pActiveItem->SetDocViewRect(rect);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerView diagnostics

#ifdef _DEBUG
void CAXDocContainerView::AssertValid() const
{
	CView::AssertValid();
}

void CAXDocContainerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAXDocContainerDoc* CAXDocContainerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAXDocContainerDoc)));
	return (CAXDocContainerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerView message handlers

void CAXDocContainerView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if(GetDocItem())
	{
		IOleInPlaceActiveObject* pOIAObj=NULL;
		if(SUCCEEDED(GetDocItem()->m_lpObject->QueryInterface(IID_IOleInPlaceActiveObject,(void**)&pOIAObj)))
		{
			if(pActivateView==pDeactiveView && pDeactiveView==this)
				pOIAObj->OnFrameWindowActivate(bActivate);	
			else if((bActivate==TRUE && pActivateView==this) || (bActivate==FALSE && pDeactiveView==this))
				pOIAObj->OnDocWindowActivate(bActivate);
			pOIAObj->Release();
		}
	}
}

void CAXDocContainerView::ExecuteCommand(DWORD nCmdID, DWORD nCmdExecOpt)
{
	IOleCommandTarget* pCT=NULL;

	IOleDocument* pDocument=NULL;
	HRESULT hr=GetDocItem()->m_lpObject->QueryInterface(IID_IOleDocument, (void**)&pDocument);
	if(SUCCEEDED(hr))
	{
		hr=pDocument->QueryInterface(IID_IOleCommandTarget,(void**)&pCT);
		if(SUCCEEDED(hr))
		{
			hr=pCT->Exec(NULL,nCmdID,nCmdExecOpt,NULL,NULL);
			pCT->Release();
		}

		pDocument->Release();
	}
}


void CAXDocContainerView::OnFilePrintPreview() 
{
	// TODO: Add your command handler code here
	ExecuteCommand(OLECMDID_PRINTPREVIEW, OLECMDEXECOPT_DODEFAULT);
}

void CAXDocContainerView::OnFilePrint() 
{
	// TODO: Add your command handler code here
	ExecuteCommand(OLECMDID_PRINT, OLECMDEXECOPT_DODEFAULT);
}

void CAXDocContainerView::OnFilePrintSetup() 
{
	// TODO: Add your command handler code here
	ExecuteCommand(OLECMDID_PAGESETUP, OLECMDEXECOPT_DODEFAULT);
}

