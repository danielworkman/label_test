
// LabelTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Label.h"
#include "LabelTest.h"
#include "LabelTestDlg.h"
#include "afxdialogex.h"

#include "Visualisation.h"
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLabelTestDlg dialog

CLabelTestDlg::CLabelTestDlg(const Path& path, CWnd* pParent /*=NULL*/)
	: m_path(path),
	  m_sliderPos(0.0),
	  CDialogEx(CLabelTestDlg::IDD, pParent)
{
}
//-----------------------------------------------------------------------------

void CLabelTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLabelTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_BOXES_EDIT, &CLabelTestDlg::OnEnChangeBoxesEdit)
	ON_EN_CHANGE(IDC_BOX_HEIGHT_EDIT, &CLabelTestDlg::OnEnChangeBoxHeightEdit)
	ON_EN_CHANGE(IDC_BOX_WIDTH_EDIT, &CLabelTestDlg::OnEnChangeBoxWidthEdit)
	ON_EN_CHANGE(IDC_SPACING_EDIT, &CLabelTestDlg::OnEnChangeSpacingEdit)
END_MESSAGE_MAP()


// CLabelTestDlg message handlers

BOOL CLabelTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_boxCount = 5;
	m_boxHeight = 25;
	m_boxWidth = 15;
	m_spacing = 2;

	GetDlgItem(IDC_BOXES_EDIT)->SetWindowText(std::to_wstring(m_boxCount).c_str());
	GetDlgItem(IDC_BOX_HEIGHT_EDIT)->SetWindowText(std::to_wstring(m_boxHeight).c_str());
	GetDlgItem(IDC_BOX_WIDTH_EDIT)->SetWindowText(std::to_wstring(m_boxWidth).c_str());
	GetDlgItem(IDC_SPACING_EDIT)->SetWindowText(std::to_wstring(m_spacing).c_str());

	CSliderCtrl* slider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER);
	slider->SetRangeMax(1000);

	m_vis = Visualisation(this->GetDC());

	m_brush = CreateSolidBrush(RGB(255, 255, 255));

	return TRUE;  // return TRUE  unless you set the focus to a control
}
//-----------------------------------------------------------------------------

void CLabelTestDlg::DrawPath()
{
	m_vis.DrawPath(m_path);
}
//-----------------------------------------------------------------------------

void CLabelTestDlg::CreateLabel()
{
	const Label label(m_boxCount, m_boxHeight, m_boxWidth, m_spacing);
	std::vector<Path> letters = label.GetLetters(m_path, m_sliderPos / 100.0);

	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* oldPen = m_vis.GetPen();

	m_vis.SelectPen(&pen);

	for (auto itr = letters.begin(); itr != letters.end(); ++itr)
	{
		m_vis.DrawPath(*itr);
	}

	m_vis.SelectPen(oldPen);
}
//-----------------------------------------------------------------------------

void CLabelTestDlg::DisplayPercentage()
{
	std::ostringstream oss;
	oss << m_sliderPos << " %";

	m_vis.DrawText(Vector2(10, 10), oss.str());
}
//-----------------------------------------------------------------------------

void CLabelTestDlg::OnPaint()
{
	DrawPath();
	CreateLabel();
	DisplayPercentage();
}
//-----------------------------------------------------------------------------

BOOL CLabelTestDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect drawingArea;
	GetClientRect(&drawingArea);

	COLORREF backcolour = RGB(255, 255, 255);
	CBrush brushBackground(backcolour);

	pDC->FillRect(&drawingArea, &brushBackground);

	return TRUE;
}
//-----------------------------------------------------------------------------

void CLabelTestDlg::Redraw()
{
	Invalidate();
	UpdateWindow();
}
//-----------------------------------------------------------------------------

void CLabelTestDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);

	CSliderCtrl* slider = (CSliderCtrl*)pScrollBar;
	m_sliderPos = slider->GetPos() / 10.0;

	Redraw();
}
//-----------------------------------------------------------------------------

HBRUSH CLabelTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SetBkMode(TRANSPARENT);

	return m_brush;
}
//-----------------------------------------------------------------------------

void CLabelTestDlg::OnEnChangeBoxesEdit()
{
	CString text;
	GetDlgItem(IDC_BOXES_EDIT)->GetWindowText(text);
	m_boxCount = _wtoi(text);
	Redraw();
}
//-----------------------------------------------------------------------------

void CLabelTestDlg::OnEnChangeBoxHeightEdit()
{
	CString text;
	GetDlgItem(IDC_BOX_HEIGHT_EDIT)->GetWindowText(text);
	m_boxHeight = _wtoi(text);
	Redraw();
}
//-----------------------------------------------------------------------------

void CLabelTestDlg::OnEnChangeBoxWidthEdit()
{
	CString text;
	GetDlgItem(IDC_BOX_WIDTH_EDIT)->GetWindowText(text);
	m_boxWidth = _wtoi(text);
	Redraw();
}
//-----------------------------------------------------------------------------

void CLabelTestDlg::OnEnChangeSpacingEdit()
{
	CString text;
	GetDlgItem(IDC_SPACING_EDIT)->GetWindowText(text);
	m_spacing = _wtoi(text);
	Redraw();
}