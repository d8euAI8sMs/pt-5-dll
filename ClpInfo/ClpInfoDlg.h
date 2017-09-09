
// ClpInfoDlg.h : header file
//

#include <Cpl.h>

#pragma once


typedef LONG (WINAPI * CplApplet) (HWND, UINT, LPARAM, LPARAM);

struct CPLMODULE
{
    HMODULE hmModule;
    CplApplet fnProc;
    DWORD dwItems;
    CPLINFO * lpCplInfo;
};


// CClpInfoDlg dialog
class CClpInfoDlg : public CDialogEx
{
// Construction
public:
	CClpInfoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CLPINFO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    CPLMODULE mModule;
    CComboBox mItemsCtrl;
    CStatic mIconCtrl;
public:
    virtual BOOL DestroyWindow();
    afx_msg void OnBnClickedButton1();
    void ShowErrorMessage(LPCTSTR pszMessage, DWORD dwErrorCode);
    afx_msg void OnCbnSelchangeCombo1();
    CString mAppletName;
    CString mAppletInfo;
    CString mPath;
    afx_msg void OnBnClickedButton2();
    void CloseModule();
};
