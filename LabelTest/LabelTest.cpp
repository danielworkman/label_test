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
}

// The one and only CLabelTestApp object

CLabelTestApp theApp;

// CLabelTestApp initialization

Path::EdgeList CLabelTestApp::ArcsTest()
{
	return {
		Edge(Vector2(100, 100), Vector2(200, 200), 100, true, false),
		Edge(Vector2(300, 300), Vector2(400, 400), 100, false, true),
		Edge(Vector2(500, 500), Vector2(600, 600), 100, false, false),
		Edge(Vector2(600, 100), Vector2(700, 200), 100, true, true),
	};
}
//-----------------------------------------------------------------------------

Path::VectorRadiusList CLabelTestApp::PathExample1()
{
	return {
		{ Vector2(100, 300) },
		{ Vector2(300, 300) },
		{ Vector2(370, 370), 80, false, true },
		{ Vector2(400, 540) },
		{ Vector2(540, 540), 70 },
		{ Vector2(570, 420) },
		{ Vector2(660, 220), 450, false, true },
		{ Vector2(690, 100), 500 },
		{ Vector2(700, 90), 20, false, true },
		{ Vector2(850, 70) },
	};
}
//-----------------------------------------------------------------------------

Path::VectorRadiusList CLabelTestApp::PathExample2()
{
	return{
		{ Vector2(100, 300) },
		{ Vector2(300, 300) },
		{ Vector2(370, 370), 80, false, true },
		{ Vector2(400, 540) },
		{ Vector2(540, 540), 70 },
		{ Vector2(570, 420) },
		{ Vector2(660, 220), 450, false, true },
		{ Vector2(690, 100), 500 },
		{ Vector2(700, 90), 20, false, true },
		{ Vector2(850, 70) },
	};
}
//-----------------------------------------------------------------------------

BOOL CLabelTestApp::InitInstance()
{
	CWinApp::InitInstance();

	Path path = Path(ArcsTest());

	CLabelTestDlg dlg(path);
	m_pMainWnd = &dlg;

	dlg.DoModal();

	return FALSE;
}