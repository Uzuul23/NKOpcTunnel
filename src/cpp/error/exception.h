#pragma once;
#include <winnt.h>

namespace NkError
{
	class CException
	{
	public:
		CException() {};
		virtual ~CException() {};
		virtual const wchar_t* error_text(DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)) = 0;
		virtual void report(DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)) = 0;
		virtual HRESULT error() const = 0;
		virtual int base_error() const = 0;
	};
}