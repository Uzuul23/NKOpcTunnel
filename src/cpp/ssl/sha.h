#pragma once
#include "types/types.h"
#include "openssl/sha.h"

namespace nk_ssl
{
	class CSHA
	{
	public:
		static void SHA256(const BYTE in[], size_t cb, BYTE digest[SHA256_DIGEST_LENGTH]);

	};	
}
