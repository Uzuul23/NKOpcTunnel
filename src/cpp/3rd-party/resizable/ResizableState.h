#if !defined(AFX_RESIZABLESTATE_H__INCLUDED_)
#define AFX_RESIZABLESTATE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace nkresize
{
	class CResizableState  
	{
	protected:
		// non-zero if successful
		BOOL LoadWindowRect(LPCTSTR pszSection, BOOL bRectOnly);
		BOOL SaveWindowRect(LPCTSTR pszSection, BOOL bRectOnly);
	
		virtual CWnd* GetResizableWnd() = 0;
	
	public:
		CResizableState();
		virtual ~CResizableState();
	};
}

#endif // !defined(AFX_RESIZABLESTATE_H__INCLUDED_)
