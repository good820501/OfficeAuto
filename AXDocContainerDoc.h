// AXDocContainerDoc.h : interface of the CAXDocContainerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXDOCCONTAINERDOC_H__3533A5D1_D3DF_11D1_8F0B_F54176DCF130__INCLUDED_)
#define AFX_AXDOCCONTAINERDOC_H__3533A5D1_D3DF_11D1_8F0B_F54176DCF130__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

struct CAXDocInfo
{
	GUID	m_clsid;
	CString	m_strDocStrings;
};

class CAXDocInfoArray : public CTypedPtrArray<CPtrArray,CAXDocInfo*>
{
public:
	~CAXDocInfoArray()
	{
		Clear();
	}
	void Clear();
	void LoadFromRegistry();
};


class CAXDocContainerTemplate : public CMultiDocTemplate
{
public:
	GUID	m_clsid;
	CAXDocContainerTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) :
	CMultiDocTemplate(nIDResource, pDocClass,pFrameClass,pViewClass)
	{
		m_clsid=CLSID_NULL;
	}


	void SetDocStrings(LPCTSTR lpstrStrings)
	{
		m_strDocStrings=lpstrStrings;
	}

	LPCTSTR GetDocStrings() const
	{
		return m_strDocStrings;
	}

	DECLARE_DYNAMIC(CAXDocContainerTemplate);
};


class CAXDocContainerCntrItem;


class CAXDocContainerDoc : public COleDocument
{
protected: // create from serialization only
	CAXDocContainerDoc();
	DECLARE_DYNCREATE(CAXDocContainerDoc)

// Attributes
public:

	static CAXDocInfoArray m_arrayDocInfo;

// Operations
public:
	CAXDocContainerCntrItem* GetDocItem();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAXDocContainerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAXDocContainerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAXDocContainerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CAXDocContainerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////



//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXDOCCONTAINERDOC_H__3533A5D1_D3DF_11D1_8F0B_F54176DCF130__INCLUDED_)
