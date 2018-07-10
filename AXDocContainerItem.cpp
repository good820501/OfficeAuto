// CntrItem.cpp : implementation of the CAXDocContainerCntrItem class
//
//Copyright Andrew A. Garbuzov

#include "stdafx.h"
#include "AXDocContainer.h"

#include "AXDocContainerDoc.h"
#include "AXDocContainerView.h"
#include "AXDocContainerItem.h"

#include <afxdocob.h>
#include <afxpriv.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerCntrItem implementation

IMPLEMENT_SERIAL(CAXDocContainerCntrItem, COleClientItem, 0)

BEGIN_INTERFACE_MAP(CAXDocContainerCntrItem, COleClientItem)
  INTERFACE_PART(CAXDocContainerCntrItem, IID_IOleDocumentSite, DocumentSite)
  INTERFACE_PART(CAXDocContainerCntrItem, IID_IOleInPlaceSite, OleIPSiteEx)
END_INTERFACE_MAP()


CAXDocContainerCntrItem::CAXDocContainerCntrItem(CAXDocContainerDoc* pContainer)
	: COleClientItem(pContainer)
{
	// TODO: add one-time construction code here
	m_pIOleDocView=0;	
}

CAXDocContainerCntrItem::~CAXDocContainerCntrItem()
{
	// TODO: add cleanup code here
	
}

	
IDispatch* CAXDocContainerCntrItem::GetIDispatch()
{
	ASSERT_VALID(this);
    ASSERT(m_lpObject != NULL);

    LPUNKNOWN lpUnk = m_lpObject;

    Run();    // must be running

    LPOLELINK lpOleLink = NULL;
    if (m_lpObject->QueryInterface(IID_IOleLink, 
        (LPVOID FAR*)&lpOleLink) == NOERROR)
    {
        ASSERT(lpOleLink != NULL);
        lpUnk = NULL;
        if (lpOleLink->GetBoundSource(&lpUnk) != NOERROR)
        {
            TRACE0("Warning: Link is not connected!\n");
            lpOleLink->Release();
            return NULL;
        }
        ASSERT(lpUnk != NULL);
    }

    LPDISPATCH lpDispatch = NULL;
    if (lpUnk->QueryInterface(IID_IDispatch, (void**)&lpDispatch) 
        != NOERROR)
    {
        TRACE0("Warning: does not support IDispatch!\n");
        return NULL;
    }

    ASSERT(lpDispatch != NULL);
    return lpDispatch;
}


void CAXDocContainerCntrItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	COleClientItem::OnChange(nCode, dwParam);

	// When an item is being edited (either in-place or fully open)
	//  it sends OnChange notifications for changes in the state of the
	//  item or visual appearance of its content.

	// TODO: invalidate the item by calling UpdateAllViews
	//  (with hints appropriate to your application)

	GetDocument()->UpdateAllViews(NULL);
		// for now just update ALL views/no hints
}

BOOL CAXDocContainerCntrItem::OnChangeItemPosition(const CRect& rectPos)
{
	ASSERT_VALID(this);

	// During in-place activation CAXDocContainerCntrItem::OnChangeItemPosition
	//  is called by the server to change the position of the in-place
	//  window.  Usually, this is a result of the data in the server
	//  document changing such that the extent has changed or as a result
	//  of in-place resizing.
	//
	// The default here is to call the base class, which will call
	//  COleClientItem::SetItemRects to move the item
	//  to the new position.

	if (!COleClientItem::OnChangeItemPosition(rectPos))
		return FALSE;

	// TODO: update any cache you may have of the item's rectangle/extent

	return TRUE;
}

void CAXDocContainerCntrItem::OnGetItemPosition(CRect& rPosition)
{
	ASSERT_VALID(this);

	// During in-place activation, CAXDocContainerCntrItem::OnGetItemPosition
	//  will be called to determine the location of this item.  The default
	//  implementation created from AppWizard simply returns a hard-coded
	//  rectangle.  Usually, this rectangle would reflect the current
	//  position of the item relative to the view used for activation.
	//  You can obtain the view by calling CAXDocContainerCntrItem::GetActiveView.

	// TODO: return correct rectangle (in pixels) in rPosition

	CView* pView = GetActiveView();
	ASSERT_VALID(pView);

	pView->GetClientRect(rPosition);
	
	CClientDC dc(pView);
	pView->OnPrepareDC(&dc, NULL);
	dc.DPtoLP(rPosition);
}

void CAXDocContainerCntrItem::OnActivate()
{
    // Allow only one inplace activate item per frame
    CAXDocContainerView* pView = GetActiveView();
    ASSERT_VALID(pView);
    COleClientItem* pItem = GetDocument()->GetInPlaceActiveItem(pView);
    if (pItem != NULL && pItem != this)
        pItem->Close();
    
    COleClientItem::OnActivate();
}

void CAXDocContainerCntrItem::OnDeactivateUI(BOOL bUndoable)
{
	COleClientItem::OnDeactivateUI(bUndoable);

	if(m_pIOleDocView)
		m_pIOleDocView->Release();	

    // Hide the object if it is not an outside-in object
    DWORD dwMisc = 0;
    m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);
    if (dwMisc & OLEMISC_INSIDEOUT)
        DoVerb(OLEIVERB_HIDE, NULL);
}

void CAXDocContainerCntrItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);
	COleClientItem::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerCntrItem diagnostics

#ifdef _DEBUG
void CAXDocContainerCntrItem::AssertValid() const
{
	COleClientItem::AssertValid();
}

void CAXDocContainerCntrItem::Dump(CDumpContext& dc) const
{
	COleClientItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(ULONG) CAXDocContainerCntrItem::XDocumentSite::AddRef()
{
	METHOD_PROLOGUE(CAXDocContainerCntrItem, DocumentSite)

	ULONG nResult = (ULONG)pThis->ExternalAddRef();

	return nResult;
}

STDMETHODIMP_(ULONG) CAXDocContainerCntrItem::XDocumentSite::Release()
{
	METHOD_PROLOGUE(CAXDocContainerCntrItem, DocumentSite)

	ULONG nResult = (ULONG)pThis->ExternalRelease();

	return nResult;
}

STDMETHODIMP CAXDocContainerCntrItem::XDocumentSite::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CAXDocContainerCntrItem, DocumentSite)

	HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);

	return hr;
}


STDMETHODIMP CAXDocContainerCntrItem::XDocumentSite::ActivateMe(IOleDocumentView *pView)
{
	METHOD_PROLOGUE(CAXDocContainerCntrItem, DocumentSite)

	CRect			rc;
    IOleDocument*	pDoc;
    
    /*
	* If we're passed a NULL view pointer, then try to get one from
	* the document object (the object within us).
	*/
    if (NULL==pView)
	{
		
        if (FAILED(pThis->m_lpObject->QueryInterface(IID_IOleDocument, (void **)&pDoc)))
            return E_FAIL;
		
        if (FAILED(pDoc->CreateView(&pThis->m_xOleIPSite, 0, 0, &pView)))            
            return E_OUTOFMEMORY;
		
        // Release doc pointer since CreateView is a good com method that addrefs
        pDoc->Release();
	}        
    else
	{
        //Make sure that the view has our client site
        pView->SetInPlaceSite(&pThis->m_xOleIPSiteEx);
		
        //We're holding onto the pointer, so AddRef it.
        pView->AddRef();
	}
	
	
    /*
	* Activation steps, now that we have a view:
	*
	*  1.  Call IOleDocumentView::SetInPlaceSite (assume done since
	*      either the view already knows, or IOleDocument::CreateView
	*      has done it already.
	*
	*  2.  Call IOleDocumentView::SetRect to give a bunch of space to
	*      the view.  In our case this is the whole client area of
	*      the CPages window.  (Patron doesn't use SetRectComplex)
	*
	*  3.  Call IOleDocumentView::Show to make the thing visible.
	*
	*  4.  Call IOleDocumentView::UIActivate to finish the job.
	*
	*/
	
    pThis->m_pIOleDocView=pView;
    
    //This sets up toolbars and menus first    
    pView->UIActivate(TRUE);
	
    //Set the window size sensitive to new toolbars
    pThis->GetActiveView()->GetClientRect(rc);
    pView->SetRect(&rc);
	
	//Makes it all active
    pView->Show(TRUE);    
    return NOERROR;
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CAXDocContFrameHook, COleFrameHook)
  INTERFACE_PART(CAXDocContFrameHook, IID_IOleCommandTarget, OleCommandTarget)
END_INTERFACE_MAP()


STDMETHODIMP_(ULONG) CAXDocContFrameHook::XOleCommandTarget::AddRef()
{
	METHOD_PROLOGUE_EX_(CAXDocContFrameHook, OleCommandTarget)

	ULONG nResult = (ULONG)pThis->ExternalAddRef();

	return nResult;
}

STDMETHODIMP_(ULONG) CAXDocContFrameHook::XOleCommandTarget::Release()
{
	METHOD_PROLOGUE_EX_(CAXDocContFrameHook, OleCommandTarget)

	ULONG nResult = (ULONG)pThis->ExternalRelease();

	return nResult;
}

STDMETHODIMP CAXDocContFrameHook::XOleCommandTarget::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CAXDocContFrameHook, OleCommandTarget)

	HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);

	return hr;
}

/*
 * IOleCommandTarget methods, provided to make PowerPoint happy
 * with this frame.
 */

STDMETHODIMP CAXDocContFrameHook::XOleCommandTarget::QueryStatus(  const GUID* pguidCmdGroup, ULONG cCmds, OLECMD rgCmds[],
   OLECMDTEXT* pcmdtext)
{
	METHOD_PROLOGUE_EX_(CAXDocContFrameHook, OleCommandTarget)

	HRESULT	hr=NOERROR;

	COleCmdUI state(rgCmds, cCmds, pguidCmdGroup);
	if (pcmdtext == NULL)
		state.m_nCmdTextFlag = 0;
	else
		state.m_nCmdTextFlag = pcmdtext->cmdtextf;
				
	for (state.m_nIndex = 0; state.m_nIndex < cCmds; state.m_nIndex++)
	{
		state.m_nID = rgCmds[state.m_nIndex].cmdID;
		state.DoUpdate(AfxGetMainWnd(), TRUE);
	}
				
	if (pcmdtext != NULL && pcmdtext->rgwz != NULL &&
		(pcmdtext->cmdtextf != OLECMDTEXTF_NONE))
	{
		USES_CONVERSION;
		ASSERT(cCmds == 1);
		state.m_strText = state.m_strText.Right(pcmdtext->cwBuf-1);
		pcmdtext->cwActual = state.m_strText.GetLength();
					
#ifdef _UNICODE
		lstrcpyW(pcmdtext->rgwz, (LPCTSTR) state.m_strText);
#elif defined(OLE2ANSI)
		lstrcpy(pcmdtext->rgwz, state.m_strText);
#else
		lstrcpyW(pcmdtext->rgwz, T2W((LPCTSTR) state.m_strText));
#endif
	}

	return hr;
}

        
STDMETHODIMP CAXDocContFrameHook::XOleCommandTarget::Exec(const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdExecOpt,
   VARIANTARG* pvarargIn, VARIANTARG* pvarargOut)
{
	METHOD_PROLOGUE_EX_(CAXDocContFrameHook, OleCommandTarget);

	HRESULT		hr=NOERROR;
	OLECMD		cmd;

	COleCmdUI	state(&cmd, 1, pguidCmdGroup);

	state.m_nIndex = 0;
	cmd.cmdf = 0;
	cmd.cmdID = nCmdID;
	state.m_nID = nCmdID;

	// help via Doc Object targeting is not supported

	if (nCmdExecOpt == OLECMDEXECOPT_SHOWHELP)
		hr = OLECMDERR_E_DISABLED;
	else
	{
		// is the command supported?

		if (!state.DoUpdate(AfxGetMainWnd(), TRUE))
			hr = OLECMDERR_E_NOTSUPPORTED;
		else
		{
			if (cmd.cmdf & OLECMDF_ENABLED)
			{
				if (AfxGetMainWnd()->OnCmdMsg(state.m_nID, CN_COMMAND, NULL, NULL))
					hr = S_OK;
				else
					hr = E_FAIL;
			}
			else
				hr = OLECMDERR_E_DISABLED;
		}
	}
	
	return hr;
}



/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerCntrItem::XOleIPSiteEx implementation

STDMETHODIMP_(ULONG) CAXDocContainerCntrItem::XOleIPSiteEx::AddRef()
{
	METHOD_PROLOGUE_EX_(CAXDocContainerCntrItem, OleIPSiteEx)
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CAXDocContainerCntrItem::XOleIPSiteEx::Release()
{
	METHOD_PROLOGUE_EX_(CAXDocContainerCntrItem, OleIPSiteEx)
	return pThis->ExternalRelease();
}

STDMETHODIMP CAXDocContainerCntrItem::XOleIPSiteEx::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CAXDocContainerCntrItem, OleIPSiteEx)
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CAXDocContainerCntrItem::XOleIPSiteEx::GetWindow(HWND* lphwnd)
{
	METHOD_PROLOGUE_EX_(CAXDocContainerCntrItem, OleIPSiteEx)

	*lphwnd = pThis->m_pView->GetSafeHwnd();
	return *lphwnd != NULL ? S_OK : E_FAIL;
}

STDMETHODIMP CAXDocContainerCntrItem::XOleIPSiteEx::ContextSensitiveHelp(
	BOOL fEnterMode)
{
	METHOD_PROLOGUE_EX_(CAXDocContainerCntrItem, OleIPSiteEx)

	if (pThis->m_pInPlaceFrame == NULL)
		return E_UNEXPECTED;

	// simply delegate to frame window implementation
	return pThis->m_pInPlaceFrame->
		m_xOleInPlaceFrame.ContextSensitiveHelp(fEnterMode);
}

STDMETHODIMP CAXDocContainerCntrItem::XOleIPSiteEx::CanInPlaceActivate()
{
	METHOD_PROLOGUE_EX(CAXDocContainerCntrItem, OleIPSiteEx)

	return pThis->CanActivate() ? S_OK : S_FALSE;
}

STDMETHODIMP CAXDocContainerCntrItem::XOleIPSiteEx::OnInPlaceActivate()
{
	METHOD_PROLOGUE_EX(CAXDocContainerCntrItem, OleIPSiteEx)
	ASSERT_VALID(pThis);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		pThis->OnActivate();
		sc = S_OK;
	}
	END_TRY

	return sc;
}

STDMETHODIMP CAXDocContainerCntrItem::XOleIPSiteEx::OnUIActivate()
{
	METHOD_PROLOGUE_EX(CAXDocContainerCntrItem, OleIPSiteEx)
	ASSERT_VALID(pThis);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		pThis->OnActivateUI();
		sc = S_OK;
	}
	END_TRY

	return sc;
}

STDMETHODIMP CAXDocContainerCntrItem::XOleIPSiteEx::GetWindowContext(
	LPOLEINPLACEFRAME* lplpFrame,
	LPOLEINPLACEUIWINDOW* lplpDoc,
	LPRECT lpPosRect, LPRECT lpClipRect,
	LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	METHOD_PROLOGUE_EX(CAXDocContainerCntrItem, OleIPSiteEx)
	ASSERT_VALID(pThis);

	*lplpFrame = NULL;  // init these in-case of mem-alloc failure
	*lplpDoc = NULL;

	CFrameWnd* pMainFrame = NULL;
	CFrameWnd* pDocFrame = NULL;

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		// get position of the item relative to activation view
		CRect rect;
		pThis->OnGetItemPosition(rect);
		::CopyRect(lpPosRect, &rect);
		pThis->OnGetClipRect(rect);
		::CopyRect(lpClipRect, &rect);

		// get the window context information
		if (pThis->OnGetWindowContext(&pMainFrame, &pDocFrame, lpFrameInfo))
		{
			// hook IOleInPlaceFrame interface to pMainFrame
			if (pThis->m_pInPlaceFrame == NULL)
				pThis->m_pInPlaceFrame = new CAXDocContFrameHook(pMainFrame, pThis);
			pThis->m_pInPlaceFrame->InternalAddRef();
			*lplpFrame = (LPOLEINPLACEFRAME)pThis->m_pInPlaceFrame->
				GetInterface(&IID_IOleInPlaceFrame);

			// save accel table for IOleInPlaceFrame::TranslateAccelerators
			pThis->m_pInPlaceFrame->m_hAccelTable = lpFrameInfo->haccel;

			// hook IOleInPlaceUIWindow to pDocFrame
			if (pDocFrame != NULL)
			{
				if (pThis->m_pInPlaceDoc == NULL)
					pThis->m_pInPlaceDoc = new CAXDocContFrameHook(pDocFrame, pThis);
				pThis->m_pInPlaceDoc->InternalAddRef();
				*lplpDoc = (LPOLEINPLACEUIWINDOW)pThis->m_pInPlaceDoc->
					GetInterface(&IID_IOleInPlaceUIWindow);
			}
			sc = S_OK;
		}
	}
	CATCH_ALL(e)
	{
		// cleanup memory that may be partially allocated
		delete *lplpFrame;
		ASSERT(*lplpDoc == NULL);
		e->Delete();
	}
	END_CATCH_ALL

	return sc;
}

STDMETHODIMP CAXDocContainerCntrItem::XOleIPSiteEx::Scroll(SIZE scrollExtent)
{
	METHOD_PROLOGUE_EX(CAXDocContainerCntrItem, OleIPSiteEx)
	ASSERT_VALID(pThis);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		if (!pThis->OnScrollBy(CSize(scrollExtent)))
			sc = S_FALSE;
		else
			sc = S_OK;
	}
	END_TRY

	return sc;
}

STDMETHODIMP CAXDocContainerCntrItem::XOleIPSiteEx::OnUIDeactivate(BOOL fUndoable)
{
	METHOD_PROLOGUE_EX(CAXDocContainerCntrItem, OleIPSiteEx)
	ASSERT_VALID(pThis);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		pThis->OnDeactivateUI(fUndoable);
		sc = S_OK;
	}
	END_TRY

	return sc;
}

STDMETHODIMP CAXDocContainerCntrItem::XOleIPSiteEx::OnInPlaceDeactivate()
{
	METHOD_PROLOGUE_EX(CAXDocContainerCntrItem, OleIPSiteEx)
	ASSERT_VALID(pThis);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		pThis->OnDeactivate();
		sc = S_OK;
	}
	END_TRY

	return sc;
}

STDMETHODIMP CAXDocContainerCntrItem::XOleIPSiteEx::DiscardUndoState()
{
	METHOD_PROLOGUE_EX(CAXDocContainerCntrItem, OleIPSiteEx)
	ASSERT_VALID(pThis);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		pThis->OnDiscardUndoState();
		sc = S_OK;
	}
	END_TRY

	return sc;
}

STDMETHODIMP CAXDocContainerCntrItem::XOleIPSiteEx::DeactivateAndUndo()
{
	METHOD_PROLOGUE_EX(CAXDocContainerCntrItem, OleIPSiteEx)
	ASSERT_VALID(pThis);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		pThis->OnDeactivateAndUndo();
		sc = S_OK;
	}
	END_TRY

	return sc;
}

STDMETHODIMP CAXDocContainerCntrItem::XOleIPSiteEx::OnPosRectChange(
	LPCRECT lpPosRect)
{
	METHOD_PROLOGUE_EX(CAXDocContainerCntrItem, OleIPSiteEx)
	ASSERT_VALID(pThis);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		CRect rect;
		rect.CopyRect(lpPosRect);
		pThis->OnChangeItemPosition(rect);
		sc = S_OK;
	}
	END_TRY

	return sc;
}

/////////////////////////////////////////////////////////////////////////////
