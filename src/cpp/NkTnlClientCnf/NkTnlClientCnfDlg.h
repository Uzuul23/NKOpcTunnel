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

class CNkTnlClientCnfDlg : public nkresize::CResizableDialog
{
public:
	CNkTnlClientCnfDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_NKTNLCLIENTCNF_DIALOG };

private:
	HICON m_hIcon;
	std::vector<NkOPC::CTunnelRegEntry> m_Entries;
	HKEY m_hBaseKey;
	CListBox m_wndList;
	CButton m_wndAddButton;
	CButton m_wndEditButton;
	CButton m_wndCheckButton;
	CButton m_wndRemoveButton;

	void EnumEntries();
	void UpdateButtons();

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnBnClickedButtonCheck();
};
