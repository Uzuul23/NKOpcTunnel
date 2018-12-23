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

#include "openssl/ssl.h"
#include "openssl/ossl_typ.h"

namespace NkSSL
{
	class MYDLLEXPORT COpenSSLCtx
	{
	public:
		COpenSSLCtx();
		COpenSSLCtx(const COpenSSLCtx&) = delete; 
		COpenSSLCtx & operator =(const COpenSSLCtx&) = delete;
		virtual ~COpenSSLCtx(void);

		static void initialize();
		static void cleanup();

		void create_TLSv1_2_server();
		void create_TLSv1_2_client();
		SSL_CTX* data();

		void certificate_file(const char* psz);
		void load_verify_locations(const char* psz_file, const char* psz_path = 0);
		void use_private_key_file(const char* psz);
		void set_default_passwd(const char* psz);
		void set_verify(int mode = SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT);

	private:		
		class CImpl;
		CImpl* _Impl;
	};
}

