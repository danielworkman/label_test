
// LabelTestDlg.h : header file
//

#pragma once

#include "Visualisation.h"
#include "Geometry.h"
#include "afxcmn.h"

// CLabelTestDlg dialog
class CLabelTestDlg : public CDialogEx
{
// Construction
public:
	CLabelTestDlg(const Path& path, CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LABELTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnEnChangeBoxesEdit();
	afx_msg void OnEnChangeBoxHeightEdit();
	afx_msg void OnEnChangeBoxWidthEdit();
	afx_msg void OnEnChangeSpacingEdit();

private:
	void Redraw();

	void DrawPath();
	void CreateLabel();
	void DisplayPercentage();

	Visualisation m_vis;

	Path m_path;

	double m_sliderPos;

	int m_boxCount;
	int m_boxHeight;
	int m_boxWidth;
	int m_spacing;

	HBRUSH m_brush;
};
