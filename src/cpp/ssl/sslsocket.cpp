#include "stdafx.h"
#include "ssl/sslsocket.h"
#include "ssl/opensslctx.h"
#include "error/baseexception.h"
#include "error/socketexception.h"
#include "error/sslexception.h"

#pragma warning(disable : 4995)
#include <WS2TCPIP.H>
#pragma warning(default : 4995)


namespace nk_ssl {
	class c_ssl_socket::CImpl {
	public:
		CImpl() {
		}

		~CImpl() {
		}

		SOCKET m_socket = INVALID_SOCKET;
		SSL* m_pSSL = nullptr;
	};

	c_ssl_socket::c_ssl_socket() noexcept : _Impl(new CImpl()) {
	}

	c_ssl_socket::c_ssl_socket(SOCKET so) noexcept : _Impl(new CImpl()) {
		_Impl->m_socket = so;
	}

	c_ssl_socket::~c_ssl_socket() {
		close();
		delete _Impl;
	}

	void c_ssl_socket::socket() {
		close();
		_Impl->m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		NkError::CSocketException::check(_Impl->m_socket, "socket", __FILE__, __LINE__);
	}

	void c_ssl_socket::shutdown(int how /*= SD_SEND*/) {
		int ret = ::shutdown(_Impl->m_socket, how);
		NkError::CSocketException::check(ret, "shutdown", __FILE__, __LINE__);
	}

	void c_ssl_socket::connect(const char* address) {
		std::string str(address);

		size_t found = str.find("/");
		if (found == std::string::npos) {
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

	void c_ssl_socket::ssl_accept(c_open_ssl_ctx& ssl_ctx) {
		_Impl->m_pSSL = SSL_new(ssl_ctx.data());
		NkError::CSSLException::check_create(_Impl->m_pSSL, "SSL_new", __FILE__, __LINE__);

		int ret = SSL_set_fd(_Impl->m_pSSL, _Impl->m_socket);
		NkError::CSSLException::check_result(ret, "SSL_set_fd", __FILE__, __LINE__);

		ret = SSL_accept(_Impl->m_pSSL);
		int error = SSL_get_error(_Impl->m_pSSL, ret);

		NkError::CSSLException::check_result(ret, "SSL_accept", __FILE__, __LINE__);
	}

	void c_ssl_socket::ssl_connect(c_open_ssl_ctx& ssl_ctx) {
		_Impl->m_pSSL = SSL_new(ssl_ctx.data());
		NkError::CSSLException::check_create(_Impl->m_pSSL, "SSL_new", __FILE__, __LINE__);

		int ret = SSL_set_fd(_Impl->m_pSSL, _Impl->m_socket);
		NkError::CSSLException::check_result(ret, "SSL_set_fd", __FILE__, __LINE__);

		ret = SSL_connect(_Impl->m_pSSL);
		int error = SSL_get_error(_Impl->m_pSSL, ret);

		NkError::CSSLException::check_result(ret, "SSL_connect", __FILE__, __LINE__);
	}

	void c_ssl_socket::get_peer_address(std::string& address) {
		sockaddr_in addr_in;
		int cb = sizeof(sockaddr_in);
		int ret = getpeername(_Impl->m_socket, (SOCKADDR*)&address, &cb);
		NkError::CSocketException::check(ret, "getpeername", __FILE__, __LINE__);

		char buffer[16] = "";
		inet_ntop(AF_INET, &addr_in.sin_addr, buffer, sizeof(buffer));
		address = buffer;
		address += "/";
		address += std::to_string(ntohs(addr_in.sin_port));
	}

	size_t c_ssl_socket::read(void* p, size_t cb, ULONG32 id /*= 0*/) {
		_ASSERT(p != nullptr);
		_ASSERT(cb > 0);

		if (p == nullptr || cb == 0) {
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

	size_t c_ssl_socket::write(const void* p, size_t cb, ULONG32 id /*= 0*/) {
		_ASSERT(p != nullptr);
		_ASSERT(cb > 0);

		if (p == nullptr || cb == 0) {
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

	void c_ssl_socket::close(ULONG32 id /*= 0*/) noexcept {
		if (_Impl->m_socket != INVALID_SOCKET) {
			const int ret = closesocket(_Impl->m_socket);
			NkError::CSocketException::check(ret, "closesocket", __FILE__, __LINE__);
			_Impl->m_socket = INVALID_SOCKET;
		}
	}

	bool c_ssl_socket::is_closed() const noexcept {
		return _Impl->m_socket == INVALID_SOCKET;
	}

}
