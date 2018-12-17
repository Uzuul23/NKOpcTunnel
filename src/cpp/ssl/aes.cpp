#include "stdafx.h"
#include "aes.h"
#include "error/sslexception.h"
#include "error/baseexception.h"

namespace nk_ssl
{
	void CAES256::encrypt(const BYTE plain[], size_t cb_plain, const BYTE key[]
		, size_t cb_key, BYTE cipher[], size_t cb_cipher, size_t& cb_cipher_used)
	{
		if (cb_cipher < (cb_plain + AES_BLOCK_SIZE)
			|| cb_plain == 0 || cb_key == 0 || cb_cipher == 0) {
			NkError::CBaseException::check(NkError::CBaseException::invalid_argument
				, __FILE__, __LINE__);
		}

		EVP_CIPHER_CTX cipher_ctx = { 0 };
		unsigned char ckey[32], civ[32];

		try {
			int ret = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), 0, key
				, cb_key, 8, ckey, civ);
			NkError::CSSLException::check_result(ret, "EVP_BytesToKey", __FILE__, __LINE__);

			EVP_CIPHER_CTX_init(&cipher_ctx);

			ret = EVP_EncryptInit_ex(&cipher_ctx, EVP_aes_256_cbc(), 0, ckey, civ);
			NkError::CSSLException::check_result(ret, "EVP_EncryptInit_ex", __FILE__, __LINE__);

			int cb_written = 0;
			cb_cipher_used = 0;

			ret = EVP_EncryptUpdate(&cipher_ctx, &cipher[cb_cipher_used], &cb_written, plain, cb_plain);
			NkError::CSSLException::check_result(ret, "EVP_EncryptUpdate", __FILE__, __LINE__);
			cb_cipher_used += cb_written;

			ret = EVP_EncryptFinal_ex(&cipher_ctx, &cipher[cb_cipher_used], &cb_written);
			NkError::CSSLException::check_result(ret, "EVP_EncryptFinal_ex", __FILE__, __LINE__);
			cb_cipher_used += cb_written;
		}
		catch (...) {
			cb_cipher_used = 0;
			EVP_CIPHER_CTX_cleanup(&cipher_ctx);
			SecureZeroMemory(ckey, sizeof(ckey));
			SecureZeroMemory(civ, sizeof(civ));
			throw;
		}

		EVP_CIPHER_CTX_cleanup(&cipher_ctx);
		SecureZeroMemory(ckey, sizeof(ckey));
		SecureZeroMemory(civ, sizeof(civ));
	}

	void CAES256::decrypt(const BYTE cipher[], size_t cb_cipher, const BYTE key[]
		, size_t cb_key, BYTE plain[], size_t cb_plain, size_t& cb_plain_used)
	{
		if (cb_plain < (cb_cipher + AES_BLOCK_SIZE)
			|| cb_plain == 0 || cb_key == 0 || cb_cipher == 0) {
			NkError::CBaseException::check(NkError::CBaseException::invalid_argument
				, __FILE__, __LINE__);
		}

		EVP_CIPHER_CTX cipher_ctx = { 0 };
		unsigned char ckey[32], civ[32];

		try {
			int ret = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), 0, key
				, cb_key, 8, ckey, civ);
			NkError::CSSLException::check_result(ret, "EVP_BytesToKey", __FILE__, __LINE__);

			EVP_CIPHER_CTX_init(&cipher_ctx);

			ret = EVP_DecryptInit_ex(&cipher_ctx, EVP_aes_256_cbc(), 0, ckey, civ);
			NkError::CSSLException::check_result(ret, "EVP_DecryptInit_ex", __FILE__, __LINE__);

			int cb_written = 0;
			cb_plain_used = 0;

			ret = EVP_DecryptUpdate(&cipher_ctx, &plain[cb_plain_used]
				, &cb_written, cipher, cb_cipher);
			NkError::CSSLException::check_result(ret, "EVP_DecryptUpdate", __FILE__, __LINE__);
			cb_plain_used += cb_written;

			ret = EVP_DecryptFinal_ex(&cipher_ctx, &plain[cb_plain_used], &cb_written);
			NkError::CSSLException::check_result(ret, "EVP_DecryptFinal_ex", __FILE__, __LINE__);
			cb_plain_used += cb_written;

			EVP_CIPHER_CTX_cleanup(&cipher_ctx);
			SecureZeroMemory(ckey, sizeof(ckey));
			SecureZeroMemory(civ, sizeof(civ));
		}
		catch (...) {
			cb_plain_used = 0;
			EVP_CIPHER_CTX_cleanup(&cipher_ctx);
			SecureZeroMemory(ckey, sizeof(ckey));
			SecureZeroMemory(civ, sizeof(civ));
			throw;
		}
	}
}