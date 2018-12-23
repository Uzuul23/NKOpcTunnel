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
#include "error/exception.h"
#include "types/string.h"
#include <winsock2.h>
#include <crtdbg.h>

namespace NkError {
	class CSocketException : public CException {
	public:
		CSocketException();
		CSocketException(const char* pszFunctionName, int error, const char* lpszfile, int line);
		CSocketException(const CSocketException& d);
		CSocketException& operator =(const CSocketException& d);

		HRESULT error() const;
		int base_error() const override;
		virtual const wchar_t* error_text(DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
		virtual void report(DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));

		static void check(const char* pszFunctionName, const char* lpszfile, int line);
		static void check(int ret, const char* pszFunctionName, const char* lpszfile, int line);
		static void check(const WSANETWORKEVENTS& wsaevents, long NetworkEvent, const char* lpszfile, int line);
		static void check_zero(int ret, const char* pszFunctionName, const char* lpszfile, int line);
		static void check_bool(BOOL ret, const char* pszFunctionName, const char* lpszfile, int line);
		static void check(SOCKET so, const char* pszFunctionName, const char* lpszfile, int line);
		static void check(WSAEVENT evt, const char* pszFunctionName, const char* lpszfile, int line);

	private:
		NkType::CString m_strFunctionName;
		NkType::CString m_strSourceFile;
		NkType::CString m_strErrorString;
		int m_Error;
		int m_nSourceLine;
	};

	inline void CSocketException::check(const char* pszFunctionName, const char* lpszfile, int line) {
		throw CSocketException(pszFunctionName, WSAGetLastError(), lpszfile, line);
	}

	inline void CSocketException::check(int ret, const char* pszFunctionName, const char* lpszfile, int line) {
		if (ret == SOCKET_ERROR) {
			throw CSocketException(pszFunctionName, WSAGetLastError(), lpszfile, line);
		}
	}

	inline void CSocketException::check(SOCKET so, const char* pszFunctionName, const char* lpszfile, int line) {
		if (so == INVALID_SOCKET) {
			throw CSocketException(pszFunctionName, WSAGetLastError(), lpszfile, line);
		}
	}

	inline void CSocketException::check(WSAEVENT evt, const char* pszFunctionName, const char* lpszfile, int line) {
		if (evt == WSA_INVALID_EVENT) {
			throw CSocketException(pszFunctionName, WSAGetLastError(), lpszfile, line);
		}
	}

	inline void CSocketException::check_zero(int ret, const char* pszFunctionName, const char* lpszfile, int line) {
		if (ret == 0) {
			throw CSocketException(pszFunctionName, WSAGetLastError(), lpszfile, line);
		}
	}

	inline void CSocketException::check_bool(BOOL ret, const char* pszFunctionName, const char* lpszfile, int line) {
		if (ret == FALSE) {
			throw CSocketException(pszFunctionName, WSAGetLastError(), lpszfile, line);
		}
	}
}
