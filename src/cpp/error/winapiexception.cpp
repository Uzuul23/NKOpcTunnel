#include "stdafx.h"
#include "winapiexception.h"
#include "trace/trace.h"


namespace NkError
{
	CWinApiException::CWinApiException() : m_Error(ERROR_SUCCESS), m_nSourceLine(-1)
	{
	}

	CWinApiException::CWinApiException(const char* pszFunctionName
		, DWORD error, const char* lpszfile, int line)
		: m_strFunctionName(pszFunctionName)
		, m_Error(error)
		, m_nSourceLine(line)
	{
		m_strSourceFile.file_name(lpszfile);
	}

	CWinApiException::CWinApiException(const CWinApiException& d)
	{
		*this = d;
	}

	CWinApiException & CWinApiException::operator=(const CWinApiException& d)
	{
		m_strFunctionName = d.m_strFunctionName;
		m_strSourceFile = d.m_strSourceFile;
		m_strErrorString = d.m_strErrorString;
		m_Error = d.m_Error;
		m_nSourceLine = d.m_nSourceLine;
		return *this;
	}

	const wchar_t* CWinApiException::error_text(DWORD dwLanguageId /*= MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/)
	{
		m_strErrorString.format_system_message(m_Error, dwLanguageId);
		return m_strErrorString;
	}

	void CWinApiException::report(DWORD dwLanguageId /*= MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/)
	{
		NkTrace::CTrace::TraceErr(L"call: %s error:%s(%d) -> file: %s line: %d"
			, m_strFunctionName.data(), error_text(dwLanguageId), m_Error
			, m_strSourceFile.file_name().data(), m_nSourceLine);
	}
}