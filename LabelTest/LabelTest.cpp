// LabelTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LabelTest.h"
#include "LabelTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLabelTestApp

BEGIN_MESSAGE_MAP(CLabelTestApp, CWinApp)
END_MESSAGE_MAP()

// CLabelTestApp construction

CLabelTestApp::CLabelTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CLabelTestApp object

CLabelTestApp theApp;

// CLabelTestApp initialization

BOOL CLabelTestApp::InitInstance()
{
	CWinApp::InitInstance();

	CLabelTestDlg dlg;
	m_pMainWnd = &dlg;

	INT_PTR nResponse = dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

