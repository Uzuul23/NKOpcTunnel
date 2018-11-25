#pragma once

#ifndef MYDLLEXPORT
#define MYDLLEXPORT
#endif

#if defined NK_USE_SSL
#include "openssl/ssl.h"
#include "openssl/ossl_typ.h"

namespace NkSSL
{
	class MYDLLEXPORT CNKOpenSSLCtx
	{
	public:
		CNKOpenSSLCtx();
		virtual ~CNKOpenSSLCtx(void);

		static void initialize();
		static void cleanup();

		void create_TLSv1_2_server();
		void create_TLSv1_2_client();
		SSL_CTX* data();

		void certificate_file(const char* psz);
		void load_verify_locations(const char* psz);
		void use_private_key_file(const char* psz);
		void set_default_passwd(const char* psz);
		void set_verify(int mode = SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT);

	private:
		CNKOpenSSLCtx & operator =(const CNKOpenSSLCtx&);
		CNKOpenSSLCtx(const CNKOpenSSLCtx&);
		class CImpl;
		CImpl* _Impl;
	};
}

#endif
