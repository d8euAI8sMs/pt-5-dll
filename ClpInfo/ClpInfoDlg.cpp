
// ClpInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClpInfo.h"
#include "ClpInfoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CClpInfoDlg dialog



CClpInfoDlg::CClpInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClpInfoDlg::IDD, pParent)
    , mAppletName(_T(""))
    , mAppletInfo(_T(""))
    , mPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClpInfoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO1, mItemsCtrl);
    DDX_Control(pDX, IDC_ICON8, mIconCtrl);
    DDX_Text(pDX, IDC_EDIT2, mAppletName);
    DDX_Text(pDX, IDC_EDIT3, mAppletInfo);
    DDX_Text(pDX, IDC_EDIT1, mPath);
}

BEGIN_MESSAGE_MAP(CClpInfoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CClpInfoDlg::OnBnClickedButton1)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CClpInfoDlg::OnCbnSelchangeCombo1)
    ON_BN_CLICKED(IDC_BUTTON2, &CClpInfoDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CClpInfoDlg message handlers

BOOL CClpInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClpInfoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClpInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CClpInfoDlg::DestroyWindow()
{
    CloseModule();

    return CDialogEx::DestroyWindow();
}


void CClpInfoDlg::OnBnClickedButton1()
{
    CFileDialog dlg(TRUE);
    dlg.DoModal();

    CloseModule();
    
    mPath = dlg.GetPathName();
    if (mPath.IsEmpty())
    {
        return;
    }

    UpdateData(FALSE);

    HMODULE h = LoadLibrary(mPath);
    if (!h)
    {
        ShowErrorMessage(_T("Cannot load library"), GetLastError());
        return;
    }
    CplApplet addr = (CplApplet)GetProcAddress(h, "CPlApplet");
    if (!addr)
    {
        ShowErrorMessage(_T("Cannot find 'CPlApplet' function"), GetLastError());
        return;
    }

    mModule.hmModule = h;
    mModule.fnProc = addr;

    if (!mModule.fnProc(GetSafeHwnd(), CPL_INIT, 0, 0))
    {
        ShowErrorMessage(_T("Something went wrong, cannot init CPL"), GetLastError());
        return;
    }

    mModule.dwItems = mModule.fnProc(GetSafeHwnd(), CPL_GETCOUNT, 0, 0);
    mModule.lpCplInfo = new CPLINFO[mModule.dwItems];

    for (DWORD i = 0; i < mModule.dwItems; i++)
    {
        if (!mModule.fnProc(GetSafeHwnd(), CPL_INQUIRE, (LPARAM)i, (LPARAM)&mModule.lpCplInfo[i]))
        {
            AfxMessageBox(_T("Item inquire failed"));
        }
        CString str; str.Format(_T("%d"), i + 1);
        mItemsCtrl.AddString(str);
    }

    if (mModule.dwItems != 0)
    {
        mItemsCtrl.SetCurSel(0);
        OnCbnSelchangeCombo1();
    }
}


void CClpInfoDlg::ShowErrorMessage(LPCTSTR pszMessage, DWORD dwErrorCode)
{
    LPTSTR messageBuffer = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dwErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer, 0, NULL);
    CString str; str.Format(_T("%s\r\n(%d) %s"), pszMessage, dwErrorCode, messageBuffer);
    AfxMessageBox(str);
	LocalFree(messageBuffer);
}

void CClpInfoDlg::OnCbnSelchangeCombo1()
{
    if (!mModule.hmModule) return;
    int selection = mItemsCtrl.GetCurSel();
    if (selection < 0) return;

    CString name;
    CString info;
    HICON icon;

    TCHAR str[1024];

    if (!LoadString(mModule.hmModule, mModule.lpCplInfo[selection].idName, str, 1024))
    {
        ShowErrorMessage(_T("Cannot load string resource"), GetLastError());
    }
    else
    {
        mAppletName = CString(str);
    }

    if (!LoadString(mModule.hmModule, mModule.lpCplInfo[selection].idInfo, str, 1024))
    {
        ShowErrorMessage(_T("Cannot load string resource"), GetLastError());
    }
    else
    {
        mAppletInfo = CString(str);
    }

    if (!(icon = LoadIcon(mModule.hmModule, MAKEINTRESOURCE(mModule.lpCplInfo[selection].idIcon))))
    {
        ShowErrorMessage(_T("Cannot load icon resource"), GetLastError());
    }
    else
    {
        DestroyIcon(mIconCtrl.SetIcon(icon));
    }

    UpdateData(FALSE);
}


void CClpInfoDlg::OnBnClickedButton2()
{
    if (!mModule.hmModule) return;
    int selection = mItemsCtrl.GetCurSel();
    if (selection < 0) return;

    if (!mModule.fnProc(GetSafeHwnd(), CPL_DBLCLK, selection, mModule.lpCplInfo[selection].lData))
    {
        ShowErrorMessage(_T("Something went wrong, cannot open item"), GetLastError());
        return;
    }
}


void CClpInfoDlg::CloseModule()
{
    if (mModule.hmModule != NULL)
    {
        for (DWORD i = 0; i < mModule.dwItems; i++)
        {
            if (!mModule.fnProc(GetSafeHwnd(), CPL_STOP, (LPARAM)i, (LPARAM)mModule.lpCplInfo[i].lData))
            {
                AfxMessageBox(_T("Item stop failed"));
            }
        }
        if (!mModule.fnProc(GetSafeHwnd(), CPL_EXIT, 0, 0))
        {
            AfxMessageBox(_T("Item exit failed"));
        }
        mItemsCtrl.ResetContent();
        mPath = "";
        mAppletName = "";
        mAppletInfo = "";
        DestroyIcon(mIconCtrl.SetIcon(NULL));
        FreeLibrary(mModule.hmModule);
        delete[] mModule.lpCplInfo;
        mModule.hmModule = NULL;
        UpdateData(FALSE);
    }
}
