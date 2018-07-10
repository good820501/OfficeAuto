#if !defined(AFX_SCRIPTDOCUMENT_H__18990D62_3474_11D2_8F0B_F54176DCF130__INCLUDED_)
#define AFX_SCRIPTDOCUMENT_H__18990D62_3474_11D2_8F0B_F54176DCF130__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ScriptDocument.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScriptDocument document

class CScriptDocument : public CDocument
{
protected:
	CScriptDocument();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScriptDocument)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptDocument)
	public:
	virtual void OnFinalRelease();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScriptDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CScriptDocument)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CScriptDocument)
	afx_msg LPDISPATCH OleOpenDocumentFile(LPCTSTR strFileName);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTDOCUMENT_H__18990D62_3474_11D2_8F0B_F54176DCF130__INCLUDED_)
