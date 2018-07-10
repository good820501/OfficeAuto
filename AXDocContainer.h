// AXDocContainer.h : main header file for the AXDOCCONTAINER application
//

#if !defined(AFX_AXDOCCONTAINER_H__3533A5C8_D3DF_11D1_8F0B_F54176DCF130__INCLUDED_)
#define AFX_AXDOCCONTAINER_H__3533A5C8_D3DF_11D1_8F0B_F54176DCF130__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerApp:
// See AXDocContainer.cpp for the implementation of this class
//

class CAXDocContainerApp : public CWinApp
{
public:
	CAXDocContainerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAXDocContainerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation

	//{{AFX_MSG(CAXDocContainerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECMD_MAP();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXDOCCONTAINER_H__3533A5C8_D3DF_11D1_8F0B_F54176DCF130__INCLUDED_)
