// CpuDlg.h : header file
//

#pragma once
#include "CpuUserIntf.h"

// Cproj2Dlg dialog
class CcpuDlg : public CDialog
{
// Construction
public:
	CcpuDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PROJ2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CpuUserIntf m_intf;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelSource();
	afx_msg void OnBnClickedSelDest();
	afx_msg void OnBnClickedProcess();
	afx_msg void OnBnClickedRunScript();
	void SelectFile(wchar_t* filename);
};
