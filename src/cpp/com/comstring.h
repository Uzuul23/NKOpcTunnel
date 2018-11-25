#pragma once
#include <wtypes.h>

namespace NkStream
{
	class CStream;
}

namespace NkCom
{
	//memory allocation with CoTaskMemAlloc; freeing with CoTaskMemFree

	class CComString
	{
	public:
		CComString();
		CComString(const wchar_t* p);
		~CComString();

		void attach(LPOLESTR p);
		void detach(LPOLESTR* pp);

		void clear();

		LPOLESTR data();
		LPOLESTR* data_ptr();
		operator LPCOLESTR() const;
		operator LPOLESTR() const;
		LPOLESTR* operator&();

		static LPOLESTR copy(LPCOLESTR src);

		void read(NkStream::CStream& stream) const;
		void write(NkStream::CStream& stream);

		static void read(NkStream::CStream& stream, const wchar_t* p);
		static void write(NkStream::CStream& stream, wchar_t** pp);

		LPOLESTR str;
	};

	inline LPOLESTR CComString::data()
	{
		return str;
	}

	inline LPOLESTR* CComString::data_ptr()
	{
		return &str;
	}

	inline CComString::operator LPCOLESTR() const
	{
		return str;
	}

	inline CComString::operator LPOLESTR() const
	{
		return str;
	}

	inline NkStream::CStream& operator<<(NkStream::CStream& l, const CComString& r){
		r.read(l);
		return l;
	}

	inline NkStream::CStream& operator>>(NkStream::CStream& l, CComString& r){
		r.write(l);
		return l;
	}

}