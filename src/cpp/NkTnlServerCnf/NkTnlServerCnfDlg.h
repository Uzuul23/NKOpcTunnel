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
public:
	afx_msg void OnCbnSelchangeTraceLevel();
	CComboBox m_wndTraceLevel;
private:
	BOOL m_bServerUseSSL;
public:
	afx_msg void OnBnClickedCheckUseSsl();
};
