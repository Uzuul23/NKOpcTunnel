
// NkTnlClientCnf.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CNkTnlClientCnfApp:
// See NkTnlClientCnf.cpp for the implementation of this class
//

class CNkTnlClientCnfApp : public CWinApp, public NkTrace::CTraceEvent
{
public:
	CNkTnlClientCnfApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	void TraceOut(NkTrace::CTrace::TraceClasses traceclass, const wchar_t* pszText);
};

extern CNkTnlClientCnfApp theApp;