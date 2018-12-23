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
#include "String.h"
#include "Strsafe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "error/winapiexception.h"
#include "stream/stream.h"

namespace NkType
{
	CString::CString()
		: m_psz(0)
		, m_cch_allocated_with_null(0)
		, m_cch_lenght_with_null(0)
	{
	}
	CString::~CString()
	{
		delete[] m_psz;
	}
	CString::CString(const wchar_t* psz)
		: m_psz(0)
		, m_cch_allocated_with_null(0)
		, m_cch_lenght_with_null(0)
	{
		*this = psz;
	}

	CString::CString(const CString& d)
		: m_psz(0)
		, m_cch_allocated_with_null(0)
		, m_cch_lenght_with_null(0)
	{
		*this = d;
	}

	CString::CString(const char* psz)
		: m_psz(0)
		, m_cch_allocated_with_null(0)
		, m_cch_lenght_with_null(0)
	{
		*this = psz;
	}
	CString & CString::operator=(const CString& d)
	{
		if (d.lenght() < 0)
		{
			m_cch_lenght_with_null = 0;
			return *this;
		}

		buffer(d.m_cch_lenght_with_null);

		HRESULT hr = StringCchCopyW(m_psz, m_cch_lenght_with_null, d.data());
		NkError::CWinApiException::check(hr, "StringCchCopyW", __FILE__, __LINE__);

		return *this;
	}
	CString & CString::operator=(const wchar_t* psz)
	{
		size_t cch_lenght = 0;
		HRESULT hr = StringCchLengthW(psz, STRSAFE_MAX_CCH, &cch_lenght);
		NkError::CWinApiException::check(hr, "StringCchLengthW", __FILE__, __LINE__);

		buffer(cch_lenght);

		hr = StringCchCopyW(m_psz, cch_lenght + 1, psz);
		NkError::CWinApiException::check(hr, "StringCchCopyW", __FILE__, __LINE__);

		return *this;
	}
	CString & CString::operator=(const char* psz)
	{
		size_t cch_lenght = 0;
		HRESULT hr = StringCchLengthA(psz, STRSAFE_MAX_CCH, &cch_lenght);
		NkError::CWinApiException::check(hr, "StringCchLengthA", __FILE__, __LINE__);

		buffer(cch_lenght);

		int ret = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, psz, cch_lenght, m_psz, cch_lenght);
		NkError::CWinApiException::check(ret, "MultiByteToWideChar", __FILE__, __LINE__);

		return *this;
	}
	CString & CString::operator+=(const CString& d)
	{
		size_t cch_lenght = m_cch_lenght_with_null + d.m_cch_lenght_with_null - 2;
		buffer(cch_lenght);

		HRESULT hr = StringCchCatW(m_psz, m_cch_lenght_with_null, d.data());
		NkError::CWinApiException::check(hr, "StringCchCopyW", __FILE__, __LINE__);

		return *this;
	}

	CString & CString::operator+=(const wchar_t* psz)
	{
		size_t cch_lenght = 0;
		HRESULT hr = StringCchLengthW(psz, STRSAFE_MAX_CCH, &cch_lenght);
		NkError::CWinApiException::check(hr, "StringCchLengthW", __FILE__, __LINE__);

		cch_lenght += m_cch_lenght_with_null - 1;
		buffer(cch_lenght);

		hr = StringCchCatW(m_psz, m_cch_lenght_with_null, psz);
		NkError::CWinApiException::check(hr, "StringCchCopyW", __FILE__, __LINE__);

		return *this;
	}

	CString & CString::operator+=(const char* psz)
	{
		size_t cch_lenght = 0;
		HRESULT hr = StringCchLengthA(psz, STRSAFE_MAX_CCH, &cch_lenght);
		NkError::CWinApiException::check(hr, "StringCchLengthA", __FILE__, __LINE__);

		return *this;
	}
	CString::operator wchar_t*()
	{
		if (m_cch_lenght_with_null == 0)
		{
			return 0;
		}
		return m_psz;
	}

	int CString::wscanf_s(const wchar_t* pszFormat, ...)
	{
		va_list args;
		va_start(args, pszFormat);
		return vswscanf_s(m_psz, pszFormat, args);
	}

	int CString::printf(const wchar_t* pszFormat, ...)
	{
		wchar_t szBuffer[1024] = { 0 };
		va_list args;
		va_start(args, pszFormat);
		int ret = _vsnwprintf_s(szBuffer, 1024, _TRUNCATE, pszFormat, args);
		*this = szBuffer;
		return ret;
	}

	bool CString::operator==(const wchar_t* psz)
	{
		return wcscmp(data(), psz) == 0 ? true: false;
	}

	bool CString::operator!=(const wchar_t* psz)
	{
		return !operator==(psz);
	}

	CString::operator const wchar_t*() const
	{
		return data();
	}

	const wchar_t* CString::data() const
	{
		if (m_cch_lenght_with_null == 0)
		{
			return 0;
		}
		return m_psz;
	}

	wchar_t* CString::buffer(size_t cch)
	{
		size_t cch_new_lenght_with_null = cch + 1;

		if (m_psz)
		{
			if (cch_new_lenght_with_null <= m_cch_allocated_with_null)
			{
				m_cch_lenght_with_null = cch_new_lenght_with_null;
				m_psz[m_cch_lenght_with_null - 1] = 0;
				return m_psz;
			}
		}

		wchar_t *psz = m_psz;
		m_psz = new wchar_t[cch_new_lenght_with_null];

		if (psz != 0 && cch_new_lenght_with_null <= m_cch_lenght_with_null)
		{
			HRESULT hr = StringCchCopyW(m_psz, cch_new_lenght_with_null, psz);
			NkError::CWinApiException::check(hr, "StringCchCopyW", __FILE__, __LINE__);
		}
		else
		{
			m_psz[cch_new_lenght_with_null - 1] = 0;
		}

		m_cch_lenght_with_null = cch_new_lenght_with_null;
		m_cch_allocated_with_null = cch_new_lenght_with_null;

		delete[] psz;
		return m_psz;
	}

	void CString::release_buffer()
	{
		size_t cch_lenght = 0;
		HRESULT hr = StringCchLengthW(m_psz, STRSAFE_MAX_CCH, &cch_lenght);
		NkError::CWinApiException::check(hr, "StringCchLengthW", __FILE__, __LINE__);

		m_cch_lenght_with_null = cch_lenght;
	}

	void CString::clear()
	{
		delete[] m_psz;
		m_psz = 0;
		m_cch_allocated_with_null = 0;
		m_cch_lenght_with_null = 0;
	}

	long CString::lenght() const
	{
		return m_cch_lenght_with_null - 1;
	}

	size_t CString::byte_lenght() const
	{
		return m_cch_lenght_with_null * sizeof(wchar_t);
	}

	void CString::format_system_message(DWORD dwMessageId, DWORD dwLanguageId /*= MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/)
	{
		LPTSTR psz = 0;
		if (::FormatMessageW(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
			, 0, dwMessageId, dwLanguageId, (LPTSTR)&psz, 0, 0) == 0) {
			*this = L"unknown error";
			return;
		}
		*this = psz;
		trim();
		::LocalFree(psz);
	}

	void CString::quote()
	{
		CString str(*this);
		*this = L"\"";
		*this += str;
		*this += L"\"";
	}

	void CString::trim_right()
	{
		if (m_psz && m_cch_lenght_with_null > 1) {
			for (size_t loop = m_cch_lenght_with_null - 2; loop >= 0; --loop) {
				if (iswspace(m_psz[loop])) {
					m_psz[loop] = 0;
				}
				else {
					break;
				}
			}
		}
	}

	void CString::trim_left()
	{
		//TODO: implement
	}

	void CString::trim()
	{
		trim_left();
		trim_right();
	}

	long CString::find(const wchar_t* pszWhat, int iStartPos /*= 0*/)
	{
		size_t cch_lenght = 0;
		HRESULT hr = StringCchLengthW(pszWhat, STRSAFE_MAX_CCH, &cch_lenght);
		NkError::CWinApiException::check(hr, "StringCchLengthW", __FILE__, __LINE__);

		if (iStartPos < 0 || iStartPos > lenght() || static_cast<long>(cch_lenght) > lenght()) {
			return -1;
		}

		LPCWSTR psz = wcsstr(data() + iStartPos, pszWhat);
		if (psz == 0) {
			return -1;
		}

		return static_cast<long>(data()-psz);
	}



	CString CString::right(long first)
	{
		if (first > lenght()) {
			return L"";
		}
		return data() + first;
	}

	CString CString::left(long last)
	{
		if (last > lenght()) {
			return L"";
		}
		CString str(data());
		str.buffer(last);
		return str;
	}

	CString CString::mid(long first, long count)
	{
		if ((first + count) > lenght()) {
			return L"";
		}
		CString str(data() + first);
		str.buffer(count);
		return str;
	}

	CString CString::file_name()
	{
		if (m_psz && m_cch_lenght_with_null > 1) {
			for (size_t loop = m_cch_lenght_with_null - 2; loop > 0; --loop){
				if (m_psz[loop] == L'\\'){
					return CString(&m_psz[loop + 1]);
				}
			}
		}

		return *this;
	}

	void CString::file_name(const char* psz)
	{
		size_t cch_lenght = 0;
		HRESULT hr = StringCchLengthA(psz, STRSAFE_MAX_CCH, &cch_lenght);
		NkError::CWinApiException::check(hr, "StringCchLengthA", __FILE__, __LINE__);

		for (size_t loop = cch_lenght; loop>0; --loop) {
			if (psz[loop] == '\\') {
				*this = &psz[loop+1];
				return;

			}
		}
		*this = psz;
	}

	void CString::read(NkStream::CStream& stream) const
	{
		stream.write_type(VT_LPWSTR);
		stream.write_count(lenght());
		if (lenght() > 0)
		{
			stream.write(static_cast<const void*>(data()), lenght()*sizeof(wchar_t));
		}
	}

	void CString::write(NkStream::CStream& stream)
	{
		stream.check_type(VT_LPWSTR);
		size_t cch_read = stream.read_count();
		if (cch_read == 0)
		{
			clear();
			return;
		}
		stream.read(static_cast<void*>(buffer(cch_read - 1)), cch_read*sizeof(wchar_t));
	}

}