/*	This file is part of NKOpcTunnel.
*
*	Copyright (c) Henryk Anschuetz 
*	Berlin, Germany
*
*	mailto:uzuul23@online.de
*
*	NKOpcTunnel is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   NKOpcTunnel is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with NKOpcTunnel.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "stdafx.h"
#include "NkTnlClientCnf.h"
#include "NkTnlClientCnfDlg.h"
#include "afxdialogex.h"
#include "NkTnlEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	CString m_strSoftware;
	CString m_strAuthor;
	CString m_strLicense;
	CMFCLinkCtrl m_wndMailto;
	CMFCLinkCtrl m_wndURL;
	CMFCLinkCtrl m_wndLicURL;
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strSoftware(_T(""))
, m_strAuthor(_T(""))
, m_strLicense(_T(""))
{
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_strAuthor.LoadString(IDS_AUTHOR);
	m_strSoftware.LoadString(IDS_SOFTWARE);
	m_strLicense.LoadString(IDS_LICENSE);

	CString str;
	str.LoadString(IDS_MAILTO);
	m_wndMailto.SetURLPrefix(L"mailto:");
	m_wndMailto.SetWindowText(str);
	m_wndMailto.SetURL(str);
	m_wndMailto.SizeToContent();

	str.LoadString(IDS_URL);
	m_wndURL.SetURLPrefix(L"http://");
	m_wndURL.SetWindowText(str);
	m_wndURL.SetURL(str);
	m_wndURL.SizeToContent();

	str.LoadString(IDS_LIC_URL);
	m_wndLicURL.SetURLPrefix(L"http://");
	m_wndLicURL.SetWindowText(str);
	m_wndLicURL.SetURL(str);
	m_wndLicURL.SizeToContent();

	UpdateData(FALSE);

	return TRUE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_SOFTWARE, m_strSoftware);
	DDX_Text(pDX, IDC_STATIC_AUTHOR, m_strAuthor);
	DDX_Text(pDX, IDC_STATIC_LICENSE, m_strLicense);
	DDX_Control(pDX, IDC_BUTTON_MAILTO, m_wndMailto);
	DDX_Control(pDX, IDC_BUTTON_URL, m_wndURL);
	DDX_Control(pDX, IDC_BUTTON_LIC_URL, m_wndLicURL);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

CNkTnlClientCnfDlg::CNkTnlClientCnfDlg(CWnd* pParent /*=NULL*/)
	: nkresize::CResizableDialog(CNkTnlClientCnfDlg::IDD, pParent)
	, m_hBaseKey(HKEY_LOCAL_MACHINE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNkTnlClientCnfDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_wndList);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_wndAddButton);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_wndEditButton);
	DDX_Control(pDX, IDC_BUTTON_CHECK, m_wndCheckButton);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_wndRemoveButton);
}

BEGIN_MESSAGE_MAP(CNkTnlClientCnfDlg, nkresize::CResizableDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CNkTnlClientCnfDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CNkTnlClientCnfDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CNkTnlClientCnfDlg::OnBnClickedButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, &CNkTnlClientCnfDlg::OnBnClickedButtonCheck)
END_MESSAGE_MAP()

BOOL CNkTnlClientCnfDlg::OnInitDialog()
{
	__super::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL) {
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// preset layout
	AddAnchor(IDOK, nkresize::BOTTOM_RIGHT);
	AddAnchor(IDC_BUTTON_ADD, nkresize::TOP_RIGHT);
	AddAnchor(IDC_BUTTON_EDIT, nkresize::TOP_RIGHT);
	AddAnchor(IDC_BUTTON_CHECK, nkresize::TOP_RIGHT);
	AddAnchor(IDC_BUTTON_REMOVE, nkresize::TOP_RIGHT);
	AddAnchor(IDC_LIST, nkresize::TOP_LEFT, nkresize::BOTTOM_RIGHT);

	EnumEntries();
	m_wndList.SetCurSel(0);
	UpdateButtons();

	NkOPC::CTunnelRegEntry Entry;

	if (!Entry.ClientInstalled())
	{
		AfxMessageBox(IDS_INSTALL_ERROR, MB_ICONSTOP | MB_OK);
		EndDialog(0);
	}

	return TRUE;
}

void CNkTnlClientCnfDlg::EnumEntries()
{
	m_wndList.ResetContent();
	m_Entries.clear();

	CWaitCursor wait;

	//scan registry
	NkOPC::CTunnelRegEntry::EnumEntries(m_Entries, m_hBaseKey);

	for (size_t loop = 0; loop < m_Entries.size(); ++loop)
	{
		try {
			int item = m_wndList.AddString(m_Entries[loop].FullName());
			m_wndList.SetItemData(item, loop);
		}
		catch (NkError::CException& e) {
			e.report();
			int item = m_wndList.AddString(L"--- entry error ---");
			m_wndList.SetItemData(item, loop);
		}
	}
}

void CNkTnlClientCnfDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CNkTnlClientCnfDlg::OnPaint()
{
	if (IsIconic()) {
		CPaintDC dc(this);

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
	else {
		CDialog::OnPaint();
	}
}

HCURSOR CNkTnlClientCnfDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CNkTnlClientCnfDlg::OnBnClickedButtonAdd()
{
	CNkTnlEditDlg dlg;
	try {
		for (auto f: m_Entries) {
			dlg.m_strNames.Add(f.Name());
		}

		dlg.m_strTunnelName = L"NewTunnel1";

		int i = 1;
		while (true) {
			bool bFind = false;
			for (auto loop = 0; loop < dlg.m_strNames.GetSize(); ++loop) {
				if (dlg.m_strNames[loop] == dlg.m_strTunnelName) {
					bFind = true;
					break;
				}
			}
			if (!bFind) {
				break;
			}
			++i;
			dlg.m_strTunnelName.Format(L"NewTunnel%d", i);
		}

		if (dlg.DoModal() == IDOK)
		{
			NkOPC::CTunnelRegEntry Entry(m_hBaseKey);
			Entry.AddNew();
			Entry.Name(dlg.m_strTunnelName);
			Entry.RemoteServerPort(dlg.m_dwRemoteServerPort);
			Entry.RemoteServerIPAddress(dlg.m_dwRemoteServerIPAddress);
			Entry.RemoteServerPort(dlg.m_dwRemoteServerPort);
			Entry.RemoteServerIPAddress(dlg.m_dwRemoteServerIPAddress);
			Entry.RemoteServerCLSID(dlg.m_RemoteServerCLSID);
			Entry.RemoteServerProgID(dlg.m_strRemoteServerProgID);
			Entry.RemoteServerUserType(dlg.m_strRemoteServerUserType);
			Entry.RemoteServerDA1(dlg.m_bRemoteServerDA1);
			Entry.RemoteServerDA2(dlg.m_bRemoteServerDA2);
			Entry.RemoteServerDA3(dlg.m_bRemoteServerDA3);
			Entry.RemoteServerUseSSL(dlg.m_bUseSSL);
			Entry.RemoteClientVerifyServer(dlg.m_bVerifyServer);
			Entry.RemoteClientUseCertificate(dlg.m_bClientCertificate);

			/*if (dlg.m_strPass.GetLength() > 0) {
				size_t cb = (dlg.m_strPass.GetLength()+1) * 2;
				const BYTE *p = reinterpret_cast<BYTE*>(dlg.m_strPass.GetBuffer());
				Entry.EncryptRemoteServerPass(p, cb);
			}*/

			EnumEntries();
			m_wndList.SelectString(-1, Entry.FullName());
			UpdateButtons();
		}
	}
	catch (NkError::CException& e) {
		e.report();
		AfxMessageBox(e.error_text(), MB_ICONERROR);
	}
}

void CNkTnlClientCnfDlg::OnBnClickedButtonEdit()
{
	int Sel = m_wndList.GetCurSel();
	if (Sel == -1) {
		return;
	}

	try {
		CNkTnlEditDlg dlg;
		NkOPC::CTunnelRegEntry& Entry = m_Entries[m_wndList.GetItemData(Sel)];
		dlg.m_strTunnelName = Entry.Name();
		dlg.m_dwRemoteServerPort = Entry.RemoteServerPort();
		dlg.m_dwRemoteServerIPAddress = Entry.RemoteServerIPAddress();
		dlg.m_RemoteServerCLSID = Entry.RemoteServerCLSID();
		dlg.m_strRemoteServerProgID = Entry.RemoteServerProgID();
		dlg.m_strRemoteServerUserType = Entry.RemoteServerUserType();
		dlg.m_bRemoteServerDA1 = Entry.RemoteServerDA1();
		dlg.m_bRemoteServerDA2 = Entry.RemoteServerDA2();
		dlg.m_bRemoteServerDA3 = Entry.RemoteServerDA3();
		dlg.m_bUseSSL = Entry.RemoteServerUseSSL();
		dlg.m_bVerifyServer = Entry.RemoteClientVerifyServer();
		dlg.m_bClientCertificate = Entry.RemoteClientUseCertificate();

		/*try {
			size_t cb_plain = Entry.RemoteServerPassPlainSize();
			if (cb_plain) {
				size_t cb_plain_used = 0;
				dlg.m_vecPass.resize(cb_plain);
				Entry.DecryptRemoteServerPass(dlg.m_vecPass.data(), cb_plain, cb_plain_used);
				dlg.m_vecPass.resize(cb_plain_used);
			}
		}
		catch(NkError::CException& e) {
			e.report();
		}*/

		for (auto f : m_Entries) {
			try {
				if (dlg.m_strTunnelName != f.Name()) {
					dlg.m_strNames.Add(f.Name());
				}
			}
			catch (NkError::CException& e) {
				e.report();
			}
		}

		if (dlg.DoModal() == IDOK)
		{
			Entry.Update();
			Entry.Name(dlg.m_strTunnelName);
			Entry.RemoteServerPort(dlg.m_dwRemoteServerPort);
			Entry.RemoteServerIPAddress(dlg.m_dwRemoteServerIPAddress);
			Entry.RemoteServerCLSID(dlg.m_RemoteServerCLSID);
			Entry.RemoteServerProgID(dlg.m_strRemoteServerProgID);
			Entry.RemoteServerUserType(dlg.m_strRemoteServerUserType);
			Entry.RemoteServerDA1(dlg.m_bRemoteServerDA1);
			Entry.RemoteServerDA2(dlg.m_bRemoteServerDA2);
			Entry.RemoteServerDA3(dlg.m_bRemoteServerDA3);
			Entry.RemoteServerUseSSL(dlg.m_bUseSSL);
			Entry.RemoteClientVerifyServer(dlg.m_bVerifyServer);
			Entry.RemoteClientUseCertificate(dlg.m_bClientCertificate);

			/*if (dlg.m_strPass.GetLength() > 0) {
				size_t cb = (dlg.m_strPass.GetLength() + 1) * 2;
				const BYTE *p = reinterpret_cast<BYTE*>(dlg.m_strPass.GetBuffer());
				Entry.EncryptRemoteServerPass(p, cb);
			}*/

			EnumEntries();
			m_wndList.SelectString(-1, Entry.FullName());
			UpdateButtons();
		}
	}
	catch (NkError::CException& e) {
		e.report();
		AfxMessageBox(e.error_text(), MB_ICONERROR);
	}
}

void CNkTnlClientCnfDlg::OnBnClickedButtonRemove()
{
	try {
		int Sel = m_wndList.GetCurSel();
		if (Sel != -1) {
			if (AfxMessageBox(IDS_ASKFORDELETETNL, MB_OKCANCEL) == IDOK)
			{
				NkOPC::CTunnelRegEntry& Entry = m_Entries[m_wndList.GetItemData(Sel)];
				Entry.Remove();
				EnumEntries();
				m_wndList.SetCurSel(0);
				UpdateButtons();
			}
		}
	}
	catch (NkError::CException& e) {
		e.report();
		AfxMessageBox(e.error_text());
	}
}

void CNkTnlClientCnfDlg::OnBnClickedButtonCheck()
{
	try {
		int Sel = m_wndList.GetCurSel();
		if (Sel != -1) {
			CWaitCursor wait;
			NkCom::CComPtr<IOPCServer> spServer;
			auto entry = m_Entries[m_wndList.GetItemData(Sel)];

			//check server connection
			CStringA address;
			NkType::to_Addr<CStringA>(entry.RemoteServerIPAddress(), entry.RemoteServerPort(), address);

			if (entry.RemoteServerUseSSL()) {
				NkSSL::COpenSSLCtx ctx;
				entry.Setup(ctx);
				
				NkCom::CComPtr<NkCom::CServer> spSrv(NkOPC::COPCNearSrv::create_new_server_ssl(address, ctx));
			}
			else {
				NkCom::CComPtr<NkCom::CServer> spSrv(NkOPC::COPCNearSrv::create_new_server(address));
			}
			
			//check com object creation
			HRESULT hr = spServer.CoCreateInstance(entry.ProgID());
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

			NkOPC::COPCServerStatus status;
			hr = spServer->GetStatus(&status);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

			spServer.Release();

			AfxMessageBox(IDS_SERVER_CHECK_OK, MB_OK | MB_ICONINFORMATION);
		}
	}
	catch (NkError::CException& e) {
		e.report();
		AfxMessageBox(e.error_text(), MB_ICONERROR);
	}
}

void CNkTnlClientCnfDlg::UpdateButtons()
{
	const auto Enable = m_wndList.GetCurSel() != -1 && m_Entries.size() > 0;
	m_wndEditButton.EnableWindow(Enable);
	m_wndCheckButton.EnableWindow(Enable);
	m_wndRemoveButton.EnableWindow(Enable);
}
