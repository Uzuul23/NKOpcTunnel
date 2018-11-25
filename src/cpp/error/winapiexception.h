#pragma once
#include "error/exception.h"
#include "types/string.h"
#include <crtdbg.h>

#ifndef MYDLLEXPORT
#define MYDLLEXPORT
#endif

namespace NkError
{
	class MYDLLEXPORT CWinApiException : public CException
	{
	public:
		CWinApiException();
		CWinApiException(const char* pszFunctionName, DWORD error, const char* lpszfile, int line);
		CWinApiException(const CWinApiException& d);
		CWinApiException &operator =(const CWinApiException& d);

		virtual const wchar_t* error_text(DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
		virtual void report(DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));

		virtual HRESULT error() const
		{
			return HRESULT_FROM_WIN32(m_Error);
		}
		virtual int base_error() const
		{
			return 0;
		}
		static void check(const char* pszFunctionName, const char* lpszfile, int line);
		static void check(HANDLE handle, const char* pszFunctionName, const char* lpszfile, int line);
		static void check(int ret, const char* pszFunctionName, const char* lpszfile, int line);
		static void check_ptr(void* p, const char* pszFunctionName, const char* lpszfile, int line);
		static void check_error(long ret, const char* pszFunctionName, const char* lpszfile, int line);
		static void check_error(unsigned long ret, const char* pszFunctionName, const char* lpszfile, int line);
		static void check(HRESULT hr, const char* pszFunctionName, const char* lpszfile, int line);
		static void check_bool(BOOL ret, const char* pszFunctionName, const char* lpszfile, int line);

#ifdef WINSVC_

		static void check(SC_HANDLE handle, const char* pszFunctionName, const char* lpszfile, int line);
		static void check(SERVICE_STATUS_HANDLE hStatus, const char* pszFunctionName, const char* lpszfile, int line);

#endif

	private:
		NkType::CString m_strFunctionName;
		NkType::CString m_strSourceFile;
		NkType::CString m_strErrorString;
		DWORD m_Error;
		int m_nSourceLine;
	};

	inline void CWinApiException::check(const char* pszFunctionName, const char* lpszfile, int line)
	{
		_ASSERT(FALSE);
		throw CWinApiException(pszFunctionName, GetLastError(), lpszfile, line);
	}
	inline void CWinApiException::check(HANDLE handle, const char* pszFunctionName, const char* lpszfile, int line)
	{
		if (handle == 0 || handle == INVALID_HANDLE_VALUE) {
			DWORD error = GetLastError();
			_ASSERT(FALSE);
			throw CWinApiException(pszFunctionName, error, lpszfile, line);
		}
	}
	inline void CWinApiException::check(int ret, const char* pszFunctionName, const char* lpszfile, int line)
	{
		if (ret == 0) {
			DWORD error = GetLastError();
			_ASSERT(FALSE);
			throw CWinApiException(pszFunctionName, error, lpszfile, line);
		}
	}
	inline void CWinApiException::check(HRESULT hr, const char* pszFunctionName, const char* lpszfile, int line)
	{
		if (FAILED(hr)) {
			DWORD error = GetLastError();
			_ASSERT(FALSE);
			throw CWinApiException(pszFunctionName, error, lpszfile, line);
		}
	}
	inline void CWinApiException::check_ptr(void* p, const char* pszFunctionName, const char* lpszfile, int line)
	{
		if (p == 0) {
			DWORD error = GetLastError();
			_ASSERT(FALSE);
			throw CWinApiException(pszFunctionName, error, lpszfile, line);
		}
	}
	inline void CWinApiException::check_error(long ret, const char* pszFunctionName, const char* lpszfile, int line)
	{
		if (ret != ERROR_SUCCESS) {
			_ASSERT(FALSE);
			throw CWinApiException(pszFunctionName, ret, lpszfile, line);
		}
	}
	inline void CWinApiException::check_error(unsigned long ret, const char* pszFunctionName, const char* lpszfile, int line)
	{
		if (ret != ERROR_SUCCESS) {
			_ASSERT(FALSE);
			throw CWinApiException(pszFunctionName, ret, lpszfile, line);
		}
	}
	inline void CWinApiException::check_bool(BOOL ret, const char* pszFunctionName, const char* lpszfile, int line)
	{
		if (ret != TRUE) {
			DWORD error = GetLastError();
			_ASSERT(FALSE);
			throw CWinApiException(pszFunctionName, error, lpszfile, line);
		}
	}

#ifdef WINSVC_

	inline void check(SC_HANDLE handle, const char* pszFunctionName, const char* lpszfile, int line)
	{
		if (handle == 0) {
			_ASSERT(FALSE);
			throw CWinApiException(pszFunctionName, HRESULT_FROM_WIN32(GetLastError()), lpszfile, line);
		}
	}
	inline void check(SERVICE_STATUS_HANDLE hStatus, const char* pszFunctionName, const char* lpszfile, int line)
	{
		if (hStatus == 0) {
			_ASSERT(FALSE);
			throw CWinApiException(pszFunctionName, HRESULT_FROM_WIN32(GetLastError()), lpszfile, line);
		}
	}

#endif
}