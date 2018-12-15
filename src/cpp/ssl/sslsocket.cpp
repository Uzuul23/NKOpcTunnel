#include "stdafx.h"
#include "ssl/sslsocket.h"
#include "ssl/NKOpenSSLCtx.h"

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
		static bool m_bInitialized;
	};

	CSSLSocket::CSSLSocket() noexcept : _Impl(new CImpl())
	{
	}

	CSSLSocket::CSSLSocket(SOCKET so, NkSSL::CNKOpenSSLCtx& ssl_ctx) noexcept : _Impl(new CImpl())
	{

	}

	CSSLSocket::~CSSLSocket()
	{
		delete _Impl;
	}

	size_t CSSLSocket::read(void* p, size_t cb, ULONG32 id /*= 0*/)
	{
		return 0;
	}

	size_t CSSLSocket::write(const void* p, size_t cb, ULONG32 id /*= 0*/)
	{
		return 0;
	}

	void CSSLSocket::close(ULONG32 id /*= 0*/) noexcept
	{

	}

	bool CSSLSocket::is_closed() const noexcept
	{
		return true;
	}

}

