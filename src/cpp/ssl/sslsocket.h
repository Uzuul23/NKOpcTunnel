#pragma once

#include <winsock2.h>

#pragma warning(disable : 4995)
#include <WS2TCPIP.H>
#pragma warning(default : 4995)

#include "stream/interfaces.h"

namespace NkSSL
{
	class CNKOpenSSLCtx;
	class CSSLSocket : public NkStream::CStreamIODevice
	{
	public:
		CSSLSocket() noexcept;
		CSSLSocket(SOCKET so, NkSSL::CNKOpenSSLCtx& ssl_ctx) noexcept;
		CSSLSocket(const CSSLSocket&) = delete;
		CSSLSocket & operator =(const CSSLSocket&) = delete;
		virtual ~CSSLSocket();

		//NkStream::CStreamIODevice
		virtual size_t read(void* p, size_t cb, ULONG32 id = 0);
		virtual size_t write(const void* p, size_t cb, ULONG32 id = 0);
		virtual void close(ULONG32 id = 0) noexcept;
		virtual bool is_closed() const noexcept;

	private:
		class CImpl;
		CImpl* _Impl;
	};
}
