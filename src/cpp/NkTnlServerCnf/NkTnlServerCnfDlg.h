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
#include "afxwin.h"

class CNkTnlServerCnfDlg : public CDialogEx
{
public:
	CNkTnlServerCnfDlg(CWnd* pParent = NULL);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NKTNLSERVERCNF_DIALOG };
#endif

private:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	void Changed();
	bool Apply();
	void NeedRestart();
	void UpdateServiceStatus();

	bool m_bServiceInstalled = false;
	bool m_bServiceRunning = false;
	bool m_bChanged = false;
	bool m_bNeedRestart = false;
	CString m_strServerPathName;
	HICON m_hIcon = 0;
	CString m_strEditServiceStatus;
	DWORD m_nPort = NKOPCTnl::DefaultPort;
	DWORD m_dwIPAdresse = 0;
	BYTE m_digest[SHA256_DIGEST_LENGTH] = { 0 };
	bool m_bPassChanged = false;
	BOOL m_bServerUseSSL;
	BOOL m_bVerifyClient;
	CButton m_wndCheckVerifyClient;
	CComboBox m_wndTraceLevel;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonLogfile();
	afx_msg void OnBnClickedButtonInstallService();
	afx_msg void OnBnClickedButtonRemoveService();
	afx_msg void OnBnClickedButtonStartService();
	afx_msg void OnBnClickedButtonStopService();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedOk();
	afx_msg void OnIpnFieldchangedIpaddress(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditPort();
	afx_msg void OnBnClickedButtonPassword();
	afx_msg void OnCbnSelchangeTraceLevel();
	afx_msg void OnBnClickedCheckUseSsl();
	afx_msg void OnBnClickedCheckVerifyClient();
	
};
