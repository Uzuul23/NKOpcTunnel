#pragma once
#include "stream/interfaces.h"

#ifndef MYDLLEXPORT
#define MYDLLEXPORT
#endif

#if defined NK_USE_SSL
namespace NkSSL
{
	class CNKOpenSSLCtx;
}

namespace NkSocket
{
	class CBind;
	class CSocket;
	class MYDLLEXPORT CStreamSocketSSl : public NkStream::CStreamIODevice
	{
	public:
		CStreamSocketSSl();
		virtual ~CStreamSocketSSl(void);

		void accept(const CSocket& Socket, NkSSL::CNKOpenSSLCtx& ssl_ctx);
		void connect(const CBind& bind, NkSSL::CNKOpenSSLCtx& ssl_ctx);

		//CStreamProvider
		virtual size_t read(void* p, size_t cb, ULONG32 id = 0);
		virtual size_t write(const void* p, size_t cb, ULONG32 id = 0);
		virtual void close(ULONG32 id = 0) throw();
		virtual bool is_closed() const throw();
		virtual bool can_read(ULONG32 id = 0);
		virtual void cancel_wait(ULONG32 id = 0);

	private:
		CStreamSocketSSl& operator =(const CStreamSocketSSl&);
		CStreamSocketSSl(const CStreamSocketSSl&);

		class CImpl;
		CImpl* _Impl;
	};
}
#endif