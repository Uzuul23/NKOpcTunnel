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

#pragma once
#include "resource.h"
#include "afxwin.h"
#include <vector>

class CNkTnlEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CNkTnlEditDlg)

public:
	CNkTnlEditDlg(CWnd* pParent = NULL);
	virtual ~CNkTnlEditDlg();
	enum { IDD = IDD_NKTNLEDIT };

public:
	std::vector<BYTE> m_vecPass;
	CStringArray m_strNames;
	CString m_strTunnelName;
	CString m_strOldTunnelName;
	CString m_strRemoteServerProgID;
	CString m_strRemoteServerUserType;
	GUID m_RemoteServerCLSID;
	DWORD m_dwRemoteServerIPAddress;
	DWORD m_dwRemoteServerPort;
	BOOL m_bRemoteServerDA1;
	BOOL m_bRemoteServerDA2;
	BOOL m_bRemoteServerDA3;
	CStatic m_wndStaticError;
	CButton m_wndOK;

private:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnBnClickedButtonSelectOpcServer();
	afx_msg void OnEnChangeEditName();
	afx_msg void OnBnClickedButtonPass();
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	BOOL m_bUseSSL;
	BOOL m_bVerifyServer;
	BOOL m_bClientCertificate;
	afx_msg void OnBnClickedCheckUseSsl();
};
