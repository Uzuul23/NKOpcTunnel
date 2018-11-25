#include "StdAfx.h"
#include "NkTnlSrvService.h"

CNkTnlSrvService::CNkTnlSrvService(LPCWSTR pszServiceName)
	: CServiceBase(pszServiceName)
	, m_shutdown(false)
{
	NkTrace::CTrace& Trace = NkTrace::CTrace::Instance();
	Trace.Subscribe(this);

	NkSocket::CSocket::initialize();

#if defined NK_USE_SSL
	NkSSL::CNKOpenSSLCtx::initialize();
#endif	


	HRESULT hr = ::CoInitializeEx(0, COINIT_MULTITHREADED);
	NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
}

CNkTnlSrvService::~CNkTnlSrvService(void)
{
	
#if defined NK_USE_SSL
	NkSSL::CNKOpenSSLCtx::cleanup();
#endif	

	NkSocket::CSocket::cleanup();

	delete m_pLogFile;
	::CoUninitialize();
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

void CNkTnlSrvService::OnStart(DWORD dwArgc, PWSTR *pszArgv)
{
	NkThreading::CLockGuard lock(m_lock);

	m_shutdown = false;

	NkTrace::CTrace& Trace = NkTrace::CTrace::Instance();

#ifdef _DEBUG
	Trace.TraceLevel(NkTrace::CTrace::TraceLevel_3);
#else
	Trace.TraceLevel(static_cast<NkTrace::CTrace::TraceLevels>
		(NkOPC::CTunnelRegEntry::ServerTraceLevel()));
#endif

	try {
		if (m_pLogFile == 0 && Trace.IsTraceLevel1()) {
			m_pLogFile = new NkTrace::CLogFile(NKOPCTnl::LogPath
				, NKOPCTnl::ServerLogAppName);
		}
	}
	catch (...) {}


	try {
		//BYTE key[32] = { 0 };
		//NkWin::CLocalMachine::local_machine_dependent_key(key);

//#if defined NK_USE_SSL
//		if (m_ssl_ctx.data() == 0) {
//			NkSSL::CNKOpenSSLCtx::initialize();
//			m_ssl_ctx.create_TLSv1_2_server();
//			m_ssl_ctx.certificate_file("C:\\Users\\Paul\\Documents\\Visual Studio 2015\\Projects\\NkOpcTunnel\\BIN\\certs\\server.crt");
//			m_ssl_ctx.set_default_passwd("yFsrT41iC2OgajP");
//			m_ssl_ctx.use_private_key_file("C:\\Users\\Paul\\Documents\\Visual Studio 2015\\Projects\\NkOpcTunnel\\BIN\\certs\\private\\server.key");
//		}
//#endif
		
		std::string local_addres("0.0.0.0/");
		local_addres += std::to_string(NkOPC::CTunnelRegEntry::ServerPort());

		m_Listener.listen(&CNkTnlSrvService::on_accept, this, local_addres.c_str());
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

	try
	{
		m_Listener.shutdown();
	}
	catch (NkError::CException& e) {
		e.report();
	}

	for (auto i : m_Servers) {
		i->shutdown();
	}

	delete m_pLogFile;
	m_pLogFile = 0;
}

void CNkTnlSrvService::on_accept(SOCKET so)
{
	NkThreading::CLockGuard lock(m_lock);

//#if defined NK_USE_SSL
//	NkSocket::CStreamSocketSSl* p_socket = 0;
//	NkOPC::COPCFarSrv* p_server = 0;
//
//	try {
//		p_socket = new NkSocket::CStreamSocketSSl();
//		p_socket->accept(so, m_ssl_ctx);
//
//		p_server = new NkOPC::COPCFarSrv(p_socket, this, true);
//		p_server->AddRef();
//
//		m_Servers.push_back(p_server);
//	}
//	catch (NkError::CException& e) {
//		delete p_socket;
//		delete p_server;
//		e.report();
//	}
//#else

	NkOPC::COPCFarSrv* p_server = 0;
	try {

		NkSocket::CSocket* p_socket = new NkSocket::CSocket(so);
		NkOPC::COPCFarSrv* p_server = new NkOPC::COPCFarSrv(p_socket, this, true);
		
		p_server->server_id(++m_next_server_id);
		p_server->AddRef();

		std::string peer_name;
		p_socket->get_peer_addr(peer_name);
		NkTrace::CTrace::trace_info("COPCFarSrv -- client[%d] connected from : %s"
			, m_next_server_id, peer_name.c_str());
			
		m_Servers.push_back(p_server);
	}
	catch (NkError::CException& e) {
		e.report();
		delete p_server;
	}

//#endif
}

void CNkTnlSrvService::on_shutdown(NkCom::CServer* p)
{
	NkThreading::CLockGuard lock(m_lock);

	if (!m_shutdown) {
		if (p)
		{
			NkTrace::CTrace::trace_info("COPCFarSrv -- client[%d] disconnected"
				, p->server_id());

			p->Release();
		}
		m_Servers.remove(p);
	}
}