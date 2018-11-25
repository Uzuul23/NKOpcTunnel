#pragma once
#include "error/exception.h"
#include "types/string.h"
#include <winsock2.h>
#include <crtdbg.h>

namespace NkError
{
	class CSocketException : public CException
	{
	public:
		CSocketException();
		CSocketException(const char* pszFunctionName, int error, const char* lpszfile, int line);
		CSocketException(const CSocketException& d);
		CSocketException &operator =(const CSocketException& d);

		HRESULT error() const;
		virtual int base_error() const;
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

	inline void CSocketException::check(const char* pszFunctionName, const char* lpszfile, int line)
	{
		throw CSocketException(pszFunctionName, WSAGetLastError(), lpszfile, line);
	}

	inline void CSocketException::check(int ret, const char* pszFunctionName, const char* lpszfile, int line)
	{
		if (ret == SOCKET_ERROR) {
			throw CSocketException(pszFunctionName, WSAGetLastError(), lpszfile, line);
		}
	}
	inline void CSocketException::check(SOCKET so, const char* pszFunctionName, const char* lpszfile, int line)
	{
		if (so == INVALID_SOCKET) {
			throw CSocketException(pszFunctionName, WSAGetLastError(), lpszfile, line);
		}
	}
	inline void CSocketException::check(WSAEVENT evt, const char* pszFunctionName, const char* lpszfile, int line)
	{
		if (evt == WSA_INVALID_EVENT) {
			throw CSocketException(pszFunctionName, WSAGetLastError(), lpszfile, line);
		}
	}
	inline void CSocketException::check_zero(int ret, const char* pszFunctionName, const char* lpszfile, int line)
	{
		if (ret == 0) {
			throw CSocketException(pszFunctionName, WSAGetLastError(), lpszfile, line);
		}
	}
	inline void CSocketException::check_bool(BOOL ret, const char* pszFunctionName, const char* lpszfile, int line)
	{
		if (ret == FALSE) {
			throw CSocketException(pszFunctionName, WSAGetLastError(), lpszfile, line);
		}
	}
}