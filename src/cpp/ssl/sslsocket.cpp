#include "stdafx.h"
#include "ssl/sslsocket.h"
#include "ssl/NKOpenSSLCtx.h"
#include "error/baseexception.h"
#include "error/socketexception.h"
#include "error/sslexception.h"

namespace NkSSL
{
	class CSSLSocket::CImpl
	{
	public:
		CImpl()
		{
		}
		~CImpl()
		{
		}
		SOCKET m_socket = INVALID_SOCKET;
		SSL* m_pSSL = 0;
	};

	CSSLSocket::CSSLSocket() noexcept : _Impl(new CImpl())
	{
	}

	CSSLSocket::CSSLSocket(SOCKET so) noexcept : _Impl(new CImpl())
	{
		_Impl->m_socket = so;
	}

	CSSLSocket::~CSSLSocket()
	{
		close();
		delete _Impl;
	}

	void CSSLSocket::socket()
	{
		close();
		_Impl->m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		NkError::CSocketException::check(_Impl->m_socket, "socket", __FILE__, __LINE__);
	}

	void CSSLSocket::shutdown(int how /*= SD_SEND*/)
	{
		int ret = ::shutdown(_Impl->m_socket, how);
		NkError::CSocketException::check(ret, "shutdown", __FILE__, __LINE__);
	}

	void CSSLSocket::connect(const char* addr)
	{
		std::string str(addr);

		size_t found = str.find("/");
		if (found == std::string::npos)
		{
			throw NkError::CBaseException(NkError::CBaseException::invalid_argument, __FILE__, __LINE__);
		}

		std::string str_addr(str.substr(0, found));
		std::string str_port(str.substr(found + 1));

		sockaddr_in addr_in;
		ZeroMemory(&addr_in, sizeof(addr_in));
		addr_in.sin_family = AF_INET;
		inet_pton(AF_INET, str_addr.c_str(), &addr_in.sin_addr);
		addr_in.sin_port = htons(static_cast<USHORT>(std::stoi(str_port)));

		int ret = ::connect(_Impl->m_socket, (SOCKADDR*)&addr_in, sizeof(addr_in));
		NkError::CSocketException::check(ret, "connect", __FILE__, __LINE__);
	}

	void CSSLSocket::ssl_accept(CNKOpenSSLCtx& ssl_ctx)
	{
		_Impl->m_pSSL = SSL_new(ssl_ctx.data());
		NkError::CSSLException::check_create(_Impl->m_pSSL, "SSL_new", __FILE__, __LINE__);

		int ret = SSL_set_fd(_Impl->m_pSSL, _Impl->m_socket);
		NkError::CSSLException::check_result(ret, "SSL_set_fd", __FILE__, __LINE__);

		ret = SSL_accept(_Impl->m_pSSL);
		int error = SSL_get_error(_Impl->m_pSSL, ret);

		NkError::CSSLException::check_result(ret, "SSL_accept", __FILE__, __LINE__);
	}

	void CSSLSocket::ssl_connect(CNKOpenSSLCtx& ssl_ctx)
	{
		_Impl->m_pSSL = SSL_new(ssl_ctx.data());
		NkError::CSSLException::check_create(_Impl->m_pSSL, "SSL_new", __FILE__, __LINE__);

		int ret = SSL_set_fd(_Impl->m_pSSL, _Impl->m_socket);
		NkError::CSSLException::check_result(ret, "SSL_set_fd", __FILE__, __LINE__);

		ret = SSL_connect(_Impl->m_pSSL);
		int error = SSL_get_error(_Impl->m_pSSL, ret);

		NkError::CSSLException::check_result(ret, "SSL_connect", __FILE__, __LINE__);
	}

	void CSSLSocket::get_peer_addr(std::string& addr)
	{
		sockaddr_in addr_in;
		int cb = sizeof(sockaddr_in);
		int ret = ::getpeername(_Impl->m_socket, (SOCKADDR*)&addr, &cb);
		NkError::CSocketException::check(ret, "getpeername", __FILE__, __LINE__);

		char buffer[16] = "";
		inet_ntop(AF_INET, &addr_in.sin_addr, buffer, sizeof(buffer));
		addr = buffer;
		addr += "/";
		addr += std::to_string(ntohs(addr_in.sin_port));
	}

	size_t CSSLSocket::read(void* p, size_t cb, ULONG32 id /*= 0*/)
	{
		_ASSERT(p != 0);
		_ASSERT(cb > 0);

		if (p == 0 || cb == 0) {
			return 0;
		}

		int ret = SSL_read(_Impl->m_pSSL, p, static_cast<int>(cb));

		if (ret == 0 || (ret < 0 
			&& SSL_get_error(_Impl->m_pSSL, ret) == SSL_ERROR_SYSCALL)) {
			throw NkError::CBaseException(NkError::CBaseException::connection_closed, __FILE__, __LINE__);
		}

		NkError::CSSLException::check_result(ret, "SSL_read", __FILE__, __LINE__);
		return static_cast<size_t>(ret);
	}

	size_t CSSLSocket::write(const void* p, size_t cb, ULONG32 id /*= 0*/)
	{
		_ASSERT(p != 0);
		_ASSERT(cb > 0);

		if (p == 0 || cb == 0) {
			return 0;
		}

		int ret = SSL_write(_Impl->m_pSSL, p, static_cast<int>(cb));

		if (ret == 0 || (ret < 0
			&& SSL_get_error(_Impl->m_pSSL, ret) == SSL_ERROR_SYSCALL)) {
			throw NkError::CBaseException(NkError::CBaseException::connection_closed, __FILE__, __LINE__);
		}

		NkError::CSSLException::check_result(ret, "SSL_write", __FILE__, __LINE__);

		return static_cast<size_t>(ret);
	}

	void CSSLSocket::close(ULONG32 id /*= 0*/) noexcept
	{
		if (_Impl->m_socket != INVALID_SOCKET) {
			int ret = ::closesocket(_Impl->m_socket);
			NkError::CSocketException::check(ret, "closesocket", __FILE__, __LINE__);
			_Impl->m_socket = INVALID_SOCKET;
		}
	}

	bool CSSLSocket::is_closed() const noexcept
	{
		return _Impl->m_socket == INVALID_SOCKET;
	}

}

