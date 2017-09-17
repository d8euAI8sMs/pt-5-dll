#pragma once

#include "resource.h"
#include "afxdialogex.h"

class CTestCplApp : public CWinApp
{
public:
	CTestCplApp();

public:
	virtual BOOL InitInstance();
    BOOL m_bInitialized;

protected:
	DECLARE_MESSAGE_MAP()
};

class CTestCplDlg : public CDialogEx
{
public:
	CTestCplDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_TESTCPL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
};

extern CTestCplApp theApp;
