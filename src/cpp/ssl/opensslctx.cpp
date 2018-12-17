#include "stdafx.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "error/baseexception.h"
#include "error/sslexception.h"
#include <strsafe.h>
#include "opensslctx.h"

namespace nk_ssl {
	class c_open_ssl_ctx::CImpl {
	public:
		SSL_CTX* m_pSSL_ctx = nullptr;
		byte* m_p_default_passwd_cb_userdata = nullptr;
		size_t m_cb_default_passwd_cb_userdata = 0;

		void clear() {
			if (m_pSSL_ctx) {
				SSL_CTX_free(m_pSSL_ctx);
			}
			clear_pass();
		}

		void clear_pass() {
			if (m_cb_default_passwd_cb_userdata > 0 && m_p_default_passwd_cb_userdata) {
				SecureZeroMemory(m_p_default_passwd_cb_userdata, m_cb_default_passwd_cb_userdata);
			}
			delete[] m_p_default_passwd_cb_userdata;
			m_p_default_passwd_cb_userdata = nullptr;
			m_cb_default_passwd_cb_userdata = 0;
		}

		static bool m_ssl_initialized;
	};

	c_open_ssl_ctx::c_open_ssl_ctx() : _Impl(new CImpl) {
	}

	c_open_ssl_ctx::~c_open_ssl_ctx(void) {
		delete _Impl;
	}

	bool c_open_ssl_ctx::CImpl::m_ssl_initialized = false;

	void c_open_ssl_ctx::initialize() {
		if (!CImpl::m_ssl_initialized) {
			SSL_load_error_strings();
			int ret = SSL_library_init();
			NkError::CSSLException::check_result(ret, "SSL_library_init", __FILE__, __LINE__);
			CImpl::m_ssl_initialized = true;
		}
	}

	void c_open_ssl_ctx::cleanup() {
		if (CImpl::m_ssl_initialized) {
			ERR_free_strings();
			EVP_cleanup();
			CImpl::m_ssl_initialized = false;
		}
	}

	void c_open_ssl_ctx::create_TLSv1_2_server() {
		_Impl->clear();

		const SSL_METHOD* pMeth = TLSv1_2_server_method();
		NkError::CSSLException::check_create(pMeth
		                                     , "TLSv1_2_server_method", __FILE__, __LINE__);

		_Impl->m_pSSL_ctx = SSL_CTX_new(pMeth);
		NkError::CSSLException::check_create(_Impl->m_pSSL_ctx
		                                     , "SSL_CTX_new", __FILE__, __LINE__);
	}

	void c_open_ssl_ctx::create_TLSv1_2_client() {
		_Impl->clear();

		const SSL_METHOD* pMeth = TLSv1_2_client_method();
		NkError::CSSLException::check_create(pMeth
		                                     , "TLSv1_2_client_method", __FILE__, __LINE__);

		_Impl->m_pSSL_ctx = SSL_CTX_new(pMeth);
		NkError::CSSLException::check_create(_Impl->m_pSSL_ctx
		                                     , "SSL_CTX_new", __FILE__, __LINE__);
	}

	void c_open_ssl_ctx::certificate_file(const char* psz) {
		NkError::CBaseException::check_pointer(_Impl->m_pSSL_ctx, __FILE__, __LINE__);

		int ret = SSL_CTX_use_certificate_file(_Impl->m_pSSL_ctx, psz, SSL_FILETYPE_PEM);
		NkError::CSSLException::check_result(ret, "SSL_CTX_use_certificate_file", __FILE__, __LINE__);
	}

	void c_open_ssl_ctx::load_verify_locations(const char* psz_file, const char* psz_path /*= 0*/) {
		NkError::CBaseException::check_pointer(_Impl->m_pSSL_ctx, __FILE__, __LINE__);

		int ret = SSL_CTX_load_verify_locations(_Impl->m_pSSL_ctx, psz_file, psz_path);
		NkError::CSSLException::check_result(ret, "SSL_CTX_load_verify_locations", __FILE__, __LINE__);
	}

	void c_open_ssl_ctx::use_private_key_file(const char* psz) {
		NkError::CBaseException::check_pointer(_Impl->m_pSSL_ctx, __FILE__, __LINE__);

		int ret = SSL_CTX_use_PrivateKey_file(_Impl->m_pSSL_ctx, psz, SSL_FILETYPE_PEM);
		NkError::CSSLException::check_result(ret, "SSL_CTX_use_PrivateKey_file", __FILE__, __LINE__);
	}

	void c_open_ssl_ctx::set_default_password(const char* psz) {
		_Impl->clear_pass();
		NkError::CBaseException::check_pointer(_Impl->m_pSSL_ctx, __FILE__, __LINE__);

		if (psz) {
			size_t cb_lenght = 0;
			HRESULT hr = StringCbLengthA(psz, STRSAFE_MAX_CCH * sizeof(char), &cb_lenght);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);

			cb_lenght += sizeof(char);
			_Impl->m_p_default_passwd_cb_userdata = new byte[cb_lenght];
			memcpy(_Impl->m_p_default_passwd_cb_userdata, psz, cb_lenght);
			_Impl->m_cb_default_passwd_cb_userdata = cb_lenght;

			SSL_CTX_set_default_passwd_cb_userdata(_Impl->m_pSSL_ctx, _Impl->m_p_default_passwd_cb_userdata);
		}
		else {
			SSL_CTX_set_default_passwd_cb_userdata(_Impl->m_pSSL_ctx, nullptr);
		}
	}

	void c_open_ssl_ctx::set_verify(int mode /*= SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT*/) {
		NkError::CBaseException::check_pointer(_Impl->m_pSSL_ctx, __FILE__, __LINE__);
		SSL_CTX_set_verify(_Impl->m_pSSL_ctx, mode, nullptr);
	}

	SSL_CTX* c_open_ssl_ctx::data() {
		return _Impl->m_pSSL_ctx;
	}
}
