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

