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

class CNkTnlSrvService
	: public NkService::CServiceBase
	  , public NkCom::CServerEvent
	  , public NkTrace::CTraceEvent
{
public:
	CNkTnlSrvService(LPCWSTR pszServiceName);
	~CNkTnlSrvService(void);

	//CTraceEvent
	void TraceOut(NkTrace::CTrace::TraceClasses traceclass, const wchar_t* pszText) override;

	//CServiceBase
	void OnStart(DWORD dwArgc, PWSTR* pszArgv) override;
	void OnStop() override;

	//NkCom::CServerEvent
	void on_shutdown(NkCom::CServer* p) override;

	//CSocketListener
	void on_accept(SOCKET so);

private:
	std::list<NkCom::CServer*> m_Servers;
	NkSocket::CSocketListener<CNkTnlSrvService> m_Listener;
	NkTrace::CLogFile* m_pLogFile = nullptr;
	NkThreading::CCriticalSection m_lock;
	bool m_shutdown = false;
	bool m_use_ssl = true;
	ULONG32 m_next_server_id = 0;
	NkSSL::COpenSSLCtx m_ssl_ctx;
};
