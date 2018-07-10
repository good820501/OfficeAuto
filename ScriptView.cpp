// ScriptView.cpp : implementation file
//
//Copyright Andrew A. Garbuzov

#include "stdafx.h"
#include "AXDocContainer.h"
#include "ScriptView.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptView

IMPLEMENT_DYNCREATE(CScriptView, CEditView)

CScriptView::CScriptView()
{
}

CScriptView::~CScriptView()
{
}


BEGIN_MESSAGE_MAP(CScriptView, CEditView)
	//{{AFX_MSG_MAP(CScriptView)
	ON_COMMAND(ID_FILE_RUN, OnFileRun)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptView drawing

void CScriptView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CScriptView diagnostics

#ifdef _DEBUG
void CScriptView::AssertValid() const
{
	CEditView::AssertValid();
}

void CScriptView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScriptView message handlers
int CScriptView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetTabStops(7);
	return 0;
}


void CScriptView::OnFileRun() 
{
	// TODO: Add your command handler code here
	const IID CLSID_VBScript = {0xb54f3741, 0x5b07, 0x11cf, {0xa4, 0xb0, 0x0, 0xaa, 0x0, 0x4a, 0x55, 0xe8 } };

	/*if(m_ScriptEngine.IsCreated())
	{
		m_ScriptEngine.Close();
		m_ScriptEngine.ReleaseEngine();
	}

	m_ScriptEngine.Create(CLSID_VBScript);
	m_ScriptEngine.SetDocument((CScriptDocument*)GetDocument());
	
	CString strScript;
	GetWindowText(strScript);

	if(!strScript.IsEmpty())
	{
		USES_CONVERSION;

		HRESULT		hr;
		
		LPCOLESTR	pstrItemName = T2COLE(_T("Document"));
		LPCOLESTR	pstrCode = T2COLE(strScript);
		EXCEPINFO   ei;
		
		//The following two lines were just a test
		const IID libidExcel={ 0x00020813,0x0000,0x0000, { 0xC0, 0x00, 0x00,0x00,0x00,0x00,0x00,0x46 } };
		hr=m_ScriptEngine.AddTypeLib(libidExcel,1,2,0);

		
		hr=m_ScriptEngine.AddNamedItem(pstrItemName, SCRIPTITEM_ISVISIBLE|SCRIPTITEM_ISSOURCE);
		if(FAILED(hr))
		{
			AfxMessageBox(_T("AddNamedItem failed. Item name: <Document>."));
			return;
		}
			
		if(SUCCEEDED(m_ScriptEngine.ParseScriptText(pstrCode, pstrItemName, NULL, NULL, 0, 0, 0L, NULL, &ei)))
		{
			m_ScriptEngine.Run();

			//Running the main subroutine
			IDispatch* pScriptDispatch=m_ScriptEngine.GetScriptDispatch(pstrItemName);
			if(pScriptDispatch)
			{
				DISPID dispid=NULL;
				OLECHAR* szMainFunc = T2OLE(_T("main"));

				HRESULT	hresult = pScriptDispatch->GetIDsOfNames(IID_NULL,
																&szMainFunc ,
																1,
																LOCALE_SYSTEM_DEFAULT,
																&dispid);

		
				if(SUCCEEDED(hresult))
				{
					//The driver will release the pScriptDispatch in any case
					COleDispatchDriver driver(pScriptDispatch);
					driver.InvokeHelper(dispid, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
				}
				else
					pScriptDispatch->Release();
			}
		}
		else
			AfxMessageBox(_T("Can't parse script text"));
		
	}*/
}

