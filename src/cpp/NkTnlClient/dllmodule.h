#pragma once

class CDllModule : public NkCom::CModule
	, public NkCom::CLogonEvent
	, public NkTrace::CTraceEvent
{
public:
	CDllModule(void);
	virtual ~CDllModule(void);

	HRESULT DllRegisterServer();
	HRESULT DllUnregisterServer();
	BOOL DllMain(HANDLE hinstDLL, DWORD  ul_reason_for_call, LPVOID lpReserved);
	HRESULT DllGetClassObject(REFCLSID clsid, REFIID riid, void** ppv);

	HRESULT on_logon(NkCom::CServer* p);
	void TraceOut(NkTrace::CTrace::TraceClasses traceclass, const wchar_t* pszText);

private:


#if defined NK_USE_SSL
	NkSSL::CNKOpenSSLCtx m_ssl_ctx;
#endif
};
