// AXDocContainer.cpp : Defines the class behaviors for the application.
//
//Copyright Andrew A. Garbuzov

#include "stdafx.h"
#include "AXDocContainer.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "AXDocContainerDoc.h"
#include "AXDocContainerView.h"
#include "AXDocContainerItem.h"

#include "ScriptDocument.h"
#include "ScriptView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerApp

BEGIN_MESSAGE_MAP(CAXDocContainerApp, CWinApp)
	//{{AFX_MSG_MAP(CAXDocContainerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()
static const CLSID clsid =
{ 0x3533a5c3, 0xd3df, 0x11d1, { 0x8f, 0xb, 0xf5, 0x41, 0x76, 0xdc, 0xf1, 0x30 } };

BEGIN_OLECMD_MAP(CAXDocContainerApp, CWinApp)
{NULL,OLECMDID_NEW,ID_FILE_NEW},
{NULL,OLECMDID_OPEN,ID_FILE_OPEN},
{NULL,OLECMDID_SAVE,ID_FILE_SAVE},
{NULL,OLECMDID_PAGESETUP,ID_FILE_PRINT_SETUP},
{NULL,OLECMDID_PRINT,ID_FILE_PRINT},
{NULL,OLECMDID_PRINTPREVIEW,ID_FILE_PRINT_PREVIEW},
END_OLECMD_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerApp construction

CAXDocContainerApp::CAXDocContainerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAXDocContainerApp object

CAXDocContainerApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {3533A5C3-D3DF-11D1-8F0B-F54176DCF130}

/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerApp initialization

BOOL CAXDocContainerApp::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CAXDocContainerDoc::m_arrayDocInfo.LoadFromRegistry();

	for(int i=0; i<CAXDocContainerDoc::m_arrayDocInfo.GetSize(); i++)
	{
		CAXDocInfo* pInfo=CAXDocContainerDoc::m_arrayDocInfo[i];
		CAXDocContainerTemplate* pDocTemplate;
		pDocTemplate = new CAXDocContainerTemplate(
			IDR_AXDOCCTYPE,
			RUNTIME_CLASS(CAXDocContainerDoc),
			RUNTIME_CLASS(CChildFrame), // custom MDI child frame
			RUNTIME_CLASS(CAXDocContainerView));
		pDocTemplate->SetContainerInfo(IDR_AXDOCCTYPE_CNTR_IP);
		AddDocTemplate(pDocTemplate);

		pDocTemplate->m_clsid=pInfo->m_clsid;
		pDocTemplate->SetDocStrings(pInfo->m_strDocStrings);
	}

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
			IDR_SCRIPTDOC,
			RUNTIME_CLASS(CScriptDocument),
			RUNTIME_CLASS(CChildFrame), // custom MDI child frame
			RUNTIME_CLASS(CScriptView));
	AddDocTemplate(pDocTemplate);


	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);
	
	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications.
	//COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line.

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Check to see if launched as OLE server
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
	COleObjectFactory::UpdateRegistryAll();

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CAXDocContainerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAXDocContainerApp commands
