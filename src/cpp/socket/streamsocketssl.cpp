#include "StdAfx.h"
#include "streamsocketssl.h"
#include "error/baseexception.h"
#include "error/socketexception.h"
#include "socket/interfaces.h"
#include "socket/socket.h"
#include "socket/wsaevent.h"
#include "ssl/openssl.h"
#include "ssl/NKOpenSSLCtx.h"
#include <crtdbg.h>

using namespace NkError;

#if defined NK_USE_SSL
namespace NkSocket
{
	class CStreamSocketSSl::CImpl
	{
	public:
		CImpl()
			: m_pThis(0)
			, m_pServer(0)
			, m_Id(0)
			, m_bIsServer(false)
		{
		}

		~CImpl()
		{
		}

		CSocket m_Socket;
		NkSSL::CNKOpenSSL m_SSL;
		CWsaEvent m_SocketEvent;
		CStreamSocketSSl* m_pThis;
		CServiceServer* m_pServer;
		size_t m_Id;
		bool m_bIsServer;
		static size_t m_NextId;

		void accept(const CSocket& socket, NkSSL::CNKOpenSSLCtx& ssl_ctx)
		{
			try {
				m_Socket = socket.accept();
				SetId();

				m_SSL.create(ssl_ctx.data());
				m_SSL.set_fd(m_Socket);

				while (1)
				{
					int error = m_SSL.accept();

					if (error == SSL_ERROR_NONE) {
						break;
					}
					else if (error == SSL_ERROR_WANT_READ) {
						wait_for_network_event(FD_READ);
					}
					else if (error == SSL_ERROR_WANT_WRITE) {
						wait_for_network_event(FD_WRITE);
					}
				}

				if (NkTrace::CTrace::IsTraceLevel1()) {
					sockaddr_in addr;
					m_Socket.get_peer_addr(addr);
					const SSL_CIPHER* cipher = m_SSL.get_current_cipher();
					NkError::CBaseException::check_pointer(cipher, __FILE__, __LINE__);
					char buffer[16] = "";
					NkTrace::CTrace::trace_info(
						"CNKStreamSocketSSl -- SSL client[%d] connected from: %s port: %d cipher: %s"
						, m_Id, inet_ntop(AF_INET, &addr.sin_addr, buffer, 16)
						, ntohs(addr.sin_port), cipher->name);
				}

				m_bIsServer = true;
			}
			catch (...) {
				m_Socket.close();
				throw;
			}
		}

		void connect(const CBind& bind, NkSSL::CNKOpenSSLCtx& ssl_ctx)
		{
			try {
				SetId();

				m_Socket.socket();
				m_Socket.non_blocking_mode();
//				m_Socket.connect(bind.sock_addr(), sizeof(sockaddr_in));

				if (!wait_for_network_event(FD_CONNECT)) {
					return;
				}

				m_SSL.create(ssl_ctx.data());
				m_SSL.set_fd(m_Socket);

				while (1)
				{
					int error = m_SSL.connect();

					if (error == SSL_ERROR_NONE) {
						break;
					}
					else if (error == SSL_ERROR_WANT_READ) {
						wait_for_network_event(FD_READ);
					}
					else if (error == SSL_ERROR_WANT_WRITE) {
						wait_for_network_event(FD_WRITE);
					}
				}

				/*
				if (NkTrace::CTrace::IsTraceLevel1()) {
					const SSL_CIPHER* cipher = m_SSL.get_current_cipher();
					NkError::CBaseException::check_pointer(cipher, __FILE__, __LINE__);
					NkTrace::CTrace::trace_info("CStreamSocket -- client[%d] connected to : %s port: %d cipher: %s"
						, m_Id, bind.ip(), bind.port(), cipher->name);
				}
				*/

				m_bIsServer = false;
			}
			catch (...) {
				m_Socket.close();
				throw;
			}
		}

		size_t read(void* p, size_t cb)
		{
			_ASSERT(p != 0);
			_ASSERT(cb > 0);

			if (p == 0 || cb == 0) {
				return 0;
			}

			size_t cb_recv = 0;

			while (cb_recv == 0) {
				cb_recv = m_SSL.read(p, cb);
				//cb_recv = m_Socket.recv(static_cast<char*>(p), cb);
				if (cb_recv == 0) {
					if (!wait_for_network_event(FD_READ)) {
						return 0;
					}
				}
			}
			return cb_recv;
		}

		size_t write(const void* p, size_t cb)
		{
			_ASSERT(p != 0);
			_ASSERT(cb > 0);

			if (p == 0 || cb == 0) {
				return 0;
			}

			size_t cb_written = 0;

			while (cb_written == 0) {
				cb_written = m_SSL.write(p, cb);
				//cb_written = m_Socket.send(static_cast<const char*>(p), cb);
				if (cb_written == 0) {
					if (!wait_for_network_event(FD_WRITE)) {
						return 0;
					}
				}
			}
			return cb_written;
		}

		void close()
		{
			if (m_Socket.issocket()) {
				//TODO:
				//m_Socket.shutdown(SD_BOTH);
				//m_SSL.close();
				m_SocketEvent.set();
				m_Socket.close();
			}
		}

		bool is_closed()
		{
			return !m_Socket.issocket();
		}

		void SetId()
		{
			if (m_NextId == MAXSIZE_T) {
				m_NextId = 0;
			}
			++m_NextId;
			m_Id = m_NextId;
		}

		bool can_read()
		{
			return wait_for_network_event(FD_READ);
		}

		void cancel_wait()
		{
			m_SocketEvent.set();
		}

		bool wait_for_network_event(long network_event)
		{
			m_SocketEvent.reset();
			m_SocketEvent.event_select(m_Socket, FD_CLOSE | network_event);
			m_SocketEvent.wait();

			if (!m_Socket.issocket()) {
				NkTrace::CTrace::Instance().Trace(NkTrace::CTrace::TraceAlwaysInfo
					, "CStreamSocket -- client[%d] disconnected", m_Id);

				throw CBaseException(CBaseException::connection_closed, __FILE__, __LINE__);
			}

			WSANETWORKEVENTS wsaevents = { 0 };
			m_SocketEvent.enum_events(m_Socket, wsaevents);
			CSocketException::check(wsaevents, FD_CLOSE | network_event, __FILE__, __LINE__);

			if (wsaevents.lNetworkEvents & FD_CLOSE) {
				NkTrace::CTrace::Instance().Trace(NkTrace::CTrace::TraceAlwaysInfo
					, "CStreamSocket -- client[%d] disconnected", m_Id);

				m_Socket.close();
				throw CBaseException(CBaseException::connection_closed, __FILE__, __LINE__);
			}

			if (wsaevents.lNetworkEvents & network_event) {
				return true;
			}

			return false;
		}
	};

	size_t CStreamSocketSSl::CImpl::m_NextId = 0;

	CStreamSocketSSl::CStreamSocketSSl() : _Impl(new CImpl())
	{
		_Impl->m_pThis = this;
	}

	CStreamSocketSSl::~CStreamSocketSSl()
	{
		delete _Impl;
	}

	void CStreamSocketSSl::accept(const CSocket& socket, NkSSL::CNKOpenSSLCtx& ssl_ctx)
	{
		_Impl->accept(socket, ssl_ctx);
	}

	void CStreamSocketSSl::connect(const CBind& bind, NkSSL::CNKOpenSSLCtx& ssl_ctx)
	{
		_Impl->connect(bind, ssl_ctx);
	}

	size_t CStreamSocketSSl::read(void* p, size_t cb, ULONG32 id /*= 0*/)
	{
		//this implementation can't handle multiple threads
		_ASSERT(id == 0);
		return _Impl->read(p, cb);
	}

	size_t CStreamSocketSSl::write(const void* p, size_t cb, ULONG32 id/*= 0*/)
	{
		//this implementation can't handle multiple threads
		_ASSERT(id == 0);
		return _Impl->write(p, cb);
	}

	void CStreamSocketSSl::close(ULONG32 id/*= 0*/) throw()
	{
		//this implementation can't handle multiple threads
		_ASSERT(id == 0);
		_Impl->close();
	}

	bool CStreamSocketSSl::is_closed() const throw()
	{
		return _Impl->is_closed();
	}

	bool CStreamSocketSSl::can_read(ULONG32 id /*= 0*/)
	{
		//this implementation can't handle multiple threads
		_ASSERT(id == 0);
		return _Impl->can_read();
	}

	void CStreamSocketSSl::cancel_wait(ULONG32 id /*= 0*/)
	{
		//this implementation can't handle multiple threads
		_ASSERT(id == 0);
		_Impl->cancel_wait();
	}
}
#endif