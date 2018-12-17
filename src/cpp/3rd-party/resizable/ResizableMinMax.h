#if !defined(AFX_RESIZABLEMINMAX_H__INCLUDED_)
#define AFX_RESIZABLEMINMAX_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace nkresize
{
	class CResizableMinMax  
	{
	// Attributes
	private:
		// flags
		BOOL m_bUseMaxTrack;
		BOOL m_bUseMinTrack;
		BOOL m_bUseMaxRect;
	
		POINT m_ptMinTrackSize;		// min tracking size
		POINT m_ptMaxTrackSize;		// max tracking size
		POINT m_ptMaxPos;			// maximized position
		POINT m_ptMaxSize;			// maximized size
	
	public:
		CResizableMinMax();
		virtual ~CResizableMinMax();
	
	protected:
		void MinMaxInfo(LPMINMAXINFO lpMMI);
	
		void SetMaximizedRect(const CRect& rc);		// set window rect when maximized
		void ResetMaximizedRect();					// reset to default maximized rect
		void SetMinTrackSize(const CSize& size);	// set minimum tracking size
		void ResetMinTrackSize();					// reset to default minimum tracking size
		void SetMaxTrackSize(const CSize& size);	// set maximum tracking size
		void ResetMaxTrackSize();					// reset to default maximum tracking size
	};
}

#endif // !defined(AFX_RESIZABLEMINMAX_H__INCLUDED_)
