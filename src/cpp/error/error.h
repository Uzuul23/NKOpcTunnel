#pragma once
#include "types/String.h"

namespace NkError
{
	static LPCWSTR error_to_string(DWORD dwError)
	{
		static NkType::CString strError;

		LPTSTR psz = 0;
		if (::FormatMessageW(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&psz, 0, 0) == 0)
		{
			strError = L"unknown error";
			return strError;
		}
		strError = psz;
		strError.trim();
		::LocalFree(psz);
		return strError;
	}
}