// CntrItem.h : interface of the CAXDocContainerCntrItem class
//

#if !defined(AFX_CNTRITEM_H__3533A5D6_D3DF_11D1_8F0B_F54176DCF130__INCLUDED_)
#define AFX_CNTRITEM_H__3533A5D6_D3DF_11D1_8F0B_F54176DCF130__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CAXDocContainerDoc;
class CAXDocContainerView;

#include "oleimpl2.h"

class CAXDocContFrameHook : public COleFrameHook
{
public:
	CAXDocContFrameHook(CFrameWnd* pFrameWnd, COleClientItem* pItem) :
	  COleFrameHook(pFrameWnd, pItem)
	  {
	  }

		 
	// Interface Maps
	DECLARE_INTERFACE_MAP()

	BEGIN_INTERFACE_PART(OleCommandTarget, IOleCommandTarget)
		INIT_INTERFACE_PART(CAXDocContFrameHook,OleCommandTarget);
		STDMETHOD(QueryStatus)(const GUID*, ULONG, OLECMD[], OLECMDTEXT*);
		STDMETHOD(Exec)(const GUID*, DWORD, DWORD, VARIANTARG*, VARIANTARG*);
	END_INTERFACE_PART(OleCommandTarget)
};




class CAXDocContainerCntrItem : public COleClientItem
{
	friend class CAXDocContainerView;

	DECLARE_SERIAL(CAXDocContainerCntrItem)

// Constructors
public:
	CAXDocContainerCntrItem(CAXDocContainerDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CAXDocContainerDoc* GetDocument()
		{ return (CAXDocContainerDoc*)COleClientItem::GetDocument(); }
	CAXDocContainerView* GetActiveView()
		{ return (CAXDocContainerView*)COleClientItem::GetActiveView(); }

	
	void SetDocViewRect(CRect& rect)
	{
		m_pIOleDocView->SetRect(&rect);
	}

	IOleDocumentView* GetIOleDocumentView() const
	{
		return m_pIOleDocView;
	}

	IDispatch* GetIDispatch();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAXDocContainerCntrItem)
	public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();
	protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CAXDocContainerCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);

	// Interface Maps
	DECLARE_INTERFACE_MAP()
		
	// IOleDocumentSite
	BEGIN_INTERFACE_PART(DocumentSite, IOleDocumentSite)
		STDMETHODIMP ActivateMe(IOleDocumentView *);
	END_INTERFACE_PART(DocumentSite)

	BEGIN_INTERFACE_PART(OleIPSiteEx, IOleInPlaceSite)
		INIT_INTERFACE_PART(COleClientItem, OleIPSiteEx)
		STDMETHOD(GetWindow)(HWND*);
		STDMETHOD(ContextSensitiveHelp)(BOOL);
		STDMETHOD(CanInPlaceActivate)();
		STDMETHOD(OnInPlaceActivate)();
		STDMETHOD(OnUIActivate)();
		STDMETHOD(GetWindowContext)(LPOLEINPLACEFRAME*,
			LPOLEINPLACEUIWINDOW*, LPRECT, LPRECT, LPOLEINPLACEFRAMEINFO);
		STDMETHOD(Scroll)(SIZE);
		STDMETHOD(OnUIDeactivate)(BOOL);
		STDMETHOD(OnInPlaceDeactivate)();
		STDMETHOD(DiscardUndoState)();
		STDMETHOD(DeactivateAndUndo)();
		STDMETHOD(OnPosRectChange)(LPCRECT);
	END_INTERFACE_PART(OleIPSiteEx)

	IOleDocumentView  *m_pIOleDocView;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__3533A5D6_D3DF_11D1_8F0B_F54176DCF130__INCLUDED_)
