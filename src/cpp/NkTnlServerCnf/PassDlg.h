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

class CPassDlg : public CDialog
{
	DECLARE_DYNAMIC(CPassDlg)

public:
	CPassDlg(CWnd* pParent = NULL);
	virtual ~CPassDlg();
	CString m_strPassOK;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PASSDLG };
#endif

private:
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);
	bool CheckPass();
	void Clean();
	CButton m_wndOKButton;
	CString m_strPass;
	CString m_strPassRepeat;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnEnChangeEditPassword();
	afx_msg void OnEnChangeEditPasswordRepeat();
};
