// NkTnlServerCnfDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NkTnlServerCnf.h"
#include "NkTnlServerCnfDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "PassDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CNkTnlServerCnfDlg dialog

CNkTnlServerCnfDlg::CNkTnlServerCnfDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NKTNLSERVERCNF_DIALOG, pParent)
	, m_strEditServiceStatus(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNkTnlServerCnfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SERVICE_STATUS, m_strEditServiceStatus);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	DDV_MinMaxInt(pDX, m_nPort, 100, 65000);
	DDX_IPAddress(pDX, IDC_IPADDRESS, m_dwIPAdresse);
	DDX_Control(pDX, IDC_TRACE_LEVEL, m_wndTraceLevel);
}

BEGIN_MESSAGE_MAP(CNkTnlServerCnfDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGFILE, &CNkTnlServerCnfDlg::OnBnClickedButtonLogfile)
	ON_BN_CLICKED(IDC_BUTTON_INSTALL_SERVICE, &CNkTnlServerCnfDlg::OnBnClickedButtonInstallService)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_SERVICE, &CNkTnlServerCnfDlg::OnBnClickedButtonRemoveService)
	ON_BN_CLICKED(IDC_BUTTON_START_SERVICE, &CNkTnlServerCnfDlg::OnBnClickedButtonStartService)
	ON_BN_CLICKED(IDC_BUTTON_STOP_SERVICE, &CNkTnlServerCnfDlg::OnBnClickedButtonStopService)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CNkTnlServerCnfDlg::OnBnClickedApply)
	ON_BN_CLICKED(IDOK, &CNkTnlServerCnfDlg::OnBnClickedOk)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS, &CNkTnlServerCnfDlg::OnIpnFieldchangedIpaddress)
	ON_EN_CHANGE(IDC_EDIT_PORT, &CNkTnlServerCnfDlg::OnEnChangeEditPort)
	ON_BN_CLICKED(IDC_BUTTON_PASSWORD, &CNkTnlServerCnfDlg::OnBnClickedButtonPassword)
	ON_CBN_SELCHANGE(IDC_TRACE_LEVEL, &CNkTnlServerCnfDlg::OnCbnSelchangeTraceLevel)
END_MESSAGE_MAP()

// CNkTnlServerCnfDlg message handlers

void CNkTnlServerCnfDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CNkTnlServerCnfDlg::OnPaint()
{
	if (IsIconic()) {
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CNkTnlServerCnfDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CNkTnlServerCnfDlg::OnInitDialog()
{
	__super::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	CString str;
	str.LoadString(IDS_TRACE_LEVEL_0);
	m_wndTraceLevel.AddString(str);
	str.LoadString(IDS_TRACE_LEVEL_1);
	m_wndTraceLevel.AddString(str);
	str.LoadString(IDS_TRACE_LEVEL_2);
	m_wndTraceLevel.AddString(str);
	str.LoadString(IDS_TRACE_LEVEL_3);
	m_wndTraceLevel.AddString(str);

	try {
		NkWin::CRegistry key(0, HKEY_LOCAL_MACHINE, KEY_READ);
		NkWin::CRegistry keySettings;
		keySettings.Open(NKOPCTnl::RegKeySettings, key);
		m_strServerPathName = keySettings.QueryValueString(NKOPCTnl::RegValueServerInstallPath);
		m_nPort = NkOPC::CTunnelRegEntry::ServerPort();
		m_dwIPAdresse = NkOPC::CTunnelRegEntry::ServerIP();
		m_wndTraceLevel.SetCurSel(NkOPC::CTunnelRegEntry::ServerTraceLevel());
	}
	catch (NkError::CException& e) {
		e.report();
		AfxMessageBox(e.error_text(), MB_ICONSTOP | MB_OK);
		EndDialog(0);
		return TRUE;
	}

	UpdateData(FALSE);
	UpdateServiceStatus();

	return TRUE;
}

bool CNkTnlServerCnfDlg::Apply()
{
	if (!m_bChanged) {
		return true;
	}

	UpdateData();

	try
	{
		NkOPC::CTunnelRegEntry::ServerPort(m_nPort);
		NkOPC::CTunnelRegEntry::ServerIP(m_dwIPAdresse);

		int sel = m_wndTraceLevel.GetCurSel();
		if (sel != -1) {
			NkOPC::CTunnelRegEntry::ServerTraceLevel(sel);
		}

		if (m_bPassChanged) {
			NkOPC::CTunnelRegEntry::SetPassHash(m_digest, sizeof(m_digest));
			m_bPassChanged = false;
		}

		m_bChanged = false;
	}
	catch (NkError::CException& e) {
		e.report();
		AfxMessageBox(e.error_text(), MB_ICONSTOP | MB_OK);
		return false;
	}

	if (m_bServiceRunning && m_bNeedRestart) {
		AfxMessageBox(IDS_NEED_RESTART, MB_ICONWARNING | MB_OK);
	}

	GetDlgItem(IDC_BUTTON_APPLY)->EnableWindow(FALSE);
	return true;
}

void CNkTnlServerCnfDlg::Changed()
{
	GetDlgItem(IDOK)->EnableWindow();
	GetDlgItem(IDC_BUTTON_APPLY)->EnableWindow();
	m_bChanged = true;
}

void CNkTnlServerCnfDlg::NeedRestart()
{
	m_bNeedRestart = true;
}

void CNkTnlServerCnfDlg::UpdateServiceStatus()
{
	SC_HANDLE hSCM = ::OpenSCManager(0, 0, GENERIC_WRITE);
	CString strStatusText(L"--error--");
	if (hSCM == 0) {
		GetDlgItem(IDC_EDIT_SERVICE_STATUS)->SetWindowText(strStatusText);
		return;
	}

	SC_HANDLE hService = ::OpenService(hSCM, NKOPCTnl::ServiceName, SC_MANAGER_ALL_ACCESS);
	m_bServiceInstalled = hService == 0 ? false : true;
	m_bServiceRunning = false;

	if (hService) {
		DWORD dwBytesNeeded = 0;
		SERVICE_STATUS_PROCESS ssStatus = { 0 };

		BOOL Ret = ::QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO
			, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded);

		m_bServiceRunning = ssStatus.dwCurrentState == SERVICE_RUNNING ? true : false;
	}

	GetDlgItem(IDC_BUTTON_INSTALL_SERVICE)
		->EnableWindow(m_bServiceInstalled == false);
	GetDlgItem(IDC_BUTTON_REMOVE_SERVICE)
		->EnableWindow(m_bServiceInstalled == true && m_bServiceRunning == false);
	GetDlgItem(IDC_BUTTON_START_SERVICE)
		->EnableWindow(m_bServiceInstalled == true && m_bServiceRunning == false);
	GetDlgItem(IDC_BUTTON_STOP_SERVICE)
		->EnableWindow(m_bServiceInstalled == true && m_bServiceRunning == true);

	if (m_bServiceInstalled) {
		if (m_bServiceRunning) {
			strStatusText.LoadString(IDS_SERVICE_RUNNING);
		}
		else {
			strStatusText.LoadString(IDS_SERVICE_STOPPED);
		}
	}
	else {
		strStatusText.LoadString(IDS_SERVICE_NOT_INSTALLED);
	}

	GetDlgItem(IDC_EDIT_SERVICE_STATUS)->SetWindowText(strStatusText);
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCM);
}

void CNkTnlServerCnfDlg::OnBnClickedButtonLogfile()
{
	WCHAR* pszFoldetPath = 0;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_ProgramData, 0, 0, &pszFoldetPath);
	if (SUCCEEDED(hr))
	{
		CString strPath(pszFoldetPath);
		CoTaskMemFree(pszFoldetPath);
		strPath += L"\\";
		strPath += NKOPCTnl::LogPath;
		::ShellExecute(0, L"explore", strPath, 0, 0, SW_SHOWDEFAULT);
	}
}

void CNkTnlServerCnfDlg::OnBnClickedButtonInstallService()
{
	if (m_bServiceInstalled) {
		return;
	}

	CWaitCursor wait;

	try {
		SHELLEXECUTEINFO ShellInfo = { 0 };
		ShellInfo.cbSize = sizeof(ShellInfo);
		ShellInfo.lpFile = m_strServerPathName;
		ShellInfo.lpParameters = L"-install";
		ShellInfo.lpVerb = L"open";
		ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

		BOOL bRet = ::ShellExecuteEx(&ShellInfo);
		if (!bRet) {
			throw GetLastError();
		}

		DWORD dwRet = ::WaitForSingleObject(ShellInfo.hProcess, INFINITE);
		if (dwRet == WAIT_FAILED) {
			::CloseHandle(ShellInfo.hProcess);
			throw GetLastError();
		}

		DWORD dwExitCode = 0;
		bRet = ::GetExitCodeProcess(ShellInfo.hProcess, &dwExitCode);
		if (!bRet) {
			::CloseHandle(ShellInfo.hProcess);
			throw GetLastError();
		}

		if (dwExitCode) {
			AfxMessageBox(IDS_UNKNOWN_ERROR, MB_ICONSTOP | MB_OK);
		}

		::CloseHandle(ShellInfo.hProcess);
	}
	catch (DWORD error) {
		AfxMessageBox(NkError::error_to_string(error), MB_ICONSTOP | MB_OK);
	}

	UpdateServiceStatus();
}

void CNkTnlServerCnfDlg::OnBnClickedButtonRemoveService()
{
	if (!m_bServiceInstalled || m_bServiceRunning) {
		return;
	}

	CWaitCursor wait;

	try {
		SHELLEXECUTEINFO ShellInfo = { 0 };
		ShellInfo.cbSize = sizeof(ShellInfo);
		ShellInfo.lpFile = m_strServerPathName;
		ShellInfo.lpParameters = L"-remove";
		ShellInfo.lpVerb = L"open";
		ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

		BOOL bRet = ::ShellExecuteEx(&ShellInfo);
		if (!bRet) {
			throw GetLastError();
		}

		DWORD dwRet = ::WaitForSingleObject(ShellInfo.hProcess, INFINITE);
		if (dwRet == WAIT_FAILED)
		{
			::CloseHandle(ShellInfo.hProcess);
			throw GetLastError();
		}

		DWORD dwExitCode = 0;
		bRet = ::GetExitCodeProcess(ShellInfo.hProcess, &dwExitCode);
		if (!bRet) {
			::CloseHandle(ShellInfo.hProcess);
			throw GetLastError();
		}

		if (dwExitCode) {
			AfxMessageBox(IDS_UNKNOWN_ERROR, MB_ICONSTOP | MB_OK);
		}

		::CloseHandle(ShellInfo.hProcess);
	}
	catch (DWORD error) {
		AfxMessageBox(NkError::error_to_string(error), MB_ICONSTOP | MB_OK);
	}

	UpdateServiceStatus();
}

void CNkTnlServerCnfDlg::OnBnClickedButtonStartService()
{
	if (!m_bServiceInstalled || m_bServiceRunning) {
		return;
	}

	CWaitCursor wait;

	try {
		SHELLEXECUTEINFO ShellInfo = { 0 };
		ShellInfo.cbSize = sizeof(ShellInfo);
		ShellInfo.lpFile = m_strServerPathName;
		ShellInfo.lpParameters = L"-start";
		ShellInfo.lpVerb = L"open";
		ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

		BOOL bRet = ::ShellExecuteEx(&ShellInfo);
		if (!bRet) {
			throw GetLastError();
		}

		DWORD dwRet = ::WaitForSingleObject(ShellInfo.hProcess, INFINITE);
		if (dwRet == WAIT_FAILED) {
			::CloseHandle(ShellInfo.hProcess);
			throw GetLastError();
		}

		DWORD dwExitCode = 0;
		bRet = ::GetExitCodeProcess(ShellInfo.hProcess, &dwExitCode);
		if (!bRet) {
			::CloseHandle(ShellInfo.hProcess);
			throw GetLastError();
		}

		if (dwExitCode) {
			AfxMessageBox(IDS_UNKNOWN_ERROR, MB_ICONSTOP | MB_OK);
		}

		::CloseHandle(ShellInfo.hProcess);
	}
	catch (DWORD error) {
		AfxMessageBox(NkError::error_to_string(error), MB_ICONSTOP | MB_OK);
	}

	UpdateServiceStatus();
}

void CNkTnlServerCnfDlg::OnBnClickedButtonStopService()
{
	if (!m_bServiceInstalled || !m_bServiceRunning) {
		return;
	}

	CWaitCursor wait;

	try {
		SHELLEXECUTEINFO ShellInfo = { 0 };
		ShellInfo.cbSize = sizeof(ShellInfo);
		ShellInfo.lpFile = m_strServerPathName;
		ShellInfo.lpParameters = L"-stop";
		ShellInfo.lpVerb = L"open";
		ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

		BOOL bRet = ::ShellExecuteEx(&ShellInfo);
		if (!bRet) {
			throw GetLastError();
		}

		DWORD dwRet = ::WaitForSingleObject(ShellInfo.hProcess, INFINITE);
		if (dwRet == WAIT_FAILED) {
			::CloseHandle(ShellInfo.hProcess);
			throw GetLastError();
		}

		DWORD dwExitCode = 0;
		bRet = ::GetExitCodeProcess(ShellInfo.hProcess, &dwExitCode);
		if (!bRet) {
			::CloseHandle(ShellInfo.hProcess);
			throw GetLastError();
		}

		if (dwExitCode) {
			AfxMessageBox(IDS_UNKNOWN_ERROR, MB_ICONSTOP | MB_OK);
		}

		::CloseHandle(ShellInfo.hProcess);
	}
	catch (DWORD error) {
		AfxMessageBox(NkError::error_to_string(error), MB_ICONSTOP | MB_OK);
	}

	UpdateServiceStatus();
}

void CNkTnlServerCnfDlg::OnBnClickedApply()
{
	Apply();
}

void CNkTnlServerCnfDlg::OnBnClickedOk()
{
	if (Apply()) {
		__super::OnOK();
	}
}

void CNkTnlServerCnfDlg::OnIpnFieldchangedIpaddress(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	Changed();
	NeedRestart();
}

void CNkTnlServerCnfDlg::OnEnChangeEditPort()
{
	Changed();
	NeedRestart();
}

void CNkTnlServerCnfDlg::OnBnClickedButtonPassword()
{
	CPassDlg dlg;
	if (dlg.DoModal() == IDOK) {
		try {
			dlg.m_strPassOK += NKOPCTnl::Salt;
			NkSSL::CSHA::SHA256(reinterpret_cast<BYTE*>(dlg.m_strPassOK.GetBuffer())
				, dlg.m_strPassOK.GetLength() * sizeof(wchar_t), m_digest);
			m_bPassChanged = true;
			Changed();
		}
		catch (NkError::CException& e) {
			e.report();
			AfxMessageBox(e.error_text(), MB_ICONERROR | MB_OK);
		}
	}
}

void CNkTnlServerCnfDlg::OnCbnSelchangeTraceLevel()
{
	if (m_wndTraceLevel.GetCurSel() == 3) {
		if (AfxMessageBox(IDS_TRACE_LEVEL_3_WARNING, MB_ICONWARNING | MB_OKCANCEL) != IDOK) {
			m_wndTraceLevel.SetCurSel(1);
		}
	}
	Changed();
	NeedRestart();
}
