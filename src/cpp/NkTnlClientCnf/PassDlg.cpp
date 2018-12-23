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
#include "PassDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CPassDlg, CDialog)

CPassDlg::CPassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PASSDLG, pParent)
{

}

CPassDlg::~CPassDlg()
{
	for (int loop = 0; loop < m_strPassOK.GetLength(); ++loop) {
		m_strPassOK.SetAt(loop, L'0');
	}
}

void CPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_wndOKButton);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPass);
	DDX_Text(pDX, IDC_EDIT_PASSWORD_REPEAT, m_strPassRepeat);
}

BEGIN_MESSAGE_MAP(CPassDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD, &CPassDlg::OnEnChangeEditPassword)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD_REPEAT, &CPassDlg::OnEnChangeEditPasswordRepeat)
END_MESSAGE_MAP()

void CPassDlg::OnOK()
{
	if (CheckPass()) {
		m_strPassOK = m_strPass;
		Clean();
		__super::OnOK();
	}
}

void CPassDlg::OnCancel()
{
	Clean();
	__super::OnCancel();
}

bool CPassDlg::CheckPass()
{
	UpdateData();
	if (m_strPass.GetLength() >= 4)
	{
		if (m_strPass == m_strPassRepeat)
		{
			m_wndOKButton.EnableWindow();
			return true;
		}
	}

	m_wndOKButton.EnableWindow(FALSE);
	return false;
}

void CPassDlg::Clean()
{
	UpdateData();
	for (int loop = 0; loop < m_strPass.GetLength(); ++loop) {
		m_strPass.SetAt(loop, L'0');
	}
	for (int loop = 0; loop < m_strPassRepeat.GetLength(); ++loop) {
		m_strPassRepeat.SetAt(loop, L'0');
	}
	UpdateData(FALSE);
}

void CPassDlg::OnEnChangeEditPassword()
{
	CheckPass();
}

void CPassDlg::OnEnChangeEditPasswordRepeat()
{
	CheckPass();
}