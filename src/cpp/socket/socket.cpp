#include "stdafx.h"
#include "socket.h"
#include "error/baseexception.h"
#include "error/socketexception.h"
#include "types/String.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32")

namespace NkSocket
{
	class CSocket::CImpl
	{
	public:
		CImpl()
		{
		}
		~CImpl()
		{
		}
		SOCKET m_socket = INVALID_SOCKET;
		static bool m_bInitialized;
	};

	bool CSocket::CImpl::m_bInitialized = false;

	CSocket::CSocket() noexcept : _Impl(new CImpl())
	{
	}

	CSocket::CSocket(SOCKET so) noexcept : _Impl(new CImpl())
	{
		_Impl->m_socket = so;
	}

	CSocket & CSocket::operator=(const SOCKET &d)
	{
		close();
		_Impl->m_socket = d;
		return *this;
	}

	CSocket::~CSocket(void)
	{
		close();
		delete _Impl;
	}

	void CSocket::socket()
	{
		close();
		_Impl->m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		NkError::CSocketException::check(_Impl->m_socket, "socket", __FILE__, __LINE__);
		non_blocking_mode(false);
	}

	void CSocket::shutdown(int how /*= SD_SEND*/)
	{
		int ret = ::shutdown(_Impl->m_socket, how);
		NkError::CSocketException::check(ret, "shutdown", __FILE__, __LINE__);
	}

	void CSocket::non_blocking_mode(bool set)
	{
		if (set)
		{
			u_long iMode = 1;
			int ret = ::ioctlsocket(_Impl->m_socket, FIONBIO, &iMode);
			NkError::CSocketException::check(ret, "ioctlsocket", __FILE__, __LINE__);
		}
		else
		{
			u_long iMode = 0;
			int ret = ::WSAEventSelect(_Impl->m_socket, 0, 0);
			NkError::CSocketException::check(ret, "WSAEventSelect", __FILE__, __LINE__);
			ret = ::ioctlsocket(_Impl->m_socket, FIONBIO, &iMode);
			NkError::CSocketException::check(ret, "ioctlsocket", __FILE__, __LINE__);
		}
	}

	bool CSocket::non_blocking_mode()
	{
		//u_long iMode = 0;
		//int ret = ::ioctlsocket(_Impl->m_socket, FIONBIO, &iMode);
		//NkError::CSocketException::check(ret, "ioctlsocket", __FILE__, __LINE__);
		return false;
	}

	void CSocket::reuse_addr()
	{
		BOOL optval = TRUE;
		int ret = ::setsockopt(_Impl->m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(BOOL));
		NkError::CSocketException::check(ret, "setsockopt", __FILE__, __LINE__);
	}

	void CSocket::exclusive_addr_use()
	{
		BOOL optval = TRUE;
		int ret = ::setsockopt(_Impl->m_socket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optval, sizeof(BOOL));
		NkError::CSocketException::check(ret, "setsockopt", __FILE__, __LINE__);
	}

	void CSocket::bind(const sockaddr_in* psa)
	{
		int ret = ::bind(_Impl->m_socket, (SOCKADDR*)psa, sizeof(sockaddr_in));
		NkError::CSocketException::check(ret, "bind", __FILE__, __LINE__);
	}

	void CSocket::bind(const char* addr, u_short port)
	{
		sockaddr_in service;
		ZeroMemory(&service, sizeof(service));

		service.sin_family = AF_INET;
		service.sin_port = htons(port);
		int ret = inet_pton(AF_INET, addr, &service.sin_addr.s_addr);
		NkError::CSocketException::check_zero(ret, "bind", __FILE__, __LINE__);

		ret = ::bind(_Impl->m_socket, (SOCKADDR*)&service, sizeof(service));
		NkError::CSocketException::check(ret, "bind", __FILE__, __LINE__);
	}

	void CSocket::bind(const sockaddr* paddr, u_short port)
	{
		sockaddr_in service;
		const sockaddr_in *sin = reinterpret_cast<const sockaddr_in*>(paddr);

		memcpy(&service, sin, sizeof(sockaddr_in));
		service.sin_port = htons(port);

		int ret = ::bind(_Impl->m_socket, (SOCKADDR*)&service, sizeof(service));
		NkError::CSocketException::check(ret, "bind", __FILE__, __LINE__);
	}

	void CSocket::bind(LPCSTR addr)
	{
		std::string str(addr);

		size_t found = str.find("/");
		if (found == std::string::npos)
		{
			throw NkError::CBaseException(NkError::CBaseException::invalid_argument, __FILE__, __LINE__);
		}

		std::string str_addr(str.substr(0, found));
		std::string str_port(str.substr(found + 1));

		addrinfo hints;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		addrinfo *result = 0;
		int ret = ::getaddrinfo(str_addr.c_str(), str_port.c_str(), &hints, &result);
		
		NkError::CSocketException::check(ret, "getaddrinfo", __FILE__, __LINE__);

		ret =::bind(_Impl->m_socket, result->ai_addr, (int)result->ai_addrlen);
		::freeaddrinfo(result);
		NkError::CSocketException::check(ret, "bind", __FILE__, __LINE__);	
	}

	int CSocket::listen(int backlog /*= SOMAXCONN*/)
	{
		int ret = ::listen(_Impl->m_socket, backlog);
		NkError::CSocketException::check(ret, "listen", __FILE__, __LINE__);
		return ret;
	}

	SOCKET CSocket::accept() const
	{
		SOCKET so = ::accept(_Impl->m_socket, 0, 0);
		NkError::CSocketException::check(so, "accept", __FILE__, __LINE__);
		return so;
	}

	void CSocket::connect(const sockaddr_in* name, int namelen)
	{
		int ret = ::connect(_Impl->m_socket, (SOCKADDR*)name, namelen);
		NkError::CSocketException::check(ret, "connect", __FILE__, __LINE__);
	}

	void CSocket::connect(const char* addr)
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

	int CSocket::recv(char* buf, int len, int flag /*= 0*/)
	{
		int ret = ::recv(_Impl->m_socket, buf, len, flag);
		if (ret == 0)
		{
			throw NkError::CBaseException(NkError::CBaseException::connection_closed, __FILE__, __LINE__);
		}
		NkError::CSocketException::check(ret, "recv", __FILE__, __LINE__);
		return ret;
	}

	int CSocket::send(const char* buf, int len, int flag /*= 0*/)
	{
		int ret = ::send(_Impl->m_socket, buf, len, flag);
		if (ret == 0)
		{
			throw NkError::CBaseException(NkError::CBaseException::connection_closed, __FILE__, __LINE__);
		}
		NkError::CSocketException::check(ret, "send", __FILE__, __LINE__);
		return ret;
	}

	void CSocket::get_peer_addr(sockaddr_in& addr)
	{
		int cb = sizeof(sockaddr_in);
		int ret = ::getpeername(_Impl->m_socket, (SOCKADDR*)&addr, &cb);
		NkError::CSocketException::check(ret, "getpeername", __FILE__, __LINE__);
	}

	void CSocket::get_peer_addr(std::string& addr)
	{
		sockaddr_in addr_in;
		get_peer_addr(addr_in);

		char buffer[16] = "";
		inet_ntop(AF_INET, &addr_in.sin_addr, buffer, sizeof(buffer));
		addr = buffer;
		addr += "/";
		addr += std::to_string(ntohs(addr_in.sin_port));
	}

	bool CSocket::issocket() const noexcept
	{
		return _Impl->m_socket != INVALID_SOCKET;
	}

	CSocket::operator SOCKET() noexcept
	{
		return	_Impl->m_socket;
	}

	size_t CSocket::read(void* p, size_t cb, ULONG32 id /*= 0*/)
	{
		_ASSERT(p != 0);
		_ASSERT(cb > 0);

		if (p == 0 || cb == 0) {
			return 0;
		}
		return static_cast<size_t>(recv(static_cast<char*>(p), cb));
	}

	size_t CSocket::write(const void* p, size_t cb, ULONG32 id /*= 0*/)
	{
		_ASSERT(p != 0);
		_ASSERT(cb > 0);

		if (p == 0 || cb == 0) {
			return 0;
		}

		size_t cb_written = 0;
		return static_cast<size_t>(send(static_cast<const char*>(p), cb));
	}

	void CSocket::close(ULONG32 id /*= 0*/) noexcept
	{
		if (_Impl->m_socket != INVALID_SOCKET) {
			int ret = ::closesocket(_Impl->m_socket);
			NkError::CSocketException::check(ret, "closesocket", __FILE__, __LINE__);
			_Impl->m_socket = INVALID_SOCKET;
		}
	}

	bool CSocket::is_closed() const noexcept
	{
		return _Impl->m_socket == INVALID_SOCKET;
	}

	void CSocket::initialize(WORD wsa_version /*= MAKEWORD(2,2)*/)
	{
		WSADATA wsa_data = { 0 };
		int ret = WSAStartup(wsa_version, &wsa_data);
		NkError::CSocketException::check(ret, "WSAStartup", __FILE__, __LINE__);
		CImpl::m_bInitialized = true;
	}

	void CSocket::cleanup()
	{
		if (!CImpl::m_bInitialized){
			return;
		}
		int ret = WSACleanup();
		NkError::CSocketException::check(ret, "WSACleanup", __FILE__, __LINE__);
	}
}