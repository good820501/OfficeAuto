// AXDocContainerView.h : interface of the CAXDocContainerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXDOCCONTAINERVIEW_H__3533A5D3_D3DF_11D1_8F0B_F54176DCF130__INCLUDED_)
#define AFX_AXDOCCONTAINERVIEW_H__3533A5D3_D3DF_11D1_8F0B_F54176DCF130__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CAXDocContainerCntrItem;

class CAXDocContainerView : public CView
{
protected: // create from serialization only
	CAXDocContainerView();
	DECLARE_DYNCREATE(CAXDocContainerView)

// Attributes
public:
	CAXDocContainerDoc* GetDocument();
	CAXDocContainerCntrItem* GetDocItem()
	{
		return GetDocument()->GetDocItem();
	}
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAXDocContainerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAXDocContainerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void ExecuteCommand(DWORD nCmdID, DWORD nCmdExecOpt);

// Generated message map functions
protected:
	//{{AFX_MSG(CAXDocContainerView)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInsertObject();
	afx_msg void OnCancelEditCntr();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintSetup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AXDocContainerView.cpp
inline CAXDocContainerDoc* CAXDocContainerView::GetDocument()
   { return (CAXDocContainerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXDOCCONTAINERVIEW_H__3533A5D3_D3DF_11D1_8F0B_F54176DCF130__INCLUDED_)
