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

#pragma once

#ifndef MYDLLEXPORT
#define MYDLLEXPORT
#endif

#include "stream/interfaces.h"

#include <openssl/bio.h>
#include <WinSock2.h>

namespace NkSSL {
	class COpenSSLCtx;

	class MYDLLEXPORT CSocket : public NkStream::CStreamIODevice
	{
	public:
		CSocket() = default;
		CSocket(SOCKET so) noexcept;
		CSocket(const CSocket&) = delete;
		CSocket(const CSocket&&) = delete;
		CSocket& operator =(const CSocket&) = delete;
		CSocket& operator =(const CSocket&&) = delete;
		virtual ~CSocket() noexcept;

		void socket();
		void shutdown(int how = SD_SEND) const;

		//"IP/PORT" e.g. "127.0.0.1/1001"
		void connect(const char* addr) const;

		void ssl_accept(COpenSSLCtx& ssl_ctx);
		void ssl_connect(COpenSSLCtx& ssl_ctx);

		void get_peer_address(std::string& address) const;

		//NkStream::CStreamIODevice
		size_t read(void* p, size_t cb, ULONG32 id = 0) override;
		size_t write(const void* p, size_t cb, ULONG32 id = 0) override;
		void close(ULONG32 id = 0) noexcept override;
		bool is_closed() const noexcept override;

	private:
		SOCKET m_socket = INVALID_SOCKET;
		SSL* m_pSSL = nullptr;
	};
}
