/*	This file is part of NKOpcTunnel.
*
*	Copyright (c) Henryk Anschuetz 
*	Berlin, Germany
*
*	mailto:uzuul23@online.de
*
*	NKOpcTunnel is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   NKOpcTunnel is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with NKOpcTunnel.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "stdafx.h"

#include "dllmodule.h"
#include "opc/opcregistry.h"


HRESULT CDllModule::DllRegisterServer()
{
	try {
		//TODO: Error handling
		WCHAR szFileName[MAX_PATH] = L"";
		DWORD ret = GetModuleFileNameW(get_module(), szFileName, MAX_PATH);

		NkWin::CRegistry key(nullptr, HKEY_LOCAL_MACHINE, KEY_CREATE_SUB_KEY);
		NkWin::CRegistry keySettings;
		key.CreateKey(NKOPCTnl::RegKeySettings, &keySettings, KEY_WRITE);

		keySettings.SetValue(szFileName, NKOPCTnl::RegValueClientInstallPath);

		std::wstring str(szFileName);
		auto find = str.rfind('\\');
		if (find != std::string::npos) {
			str = str.substr(0, find);
		}
		find = str.rfind('\\');
		if (find != std::string::npos) {
			str = str.substr(0, find + 1);
		}
		str += L"cert\\";
		key.CreateKey(NKOPCTnl::RegKeySettings, &keySettings, KEY_WRITE);
		keySettings.SetValue(str.c_str(), NKOPCTnl::RegValueClientCertPath);
	}
	catch (NkError::CException& e) {
		e.report();
		return e.error();
	}

	return S_OK;
}

HRESULT CDllModule::DllUnregisterServer()
{
	try {
		NkWin::CRegistry key(nullptr, HKEY_LOCAL_MACHINE, KEY_WRITE);
		NkWin::CRegistry keySettings;
		keySettings.Open(NKOPCTnl::RegKeySettings, key);
		keySettings.DeleteKeyValueIf(NKOPCTnl::RegValueClientInstallPath);
		keySettings.DeleteKeyValueIf(NKOPCTnl::RegValueClientCertPath);

		std::vector<NkOPC::CTunnelRegEntry> entries;
		NkOPC::CTunnelRegEntry::EnumEntries(entries);
		for (auto i : entries) {
			i.Remove();
		}
	}
	catch (NkError::CException& e) {
		e.report();
	}
	return S_OK;
}

HRESULT CDllModule::DllGetClassObject(REFCLSID clsid, REFIID riid, void** ppv)
{
	try {
		NkThreading::CLockGuard lock(get_global_lock());

		NkError::CBaseException::check(NkType::to_bool(riid == IID_IClassFactory
			|| riid == IID_IClassFactory2)
			, CLASS_E_CLASSNOTAVAILABLE, __FILE__, __LINE__);

		NkOPC::CTunnelRegEntry entry(clsid);

		std::string address;
		NkType::to_Addr(entry.RemoteServerIPAddress(), entry.RemoteServerPort(), address);

		if (entry.RemoteServerUseSSl()) {
			NkSSL::COpenSSLCtx ctx;
			entry.Setup(ctx);

			auto srv = NkOPC::COPCNearSrv::create_new_server_ssl(address.c_str(), ctx);
			srv->clsid(clsid);

			//NkError::CBaseException::check_result(on_logon(p_srv), __FILE__, __LINE__);

			const auto hr = srv->remote_create_class_factory(entry.RemoteServerCLSID()
				, entry.ClsContext(), IID_IOPCServer, riid, ppv);

			return hr;
		}

		auto srv = NkOPC::COPCNearSrv::create_new_server(address.c_str());
		srv->clsid(clsid);

		//NkError::CBaseException::check_result(on_logon(srv), __FILE__, __LINE__);

		const auto hr = srv->remote_create_class_factory(entry.RemoteServerCLSID()
			, entry.ClsContext(), IID_IOPCServer, riid, ppv);

		return hr;
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

	try {
		switch (ul_reason_for_call) {
			case DLL_PROCESS_ATTACH: {
				NkTrace::CTrace& Trace = NkTrace::CTrace::Instance();
				Trace.Subscribe(this);
				Trace.TraceLevel(NkTrace::CTrace::TraceLevel_3);
				NkSocket::CSocket::initialize();
				NkSSL::COpenSSLCtx::initialize();
				break;
			}
			case DLL_PROCESS_DETACH: {
				NkSocket::CSocket::cleanup();
				NkSSL::COpenSSLCtx::cleanup();
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
		LPBYTE pBlob = nullptr;
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
		LPBYTE pBlob = nullptr;
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
