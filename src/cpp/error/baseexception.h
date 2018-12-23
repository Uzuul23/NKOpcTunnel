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
#include "exception.h"
#include "trace/trace.h"
#include "types/string.h"
#include <crtdbg.h>

namespace NkError
{
	class CBaseException : public CException
	{
	public:
		enum BaseErrors
		{
			no_error = 0,
			result_error,
			out_of_memory,
			invalid_pointer,
			invalid_argument,
			buffer_to_small,
			not_connected,
			connection_closed,
			mutex_abandoned_state,
			invalid_type,
			server_down,
			logon_failed
		};
		CBaseException();
		CBaseException(BaseErrors error, const char* lpszfile, int line);
		CBaseException(HRESULT error, const char* lpszfile, int line);
		CBaseException(const CBaseException& d);
		CBaseException &operator =(const CBaseException& d);

		HRESULT error() const;
		int base_error() const;
		virtual void report(DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
		virtual const wchar_t* error_text(DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));

		static void check_alloc(const void* p, const char* lpszfile, int line);
		static void check_pointer(const void* p, const char* lpszfile, int line);
		static void check_result(HRESULT hr, const char* lpszfile, int line);
		static void check(int error, const char* lpszfile, int line);
		static void check(bool ret, HRESULT error, const char* lpszfile, int line);
		static void check(bool ret, BaseErrors error, const char* lpszfile, int line);
		static void check_arg(const void* p, const char* lpszfile, int line);
		static void check_type(bool ret, const char* lpszfile, int line);

	private:
		static const wchar_t* text(int error);
		NkType::CString m_strSourceFile;
		NkType::CString m_strErrorString;
		BaseErrors m_Error;
		int m_nSourceLine;
		HRESULT m_hrError;
	};

	inline void CBaseException::check_alloc(const void* p, const char* lpszfile, int line)
	{
		if (p == NULL) {
			_ASSERT(FALSE);
			throw CBaseException(out_of_memory, lpszfile, line);
		}
	}

	inline void CBaseException::check_pointer(const void* p, const char* lpszfile, int line)
	{
		if (p == 0) {
			_ASSERT(FALSE);
			throw CBaseException(invalid_pointer, lpszfile, line);
		}
	}

	inline void CBaseException::check_result(HRESULT hr, const char* lpszfile, int line)
	{
		if (FAILED(hr)) {
			_ASSERT(FALSE);
			throw CBaseException(hr, lpszfile, line);
		}
	}

	inline void CBaseException::check(int error, const char* lpszfile, int line)
	{
		_ASSERT(FALSE);
		throw CBaseException(error, lpszfile, line);
	}

	inline void CBaseException::check(bool ret, HRESULT error, const char* lpszfile, int line)
	{
		if (ret == false) { 
			_ASSERT(FALSE);
			throw CBaseException(error, lpszfile, line);
		}
	}

	inline void CBaseException::check(bool ret, BaseErrors error, const char* lpszfile, int line)
	{
		if (ret == true) {
			_ASSERT(FALSE);
			throw CBaseException(error, lpszfile, line);
		}
	}

	inline void CBaseException::check_arg(const void* p, const char* lpszfile, int line)
	{
		if (p == 0) {
			_ASSERT(FALSE);
			throw CBaseException(E_INVALIDARG, lpszfile, line);
		}
	}

	inline void CBaseException::check_type(bool ret, const char* lpszfile, int line)
	{
		if (ret == false) {
			_ASSERT(FALSE);
			throw CBaseException(invalid_type, lpszfile, line);
		}
	}
}
