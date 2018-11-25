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
};
