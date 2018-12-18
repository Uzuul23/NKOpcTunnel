#pragma once

#ifndef MYDLLEXPORT
#define MYDLLEXPORT
#endif

#include <winsock2.h>

#pragma warning(disable : 4995)
#include <WS2TCPIP.H>
#pragma warning(default : 4995)

#include "stream/interfaces.h"

namespace NkSSL
{
	class CNKOpenSSLCtx;
	class MYDLLEXPORT CSSLSocket : public NkStream::CStreamIODevice
	{
	public:
		CSSLSocket() noexcept;
		CSSLSocket(SOCKET so) noexcept;
		CSSLSocket(const CSSLSocket&) = delete;
		CSSLSocket & operator =(const CSSLSocket&) = delete;
		virtual ~CSSLSocket();

		void socket();
		void shutdown(int how = SD_SEND);

		//"IP/PORT" e.g. "127.0.0.1/1001"
		void connect(const char* addr);

		void ssl_accept(NkSSL::CNKOpenSSLCtx& ssl_ctx);
		void ssl_connect(NkSSL::CNKOpenSSLCtx& ssl_ctx);

		void get_peer_addr(std::string& addr);

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
