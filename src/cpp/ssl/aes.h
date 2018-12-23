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
