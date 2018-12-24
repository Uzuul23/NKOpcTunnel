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
#include "NkTnlBrowseOPCServerDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CNkTnlBrowseOPCServerDlg, CDialog)

CNkTnlBrowseOPCServerDlg::CNkTnlBrowseOPCServerDlg(CWnd* pParent /*=NULL*/)
: nkresize::CResizableDialog(CNkTnlBrowseOPCServerDlg::IDD, pParent)
, m_IndexSel(-1)
{
}

CNkTnlBrowseOPCServerDlg::~CNkTnlBrowseOPCServerDlg()
{
}

void CNkTnlBrowseOPCServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER_TREE, m_wndServerTree);
	DDX_Control(pDX, IDOK, m_wndOKButton);
}

BEGIN_MESSAGE_MAP(CNkTnlBrowseOPCServerDlg, nkresize::CResizableDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SERVER_TREE, &CNkTnlBrowseOPCServerDlg::OnTvnSelchangedServerTree)
END_MESSAGE_MAP()

BOOL CNkTnlBrowseOPCServerDlg::OnInitDialog()
{
	CWaitCursor wait;
	__super::OnInitDialog();

	//preset layout
	AddAnchor(IDOK, nkresize::BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, nkresize::BOTTOM_RIGHT);
	AddAnchor(IDC_SERVER_TREE, nkresize::TOP_LEFT, nkresize::BOTTOM_RIGHT);

	m_wndServerTree.DeleteAllItems();

	HDC hDC = ::GetDC(m_wndServerTree);
	int nBitPxel = ::GetDeviceCaps(hDC, BITSPIXEL);
	::ReleaseDC(m_wndServerTree, hDC);

	CBitmap cBitMap;
	if (nBitPxel >= 16) {
		cBitMap.LoadBitmap(IDB_SERVERTREE_HC);

		BITMAP BitMap;
		::GetObject(cBitMap, sizeof(BITMAP), &BitMap);

		m_listImages.Create(16, BitMap.bmHeight, ILC_MASK | ILC_COLOR24, 0, 0);
	}
	else {
		m_listImages.Create(IDB_SERVERTREE, 16, 0, RGB(255, 0, 0));
	}

	m_listImages.Add(&cBitMap, RGB(255, 0, 0));
	m_wndServerTree.SetImageList(&m_listImages, TVSIL_NORMAL);
	m_wndServerTree.ModifyStyle(0, TVS_HASBUTTONS | TVS_LINESATROOT
		| TVS_SHOWSELALWAYS | TVS_HASLINES | TVS_FULLROWSELECT
		| TVS_EDITLABELS, 0);

	try
	{
		NkCom::CComPtr<NkCom::CServer> spSrv;

		if (m_bUseSSL) {
			NkSSL::COpenSSLCtx ctx;
			ctx.create_TLSv1_2_client();

			NkWin::CRegistry key(NKOPCTnl::RegKeySettings, HKEY_LOCAL_MACHINE, KEY_READ);
			
			if (m_bVerifyServer) {
				
				CStringA path(key.QueryValueAnsiString(NKOPCTnl::RegValueClientCertPath));
				path += L"ca.crt";
				ctx.load_verify_locations(path);
				ctx.set_verify();	
			}
			if (m_bClientCertificate) {
				CStringA path(key.QueryValueAnsiString(NKOPCTnl::RegValueClientCertPath));
				path += L"client.crt";
				ctx.certificate_file(path);

				path = key.QueryValueAnsiString(NKOPCTnl::RegValueClientCertPath);
				path += L"client.key";
				ctx.use_private_key_file(path);
			}
			
			spSrv = NkOPC::COPCNearSrv::create_new_server_ssl(m_strServerAddr, ctx);
		}
		else {
			spSrv = NkOPC::COPCNearSrv::create_new_server(m_strServerAddr);
		}

		ASSERT(m_pvecPass);
		if (m_pvecPass == 0) {
			return TRUE;
		}
		HRESULT hr = spSrv->remote_logon(m_pvecPass->data(), m_pvecPass->size());
		if (FAILED(hr)) {
			throw NkError::CBaseException(NkError::CBaseException::logon_failed
				, __FILE__, __LINE__);
		}

		NkCom::CComPtrNear<IOPCServerList2, NkOPC::COPCServerList2Near> spServerList(spSrv);
		spServerList.create(CLSID_OpcServerList);

		CATID catidImpl = IID_CATID_OPCDAServer10;
		CComPtr<IOPCEnumGUID> pEnum;
		hr = spServerList->EnumClassesOfCategories(1, &catidImpl, 0, 0, &pEnum);
		NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

		GUID guid = GUID_NULL;
		NkCom::CComString szProgID;
		NkCom::CComString szUserType;
		NkCom::CComString szVerIndProgID;

		TVINSERTSTRUCT Item;
		ZeroMemory(&Item, sizeof(Item));

		Item.hParent = TVI_ROOT;
		Item.hInsertAfter = TVI_LAST;
		Item.item.iImage = 3;
		Item.item.iSelectedImage = 3;
		Item.item.pszText = L"Data Access Version 1";
		Item.item.lParam = -1;
		Item.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
		Item.hParent = m_wndServerTree.InsertItem(&Item);

		ULONG celtFetched = 0;
		while (pEnum->Next(1, &guid, &celtFetched) == S_OK) {
			hr = spServerList->GetClassDetails(guid, &szProgID, &szUserType, &szVerIndProgID);
			if (FAILED(hr)) {
				continue;
			}

			Item.item.pszText = szUserType;
			Item.item.iImage = 2;
			Item.item.iSelectedImage = 2;
			bool b = true;
			Item.item.lParam = AddServer(guid, szProgID, szUserType, &b, 0, 0);
			m_wndServerTree.InsertItem(&Item);

			szProgID.clear();
			szUserType.clear();
			szVerIndProgID.clear();
		}
		m_wndServerTree.Expand(Item.hParent, TVE_EXPAND);

		pEnum.Release();

		catidImpl = IID_CATID_OPCDAServer20;
		hr = spServerList->EnumClassesOfCategories(1, &catidImpl, 0, 0, &pEnum);
		NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

		Item.hParent = TVI_ROOT;
		Item.item.iImage = 3;
		Item.item.iSelectedImage = 3;
		Item.item.pszText = L"Data Access Version 2";
		Item.item.lParam = -1;
		Item.hParent = m_wndServerTree.InsertItem(&Item);

		while (pEnum->Next(1, &guid, &celtFetched) == S_OK) {
			hr = spServerList->GetClassDetails(guid, &szProgID, &szUserType, &szVerIndProgID);
			if (FAILED(hr)) {
				continue;
			}

			Item.item.pszText = szUserType;
			Item.item.iImage = 2;
			Item.item.iSelectedImage = 2;
			bool b = true;
			Item.item.lParam = AddServer(guid, szProgID, szUserType, 0, &b, 0);
			m_wndServerTree.InsertItem(&Item);

			szProgID.clear();
			szUserType.clear();
			szVerIndProgID.clear();
		}
		m_wndServerTree.Expand(Item.hParent, TVE_EXPAND);
		pEnum.Release();

		catidImpl = IID_CATID_OPCDAServer30;
		hr = spServerList->EnumClassesOfCategories(1, &catidImpl, 0, 0, &pEnum);
		NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

		Item.hParent = TVI_ROOT;
		Item.item.iImage = 3;
		Item.item.iSelectedImage = 3;
		Item.item.pszText = L"Data Access Version 3";
		Item.item.lParam = -1;
		Item.hParent = m_wndServerTree.InsertItem(&Item);

		while (pEnum->Next(1, &guid, &celtFetched) == S_OK) {
			hr = spServerList->GetClassDetails(guid, &szProgID, &szUserType, &szVerIndProgID);
			if (FAILED(hr)) {
				continue;
			}

			Item.item.pszText = szUserType;
			Item.item.iImage = 2;
			Item.item.iSelectedImage = 2;
			bool b = true;
			Item.item.lParam = AddServer(guid, szProgID, szUserType, 0, 0, &b);
			m_wndServerTree.InsertItem(&Item);

			szProgID.clear();
			szUserType.clear();
			szVerIndProgID.clear();
		}
		m_wndServerTree.Expand(Item.hParent, TVE_EXPAND);
		pEnum.Release();
	}
	catch (NkError::CException& e) {
		e.report();
		AfxMessageBox(e.error_text(), MB_ICONERROR);
		EndDialog(IDCANCEL);
	}

	return TRUE;
}

LPARAM CNkTnlBrowseOPCServerDlg::AddServer(GUID guid, LPOLESTR pszProgID, LPOLESTR pszUserType
	, bool* pbOPCDAServer10, bool* pbOPCDAServer20, bool* pbOPCDAServer30)
{
	if (guid == GUID_NULL || pszProgID == 0) {
		return -1;
	}
	for (INT_PTR loop = 0; loop < m_Servers.GetCount(); ++loop) {
		if (m_Servers[loop].m_CLSID == guid) {
			if (pbOPCDAServer10) {
				m_Servers[loop].m_bOPCDAServer10 = *pbOPCDAServer10;
			}
			if (pbOPCDAServer20) {
				m_Servers[loop].m_bOPCDAServer20 = *pbOPCDAServer20;
			}
			if (pbOPCDAServer30) {
				m_Servers[loop].m_bOPCDAServer30 = *pbOPCDAServer30;
			}
			return static_cast<LPARAM>(loop);
		}
	}

	return static_cast<LPARAM>(m_Servers.Add(CServerInfo(guid, pszProgID, pszUserType
		, pbOPCDAServer10, pbOPCDAServer20, pbOPCDAServer30)));
}

void CNkTnlBrowseOPCServerDlg::OnTvnSelchangedServerTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	m_wndOKButton.EnableWindow(pNMTreeView->itemNew.lParam != -1);
	*pResult = 0;
}

void CNkTnlBrowseOPCServerDlg::OnOK()
{
	HTREEITEM hSel = m_wndServerTree.GetSelectedItem();
	if (hSel == 0) {
		return;
	}
	DWORD_PTR nIndex = m_wndServerTree.GetItemData(hSel);
	if (nIndex == -1) {
		return;
	}
	m_IndexSel = static_cast<INT_PTR>(nIndex);

	CResizableDialog::OnOK();
}

CNkTnlBrowseOPCServerDlg::CServerInfo* CNkTnlBrowseOPCServerDlg::GetSelServerInfo()
{
	if (m_IndexSel >= 0 && m_IndexSel < m_Servers.GetCount()) {
		return &m_Servers[m_IndexSel];
	}
	return 0;
}
