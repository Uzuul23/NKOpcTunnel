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

#include "stdafx.h"
#include "sha.h"
#include "error/sslexception.h"

namespace nk_ssl
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