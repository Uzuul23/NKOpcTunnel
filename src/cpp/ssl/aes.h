#pragma once
#include "types/types.h"
#include "openssl/aes.h"

namespace nk_ssl
{
	class CAES256
	{
	public:
		static void encrypt(const BYTE plain[], size_t cb_plain, const BYTE key[], size_t cb_key
			, BYTE cipher[], size_t cb_cipher, size_t& cb_cipher_used);
		static void decrypt(const BYTE cipher[], size_t cb_cipher, const BYTE key[], size_t cb_key
			, BYTE plain[], size_t cb_plain, size_t& cb_plain_used);
	};
}
