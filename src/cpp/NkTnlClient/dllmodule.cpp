#include "StdAfx.h"
#include "dllmodule.h"

CDllModule::CDllModule(void)
{
}

CDllModule::~CDllModule(void)
{
}

HRESULT CDllModule::DllRegisterServer()
{
	try
	{
		//TODO: Error handling
		WCHAR szFileName[MAX_PATH] = L"";
		DWORD ret = ::GetModuleFileNameW(get_module(), szFileName, MAX_PATH);

		NkWin::CRegistry key(0, HKEY_LOCAL_MACHINE, KEY_CREATE_SUB_KEY);
		NkWin::CRegistry keySettings;
		key.CreateKey(NKOPCTnl::RegKeySettings, &keySettings, KEY_WRITE);
		keySettings.SetValue(szFileName, NKOPCTnl::RegValueClientInstallPath);
	}
	catch (NkError::CException& e)
	{
		e.report();
		return e.error();
	}

	return S_OK;
}
HRESULT CDllModule::DllUnregisterServer()
{
	try
	{
		NkWin::CRegistry key(0, HKEY_LOCAL_MACHINE, KEY_WRITE);
		NkWin::CRegistry keySettings;
		keySettings.Open(NKOPCTnl::RegKeySettings, key);
		keySettings.DeleteKeyValue(NKOPCTnl::RegValueClientInstallPath);
	}
	catch (NkError::CException& e)
	{
		e.report();
	}
	return S_OK;
}

HRESULT CDllModule::DllGetClassObject(REFCLSID clsid, REFIID riid, void** ppv)
{
	try {
		NkThreading::CLockGuard lock(get_global_lock());

		NkError::CBaseException::check(NkType::to_bool(riid == IID_IClassFactory
			|| riid == IID_IClassFactory2), CLASS_E_CLASSNOTAVAILABLE, __FILE__, __LINE__);

		NkOPC::CTunnelRegEntry Entry(clsid);

//#if defined NK_USE_SSL
#if defined NK__
		if (Entry.RemoteServerUseSSl()) {
			if (m_ssl_ctx.data() == 0) {
				nk_ssl::c_open_ssl_ctx::initialize();
				m_ssl_ctx.create_TLSv1_2_client();
			}

			NkSocket::CBind bind;
			bind.bin_ip(Entry.RemoteServerIPAddress());
			bind.port(static_cast<u_short>(Entry.RemoteServerPort()));

			NkCom::CComPtr<NkCom::CServer> spSrv(NkOPC::COPCNearSrv::create_new_server_ssl(bind
				, m_ssl_ctx, this));

			return spSrv->remote_create_class_factory(Entry.RemoteServerCLSID()
				, Entry.ClsContext(), IID_IOPCServer, riid, ppv);
		}
		else {
			NkSocket::CBind bind;
			bind.bin_ip(Entry.RemoteServerIPAddress());
			bind.port(static_cast<u_short>(Entry.RemoteServerPort()));

			NkCom::CComPtr<NkCom::CServer> spSrv(NkOPC::COPCNearSrv::create_new_server(&bind
				, this));

			return spSrv->remote_create_class_factory(Entry.RemoteServerCLSID()
				, Entry.ClsContext(), IID_IOPCServer, riid, ppv);
		}
#else
		
		std::string addr;
		NkType::to_Addr(Entry.RemoteServerIPAddress(), Entry.RemoteServerPort(), addr);

		NkOPC::COPCNearSrv* p_srv = NkOPC::COPCNearSrv::create_new_server(addr.c_str());
		p_srv->clsid(clsid);

		NkError::CBaseException::check_result(on_logon(p_srv), __FILE__, __LINE__);

		HRESULT hr = p_srv->remote_create_class_factory(Entry.RemoteServerCLSID()
			, Entry.ClsContext(), IID_IOPCServer, riid, ppv);

		return hr;
#endif
	}
	catch (NkError::CException& e) {
		e.report();
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	return S_OK;
}

BOOL CDllModule::DllMain(HANDLE hinstDLL, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (__super::DllMain(hinstDLL, ul_reason_for_call, lpReserved) == FALSE) {
		return FALSE;
	}

	try
	{
		switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: {
			NkTrace::CTrace& Trace = NkTrace::CTrace::Instance();
			Trace.Subscribe(this);
			Trace.TraceLevel(NkTrace::CTrace::TraceLevel_3);
			NkSocket::CSocket::initialize();
			break;
		}
		case DLL_PROCESS_DETACH: {
#if defined NK_USE_SSL
			nk_ssl::c_open_ssl_ctx::cleanup();
#endif
			NkSocket::CSocket::cleanup();
			NkTrace::CTrace::Instance().UnSubscribe(this);
			break;
		}
		}
	}
	catch (NkError::CException& e) {
		e.report();
		return FALSE;
	}
	return TRUE;
}

void CDllModule::TraceOut(NkTrace::CTrace::TraceClasses traceclass, const wchar_t* pszText)
{
	::OutputDebugString(pszText);
	::OutputDebugString(L"\r\n");
}

HRESULT CDllModule::on_logon(NkCom::CServer* p)
{
	_ASSERT(p);

	NkOPC::CTunnelRegEntry reg(p->clsid());
	NkUtil::CPassVault vault(reg.FullName());

	if (vault.readIf()) {
		LPBYTE pBlob = 0;
		size_t cb = 0;
		vault.createAuthenticationBlob(&pBlob, cb);
		HRESULT hr = p->remote_logon(pBlob, cb);
		vault.deleteAuthenticationBlob(pBlob, cb);
		if (SUCCEEDED(hr)) {
			return hr;
		}
	}

	bool bSafe = false;
	NkType::CString strCaption(L"Please enter your server credentials.");
	if (vault.cred_dlg(strCaption, reg.FullName(), &bSafe)) {
		LPBYTE pBlob = 0;
		size_t cb = 0;
		vault.createAuthenticationBlob(&pBlob, cb);
		HRESULT hr = p->remote_logon(pBlob, cb);
		vault.deleteAuthenticationBlob(pBlob, cb);
		if (SUCCEEDED(hr)) {
			if (bSafe) {
				vault.write();
			}
			return hr;
		}
	}

	return E_ACCESSDENIED;
}
