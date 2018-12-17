#include "stdafx.h"
#include "ResizablePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizablePage

namespace nkresize
{
	IMPLEMENT_DYNCREATE(CResizablePage, CPropertyPage)
	
	inline void CResizablePage::Construct()
	{
		m_bInitDone = FALSE;
	}
	
	CResizablePage::CResizablePage()
	{
		Construct();
	}
	
	CResizablePage::CResizablePage(UINT nIDTemplate, UINT nIDCaption)
		: CPropertyPage(nIDTemplate, nIDCaption)
	{
		Construct();
	}
	
	CResizablePage::CResizablePage(LPCTSTR lpszTemplateName, UINT nIDCaption)
		: CPropertyPage(lpszTemplateName, nIDCaption)
	{
		Construct();
	}
	
	CResizablePage::~CResizablePage()
	{
	}
	
	
	BEGIN_MESSAGE_MAP(CResizablePage, CPropertyPage)
		//{{AFX_MSG_MAP(CResizablePage)
		ON_WM_SIZE()
		ON_WM_ERASEBKGND()
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()
	
	
	/////////////////////////////////////////////////////////////////////////////
	// CResizablePage message handlers
	
	
	BOOL CResizablePage::OnInitDialog() 
	{
		CPropertyPage::OnInitDialog();
		
	/*	// gets the initial size as the min track size
		CRect rc;
		GetWindowRect(&rc);
	*/
		m_bInitDone = TRUE;
	
		return TRUE;  // return TRUE unless you set the focus to a control
		              // EXCEPTION: OCX Property Pages should return FALSE
	}
	
	void CResizablePage::OnSize(UINT nType, int cx, int cy) 
	{
		CWnd::OnSize(nType, cx, cy);
		
		if (m_bInitDone)
			ArrangeLayout();
	}
	
	BOOL CResizablePage::OnEraseBkgnd(CDC* pDC) 
	{
		ClipChildren(pDC);
		
		return CPropertyPage::OnEraseBkgnd(pDC);
	}
}
