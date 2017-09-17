// TestCpl.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TestCpl.h"

CTestCplApp theApp;

BOOL CTestCplApp::InitInstance()
{
    if (!m_bInitialized) return TRUE;

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	CTestCplDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	return FALSE;
}

CTestCplApp::CTestCplApp()
    : m_bInitialized(FALSE)
{
}

BEGIN_MESSAGE_MAP(CTestCplApp, CWinApp)
END_MESSAGE_MAP()

HCURSOR CTestCplDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
CTestCplDlg::CTestCplDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestCplDlg::IDD, pParent)
    , mWorkingDir(_T(""))
    , mCmdLine(_T(""))
    , mSysTime(_T(""))
    , mLocalTime(_T(""))
    , mCpuBrand(_T(""))
    , mTotalMemory(_T(""))
    , mCores(0)
    , mUserName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
}

void CTestCplDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT3, mWorkingDir);
    DDX_Text(pDX, IDC_EDIT4, mCmdLine);
    DDX_Text(pDX, IDC_EDIT1, mSysTime);
    DDX_Text(pDX, IDC_EDIT2, mLocalTime);
    DDX_Text(pDX, IDC_EDIT5, mCpuBrand);
    DDX_Text(pDX, IDC_EDIT6, mTotalMemory);
    DDX_Text(pDX, IDC_EDIT7, mCores);
    DDX_Text(pDX, IDC_EDIT8, mUserName);
}

void CTestCplDlg::OnPaint()
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

BEGIN_MESSAGE_MAP(CTestCplDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CClpInfoDlg message handlers

BOOL CTestCplDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    TCHAR path[MAX_PATH + 1];
    GetCurrentDirectory(MAX_PATH + 1, path);
    mWorkingDir.Format(_T("%s"), path);

    mCmdLine.Format(_T("%s"), GetCommandLine());

    int CPUInfo[4] = { -1 };
    unsigned nExIds, i = 0;
    char CPUBrandString[0x40];
    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];
    for (i = 0x80000000; i <= nExIds; ++i)
    {
        __cpuid(CPUInfo, i);
        // Interpret CPU brand string
        if (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }

    mCpuBrand = CString(CPUBrandString);

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    mCores = sysInfo.dwNumberOfProcessors;

    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx(&statex);

    mTotalMemory.Format(_T("%u MB"), (statex.ullTotalPhys / 1024) / 1024);

    DWORD size = MAX_PATH + 1;
    if (GetUserName(path, &size))
    {
        mUserName.Format(_T("%s"), path);
    }

    UpdateData(FALSE);

    SetTimer(123456, 500, NULL);

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CTestCplDlg::OnTimer(UINT_PTR nIDEvent)
{
    TCHAR datebuf[256];
    TCHAR timebuf[256];

    SYSTEMTIME datetime;
    GetSystemTime(&datetime);
    GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &datetime, NULL, datebuf, 256);
    GetTimeFormat(LOCALE_USER_DEFAULT, 0, &datetime, NULL, timebuf, 256);
    mSysTime.Format(_T("%s %s"), datebuf, timebuf);

    GetLocalTime(&datetime);
    GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &datetime, NULL, datebuf, 256);
    GetTimeFormat(LOCALE_USER_DEFAULT, 0, &datetime, NULL, timebuf, 256);
    mLocalTime.Format(_T("%s %s"), datebuf, timebuf);

    UpdateData(FALSE);

    CDialogEx::OnTimer(nIDEvent);
}
