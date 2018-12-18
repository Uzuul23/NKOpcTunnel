#pragma once
#include "openssl/ssl.h"
#include "error/sslexception.h"

#if defined NK_USE_SSL
namespace NkSSL
{
	class CNKOpenSSL
	{
	public:
		CNKOpenSSL()
		{
		}
		virtual ~CNKOpenSSL(void)
		{
			close();
		}
		void create(SSL_CTX* pCtx)
		{
			m_pSSL = SSL_new(pCtx);
			NkError::CSSLException::check_create(m_pSSL, "SSL_new", __FILE__, __LINE__);
		}
		void close()
		{
			if (m_pSSL) {
				SSL_clear(m_pSSL);
				SSL_free(m_pSSL);
				m_pSSL = 0;
			}
		}
		void set_fd(SOCKET socket)
		{
			int ret = SSL_set_fd(m_pSSL, socket);
			NkError::CSSLException::check_result(ret, "SSL_set_fd", __FILE__, __LINE__);
		}
		int get_error(int err)
		{
			return SSL_get_error(m_pSSL, err);
		}
		int connect()
		{
			int ret = SSL_connect(m_pSSL);
			int error = SSL_get_error(m_pSSL, ret);

			switch (error)
			{
			case SSL_ERROR_NONE:
			case SSL_ERROR_WANT_WRITE:
			case SSL_ERROR_WANT_READ:
				break;
			default:
				NkError::CSSLException::check_result(ret, "SSL_connect", __FILE__, __LINE__);
			}

			return error;
		}
		int accept()
		{
			int ret = SSL_accept(m_pSSL);
			int error = SSL_get_error(m_pSSL, ret);

			switch (error)
			{
			case SSL_ERROR_NONE:
			case SSL_ERROR_WANT_WRITE:
			case SSL_ERROR_WANT_READ:
				break;
			default:
				NkError::CSSLException::check_result(ret, "SSL_accept", __FILE__, __LINE__);
			}

			return error;
		}
		const SSL_CIPHER* get_current_cipher()
		{
			return SSL_get_current_cipher(m_pSSL);
		}
		size_t read(void* p, size_t cb)
		{
			int ret = SSL_read(m_pSSL, p, static_cast<int>(cb));
			if (ret <= 0) {
				int error = SSL_get_error(m_pSSL, ret);
				switch (error)
				{
				case SSL_ERROR_NONE:
				case SSL_ERROR_WANT_WRITE:
				case SSL_ERROR_WANT_READ:
					return 0;
				default:
					throw NkError::CSocketException("SSL_read", ret, __FILE__, __LINE__);
				}
			}
			return static_cast<size_t>(ret);
		}
		size_t write(const void* p, size_t cb)
		{
			int ret = SSL_write(m_pSSL, p, static_cast<int>(cb));
			if (ret <= 0) {
				int error = SSL_get_error(m_pSSL, ret);
				switch (error)
				{
				case SSL_ERROR_NONE:
				case SSL_ERROR_WANT_WRITE:
				case SSL_ERROR_WANT_READ:
					return 0;
				default:
					throw NkError::CSocketException("SSL_write", ret, __FILE__, __LINE__);
				}
			}
			return static_cast<size_t>(ret);
		}

	private:
		SSL* m_pSSL = 0;
	};
}
#endif