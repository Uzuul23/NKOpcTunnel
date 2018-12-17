#pragma once

#ifndef MYDLLEXPORT
#define MYDLLEXPORT
#endif

#include "openssl/ssl.h"
#include "openssl/ossl_typ.h"

namespace nk_ssl {
	class MYDLLEXPORT c_open_ssl_ctx {
	public:
		c_open_ssl_ctx();
		c_open_ssl_ctx(const c_open_ssl_ctx&) = delete;
		c_open_ssl_ctx(const c_open_ssl_ctx&&) = delete;
		c_open_ssl_ctx& operator =(const c_open_ssl_ctx&) = delete;
		c_open_ssl_ctx& operator =(const c_open_ssl_ctx&&) = delete;
		virtual ~c_open_ssl_ctx();

		static void initialize();
		static void cleanup();

		void create_TLSv1_2_server();
		void create_TLSv1_2_client();
		SSL_CTX* data();

		void certificate_file(const char* psz);
		void load_verify_locations(const char* psz_file, const char* psz_path = nullptr);
		void use_private_key_file(const char* psz);
		void set_default_password(const char* psz);
		void set_verify(int mode = SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT);

	private:
		class CImpl;
		CImpl* _Impl;
	};
}
