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
