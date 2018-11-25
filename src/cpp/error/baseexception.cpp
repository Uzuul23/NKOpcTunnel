#include "stdafx.h"
#include "baseexception.h"

namespace NkError
{
	CBaseException::CBaseException() : m_Error(no_error), m_nSourceLine(-1), m_hrError(S_OK)
	{
	}

	CBaseException::CBaseException(BaseErrors error, const char* lpszfile, int line) : m_Error(error)
		, m_nSourceLine(line)
		, m_hrError(S_OK)
	{
		m_strSourceFile.file_name(lpszfile);
	}

	CBaseException::CBaseException(HRESULT error, const char* lpszfile, int line) : m_Error(result_error)
		, m_nSourceLine(line)
		, m_hrError(error)
	{
		m_strSourceFile.file_name(lpszfile);
	}

	CBaseException::CBaseException(const CBaseException& d)
	{
		*this = d;
	}

	CBaseException & CBaseException::operator=(const CBaseException& d)
	{
		m_strSourceFile = d.m_strSourceFile;
		m_Error = d.m_Error;
		m_nSourceLine = d.m_nSourceLine;
		return *this;
	}

	const wchar_t* CBaseException::text(int error)
	{
		switch (error)
		{
		case no_error:					return L"OK";
		case out_of_memory:				return L"out of memory";
		case invalid_pointer:			return L"invalid pointer";
		case invalid_argument:			return L"invalid argument";
		case buffer_to_small:			return L"buffer to small";
		case not_connected:				return L"not connected";
		case connection_closed:			return L"connection closed";
		case mutex_abandoned_state:		return L"ownership of an abandoned mutex";
		case invalid_type:				return L"invalid type";
		case server_down:				return L"server is down";
		case logon_failed:				return L"logon failed";
		default: return L"unknown error";
		}
	}

	HRESULT CBaseException::error() const
	{
		if (m_Error == result_error) {
			return m_hrError;
		}
		return E_UNEXPECTED;
	}

	int CBaseException::base_error() const
	{
		return m_Error;
	}

	void CBaseException::report(DWORD dwLanguageId /*= MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/)
	{
		NkTrace::CTrace::TraceErr(L"(%d) %s -> file: %s line: %d"
			, m_Error, error_text(m_Error), m_strSourceFile.data(), m_nSourceLine);
	}

	const wchar_t* CBaseException::error_text(DWORD dwLanguageId /*= MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/)
	{
		if (m_Error == result_error) {
			m_strErrorString.format_system_message(m_hrError);
			return m_strErrorString;
		}
		return text(m_Error);
	}
}