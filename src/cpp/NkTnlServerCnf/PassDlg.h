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
