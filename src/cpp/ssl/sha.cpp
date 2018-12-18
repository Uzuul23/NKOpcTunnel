#include "stdafx.h"
#include "sha.h"
#include "error/sslexception.h"

namespace NkSSL
{
	void CSHA::SHA256(const BYTE in[], size_t cb, BYTE digest[SHA256_DIGEST_LENGTH])
	{
		SHA256_CTX sha256;
		int ret = SHA256_Init(&sha256);
		NkError::CSSLException::check_result(ret, "SHA256_Init", __FILE__, __LINE__);
		ret = SHA256_Update(&sha256, &in[0], cb);
		NkError::CSSLException::check_result(ret, "SHA256_Update", __FILE__, __LINE__);
		ret = SHA256_Final(&digest[0], &sha256);
		NkError::CSSLException::check_result(ret, "SHA256_Final", __FILE__, __LINE__);
	}
}