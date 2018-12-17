#pragma once

#ifndef MYDLLEXPORT
#define MYDLLEXPORT
#endif

#include <WinSock2.h>
#include "stream/interfaces.h"

namespace nk_ssl {
	class c_open_ssl_ctx;
	class MYDLLEXPORT c_ssl_socket : public NkStream::CStreamIODevice {
	public:
		c_ssl_socket() noexcept;
		c_ssl_socket(SOCKET so) noexcept;
		c_ssl_socket(const c_ssl_socket&) = delete;
		c_ssl_socket(const c_ssl_socket&&) = delete;
		c_ssl_socket& operator =(const c_ssl_socket&) = delete;
		c_ssl_socket& operator =(const c_ssl_socket&&) = delete;

		virtual ~c_ssl_socket();

		void socket();
		void shutdown(int how = SD_SEND);

		//"IP/PORT" e.g. "127.0.0.1/1001"
		void connect(const char* address);

		void ssl_accept(c_open_ssl_ctx& ssl_ctx);
		void ssl_connect(c_open_ssl_ctx& ssl_ctx);

		void get_peer_address(std::string& address);

		//NkStream::CStreamIODevice
		size_t read(void* p, size_t cb, ULONG32 id = 0) override;
		size_t write(const void* p, size_t cb, ULONG32 id = 0) override;
		void close(ULONG32 id = 0) noexcept override;
		bool is_closed() const noexcept override;

	private:
		class CImpl;
		CImpl* _Impl;
	};
}
