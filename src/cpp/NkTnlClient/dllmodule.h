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

#pragma once

class CDllModule : public NkCom::CModule
	, public NkCom::CLogonEvent
	, public NkTrace::CTraceEvent
{
public:
	CDllModule(void) = default;
	virtual ~CDllModule(void) = default;

	HRESULT DllRegisterServer();
	HRESULT DllUnregisterServer();
	BOOL DllMain(HANDLE hinstDLL, DWORD  ul_reason_for_call, LPVOID lpReserved);
	HRESULT DllGetClassObject(REFCLSID clsid, REFIID riid, void** ppv);

	HRESULT on_logon(NkCom::CServer* p);
	void TraceOut(NkTrace::CTrace::TraceClasses traceclass, const wchar_t* pszText);

private:


#if defined NK_USE_SSL
	NkSSL::COpenSSLCtx m_ssl_ctx;
#endif
};
