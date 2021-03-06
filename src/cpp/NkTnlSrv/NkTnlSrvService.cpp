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

#include "NkTnlSrvService.h"

CNkTnlSrvService::CNkTnlSrvService(LPCWSTR pszServiceName)
	: CServiceBase(pszServiceName)
{
	NkTrace::CTrace& Trace = NkTrace::CTrace::Instance();
	Trace.Subscribe(this);

	NkSocket::CSocket::initialize();
	NkSSL::COpenSSLCtx::initialize();

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
}

CNkTnlSrvService::~CNkTnlSrvService(void)
{
	NkSSL::COpenSSLCtx::cleanup();
	NkSocket::CSocket::cleanup();

	delete m_pLogFile;
	CoUninitialize();
}

void CNkTnlSrvService::TraceOut(NkTrace::CTrace::TraceClasses traceclass, const wchar_t* pszText)
{
	if (!StartedAsService()) {
		wprintf(L"%s\r\n", pszText);
	}
	if (m_pLogFile) {
		m_pLogFile->FormatLog(L"%s\r\n", pszText);
	}
}

void CNkTnlSrvService::OnStart(DWORD dwArgc, PWSTR* pszArgv)
{
	NkThreading::CLockGuard lock(m_lock);

	m_shutdown = false;
	m_use_ssl = NkType::to_bool(NkOPC::CTunnelRegEntry::ServerUseSSL());

	NkTrace::CTrace& Trace = NkTrace::CTrace::Instance();

#ifdef _DEBUG
	Trace.TraceLevel(NkTrace::CTrace::TraceLevel_3);
#else
	Trace.TraceLevel(static_cast<NkTrace::CTrace::TraceLevels>
		(NkOPC::CTunnelRegEntry::ServerTraceLevel()));
#endif

	try {
		if (m_pLogFile == nullptr && Trace.IsTraceLevel1()) {
			m_pLogFile = new NkTrace::CLogFile(NKOPCTnl::LogPath
			                                   , NKOPCTnl::ServerLogAppName);
		}
	}
	catch (...) {
	}


	try {

		if (m_use_ssl) {

			m_ssl_ctx.create_TLSv1_2_server();

			std::string path;
			NkOPC::CTunnelRegEntry::GetServerCertPath(path);
			path += "server.crt";
			m_ssl_ctx.certificate_file(path.c_str());

			NkOPC::CTunnelRegEntry::GetServerCertPath(path);
			path += "server.key";
			m_ssl_ctx.use_private_key_file(path.c_str());

			if (NkOPC::CTunnelRegEntry::ServerVerifyClient()) {
				NkOPC::CTunnelRegEntry::GetServerCertPath(path);
				path += "ca.crt";
				m_ssl_ctx.load_verify_locations(path.c_str());
				m_ssl_ctx.set_verify();
			}
		}

		std::string local_address("0.0.0.0/");
		local_address += std::to_string(NkOPC::CTunnelRegEntry::ServerPort());

		m_Listener.listen(&CNkTnlSrvService::on_accept, this, local_address.c_str());
	}
	catch (NkError::CException& e) {
		e.report();
		throw;
	}
}

void CNkTnlSrvService::OnStop()
{
	NkThreading::CLockGuard lock(m_lock);

	m_shutdown = true;

	try {
		m_Listener.shutdown();
	}
	catch (NkError::CException& e) {
		e.report();
	}

	for (auto i : m_Servers) {
		i->shutdown();
	}

	delete m_pLogFile;
	m_pLogFile = nullptr;
}

void CNkTnlSrvService::on_accept(SOCKET so)
{
	NkThreading::CLockGuard lock(m_lock);

	NkOPC::COPCFarSrv* server = nullptr;

	try {

		if (m_use_ssl) {
			auto socket = new NkSSL::CSocket(so);

			try {
				socket->ssl_accept(m_ssl_ctx);
			}
			catch (...) {
				delete socket;
				throw;
			}

			server = new NkOPC::COPCFarSrv(socket, this);

			server->server_id(++m_next_server_id);
			server->AddRef();

			std::string peer_name;
			socket->get_peer_address(peer_name);
			NkTrace::CTrace::trace_info("COPCFarSrv -- client[%d] ssl connected from : %s"
				, m_next_server_id, peer_name.c_str());

		}
		else {
			auto socket = new NkSocket::CSocket(so);
			
			server = new NkOPC::COPCFarSrv(socket, this);

			server->server_id(++m_next_server_id);
			server->AddRef();

			std::string peer_name;
			socket->get_peer_addr(peer_name);
			NkTrace::CTrace::trace_info("COPCFarSrv -- client[%d] connected from : %s"
			                            , m_next_server_id, peer_name.c_str());

		}

		m_Servers.push_back(server);
	}
	catch (NkError::CException& e) {
		e.report();
		delete server;
	}
}

void CNkTnlSrvService::on_shutdown(NkCom::CServer* p)
{
	NkThreading::CLockGuard lock(m_lock);

	if (!m_shutdown) {
		if (p) {
			NkTrace::CTrace::trace_info("COPCFarSrv -- client[%d] disconnected"
			                            , p->server_id());

			p->Release();
		}
		m_Servers.remove(p);
	}
}
