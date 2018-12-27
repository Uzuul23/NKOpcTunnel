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

#include "ssl/sslsocket.h"

#include "ssl/opensslctx.h"
#include "error/baseexception.h"
#include "error/socketexception.h"
#include "error/sslexception.h"

#pragma warning(disable : 4995)
#include <WS2tcpip.h>
#pragma warning(default : 4995)

namespace NkSSL {

	CSocket::CSocket(SOCKET so) noexcept
	{
		m_socket = so;
	}

	CSocket::~CSocket() noexcept
	{
		close();
	}

	void CSocket::socket()
	{
		close();
		m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		NkError::CSocketException::check(m_socket, "socket", __FILE__, __LINE__);
	}

	void CSocket::shutdown(int how /*= SD_SEND*/) const
	{
		const int ret = ::shutdown(m_socket, how);
		NkError::CSocketException::check(ret, "shutdown", __FILE__, __LINE__);
	}

	void CSocket::connect(const char* address) const
	{
		std::string str(address);
		const auto found = str.find('/');
		if (found == std::string::npos) {
			throw NkError::CBaseException(NkError::CBaseException::invalid_argument, __FILE__, __LINE__);
		}

		sockaddr_in in = {0};
		in.sin_family = AF_INET;
		inet_pton(AF_INET, str.substr(0, found).c_str(), &in.sin_addr);
		in.sin_port = htons(static_cast<USHORT>(std::stoi(str.substr(found + 1))));

		const auto ret = ::connect(m_socket, reinterpret_cast<sockaddr*>(&in), sizeof(in));
		NkError::CSocketException::check(ret, "connect", __FILE__, __LINE__);
	}

	void CSocket::ssl_accept(COpenSSLCtx& ssl_ctx)
	{
		m_pSSL = SSL_new(ssl_ctx.data());
		NkError::CSSLException::check_create(m_pSSL, "SSL_new", __FILE__, __LINE__);

		int ret = SSL_set_fd(m_pSSL, m_socket);
		NkError::CSSLException::check_result(ret, "SSL_set_fd", __FILE__, __LINE__);

		ret = SSL_accept(m_pSSL);
		int error = SSL_get_error(m_pSSL, ret);

		NkError::CSSLException::check_result(ret, "SSL_accept", __FILE__, __LINE__);
	}

	void CSocket::ssl_connect(COpenSSLCtx& ssl_ctx)
	{
		m_pSSL = SSL_new(ssl_ctx.data());
		NkError::CSSLException::check_create(m_pSSL, "SSL_new", __FILE__, __LINE__);

		int ret = SSL_set_fd(m_pSSL, m_socket);
		NkError::CSSLException::check_result(ret, "SSL_set_fd", __FILE__, __LINE__);

		ret = SSL_connect(m_pSSL);
		int error = SSL_get_error(m_pSSL, ret);

		NkError::CSSLException::check_result(ret, "SSL_connect", __FILE__, __LINE__);
	}

	void CSocket::get_peer_address(std::string& address) const
	{
		sockaddr_in address_in = { 0 };
		int cb = sizeof(sockaddr_in);
		const auto ret = getpeername(m_socket, reinterpret_cast<SOCKADDR*>(&address_in), &cb);
		NkError::CSocketException::check(ret, "getpeername", __FILE__, __LINE__);

		char buffer[16] = "";
		inet_ntop(AF_INET, &address_in.sin_addr, buffer, sizeof(buffer));
		address = buffer;
		address += "/";
		address += std::to_string(ntohs(address_in.sin_port));
	}

	size_t CSocket::read(void* p, size_t cb, ULONG32 id /*= 0*/)
	{
		_ASSERT(p != nullptr);
		_ASSERT(cb > 0);

		if (p == nullptr || cb == 0) {
			return 0;
		}

		int ret = SSL_read(m_pSSL, p, static_cast<int>(cb));

		if (ret == 0 || (ret < 0
			&& SSL_get_error(m_pSSL, ret) == SSL_ERROR_SYSCALL)) {
			throw NkError::CBaseException(NkError::CBaseException::connection_closed, __FILE__, __LINE__);
		}

		NkError::CSSLException::check_result(ret, "SSL_read", __FILE__, __LINE__);
		return static_cast<size_t>(ret);
	}

	size_t CSocket::write(const void* p, size_t cb, ULONG32 id /*= 0*/)
	{
		_ASSERT(p != nullptr);
		_ASSERT(cb > 0);

		if (p == nullptr || cb == 0) {
			return 0;
		}

		int ret = SSL_write(m_pSSL, p, static_cast<int>(cb));

		if (ret == 0 || (ret < 0
			&& SSL_get_error(m_pSSL, ret) == SSL_ERROR_SYSCALL)) {
			throw NkError::CBaseException(NkError::CBaseException::connection_closed, __FILE__, __LINE__);
		}

		NkError::CSSLException::check_result(ret, "SSL_write", __FILE__, __LINE__);

		return static_cast<size_t>(ret);
	}

	void CSocket::close(ULONG32 id /*= 0*/) noexcept
	{
		if (m_socket != INVALID_SOCKET) {
			int ret = closesocket(m_socket);
			NkError::CSocketException::check(ret, "closesocket", __FILE__, __LINE__);
			m_socket = INVALID_SOCKET;
		}
	}

	bool CSocket::is_closed() const noexcept
	{
		return m_socket == INVALID_SOCKET;
	}

}
