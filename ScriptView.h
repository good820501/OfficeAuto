#if !defined(AFX_SCRIPTVIEW_H__18990D64_3474_11D2_8F0B_F54176DCF130__INCLUDED_)
#define AFX_SCRIPTVIEW_H__18990D64_3474_11D2_8F0B_F54176DCF130__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ScriptView.h : header file
//

//#include "ScriptDocEngine.h"
//#include "ScriptDocument.h"

/////////////////////////////////////////////////////////////////////////////
// CScriptView view

class CScriptView : public CEditView
{
protected:
	CScriptView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScriptView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScriptView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CScriptView)
	afx_msg void OnFileRun();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//	CScriptDocEngine	m_ScriptEngine;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTVIEW_H__18990D64_3474_11D2_8F0B_F54176DCF130__INCLUDED_)
