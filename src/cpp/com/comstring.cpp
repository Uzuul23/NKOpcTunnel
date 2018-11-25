#include "stdafx.h"
#include "comstring.h"
#include "error/baseexception.h"
#include "stream/stream.h"
#include <objbase.h>
#include <strsafe.h>

namespace NkCom
{
	CComString::CComString() : str(0)
	{
	}

	CComString::CComString(const wchar_t* p) : str(copy(p))
	{
	}

	CComString::~CComString()
	{
		clear();
	}

	void CComString::clear()
	{
		if (str) {
			::CoTaskMemFree(str);
			str = 0;
		}
	}

	void CComString::read(NkStream::CStream& stream) const
	{
		read(stream, str);
	}

	void CComString::write(NkStream::CStream& stream)
	{
		write(stream, &str);
	}

	void CComString::read(NkStream::CStream& stream, const wchar_t* p)
	{
		stream.write_type(VT_LPWSTR);
		if (p == 0) {
			stream.write_count(0);
		}
		else {
			size_t c = 0;
			HRESULT hr = StringCbLengthW(p, (STRSAFE_MAX_CCH - 1) * sizeof(TCHAR), &c);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
			c += sizeof(TCHAR);
			stream.write_count(c);
			stream.write(static_cast<const void*>(p), c);
		}
	}

	void CComString::write(NkStream::CStream& stream, wchar_t** pp)
	{
		stream.check_type(VT_LPWSTR);
		size_t c = stream.read_count();
		if (c > 0) {
			wchar_t* p = static_cast<wchar_t*>(CoTaskMemAlloc(c));
			NkError::CBaseException::check_alloc(p, __FILE__, __LINE__);
			stream.read(static_cast<void*>(p), c);
			(*pp) = p;
		}
		else {
			(*pp) = 0;
		}
	}

	LPOLESTR CComString::copy(LPCOLESTR src)
	{
		if (src == 0) {
			return 0;
		}
		size_t c = 0;
		NkError::CBaseException::check_result(::StringCbLengthW(src, (STRSAFE_MAX_CCH - 1) * sizeof(TCHAR), &c), __FILE__, __LINE__);
		c += sizeof(TCHAR);
		LPWSTR dst = (LPWSTR)CoTaskMemAlloc(c);
		NkError::CBaseException::check_alloc(dst, __FILE__, __LINE__);
		NkError::CBaseException::check_result(StringCbCopyW(dst, c, src), __FILE__, __LINE__);
		return dst;
	}

	void CComString::attach(LPOLESTR p)
	{
		clear();
		str = p;
	}

	void CComString::detach(LPOLESTR* pp)
	{
		NkError::CBaseException::check_arg(pp, __FILE__, __LINE__);

		(*pp) = str;
		str = 0;
	}

	LPOLESTR* CComString::operator&()
	{
		_ASSERT(str == 0);
		clear();
		return &str;
	}
}