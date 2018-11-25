#pragma once
#include "stream/stream.h"
#include "error/baseexception.h"
#include <wtypes.h>
#include <OleAuto.h>

namespace NkCom
{
	class CBSTR
	{
	public:
		CBSTR() : _bstr(0)
		{
		}
		CBSTR(const wchar_t* psz) : _bstr(::SysAllocString(psz))
		{
		}
		~CBSTR()
		{
			clear();
		}
		operator const BSTR()
		{
			return _bstr;
		}
		BSTR* operator&()
		{
			return &_bstr;
		}
		void attach(BSTR bstr)
		{
			if (_bstr != bstr) {
				clear();
				_bstr = bstr;
			}
		}
		void detach(BSTR* p)
		{
			(*p) = _bstr;
			_bstr = 0;
		}
		void clear()
		{
			if (_bstr) {
				::SysFreeString(_bstr);
				_bstr = 0;
			}
		}
		static void read(NkStream::CStream& stream, const BSTR& bstr)
		{
			stream.write_type(VT_BSTR);
			size_t c = ::SysStringByteLen(bstr);
			stream.write_count(c);
			if (c > 0) {
				stream.write(static_cast<void*>(bstr), c);
			}
		}
		static void write(NkStream::CStream& stream, BSTR& bstr)
		{
			stream.check_type(VT_BSTR);
			size_t c = stream.read_count();
			bstr = ::SysAllocStringByteLen(0, static_cast<UINT>(c));
			NkError::CBaseException::check_alloc(bstr, __FILE__, __LINE__);
			if (c > 0) {
				stream.read(static_cast<void*>(bstr), c);
			}
		}
		void read(NkStream::CStream& stream) const {
			read(stream, _bstr);
		}
		void write(NkStream::CStream& stream) {
			clear();
			write(stream, _bstr);
		}
	private:
		BSTR _bstr;
	};

	inline NkStream::CStream& operator<<(NkStream::CStream& l, const CBSTR& r)
	{
		r.read(l);
		return l;
	}
	inline NkStream::CStream& operator>>(NkStream::CStream& l, CBSTR& r)
	{
		r.write(l);
		return l;
	}
}
